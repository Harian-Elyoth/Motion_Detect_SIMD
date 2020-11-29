/*
===============================================================
==================== Test SD + Morpho =========================
===============================================================
*/

#include "test_mouvement_morpho.h"

void test_mouvement_morpho_car(bool is_visual){

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

	char *format = "%d ";

	// taille noyau de convolution	
    int kernel_size = 3;

    puts("===============================");
    puts("=== test mouvement unitaire ===");
    puts("===============================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    // 1 for 3x3 / 2 for 5x5
    b = 1; 

    if (is_visual) {
    	// indices matrices
		mi0 = 0; mi1 = HEIGHT_TEST_UNIT - 1;
		mj0 = 0; mj1 = WIDTH_TEST_UNIT  - 1;
    }
    else {
		// indices matrices
		mi0 = 0; mi1 = HEIGHT - 1;
		mj0 = 0; mj1 = WIDTH  - 1;
    }
	
	// indices matrices avec bord
	mi0b = mi0-b; mi1b = mi1+b;
	mj0b = mj0-b; mj1b = mj1+b;

	// ---------------- //
    // -- allocation -- //
    // ---------------- //

	uint8** image 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** mean0 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** mean1 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** std0 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** std1 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** img_diff 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** img_bin 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** img_filtered	= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** tmp1			= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** tmp2 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	// -------------- //
    // -- prologue -- //
    // -------------- //

    if (is_visual)
    {
    	gen_pgm_img(mi0, mi1, mj0, mj1, b, mean0, std0, image);
    }
    else
    {
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
    }

	// ----------------- //
    // -- traitements -- //
    // ----------------- //

	// SIGMA DELTA
	SigmaDelta_step1(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image);
	SigmaDelta_step2(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff);
	SigmaDelta_step3(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff);
	SigmaDelta_step4(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin);

	// MORPHOLOGIE

	morpho_3_opti(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1); 


	// convert binary img to pgm img
	bin_to_pgm(mi0, mi1, mj0, mj1, img_filtered,"SD_out.pgm");

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

void test_mouvement_morpho_dataset(){

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord
  char *format = "%d ";

  puts("=======================================");
	puts("=== test mouvement + morpho dataset ===");
	puts("=======================================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    // 1 for 3x3 / 2 for 5x5
    b = 2; 

    // indices matrices
	mi0 = 0; mi1 = HEIGHT - 1;
	mj0 = 0; mj1 = WIDTH  - 1;
	
	// indices matrices avec bord
	mi0b = mi0-b; mi1b = mi1+b;
	mj0b = mj0-b; mj1b = mj1+b;

	// ---------------- //
    // -- allocation -- //
    // ---------------- //

	uint8 ** image = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8 ** mean0 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8 ** mean1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8 ** std0 = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8 ** std1 = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8 ** img_diff = ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8 ** img_bin = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8 ** img_filtered = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** tmp1			= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** tmp2 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

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

		// SIGMA DELTA
		SigmaDelta_step1(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image);
		SigmaDelta_step2(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff);
		SigmaDelta_step3(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff);
		SigmaDelta_step4(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin);

		// MORPHOLOGIE
		morpho_5_opti(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1); 

		// built pgm filename out
		char filename_out[25] = "";
		snprintf(filename_out, 25, "MM_out_%d.pgm", i);
		bin_to_pgm(mi0, mi1, mj0, mj1, img_filtered, filename_out);
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

	free_ui8matrix(img_filtered, mi0b, mi1b, mj0b, mj1b);
}

void main_test_mouvement_morpho(int argc, char *argv[])
{
	// Genere les images pgm dans pgm_imgs/

	// test unitaire sur petite image generer
	// test_mouvement_morpho_car(true);

	// test unitaire sur image du set
	test_mouvement_morpho_car(false);

	// test global sur tout le set
	// test_mouvement_morpho_dataset();	
}