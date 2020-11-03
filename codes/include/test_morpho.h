/*
====================================================
=================== test_morpho.h ==================
====================================================
*/

#ifndef __TEST_MORPHO_H__
#define __TEST_MORPHO_H__

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

#include "morpho.h"


#define HEIGHT_TEST 24
#define WIDTH_TEST 32

int b_test;

uint8 ** img_bin_test;
uint8 ** img_filtered_test;

int mi0_test, mi1_test, mj0_test, mj1_test; 	// indices scalaire
int mi0b_test, mi1b_test, mj0b_test, mj1b_test; // indices scalaires avec bord

void test_erosion_3();
void test_dilatation_3();
void test_erosion_5();
void test_dilatation_5();
void test_morpho_3();
void test_morpho_5();
void gen_img_bin_test(int type, int kernel_size);

// fonction principale
void main_test_morpho(int argc, char *argv[]);

#ifdef __cpluplus
}
#endif

#endif