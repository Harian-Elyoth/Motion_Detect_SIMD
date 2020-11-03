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


void allocate_matrix(int kernel_size){
	
	if (kernel_size == 3)
	{
		// 1 for 3x3 
		b = 1; 
	}
	else if(kernel_size == 5){
		// 2 for 5x5
		b = 2;
	}
	else
	{
		DEBUG(printf("ERROR on Kernel Size !\n"));
		exit(1);
	}

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

	// image filtrée
	img_filtered = ui8matrix(mi0b, mi1b, mj0b, mj1b);
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

	free_ui8matrix(img_filtered, mi0b, mi1b, mj0b, mj1b);
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

void filtered_to_pgm(char* filename){

	// allocate pgm matrix
	uint8** pgm_out = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	for (int i = mi0b; i < mi1b; ++i)
	{
		for (int j = mj0b; j < mj1b; ++j)
		{
			if (img_filtered[i][j] == 0){
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

