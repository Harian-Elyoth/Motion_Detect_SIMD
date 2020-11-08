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
            a0 = X[i - 1][j - 1]; b0 = X[i - 1][j ]; c0 = X[i - 1][j + 1];
            a1 = X[i ][j - 1]; b1 = X[i ][j ]; c1 = X[i ][j + 1];
            a2 = X[i + 1][j - 1]; b2 = X[i + 1][j ]; c2 = X[i + 1][j + 1];
            s = a0 && a1 && a2 && b0 && b1 && b2 && c0 && c1 && c2;
            Y[i][j] = s;
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

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){

            a0 = X[i - 2][j - 2] ; b0 = X[i - 2][j - 1] ; c0 = X[i - 2][j ] ; d0 = X[i - 2][j + 1]; e0 = X[i - 2][j + 2];
            a1 = X[i - 1][j - 2]; b1 = X[i - 1][j - 1]; c1 = X[i - 1][j ]; d1 = X[i - 1][j + 1]; e1 = X[i - 1][j + 2];
            a2 = X[i ][j - 2]; b2 = X[i ][j - 1]; c2 = X[i ][j ]; d2 = X[i ][j + 1]; e2 = X[i ][j + 1];
            a3 = X[i + 1][j - 2]; b3 = X[i + 1][j - 1]; c3 = X[i + 1][j]; d3 = X[i + 1][j + 1] ; e3 = X[i + 2][j + 2];
            a4 = X[i + 2][j - 2] ; b4 = X[i + 2][j - 1]; c4 = X[i + 2][j]; d4 = X[i + 2][j + 1]; e4 = X[i + 2][j + 2];
            s = a0 && a1 && a2 && a3 && a4 && b0 && b1 && b2 && b3 && b4 && c0 && c1 && c2 && c3 && c4 && d0 && d1 && d2 && d3 && d4 && e0 && e1 && e2 && e3 && e4;
            Y[i][j] = s;
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1

            //Y[i][j] = (X[i - 1][j - 1] && X[i - 1][j] && X[i - 1][j + 1] && X[i][j - 1] && X[i][j] && X[i][j + 1] && X[i + 1][j - 1] && X[i + 1][j] && X[i + 1][j + 1]);
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
            
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1
            a0 = X[i - 1][j - 1]; b0 = X[i - 1][j ]; c0 = X[i - 1][j + 1];
            a1 = X[i ][j - 1]; b1 = X[i ][j ]; c1 = X[i ][j + 1];
            a2 = X[i + 1][j - 1]; b2 = X[i + 1][j ]; c2 = X[i + 1][j + 1];
            s = a0 || a1 || a2 || b0 || b1 || b2 || c0 || c1 || c2;
            Y[i][j] = s;
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

            a0 = X[i - 2][j - 2] ; b0 = X[i - 2][j - 1] ; c0 = X[i - 2][j ] ; d0 = X[i - 2][j + 1]; e0 = X[i - 2][j + 2];
            a1 = X[i - 1][j - 2]; b1 = X[i - 1][j - 1]; c1 = X[i - 1][j ]; d1 = X[i - 1][j + 1]; e1 = X[i - 1][j + 2];
            a2 = X[i ][j - 2]; b2 = X[i ][j - 1]; c2 = X[i ][j ]; d2 = X[i ][j + 1]; e2 = X[i ][j + 1];
            a3 = X[i + 1][j - 2]; b3 = X[i + 1][j - 1]; c3 = X[i + 1][j]; d3 = X[i + 1][j + 1] ; e3 = X[i + 2][j + 2];
            a4=X[i + 2][j - 2] ; b4 = X[i + 2][j - 1]; c4 = X[i + 2][j]; d4 = X[i + 2][j + 1]; e4 = X[i + 2][j + 2];
            s = a0 || a1 || a2 || a3 || a4 || b0 || b1 || b2 || b3 || b4 || c0 || c1 || c2 || c3 || c4 || d0 || d1 || d2 || d3 || d4 || e0 || e1 || e2 || e3 || e4;
            Y[i][j] = s;
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1

            //Y[i][j] = (X[i - 1][j - 1] && X[i - 1][j] && X[i - 1][j + 1] && X[i][j - 1] && X[i][j] && X[i][j + 1] && X[i + 1][j - 1] && X[i + 1][j] && X[i + 1][j + 1]);
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
