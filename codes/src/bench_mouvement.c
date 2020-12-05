/* ---------------------------------------------------------------- */
/* ---  Bench Algorithme Sigma Delta pour le traitement d'image --- */
/* ---------------------------------------------------------------- */

#include "bench_mouvement.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void bench_mouvement_car(){

	int mi0, mi1, mj0, mj1; 	// indices scalaire

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char *format = "%d ";

	// calcul cpp
	double cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4;
	double cycles_total_opti, cycles_step1_opti, cycles_step2_opti, cycles_step3_opti, cycles_step4_opti;
	double cycles_total_full, cycles_total_full_opti;

	// calcul temps
	double time_total, time_step1, time_step2, time_step3, time_step4;
	double time_total_opti, time_step1_opti, time_step2_opti, time_step3_opti, time_step4_opti;
	double time_total_full, time_total_full_opti;

	// calcul debit
	double debit_total, debit_step1, debit_step2, debit_step3, debit_step4;
	double debit_total_opti, debit_step1_opti, debit_step2_opti, debit_step3_opti, debit_step4_opti;
	double debit_total_full, debit_total_full_opti;

    puts("====================================");
    puts("=== benchmark mouvement unitaire ===");
    puts("====================================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

	// indices matrices
	mi0 = 0; mi1 = HEIGHT - 1;
	mj0 = 0; mj1 = WIDTH  - 1;

	// ---------------- //
    // -- allocation -- //
    // ---------------- //

	uint8** image 		= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** mean0 		= ui8matrix(mi0, mi1, mj0, mj1);
	uint8** mean1 		= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** std0 		= ui8matrix(mi0, mi1, mj0, mj1);
	uint8** std1 		= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** img_diff 	= ui8matrix(mi0, mi1, mj0, mj1);
	uint8** img_bin 	= ui8matrix(mi0, mi1, mj0, mj1);

	// -------------- //
    // -- prologue -- //
    // -------------- //

	MLoadPGM_ui8matrix("../car3/car_3037.pgm", mi0, mi1, mj0, mj1, image);

	// initiate mean0 et std0 for first iteration
	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
		{
			mean0[i][j] = image[i][j];
			std0[i][j]  = VMIN;
		}
	}

	MLoadPGM_ui8matrix("../car3/car_3038.pgm", mi0, mi1, mj0, mj1, image);

	// ----------------- //
    // -- traitements -- //
    // ----------------- //

    BENCH(printf("Sigma Delta :\n\n"));

    /*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

    // STEP 1
	CHRONO(SigmaDelta_step1(mi0, mi1, mj0, mj1, mean0, mean1, image), cycles_step1);
	time_step1 = (double)(cycles_step1/CLK_PROC);
	debit_step1 = (WIDTH*HEIGHT) / time_step1;
	time_step1 *= 1000;

	BENCH(printf("step 1 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1)); BENCH(puts("")); BENCH(puts(""));

	// STEP 1 OPTI
	CHRONO(SigmaDelta_step1_opti(mi0, mi1, mj0, mj1, mean0, mean1, image), cycles_step1_opti);
	time_step1_opti = (double)(cycles_step1_opti/CLK_PROC);
	debit_step1_opti = (WIDTH*HEIGHT) / time_step1_opti;
	time_step1_opti *= 1000;

	BENCH(printf("step 1 optimisée :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1_opti)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1_opti/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/
	
	// STEP 2
	CHRONO(SigmaDelta_step2(mi0, mi1, mj0, mj1, image, mean1, img_diff), cycles_step2);
	time_step2 = (double)(cycles_step2/CLK_PROC);
	debit_step2 = (WIDTH*HEIGHT) / time_step2;
	time_step2 *= 1000;

	BENCH(printf("step 2 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2)); BENCH(puts("")); BENCH(puts(""));

	// STEP 2 OPTI
	CHRONO(SigmaDelta_step2_opti(mi0, mi1, mj0, mj1, mean0, mean1, image), cycles_step2_opti);
	time_step2_opti = (double)(cycles_step2_opti/CLK_PROC);
	debit_step2_opti = (WIDTH*HEIGHT) / time_step2_opti;
	time_step2_opti *= 1000;

	BENCH(printf("step 2 optimisée :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2_opti)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2_opti/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 3
	CHRONO(SigmaDelta_step3(mi0, mi1, mj0, mj1, std0, std1, img_diff), cycles_step3);
	time_step3 = (double)(cycles_step3/CLK_PROC);
	debit_step3 = (WIDTH*HEIGHT) / time_step3;
	time_step3 *= 1000;

	BENCH(printf("step 3 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3)); BENCH(puts("")); BENCH(puts(""));

	// STEP 3 OPTI
	CHRONO(SigmaDelta_step3_opti(mi0, mi1, mj0, mj1, mean0, mean1, image), cycles_step3_opti);
	time_step3_opti = (double)(cycles_step3_opti/CLK_PROC);
	debit_step3_opti = (WIDTH*HEIGHT) / time_step3_opti;
	time_step3_opti *= 1000;

	BENCH(printf("step 3 optimisée :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3_opti)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3_opti/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 4
	CHRONO(SigmaDelta_step4(mi0, mi1, mj0, mj1, std1, img_diff, img_bin), cycles_step4);
	time_step4 = (double)(cycles_step4/CLK_PROC);
	debit_step4 = (WIDTH*HEIGHT) / time_step4;
	time_step4 *= 1000;

	BENCH(printf("step 4 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4)); BENCH(puts("")); BENCH(puts(""));

	// STEP 4 OPTI
	CHRONO(SigmaDelta_step4_opti(mi0, mi1, mj0, mj1, std1, img_diff, img_bin), cycles_step4_opti);
	time_step4_opti = (double)(cycles_step4_opti/CLK_PROC);
	debit_step4_opti = (WIDTH*HEIGHT) / time_step4_opti;
	time_step4_opti *= 1000;

	BENCH(printf("step 4 optimisée :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4_opti)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4_opti/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// TOTAL
	cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
	time_total   = time_step1   + time_step2   + time_step3   + time_step4;
	debit_total  = ((WIDTH*HEIGHT) / time_total)*1000;

	BENCH(printf("Total :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total)); BENCH(puts("")); BENCH(puts(""));

	// TOTAL OPTI
	cycles_total_opti = cycles_step1_opti + cycles_step2_opti + cycles_step3_opti + cycles_step4_opti;
	time_total_opti   = time_step1_opti   + time_step2_opti   + time_step3_opti   + time_step4_opti;
	debit_total_opti  = ((WIDTH*HEIGHT) / time_total_opti)*1000;

	BENCH(printf("Total optimisé :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_opti)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_opti/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// FULL
	CHRONO(SigmaDelta_full(mi0, mi1, mj0, mj1, image, mean0, mean1, img_diff, std0, std1, img_bin), cycles_total_full);
	time_total_full = (double)(cycles_total_full/CLK_PROC);
	debit_total_full = (WIDTH*HEIGHT) / time_total_full;
	time_total_full *= 1000;

	BENCH(printf("Total full :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_full)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_full/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_full)); BENCH(puts("")); BENCH(puts(""));	

	// FULL OPTI
	CHRONO(SigmaDelta_full_opti(mi0, mi1, mj0, mj1, image, mean0, std0, img_bin), cycles_total_full_opti);
	time_total_full_opti = (double)(cycles_total_full_opti/CLK_PROC);
	debit_total_full_opti = (WIDTH*HEIGHT) / time_total_full_opti;
	time_total_full_opti *= 1000;

	BENCH(printf("Total full optimisé :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_full_opti)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_full_opti/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_full_opti)); BENCH(puts("")); BENCH(puts(""));	

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// ---------- //
    // -- free -- //
    // ---------- //

	free_ui8matrix(image, mi0, mi1, mj0, mj1);

	free_ui8matrix(mean0, mi0, mi1, mj0, mj1);
	free_ui8matrix(mean1, mi0, mi1, mj0, mj1);

	free_ui8matrix(std0, mi0, mi1, mj0, mj1);
	free_ui8matrix(std1, mi0, mi1, mj0, mj1);

	free_ui8matrix(img_diff, mi0, mi1, mj0, mj1);
	free_ui8matrix(img_bin , mi0, mi1, mj0, mj1);
}

void bench_mouvement_dataset(){

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	
	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

    char *format = "%d ";

	// calcul cpp
	// double cycles_step1 = 0, cycles_step1_mean = 0, cycles_step1_opti = 0, cycles_step1_opti_mean = 0;
	// double cycles_step2 = 0, cycles_step2_mean = 0, cycles_step2_opti = 0, cycles_step2_opti_mean = 0;
	// double cycles_step3 = 0, cycles_step3_mean = 0, cycles_step3_opti = 0, cycles_step3_opti_mean = 0;
	// double cycles_step4 = 0, cycles_step4_mean = 0, cycles_step4_opti = 0, cycles_step4_opti_mean = 0;
 //   	double cycles_total = 0, cycles_total_mean = 0, cycles_total_opti = 0, cycles_total_opti_mean = 0; 
   	
   	double cycles_total_full = 0, cycles_total_full_mean = 0, cycles_total_full_opti = 0, cycles_total_full_opti_mean = 0;

	// calcul temps
 //   	double time_step1 = 0, time_step1_mean = 0, time_step1_opti = 0, time_step1_opti_mean = 0;
 //   	double time_step2 = 0, time_step2_mean = 0, time_step2_opti = 0, time_step2_opti_mean = 0;
 //   	double time_step3 = 0, time_step3_mean = 0, time_step3_opti = 0, time_step3_opti_mean = 0;
 //   	double time_step4 = 0, time_step4_mean = 0, time_step4_opti = 0, time_step4_opti_mean = 0;
	// double time_total = 0, time_total_mean = 0, time_total_opti = 0, time_total_opti_mean = 0;

	double time_total_full = 0, time_total_full_mean = 0, time_total_full_opti = 0, time_total_full_opti_mean = 0;

	// calcul debit
	// double debit_step1 = 0, debit_step1_mean = 0, debit_step1_opti = 0, debit_step1_opti_mean = 0;
	// double debit_step2 = 0, debit_step2_mean = 0, debit_step2_opti = 0, debit_step2_opti_mean = 0;
	// double debit_step3 = 0, debit_step3_mean = 0, debit_step3_opti = 0, debit_step3_opti_mean = 0;
	// double debit_step4 = 0, debit_step4_mean = 0, debit_step4_opti = 0, debit_step4_opti_mean = 0;
	// double debit_total = 0, debit_total_mean = 0, debit_total_opti = 0, debit_total_opti_mean = 0;

	double debit_total_full = 0, debit_total_full_mean = 0, debit_total_full_opti = 0, debit_total_full_opti_mean = 0;

    puts("===================================");
	puts("=== benchmark mouvement dataset ===");
	puts("===================================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    // indices matrices
	mi0 = 0; mi1 = HEIGHT-1;
	mj0 = 0; mj1 = WIDTH-1;
	
	// ---------------- //
    // -- allocation -- //
    // ---------------- //

	uint8** image 		= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** mean0 		= ui8matrix(mi0, mi1, mj0, mj1);
	uint8** mean1 		= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** std0 		= ui8matrix(mi0, mi1, mj0, mj1);
	uint8** std1 		= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** img_diff 	= ui8matrix(mi0, mi1, mj0, mj1);
	uint8** img_bin 	= ui8matrix(mi0, mi1, mj0, mj1);

	// -------------- //
    // -- prologue -- //
    // -------------- //

	MLoadPGM_ui8matrix("../car3/car_3000.pgm", mi0, mi1, mj0, mj1, image);

	// initiate mean0 et std0 for first iteration
	for (int i = mi0; i <= mi1; ++i)
	{
		for (int j = mj0; j <= mj1; ++j)
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

		MLoadPGM_ui8matrix(filename, mi0, mi1, mj0, mj1, image);

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		// STEP 1
		// CHRONO(SigmaDelta_step1(mi0, mi1, mj0, mj1, mean0, mean1, image), cycles_step1);
		// time_step1  = (double)(cycles_step1/CLK_PROC);
		// debit_step1 = (WIDTH*HEIGHT) / time_step1;
		// time_step1  *= 1000;

		// // STEP 1 OPTI
		// CHRONO(SigmaDelta_step1_opti(mi0, mi1, mj0, mj1, mean0, mean1, image), cycles_step1_opti);
		// time_step1_opti  = (double)(cycles_step1_opti/CLK_PROC);
		// debit_step1_opti = (WIDTH*HEIGHT) / time_step1_opti;
		// time_step1_opti  *= 1000;

		// /*----------------------------------------------------------------------------------------------*/
		// /*----------------------------------------------------------------------------------------------*/

		// // STEP 2
		// CHRONO(SigmaDelta_step2(mi0, mi1, mj0, mj1, image, mean1, img_diff), cycles_step2);
		// time_step2  = (double)(cycles_step2/CLK_PROC);
		// debit_step2 = (WIDTH*HEIGHT) / time_step2;
		// time_step2  *= 1000;
		
		// // STEP 2 OPTI
		// CHRONO(SigmaDelta_step2_opti(mi0, mi1, mj0, mj1, image, mean1, img_diff), cycles_step2_opti);
		// time_step2_opti  = (double)(cycles_step2_opti/CLK_PROC);
		// debit_step2_opti = (WIDTH*HEIGHT) / time_step2_opti;
		// time_step2_opti  *= 1000;

		// /*----------------------------------------------------------------------------------------------*/
		// ----------------------------------------------------------------------------------------------

		// // STEP 3
		// CHRONO(SigmaDelta_step3(mi0, mi1, mj0, mj1, std0, std1, img_diff), cycles_step3);
		// time_step3  = (double)(cycles_step3/CLK_PROC);
		// debit_step3 = (WIDTH*HEIGHT) / time_step3;
		// time_step3  *= 1000;

		// // STEP 3 OPTI
		// CHRONO(SigmaDelta_step3_opti(mi0, mi1, mj0, mj1, std0, std1, img_diff), cycles_step3_opti);
		// time_step3_opti  = (double)(cycles_step3_opti/CLK_PROC);
		// debit_step3_opti = (WIDTH*HEIGHT) / time_step3_opti;
		// time_step3_opti  *= 1000;

		// /*----------------------------------------------------------------------------------------------*/
		// /*----------------------------------------------------------------------------------------------*/

		// // STEP 4
		// CHRONO(SigmaDelta_step4(mi0, mi1, mj0, mj1, std1, img_diff, img_bin), cycles_step4);
		// time_step4  = (double)(cycles_step4/CLK_PROC);
		// debit_step4 = (WIDTH*HEIGHT) / time_step4;
		// time_step4  *= 1000;

		// // STEP 4 OPTI
		// CHRONO(SigmaDelta_step4_opti(mi0, mi1, mj0, mj1, std1, img_diff, img_bin), cycles_step4_opti);
		// time_step4_opti  = (double)(cycles_step4_opti/CLK_PROC);
		// debit_step4_opti = (WIDTH*HEIGHT) / time_step4_opti;
		// time_step4_opti  *= 1000;

		/*----------------------------------------------------------------------------------------------*/
		/*----------------------------------------------------------------------------------------------*/

		// // FULL
		// CHRONO(SigmaDelta_full(mi0, mi1, mj0, mj1, image, mean0, mean1, img_diff, std0, std1, img_bin), cycles_total_full);
		// time_total_full  = (double)(cycles_total_full/CLK_PROC);
		// debit_total_full = (WIDTH*HEIGHT) / time_total_full;
		// time_total_full  *= 1000;

		// FULL OPTI
		CHRONO(SigmaDelta_full_opti(mi0, mi1, mj0, mj1, image, mean0, std0, img_bin), cycles_total_full_opti);
		time_total_full_opti  = (double)(cycles_total_full_opti/CLK_PROC);
		debit_total_full_opti = (WIDTH*HEIGHT) / time_total_full_opti;
		time_total_full_opti  *= 1000;

		/*----------------------------------------------------------------------------------------------*/
		/*----------------------------------------------------------------------------------------------*/

		// rotate mean and std matrix
		copy_ui8matrix(mi0, mi1, mj0, mj1, mean1, mean0);
		copy_ui8matrix(mi0, mi1, mj0, mj1, std1 , std0 );

		/*----------------------------------------------------------------------------------------------*/
		/*----------------------------------------------------------------------------------------------*/

		// cycles_step1_mean 			+= cycles_step1;
		// time_step1_mean  			+= time_step1;
		// debit_step1_mean 			+= debit_step1;

		// cycles_step2_mean 			+= cycles_step2;
		// time_step2_mean  			+= time_step2;
		// debit_step2_mean 			+= debit_step2;

		// cycles_step3_mean 			+= cycles_step3;
		// time_step3_mean  			+= time_step3;
		// debit_step3_mean 			+= debit_step3;

		// cycles_step4_mean 			+= cycles_step4;
		// time_step4_mean  			+= time_step4;
		// debit_step4_mean 			+= debit_step4;

		// cycles_total 				= cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
		// time_total   				= time_step1   + time_step2   + time_step3   + time_step4;
		// debit_total  				= (WIDTH*HEIGHT) / time_total;

		// cycles_total_mean 			+= cycles_total;
		// time_total_mean   			+= time_total;
		// debit_total_mean  			+= debit_total;

		// cycles_step1_opti_mean 		+= cycles_step1_opti;
		// time_step1_opti_mean  		+= time_step1_opti;
		// debit_step1_opti_mean 		+= debit_step1_opti;

		// cycles_step2_opti_mean 		+= cycles_step2_opti;
		// time_step2_opti_mean  		+= time_step2_opti;
		// debit_step2_opti_mean 		+= debit_step2_opti;

		// cycles_step3_opti_mean 		+= cycles_step3_opti;
		// time_step3_opti_mean  		+= time_step3_opti;
		// debit_step3_opti_mean 		+= debit_step3_opti;

		// cycles_step4_opti_mean 		+= cycles_step4_opti;
		// time_step4_opti_mean  		+= time_step4_opti;
		// debit_step4_opti_mean 		+= debit_step4_opti;

		// cycles_total_opti 			= cycles_step1_opti + cycles_step2_opti + cycles_step3_opti + cycles_step4_opti;
		// time_total_opti   			= time_step1_opti   + time_step2_opti   + time_step3_opti   + time_step4_opti;
		// debit_total_opti  			= (WIDTH*HEIGHT) / time_total_opti;

		// cycles_total_opti_mean 		+= cycles_total_opti; 
		// time_total_opti_mean 		+= time_total_opti;
		// debit_total_opti_mean		+= debit_total_opti;

		// cycles_total_full_mean 		+= cycles_total_full;
		// time_total_full_mean		+= time_total_full;
		// debit_total_full_mean		+= debit_total_full;

		cycles_total_full_opti_mean += cycles_total_full_opti;
		time_total_full_opti_mean 	+= time_total_full_opti;
		debit_total_full_opti_mean	+= debit_total_full_opti;
	}


	BENCH(printf("Total Mesure Dataset :\n\n"));

	// BENCH(printf("step 1 :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_step1_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step1_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts("")); 
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("step 1 optimisée :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_step1_opti_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step1_opti_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts("")); 
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1_opti_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("step 2 :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_step2_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step2_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("step 2 optimisée :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_step2_opti_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step2_opti_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts("")); 
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2_opti_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("step 3 :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_step3_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step3_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("step 3 optimisée :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_step3_opti_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step3_opti_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts("")); 
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3_opti)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("step 4 :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_step4_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step4_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("step 4 optimisée :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_step4_opti_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step4_opti_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4_opti_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("Total :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_total_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("Total optimisé :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_total_opti_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_opti_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_opti_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	// BENCH(printf("Total full :")); BENCH(puts(""));
	// BENCH(printf("temps (ms) \t    = %0.6f", time_total_full_mean/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_full_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
	// BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_full_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	BENCH(printf("Total full optimisé :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_full_opti_mean/NB_IMG)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_full_opti_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_full_opti_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));	

	// ---------- //
    // -- free -- //
    // ---------- //

	free_ui8matrix(image, mi0, mi1, mj0, mj1);

	free_ui8matrix(mean0, mi0, mi1, mj0, mj1);
	free_ui8matrix(mean1, mi0, mi1, mj0, mj1);

	free_ui8matrix(std0, mi0, mi1, mj0, mj1);
	free_ui8matrix(std1, mi0, mi1, mj0, mj1);

	free_ui8matrix(img_diff, mi0, mi1, mj0, mj1);
	free_ui8matrix(img_bin , mi0, mi1, mj0, mj1);
}

void bench_mouvement_graphic(){

	// init fichier csv
	FILE* fichier_csv = fopen("csv_files/perf_SigmaDelta.csv","w");
	fprintf(fichier_csv, "%s;;;;;%s\n", "Sigma Delta", "Sigma Delta Optimise");
	fprintf(fichier_csv, "%s;%s;%s;%s;", "Taille (pixels)", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
	fprintf(fichier_csv, ";%s;%s;%s\n", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");

	int mi0, mi1, mj0, mj1; 	// indices scalaire

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

    char *format = "%d ";

    // calcul cpp
	double cycles_dataset, cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4, cycles_total_full_opti;

	// calcul temps
	double time_total, time_step1, time_step2, time_step3, time_step4, time_total_full_opti;

	// calcul debit
	double debit_total, debit_total_full_opti;

    puts("====================================");
	puts("=== benchmark mouvement graphics ===");
	puts("====================================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //
    
    // Dimension initial des matrices générés
    int height = 32;
    int width  = 32;

    for (int i = 0; i < 300; ++i)
    {

    	// printf("\ni = %d\n", i);

    	// commence a 8 x 8
    	height += 16;
    	width  += 16;

    	// indices matrices
		mi0 = 0; mi1 = height-1;
		mj0 = 0; mj1 = width-1;

		// ---------------- //
	    // -- allocation -- //
	    // ---------------- //

	    uint8 ** image_init = ui8matrix(mi0, mi1, mj0, mj1);
	   	uint8 ** image 		= ui8matrix(mi0, mi1, mj0, mj1);

		uint8** mean0 		= ui8matrix(mi0, mi1, mj0, mj1);
		uint8** mean1 		= ui8matrix(mi0, mi1, mj0, mj1);

		uint8** std0 		= ui8matrix(mi0, mi1, mj0, mj1);
		uint8** std1 		= ui8matrix(mi0, mi1, mj0, mj1);

		uint8** img_diff 	= ui8matrix(mi0, mi1, mj0, mj1);
		uint8** img_bin 	= ui8matrix(mi0, mi1, mj0, mj1);

	   	for (int i = mi0; i <= mi1; ++i)
	   	{
	   		for (int j = mj0; j <= mj1; ++j)
	   		{
	   			// Générer des valeurs de pixels avec un motif qui evoluent avec la taille
	   			// Génére 2 images censé etre tres similaire (seul quelques pixels doivent varier)
	   			image_init [i][j]   = (i + j)*10 + j;
	   			image[i][j]  		= (i + j)*10 + j + i; 
	   		}
	   	}

	   	// initiate mean0 et std0 for first iteration
		for (int i = mi0; i <= mi1; ++i)
		{
			for (int j = mj0; j <= mj1; ++j)
			{
				mean0[i][j] = image_init[i][j];
				std0[i][j]  = VMIN;
			}
		}

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		// CHRONO(SigmaDelta_step1(mi0, mi1, mj0, mj1, mean0, mean1, image), cycles_step1);
		// time_step1 = (double)(cycles_step1/CLK_PROC);

		// CHRONO(SigmaDelta_step2(mi0, mi1, mj0, mj1, image, mean1, img_diff), cycles_step2);
		// time_step2 = (double)(cycles_step2/CLK_PROC);
		
		// CHRONO(SigmaDelta_step3(mi0, mi1, mj0, mj1, std0, std1, img_diff), cycles_step3);
		// time_step3 = (double)(cycles_step3/CLK_PROC);

		// CHRONO(SigmaDelta_step4(mi0, mi1, mj0, mj1, std1, img_diff, img_bin), cycles_step4);
		// time_step4 = (double)(cycles_step4/CLK_PROC);

		// cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
		// time_total   = time_step1   + time_step2   + time_step3   + time_step4;
		// debit_total  = (width*height) / time_total;

		// FULL OPTI
		CHRONO(SigmaDelta_full_opti(mi0, mi1, mj0, mj1, image, mean0, std0, img_bin), cycles_total_full_opti);
		time_total_full_opti = (double)(cycles_total_full_opti/CLK_PROC);
		debit_total_full_opti = (width*height) / time_total_full_opti;

		// ecrire les donnees dans un fichier csv
		fprintf(fichier_csv, "%d;", height);
		fprintf(fichier_csv, "%f;", time_total*1000);
		fprintf(fichier_csv, "%f;", cycles_total/(height*width));
		fprintf(fichier_csv, "%f;", debit_total);

		fprintf(fichier_csv, ";%f;", time_total_full_opti*1000);
		fprintf(fichier_csv, "%f;" , cycles_total_full_opti/(height*width));
		fprintf(fichier_csv, "%f\n", debit_total_full_opti);

		// ---------- //
	    // -- free -- //
	    // ---------- //

		free_ui8matrix(image, mi0, mi1, mj0, mj1);

		free_ui8matrix(mean0, mi0, mi1, mj0, mj1);
		free_ui8matrix(mean1, mi0, mi1, mj0, mj1);

		free_ui8matrix(std0, mi0, mi1, mj0, mj1);
		free_ui8matrix(std1, mi0, mi1, mj0, mj1);

		free_ui8matrix(img_diff, mi0, mi1, mj0, mj1);
		free_ui8matrix(img_bin, mi0, mi1, mj0, mj1);
    }

    fclose(fichier_csv);
}

void main_bench_mouvement(int argc, char *argv[]){

	// Affiche les métriques de performance

	// benchmark unitaire sur une image du set
	// bench_mouvement_car();

	// benchmark global sur tout le dataset
	// bench_mouvement_dataset();

	// benchmark sur images generees pour fichier csv
	bench_mouvement_graphic();
}
