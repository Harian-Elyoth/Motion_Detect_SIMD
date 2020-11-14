/*
==========================================================
==================== Morpho Mathematiques ================
==========================================================
*/

//=============== INCLUDES ===============================

#include "morpho.h"

// Kernel 3x3
// indice scalaire sans bord
void erosion_3(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    uint8 a0, a1, a2;
    uint8 b0, b1, b2;
    uint8 c0, c1, c2;
    uint8 s;

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1
            a0 = X[i - 1][j - 1]; a1 = X[i - 1][j ]; a2 = X[i - 1][j + 1];
            b0 = X[i ][j - 1]; b1 = X[i ][j ]; b2 = X[i ][j + 1];
            c0 = X[i + 1][j - 1]; c1 = X[i + 1][j ]; c2 = X[i + 1][j + 1];
            s = a0 && a1 && a2 && b0 && b1 && b2 && c0 && c1 && c2;

            Y[i][j] = s;
            
        }
    }
}

void erosion_3_opti(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    uint8 a0, a1, a2, a3, a4;
    uint8 b0, b1, b2, b3, b4;
    uint8 c0, c1, c2, c3, c4;
    uint8 r0, r1, r2, r3, r4; //registre de resultats pour la lisibilité
    uint8 s0, s1, s2;

    int i, j;

    int k = 3;

    int r = mj1 % k;

    for(i = mi0 ; i <= mi1 ; i++){
        //printf("i : %d\n", i);
        j = mj0;

        a0 = X[i - 1][j - 1]; a1 = X[i - 1][j + 0];
        b0 = X[i + 0][j - 1]; b1 = X[i + 0][j + 0];
        c0 = X[i + 1][j - 1]; c1 = X[i + 1][j + 0];
          

        for(j = mj0 ; j <= mj1 - r - 1; j = j + k){
            //printf("j : %d\n", j);
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1
            a2 = X[i - 1][j + 1]; a3 = X[i - 1][j + 2]; a4 = X[i - 1][j + 3];
            b2 = X[i + 0][j + 1]; b3 = X[i + 0][j + 2]; b4 = X[i + 0][j + 3];
            c2 = X[i + 1][j + 1]; c3 = X[i + 1][j + 2]; c4 = X[i + 1][j + 3];
              

            
            r0 = a0 && b0 && c0;
            r1 = a1 && b1 && c1;
            r2 = a2 && b2 && c2;
            
            s0 = r0 && r1 && r2;

            r3 = a3 && b3 && c3;

            s1 = r1 && r2 && r3;

            r4 = a4 && b4 && c4;

            s2 = r2 && r3 && r4;

            Y[i][j + 0] = s0;
            Y[i][j + 1] = s1;
            Y[i][j + 2] = s2;

            a0 = a3; a1 = a4;
            b0 = b3; b1 = b4;
            c0 = c3; c1 = c4;

        }

        switch(r) {
            case 1 :
                a2 = X[i - 1][j + 1];
                b2 = X[i + 0][j + 1];
                c2 = X[i + 1][j + 1];

                r2 = a2 && b2 && c2;

                s0 = r0 && r1 && r2;

                Y[i][mj1] = s0;
                break;

            case 2 :

                a2 = X[i - 1][j + 1]; a3 = X[i - 1][j + 2];
                b2 = X[i + 0][j + 1]; b3 = X[i + 0][j + 2]; 
                c2 = X[i + 1][j + 1]; c3 = X[i + 1][j + 2];

                r2 = a2 && b2 && c2;
                r3 = a3 && b3 && c3;

                s0 = r0 && r1 && r2;
                s1 = r1 && r2 && r3;


                Y[i][mj1 - 1] = s0;
                Y[i][mj1] = s1;

            default :
                break;

        }

    }
}

// Kernel 5x5
// indice scalaire sans bord
void erosion_5(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    uint8 a0, a1, a2, a3, a4;
    uint8 b0, b1, b2, b3, b4;
    uint8 c0, c1, c2, c3, c4;
    uint8 d0, d1, d2, d3, d4;
    uint8 e0, e1, e2, e3, e4;
    uint8 s;

    int k = 5;

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){

            a0 = X[i - 2][j - 2] ; a1 = X[i - 2][j - 1] ; a2 = X[i - 2][j ] ; a3 = X[i - 2][j + 1]; a4 = X[i - 2][j + 2];
            b0 = X[i - 1][j - 2]; b1 = X[i - 1][j - 1]; b2 = X[i - 1][j ]; b3 = X[i - 1][j + 1]; b4 = X[i - 1][j + 2];
            c0 = X[i ][j - 2]; c1 = X[i ][j - 1]; c2 = X[i ][j ]; c3 = X[i ][j + 1]; c4 = X[i ][j + 1];
            d0 = X[i + 1][j - 2]; d1 = X[i + 1][j - 1]; d2 = X[i + 1][j]; d3 = X[i + 1][j + 1] ; d4 = X[i + 2][j + 2];
            e0 = X[i + 2][j - 2] ; e1 = X[i + 2][j - 1]; e2 = X[i + 2][j]; e3 = X[i + 2][j + 1]; e4 = X[i + 2][j + 2];
            s = a0 && a1 && a2 && a3 && a4 && b0 && b1 && b2 && b3 && b4 && c0 && c1 && c2 && c3 && c4 && d0 && d1 && d2 && d3 && d4 && e0 && e1 && e2 && e3 && e4;
            Y[i][j] = s;
        }
    }
}

void erosion_5_opti(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    uint8 a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
    uint8 b0, b1, b2, b3, b4, b5, b6, b7, b8, b9;
    uint8 c0, c1, c2, c3, c4, c5, c6, c7, c8, c9;
    uint8 d0, d1, d2, d3, d4, d5, d6, d7, d8, d9;
    uint8 e0, e1, e2, e3, e4, e5, e6, e7, e8, e9;

    uint8 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9; 
    uint8 s0, s1, s2, s3, s4;
    int i, j;
    int k = 5;
    int r = mj1 % k;
    for(i = mi0 ; i <= mi1 ; i++){

        j = mj0;
        a0 = X[i - 2][j - 2]; a1 = X[i - 2][j - 1]; a2 = X[i - 2][j + 0]; a3 = X[i - 2][j + 1]; 
        b0 = X[i - 1][j - 2]; b1 = X[i - 1][j - 1]; b2 = X[i - 1][j + 0]; b3 = X[i - 1][j + 1]; 
        c0 = X[i + 0][j - 2]; c1 = X[i + 0][j - 1]; c2 = X[i + 0][j + 0]; c3 = X[i + 0][j + 1]; 
        d0 = X[i + 1][j - 2]; d1 = X[i + 1][j - 1]; d2 = X[i + 1][j + 0]; d3 = X[i + 1][j + 1];
        e0 = X[i + 2][j - 2]; e1 = X[i + 2][j - 1]; e2 = X[i + 2][j + 0]; e3 = X[i + 2][j + 1];

        for(j = mj0 ; j <= mj1 - r - 1; j = j + 5){

            a4 = X[i - 2][j + 2]; a5 = X[i - 2][j + 3]; a6 = X[i - 2][j + 4]; a7 = X[i - 2][j + 5]; a8 = X[i - 2][j + 6];
            b4 = X[i - 1][j + 2]; b5 = X[i - 1][j + 3]; b6 = X[i - 1][j + 4]; b7 = X[i - 1][j + 5]; b8 = X[i - 1][j + 6];
            c4 = X[i + 0][j + 2]; c5 = X[i + 0][j + 3]; c6 = X[i + 0][j + 4]; c7 = X[i + 0][j + 5]; c8 = X[i + 0][j + 6];
            d4 = X[i + 1][j + 2]; d5 = X[i + 1][j + 3]; d6 = X[i + 1][j + 4]; d7 = X[i + 1][j + 5]; d8 = X[i + 1][j + 6];
            e4 = X[i + 2][j + 2]; e5 = X[i + 2][j + 3]; e6 = X[i + 2][j + 4]; e7 = X[i + 2][j + 5]; e8 = X[i + 2][j + 6];
                           
               

            r0 = a0 && b0 && c0 && d0 && e0;
            r1 = a1 && b1 && c1 && d1 && e1;
            r2 = a2 && b2 && c2 && d2 && e2;
            r3 = a3 && b3 && c3 && d3 && e3;
            r4 = a4 && b4 && c4 && d4 && e4;

            s0 = r0 && r1 && r2 && r3 && r4;


            r5 = a5 && b5 && c5 && d5 && e5;

            s1 = r1 && r2 && r3 && r4 && r5;


            r6 = a6 && b6 && c6 && d6 && e6;

            s2 = r2 && r3 && r4 && r5 && r6;


            r7 = a7 && b7 && c7 && d7 && e7;

            s3 = r3 && r4 && r5 && r6 && r7;


            r8 = a8 && b8 && c8 && d8 && e8;

            s4 = r4 && r5 && r6 && r7 && r8;

            Y[i][j + 0] = s0;
            Y[i][j + 1] = s1;
            Y[i][j + 2] = s2;
            Y[i][j + 3] = s3;
            Y[i][j + 4] = s4;

            a0 = a5; a1 = a6; a2 = a7; a3 = a8;
            b0 = b5; b1 = b6; b2 = b7; b3 = b8;
            c0 = c5; c1 = c6; c2 = c7; c3 = c8;
            d0 = d5; d1 = d6; d2 = d7; d3 = d8;
            e0 = e5; e1 = e6; e2 = e7; e3 = e8;
        }

               switch(r) {
           case 1 :

                a4 = X[i - 2][j + 2];
                b4 = X[i - 1][j + 2];
                c4 = X[i + 0][j + 2];
                d4 = X[i + 1][j + 2];
                e4 = X[i + 2][j + 2];

                r4 = a4 && b4 && c4 && d4 && e4;

                s0 = r0 && r1 && r2 && r3 && r4;

                Y[i][mj1] = s0;

                break;

           case 2 :

                a5 = X[i - 2][j + 3];
                b5 = X[i - 1][j + 3];
                c5 = X[i + 0][j + 3];
                d5 = X[i + 1][j + 3];
                e5 = X[i + 2][j + 3];

                a4 = X[i - 2][j + 2];
                b4 = X[i - 1][j + 2];
                c4 = X[i + 0][j + 2];
                d4 = X[i + 1][j + 2];
                e4 = X[i + 2][j + 2];

                r4 = a4 && b4 && c4 && d4 && e4;
                r5 = a5 && b5 && c5 && d5 && e5;

                s0 = r0 && r1 && r2 && r3 && r4;
                s1 = r1 && r2 && r3 && r4 && r5;

                Y[i][mj1 - 1] = s0;
                Y[i][mj1] = s1;

                break;

           case 3 :

                a5 = X[i - 2][j + 3];
                b5 = X[i - 1][j + 3];
                c5 = X[i + 0][j + 3];
                d5 = X[i + 1][j + 3];
                e5 = X[i + 2][j + 3];

                a4 = X[i - 2][j + 2];
                b4 = X[i - 1][j + 2];
                c4 = X[i + 0][j + 2];
                d4 = X[i + 1][j + 2];
                e4 = X[i + 2][j + 2];

                a6 = X[i - 2][j + 4];
                b6 = X[i - 1][j + 4];
                c6 = X[i + 0][j + 4];
                d6 = X[i + 1][j + 4];
                e6 = X[i + 2][j + 4];

                r4 = a4 && b4 && c4 && d4 && e4;
                r5 = a5 && b5 && c5 && d5 && e5;
                r6 = a6 && b6 && c6 && d6 && e6;

                s0 = r0 && r1 && r2 && r3 && r4;
                s1 = r1 && r2 && r3 && r4 && r5;
                s2 = r2 && r3 && r4 && r5 && r6;

                Y[i][mj1 - 2] = s0;
                Y[i][mj1 - 1] = s1;
                Y[i][mj1] = s3;

                break;
                
           case 4 :

                a5 = X[i - 2][j + 3];
                b5 = X[i - 1][j + 3];
                c5 = X[i + 0][j + 3];
                d5 = X[i + 1][j + 3];
                e5 = X[i + 2][j + 3];

                a4 = X[i - 2][j + 2];
                b4 = X[i - 1][j + 2];
                c4 = X[i + 0][j + 2];
                d4 = X[i + 1][j + 2];
                e4 = X[i + 2][j + 2];

                a6 = X[i - 2][j + 4];
                b6 = X[i - 1][j + 4];
                c6 = X[i + 0][j + 4];
                d6 = X[i + 1][j + 4];
                e6 = X[i + 2][j + 4];

                a7 = X[i - 2][j + 5];
                b7 = X[i - 1][j + 5];
                c7 = X[i + 0][j + 5];
                d7 = X[i + 1][j + 5];
                e7 = X[i + 2][j + 5];

                r4 = a4 && b4 && c4 && d4 && e4;
                r5 = a5 && b5 && c5 && d5 && e5;
                r6 = a6 && b6 && c6 && d6 && e6;
                r7 = a7 && b7 && c7 && d7 && e7;


                s0 = r0 && r1 && r2 && r3 && r4;
                s1 = r1 && r2 && r3 && r4 && r5;
                s2 = r2 && r3 && r4 && r5 && r6;  
                s3 = r3 && r4 && r5 && r6 && r7;
              
                Y[i][mj1 - 3] = s0;
                Y[i][mj1 - 2] = s1;
                Y[i][mj1 - 1] = s2;
                Y[i][mj1 - 0] = s3;

            default :
                break;
       }
        
    }
}

// Kernel 3x3
// indice scalaire sans bord
void dilatation_3(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    uint8 a0, a1, a2;
    uint8 b0, b1, b2;
    uint8 c0, c1, c2;
    uint8 s;

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){
            
            a0 = X[i - 1][j - 1]; a1 = X[i - 1][j ]; a2 = X[i - 1][j + 1];
            b0 = X[i ][j - 1]; b1 = X[i ][j ]; b2 = X[i ][j + 1];
            c0 = X[i + 1][j - 1]; c1 = X[i + 1][j ]; c2 = X[i + 1][j + 1];
            s = a0 || a1 || a2 || b0 || b1 || b2 || c0 || c1 || c2;
            Y[i][j] = s;
        }
    }
}

void dilatation_3_opti(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    uint8 a0, a1, a2, a3, a4;
    uint8 b0, b1, b2, b3, b4;
    uint8 c0, c1, c2, c3, c4;
    uint8 r0, r1, r2, r3, r4; //registre de resultats pour la lisibilité
    uint8 s0, s1, s2;

    int i, j;

    int k = 3;

    int r = mj1 % k;

    for(i = mi0 ; i <= mi1 ; i++){
        
        j = mj0;

        a0 = X[i - 1][j - 1]; a1 = X[i - 1][j + 0];
        b0 = X[i + 0][j - 1]; b1 = X[i + 0][j + 0];
        c0 = X[i + 1][j - 1]; c1 = X[i + 1][j + 0];

        

        for(j = mj0 ; j <= mj1 - r - 1; j = j + 3){

            a2 = X[i - 1][j + 1]; a3 = X[i - 1][j + 2]; a4 = X[i - 1][j + 3];
            b2 = X[i + 0][j + 1]; b3 = X[i + 0][j + 2]; b4 = X[i + 0][j + 3];
            c2 = X[i + 1][j + 1]; c3 = X[i + 1][j + 2]; c4 = X[i + 1][j + 3];

            r0 = a0 || b0 || c0;
            r1 = a1 || b1 || c1;
            r2 = a2 || b2 || c2;
            r3 = a3 || b3 || c3;
            r4 = a4 || b4 || c4;

            s0 = r0 || r1 || r2;
            s1 = r1 || r2 || r3;
            s2 = r2 || r3 || r4;

            Y[i][j + 0] = s0;
            Y[i][j + 1] = s1;
            Y[i][j + 2] = s2;

            a0 = a3; a1 = a4;
            b0 = b3; b1 = b4;
            c0 = c3; c1 = c4;

        }
        switch(r) {
            case 1 :
                a2 = X[i - 1][j + 1];
                b2 = X[i + 0][j + 1];
                c2 = X[i + 1][j + 1];

                r2 = a2 || b2 || c2;

                s0 = r0 || r1 || r2;

                Y[i][mj1] = s0;
                break;

            case 2 :

                a2 = X[i - 1][j + 1]; a3 = X[i - 1][j + 2];
                b2 = X[i + 0][j + 1]; b3 = X[i + 0][j + 2]; 
                c2 = X[i + 1][j + 1]; c3 = X[i + 1][j + 2];

                r2 = a2 || b2 || c2;
                r3 = a3 || b3 || c3;

                s0 = r0 || r1 || r2;
                s1 = r1 || r2 || r3;


                Y[i][mj1 - 1] = s0;
                Y[i][mj1] = s1;

            default :
                break;
                
        }

    }


}

// Kernel 5x5
// indice scalaire sans bord
void dilatation_5(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    uint8 a0, a1, a2, a3, a4;
    uint8 b0, b1, b2, b3, b4;
    uint8 c0, c1, c2, c3, c4;
    uint8 d0, d1, d2, d3, d4;
    uint8 e0, e1, e2, e3, e4;
    uint8 s;

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){

            a0 = X[i - 2][j - 2] ; a1 = X[i - 2][j - 1] ; a2 = X[i - 2][j ] ; a3 = X[i - 2][j + 1]; a4 = X[i - 2][j + 2];
            b0 = X[i - 1][j - 2]; b1 = X[i - 1][j - 1]; b2 = X[i - 1][j ]; b3 = X[i - 1][j + 1]; b4 = X[i - 1][j + 2];
            c0 = X[i ][j - 2]; c1 = X[i ][j - 1]; c2 = X[i ][j ]; c3 = X[i ][j + 1]; c4 = X[i ][j + 1];
            d0 = X[i + 1][j - 2]; d1 = X[i + 1][j - 1]; d2 = X[i + 1][j]; d3 = X[i + 1][j + 1] ; d4 = X[i + 2][j + 2];
            e0 = X[i + 2][j - 2] ; e1 = X[i + 2][j - 1]; e2 = X[i + 2][j]; e3 = X[i + 2][j + 1]; e4 = X[i + 2][j + 2];
            s = a0 || a1 || a2 || a3 || a4 || b0 || b1 || b2 || b3 || b4 || c0 || c1 || c2 || c3 || c4 || d0 || d1 || d2 || d3 || d4 || e0 || e1 || e2 || e3 || e4;
            Y[i][j] = s;

        }
    }
}

void dilatation_5_opti(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    uint8 a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
    uint8 b0, b1, b2, b3, b4, b5, b6, b7, b8, b9;
    uint8 c0, c1, c2, c3, c4, c5, c6, c7, c8, c9;
    uint8 d0, d1, d2, d3, d4, d5, d6, d7, d8, d9;
    uint8 e0, e1, e2, e3, e4, e5, e6, e7, e8, e9;

    uint8 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9; 
    uint8 s0, s1, s2, s3, s4;
    int i, j;
    int k = 5;
    int r = mj1 % k;

    for(i = mi0 ; i <= mi1 ; i++){

        j = mj0;
        a0 = X[i - 2][j - 2]; a1 = X[i - 2][j - 1]; a2 = X[i - 2][j + 0]; a3 = X[i - 2][j + 1]; 
        b0 = X[i - 1][j - 2]; b1 = X[i - 1][j - 1]; b2 = X[i - 1][j + 0]; b3 = X[i - 1][j + 1]; 
        c0 = X[i + 0][j - 2]; c1 = X[i + 0][j - 1]; c2 = X[i + 0][j + 0]; c3 = X[i + 0][j + 1]; 
        d0 = X[i + 1][j - 2]; d1 = X[i + 1][j - 1]; d2 = X[i + 1][j + 0]; d3 = X[i + 1][j + 1];
        e0 = X[i + 2][j - 2]; e1 = X[i + 2][j - 1]; e2 = X[i + 2][j + 0]; e3 = X[i + 2][j + 1];

        for(j = mj0 ; j <= mj1 - r - 1; j = j + 5){

            a4 = X[i - 2][j + 2]; a5 = X[i - 2][j + 3]; a6 = X[i - 2][j + 4]; a7 = X[i - 2][j + 5]; a8 = X[i - 2][j + 6];
            b4 = X[i - 1][j + 2]; b5 = X[i - 1][j + 3]; b6 = X[i - 1][j + 4]; b7 = X[i - 1][j + 5]; b8 = X[i - 1][j + 6];
            c4 = X[i + 0][j + 2]; c5 = X[i + 0][j + 3]; c6 = X[i + 0][j + 4]; c7 = X[i + 0][j + 5]; c8 = X[i + 0][j + 6];
            d4 = X[i + 1][j + 2]; d5 = X[i + 1][j + 3]; d6 = X[i + 1][j + 4]; d7 = X[i + 1][j + 5]; d8 = X[i + 1][j + 6];
            e4 = X[i + 2][j + 2]; e5 = X[i + 2][j + 3]; e6 = X[i + 2][j + 4]; e7 = X[i + 2][j + 5]; e8 = X[i + 2][j + 6];

            r0 = a0 || b0 || c0 || d0 || e0;
            r1 = a1 || b1 || c1 || d1 || e1;
            r2 = a2 || b2 || c2 || d2 || e2;
            r3 = a3 || b3 || d3 || c3 || e3;
            r4 = a4 || b4 || c4 || d4 || e4;
            r5 = a5 || b5 || c5 || d5 || e5;
            r6 = a6 || b6 || c6 || d6 || e6;
            r7 = a7 || b7 || c7 || d7 || e7;
            r8 = a8 || b8 || c8 || d8 || e8;

            s0 = r0 || r1 || r2 || r3 || r4;
            s1 = r1 || r2 || r3 || r4 || r5;
            s2 = r2 || r3 || r4 || r5 || r6;
            s3 = r3 || r4 || r5 || r6 || r7;
            s4 = r4 || r5 || r6 || r7 || r8;

            Y[i][j + 0] = s0;
            Y[i][j + 1] = s1;
            Y[i][j + 2] = s2;
            Y[i][j + 3] = s3;
            Y[i][j + 4] = s4;

            a0 = a5; a1 = a6; a2 = a7; a3 = a8;
            b0 = b5; b1 = b6; b2 = b7; b3 = b8;
            c0 = c5; c1 = c6; c2 = c7; c3 = c8;
            d0 = d5; d1 = d6; d2 = d7; d3 = d8;
            e0 = e5; e1 = e6; e2 = e7; e3 = e8;
        }

       switch(r) {
           case 1 :

                a4 = X[i - 2][j + 2];
                b4 = X[i - 1][j + 2];
                c4 = X[i + 0][j + 2];
                d4 = X[i + 1][j + 2];
                e4 = X[i + 2][j + 2];

                r4 = a4 || b4 || c4 || d4 || e4;

                s0 = r0 || r1 || r2 || r3 || r4;

                Y[i][mj1] = s0;

                break;

           case 2 :

                a5 = X[i - 2][j + 3];
                b5 = X[i - 1][j + 3];
                c5 = X[i + 0][j + 3];
                d5 = X[i + 1][j + 3];
                e5 = X[i + 2][j + 3];

                a4 = X[i - 2][j + 2];
                b4 = X[i - 1][j + 2];
                c4 = X[i + 0][j + 2];
                d4 = X[i + 1][j + 2];
                e4 = X[i + 2][j + 2];

                r4 = a4 || b4 || c4 || d4 || e4;
                r5 = a5 || b5 || c5 || d5 || e5;

                s0 = r0 || r1 || r2 || r3 || r4;
                s1 = r1 || r2 || r3 || r4 || r5;

                Y[i][mj1 - 1] = s0;
                Y[i][mj1] = s1;

                break;

           case 3 :

                a5 = X[i - 2][j + 3];
                b5 = X[i - 1][j + 3];
                c5 = X[i + 0][j + 3];
                d5 = X[i + 1][j + 3];
                e5 = X[i + 2][j + 3];

                a4 = X[i - 2][j + 2];
                b4 = X[i - 1][j + 2];
                c4 = X[i + 0][j + 2];
                d4 = X[i + 1][j + 2];
                e4 = X[i + 2][j + 2];

                a6 = X[i - 2][j + 4];
                b6 = X[i - 1][j + 4];
                c6 = X[i + 0][j + 4];
                d6 = X[i + 1][j + 4];
                e6 = X[i + 2][j + 4];

                r4 = a4 || b4 || c4 || d4 || e4;
                r5 = a5 || b5 || c5 || d5 || e5;
                r6 = a6 || b6 || c6 || d6 || e6;

                s0 = r0 || r1 || r2 || r3 || r4;
                s1 = r1 || r2 || r3 || r4 || r5;
                s2 = r2 || r3 || r4 || r5 || r6;

                Y[i][mj1 - 2] = s0;
                Y[i][mj1 - 1] = s1;
                Y[i][mj1] = s3;

                break;
                
           case 4 :

                a5 = X[i - 2][j + 3];
                b5 = X[i - 1][j + 3];
                c5 = X[i + 0][j + 3];
                d5 = X[i + 1][j + 3];
                e5 = X[i + 2][j + 3];

                a4 = X[i - 2][j + 2];
                b4 = X[i - 1][j + 2];
                c4 = X[i + 0][j + 2];
                d4 = X[i + 1][j + 2];
                e4 = X[i + 2][j + 2];

                a6 = X[i - 2][j + 4];
                b6 = X[i - 1][j + 4];
                c6 = X[i + 0][j + 4];
                d6 = X[i + 1][j + 4];
                e6 = X[i + 2][j + 4];

                a7 = X[i - 2][j + 5];
                b7 = X[i - 1][j + 5];
                c7 = X[i + 0][j + 5];
                d7 = X[i + 1][j + 5];
                e7 = X[i + 2][j + 5];

                r4 = a4 || b4 || c4 || d4 || e4;
                r5 = a5 || b5 || c5 || d5 || e5;
                r6 = a6 || b6 || c6 || d6 || e6;
                r7 = a7 || b7 || c7 || d7 || e7;


                s0 = r0 || r1 || r2 || r3 || r4;
                s1 = r1 || r2 || r3 || r4 || r5;
                s2 = r2 || r3 || r4 || r5 || r6;  
                s3 = r3 || r4 || r5 || r6 || r7;
              
                Y[i][mj1 - 3] = s0;
                Y[i][mj1 - 2] = s1;
                Y[i][mj1 - 1] = s2;
                Y[i][mj1 - 0] = s3;

            default :
                break;
       }
    }
}

void morpho_3(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    int mi0b = mi0 - 1;
    int mi1b = mi1 + 1;
    int mj0b = mj0 - 1;
    int mj1b = mj1 + 1;

    uint8 ** tmp1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
    uint8 ** tmp2 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

    erosion_3(X, tmp1, mi0, mi1, mj0, mj1);
    dilatation_3(tmp1, tmp2, mi0, mi1, mj0, mj1);
    dilatation_3(tmp2, tmp1, mi0, mi1, mj0, mj1);
    erosion_3(tmp1, Y, mi0, mi1, mj0, mj1);

    free_ui8matrix(tmp1, mi0b, mi1b, mj0b, mj1b);
    free_ui8matrix(tmp2, mi0b, mi1b, mj0b, mj1b);
}

void morpho_3_opti(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){

    int mi0b = mi0 - 1;
    int mi1b = mi1 + 1;
    int mj0b = mj0 - 1;
    int mj1b = mj1 + 1;

    uint8 ** tmp1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
    uint8 ** tmp2 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

    erosion_3_opti(X, tmp1, mi0, mi1, mj0, mj1);
    dilatation_3_opti(tmp1, tmp2, mi0, mi1, mj0, mj1);
    dilatation_3_opti(tmp2, tmp1, mi0, mi1, mj0, mj1);
    erosion_3_opti(tmp1, Y, mi0, mi1, mj0, mj1);

    free_ui8matrix(tmp1, mi0b, mi1b, mj0b, mj1b);
    free_ui8matrix(tmp2, mi0b, mi1b, mj0b, mj1b);
}

void morpho_5(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){
    
    int mi0b = mi0 - 2;
    int mi1b = mi1 + 2;
    int mj0b = mj0 - 2;
    int mj1b = mj1 + 2;

    uint8 ** tmp1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
    uint8 ** tmp2 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

    erosion_5(X, tmp1, mi0, mi1, mj0, mj1);
    dilatation_5(tmp1, tmp2, mi0, mi1, mj0, mj1);
    dilatation_5(tmp2, tmp1, mi0, mi1, mj0, mj1);
    erosion_5(tmp1, Y, mi0, mi1, mj0, mj1);

    free_ui8matrix(tmp1, mi0b, mi1b, mj0b, mj1b);
    free_ui8matrix(tmp2, mi0b, mi1b, mj0b, mj1b);
}

void morpho_5_opti(uint8 ** X, uint8 ** Y, int mi0, int mi1, int mj0, int mj1){
    
    int mi0b = mi0 - 2;
    int mi1b = mi1 + 2;
    int mj0b = mj0 - 2;
    int mj1b = mj1 + 2;

    uint8 ** tmp1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
    uint8 ** tmp2 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

    erosion_5_opti(X, tmp1, mi0, mi1, mj0, mj1);
    dilatation_5_opti(tmp1, tmp2, mi0, mi1, mj0, mj1);
    dilatation_5_opti(tmp2, tmp1, mi0, mi1, mj0, mj1);
    erosion_5_opti(tmp1, Y, mi0, mi1, mj0, mj1);

    free_ui8matrix(tmp1, mi0b, mi1b, mj0b, mj1b);
    free_ui8matrix(tmp2, mi0b, mi1b, mj0b, mj1b);
}

