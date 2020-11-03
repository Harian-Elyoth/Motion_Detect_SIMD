/* ---------------------------- */
/* ------- mouvement.h -------- */
/* ---------------------------- */

#ifndef __MOUVEMENT_H__
#define __MOUVEMENT_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

// MACRO MIN MAX
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// NB IMAGES
#define NB_IMG 200

// facteur ecart type
#define N 3

// valeurs ecart type min et max (fix)
#define VMIN 1
#define VMAX 254

// img size
#define WIDTH   320 // correspond au nb de colonne  => indice boucle j
#define HEIGHT  240 // correspond au nb de ligne    => indice boucle i

// BORD
int b;

int mi0, mi1, mj0, mj1; 	// indices scalaire
int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

// images
uint8** image0;
uint8** image1;

// moyennes
uint8** mean0;
uint8** mean1;		

// ecart-types
uint8** std0;
uint8** std1;			

// image de difference
uint8 ** img_diff;	

// image binaire (sortie de Sigma Delta)
uint8 ** img_bin;	

// image filtree (apres morphologie)
uint8 ** img_filtered;

void allocate_matrix(int kernel_size);
void free_matrix();
void load_imgs();

void step_1();
void step_2();
void step_3();
void step_4();

void SigmaDelta();

void duplicate_border();

void gen_pgm_img();

void bin_to_pgm(char* filename);

void filtered_to_pgm(char* filename);


#ifdef __cplusplus
}
#endif

#endif