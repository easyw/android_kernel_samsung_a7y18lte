/*
 * Copyright (C) 2012 Red Hat, Inc.
 *
 * Author: Mikulas Patocka <mpatocka@redhat.com>
 *
 * Based on Chromium dm-verity driver (C) 2011 The Chromium OS Authors
 *
 * This file is released under the GPLv2.
 *
 * In the file "/sys/module/dm_verity/parameters/prefetch_cluster" you can set
 * default prefetch value. Data are read in "prefetch_cluster" chunks from the
 * hash device. Setting this greatly improves performance when data and hash
 * are on the same disk on different partitions on devices with poor random
 * access behavior.
 */

#include "dm-verity.h"
#include "dm-verity-fec.h"
#include "dm-verity-debug.h"

#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/vmalloc.h>

#include <linux/ctype.h>
#if defined(CONFIG_TZ_ICCC)
#include <linux/smc.h>
#define SMC_CMD_DMV_WRITE_STATUS (0x83000014)
#endif

#define DM_MSG_PREFIX			"verity"

#define DM_VERITY_ENV_LENGTH		42
#define DM_VERITY_ENV_VAR_NAME		"DM_VERITY_ERR_BLOCK_NR"

#define DM_VERITY_DEFAULT_PREFETCH_SIZE	262144

#define DM_VERITY_MAX_CORRUPTED_ERRS	100

#define DM_VERITY_OPT_LOGGING		"ignore_corruption"
#define DM_VERITY_OPT_RESTART		"restart_on_corruption"
#define DM_VERITY_OPT_IGN_ZEROES	"ignore_zero_blocks"
#define DM_VERITY_OPT_AT_MOST_ONCE	"check_at_most_once"

#define DM_VERITY_OPTS_MAX		(2 + DM_VERITY_OPTS_FEC)

#ifdef VERIFY_META_ONLY
extern struct rb_root *ext4_system_zone_root(struct super_block *sb);

struct rb_root *system_blks;

struct ext4_system_zone {
    struct rb_node  node;
    unsigned long long  start_blk;
    unsigned int    count;
};
int start_meta;
#endif

static unsigned dm_verity_prefetch_cluster = DM_VERITY_DEFAULT_PREFETCH_SIZE;

ulong gTotalBlock = 0;
ulong gMetaTotalBlock = 0;
module_param_named(total, gTotalBlock, ulong, 0444);
module_param_named(mtotal, gMetaTotalBlock, ulong, 0444);
module_param_named(prefetch_cluster, dm_verity_prefetch_cluster, uint, S_IRUGO | S_IWUSR);

#ifdef DMV_ALTA
/* Verity bitmap. Each bit represents one block and will be set when integrity
 * on that block is verified.
 *
 * Entire bitmap has to be cleared when:
 * - Target device is remounted.
 * - Intruding syscalls are called for target device.
 * - Sideband attack detected.
 */
#ifdef DMV_ALTA_PROF
static sector_t total_blks = 0, skipped_blks = 0, prev_total_blks = 0;
#endif
#endif

extern int ignore_fs_panic;

struct dm_verity_prefetch_work {
	struct work_struct work;
	struct dm_verity *v;
	sector_t block;
	unsigned n_blocks;
};

/*
 * Auxiliary structure appended to each dm-bufio buffer. If the value
 * hash_verified is nonzero, hash of the block has been verified.
 *
 * The variable hash_verified is set to 0 when allocating the buffer, then
 * it can be changed to 1 and it is never reset to 0 again.
 *
 * There is no lock around this value, a race condition can at worst cause
 * that multiple processes verify the hash of the same buffer simultaneously
 * and write 1 to hash_verified simultaneously.
 * This condition is harmless, so we don't need locking.
 */
struct buffer_aux {
	int hash_verified;
};

/*
 * Initialize struct buffer_aux for a freshly created buffer.
 */
static void dm_bufio_alloc_callback(struct dm_buffer *buf)
{
	struct buffer_aux *aux = dm_bufio_get_aux_data(buf);

	aux->hash_verified = 0;
}

/*
 * Translate input sector number to the sector number on the target device.
 */
static sector_t verity_map_sector(struct dm_verity *v, sector_t bi_sector)
{
	return v->data_start + dm_target_offset(v->ti, bi_sector);
}

/*
 * Return hash position of a specified block at a specified tree level
 * (0 is the lowest level).
 * The lowest "hash_per_block_bits"-bits of the result denote hash position
 * inside a hash block. The remaining bits denote location of the hash block.
 */
static sector_t verity_position_at_level(struct dm_verity *v, sector_t block,
					 int level)
{
	return block >> (level * v->hash_per_block_bits);
}

/*
 * Wrapper for crypto_shash_init, which handles verity salting.
 */
static int verity_hash_init(struct dm_verity *v, struct shash_desc *desc)
{
	int r;

	desc->tfm = v->tfm;
	desc->flags = CRYPTO_TFM_REQ_MAY_SLEEP;

	r = crypto_shash_init(desc);

	if (unlikely(r < 0)) {
		DMERR("crypto_shash_init failed: %d", r);
		return r;
	}

	if (likely(v->version >= 1)) {
		r = crypto_shash_update(desc, v->salt, v->salt_size);

		if (unlikely(r < 0)) {
			DMERR("crypto_shash_update failed: %d", r);
			return r;
		}
	}

	return 0;
}

static int verity_hash_update(struct dm_verity *v, struct shash_desc *desc,
			      const u8 *data, size_t len)
{
	int r = crypto_shash_update(desc, data, len);

	if (unlikely(r < 0))
		DMERR("crypto_shash_update failed: %d", r);

	return r;
}

static int verity_hash_final(struct dm_verity *v, struct shash_desc *desc,
			     u8 *digest)
{
	int r;

	if (unlikely(!v->version)) {
		r = crypto_shash_update(desc, v->salt, v->salt_size);

		if (r < 0) {
			DMERR("crypto_shash_update failed: %d", r);
			return r;
		}
	}

	r = crypto_shash_final(desc, digest);

	if (unlikely(r < 0))
		DMERR("crypto_shash_final failed: %d", r);

	return r;
}

int verity_hash(struct dm_verity *v, struct shash_desc *desc,
		const u8 *data, size_t len, u8 *digest)
{
	int r;

	r = verity_hash_init(v, desc);
	if (unlikely(r < 0))
		return r;

	r = verity_hash_update(v, desc, data, len);
	if (unlikely(r < 0))
		return r;

	return verity_hash_final(v, desc, digest);
}

static void verity_hash_at_level(struct dm_verity *v, sector_t block, int level,
				 sector_t *hash_block, unsigned *offset)
{
	sector_t position = verity_position_at_level(v, block, level);
	unsigned idx;

	*hash_block = v->hash_level_block[level] + (position >> v->hash_per_block_bits);

	if (!offset)
		return;

	idx = position & ((1 << v->hash_per_block_bits) - 1);
	if (!v->version)
		*offset = idx * v->digest_size;
	else
		*offset = idx << (v->hash_dev_block_bits - v->hash_per_block_bits);
}

/*
 * Handle verification errors.
 */
#ifndef SEC_HEX_DEBUG
static int verity_handle_err(struct dm_verity *v, enum verity_block_type type,
			     unsigned long long block)
{
	char verity_env[DM_VERITY_ENV_LENGTH];
	char *envp[] = { verity_env, NULL };
	const char *type_str = "";
	struct mapped_device *md = dm_table_get_md(v->ti->table);

	/* Corruption should be visible in device status in all modes */
	v->hash_failed = 1;

	if (ignore_fs_panic) {
		DMERR("%s: Don't trigger a panic during cleanup for shutdown. Skipping %s",
				v->data_dev->name, __func__);
		return 0;
	}

	if (v->corrupted_errs >= DM_VERITY_MAX_CORRUPTED_ERRS)
		goto out;

	v->corrupted_errs++;

	switch (type) {
	case DM_VERITY_BLOCK_TYPE_DATA:
		type_str = "data";
		break;
	case DM_VERITY_BLOCK_TYPE_METADATA:
		type_str = "metadata";
		break;
	default:
		BUG();
	}

	DMERR_LIMIT("%s: %s block %llu is corrupted", v->data_dev->name,
		    type_str, block);

	if (v->corrupted_errs == DM_VERITY_MAX_CORRUPTED_ERRS)
		DMERR("%s: reached maximum errors", v->data_dev->name);

	snprintf(verity_env, DM_VERITY_ENV_LENGTH, "%s=%d,%llu",
		DM_VERITY_ENV_VAR_NAME, type, block);

	kobject_uevent_env(&disk_to_dev(dm_disk(md))->kobj, KOBJ_CHANGE, envp);

out:
	if (v->mode == DM_VERITY_MODE_LOGGING)
		return 0;

	if (v->mode == DM_VERITY_MODE_RESTART) {
#ifdef CONFIG_DM_VERITY_AVB
		dm_verity_avb_error_handler();
#endif
		kernel_restart("dm-verity device corrupted");
	}

	return 1;
}
#endif
/*
 * Verify hash of a metadata block pertaining to the specified data block
 * ("block" argument) at a specified level ("level" argument).
 *
 * On successful return, verity_io_want_digest(v, io) contains the hash value
 * for a lower tree level or for the data block (if we're at the lowest level).
 *
 * If "skip_unverified" is true, unverified buffer is skipped and 1 is returned.
 * If "skip_unverified" is false, unverified buffer is hashed and verified
 * against current value of verity_io_want_digest(v, io).
 */
static int verity_verify_level(struct dm_verity *v, struct dm_verity_io *io,
			       sector_t block, int level, bool skip_unverified,
			       u8 *want_digest)
{
	struct dm_buffer *buf;
	struct buffer_aux *aux;
	u8 *data;
	int r;
	sector_t hash_block;
	unsigned offset;

	verity_hash_at_level(v, block, level, &hash_block, &offset);

	data = dm_bufio_read(v->bufio, hash_block, &buf);
	if (IS_ERR(data))
		return PTR_ERR(data);

	aux = dm_bufio_get_aux_data(buf);

	if (!aux->hash_verified) {
		if (skip_unverified) {
			r = 1;
			goto release_ret_r;
		}

		r = verity_hash(v, verity_io_hash_desc(v, io),
				data, 1 << v->hash_dev_block_bits,
				verity_io_real_digest(v, io));
		if (unlikely(r < 0))
			goto release_ret_r;

		if (likely(memcmp(verity_io_real_digest(v, io), want_digest,
				  v->digest_size) == 0))
			aux->hash_verified = 1;
		else if (verity_fec_decode(v, io,
					DM_VERITY_BLOCK_TYPE_METADATA,
					hash_block, data, NULL) == 0) {
#ifdef SEC_HEX_DEBUG
			add_fec_correct_blks();
			add_fc_blks_entry(hash_block,v->data_dev->name);
#endif
			aux->hash_verified = 1;
		}
#ifdef SEC_HEX_DEBUG
		else if (verity_handle_err_hex_debug(v,
					DM_VERITY_BLOCK_TYPE_METADATA,
					hash_block, io, NULL)) {
			add_corrupted_blks();
#else
		else if (verity_handle_err(v,
					DM_VERITY_BLOCK_TYPE_METADATA,
					hash_block)) {
#endif
			r = -EIO;
			goto release_ret_r;
		}
	}

	data += offset;
	memcpy(want_digest, data, v->digest_size);
	r = 0;

release_ret_r:
	dm_bufio_release(buf);
	return r;
}

/*
 * Find a hash for a given block, write it to digest and verify the integrity
 * of the hash tree if necessary.
 */
int verity_hash_for_block(struct dm_verity *v, struct dm_verity_io *io,
			  sector_t block, u8 *digest, bool *is_zero)
{
	int r = 0, i;

	if (likely(v->levels)) {
		/*
		 * First, we try to get the requested hash for
		 * the current block. If the hash block itself is
		 * verified, zero is returned. If it isn't, this
		 * function returns 1 and we fall back to whole
		 * chain verification.
		 */
		r = verity_verify_level(v, io, block, 0, true, digest);
		if (likely(r <= 0))
			goto out;
	}

	memcpy(digest, v->root_digest, v->digest_size);

	for (i = v->levels - 1; i >= 0; i--) {
		r = verity_verify_level(v, io, block, i, false, digest);
		if (unlikely(r))
			goto out;
	}
out:
	if (!r && v->zero_digest)
		*is_zero = !memcmp(v->zero_digest, digest, v->digest_size);
	else
		*is_zero = false;

	return r;
}

/*
 * Calls function process for 1 << v->data_dev_block_bits bytes in the bio_vec
 * starting from iter.
 */
int verity_for_bv_block(struct dm_verity *v, struct dm_verity_io *io,
			struct bvec_iter *iter,
			int (*process)(struct dm_verity *v,
				       struct dm_verity_io *io, u8 *data,
				       size_t len))
{
	unsigned todo = 1 << v->data_dev_block_bits;
	struct bio *bio = dm_bio_from_per_bio_data(io, v->ti->per_bio_data_size);

	do {
		int r;
		u8 *page;
		unsigned len;
		struct bio_vec bv = bio_iter_iovec(bio, *iter);

		page = kmap_atomic(bv.bv_page);
		len = bv.bv_len;

		if (likely(len >= todo))
			len = todo;

		r = process(v, io, page + bv.bv_offset, len);
		kunmap_atomic(page);

		if (r < 0)
			return r;

		bio_advance_iter(bio, iter, len);
		todo -= len;
	} while (todo);

	return 0;
}

static int verity_bv_hash_update(struct dm_verity *v, struct dm_verity_io *io,
				 u8 *data, size_t len)
{
	return verity_hash_update(v, verity_io_hash_desc(v, io), data, len);
}

static int verity_bv_zero(struct dm_verity *v, struct dm_verity_io *io,
			  u8 *data, size_t len)
{
	memset(data, 0, len);
	return 0;
}

/*
 * Moves the bio iter one data block forward.
 */
static inline void verity_bv_skip_block(struct dm_verity *v,
					struct dm_verity_io *io,
					struct bvec_iter *iter)
{
	struct bio *bio = dm_bio_from_per_bio_data(io, v->ti->per_bio_data_size);

	bio_advance_iter(bio, iter, 1 << v->data_dev_block_bits);
}

/*
 * Verify one "dm_verity_io" structure.
 */
static int verity_verify_io(struct dm_verity_io *io)
{
	bool is_zero;
	struct dm_verity *v = io->v;
	struct bvec_iter start;
	unsigned b;

	for (b = 0; b < io->n_blocks; b++) {
		int r;
		sector_t cur_block = io->block + b;
		struct shash_desc *desc = verity_io_hash_desc(v, io);

		if (v->validated_blocks &&
		    likely(test_bit(cur_block, v->validated_blocks))) {
			verity_bv_skip_block(v, io, &io->iter);
#ifdef SEC_HEX_DEBUG
			add_skipped_blks();
#endif
			continue;
		}

		r = verity_hash_for_block(v, io, cur_block,
					  verity_io_want_digest(v, io),
					  &is_zero);
		if (unlikely(r < 0))
			return r;

		if (is_zero) {
			/*
			 * If we expect a zero block, don't validate, just
			 * return zeros.
			 */
			r = verity_for_bv_block(v, io, &io->iter,
						verity_bv_zero);
			if (unlikely(r < 0))
				return r;

			continue;
		}

		r = verity_hash_init(v, desc);
		if (unlikely(r < 0))
			return r;

		start = io->iter;
		r = verity_for_bv_block(v, io, &io->iter, verity_bv_hash_update);
		if (unlikely(r < 0))
			return r;

		r = verity_hash_final(v, desc, verity_io_real_digest(v, io));
		if (unlikely(r < 0))
			return r;

		if (likely(memcmp(verity_io_real_digest(v, io),
				  verity_io_want_digest(v, io), v->digest_size) == 0)) {
			if (v->validated_blocks)
				set_bit(cur_block, v->validated_blocks);
#ifdef DMV_ALTA
			set_bit(io->block + b, (volatile unsigned long *)io->v->verity_bitmap);
#endif
			continue;
		}
		else if (verity_fec_decode(v, io, DM_VERITY_BLOCK_TYPE_DATA,
					   cur_block, NULL, &start) == 0) {
#ifdef SEC_HEX_DEBUG
			add_fec_correct_blks();
			add_fc_blks_entry(cur_block,v->data_dev->name);
#endif
			continue;
		}
		else
#ifdef SEC_HEX_DEBUG
		r = verity_handle_err_hex_debug(v, DM_VERITY_BLOCK_TYPE_DATA,cur_block, io , &start);
#else
		r = verity_handle_err(v, DM_VERITY_BLOCK_TYPE_DATA,cur_block);
#endif
		if(r){
#if defined(CONFIG_TZ_ICCC)
			printk(KERN_ERR "ICCC smc ret = %llu \n",(unsigned long long)exynos_smc(SMC_CMD_DMV_WRITE_STATUS, 1, 0, 0));
#endif
			return -EIO;
		}
	}
	return 0;
}

/*
 * End one "io" structure with a given error.
 */
static void verity_finish_io(struct dm_verity_io *io, int error)
{
	struct dm_verity *v = io->v;
	struct bio *bio = dm_bio_from_per_bio_data(io, v->ti->per_bio_data_size);

	bio->bi_end_io = io->orig_bi_end_io;
	bio->bi_error = error;

	verity_fec_finish_io(io);

	bio_endio(bio);
}

static void verity_work(struct work_struct *w)
{
	struct dm_verity_io *io = container_of(w, struct dm_verity_io, work);
#ifdef DMV_ALTA
	struct bvec_iter iter = io->iter;
	int ret = verity_verify_io(io);
	struct bio *bio = dm_bio_from_per_bio_data(io, io->v->ti->per_bio_data_size);

	if (ret == -EIO && io->io_retry < IO_RETRY_MAX){
		bio->bi_next = NULL;
		bio_clear_flag(bio, BIO_SEG_VALID);
		bio->bi_iter = iter;
		bio->bi_phys_segments = 0;
		bio->bi_seg_front_size = 0;
		bio->bi_seg_back_size = 0;

		io->iter = iter;

		DMERR_LIMIT("%s: submitting io-request. io_retry cnt: %d", __func__, io->io_retry);
		io->io_retry++;
		generic_make_request(bio);

	} else {
		verity_finish_io(io, ret);
	}
#else
	verity_finish_io(io, verity_verify_io(io));
#endif

	
}

static void verity_end_io(struct bio *bio)
{
	struct dm_verity_io *io = bio->bi_private;

	if (bio->bi_error && !verity_fec_is_enabled(io->v)) {
		verity_finish_io(io, bio->bi_error);
		return;
	}

	INIT_WORK(&io->work, verity_work);
	queue_work(io->v->verify_wq, &io->work);
}

/*
 * Prefetch buffers for the specified io.
 * The root buffer is not prefetched, it is assumed that it will be cached
 * all the time.
 */
static void verity_prefetch_io(struct work_struct *work)
{
	struct dm_verity_prefetch_work *pw =
		container_of(work, struct dm_verity_prefetch_work, work);
	struct dm_verity *v = pw->v;
	int i;

	for (i = v->levels - 2; i >= 0; i--) {
		sector_t hash_block_start;
		sector_t hash_block_end;
		verity_hash_at_level(v, pw->block, i, &hash_block_start, NULL);
		verity_hash_at_level(v, pw->block + pw->n_blocks - 1, i, &hash_block_end, NULL);
		if (!i) {
			unsigned cluster = ACCESS_ONCE(dm_verity_prefetch_cluster);

			cluster >>= v->data_dev_block_bits;
			if (unlikely(!cluster))
				goto no_prefetch_cluster;

			if (unlikely(cluster & (cluster - 1)))
				cluster = 1 << __fls(cluster);

			hash_block_start &= ~(sector_t)(cluster - 1);
			hash_block_end |= cluster - 1;
			if (unlikely(hash_block_end >= v->hash_blocks))
				hash_block_end = v->hash_blocks - 1;
		}
no_prefetch_cluster:
		dm_bufio_prefetch(v->bufio, hash_block_start,
				  hash_block_end - hash_block_start + 1);
	}

	kfree(pw);
}

static void verity_submit_prefetch(struct dm_verity *v, struct dm_verity_io *io)
{
	sector_t block = io->block;
	unsigned int n_blocks = io->n_blocks;
	struct dm_verity_prefetch_work *pw;

	if (v->validated_blocks) {
		while (n_blocks && test_bit(block, v->validated_blocks)) {
			block++;
			n_blocks--;
		}
		while (n_blocks && test_bit(block + n_blocks - 1,
					v->validated_blocks))
			n_blocks--;
		if (!n_blocks)
			return;
	}

	pw = kmalloc(sizeof(struct dm_verity_prefetch_work),
		GFP_NOIO | __GFP_NORETRY | __GFP_NOMEMALLOC | __GFP_NOWARN);

	if (!pw)
		return;

	INIT_WORK(&pw->work, verity_prefetch_io);
	pw->v = v;
	pw->block = block;
	pw->n_blocks = n_blocks;
	queue_work(v->verify_wq, &pw->work);
}

#ifdef VERIFY_META_ONLY
static bool is_metablock(unsigned long long n_block)
{
    struct rb_node *node;
    struct ext4_system_zone *entry;
    bool result = false;

    node = rb_first(system_blks);
	if(node == NULL)
		return true;
    while (node) {
        entry = rb_entry(node, struct ext4_system_zone, node);
        if (n_block >= entry->start_blk && n_block <= entry->start_blk + entry->count - 1 ) {
            result = true;
            return result;
        }
        node = rb_next(node);
    }
    return result;
}
#endif

/*
 * Bio map function. It allocates dm_verity_io structure and bio vector and
 * fills them. Then it issues prefetches and the I/O.
 */
int verity_map(struct dm_target *ti, struct bio *bio)
{
	struct dm_verity *v = ti->private;
	struct dm_verity_io *io;
	unsigned int meta = 0;
	unsigned int test_blocks = 0;
#ifdef DMV_ALTA
    bool skip = true;
    sector_t bitpos, nblks;
#endif

#ifdef VERIFY_META_ONLY
	if (!start_meta && bio->bi_bdev->bd_super) {
		system_blks = ext4_system_zone_root(bio->bi_bdev->bd_super);
		DMERR_LIMIT("Successfully Get the system block information");
		start_meta = 1;
	}
#endif

	bio->bi_bdev = v->data_dev->bdev;
	bio->bi_iter.bi_sector = verity_map_sector(v, bio->bi_iter.bi_sector);

	if (((unsigned)bio->bi_iter.bi_sector | bio_sectors(bio)) &
	    ((1 << (v->data_dev_block_bits - SECTOR_SHIFT)) - 1)) {
		DMERR_LIMIT("unaligned io");
		return -EIO;
	}

	if (bio_end_sector(bio) >>
	    (v->data_dev_block_bits - SECTOR_SHIFT) > v->data_blocks) {
		DMERR_LIMIT("io out of range");
		return -EIO;
	}

	if (bio_data_dir(bio) == WRITE)
		return -EIO;
	
#ifdef DMV_ALTA
    bitpos = bio->bi_iter.bi_sector >> (v->data_dev_block_bits - SECTOR_SHIFT);
    nblks = bio->bi_iter.bi_size >> v->data_dev_block_bits;

    while (nblks) {
        if (!test_bit(bitpos, (const volatile unsigned long *)v->verity_bitmap)) {
            skip = false;
            break;
        }
        bitpos++;
        nblks--;
    }

#ifdef DMV_ALTA_PROF
    total_blks += (bio->bi_iter.bi_size >> v->data_dev_block_bits);
    if ((total_blks - prev_total_blks) > 0x1000) {
        prev_total_blks = total_blks;
        DMERR_LIMIT("total_blks=%lu skipped_blks=%lu delta=%lu, device=%s", total_blks, skipped_blks, total_blks-skipped_blks, v->data_dev->name);
    } 
#endif

    if (skip == true) {
#ifdef DMV_ALTA_PROF
        skipped_blks += (bio->bi_iter.bi_size >> v->data_dev_block_bits);
#endif
        goto skip_verity;
    }
#endif		
	
#ifdef VERIFY_META_ONLY
	if (start_meta && !is_metablock(bio->bi_iter.bi_sector >> (v->data_dev_block_bits - SECTOR_SHIFT)))
	{
		meta = 0;
		goto skip_verity;
	} else {
		meta = 1;
	}
#endif

	io = dm_per_bio_data(bio, ti->per_bio_data_size);
	io->v = v;
	io->orig_bi_end_io = bio->bi_end_io;
	io->block = bio->bi_iter.bi_sector >> (v->data_dev_block_bits - SECTOR_SHIFT);
	io->n_blocks = bio->bi_iter.bi_size >> v->data_dev_block_bits;

#ifdef SEC_HEX_DEBUG
	add_total_blks(io->n_blocks);

	if (get_total_blks() - get_prev_total_blks() > 0x4000){
	    set_prev_total_blks(get_total_blks());
	    print_blks_cnt(v->data_dev->name);
	}
#endif

	bio->bi_end_io = verity_end_io;
	bio->bi_private = io;
	io->iter = bio->bi_iter;
#ifdef DMV_ALTA
	io->io_retry = 0;
#endif
	verity_fec_init_io(io);

	verity_submit_prefetch(v, io);
#ifdef DMV_ALTA
skip_verity:
#endif	
#ifdef VERIFY_META_ONLY
skip_verity:
#endif
	test_blocks = bio->bi_iter.bi_size >> v->data_dev_block_bits;
	if (meta == 1) {
		gMetaTotalBlock += test_blocks;
	}
	gTotalBlock += test_blocks;

	generic_make_request(bio);

	return DM_MAPIO_SUBMITTED;
}
EXPORT_SYMBOL_GPL(verity_map);

/*
 * Status: V (valid) or C (corruption found)
 */
void verity_status(struct dm_target *ti, status_type_t type,
			  unsigned status_flags, char *result, unsigned maxlen)
{
	struct dm_verity *v = ti->private;
	unsigned args = 0;
	unsigned sz = 0;
	unsigned x;

	switch (type) {
	case STATUSTYPE_INFO:
		DMEMIT("%c", v->hash_failed ? 'C' : 'V');
		break;
	case STATUSTYPE_TABLE:
		DMEMIT("%u %s %s %u %u %llu %llu %s ",
			v->version,
			v->data_dev->name,
			v->hash_dev->name,
			1 << v->data_dev_block_bits,
			1 << v->hash_dev_block_bits,
			(unsigned long long)v->data_blocks,
			(unsigned long long)v->hash_start,
			v->alg_name
			);
		for (x = 0; x < v->digest_size; x++)
			DMEMIT("%02x", v->root_digest[x]);
		DMEMIT(" ");
		if (!v->salt_size)
			DMEMIT("-");
		else
			for (x = 0; x < v->salt_size; x++)
				DMEMIT("%02x", v->salt[x]);
		if (v->mode != DM_VERITY_MODE_EIO)
			args++;
		if (verity_fec_is_enabled(v))
			args += DM_VERITY_OPTS_FEC;
		if (v->zero_digest)
			args++;
		if (v->validated_blocks)
			args++;
		if (!args)
			return;
		DMEMIT(" %u", args);
		if (v->mode != DM_VERITY_MODE_EIO) {
			DMEMIT(" ");
			switch (v->mode) {
			case DM_VERITY_MODE_LOGGING:
				DMEMIT(DM_VERITY_OPT_LOGGING);
				break;
			case DM_VERITY_MODE_RESTART:
				DMEMIT(DM_VERITY_OPT_RESTART);
				break;
			default:
				BUG();
			}
		}
		if (v->zero_digest)
			DMEMIT(" " DM_VERITY_OPT_IGN_ZEROES);
		if (v->validated_blocks)
			DMEMIT(" " DM_VERITY_OPT_AT_MOST_ONCE);
		sz = verity_fec_status_table(v, sz, result, maxlen);
		break;
	}
}
EXPORT_SYMBOL_GPL(verity_status);

int verity_prepare_ioctl(struct dm_target *ti,
		struct block_device **bdev, fmode_t *mode)
{
	struct dm_verity *v = ti->private;

	*bdev = v->data_dev->bdev;

	if (v->data_start ||
	    ti->len != i_size_read(v->data_dev->bdev->bd_inode) >> SECTOR_SHIFT)
		return 1;
	return 0;
}
EXPORT_SYMBOL_GPL(verity_prepare_ioctl);

int verity_iterate_devices(struct dm_target *ti,
				  iterate_devices_callout_fn fn, void *data)
{
	struct dm_verity *v = ti->private;

	return fn(ti, v->data_dev, v->data_start, ti->len, data);
}
EXPORT_SYMBOL_GPL(verity_iterate_devices);

void verity_io_hints(struct dm_target *ti, struct queue_limits *limits)
{
	struct dm_verity *v = ti->private;

	if (limits->logical_block_size < 1 << v->data_dev_block_bits)
		limits->logical_block_size = 1 << v->data_dev_block_bits;

	if (limits->physical_block_size < 1 << v->data_dev_block_bits)
		limits->physical_block_size = 1 << v->data_dev_block_bits;

	blk_limits_io_min(limits, limits->logical_block_size);
}
EXPORT_SYMBOL_GPL(verity_io_hints);

void verity_dtr(struct dm_target *ti)
{
	struct dm_verity *v = ti->private;
	
#ifdef DMV_ALTA
    if (v->verity_bitmap) {
        kfree(v->verity_bitmap);
    }
#endif
	if (v->verify_wq)
		destroy_workqueue(v->verify_wq);

	if (v->bufio)
		dm_bufio_client_destroy(v->bufio);

	vfree(v->validated_blocks);
	kfree(v->salt);
	kfree(v->root_digest);
	kfree(v->zero_digest);

	if (v->tfm)
		crypto_free_shash(v->tfm);

	kfree(v->alg_name);

	if (v->hash_dev)
		dm_put_device(ti, v->hash_dev);

	if (v->data_dev)
		dm_put_device(ti, v->data_dev);

	verity_fec_dtr(v);

	kfree(v);
}
EXPORT_SYMBOL_GPL(verity_dtr);

static int verity_alloc_most_once(struct dm_verity *v)
{
	struct dm_target *ti = v->ti;

	/* the bitset can only handle INT_MAX blocks */
	if (v->data_blocks > INT_MAX) {
		ti->error = "device too large to use check_at_most_once";
		return -E2BIG;
	}

	v->validated_blocks = vzalloc(BITS_TO_LONGS(v->data_blocks) *
				       sizeof(unsigned long));
	if (!v->validated_blocks) {
		ti->error = "failed to allocate bitset for check_at_most_once";
		return -ENOMEM;
	}

	return 0;
}

static int verity_alloc_zero_digest(struct dm_verity *v)
{
	int r = -ENOMEM;
	struct shash_desc *desc;
	u8 *zero_data;

	v->zero_digest = kmalloc(v->digest_size, GFP_KERNEL);

	if (!v->zero_digest)
		return r;

	desc = kmalloc(v->shash_descsize, GFP_KERNEL);

	if (!desc)
		return r; /* verity_dtr will free zero_digest */

	zero_data = kzalloc(1 << v->data_dev_block_bits, GFP_KERNEL);

	if (!zero_data)
		goto out;

	r = verity_hash(v, desc, zero_data, 1 << v->data_dev_block_bits,
			v->zero_digest);

out:
	kfree(desc);
	kfree(zero_data);

	return r;
}

static int verity_parse_opt_args(struct dm_arg_set *as, struct dm_verity *v)
{
	int r;
	unsigned argc;
	struct dm_target *ti = v->ti;
	const char *arg_name;

	static struct dm_arg _args[] = {
		{0, DM_VERITY_OPTS_MAX, "Invalid number of feature args"},
	};

	r = dm_read_arg_group(_args, as, &argc, &ti->error);
	if (r)
		return -EINVAL;

	if (!argc)
		return 0;

	do {
		arg_name = dm_shift_arg(as);
		argc--;

		if (!strcasecmp(arg_name, DM_VERITY_OPT_LOGGING)) {
			v->mode = DM_VERITY_MODE_LOGGING;
			continue;

		} else if (!strcasecmp(arg_name, DM_VERITY_OPT_RESTART)) {
			v->mode = DM_VERITY_MODE_RESTART;
			continue;

		} else if (!strcasecmp(arg_name, DM_VERITY_OPT_IGN_ZEROES)) {
			r = verity_alloc_zero_digest(v);
			if (r) {
				ti->error = "Cannot allocate zero digest";
				return r;
			}
			continue;

		} else if (!strcasecmp(arg_name, DM_VERITY_OPT_AT_MOST_ONCE)) {
			r = verity_alloc_most_once(v);
			if (r)
				return r;
			continue;

		} else if (verity_is_fec_opt_arg(arg_name)) {
			r = verity_fec_parse_opt_args(as, v, &argc, arg_name);
			if (r)
				return r;
			continue;
		}

		ti->error = "Unrecognized verity feature request";
		return -EINVAL;
	} while (argc && !r);

	return r;
}

/*
 * Target parameters:
 *	<version>	The current format is version 1.
 *			Vsn 0 is compatible with original Chromium OS releases.
 *	<data device>
 *	<hash device>
 *	<data block size>
 *	<hash block size>
 *	<the number of data blocks>
 *	<hash start block>
 *	<algorithm>
 *	<digest>
 *	<salt>		Hex string or "-" if no salt.
 */
int verity_ctr(struct dm_target *ti, unsigned argc, char **argv)
{
	struct dm_verity *v;
	struct dm_arg_set as;
	unsigned int num;
	unsigned long long num_ll;
	int r;
	int i;
	sector_t hash_position;
	char dummy;

	v = kzalloc(sizeof(struct dm_verity), GFP_KERNEL);
	if (!v) {
		ti->error = "Cannot allocate verity structure";
		return -ENOMEM;
	}
	ti->private = v;
	v->ti = ti;

	r = verity_fec_ctr_alloc(v);
	if (r)
		goto bad;

	if ((dm_table_get_mode(ti->table) & ~FMODE_READ)) {
		ti->error = "Device must be readonly";
		r = -EINVAL;
		goto bad;
	}

	if (argc < 10) {
		ti->error = "Not enough arguments";
		r = -EINVAL;
		goto bad;
	}

	if (sscanf(argv[0], "%u%c", &num, &dummy) != 1 ||
	    num > 1) {
		ti->error = "Invalid version";
		r = -EINVAL;
		goto bad;
	}
	v->version = num;

	r = dm_get_device(ti, argv[1], FMODE_READ, &v->data_dev);
	if (r) {
		ti->error = "Data device lookup failed";
		goto bad;
	}

	r = dm_get_device(ti, argv[2], FMODE_READ, &v->hash_dev);
	if (r) {
		ti->error = "Data device lookup failed";
		goto bad;
	}

	if (sscanf(argv[3], "%u%c", &num, &dummy) != 1 ||
	    !num || (num & (num - 1)) ||
	    num < bdev_logical_block_size(v->data_dev->bdev) ||
	    num > PAGE_SIZE) {
		ti->error = "Invalid data device block size";
		r = -EINVAL;
		goto bad;
	}
	v->data_dev_block_bits = __ffs(num);

	if (sscanf(argv[4], "%u%c", &num, &dummy) != 1 ||
	    !num || (num & (num - 1)) ||
	    num < bdev_logical_block_size(v->hash_dev->bdev) ||
	    num > INT_MAX) {
		ti->error = "Invalid hash device block size";
		r = -EINVAL;
		goto bad;
	}
	v->hash_dev_block_bits = __ffs(num);

	if (sscanf(argv[5], "%llu%c", &num_ll, &dummy) != 1 ||
	    (sector_t)(num_ll << (v->data_dev_block_bits - SECTOR_SHIFT))
	    >> (v->data_dev_block_bits - SECTOR_SHIFT) != num_ll) {
		ti->error = "Invalid data blocks";
		r = -EINVAL;
		goto bad;
	}
	v->data_blocks = num_ll;

	if (ti->len > (v->data_blocks << (v->data_dev_block_bits - SECTOR_SHIFT))) {
		ti->error = "Data device is too small";
		r = -EINVAL;
		goto bad;
	}

	if (sscanf(argv[6], "%llu%c", &num_ll, &dummy) != 1 ||
	    (sector_t)(num_ll << (v->hash_dev_block_bits - SECTOR_SHIFT))
	    >> (v->hash_dev_block_bits - SECTOR_SHIFT) != num_ll) {
		ti->error = "Invalid hash start";
		r = -EINVAL;
		goto bad;
	}
	v->hash_start = num_ll;

	v->alg_name = kstrdup(argv[7], GFP_KERNEL);
	if (!v->alg_name) {
		ti->error = "Cannot allocate algorithm name";
		r = -ENOMEM;
		goto bad;
	}

	v->tfm = crypto_alloc_shash(v->alg_name, 0, 0);
	if (IS_ERR(v->tfm)) {
		ti->error = "Cannot initialize hash function";
		r = PTR_ERR(v->tfm);
		v->tfm = NULL;
		goto bad;
	}
	v->digest_size = crypto_shash_digestsize(v->tfm);
	if ((1 << v->hash_dev_block_bits) < v->digest_size * 2) {
		ti->error = "Digest size too big";
		r = -EINVAL;
		goto bad;
	}
	v->shash_descsize =
		sizeof(struct shash_desc) + crypto_shash_descsize(v->tfm);

	v->root_digest = kmalloc(v->digest_size, GFP_KERNEL);
	if (!v->root_digest) {
		ti->error = "Cannot allocate root digest";
		r = -ENOMEM;
		goto bad;
	}
	if (strlen(argv[8]) != v->digest_size * 2 ||
	    hex2bin(v->root_digest, argv[8], v->digest_size)) {
		ti->error = "Invalid root digest";
		r = -EINVAL;
		goto bad;
	}

	if (strcmp(argv[9], "-")) {
		v->salt_size = strlen(argv[9]) / 2;
		v->salt = kmalloc(v->salt_size, GFP_KERNEL);
		if (!v->salt) {
			ti->error = "Cannot allocate salt";
			r = -ENOMEM;
			goto bad;
		}
		if (strlen(argv[9]) != v->salt_size * 2 ||
		    hex2bin(v->salt, argv[9], v->salt_size)) {
			ti->error = "Invalid salt";
			r = -EINVAL;
			goto bad;
		}
	}

	argv += 10;
	argc -= 10;

	/* Optional parameters */
	if (argc) {
		as.argc = argc;
		as.argv = argv;

		r = verity_parse_opt_args(&as, v);
		if (r < 0)
			goto bad;
	}

#ifdef SEC_HEX_DEBUG
	get_b_info(v->data_dev->name);
#endif

#ifdef CONFIG_DM_ANDROID_VERITY_AT_MOST_ONCE_DEFAULT_ENABLED
	if (!v->validated_blocks) {
		r = verity_alloc_most_once(v);
		if (r)
			goto bad;
	}
#endif

	v->hash_per_block_bits =
		__fls((1 << v->hash_dev_block_bits) / v->digest_size);

	v->levels = 0;
	if (v->data_blocks)
		while (v->hash_per_block_bits * v->levels < 64 &&
		       (unsigned long long)(v->data_blocks - 1) >>
		       (v->hash_per_block_bits * v->levels))
			v->levels++;

	if (v->levels > DM_VERITY_MAX_LEVELS) {
		ti->error = "Too many tree levels";
		r = -E2BIG;
		goto bad;
	}

	hash_position = v->hash_start;
	for (i = v->levels - 1; i >= 0; i--) {
		sector_t s;
		v->hash_level_block[i] = hash_position;
		s = (v->data_blocks + ((sector_t)1 << ((i + 1) * v->hash_per_block_bits)) - 1)
					>> ((i + 1) * v->hash_per_block_bits);
		if (hash_position + s < hash_position) {
			ti->error = "Hash device offset overflow";
			r = -E2BIG;
			goto bad;
		}
		hash_position += s;
	}
	v->hash_blocks = hash_position;

	v->bufio = dm_bufio_client_create(v->hash_dev->bdev,
		1 << v->hash_dev_block_bits, 1, sizeof(struct buffer_aux),
		dm_bufio_alloc_callback, NULL);
	if (IS_ERR(v->bufio)) {
		ti->error = "Cannot initialize dm-bufio";
		r = PTR_ERR(v->bufio);
		v->bufio = NULL;
		goto bad;
	}

	if (dm_bufio_get_device_size(v->bufio) < v->hash_blocks) {
		ti->error = "Hash device is too small";
		r = -E2BIG;
		goto bad;
	}

	/* WQ_UNBOUND greatly improves performance when running on ramdisk */
	v->verify_wq = alloc_workqueue("kverityd", WQ_CPU_INTENSIVE | WQ_MEM_RECLAIM | WQ_UNBOUND, num_online_cpus());
	if (!v->verify_wq) {
		ti->error = "Cannot allocate workqueue";
		r = -ENOMEM;
		goto bad;
	}
#ifdef DMV_ALTA
    v->verity_bitmap = kmalloc(round_up(v->data_blocks, 8) >> 3, GFP_KERNEL);
    if (v->verity_bitmap == NULL) {
        ti->error = "Cannot allocate verity_bitmap";
        r = -ENOMEM;
        goto bad;
    }
    memset(v->verity_bitmap, 0, round_up(v->data_blocks, 8) >> 3);
#endif

	ti->per_bio_data_size = sizeof(struct dm_verity_io) +
				v->shash_descsize + v->digest_size * 2;

	r = verity_fec_ctr(v);
	if (r)
		goto bad;

	ti->per_bio_data_size = roundup(ti->per_bio_data_size,
					__alignof__(struct dm_verity_io));

#ifdef SEC_HEX_DEBUG
	if (!verity_fec_is_enabled(v))
		add_fec_off_cnt(v->data_dev->name);
#endif

	return 0;

bad:

#ifdef SEC_HEX_DEBUG
	add_fec_off_cnt("bad");
#endif

	verity_dtr(ti);

	return r;
}
EXPORT_SYMBOL_GPL(verity_ctr);

static struct target_type verity_target = {
	.name		= "verity",
	.version	= {1, 4, 0},
	.module		= THIS_MODULE,
	.ctr		= verity_ctr,
	.dtr		= verity_dtr,
	.map		= verity_map,
	.status		= verity_status,
	.prepare_ioctl	= verity_prepare_ioctl,
	.iterate_devices = verity_iterate_devices,
	.io_hints	= verity_io_hints,
};

static int __init dm_verity_init(void)
{
	int r;

	r = dm_register_target(&verity_target);
	if (r < 0)
		DMERR("register failed %d", r);

	return r;
}

static void __exit dm_verity_exit(void)
{
	dm_unregister_target(&verity_target);
}

module_init(dm_verity_init);
module_exit(dm_verity_exit);

MODULE_AUTHOR("Mikulas Patocka <mpatocka@redhat.com>");
MODULE_AUTHOR("Mandeep Baines <msb@chromium.org>");
MODULE_AUTHOR("Will Drewry <wad@chromium.org>");
MODULE_DESCRIPTION(DM_NAME " target for transparent disk integrity checking");
MODULE_LICENSE("GPL");
