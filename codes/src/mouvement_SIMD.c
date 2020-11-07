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

	vuint8 mean0_reg, mean1_reg, mean1_reg_temp, image_reg;
	vuint8 cmplt, cmpgt;
	vuint8 ones, reslt, resgt;

	ones = init_vuint8(1);
	DEBUG(display_vuint8(ones, " %d ", " ones : ")); DEBUG(puts(""));

	for (int i = vmi0b; i <= vmi1b; ++i)
	{
		for (int j = vmj0b; j <= vmj1b; ++j)
		{
			mean0_reg = VEC_LOAD_2D_EPI8(i, j, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j, image);

			// DEBUG(display_vuint8(mean0_reg, " %d ", " mean0_reg : ")); DEBUG(puts(""));
			// DEBUG(display_vuint8(image_reg, " %d ", " image_reg :  ")); DEBUG(puts(""));
			// DEBUG(display_vuint8(mean1_reg, " %d ", " mean1_reg :  ")); DEBUG(puts(""));
			// DEBUG(puts(""));

			cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
			cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

			// DEBUG(display_vuint8(cmplt, " %d ", " cmplt : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(cmpgt, " %d ", " cmpgt : ")); DEBUG(printf("\n\n"));

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			// DEBUG(display_vuint8(reslt, " %d ", " reslt : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(resgt, " %d ", " resgt : ")); DEBUG(printf("\n\n"));

			mean1_reg_temp = VEC_ADD_EPI8(mean0_reg, reslt);
			mean1_reg = VEC_SUB_EPI8(mean1_reg_temp, resgt); 
			// DEBUG(display_vuint8(mean1_reg, " %d ", " mean1_reg : ")); DEBUG(printf("\n\n"));

			VEC_STORE_2D_EPI8(mean1_reg, i, j, mean1);
		}
	}
}

void SigmaDelta_step2_simd(int vmi0b, int vmi1b, int vmj0b, int vmj1b, vuint8** image, vuint8** mean1, vuint8** img_diff){

	vuint8 mean1_reg, image_reg, abs_reg;

	for (int i = vmi0b; i < vmi1b; ++i)
	{
		for (int j = vmj0b; j < vmj1b; ++j)
		{
			mean1_reg = VEC_LOAD_2D_EPI8(i, j, mean1);
			image_reg = VEC_LOAD_2D_EPI8(i, j, image);

			abs_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg - image_reg));

			VEC_STORE_2D_EPI8(abs_reg, i, j, img_diff);
		}
	}
}

void SigmaDelta_step3_simd(int vmi0b, int vmi1b, int vmj0b, int vmj1b, vuint8** std0, vuint8** std1, vuint8** img_diff){

	vuint8 std0_reg, std1_reg, 

	for (int i = vmi0b; i < vmi1b; ++i)
	{
		for (int j = vmj0b; j < vmj1b; ++j)
		{
			/* code */
		}
	}


	// for (int i = mi0b; i <= mi1b; ++i)
	// {
	// 	for (int j = mj0b; j <= mj1b; ++j)
	// 	{
	// 		if (std0[i][j] < N * img_diff[i][j]){
	// 			std1[i][j] = std0[i][j] + 1;
	// 		}

	// 		else if (std0[i][j] > N * img_diff[i][j]){
	// 			std1[i][j] = std0[i][j] - 1;
	// 		}

	// 		else{
	// 			std1[i][j] = std0[i][j];
	// 		}

	// 		// clamp to [Vmin,Vmax]
	// 		std1[i][j] = MAX(MIN(std1[i][j], VMAX), VMIN);
	// 	}
	// }
}	