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
#include "omp.h"

#include "mymacro.h"
#include "simd_macro.h"

#define load_uint8(vX, i, j) _mm_load_si128((vuint8 *) &vX[i][j])

void erosion_3_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void erosion_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void dilatation_3_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void dilatation_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void morpho_3_SIMD(vuint8 **X, vuint8 **Y, vuint8 ** tmp1, vuint8 ** tmp2, int mi0, int mj0, int mi1, int mj1);
void morpho_5_SIMD(vuint8 **X, vuint8 **Y, vuint8 ** tmp1, vuint8 ** tmp2, int mi0, int mj0, int mi1, int mj1);

void erosion_3_SIMD_unroll(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void erosion_5_SIMD_unroll(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void dilatation_3_SIMD_unroll(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void dilatation_5_SIMD_unroll(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void morpho_3_SIMD_unroll(vuint8 **X, vuint8 **Y, vuint8 ** tmp1, vuint8 ** tmp2, int mi0, int mj0, int mi1, int mj1);
void morpho_5_SIMD_unroll(vuint8 **X, vuint8 **Y, vuint8 ** tmp1, vuint8 ** tmp2, int mi0, int mj0, int mi1, int mj1);

void morpho_3_SIMD_pipeline(vuint8 **vX, vuint8 **tmp1, vuint8 **tmp2, vuint8 **tmp3, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1); 
void morpho_3_SIMD_pipeline_fusion(vuint8 **vX, vuint8 **tmp1, vuint8 **tmp2, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);

void erosion_3_SIMD_omp(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void erosion_5_SIMD_omp(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void dilatation_3_SIMD_omp(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void dilatation_5_SIMD_omp(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void morpho_3_SIMD_omp(vuint8 **X, vuint8 **Y, vuint8 ** tmp1, vuint8 ** tmp2, int mi0, int mj0, int mi1, int mj1);
void morpho_5_SIMD_omp(vuint8 **X, vuint8 **Y, vuint8 ** tmp1, vuint8 ** tmp2, int mi0, int mj0, int mi1, int mj1);

void erosion_3_SIMD_unroll_omp(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void erosion_5_SIMD_unroll_omp(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void dilatation_3_SIMD_unroll_omp(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);
void dilatation_5_SIMD_unroll_omp(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1);

void morpho_3_SIMD_unroll_omp(vuint8 **X, vuint8 **Y, vuint8 ** tmp1, vuint8 ** tmp2, int mi0, int mj0, int mi1, int mj1);
void morpho_5_SIMD_unroll_omp(vuint8 **X, vuint8 **Y, vuint8 ** tmp1, vuint8 ** tmp2, int mi0, int mj0, int mi1, int mj1);

void morpho_3_SIMD_pipeline_omp(vuint8 **vX, vuint8 **tmp1, vuint8 **tmp2, vuint8 **tmp3, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1); 
void morpho_3_SIMD_pipeline_fusion_omp(vuint8 **vX, vuint8 **tmp1, vuint8 **tmp2, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);

void erosion_3_SIMD_c(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);
void erosion_3_SIMD_c_unroll(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);

void erosion_5_SIMD_c(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);
void erosion_5_SIMD_c_unroll(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);

void dilatation_3_SIMD_c(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);
void dilatation_3_SIMD_c_unroll(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);

void dilatation_5_SIMD_c(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);
void dilatation_5_SIMD_c_unroll(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1);

void morpho_3_SIMD_c(vuint8 **vX, vuint8 ** vY, vuint8 ** tmp1, vuint8 ** tmp2, int vmi0, int vmi1, int vmj0, int vmj1);
void morpho_3_SIMD_c_unroll(vuint8 **vX, vuint8 ** vY, vuint8 ** tmp1, vuint8 ** tmp2, int vmi0, int vmi1, int vmj0, int vmj1);

void morpho_5_SIMD_c(vuint8 **vX, vuint8 ** vY, vuint8 ** tmp1, vuint8 ** tmp2, int vmi0, int vmi1, int vmj0, int vmj1);
void morpho_5_SIMD_c_unroll(vuint8 **vX, vuint8 ** vY, vuint8 ** tmp1, vuint8 ** tmp2, int vmi0, int vmi1, int vmj0, int vmj1);


#ifdef __cplusplus
}
#endif

#endif
