/* ----------------------------------------------------------------------- */
/* ---  Test Algorithme Sigma Delta en SIMD pour le traitement d'image --- */
/* ----------------------------------------------------------------------- */

#include "test_mouvement_SIMD.h"

void test_mouvement_SIMD_unit(){

	// img test size
	int width  = 16; // correspond au nb de colonne  => indice boucle j
	int height = 4; // correspond au nb de ligne    => indice boucle i

	char *format = "%03d ";

	puts("====================================");
	puts("=== test mouvement unitaire SIMD ===");
	puts("====================================");


	// cardinalité des registres
	int card = card_vuint8(); // 16

	// ------------------------- //
	// -- calculs des indices -- //
	// ------------------------- //

	// indices scalaires matrices
	int mi0 = 0; int mi1 = height-1;
	int mj0 = 0; int mj1 = width-1;

	// indices vectoriels matrices
	int vmi0 = 0; int vmi1 = (height)-1;
	int vmj0 = 0; int vmj1 = (width/card)-1;

	DEBUG(puts("")); 
	DEBUG(printf("mi0 : %d\n", mi0)); 
	DEBUG(printf("mi1 : %d\n", mi1)); 
	DEBUG(printf("mj0 : %d\n", mj0)); 
	DEBUG(printf("mj1 : %d\n", mj1));
	DEBUG(puts("")); 

	DEBUG(puts("")); 
	DEBUG(printf("vmi0 : %d\n", vmi0)); 
	DEBUG(printf("vmi1 : %d\n", vmi1)); 
	DEBUG(printf("vmj0 : %d\n", vmj0)); 
	DEBUG(printf("vmj1 : %d\n", vmj1));
	DEBUG(puts("")); 

	// images
	vuint8** image 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// moyennes
	vuint8** mean0 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** mean1 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// ecart-types
	vuint8** std0 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** std1 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// image de différence
	vuint8 ** img_diff 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// image binaire (sortie)
	vuint8 ** img_bin 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);


	/*---------------------------------------------------*/

	DEBUG(puts("================================"));
	DEBUG(puts("=== chargement et conversion ==="));
	DEBUG(puts("================================"));

	gen_pgm_img_simd(mi0, mi1, mj0, mj1, vmi0, vmi1, vmj0, vmj1, card, image, mean0, std0);

	/*---------------------------------------------------*/

	DEBUG(puts("==================="));
	DEBUG(puts("=== traitements ==="));
	DEBUG(puts("==================="));

	// ----------------- //
	// -- traitements -- //
	// ----------------- //

	DEBUG(display_vui8matrix(mean0, vmi0, vmi1, vmj0, vmj1, format, "\nmean0 :\n"));
	DEBUG(display_vui8matrix(image, vmi0, vmi1, vmj0, vmj1, format, "\nimage :\n"));

	SigmaDelta_step1_simd_opti(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image);

	vuint8** mean1_correct 	= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	mean1_correct[0][0]    	= init_vuint8_all(0, 10, 21, 32, 43, 54, 65, 76, 87, 98 , 109, 120, 131, 142, 153, 164); 
	mean1_correct[1][0]    	= init_vuint8_all(2, 12, 22, 33, 44, 55, 66, 77, 88, 99 , 110, 121, 132, 143, 154, 165); 
    mean1_correct[2][0]    	= init_vuint8_all(3, 14, 24, 34, 45, 56, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166);
    mean1_correct[3][0]    	= init_vuint8_all(4, 15, 26, 36, 46, 57, 68, 79, 90, 101, 112, 123, 134, 145, 156, 167);

    for (int i = vmi0; i <= vmi1; ++i)
    {
    	for (int j = vmj0; j <= vmj1; ++j)
    	{
    		for (int k = 0; k < card; ++k)
    		{
    			assert(mean1[i][j][k] == mean1_correct[i][j][k]);
    		}
    	}
    }
    printf("\nStep 1 : \033[32mOK\033[00m\n");

	DEBUG(display_vui8matrix(mean1, vmi0, vmi1, vmj0, vmj1, format, "\nmean1 :\n"));

	SigmaDelta_step2_simd_opti(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff);

	vuint8** img_diff_correct 	= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	img_diff_correct[0][0]    	= init_vuint8_all(0 , 9 , 19, 29, 39, 49, 59, 69, 79, 89, 99, 109, 119, 129, 139, 149); 
	img_diff_correct[1][0]    	= init_vuint8_all(9 , 0 , 9 , 19, 29, 39, 49, 59, 69, 79, 89, 99 , 109, 119, 129, 139); 
    img_diff_correct[2][0]    	= init_vuint8_all(19, 9 , 0 , 9 , 19, 29, 39, 49, 59, 69, 79, 89 , 99 , 109, 119, 129);
    img_diff_correct[3][0]    	= init_vuint8_all(29, 19, 9 , 0 , 9 , 19, 29, 39, 49, 59, 69, 79 , 89 , 99 , 109, 119);

    for (int i = vmi0; i <= vmi1; ++i)
    {
    	for (int j = vmj0; j <= vmj1; ++j)
    	{
    		for (int k = 0; k < card; ++k)
    		{
    			assert(img_diff[i][j][k] == img_diff_correct[i][j][k]);
    		}
    	}
    }
    printf("\nStep 2 : \033[32mOK\033[00m\n");

    DEBUG(display_vui8matrix(img_diff, vmi0, vmi1, vmj0, vmj1, format, "\nimg_diff :\n"));

	SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff);

	vuint8** std1_correct	= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	std1_correct[0][0]    	= init_vuint8_all(1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2); 
	std1_correct[1][0]    	= init_vuint8_all(2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2); 
    std1_correct[2][0]    	= init_vuint8_all(2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
    std1_correct[3][0]    	= init_vuint8_all(2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);

    for (int i = vmi0; i <= vmi1; ++i)
    {
    	for (int j = vmj0; j <= vmj1; ++j)
    	{
    		for (int k = 0; k < card; ++k)
    		{
    			assert(std1[i][j][k] == std1_correct[i][j][k]);
    		}
    	}
    }
    printf("\nStep 3 : \033[32mOK\033[00m\n");

    DEBUG(display_vui8matrix(std1, vmi0, vmi1, vmj0, vmj1, format, "\nstd1 :\n"));

	// SigmaDelta_step4_simd_opti(vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin);
	// SigmaDelta_simd_full(vmi0, vmi1, vmj0, vmj1, image, mean0, std0, img_bin);
	// SigmaDelta_simd_full_opti(vmi0, vmi1, vmj0, vmj1, image, mean0, std0, img_bin);

	// vuint8** img_bin_correct		= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	// img_bin_correct[0][0]    		= init_vuint8_all(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1); 
	// img_bin_correct[1][0]    		= init_vuint8_all(1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1); 
 //    img_bin_correct[2][0]    		= init_vuint8_all(1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
 //    img_bin_correct[3][0]    		= init_vuint8_all(1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

 //    for (int i = vmi0; i <= vmi1; ++i)
 //    {
 //    	for (int j = vmj0; j <= vmj1; ++j)
 //    	{
 //    		for (int k = 0; k < card; ++k)
 //    		{
 //    			assert(img_bin[i][j][k] == img_bin_correct[i][j][k]);
 //    		}
 //    	}
 //    }
 //    printf("\nStep 4 : \033[32mOK\033[00m\n");
	
 //    DEBUG(display_vui8matrix(img_bin, vmi0, vmi1, vmj0, vmj1, format, "\nimg_bin :\n"));

	/*---------------------------------------------------*/

	// ---------- //
	// -- free -- //
	// ---------- //

	free_vui8matrix(image, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(mean0, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(mean1, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(std0, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(std1, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(img_diff, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(img_bin, vmi0, vmi1, vmj0, vmj1);
}

void test_mouvement_SIMD_car(){

	int width, height;

	// img reel size
	width  = 320; // correspond au nb de colonne  => indice boucle j
	height = 240; // correspond au nb de ligne    => indice boucle i

	char *format = "%d ";

	puts("================================");
	puts("=== test mouvement reel SIMD ===");
	puts("================================");

	// cardinalité des registres
	int card = card_vuint8(); // 16

	// ------------------------- //
	// -- calculs des indices -- //
	// ------------------------- //

	// indices scalaires matrices
	int mi0 = 0; int mi1 = height-1;
	int mj0 = 0; int mj1 = width-1;

	// indices vectoriels matrices
	int vmi0 = 0; int vmi1 = (height)-1;
	int vmj0 = 0; int vmj1 = (width/card)-1;

	// images
	vuint8** image 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// moyennes
	vuint8** mean0 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** mean1 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// ecart-types
	vuint8** std0 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** std1 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// image de différence
	vuint8 ** img_diff 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// image binaire (sortie)
	vuint8 ** img_bin 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	/*---------------------------------------------------*/

	DEBUG(puts("================================"));
	DEBUG(puts("=== chargement et conversion ==="));
	DEBUG(puts("================================"));

	// ------------------------------ //
	// -- chargement et conversion -- //
	// ------------------------------ //

	uint8 ** img_temp = ui8matrix(mi0, mi1, mj0, mj1);

	MLoadPGM_ui8matrix("../car3/car_3037.pgm", mi0, mi1, mj0, mj1, img_temp);

	// transfert ui8matrix à vui8matrix init
	ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_temp, image);

	// initiate mean0 et std0 for first iteration
	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1; ++j)
		{
			mean0[i][j] = image[i][j];
			std0[i][j]  = init_vuint8(VMIN);
		}
	}

	MLoadPGM_ui8matrix("../car3/car_3038.pgm", mi0, mi1, mj0, mj1, img_temp);

	// transfert ui8matrix à vui8matrix real
	ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_temp, image);

	DEBUG(printf("After conversion : \n"));DEBUG(puts(""));

	/*---------------------------------------------------*/

	DEBUG(puts("==================="));
	DEBUG(puts("=== traitements ==="));
	DEBUG(puts("==================="));

	// ----------------- //
	// -- traitements -- //
	// ----------------- //

	// SIGMA DELTA
	SigmaDelta_step1_simd(vmi0, vmi1, vmj0, vmj1, mean0, mean1   , image);
	SigmaDelta_step2_simd(vmi0, vmi1, vmj0, vmj1, image, mean1   , img_diff);
	SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0 , std1    , img_diff);
	SigmaDelta_step4_simd(vmi0, vmi1, vmj0, vmj1, std1 , img_diff, img_bin);
	
	// SIGMA DELTA OPTI
	// SigmaDelta_step1_simd_opti(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image);
	// SigmaDelta_step2_simd_opti(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff);
	// SigmaDelta_step3_simd_opti(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff);
	// SigmaDelta_step4_simd_opti(vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin);

	// SigmaDelta_simd_full(vmi0, vmi1, vmj0, vmj1, image, mean0, std0, img_bin);

	// SigmaDelta_simd_full_opti(vmi0, vmi1, vmj0, vmj1, image, mean0, std0, img_bin);

	/*---------------------------------------------------*/

	DEBUG(puts("================="));
	DEBUG(puts("=== affichage ==="));
	DEBUG(puts("================="));

	// --------------- //
	// -- affichage -- //
	// --------------- //

	uint8 ** img_print = ui8matrix(mi0, mi1, mj0, mj1);

	// transfert vui8matrix à ui8matrix 
	vui8matrix_to_ui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_print, img_bin);

	// convert binary img to pgm img
	bin_to_pgm(mi0, mi1, mj0, mj1, img_print, "SD_out.pgm");

	/*---------------------------------------------------*/

	DEBUG(puts("============"));
	DEBUG(puts("=== free ==="));
	DEBUG(puts("============"));

	// ---------- //
	// -- free -- //
	// ---------- //

	free_vui8matrix(image, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(mean0, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(mean1, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(std0, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(std1, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(img_diff, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(img_bin , vmi0, vmi1, vmj0, vmj1);

	free_ui8matrix(img_print, mi0, mi1, mj0, mj1);
}

void test_mouvement_SIMD_dataset(){

	// img reel size
	int width  = 320; // correspond au nb de colonne  => indice boucle j
	int height = 240; // correspond au nb de ligne    => indice boucle i

	puts("===================================");
	puts("=== test mouvement dataset SIMD ===");
	puts("===================================");

	// cardinalité des registres
	int card = card_vuint8(); // 16

	/*---------------------------------------------------*/

	// ------------------------- //
	// -- calculs des indices -- //
	// ------------------------- //

	// indices scalaires matrices
	int mi0 = 0; int mi1 = height-1;
	int mj0 = 0; int mj1 = width-1;

	// indices vectoriels matrices
	int vmi0 = 0; int vmi1 = (height)-1;
	int vmj0 = 0; int vmj1 = (width/card)-1;

	// images
	vuint8** image 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// moyennes
	vuint8** mean0 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** mean1 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// ecart-types
	vuint8** std0 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** std1 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// image de différence
	vuint8 ** img_diff 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// image binaire (sortie)
	vuint8 ** img_bin 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);

	/*---------------------------------------------------*/

	// -------------- //
    // -- prologue -- //
    // -------------- //

	uint8 ** img_temp = ui8matrix(mi0, mi1, mj0, mj1);

	MLoadPGM_ui8matrix("../car3/car_3000.pgm", mi0, mi1, mj0, mj1, img_temp);

	// transfert ui8matrix à vui8matrix init
	ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_temp, image);

	// initiate mean0 et std0 for first iteration
	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1; ++j)
		{
			mean0[i][j] = image[i][j];
			std0[i][j]  = init_vuint8(VMIN);
		}
	}

	int count = 3000;

	for (int i = 1; i < NB_IMG; ++i)
	{
		count++;

		char filename[25] = "";

		snprintf(filename, 25, "../car3/car_%d.pgm", count);

		// --------------------------- //
    	// -- chargement de l'image -- //
    	// --------------------------- //

		MLoadPGM_ui8matrix(filename, mi0, mi1, mj0, mj1, img_temp);

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_temp, image);

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		// SIGMA DELTA
		SigmaDelta_step1_simd(vmi0, vmi1, vmj0, vmj1, mean0, mean1   , image);
		SigmaDelta_step2_simd(vmi0, vmi1, vmj0, vmj1, image, mean1   , img_diff);
		SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0 , std1    , img_diff);
		SigmaDelta_step4_simd(vmi0, vmi1, vmj0, vmj1, std1 , img_diff, img_bin);
		
		// SIGMA DELTA OPTI
		// SigmaDelta_step1_simd_opti(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image);
		// SigmaDelta_step2_simd_opti(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff);
		// SigmaDelta_step3_simd_opti(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff);
		// SigmaDelta_step4_simd_opti(vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin);

		// SigmaDelta_simd_full(vmi0, vmi1, vmj0, vmj1, image, mean0, std0, img_bin);

		// SigmaDelta_simd_full_opti(vmi0, vmi1, vmj0, vmj1, image, mean0, std0, img_bin);

		// --------------- //
		// -- affichage -- //
		// --------------- //

		uint8 ** img_print = ui8matrix(mi0, mi1, mj0, mj1);

		// transfert vui8matrix à ui8matrix 
		vui8matrix_to_ui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_print, img_bin);

		// built pgm filename out
		char filename_out[25] = "";
		snprintf(filename_out, 25, "SD_out_%d.pgm", i);

		// convert binary img to pgm img
		bin_to_pgm(mi0, mi1, mj0, mj1, img_print, filename_out);

		// free temp ui8matrix
		free_ui8matrix(img_print, mi0, mi1, mj0, mj1);

		// rotate mean and std vmatrix
		copy_vui8matrix(vmi0, vmi1, vmj0, vmj1, mean1, mean0);
		copy_vui8matrix(vmi0, vmi1, vmj0, vmj1, std1 , std0 );
	}

	/*---------------------------------------------------*/

	// ---------- //
	// -- free -- //
	// ---------- //

	free_vui8matrix(image   , vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(mean0   , vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(mean1   , vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(std0    , vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(std1    , vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(img_diff, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(img_bin , vmi0, vmi1, vmj0, vmj1);
}

void main_test_mouvement_SIMD(int argc, char *argv[]){

	// Genere les images pgm dans pgm_imgs/

	// test unitaire sur petite image generer
	// test_mouvement_SIMD_unit();

	// test unitaire sur image du set
	// test_mouvement_SIMD_car();

	// test global sur tout le set
	// test_mouvement_SIMD_dataset();	
}