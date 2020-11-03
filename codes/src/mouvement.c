/* ---------------------------------------------------------- */
/* ---  Algorithme Sigma Delta pour le traitement d'image --- */
/* ---------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"

#include "mouvement.h"

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
int WIDTH  = 320; // correspond au nb de colonne  => indice boucle j
int HEIGHT = 240; // correspond au nb de ligne   => indice boucle i

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

// image de différence
uint8 ** img_diff;	

// image binaire (sortie)
uint8 ** img_bin;	


void allocate_matrix(){
	// 1 for 3x3 
	b = 1; 

	// 2 for 5x5
	//int b = 2;

	// indices matrices
	mi0 = 0; mi1 = HEIGHT-1;
	mj0 = 0; mj1 = WIDTH-1;
	
	// indices matrices avec bord
	mi0b = mi0-b; mi1b = mi1+b;
	mj0b = mj0-b; mj1b = mj1+b;
	
	DEBUG(puts("")); 
	DEBUG(printf("mi0b : %d\n", mi0b)); 
	DEBUG(printf("mi1b : %d\n", mi1b)); 
	DEBUG(printf("mj0b : %d\n", mj0b)); 
	DEBUG(printf("mj1b : %d\n", mj1b));
	DEBUG(puts("")); 

	image0 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	image1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	mean0 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	mean1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	std0 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	std1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	img_diff = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	img_bin = ui8matrix(mi0b, mi1b, mj0b, mj1b);
}

void duplicate_border(){

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b; ++j)
		{
			for (int k = 0; k <= b; ++k)
			{

				// economise des tours de boucles => plus performant !

				if(i > mi0 && i <= mi1){
					if (j == mj0)
					{
						j = mj1;
					}
				}

				// Bord Gauche
				image0[i][mj0 - k] = image0[i][mj0];
				image1[i][mj0 - k] = image1[i][mj0];

				// Bord Droit
				image0[i][mj1 + k] = image0[i][mj1];
				image1[i][mj1 + k] = image1[i][mj1];

				// Bord Haut
				image0[mi0 - k][j] = image0[mi0][j];
				image1[mi0 - k][j] = image1[mi0][j];

				// Bord Bas
				image0[mi1 + k][j] = image0[mi1][j];
				image1[mi1 + k][j] = image1[mi1][j];
			}
		}
	}

}

void free_matrix(){
	free_ui8matrix(image0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(image1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(mean0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(mean1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(std0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(std1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(img_diff, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b);
}

void load_imgs(){

	char *filename0 = "../car3/car_3037.pgm";
	char *filename1 = "../car3/car_3038.pgm";

	MLoadPGM_ui8matrix(filename0, mi0b, mi1b, mj0b, mj1b, image0);
	MLoadPGM_ui8matrix(filename1, mi0b, mi1b, mj0b, mj1b, image1);

	// initiate mean0 et std0
	for (int i = mi0b; i < mi1b; ++i)
	{
		for (int j = mj0b; j < mj1b; ++j)
		{
			mean0[i][j] = image0[i][j];
			std0[i][j]  = VMIN;
		}
	}
}

void step_1(){
	// DEBUG(printf("step 1 : Mt estimation \n"));

	for (int i = mi0b; i < mi1b; ++i)
	{
		for (int j = mj0b; j < mj1b; ++j)
		{
			if (mean0[i][j] < image1[i][j]){
				mean1[i][j] = mean0[i][j] + 1;
			}
			else if (mean0[i][j] > image1[i][j]){
				mean1[i][j] = mean0[i][j] - 1;
			}
			else{
				mean1[i][j] = mean0[i][j];
			}
		}
	}
}

void step_2(){
	// DEBUG(printf("step 2 : Ot computation\n"));

	for (int i = mi0b; i < mi1b; ++i){

		for (int j = mj0b; j < mj1b; ++j){
			img_diff[i][j] = abs(mean1[i][j] - image1[i][j]);
		}
	}
}

void step_3(){
	// DEBUG(printf("step 3 : Vt update and clamping\n"));

	for (int i = mi0b; i < mi1b; ++i)
	{
		for (int j = mj0b; j < mj1b; ++j)
		{
			if (std0[i][j] < N * img_diff[i][j]){
				std1[i][j] = std0[i][j] + 1;
			}

			else if (std0[i][j] > N * img_diff[i][j]){
				std1[i][j] = std0[i][j] - 1;
			}

			else{
				std1[i][j] = std0[i][j];
			}

			// clamp to [Vmin,Vmax]
			std1[i][j] = MAX(MIN(std1[i][j], VMAX), VMIN);
		}
	}
}

void step_4(){
	// DEBUG(printf("step 4 : Et estimation\n"));

	for (int i = mi0b; i < mi1b; ++i)
	{
		for (int j = mj0b; j < mj1b; ++j)
		{
			if (img_diff[i][j] < std1[i][j]){
				img_bin[i][j] = 0;
			}
			else{
				img_bin[i][j] = 1;
			}
		}
	}
}

void SigmaDelta(){
	step_1();
	step_2();
	step_3();
	step_4();
}

// ce fichier de test est fortement inspiré de la source suivante : https://www.tutorialspoint.com/c-program-to-write-an-image-in-pgm-format 
void gen_pgm_img(){
   int i, j;
   int w = 6, h = 8;

   int image[6][8] = {
      { 255, 255, 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255, 255, 255 },
   };

   int image2[6][8] = {
      { 200, 200, 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200, 200, 200 },
   };

	FILE* pgm1;
	FILE* pgm2;

	pgm1 = fopen("pgm_imgs/my_pgm1.pgm", "wb");
	pgm2 = fopen("pgm_imgs/my_pgm2.pgm", "wb");

	fprintf(pgm1, "P5\n");
	fprintf(pgm2, "P5\n");

	fprintf(pgm1, "%d %d\n", w, h);
	fprintf(pgm2, "%d %d\n", w, h);

	fprintf(pgm1, "255\n");
	fprintf(pgm2, "255\n");

	for (i = 0; i < h; i++) {
	  	for (j = 0; j < w; j++) {
	     	fprintf(pgm1, "%d ", image1[i][j]);
	  	}
	  	fprintf(pgm1, "\n");
	}
	fclose(pgm1);

	for (i = 0; i < h; i++) {
	 	for (j = 0; j < w; j++) {
	     	fprintf(pgm2, "%d ", image2[i][j]);
	  	}
	  	fprintf(pgm2, "\n");
	}
	fclose(pgm2);

	char *filename_pgm0 = "pgm_imgs/my_pgm1.pgm";
	char *filename_pgm1 = "pgm_imgs/my_pgm2.pgm";

	MLoadPGM_ui8matrix(filename_pgm0, mi0b, mi1b, mj0b, mj1b, image0);
	MLoadPGM_ui8matrix(filename_pgm1, mi0b, mi1b, mj0b, mj1b, image1);

	for (int i = mi0b; i < mi1b; ++i)
	{
		for (int j = mj0b; j < mj1b; ++j)
		{
			mean0[i][j] = image0[i][j];
			std0[i][j]  = VMIN;
		}
	}
}

void test_SD(int is_test){

	if(is_test == 1)	{
		WIDTH=6; HEIGHT=8;
	}
	else{
		WIDTH=320; HEIGHT=240;
	}

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char *format = "%d ";
    double cycles;

	// alloue les matrices images, moyennes, ecart-types, diff, binaire
	allocate_matrix();

	if (is_test == 1){
		// charge deux images pgm test taille 6x8 
		gen_pgm_img();
	}
	else{
		// charge les deux premières images du set
		load_imgs();
	}

	// affiche les images initiales avec bord vide
	DEBUG(display_ui8matrix(image0, mi0b, mi1b, mj0b, mj1b, format, "1ere image (bord a 0) : ")); DEBUG(puts(""));
	DEBUG(display_ui8matrix(image1, mi0b, mi1b, mj0b, mj1b, format, "2eme image (bord a 0) : ")); DEBUG(puts(""));

	duplicate_border();

	// affiche les images initiales avec bord dupliques
	DEBUG(display_ui8matrix(image0, mi0b, mi1b, mj0b, mj1b, format, "1ere image : ")); DEBUG(puts(""));
	DEBUG(display_ui8matrix(image1, mi0b, mi1b, mj0b, mj1b, format, "2eme image : ")); DEBUG(puts(""));

	CHRONO(SigmaDelta(),cycles);

	BENCH(printf("cycles = %0.6f", cycles)); BENCH(puts(""));

	BENCH(printf("cycles/X*Y = %0.6f", cycles/(WIDTH*HEIGHT))); BENCH(puts(""));

	// affiche l'image binaire resultante
	DEBUG(display_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b, format, "image binaire : ")); DEBUG(puts(""));

	bin_to_pgm("SD_out.pgm");

	// free all matrix
	void free_matrix();
}

void test_SD_dataset(){

	// dimensions imgs set
	WIDTH = 320;
	HEIGHT = 240;

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;

    char *format = "%d ";

    // alloue les matrices images, moyennes, ecart-types, diff, binaire
	allocate_matrix();

	int count = 3000;

	for (int i = 1; i < NB_IMG ; ++i)
	{

		char filename0[25] = "";

		snprintf(filename0, 25, "../car3/car_%d.pgm", count);

		count += 1;

		char filename1[25] = "";

		snprintf(filename1, 25, "../car3/car_%d.pgm", count);

		// DEBUG(printf("file 0 : %s\n", filename0));
		// DEBUG(printf("file 1 : %s\n", filename1));

		// CHARGER LES IMAGES PUIS FAIRE TRAITEMENTS

		MLoadPGM_ui8matrix(filename0, mi0b, mi1b, mj0b, mj1b, image0);
		MLoadPGM_ui8matrix(filename1, mi0b, mi1b, mj0b, mj1b, image1);

		if (i == 1){

			// initiate mean0 et std0
			for (int i = mi0b; i < mi1b; ++i)
			{
				for (int j = mj0b; j < mj1b; ++j)
				{
					mean0[i][j] = image0[i][j];
					std0[i][j]  = VMIN;
				}
			}
		}

		duplicate_border();

		CHRONO(SigmaDelta(),cycles);

		BENCH(printf("it : %d, cycles/X*Y = %0.6f", i, cycles/(WIDTH*HEIGHT))); BENCH(puts(""));

		// built pgm filename out
		char filename_out[25] = "";
		snprintf(filename_out, 25, "SD_out_%d.pgm", i);

		bin_to_pgm(filename_out);
	}

	// free all matrix
	void free_matrix();
}

void bin_to_pgm(char* filename){

	// allocate pgm matrix
	uint8** pgm_out = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	for (int i = mi0b; i < mi1b; ++i)
	{
		for (int j = mj0b; j < mj1b; ++j)
		{
			if (img_bin[i][j] == 0){
				pgm_out[i][j] = 255;
			}
			else{
				pgm_out[i][j] = 0;
			}
		}
	}

	char Fname[25] = "";
	const char *path = "pgm_imgs/";
	strcat(Fname, path);
	strcat(Fname, filename);

	// save result on pgm file
	SavePGM_ui8matrix(pgm_out, mi0b, mi1b, mj0b, mj1b, Fname);
}

void main_SD(int argc, char *argv[]){
	
	// DEBUG(test_SD(1));
	DEBUG(test_SD_dataset());

	BENCH(test_SD_dataset());	
}
