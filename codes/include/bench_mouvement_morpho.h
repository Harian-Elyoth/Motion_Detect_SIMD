/* ----------------------------------------- */
/* ------- bench_mouvement_morpho.h -------- */
/* ----------------------------------------- */

#ifndef __BENCH_MOUVEMENT_MORPHO_H__
#define __BENCH_MOUVEMENT_MORPHO_H__

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
#include "morpho.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif


//Effectue le bench sur une SD + Morphologie 3 non optimisé.
void bench_mouvement_morpho_car(bool is_visual);

//Effectue le bench de SD + Morpho 3  sur l'ensemble des images.
void bench_mouvement_morpho_dataset();

//Ecris les performances de la combinaison SD + Morpho avec différentes tailles de matrice sur un fichier .csv
void bench_mouvement_morpho_graphic();

// fonction principale     
void main_bench_mouvement_morpho(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif