/*
==========================================================
==================== Morpho Mathematiques ================
==========================================================
*/

//=============== INCLUDES ===============================

#include <stdio.h>
#include <stdlib.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"

#include "morpho.h"

//==================== MACROS ============================

//IMAGE_SIZE
#define SIZE_X 320
#define SIZE_Y 240

//======================= PROGRAM =======================

int b;

int mx0, mx1, my0, my1;     //indices scalaire
int mx0b, mx1b, my0b, my1b; // indices scalaires avec bord


mx0 = 0; mx1 = SIZE_X-1;
my0 = 0; my1 = SIZE_Y-1;


// Kernel 3x3
// indice scalaire sans bord
void erosion_3(uint8 ** X, uint8 ** Y){

    uint8 a0, a1, a2;
    uint8 b0, b1, b2;
    uint8 c0, c1, c2;
    uint8 s;

    for(int i = mx0 ; i < mx1 ; i++){
        for(int j = my0 ; j < my1 ; j++){
            
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
void erosion_5(uint8 ** X, uint8 ** Y){

    uint8 a0, a1, a2, a3, a4;
    uint8 b0, b1, b2, b3, b4;
    uint8 c0, c1, c2, c3, c4;
    uint8 d0, d1, d2, d3, d4;
    uint8 e0, e1, e2, e3, e4;
    uint8 s;

    for(int i = mx0 ; i < mx1 ; i++){
        for(int j = my0 ; j < my1 ; j++){

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
void dilatation_3(uint8 ** X, uint8 ** Y){

    uint8 a0, a1, a2;
    uint8 b0, b1, b2;
    uint8 c0, c1, c2;
    uint8 s;

    for(int i = mx0 ; i < mx1 ; i++){
        for(int j = my0 ; j < my1 ; j++){
            
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
void dilatation_5(uint8 ** X, uint8 ** Y){

    uint8 a0, a1, a2, a3, a4;
    uint8 b0, b1, b2, b3, b4;
    uint8 c0, c1, c2, c3, c4;
    uint8 d0, d1, d2, d3, d4;
    uint8 e0, e1, e2, e3, e4;
    uint8 s;

    for(int i = mx0 ; i < mx1 ; i++){
        for(int j = my0 ; j < my1 ; j++){

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

void morpho_3(uint8 ** X, uint8 ** Y){

    b = 1;

    mx0b = mx0-b; mx1b = mx1+b;
	my0b = my0-b; my1b = my1+b;

    uint8 ** tmp1 = ui8matrix(mx0b, mx1b, my0b, my1b);
    uint8 ** tmp2 = ui8matrix(mx0b, mx1b, my0b, my1b);

    erosion_3(X, tmp1);
    dilatation_3(tmp1, tmp2);
    erosion_3(tmp2, tmp1);
    dilatation_3(tmp1, Y);
}

void morpho_5(uint8 ** X, uint8 ** Y){
    
    b = 2;

    mx0b = mx0-b; mx1b = mx1+b;
	my0b = my0-b; my1b = my1+b;

    uint8 ** tmp1 = ui8matrix(mx0b, mx1b, my0b, my1b);
    uint8 ** tmp2 = ui8matrix(mx0b, mx1b, my0b, my1b);

    erosion_5(X, tmp1);
    dilatation_5(tmp1, tmp2);
    erosion_5(tmp2, tmp1);
    dilatation_5(tmp1, Y);
}


void main_morpho(int argc, char *argv[])
{



    //test_morphos(X, TEST_X_SIZE);


}