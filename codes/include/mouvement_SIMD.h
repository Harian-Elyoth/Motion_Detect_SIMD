/* --------------------------------- */
/* ------- mouvement_SIMD.h -------- */
/* --------------------------------- */

#ifndef __MOUVEMENT_SIMD_H__
#define __MOUVEMENT_SIMD_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif


void allocate_vmatrix();
void free_vmatrix();
void duplicate_vborder();
void gen_pgm_img_simd();

void load_img_to_matrix(char *filename0, char *filename1);


// fonction principale     
void main_mouvement_simd(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif