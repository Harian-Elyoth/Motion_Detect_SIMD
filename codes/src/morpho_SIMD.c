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

    int r;

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

        r = vmi1 % k;

        for(i = vmi0 ; i <= vmi1 - r - 1; i = i + k){


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

        switch(r) {
            case 1 :
                c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);
                cc0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
                cc0 = VEC_LEFT1_EPI8(cc0, c1);
                cc2 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                cc2 = VEC_RIGHT1_EPI8(c1, cc2);

                y0 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
                VEC_STORE_2D_EPI8(y0, vmi1, j, vY);
                break;

            case 2 :

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

                y0 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
                y1 = VEC_AND_9_EPI8(bb0, b1, bb2, cc0, c1, cc2, dd0, d1, dd2);

                VEC_STORE_2D_EPI8(y0, vmi1 - 1, j, vY);
                VEC_STORE_2D_EPI8(y1, vmi1, j, vY);
                break;

            default :
                break;
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

        for(i = vmi0 ; i <= vmi1 - r - 1; i = i + k){
            
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
        switch(r) {
            case 1 :
                e2 = VEC_LOAD_2D_EPI8(i + 2, j, vX);
                ee0 = VEC_LOAD_2D_EPI8(i + 2, j - 1, vX);
                ee1 = VEC_LEFT1_EPI8(ee0, e2);
                ee0 = VEC_LEFT2_EPI8(ee0, e2);
                ee4 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee3 = VEC_RIGHT1_EPI8(e2, ee4);
                ee4 = VEC_RIGHT2_EPI8(e2, ee4);

                y0 = VEC_AND_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);

                VEC_STORE_2D_EPI8(y0, vmi1, j, vY);
                break;

            case 2 :

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

                y0 = VEC_AND_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);
                y1 = VEC_AND_25_EPI8(bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4);

                VEC_STORE_2D_EPI8(y0, vmi1 - 1, j, vY);
                VEC_STORE_2D_EPI8(y1, vmi1, j, vY);
                break;
            
            case 3 :

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

                y0 = VEC_AND_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);
                y1 = VEC_AND_25_EPI8(bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4);
                y2 = VEC_AND_25_EPI8(cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4);

                VEC_STORE_2D_EPI8(y0, vmi1 - 2, j, vY);
                VEC_STORE_2D_EPI8(y1, vmi1 - 1, j, vY);
                VEC_STORE_2D_EPI8(y2, vmi1 - 0, j, vY);
                break;

            case 4 :

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

                y0 = VEC_AND_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);
                y1 = VEC_AND_25_EPI8(bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4);
                y2 = VEC_AND_25_EPI8(cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4);
                y3 = VEC_AND_25_EPI8(dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4, hh0, hh1, h2, hh3, hh4);

                VEC_STORE_2D_EPI8(y0, vmi1 - 3, j, vY);
                VEC_STORE_2D_EPI8(y1, vmi1 - 2, j, vY);
                VEC_STORE_2D_EPI8(y2, vmi1 - 1, j, vY);
                VEC_STORE_2D_EPI8(y3, vmi1 - 0, j, vY);

                break;

            default :
                break;
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

        for(i = vmi0 ; i <= vmi1 - r - 1; i = i + 3){


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
        switch(r) {
            case 1 :

                c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);
                cc0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
                cc0 = VEC_LEFT1_EPI8(cc0, c1);
                cc2 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                cc2 = VEC_RIGHT1_EPI8(c1, cc2);

                y0 = VEC_OR_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
                VEC_STORE_2D_EPI8(y0, vmi1, j, vY);
                break;

            case 2 :

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

                y0 = VEC_OR_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
                y1 = VEC_OR_9_EPI8(bb0, b1, bb2, cc0, c1, cc2, dd0, d1, dd2);

                VEC_STORE_2D_EPI8(y0, vmi1 - 1, j, vY);
                VEC_STORE_2D_EPI8(y1, vmi1, j, vY);

            default :
                break;
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


        for(i = vmi0 ; i <= vmi1 - r - 1; i = i + k){

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

        switch(r) {
            case 1 :
                e2 = VEC_LOAD_2D_EPI8(i + 2, j, vX);
                ee0 = VEC_LOAD_2D_EPI8(i + 2, j - 1, vX);
                ee1 = VEC_LEFT1_EPI8(ee0, e2);
                ee0 = VEC_LEFT2_EPI8(ee0, e2);
                ee4 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee3 = VEC_RIGHT1_EPI8(e2, ee4);
                ee4 = VEC_RIGHT2_EPI8(e2, ee4);

                y0 = VEC_OR_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);

                VEC_STORE_2D_EPI8(y0, vmi1, j, vY);
                break;

            case 2 :

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

                y0 = VEC_OR_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);
                y1 = VEC_OR_25_EPI8(bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4);

                VEC_STORE_2D_EPI8(y0, vmi1 - 1, j, vY);
                VEC_STORE_2D_EPI8(y1, vmi1, j, vY);
                break;
            
            case 3 :

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

                y0 = VEC_OR_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);
                y1 = VEC_OR_25_EPI8(bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4);
                y2 = VEC_OR_25_EPI8(cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4);

                VEC_STORE_2D_EPI8(y0, vmi1 - 2, j, vY);
                VEC_STORE_2D_EPI8(y1, vmi1 - 1, j, vY);
                VEC_STORE_2D_EPI8(y2, vmi1 - 0, j, vY);
                break;

            case 4 :

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

                y0 = VEC_OR_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);
                y1 = VEC_OR_25_EPI8(bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4);
                y2 = VEC_OR_25_EPI8(cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4);
                y3 = VEC_OR_25_EPI8(dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4, ff0, ff1, f2, ff3, ff4, gg0, gg1, g2, gg3, gg4, hh0, hh1, h2, hh3, hh4);

                VEC_STORE_2D_EPI8(y0, vmi1 - 3, j, vY);
                VEC_STORE_2D_EPI8(y1, vmi1 - 2, j, vY);
                VEC_STORE_2D_EPI8(y2, vmi1 - 1, j, vY);
                VEC_STORE_2D_EPI8(y3, vmi1 - 0, j, vY);

                break;

            default :
                break;
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

void morpho_5_SIMD(vuint8 **vX, vuint8 **vY, int vmi0, int vmi1, int vmj0, int vmj1){
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

void morpho_5_SIMD_opti(vuint8 **vX, vuint8 **vY, int vmi0, int vmi1, int vmj0, int vmj1){
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

void morpho_3_SIMD_pipeline(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {

    printf("vmi0 = %d, vmi1 = %d, vmj0 = %d, vmj1 = %d\n", vmi0, vmi1, vmj0, vmj1);

    // TEST SUR LA TAILLE DES MATRICES
    // il faut au 4 colonnes SIMD
    // width >= 16 * 4 = 64

    int i, j;

    // PROLOGUE GLOBAL

    // erosion sur les 3 premiere colonnes et toutes les lignes
    erosion_3_SIMD(vX, vY, vmi0, vmi1, vmj0, vmj0 + 2);

    // dilation sur les 2 premieres colonnes et toutes les lignes
    dilatation_3_SIMD(vX, vY, vmi0, vmi1, vmj0, vmj0 + 1);

    // dilation sur la 1ere colonnes et toutes les lignes
    dilatation_3_SIMD(vX, vY, vmi0, vmi1, vmj0, vmj0);

    // aa, bb : non alignés
    // a, b   : alignés

    // colonne 0
    vuint8 aa0, a1, aa2; 
    vuint8 bb0, b1, bb2;
    vuint8 cc0, c1, cc2; 

    // colonne 1
    vuint8 cc3, c4, cc5; 
    vuint8 dd3, d4, dd5;
    vuint8 ee3, e4, ee5;

    // colonne 2
    vuint8 ee6, e7, ee8;
    vuint8 ff6, f7, ff8;
    vuint8 gg6, g7, gg8;

    // colonne 3
    vuint8 gg9, g10, gg11;
    vuint8 hh9, h10, hh11;
    vuint8 ii9, i10, ii11;

    // vecteur de sorties
    vuint8 y0, y1, y2, y3;

    // on commence a j egale 3 (ie : la 4e colonne)
    for (j = vmj0 + 3; j <= vmj1; ++j)
    {   
        printf("j = %d\n", j);

        // PROLOGUE BOUCLE INTERNE

        // erosion sur la 4e colonne et les 6 premières lignes
        erosion_3_SIMD(vX, vY, vmi0, vmi0 + 5, vmj0 + 3, vmj0 + 3);

        // dilation sur la 3e colonnes et les 4 premieres lignes
        dilatation_3_SIMD(vX, vY, vmi0, vmi0 + 3, vmj0 + 2, vmj0 + 2);

        // dilation sur la 2e colonnes et les 2 premieres lignes
        dilatation_3_SIMD(vX, vY, vmi0, vmi0 + 1, vmj0 + 1, vmj0 + 1);

        // on commence a i = 6 (ie : la 6e ligne)
        i = vmi0 + 6;

        // on considère i et j pour la ligne jaune (4e colonne, 6e ligne)

        // ligne de bord, autour de la colonne 0
        aa0 = VEC_LOAD_2D_EPI8(i - 7, j - 4, vX);
        a1  = VEC_LOAD_2D_EPI8(i - 7, j - 3, vX);
        aa0 = VEC_LEFT1_EPI8(aa0, a1);
        aa2 = VEC_LOAD_2D_EPI8(i - 7, j - 2, vX);
        aa2 = VEC_RIGHT1_EPI8(a1, aa2);

        // ligne 0, autour de la colonne 0
        bb0 = VEC_LOAD_2D_EPI8(i - 6, j - 4, vX);
        b1  = VEC_LOAD_2D_EPI8(i - 6, j - 3, vX);
        bb0 = VEC_LEFT1_EPI8(bb0, b1);
        bb2 = VEC_LOAD_2D_EPI8(i - 6, j - 2, vX);
        bb2 = VEC_RIGHT1_EPI8(b1, bb2);

        // ligne 1, autour de la colonne 1
        cc3 = VEC_LOAD_2D_EPI8(i - 5, j - 3, vX);
        c4  = VEC_LOAD_2D_EPI8(i - 5, j - 2, vX);
        cc3 = VEC_LEFT1_EPI8(cc3, c4);
        cc5 = VEC_LOAD_2D_EPI8(i - 5, j - 1, vX);
        cc5 = VEC_RIGHT1_EPI8(c4, cc5);

        // ligne 2, autour de la colonne 1
        dd3 = VEC_LOAD_2D_EPI8(i - 4, j - 3, vX);
        d4  = VEC_LOAD_2D_EPI8(i - 4, j - 2, vX);
        dd3 = VEC_LEFT1_EPI8(dd3, d4);
        dd5 = VEC_LOAD_2D_EPI8(i - 4, j - 1, vX);
        dd5 = VEC_RIGHT1_EPI8(d4, dd5); 

        // ligne 3, autour de la colonne 2
        ee6 = VEC_LOAD_2D_EPI8(i - 3, j - 2, vX);
        e7  = VEC_LOAD_2D_EPI8(i - 3, j - 1, vX);
        ee6 = VEC_LEFT1_EPI8(ee6, e7);
        ee8 = VEC_LOAD_2D_EPI8(i - 3, j - 0, vX);
        ee8 = VEC_RIGHT1_EPI8(e7, ee8);

        // ligne 4, autour de la colonne 2
        ff6 = VEC_LOAD_2D_EPI8(i - 2, j - 2, vX);
        f7  = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        ff6 = VEC_LEFT1_EPI8(ff6, f7);
        ff8 = VEC_LOAD_2D_EPI8(i - 2, j - 0, vX);
        ff8 = VEC_RIGHT1_EPI8(f7, ff8);

        // ligne 5, autour de la colonne 3
        gg9  = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        g10  = VEC_LOAD_2D_EPI8(i - 1, j - 0, vX);
        gg9  = VEC_LEFT1_EPI8(gg9, g10);
        gg11 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
        gg11 = VEC_RIGHT1_EPI8(g10, gg11);

        // ligne 6, autour de la colonne 3
        hh9  = VEC_LOAD_2D_EPI8(i - 0, j - 1, vX);
        h10  = VEC_LOAD_2D_EPI8(i - 0, j - 0, vX);
        hh9  = VEC_LEFT1_EPI8(hh9, h10);
        hh11 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
        hh11 = VEC_RIGHT1_EPI8(h10, hh11);

        for (i = vmi0 + 6; i <= vmi1; ++i)
        {

            printf("i = %d\n", i);

            // ligne 1, autour de la colonne 0
            cc0 = VEC_LOAD_2D_EPI8(i - 5, j - 4, vX);
            c1  = VEC_LOAD_2D_EPI8(i - 5, j - 3, vX);
            cc0 = VEC_LEFT1_EPI8(cc0, c1);
            cc2 = VEC_LOAD_2D_EPI8(i - 5, j - 2, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, cc2);

            // ligne 3, autour de la colonne 1
            ee3 = VEC_LOAD_2D_EPI8(i - 3, j - 3, vX);
            e4  = VEC_LOAD_2D_EPI8(i - 3, j - 2, vX);
            ee3 = VEC_LEFT1_EPI8(ee3, e4);
            ee5 = VEC_LOAD_2D_EPI8(i - 3, j - 1, vX);
            ee5 = VEC_RIGHT1_EPI8(e4, ee5); 

            // ligne 5, autour de la colonne 2
            gg6 = VEC_LOAD_2D_EPI8(i - 1, j - 2, vX);
            g7  = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
            gg6 = VEC_LEFT1_EPI8(gg6, g7);
            gg8 = VEC_LOAD_2D_EPI8(i - 1, j - 0, vX);
            gg8 = VEC_RIGHT1_EPI8(g7, gg8);

            // ligne 7, autour de la colonne 3
            ii9  = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
            i10  = VEC_LOAD_2D_EPI8(i + 1, j - 0, vX);
            ii9  = VEC_LEFT1_EPI8(hh9, h10);
            ii11 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            ii11 = VEC_RIGHT1_EPI8(h10, hh11);

            // TRAITEMENT

            // 1ere erosion 3 : ligne jaune : ligne 6, colonne 3
            y0 = VEC_AND_9_EPI8(gg9, g10, gg11, hh9, h10, hh11, ii9, i10, ii11);
            VEC_STORE_2D_EPI8(y0, i, j, vY);

            // 1ere dilatation 3 : ligne verte : ligne 4, colonne 2
            y1 = VEC_OR_9_EPI8(ee6, e7, ee8, ff6, f7, ff8, gg6, g7, gg8);
            VEC_STORE_2D_EPI8(y1, i - 2, j - 1, vY);

            // 2e dilation 3 : ligne bleue : ligne 2, colonne 1
            y2 = VEC_OR_9_EPI8(cc3, c4, cc5, dd3, d4, dd5, ee3, e4, ee5);
            VEC_STORE_2D_EPI8(y2, i - 4, j - 2, vY);            

            // 2e erosion 3 : ligne rouge : ligne 0, colonne 0
            y3 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
            VEC_STORE_2D_EPI8(y3, i - 6, j - 3, vY);

            // ROTATION DE REGISTRE  

            // colonne 0   
            aa0 = bb0 ; bb0 = cc0 ;
            a1  = b1  ; b1  = c1  ;
            aa2 = bb2 ; bb2 = cc2 ;

            // colonne 1
            cc3 = dd3 ; dd3 = ee3 ; 
            c4  = d4  ; d4  = e4  ;
            cc5 = dd5 ; dd5 = ee5  ;

            // colonne 2
            ee6 = ff6 ; ff6 = gg6 ;
            e7  = f7  ; f7  = g7  ;
            ee8 = ff8 ; ff8 = gg8 ;

            // colonne 3
            gg9  = hh9  ; hh9  = ii9 ;
            g10  = h10  ; h10  = i10 ;
            gg11 = hh11 ; hh11 = ii11;

            // EPILOGUE BOUCLE INTERNE
        }
    }

    // EPILOGUE GLOBAL

    // dilation sur les 2 derniere ligne de l'avant derniere colonnes
    dilatation_3_SIMD(vX, vY, vmi1 - 2, vmi1, vmj1 - 1, vmj1 - 1);

    // dilation sur toutes lignes de la derniere colonnes
    dilatation_3_SIMD(vX, vY, vmi0, vmi1, vmj1, vmj1);

    // dilation sur la 4 dernières lignes de l'avant avant derniere colonne
    dilatation_3_SIMD(vX, vY, vmi1 - 4, vmi1, vmj1 - 2, vmj1 - 2);

    // dilation sur toutes les lignes des deux dernieres colonne
    dilatation_3_SIMD(vX, vY, vmi0, vmi1, vmj1 - 1, vmj1);

    // dilation sur la 4 dernières lignes de l'avant avant avant derniere colonne
    erosion_3_SIMD(vX, vY, vmi1 - 6, vmi1, vmj1 - 3, vmj1 - 3);

    // erosion sur sur toutes les lignes des trois dernieres colonne
    erosion_3_SIMD(vX, vY, vmi0, vmi1, vmj1 - 2, vmj1);
}










