/* ---------------------------------------------------------- */
/* ---  Algorithme Sigma Delta pour le traitement d'image --- */
/* ---------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"

// MACRO MIN MAX
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))


// img size
#define SIZE_X 320
#define SIZE_Y 240

// facteur ecart type
#define N 3

// valeurs ecart type min et max (fix)
#define VMIN 1
#define VMAX 254

// BORD
int b;

int mx0, mx1, my0, my1; 	// indices scalaire
int mx0b, mx1b, my0b, my1b; // indices scalaires avec bord

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

void allocate_matrix()
{
	// 1 for 3x3 
	b = 1; 

	// 2 for 5x5
	//int b = 2;

	// indices matrices
	mx0 = 0; mx1 = SIZE_X-1;
	my0 = 0; my1 = SIZE_Y-1;
	
	// indices matrices avec bord
	mx0b = mx0-b; mx1b = mx1+b;
	my0b = my0-b; my1b = my1+b;

	printf("mx0b : %d ", mx0b);
	printf("mx1b : %d ", mx1b);
	printf("my0b : %d ", my0b);
	printf("my1b : %d ", my1b);
	printf("\n");

	image0 = ui8matrix(mx0b, mx1b, my0b, my1b);
	image1 = ui8matrix(mx0b, mx1b, my0b, my1b);

	mean0 = ui8matrix(mx0b, mx1b, my0b, my1b);
	mean1 = ui8matrix(mx0b, mx1b, my0b, my1b);

	std0 = ui8matrix(mx0b, mx1b, my0b, my1b);
	std1 = ui8matrix(mx0b, mx1b, my0b, my1b);

	img_diff = ui8matrix(mx0b, mx1b, my0b, my1b);
	img_bin = ui8matrix(mx0b, mx1b, my0b, my1b); 

	for (int i = 0; i < SIZE_X; ++i)
	{
		for (int j = 0; j < SIZE_Y; ++j)
		{
			mean0[i][j] = image0[i][j];
			std0[i][j]  = VMIN;
		}
	}
}

void load_imgs()
{

	/* -- Faire le traitement pour transformer les chemins -- */

	char *filename0 = "../car3/car_3000.pgm";
	char *filename1 = "../car3/car_3001.pgm";

	MLoadPGM_ui8matrix(filename0,  mx0b, mx1b, my0b, my1b, image0);
	MLoadPGM_ui8matrix(filename1,  mx0b, mx1b, my0b, my1b, image1);

	// initiate mean0 et std0

	for (int i = 0; i < SIZE_X; ++i)
	{
		for (int j = 0; j < SIZE_Y; ++j)
		{
			mean0[i][j] = image0[i][j];
			std0[i][j]  = VMIN;
		}
	}

}

void print_img(uint8 ** img, int nrl, int nrh, int ncl, int nch)
{
	for (int i = ncl; i <= nch; ++i)
	{
		for (int j = nrl; j <= nrh; ++j)
		{
			printf("%d ", img[i][j]);
		}

		printf("\n");
	}

	printf("\n");
}

void SigmaDelta()
{

	// step 1 : Mt estimation 

	for (int i = mx0b; i < mx1b; ++i)
	{
		for (int j = my0b; j < my1b; ++j)
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

	// step 2 : Ot computation

	for (int i = mx0b; i < mx1b; ++i){

		for (int j = my0b; j < my1b; ++j){
			img_diff[i][j] = abs(mean1[i][j] - image1[i][j]);
		}
	}

	// step 3 : Vt update and clamping

	for (int i = mx0b; i < mx1b; ++i)
	{
		for (int j = my0b; j < my1b; ++j)
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

	// step 4 : Et estimation

	for (int i = mx0b; i < mx1b; ++i)
	{
		for (int j = my0b; j < my1b; ++j)
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

// ce fichier de test est fortement inspiré de la source suivante : https://www.tutorialspoint.com/c-program-to-write-an-image-in-pgm-format 
void test_pgm_img()
{
   int i, j;
   int w = 6, h = 6;

   int image[6][6] = {
      { 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255 },
      { 255, 255, 255, 255, 255, 255 },
   };

   int image2[6][6] = {
      { 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200 },
      { 200, 200, 200, 200, 200, 200 },
   };

	FILE* pgm1;
	FILE* pgm2;

	pgm1 = fopen("my_pgm1.pgm", "wb");
	pgm2 = fopen("my_pgm2.pgm", "wb");

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

	char *filename0 = "my_pgm1.pgm";
	char *filename1 = "my_pgm2.pgm";

	MLoadPGM_ui8matrix(filename0,  mx0b, mx1b, my0b, my1b, image0);
	MLoadPGM_ui8matrix(filename1,  mx0b, mx1b, my0b, my1b, image1);

	for (int i = 0; i < SIZE_X; ++i)
	{
		for (int j = 0; j < SIZE_Y; ++j)
		{
			mean0[i][j] = image0[i][j];
			std0[i][j]  = VMIN;
		}
	}
}

int main_SD(int argc, char const *argv[])
{
	allocate_matrix();
	load_imgs();
	//test_pgm_img();
	// print_img(image0, mx0b, mx1b, my0b, my1b);
	// print_img(image1, mx0b, mx1b, my0b, my1b);
	// print_img(mean0, mx0b, mx1b, my0b, my1b);
	// print_img(std0, mx0b, mx1b, my0b, my1b);

	SigmaDelta();
	print_img(img_bin, my0b, my1b, mx0b, mx1b);
	return 0;
}