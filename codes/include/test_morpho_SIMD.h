/*
===================================================
================ test_morpho_SIMD.h ===============
===================================================
*/

#ifndef __TEST_MORPHO_SIMD_H__
#define __TEST_MORPHO_SIMD_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"
#include "morpho_SIMD.h"

#define load_uint8(vX, i, j) _mm_load_si128((vuint8 *) &vX[i][j])

#define HEIGHT_TEST 24
#define WIDTH_TEST 32

int b_test;

vuint8 ** vimg_bin_test;
vuint8 ** vimg_filtered_test;
vuint8 ** tmp1_SIMD;
vuint8 ** tmp2_SIMD;
int vmi0_test, vmi1_test, vmj0_test, vmj1_test; 	// indices scalaire
int vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test; // indices scalaires avec bord


void test_erosion_3_SIMD();
void test_erosion_5_SIMD();
void test_dilatation_3_SIMD();
void test_dilatation_5_SIMD();
void test_morpho_3_SIMD();
void test_morpho_5_SIMD();

void test_erosion_3_SIMD_opti();
void test_erosion_5_SIMD_opti();
void test_dilatation_3_SIMD_opti();
void test_dilatation_5_SIMD_opti();
void test_morpho_3_SIMD_opti();
void test_morpho_5_SIMD_opti();

void gen_vimg_bin_test_SIMD(int type, int kernel_size);

void main_test_morpho_SIMD(int arg, char * argv[]);

#ifdef __cplusplus
}
#endif

#endif
