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
#define MatPrint(X) for(int i = 0 ; i < TEST_X_SIZE ; i++){ for(int j = 0 ; j < TEST_X_SIZE ; j++){ printf("%d\t", X[i][j]); } printf("\n");}
#define IntMatAlloc(X, TEST_X_SIZE) int ** X = (int **)malloc(sizeof(int *) * TEST_X_SIZE);for(int i = 0 ; i < TEST_X_SIZE ; i++){X[i] = (int *)malloc(sizeof(int) * TEST_X_SIZE);}
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

            for(int k = i - padding ; k < kernel_size ; k++){
                for(int l = j - padding ; l < kernel_size ; l++){
                    isZero = !X[k][l];
                    if(isZero){
                        //Si il y a au moins 1 zero dans le kernel on met le pixel Yij à zero
                        Y[i][j] = 0;
                        break;
                    }
                }
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

int main(int argc, char const *argv[])
{
    IntMatAlloc(X, TEST_X_SIZE)
    for(int i = 0 ; i < TEST_X_SIZE ; i++){
        for(int j = 0 ; j < TEST_X_SIZE ; j++){
            if(i == j || i == j - 1 || i == j + 1 || i == j - 2 || i == j + 2){
                X[i][j] = 1;
            }
            else {
                X[i][j] = 0;
            }
        }
    }

    MatPrint(X);
    IntMatAlloc(Y, TEST_X_SIZE);
    erosion(X, TEST_X_SIZE, TEST_X_SIZE, Y, 3);
    printf("================= EROSION ==============\n");
    MatPrint(Y);
    return 0;
}
