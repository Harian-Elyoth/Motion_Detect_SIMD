/* ------------- */
/* --- def.h --- */
/* ------------- */


#ifndef __DEF_H__
#define __DEF_H__

typedef unsigned char byte;

typedef          char  int8;
typedef unsigned char uint8;
typedef   signed char sint8;

typedef          short  int16;
typedef unsigned short uint16;
typedef   signed short sint16;

typedef          int  int32;
typedef unsigned int uint32;
typedef   signed int sint32;

typedef float         float32;
typedef double        float64;
typedef struct { byte r; byte g; byte b;} rgb8;

//Types des différentes morphologies
typedef enum {
    EROSION3,
    DILATATION3,
    EROSION5,
    DILATATION5,
    MORPHO3,
    MORPHO5
} type_morpho_t;

//types des différentes SD (fusion ou non)
typedef enum {
    SD,
    SD_FULL
} type_sd_t;

//Types des différentes optimisations
typedef enum {
    SIMD,
    SIMD_UNROLL,
    SIMD_OMP,
    SIMD_UNROLL_OMP,
    SCALAIRE,
    SCALAIRE_UNROLL,
    PIPELINE,
    PIPELINE_FUSION,
    PIPELINE_OMP,
    PIPELINE_FUSION_OMP
} type_opti_t;

//Affichage des types morphos
void display_type_morpho(type_morpho_t MORPHO);

//Affichage des types optis
void display_type_opti(type_opti_t UNROLL);

#endif // __DEF_H__
