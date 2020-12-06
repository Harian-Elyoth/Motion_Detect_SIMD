/* ------------------------------------------------------------------------- */
/* ---  Bench Algorithme Sigma Delta + Morpho pour le traitement d'image --- */
/* ------------------------------------------------------------------------- */

#include "bench_mouvement_morpho.h"

void bench_mouvement_morpho_car(bool is_visual){

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char *format = "%d ";

	// calcul cpp
	double cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4, cycles_morpho;

	// calcul temps
	double time_total, time_step1, time_step2, time_step3, time_step4, time_morpho;

	// calcul debit
	double debit_total, debit_step1, debit_step2, debit_step3, debit_step4, debit_morpho;

	// taille noyau de convolution	
    int kernel_size = 3;

    puts("=============================================");
    puts("=== benchmark mouvement + morpho unitaire ===");
    puts("=============================================");

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
	uint8** tmp1 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** tmp2 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	// -------------- //
    // -- prologue -- //
    // -------------- //

    if (is_visual)
    {
    	gen_pgm_img(mi0, mi1, mj0, mj1, mean0, std0, image);
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

    BENCH(printf("Sigma Delta :\n\n"));

	CHRONO(SigmaDelta_step1(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image), cycles_step1);
	time_step1 = (double)(cycles_step1/CLK_PROC);
	debit_step1 = (WIDTH*HEIGHT) / time_step1;
	time_step1 *= 1000;

	BENCH(printf("step 1 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1)); BENCH(puts("")); BENCH(puts(""));
	
	CHRONO(SigmaDelta_step2(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff), cycles_step2);
	time_step2 = (double)(cycles_step2/CLK_PROC);
	debit_step2 = (WIDTH*HEIGHT) / time_step2;
	time_step2 *= 1000;

	BENCH(printf("step 2 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2)); BENCH(puts("")); BENCH(puts(""));

	CHRONO(SigmaDelta_step3(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff), cycles_step3);
	time_step3 = (double)(cycles_step3/CLK_PROC);
	debit_step3 = (WIDTH*HEIGHT) / time_step3;
	time_step3 *= 1000;

	BENCH(printf("step 3 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3)); BENCH(puts("")); BENCH(puts(""));

	CHRONO(SigmaDelta_step4(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin), cycles_step4);
	time_step4 = (double)(cycles_step4/CLK_PROC);
	debit_step4 = (WIDTH*HEIGHT) / time_step4;
	time_step4 *= 1000;

	BENCH(printf("step 4 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4)); BENCH(puts("")); BENCH(puts(""));


	CHRONO(morpho_3(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho); 
	time_morpho = (double)(cycles_morpho/CLK_PROC);
	debit_morpho = (WIDTH*HEIGHT) / time_morpho;
	time_morpho *= 1000;

	BENCH(printf("Morphologie :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));

	cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4 + cycles_morpho;
	time_total   = time_step1   + time_step2   + time_step3   + time_step4 + time_morpho;
	debit_total  = (WIDTH*HEIGHT) / time_total;

	BENCH(printf("Total :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total)); BENCH(puts("")); BENCH(puts(""));

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

void bench_mouvement_morpho_dataset(){

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

    char *format = "%d ";

    // calcul cpp
	double cycles_dataset, cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4, cycles_morpho;

	// calcul temps
	double time_dataset, time_total, time_step1, time_step2, time_step3, time_step4, time_morpho;

	// calcul debit
	double debit_dataset, debit_total, debit_step1, debit_step2, debit_step3, debit_step4, debit_morpho;

	// taille noyau de convolution	
    int kernel_size = 3;

    puts("============================================");
	puts("=== benchmark mouvement + morpho dataset ===");
	puts("============================================");

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

	uint8** image 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** mean0 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** mean1 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** std0 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** std1 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** img_diff 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** img_bin 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	uint8** img_filtered 	= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	uint8** tmp1 			= ui8matrix(mi0b, mi1b, mj0b, mj1b);
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
		CHRONO(SigmaDelta_step1(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image), cycles_step1);
		time_step1 = (double)(cycles_step1/CLK_PROC);
		debit_step1 = (WIDTH*HEIGHT) / time_step1;
		time_step1 *= 1000;

		CHRONO(SigmaDelta_step2(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff), cycles_step2);
		time_step2 = (double)(cycles_step2/CLK_PROC);
		debit_step2 = (WIDTH*HEIGHT) / time_step2;
		time_step2 *= 1000;
		
		CHRONO(SigmaDelta_step3(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff), cycles_step3);
		time_step3 = (double)(cycles_step3/CLK_PROC);
		debit_step3 = (WIDTH*HEIGHT) / time_step3;
		time_step3 *= 1000;

		CHRONO(SigmaDelta_step4(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin), cycles_step4);
		time_step4 = (double)(cycles_step4/CLK_PROC);
		debit_step4 = (WIDTH*HEIGHT) / time_step4;
		time_step4 *= 1000;

		// MORPHOLOGIE
		CHRONO(morpho_3(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho); 
		time_morpho = (double)(cycles_morpho/CLK_PROC);
		debit_morpho = (WIDTH*HEIGHT) / time_morpho;
		time_morpho *= 1000;

		// TOTAL
		cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4 + cycles_morpho;
		time_total   = time_step1   + time_step2   + time_step3   + time_step4 + time_morpho;
		debit_total  = (WIDTH*HEIGHT) / time_total;

		cycles_dataset += cycles_total/(WIDTH*HEIGHT);
		time_dataset += time_total;
		// DEBIT DATASET PAS TROP DE SENS ICI ?

	}

	BENCH(printf("\nTotal dataset :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_dataset)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dataset)); BENCH(puts("")); BENCH(puts(""));

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

void bench_mouvement_morpho_graphic(){

	// init fichier csv
	FILE* fichier_csv = fopen("csv_files/perf_mouvement_morpho.csv","w");
	fprintf(fichier_csv, ";%s;;;;%s\n", "Traitement", "Traitement OPTI");
	fprintf(fichier_csv, "%s;%s;%s;%s;", "Taille (pixels)", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
	fprintf(fichier_csv, "%s;%s;%s\n", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

    char *format = "%d ";

    // calcul cpp
	double cycles_total, cycles_total_full_opti, cycles_step1, cycles_step2, cycles_step3, cycles_step4, cycles_morpho_3, cycles_morpho_3_opti;
	// calcul temps
	double time_total, time_total_full_opti, time_step1, time_step2, time_step3, time_step4, time_morpho_3, time_morpho_3_opti;

	// calcul debit
	double debit_total, debit_total_full_opti, debit_morpho_3, debit_morpho_3_opti;

    puts("=========================================");
	puts("=== benchmark mouvement SIMD graphics ===");
	puts("=========================================");

    // 1 for 3x3 / 2 for 5x5
    b = 1; 
    
    // Dimension initial des matrices générés
    int height = 32;
    int width  = 32;

    for (int i = 0; i < 300; ++i)
    {

    	// commence a 8 x 8
    	height += 16;
    	width  += 16;

	    // ------------------------- //
	    // -- calculs des indices -- //
	    // ------------------------- //

    	// indices matrices
		mi0 = 0; mi1 = height-1;
		mj0 = 0; mj1 = width-1;

    	// indices matrices avec bord
		mi0b = mi0-b; mi1b = mi1+b;
		mj0b = mj0-b; mj1b = mj1+b;

		// DEBUG(printf("mi0b = %d, mi1b = %d, mj0b = %d, mj1b = %d\n", mi0b, mi1b, mj0b, mj1b));
		// DEBUG(printf("vmi0b = %d, vmi1b = %d, vmj0b = %d, vmj1b = %d\n", vmi0b, vmi1b, vmj0b, vmj1b));

		// ---------------- //
	    // -- allocation -- //
	    // ---------------- //

	    uint8** image_init 		= ui8matrix(mi0, mi1, mj0, mj1);

	   	// images
		uint8** image 			= ui8matrix(mi0, mi1, mj0, mj1);

		// moyennes
		uint8** mean0 			= ui8matrix(mi0, mi1, mj0, mj1);
		uint8** mean1 			= ui8matrix(mi0, mi1, mj0, mj1);

		// ecart-types
		uint8** std0 			= ui8matrix(mi0, mi1, mj0, mj1);
		uint8** std1 			= ui8matrix(mi0, mi1, mj0, mj1);

		// image de différence
		uint8 ** img_diff 		= ui8matrix(mi0, mi1, mj0, mj1);

		// image binaire (sortie)
		uint8 ** img_bin 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);
        uint8 ** img_filtered  	= ui8matrix(mi0b, mi1b, mj0b, mj1b);

        uint8 ** tmp1       	= ui8matrix(mi0b, mi1b, mj0b, mj1b);
        uint8 ** tmp2     		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	   	for (int i = mi0; i <= mi1; ++i)
	   	{
	   		for (int j = mj0; j <= mj1; ++j)
	   		{
	   			// Générer des valeurs de pixels avec un motif qui evoluent avec la taille
	   			// Génére 2 images censé etre tres similaire (seul quelques pixels doivent varier)
	   			int val1 = (i + j)*10 + j;
	   			int val2 = (i + j)*10 + j + i;
	   			if(val1 > 255)	val1 = val1 / 255;
	   			if(val2 > 255)	val2 = val2 / 255;

	   			image_init [i][j]   = val1;
	   			image[i][j]  		= val2; 
	   		}
	   	}

	   	// initiate mean0 et std0 for first iteration
		for (int i = mi0; i <= mi1; ++i)
		{
			for (int j = mj0; j <= mj1; ++j)
			{
				mean0[i][j] = image[i][j];
				std0[i][j]  = VMIN;
			}
		}

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		CHRONO(SigmaDelta_step1(mi0, mi1, mj0, mj1, mean0, mean1, image), cycles_step1);
		time_step1 = (double)(cycles_step1/CLK_PROC);

		CHRONO(SigmaDelta_step2(mi0, mi1, mj0, mj1, image, mean1, img_diff), cycles_step2);
		time_step2 = (double)(cycles_step2/CLK_PROC);
		
		CHRONO(SigmaDelta_step3(mi0, mi1, mj0, mj1, std0, std1, img_diff), cycles_step3);
		time_step3 = (double)(cycles_step3/CLK_PROC);

		CHRONO(SigmaDelta_step4(mi0, mi1, mj0, mj1, std1, img_diff, img_bin), cycles_step4);
		time_step4 = (double)(cycles_step4/CLK_PROC);

       	duplicate_border(mi0, mi1, mj0, mj1, b, img_bin);
		
	    CHRONO(morpho_3(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho_3);
        time_morpho_3 = (double)(cycles_morpho_3/CLK_PROC);

		cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4 + cycles_morpho_3;
		time_total   = time_step1   + time_step2   + time_step3   + time_step4 + time_morpho_3;
		debit_total  = (width*height) / time_total;

		fprintf(fichier_csv, "%d;", height);
		fprintf(fichier_csv, "%f;", time_total*1000);
		fprintf(fichier_csv, "%f;", cycles_total/(height*width));
		fprintf(fichier_csv, "%f;", debit_total);

		// FULL OPTI
		CHRONO(SigmaDelta_full_opti(mi0, mi1, mj0, mj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_full_opti);
		time_total_full_opti = (double)(cycles_total_full_opti/CLK_PROC);

		duplicate_border(mi0, mi1, mj0, mj1, b, img_bin);

		CHRONO(morpho_3_opti(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho_3_opti);
        time_morpho_3_opti = (double)(cycles_morpho_3_opti/CLK_PROC);

		cycles_total = cycles_morpho_3_opti + cycles_total_full_opti;
		time_total   = time_morpho_3 + time_total_full_opti;
		debit_total  = (width*height) / time_total;

		fprintf(fichier_csv, ";%f;", time_total*1000);
		fprintf(fichier_csv, "%f;", cycles_total/(height*width));
		fprintf(fichier_csv, "%f\n", debit_total);

		// ---------- //
		// -- free -- //
		// ---------- //

		free_ui8matrix(image_init, mi0, mi1, mj0, mj1);
		
		free_ui8matrix(image, mi0, mi1, mj0, mj1);
		
		free_ui8matrix(mean0, mi0, mi1, mj0, mj1);
		free_ui8matrix(mean1, mi0, mi1, mj0, mj1);
		
		free_ui8matrix(std0, mi0, mi1, mj0, mj1);
		free_ui8matrix(std1, mi0, mi1, mj0, mj1);
		
		free_ui8matrix(img_diff, mi0, mi1, mj0, mj1);

		free_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b);
		free_ui8matrix(img_filtered, mi0b, mi1b, mj0b, mj1b);

		free_ui8matrix(tmp1, mi0b, mi1b, mj0b, mj1b);
		free_ui8matrix(tmp2, mi0b, mi1b, mj0b, mj1b);

    }

    fclose(fichier_csv);
}

void main_bench_mouvement_morpho(int argc, char *argv[]){

	// Affiche les métriques de performance

	// benchmark unitaire sur petite images test generer
	// bench_mouvement_morpho_car(true);

	// benchmark unitaire sur image du set
	bench_mouvement_morpho_car(false);

	// benchmark global sur tout le dataset
	// bench_mouvement_morpho_dataset();
}