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

void copy_vui8matrix(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** src, vuint8** dest);

void duplicate_vborder();
void gen_pgm_img_simd(int mi0, int mi1, int mj0, int mj1, int vmi0, int vmi1, int vmj0, int vmj1, int card, vuint8** image, vuint8** mean0, vuint8** std0);

// fonctions de conversions sans bord
void ui8matrix_to_vui8matrix(int card, int vmi0, int vmi1, int vmj0, int vmj1, uint8** img, vuint8** img_simd);
void vui8matrix_to_ui8matrix(int card, int vmi0, int vmi1, int vmj0, int vmj1, uint8** img, vuint8** img_simd);

// fonctions de conversions avec bord
void vui8matrix_to_ui8matrix_wb(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8** img, vuint8** img_simd);
void ui8matrix_to_vui8matrix_wb(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8** img, vuint8** img_simd);

void SigmaDelta_step1_simd(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** mean0, vuint8** mean1, vuint8** image);
void SigmaDelta_step1_simd_opti(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** mean0, vuint8** mean1, vuint8** image);

void SigmaDelta_step2_simd(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** image, vuint8** mean1, vuint8** img_diff);
void SigmaDelta_step2_simd_opti(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** image, vuint8** mean1, vuint8** img_diff);

void SigmaDelta_step3_simd(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** std0, vuint8** std1, vuint8** img_diff);
void SigmaDelta_step3_simd_opti(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** std0, vuint8** std1, vuint8** img_diff);

void SigmaDelta_step4_simd(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** std1, vuint8** img_diff, vuint8** img_bin);
void SigmaDelta_step4_simd_opti(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** std1, vuint8** img_diff, vuint8** img_bin);

void SigmaDelta_simd_full(int vmi0, int vmi1, int vmj0, int vmj1,  vuint8** image, vuint8** mean0, vuint8** std0, vuint8** img_bin);
void SigmaDelta_simd_full_opti(int vmi0, int vmi1, int vmj0, int vmj1,  vuint8** image, vuint8** mean0, vuint8** std0, vuint8** img_bin);

void SigmaDelta_simd_full_opti_openMP(int vmi0, int vmi1, int vmj0, int vmj1,  vuint8** image, vuint8** mean0, vuint8** std0, vuint8** img_bin);

// fonction principale     
void main_mouvement_simd(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif