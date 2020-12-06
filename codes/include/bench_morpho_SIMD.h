/* ----------------------------------------- */
/* ------- bench_mouvement_morpho.h -------- */
/* ----------------------------------------- */

#ifndef __BENCH_MORPHO_SIMD_H__
#define __BENCH_MORPHO_SIMD_H__

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
#include "morpho_SIMD.h"

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

//Ecris les graphics des performances dans un fichier .csv
void bench_morpho_SIMD_graphic();

//Fait le bench unitaire d'une transformation morphologique MORPHO avec une optimisation OPTI. 
//le parametre fract est pour la morphologie. 0 si on veut les performances de la morphologie entière ou 1 si on veut voir les performances des 
//opération intermiédiare
void bench_morpho_SIMD_car(bool is_visual, type_morpho_t MORPHO, type_opti_t OPTI, int fract);
// fonction principale     
void main_bench_morpho_SIMD(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif