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

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"
#include "morpho_SIMD.h"

#define load_uint8(vX, i, j) _mm_load_si128((vuint8 *) &vX[i][j])

void test_erosion_3_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void test_erosion_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void test_dilatation_3_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void test_dilatation_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void test_morpho_3_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void test_morpho_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void main_test_morpho_SIMD(int arg, char * argv[]);

#ifdef __cplusplus
}
#endif

#endif
