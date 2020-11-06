/* -------------------- */
/* --- simd_macro.h --- */
/* -------------------- */


#ifndef __SIMD_MACRO_H__
#define __SIMD_MACRO_H__

// ---------------- //
// --- MACRO PS --- //
// ---------------- //

#define ROT_RIGHT_PS(x) _mm_shuffle_ps(x,x,_MM_SHUFFLE(2,1,0,3)) // abcd -> dabc
#define ROT_LEFT_PS(x) _mm_shuffle_ps(x,x,_MM_SHUFFLE(0,3,2,1)) // abcd -> bcda

#define SWITCH_1_PS(x) _mm_shuffle_ps(x,x,_MM_SHUFFLE(2,3,0,1)) // abcd -> badc
#define SWITCH_2_PS(x) _mm_shuffle_ps(x,x,_MM_SHUFFLE(1,0,3,2)) // abcd -> cdab

#define VEC_LEFT_1_PS(a, b) _mm_shuffle_ps(_mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 2, 1, 0)), b, _MM_SHUFFLE(2, 1, 0, 3)) // abcd efgh -> defg
#define VEC_RIGHT_1_PS(a, b) _mm_shuffle_ps(a, _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 2, 1, 0)), _MM_SHUFFLE(0, 3, 2, 1)) // abcd efgh -> bcde
#define VEC_2x2_PS(a,b) _mm_shuffle_ps(a, b, _MM_SHUFFLE(1, 0, 3, 2)) // abcd efgh -> cdef	

#define VEC_SET_PS(x) _mm_set_ps(x,x,x,x)

#define VEC_LOAD_1D_PS(i, X) _mm_load_ps((float32*) &X[i])
#define VEC_LOAD_2D_PS(i, j, X) _mm_load_ps((float32*) &X[i][j])

// calculs
#define VEC_DIV3_PS(x) _mm_mul_ps(VEC_SET(1/3), x)
#define VEC_DIV5_PS(x) _mm_mul_ps(VEC_SET(1/5), x)

#define VEC_ADD3_PS(x0, x1, x2) _mm_add_ps(x0,_mm_add_ps(x1,x2))
#define VEC_ADD5_PS(x0, x1, x2, x3, x4) _mm_add_ps(x0,_mm_add_ps(x1,_mm_add_ps(x2,_mm_add_ps(x3,x4))))

#define VEC_AVERAGE3_PS(x0,x1,x2) VEC_DIV3(VEC_ADD3(x0, x1, x2))
#define VEC_AVERAGE5_PS(x0,x1,x2,x3,x4) VEC_DIV5(VEC_ADD5(x0, x1, x2, x3, x4))

// ------------------ //
// --- MACRO EPI8 --- //
// ------------------ //

#define VEC_LOAD_2D_EPI(i, j, X) _mm_load_si128((vuint8 *) &X[i][j])

#define VEC_CMPEQ_EPI(x, y) _mm_cmpeq_epi8(x, y) // if x = y => 255 else 0
#define VEC_CMPGT_EPI(x, y) _mm_cmpgt_epi8(x, y) // if x > y => 255 else 0
#define VEC_CMPLT_EPI(x, y) _mm_cmplt_epi8(x, y) // if x < y => 255 else 0

#endif // __SIMD_MACRO_H__
