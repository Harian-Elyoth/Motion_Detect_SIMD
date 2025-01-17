/* ---------------------------- */
/* ------- mouvement.h -------- */
/* ---------------------------- */

#ifndef __MOUVEMENT_H__
#define __MOUVEMENT_H__

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

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

// MACRO MIN MAX
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// NB IMAGES
#define NB_IMG 200

// facteur ecart type
#define N 3

// valeurs ecart type min et max (fix)
#define VMIN 1
#define VMAX 254

// img size
#define WIDTH   320 // correspond au nb de colonne  => indice boucle j
#define HEIGHT  240 // correspond au nb de ligne    => indice boucle i

// img test size
#define WIDTH_TEST_UNIT   6 // correspond au nb de colonne  => indice boucle j
#define HEIGHT_TEST_UNIT  8 // correspond au nb de ligne    => indice boucle i


void SigmaDelta_step1(int mi0, int mi1, int mj0, int mj1, uint8** mean0, uint8** mean1, uint8** image);
void SigmaDelta_step1_unroll(int mi0, int mi1, int mj0, int mj1, uint8** mean0, uint8** mean1, uint8** image);

void SigmaDelta_step2(int mi0, int mi1, int mj0, int mj1, uint8** image, uint8** mean1, uint8** img_diff);
void SigmaDelta_step2_unroll(int mi0, int mi1, int mj0, int mj1, uint8** image, uint8** mean1, uint8** img_diff);

void SigmaDelta_step3(int mi0, int mi1, int mj0, int mj1, uint8** std0, uint8** std1, uint8** img_diff);
void SigmaDelta_step3_unroll(int mi0, int mi1, int mj0, int mj1, uint8** std0, uint8** std1, uint8** img_diff);

void SigmaDelta_step4(int mi0, int mi1, int mj0, int mj1, uint8** std1, uint8** img_diff, uint8** img_bin);
void SigmaDelta_step4_unroll(int mi0, int mi1, int mj0, int mj1, uint8** std1, uint8** img_diff, uint8** img_bin);

void SigmaDelta_fusion(int mi0, int mi1, int mj0, int mj1,  uint8** image, uint8** mean0, uint8** mean1, uint8** img_diff, uint8** std0, uint8** std1, uint8** img_bin);
void SigmaDelta_fusion_unroll(int mi0, int mi1, int mj0, int mj1,  uint8** image, uint8** mean0, uint8** mean1, uint8** std0, uint8** std1, uint8** img_bin);

void duplicate_border(int mi0, int mi1, int mj0, int mj1, int b, uint8** image);

void gen_pgm_img(int mi0, int mi1, int mj0, int mj1, uint8** mean0, uint8** std0, uint8** image);

void bin_to_pgm(int mi0, int mi1, int mj0, int mj1, uint8** img_bin, char* filename);

void copy_ui8matrix(int mi0, int mi1, int mj0, int mj1, uint8** src, uint8** dest);

#ifdef __cplusplus
}
#endif

#endif