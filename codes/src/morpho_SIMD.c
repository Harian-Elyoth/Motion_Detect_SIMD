/*
==================================================================
==================== Morpho Mathematiques en SIMD ================
==================================================================
*/

//=============== INCLUDES ===============================

#include "morpho_SIMD.h"


void erosion_3_SIMD(vuint8 ** vX, vuint8 ** vY, int mi0, int mi1, int mj0, int mj1){

    //vecteur aligné
    vuint8 a1, b1, c1;
    vuint8 aa0, aa2, bb0, bb2, cc0, cc2;
    vuint8 mask;
    int i, j;

    int bord = 1;
    
    int mi0b = mi0 - bord;
    int mi1b = mi1 + bord;

    int mj0b = mj0 - bord;
    int mj1b = mj1 + bord;
    
    a1 = _mm_load_si128((vuint8 *) &vX[mi0][mj0]);
    mask = _mm_set_epi8(14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0);
    aa0 = 
    DEBUG(display_vuint8(aa2, " %d", "aa2 = ")); DEBUG(puts("\n"));

}

void erosion_5_SIMD(vuint8 **vX, vuint8 ** vY, int mi0, int mi1, int mj0, int mj1){
    
}

void dilatation_3_SIMD(vuint8 **vX, vuint8 ** vY, int mi0, int mi1, int mj0, int mj1){
    
}

void dilatation_5_SIMD(vuint8 **vX, vuint8 ** vY, int mi0, int mi1, int mj0, int mj1){
    
}

void morpho_3_SIMD(vuint8 **vX, vuint8 ** vY, int mi0, int mi1, int mj0, int mj1){
    
    int bord = 1;
    
    int mi0b = mi0 - bord;
    int mi1b = mi1 + bord;

    int mj0b = mj0 - bord;
    int mj1b = mj1 + bord;

    uint8 ** tmp1 = vui8matrix(mi0b, mi1b, mj0b, mj1b);
    uint8 ** tmp2 = vui8matrix(mi0b, mi1b, mj0b, mj1b);  

    erosion_3_SIMD(vX, tmp1, mi0, mi1, mj0, mj1);
    dilatation_3_SIMD(tmp1, tmp2, mi0, mi1, mj0, mj1);
    dilatation_3_SIMD(tmp2, tmp1, mi0, mi1, mj0, mj1);
    erosion_3_SIMD(tmp1, vY, mi0, mj1, mj0, mj1);

}

void morpho_5_SIMD(vuint8 **vX, vuint8 **vY, int mi0, int mi1, int mj0, int mj1)
{
    int bord = 2;
    
    int mi0b = mi0 - bord;
    int mi1b = mi1 + bord;

    int mj0b = mj0 - bord;
    int mj1b = mj1 + bord;

    uint8 ** tmp1 = vui8matrix(mi0b, mi1b, mj0b, mj1b);
    uint8 ** tmp2 = vui8matrix(mi0b, mi1b, mj0b, mj1b);

    erosion_5_SIMD(vX, tmp1, mi0, mi1, mj0, mj1);
    dilatation_5_SIMD(tmp1, tmp2, mi0, mi1, mj0, mj1);
    dilatation_5_SIMD(tmp2, tmp1, mi0, mi1, mj0, mj1);
    erosion_5_SIMD(tmp1, vY, mi0, mi1, mj0, mj1);
}