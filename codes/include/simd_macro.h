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

// ----------------- //
// --- MACRO EPI --- //
// ----------------- //

// MEMORY
#define VEC_LOAD_2D_EPI8(i, j, X) _mm_load_si128((vuint8 *) &X[i][j])
#define VEC_STORE_2D_EPI8(x, i, j, X) _mm_store_si128((vuint8 *) &X[i][j], x)

// SHUFFLE
#define ROT_RIGHT_EPI8(x) _mm_shuffle_epi8(x, init_vuint8_all(15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14)) // abcdefghijklmnop -> pabcdefghijklmno
#define ROT_LEFT_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0)) // abcdefghijklmnop -> bcdefghijklmnopa
#define SWITCH_1_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14)) // abcdefghijklmnop -> badcfehgjilknmpo
#define SWITCH_2_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13)) // abcdefghijklmnop -> cdabghefklijopmn
#define SWITCH_4_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(4, 5, 6, 7, 0, 1, 2, 3, 12, 13, 14, 15, 8, 9, 10, 11)) // abcdefghijklmnop -> efghabcdmnopijkl
#define SWITCH_8_EPI8(x)  _mm_shuffle_epi8(x, init_vuint8_all(8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7)) // abcdefghijklmnop -> ijklmnopabcdefgh

// VEC LEFT AND RIGHT
#define VEC_LEFT1_EPI8(x1, x2)  _mm_add_epi8(_mm_bslli_si128(x2, 1),  _mm_bsrli_si128(x1, 15)) // x1 abcdefghijklmnop ; x2 qrstuvwxyz012345 -> pqrstuvwxyz01234
#define VEC_RIGHT1_EPI8(x1, x2) _mm_add_epi8(_mm_bslli_si128(x2, 15), _mm_bsrli_si128(x1, 1)) // x1 abcdefghijklmnop ; x2 qrstuvwxyz012345 -> bcdefghijklmnopq
#define VEC_LEFT2_EPI8(x1, x2)  _mm_add_epi8(_mm_bslli_si128(x2, 2),  _mm_bsrli_si128(x1, 14))
#define VEC_RIGHT2_EPI8(x1, x2) _mm_add_epi8(_mm_bslli_si128(x2, 14), _mm_bsrli_si128(x1, 2))

// CAST EPI8 EPI16
#define EPI8_TO_EPI16(x, x1, x2) \
		x1 = init_vuint16_all(_mm_extract_epi8(x, 0), _mm_extract_epi8(x, 1), _mm_extract_epi8(x, 2)  , _mm_extract_epi8(x, 3)  , _mm_extract_epi8(x, 4)  , _mm_extract_epi8(x, 5)  , _mm_extract_epi8(x, 6)  , _mm_extract_epi8(x, 7)); \
		x2 = init_vuint16_all(_mm_extract_epi8(x, 8), _mm_extract_epi8(x, 9), _mm_extract_epi8(x, 10) , _mm_extract_epi8(x, 11) , _mm_extract_epi8(x, 12) , _mm_extract_epi8(x, 13) , _mm_extract_epi8(x, 14) , _mm_extract_epi8(x, 15)); \

#define EPI16_TO_EPI8(x, x1, x2) \
		x = init_vuint8_all( _mm_extract_epi16(x1, 0), _mm_extract_epi16(x1, 1), _mm_extract_epi16(x1, 2), _mm_extract_epi16(x1, 3),	\
							 _mm_extract_epi16(x1, 4), _mm_extract_epi16(x1, 5), _mm_extract_epi16(x1, 6), _mm_extract_epi16(x1, 7),	\
							 _mm_extract_epi16(x2, 0), _mm_extract_epi16(x2, 1), _mm_extract_epi16(x2, 2), _mm_extract_epi16(x2, 3),	\
							 _mm_extract_epi16(x2, 4), _mm_extract_epi16(x2, 5), _mm_extract_epi16(x2, 6), _mm_extract_epi16(x2, 7));	\

// MIN MAX UNSIGNED EPI8
#define VEC_MIN_EPU8(x, y) _mm_min_epu8(x, y)
#define VEC_MAX_EPU8(x, y) _mm_max_epu8(x, y)

// COMPARE EPI8
#define VEC_CMPEQ_EPI8(x, y) _mm_cmpeq_epi8(x, y) // if x = y => 255 else 0
#define VEC_CMPGT_EPI8(x, y) _mm_cmpgt_epi8(x, y) // if x > y => 255 else 0
#define VEC_CMPLT_EPI8(x, y) _mm_cmplt_epi8(x, y) // if x < y => 255 else 0

// COMPARE EPI8 UNSIGNED

#define VEC_CMPGE_EPU8(a, b) VEC_CMPEQ_EPI8(VEC_MAX_EPU8(a, b), a)

#define VEC_CMPLE_EPU8(a, b) VEC_CMPGE_EPU8(b, a)

#define VEC_CMPGT_EPU8(a, b) _mm_xor_si128(VEC_CMPLE_EPU8(a, b), _mm_set1_epi8(-1))

#define VEC_CMPLT_EPU8(a, b) VEC_CMPGT_EPU8(b, a)

// COMPARE EPI16
#define VEC_CMPEQ_EPI16(x, y) _mm_cmpeq_epi16(x, y) // if x = y => 255 else 0
#define VEC_CMPGT_EPI16(x, y) _mm_cmpgt_epi16(x, y) // if x > y => 255 else 0
#define VEC_CMPLT_EPI16(x, y) _mm_cmplt_epi16(x, y) // if x < y => 255 else 0

// CALCULS BASES
#define VEC_AND_EPI8(x, y)  _mm_and_si128(x, y) 	// ET logique bit a bit
#define VEC_ADDU_EPI8(x, y) _mm_add_epi8(x, y) 		// Addition non signé 8 bits 
#define VEC_ADD_EPI8(x, y)  _mm_adds_epi8(x, y) 	// Addition signé 8 bits 
#define VEC_SUBU_EPI8(x, y) _mm_sub_epi8(x, y)		// Soustraction non signé 8 bits
#define VEC_SUB_EPI8(x, y)  _mm_subs_epi8(x, y)		// Soustraction signé 8 bits 

#define VEC_MULU_EPI8(x, y, res)						\
		EPI8_TO_EPI16(x, x1_mul, x2_mul);				\
		EPI8_TO_EPI16(y, y1_mul, y2_mul);				\
		mullo_1 = _mm_mullo_epi16(x1_mul, y1_mul);		\
		mullo_2 = _mm_mullo_epi16(x2_mul, y2_mul);		\
		cmpgt1_mul = _mm_cmpgt_epi16(mullo_1, full);	\
		cmpgt2_mul = _mm_cmpgt_epi16(mullo_2, full);	\
		cmpgt1_mul = _mm_or_si128(mullo_1, cmpgt1_mul); \
		cmpgt2_mul = _mm_or_si128(mullo_2, cmpgt2_mul); \
		res1 = _mm_and_si128(cmpgt1_mul, full); 		\
		res2 = _mm_and_si128(cmpgt2_mul, full); 		\
		EPI16_TO_EPI8(res, res1, res2);	 				\

#define VEC_MULLO_EPU8(a, b, res) _mm_or_si128(_mm_slli_epi16(_mm_mullo_epi16(_mm_srli_epi16(a, 8),_mm_srli_epi16(b, 8)), 8), _mm_srli_epi16(_mm_slli_epi16(_mm_mullo_epi16(a, b),8), 8))

 
#define VEC_ABS_EPI8(x) _mm_abs_epi8(x)			// Valeur absolue
#define VEC_MAX_EPI8(x, y) _mm_max_epi8(x, y) 	// Max 8 bits
#define VEC_MIN_EPI8(x, y) _mm_min_epi8(x, y) 	// Min 8 bits

#define VEC_ABS_SUB_EPU8(a, b) _mm_or_si128(_mm_subs_epu8(a,  b), _mm_subs_epu8(b, a))

// CALCULS SUR 9 VECTEURS
#define VEC_AND_9_EPI8(x1, x2, x3, x4, x5, x6, x7, x8, x9) _mm_and_si128(x1, _mm_and_si128(x2, _mm_and_si128(x3, _mm_and_si128(x4, _mm_and_si128(x5, _mm_and_si128(x6, _mm_and_si128(x7, _mm_and_si128(x8, x9))))))))
#define VEC_OR_9_EPI8(x1, x2, x3, x4, x5, x6, x7, x8, x9) _mm_or_si128(x1, _mm_or_si128(x2, _mm_or_si128(x3, _mm_or_si128(x4, _mm_or_si128(x5, _mm_or_si128(x6, _mm_or_si128(x7, _mm_or_si128(x8, x9))))))))

// CALCULS SUR 25 VECTEURS
#define VEC_AND_25_EPI8(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25) \
_mm_and_si128(x1, _mm_and_si128(x2, _mm_and_si128(x3, _mm_and_si128(x4,_mm_and_si128(x5,\
_mm_and_si128(x6, _mm_and_si128(x7, _mm_and_si128(x8, _mm_and_si128(x9, _mm_and_si128(x10, \
_mm_and_si128(x11, _mm_and_si128(x12, _mm_and_si128(x13, _mm_and_si128(x14, _mm_and_si128(x15, \
_mm_and_si128(x16, _mm_and_si128(x17, _mm_and_si128(x18, _mm_and_si128(x19, _mm_and_si128(x20, \
_mm_and_si128(x21, _mm_and_si128(x22, _mm_and_si128(x23, _mm_and_si128(x24, x25))))))))))))))))))))))))

#define VEC_OR_25_EPI8(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25) \
_mm_or_si128(x1, _mm_or_si128(x2, _mm_or_si128(x3, _mm_or_si128(x4, _mm_or_si128(x5,\
_mm_or_si128(x6, _mm_or_si128(x7, _mm_or_si128(x8, _mm_or_si128(x9, _mm_or_si128(x10, \
_mm_or_si128(x11, _mm_or_si128(x12, _mm_or_si128(x13, _mm_or_si128(x14, _mm_or_si128(x15, \
_mm_or_si128(x16, _mm_or_si128(x17, _mm_or_si128(x18, _mm_or_si128(x19, _mm_or_si128(x20, \
_mm_or_si128(x21, _mm_or_si128(x22, _mm_or_si128(x23, _mm_or_si128(x24, x25))))))))))))))))))))))))

#endif // __SIMD_MACRO_H__
