/*
==========================================================
==================== SD + Morpho =========================
==========================================================
*/

//=============== INCLUDES ===============================

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

#include "mouvement_morpho.h"


//==================== MACROS ============================

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
#define WIDTH  320; // correspond au nb de colonne  => indice boucle j
#define HEIGHT 240; // correspond au nb de ligne   => indice boucle i

//======================= PROGRAM =======================

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

// image filtrée
uint8 ** img_filtered;

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

void step_1(){
	// DEBUG(printf("step 1 : Mt estimation \n"));

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b; ++j)
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

	for (int i = mi0b; i <= mi1b; ++i){

		for (int j = mj0b; j <= mj1b; ++j){
			img_diff[i][j] = abs(mean1[i][j] - image1[i][j]);
		}
	}
}

void step_3(){
	// DEBUG(printf("step 3 : Vt update and clamping\n"));

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b; ++j)
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

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b; ++j)
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

// Kernel 3x3
// indice scalaire sans bord
void erosion_3(uint8 ** X, uint8 ** Y){

    uint8 a0, a1, a2;
    uint8 b0, b1, b2;
    uint8 c0, c1, c2;
    uint8 s;

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){
            
            // Detection d'un zero sur le kernel autour du IJ
            // Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1

            a0 = X[i - 1][j - 1]; b0 = X[i - 1][j + 0]; c0 = X[i - 1][j + 1];
            a1 = X[i + 0][j - 1]; b1 = X[i + 0][j + 0]; c1 = X[i + 0][j + 1];
            a2 = X[i + 1][j - 1]; b2 = X[i + 1][j + 0]; c2 = X[i + 1][j + 1];

            s = a0 && a1 && a2 && b0 && b1 && b2 && c0 && c1 && c2;
            Y[i][j] = s;
        }
    }
}

// Kernel 5x5
// indice scalaire sans bord
void erosion_5(uint8 ** X, uint8 ** Y){

    uint8 a0, a1, a2, a3, a4;
    uint8 b0, b1, b2, b3, b4;
    uint8 c0, c1, c2, c3, c4;
    uint8 d0, d1, d2, d3, d4;
    uint8 e0, e1, e2, e3, e4;
    uint8 s;

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){

        	//Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1

            a0 = X[i - 2][j - 2] ; b0 = X[i - 2][j - 1] ; c0 = X[i - 2][j + 0] ; d0 = X[i - 2][j + 1] ; e0 = X[i - 2][j + 2];
            a1 = X[i - 1][j - 2] ; b1 = X[i - 1][j - 1] ; c1 = X[i - 1][j + 0] ; d1 = X[i - 1][j + 1] ; e1 = X[i - 1][j + 2];
            a2 = X[i + 0][j - 2] ; b2 = X[i + 0][j - 1] ; c2 = X[i + 0][j + 0] ; d2 = X[i + 0][j + 1] ; e2 = X[i + 0][j + 1];
            a3 = X[i + 1][j - 2] ; b3 = X[i + 1][j - 1] ; c3 = X[i + 1][j + 0] ; d3 = X[i + 1][j + 1] ; e3 = X[i + 2][j + 2];
            a4 = X[i + 2][j - 2] ; b4 = X[i + 2][j - 1] ; c4 = X[i + 2][j + 0] ; d4 = X[i + 2][j + 1] ; e4 = X[i + 2][j + 2];

            s = a0 && a1 && a2 && a3 && a4 && b0 && b1 && b2 && b3 && b4 && c0 && c1 && c2 && c3 && c4 && d0 && d1 && d2 && d3 && d4 && e0 && e1 && e2 && e3 && e4;

            Y[i][j] = s;
        }
    }
}

// Kernel 3x3
// indice scalaire sans bord
void dilatation_3(uint8 ** X, uint8 ** Y){

    uint8 a0, a1, a2;
    uint8 b0, b1, b2;
    uint8 c0, c1, c2;
    uint8 s;

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){
            
            //Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1

            a0 = X[i - 1][j - 1] ; b0 = X[i - 1][j + 0] ; c0 = X[i - 1][j + 1];
            a1 = X[i + 0][j - 1] ; b1 = X[i + 0][j + 0] ; c1 = X[i + 0][j + 1];
            a2 = X[i + 1][j - 1] ; b2 = X[i + 1][j + 0] ; c2 = X[i + 1][j + 1];

            s = a0 || a1 || a2 || b0 || b1 || b2 || c0 || c1 || c2;
            Y[i][j] = s;
        }
    }
}

// Kernel 5x5
// indice scalaire sans bord
void dilatation_5(uint8 ** X, uint8 ** Y){

    uint8 a0, a1, a2, a3, a4;
    uint8 b0, b1, b2, b3, b4;
    uint8 c0, c1, c2, c3, c4;
    uint8 d0, d1, d2, d3, d4;
    uint8 e0, e1, e2, e3, e4;
    uint8 s;

    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0 ; j <= mj1 ; j++){

        	//Detection d'un zero sur le kernel autour du IJ
            //Si il y a un zero, dans le kernel alors le Yij prends 0, sinon il prends 1

            a0 = X[i - 2][j - 2] ; b0 = X[i - 2][j - 1] ; c0 = X[i - 2][j + 0] ; d0 = X[i - 2][j + 1] ; e0 = X[i - 2][j + 2];
            a1 = X[i - 1][j - 2] ; b1 = X[i - 1][j - 1] ; c1 = X[i - 1][j + 0] ; d1 = X[i - 1][j + 1] ; e1 = X[i - 1][j + 2];
            a2 = X[i + 0][j - 2] ; b2 = X[i + 0][j - 1] ; c2 = X[i + 0][j + 0] ; d2 = X[i + 0][j + 1] ; e2 = X[i + 0][j + 1];
            a3 = X[i + 1][j - 2] ; b3 = X[i + 1][j - 1] ; c3 = X[i + 1][j + 0] ; d3 = X[i + 1][j + 1] ; e3 = X[i + 2][j + 2];
            a4 = X[i + 2][j - 2] ; b4 = X[i + 2][j - 1] ; c4 = X[i + 2][j + 0] ; d4 = X[i + 2][j + 1] ; e4 = X[i + 2][j + 2];

            s = a0 || a1 || a2 || a3 || a4 || b0 || b1 || b2 || b3 || b4 || c0 || c1 || c2 || c3 || c4 || d0 || d1 || d2 || d3 || d4 || e0 || e1 || e2 || e3 || e4;
            Y[i][j] = s;
        }
    }
}

void morpho_3(uint8 ** X, uint8 ** Y){

    uint8 ** tmp1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
    uint8 ** tmp2 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

    erosion_3(X, tmp1);
    dilatation_3(tmp1, tmp2);
    erosion_3(tmp2, tmp1);
    dilatation_3(tmp1, Y);
}

void morpho_5(uint8 ** X, uint8 ** Y){

    uint8 ** tmp1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
    uint8 ** tmp2 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

    erosion_5(X, tmp1);
    dilatation_5(tmp1, tmp2);
    erosion_5(tmp2, tmp1);
    dilatation_5(tmp1, Y);
}

void bin_to_pgm(char* filename){

	// allocate pgm matrix
	uint8** pgm_out = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b; ++j)
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

void test_SD_morpho(){

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;

    char *format = "%d ";
    int kernel_size = 3;

    // alloue les matrices images, moyennes, ecart-types, diff, binaire
	allocate_matrix(kernel_size);

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

		// Fait uniquement au 1er tour de boucle	
		if (i == 1){

			// initiate mean0 et std0
			for (int i = mi0b; i <= mi1b; ++i)
			{
				for (int j = mj0b; j <= mj1b; ++j)
				{
					mean0[i][j] = image0[i][j];
					std0[i][j]  = VMIN;
				}
			}
		}

		CHRONO(SigmaDelta(),cycles);

		BENCH(printf("SIGMA DELTA : "));
		BENCH(printf("it = %d, cycles/X*Y = %0.6f", i, cycles/(WIDTH * HEIGHT))); BENCH(puts(""));

		CHRONO(morpho_3(img_bin, img_filtered),cycles);

		BENCH(printf("MORPHOLOGIE : "));
		BENCH(printf("it = %d, cycles/X*Y = %0.6f", i, cycles/(WIDTH * HEIGHT))); BENCH(puts(""));

		// built pgm filename out
		char filename_out[25] = "";
		snprintf(filename_out, 25, "SD_out_%d.pgm", i);

		bin_to_pgm(filename_out);
	}

	// free all matrix
	void free_matrix();
}

void main_SD_morpho(int argc, char  *argv[])
{
	DEBUG(test_SD_morpho());
	BENCH(test_SD_morpho());
}