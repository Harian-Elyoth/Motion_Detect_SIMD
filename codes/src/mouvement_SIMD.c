/* ------------------------------------------------------------------ */
/* ---  Algorithme Sigma Delta en SIMD pour le traitement d'image --- */
/* ------------------------------------------------------------------ */

#include "mouvement_SIMD.h"


void duplicate_vborder(int mi0, int mi1, int mj0, int mj1, int b, vuint8** image){

	// indices matrices avec bord
	int mi0b = mi0-b; int mi1b = mi1+b;
	int mj0b = mj0-b; int mj1b = mj1+b;

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
				image[i][mj0 - k] = image[i][mj0];

				// Bord Droit
				image[i][mj1 + k] = image[i][mj1];

				// Bord Haut
				image[mi0 - k][j] = image[mi0][j];

				// Bord Bas
				image[mi1 + k][j] = image[mi1][j];
			}
		}
	}
}

void gen_pgm_img_simd(){

   	int w = 32, h = 16;

   	uint8 **image_t = ui8matrix(0, h, 0, w);

   	for (int i = 0; i < h; ++i)
   	{
   		for (int j = 0; j < w; ++j)
   		{
   			image_t[i][j]  = i + j + 1;
   		}
   	}

	// save result on pgm file
	SavePGM_ui8matrix(image_t , 0, h, 0, w, "pgm_imgs/my_pgm1.pgm");
}

void SigmaDelta_step1_simd(int vmi0b, int vmi1b, int vmj0b, int vmj1b, vuint8** mean0, vuint8** mean1, vuint8** image){

	vuint8 x, y, cmp;
	vuint8 x_sh;	

	for (int i = vmi0b; i <= vmi1b; ++i)
	{
		for (int j = vmj0b; j < vmj1b; ++j)
		{
			x = VEC_LOAD_2D_EPI(i, j + 0, image);
			y = VEC_LOAD_2D_EPI(i, j + 1, image);

			DEBUG(display_vuint8(x, " %d ", " x : ")); DEBUG(puts(""));
			DEBUG(display_vuint8(y, " %d ", " y :  ")); DEBUG(puts(""));
			DEBUG(puts(""));

			cmp = _mm_cmplt_epi8(x, y);	

			DEBUG(display_vuint8(cmp, " %d ", " cmp : ")); DEBUG(puts(""));
			DEBUG(puts(""));

			x_sh = _mm_shuffle_epi8(y, init_vuint8_all(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0)); 

			DEBUG(display_vuint8(x_sh, " %d ", " x_sh : ")); DEBUG(puts(""));
			DEBUG(puts(""));

			break;
		}
	}


	// for (int i = mi0b; i <= mi1b; ++i)
	// {
	// 	for (int j = mj0b; j <= mj1b; ++j)
	// 	{
	// 		if (mean0[i][j] < image[i][j]){
	// 			mean1[i][j] = mean0[i][j] + 1;
	// 		}
	// 		else if (mean0[i][j] > image[i][j]){
	// 			mean1[i][j] = mean0[i][j] - 1;
	// 		}
	// 		else{
	// 			mean1[i][j] = mean0[i][j];
	// 		}
	// 	}
	// }
}



