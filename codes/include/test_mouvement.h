/* --------------------------------- */
/* ------- test_mouvement.h -------- */
/* --------------------------------- */

#ifndef __TEST_MOUVEMENT_H__
#define __TEST_MOUVEMENT_H__

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

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

void test_SD(bool is_visual);
void test_SD_dataset();

// fonction principale     
void main_test_mouvement(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif