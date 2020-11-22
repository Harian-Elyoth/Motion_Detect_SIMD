/* --------------------------------------------------------------- */
/* ---  Test Algorithme Sigma Delta pour le traitement d'image --- */
/* --------------------------------------------------------------- */

#include "test_mouvement.h"

void test_mouvement_unit(){

	// TAILLE MATRICE
	int height_test_unit = 2; // correspond au nb de colonne  => indice boucle j
	int width_test_unit  = 3; // correspond au nb de ligne    => indice boucle i

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char *format = "%02d ";

    puts("===============================");
    puts("=== test mouvement unitaire ===");
    puts("===============================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    // 1 for 3x3 / 2 for 5x5
    b = 1; 

    // indices matrices
	mi0 = 0; mi1 = height_test_unit - 1;
	mj0 = 0; mj1 = width_test_unit  - 1;
	
	// indices matrices avec bord
	mi0b = mi0 - b ; mi1b = mi1 + b;
	mj0b = mj0 - b ; mj1b = mj1 + b;

	DEBUG(puts("")); 
	DEBUG(printf("mi0b : %d\n", mi0b)); 
	DEBUG(printf("mi1b : %d\n", mi1b)); 
	DEBUG(printf("mj0b : %d\n", mj0b)); 
	DEBUG(printf("mj1b : %d\n", mj1b));
	DEBUG(puts("")); 

	// ---------------- //
    // -- allocation -- //
    // ---------------- //

	uint8** image 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** mean0 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** mean1 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** std0 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** std1 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** img_diff 	= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** img_bin 	= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	// -------------- //
    // -- prologue -- //
    // -------------- //

    gen_pgm_img(mi0, mi1, mj0, mj1, b, mean0, std0, image);

	// ----------------- //
    // -- traitements -- //
    // ----------------- //

    printf("\nTest unitaire Sigma Delta\n");

    DEBUG(display_ui8matrix(mean0, mi0b, mi1b, mj0b, mj1b, format, "\nmean0 : "));
    DEBUG(display_ui8matrix(image, mi0b, mi1b, mj0b, mj1b, format, "\nimage : "));

    SigmaDelta_step1_opti(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image);

    int mean1_correct[4][5] = 
    { 
    	{4, 4, 7, 6, 6}, 
    	{4, 4, 7, 6, 6}, 
    	{5, 5, 8, 7, 7}, 
    	{5, 5, 8, 7, 7} 
    };

    for (int i = mi0b; i <= mi1b; ++i)
    {
    	for (int j = mj0b; j <= mj1b; ++j)
    	{
    		assert(mean1[i][j] == mean1_correct[i+1][j+1]);
    	}
    }
    printf("\nStep 1 : \033[32mOK\033[00m\n");

    DEBUG(display_ui8matrix(mean1, mi0b, mi1b, mj0b, mj1b, format, "\nmean1 : "));

	SigmaDelta_step2_opti(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff);

	int img_diff_correct[4][5] = 
    { 
    	{1, 1, 1, 1, 1}, 
    	{1, 1, 1, 1, 1}, 
    	{1, 1, 1, 1, 1}, 
    	{1, 1, 1, 1, 1},
    };

    for (int i = mi0b; i <= mi1b; ++i)
    {
    	for (int j = mj0b; j <= mj1b; ++j)
    	{
    		assert(img_diff[i][j] == img_diff_correct[i+1][j+1]);
    	}
    }
    printf("\nStep 2 : \033[32mOK\033[00m\n");

    DEBUG(display_ui8matrix(img_diff, mi0b, mi1b, mj0b, mj1b, format, "\nimg_diff : "));

    DEBUG(display_ui8matrix(std0, mi0b, mi1b, mj0b, mj1b, format, "\nstd0 : "));

    SigmaDelta_step3_opti(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff);

	int std1_correct[4][5] = 
    { 
    	{2, 2, 2, 2, 2}, 
    	{2, 2, 2, 2, 2}, 
    	{2, 2, 2, 2, 2}, 
    	{2, 2, 2, 2, 2},
    };

    for (int i = mi0b; i <= mi1b; ++i)
    {
    	for (int j = mj0b; j <= mj1b; ++j)
    	{
    		assert(std1[i][j] == std1_correct[i+1][j+1]);
    	}
    }
    printf("\nStep 3 : \033[32mOK\033[00m\n");

    DEBUG(display_ui8matrix(std1, mi0b, mi1b, mj0b, mj1b, format, "\nstd1 : "));

    SigmaDelta_step4_opti(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin);

    int img_bin_correct[4][5] = 
    { 
    	{0, 0, 0, 0, 0},
    	{0, 0, 0, 0, 0},
    	{0, 0, 0, 0, 0},
    	{0, 0, 0, 0, 0},
    };

    for (int i = mi0b; i <= mi1b; ++i)
    {
    	for (int j = mj0b; j <= mj1b; ++j)
    	{
    		assert(img_bin[i][j] == img_bin_correct[i+1][j+1]);
    	}
    }
    printf("\nStep 4 : \033[32mOK\033[00m\n");

    DEBUG(display_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b, format, "\nimg_bin : "));

    // TEST FULL VERSION 

	// SigmaDelta_full(mi0b, mi1b, mj0b, mj1b, image, mean0, mean1, img_diff, std0, std1, img_bin);

	// SigmaDelta_full_opti(mi0b, mi1b, mj0b, mj1b, image, mean0, std0, img_bin);

	// DEBUG(display_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b, " %d ", "img_bin"));

	// ---------- //
    // -- free -- //
    // ---------- //

	free_ui8matrix(image, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(mean0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(mean1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(std0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(std1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(img_diff, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b);
}

void test_mouvement_car(){

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char *format = "%d ";

    puts("===========================");
    puts("=== test mouvement reel ===");
    puts("===========================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    // 1 for 3x3 / 2 for 5x5
    b = 1; 

	// indices matrices
	mi0 = 0; mi1 = HEIGHT - 1;
	mj0 = 0; mj1 = WIDTH  - 1;
	
	// indices matrices avec bord
	mi0b = mi0-b; mi1b = mi1+b;
	mj0b = mj0-b; mj1b = mj1+b;

	DEBUG(puts("")); 
	DEBUG(printf("mi0b : %d\n", mi0b)); 
	DEBUG(printf("mi1b : %d\n", mi1b)); 
	DEBUG(printf("mj0b : %d\n", mj0b)); 
	DEBUG(printf("mj1b : %d\n", mj1b));
	DEBUG(puts("")); 

	// ---------------- //
    // -- allocation -- //
    // ---------------- //

	uint8** image 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** mean0 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** mean1 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** std0 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** std1 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** img_diff 	= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** img_bin 	= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	// -------------- //
    // -- prologue -- //
    // -------------- //

	MLoadPGM_ui8matrix("../car3/car_3037.pgm", mi0b, mi1b, mj0b, mj1b, image);

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

	MLoadPGM_ui8matrix("../car3/car_3038.pgm", mi0b, mi1b, mj0b, mj1b, image);

	duplicate_border(mi0, mi1, mj0, mj1, b, image);

	// ----------------- //
    // -- traitements -- //
    // ----------------- //


    // SigmaDelta_step1_opti(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image);
	// SigmaDelta_step2_opti(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff);
	// SigmaDelta_step3_opti(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff);
	// SigmaDelta_step4_opti(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin);

	// SigmaDelta_full(mi0b, mi1b, mj0b, mj1b, image, mean0, mean1, img_diff, std0, std1, img_bin);

	SigmaDelta_full_opti(mi0b, mi1b, mj0b, mj1b, image, mean0, std0, img_bin);

	// convert binary img to pgm img
	bin_to_pgm(mi0, mi1, mj0, mj1, img_bin,"SD_out.pgm");

	// ---------- //
    // -- free -- //
    // ---------- //

	free_ui8matrix(image, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(mean0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(mean1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(std0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(std1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(img_diff, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b);
}

void test_mouvement_dataset(){

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

    char *format = "%d ";

	// taille noyau de convolution	
    int kernel_size = 3;

    puts("==============================");
	puts("=== test mouvement dataset ===");
	puts("==============================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    // 1 for 3x3 / 2 for 5x5
    b = 1; 

    // indices matrices
	mi0 = 0; mi1 = HEIGHT-1;
	mj0 = 0; mj1 = WIDTH-1;
	
	// indices matrices avec bord
	mi0b = mi0-b; mi1b = mi1+b;
	mj0b = mj0-b; mj1b = mj1+b;

	// ---------------- //
    // -- allocation -- //
    // ---------------- //

	uint8** image = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** mean0 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** mean1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** std0 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** std1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** img_diff = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** img_bin = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	// -------------- //
    // -- prologue -- //
    // -------------- //

	MLoadPGM_ui8matrix("../car3/car_3000.pgm", mi0b, mi1b, mj0b, mj1b, image);

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

	int count = 3000;

	for (int i = 1; i < NB_IMG ; ++i)
	{
		count++;

		char filename[25] = "";

		snprintf(filename, 25, "../car3/car_%d.pgm", count);

		// --------------------------- //
    	// -- chargement de l'image -- //
    	// --------------------------- //

		MLoadPGM_ui8matrix(filename, mi0b, mi1b, mj0b, mj1b, image);

		duplicate_border(mi0, mi1, mj0, mj1, b, image);

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		// SigmaDelta_step1(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image);
		// SigmaDelta_step2(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff);
		// SigmaDelta_step3(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff);
		// SigmaDelta_step4(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin);

		// SigmaDelta_full(mi0b, mi1b, mj0b, mj1b, image, mean0, mean1, img_diff, std0, std1, img_bin);

		SigmaDelta_full_opti(mi0b, mi1b, mj0b, mj1b, image, mean0, std0, img_bin);

		// built pgm filename out
		char filename_out[25] = "";
		snprintf(filename_out, 25, "SD_out_%d.pgm", i);

		// convert binary img to pgm img
		bin_to_pgm(mi0, mi1, mj0, mj1, img_bin, filename_out);
	}

	// ---------- //
    // -- free -- //
    // ---------- //

	free_ui8matrix(image, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(mean0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(mean1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(std0, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(std1, mi0b, mi1b, mj0b, mj1b);

	free_ui8matrix(img_diff, mi0b, mi1b, mj0b, mj1b);
	free_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b);
}

void main_test_mouvement(int argc, char *argv[])
{
	// Genere les images pgm dans pgm_imgs/

	// test unitaire sur petite image generer
	test_mouvement_unit();

	// test unitaire sur image du set
	// test_mouvement_car();

	// test global sur tout le set
	// test_mouvement_dataset();	
}
