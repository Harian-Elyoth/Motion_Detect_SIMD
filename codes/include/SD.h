/* --------------------- */
/* ------- SD.h -------- */
/* --------------------- */

#ifndef __SD_H__
#define __SD_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

void allocate_matrix();
void free_matrix();
void load_imgs();

void step_1();
void step_2();
void step_3();
void step_4();

void SigmaDelta();

void test_pgm_img();
void test_SD(int is_test);
void test_SD_dataset();
void bin_to_pgm(char* filename);

// fonction principale     
void main_SD(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif