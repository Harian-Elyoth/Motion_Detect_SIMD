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

void morpho_3(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);
void morpho_5(uint8 **X, uint8 **Y, int mi0, int mj0, int mi1, int mj1);


#ifdef __cplusplus
}
#endif

#endif