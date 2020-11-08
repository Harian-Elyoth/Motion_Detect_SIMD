/* ---------------- */
/* --- simd1D.c --- */
/* ---------------- */

#include <stdio.h>
#include <stdlib.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"
#include "simd1D.h"
// -------------------------------------------------------------------
void add_vf32vector(vfloat32 *vX1, vfloat32 *vX2, int n, vfloat32 *vY)
// -------------------------------------------------------------------
{
    int i;
    vfloat32 x1, x2, y;

    for(i=0; i<n; i++) {
        
        x1 = _mm_load_ps((float32*) &vX1[i]);
        x2 = _mm_load_ps((float32*) &vX2[i]);
        
        y = _mm_add_ps(x1, x2);
        
        DEBUG(display_vfloat32(x1, "%4.0f", "x1 =")); DEBUG(puts(""));
        DEBUG(display_vfloat32(x2, "%4.0f", "x2 =")); DEBUG(puts(""));
        DEBUG(display_vfloat32(y,  "%4.0f", "y  =")); DEBUG(puts(""));

        _mm_store_ps((float*) &vY[i], y);
    }
}
// ---------------------------------------------------------
vfloat32 dot_vf32vector(vfloat32 *vX1, vfloat32 *vX2, int n)
// ---------------------------------------------------------
{
    int i;
    vfloat32 x1, x2, p, s;

    // TEST SHUFFLE

    // vfloat32 p1,p2,p3,p4;

    // x1 = _mm_load_ps((float32*) &vX1[0]);
    // x2 = _mm_load_ps((float32*) &vX1[1]);
    // DEBUG(display_vfloat32(x1, "%4.0f", "x1 =")); DEBUG(puts(""));
    // DEBUG(display_vfloat32(x2, "%4.0f", "x2 =")); DEBUG(puts(""));

    // p1 = _mm_shuffle_ps(x2, x1, _MM_SHUFFLE(3, 2, 1, 0));
    // DEBUG(display_vfloat32(p1, "%4.0f", "p1 =")); DEBUG(puts(""));

    // p2 = _mm_shuffle_ps(x1, p1, _MM_SHUFFLE(0, 3, 2, 1));
    // DEBUG(display_vfloat32(p2, "%4.0f", "p2 =")); DEBUG(puts(""));

    // p3 = _mm_shuffle_ps(x1, x2, _MM_SHUFFLE(0, 3, 3, 2));
    // DEBUG(display_vfloat32(p3, "%4.0f", "p3 =")); DEBUG(puts(""));

    // p4 = _mm_shuffle_ps(x1, x2, _MM_SHUFFLE(2, 1, 0, 3));
    // DEBUG(display_vfloat32(p4, "%4.0f", "p4 =")); DEBUG(puts(""));

    // CODE A COMPLETER EN SSE1

    s = _mm_set_ps(0, 0, 0, 0);

    for (i = 0; i < n; ++i)
    {
        x1 = _mm_load_ps((float32*) &vX1[i]);
        x2 = _mm_load_ps((float32*) &vX2[i]);

        p = _mm_mul_ps(x1, x2);
        s = _mm_add_ps(s, p);

        DEBUG(display_vfloat32(x1, "%4.0f", "x1 =")); DEBUG(puts(""));
        DEBUG(display_vfloat32(x2, "%4.0f", "x2 =")); DEBUG(puts(""));
        DEBUG(display_vfloat32(p,  "%4.0f", "p  =")); DEBUG(puts(""));
        DEBUG(display_vfloat32(s,  "%4.0f", "s  =")); DEBUG(puts(""));
    }

    //SHUFFLE

    vfloat32 x;

    x = SWITCH_1(s);
    s = _mm_add_ps(s, x);

    x = SWITCH_2(s);
    s = _mm_add_ps(s, x);

    return s; // attention il faut retourner un registre SIMD et non un scalaire
}
// ----------------------------------------------------
void avg3_vf32vector(vfloat32 *vX, int n, vfloat32 *vY)
// ----------------------------------------------------
{
    int i;
    vfloat32 x0, x1, x2; // registres alignes
    vfloat32 xx0, xx2; // registres non alignes
    vfloat32 y;
    
    // CODE A COMPLETER EN SSE1

    vfloat32 q = _mm_set_ps(3,3,3,3);

    // Prologue

    x0 = _mm_load_ps((float32*) &vX[0]);
    x1 = x0;
    
    for(int i = 0 ; i < n-1 ; i++){
        x2 =  _mm_load_ps((float32*) &vX[i+1]);

        xx0 = VEC_LEFT_1(x0, x1);
        xx2 = VEC_RIGHT_1(x1, x2);

        y = _mm_add_ps(xx0, x1);
        y = _mm_add_ps(y, xx2);

        y = _mm_div_ps(y, q);

        _mm_store_ps((float*) &vY[i], y);

        x0 = x1;
        x1 = x2;
    }  

    // Epilogue

    xx0 = VEC_LEFT_1(x0, x1);
    xx2 = VEC_RIGHT_1(x1, x2);

    y = _mm_add_ps(xx0, x1);
    y = _mm_add_ps(y, xx2);

    y = _mm_div_ps(y, q);

    _mm_store_ps((float*) &vY[n-1], y);

}
// ----------------------------------------------------
void avg5_vf32vector(vfloat32 *vX, int n, vfloat32 *vY)
// ----------------------------------------------------
{
    int i;
    vfloat32 x0, x1, x2; // registres alignes
    vfloat32 xx0, xx1, xx3, xx4; // registres non alignes
    vfloat32 y;

    vfloat32 q = _mm_set_ps(5,5,5,5);

    // Prologue

    x0 = _mm_load_ps((float32*) &vX[0]);
    x1 = x0;
    
    for(int i = 0 ; i < n-1 ; i++){

        x2 = _mm_load_ps((float32*) &vX[i+1]);

        xx0 = VEC_2x2(x0, x1);
        xx1 = VEC_LEFT_1(x0, x1);
        xx3 = VEC_RIGHT_1(x1, x2);
        xx4 = VEC_2x2(x1, x2);

        y = _mm_add_ps(xx0, xx1);
        y = _mm_add_ps(y, x1);
        y = _mm_add_ps(y, xx3);
        y = _mm_add_ps(y, xx4);

        y = _mm_div_ps(y, q);

        _mm_store_ps((float*) &vY[i], y);

        x0 = x1;
        x1 = x2;
    }  
    
    // Epilogue

    xx0 = VEC_2x2(x0, x1);
    xx1 = VEC_LEFT_1(x0, x1);
    xx3 = VEC_RIGHT_1(x1, x2);
    xx4 = VEC_2x2(x1, x2);

    y = _mm_add_ps(xx0, xx1);
    y = _mm_add_ps(y, x1);
    y = _mm_add_ps(y, xx3);
    y = _mm_add_ps(y, xx4);

    y = _mm_div_ps(y, q);

    _mm_store_ps((float*) &vY[n-1], y);

}
// --------------------------------------------------------
void avg3_rot_vf32vector(vfloat32 *vX, int n, vfloat32 *vY)
// --------------------------------------------------------
{
    int i;
    vfloat32 x0, x1, x2; // registres alignes
    vfloat32 xx0, xx2; // registres non alignes
    vfloat32 y;
    
    // CODE A COMPLETER EN SSE1
}
// --------------------------------------------------------
void avg5_rot_vf32vector(vfloat32 *vX, int n, vfloat32 *vY)
// --------------------------------------------------------
{
    int i;
    vfloat32 x1, x2, x3; // registres alignes
    vfloat32 xx0, xx1, xx3, xx4; // registres non alignes
    vfloat32 y;
    
    // CODE A COMPLETER EN SSE1
}
/* ========================== */
/* === Fonctions de tests === */
/* ========================== */


// ---------------
void test1D(int n)
// ---------------
{
    int b = 2; // border
    int card; // cardinal of vector type

    int si0, si1; // scalar indices
    int vi0, vi1; // vector indices
    int mi0, mi1; // memory (bounded) indices

    int si0b, si1b; // scalar indices with border
    int vi0b, vi1b; // vector indices with border
    int mi0b, mi1b; // memory (bounded) indices  with border

    
    vfloat32 *vX1, *vX2, *vY, *vY3, *vY5;
    vfloat32 d;

    char* format = "%6.2f ";
    
    // chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;
    
    puts("===============");
    puts("=== test 1D ===");
    puts("===============");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    printf("n = %d\n", n);
    
    card = card_vfloat32();

    si0 = 0;
    si1 = n-1;
    
    s2v1D(si0, si1, card, &vi0, &vi1);
    v2m1D(vi0, vi1, card, &mi0, &mi1);
    
    si0b = si0-b;
    si1b = si1+b;

    s2v1D(si0b, si1b, card, &vi0b, &vi1b);
    v2m1D(vi0b, vi1b, card, &mi0b, &mi1b);

    // ------------------------------------------- //
    // -- allocation des tableaux 1D vectoriels -- //
    // ------------------------------------------- //

    vX1 = vf32vector(vi0b, vi1b);
    vX2 = vf32vector(vi0b, vi1b);
    
    vY  = vf32vector(vi0, vi1);
    vY3 = vf32vector(vi0, vi1);
    vY5 = vf32vector(vi0, vi1);

    // ---------- //
    // -- init -- //
    // ---------- //

    zero_vf32vector(vX1, vi0b, vi1b);
    zero_vf32vector(vX2, vi0b, vi1b);
    zero_vf32vector(vY,  vi0, vi1);
    zero_vf32vector(vY3, vi0, vi1);
    zero_vf32vector(vY5, vi0, vi1);

    init_vf32vector_param(vX1, vi0, vi1, 1, 1);
    init_vf32vector_param(vX2, vi0, vi1, 1, 2);
    
    // --------------- //
    // -- affichage -- //
    // --------------- //

    // affichage classique sur une ligne: appel de la fonction scalaire
    DEBUG(display_f32vector((float32*) vX1, si0, si1, "%4.0f", "sX1"));

    // affichage par bloc SIMD: appel de la fonction SIMD
    DEBUG(display_vf32vector(vX1, vi0, vi1, "%4.0f", "vX1"));
    DEBUG(puts(""));

    // affichage classique sur une ligne: appel de la fonction scalaire
    DEBUG(display_f32vector((float32*) vX2, si0, si1, "%4.0f", "sX2"));

    // affichage par bloc SIMD: appel de la fonction SIMD
    DEBUG(display_vf32vector(vX2, vi0, vi1, "%4.0f", "vX2"));
    DEBUG(puts(""));

    // ------------ //
    // -- calcul -- //
    // ------------ //
    
    puts("----------------");
    puts("--- addition ---");
    puts("----------------");
    
    CHRONO(add_vf32vector(vX1, vX2, n/card, vY),cycles); printf("add: "); DEBUG(display_vf32vector(vY, vi0, vi1, format, "Y")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    
    // affichage classique sur une ligne: appel de la fonction scalaire
    DEBUG(display_f32vector((float32*) vY, si0, si1, "%4.0f", "sY"));
    DEBUG(puts(""));
    
    puts("-------------------");
    puts("--- dot product ---");
    puts("-------------------");

    CHRONO(d = dot_vf32vector(vX1, vX2, n/card),cycles); printf("dot product: "); DEBUG(display_vfloat32(d, "%6.0f ", "d")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    display_vfloat32(d, "%6.0f ", "d"); puts(""); // laisser sinon dead-code-elimination enleve tout le calcul ...
    
    puts("-----------");
    puts("--- avg ---");
    puts("-----------");
    
    CHRONO(avg3_vf32vector(vX1, n/card, vY3),cycles); printf("avg 3   "); DEBUG(display_vf32vector(vY3, vi0, vi1, format, "Y3")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    CHRONO(avg5_vf32vector(vX1, n/card, vY5),cycles); printf("avg 5   "); DEBUG(display_vf32vector(vY5, vi0, vi1, format, "Y5")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    BENCH(puts(""));
    
    CHRONO(avg3_rot_vf32vector(vX1, n/card, vY3),cycles); printf("avg 3 rot"); DEBUG(display_vf32vector(vY3, vi0, vi1, format, "Y3")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    CHRONO(avg5_rot_vf32vector(vX1, n/card, vY5),cycles); printf("avg 5 rot"); DEBUG(display_vf32vector(vY5, vi0, vi1, format, "Y5")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    BENCH(puts(""));
    

    // ---------- //
    // -- free -- //
    // ---------- //

    free_vf32vector(vX1, vi0b, vi1b);
    free_vf32vector(vX2, vi0b, vi1b);
    
    free_vf32vector(vY,  vi0, vi1);
    free_vf32vector(vY3, vi0, vi1);
    free_vf32vector(vY5, vi0, vi1);

    
}
// ================================
void main_1D(int argc, char *argv[])
// ================================
{
    DEBUG(test1D(12));
    
    BENCH(test1D(100 * 100));
    BENCH(test1D(1000 * 1000));


}
