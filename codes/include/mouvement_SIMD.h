/* --------------------------------- */
/* ------- mouvement_SIMD.h -------- */
/* --------------------------------- */

#ifndef __MOUVEMENT_SIMD_H__
#define __MOUVEMENT_SIMD_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

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


void duplicate_vborder();
void gen_pgm_img_simd();
void ui8matrix_to_vui8matrix(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8** img, vuint8** img_simd);
void vui8matrix_to_ui8matrix(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8** img, vuint8** img_simd);

void SigmaDelta_step1_simd(int vmi0b, int vmi1b, int vmj0b, int vmj1b, vuint8** mean0, vuint8** mean1, vuint8** image);
void SigmaDelta_step2_simd(int vmi0b, int vmi1b, int vmj0b, int vmj1b, vuint8** image, vuint8** mean1, vuint8** img_diff);
void SigmaDelta_step3_simd(int vmi0b, int vmi1b, int vmj0b, int vmj1b, vuint8** std0, vuint8** std1, vuint8** img_diff);
void SigmaDelta_step4_simd(int vmi0b, int vmi1b, int vmj0b, int vmj1b, vuint8** std1, vuint8** img_diff, vuint8** img_bin);

// fonction principale     
void main_mouvement_simd(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif