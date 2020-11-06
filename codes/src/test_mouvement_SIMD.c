/* ----------------------------------------------------------------------- */
/* ---  Test Algorithme Sigma Delta en SIMD pour le traitement d'image --- */
/* ----------------------------------------------------------------------- */

#include "test_mouvement_SIMD.h"

void test_mouvement_SIMD_visual(){

// img test size
int width  = 32; // correspond au nb de colonne  => indice boucle j
int height = 16; // correspond au nb de ligne    => indice boucle i

// chronometrie
int iter, niter = 4;
int run, nrun = 5;
double t0, t1, dt, tmin, t;

char *format = "%d ";
double cycles;
int kernel_size = 3;

puts("===============================");
puts("=== test mouvement unitaire ===");
puts("===============================");

// BORD
// 1 for 3x3 
int b = 1; 

// 2 for 5x5
//int b = 2;

// cardinalité des registres
int card = card_vuint8(); // 16

// ------------------------- //
// -- calculs des indices -- //
// ------------------------- //

// indices scalaires matrices
int mi0 = 0; int mi1 = height-1;
int mj0 = 0; int mj1 = width-1;

// indices scalaires matrices avec bord
int mi0b = mi0-b; int mi1b = mi1+b;
int mj0b = mj0-b; int mj1b = mj1+b;

// indices vectoriels matrices
int vmi0 = 0; int vmi1 = (height)-1;
int vmj0 = 0; int vmj1 = (width/card)-1;

// indices vectoriels matrices avec bord
int vmi0b = vmi0-b; int vmi1b = vmi1+b;
int vmj0b = vmj0-b; int vmj1b = vmj1+b;

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

// images
vuint8** image = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

// moyennes
vuint8** mean0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
vuint8** mean1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);		

// ecart-types
vuint8** std0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
vuint8** std1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);			

// image de différence
vuint8 ** img_diff = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);	

// image binaire (sortie)
vuint8 ** img_bin = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);


/*---------------------------------------------------*/

// ------------------------------ //
// -- chargement et conversion -- //
// ------------------------------ //

// gen img visuel
gen_pgm_img_simd();

uint8 ** img_temp = ui8matrix(mi0b, mi1b, mj0b, mj1b);

MLoadPGM_ui8matrix("pgm_imgs/my_pgm1.pgm", mi0b, mi1b, mj0b, mj1b, img_temp);

DEBUG(printf("Before duplicate_border : \n"));DEBUG(puts(""));
DEBUG(display_ui8matrix(img_temp, mi0b, mi1b, mj0b, mj1b, "%d ", "img_temp : ")); DEBUG(puts(""));

duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);

DEBUG(printf("After duplicate_vborde : \n")); DEBUG(puts(""));
DEBUG(display_ui8matrix(img_temp, mi0b, mi1b, mj0b, mj1b, "%d ", "img_temp : ")); DEBUG(puts(""));

// PROBLEME ICI POUR LES INDICE DE BOUCLE
// PAS DE BORD DROITE ET GAUCHE 

for (int i = vmi0b; i <= vmi1b; ++i)
{
	for (int j = vmj0; j <= vmj1; ++j)
	{
		image[i][j] = init_vuint8_all(	img_temp[i][(j * card) + 0 ], img_temp[i][(j * card) + 1 ], img_temp[i][(j * card) + 2 ], img_temp[i][(j * card) + 3 ], 
										img_temp[i][(j * card) + 4 ], img_temp[i][(j * card) + 5 ], img_temp[i][(j * card) + 6 ], img_temp[i][(j * card) + 7 ], 
										img_temp[i][(j * card) + 8 ], img_temp[i][(j * card) + 9 ], img_temp[i][(j * card) + 10], img_temp[i][(j * card) + 11], 
										img_temp[i][(j * card) + 12], img_temp[i][(j * card) + 13], img_temp[i][(j * card) + 14], img_temp[i][(j * card) + 15]);

		DEBUG(printf("i =  %d, j = %d \n", i, j));
		DEBUG(display_vuint8(image[i][j], format, "image[i][j] : "));DEBUG(puts(""));
	} 
}

DEBUG(printf("After conversion : \n"));DEBUG(puts(""));

DEBUG(display_vui8matrix(image, vmi0b, vmi1b, vmj0b, vmj1b, "%d ", "image : ")); DEBUG(puts(""));

/*---------------------------------------------------*/

// ----------------- //
// -- traitements -- //
// ----------------- //


SigmaDelta_step1_simd(vmi0b, vmi1b, vmj0b, vmj1b, mean0, mean1, image);

/*---------------------------------------------------*/

// ---------- //
// -- free -- //
// ---------- //

free_vui8matrix(image, vmi0b, vmi1b, vmj0b, vmj1b);

free_vui8matrix(mean0, vmi0b, vmi1b, vmj0b, vmj1b);
free_vui8matrix(mean1, vmi0b, vmi1b, vmj0b, vmj1b);

free_vui8matrix(std0, vmi0b, vmi1b, vmj0b, vmj1b);
free_vui8matrix(std1, vmi0b, vmi1b, vmj0b, vmj1b);

free_vui8matrix(img_diff, vmi0b, vmi1b, vmj0b, vmj1b);
free_vui8matrix(img_bin, vmi0b, vmi1b, vmj0b, vmj1b);

}

void main_test_mouvement_SIMD(int argc, char *argv[]){

	DEBUG(test_mouvement_SIMD_visual());

	BENCH(test_mouvement_SIMD());
}