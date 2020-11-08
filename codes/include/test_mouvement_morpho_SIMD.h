/* --------------------------------------------- */
/* ------- test_mouvement_morpho_SIMD.h -------- */
/* --------------------------------------------- */

#ifndef __TEST_MOUVEMENT_MORPHO_SIMD_H__
#define __TEST_MOUVEMENT_MORPHO_SIMD_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

void test_mouvement_morpho_SIMD_car(bool is_visual);
void test_mouvement_morpho_SIMD_dataset();

// fonction principale     
void main_test_mouvement_morpho_SIMD(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif