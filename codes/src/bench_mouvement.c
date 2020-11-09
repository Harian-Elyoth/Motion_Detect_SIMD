/* ---------------------------------------------------------------- */
/* ---  Bench Algorithme Sigma Delta pour le traitement d'image --- */
/* ---------------------------------------------------------------- */

#include "bench_mouvement.h"

void bench_mouvement_car(bool is_visual){

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
	double cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4;

	// calcul temps
	double time_total, time_step1, time_step2, time_step3, time_step4;
	clock_t start, finish;

	// calcul debit
	double debit_total, debit_step1, debit_step2, debit_step3, debit_step4;

	// taille noyau de convolution	
    int kernel_size = 3;

    puts("====================================");
    puts("=== benchmark mouvement unitaire ===");
    puts("====================================");

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

    BENCH(printf("Sigma Delta :\n\n"));

    start = clock();
	CHRONO(SigmaDelta_step1(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image), cycles_step1);
	finish = clock();
	time_step1 = (double)(finish-start)/CLOCKS_PER_SEC;
	debit_step1 = (WIDTH*HEIGHT) / time_step1;
	time_step1 *= 1000;

	BENCH(printf("step 1 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1)); BENCH(puts("")); BENCH(puts(""));
	
    start = clock();
	CHRONO(SigmaDelta_step2(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff), cycles_step2);
	finish = clock();
	time_step2 = (double)(finish-start)/CLOCKS_PER_SEC;
	debit_step2 = (WIDTH*HEIGHT) / time_step2;
	time_step2 *= 1000;

	BENCH(printf("step 2 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2)); BENCH(puts("")); BENCH(puts(""));

    start = clock();
	CHRONO(SigmaDelta_step3(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff), cycles_step3);
	finish = clock();
	time_step3 = (double)(finish-start)/CLOCKS_PER_SEC;
	debit_step3 = (WIDTH*HEIGHT) / time_step3;
	time_step3 *= 1000;

	BENCH(printf("step 3 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3)); BENCH(puts("")); BENCH(puts(""));

	start = clock();
	CHRONO(SigmaDelta_step4(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin), cycles_step4);
	finish = clock();
	time_step4 = (double)(finish-start)/CLOCKS_PER_SEC;
	debit_step4 = (WIDTH*HEIGHT) / time_step4;
	time_step4 *= 1000;

	BENCH(printf("step 4 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4)); BENCH(puts("")); BENCH(puts(""));

	cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
	time_total   = time_step1   + time_step2   + time_step3   + time_step4;
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

void bench_mouvement_dataset(){

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
	double cycles_dataset, cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4;

	// calcul temps
	double time_dataset, time_total, time_step1, time_step2, time_step3, time_step4;
	clock_t start, finish;

	// calcul debit
	double debit_dataset, debit_total, debit_step1, debit_step2, debit_step3, debit_step4;

	// taille noyau de convolution	
    int kernel_size = 3;

    puts("===================================");
	puts("=== benchmark mouvement dataset ===");
	puts("===================================");

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

		start = clock();
		CHRONO(SigmaDelta_step1(mi0b, mi1b, mj0b, mj1b, mean0, mean1, image), cycles_step1);
		finish = clock();
		time_step1 = (double)(finish-start)/CLOCKS_PER_SEC;
		debit_step1 = (WIDTH*HEIGHT) / time_step1;
		time_step1 *= 1000;

		start = clock();
		CHRONO(SigmaDelta_step2(mi0b, mi1b, mj0b, mj1b, image, mean1, img_diff), cycles_step2);
		finish = clock();
		time_step2 = (double)(finish-start)/CLOCKS_PER_SEC;
		debit_step2 = (WIDTH*HEIGHT) / time_step2;
		time_step2 *= 1000;
		
		start = clock();
		CHRONO(SigmaDelta_step3(mi0b, mi1b, mj0b, mj1b, std0, std1, img_diff), cycles_step3);
		finish = clock();
		time_step3 = (double)(finish-start)/CLOCKS_PER_SEC;
		debit_step3 = (WIDTH*HEIGHT) / time_step3;
		time_step3 *= 1000;

		start = clock();
		CHRONO(SigmaDelta_step4(mi0b, mi1b, mj0b, mj1b, std1, img_diff, img_bin), cycles_step4);
		finish = clock();
		time_step4 = (double)(finish-start)/CLOCKS_PER_SEC;
		debit_step4 = (WIDTH*HEIGHT) / time_step4;
		time_step4 *= 1000;

		cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
		time_total   = time_step1   + time_step2   + time_step3   + time_step4;
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

void main_bench_mouvement(int argc, char *argv[]){

	// Affiche les métriques de performance

	// benchmark unitaire sur petite images test generer
	// bench_mouvement_car(true);

	// benchmark unitaire sur image du set
	bench_mouvement_car(false);

	// benchmark global sur tout le dataset
	// bench_mouvement_dataset();
}