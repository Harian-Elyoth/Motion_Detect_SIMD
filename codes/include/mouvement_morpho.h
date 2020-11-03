/* ----------------------------------- */
/* ------- mouvement_morpho.h -------- */
/* ----------------------------------- */

#ifndef __MOUVEMENT_MORPHO_H__
#define __MOUVEMENT_MORPHO_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

void allocate_matrix(int kernel_size);
void free_matrix();
void step_1();
void step_2();
void step_3();
void step_4();

void SigmaDelta();

void bin_to_pgm(char* filename);

void erosion_3(uint8 ** X, uint8 ** Y);
void erosion_5(uint8 ** X, uint8 ** Y);

void dilatation_3(uint8 ** X, uint8 ** Y);
void dilatation_5(uint8 ** X, uint8 ** Y);

void morpho_3(uint8 ** X, uint8 ** Y);
void morpho_5(uint8 ** X, uint8 ** Y);

// fonction principale     
void main_SD_morpho(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif