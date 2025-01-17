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

#define HEIGHT_TEST_SIMD 24
#define WIDTH_TEST_SIMD 64

//Fonctions de converstion scalaire vers SIMD  / SIMD vers scalaire
void vui8matrix_to_ui8matrix_morpho(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8** img, vuint8** img_simd);
void ui8matrix_to_vui8matrix_wb_morpho(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8 **img, vuint8 **img_simd);
void ui8matrix_to_vui8matrix_morpho(int card, int vmi0, int vmi1, int vmj0, int vmj1, uint8 **img, vuint8 **img_simd);

//Fonction qui génère une matrice binaire.
void gen_img_bin_test_SIMD(type_morpho_t type, type_opti_t OPTI);

//Fonction qui effectue tous les tests unitaires
void tests_unitaires_SIMD();

//Fonction qui vérifie le bon fonctionnement des algos avec une matrice de référence.
void test_unitaire_SIMD(type_morpho_t MORPHO, type_opti_t OPTI);

//Renvoi 1 si A est égale à B, 0 Sinon.
int equal_SIMD(uint8 ** A, uint8 ** B, int mi0, int mi1, int mj0, int mj1);

//Fonction principale
void main_test_morpho_SIMD(int arg, char * argv[]);

#ifdef __cplusplus
}
#endif

#endif
