/*
==================================================================
==================== Morpho Mathematiques en SIMD ================
==================================================================
*/

//=============== INCLUDES ===============================

#include "morpho_SIMD.h"


void erosion_3_SIMD(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1){

    //vecteur aligné
    vuint8 a1, b1, c1;
    //vecteur non-alignés
    vuint8 aa0, aa2, bb0, bb2, cc0, cc2;
    //vecteur sortie
    vuint8 y;

    int i, j;

    int bord = 1;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;


    for(j = vmj0 ; j <= vmj1 ; j++){

        i = vmi0;

        a1 = VEC_LOAD_2D_EPI8(i - 1, j, vX);
        aa0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        aa0 = VEC_LEFT1_EPI8(aa0, a1);
        aa2 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        aa2 = VEC_RIGHT1_EPI8(a1, aa2);

        b1 = VEC_LOAD_2D_EPI8(i, j, vX);
        bb0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        bb0 = VEC_LEFT1_EPI8(bb0, b1);
        bb2 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
        bb2 = VEC_RIGHT1_EPI8(b1, bb2);

        for(i = vmi0 ; i <= vmi1 ; i++){


            c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);
            cc0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
            cc0 = VEC_LEFT1_EPI8(cc0, c1);
            cc2 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, cc2);

            y = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);

            VEC_STORE_2D_EPI8(y, i, j, vY);
            
            //rotation de variable
            aa0 = bb0; bb0 = cc0;
            a1 = b1 ; b1 = c1;
            aa2 = bb2; bb2 = cc2;

        }
    }
}

void erosion_3_SIMD_opti(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1){

    //vecteur aligné
    vuint8 a1, b1, c1, d1, e1;
    //vecteur non-alignés
    vuint8 aa0, aa2, bb0, bb2, cc0, cc2, dd0, dd2, ee0, ee2;
    //vecteur sortie
    vuint8 y0, y1, y2;

    int i, j;

    int k = 3;

    int r = vmi1 % k;

    int bord = 1;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;


    for(j = vmj0 ; j <= vmj1 ; j++){

        i = vmi0;

        a1 = VEC_LOAD_2D_EPI8(i - 1, j, vX);
        aa0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        aa0 = VEC_LEFT1_EPI8(aa0, a1);
        aa2 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        aa2 = VEC_RIGHT1_EPI8(a1, aa2);

        b1 = VEC_LOAD_2D_EPI8(i, j, vX);
        bb0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        bb0 = VEC_LEFT1_EPI8(bb0, b1);
        bb2 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
        bb2 = VEC_RIGHT1_EPI8(b1, bb2);

        for(i = vmi0 ; i <= vmi1 - r; i = i + 3){


            c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);
            cc0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
            cc0 = VEC_LEFT1_EPI8(cc0, c1);
            cc2 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, cc2);

            d1 = VEC_LOAD_2D_EPI8(i + 2, j, vX);
            dd0 = VEC_LOAD_2D_EPI8(i + 2, j - 1, vX);
            dd0 = VEC_LEFT1_EPI8(dd0, d1);
            dd2 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            dd2 = VEC_RIGHT1_EPI8(d1, dd2);

            e1 = VEC_LOAD_2D_EPI8(i + 3, j, vX);
            ee0 = VEC_LOAD_2D_EPI8(i + 3, j - 1, vX);
            ee0 = VEC_LEFT1_EPI8(ee0, e1);
            ee2 = VEC_LOAD_2D_EPI8(i + 3, j + 1, vX);
            ee2 = VEC_RIGHT1_EPI8(e1, ee2);

            y0 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
            y1 = VEC_AND_9_EPI8(bb0, b1, bb2, cc0, c1, cc2, dd0, d1, dd2);
            y2 = VEC_AND_9_EPI8(cc0, c1, cc2, dd0, d1, dd2, ee0, e1, ee2);

            VEC_STORE_2D_EPI8(y0, i, j, vY);
            VEC_STORE_2D_EPI8(y1, i + 1, j, vY);
            VEC_STORE_2D_EPI8(y2, i + 2, j, vY);
            //rotation de variable

            aa0 = dd0; a1 = d1; aa2 = dd2;
            bb0 = ee0; b1 = e1; bb2 = ee2;

        }
    }
}
void erosion_5_SIMD(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    //vecteur aligné
    vuint8 a2, b2, c2, d2, e2, f2, g2, h2, i2;

    //vecteur non-alignés
    vuint8 aa0, aa1, aa3, aa4;
    vuint8 bb0, bb1, bb3, bb4;
    vuint8 cc0, cc1, cc3, cc4;
    vuint8 dd0, dd1, dd3, dd4;
    vuint8 ee0, ee1, ee3, ee4;

    //vecteur sortie
    vuint8 y;

    int i, j;

    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;


    for(j = vmj0 ; j <= vmj1 ; j++){

        i = vmi0;

        a2 = VEC_LOAD_2D_EPI8(i - 2, j, vX);

        aa0 = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        aa1 = VEC_LEFT1_EPI8(aa0, a2);
        aa0 = VEC_LEFT2_EPI8(aa0, a2);
        aa4 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
        aa3 = VEC_RIGHT1_EPI8(a2, aa4);
        aa4 = VEC_RIGHT2_EPI8(a2, aa4);

        b2 = VEC_LOAD_2D_EPI8(i - 1, j, vX);

        bb0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        bb1 = VEC_LEFT1_EPI8(bb0, b2);
        bb0 = VEC_LEFT2_EPI8(bb0, b2);
        bb4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        bb3 = VEC_RIGHT1_EPI8(b2, bb4);
        bb4 = VEC_RIGHT2_EPI8(b2, bb4);

        c2 = VEC_LOAD_2D_EPI8(i, j, vX);

        cc0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        cc1 = VEC_LEFT1_EPI8(cc0, c2);
        cc0 = VEC_LEFT2_EPI8(cc0, c2);
        cc4 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
        cc3 = VEC_RIGHT1_EPI8(c2, cc4);
        cc4 = VEC_RIGHT2_EPI8(c2, cc4);

        d2 = VEC_LOAD_2D_EPI8(i + 1, j, vX);

        dd0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        dd1 = VEC_LEFT1_EPI8(dd0, d2);
        dd0 = VEC_LEFT2_EPI8(dd0, d2);
        dd4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
        dd3 = VEC_RIGHT1_EPI8(d2, dd4);
        dd4 = VEC_RIGHT2_EPI8(d2, dd4);


        for(i = vmi0 ; i <= vmi1 ; i++){

            e2 = VEC_LOAD_2D_EPI8(i + 2, j, vX);

            ee0 = VEC_LOAD_2D_EPI8(i + 2, j - 1, vX);
            ee1 = VEC_LEFT1_EPI8(ee0, e2);
            ee0 = VEC_LEFT2_EPI8(ee0, e2);
            ee4 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            ee3 = VEC_RIGHT1_EPI8(e2, ee4);
            ee4 = VEC_RIGHT2_EPI8(e2, ee4);

            y = VEC_AND_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);

            VEC_STORE_2D_EPI8(y, i, j, vY);
            
            //rotation de variable
            aa0 = bb0 ; bb0 = cc0 ; cc0 = dd0 ; dd0 = ee0;
            aa1 = bb1 ; bb1 = cc1 ; cc1 = dd1 ; dd1 = ee1;
            a2  = b2  ; b2  = c2  ; c2  = d2  ;  d2 = e2;
            aa3 = bb3 ; bb3 = cc3 ; cc3 = dd3 ; dd3 = ee3;
            aa4 = bb4 ; bb4 = cc4 ; cc4 = dd4 ; dd4 = ee4;
            
        }
    }
}

void erosion_5_SIMD_opti(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    //vecteur aligné
    vuint8 a2, b2, c2, d2, e2, f2, g2, h2, i2;

    //vecteur non-alignés
    vuint8 aa0, aa1, aa3, aa4;
    vuint8 bb0, bb1, bb3, bb4;
    vuint8 cc0, cc1, cc3, cc4;
    vuint8 dd0, dd1, dd3, dd4;
    vuint8 ee0, ee1, ee3, ee4;
    vuint8 ff0, ff1, ff3, ff4;
    vuint8 gg0, gg1, gg3, gg4;
    vuint8 hh0, hh1, hh3, hh4;
    vuint8 ii0, ii1, ii3, ii4;
    //vecteur sortie
    vuint8 y0, y1, y2, y3, y4;

    int k = 5;

    int r = vmi1 % k;

    int i, j;

    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;

    printf("mi1 = %d, r = %d\n", vmi1, r);

    for(j = vmj0 ; j <= vmj1 ; j++){

        i = vmi0;

        a2 = VEC_LOAD_2D_EPI8(i - 2, j, vX);

        aa0 = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        aa1 = VEC_LEFT1_EPI8(aa0, a2);
        aa0 = VEC_LEFT2_EPI8(aa0, a2);
        aa4 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
        aa3 = VEC_RIGHT1_EPI8(a2, aa4);
        aa4 = VEC_RIGHT2_EPI8(a2, aa4);

        b2 = VEC_LOAD_2D_EPI8(i - 1, j, vX);

        bb0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        bb1 = VEC_LEFT1_EPI8(bb0, b2);
        bb0 = VEC_LEFT2_EPI8(bb0, b2);
        bb4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        bb3 = VEC_RIGHT1_EPI8(b2, bb4);
        bb4 = VEC_RIGHT2_EPI8(b2, bb4);

        c2 = VEC_LOAD_2D_EPI8(i, j, vX);

        cc0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        cc1 = VEC_LEFT1_EPI8(cc0, c2);
        cc0 = VEC_LEFT2_EPI8(cc0, c2);
        cc4 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
        cc3 = VEC_RIGHT1_EPI8(c2, cc4);
        cc4 = VEC_RIGHT2_EPI8(c2, cc4);

        d2 = VEC_LOAD_2D_EPI8(i + 1, j, vX);

        dd0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        dd1 = VEC_LEFT1_EPI8(dd0, d2);
        dd0 = VEC_LEFT2_EPI8(dd0, d2);
        dd4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
        dd3 = VEC_RIGHT1_EPI8(d2, dd4);
        dd4 = VEC_RIGHT2_EPI8(d2, dd4);

        printf("%deme colonne !\n", j);
        for(i = vmi0 ; i <= vmi1 - r; i = i + 5){
            printf("i = %d\n", i);
            e2 = VEC_LOAD_2D_EPI8(i + 2, j, vX);
            ee0 = VEC_LOAD_2D_EPI8(i + 2, j - 1, vX);
            ee1 = VEC_LEFT1_EPI8(ee0, e2);
            ee0 = VEC_LEFT2_EPI8(ee0, e2);
            ee4 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            ee3 = VEC_RIGHT1_EPI8(e2, ee4);
            ee4 = VEC_RIGHT2_EPI8(e2, ee4);

            f2 = VEC_LOAD_2D_EPI8(i + 3, j, vX);
            ff0 = VEC_LOAD_2D_EPI8(i + 3, j - 1, vX);
            ff1 = VEC_LEFT1_EPI8(ff0, f2);
            ff0 = VEC_LEFT2_EPI8(ff0, f2);
            ff4 = VEC_LOAD_2D_EPI8(i + 3, j + 1, vX);
            ff3 = VEC_RIGHT1_EPI8(f2, ff4);
            ff4 = VEC_RIGHT2_EPI8(f2, ff4);

            g2 = VEC_LOAD_2D_EPI8(i + 4, j, vX);
            gg0 = VEC_LOAD_2D_EPI8(i + 4, j - 1, vX);
            gg1 = VEC_LEFT1_EPI8(gg0, g2);
            gg0 = VEC_LEFT2_EPI8(gg0, g2);
            gg4 = VEC_LOAD_2D_EPI8(i + 4, j + 1, vX);
            gg3 = VEC_RIGHT1_EPI8(g2, gg4);
            gg4 = VEC_RIGHT2_EPI8(g2, gg4);
        
            h2 = VEC_LOAD_2D_EPI8(i + 5, j, vX);
            hh0 = VEC_LOAD_2D_EPI8(i + 5, j - 1, vX);
            hh1 = VEC_LEFT1_EPI8(hh0, h2);
            hh0 = VEC_LEFT2_EPI8(hh0, h2);
            hh4 = VEC_LOAD_2D_EPI8(i + 5, j + 1, vX);
            hh3 = VEC_RIGHT1_EPI8(h2, hh4);
            hh4 = VEC_RIGHT2_EPI8(h2, hh4);

            i2 = VEC_LOAD_2D_EPI8(i + 6, j, vX);
            ii0 = VEC_LOAD_2D_EPI8(i + 6, j - 1, vX);
            ii1 = VEC_LEFT1_EPI8(ii0, i2);
            ii0 = VEC_LEFT2_EPI8(ii0, i2);
            ii4 = VEC_LOAD_2D_EPI8(i + 6, j + 1, vX);
            ii3 = VEC_RIGHT1_EPI8(i2, ii4);
            ii4 = VEC_RIGHT2_EPI8(i2, ii4);

            y0 = VEC_AND_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);
            y1 = VEC_AND_25_EPI8(bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4);
            y2 = VEC_AND_25_EPI8(cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4);
            y3 = VEC_AND_25_EPI8(dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4, hh0, hh1, h2, hh3, hh4);
            y4 = VEC_AND_25_EPI8(ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4, hh0, hh1, h2, hh3, hh4, ii0, ii1, i2, ii3, ii4);

            VEC_STORE_2D_EPI8(y0, i, j, vY);
            VEC_STORE_2D_EPI8(y1, i + 1, j, vY);
            VEC_STORE_2D_EPI8(y2, i + 2, j, vY);
            VEC_STORE_2D_EPI8(y3, i + 3, j, vY);
            VEC_STORE_2D_EPI8(y4, i + 4, j, vY);
            
            //rotation de variable

            aa0 = ff0; aa1 = ff1; a2 = f2; aa3 = ff3; aa4 = ff4;
            bb0 = gg0; bb1 = gg1; b2 = g2; bb3 = gg3; bb4 = gg4;
            cc0 = hh0; cc1 = hh1; c2 = h2; cc3 = hh3; cc4 = hh4;
            dd0 = ii0; dd1 = ii1; d2 = i2; dd3 = ii3; dd4 = ii4;
            
        }
    }
}


void dilatation_3_SIMD(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
        
    //vecteur aligné
    vuint8 a1, b1, c1;

    //vecteur non-alignés
    vuint8 aa0, aa2, bb0, bb2, cc0, cc2;

    //vecteur sortie
    vuint8 y;

    int i, j;

    int bord = 1;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;

    for(j = vmj0 ; j <= vmj1 ; j++){

        i = vmi0;
        a1 = VEC_LOAD_2D_EPI8(i - 1, j, vX);
        aa0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        aa0 = VEC_LEFT1_EPI8(aa0, a1);
        aa2 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        aa2 = VEC_RIGHT1_EPI8(a1, aa2);

        b1 = VEC_LOAD_2D_EPI8(i, j, vX);
        bb0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        bb0 = VEC_LEFT1_EPI8(bb0, b1);
        bb2 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
        bb2 = VEC_RIGHT1_EPI8(b1, bb2);

        for(i = vmi0 ; i <= vmi1 ; i++){

            c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);
            cc0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
            cc0 = VEC_LEFT1_EPI8(cc0, c1);
            cc2 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, cc2);

            y = VEC_OR_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);

            VEC_STORE_2D_EPI8(y, i, j, vY);
            
            //rotation de variable
            aa0 = bb0; bb0 = cc0;
            a1 = b1 ; b1 = c1;
            aa2 = bb2; bb2 = cc2;

        }
    }
}

void dilatation_3_SIMD_opti(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1){

    //vecteur aligné
    vuint8 a1, b1, c1, d1, e1;
    //vecteur non-alignés
    vuint8 aa0, aa2, bb0, bb2, cc0, cc2, dd0, dd2, ee0, ee2;
    //vecteur sortie
    vuint8 y0, y1, y2;

    int i, j;

    int k = 3;

    int r = vmi1 % k;

    int bord = 1;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;


    for(j = vmj0 ; j <= vmj1 ; j++){

        i = vmi0;

        a1 = VEC_LOAD_2D_EPI8(i - 1, j, vX);
        aa0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        aa0 = VEC_LEFT1_EPI8(aa0, a1);
        aa2 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        aa2 = VEC_RIGHT1_EPI8(a1, aa2);

        b1 = VEC_LOAD_2D_EPI8(i, j, vX);
        bb0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        bb0 = VEC_LEFT1_EPI8(bb0, b1);
        bb2 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
        bb2 = VEC_RIGHT1_EPI8(b1, bb2);

        for(i = vmi0 ; i <= vmi1 - r; i = i + 3){


            c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);
            cc0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
            cc0 = VEC_LEFT1_EPI8(cc0, c1);
            cc2 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, cc2);

            d1 = VEC_LOAD_2D_EPI8(i + 2, j, vX);
            dd0 = VEC_LOAD_2D_EPI8(i + 2, j - 1, vX);
            dd0 = VEC_LEFT1_EPI8(dd0, d1);
            dd2 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            dd2 = VEC_RIGHT1_EPI8(d1, dd2);

            e1 = VEC_LOAD_2D_EPI8(i + 3, j, vX);
            ee0 = VEC_LOAD_2D_EPI8(i + 3, j - 1, vX);
            ee0 = VEC_LEFT1_EPI8(ee0, e1);
            ee2 = VEC_LOAD_2D_EPI8(i + 3, j + 1, vX);
            ee2 = VEC_RIGHT1_EPI8(e1, ee2);

            y0 = VEC_OR_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
            y1 = VEC_OR_9_EPI8(bb0, b1, bb2, cc0, c1, cc2, dd0, d1, dd2);
            y2 = VEC_OR_9_EPI8(cc0, c1, cc2, dd0, d1, dd2, ee0, e1, ee2);

            VEC_STORE_2D_EPI8(y0, i, j, vY);
            VEC_STORE_2D_EPI8(y1, i + 1, j, vY);
            VEC_STORE_2D_EPI8(y2, i + 2, j, vY);
            //rotation de variable

            aa0 = dd0; a1 = d1; aa2 = dd2;
            bb0 = ee0; b1 = e1; bb2 = ee2;

        }
    }
}

void dilatation_5_SIMD(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    //vecteur aligné
    vuint8 a2, b2, c2, d2, e2;
    //vecteur non-alignés
    vuint8 aa0, aa1, aa3, aa4;
    vuint8 bb0, bb1, bb3, bb4;
    vuint8 cc0, cc1, cc3, cc4;
    vuint8 dd0, dd1, dd3, dd4;
    vuint8 ee0, ee1, ee3, ee4;
    //vecteur sortie
    vuint8 y;

    int i, j;

    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;


    for(j = vmj0 ; j <= vmj1 ; j++){

        i = vmi0;

        a2 = VEC_LOAD_2D_EPI8(i - 2, j, vX);

        aa0 = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        aa1 = VEC_LEFT1_EPI8(aa0, a2);
        aa0 = VEC_LEFT2_EPI8(aa0, a2);
        aa4 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
        aa3 = VEC_RIGHT1_EPI8(a2, aa4);
        aa4 = VEC_RIGHT2_EPI8(a2, aa4);

        b2 = VEC_LOAD_2D_EPI8(i - 1, j, vX);

        bb0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        bb1 = VEC_LEFT1_EPI8(bb0, b2);
        bb0 = VEC_LEFT2_EPI8(bb0, b2);
        bb4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        bb3 = VEC_RIGHT1_EPI8(b2, bb4);
        bb4 = VEC_RIGHT2_EPI8(b2, bb4);

        c2 = VEC_LOAD_2D_EPI8(i, j, vX);

        cc0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        cc1 = VEC_LEFT1_EPI8(cc0, c2);
        cc0 = VEC_LEFT2_EPI8(cc0, c2);
        cc4 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
        cc3 = VEC_RIGHT1_EPI8(c2, cc4);
        cc4 = VEC_RIGHT2_EPI8(c2, cc4);

        d2 = VEC_LOAD_2D_EPI8(i + 1, j, vX);

        dd0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        dd1 = VEC_LEFT1_EPI8(dd0, d2);
        dd0 = VEC_LEFT2_EPI8(dd0, d2);
        dd4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
        dd3 = VEC_RIGHT1_EPI8(d2, dd4);
        dd4 = VEC_RIGHT2_EPI8(d2, dd4);


        for(i = vmi0 ; i <= vmi1 ; i++){

            e2 = VEC_LOAD_2D_EPI8(i + 2, j, vX);

            ee0 = VEC_LOAD_2D_EPI8(i + 2, j - 1, vX);
            ee1 = VEC_LEFT1_EPI8(ee0, e2);
            ee0 = VEC_LEFT2_EPI8(ee0, e2);
            ee4 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            ee3 = VEC_RIGHT1_EPI8(e2, ee4);
            ee4 = VEC_RIGHT2_EPI8(e2, ee4);

            y = VEC_OR_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);

            VEC_STORE_2D_EPI8(y, i, j, vY);
            
            //rotation de variable
            aa0 = bb0 ; bb0 = cc0 ; cc0 = dd0 ; dd0 = ee0;
            aa1 = bb1 ; bb1 = cc1 ; cc1 = dd1 ; dd1 = ee1;
            a2  = b2  ; b2  = c2  ; c2  = d2  ;  d2 = e2;
            aa3 = bb3 ; bb3 = cc3 ; cc3 = dd3 ; dd3 = ee3;
            aa4 = bb4 ; bb4 = cc4 ; cc4 = dd4 ; dd4 = ee4;
            
        }
    }
}

void dilatation_5_SIMD_opti(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    //vecteur aligné
    vuint8 a2, b2, c2, d2, e2, f2, g2, h2, i2;

    //vecteur non-alignés
    vuint8 aa0, aa1, aa3, aa4;
    vuint8 bb0, bb1, bb3, bb4;
    vuint8 cc0, cc1, cc3, cc4;
    vuint8 dd0, dd1, dd3, dd4;
    vuint8 ee0, ee1, ee3, ee4;
    vuint8 ff0, ff1, ff3, ff4;
    vuint8 gg0, gg1, gg3, gg4;
    vuint8 hh0, hh1, hh3, hh4;
    vuint8 ii0, ii1, ii3, ii4;
    //vecteur sortie
    vuint8 y0, y1, y2, y3, y4;

    int k = 5;

    int r = vmi1 % k;

    int i, j;

    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;


    for(j = vmj0 ; j <= vmj1 ; j++){

        i = vmi0;

        a2 = VEC_LOAD_2D_EPI8(i - 2, j, vX);

        aa0 = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        aa1 = VEC_LEFT1_EPI8(aa0, a2);
        aa0 = VEC_LEFT2_EPI8(aa0, a2);
        aa4 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
        aa3 = VEC_RIGHT1_EPI8(a2, aa4);
        aa4 = VEC_RIGHT2_EPI8(a2, aa4);

        b2 = VEC_LOAD_2D_EPI8(i - 1, j, vX);

        bb0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        bb1 = VEC_LEFT1_EPI8(bb0, b2);
        bb0 = VEC_LEFT2_EPI8(bb0, b2);
        bb4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        bb3 = VEC_RIGHT1_EPI8(b2, bb4);
        bb4 = VEC_RIGHT2_EPI8(b2, bb4);

        c2 = VEC_LOAD_2D_EPI8(i, j, vX);

        cc0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        cc1 = VEC_LEFT1_EPI8(cc0, c2);
        cc0 = VEC_LEFT2_EPI8(cc0, c2);
        cc4 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
        cc3 = VEC_RIGHT1_EPI8(c2, cc4);
        cc4 = VEC_RIGHT2_EPI8(c2, cc4);

        d2 = VEC_LOAD_2D_EPI8(i + 1, j, vX);

        dd0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        dd1 = VEC_LEFT1_EPI8(dd0, d2);
        dd0 = VEC_LEFT2_EPI8(dd0, d2);
        dd4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
        dd3 = VEC_RIGHT1_EPI8(d2, dd4);
        dd4 = VEC_RIGHT2_EPI8(d2, dd4);


        for(i = vmi0 ; i <= vmi1 - r ; i = i + 5){

            e2 = VEC_LOAD_2D_EPI8(i + 2, j, vX);
            ee0 = VEC_LOAD_2D_EPI8(i + 2, j - 1, vX);
            ee1 = VEC_LEFT1_EPI8(ee0, e2);
            ee0 = VEC_LEFT2_EPI8(ee0, e2);
            ee4 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            ee3 = VEC_RIGHT1_EPI8(e2, ee4);
            ee4 = VEC_RIGHT2_EPI8(e2, ee4);

            f2 = VEC_LOAD_2D_EPI8(i + 3, j, vX);
            ff0 = VEC_LOAD_2D_EPI8(i + 3, j - 1, vX);
            ff1 = VEC_LEFT1_EPI8(ff0, f2);
            ff0 = VEC_LEFT2_EPI8(ff0, f2);
            ff4 = VEC_LOAD_2D_EPI8(i + 3, j + 1, vX);
            ff3 = VEC_RIGHT1_EPI8(f2, ff4);
            ff4 = VEC_RIGHT2_EPI8(f2, ff4);

            g2 = VEC_LOAD_2D_EPI8(i + 4, j, vX);
            gg0 = VEC_LOAD_2D_EPI8(i + 4, j - 1, vX);
            gg1 = VEC_LEFT1_EPI8(gg0, g2);
            gg0 = VEC_LEFT2_EPI8(gg0, g2);
            gg4 = VEC_LOAD_2D_EPI8(i + 4, j + 1, vX);
            gg3 = VEC_RIGHT1_EPI8(g2, gg4);
            gg4 = VEC_RIGHT2_EPI8(g2, gg4);
        
            h2 = VEC_LOAD_2D_EPI8(i + 5, j, vX);
            hh0 = VEC_LOAD_2D_EPI8(i + 5, j - 1, vX);
            hh1 = VEC_LEFT1_EPI8(hh0, h2);
            hh0 = VEC_LEFT2_EPI8(hh0, h2);
            hh4 = VEC_LOAD_2D_EPI8(i + 5, j + 1, vX);
            hh3 = VEC_RIGHT1_EPI8(h2, hh4);
            hh4 = VEC_RIGHT2_EPI8(h2, hh4);

            i2 = VEC_LOAD_2D_EPI8(i + 6, j, vX);
            ii0 = VEC_LOAD_2D_EPI8(i + 6, j - 1, vX);
            ii1 = VEC_LEFT1_EPI8(ii0, i2);
            ii0 = VEC_LEFT2_EPI8(ii0, i2);
            ii4 = VEC_LOAD_2D_EPI8(i + 6, j + 1, vX);
            ii3 = VEC_RIGHT1_EPI8(i2, ii4);
            ii4 = VEC_RIGHT2_EPI8(i2, ii4);

            y0 = VEC_OR_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);
            y1 = VEC_OR_25_EPI8(bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4);
            y2 = VEC_OR_25_EPI8(cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4);
            y3 = VEC_OR_25_EPI8(dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4, hh0, hh1, h2, hh3, hh4);
            y4 = VEC_OR_25_EPI8(ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4, hh0, hh1, h2, hh3, hh4, ii0, ii1, i2, ii3, ii4);

            VEC_STORE_2D_EPI8(y0, i, j, vY);
            VEC_STORE_2D_EPI8(y1, i + 1, j, vY);
            VEC_STORE_2D_EPI8(y2, i + 2, j, vY);
            VEC_STORE_2D_EPI8(y3, i + 3, j, vY);
            VEC_STORE_2D_EPI8(y4, i + 4, j, vY);
            
            //rotation de variable

            aa0 = ff0; aa1 = ff1; a2 = f2; aa3 = ff3; aa4 = ff4;
            bb0 = gg0; bb1 = gg1; b2 = g2; bb3 = gg3; bb4 = gg4;
            cc0 = hh0; cc1 = hh1; c2 = h2; cc3 = hh3; cc4 = hh4;
            dd0 = ii0; dd1 = ii1; d2 = i2; dd3 = ii3; dd4 = ii4;
            
        }
    }
}

void morpho_3_SIMD(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    int bord = 1;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;

    vuint8 ** tmp1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
    vuint8 ** tmp2 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);  

    erosion_3_SIMD(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    dilatation_3_SIMD(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    dilatation_3_SIMD(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);
    erosion_3_SIMD(tmp1, vY, vmi0, vmi1, vmj0, vmj1);

}

void morpho_3_SIMD_opti(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    int bord = 1;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;

    vuint8 ** tmp1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
    vuint8 ** tmp2 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);  

    erosion_3_SIMD_opti(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    dilatation_3_SIMD_opti(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    dilatation_3_SIMD_opti(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);
    erosion_3_SIMD_opti(tmp1, vY, vmi0, vmi1, vmj0, vmj1);

}

void morpho_5_SIMD(vuint8 **vX, vuint8 **vY, int vmi0, int vmi1, int vmj0, int vmj1)
{
    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;

    vuint8 ** tmp1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
    vuint8 ** tmp2 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

    erosion_5_SIMD(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    dilatation_5_SIMD(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    dilatation_5_SIMD(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);
    erosion_5_SIMD(tmp1, vY, vmi0, vmi1, vmj0, vmj1);
    
}

void morpho_5_SIMD_opti(vuint8 **vX, vuint8 **vY, int vmi0, int vmi1, int vmj0, int vmj1)
{
    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;

    vuint8 ** tmp1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
    vuint8 ** tmp2 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

    erosion_5_SIMD_opti(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    dilatation_5_SIMD_opti(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    dilatation_5_SIMD_opti(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);
    erosion_5_SIMD_opti(tmp1, vY, vmi0, vmi1, vmj0, vmj1);
    
}