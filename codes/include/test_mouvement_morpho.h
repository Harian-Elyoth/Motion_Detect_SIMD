/* ---------------------------------------- */
/* ------- test_mouvement_morpho.h -------- */
/* ---------------------------------------- */

#ifndef __TEST_MOUVEMENT_MORPHO_H__
#define __TEST_MOUVEMENT_MORPHO_H__

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

#include "mouvement.h"
#include "morpho.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

void test_mouvement_morpho();

// fonction principale     
void main_test_mouvement_morpho(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif