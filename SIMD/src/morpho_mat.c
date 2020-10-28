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
#define MatPrint(X, H, W) for(int i = 0 ; i < H ; i++){ for(int j = 0 ; j < W ; j++){ printf("%d\t", X[i][j]); } printf("\n");}
#define IntMatAlloc(X, H, W) int ** X = (int **)malloc(sizeof(int *) * H);for(int i = 0 ; i < H ; i++){X[i] = (int *)malloc(sizeof(int) * W);}
//======================= PROGRAM =======================

void erosion(int ** X, int h, int w, int ** Y, int kernel_size){
    int padding;
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

    int isZero = 0;
    for(int i = padding ; i < h - padding ; i++){
        for(int j = padding ; j < w - padding ; j++){
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1
            for(int k = i - padding ; k < i - padding + kernel_size ; k++){
                for(int l = j - padding ; l < j - padding + kernel_size ; l++){
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
        }
    }
}

void dilatation(int ** X, int h, int w, int ** Y, int kernel_size){
    int padding;
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

    int isOne = 0;
    for(int i = padding ; i < h - padding ; i++){
        for(int j = padding ; j < w - padding ; j++){
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1
            for(int k = i - padding ; k < i - padding + kernel_size ; k++){
                for(int l = j - padding ; l < j - padding + kernel_size ; l++){
                    isOne = X[k][l];
                    if(isOne){
                        //Si il y a au moins 1 zero dans le kernel on met le pixel Yij à zero
                        Y[i][j] = 1;
                        break;
                    }
                } //Ici on fait des break pour ne pas avoir a regarder le reste du kernel si on detecte 1 zero
                if(isOne){
                    break;
                }
            }
            if(!isOne){
                Y[i][j] = 0;
            }
            isOne = 0;
        }
    }
}

void morpho(int ** X, int h, int w, int ** Y, int kernel_size){
    IntMatAlloc(TEMP1, h, w);
    erosion(X, h, w, TEMP1, kernel_size);
    IntMatAlloc(TEMP2, h, w);
    dilatation(TEMP1, h, w, TEMP2, kernel_size);
    dilatation(TEMP2, h, w, TEMP1, kernel_size);
    erosion(TEMP1, h, w, Y, kernel_size);

}

void test_morphos(int ** X, int size){

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

int main(int argc, char const *argv[])
{
    IntMatAlloc(X, TEST_X_SIZE, TEST_X_SIZE);
    IntMatAlloc(Y, TEST_X_SIZE, TEST_X_SIZE);
    for(int i = 0 ; i < TEST_X_SIZE ; i++){
        for(int j = 0 ; j < TEST_X_SIZE ; j++){
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
