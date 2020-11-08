/*
=======================================
=========== morpho_SIMD.h =============
=======================================
*/


#ifndef __MORPHO_SIMD_H__
#define __MORPHO_SIMD_H__

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

#define load_uint8(vX, i, j) _mm_load_si128((vuint8 *) &vX[i][j])

void erosion_3_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void erosion_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void dilatation_3_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void dilatation_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void morpho_3_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void morpho_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);


#ifdef __cplusplus
}
#endif

#endif
