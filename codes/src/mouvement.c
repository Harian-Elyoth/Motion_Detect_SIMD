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

void SigmaDelta_step1(int mi0b, int mi1b, int mj0b, int mj1b, uint8** mean0, uint8** mean1, uint8** image){

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b; ++j)
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

void SigmaDelta_step1_opti(int mi0b, int mi1b, int mj0b, int mj1b, uint8** mean0, uint8** mean1, uint8** image){

	int k = 4; 
	int r = mj1b % k;

	DEBUG(printf("mj1b - r = %d\n", mj1b-r));

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b - r; j = j + k)
		{

			int meanj0 = mean0[i][j];
			if (meanj0 < image[i][j]){
				mean1[i][j] = meanj0 + 1;
			}
			else if (meanj0 > image[i][j]){
				mean1[i][j] = meanj0 - 1;
			}
			else{
				mean1[i][j] = meanj0;
			}

			/*---------------------------------------------------*/

			int meanj1 = mean0[i][j+1];
			if (meanj1 < image[i][j+1]){
				mean1[i][j+1] = meanj1 + 1;
			}
			else if (meanj1 > image[i][j+1]){
				mean1[i][j+1] = meanj1 - 1;
			}
			else{
				mean1[i][j+1] = meanj1;
			}

			/*---------------------------------------------------*/

			int meanj2 = mean0[i][j+2];
			if (meanj2 < image[i][j+2]){
				mean1[i][j+2] = meanj2 + 1;
			}
			else if (meanj2 > image[i][j+2]){
				mean1[i][j+2] = meanj2 - 1;
			}
			else{
				mean1[i][j+2] = meanj2;
			}

			/*---------------------------------------------------*/

			int meanj3 = mean0[i][j+3];
			if (meanj3 < image[i][j+3]){
				mean1[i][j+3] = meanj3 + 1;
			}
			else if (meanj3 > image[i][j+3]){
				mean1[i][j+3] = meanj3 - 1;
			}
			else{
				mean1[i][j+3] = meanj3;
			}
		}
	}
}

void SigmaDelta_step2(int mi0b, int mi1b, int mj0b, int mj1b, uint8** image, uint8** mean1, uint8** img_diff){

	for (int i = mi0b; i <= mi1b; ++i){

		for (int j = mj0b; j <= mj1b; ++j){
			img_diff[i][j] = abs(mean1[i][j] - image[i][j]);
		}
	}
}

void SigmaDelta_step2_opti(int mi0b, int mi1b, int mj0b, int mj1b, uint8** image, uint8** mean1, uint8** img_diff){

	int k = 4;
	int r = mj1b % k;

	for (int i = mi0b; i <= mi1b; ++i){

		for (int j = mj0b; j <= mj1b - r ; j = j + k){
			
			img_diff[i][j + 0] = abs(mean1[i][j + 0] - image[i][j + 0]);
			img_diff[i][j + 1] = abs(mean1[i][j + 1] - image[i][j + 1]);
			img_diff[i][j + 2] = abs(mean1[i][j + 2] - image[i][j + 2]);
			img_diff[i][j + 3] = abs(mean1[i][j + 3] - image[i][j + 3]);
		}
	}
}

void SigmaDelta_step3(int mi0b, int mi1b, int mj0b, int mj1b, uint8** std0, uint8** std1, uint8** img_diff){

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

void SigmaDelta_step3_opti(int mi0b, int mi1b, int mj0b, int mj1b, uint8** std0, uint8** std1, uint8** img_diff){

	int k = 4;
	int r = mj1b % k;

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b - r; j = j + k)
		{
			int std0_j0 = std0[i][j + 0];
			int std1_j0 = std1[i][j + 0];

			if (std0_j0 < N * img_diff[i][j + 0]){
				std1_j0 = std0_j0 + 1;
			}

			else if (std0_j0 > N * img_diff[i][j + 0]){
				std1_j0 = std0_j0 - 1;
			}

			else{
				std1_j0 = std0_j0;
			}

			/*---------------------------------------------------*/

			int std0_j1 = std0[i][j + 1];
			int std1_j1 = std1[i][j + 1];

			if (std0_j1 < N * img_diff[i][j + 1]){
				std1_j1 = std0_j1 + 1;
			}

			else if (std0_j1 > N * img_diff[i][j + 1]){
				std1_j1 = std0_j1 - 1;
			}

			else{
				std1_j1 = std0_j1;
			}

			/*---------------------------------------------------*/
			
			int std0_j2 = std0[i][j + 2];
			int std1_j2 = std1[i][j + 2];

			if (std0_j2 < N * img_diff[i][j + 2]){
				std1_j2 = std0_j2 + 1;
			}

			else if (std0_j2 > N * img_diff[i][j + 2]){
				std1_j2 = std0_j2 - 1;
			}

			else{
				std1_j2 = std0_j2;
			}

			/*---------------------------------------------------*/
			
			int std0_j3 = std0[i][j + 3];
			int std1_j3 = std1[i][j + 3];

			if (std0_j3 < N * img_diff[i][j + 3]){
				std1_j3 = std0_j3 + 1;
			}

			else if (std0_j3 > N * img_diff[i][j + 3]){
				std1_j3 = std0_j3 - 1;
			}

			else{
				std1_j3 = std0_j3;
			}

			/*---------------------------------------------------*/
			
			// clamp to [Vmin,Vmax]
			std1[i][j + 0] = MAX(MIN(std1_j0, VMAX), VMIN);
			std1[i][j + 1] = MAX(MIN(std1_j1, VMAX), VMIN);
			std1[i][j + 2] = MAX(MIN(std1_j2, VMAX), VMIN);
			std1[i][j + 3] = MAX(MIN(std1_j3, VMAX), VMIN);
		}
	}
}

void SigmaDelta_step4(int mi0b, int mi1b, int mj0b, int mj1b, uint8** std1, uint8** img_diff, uint8** img_bin){

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

void SigmaDelta_step4_opti(int mi0b, int mi1b, int mj0b, int mj1b, uint8** std1, uint8** img_diff, uint8** img_bin){

	int k = 4;
	int r = mj1b % k;

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b - r; j = j + k)
		{
			if (img_diff[i][j + 0] < std1[i][j + 0]){
				img_bin[i][j + 0] = 0;
			}
			else{
				img_bin[i][j + 0] = 1;
			}

			/*---------------------------------------------------*/

			if (img_diff[i][j + 1] < std1[i][j + 1]){
				img_bin[i][j + 1] = 0;
			}
			else{
				img_bin[i][j + 1] = 1;
			}

			/*---------------------------------------------------*/

			if (img_diff[i][j + 2] < std1[i][j + 2]){
				img_bin[i][j + 2] = 0;
			}
			else{
				img_bin[i][j + 2] = 1;
			}

			/*---------------------------------------------------*/

			if (img_diff[i][j + 3] < std1[i][j + 3]){
				img_bin[i][j + 3] = 0;
			}
			else{
				img_bin[i][j + 3] = 1;
			}
		}
	}
}

void gen_pgm_img(int mi0, int mi1, int mj0, int mj1, int b, uint8** mean0, uint8** std0, uint8** image){
   
   	// indices matrices avec bord
	int mi0b = mi0-b; int mi1b = mi1+b;
	int mj0b = mj0-b; int mj1b = mj1+b;

   	uint8 **image_t  = ui8matrix(mi0, mi1, mj0, mj1);
   	uint8 **image_t2 = ui8matrix(mi0, mi1, mj0, mj1);

   	for (int i = mi0; i <= mi1; ++i)
   	{
   		for (int j = mj0; j <= mj1; ++j)
   		{
   			image_t [i][j]  = i + j + 11;
   			image_t2[i][j]  = (i*j) + i + j + 11;
   		}
   	}

   	BENCH(display_ui8matrix(image_t,  mi0, mi1, mj0, mj1, "%d ", "\nimage_t :\n"));
   	BENCH(display_ui8matrix(image_t2, mi0, mi1, mj0, mj1, "%d ", "\nimage_t2 :\n"));

   	char *filename_pgm1 = "pgm_imgs/my_pgm1.pgm";
   	char *filename_pgm2 = "pgm_imgs/my_pgm2.pgm";

	// save result on pgm file
	SavePGM_ui8matrix(image_t  , mi0, mi1, mj0, mj1, filename_pgm1);
	SavePGM_ui8matrix(image_t2 , mi0, mi1, mj0, mj1, filename_pgm2);

	// load pgm gen file on images
	MLoadPGM_ui8matrix(filename_pgm1, mi0b, mi1b, mj0b, mj1b, image);

	duplicate_border(mi0, mi1, mj0, mj1, b, image);

	// initiate mean0 et std0 for first iteration
	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b; ++j)
		{
			mean0[i][j] = image[i][j];
			std0[i][j]  = VMIN;
		}
	}

	// load pgm gen file on images
	MLoadPGM_ui8matrix(filename_pgm2, mi0b, mi1b, mj0b, mj1b, image);

	duplicate_border(mi0, mi1, mj0, mj1, b, image);
}

void bin_to_pgm(int mi0b, int mi1b, int mj0b, int mj1b, uint8** img, char* filename){

	// allocate pgm matrix
	uint8** pgm_out = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	for (int i = mi0b; i <= mi1b; ++i)
	{
		for (int j = mj0b; j <= mj1b; ++j)
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
	SavePGM_ui8matrix(pgm_out, mi0b, mi1b, mj0b, mj1b, Fname);
}

