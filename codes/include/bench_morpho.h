/* --------------------------------- */
/* ------- bench_morpho.h -------- */
/* --------------------------------- */

#ifndef __BENCH_MORPHO_H__
#define __BENCH_MORPHO_H__

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

#include "morpho.h"
#include "mouvement.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

int b_bench;

uint8 ** img_bin_bench;
uint8 ** img_filtered_bench;

int mi0_bench, mi1_bench, mj0_bench, mj1_bench; 	// indices scalaire
int mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench; // indices scalaires avec bord

void bench_erosion_3();
void bench_erosion_5();

void bench_dilatation_3();
void bench_dilatation_5();

void bench_morpho_3();
void bench_morpho_5();

void bench_erosion_3_opti();
void bench_erosion_5_opti();

void bench_dilatation_3_opti();
void bench_dilatation_5_opti();

void bench_morpho_3_opti();
void bench_morpho_5_opti();
void gen_img_bin_bench(int type, int kernel_size);

// fonction principale     
void main_bench_morpho(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif