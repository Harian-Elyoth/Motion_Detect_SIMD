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

typedef enum {
    EROSION3,
    DILATATION3,
    EROSION5,
    DILATATION5,
    MORPHO3,
    MORPHO5
} type_morpho_t;

typedef enum {
    SD,
    SD_FULL
} type_sd_t;

typedef enum {
    SIMD,
    SIMD_OPTI,
    SIMD_OMP,
    SIMD_OPTI_OMP,
    SCALAIRE,
    SCALAIRE_OPTI,
    PIPELINE,
    PIPELINE_OPTI,
    SIMD_OMP,
    SIMD_OPTI_OMP
} type_opti_t;

void display_type_morpho(type_morpho_t MORPHO);
void display_type_opti(type_opti_t OPTI);

#endif // __DEF_H__
