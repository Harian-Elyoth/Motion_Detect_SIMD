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

void gen_pgm_img_simd(int mi0, int mi1, int mj0, int mj1, int vmi0, int vmi1, int vmj0, int vmj1, int b, int card, vuint8** image, vuint8** mean0, vuint8** std0){

	DEBUG(printf("GEN IMG SIMD\n"));

	// indices scalaires matrices avec bord
	int mi0b = mi0-b; int mi1b = mi1+b;
	int mj0b = mj0-b; int mj1b = mj1+b;

	// indices vectoriels matrices avec bord
	int vmi0b = vmi0-b; int vmi1b = vmi1+b;
	int vmj0b = vmj0-1; int vmj1b = vmj1+1;

   	uint8** img_temp1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
   	uint8** img_temp2 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

   	for (int i = mi0; i <= mi1; ++i)
   	{
   		for (int j = mj0; j <= mj1; ++j)
   		{
   			img_temp1[i][j]  = i + j + (j*10);
   			img_temp2[i][j]  = i + j + (i*10);
   		}
   	}

	duplicate_border(mi0, mi1, mj0, mj1, b, img_temp1);

	// transfert ui8matrix à vui8matrix real
	ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp1, image);

	// initiate mean0 et std0 for first iteration
	for (int i = vmi0b; i <= vmi1b; ++i)
	{
		for (int j = vmj0b; j <= vmj1b; ++j)
		{
			mean0[i][j] = image[i][j];
			std0[i][j]  = init_vuint8(VMIN);
		}
	}

	duplicate_border(mi0, mi1, mj0, mj1, b, img_temp2);

	// transfert ui8matrix à vui8matrix real
	ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp2, image);

	// FREE
	free_ui8matrix(img_temp1, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(img_temp2, mi0b, mi1b, mj0b, mj1b);
}

void SigmaDelta_step1_simd(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** mean0, vuint8** mean1, vuint8** image){

	vuint8 mean0_reg, mean1_reg, image_reg;
	vuint8 cmplt, cmpgt;
	vuint8 ones, reslt, resgt;

	vuint16 x1_gt, x2_gt, y1_gt, y2_gt;
	vuint16 cmpgt1, cmpgt2;

	vuint16 x1_lt, x2_lt, y1_lt, y2_lt;
	vuint16 cmplt1, cmplt2;	

	ones = init_vuint8(1);

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1; ++j)
		{
			mean0_reg = VEC_LOAD_2D_EPI8(i, j, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j, image);

			// DEBUG(display_vuint8(mean0_reg, "%03d ", "mean0_reg : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(image_reg, "%03d ", "image_reg : ")); DEBUG(printf("\n\n"));

			VEC_CMPLTU_EPI8(mean0_reg, image_reg, cmplt);
			VEC_CMPGTU_EPI8(mean0_reg, image_reg, cmpgt);
	
			// DEBUG(display_vuint8(cmplt, "%03d ", "cmplt : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(cmpgt, "%03d ", "cmpgt : ")); DEBUG(printf("\n\n"));

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			// DEBUG(display_vuint8(reslt, "%03d ", "reslt : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(resgt, "%03d ", "resgt : ")); DEBUG(printf("\n\n"));

			mean1_reg = VEC_SUBU_EPI8(VEC_ADDU_EPI8(mean0_reg, reslt), resgt); 
			// DEBUG(display_vuint8(mean1_reg, "%03d ", "mean1_reg : ")); DEBUG(printf("\n\n"));

			VEC_STORE_2D_EPI8(mean1_reg, i, j, mean1);
		}
	}
}

void SigmaDelta_step1_simd_opti(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** mean0, vuint8** mean1, vuint8** image){

	int k = 4; int r = (vmj1+1) % k;

	vuint8 mean0_reg, mean1_reg, image_reg;
	vuint8 cmplt, cmpgt;
	vuint8 ones, reslt, resgt;

	vuint16 x1_gt, x2_gt, y1_gt, y2_gt;
	vuint16 cmpgt1, cmpgt2;

	vuint16 x1_lt, x2_lt, y1_lt, y2_lt;
	vuint16 cmplt1, cmplt2;	

	ones = init_vuint8(1);

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1 - r; j = j + k)
		{
			mean0_reg = VEC_LOAD_2D_EPI8(i, j + 0, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 0, image);

			VEC_CMPLTU_EPI8(mean0_reg, image_reg, cmplt);
			VEC_CMPGTU_EPI8(mean0_reg, image_reg, cmpgt);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			VEC_STORE_2D_EPI8(mean1_reg, i, j + 0, mean1);

			/*-----------------------------------------------------------------*/

			mean0_reg = VEC_LOAD_2D_EPI8(i, j + 1, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 1, image);

			VEC_CMPLTU_EPI8(mean0_reg, image_reg, cmplt);
			VEC_CMPGTU_EPI8(mean0_reg, image_reg, cmpgt);

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			VEC_STORE_2D_EPI8(mean1_reg, i, j + 1, mean1);

			/*-----------------------------------------------------------------*/

			mean0_reg = VEC_LOAD_2D_EPI8(i, j + 2, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 2, image);

			VEC_CMPLTU_EPI8(mean0_reg, image_reg, cmplt)
			VEC_CMPGTU_EPI8(mean0_reg, image_reg, cmpgt)	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			VEC_STORE_2D_EPI8(mean1_reg, i, j + 2, mean1);

			/*-----------------------------------------------------------------*/

			mean0_reg = VEC_LOAD_2D_EPI8(i, j + 3, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 3, image);

			VEC_CMPLTU_EPI8(mean0_reg, image_reg, cmplt)
			VEC_CMPGTU_EPI8(mean0_reg, image_reg, cmpgt)	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			VEC_STORE_2D_EPI8(mean1_reg, i, j + 3, mean1);
		}

		// EPILOGUE
		switch(r){

			case 3:
				mean0_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 2, mean0);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 2, image);

				VEC_CMPLTU_EPI8(mean0_reg, image_reg, cmplt)
				VEC_CMPGTU_EPI8(mean0_reg, image_reg, cmpgt)	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);
				
				mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

				VEC_STORE_2D_EPI8(mean1_reg, i, vmj1 - 2, mean1);

			case 2:
				mean0_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 1, mean0);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 1, image);

				VEC_CMPLTU_EPI8(mean0_reg, image_reg, cmplt)
				VEC_CMPGTU_EPI8(mean0_reg, image_reg, cmpgt)	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);
				
				mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

				VEC_STORE_2D_EPI8(mean1_reg, i, vmj1 - 1, mean1);

			case 1:
				mean0_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 0, mean0);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 0, image);

				VEC_CMPLTU_EPI8(mean0_reg, image_reg, cmplt)
				VEC_CMPGTU_EPI8(mean0_reg, image_reg, cmpgt)	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);
				
				mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

				VEC_STORE_2D_EPI8(mean1_reg, i, vmj1 - 0, mean1);

			default:
				break;
		}
	}
}

void SigmaDelta_step2_simd(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** image, vuint8** mean1, vuint8** img_diff){

	vuint8 mean1_reg, image_reg, sub_reg, abs_reg;

	vuint16 x1_abs, x2_abs;
	vuint16 y1_abs, y2_abs;
	vuint16 sub1_abs, sub2_abs;
	vuint16 res1_abs, res2_abs;

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1; ++j)
		{
			mean1_reg = VEC_LOAD_2D_EPI8(i, j, mean1);
			image_reg = VEC_LOAD_2D_EPI8(i, j, image);

			// DEBUG(display_vuint8(mean1_reg, "%03d ", "mean1_reg : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(image_reg, "%03d ", "image_reg : ")); DEBUG(printf("\n\n"));

			VEC_ABS_SUB_EPI8(mean1_reg, image_reg, abs_reg);
			// abs_reg = VEC_ABS_EPI8(sub_reg);

			// DEBUG(display_vuint8(abs_reg, "%03d ", "abs_reg : ")); DEBUG(printf("\n\n"));

			VEC_STORE_2D_EPI8(abs_reg, i, j, img_diff);
		}
	}
}

void SigmaDelta_step2_simd_opti(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** image, vuint8** mean1, vuint8** img_diff){

	int k = 4; int r = (vmj1+1) % k;

	vuint8 mean1_reg, image_reg, abs_reg;

	vuint16 x1_abs, x2_abs;
	vuint16 y1_abs, y2_abs;
	vuint16 sub1_abs, sub2_abs;
	vuint16 res1_abs, res2_abs;

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1 - r; j = j + k)
		{
			mean1_reg = VEC_LOAD_2D_EPI8(i, j + 0, mean1);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 0, image);

			VEC_ABS_SUB_EPI8(mean1_reg, image_reg, abs_reg);

			VEC_STORE_2D_EPI8(abs_reg, i, j + 0, img_diff);

			/*-----------------------------------------------------------*/

			mean1_reg = VEC_LOAD_2D_EPI8(i, j + 1, mean1);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 1, image);

			VEC_ABS_SUB_EPI8(mean1_reg, image_reg, abs_reg);

			VEC_STORE_2D_EPI8(abs_reg, i, j + 1, img_diff);

			/*-----------------------------------------------------------*/

			mean1_reg = VEC_LOAD_2D_EPI8(i, j + 2, mean1);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 2, image);

			VEC_ABS_SUB_EPI8(mean1_reg, image_reg, abs_reg);

			VEC_STORE_2D_EPI8(abs_reg, i, j + 2, img_diff);

			/*-----------------------------------------------------------*/

			mean1_reg = VEC_LOAD_2D_EPI8(i, j + 3, mean1);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 3, image);

			VEC_ABS_SUB_EPI8(mean1_reg, image_reg, abs_reg);

			VEC_STORE_2D_EPI8(abs_reg, i, j + 3, img_diff);
		}

		// EPILOGUE
		switch(r){

			case 3:
				mean1_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 2, mean1);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 2, image);

				VEC_ABS_SUB_EPI8(mean1_reg, image_reg, abs_reg);

				VEC_STORE_2D_EPI8(abs_reg, i, vmj1 - 2, img_diff);

			case 2:
				mean1_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 1, mean1);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 1, image);

				VEC_ABS_SUB_EPI8(mean1_reg, image_reg, abs_reg);

				VEC_STORE_2D_EPI8(abs_reg, i, vmj1 - 1, img_diff);

			case 1:
				mean1_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 0, mean1);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 0, image);

				VEC_ABS_SUB_EPI8(mean1_reg, image_reg, abs_reg);

				VEC_STORE_2D_EPI8(abs_reg, i, vmj1 - 0, img_diff);

			default:
				break;
		}
	}
}

void SigmaDelta_step3_simd(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** std0, vuint8** std1, vuint8** img_diff){

	vuint8 std0_reg, std1_reg;
	vuint8 N_reg, img_diff_reg, N_img_diff_reg, VMAX_reg, VMIN_reg;
	vuint8 cmplt, cmpgt, reslt, resgt, ones;
	vuint8 std1_min, std1_max;

	vuint16 x1_mul, x2_mul, y1_mul, y2_mul;
	vuint16 mullo_1, mullo_2;
	vuint16 cmpgt1_mul, cmpgt2_mul, res1, res2;

	vuint16 x1_gt, x2_gt, y1_gt, y2_gt;
	vuint16 cmpgt1, cmpgt2;

	vuint16 x1_lt, x2_lt, y1_lt, y2_lt;
	vuint16 cmplt1, cmplt2;	

	vuint16 x1_min, x2_min, y1_min, y2_min;
	vuint16 min1, min2;

	vuint16 x1_max, x2_max, y1_max, y2_max;
	vuint16 max1, max2;

	vuint16 full = init_vuint16(255);

	N_reg 		= init_vuint8(N);
	ones  		= init_vuint8(1);
	VMAX_reg	= init_vuint8(VMAX);
	VMIN_reg	= init_vuint8(VMIN);

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1; ++j)
		{
			img_diff_reg = VEC_LOAD_2D_EPI8(i, j, img_diff);

			// DEBUG(display_vuint8(img_diff_reg, "%03d ", "img_diff_reg : ")); DEBUG(printf("\n\n"));

			VEC_MULU_EPI8(img_diff_reg, N_reg, N_img_diff_reg);

			// DEBUG(display_vuint8(N_img_diff_reg, "%03d ", "N_img_diff_reg : ")); DEBUG(printf("\n\n"));

			std0_reg = VEC_LOAD_2D_EPI8(i, j, std0);

			// DEBUG(display_vuint8(std0_reg, "%03d ", "std0_reg : ")); DEBUG(printf("\n\n"));

			VEC_CMPLTU_EPI8(std0_reg, N_img_diff_reg, cmplt);
			VEC_CMPGTU_EPI8(std0_reg, N_img_diff_reg, cmpgt);	

			// DEBUG(display_vuint8(cmplt, "%03d ", "cmplt : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(cmpgt, "%03d ", "cmpgt : ")); DEBUG(printf("\n\n"));

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			// DEBUG(display_vuint8(reslt, "%03d ", "reslt : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(resgt, "%03d ", "resgt : ")); DEBUG(printf("\n\n"));

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std0_reg, reslt), resgt); 

			// DEBUG(display_vuint8(std1_reg, "%03d ", "std1_reg : ")); DEBUG(printf("\n\n"));

			VEC_MINU_EPI8(std1_reg, VMAX_reg, std1_min);

			// DEBUG(display_vuint8(std1_min, "%03d ", "std1_reg after min : ")); DEBUG(printf("\n\n"));

			VEC_MAXU_EPI8(std1_min, VMIN_reg, std1_max);

			// DEBUG(display_vuint8(std1_max, "%03d ", "std1_reg after max : ")); DEBUG(printf("\n\n"));

			VEC_STORE_2D_EPI8(std1_max, i, j, std1);
		}
	}
}	

void SigmaDelta_step3_simd_opti(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** std0, vuint8** std1, vuint8** img_diff){

	int k = 4; int r = (vmj1+1) % k;

	vuint8 std0_reg, std1_reg;
	vuint8 N_reg, N_img_diff_reg, VMAX_reg, VMIN_reg;
	vuint8 cmplt, cmpgt, reslt, resgt, ones;
	vuint8 std1_min, std1_max;

	vuint16 x1_mul, x2_mul, y1_mul, y2_mul;
	vuint16 mullo_1, mullo_2;
	vuint16 cmpgt1_mul, cmpgt2_mul, res1, res2;

	vuint16 x1_gt, x2_gt, y1_gt, y2_gt;
	vuint16 cmpgt1, cmpgt2;

	vuint16 x1_lt, x2_lt, y1_lt, y2_lt;
	vuint16 cmplt1, cmplt2;	

	vuint16 x1_min, x2_min, y1_min, y2_min;
	vuint16 min1, min2;

	vuint16 x1_max, x2_max, y1_max, y2_max;
	vuint16 max1, max2;

	vuint16 full = init_vuint16(255);

	N_reg 		= init_vuint8(N);
	ones  		= init_vuint8(1);
	VMAX_reg	= init_vuint8(VMAX);
	VMIN_reg	= init_vuint8(VMIN);

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1 - r; j = j + k)
		{
			N_img_diff_reg = VEC_LOAD_2D_EPI8(i, j + 0, img_diff);

			VEC_MULU_EPI8(N_img_diff_reg, N_reg, N_img_diff_reg);	

			std0_reg = VEC_LOAD_2D_EPI8(i, j, std0);

			VEC_CMPLTU_EPI8(std0_reg, N_img_diff_reg, cmplt);
			VEC_CMPGTU_EPI8(std0_reg, N_img_diff_reg, cmpgt);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std0_reg, reslt), resgt); 

			VEC_MINU_EPI8(std1_reg, VMAX_reg, std1_min);
			VEC_MAXU_EPI8(std1_min, VMIN_reg, std1_max);

			VEC_STORE_2D_EPI8(std1_max, i, j + 0, std1);

			/*--------------------------------------------------------------------*/

			N_img_diff_reg = VEC_LOAD_2D_EPI8(i, j + 1, img_diff);

			VEC_MULU_EPI8(N_img_diff_reg, N_reg, N_img_diff_reg);	

			std0_reg = VEC_LOAD_2D_EPI8(i, j + 1, std0);

			VEC_CMPLTU_EPI8(std0_reg, N_img_diff_reg, cmplt);
			VEC_CMPGTU_EPI8(std0_reg, N_img_diff_reg, cmpgt);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std0_reg, reslt), resgt); 

			VEC_MINU_EPI8(std1_reg, VMAX_reg, std1_min);
			VEC_MAXU_EPI8(std1_min, VMIN_reg, std1_max);

			VEC_STORE_2D_EPI8(std1_max, i, j + 1, std1);

			/*--------------------------------------------------------------------*/

			N_img_diff_reg = VEC_LOAD_2D_EPI8(i, j + 2, img_diff);

			VEC_MULU_EPI8(N_img_diff_reg, N_reg, N_img_diff_reg);	

			std0_reg = VEC_LOAD_2D_EPI8(i, j + 2, std0);

			VEC_CMPLTU_EPI8(std0_reg, N_img_diff_reg, cmplt);
			VEC_CMPGTU_EPI8(std0_reg, N_img_diff_reg, cmpgt);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std0_reg, reslt), resgt); 

			VEC_MINU_EPI8(std1_reg, VMAX_reg, std1_min);
			VEC_MAXU_EPI8(std1_min, VMIN_reg, std1_max);

			VEC_STORE_2D_EPI8(std1_max, i, j + 2, std1);

			/*--------------------------------------------------------------------*/

			N_img_diff_reg = VEC_LOAD_2D_EPI8(i, j + 3, img_diff);

			VEC_MULU_EPI8(N_img_diff_reg, N_reg, N_img_diff_reg);	

			std0_reg = VEC_LOAD_2D_EPI8(i, j + 3, std0);

			VEC_CMPLTU_EPI8(std0_reg, N_img_diff_reg, cmplt);
			VEC_CMPGTU_EPI8(std0_reg, N_img_diff_reg, cmpgt);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std0_reg, reslt), resgt); 

			VEC_MINU_EPI8(std1_reg, VMAX_reg, std1_min);
			VEC_MAXU_EPI8(std1_min, VMIN_reg, std1_max);

			VEC_STORE_2D_EPI8(std1_max, i, j + 3, std1);
		}

		// EPILOGUE
		switch(r){

			case 3:
				N_img_diff_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 2, img_diff);

				VEC_MULU_EPI8(N_img_diff_reg, N_reg, N_img_diff_reg);	

				std0_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 2, std0);

				VEC_CMPLTU_EPI8(std0_reg, N_img_diff_reg, cmplt);
				VEC_CMPGTU_EPI8(std0_reg, N_img_diff_reg, cmpgt);	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);

				std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std0_reg, reslt), resgt); 

				VEC_MINU_EPI8(std1_reg, VMAX_reg, std1_min);

				// clamp to [Vmin,Vmax]
				VEC_MAXU_EPI8(std1_min, VMIN_reg, std1_max);

				VEC_STORE_2D_EPI8(std1_max, i, vmj1 - 2, std1);

			case 2:
				N_img_diff_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 1, img_diff);

				VEC_MULU_EPI8(N_img_diff_reg, N_reg, N_img_diff_reg);	

				std0_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 1, std0);

				VEC_CMPLTU_EPI8(std0_reg, N_img_diff_reg, cmplt);
				VEC_CMPGTU_EPI8(std0_reg, N_img_diff_reg, cmpgt);	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);

				std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std0_reg, reslt), resgt); 

				VEC_MINU_EPI8(std1_reg, VMAX_reg, std1_min);

				// clamp to [Vmin,Vmax]
				VEC_MAXU_EPI8(std1_min, VMIN_reg, std1_max);

				VEC_STORE_2D_EPI8(std1_max, i, vmj1 - 1, std1);

			case 1:
				N_img_diff_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 0, img_diff);

				VEC_MULU_EPI8(N_img_diff_reg, N_reg, N_img_diff_reg);	

				std0_reg = VEC_LOAD_2D_EPI8(i, vmj1 - 0, std0);

				VEC_CMPLTU_EPI8(std0_reg, N_img_diff_reg, cmplt);
				VEC_CMPGTU_EPI8(std0_reg, N_img_diff_reg, cmpgt);	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);

				std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std0_reg, reslt), resgt); 

				VEC_MINU_EPI8(std1_reg, VMAX_reg, std1_min);

				// clamp to [Vmin,Vmax]
				VEC_MAXU_EPI8(std1_min, VMIN_reg, std1_max);

				VEC_STORE_2D_EPI8(std1_max, i, vmj1 - 0, std1);

			default:
				break;
		}
	}
}

void SigmaDelta_step4_simd(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** std1, vuint8** img_diff, vuint8** img_bin){

	vuint8 std1_reg, img_diff_reg, img_bin_reg;
	vuint8 cmplt, ones;

	vuint16 x1_lt, x2_lt, y1_lt, y2_lt;
	vuint16 cmplt1, cmplt2;

	ones = init_vuint8(1);

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1; ++j)
		{
			std1_reg 		= VEC_LOAD_2D_EPI8(i, j, std1);
			img_diff_reg 	= VEC_LOAD_2D_EPI8(i, j, img_diff);

			// DEBUG(display_vuint8(std1_reg, "%03d ", "std1_reg : ")); DEBUG(printf("\n\n"));
			// DEBUG(display_vuint8(img_diff_reg, "%03d ", "img_diff_reg : ")); DEBUG(printf("\n\n"));

			VEC_CMPLTU_EPI8(std1_reg, img_diff_reg, cmplt);

			// DEBUG(display_vuint8(cmplt, "%03d ", "cmpgt : ")); DEBUG(printf("\n\n"));

			img_bin_reg = VEC_AND_EPI8(cmplt, ones);

			// DEBUG(display_vuint8(img_bin_reg, "%03d ", "img_bin_reg : ")); DEBUG(printf("\n\n"));

			VEC_STORE_2D_EPI8(img_bin_reg, i, j, img_bin);
		}
	}
}

void SigmaDelta_step4_simd_opti(int vmi0, int vmi1, int vmj0, int vmj1, vuint8** std1, vuint8** img_diff, vuint8** img_bin){

	int k = 4; int r = (vmj1+1) % k;

	vuint8 std1_reg, img_diff_reg, img_bin_reg;
	vuint8 cmplt, ones;

	vuint16 x1_lt, x2_lt, y1_lt, y2_lt;
	vuint16 cmplt1, cmplt2;

	ones = init_vuint8(1);

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1 - r; j = j + k)
		{
			std1_reg 		= VEC_LOAD_2D_EPI8(i, j + 0, std1);
			img_diff_reg 	= VEC_LOAD_2D_EPI8(i, j + 0, img_diff);

			VEC_CMPLTU_EPI8(std1_reg, img_diff_reg, cmplt);

			img_bin_reg = VEC_AND_EPI8(cmplt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j + 0, img_bin);

			/*--------------------------------------------------*/

			std1_reg 		= VEC_LOAD_2D_EPI8(i, j + 1, std1);
			img_diff_reg 	= VEC_LOAD_2D_EPI8(i, j + 1, img_diff);

			VEC_CMPLTU_EPI8(std1_reg, img_diff_reg, cmplt);

			img_bin_reg = VEC_AND_EPI8(cmplt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j + 1, img_bin);

			/*--------------------------------------------------*/

			std1_reg 		= VEC_LOAD_2D_EPI8(i, j + 2, std1);
			img_diff_reg 	= VEC_LOAD_2D_EPI8(i, j + 2, img_diff);

			VEC_CMPLTU_EPI8(std1_reg, img_diff_reg, cmplt);

			img_bin_reg = VEC_AND_EPI8(cmplt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j + 2, img_bin);

			/*--------------------------------------------------*/

			std1_reg 		= VEC_LOAD_2D_EPI8(i, j + 3, std1);
			img_diff_reg 	= VEC_LOAD_2D_EPI8(i, j + 3, img_diff);

			VEC_CMPLTU_EPI8(std1_reg, img_diff_reg, cmplt);

			img_bin_reg = VEC_AND_EPI8(cmplt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j + 3, img_bin);
		}

		// EPILOGUE
		switch(r){

			case 3:
				std1_reg 		= VEC_LOAD_2D_EPI8(i, vmj1 - 2, std1);
				img_diff_reg 	= VEC_LOAD_2D_EPI8(i, vmj1 - 2, img_diff);

				VEC_CMPLTU_EPI8(std1_reg, img_diff_reg, cmplt);

				img_bin_reg = VEC_AND_EPI8(cmplt, ones);

				VEC_STORE_2D_EPI8(img_bin_reg, i, vmj1 - 2, img_bin);

			case 2:
				std1_reg 		= VEC_LOAD_2D_EPI8(i, vmj1 - 1, std1);
				img_diff_reg 	= VEC_LOAD_2D_EPI8(i, vmj1 - 1, img_diff);

				VEC_CMPLTU_EPI8(std1_reg, img_diff_reg, cmplt);

				img_bin_reg = VEC_AND_EPI8(cmplt, ones);

				VEC_STORE_2D_EPI8(img_bin_reg, i, vmj1 - 1, img_bin);

			case 1:
				std1_reg 		= VEC_LOAD_2D_EPI8(i, vmj1 - 0, std1);
				img_diff_reg 	= VEC_LOAD_2D_EPI8(i, vmj1 - 0, img_diff);

				VEC_CMPLTU_EPI8(std1_reg, img_diff_reg, cmplt);

				img_bin_reg = VEC_AND_EPI8(cmplt, ones);

				VEC_STORE_2D_EPI8(img_bin_reg, i, vmj1 - 0, img_bin);

			default:
				break;
		}
	}
}

void SigmaDelta_simd_full(int vmi0b, int vmi1b, int vmj0b, int vmj1b,  vuint8** image, vuint8** mean0, vuint8** std0, vuint8** img_bin){

	// UN SEULE DOUBLE BOUCLE FOR + SCALARISATION

	vuint8 mean0_reg, mean1_reg, image_reg;
	vuint8 std0_reg, std1_reg;
	vuint8 N_reg, N_img_diff_reg, VMAX_reg, VMIN_reg;
	vuint8 img_diff_reg, img_bin_reg;

	vuint8 cmplt, cmpgt, reslt, resgt, ones;

	N_reg 		= init_vuint8(N);
	ones  		= init_vuint8(1);
	VMAX_reg	= init_vuint8(VMAX);
	VMIN_reg	= init_vuint8(VMIN);

	for (int i = vmi0b; i <= vmi1b; ++i)
	{
		for (int j = vmj0b; j <= vmj1b; ++j)
		{

			// STEP 1
			mean0_reg = VEC_LOAD_2D_EPI8(i, j, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j, image);

			cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
			cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			/*----------------------------------------------------------------*/
			
			// STEP 2
			img_diff_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg, image_reg));

			/*----------------------------------------------------------------*/

			// STEP 3

			// simul multiplication
			for(int k = 0; k < N; ++k)
			{
				N_img_diff_reg = VEC_ADD_EPI8(img_diff_reg, N_reg);
			}

			std0_reg = VEC_LOAD_2D_EPI8(i, j, std0);

			cmplt = VEC_CMPLT_EPI8(std0_reg, N_img_diff_reg);
			cmpgt = VEC_CMPGT_EPI8(std0_reg, N_img_diff_reg);

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std1_reg, reslt), resgt); 

			// clamp to [Vmin,Vmax]
			std1_reg = VEC_MAX_EPI8(VEC_MIN_EPI8(std1_reg, VMAX_reg), VMIN_reg);

			/*----------------------------------------------------------------*/

			// STEP 4
			cmpgt = VEC_CMPLT_EPI8(std1_reg, img_diff_reg);

			img_bin_reg = VEC_AND_EPI8(cmpgt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j, img_bin);
		}
	}
}

void SigmaDelta_simd_full_opti(int vmi0b, int vmi1b, int vmj0b, int vmj1b,  vuint8** image, vuint8** mean0, vuint8** std0, vuint8** img_bin){

	// UN SEULE DOUBLE BOUCLE FOR + SCALARISATION + DEROULEMENT BOUCLE J

	int k = 4; int r = vmj1b % k;

	vuint8 mean0_reg, mean1_reg, image_reg;
	vuint8 std0_reg, std1_reg;
	vuint8 N_reg, N_img_diff_reg, VMAX_reg, VMIN_reg;
	vuint8 img_diff_reg, img_bin_reg;

	vuint8 cmplt, cmpgt, reslt, resgt, ones;

	N_reg 		= init_vuint8(N);
	ones  		= init_vuint8(1);
	VMAX_reg	= init_vuint8(VMAX);
	VMIN_reg	= init_vuint8(VMIN);

	for (int i = vmi0b; i <= vmi1b; ++i)
	{
		for (int j = vmj0b; j <= vmj1b - r; j = j + k)
		{
			/*----------------------------------------------------------------*/			
			/*---------------------- Iteration j + 0 -------------------------*/
			/*----------------------------------------------------------------*/

			// STEP 1
			mean0_reg = VEC_LOAD_2D_EPI8(i, j + 0, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 0, image);

			cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
			cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			/*----------------------------------------------------------------*/
			
			// STEP 2
			img_diff_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg, image_reg));

			/*----------------------------------------------------------------*/

			// STEP 3

			// simul multiplication
			for(int k = 0; k < N; ++k)
			{
				N_img_diff_reg = VEC_ADD_EPI8(img_diff_reg, N_reg);
			}

			std0_reg = VEC_LOAD_2D_EPI8(i, j + 0, std0);

			cmplt = VEC_CMPLT_EPI8(std0_reg, N_img_diff_reg);
			cmpgt = VEC_CMPGT_EPI8(std0_reg, N_img_diff_reg);

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std1_reg, reslt), resgt); 

			// clamp to [Vmin,Vmax]
			std1_reg = VEC_MAX_EPI8(VEC_MIN_EPI8(std1_reg, VMAX_reg), VMIN_reg);

			/*----------------------------------------------------------------*/

			// STEP 4
			cmpgt = VEC_CMPLT_EPI8(std1_reg, img_diff_reg);

			img_bin_reg = VEC_AND_EPI8(cmpgt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j + 0, img_bin);

			/*----------------------------------------------------------------*/			
			/*---------------------- Iteration j + 1 -------------------------*/
			/*----------------------------------------------------------------*/

			// STEP 1
			mean0_reg = VEC_LOAD_2D_EPI8(i, j + 1, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 1, image);

			cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
			cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			/*----------------------------------------------------------------*/
			
			// STEP 2
			img_diff_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg, image_reg));

			/*----------------------------------------------------------------*/

			// STEP 3

			// simul multiplication
			for(int k = 0; k < N; ++k)
			{
				N_img_diff_reg = VEC_ADD_EPI8(img_diff_reg, N_reg);
			}

			std0_reg = VEC_LOAD_2D_EPI8(i, j + 1, std0);

			cmplt = VEC_CMPLT_EPI8(std0_reg, N_img_diff_reg);
			cmpgt = VEC_CMPGT_EPI8(std0_reg, N_img_diff_reg);

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std1_reg, reslt), resgt); 

			// clamp to [Vmin,Vmax]
			std1_reg = VEC_MAX_EPI8(VEC_MIN_EPI8(std1_reg, VMAX_reg), VMIN_reg);

			/*----------------------------------------------------------------*/

			// STEP 4
			cmpgt = VEC_CMPLT_EPI8(std1_reg, img_diff_reg);

			img_bin_reg = VEC_AND_EPI8(cmpgt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j + 1, img_bin);

			/*----------------------------------------------------------------*/			
			/*---------------------- Iteration j + 2 -------------------------*/
			/*----------------------------------------------------------------*/

			// STEP 1
			mean0_reg = VEC_LOAD_2D_EPI8(i, j + 2, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 2, image);

			cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
			cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			/*----------------------------------------------------------------*/
			
			// STEP 2
			img_diff_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg, image_reg));

			/*----------------------------------------------------------------*/

			// STEP 3

			// simul multiplication
			for(int k = 0; k < N; ++k)
			{
				N_img_diff_reg = VEC_ADD_EPI8(img_diff_reg, N_reg);
			}

			std0_reg = VEC_LOAD_2D_EPI8(i, j + 2, std0);

			cmplt = VEC_CMPLT_EPI8(std0_reg, N_img_diff_reg);
			cmpgt = VEC_CMPGT_EPI8(std0_reg, N_img_diff_reg);

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std1_reg, reslt), resgt); 

			// clamp to [Vmin,Vmax]
			std1_reg = VEC_MAX_EPI8(VEC_MIN_EPI8(std1_reg, VMAX_reg), VMIN_reg);

			/*----------------------------------------------------------------*/

			// STEP 4
			cmpgt = VEC_CMPLT_EPI8(std1_reg, img_diff_reg);

			img_bin_reg = VEC_AND_EPI8(cmpgt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j + 2, img_bin);

			/*----------------------------------------------------------------*/			
			/*---------------------- Iteration j + 3 -------------------------*/
			/*----------------------------------------------------------------*/

			// STEP 1
			mean0_reg = VEC_LOAD_2D_EPI8(i, j + 3, mean0);
			image_reg = VEC_LOAD_2D_EPI8(i, j + 3, image);

			cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
			cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);
			
			mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

			/*----------------------------------------------------------------*/
			
			// STEP 2
			img_diff_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg, image_reg));

			/*----------------------------------------------------------------*/

			// STEP 3

			// simul multiplication
			for(int k = 0; k < N; ++k)
			{
				N_img_diff_reg = VEC_ADD_EPI8(img_diff_reg, N_reg);
			}

			std0_reg = VEC_LOAD_2D_EPI8(i, j + 3, std0);

			cmplt = VEC_CMPLT_EPI8(std0_reg, N_img_diff_reg);
			cmpgt = VEC_CMPGT_EPI8(std0_reg, N_img_diff_reg);

			reslt = VEC_AND_EPI8(cmplt, ones);
			resgt = VEC_AND_EPI8(cmpgt, ones);

			std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std1_reg, reslt), resgt); 

			// clamp to [Vmin,Vmax]
			std1_reg = VEC_MAX_EPI8(VEC_MIN_EPI8(std1_reg, VMAX_reg), VMIN_reg);

			/*----------------------------------------------------------------*/

			// STEP 4
			cmpgt = VEC_CMPLT_EPI8(std1_reg, img_diff_reg);

			img_bin_reg = VEC_AND_EPI8(cmpgt, ones);

			VEC_STORE_2D_EPI8(img_bin_reg, i, j + 3, img_bin);
		}

		// EPILOGUE
		switch(r){

			case 3:
				// STEP 1
				mean0_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 2, mean0);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 2, image);

				cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
				cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);
				
				mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

				// STEP 2
				img_diff_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg, image_reg));

				// STEP 3
				// simul multiplication
				for(int k = 0; k < N; ++k)
				{
					N_img_diff_reg = VEC_ADD_EPI8(img_diff_reg, N_reg);
				}

				std0_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 2, std0);

				cmplt = VEC_CMPLT_EPI8(std0_reg, N_img_diff_reg);
				cmpgt = VEC_CMPGT_EPI8(std0_reg, N_img_diff_reg);

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);

				std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std1_reg, reslt), resgt); 

				// clamp to [Vmin,Vmax]
				std1_reg = VEC_MAX_EPI8(VEC_MIN_EPI8(std1_reg, VMAX_reg), VMIN_reg);

				// STEP 4
				cmpgt = VEC_CMPLT_EPI8(std1_reg, img_diff_reg);

				img_bin_reg = VEC_AND_EPI8(cmpgt, ones);

				VEC_STORE_2D_EPI8(img_bin_reg, i, vmj1b - 2, img_bin);

			case 2:
				// STEP 1
				mean0_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 1, mean0);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 1, image);

				cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
				cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);
				
				mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

				// STEP 2
				img_diff_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg, image_reg));

				// STEP 3
				// simul multiplication
				for(int k = 0; k < N; ++k)
				{
					N_img_diff_reg = VEC_ADD_EPI8(img_diff_reg, N_reg);
				}

				std0_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 1, std0);

				cmplt = VEC_CMPLT_EPI8(std0_reg, N_img_diff_reg);
				cmpgt = VEC_CMPGT_EPI8(std0_reg, N_img_diff_reg);

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);

				std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std1_reg, reslt), resgt); 

				// clamp to [Vmin,Vmax]
				std1_reg = VEC_MAX_EPI8(VEC_MIN_EPI8(std1_reg, VMAX_reg), VMIN_reg);

				// STEP 4
				cmpgt = VEC_CMPLT_EPI8(std1_reg, img_diff_reg);

				img_bin_reg = VEC_AND_EPI8(cmpgt, ones);

				VEC_STORE_2D_EPI8(img_bin_reg, i, vmj1b - 1, img_bin);

			case 1:
				// STEP 1
				mean0_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 0, mean0);
				image_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 0, image);

				cmplt = VEC_CMPLT_EPI8(mean0_reg, image_reg);
				cmpgt = VEC_CMPGT_EPI8(mean0_reg, image_reg);	

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);
				
				mean1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(mean0_reg, reslt), resgt); 

				// STEP 2
				img_diff_reg = VEC_ABS_EPI8(VEC_SUB_EPI8(mean1_reg, image_reg));

				// STEP 3
				// simul multiplication
				for(int k = 0; k < N; ++k)
				{
					N_img_diff_reg = VEC_ADD_EPI8(img_diff_reg, N_reg);
				}

				std0_reg = VEC_LOAD_2D_EPI8(i, vmj1b - 0, std0);

				cmplt = VEC_CMPLT_EPI8(std0_reg, N_img_diff_reg);
				cmpgt = VEC_CMPGT_EPI8(std0_reg, N_img_diff_reg);

				reslt = VEC_AND_EPI8(cmplt, ones);
				resgt = VEC_AND_EPI8(cmpgt, ones);

				std1_reg = VEC_SUB_EPI8(VEC_ADD_EPI8(std1_reg, reslt), resgt); 

				// clamp to [Vmin,Vmax]
				std1_reg = VEC_MAX_EPI8(VEC_MIN_EPI8(std1_reg, VMAX_reg), VMIN_reg);

				// STEP 4
				cmpgt = VEC_CMPLT_EPI8(std1_reg, img_diff_reg);

				img_bin_reg = VEC_AND_EPI8(cmpgt, ones);

				VEC_STORE_2D_EPI8(img_bin_reg, i, vmj1b - 0, img_bin);

			default:
				break;
		}
	}
}

void ui8matrix_to_vui8matrix(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8** img, vuint8** img_simd){

	for (int i = vmi0b; i <= vmi1b; ++i)
	{
		for (int j = vmj0b; j <= vmj1b; ++j)
		{
			if (j == vmj0b)
			{
				img_simd[i][j] = init_vuint8(img[i][j]);
			}
			else if (j == vmj1b)
			{
				img_simd[i][j] = init_vuint8(img[i][j*card]);
			}
			else{
				img_simd[i][j] = init_vuint8_all(	img[i][(j * card) + 0 ], img[i][(j * card) + 1 ], img[i][(j * card) + 2 ], img[i][(j * card) + 3 ], 
													img[i][(j * card) + 4 ], img[i][(j * card) + 5 ], img[i][(j * card) + 6 ], img[i][(j * card) + 7 ], 
													img[i][(j * card) + 8 ], img[i][(j * card) + 9 ], img[i][(j * card) + 10], img[i][(j * card) + 11], 
													img[i][(j * card) + 12], img[i][(j * card) + 13], img[i][(j * card) + 14], img[i][(j * card) + 15]);
			}
		} 
	}
}

void vui8matrix_to_ui8matrix(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8** img, vuint8** img_simd){

	for (int i = vmi0b; i <= vmi1b; ++i)
	{
		for (int j = vmj0b; j <= vmj1b; ++j)
		{
			if (j == vmj0b)
			{
				img[i][j] = (uint8)(_mm_extract_epi8(img_simd[i][j], 15));
			}
			else if (j == vmj1b)
			{
				img[i][j] = (uint8)(_mm_extract_epi8(img_simd[i][j], 0));	
			}
			else{

				img[i][j * card + 0 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 0 ));
				img[i][j * card + 1 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 1 ));
				img[i][j * card + 2 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 2 ));
				img[i][j * card + 3 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 3 ));
				img[i][j * card + 4 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 4 ));
				img[i][j * card + 5 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 5 ));
				img[i][j * card + 6 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 6 ));
				img[i][j * card + 7 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 7 ));
				img[i][j * card + 8 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 8 ));
				img[i][j * card + 9 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 9 ));
				img[i][j * card + 10] = (uint8)(_mm_extract_epi8(img_simd[i][j], 10));
				img[i][j * card + 11] = (uint8)(_mm_extract_epi8(img_simd[i][j], 11));
				img[i][j * card + 12] = (uint8)(_mm_extract_epi8(img_simd[i][j], 12));
				img[i][j * card + 13] = (uint8)(_mm_extract_epi8(img_simd[i][j], 13));
				img[i][j * card + 14] = (uint8)(_mm_extract_epi8(img_simd[i][j], 14));
				img[i][j * card + 15] = (uint8)(_mm_extract_epi8(img_simd[i][j], 15));
			}
		} 
	}
}