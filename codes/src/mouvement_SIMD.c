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

#include "mouvement_SIMD.h"

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
int WIDTH  = 32; // correspond au nb de colonne  => indice boucle j
int HEIGHT = 16; // correspond au nb de ligne    => indice boucle i

// BORD
int b;

// CARDINALITE
int card;

int mi0, mi1, mj0, mj1; 	// indices scalaires
int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

int vmi0, vmi1, vmj0, vmj1; 	// indices vectoriels
int vmi0b, vmi1b, vmj0b, vmj1b; // indices vectoriels avec bord

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
	
	// cardinalité des registres
	card = card_vuint8(); // 16

	// 1 for 3x3 
	b = 1; 

	// 2 for 5x5
	//b = 2;

	// indices scalaires matrices
	mi0 = 0; mi1 = HEIGHT-1;
	mj0 = 0; mj1 = WIDTH-1;
	
	// indices scalaires matrices avec bord
	mi0b = mi0-b; mi1b = mi1+b;
	mj0b = mj0-b; mj1b = mj1+b;

	// indices vectoriels matrices
	vmi0 = 0; vmi1 = (HEIGHT)-1;
	vmj0 = 0; vmj1 = (WIDTH/card)-1;
	
	// indices vectoriels matrices avec bord
	vmi0b = vmi0-b; vmi1b = vmi1+b;
	vmj0b = vmj0-b; vmj1b = vmj1+b;
	
	DEBUG(puts("")); 
	DEBUG(printf("mi0b : %d\n", mi0b)); 
	DEBUG(printf("mi1b : %d\n", mi1b)); 
	DEBUG(printf("mj0b : %d\n", mj0b)); 
	DEBUG(printf("mj1b : %d\n", mj1b));
	DEBUG(puts("")); 

	DEBUG(puts("")); 
	DEBUG(printf("vmi0b : %d\n", vmi0b)); 
	DEBUG(printf("vmi1b : %d\n", vmi1b)); 
	DEBUG(printf("vmj0b : %d\n", vmj0b)); 
	DEBUG(printf("vmj1b : %d\n", vmj1b));
	DEBUG(puts("")); 

	image0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	image1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	mean0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	mean1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	std0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	std1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	img_diff = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	img_bin = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
}

void free_vmatrix(){
	free_vui8matrix(image0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(image1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(mean0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(mean1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(std0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(std1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(img_diff, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(img_bin, vmi0b, vmi1b, vmj0b, vmj1b);
}

void load_img_to_matrix(char *filename0, char *filename1){

	uint8 ** img0 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8 ** img1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	MLoadPGM_ui8matrix(filename0, mi0b, mi1b, mj0b, mj1b, img0);
	MLoadPGM_ui8matrix(filename1, mi0b, mi1b, mj0b, mj1b, img1);

	DEBUG(display_ui8matrix(img0, mi0b, mi1b, mj0b, mj1b, "%d ", "img0 : ")); DEBUG(puts(""));

	DEBUG(printf("Before conversion\n");)

	DEBUG(puts("")); 
	DEBUG(printf("vmi0 : %d\n", vmi0)); 
	DEBUG(printf("vmi1 : %d\n", vmi1)); 
	DEBUG(printf("vmj0 : %d\n", vmj0)); 
	DEBUG(printf("vmj1 : %d\n", vmj1));
	DEBUG(puts("")); 

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1; ++j)
		{
			image0[i][j] = init_vuint8_all(	img0[i][(j * card) + 0 ], img0[i][(j * card) + 1 ], img0[i][(j * card) + 2 ], img0[i][(j * card) + 3 ], 
											img0[i][(j * card) + 4 ], img0[i][(j * card) + 5 ], img0[i][(j * card) + 6 ], img0[i][(j * card) + 7 ], 
											img0[i][(j * card) + 8 ], img0[i][(j * card) + 9 ], img0[i][(j * card) + 10], img0[i][(j * card) + 11], 
											img0[i][(j * card) + 12], img0[i][(j * card) + 13], img0[i][(j * card) + 14], img0[i][(j * card) + 15]);
		} 
	}

	DEBUG(printf("After conversion\n");)DEBUG(puts(""));

}

// ce fichier de test est fortement inspiré de la source suivante : https://www.tutorialspoint.com/c-program-to-write-an-image-in-pgm-format 
void gen_pgm_img(){
   	int i, j;
   	int w = 32, h = 16;

   	uint8 **image_t = ui8matrix(mi0b, mi1b, mj0b, mj1b);
   	uint8 **image_t2 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

   	for (int i = mi0; i <= mi1; ++i)
   	{
   		for (int j = mj0; j <= mj1; ++j)
   		{
   			image_t[i][j]  = i + j;
   			image_t2[i][j] = i + j;
   		}
   	}

	// save result on pgm file
	SavePGM_ui8matrix(image_t , 0, h, 0, w, "pgm_imgs/my_pgm1.pgm");
	SavePGM_ui8matrix(image_t2, 0, h, 0, w, "pgm_imgs/my_pgm2.pgm");

	load_img_to_matrix("pgm_imgs/my_pgm1.pgm", "pgm_imgs/my_pgm2.pgm");
}

// void step_1_simd(){
// 	//step 1 : Mt estimation

// 	for (int i = mi0b; i < mi1b; ++i)
// 	{
// 		for (int j = mj0b; j < mj1b; ++j)
// 		{

// 			if (mean0[i][j] < image1[i][j]){
// 				mean1[i][j] = mean0[i][j] + 1;
// 			}
// 			else if (mean0[i][j] > image1[i][j]){
// 				mean1[i][j] = mean0[i][j] - 1;
// 			}
// 			else{
// 				mean1[i][j] = mean0[i][j];
// 			}
// 		}
// 	}
// }


void test_SD_simd(){

	allocate_vmatrix();

	gen_pgm_img();

	DEBUG(puts(""));

	DEBUG(display_vui8matrix(image0, vmi0b, vmi1b, vmj0b, vmj1b, "%d ", "image0 : ")); DEBUG(puts(""));

	free_vmatrix();

}

void main_SD_simd(int argc, char *argv[])
{
	DEBUG(test_SD_simd());
}


