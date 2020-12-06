/* --------------------------------------- */
/* ------- bench_mouvement_SIMD.h -------- */
/* --------------------------------------- */

#ifndef __BENCH_MOUVEMENT_SIMD_H__
#define __BENCH_MOUVEMENT_SIMD_H__

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

#include "mouvement_SIMD.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

//Effectue le bench sur une SD
void bench_mouvement_SIMD_car();

//Effectue le bench de SD sur l'ensemble des images.
void bench_mouvement_SIMD_dataset(type_sd_t TYPE_SD, type_opti_t TYPE_OPTI);

//Ecris les performances de la combinaison SD avec différentes tailles de matrice sur un fichier .csv
void bench_mouvement_SIMD_graphic();

// fonction principale     
void main_bench_mouvement_SIMD(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif