/* ----------------------------- */
/* ------- morpho.h -------- */
/* ----------------------------- */

#ifndef __MORPHO_H__
#define __MORPHO_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"

void erosion_3(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);
void erosion_5(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);

void dilatation_3(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);
void dilatation_5(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);

void morpho_3(uint8 **X, uint8 **Y, uint8 **tmp1, uint8 **tmp2, int mi0, int mj0, int mi1, int mj1);
void morpho_5(uint8 **X, uint8 **Y, uint8 **tmp1, uint8 **tmp2, int mi0, int mj0, int mi1, int mj1);

void erosion_3_unroll(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);
void erosion_5_unroll(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);

void dilatation_3_unroll(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);
void dilatation_5_unroll(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);

void morpho_3_unroll(uint8 **X, uint8 **Y, uint8 **tmp1, uint8 **tmp2, int mi0, int mj0, int mi1, int mj1);
void morpho_5_unroll(uint8 **X, uint8 **Y, uint8 **tmp1, uint8 **tmp2, int mi0, int mj0, int mi1, int mj1);


#ifdef __cplusplus
}
#endif

#endif