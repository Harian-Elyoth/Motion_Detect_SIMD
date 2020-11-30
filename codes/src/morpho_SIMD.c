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
    
    int bord = 2;
    
    int vmi0b = vmi0 - bord;
    int vmi1b = vmi1 + bord;

    int vmj0b = vmj0 - 1;
    int vmj1b = vmj1 + 1;

    erosion_3_SIMD(vX, tmp1, vmi0, vmi1, vmj0, vmj1);
    // dilatation_3_SIMD(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);
    // dilatation_3_SIMD(tmp2, tmp1, vmi0, vmi1, vmj0, vmj1);

    dilatation_5_SIMD(tmp1, tmp2, vmi0, vmi1, vmj0, vmj1);

    erosion_3_SIMD(tmp2, vY, vmi0, vmi1, vmj0, vmj1);
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

void morpho_3_SIMD_pipeline(vuint8 **vX, vuint8 **tmp1, vuint8 **tmp2, vuint8 **tmp3, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1){

    DEBUG(printf("vmi0 = %d, vmi1 = %d, vmj0 = %d, vmj1 = %d\n", vmi0, vmi1, vmj0, vmj1));

    // CONDITION SUR LA TAILLE DES MATRICES
    // il faut au moins 4 colonnes SIMD et 6 lignes
    // width >= 16 * 4 = 64
    // height >= 6

    int i, j;

    // PROLOGUE GLOBAL

    // 1ere erosion sur les 3 premiere lignes et toutes les colonnes : ligne jaune
    erosion_3_SIMD(vX, tmp3, vmi0, vmi0 + 2, vmj0, vmj1);

    // 1ere dilation sur les 2 premieres lignes et toutes les colonnes : ligne verte
    dilatation_3_SIMD(tmp3, tmp2, vmi0, vmi0 + 1, vmj0, vmj1);

    // 2e dilation sur la 1ere lignes et toutes les colonnes : ligne bleue
    dilatation_3_SIMD(tmp2, tmp1, vmi0, vmi0, vmj0, vmj1);

    // aa, bb : non alignés
    // a , b   : alignés

    // colonne 0
    vuint8 a0, aa0, a1, aa2, a2; 
    vuint8 b0, bb0, b1, bb2, b2;
    vuint8 cc0, c1, cc2; 

    // colonne 1
    vuint8 b3, bb3, b4, bb5, b5;
    vuint8 c3, cc3, c4, cc5, c5; 
    vuint8 dd3, d4, dd5;

    // colonne 2
    vuint8 c6, cc6, c7, cc8, c8;
    vuint8 d6, dd6, d7, dd8, d8;
    vuint8 ee6, e7, ee8;

    // colonne 3
    vuint8 d9, dd9, d10, dd11, d11;
    vuint8 e9, ee9, e10, ee11, e11;
    vuint8 ff9, f10, ff11;

    // vecteur de sorties
    vuint8 y0, y1, y2, y3;

    // on commence a i = 3
    for (i = vmi0 + 3; i <= vmi1; ++i){   

        // on commence a j = 3
        j = vmj0 + 3;

        // PROLOGUE BOUCLE

        // 1ere erosion sur la 4e ligne et les 3 premières colonnes : ligne jaune
        erosion_3_SIMD(vX, tmp3, i, i, j - 3, j - 1);

        // 1ere dilation sur la 3e ligne et les 2 premieres colonnes : ligne verte
        dilatation_3_SIMD(tmp3, tmp2, i - 1, i - 1, j - 3, j - 2);

        // 2e dilation sur la 2e ligne et la premieres colonne : ligne bleue
        dilatation_3_SIMD(tmp2, tmp1, i - 2, i - 2, j - 3, j - 3);

        // on considère i et j pour la ligne jaune

        // ligne -1, autour de la colonne 0
        a0 = VEC_LOAD_2D_EPI8(i - 4, j - 4, tmp1);
        a1  = VEC_LOAD_2D_EPI8(i - 4, j - 3, tmp1);
        

        // ligne 0, autour de la colonne 0
        b0 = VEC_LOAD_2D_EPI8(i - 3, j - 4, tmp1);
        b1  = VEC_LOAD_2D_EPI8(i - 3, j - 3, tmp1);
        

        // ligne 0, autour la colonne 1
        b3 = VEC_LOAD_2D_EPI8(i - 3, j - 3, tmp2);
        b4  = VEC_LOAD_2D_EPI8(i - 3, j - 2, tmp2);
        

        // ligne 1, autour de la colonne 1
        c3 = VEC_LOAD_2D_EPI8(i - 2, j - 3, tmp2);
        c4  = VEC_LOAD_2D_EPI8(i - 2, j - 2, tmp2);
        

        // ligne 1, autour de la colonne 2
        c6 = VEC_LOAD_2D_EPI8(i - 2, j - 2, tmp3);
        c7  = VEC_LOAD_2D_EPI8(i - 2, j - 1, tmp3);
        

        // ligne 2, autour de la colonne 2
        d6 = VEC_LOAD_2D_EPI8(i - 1, j - 2, tmp3);
        d7  = VEC_LOAD_2D_EPI8(i - 1, j - 1, tmp3);
        

        // ligne 2, autour de la colonne 3
        d9  = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        d10  = VEC_LOAD_2D_EPI8(i - 1, j    , vX);
        

        // ligne 3, autour de la colonne 3
        e9  = VEC_LOAD_2D_EPI8(i , j - 1, vX);
        e10  = VEC_LOAD_2D_EPI8(i , j    , vX);
        

        for (j = vmj0 + 3; j <= vmj1; ++j)
        {

            // ALLOCATION + CALCUL VECTEUR NON ALIGNES

            aa0 = VEC_LEFT1_EPI8(a0, a1);
            a2 = VEC_LOAD_2D_EPI8(i - 4, j - 2, tmp1);
            aa2 = VEC_RIGHT1_EPI8(a1, a2);

            bb0 = VEC_LEFT1_EPI8(b0, b1);
            b2 = VEC_LOAD_2D_EPI8(i - 3, j - 2, tmp1);
            bb2 = VEC_RIGHT1_EPI8(b1, b2);

            bb3 = VEC_LEFT1_EPI8(b3, b4);
            b5 = VEC_LOAD_2D_EPI8(i - 3, j - 1, tmp2);
            bb5 = VEC_RIGHT1_EPI8(b4, b5);

            cc3 = VEC_LEFT1_EPI8(c3, c4);
            c5 = VEC_LOAD_2D_EPI8(i - 2, j - 1, tmp2);
            cc5 = VEC_RIGHT1_EPI8(c4, c5);

            cc6 = VEC_LEFT1_EPI8(c6, c7);
            c8 = VEC_LOAD_2D_EPI8(i - 2, j    , tmp3);
            cc8 = VEC_RIGHT1_EPI8(c7, c8);

            dd6 = VEC_LEFT1_EPI8(d6, d7);
            d8 = VEC_LOAD_2D_EPI8(i - 1, j    , tmp3);
            dd8 = VEC_RIGHT1_EPI8(d7, d8);

            dd9  = VEC_LEFT1_EPI8(d9, d10);
            d11 = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            dd11 = VEC_RIGHT1_EPI8(d10, d11);

            ee9  = VEC_LEFT1_EPI8(e9, e10);
            e11 = VEC_LOAD_2D_EPI8(i , j + 1, vX);
            ee11 = VEC_RIGHT1_EPI8(e10, e11);

            /*----------------------------------------------*/

            // ligne 4, autour de la colonne 3
            ff9  = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
            f10  = VEC_LOAD_2D_EPI8(i + 1, j    , vX);
            ff9  = VEC_LEFT1_EPI8(ff9, f10);
            ff11 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            ff11 = VEC_RIGHT1_EPI8(f10, ff11);

            // 1ere erosion 3 : ligne jaune
            y0 = VEC_AND_9_EPI8(dd9, d10, dd11, ee9, e10, ee11, ff9, f10, ff11);
            VEC_STORE_2D_EPI8(y0, i, j, tmp3);

            // ligne 3, autour de la colonne 2
            ee6 = VEC_LOAD_2D_EPI8(i , j - 2, tmp3);
            e7  = VEC_LOAD_2D_EPI8(i , j - 1, tmp3);
            ee6 = VEC_LEFT1_EPI8(ee6, e7);
            ee8 = VEC_LOAD_2D_EPI8(i , j    , tmp3);
            ee8 = VEC_RIGHT1_EPI8(e7, ee8);

            // 1ere dilatation 3 : ligne verte 
            y1 = VEC_OR_9_EPI8(cc6, c7, cc8, dd6, d7, dd8, ee6, e7, ee8);
            VEC_STORE_2D_EPI8(y1, i - 1, j - 1, tmp2);

            // ligne 3, autour de la colonne 1
            dd3 = VEC_LOAD_2D_EPI8(i - 1, j - 3, tmp2);
            d4  = VEC_LOAD_2D_EPI8(i - 1, j - 2, tmp2);
            dd3 = VEC_LEFT1_EPI8(dd3, d4);
            dd5 = VEC_LOAD_2D_EPI8(i - 1, j - 1, tmp2);
            dd5 = VEC_RIGHT1_EPI8(d4, dd5);

            // 2e dilation 3 : ligne bleue
            y2 = VEC_OR_9_EPI8(bb3, b4, bb5, cc3, c4, cc5, dd3, d4, dd5);
            VEC_STORE_2D_EPI8(y2, i - 2, j - 2, tmp1);   

            // ligne 2, autour de la colonne 0
            cc0 = VEC_LOAD_2D_EPI8(i - 2, j - 4, tmp1);
            c1  = VEC_LOAD_2D_EPI8(i - 2, j - 3, tmp1);
            cc0 = VEC_LEFT1_EPI8(cc0, c1);
            cc2 = VEC_LOAD_2D_EPI8(i - 2, j - 2, tmp1);
            cc2 = VEC_RIGHT1_EPI8(c1, cc2);

            // 2e erosion 3 : ligne rouge 
            y3 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
            VEC_STORE_2D_EPI8(y3, i - 3, j - 3, vY);

            // ROTATION DE REGISTRE  

            // colonne 0   
            a0 = a1 ; a1 = a2 ;
            b0 = b1 ; b1 = b2 ;

            // colonne 1
            b3 = b4 ; b4 = b5 ; 
            c3 = c4 ; c4 = c5 ;

            // colonne 2
            c6 = c7 ; c7 = c8 ;
            d6 = d7 ; d7 = d8 ;

            // colonne 3
            d9  = d10  ; d10  = d11 ;
            e9  = e10  ; e10  = e11 ;
        }

        // EPILOGUE BOUCLE 

        // dilation sur l'avant derniere ligne et la derniere colonnes de l'iteration courante : ligne verte
        dilatation_3_SIMD(tmp3, tmp2, i - 1, i - 1, vmj1, vmj1);

        // dilation sur l'avant avant dernière ligne des 2 dernieres colonnes de l'iteration courante : ligne bleue
        dilatation_3_SIMD(tmp2, tmp1, i - 2, i - 2, vmj1 - 1, vmj1);

        // printf("\n\nvmi1 - 6 = %d, vmi1 = %d, j - 3 = %d, j - 3 = %d\n\n", vmi1 - 6, vmi1, j - 3, j - 3);

        // erosion sur l'avant avant avant derniere lignes des 3 dernieres colonnes de l'iteration courante : ligne rouge
        erosion_3_SIMD(tmp1, vY, i - 3, i - 3, vmj1 - 2, vmj1);
    }

    // EPILOGUE GLOBAL

    // dilation sur la derniere ligne
    dilatation_3_SIMD(tmp3, tmp2, vmi1, vmi1, vmj0, vmj1);

    // dilation les 2 dernieres lignes
    dilatation_3_SIMD(tmp2, tmp1, vmi1 - 1, vmi1, vmj0, vmj1);

    // erosion sur les 3 derniere lignes
    erosion_3_SIMD(tmp1, vY, vmi1 - 2, vmi1, vmj0, vmj1);
}

void morpho_3_SIMD_pipeline_opti(vuint8 **vX, vuint8 **tmp1, vuint8 **tmp2, vuint8 ** vY, int vmi0, int vmi1, int vmj0, int vmj1){

    DEBUG(printf("vmi0 = %d, vmi1 = %d, vmj0 = %d, vmj1 = %d\n", vmi0, vmi1, vmj0, vmj1));

    // CONDITION SUR LA TAILLE DES MATRICES
    // il faut au moins 4 colonnes SIMD et 6 lignes
    // width >= 16 * 4 = 64
    // height >= 6

    int i, j;

    // PROLOGUE GLOBAL

    // printf("1\n");

    // 1ere erosion sur les 3 premieres lignes
    erosion_3_SIMD_opti(vX, tmp2, vmi0, vmi0 + 2, vmj0, vmj1);

    // dilation 5 sur la 1ere ligne
    dilatation_5_SIMD_opti(tmp2, tmp1, vmi0, vmi0, vmj0, vmj1);

    // printf("2\n");

    // aa, bb : non alignés
    // a , b  : alignés

    // colonne 0
    vuint8 a0, aa0, a1, aa2, a2; 
    vuint8 b0, bb0, b1, bb2, b2;
    vuint8 cc0, c1, cc2; 

    // colonne 1
    vuint8 a3, aa3, aa4, a5, aa6, aa7, a7;
    vuint8 b3, bb3, bb4, b5, bb6, bb7, b7;
    vuint8 c3, cc3, cc4, c5, cc6, cc7, c7;  
    vuint8 d3, dd3, dd4, d5, dd6, dd7, d7;
    vuint8 ee3, ee4, e5, ee6, ee7;

    // colonne 2
    vuint8 d8, dd8, d9, dd10, d10;
    vuint8 e8, ee8, e9, ee10, e10;
    vuint8 ff8, f9, ff10;

    // vecteur de sorties
    vuint8 y0, y1, y2;

    // on commence a i egale 3
    for (i = vmi0 + 3; i <= vmi1; ++i){   

        // printf("i = %d\n", j);

        // on commence a j = 2
        j = vmj0 + 2;

        // PROLOGUE BOUCLE

        // 1ere erosion sur la ligne 3 et la colonne 2
        erosion_3_SIMD_opti(vX, tmp2, i, i, j - 2, j - 1);

        // dilation sur la ...
        dilatation_5_SIMD_opti(tmp2, tmp1, i - 2, i - 2, j - 2, j - 2);

        // on considère i et j pour la ligne jaune

        // EROSION 3 - 2 ligne rouge

        // ligne -1, autour de la colonne 0
        a0  = VEC_LOAD_2D_EPI8(i - 4, j - 3, tmp1);
        a1  = VEC_LOAD_2D_EPI8(i - 4, j - 2, tmp1);
        
        // ligne 0, autour de la colonne 0
        b0  = VEC_LOAD_2D_EPI8(i - 3, j - 3, tmp1);
        b1  = VEC_LOAD_2D_EPI8(i - 3, j - 2, tmp1);
        
        /*---------------------------------------------*/

        // DILATAION 5 ligne bleue

        a3  = VEC_LOAD_2D_EPI8(i - 4, j - 2, tmp2);
        a5  = VEC_LOAD_2D_EPI8(i - 4, j - 1, tmp2);

        // ligne 0 autour de la colonne 1
        b3  = VEC_LOAD_2D_EPI8(i - 3, j - 2, tmp2);
        b5  = VEC_LOAD_2D_EPI8(i - 3, j - 1, tmp2);

        // ligne 1 autour de colonne 1
        c3  = VEC_LOAD_2D_EPI8(i - 2, j - 2, tmp2);
        c5  = VEC_LOAD_2D_EPI8(i - 2, j - 1, tmp2);

        // ligne 2 autour de la colonne 1
        d3  = VEC_LOAD_2D_EPI8(i - 1, j - 2, tmp2);
        d5  = VEC_LOAD_2D_EPI8(i - 1, j - 1, tmp2);

        /*---------------------------------------------*/

        // EROSION 3 - 1, ligne jaune

        // ligne 2, autour de la colonne 2
        d8 = VEC_LOAD_2D_EPI8(i - 1, j - 1, vX);
        d9 = VEC_LOAD_2D_EPI8(i - 1, j    , vX);
        
        // ligne 3, autour de la colonne 2
        e8 = VEC_LOAD_2D_EPI8(i , j - 1, vX);
        e9 = VEC_LOAD_2D_EPI8(i , j    , vX);
        
        for (j = vmj0 + 2; j <= vmj1; ++j)
        {

            // printf("i = %d\n", i);

            // ALLOCATION + CALCUL VECTEUR NON ALIGNES

            // EROSION 3 - 2 ligne rouge
            aa0 = VEC_LEFT1_EPI8(a0, a1);
            a2  = VEC_LOAD_2D_EPI8(i - 4, j - 1, tmp1);
            aa2 = VEC_RIGHT1_EPI8(a1, a2);

            bb0 = VEC_LEFT1_EPI8(b0, b1);
            b2  = VEC_LOAD_2D_EPI8(i - 3, j - 1, tmp1);
            bb2 = VEC_RIGHT1_EPI8(b1, b2);

            /*---------------------------------------------*/

            // DILATAION 5 ligne bleue
            aa3 = VEC_LEFT2_EPI8(a3, a5);
            aa4 = VEC_LEFT1_EPI8(a3, a5);
            a7  = VEC_LOAD_2D_EPI8(i - 4, j , tmp2);
            aa6 = VEC_RIGHT1_EPI8(a5, a7);
            aa7 = VEC_RIGHT2_EPI8(a5, a7);

            bb3 = VEC_LEFT2_EPI8(b3, b5);
            bb4 = VEC_LEFT1_EPI8(b3, b5);
            b7  = VEC_LOAD_2D_EPI8(i - 3, j , tmp2);
            bb6 = VEC_RIGHT1_EPI8(b5, b7);
            bb7 = VEC_RIGHT2_EPI8(b5, b7);

            cc3 = VEC_LEFT2_EPI8(c3, c5);
            cc4 = VEC_LEFT1_EPI8(c3, c5);
            c7  = VEC_LOAD_2D_EPI8(i - 2, j , tmp2);
            cc6 = VEC_RIGHT1_EPI8(c5, c7);
            cc7 = VEC_RIGHT2_EPI8(c5, c7);

            dd3 = VEC_LEFT2_EPI8(d3, d5);
            dd4 = VEC_LEFT1_EPI8(d3, d5);
            d7  = VEC_LOAD_2D_EPI8(i - 1, j , tmp2);
            dd6 = VEC_RIGHT1_EPI8(d5, d7);
            dd7 = VEC_RIGHT2_EPI8(d5, d7);

            /*---------------------------------------------*/

            // EROSION 3 - 1, ligne jaune
            dd8  = VEC_LEFT1_EPI8(d8, d9);
            d10  = VEC_LOAD_2D_EPI8(i - 1, j + 1, vX);
            dd10 = VEC_RIGHT1_EPI8(d9, d10);

            ee8  = VEC_LEFT1_EPI8(e8, e9);
            e10  = VEC_LOAD_2D_EPI8(i , j + 1, vX);
            ee10 = VEC_RIGHT1_EPI8(e9, e10);

            /*---------------------------------------------*/
            /*---------------------------------------------*/

            // TRAITEMENTS

            // EROSION 3 - 1
            // ligne 4, autour de la colonne 2
            ff8  = VEC_LOAD_2D_EPI8(i + 1, j - 1, vX);
            f9   = VEC_LOAD_2D_EPI8(i + 1, j    , vX);
            ff8  = VEC_LEFT1_EPI8(ff8, f9);
            ff10 = VEC_LOAD_2D_EPI8(i + 1, j + 1, vX);
            ff10 = VEC_RIGHT1_EPI8(f9, ff10);

            // CALCUL
            // 1ere erosion 3 : ligne jaune 
            y0 = VEC_AND_9_EPI8(dd8, d9, dd10, ee8, e9, ee10, ff8, f9, ff10);
            VEC_STORE_2D_EPI8(y0, i, j, tmp2);

            // DILATAION 5
            // ligne 3 autour de la colonne 1
            ee3 = VEC_LOAD_2D_EPI8(i , j - 2, tmp2);
            e5  = VEC_LOAD_2D_EPI8(i , j - 1, tmp2);
            ee3 = VEC_LEFT2_EPI8(ee3, e5);
            ee4 = VEC_LEFT1_EPI8(ee3, e5);
            ee7 = VEC_LOAD_2D_EPI8(i , j    , tmp2);
            ee6 = VEC_RIGHT1_EPI8(e5, ee7);
            ee7 = VEC_RIGHT2_EPI8(e5, ee7);

            // CALCUL
            // dilatation 5 : ligne bleue
            y1 = VEC_OR_25_EPI8(aa3, aa4, a5, aa6, aa7, bb3, bb4, b5, bb6, bb7, cc3, cc4, c5, cc6, cc7, dd3, dd4, d5, dd6, dd7, ee3, ee4, e5, ee6, ee7);
            VEC_STORE_2D_EPI8(y1, i - 2, j - 1, tmp1);

            // EROSION 3 - 2
            // ligne 1, autour de la colonne 0
            cc0 = VEC_LOAD_2D_EPI8(i - 2, j - 3, tmp1);
            c1  = VEC_LOAD_2D_EPI8(i - 2, j - 2, tmp1);
            cc0 = VEC_LEFT1_EPI8(cc0, c1);
            cc2 = VEC_LOAD_2D_EPI8(i - 2, j - 1, tmp1);
            cc2 = VEC_RIGHT1_EPI8(c1, cc2);            

            // CALCUL
            // 2e erosion 3 : ligne rouge 
            y2 = VEC_AND_9_EPI8(aa0, a1, aa2, bb0, b1, bb2, cc0, c1, cc2);
            VEC_STORE_2D_EPI8(y2, i - 3, j - 2, vY);

            // ROTATION DE REGISTRE  

            // colonne 0   
            a0 = a1 ; a1 = a2 ;
            b0 = b1 ; b1 = b2 ;

            // colonne 1
            a3 = a5 ; a5 = a7 ; 
            b3 = b5 ; b5 = b7 ;
            c3 = c5 ; c5 = c7 ;
            d3 = d5 ; d5 = d7 ;

            // colonne 2
            d8 = d9 ; d9 = d10 ;
            e8 = e9 ; e9 = e10 ;
        }

        // EPILOGUE BOUCLE

        // printf("3\n");

        // dilataion 5 sur l'avant avant derniere ligne et la derniere colonne de l'iteration courante
        dilatation_5_SIMD_opti(tmp2, tmp1, i - 2, i - 2, vmj1, vmj1);

        // printf("\n\nvmi1 - 3 = %d, vmi1 = %d, j - 2 = %d, j - 2 = %d\n\n", vmi1 - 3, vmi1, j - 2, j - 2);

        // printf("4\n");

        // erosion 3 sur l'avant avant avant derniere ligne et les 2 dernières colonnes de l'iteration courante
        erosion_3_SIMD_opti(tmp1, vY, i - 3, i - 3, vmj1 - 1, vmj1);
    }

    // EPILOGUE GLOBAL

    // printf("5\n");

    // dilatation 5 sur les 2 dernieres lignes
    dilatation_5_SIMD_opti(tmp2, tmp1, vmi1 - 1, vmi1, vmj0, vmj1);

    // printf("6\n");

    // erosion 3 sur les 3 dernieres lignes
    erosion_3_SIMD_opti(tmp1, vY, vmi1 - 2, vmi1, vmj0, vmj1);
}

