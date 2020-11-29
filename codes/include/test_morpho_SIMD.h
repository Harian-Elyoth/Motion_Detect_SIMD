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
#include <time.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"
#include "morpho_SIMD.h"

#define load_uint8(vX, i, j) _mm_load_si128((vuint8 *) &vX[i][j])

#define HEIGHT_TEST 10
#define WIDTH_TEST 64


int b_test;


vuint8 ** vimg_filtered_test;
vuint8 ** tmp1_SIMD;
vuint8 ** tmp2_SIMD;
vuint8 ** vimg_bin_test;
uint8 ** img_filtered_test;
uint8 ** assertion;
uint8 ** img_bin_test;
int mi0_test, mi1_test, mj0_test, mj1_test;
int mi0b_test, mi1b_test, mj0b_test, mj1b_test;
int vmi0_test, vmi1_test, vmj0_test, vmj1_test; 	// indices scalaire
int vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test; // indices scalaires avec bord

void vui8matrix_to_ui8matrix_morpho(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8** img, vuint8** img_simd);
void ui8matrix_to_vui8matrix_wb_morpho(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8 **img, vuint8 **img_simd);
void ui8matrix_to_vui8matrix_morpho(int card, int vmi0, int vmi1, int vmj0, int vmj1, uint8 **img, vuint8 **img_simd);
void gen_img_bin_test_SIMD(type_morpho_t type);
void tests_unitaires_SIMD();
void test_unitaire_SIMD(type_morpho_t MORPHO, type_opti_t OPTI);
int equal_SIMD(uint8 ** A, uint8 ** B, int mi0, int mi1, int mj0, int mj1);
void main_test_morpho_SIMD(int arg, char * argv[]);

#ifdef __cplusplus
}
#endif

#endif
