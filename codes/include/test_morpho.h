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
uint8 ** tmp1;
uint8 ** tmp2;
uint8 ** assertion;

int mi0_test, mi1_test, mj0_test, mj1_test; 	// indices scalaire
int mi0b_test, mi1b_test, mj0b_test, mj1b_test; // indices scalaires avec bord


void gen_img_bin_test(type_morpho_t type);
void test_unitaire(type_morpho_t MORPHO, type_opti_t OPTI);
int equal(uint8 ** A, uint8 ** B, int mi0, int mi1, int mj0, int mj1);
// fonction principale
void main_test_morpho(int argc, char *argv[]);



#ifdef __cpluplus
}
#endif

#endif