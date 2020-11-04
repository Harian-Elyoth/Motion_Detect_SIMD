/*
==================================================================
==================== Morpho Mathematiques en SIMD ================
==================================================================
*/

//=============== INCLUDES ===============================

#include "morpho_SIMD.h"


void erosion_3_SIMD(vuint8 ** vX, vuint8 ** vY, int mi0, int mj0, int mi1, int mj1){

    //vecteur aligné
    vuint8 a1, b1, c1;
    vuint8 aa0, aa2, bb0, bb2, cc0, cc2;

    int i, j;

    int bord = 1;
    
    int mi0b = mi0 - bord;
    int mi1b = mi1 + bord;

    int mj0b = mj0 - bord;
    int mj1b = mj1 + bord;
    
    aa0 = _mm_set_epi8(1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15);
    _mm_store_si128((vuint8 *) &vX[0][0], aa0);
    aa2 = _mm_load_si128((vuint8 *) &vX[0][0]);
    DEBUG(display_vuint8(aa2, " %d", "aa2 = ")); DEBUG(puts("\n"));

}

void erosion_5_SIMD(vuint8 **X, vuint8 ** Y, int mi0, int mj0, int mi1, int mj1){
    
}

void dilatation_3_SIMD(vuint8 **X, vuint8 ** Y, int mi0, int mj0, int mi1, int mj1){
    
}

void dilatation_5_SIMD(vuint8 **X, vuint8 ** Y, int mi0, int mj0, int mi1, int mj1){
    
}

void morpho_3_SIMD(vuint8 **X, vuint8 ** Y, int mi0, int mj0, int mi1, int mj1){
    
}

void morpho_5_SIMD(vuint8 **X, vuint8 **Y, int mi0, int mj0, int mi1, int mj1)
{
}