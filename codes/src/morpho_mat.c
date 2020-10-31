/*
==========================================================
==================== Morpho Mathematiques ================
==========================================================
*/

//=============== INCLUDES ===============================

#include <stdio.h>
#include <stdlib.h>
/*
#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"
*/
//==================== MACROS ============================

#define TEST_X_SIZE 10
#define MatPrint(X, H, W) for(uint8 i = 0 ; i < H ; i++){ for(uint8 j = 0 ; j < W ; j++){ printf("%d\t", X[i][j]); } printf("\n");}
#define IntMatAlloc(X, H, W) uint8 ** X = (uint8 **)malloc(sizeof(uint8 *) * H);for(uint8 i = 0 ; i < H ; i++){X[i] = (uint8 *)malloc(sizeof(uint8) * W);}
//======================= PROGRAM =======================

// Kernel 3x3
// indice scalaire sans bord
void erosion_3(uint8 ** X, uint8 ** Y){

    for(uint8 i = mx0 ; i < mx1 ; i++){
        for(uint8 j = my0 ; j < my1 ; j++){
            
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1

            Y[i][j] = (X[i - 1][j - 1] && X[i - 1][j] && X[i - 1][j + 1] && X[i][j - 1] && X[i][j] && X[i][j + 1] && X[i + 1][j - 1] && X[i + 1][j] && X[i + 1][j + 1]);
        }
    }
}

// Kernel 5x5
// indice scalaire sans bord
void erosion_5(uint8 ** X, uint8 ** Y){

    for(uint8 i = mx0 ; i < mx1 ; i++){
        for(uint8 j = my0 ; j < my1 ; j++){
            
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1

            //Y[i][j] = (X[i - 1][j - 1] && X[i - 1][j] && X[i - 1][j + 1] && X[i][j - 1] && X[i][j] && X[i][j + 1] && X[i + 1][j - 1] && X[i + 1][j] && X[i + 1][j + 1]);
        }
    }
}

void erosion(uint8 ** X, uint8 h, uint8 w, uint8 ** Y, uint8 kernel_size){
    uint8 padding;
    if(kernel_size == 3){
        padding = 1;
    }
    else if(kernel_size == 5){
        padding = 2;
    }
    else {
        printf("BAD KERNEL_SIZE, SIZE 3 OF 5 REQUIRED\n");
        exit(-1);
    }

    uint8 isZero = 0;
    for(uint8 i = padding ; i < h - padding ; i++){
        for(uint8 j = padding ; j < w - padding ; j++){
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1
            Y[i][j] = (X[i - 1][j - 1] && X[i - 1][j] && X[i - 1][j + 1] && X[i][j - 1] && X[i][j] && X[i][j + 1] && X[i + 1][j - 1] && X[i + 1][j] && X[i + 1][j + 1]);
            /*
            for(uint8 k = i - padding ; k < i - padding + kernel_size ; k++){
                for(uint8 l = j - padding ; l < j - padding + kernel_size ; l++){
                    isZero = !X[k][l];
                    if(isZero){
                        //Si il y a au moins 1 zero dans le kernel on met le pixel Yij à zero
                        
                        Y[i][j] = 0;
                        break;
                    }
                } //Ici on fait des break pour ne pas avoir a regarder le reste du kernel si on detecte 1 zero
                if(isZero){
                    break;
                }
            }
            if(!isZero){
                Y[i][j] = 1;
            }
            isZero = 0;
            */
        }
    }
}

// Kernel 3x3
// indice scalaire sans bord
void dilatation_3(uint8 ** X, uint8 ** Y){

    for(uint8 i = mx0 ; i < mx1 ; i++){
        for(uint8 j = my0 ; j < my1 ; j++){
            
            //Detection d'un UN sur le kernel autour du IJ
            //Si il y a un UN, dans le kernel alors le Yij prends 1, sinon il prends 0

            Y[i][j] = (X[i - 1][j - 1] || X[i - 1][j] || X[i - 1][j + 1] || X[i][j - 1] || X[i][j] || X[i][j + 1] || X[i + 1][j - 1] || X[i + 1][j] || X[i + 1][j + 1]);
        }
    }
}

// Kernel 5x()
// indice scalaire sans bord
void dilatation_5(uint8 ** X, uint8 ** Y){

    for(uint8 i = mx0 ; i < mx1 ; i++){
        for(uint8 j = my0 ; j < my1 ; j++){
            
            //Detection d'un UN sur le kernel autour du IJ
            //Si il y a un UN, dans le kernel alors le Yij prends 1, sinon il prends 0

            //Y[i][j] = (X[i - 1][j - 1] || X[i - 1][j] || X[i - 1][j + 1] || X[i][j - 1] || X[i][j] || X[i][j + 1] || X[i + 1][j - 1] || X[i + 1][j] || X[i + 1][j + 1]);
        }
    }
}

void dilatation(uint8 ** X, uint8 h, uint8 w, uint8 ** Y, uint8 kernel_size){
    uint8 padding;
    if(kernel_size == 3){
        padding = 1;
    }
    else if(kernel_size == 5){
        padding = 2;
    }
    else {
        printf("BAD KERNEL_SIZE, SIZE 3 OF 5 REQUIRED\n");
        exit(-1);
    }

    uint8 isOne = 0;
    for(uint8 i = padding ; i < h - padding ; i++){
        for(uint8 j = padding ; j < w - padding ; j++){
            //Detection d'un UN sur le kernel autour du IJ
            //Si il y a un UN, dans le kernel alors le Yij prends 1, sinon il prends 0
            Y[i][j] = (X[i - 1][j - 1] || X[i - 1][j] || X[i - 1][j + 1] || X[i][j - 1] || X[i][j] || X[i][j + 1] || X[i + 1][j - 1] || X[i + 1][j] || X[i + 1][j + 1]);

            /*
            for(uint8 k = i - padding ; k < i - padding + kernel_size ; k++){
                for(uint8 l = j - padding ; l < j - padding + kernel_size ; l++){
                    isOne = X[k][l];
                    if(isOne){
                        //Si il y a au moins 1 UN dans le kernel on met le pixel Yij à UN
                        Y[i][j] = 1;
                        break;
                    }
                } //Ici on fait des break pour ne pas avoir a regarder le reste du kernel si on detecte 1 UN
                if(isOne){
                    break;
                }
            }
            if(!isOne){
                Y[i][j] = 0;
            }
            isOne = 0;
            */
        }
    }
}

void morpho(uint8 ** X, uint8 ** Y){

    tmp1 = ui8matrix(mx0b, mx1b, my0b, my1b);
    tmp2 = ui8matrix(mx0b, mx1b, my0b, my1b);

    erosion_3(X, tmp1);
    dilatation_3(tmp1, tmp2);
    erosion_3(tmp2, tmp1);
    dilatation_3(tmp1, Y);

    // IntMatAlloc(TEMP1, h, w);
    // IntMatAlloc(TEMP2, h, w);

    // erosion(X, h, w, TEMP1, kernel_size);
    // dilatation(TEMP1, h, w, TEMP2, kernel_size);
    // dilatation(TEMP2, h, w, TEMP1, kernel_size);
    // erosion(TEMP1, h, w, Y, kernel_size);

}

void test_morphos(uint8 ** X, uint8 size){

    printf("================ MATRICE ==============\n");
    MatPrint(X, TEST_X_SIZE, TEST_X_SIZE);

    IntMatAlloc(Y, TEST_X_SIZE, TEST_X_SIZE);
    IntMatAlloc(Z, TEST_X_SIZE, TEST_X_SIZE);
    erosion(X, TEST_X_SIZE, TEST_X_SIZE, Y, 3);
    printf("================= EROSION ==============\n");
    MatPrint(Y, TEST_X_SIZE, TEST_X_SIZE);
    printf("================= DILATATION ===========\n");
    dilatation(X, TEST_X_SIZE, TEST_X_SIZE, Z, 3);
    MatPrint(Z, TEST_X_SIZE, TEST_X_SIZE);
}

uint8 main_morpho(uint8 argc, char const *argv[])
{
    IntMatAlloc(X, TEST_X_SIZE, TEST_X_SIZE);
    IntMatAlloc(Y, TEST_X_SIZE, TEST_X_SIZE);
    for(uint8 i = 0 ; i < TEST_X_SIZE ; i++){
        for(uint8 j = 0 ; j < TEST_X_SIZE ; j++){
            if((i == j || i == j - 1 || i == j + 1 || i == j - 2 || i == j + 2)){
                X[i][j] = 1;
            }
            else {
                X[i][j] = 0;
            }
        }
    }

    //test_morphos(X, TEST_X_SIZE);
    morpho(X, TEST_X_SIZE, TEST_X_SIZE, Y, 3);
    MatPrint(Y, TEST_X_SIZE, TEST_X_SIZE);

    return 0;
}
