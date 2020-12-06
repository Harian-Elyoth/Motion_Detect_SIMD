#include "nrdef.h"
#include "stdio.h"

void display_type_morpho(type_morpho_t MORPHO){
    switch(MORPHO){
        case EROSION3 :
            printf("EROSION 3");
            break;
        case EROSION5 :
            printf("EROSION 5");
            break;
        case DILATATION3 : 
            printf("DILATATION 3");
            break;
        case DILATATION5 :
            printf("DILATATION 5");
            break;
        case MORPHO3 :
            printf("MORPHO 3");
            break;
        case MORPHO5 : 
            printf("MORPHO 5");
            break;
        default : 
            break;
    }
} 

void display_type_opti(type_opti_t OPTI){
    switch(OPTI){
        case SIMD :
            printf("SIMD");
            break;
        case SIMD_UNROLL :
            printf("SIMD UNROLL");
            break;
        case SCALAIRE :
            printf("SCALAIRE");
            break;
        case SCALAIRE_UNROLL :
            printf("SCALAIRE UNROLL");
            break;
        case PIPELINE :
            printf("PIPELINE");
            break;
        case PIPELINE_FUSION :
            printf("PIPELINE_FUSION");
        default : 
            break;
    }
} 