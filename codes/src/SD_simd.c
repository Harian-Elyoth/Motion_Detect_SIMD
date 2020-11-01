/* ------------------------------------------------------------------ */
/* ---  Algorithme Sigma Delta en SIMD pour le traitement d'image --- */
/* ------------------------------------------------------------------ */

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

#include "SD_simd.h"

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
int WIDTH  = 6; // correspond au nb de colonne  => indice boucle j
int HEIGHT = 8; // correspond au nb de ligne   => indice boucle i

// BORD
int b;

int mi0, mi1, mj0, mj1; 	// indices scalaire
int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

// images
vuint8** image0;
vuint8** image1;

// moyennes
vuint8** mean0;
vuint8** mean1;		

// ecart-types
vuint8** std0;
vuint8** std1;			

// image de différence
vuint8 ** img_diff;	

// image binaire (sortie)
vuint8 ** img_bin;

void allocate_vmatrix(){
	// 1 for 3x3 
	b = 1; 

	// 2 for 5x5
	//b = 2;

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

	image0 = vui8matrix(mi0b, mi1b, mj0b, mj1b);
	image1 = vui8matrix(mi0b, mi1b, mj0b, mj1b);

	mean0 = vui8matrix(mi0b, mi1b, mj0b, mj1b);
	mean1 = vui8matrix(mi0b, mi1b, mj0b, mj1b);

	std0 = vui8matrix(mi0b, mi1b, mj0b, mj1b);
	std1 = vui8matrix(mi0b, mi1b, mj0b, mj1b);

	img_diff = vui8matrix(mi0b, mi1b, mj0b, mj1b);
	img_bin = vui8matrix(mi0b, mi1b, mj0b, mj1b);
}

void free_vmatrix(){
	free_vui8matrix(image0, mi0b, mi1b, mj0b, mj1b);
	free_vui8matrix(image1, mi0b, mi1b, mj0b, mj1b);

	free_vui8matrix(mean0, mi0b, mi1b, mj0b, mj1b);
	free_vui8matrix(mean1, mi0b, mi1b, mj0b, mj1b);

	free_vui8matrix(std0, mi0b, mi1b, mj0b, mj1b);
	free_vui8matrix(std1, mi0b, mi1b, mj0b, mj1b);

	free_vui8matrix(img_diff, mi0b, mi1b, mj0b, mj1b);
	free_vui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b);
}

void test_SD_simd(){

	allocate_vmatrix();

	DEBUG(display_vui8matrix (image0, mi0b, mi1b, mj0b, mj1b, "%d", "image0 : ")); DEBUG(puts(""));

	free_vmatrix();

}

void main_SD_simd(int argc, char *argv[])
{
	DEBUG(test_SD_simd());
}


