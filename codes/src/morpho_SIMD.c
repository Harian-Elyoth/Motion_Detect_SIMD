/*
==================================================================
==================== Morpho Mathematiques en SIMD ================
==================================================================
*/

//=============== INCLUDES ===============================

#include "morpho_SIMD.h"



void erosion_3_SIMD(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1){

    //vecteur aligné
    vuint8 a0, b0, c0, a1, b1, c1, a2, b2, c2;
    //vecteur non-alignés
    vuint8 aa0, aa2, bb0, bb2, cc0, cc2;
    //vecteur sortie
    vuint8 y;

    int i, j;


    for(i = vmi0 ; i <= vmi1 ; i++){

        j = vmj0;

        a0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);  a1 = VEC_LOAD_2D_EPI8(i - 1, j, vX);   

        b0 = VEC_LOAD_2D_EPI8(i + 0, j - 1, vX);  b1 = VEC_LOAD_2D_EPI8(i + 0, j, vX);

        c0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);  c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);
        

        for(j = vmj0 ; j <= vmj1 ; j++){
            //printf("indice i, j : %d, %d\n", i, j);
            aa0 = VEC_LEFT1_EPI8(a0, a1);
            a2 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            aa2 = VEC_RIGHT1_EPI8(a1, a2);
    
            bb0 = VEC_LEFT1_EPI8(b0, b1);
            b2 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
            bb2 = VEC_RIGHT1_EPI8(b1, b2);

            cc0 = VEC_LEFT1_EPI8(c0, c1);
            c2 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, c2);

            y = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);

            VEC_STORE_2D_EPI8(y, i, j, vY);
            
            //rotation de variable
            a0 = a1; b0 = b1; c0 = c1;
            a1 = a2; b1 = b2; c1 = c2;

        }
    }
}

void erosion_3_SIMD_opti(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1){

    //vecteur aligné
    vuint8 a0, a1, a4, a7, a10;
    vuint8 b0, b1, b4, b7, b10;
    vuint8 c0, c1, c4, c7, c10;
    //vecteur non-alignés
    vuint8 aa0, aa2, aa3, aa5, aa6, aa8;
    vuint8 bb0, bb2, bb3, bb5, bb6, bb8;
    vuint8 cc0, cc2, cc3, cc5, cc6, cc8;
    //vecteur sortie
    vuint8 y0, y1, y2;

    int i, j;

    int k = 3;

    int r = (vmj1 + 1) % k;

    for(i = vmi0 ; i <= vmi1 ; i++){

        j = vmj0;

        a0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX) ; a1 = VEC_LOAD_2D_EPI8(i - 1, j, vX);   

        b0 = VEC_LOAD_2D_EPI8(i + 0, j - 1, vX) ; b1 = VEC_LOAD_2D_EPI8(i + 0, j, vX);

        c0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX) ; c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);


        for(j = vmj0 ; j <= vmj1 - r; j = j + k){

            aa0 = VEC_LEFT1_EPI8(a0, a1);
            a4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            aa2 = VEC_RIGHT1_EPI8(a1, a4);
            aa3 = VEC_LEFT1_EPI8(a1, a4);
            a7 = VEC_LOAD_2D_EPI8(i - 1, j + 2, vX);
            aa5 = VEC_RIGHT1_EPI8(a4, a7);
            aa6 = VEC_LEFT1_EPI8(a4, a7);
            a10 = VEC_LOAD_2D_EPI8(i - 1, j + 3, vX);
            aa8 = VEC_RIGHT1_EPI8(a7, a10);

            bb0 = VEC_LEFT1_EPI8(b0, b1);
            b4 = VEC_LOAD_2D_EPI8(i + 0, j + 1, vX);
            bb2 = VEC_RIGHT1_EPI8(b1, b4);
            bb3 = VEC_LEFT1_EPI8(b1, b4);
            b7 = VEC_LOAD_2D_EPI8(i + 0, j + 2, vX);
            bb5 = VEC_RIGHT1_EPI8(b4, b7);
            bb6 = VEC_LEFT1_EPI8(b4, b7);
            b10 = VEC_LOAD_2D_EPI8(i + 0, j + 3, vX);
            bb8 = VEC_RIGHT1_EPI8(b7, b10);

            cc0 = VEC_LEFT1_EPI8(c0, c1);
            c4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, c4);
            cc3 = VEC_LEFT1_EPI8(c1, c4);
            c7 = VEC_LOAD_2D_EPI8(i + 1, j + 2, vX);
            cc5 = VEC_RIGHT1_EPI8(c4, c7);
            cc6 = VEC_LEFT1_EPI8(c4, c7);
            c10 = VEC_LOAD_2D_EPI8(i + 1, j + 3, vX);
            cc8 = VEC_RIGHT1_EPI8(c7, c10);


            y0 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
            y1 = VEC_AND_9_EPI8(aa3, a4, aa5, bb3, b4, bb5, cc3, c4, cc5);
            y2 = VEC_AND_9_EPI8(aa6, a7, aa8, bb6, b7, bb8, cc6, c7, cc8);

            VEC_STORE_2D_EPI8(y0, i, j + 0, vY);
            VEC_STORE_2D_EPI8(y1, i, j + 1, vY);
            VEC_STORE_2D_EPI8(y2, i, j + 2, vY);
            //rotation de variable
            a0 = a7 ; b0 = b7 ; c0 = c7;
            a1 = a10; b1 = b10; c1 = c10;


        }

        switch(r) {
            case 1 :
                printf("Je fais la derniere colonne\n");
                aa0 = VEC_LEFT1_EPI8(a0, a1);
                a4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                aa2 = VEC_RIGHT1_EPI8(a1, a4);

                bb0 = VEC_LEFT1_EPI8(b0, b1);
                b4 = VEC_LOAD_2D_EPI8(i + 0, j + 1, vX);
                bb2 = VEC_RIGHT1_EPI8(b1, b4);

                cc0 = VEC_LEFT1_EPI8(c0, c1);
                c4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                cc2 = VEC_RIGHT1_EPI8(c1, c4);

                y0 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);

                VEC_STORE_2D_EPI8(y0, i, vmj1, vY);
                break;

            case 2 :
                aa0 = VEC_LEFT1_EPI8(a0, a1);
                a4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                aa2 = VEC_RIGHT1_EPI8(a1, a4);
                aa3 = VEC_LEFT1_EPI8(a1, a4);
                a7 = VEC_LOAD_2D_EPI8(i - 1, j + 2, vX);
                aa5 = VEC_RIGHT1_EPI8(a4, a7);

                bb0 = VEC_LEFT1_EPI8(b0, b1);
                b4 = VEC_LOAD_2D_EPI8(i + 0, j + 1, vX);
                bb2 = VEC_RIGHT1_EPI8(b1, b4);
                bb3 = VEC_LEFT1_EPI8(b1, b4);
                b7 = VEC_LOAD_2D_EPI8(i + 0, j + 2, vX);
                bb5 = VEC_RIGHT1_EPI8(b4, b7);

                cc0 = VEC_LEFT1_EPI8(c0, c1);
                c4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                cc2 = VEC_RIGHT1_EPI8(c1, c4);
                cc3 = VEC_LEFT1_EPI8(c1, c4);
                c7 = VEC_LOAD_2D_EPI8(i + 1, j + 2, vX);
                cc5 = VEC_RIGHT1_EPI8(c4, c7);

                y0 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
                y1 = VEC_AND_9_EPI8(aa3, a4, aa5, bb3, b4, bb5, cc3, c4, cc5);

                VEC_STORE_2D_EPI8(y0, i, vmj1 - 1, vY);
                VEC_STORE_2D_EPI8(y1, i, vmj1, vY);
                break;

            default :
                break;
        }

    }
}

void erosion_5_SIMD(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    //vecteur aligné
    vuint8 a0, a2, a4;
    vuint8 b0, b2, b4;
    vuint8 c0, c2, c4;
    vuint8 d0, d2, d4;
    vuint8 e0, e2, e4;
    //vecteur non-alignés
    vuint8 aa0, aa1, aa3, aa4;
    vuint8 bb0, bb1, bb3, bb4;
    vuint8 cc0, cc1, cc3, cc4;
    vuint8 dd0, dd1, dd3, dd4;
    vuint8 ee0, ee1, ee3, ee4;
    //vecteur sortie
    vuint8 y;

    int i, j;


    for(i = vmi0 ; i <= vmi1 ; i++){

        j = vmj0;

        a0 = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        a2 = VEC_LOAD_2D_EPI8(i - 2, j, vX);   

        b0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        b2 = VEC_LOAD_2D_EPI8(i - 1, j, vX);

        c0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        c2 = VEC_LOAD_2D_EPI8(i, j, vX);

        d0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        d2 = VEC_LOAD_2D_EPI8(i + 1, j, vX);

        e0 = VEC_LOAD_2D_EPI8(i + 2, j- 1, vX);
        e2 = VEC_LOAD_2D_EPI8(i + 2, j, vX);
        

        for(j = vmj0 ; j <= vmj1 ; j++){
            //printf("indice i, j : %d, %d\n", i, j);
            aa0 = VEC_LEFT2_EPI8(a0, a2);
            aa1 = VEC_LEFT1_EPI8(a0, a2);
            a4 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
            aa3 = VEC_RIGHT1_EPI8(a2, a4);
            aa4 = VEC_RIGHT2_EPI8(a2, a4);
    
            bb0 = VEC_LEFT2_EPI8(b0, b2);
            bb1 = VEC_LEFT1_EPI8(b0, b2);
            b4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            bb3 = VEC_RIGHT1_EPI8(b2, b4);
            bb4 = VEC_RIGHT2_EPI8(b2, b4);

            cc0 = VEC_LEFT2_EPI8(c0, c2);
            cc1 = VEC_LEFT1_EPI8(c0, c2);
            c4 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
            cc3 = VEC_RIGHT1_EPI8(c2, c4);
            cc4 = VEC_RIGHT2_EPI8(c2, c4);

            dd0 = VEC_LEFT2_EPI8(d0, d2);
            dd1 = VEC_LEFT1_EPI8(d0, d2);
            d4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            dd3 = VEC_RIGHT1_EPI8(d2, d4);
            dd4 = VEC_RIGHT2_EPI8(d2, d4);

            ee0 = VEC_LEFT2_EPI8(e0, e2);
            ee1 = VEC_LEFT1_EPI8(e0, e2);
            e4 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            ee3 = VEC_RIGHT1_EPI8(e2, e4);
            ee4 = VEC_RIGHT2_EPI8(e2, e4);

            y = VEC_AND_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);

            VEC_STORE_2D_EPI8(y, i, j, vY);
            
            //rotation de variable
            a0 = a2; b0 = b2; c0 = c2; d0 = d2; e0 = e2;
            a2 = a4; b2 = b4; c2 = c4; d2 = d4; e2 = e4;

        }
    }
}

void erosion_5_SIMD_opti(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    //vecteur aligné
    vuint8 a0, a3, a8, a13, a18, a23, a26;
    vuint8 b0, b3, b8, b13, b18, b23, b26;
    vuint8 c0, c3, c8, c13, c18, c23, c26;
    vuint8 d0, d3, d8, d13, d18, d23, d26;
    vuint8 e0, e3, e8, e13, e18, e23, e26;
    //vecteur non-alignés
    vuint8 aa1, aa2, aa4, aa5, aa6, aa7, aa9, aa10, aa11, aa12, aa14, aa15, aa16, aa17, aa19, aa20, aa21, aa22, aa24, aa25;
    vuint8 bb1, bb2, bb4, bb5, bb6, bb7, bb9, bb10, bb11, bb12, bb14, bb15, bb16, bb17, bb19, bb20, bb21, bb22, bb24, bb25;
    vuint8 cc1, cc2, cc4, cc5, cc6, cc7, cc9, cc10, cc11, cc12, cc14, cc15, cc16, cc17, cc19, cc20, cc21, cc22, cc24, cc25;
    vuint8 dd1, dd2, dd4, dd5, dd6, dd7, dd9, dd10, dd11, dd12, dd14, dd15, dd16, dd17, dd19, dd20, dd21, dd22, dd24, dd25;
    vuint8 ee1, ee2, ee4, ee5, ee6, ee7, ee9, ee10, ee11, ee12, ee14, ee15, ee16, ee17, ee19, ee20, ee21, ee22, ee24, ee25;
    //vecteur sortie
    vuint8 y0, y1, y2, y3, y4;

    int k = 5;

    int r = (vmj1 + 1)% k;

    int i, j;

    int bord = 2;
    
    printf("r = %d\n", r);
    for(i = vmi0 ; i <= vmi1 ; i++){
        j = vmj0;
        
        a0 = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        a3 = VEC_LOAD_2D_EPI8(i - 2, j, vX);   

        b0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        b3 = VEC_LOAD_2D_EPI8(i - 1, j, vX);

        c0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        c3 = VEC_LOAD_2D_EPI8(i, j, vX);

        d0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        d3 = VEC_LOAD_2D_EPI8(i + 1, j, vX);

        e0 = VEC_LOAD_2D_EPI8(i + 2, j- 1, vX);
        e3 = VEC_LOAD_2D_EPI8(i + 2, j, vX);


        for(j = vmj0 ; j <= vmj1 - r; j = j + k){
            
            aa1 = VEC_LEFT2_EPI8(a0, a3);
            aa2 = VEC_LEFT1_EPI8(a0, a3);

            a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
            aa4 = VEC_RIGHT1_EPI8(a3, a8);
            aa5 = VEC_RIGHT2_EPI8(a3, a8);

            aa6 = VEC_LEFT2_EPI8(a3, a8);
            aa7 = VEC_LEFT1_EPI8(a3, a8);

            a13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
            aa9 = VEC_RIGHT1_EPI8(a8, a13);
            aa10 = VEC_RIGHT2_EPI8(a8, a13);

            aa11 = VEC_LEFT2_EPI8(a8, a13);
            aa12 = VEC_LEFT1_EPI8(a8, a13);

            a18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
            aa14 = VEC_RIGHT1_EPI8(a13, a18);
            aa15 = VEC_RIGHT2_EPI8(a13, a18);

            aa16 = VEC_LEFT2_EPI8(a13, a18);
            aa17 = VEC_LEFT1_EPI8(a13, a18);

            a23 = VEC_LOAD_2D_EPI8(i - 2, j + 4, vX);
            aa19 = VEC_RIGHT1_EPI8(a18, a23);
            aa20 = VEC_RIGHT2_EPI8(a18, a23);

            aa21 = VEC_LEFT2_EPI8(a18, a23);
            aa22 = VEC_LEFT1_EPI8(a18, a23);

            a26 = VEC_LOAD_2D_EPI8(i - 2, j + 5, vX);
            aa24 = VEC_RIGHT1_EPI8(a23, a26);
            aa25 = VEC_RIGHT2_EPI8(a23, a26);

            bb1 = VEC_LEFT2_EPI8(b0, b3);
            bb2 = VEC_LEFT1_EPI8(b0, b3);

            b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            bb4 = VEC_RIGHT1_EPI8(b3, b8);
            bb5 = VEC_RIGHT2_EPI8(b3, b8);

            bb6 = VEC_LEFT2_EPI8(b3, b8);
            bb7 = VEC_LEFT1_EPI8(b3, b8);

            b13 = VEC_LOAD_2D_EPI8(i - 1, j + 2, vX);
            bb9 = VEC_RIGHT1_EPI8(b8, b13);
            bb10 = VEC_RIGHT2_EPI8(b8, b13);

            bb11 = VEC_LEFT2_EPI8(b8, b13);
            bb12 = VEC_LEFT1_EPI8(b8, b13);

            b18 = VEC_LOAD_2D_EPI8(i - 1,  j + 3, vX);
            bb14 = VEC_RIGHT1_EPI8(b13, b18);
            bb15 = VEC_RIGHT2_EPI8(b13, b18);

            bb16 = VEC_LEFT2_EPI8(b13, b18);
            bb17 = VEC_LEFT1_EPI8(b13, b18);

            b23 = VEC_LOAD_2D_EPI8(i - 1, j + 4, vX);
            bb19 = VEC_RIGHT1_EPI8(b18, b23);
            bb20 = VEC_RIGHT2_EPI8(b18, b23);

            bb21 = VEC_LEFT2_EPI8(b18, b23);
            bb22 = VEC_LEFT1_EPI8(b18, b23);

            b26 = VEC_LOAD_2D_EPI8(i - 1, j + 5, vX);
            bb24 = VEC_RIGHT1_EPI8(b23, b26);
            bb25 = VEC_RIGHT2_EPI8(b23, b26);


            cc1 = VEC_LEFT2_EPI8(c0, c3);
            cc2 = VEC_LEFT1_EPI8(c0, c3);

            c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
            cc4 = VEC_RIGHT1_EPI8(c3, c8);
            cc5 = VEC_RIGHT2_EPI8(c3, c8);

            cc6 = VEC_LEFT2_EPI8(c3, c8);
            cc7 = VEC_LEFT1_EPI8(c3, c8);

            c13 = VEC_LOAD_2D_EPI8(i - 0, j + 2, vX);
            cc9 = VEC_RIGHT1_EPI8(c8, c13);
            cc10 = VEC_RIGHT2_EPI8(c8, c13);

            cc11 = VEC_LEFT2_EPI8(c8, c13);
            cc12 = VEC_LEFT1_EPI8(c8, c13);

            c18 = VEC_LOAD_2D_EPI8(i - 0,  j + 3, vX);
            cc14 = VEC_RIGHT1_EPI8(c13, c18);
            cc15 = VEC_RIGHT2_EPI8(c13, c18);

            cc16 = VEC_LEFT2_EPI8(c13, c18);
            cc17 = VEC_LEFT1_EPI8(c13, c18);

            c23 = VEC_LOAD_2D_EPI8(i - 0, j + 4, vX);
            cc19 = VEC_RIGHT1_EPI8(c18, c23);
            cc20 = VEC_RIGHT2_EPI8(c18, c23);

            cc21 = VEC_LEFT2_EPI8(c18, c23);
            cc22 = VEC_LEFT1_EPI8(c18, c23);

            c26 = VEC_LOAD_2D_EPI8(i - 0, j + 5, vX);
            cc24 = VEC_RIGHT1_EPI8(c23, c26);
            cc25 = VEC_RIGHT2_EPI8(c23, c26);


            dd1 = VEC_LEFT2_EPI8(d0, d3);
            dd2 = VEC_LEFT1_EPI8(d0, d3);

            d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            dd4 = VEC_RIGHT1_EPI8(d3, d8);
            dd5 = VEC_RIGHT2_EPI8(d3, d8);

            dd6 = VEC_LEFT2_EPI8(d3, d8);
            dd7 = VEC_LEFT1_EPI8(d3, d8);

            d13 = VEC_LOAD_2D_EPI8(i + 1, j + 2, vX);
            dd9 = VEC_RIGHT1_EPI8(d8, d13);
            dd10 = VEC_RIGHT2_EPI8(d8, d13);

            dd11 = VEC_LEFT2_EPI8(d8, d13);
            dd12 = VEC_LEFT1_EPI8(d8, d13);

            d18 = VEC_LOAD_2D_EPI8(i + 1,  j + 3, vX);
            dd14 = VEC_RIGHT1_EPI8(d13, d18);
            dd15 = VEC_RIGHT2_EPI8(d13, d18);

            dd16 = VEC_LEFT2_EPI8(d13, d18);
            dd17 = VEC_LEFT1_EPI8(d13, d18);

            d23 = VEC_LOAD_2D_EPI8(i + 1, j + 4, vX);
            dd19 = VEC_RIGHT1_EPI8(d18, d23);
            dd20 = VEC_RIGHT2_EPI8(d18, d23);

            dd21 = VEC_LEFT2_EPI8(d18, d23);
            dd22 = VEC_LEFT1_EPI8(d18, d23);

            d26 = VEC_LOAD_2D_EPI8(i + 1, j + 5, vX);
            dd24 = VEC_RIGHT1_EPI8(d23, d26);
            dd25 = VEC_RIGHT2_EPI8(d23, d26);


            ee1 = VEC_LEFT2_EPI8(e0, e3);
            ee2 = VEC_LEFT1_EPI8(e0, e3);

            e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            ee4 = VEC_RIGHT1_EPI8(e3, e8);
            ee5 = VEC_RIGHT2_EPI8(e3, e8);

            ee6 = VEC_LEFT2_EPI8(e3, e8);
            ee7 = VEC_LEFT1_EPI8(e3, e8);

            e13 = VEC_LOAD_2D_EPI8(i + 2, j + 2, vX);
            ee9 = VEC_RIGHT1_EPI8(e8, e13);
            ee10 = VEC_RIGHT2_EPI8(e8, e13);

            ee11 = VEC_LEFT2_EPI8(e8, e13);
            ee12 = VEC_LEFT1_EPI8(e8, e13);

            e18 = VEC_LOAD_2D_EPI8(i + 2,  j + 3, vX);
            ee14 = VEC_RIGHT1_EPI8(e13, e18);
            ee15 = VEC_RIGHT2_EPI8(e13, e18);

            ee16 = VEC_LEFT2_EPI8(e13, e18);
            ee17 = VEC_LEFT1_EPI8(e13, e18);

            e23 = VEC_LOAD_2D_EPI8(i + 2, j + 4, vX);
            ee19 = VEC_RIGHT1_EPI8(e18, e23);
            ee20 = VEC_RIGHT2_EPI8(e18, e23);

            ee21 = VEC_LEFT2_EPI8(e18, e23);
            ee22 = VEC_LEFT1_EPI8(e18, e23);

            e26 = VEC_LOAD_2D_EPI8(i + 2, j + 5, vX);
            ee24 = VEC_RIGHT1_EPI8(e23, e26);
            ee25 = VEC_RIGHT2_EPI8(e23, e26);

            y0 = VEC_AND_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);
            y1 = VEC_AND_25_EPI8(aa6, aa7, a8, aa9, aa10, bb6, bb7, b8, bb9, bb10, cc6, cc7, c8, cc9, cc10, dd6, dd7, d8, dd9, dd10, ee6, ee7, e8, ee9, ee10);
            y2 = VEC_AND_25_EPI8(aa11, aa12, a13, aa14, aa15, bb11, bb12, b13, bb14, bb15, cc11, cc12, c13, cc14, cc15, dd11, dd12, d13, dd14, dd15, ee11, ee12, e13, ee14, ee15);
            y3 = VEC_AND_25_EPI8(aa16, aa17, a18, aa19, aa20, bb16, bb17, b18, bb19, bb20, cc16, cc17, c18, cc19, cc20, dd16, dd17, d18, dd19, dd20, ee16, ee17, e18, ee19, ee20);
            y4 = VEC_AND_25_EPI8(aa21, aa22, a23, aa24, aa25, bb21, bb22, b23, bb24, bb25, cc21, cc22, c23, cc24, cc25, dd21, dd22, d23, dd24, dd25, ee21, ee22, e23, ee24, ee25);

            VEC_STORE_2D_EPI8(y0, i, j + 0, vY);
            VEC_STORE_2D_EPI8(y0, i, j + 1, vY);
            VEC_STORE_2D_EPI8(y0, i, j + 2, vY);
            VEC_STORE_2D_EPI8(y0, i, j + 3, vY);
            VEC_STORE_2D_EPI8(y0, i, j + 4, vY);
            
        }
        switch(r) {
            case 1 :

                aa1 = VEC_LEFT2_EPI8(a0, a3);
                aa2 = VEC_LEFT1_EPI8(a0, a3);

                a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
                aa4 = VEC_RIGHT1_EPI8(a3, a8);
                aa5 = VEC_RIGHT2_EPI8(a3, a8);

                bb1 = VEC_LEFT2_EPI8(b0, b3);
                bb2 = VEC_LEFT1_EPI8(b0, b3);

                b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                bb4 = VEC_RIGHT1_EPI8(b3, b8);
                bb5 = VEC_RIGHT2_EPI8(b3, b8);

                cc1 = VEC_LEFT2_EPI8(c0, c3);
                cc2 = VEC_LEFT1_EPI8(c0, c3);

                c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
                cc4 = VEC_RIGHT1_EPI8(c3, c8);
                cc5 = VEC_RIGHT2_EPI8(c3, c8);

                dd1 = VEC_LEFT2_EPI8(d0, d3);
                dd2 = VEC_LEFT1_EPI8(d0, d3);

                d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                dd4 = VEC_RIGHT1_EPI8(d3, d8);
                dd5 = VEC_RIGHT2_EPI8(d3, d8);

                ee1 = VEC_LEFT2_EPI8(e0, e3);
                ee2 = VEC_LEFT1_EPI8(e0, e3);

                e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee4 = VEC_RIGHT1_EPI8(e3, e8);
                ee5 = VEC_RIGHT2_EPI8(e3, e8);

                y0 = VEC_AND_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);

                VEC_STORE_2D_EPI8(y0, i, vmj1, vY);
                break;

            case 2 :

                aa1 = VEC_LEFT2_EPI8(a0, a3);
                aa2 = VEC_LEFT1_EPI8(a0, a3);

                a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
                aa4 = VEC_RIGHT1_EPI8(a3, a8);
                aa5 = VEC_RIGHT2_EPI8(a3, a8);

                aa6 = VEC_LEFT2_EPI8(a3, a8);
                aa7 = VEC_LEFT1_EPI8(a3, a8);

                a13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                aa9 = VEC_RIGHT1_EPI8(a8, a13);
                aa10 = VEC_RIGHT2_EPI8(a8, a13);

                bb1 = VEC_LEFT2_EPI8(b0, b3);
                bb2 = VEC_LEFT1_EPI8(b0, b3);

                b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                bb4 = VEC_RIGHT1_EPI8(b3, b8);
                bb5 = VEC_RIGHT2_EPI8(b3, b8);

                bb6 = VEC_LEFT2_EPI8(b3, b8);
                bb7 = VEC_LEFT1_EPI8(b3, b8);

                b13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                bb9 = VEC_RIGHT1_EPI8(b8, b13);
                bb10 = VEC_RIGHT2_EPI8(b8, b13);

                cc1 = VEC_LEFT2_EPI8(c0, c3);
                cc2 = VEC_LEFT1_EPI8(c0, c3);

                c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
                cc4 = VEC_RIGHT1_EPI8(c3, c8);
                cc5 = VEC_RIGHT2_EPI8(c3, c8);

                cc6 = VEC_LEFT2_EPI8(c3, c8);
                cc7 = VEC_LEFT1_EPI8(c3, c8);

                c13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                cc9 = VEC_RIGHT1_EPI8(c8, c13);
                cc10 = VEC_RIGHT2_EPI8(c8, c13);

                dd1 = VEC_LEFT2_EPI8(d0, d3);
                dd2 = VEC_LEFT1_EPI8(d0, d3);

                d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                dd4 = VEC_RIGHT1_EPI8(d3, d8);
                dd5 = VEC_RIGHT2_EPI8(d3, d8);

                dd6 = VEC_LEFT2_EPI8(d3, d8);
                dd7 = VEC_LEFT1_EPI8(d3, d8);

                d13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                dd9 = VEC_RIGHT1_EPI8(d8, d13);
                dd10 = VEC_RIGHT2_EPI8(d8, d13);

                ee1 = VEC_LEFT2_EPI8(e0, e3);
                ee2 = VEC_LEFT1_EPI8(e0, e3);

                e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee4 = VEC_RIGHT1_EPI8(e3, e8);
                ee5 = VEC_RIGHT2_EPI8(e3, e8);

                ee6 = VEC_LEFT2_EPI8(e3, e8);
                ee7 = VEC_LEFT1_EPI8(e3, e8);

                e13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                ee9 = VEC_RIGHT1_EPI8(e8, e13);
                ee10 = VEC_RIGHT2_EPI8(e8, e13);

                y0 = VEC_AND_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);
                y1 = VEC_AND_25_EPI8(aa6, aa7, a8, aa9, aa10, bb6, bb7, b8, bb9, bb10, cc6, cc7, c8, cc9, cc10, dd6, dd7, d8, dd9, dd10, ee6, ee7, e8, ee9, ee10);


                VEC_STORE_2D_EPI8(y0, i, vmj1 - 1, vY);
                VEC_STORE_2D_EPI8(y1, i, vmj1, vY);
                break;
            
            case 3 :

                aa1 = VEC_LEFT2_EPI8(a0, a3);
                aa2 = VEC_LEFT1_EPI8(a0, a3);

                a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
                aa4 = VEC_RIGHT1_EPI8(a3, a8);
                aa5 = VEC_RIGHT2_EPI8(a3, a8);

                aa6 = VEC_LEFT2_EPI8(a3, a8);
                aa7 = VEC_LEFT1_EPI8(a3, a8);

                a13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                aa9 = VEC_RIGHT1_EPI8(a8, a13);
                aa10 = VEC_RIGHT2_EPI8(a8, a13);

                aa11 = VEC_LEFT2_EPI8(a8, a13);
                aa12 = VEC_LEFT1_EPI8(a8, a13);

                a18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                aa14 = VEC_RIGHT1_EPI8(a13, a18);
                aa15 = VEC_RIGHT2_EPI8(a13, a18);

                bb1 = VEC_LEFT2_EPI8(b0, b3);
                bb2 = VEC_LEFT1_EPI8(b0, b3);

                b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                bb4 = VEC_RIGHT1_EPI8(b3, b8);
                bb5 = VEC_RIGHT2_EPI8(b3, b8);

                bb6 = VEC_LEFT2_EPI8(b3, b8);
                bb7 = VEC_LEFT1_EPI8(b3, b8);

                b13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                bb9 = VEC_RIGHT1_EPI8(b8, b13);
                bb10 = VEC_RIGHT2_EPI8(b8, b13);

                bb11 = VEC_LEFT2_EPI8(b8, b13);
                bb12 = VEC_LEFT1_EPI8(b8, b13);

                b18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                bb14 = VEC_RIGHT1_EPI8(b13, b18);
                bb15 = VEC_RIGHT2_EPI8(b13, b18);

                cc1 = VEC_LEFT2_EPI8(c0, c3);
                cc2 = VEC_LEFT1_EPI8(c0, c3);

                c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
                cc4 = VEC_RIGHT1_EPI8(c3, c8);
                cc5 = VEC_RIGHT2_EPI8(c3, c8);

                cc6 = VEC_LEFT2_EPI8(c3, c8);
                cc7 = VEC_LEFT1_EPI8(c3, c8);

                c13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                cc9 = VEC_RIGHT1_EPI8(c8, c13);
                cc10 = VEC_RIGHT2_EPI8(c8, c13);

                cc11 = VEC_LEFT2_EPI8(c8, c13);
                cc12 = VEC_LEFT1_EPI8(c8, c13);

                c18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                cc14 = VEC_RIGHT1_EPI8(c13, c18);
                cc15 = VEC_RIGHT2_EPI8(c13, c18);

                dd1 = VEC_LEFT2_EPI8(d0, d3);
                dd2 = VEC_LEFT1_EPI8(d0, d3);

                d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                dd4 = VEC_RIGHT1_EPI8(d3, d8);
                dd5 = VEC_RIGHT2_EPI8(d3, d8);

                dd6 = VEC_LEFT2_EPI8(d3, d8);
                dd7 = VEC_LEFT1_EPI8(d3, d8);

                d13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                dd9 = VEC_RIGHT1_EPI8(d8, d13);
                dd10 = VEC_RIGHT2_EPI8(d8, d13);

                dd11 = VEC_LEFT2_EPI8(d8, d13);
                dd12 = VEC_LEFT1_EPI8(d8, d13);

                d18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                dd14 = VEC_RIGHT1_EPI8(d13, d18);
                dd15 = VEC_RIGHT2_EPI8(d13, d18);

                ee1 = VEC_LEFT2_EPI8(e0, e3);
                ee2 = VEC_LEFT1_EPI8(e0, e3);

                e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee4 = VEC_RIGHT1_EPI8(e3, e8);
                ee5 = VEC_RIGHT2_EPI8(e3, e8);

                ee6 = VEC_LEFT2_EPI8(e3, e8);
                ee7 = VEC_LEFT1_EPI8(e3, e8);

                e13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                ee9 = VEC_RIGHT1_EPI8(e8, e13);
                ee10 = VEC_RIGHT2_EPI8(e8, e13);

                ee11 = VEC_LEFT2_EPI8(e8, e13);
                ee12 = VEC_LEFT1_EPI8(e8, e13);

                e18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                ee14 = VEC_RIGHT1_EPI8(e13, e18);
                ee15 = VEC_RIGHT2_EPI8(e13, e18);

                y0 = VEC_AND_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);
                y1 = VEC_AND_25_EPI8(aa6, aa7, a8, aa9, aa10, bb6, bb7, b8, bb9, bb10, cc6, cc7, c8, cc9, cc10, dd6, dd7, d8, dd9, dd10, ee6, ee7, e8, ee9, ee10);
                y2 = VEC_AND_25_EPI8(aa11, aa12, a13, aa14, aa15, bb11, bb12, b13, bb14, bb15, cc11, cc12, c13, cc14, cc15, dd11, dd12, d13, dd14, dd15, ee11, ee12, e13, ee14, ee15);


                VEC_STORE_2D_EPI8(y0, i, vmj1 - 2, vY);
                VEC_STORE_2D_EPI8(y1, i, vmj1 - 1, vY);
                VEC_STORE_2D_EPI8(y2, i, vmj1 - 0, vY);

                break;

            case 4 :

                aa1 = VEC_LEFT2_EPI8(a0, a3);
                aa2 = VEC_LEFT1_EPI8(a0, a3);

                a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
                aa4 = VEC_RIGHT1_EPI8(a3, a8);
                aa5 = VEC_RIGHT2_EPI8(a3, a8);

                aa6 = VEC_LEFT2_EPI8(a3, a8);
                aa7 = VEC_LEFT1_EPI8(a3, a8);

                a13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                aa9 = VEC_RIGHT1_EPI8(a8, a13);
                aa10 = VEC_RIGHT2_EPI8(a8, a13);

                aa11 = VEC_LEFT2_EPI8(a8, a13);
                aa12 = VEC_LEFT1_EPI8(a8, a13);

                a18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                aa14 = VEC_RIGHT1_EPI8(a13, a18);
                aa15 = VEC_RIGHT2_EPI8(a13, a18);

                aa16 = VEC_LEFT2_EPI8(a13, a18);
                aa17 = VEC_LEFT1_EPI8(a13, a18);

                a23 = VEC_LOAD_2D_EPI8(i - 2, j + 4, vX);
                aa19 = VEC_RIGHT1_EPI8(a18, a23);
                aa20 = VEC_RIGHT2_EPI8(a18, a23);

                bb1 = VEC_LEFT2_EPI8(b0, b3);
                bb2 = VEC_LEFT1_EPI8(b0, b3);

                b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                bb4 = VEC_RIGHT1_EPI8(b3, b8);
                bb5 = VEC_RIGHT2_EPI8(b3, b8);

                bb6 = VEC_LEFT2_EPI8(b3, b8);
                bb7 = VEC_LEFT1_EPI8(b3, b8);

                b13 = VEC_LOAD_2D_EPI8(i - 1, j + 2, vX);
                bb9 = VEC_RIGHT1_EPI8(b8, b13);
                bb10 = VEC_RIGHT2_EPI8(b8, b13);

                bb11 = VEC_LEFT2_EPI8(b8, b13);
                bb12 = VEC_LEFT1_EPI8(b8, b13);

                b18 = VEC_LOAD_2D_EPI8(i - 1,  j + 3, vX);
                bb14 = VEC_RIGHT1_EPI8(b13, b18);
                bb15 = VEC_RIGHT2_EPI8(b13, b18);

                bb16 = VEC_LEFT2_EPI8(b13, b18);
                bb17 = VEC_LEFT1_EPI8(b13, b18);

                b23 = VEC_LOAD_2D_EPI8(i - 1, j + 4, vX);
                bb19 = VEC_RIGHT1_EPI8(b18, b23);
                bb20 = VEC_RIGHT2_EPI8(b18, b23);

                cc1 = VEC_LEFT2_EPI8(c0, c3);
                cc2 = VEC_LEFT1_EPI8(c0, c3);

                c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
                cc4 = VEC_RIGHT1_EPI8(c3, c8);
                cc5 = VEC_RIGHT2_EPI8(c3, c8);

                cc6 = VEC_LEFT2_EPI8(c3, c8);
                cc7 = VEC_LEFT1_EPI8(c3, c8);

                c13 = VEC_LOAD_2D_EPI8(i - 0, j + 2, vX);
                cc9 = VEC_RIGHT1_EPI8(c8, c13);
                cc10 = VEC_RIGHT2_EPI8(c8, c13);

                cc11 = VEC_LEFT2_EPI8(c8, c13);
                cc12 = VEC_LEFT1_EPI8(c8, c13);

                c18 = VEC_LOAD_2D_EPI8(i - 0,  j + 3, vX);
                cc14 = VEC_RIGHT1_EPI8(c13, c18);
                cc15 = VEC_RIGHT2_EPI8(c13, c18);

                cc16 = VEC_LEFT2_EPI8(c13, c18);
                cc17 = VEC_LEFT1_EPI8(c13, c18);

                c23 = VEC_LOAD_2D_EPI8(i - 0, j + 4, vX);
                cc19 = VEC_RIGHT1_EPI8(c18, c23);
                cc20 = VEC_RIGHT2_EPI8(c18, c23);

                dd1 = VEC_LEFT2_EPI8(d0, d3);
                dd2 = VEC_LEFT1_EPI8(d0, d3);

                d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                dd4 = VEC_RIGHT1_EPI8(d3, d8);
                dd5 = VEC_RIGHT2_EPI8(d3, d8);

                dd6 = VEC_LEFT2_EPI8(d3, d8);
                dd7 = VEC_LEFT1_EPI8(d3, d8);

                d13 = VEC_LOAD_2D_EPI8(i + 1, j + 2, vX);
                dd9 = VEC_RIGHT1_EPI8(d8, d13);
                dd10 = VEC_RIGHT2_EPI8(d8, d13);

                dd11 = VEC_LEFT2_EPI8(d8, d13);
                dd12 = VEC_LEFT1_EPI8(d8, d13);

                d18 = VEC_LOAD_2D_EPI8(i + 1,  j + 3, vX);
                dd14 = VEC_RIGHT1_EPI8(d13, d18);
                dd15 = VEC_RIGHT2_EPI8(d13, d18);

                dd16 = VEC_LEFT2_EPI8(d13, d18);
                dd17 = VEC_LEFT1_EPI8(d13, d18);

                d23 = VEC_LOAD_2D_EPI8(i + 1, j + 4, vX);
                dd19 = VEC_RIGHT1_EPI8(d18, d23);
                dd20 = VEC_RIGHT2_EPI8(d18, d23);

                ee1 = VEC_LEFT2_EPI8(e0, e3);
                ee2 = VEC_LEFT1_EPI8(e0, e3);

                e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee4 = VEC_RIGHT1_EPI8(e3, e8);
                ee5 = VEC_RIGHT2_EPI8(e3, e8);

                ee6 = VEC_LEFT2_EPI8(e3, e8);
                ee7 = VEC_LEFT1_EPI8(e3, e8);

                e13 = VEC_LOAD_2D_EPI8(i + 2, j + 2, vX);
                ee9 = VEC_RIGHT1_EPI8(e8, e13);
                ee10 = VEC_RIGHT2_EPI8(e8, e13);

                ee11 = VEC_LEFT2_EPI8(e8, e13);
                ee12 = VEC_LEFT1_EPI8(e8, e13);

                e18 = VEC_LOAD_2D_EPI8(i + 2,  j + 3, vX);
                ee14 = VEC_RIGHT1_EPI8(e13, e18);
                ee15 = VEC_RIGHT2_EPI8(e13, e18);

                ee16 = VEC_LEFT2_EPI8(e13, e18);
                ee17 = VEC_LEFT1_EPI8(e13, e18);

                e23 = VEC_LOAD_2D_EPI8(i + 2, j + 4, vX);
                ee19 = VEC_RIGHT1_EPI8(e18, e23);
                ee20 = VEC_RIGHT2_EPI8(e18, e23);


                y0 = VEC_AND_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);
                y1 = VEC_AND_25_EPI8(aa6, aa7, a8, aa9, aa10, bb6, bb7, b8, bb9, bb10, cc6, cc7, c8, cc9, cc10, dd6, dd7, d8, dd9, dd10, ee6, ee7, e8, ee9, ee10);
                y2 = VEC_AND_25_EPI8(aa11, aa12, a13, aa14, aa15, bb11, bb12, b13, bb14, bb15, cc11, cc12, c13, cc14, cc15, dd11, dd12, d13, dd14, dd15, ee11, ee12, e13, ee14, ee15);
                y3 = VEC_AND_25_EPI8(aa16, aa17, a18, aa19, aa20, bb16, bb17, b18, bb19, bb20, cc16, cc17, c18, cc19, cc20, dd16, dd17, d18, dd19, dd20, ee16, ee17, e18, ee19, ee20);
    

                VEC_STORE_2D_EPI8(y0, i, vmj1 - 3, vY);
                VEC_STORE_2D_EPI8(y1, i, vmj1 - 2, vY);
                VEC_STORE_2D_EPI8(y2, i, vmj1 - 1, vY);
                VEC_STORE_2D_EPI8(y3, i, vmj1 - 0, vY);

                break;

            default :
                break;
        }
    }
}

void dilatation_3_SIMD(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
        
    //vecteur aligné
    vuint8 a0, b0, c0, a1, b1, c1, a2, b2, c2;
    //vecteur non-alignés
    vuint8 aa0, aa2, bb0, bb2, cc0, cc2;
    //vecteur sortie
    vuint8 y;

    int i, j;


    for(i = vmi0 ; i <= vmi1 ; i++){

        j = vmj0;

        a0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        a1 = VEC_LOAD_2D_EPI8(i - 1, j, vX);   

        b0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        b1 = VEC_LOAD_2D_EPI8(i, j, vX);

        c0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);
        

        for(j = vmj0 ; j <= vmj1 ; j++){
            //printf("indice i, j : %d, %d\n", i, j);
            aa0 = VEC_LEFT1_EPI8(a0, a1);
            a2 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            aa2 = VEC_RIGHT1_EPI8(a1, a2);
    
            bb0 = VEC_LEFT1_EPI8(b0, b1);
            b2 = VEC_LOAD_2D_EPI8(i, j + 1, vX);
            bb2 = VEC_RIGHT1_EPI8(b1, b2);

            cc0 = VEC_LEFT1_EPI8(c0, c1);
            c2 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, c2);

            y = VEC_OR_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);

            VEC_STORE_2D_EPI8(y, i, j, vY);
            
            //rotation de variable
            a0 = a1; b0 = b1; c0 = c1;
            a1 = a2; b1 = b2; c1 = c2;

        }
    }
}

void dilatation_3_SIMD_opti(vuint8 ** vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1){


    //vecteur aligné
    vuint8 a0, a1, a4, a7, a10;
    vuint8 b0, b1, b4, b7, b10;
    vuint8 c0, c1, c4, c7, c10;
    //vecteur non-alignés
    vuint8 aa0, aa2, aa3, aa5, aa6, aa8;
    vuint8 bb0, bb2, bb3, bb5, bb6, bb8;
    vuint8 cc0, cc2, cc3, cc5, cc6, cc8;
    //vecteur sortie
    vuint8 y0, y1, y2;

    int i, j;

    int k = 3;

    int r = (vmj1 + 1) % k;

    for(i = vmi0 ; i <= vmi1 ; i++){

        j = vmj0;

        a0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX) ; a1 = VEC_LOAD_2D_EPI8(i - 1, j, vX);   

        b0 = VEC_LOAD_2D_EPI8(i + 0, j - 1, vX) ; b1 = VEC_LOAD_2D_EPI8(i + 0, j, vX);

        c0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX) ; c1 = VEC_LOAD_2D_EPI8(i + 1, j, vX);


        for(j = vmj0 ; j <= vmj1 - r; j = j + k){

            aa0 = VEC_LEFT1_EPI8(a0, a1);
            a4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            aa2 = VEC_RIGHT1_EPI8(a1, a4);
            aa3 = VEC_LEFT1_EPI8(a1, a4);
            a7 = VEC_LOAD_2D_EPI8(i - 1, j + 2, vX);
            aa5 = VEC_RIGHT1_EPI8(a4, a7);
            aa6 = VEC_LEFT1_EPI8(a4, a7);
            a10 = VEC_LOAD_2D_EPI8(i - 1, j + 3, vX);
            aa8 = VEC_RIGHT1_EPI8(a7, a10);

            bb0 = VEC_LEFT1_EPI8(b0, b1);
            b4 = VEC_LOAD_2D_EPI8(i + 0, j + 1, vX);
            bb2 = VEC_RIGHT1_EPI8(b1, b4);
            bb3 = VEC_LEFT1_EPI8(b1, b4);
            b7 = VEC_LOAD_2D_EPI8(i + 0, j + 2, vX);
            bb5 = VEC_RIGHT1_EPI8(b4, b7);
            bb6 = VEC_LEFT1_EPI8(b4, b7);
            b10 = VEC_LOAD_2D_EPI8(i + 0, j + 3, vX);
            bb8 = VEC_RIGHT1_EPI8(b7, b10);

            cc0 = VEC_LEFT1_EPI8(c0, c1);
            c4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            cc2 = VEC_RIGHT1_EPI8(c1, c4);
            cc3 = VEC_LEFT1_EPI8(c1, c4);
            c7 = VEC_LOAD_2D_EPI8(i + 1, j + 2, vX);
            cc5 = VEC_RIGHT1_EPI8(c4, c7);
            cc6 = VEC_LEFT1_EPI8(c4, c7);
            c10 = VEC_LOAD_2D_EPI8(i + 1, j + 3, vX);
            cc8 = VEC_RIGHT1_EPI8(c7, c10);


            y0 = VEC_OR_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
            y1 = VEC_OR_9_EPI8(aa3, a4, aa5, bb3, b4, bb5, cc3, c4, cc5);
            y2 = VEC_OR_9_EPI8(aa6, a7, aa8, bb6, b7, bb8, cc6, c7, cc8);

            VEC_STORE_2D_EPI8(y0, i, j + 0, vY);
            VEC_STORE_2D_EPI8(y1, i, j + 1, vY);
            VEC_STORE_2D_EPI8(y2, i, j + 2, vY);
            //rotation de variable
            a0 = a7 ; b0 = b7 ; c0 = c7;
            a1 = a10; b1 = b10; c1 = c10;


        }

        switch(r) {
            case 1 :
                aa0 = VEC_LEFT1_EPI8(a0, a1);
                a4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                aa2 = VEC_RIGHT1_EPI8(a1, a4);

                bb0 = VEC_LEFT1_EPI8(b0, b1);
                b4 = VEC_LOAD_2D_EPI8(i + 0, j + 1, vX);
                bb2 = VEC_RIGHT1_EPI8(b1, b4);

                cc0 = VEC_LEFT1_EPI8(c0, c1);
                c4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                cc2 = VEC_RIGHT1_EPI8(c1, c4);

                y0 = VEC_OR_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);

                VEC_STORE_2D_EPI8(y0, i, vmj1, vY);
                break;

            case 2 :
                aa0 = VEC_LEFT1_EPI8(a0, a1);
                a4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                aa2 = VEC_RIGHT1_EPI8(a1, a4);
                aa3 = VEC_LEFT1_EPI8(a1, a4);
                a7 = VEC_LOAD_2D_EPI8(i - 1, j + 2, vX);
                aa5 = VEC_RIGHT1_EPI8(a4, a7);

                bb0 = VEC_LEFT1_EPI8(b0, b1);
                b4 = VEC_LOAD_2D_EPI8(i + 0, j + 1, vX);
                bb2 = VEC_RIGHT1_EPI8(b1, b4);
                bb3 = VEC_LEFT1_EPI8(b1, b4);
                b7 = VEC_LOAD_2D_EPI8(i + 0, j + 2, vX);
                bb5 = VEC_RIGHT1_EPI8(b4, b7);

                cc0 = VEC_LEFT1_EPI8(c0, c1);
                c4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                cc2 = VEC_RIGHT1_EPI8(c1, c4);
                cc3 = VEC_LEFT1_EPI8(c1, c4);
                c7 = VEC_LOAD_2D_EPI8(i + 1, j + 2, vX);
                cc5 = VEC_RIGHT1_EPI8(c4, c7);

                y0 = VEC_OR_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
                y1 = VEC_OR_9_EPI8(aa3, a4, aa5, bb3, b4, bb5, cc3, c4, cc5);

                VEC_STORE_2D_EPI8(y0, i, vmj1 - 1, vY);
                VEC_STORE_2D_EPI8(y1, i, vmj1, vY);
                break;

            default :
                break;
        }

    }
}

void dilatation_5_SIMD(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    //vecteur aligné
    vuint8 a0, a2, a4;
    vuint8 b0, b2, b4;
    vuint8 c0, c2, c4;
    vuint8 d0, d2, d4;
    vuint8 e0, e2, e4;
    //vecteur non-alignés
    vuint8 aa0, aa1, aa3, aa4;
    vuint8 bb0, bb1, bb3, bb4;
    vuint8 cc0, cc1, cc3, cc4;
    vuint8 dd0, dd1, dd3, dd4;
    vuint8 ee0, ee1, ee3, ee4;
    //vecteur sortie
    vuint8 y;

    int i, j;


    for(i = vmi0 ; i <= vmi1 ; i++){

        j = vmj0;

        a0 = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        a2 = VEC_LOAD_2D_EPI8(i - 2, j, vX);   

        b0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        b2 = VEC_LOAD_2D_EPI8(i - 1, j, vX);

        c0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        c2 = VEC_LOAD_2D_EPI8(i, j, vX);

        d0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        d2 = VEC_LOAD_2D_EPI8(i + 1, j, vX);

        e0 = VEC_LOAD_2D_EPI8(i + 2, j- 1, vX);
        e2 = VEC_LOAD_2D_EPI8(i + 2, j, vX);
        

        for(j = vmj0 ; j <= vmj1 ; j++){
            //printf("indice i, j : %d, %d\n", i, j);
            aa0 = VEC_LEFT2_EPI8(a0, a2);
            aa1 = VEC_LEFT1_EPI8(a0, a2);
            a4 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
            aa3 = VEC_RIGHT1_EPI8(a2, a4);
            aa4 = VEC_RIGHT2_EPI8(a2, a4);
    
            bb0 = VEC_LEFT2_EPI8(b0, b2);
            bb1 = VEC_LEFT1_EPI8(b0, b2);
            b4 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            bb3 = VEC_RIGHT1_EPI8(b2, b4);
            bb4 = VEC_RIGHT2_EPI8(b2, b4);

            cc0 = VEC_LEFT2_EPI8(c0, c2);
            cc1 = VEC_LEFT1_EPI8(c0, c2);
            c4 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
            cc3 = VEC_RIGHT1_EPI8(c2, c4);
            cc4 = VEC_RIGHT2_EPI8(c2, c4);

            dd0 = VEC_LEFT2_EPI8(d0, d2);
            dd1 = VEC_LEFT1_EPI8(d0, d2);
            d4 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            dd3 = VEC_RIGHT1_EPI8(d2, d4);
            dd4 = VEC_RIGHT2_EPI8(d2, d4);

            ee0 = VEC_LEFT2_EPI8(e0, e2);
            ee1 = VEC_LEFT1_EPI8(e0, e2);
            e4 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            ee3 = VEC_RIGHT1_EPI8(e2, e4);
            ee4 = VEC_RIGHT2_EPI8(e2, e4);

            y = VEC_OR_25_EPI8(aa0, aa1, a2, aa3, aa4, bb0, bb1, b2, bb3, bb4, cc0, cc1, c2, cc3, cc4, dd0, dd1, d2, dd3, dd4, ee0, ee1, e2, ee3, ee4);

            VEC_STORE_2D_EPI8(y, i, j, vY);
            
            //rotation de variable
            a0 = a2; b0 = b2; c0 = c2; d0 = d2; e0 = e2;
            a2 = a4; b2 = b4; c2 = c4; d2 = d4; e2 = e4;

        }
    }
}

void dilatation_5_SIMD_opti(vuint8 **vX, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1) {
    
       
    //vecteur aligné
    vuint8 a0, a3, a8, a13, a18, a23, a26;
    vuint8 b0, b3, b8, b13, b18, b23, b26;
    vuint8 c0, c3, c8, c13, c18, c23, c26;
    vuint8 d0, d3, d8, d13, d18, d23, d26;
    vuint8 e0, e3, e8, e13, e18, e23, e26;
    //vecteur non-alignés
    vuint8 aa1, aa2, aa4, aa5, aa6, aa7, aa9, aa10, aa11, aa12, aa14, aa15, aa16, aa17, aa19, aa20, aa21, aa22, aa24, aa25;
    vuint8 bb1, bb2, bb4, bb5, bb6, bb7, bb9, bb10, bb11, bb12, bb14, bb15, bb16, bb17, bb19, bb20, bb21, bb22, bb24, bb25;
    vuint8 cc1, cc2, cc4, cc5, cc6, cc7, cc9, cc10, cc11, cc12, cc14, cc15, cc16, cc17, cc19, cc20, cc21, cc22, cc24, cc25;
    vuint8 dd1, dd2, dd4, dd5, dd6, dd7, dd9, dd10, dd11, dd12, dd14, dd15, dd16, dd17, dd19, dd20, dd21, dd22, dd24, dd25;
    vuint8 ee1, ee2, ee4, ee5, ee6, ee7, ee9, ee10, ee11, ee12, ee14, ee15, ee16, ee17, ee19, ee20, ee21, ee22, ee24, ee25;
    //vecteur sortie
    vuint8 y0, y1, y2, y3, y4;

    int k = 5;

    int r = (vmj1 + 1)% k;

    int i, j;

    int bord = 2;
    
    for(i = vmi0 ; i <= vmi1 ; i++){
        j = vmj0;
        
        a0 = VEC_LOAD_2D_EPI8(i - 2, j - 1, vX);
        a3 = VEC_LOAD_2D_EPI8(i - 2, j, vX);   

        b0 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        b3 = VEC_LOAD_2D_EPI8(i - 1, j, vX);

        c0 = VEC_LOAD_2D_EPI8(i, j - 1, vX);
        c3 = VEC_LOAD_2D_EPI8(i, j, vX);

        d0 = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
        d3 = VEC_LOAD_2D_EPI8(i + 1, j, vX);

        e0 = VEC_LOAD_2D_EPI8(i + 2, j- 1, vX);
        e3 = VEC_LOAD_2D_EPI8(i + 2, j, vX);


        for(j = vmj0 ; j <= vmj1 - r; j = j + k){
            
            aa1 = VEC_LEFT2_EPI8(a0, a3);
            aa2 = VEC_LEFT1_EPI8(a0, a3);

            a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
            aa4 = VEC_RIGHT1_EPI8(a3, a8);
            aa5 = VEC_RIGHT2_EPI8(a3, a8);

            aa6 = VEC_LEFT2_EPI8(a3, a8);
            aa7 = VEC_LEFT1_EPI8(a3, a8);

            a13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
            aa9 = VEC_RIGHT1_EPI8(a8, a13);
            aa10 = VEC_RIGHT2_EPI8(a8, a13);

            aa11 = VEC_LEFT2_EPI8(a8, a13);
            aa12 = VEC_LEFT1_EPI8(a8, a13);

            a18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
            aa14 = VEC_RIGHT1_EPI8(a13, a18);
            aa15 = VEC_RIGHT2_EPI8(a13, a18);

            aa16 = VEC_LEFT2_EPI8(a13, a18);
            aa17 = VEC_LEFT1_EPI8(a13, a18);

            a23 = VEC_LOAD_2D_EPI8(i - 2, j + 4, vX);
            aa19 = VEC_RIGHT1_EPI8(a18, a23);
            aa20 = VEC_RIGHT2_EPI8(a18, a23);

            aa21 = VEC_LEFT2_EPI8(a18, a23);
            aa22 = VEC_LEFT1_EPI8(a18, a23);

            a26 = VEC_LOAD_2D_EPI8(i - 2, j + 5, vX);
            aa24 = VEC_RIGHT1_EPI8(a23, a26);
            aa25 = VEC_RIGHT2_EPI8(a23, a26);

            bb1 = VEC_LEFT2_EPI8(b0, b3);
            bb2 = VEC_LEFT1_EPI8(b0, b3);

            b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            bb4 = VEC_RIGHT1_EPI8(b3, b8);
            bb5 = VEC_RIGHT2_EPI8(b3, b8);

            bb6 = VEC_LEFT2_EPI8(b3, b8);
            bb7 = VEC_LEFT1_EPI8(b3, b8);

            b13 = VEC_LOAD_2D_EPI8(i - 1, j + 2, vX);
            bb9 = VEC_RIGHT1_EPI8(b8, b13);
            bb10 = VEC_RIGHT2_EPI8(b8, b13);

            bb11 = VEC_LEFT2_EPI8(b8, b13);
            bb12 = VEC_LEFT1_EPI8(b8, b13);

            b18 = VEC_LOAD_2D_EPI8(i - 1,  j + 3, vX);
            bb14 = VEC_RIGHT1_EPI8(b13, b18);
            bb15 = VEC_RIGHT2_EPI8(b13, b18);

            bb16 = VEC_LEFT2_EPI8(b13, b18);
            bb17 = VEC_LEFT1_EPI8(b13, b18);

            b23 = VEC_LOAD_2D_EPI8(i - 1, j + 4, vX);
            bb19 = VEC_RIGHT1_EPI8(b18, b23);
            bb20 = VEC_RIGHT2_EPI8(b18, b23);

            bb21 = VEC_LEFT2_EPI8(b18, b23);
            bb22 = VEC_LEFT1_EPI8(b18, b23);

            b26 = VEC_LOAD_2D_EPI8(i - 1, j + 5, vX);
            bb24 = VEC_RIGHT1_EPI8(b23, b26);
            bb25 = VEC_RIGHT2_EPI8(b23, b26);


            cc1 = VEC_LEFT2_EPI8(c0, c3);
            cc2 = VEC_LEFT1_EPI8(c0, c3);

            c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
            cc4 = VEC_RIGHT1_EPI8(c3, c8);
            cc5 = VEC_RIGHT2_EPI8(c3, c8);

            cc6 = VEC_LEFT2_EPI8(c3, c8);
            cc7 = VEC_LEFT1_EPI8(c3, c8);

            c13 = VEC_LOAD_2D_EPI8(i - 0, j + 2, vX);
            cc9 = VEC_RIGHT1_EPI8(c8, c13);
            cc10 = VEC_RIGHT2_EPI8(c8, c13);

            cc11 = VEC_LEFT2_EPI8(c8, c13);
            cc12 = VEC_LEFT1_EPI8(c8, c13);

            c18 = VEC_LOAD_2D_EPI8(i - 0,  j + 3, vX);
            cc14 = VEC_RIGHT1_EPI8(c13, c18);
            cc15 = VEC_RIGHT2_EPI8(c13, c18);

            cc16 = VEC_LEFT2_EPI8(c13, c18);
            cc17 = VEC_LEFT1_EPI8(c13, c18);

            c23 = VEC_LOAD_2D_EPI8(i - 0, j + 4, vX);
            cc19 = VEC_RIGHT1_EPI8(c18, c23);
            cc20 = VEC_RIGHT2_EPI8(c18, c23);

            cc21 = VEC_LEFT2_EPI8(c18, c23);
            cc22 = VEC_LEFT1_EPI8(c18, c23);

            c26 = VEC_LOAD_2D_EPI8(i - 0, j + 5, vX);
            cc24 = VEC_RIGHT1_EPI8(c23, c26);
            cc25 = VEC_RIGHT2_EPI8(c23, c26);


            dd1 = VEC_LEFT2_EPI8(d0, d3);
            dd2 = VEC_LEFT1_EPI8(d0, d3);

            d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            dd4 = VEC_RIGHT1_EPI8(d3, d8);
            dd5 = VEC_RIGHT2_EPI8(d3, d8);

            dd6 = VEC_LEFT2_EPI8(d3, d8);
            dd7 = VEC_LEFT1_EPI8(d3, d8);

            d13 = VEC_LOAD_2D_EPI8(i + 1, j + 2, vX);
            dd9 = VEC_RIGHT1_EPI8(d8, d13);
            dd10 = VEC_RIGHT2_EPI8(d8, d13);

            dd11 = VEC_LEFT2_EPI8(d8, d13);
            dd12 = VEC_LEFT1_EPI8(d8, d13);

            d18 = VEC_LOAD_2D_EPI8(i + 1,  j + 3, vX);
            dd14 = VEC_RIGHT1_EPI8(d13, d18);
            dd15 = VEC_RIGHT2_EPI8(d13, d18);

            dd16 = VEC_LEFT2_EPI8(d13, d18);
            dd17 = VEC_LEFT1_EPI8(d13, d18);

            d23 = VEC_LOAD_2D_EPI8(i + 1, j + 4, vX);
            dd19 = VEC_RIGHT1_EPI8(d18, d23);
            dd20 = VEC_RIGHT2_EPI8(d18, d23);

            dd21 = VEC_LEFT2_EPI8(d18, d23);
            dd22 = VEC_LEFT1_EPI8(d18, d23);

            d26 = VEC_LOAD_2D_EPI8(i + 1, j + 5, vX);
            dd24 = VEC_RIGHT1_EPI8(d23, d26);
            dd25 = VEC_RIGHT2_EPI8(d23, d26);


            ee1 = VEC_LEFT2_EPI8(e0, e3);
            ee2 = VEC_LEFT1_EPI8(e0, e3);

            e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
            ee4 = VEC_RIGHT1_EPI8(e3, e8);
            ee5 = VEC_RIGHT2_EPI8(e3, e8);

            ee6 = VEC_LEFT2_EPI8(e3, e8);
            ee7 = VEC_LEFT1_EPI8(e3, e8);

            e13 = VEC_LOAD_2D_EPI8(i + 2, j + 2, vX);
            ee9 = VEC_RIGHT1_EPI8(e8, e13);
            ee10 = VEC_RIGHT2_EPI8(e8, e13);

            ee11 = VEC_LEFT2_EPI8(e8, e13);
            ee12 = VEC_LEFT1_EPI8(e8, e13);

            e18 = VEC_LOAD_2D_EPI8(i + 2,  j + 3, vX);
            ee14 = VEC_RIGHT1_EPI8(e13, e18);
            ee15 = VEC_RIGHT2_EPI8(e13, e18);

            ee16 = VEC_LEFT2_EPI8(e13, e18);
            ee17 = VEC_LEFT1_EPI8(e13, e18);

            e23 = VEC_LOAD_2D_EPI8(i + 2, j + 4, vX);
            ee19 = VEC_RIGHT1_EPI8(e18, e23);
            ee20 = VEC_RIGHT2_EPI8(e18, e23);

            ee21 = VEC_LEFT2_EPI8(e18, e23);
            ee22 = VEC_LEFT1_EPI8(e18, e23);

            e26 = VEC_LOAD_2D_EPI8(i + 2, j + 5, vX);
            ee24 = VEC_RIGHT1_EPI8(e23, e26);
            ee25 = VEC_RIGHT2_EPI8(e23, e26);

            y0 = VEC_OR_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);
            y1 = VEC_OR_25_EPI8(aa6, aa7, a8, aa9, aa10, bb6, bb7, b8, bb9, bb10, cc6, cc7, c8, cc9, cc10, dd6, dd7, d8, dd9, dd10, ee6, ee7, e8, ee9, ee10);
            y2 = VEC_OR_25_EPI8(aa11, aa12, a13, aa14, aa15, bb11, bb12, b13, bb14, bb15, cc11, cc12, c13, cc14, cc15, dd11, dd12, d13, dd14, dd15, ee11, ee12, e13, ee14, ee15);
            y3 = VEC_OR_25_EPI8(aa16, aa17, a18, aa19, aa20, bb16, bb17, b18, bb19, bb20, cc16, cc17, c18, cc19, cc20, dd16, dd17, d18, dd19, dd20, ee16, ee17, e18, ee19, ee20);
            y4 = VEC_OR_25_EPI8(aa21, aa22, a23, aa24, aa25, bb21, bb22, b23, bb24, bb25, cc21, cc22, c23, cc24, cc25, dd21, dd22, d23, dd24, dd25, ee21, ee22, e23, ee24, ee25);

            VEC_STORE_2D_EPI8(y0, i, j + 0, vY);
            VEC_STORE_2D_EPI8(y0, i, j + 1, vY);
            VEC_STORE_2D_EPI8(y0, i, j + 2, vY);
            VEC_STORE_2D_EPI8(y0, i, j + 3, vY);
            VEC_STORE_2D_EPI8(y0, i, j + 4, vY);
            
        }
        switch(r) {
            case 1 :

                aa1 = VEC_LEFT2_EPI8(a0, a3);
                aa2 = VEC_LEFT1_EPI8(a0, a3);

                a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
                aa4 = VEC_RIGHT1_EPI8(a3, a8);
                aa5 = VEC_RIGHT2_EPI8(a3, a8);

                bb1 = VEC_LEFT2_EPI8(b0, b3);
                bb2 = VEC_LEFT1_EPI8(b0, b3);

                b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                bb4 = VEC_RIGHT1_EPI8(b3, b8);
                bb5 = VEC_RIGHT2_EPI8(b3, b8);

                cc1 = VEC_LEFT2_EPI8(c0, c3);
                cc2 = VEC_LEFT1_EPI8(c0, c3);

                c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
                cc4 = VEC_RIGHT1_EPI8(c3, c8);
                cc5 = VEC_RIGHT2_EPI8(c3, c8);

                dd1 = VEC_LEFT2_EPI8(d0, d3);
                dd2 = VEC_LEFT1_EPI8(d0, d3);

                d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                dd4 = VEC_RIGHT1_EPI8(d3, d8);
                dd5 = VEC_RIGHT2_EPI8(d3, d8);

                ee1 = VEC_LEFT2_EPI8(e0, e3);
                ee2 = VEC_LEFT1_EPI8(e0, e3);

                e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee4 = VEC_RIGHT1_EPI8(e3, e8);
                ee5 = VEC_RIGHT2_EPI8(e3, e8);

                y0 = VEC_OR_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);

                VEC_STORE_2D_EPI8(y0, i, vmj1, vY);
                break;

            case 2 :

                aa1 = VEC_LEFT2_EPI8(a0, a3);
                aa2 = VEC_LEFT1_EPI8(a0, a3);

                a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
                aa4 = VEC_RIGHT1_EPI8(a3, a8);
                aa5 = VEC_RIGHT2_EPI8(a3, a8);

                aa6 = VEC_LEFT2_EPI8(a3, a8);
                aa7 = VEC_LEFT1_EPI8(a3, a8);

                a13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                aa9 = VEC_RIGHT1_EPI8(a8, a13);
                aa10 = VEC_RIGHT2_EPI8(a8, a13);

                bb1 = VEC_LEFT2_EPI8(b0, b3);
                bb2 = VEC_LEFT1_EPI8(b0, b3);

                b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                bb4 = VEC_RIGHT1_EPI8(b3, b8);
                bb5 = VEC_RIGHT2_EPI8(b3, b8);

                bb6 = VEC_LEFT2_EPI8(b3, b8);
                bb7 = VEC_LEFT1_EPI8(b3, b8);

                b13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                bb9 = VEC_RIGHT1_EPI8(b8, b13);
                bb10 = VEC_RIGHT2_EPI8(b8, b13);

                cc1 = VEC_LEFT2_EPI8(c0, c3);
                cc2 = VEC_LEFT1_EPI8(c0, c3);

                c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
                cc4 = VEC_RIGHT1_EPI8(c3, c8);
                cc5 = VEC_RIGHT2_EPI8(c3, c8);

                cc6 = VEC_LEFT2_EPI8(c3, c8);
                cc7 = VEC_LEFT1_EPI8(c3, c8);

                c13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                cc9 = VEC_RIGHT1_EPI8(c8, c13);
                cc10 = VEC_RIGHT2_EPI8(c8, c13);

                dd1 = VEC_LEFT2_EPI8(d0, d3);
                dd2 = VEC_LEFT1_EPI8(d0, d3);

                d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                dd4 = VEC_RIGHT1_EPI8(d3, d8);
                dd5 = VEC_RIGHT2_EPI8(d3, d8);

                dd6 = VEC_LEFT2_EPI8(d3, d8);
                dd7 = VEC_LEFT1_EPI8(d3, d8);

                d13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                dd9 = VEC_RIGHT1_EPI8(d8, d13);
                dd10 = VEC_RIGHT2_EPI8(d8, d13);

                ee1 = VEC_LEFT2_EPI8(e0, e3);
                ee2 = VEC_LEFT1_EPI8(e0, e3);

                e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee4 = VEC_RIGHT1_EPI8(e3, e8);
                ee5 = VEC_RIGHT2_EPI8(e3, e8);

                ee6 = VEC_LEFT2_EPI8(e3, e8);
                ee7 = VEC_LEFT1_EPI8(e3, e8);

                e13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                ee9 = VEC_RIGHT1_EPI8(e8, e13);
                ee10 = VEC_RIGHT2_EPI8(e8, e13);

                y0 = VEC_OR_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);
                y1 = VEC_OR_25_EPI8(aa6, aa7, a8, aa9, aa10, bb6, bb7, b8, bb9, bb10, cc6, cc7, c8, cc9, cc10, dd6, dd7, d8, dd9, dd10, ee6, ee7, e8, ee9, ee10);


                VEC_STORE_2D_EPI8(y0, i, vmj1 - 1, vY);
                VEC_STORE_2D_EPI8(y1, i, vmj1, vY);
                break;
            
            case 3 :

                aa1 = VEC_LEFT2_EPI8(a0, a3);
                aa2 = VEC_LEFT1_EPI8(a0, a3);

                a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
                aa4 = VEC_RIGHT1_EPI8(a3, a8);
                aa5 = VEC_RIGHT2_EPI8(a3, a8);

                aa6 = VEC_LEFT2_EPI8(a3, a8);
                aa7 = VEC_LEFT1_EPI8(a3, a8);

                a13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                aa9 = VEC_RIGHT1_EPI8(a8, a13);
                aa10 = VEC_RIGHT2_EPI8(a8, a13);

                aa11 = VEC_LEFT2_EPI8(a8, a13);
                aa12 = VEC_LEFT1_EPI8(a8, a13);

                a18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                aa14 = VEC_RIGHT1_EPI8(a13, a18);
                aa15 = VEC_RIGHT2_EPI8(a13, a18);

                bb1 = VEC_LEFT2_EPI8(b0, b3);
                bb2 = VEC_LEFT1_EPI8(b0, b3);

                b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                bb4 = VEC_RIGHT1_EPI8(b3, b8);
                bb5 = VEC_RIGHT2_EPI8(b3, b8);

                bb6 = VEC_LEFT2_EPI8(b3, b8);
                bb7 = VEC_LEFT1_EPI8(b3, b8);

                b13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                bb9 = VEC_RIGHT1_EPI8(b8, b13);
                bb10 = VEC_RIGHT2_EPI8(b8, b13);

                bb11 = VEC_LEFT2_EPI8(b8, b13);
                bb12 = VEC_LEFT1_EPI8(b8, b13);

                b18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                bb14 = VEC_RIGHT1_EPI8(b13, b18);
                bb15 = VEC_RIGHT2_EPI8(b13, b18);

                cc1 = VEC_LEFT2_EPI8(c0, c3);
                cc2 = VEC_LEFT1_EPI8(c0, c3);

                c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
                cc4 = VEC_RIGHT1_EPI8(c3, c8);
                cc5 = VEC_RIGHT2_EPI8(c3, c8);

                cc6 = VEC_LEFT2_EPI8(c3, c8);
                cc7 = VEC_LEFT1_EPI8(c3, c8);

                c13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                cc9 = VEC_RIGHT1_EPI8(c8, c13);
                cc10 = VEC_RIGHT2_EPI8(c8, c13);

                cc11 = VEC_LEFT2_EPI8(c8, c13);
                cc12 = VEC_LEFT1_EPI8(c8, c13);

                c18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                cc14 = VEC_RIGHT1_EPI8(c13, c18);
                cc15 = VEC_RIGHT2_EPI8(c13, c18);

                dd1 = VEC_LEFT2_EPI8(d0, d3);
                dd2 = VEC_LEFT1_EPI8(d0, d3);

                d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                dd4 = VEC_RIGHT1_EPI8(d3, d8);
                dd5 = VEC_RIGHT2_EPI8(d3, d8);

                dd6 = VEC_LEFT2_EPI8(d3, d8);
                dd7 = VEC_LEFT1_EPI8(d3, d8);

                d13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                dd9 = VEC_RIGHT1_EPI8(d8, d13);
                dd10 = VEC_RIGHT2_EPI8(d8, d13);

                dd11 = VEC_LEFT2_EPI8(d8, d13);
                dd12 = VEC_LEFT1_EPI8(d8, d13);

                d18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                dd14 = VEC_RIGHT1_EPI8(d13, d18);
                dd15 = VEC_RIGHT2_EPI8(d13, d18);

                ee1 = VEC_LEFT2_EPI8(e0, e3);
                ee2 = VEC_LEFT1_EPI8(e0, e3);

                e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee4 = VEC_RIGHT1_EPI8(e3, e8);
                ee5 = VEC_RIGHT2_EPI8(e3, e8);

                ee6 = VEC_LEFT2_EPI8(e3, e8);
                ee7 = VEC_LEFT1_EPI8(e3, e8);

                e13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                ee9 = VEC_RIGHT1_EPI8(e8, e13);
                ee10 = VEC_RIGHT2_EPI8(e8, e13);

                ee11 = VEC_LEFT2_EPI8(e8, e13);
                ee12 = VEC_LEFT1_EPI8(e8, e13);

                e18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                ee14 = VEC_RIGHT1_EPI8(e13, e18);
                ee15 = VEC_RIGHT2_EPI8(e13, e18);

                y0 = VEC_OR_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);
                y1 = VEC_OR_25_EPI8(aa6, aa7, a8, aa9, aa10, bb6, bb7, b8, bb9, bb10, cc6, cc7, c8, cc9, cc10, dd6, dd7, d8, dd9, dd10, ee6, ee7, e8, ee9, ee10);
                y2 = VEC_OR_25_EPI8(aa11, aa12, a13, aa14, aa15, bb11, bb12, b13, bb14, bb15, cc11, cc12, c13, cc14, cc15, dd11, dd12, d13, dd14, dd15, ee11, ee12, e13, ee14, ee15);


                VEC_STORE_2D_EPI8(y0, i, vmj1 - 2, vY);
                VEC_STORE_2D_EPI8(y1, i, vmj1 - 1, vY);
                VEC_STORE_2D_EPI8(y2, i, vmj1 - 0, vY);

                break;

            case 4 :

                aa1 = VEC_LEFT2_EPI8(a0, a3);
                aa2 = VEC_LEFT1_EPI8(a0, a3);

                a8 = VEC_LOAD_2D_EPI8(i - 2, j + 1, vX);
                aa4 = VEC_RIGHT1_EPI8(a3, a8);
                aa5 = VEC_RIGHT2_EPI8(a3, a8);

                aa6 = VEC_LEFT2_EPI8(a3, a8);
                aa7 = VEC_LEFT1_EPI8(a3, a8);

                a13 = VEC_LOAD_2D_EPI8(i - 2, j + 2, vX);
                aa9 = VEC_RIGHT1_EPI8(a8, a13);
                aa10 = VEC_RIGHT2_EPI8(a8, a13);

                aa11 = VEC_LEFT2_EPI8(a8, a13);
                aa12 = VEC_LEFT1_EPI8(a8, a13);

                a18 = VEC_LOAD_2D_EPI8(i - 2,  j + 3, vX);
                aa14 = VEC_RIGHT1_EPI8(a13, a18);
                aa15 = VEC_RIGHT2_EPI8(a13, a18);

                aa16 = VEC_LEFT2_EPI8(a13, a18);
                aa17 = VEC_LEFT1_EPI8(a13, a18);

                a23 = VEC_LOAD_2D_EPI8(i - 2, j + 4, vX);
                aa19 = VEC_RIGHT1_EPI8(a18, a23);
                aa20 = VEC_RIGHT2_EPI8(a18, a23);

                bb1 = VEC_LEFT2_EPI8(b0, b3);
                bb2 = VEC_LEFT1_EPI8(b0, b3);

                b8 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
                bb4 = VEC_RIGHT1_EPI8(b3, b8);
                bb5 = VEC_RIGHT2_EPI8(b3, b8);

                bb6 = VEC_LEFT2_EPI8(b3, b8);
                bb7 = VEC_LEFT1_EPI8(b3, b8);

                b13 = VEC_LOAD_2D_EPI8(i - 1, j + 2, vX);
                bb9 = VEC_RIGHT1_EPI8(b8, b13);
                bb10 = VEC_RIGHT2_EPI8(b8, b13);

                bb11 = VEC_LEFT2_EPI8(b8, b13);
                bb12 = VEC_LEFT1_EPI8(b8, b13);

                b18 = VEC_LOAD_2D_EPI8(i - 1,  j + 3, vX);
                bb14 = VEC_RIGHT1_EPI8(b13, b18);
                bb15 = VEC_RIGHT2_EPI8(b13, b18);

                bb16 = VEC_LEFT2_EPI8(b13, b18);
                bb17 = VEC_LEFT1_EPI8(b13, b18);

                b23 = VEC_LOAD_2D_EPI8(i - 1, j + 4, vX);
                bb19 = VEC_RIGHT1_EPI8(b18, b23);
                bb20 = VEC_RIGHT2_EPI8(b18, b23);

                cc1 = VEC_LEFT2_EPI8(c0, c3);
                cc2 = VEC_LEFT1_EPI8(c0, c3);

                c8 = VEC_LOAD_2D_EPI8(i - 0, j + 1, vX);
                cc4 = VEC_RIGHT1_EPI8(c3, c8);
                cc5 = VEC_RIGHT2_EPI8(c3, c8);

                cc6 = VEC_LEFT2_EPI8(c3, c8);
                cc7 = VEC_LEFT1_EPI8(c3, c8);

                c13 = VEC_LOAD_2D_EPI8(i - 0, j + 2, vX);
                cc9 = VEC_RIGHT1_EPI8(c8, c13);
                cc10 = VEC_RIGHT2_EPI8(c8, c13);

                cc11 = VEC_LEFT2_EPI8(c8, c13);
                cc12 = VEC_LEFT1_EPI8(c8, c13);

                c18 = VEC_LOAD_2D_EPI8(i - 0,  j + 3, vX);
                cc14 = VEC_RIGHT1_EPI8(c13, c18);
                cc15 = VEC_RIGHT2_EPI8(c13, c18);

                cc16 = VEC_LEFT2_EPI8(c13, c18);
                cc17 = VEC_LEFT1_EPI8(c13, c18);

                c23 = VEC_LOAD_2D_EPI8(i - 0, j + 4, vX);
                cc19 = VEC_RIGHT1_EPI8(c18, c23);
                cc20 = VEC_RIGHT2_EPI8(c18, c23);

                dd1 = VEC_LEFT2_EPI8(d0, d3);
                dd2 = VEC_LEFT1_EPI8(d0, d3);

                d8 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
                dd4 = VEC_RIGHT1_EPI8(d3, d8);
                dd5 = VEC_RIGHT2_EPI8(d3, d8);

                dd6 = VEC_LEFT2_EPI8(d3, d8);
                dd7 = VEC_LEFT1_EPI8(d3, d8);

                d13 = VEC_LOAD_2D_EPI8(i + 1, j + 2, vX);
                dd9 = VEC_RIGHT1_EPI8(d8, d13);
                dd10 = VEC_RIGHT2_EPI8(d8, d13);

                dd11 = VEC_LEFT2_EPI8(d8, d13);
                dd12 = VEC_LEFT1_EPI8(d8, d13);

                d18 = VEC_LOAD_2D_EPI8(i + 1,  j + 3, vX);
                dd14 = VEC_RIGHT1_EPI8(d13, d18);
                dd15 = VEC_RIGHT2_EPI8(d13, d18);

                dd16 = VEC_LEFT2_EPI8(d13, d18);
                dd17 = VEC_LEFT1_EPI8(d13, d18);

                d23 = VEC_LOAD_2D_EPI8(i + 1, j + 4, vX);
                dd19 = VEC_RIGHT1_EPI8(d18, d23);
                dd20 = VEC_RIGHT2_EPI8(d18, d23);

                ee1 = VEC_LEFT2_EPI8(e0, e3);
                ee2 = VEC_LEFT1_EPI8(e0, e3);

                e8 = VEC_LOAD_2D_EPI8(i + 2, j + 1, vX);
                ee4 = VEC_RIGHT1_EPI8(e3, e8);
                ee5 = VEC_RIGHT2_EPI8(e3, e8);

                ee6 = VEC_LEFT2_EPI8(e3, e8);
                ee7 = VEC_LEFT1_EPI8(e3, e8);

                e13 = VEC_LOAD_2D_EPI8(i + 2, j + 2, vX);
                ee9 = VEC_RIGHT1_EPI8(e8, e13);
                ee10 = VEC_RIGHT2_EPI8(e8, e13);

                ee11 = VEC_LEFT2_EPI8(e8, e13);
                ee12 = VEC_LEFT1_EPI8(e8, e13);

                e18 = VEC_LOAD_2D_EPI8(i + 2,  j + 3, vX);
                ee14 = VEC_RIGHT1_EPI8(e13, e18);
                ee15 = VEC_RIGHT2_EPI8(e13, e18);

                ee16 = VEC_LEFT2_EPI8(e13, e18);
                ee17 = VEC_LEFT1_EPI8(e13, e18);

                e23 = VEC_LOAD_2D_EPI8(i + 2, j + 4, vX);
                ee19 = VEC_RIGHT1_EPI8(e18, e23);
                ee20 = VEC_RIGHT2_EPI8(e18, e23);


                y0 = VEC_OR_25_EPI8(aa1, aa2, a3, aa4, aa5, bb1, bb2, b3, bb4, bb5, cc1, cc2, c3, cc4, cc5, dd1, dd2, d3, dd4, dd5, ee1, ee2, e3, ee4, ee5);
                y1 = VEC_OR_25_EPI8(aa6, aa7, a8, aa9, aa10, bb6, bb7, b8, bb9, bb10, cc6, cc7, c8, cc9, cc10, dd6, dd7, d8, dd9, dd10, ee6, ee7, e8, ee9, ee10);
                y2 = VEC_OR_25_EPI8(aa11, aa12, a13, aa14, aa15, bb11, bb12, b13, bb14, bb15, cc11, cc12, c13, cc14, cc15, dd11, dd12, d13, dd14, dd15, ee11, ee12, e13, ee14, ee15);
                y3 = VEC_OR_25_EPI8(aa16, aa17, a18, aa19, aa20, bb16, bb17, b18, bb19, bb20, cc16, cc17, c18, cc19, cc20, dd16, dd17, d18, dd19, dd20, ee16, ee17, e18, ee19, ee20);
    

                VEC_STORE_2D_EPI8(y0, i, vmj1 - 3, vY);
                VEC_STORE_2D_EPI8(y1, i, vmj1 - 2, vY);
                VEC_STORE_2D_EPI8(y2, i, vmj1 - 1, vY);
                VEC_STORE_2D_EPI8(y3, i, vmj1 - 0, vY);

                break;

            default :
                break;
        }
    }
}

void morpho_3_SIMD(vuint8 **vX, vuint8 ** vY, vuint8 ** tmp1, vuint8 ** tmp2, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    int bord = 1;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;

    erosion_3_SIMD(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    dilatation_3_SIMD(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    dilatation_3_SIMD(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);
    erosion_3_SIMD(tmp1, vY, vmi0, vmi1, vmj0, vmj1);

}

void morpho_3_SIMD_opti(vuint8 **vX, vuint8 ** vY, vuint8 ** tmp1, vuint8 ** tmp2, int vmi0, int vmi1, int vmj0, int vmj1) {
    
    int bord = 1;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;

    erosion_3_SIMD_opti(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    dilatation_3_SIMD_opti(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    dilatation_3_SIMD_opti(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);
    erosion_3_SIMD_opti(tmp1, vY, vmi0, vmi1, vmj0, vmj1);

}

void morpho_5_SIMD(vuint8 **vX, vuint8 **vY, vuint8 ** tmp1, vuint8 ** tmp2, int vmi0, int vmi1, int vmj0, int vmj1)
{
    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;


    erosion_5_SIMD(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    dilatation_5_SIMD(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    dilatation_5_SIMD(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);
    erosion_5_SIMD(tmp1, vY, vmi0, vmi1, vmj0, vmj1);
    
}

void morpho_5_SIMD_opti(vuint8 **vX, vuint8 **vY, vuint8 ** tmp1, vuint8 ** tmp2, int vmi0, int vmi1, int vmj0, int vmj1)
{
    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - bord;
    int vmj1b = vmj1 + bord;


    erosion_5_SIMD_opti(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    dilatation_5_SIMD_opti(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    dilatation_5_SIMD_opti(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);
    erosion_5_SIMD_opti(tmp1, vY, vmi0, vmi1, vmj0, vmj1);
    
}

void morpho_3_SIMD_pipeline(vuint8 ** vX, vuint8 **vY, int vmi0, int vmi1, int vmj0, int vmj1){
    
}