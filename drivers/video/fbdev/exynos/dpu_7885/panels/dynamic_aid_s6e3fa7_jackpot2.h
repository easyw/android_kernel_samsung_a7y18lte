#ifndef __DYNAMIC_AID_XXXX_H
#define __DYNAMIC_AID_XXXX_H __FILE__

#include "dynamic_aid.h"

enum {
	IV_VT,
	IV_1,
	IV_7,
	IV_11,
	IV_23,
	IV_35,
	IV_51,
	IV_87,
	IV_151,
	IV_203,
	IV_255,
	IV_MAX
};

enum {
	IBRIGHTNESS_002NIT,
	IBRIGHTNESS_003NIT,
	IBRIGHTNESS_004NIT,
	IBRIGHTNESS_005NIT,
	IBRIGHTNESS_006NIT,
	IBRIGHTNESS_007NIT,
	IBRIGHTNESS_008NIT,
	IBRIGHTNESS_009NIT,
	IBRIGHTNESS_010NIT,
	IBRIGHTNESS_011NIT,
	IBRIGHTNESS_012NIT,
	IBRIGHTNESS_013NIT,
	IBRIGHTNESS_014NIT,
	IBRIGHTNESS_015NIT,
	IBRIGHTNESS_016NIT,
	IBRIGHTNESS_017NIT,
	IBRIGHTNESS_019NIT,
	IBRIGHTNESS_020NIT,
	IBRIGHTNESS_021NIT,
	IBRIGHTNESS_022NIT,
	IBRIGHTNESS_024NIT,
	IBRIGHTNESS_025NIT,
	IBRIGHTNESS_027NIT,
	IBRIGHTNESS_029NIT,
	IBRIGHTNESS_030NIT,
	IBRIGHTNESS_032NIT,
	IBRIGHTNESS_034NIT,
	IBRIGHTNESS_037NIT,
	IBRIGHTNESS_039NIT,
	IBRIGHTNESS_041NIT,
	IBRIGHTNESS_044NIT,
	IBRIGHTNESS_047NIT,
	IBRIGHTNESS_050NIT,
	IBRIGHTNESS_053NIT,
	IBRIGHTNESS_056NIT,
	IBRIGHTNESS_060NIT,
	IBRIGHTNESS_064NIT,
	IBRIGHTNESS_068NIT,
	IBRIGHTNESS_072NIT,
	IBRIGHTNESS_077NIT,
	IBRIGHTNESS_082NIT,
	IBRIGHTNESS_087NIT,
	IBRIGHTNESS_093NIT,
	IBRIGHTNESS_098NIT,
	IBRIGHTNESS_105NIT,
	IBRIGHTNESS_111NIT,
	IBRIGHTNESS_119NIT,
	IBRIGHTNESS_126NIT,
	IBRIGHTNESS_134NIT,
	IBRIGHTNESS_143NIT,
	IBRIGHTNESS_152NIT,
	IBRIGHTNESS_162NIT,
	IBRIGHTNESS_172NIT,
	IBRIGHTNESS_183NIT,
	IBRIGHTNESS_195NIT,
	IBRIGHTNESS_207NIT,
	IBRIGHTNESS_220NIT,
	IBRIGHTNESS_234NIT,
	IBRIGHTNESS_249NIT,
	IBRIGHTNESS_265NIT,
	IBRIGHTNESS_282NIT,
	IBRIGHTNESS_300NIT,
	IBRIGHTNESS_316NIT,
	IBRIGHTNESS_333NIT,
	IBRIGHTNESS_350NIT,
	IBRIGHTNESS_357NIT,
	IBRIGHTNESS_365NIT,
	IBRIGHTNESS_372NIT,
	IBRIGHTNESS_380NIT,
	IBRIGHTNESS_387NIT,
	IBRIGHTNESS_395NIT,
	IBRIGHTNESS_403NIT,
	IBRIGHTNESS_412NIT,
	IBRIGHTNESS_420NIT,
	IBRIGHTNESS_MAX,
	IBRIGHTNESS_443NIT = IBRIGHTNESS_MAX,
	IBRIGHTNESS_465NIT,
	IBRIGHTNESS_488NIT,
	IBRIGHTNESS_510NIT,
	IBRIGHTNESS_533NIT,
	IBRIGHTNESS_555NIT,
	IBRIGHTNESS_578NIT,
	IBRIGHTNESS_600NIT,
	IBRIGHTNESS_HBM_MAX
};

#define VREG_OUT_X1000		6500	/* VREG_OUT x 1000 */

static const int index_voltage_table[IV_MAX] = {
	0,	/* IV_VT */
	1,	/* IV_1 */
	7,	/* IV_7 */
	11,	/* IV_11 */
	23,	/* IV_23 */
	35,	/* IV_35 */
	51,	/* IV_51 */
	87,	/* IV_87 */
	151,	/* IV_151 */
	203,	/* IV_203 */
	255	/* IV_255 */
};

static int index_voltage_reference[IV_MAX] = {
	VREG_OUT_X1000,		/* IV_VT */
	VREG_OUT_X1000,
	VREG_OUT_X1000,
	IV_VT,
	IV_VT,
	IV_VT,
	IV_VT,
	IV_VT,
	IV_VT,
	IV_VT,
	VREG_OUT_X1000		/* IV_255 */
};

static const int index_brightness_table[IBRIGHTNESS_HBM_MAX] = {
	2,	/* IBRIGHTNESS_002NIT */
	3,	/* IBRIGHTNESS_003NIT */
	4,	/* IBRIGHTNESS_004NIT */
	5,	/* IBRIGHTNESS_005NIT */
	6,	/* IBRIGHTNESS_006NIT */
	7,	/* IBRIGHTNESS_007NIT */
	8,	/* IBRIGHTNESS_008NIT */
	9,	/* IBRIGHTNESS_009NIT */
	10,	/* IBRIGHTNESS_010NIT */
	11,	/* IBRIGHTNESS_011NIT */
	12,	/* IBRIGHTNESS_012NIT */
	13,	/* IBRIGHTNESS_013NIT */
	14,	/* IBRIGHTNESS_014NIT */
	15,	/* IBRIGHTNESS_015NIT */
	16,	/* IBRIGHTNESS_016NIT */
	17,	/* IBRIGHTNESS_017NIT */
	19,	/* IBRIGHTNESS_019NIT */
	20,	/* IBRIGHTNESS_020NIT */
	21,	/* IBRIGHTNESS_021NIT */
	22,	/* IBRIGHTNESS_022NIT */
	24,	/* IBRIGHTNESS_024NIT */
	25,	/* IBRIGHTNESS_025NIT */
	27,	/* IBRIGHTNESS_027NIT */
	29,	/* IBRIGHTNESS_029NIT */
	30,	/* IBRIGHTNESS_030NIT */
	32,	/* IBRIGHTNESS_032NIT */
	34,	/* IBRIGHTNESS_034NIT */
	37,	/* IBRIGHTNESS_037NIT */
	39,	/* IBRIGHTNESS_039NIT */
	41,	/* IBRIGHTNESS_041NIT */
	44,	/* IBRIGHTNESS_044NIT */
	47,	/* IBRIGHTNESS_047NIT */
	50,	/* IBRIGHTNESS_050NIT */
	53,	/* IBRIGHTNESS_053NIT */
	56,	/* IBRIGHTNESS_056NIT */
	60,	/* IBRIGHTNESS_060NIT */
	64,	/* IBRIGHTNESS_064NIT */
	68,	/* IBRIGHTNESS_068NIT */
	72,	/* IBRIGHTNESS_072NIT */
	77,	/* IBRIGHTNESS_077NIT */
	82,	/* IBRIGHTNESS_082NIT */
	87,	/* IBRIGHTNESS_087NIT */
	93,	/* IBRIGHTNESS_093NIT */
	98,	/* IBRIGHTNESS_098NIT */
	105,	/* IBRIGHTNESS_105NIT */
	111,	/* IBRIGHTNESS_111NIT */
	119,	/* IBRIGHTNESS_119NIT */
	126,	/* IBRIGHTNESS_126NIT */
	134,	/* IBRIGHTNESS_134NIT */
	143,	/* IBRIGHTNESS_143NIT */
	152,	/* IBRIGHTNESS_152NIT */
	162,	/* IBRIGHTNESS_162NIT */
	172,	/* IBRIGHTNESS_172NIT */
	183,	/* IBRIGHTNESS_183NIT */
	195,	/* IBRIGHTNESS_195NIT */
	207,	/* IBRIGHTNESS_207NIT */
	220,	/* IBRIGHTNESS_220NIT */
	234,	/* IBRIGHTNESS_234NIT */
	249,	/* IBRIGHTNESS_249NIT */
	265,	/* IBRIGHTNESS_265NIT */
	282,	/* IBRIGHTNESS_282NIT */
	300,	/* IBRIGHTNESS_300NIT */
	316,	/* IBRIGHTNESS_316NIT */
	333,	/* IBRIGHTNESS_333NIT */
	350,	/* IBRIGHTNESS_350NIT */
	357,	/* IBRIGHTNESS_357NIT */
	365,	/* IBRIGHTNESS_365NIT */
	372,	/* IBRIGHTNESS_372NIT */
	380,	/* IBRIGHTNESS_380NIT */
	387,	/* IBRIGHTNESS_387NIT */
	395,	/* IBRIGHTNESS_395NIT */
	403,	/* IBRIGHTNESS_403NIT */
	412,	/* IBRIGHTNESS_412NIT */
	420,	/* IBRIGHTNESS_420NIT */
	443,	/* IBRIGHTNESS_443NIT */
	465,	/* IBRIGHTNESS_465NIT */
	488,	/* IBRIGHTNESS_488NIT */
	510,	/* IBRIGHTNESS_510NIT */
	533,	/* IBRIGHTNESS_533NIT */
	555,	/* IBRIGHTNESS_555NIT */
	578,	/* IBRIGHTNESS_578NIT */
	600	/* IBRIGHTNESS_600NIT */
};

static const int gamma_default_0[IV_MAX*CI_MAX] = {
	0x00, 0x00, 0x00,	/* IV_VT */
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x100, 0x100, 0x100	/* IV_255 */
};

static const int *gamma_default = gamma_default_0;

static const struct formular_t gamma_formula[IV_MAX] = {
	{0, 860},	/* IV_VT */
	{0, 256},
	{64, 320},
	{64, 320},
	{64, 320},
	{64, 320},
	{64, 320},
	{64, 320},
	{64, 320},
	{64, 320},
	{129, 860}	/* IV_255 */
};

static const int vt_voltage_value[] = {
	0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 138, 148, 158, 168, 178, 186
};

static const int m_gray[IBRIGHTNESS_MAX][IV_MAX] = {
	/* VT ~ V255 */
	{0, 1, 34, 34, 38, 38, 43, 55, 83, 109, 134},
	{0, 1, 31, 33, 35, 36, 41, 52, 82, 108, 134},
	{0, 1, 30, 31, 33, 34, 39, 52, 82, 108, 134},
	{0, 1, 29, 29, 31, 34, 39, 51, 82, 108, 134},
	{0, 1, 28, 28, 30, 33, 37, 50, 82, 108, 134},
	{0, 1, 27, 27, 29, 32, 37, 50, 82, 108, 134},
	{0, 1, 26, 26, 28, 31, 37, 50, 82, 108, 134},
	{0, 1, 25, 25, 27, 30, 36, 50, 82, 108, 134},
	{0, 1, 24, 24, 26, 29, 36, 50, 82, 108, 134},
	{0, 1, 23, 23, 25, 28, 35, 49, 82, 108, 134},
	{0, 1, 22, 22, 25, 28, 34, 49, 82, 108, 134},
	{0, 1, 22, 22, 25, 28, 34, 49, 82, 108, 134},
	{0, 1, 22, 22, 25, 28, 34, 49, 82, 108, 134},
	{0, 1, 22, 22, 25, 28, 34, 49, 82, 108, 134},
	{0, 1, 21, 21, 24, 28, 34, 49, 81, 108, 134},
	{0, 1, 21, 21, 24, 28, 34, 49, 81, 108, 134},
	{0, 1, 20, 20, 23, 27, 33, 49, 81, 107, 134},
	{0, 1, 20, 20, 23, 26, 33, 48, 81, 107, 134},
	{0, 1, 20, 20, 22, 26, 32, 48, 81, 107, 134},
	{0, 1, 19, 19, 22, 26, 32, 48, 81, 107, 134},
	{0, 1, 18, 18, 21, 25, 32, 48, 81, 107, 134},
	{0, 1, 17, 17, 20, 24, 31, 47, 80, 107, 134},
	{0, 1, 16, 16, 19, 23, 31, 47, 80, 107, 134},
	{0, 1, 15, 16, 19, 23, 31, 47, 80, 107, 134},
	{0, 1, 15, 16, 19, 23, 30, 47, 80, 107, 134},
	{0, 1, 14, 15, 18, 22, 30, 47, 80, 107, 134},
	{0, 1, 14, 15, 18, 22, 30, 47, 80, 107, 134},
	{0, 1, 13, 14, 18, 22, 30, 47, 80, 107, 134},
	{0, 1, 13, 14, 18, 22, 30, 47, 80, 107, 134},
	{0, 1, 13, 14, 18, 22, 30, 47, 80, 107, 134},
	{0, 1, 12, 13, 17, 21, 29, 47, 80, 107, 134},
	{0, 1, 11, 12, 16, 21, 29, 47, 80, 107, 134},
	{0, 1, 10, 11, 16, 21, 29, 47, 80, 107, 134},
	{0, 1, 10, 11, 16, 21, 29, 47, 80, 107, 134},
	{0, 1, 10, 11, 16, 21, 29, 47, 80, 107, 134},
	{0, 1, 10, 11, 16, 21, 29, 47, 80, 107, 134},
	{0, 1, 10, 11, 16, 21, 29, 47, 80, 107, 134},
	{0, 1, 10, 11, 16, 22, 30, 48, 83, 111, 138},
	{0, 1, 10, 12, 17, 22, 31, 50, 85, 114, 142},
	{0, 1, 10, 12, 17, 23, 31, 51, 87, 117, 146},
	{0, 1, 10, 11, 17, 23, 32, 52, 90, 121, 150},
	{0, 1, 10, 11, 17, 23, 33, 54, 92, 124, 154},
	{0, 1, 10, 11, 17, 24, 33, 55, 95, 128, 158},
	{0, 1, 10, 12, 18, 24, 34, 57, 98, 131, 163},
	{0, 1, 10, 12, 18, 25, 35, 59, 101, 136, 168},
	{0, 1, 10, 12, 18, 26, 36, 60, 104, 138, 173},
	{0, 1, 10, 12, 19, 27, 38, 63, 108, 142, 178},
	{0, 1, 10, 12, 19, 27, 39, 65, 110, 145, 182},
	{0, 1, 11, 13, 20, 28, 40, 67, 114, 151, 188},
	{0, 1, 11, 13, 20, 28, 41, 69, 117, 154, 193},
	{0, 1, 11, 13, 21, 29, 42, 71, 121, 159, 199},
	{0, 1, 11, 13, 21, 30, 43, 72, 125, 164, 205},
	{0, 1, 10, 12, 21, 30, 43, 73, 126, 167, 209},
	{0, 1, 10, 12, 21, 31, 44, 75, 129, 171, 214},
	{0, 1, 10, 12, 21, 31, 44, 75, 129, 171, 214},
	{0, 1, 10, 12, 21, 31, 44, 75, 129, 171, 214},
	{0, 1, 9, 11, 21, 31, 44, 75, 129, 171, 214},
	{0, 1, 8, 11, 20, 31, 44, 75, 129, 170, 214},
	{0, 1, 8, 11, 21, 31, 45, 76, 131, 174, 218},
	{0, 1, 9, 11, 21, 32, 46, 78, 134, 179, 223},
	{0, 1, 9, 11, 22, 33, 47, 80, 138, 182, 228},
	{0, 1, 9, 11, 22, 33, 48, 81, 140, 186, 232},
	{0, 1, 9, 11, 22, 34, 49, 83, 143, 190, 237},
	{0, 1, 8, 11, 23, 34, 49, 84, 146, 194, 242},
	{0, 1, 8, 11, 22, 35, 50, 85, 146, 197, 246},
	{0, 1, 8, 10, 23, 34, 49, 86, 149, 198, 248},
	{0, 1, 7, 11, 23, 35, 51, 87, 150, 200, 250},
	{0, 1, 7, 11, 23, 35, 51, 87, 149, 200, 250},
	{0, 1, 7, 11, 23, 35, 52, 87, 149, 200, 250},
	{0, 1, 7, 11, 23, 35, 52, 87, 149, 200, 250},
	{0, 1, 7, 11, 23, 35, 52, 87, 149, 200, 250},
	{0, 1, 7, 11, 24, 36, 52, 88, 151, 202, 252},
	{0, 1, 7, 12, 24, 36, 52, 88, 152, 203, 253},
	{0, 1, 7, 11, 23, 35, 51, 87, 151, 203, 255},
};

static const int offset_color[IBRIGHTNESS_MAX][CI_MAX * IV_MAX] = {
	/* VT ~ V255 */
	{0, 0, 0, 0, 0, 0, -13, -6, -5, -23, -3, -4, -22, -8, -10, -19, -1, -6, -22, -1, -10, -14, 1, -7, -5, 1, -2, -2, 1, -2, -5, 0, -2},
	{0, 0, 0, 0, 0, 0, -13, -6, -8, -25, -5, -8, -24, -8, -11, -19, -3, -7, -21, -4, -12, -11, 0, -7, -3, 0, -1, -2, 1, -1, -3, 0, -1},
	{0, 0, 0, 0, 0, 0, -13, -6, -8, -24, -7, -11, -22, -9, -10, -19, -3, -7, -20, -2, -12, -11, -1, -7, -3, -1, -1, -2, 1, -1, -2, 0, -1},
	{0, 0, 0, 0, 0, 0, -13, -6, -8, -24, -7, -11, -21, -2, -7, -20, -6, -9, -20, -5, -15, -10, 0, -7, -3, -1, -1, -2, 0, -1, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, -13, -6, -8, -24, -7, -11, -22, -5, -9, -20, -6, -9, -20, -5, -16, -9, 0, -6, -3, -1, -1, -1, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -13, -6, -8, -25, -8, -12, -22, -7, -10, -18, -6, -10, -20, -5, -16, -7, -1, -5, -3, -1, -1, -1, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -10, -6, -8, -20, -8, -12, -20, -5, -10, -15, -2, -7, -18, -5, -16, -7, -1, -5, -3, -1, -1, -1, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -10, -6, -8, -20, -8, -12, -17, -4, -10, -15, -2, -7, -16, -4, -14, -6, -1, -5, -3, -1, -1, -1, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -9, -5, -8, -16, -6, -12, -16, -3, -9, -11, 0, -6, -14, -3, -13, -6, -1, -5, -3, -1, -1, -1, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -9, -5, -8, -15, -5, -11, -16, -4, -10, -9, 2, -4, -14, -2, -14, -4, -1, -3, -3, -1, -1, 0, 1, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -8, -4, -6, -11, 0, -8, -16, -4, -10, -10, 1, -6, -13, -1, -13, -4, -1, -3, -3, -1, -1, 0, 1, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -8, -4, -6, -11, 0, -8, -14, -3, -10, -10, 0, -7, -13, -1, -13, -4, -1, -3, -3, -1, -1, 0, 1, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -8, -4, -6, -9, 0, -8, -14, -3, -10, -10, 0, -7, -13, -2, -13, -4, -1, -3, -3, -1, -1, 0, 1, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -8, -4, -6, -9, 0, -8, -14, -3, -11, -10, 0, -7, -13, -2, -13, -4, -1, -3, -3, -1, -1, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -3, -5, -7, 2, -8, -12, 0, -10, -10, 0, -7, -13, -2, -13, -4, -1, -3, -2, 0, -1, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -5, -7, -9, 2, -9, -13, -1, -11, -10, -1, -7, -12, -2, -12, -4, -1, -3, -2, 0, -1, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -5, -7, -10, 1, -9, -13, -2, -11, -10, -1, -7, -11, -1, -12, -3, -1, -2, -2, 0, -1, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -5, -7, -12, 0, -11, -13, -4, -11, -9, 1, -7, -11, -2, -11, -3, -1, -2, -2, 0, -1, 0, 1, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -5, -7, -15, -3, -14, -11, 0, -9, -11, -3, -9, -7, 1, -9, -3, 0, -2, -2, -1, -1, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -5, -7, -11, 2, -10, -12, -1, -10, -11, -3, -9, -7, 0, -9, -3, 0, -2, -2, -1, -1, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -5, -7, -12, 1, -12, -11, 0, -9, -10, -1, -8, -7, 0, -9, -2, 0, -1, -2, -1, -1, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -5, -7, -13, 0, -12, -10, 1, -8, -7, 1, -5, -7, 0, -9, -2, 0, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -7, -5, -7, -15, 1, -12, -8, 2, -7, -6, 3, -4, -6, 0, -8, -2, 0, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -4, 1, -4, -15, 1, -12, -9, 1, -8, -6, 2, -4, -6, 0, -8, -2, -1, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -4, 1, -4, -15, -1, -12, -10, 0, -8, -7, 0, -5, -4, 2, -7, -2, -1, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -5, 0, -4, -14, 0, -13, -10, 1, -8, -5, 2, -2, -4, 2, -7, -2, -1, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -5, 0, -4, -16, -2, -15, -10, 0, -8, -5, 2, -2, -4, 1, -7, -2, -1, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -4, 1, -3, -14, 2, -13, -11, -1, -9, -5, 2, -2, -4, 0, -7, -2, -1, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -4, 1, -4, -14, 2, -13, -11, -1, -8, -5, 1, -2, -5, -1, -8, -2, -1, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -5, 0, -5, -15, 0, -14, -11, -2, -8, -4, 1, -2, -5, -1, -8, -2, -1, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -5, -1, -5, -15, 0, -14, -9, -2, -7, -3, 2, -2, -2, 2, -5, -2, -2, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -3, 1, -2, -13, 2, -14, -7, 1, -5, -3, 1, -2, -2, 1, -5, -2, -2, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -3, 1, -2, -11, 4, -12, -7, 1, -5, -3, 1, -2, -2, 1, -5, -2, -2, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -5, 0, -4, -11, 3, -12, -7, 1, -5, -3, 0, -2, -2, 1, -5, -2, -2, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -3, 0, -4, -15, 1, -14, -6, 0, -5, -4, 0, -2, -3, 1, -5, -1, -2, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -5, -1, -4, -15, 0, -14, -7, -1, -5, -2, 1, -1, -2, 0, -5, -2, -2, -2, -1, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -4, -2, -5, -15, 0, -14, -9, -3, -7, -3, -1, -2, -2, 0, -5, -1, -1, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -4, -1, -5, -16, -1, -15, -5, 2, -4, -4, -2, -2, -2, 0, -5, -1, -1, -1, -1, 0, 0, 0, 1, 1, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -2, 4, -3, -18, -3, -15, -8, -2, -7, -3, 0, -1, -1, 1, -4, -1, -1, -1, -2, -1, 0, 0, 0, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -1, 4, -3, -18, -2, -15, -6, 0, -5, -5, -3, -3, -2, 0, -5, -1, -1, -1, -1, 0, 1, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -6, -2, -7, -14, 1, -12, -4, 1, -3, -4, -2, -2, -2, 1, -5, 1, 0, 1, -2, 0, 0, 0, 1, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -8, -2, -8, -13, 1, -12, -4, 1, -3, -1, 1, 0, -3, 0, -4, -1, -1, -1, 0, 0, 1, 1, 1, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -8, -3, -9, -13, 2, -11, -3, 1, -3, -5, -1, -2, -1, 1, -4, 0, 0, 1, -1, 0, 0, 0, 0, 0, 1, 0, -1},
	{0, 0, 0, 0, 0, 0, -5, 3, -4, -16, -2, -14, -6, -2, -5, -1, 1, -1, -2, 1, -4, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -5, 2, -5, -16, -1, -12, -4, -1, -5, -2, 1, 0, -1, 0, -4, 0, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, -6, 2, -6, -16, -2, -13, -2, 0, -2, -1, 2, 0, -1, 0, -2, -1, -1, -1, 0, 0, 0, 1, 1, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -6, 2, -6, -13, 1, -11, -3, 1, -2, -1, 0, -1, -2, -2, -5, -1, -1, 1, -1, 0, 0, 0, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -6, 1, -7, -14, 0, -12, -3, 2, -1, 0, 2, 0, -2, -1, -3, -2, -1, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, -8, -1, -9, -15, -2, -12, -5, -2, -4, 0, 1, 0, -3, -1, -4, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -11, -1, -11, -15, -4, -14, -2, 1, -2, -1, 0, -1, -2, -1, -2, 0, 0, 1, -1, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -11, -1, -11, -12, 0, -11, -6, -3, -5, 1, 2, 1, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, -11, 0, -10, -11, 0, -10, -4, -1, -4, -1, 1, -1, -1, -1, -2, 1, 1, 1, -1, 0, -1, 0, -1, -1, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, -11, -1, -11, -8, 2, -6, -1, 2, -2, 0, 1, 0, 0, 1, -1, 0, 0, 1, 0, 1, 1, 1, 0, 0, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, -10, -1, -10, -8, 3, -7, 0, 3, 0, -1, 0, -1, -2, 0, -2, -1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -13, -2, -11, -7, 1, -7, 0, 2, -1, 0, 1, 0, -1, 1, -1, -1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -5, 2, -4, -9, 0, -8, -1, 1, -2, -1, -1, -1, -1, 1, -1, -1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -3, 2, -4, -8, 2, -7, -2, 1, -1, 0, 0, 0, 0, 1, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -4, 1, -5, -7, 2, -4, 1, 3, 1, 0, 1, 1, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -4, 2, -5, -5, 3, -4, -2, -1, -2, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, -5, 0, -3, -5, 2, -4, 0, 2, 0, -1, 1, 0, -1, 0, -2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -7, -2, -6, -7, 1, -6, 0, 1, 0, -1, 2, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -8, -3, -7, -2, 2, -1, -2, -1, -2, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, -9, -4, -8, -2, 1, -1, 1, 2, 1, 1, 1, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, -5, 2, 0, 0, 3, 2, -3, -2, -2, 0, 0, 0, 0, 0, -1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1},
	{0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 2, 1, -3, -2, -3, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -2, 2, 0, 0, 2, 1, -4, -3, -4, 2, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};


#ifdef CONFIG_LCD_HMT
enum {
	IBRIGHTNESS_HMT_010NIT,
	IBRIGHTNESS_HMT_011NIT,
	IBRIGHTNESS_HMT_012NIT,
	IBRIGHTNESS_HMT_013NIT,
	IBRIGHTNESS_HMT_014NIT,
	IBRIGHTNESS_HMT_015NIT,
	IBRIGHTNESS_HMT_016NIT,
	IBRIGHTNESS_HMT_017NIT,
	IBRIGHTNESS_HMT_019NIT,
	IBRIGHTNESS_HMT_020NIT,
	IBRIGHTNESS_HMT_021NIT,
	IBRIGHTNESS_HMT_022NIT,
	IBRIGHTNESS_HMT_023NIT,
	IBRIGHTNESS_HMT_025NIT,
	IBRIGHTNESS_HMT_027NIT,
	IBRIGHTNESS_HMT_029NIT,
	IBRIGHTNESS_HMT_031NIT,
	IBRIGHTNESS_HMT_033NIT,
	IBRIGHTNESS_HMT_035NIT,
	IBRIGHTNESS_HMT_037NIT,
	IBRIGHTNESS_HMT_039NIT,
	IBRIGHTNESS_HMT_041NIT,
	IBRIGHTNESS_HMT_044NIT,
	IBRIGHTNESS_HMT_047NIT,
	IBRIGHTNESS_HMT_050NIT,
	IBRIGHTNESS_HMT_053NIT,
	IBRIGHTNESS_HMT_056NIT,
	IBRIGHTNESS_HMT_060NIT,
	IBRIGHTNESS_HMT_064NIT,
	IBRIGHTNESS_HMT_068NIT,
	IBRIGHTNESS_HMT_072NIT,
	IBRIGHTNESS_HMT_077NIT,
	IBRIGHTNESS_HMT_082NIT,
	IBRIGHTNESS_HMT_087NIT,
	IBRIGHTNESS_HMT_093NIT,
	IBRIGHTNESS_HMT_099NIT,
	IBRIGHTNESS_HMT_105NIT,
	IBRIGHTNESS_HMT_MAX
};

static const int index_brightness_table_hmt[IBRIGHTNESS_HMT_MAX] = {
	10,	/* IBRIGHTNESS_HMT_010NIT */
	11,	/* IBRIGHTNESS_HMT_011NIT */
	12,	/* IBRIGHTNESS_HMT_012NIT */
	13,	/* IBRIGHTNESS_HMT_013NIT */
	14,	/* IBRIGHTNESS_HMT_014NIT */
	15,	/* IBRIGHTNESS_HMT_015NIT */
	16,	/* IBRIGHTNESS_HMT_016NIT */
	17,	/* IBRIGHTNESS_HMT_017NIT */
	19,	/* IBRIGHTNESS_HMT_019NIT */
	20,	/* IBRIGHTNESS_HMT_020NIT */
	21,	/* IBRIGHTNESS_HMT_021NIT */
	22,	/* IBRIGHTNESS_HMT_022NIT */
	23,	/* IBRIGHTNESS_HMT_023NIT */
	25,	/* IBRIGHTNESS_HMT_025NIT */
	27,	/* IBRIGHTNESS_HMT_027NIT */
	29,	/* IBRIGHTNESS_HMT_029NIT */
	31,	/* IBRIGHTNESS_HMT_031NIT */
	33,	/* IBRIGHTNESS_HMT_033NIT */
	35,	/* IBRIGHTNESS_HMT_035NIT */
	37,	/* IBRIGHTNESS_HMT_037NIT */
	39,	/* IBRIGHTNESS_HMT_039NIT */
	41,	/* IBRIGHTNESS_HMT_041NIT */
	44,	/* IBRIGHTNESS_HMT_044NIT */
	47,	/* IBRIGHTNESS_HMT_047NIT */
	50,	/* IBRIGHTNESS_HMT_050NIT */
	53,	/* IBRIGHTNESS_HMT_053NIT */
	56,	/* IBRIGHTNESS_HMT_056NIT */
	60,	/* IBRIGHTNESS_HMT_060NIT */
	64,	/* IBRIGHTNESS_HMT_064NIT */
	68,	/* IBRIGHTNESS_HMT_068NIT */
	72,	/* IBRIGHTNESS_HMT_072NIT */
	77,	/* IBRIGHTNESS_HMT_077NIT */
	82,	/* IBRIGHTNESS_HMT_082NIT */
	87,	/* IBRIGHTNESS_HMT_087NIT */
	93,	/* IBRIGHTNESS_HMT_093NIT */
	99,	/* IBRIGHTNESS_HMT_099NIT */
	105,	/* IBRIGHTNESS_HMT_105NIT */
};

static const int m_gray_hmt[IBRIGHTNESS_HMT_MAX][IV_MAX] = {
	/* VT ~ V255 */
	{0, 1, 12, 13, 16, 20, 24, 36, 60, 78, 97},
	{0, 1, 12, 13, 17, 20, 25, 38, 62, 82, 102},
	{0, 1, 13, 14, 17, 21, 26, 39, 65, 85, 106},
	{0, 1, 13, 13, 17, 21, 27, 40, 67, 87, 110},
	{0, 1, 13, 14, 17, 21, 27, 42, 70, 91, 114},
	{0, 1, 15, 14, 18, 22, 28, 43, 72, 94, 117},
	{0, 1, 13, 14, 18, 22, 29, 44, 74, 96, 120},
	{0, 1, 13, 14, 18, 22, 29, 45, 75, 99, 124},
	{0, 1, 13, 14, 19, 23, 30, 47, 79, 104, 129},
	{0, 1, 13, 14, 19, 23, 31, 48, 81, 107, 132},
	{0, 1, 13, 14, 19, 24, 32, 49, 82, 109, 135},
	{0, 1, 13, 14, 19, 24, 32, 49, 84, 112, 138},
	{0, 1, 13, 15, 19, 24, 32, 50, 85, 114, 140},
	{0, 1, 13, 15, 20, 25, 33, 52, 88, 118, 145},
	{0, 1, 14, 15, 20, 26, 34, 54, 91, 121, 149},
	{0, 1, 14, 15, 20, 27, 35, 56, 95, 126, 154},
	{0, 1, 14, 15, 21, 27, 36, 58, 98, 130, 159},
	{0, 1, 14, 15, 21, 28, 37, 60, 101, 133, 163},
	{0, 1, 14, 16, 21, 28, 38, 62, 104, 136, 167},
	{0, 1, 14, 16, 22, 29, 39, 63, 106, 139, 171},
	{0, 1, 14, 16, 22, 29, 40, 64, 108, 142, 175},
	{0, 1, 14, 16, 22, 30, 40, 66, 112, 145, 179},
	{0, 1, 15, 16, 23, 30, 41, 68, 115, 149, 184},
	{0, 1, 14, 16, 23, 31, 43, 70, 118, 154, 189},
	{0, 1, 15, 17, 24, 32, 44, 72, 122, 158, 194},
	{0, 1, 15, 16, 24, 32, 45, 74, 124, 162, 199},
	{0, 1, 15, 17, 24, 33, 46, 75, 127, 165, 203},
	{0, 1, 15, 17, 24, 33, 47, 77, 130, 170, 208},
	{0, 1, 15, 17, 25, 34, 48, 79, 134, 175, 213},
	{0, 1, 15, 17, 25, 35, 49, 81, 137, 178, 217},
	{0, 1, 15, 18, 26, 36, 51, 84, 141, 184, 222},
	{0, 1, 11, 13, 21, 30, 42, 72, 122, 160, 197},
	{0, 1, 11, 13, 21, 30, 43, 73, 125, 164, 202},
	{0, 1, 11, 14, 22, 32, 45, 75, 129, 169, 207},
	{0, 1, 12, 14, 23, 33, 46, 77, 132, 173, 212},
	{0, 1, 12, 14, 23, 33, 47, 79, 136, 177, 216},
	{0, 1, 12, 15, 24, 34, 48, 81, 139, 182, 221},
};

static const int offset_color_hmt[IBRIGHTNESS_HMT_MAX][CI_MAX * IV_MAX] = {
	/* VT ~ V255 */
	{0, 0, 0, 0, 0, 0, -9, 0, -2, -18, 3, -7, -13, 4, -8, -7, 2, -5, -7, 3, -7, -1, 2, -4, -1, 0, -1, 0, 0, 0, -1, 0, 0},
	{0, 0, 0, 0, 0, 0, -9, 1, -2, -21, 4, -8, -10, 3, -6, -7, 3, -6, -5, 3, -6, -2, 2, -4, -2, 0, -2, 0, 0, 0, -1, 0, 0},
	{0, 0, 0, 0, 0, 0, -6, 0, -2, -16, 3, -7, -12, 3, -7, -7, 2, -6, -6, 2, -6, -2, 2, -4, -1, 0, -2, 0, 0, 0, -1, 0, 1},
	{0, 0, 0, 0, 0, 0, -5, 0, -1, -22, 4, -9, -10, 3, -6, -7, 3, -6, -5, 2, -6, -2, 1, -4, 0, 0, 0, 0, 0, 0, -1, 0, 0},
	{0, 0, 0, 0, 0, 0, -5, 0, -2, -17, 3, -7, -12, 3, -7, -6, 3, -6, -6, 3, -6, -2, 1, -3, -1, 0, -1, 0, 0, 0, -1, 0, 0},
	{0, 0, 0, 0, 0, 0, -1, 0, -1, -22, 4, -10, -10, 3, -6, -6, 2, -5, -6, 2, -6, -2, 1, -4, -1, 0, -1, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -5, 0, -2, -20, 4, -9, -10, 3, -6, -7, 2, -6, -5, 2, -6, -1, 1, -2, -1, 0, -1, 0, 0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -6, 1, -2, -19, 4, -8, -11, 3, -7, -7, 2, -6, -5, 2, -6, -1, 1, -2, -2, 0, -1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -8, 1, -2, -22, 4, -10, -8, 3, -6, -6, 2, -6, -5, 3, -6, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, -7, 1, -2, -21, 5, -10, -9, 2, -6, -6, 2, -6, -4, 2, -5, -2, 1, -2, 0, 0, -1, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -7, 1, -2, -21, 4, -10, -9, 3, -6, -6, 2, -6, -4, 2, -5, -1, 0, -1, -1, 0, -1, 0, 0, 0, -1, 0, 0},
	{0, 0, 0, 0, 0, 0, -7, 1, -2, -22, 4, -10, -9, 3, -6, -5, 2, -5, -4, 2, -5, 0, 1, -2, -1, 0, -1, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -14, 1, -4, -16, 4, -8, -10, 3, -6, -5, 2, -6, -5, 2, -6, -1, 1, -2, -1, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -14, 1, -4, -19, 5, -10, -8, 3, -6, -4, 2, -4, -3, 2, -5, -2, 0, -2, -1, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -6, 1, -2, -19, 5, -10, -8, 3, -7, -4, 2, -4, -3, 2, -5, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -5, 0, -2, -21, 5, -11, -10, 3, -7, -4, 2, -5, -3, 2, -5, -1, 0, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0},
	{0, 0, 0, 0, 0, 0, -6, 1, -2, -20, 5, -11, -8, 2, -6, -4, 2, -4, -2, 2, -4, -1, 0, -1, 0, 0, 0, 0, 0, 0, -1, 0, 1},
	{0, 0, 0, 0, 0, 0, -6, 1, -2, -20, 5, -11, -8, 3, -6, -4, 1, -4, -3, 2, -4, -1, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -10, 1, -3, -17, 4, -10, -9, 3, -7, -3, 1, -4, -3, 2, -5, -1, 0, -1, -2, 0, -1, 0, 0, 0, 0, 0, 2},
	{0, 0, 0, 0, 0, 0, -10, 1, -3, -18, 5, -12, -8, 2, -5, -3, 2, -4, -3, 1, -4, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -10, 1, -3, -17, 5, -11, -9, 2, -6, -4, 2, -5, -3, 1, -4, -1, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -10, 2, -4, -17, 5, -11, -7, 3, -6, -4, 1, -4, -2, 1, -3, -1, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -5, 1, -2, -18, 5, -12, -7, 2, -6, -3, 1, -4, -2, 1, -4, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, -11, 2, -4, -17, 5, -12, -6, 2, -5, -3, 2, -4, -2, 1, -3, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 2},
	{0, 0, 0, 0, 0, 0, -8, 1, -4, -15, 5, -11, -5, 2, -5, -3, 2, -4, -2, 1, -3, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 2},
	{0, 0, 0, 0, 0, 0, -4, 1, -2, -18, 6, -12, -5, 2, -5, -3, 2, -4, -1, 1, -2, -2, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
	{0, 0, 0, 0, 0, 0, -8, 1, -4, -14, 5, -11, -7, 2, -6, -3, 1, -4, -1, 1, -2, -1, 0, -1, -1, 0, 0, 0, 0, 0, 2, 0, 3},
	{0, 0, 0, 0, 0, 0, -8, 1, -4, -12, 5, -10, -6, 2, -5, -2, 2, -4, -1, 1, -2, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 2},
	{0, 0, 0, 0, 0, 0, -8, 1, -4, -16, 5, -12, -6, 2, -5, -1, 1, -3, -1, 1, -3, -2, 0, -1, 0, 0, -1, 0, 0, 0, 1, 0, 3},
	{0, 0, 0, 0, 0, 0, -8, 1, -4, -15, 5, -12, -6, 2, -5, 0, 1, -3, -2, 1, -3, 0, 0, 0, -1, 0, -1, 0, 0, 0, 2, 0, 3},
	{0, 0, 0, 0, 0, 0, -12, 2, -6, -13, 4, -10, -5, 2, -5, -1, 1, -3, -1, 1, -2, -1, 0, -1, -2, 0, 0, 0, 0, 0, 1, 0, 3},
	{0, 0, 0, 0, 0, 0, -8, 2, -4, -12, 5, -10, -4, 2, -4, -2, 1, -3, -1, 0, -2, 0, 0, 0, -1, 0, 0, 0, 0, -1, 1, 0, 3},
	{0, 0, 0, 0, 0, 0, -7, 2, -4, -14, 5, -11, -3, 1, -4, -1, 1, -3, -1, 1, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3},
	{0, 0, 0, 0, 0, 0, -9, 2, -5, -11, 5, -10, -3, 1, -4, -2, 1, -3, -1, 0, -2, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 3},
	{0, 0, 0, 0, 0, 0, -6, 1, -4, -11, 5, -10, -3, 1, -4, -2, 1, -3, -2, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3},
	{0, 0, 0, 0, 0, 0, -6, 2, -4, -11, 4, -10, -3, 1, -4, -1, 1, -3, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 3},
	{0, 0, 0, 0, 0, 0, -8, 2, -5, -11, 4, -10, -3, 1, -3, -1, 1, -3, -1, 0, -2, 0, 0, 0, -1, 0, -1, 0, 0, 0, 1, 0, 3},
};
#endif
#endif /* __DYNAMIC_AID_XXXX_H */
