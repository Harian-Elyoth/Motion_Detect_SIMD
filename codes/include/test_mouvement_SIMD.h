/* --------------------------------- */
/* ------- test_mouvement_SIMD.h -------- */
/* --------------------------------- */

#ifndef __TEST_MOUVEMENT_SIMD_H__
#define __TEST_MOUVEMENT_SIMD_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"

#include "mouvement.h"
#include "mouvement_SIMD.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

//Effectue le test unitaire pour une fonction SD
void test_mouvement_SIMD_unit();

//Effectue le test unitaire partant d'une image.
void test_mouvement_SIMD_car();

//Effectue le test sur tout le dataset.
void test_mouvement_SIMD_dataset();

// fonction principale     
void main_test_mouvement_SIMD(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif