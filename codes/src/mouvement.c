/* ---------------------------------------------------------- */
/* ---  Algorithme Sigma Delta pour le traitement d'image --- */
/* ---------------------------------------------------------- */

#include "mouvement.h"

void duplicate_border(int mi0, int mi1, int mj0, int mj1, int b, uint8** image){

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

void SigmaDelta_step1(int mi0, int mi1, int mj0, int mj1, uint8** mean0, uint8** mean1, uint8** image){

	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
		{
			if (mean0[i][j] < image[i][j]){
				mean1[i][j] = mean0[i][j] + 1;
			}
			else if (mean0[i][j] > image[i][j]){
				mean1[i][j] = mean0[i][j] - 1;
			}
			else{
				mean1[i][j] = mean0[i][j];
			}
		}
	}
}

void SigmaDelta_step1_unroll(int mi0, int mi1, int mj0, int mj1, uint8** mean0, uint8** mean1, uint8** image){

	int k = 4; 
	int r = (mj1+1) % k;



	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j < mj1 - r; j = j + k)
		{
			int meanj0 = mean0[i][j];
			if (meanj0 < image[i][j]){
				mean1[i][j] = meanj0 + 1;
			}
			else if (meanj0 > image[i][j]){
				mean1[i][j] = meanj0 - 1;
			}
			else{ mean1[i][j] = meanj0; }
			
			/*---------------------------------------------------*/
			
			int meanj1 = mean0[i][j+1];
			if (meanj1 < image[i][j+1]){
				mean1[i][j+1] = meanj1 + 1;
			}
			else if (meanj1 > image[i][j+1]){
				mean1[i][j+1] = meanj1 - 1;
			}
			else{ mean1[i][j+1] = meanj1; }

			/*---------------------------------------------------*/

			int meanj2 = mean0[i][j+2];
			if (meanj2 < image[i][j+2]){
				mean1[i][j+2] = meanj2 + 1;
			}
			else if (meanj2 > image[i][j+2]){
				mean1[i][j+2] = meanj2 - 1;
			}
			else{ mean1[i][j+2] = meanj2; }

			/*---------------------------------------------------*/

			int meanj3 = mean0[i][j+3];
			if (meanj3 < image[i][j+3]){
				mean1[i][j+3] = meanj3 + 1;
			}
			else if (meanj3 > image[i][j+3]){
				mean1[i][j+3] = meanj3 - 1;
			}
			else{ mean1[i][j+3] = meanj3; }
		}
		// EPILOGUE
		uint8 meanj0;
		uint8 meanj1;	
		uint8 meanj2;
		switch(r){

			case 3:
				meanj2 = mean0[i][mj1 - 2];
				if (meanj2 < image[i][mj1 - 2]){
					mean1[i][mj1 - 2] = meanj2 + 1;
				}
				else if (meanj2 > image[i][mj1 - 2]){
					mean1[i][mj1 - 2] = meanj2 - 1;
				}
				else{
					mean1[i][mj1 - 2] = meanj2;
				}

			case 2:
				meanj1 = mean0[i][mj1 - 1];
				if (meanj1 < image[i][mj1 - 1]){
					mean1[i][mj1 - 1] = meanj1 + 1;
				}
				else if (meanj1 > image[i][mj1 - 1]){
					mean1[i][mj1 - 1] = meanj1 - 1;
				}
				else{
					mean1[i][mj1 - 1] = meanj1;
				}

			case 1:
				meanj0 = mean0[i][mj1 - 0];
				if (meanj0 < image[i][mj1 - 0]){
					mean1[i][mj1 - 0] = meanj0 + 1;
				}
				else if (meanj0 > image[i][mj1 - 0]){
					mean1[i][mj1 - 0] = meanj0 - 1;
				}
				else{
					mean1[i][mj1 - 0] = meanj0;
				}

			default:
				break;
		}
	}
}

void SigmaDelta_step2(int mi0, int mi1, int mj0, int mj1, uint8** image, uint8** mean1, uint8** img_diff){

	for (int i = mi0; i <= mi1; ++i){

		for (int j = mj0; j <= mj1; ++j){
			img_diff[i][j] = abs(mean1[i][j] - image[i][j]);
		}
	}
}

void SigmaDelta_step2_unroll(int mi0, int mi1, int mj0, int mj1, uint8** image, uint8** mean1, uint8** img_diff){

	int k = 4;
	int r = (mj1+1) % k;

	for (int i = mi0; i <= mi1; ++i){

		for (int j = mj0; j <= mj1 - r ; j = j + k){

			img_diff[i][j + 0] = abs(mean1[i][j + 0] - image[i][j + 0]);
			img_diff[i][j + 1] = abs(mean1[i][j + 1] - image[i][j + 1]);
			img_diff[i][j + 2] = abs(mean1[i][j + 2] - image[i][j + 2]);
			img_diff[i][j + 3] = abs(mean1[i][j + 3] - image[i][j + 3]);
		}
		//EPILOGUE
		switch(r){

			case 3:
				img_diff[i][mj1 - 2] = abs(mean1[i][mj1 - 2] - image[i][mj1 - 2]);

			case 2:
				img_diff[i][mj1 - 1] = abs(mean1[i][mj1 - 1] - image[i][mj1 - 1]);

			case 1:
				img_diff[i][mj1 - 0] = abs(mean1[i][mj1 - 0] - image[i][mj1 - 0]);

			default:
				break;
		}
	}
}

void SigmaDelta_step3(int mi0, int mi1, int mj0, int mj1, uint8** std0, uint8** std1, uint8** img_diff){

	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
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

void SigmaDelta_step3_unroll(int mi0, int mi1, int mj0, int mj1, uint8** std0, uint8** std1, uint8** img_diff){

	int k = 4;
	int r = (mj1+1) % k;

	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1 - r; j = j + k)
		{
			int std0_j0 = std0[i][j + 0];
			int std1_j0 = std1[i][j + 0];

			if (std0_j0 < N * img_diff[i][j + 0]){
				std1_j0 = std0_j0 + 1;
			}
			else if (std0_j0 > N * img_diff[i][j + 0]){
				std1_j0 = std0_j0 - 1;
			}
			else{ std1_j0 = std0_j0; }
			/*---------------------------------------------------*/
			int std0_j1 = std0[i][j + 1];
			int std1_j1 = std1[i][j + 1];

			if (std0_j1 < N * img_diff[i][j + 1]){
				std1_j1 = std0_j1 + 1;
			}
			else if (std0_j1 > N * img_diff[i][j + 1]){
				std1_j1 = std0_j1 - 1;
			}
			else{ std1_j1 = std0_j1; }
			/*---------------------------------------------------*/
			int std0_j2 = std0[i][j + 2];
			int std1_j2 = std1[i][j + 2];

			if (std0_j2 < N * img_diff[i][j + 2]){
				std1_j2 = std0_j2 + 1;
			}
			else if (std0_j2 > N * img_diff[i][j + 2]){
				std1_j2 = std0_j2 - 1;
			}
			else{ std1_j2 = std0_j2; }
			/*---------------------------------------------------*/
			int std0_j3 = std0[i][j + 3];
			int std1_j3 = std1[i][j + 3];

			if (std0_j3 < N * img_diff[i][j + 3]){
				std1_j3 = std0_j3 + 1;
			}
			else if (std0_j3 > N * img_diff[i][j + 3]){
				std1_j3 = std0_j3 - 1;
			}
			else{ std1_j3 = std0_j3; }
			/*---------------------------------------------------*/
	
			// clamp to [Vmin,Vmax]
			std1[i][j + 0] = MAX(MIN(std1_j0, VMAX), VMIN);
			std1[i][j + 1] = MAX(MIN(std1_j1, VMAX), VMIN);
			std1[i][j + 2] = MAX(MIN(std1_j2, VMAX), VMIN);
			std1[i][j + 3] = MAX(MIN(std1_j3, VMAX), VMIN);
		}
		//EPILOGUE
		uint8 std0_j0, std1_j0;
		uint8 std0_j1, std1_j1;
		uint8 std0_j2, std1_j2;
		switch(r){

			case 3:
				std0_j2 = std0[i][mj1 - 2];
				std1_j2 = std1[i][mj1 - 2];
				if (std0_j2 < N * img_diff[i][mj1 - 2]){
					std1_j2 = std0_j2 + 1;
				}
				else if (std0_j2 > N * img_diff[i][mj1 - 2]){
					std1_j2 = std0_j2 - 1;
				}
				else{ std1_j2 = std0_j2; }
				std1[i][mj1 - 2] = MAX(MIN(std1_j2, VMAX), VMIN);

			case 2:
				std0_j2 = std0[i][mj1 - 1];
				std1_j2 = std1[i][mj1 - 1];
				if (std0_j2 < N * img_diff[i][mj1 - 1]){
					std1_j2 = std0_j2 + 1;
				}
				else if (std0_j2 > N * img_diff[i][mj1 - 1]){
					std1_j2 = std0_j2 - 1;
				}
				else{ std1_j2 = std0_j2; }
				std1[i][mj1 - 1] = MAX(MIN(std1_j2, VMAX), VMIN);

			case 1:
				std0_j2 = std0[i][mj1 - 0];
				std1_j2 = std1[i][mj1 - 0];
				if (std0_j2 < N * img_diff[i][mj1 - 0]){
					std1_j2 = std0_j2 + 1;
				}
				else if (std0_j2 > N * img_diff[i][mj1 - 0]){
					std1_j2 = std0_j2 - 1;
				}
				else{ std1_j2 = std0_j2; }
				std1[i][mj1 - 0] = MAX(MIN(std1_j2, VMAX), VMIN);

			default:
				break;
		}
	}
}

void SigmaDelta_step4(int mi0, int mi1, int mj0, int mj1, uint8** std1, uint8** img_diff, uint8** img_bin){

	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
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

void SigmaDelta_step4_unroll(int mi0, int mi1, int mj0, int mj1, uint8** std1, uint8** img_diff, uint8** img_bin){

	int k = 4;
	int r = (mj1+1) % k;

	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1 - r; j = j + k)
		{
			if (img_diff[i][j + 0] < std1[i][j + 0]){
				img_bin[i][j + 0] = 0;
			}
			else{ img_bin[i][j + 0] = 1; }
			/*---------------------------------------------------*/
			if (img_diff[i][j + 1] < std1[i][j + 1]){
				img_bin[i][j + 1] = 0;
			}
			else{ img_bin[i][j + 1] = 1; }
			/*---------------------------------------------------*/
			if (img_diff[i][j + 2] < std1[i][j + 2]){
				img_bin[i][j + 2] = 0;
			}
			else{ img_bin[i][j + 2] = 1; }
			/*---------------------------------------------------*/
			if (img_diff[i][j + 3] < std1[i][j + 3]){
				img_bin[i][j + 3] = 0;
			}
			else{ img_bin[i][j + 3] = 1; }
		}

		//EPILOGUE
		switch(r){

			case 3:
				if (img_diff[i][mj1 - 2] < std1[i][mj1 - 2]){
					img_bin[i][mj1 - 2] = 0;
				}
				else{ img_bin[i][mj1 - 2] = 1; }

			case 2:
				if (img_diff[i][mj1 - 1] < std1[i][mj1 - 1]){
					img_bin[i][mj1 - 2] = 0;
				}
				else{ img_bin[i][mj1 - 1] = 1; }

			case 1:
				if (img_diff[i][mj1 - 0] < std1[i][mj1 - 0]){
					img_bin[i][mj1 - 0] = 0;
				}
				else{ img_bin[i][mj1 - 0] = 1; }

			default:
				break;
		}
	}
}

void SigmaDelta_fusion(int mi0, int mi1, int mj0, int mj1,  uint8** image, uint8** mean0, uint8** mean1, uint8** img_diff, uint8** std0, uint8** std1, uint8** img_bin){

	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
		{
			// STEP 1
			if (mean0[i][j] < image[i][j]){
				mean1[i][j] = mean0[i][j] + 1;
			}
			else if (mean0[i][j] > image[i][j]){
				mean1[i][j] = mean0[i][j] - 1;
			}
			else{
				mean1[i][j] = mean0[i][j];
			}

			// STEP 2
			img_diff[i][j] = abs(mean1[i][j] - image[i][j]);

			// STEP 3
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

			// STEP 4
			if (img_diff[i][j] < std1[i][j]){
				img_bin[i][j] = 0;
			}
			else{
				img_bin[i][j] = 1;
			}
		}
	}
}

void SigmaDelta_fusion_unroll(int mi0, int mi1, int mj0, int mj1,  uint8** image, uint8** mean0, uint8** mean1, uint8** std0, uint8** std1, uint8** img_bin){

	int k = 4; 
	int r = (mj1+1) % k;



	uint8 mean0_reg, mean1_reg, image_reg;
	uint8 std0_reg, std1_reg;
	uint8 img_diff_reg, img_bin_reg;

	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1 - r; j = j + k)
		{
			/*---------------------------------------------------*/
			/*-------------------ITERATION J + 0-----------------*/
			/*---------------------------------------------------*/

			// STEP 1
			mean0_reg = mean0[i][j];
			image_reg = image[i][j];

			if (mean0_reg < image_reg){
				mean1_reg = mean0_reg + 1;
			}
			else if (mean0_reg > image_reg){
				mean1_reg = mean0_reg - 1;
			}
			else{
				mean1_reg = mean0_reg;
			}

			mean1[i][j] = mean1_reg;

			/*---------------------------------------------------*/

			// STEP 2
			img_diff_reg = abs(mean1_reg - image_reg);

			/*---------------------------------------------------*/

			// STEP 3
			std0_reg = std0[i][j];

			if (std0_reg < N * img_diff_reg){
				std1_reg = std0_reg + 1;
			}
			else if (std0_reg > N * img_diff_reg){
				std1_reg = std0_reg - 1;
			}
			else{ std1_reg = std0_reg; }

			// clamp to [Vmin,Vmax]
			std1_reg = MAX(MIN(std1_reg, VMAX), VMIN);

			std1[i][j] = std1_reg;

			/*---------------------------------------------------*/

			// STEP 4
			if (img_diff_reg < std1_reg){
				img_bin[i][j] = 0;
			}
			else{ img_bin[i][j] = 1; }

			/*---------------------------------------------------*/
			/*-------------------ITERATION J + 1-----------------*/
			/*---------------------------------------------------*/
			
			// STEP 1
			mean0_reg = mean0[i][j + 1];
			image_reg = image[i][j + 1];

			if (mean0_reg < image_reg){
				mean1_reg = mean0_reg + 1;
			}
			else if (mean0_reg > image_reg){
				mean1_reg = mean0_reg - 1;
			}
			else{ mean1_reg = mean0_reg; }

			mean1[i][j + 1] = mean1_reg;

			/*---------------------------------------------------*/

			// STEP 2
			img_diff_reg = abs(mean1_reg - image_reg);

			/*---------------------------------------------------*/

			// STEP 3
			std0_reg = std0[i][j + 1];

			if (std0_reg < N * img_diff_reg){
				std1_reg = std0_reg + 1;
			}
			else if (std0_reg > N * img_diff_reg){
				std1_reg = std0_reg - 1;
			}
			else{ std1_reg = std0_reg; }

			// clamp to [Vmin,Vmax]
			std1_reg = MAX(MIN(std1_reg, VMAX), VMIN);

			std1[i][j + 1] = std1_reg;

			/*---------------------------------------------------*/

			// STEP 4
			if (img_diff_reg < std1_reg){
				img_bin[i][j + 1] = 0;
			}
			else{ img_bin[i][j + 1] = 1; }

			/*---------------------------------------------------*/
			/*-------------------ITERATION J + 2-----------------*/
			/*---------------------------------------------------*/

			// STEP 1
			mean0_reg = mean0[i][j + 2];
			image_reg = image[i][j + 2];

			if (mean0_reg < image_reg){
				mean1_reg = mean0_reg + 1;
			}
			else if (mean0_reg > image_reg){
				mean1_reg = mean0_reg - 1;
			}
			else{ mean1_reg = mean0_reg; }

			mean1[i][j + 2] = mean1_reg;

			/*---------------------------------------------------*/

			// STEP 2
			img_diff_reg = abs(mean1_reg - image_reg);

			/*---------------------------------------------------*/

			// STEP 3
			std0_reg = std0[i][j + 2];

			if (std0_reg < N * img_diff_reg){
				std1_reg = std0_reg + 1;
			}
			else if (std0_reg > N * img_diff_reg){
				std1_reg = std0_reg - 1;
			}
			else{ std1_reg = std0_reg; }

			// clamp to [Vmin,Vmax]
			std1_reg = MAX(MIN(std1_reg, VMAX), VMIN);

			std1[i][j + 2] = std1_reg;

			/*---------------------------------------------------*/

			// STEP 4
			if (img_diff_reg < std1_reg){
				img_bin[i][j + 2] = 0;
			}
			else{ img_bin[i][j + 2] = 1; }

			/*---------------------------------------------------*/
			/*-------------------ITERATION J + 3-----------------*/
			/*---------------------------------------------------*/

			// STEP 1
			mean0_reg = mean0[i][j + 3];
			image_reg = image[i][j + 3];

			if (mean0_reg < image_reg){
				mean1_reg = mean0_reg + 1;
			}
			else if (mean0_reg > image_reg){
				mean1_reg = mean0_reg - 1;
			}
			else{
				mean1_reg = mean0_reg;
			}

			mean1[i][j + 3] = mean1_reg;

			/*---------------------------------------------------*/

			// STEP 2
			img_diff_reg = abs(mean1_reg - image_reg);

			/*---------------------------------------------------*/

			// STEP 3
			std0_reg = std0[i][j + 3];

			if (std0_reg < N * img_diff_reg){
				std1_reg = std0_reg + 1;
			}
			else if (std0_reg > N * img_diff_reg){
				std1_reg = std0_reg - 1;
			}
			else{ std1_reg = std0_reg; }

			// clamp to [Vmin,Vmax]
			std1_reg = MAX(MIN(std1_reg, VMAX), VMIN);

			std1[i][j + 3] = std1_reg;

			/*---------------------------------------------------*/

			// STEP 4
			if (img_diff_reg < std1_reg){
				img_bin[i][j + 3] = 0;
			}
			else{ img_bin[i][j + 3] = 1; }
		}

		// EPILOGUE
		switch(r){

			case 3:
				// STEP 1
				mean0_reg = mean0[i][mj1 - 2];
				image_reg = image[i][mj1 - 2];

				if (mean0_reg < image_reg){
					mean1_reg = mean0_reg + 1;
				}
				else if (mean0_reg > image_reg){
					mean1_reg = mean0_reg - 1;
				}
				else{ mean1_reg = mean0_reg; }

				mean1[i][mj1 - 2] = mean1_reg;

				// STEP 2
				img_diff_reg = abs(mean1_reg - image_reg);

				// STEP 3
				std0_reg = std0[i][mj1 - 2];

				if (std0_reg < N * img_diff_reg){
					std1_reg = std0_reg + 1;
				}
				else if (std0_reg > N * img_diff_reg){
					std1_reg = std0_reg - 1;
				}
				else{ std1_reg = std0_reg; }

				std1_reg = MAX(MIN(std1_reg, VMAX), VMIN);

				std1[i][mj1 - 2] = std1_reg;

				// STEP 4
				if (img_diff_reg < std1_reg){
					img_bin[i][mj1 - 2] = 0;
				}
				else{ img_bin[i][mj1 - 2] = 1; }

			case 2:
				// STEP 1
				mean0_reg = mean0[i][mj1 - 1];
				image_reg = image[i][mj1 - 1];

				if (mean0_reg < image_reg){
					mean1_reg = mean0_reg + 1;
				}
				else if (mean0_reg > image_reg){
					mean1_reg = mean0_reg - 1;
				}
				else{ mean1_reg = mean0_reg; }

				mean1[i][mj1 - 1] = mean1_reg;

				// STEP 2
				img_diff_reg = abs(mean1_reg - image_reg);

				// STEP 3
				std0_reg = std0[i][mj1 - 1];

				if (std0_reg < N * img_diff_reg){
					std1_reg = std0_reg + 1;
				}
				else if (std0_reg > N * img_diff_reg){
					std1_reg = std0_reg - 1;
				}
				else{ std1_reg = std0_reg; }

				std1_reg = MAX(MIN(std1_reg, VMAX), VMIN);

				std1[i][mj1 - 1] = std1_reg;

				// STEP 4
				if (img_diff_reg < std1_reg){
					img_bin[i][mj1 - 1] = 0;
				}
				else{ img_bin[i][mj1 - 1] = 1; }

			case 1:
				// STEP 1
				mean0_reg = mean0[i][mj1];
				image_reg = image[i][mj1];

				if (mean0_reg < image_reg){
					mean1_reg = mean0_reg + 1;
				}
				else if (mean0_reg > image_reg){
					mean1_reg = mean0_reg - 1;
				}
				else{ mean1_reg = mean0_reg; }

				mean1[i][mj1] = mean1_reg;

				// STEP 2
				img_diff_reg = abs(mean1_reg - image_reg);

				// STEP 3
				std0_reg = std0[i][mj1];

				if (std0_reg < N * img_diff_reg){
					std1_reg = std0_reg + 1;
				}
				else if (std0_reg > N * img_diff_reg){
					std1_reg = std0_reg - 1;
				}
				else{ std1_reg = std0_reg; }

				std1_reg = MAX(MIN(std1_reg, VMAX), VMIN);

				std1[i][mj1] = std1_reg;

				// STEP 4
				if (img_diff_reg < std1_reg){
					img_bin[i][mj1 - 2] = 0;
				}
				else{ img_bin[i][mj1] = 1; }

			default:
				break;
		}
	}
}

void gen_pgm_img(int mi0, int mi1, int mj0, int mj1, uint8** mean0, uint8** std0, uint8** image){
   
	DEBUG(printf("GEN IMG\n"));

   	uint8 **image_t  = ui8matrix(mi0, mi1, mj0, mj1);

   	for (int i = mi0 ; i <= mi1 ; ++i)
   	{
   		for (int j = mj0 ; j <= mj1 ; ++j)
   		{
   			image_t [i][j]  = i + j + 5;

   			if (j % 2 == 0){
   				image   [i][j]  = i + j + 3;
   			}
   			else{
   				image   [i][j]  = i + j + 7;
   			}
   		}
   	}

   	DEBUG(display_ui8matrix(image_t, mi0, mi1, mj0, mj1, "%02d ", "\nimage_gen_1 :"));
   	DEBUG(display_ui8matrix(image  , mi0, mi1, mj0, mj1, "%02d ", "\nimage_gen_2 :"));

	// initiate mean0 et std0 for first iteration
	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
		{
			mean0[i][j] = image_t[i][j];
			std0[i][j]  = VMIN;
		}
	}
}

void bin_to_pgm(int mi0, int mi1, int mj0, int mj1, uint8** img, char* filename){

	// allocate pgm matrix
	uint8** pgm_out = ui8matrix(mi0, mi1, mj0, mj1);

	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
		{
			if (img[i][j] == 0){
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
	SavePGM_ui8matrix(pgm_out, mi0, mi1, mj0, mj1, Fname);
}

void copy_ui8matrix(int mi0, int mi1, int mj0, int mj1, uint8** src, uint8** dest){
	
	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
		{
			dest[i][j] = src[i][j];
		}
	}
}

