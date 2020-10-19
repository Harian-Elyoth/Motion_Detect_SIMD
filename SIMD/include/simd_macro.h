/* -------------------- */
/* --- simd_macro.h --- */
/* -------------------- */


#ifndef __SIMD_MACRO_H__
#define __SIMD_MACRO_H__

// a remplir

#define ROT_RIGHT(x) _mm_shuffle_ps(x,x,_MM_SHUFFLE(2,1,0,3));
#define ROT_LEFT(x) _mm_shuffle_ps(x,x,_MM_SHUFFLE(0,3,2,1));

#define SWITCH_1(x) _mm_shuffle_ps(x,x,_MM_SHUFFLE(2,3,0,1));
#define SWITCH_2(x) _mm_shuffle_ps(x,x,_MM_SHUFFLE(1,0,3,2));

#define VEC_LEFT_1(a, b) _mm_shuffle_ps(_mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 2, 1, 0)), b, _MM_SHUFFLE(2, 1, 0, 3)); // abcd efgh -> defg
#define VEC_RIGHT_1(a, b) _mm_shuffle_ps(a, _mm_shuffle_ps(b, a, _MM_SHUFFLE(3, 2, 1, 0)), _MM_SHUFFLE(0, 3, 2, 1)); // abcd efgh -> bcde
#define VEC_2x2(a,b) _mm_shuffle_ps(a, b, _MM_SHUFFLE(1, 0, 3, 2)); // abcd efgh -> cdef	

#define VEC_SET(x) _mm_set_ps(x,x,x,x);

#define vec_left1(v0, v1)  v0
#define vec_left2(v0, v1)  v0
#define vec_left3(v0, v1)  v0
#define vec_left4(v0, v1)  v0

#define vec_right1(v1, v2) v1
#define vec_right2(v1, v2) v1
#define vec_right3(v1, v2) v1
#define vec_right4(v1, v2) v1


// calculs
#define vec_div3(x) x
#define vec_div5(x) x

#define VEC_ADD3(x0, x1, x2) _mm_add_ps(x0,_mm_add_ps(x1,x2));
#define VEC_ADD5(x0, x1, x2, x3, x4) _mm_add_ps(x0,_mm_add_ps(x1,_mm_add_ps(x2,_mm_add_ps(x3,x4))));


#define vAVERAGE3(x0,x1,x2) x0
#define vAVERAGE5(x0,x1,x2,x3,x4) x0

#endif // __SIMD_MACRO_H__
