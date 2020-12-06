/* --------------------------------- */
/* ------- bench_mouvement.h -------- */
/* --------------------------------- */

#ifndef __BENCH_MOUVEMENT_H__
#define __BENCH_MOUVEMENT_H__

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

#include "mouvement.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif
//Effectue le bench sur une SD
void bench_mouvement_car();

//Effectue le bench de SD sur l'ensemble des images.
void bench_mouvement_dataset();

//Ecris les performances de la combinaison SD avec différentes tailles de matrice sur un fichier .csv
void bench_mouvement_graphic();

// fonction principale     
void main_bench_mouvement(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif