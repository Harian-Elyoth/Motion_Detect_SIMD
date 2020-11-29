/* --------------------------------- */
/* ------- bench_morpho_SIMD.h -------- */
/* --------------------------------- */

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

#include "morpho_SIMD.h"
#include "mouvement_SIMD.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

#define HEIGHT_BENCH 240
#define WIDTH_BENCH 320

int b_bench;

vuint8 ** vimg_bin_bench;
vuint8 ** vimg_filtered_bench;
vuint8 ** tmp1_SIMD;
vuint8 ** tmp2_SIMD;

int vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench; 	// indices scalaire
int vmi0b_bench, vmi1b_bench, vmj0b_bench, vmj1b_bench; // indices scalaires avec bord


void bench_erosion_3_SIMD();
void bench_erosion_5_SIMD();

void bench_dilatation_3_SIMD();
void bench_dilatation_5_SIMD();

void bench_morpho_3_SIMD();
void bench_morpho_5_SIMD();

void bench_erosion_3_SIMD_opti();
void bench_erosion_5_SIMD_opti();

void bench_dilatation_3_SIMD_opti();
void bench_dilatation_5_SIMD_opti();

void bench_morpho_3_SIMD_opti();
void bench_morpho_5_SIMD_opti();

void bench_morpho_3_SIMD_pipeline();

void gen_vimg_bin_bench_SIMD(int kernel_size);

void main_bench_morpho_SIMD(int arg, char * argv[]);

#ifdef __cplusplus
}
#endif

#endif