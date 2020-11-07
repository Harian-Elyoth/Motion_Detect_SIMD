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

// MEMORY
#define VEC_LOAD_2D_EPI8(i, j, X) _mm_load_si128((vuint8 *) &X[i][j])
#define VEC_STORE_2D_EPI8(x, i, j, X) _mm_store_si128((vuint8 *) &X[i][j], x)

// SHUFFLE
#define ROT_RIGHT_EPI8(x) _mm_shuffle_epi8(x, init_vuint8_all(15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14)); // abcdefghijklmnop -> pabcdefghijklmno
#define ROT_LEFT_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0)); // abcdefghijklmnop -> bcdefghijklmnopa
#define SWITCH_1_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14)); // abcdefghijklmnop -> badcfehgjilknmpo
#define SWITCH_2_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13)); // abcdefghijklmnop -> cdabghefklijopmn
#define SWITCH_4_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(4, 5, 6, 7, 0, 1, 2, 3, 12, 13, 14, 15, 8, 9, 10, 11)); // abcdefghijklmnop -> efghabcdmnopijkl
#define SWITCH_8_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7)); // abcdefghijklmnop -> ijklmnopabcdefgh

// COMPARE
#define VEC_CMPEQ_EPI8(x, y) _mm_cmpeq_epi8(x, y) // if x = y => 255 else 0
#define VEC_CMPGT_EPI8(x, y) _mm_cmpgt_epi8(x, y) // if x > y => 255 else 0
#define VEC_CMPLT_EPI8(x, y) _mm_cmplt_epi8(x, y) // if x < y => 255 else 0

// CALCULS
#define VEC_AND_EPI8(x, y) _mm_and_si128(x, y); // ET logique bit a bit
#define VEC_ADD_EPI8(x, y) _mm_add_epi8(x, y);  // Addition 8 bit 
#define VEC_SUB_EPI8(x, y) _mm_sub_epi8(x, y);  // Soustraction 8 bit 

#define VEC_ABS_EPI8(x) _mm_abs_epi8(x);		// Valeur absolue

#endif // __SIMD_MACRO_H__
