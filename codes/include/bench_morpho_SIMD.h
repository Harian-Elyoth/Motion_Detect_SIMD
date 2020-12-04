/* ----------------------------------------- */
/* ------- bench_mouvement_morpho.h -------- */
/* ----------------------------------------- */

#ifndef __BENCH_MORPHO_SIMD_H__
#define __BENCH_MORPHO_SIMD_H__

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

#include "mouvement_SIMD.h"
#include "morpho_SIMD.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif



void bench_morpho_SIMD_car(bool is_visual, type_morpho_t MORPHO, type_opti_t OPTI, int fract);
// fonction principale     
void main_bench_morpho_SIMD(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif