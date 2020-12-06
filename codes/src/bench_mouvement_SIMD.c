/* --------------------------------------------------------------------- */
/* ---  Bench Algorithme Sigma Delta SIMD pour le traitement d'image --- */
/* --------------------------------------------------------------------- */

#include "bench_mouvement_SIMD.h"

void bench_mouvement_SIMD_car(){

	int width, height;

	// img reel size
	width  = 320; // correspond au nb de colonne  => indice boucle j
	height = 240; // correspond au nb de ligne    => indice boucle i

	// chronometrie
	int iter, niter = 4;
	int run, nrun = 5;
	double t0, t1, dt, tmin, t;

	char *format = "%d ";

	// calcul cpp
	double cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4;
	double cycles_total_unroll, cycles_step1_unroll, cycles_step2_unroll, cycles_step3_unroll, cycles_step4_unroll;
	double cycles_total_fusion, cycles_total_fusion_unroll;

	// calcul temps
	double time_total, time_step1, time_step2, time_step3, time_step4;
	double time_total_unroll, time_step1_unroll, time_step2_unroll, time_step3_unroll, time_step4_unroll;
	double time_total_fusion, time_total_fusion_unroll;

	// calcul debit
	double debit_total, debit_step1, debit_step2, debit_step3, debit_step4;
	double debit_total_unroll, debit_step1_unroll, debit_step2_unroll, debit_step3_unroll, debit_step4_unroll;
	double debit_total_fusion, debit_total_fusion_unroll;

	puts("=========================================");
	puts("=== benchmark mouvement unitaire SIMD ===");
	puts("=========================================");

	// cardinalité des registres
	int card = card_vuint8(); // = 16 ici

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
	vuint8** mean0				= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** mean1 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);		

	// ecart-types
	vuint8** std0 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** std1 				= vui8matrix(vmi0, vmi1, vmj0, vmj1);			

	// image de différence
	vuint8 ** img_diff 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);	

	// image binaire (sortie)
	vuint8 ** img_bin 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);


	/*---------------------------------------------------*/

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


	/*---------------------------------------------------*/

	// ----------------- //
	// -- traitements -- //
	// ----------------- //

	BENCH(printf("Sigma Delta SIMD :\n\n"));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 1
	CHRONO(SigmaDelta_step1_simd(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image), cycles_step1);
	time_step1 = (double)(cycles_step1/CLK_PROC);
	debit_step1 = (width*height) / time_step1;	

	BENCH(printf("step 1 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1)); BENCH(puts("")); BENCH(puts(""));

	// STEP 1 UNROLL
	CHRONO(SigmaDelta_step1_simd_unroll(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image), cycles_step1_unroll);
	time_step1_unroll = (double)(cycles_step1_unroll/CLK_PROC);
	debit_step1_unroll = (WIDTH*HEIGHT) / time_step1_unroll;

	BENCH(printf("step 1 unroll :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1_unroll*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1_unroll/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1_unroll)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 2
	CHRONO(SigmaDelta_step2_simd(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff), cycles_step2);
	time_step2 = (double)(cycles_step2/CLK_PROC);
	debit_step2 = (width*height) / time_step2;

	BENCH(printf("step 2 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2)); BENCH(puts("")); BENCH(puts(""));

	// STEP 2 UNROLL
	CHRONO(SigmaDelta_step2_simd_unroll(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff), cycles_step2_unroll);
	time_step2_unroll = (double)(cycles_step2_unroll/CLK_PROC);
	debit_step2_unroll = (WIDTH*HEIGHT) / time_step2_unroll;

	BENCH(printf("step 2 unroll :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2_unroll*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2_unroll/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2_unroll)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 3
	CHRONO(SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff), cycles_step3);
	time_step3 = (double)(cycles_step3/CLK_PROC);
	debit_step3 = (width*height) / time_step3;

	BENCH(printf("step 3 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3)); BENCH(puts("")); BENCH(puts(""));

	// STEP 3 UNROLL
	CHRONO(SigmaDelta_step3_simd_unroll(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff), cycles_step3_unroll);
	time_step3_unroll = (double)(cycles_step3_unroll/CLK_PROC);
	debit_step3_unroll = (WIDTH*HEIGHT) / time_step3_unroll;

	BENCH(printf("step 3 unroll :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3_unroll*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3_unroll/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3_unroll)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 4
	CHRONO(SigmaDelta_step4_simd(vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin), cycles_step4);
	time_step4 = (double)(cycles_step4/CLK_PROC);
	debit_step4 = (width*height) / time_step4;

	BENCH(printf("step 4 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4)); BENCH(puts("")); BENCH(puts(""));

	// STEP 4 UNROLL
	CHRONO(SigmaDelta_step4_simd_unroll(vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin), cycles_step4_unroll);
	time_step4_unroll = (double)(cycles_step4_unroll/CLK_PROC);
	debit_step4_unroll = (WIDTH*HEIGHT) / time_step4_unroll;

	BENCH(printf("step 4 unroll :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4_unroll*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4_unroll/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4_unroll)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// TOTAL
	cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
	time_total   = time_step1   + time_step2   + time_step3   + time_step4;
	debit_total  = (width*height) / time_total;

	BENCH(printf("Total :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total)); BENCH(puts("")); BENCH(puts(""));

	// TOTAL UNROLL
	cycles_total_unroll = cycles_step1_unroll + cycles_step2_unroll + cycles_step3_unroll + cycles_step4_unroll;
	time_total_unroll   = time_step1_unroll   + time_step2_unroll   + time_step3_unroll   + time_step4_unroll;
	debit_total_unroll  = (WIDTH*HEIGHT) / time_total_unroll;

	BENCH(printf("Total unroll :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_unroll * 1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_unroll / (WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_unroll)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// FULL
	CHRONO(SigmaDelta_simd_fusion(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_fusion);
	time_total_fusion  = (double)(cycles_total_fusion/CLK_PROC);
	debit_total_fusion = (WIDTH*HEIGHT) / time_total_fusion;
	time_total_fusion  *= 1000;

	BENCH(printf("Total fusion :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_fusion)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_fusion/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_fusion)); BENCH(puts("")); BENCH(puts(""));	

	// FULL UNROLL
	CHRONO(SigmaDelta_simd_fusion_unroll(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_fusion_unroll);
	time_total_fusion_unroll  = (double)(cycles_total_fusion_unroll/CLK_PROC);
	debit_total_fusion_unroll = (WIDTH*HEIGHT) / time_total_fusion_unroll;
	time_total_fusion_unroll  *= 1000;

	BENCH(printf("Total fusion unroll :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_fusion_unroll)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_fusion_unroll/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_fusion_unroll)); BENCH(puts("")); BENCH(puts(""));

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

void bench_mouvement_SIMD_dataset(type_sd_t TYPE_SD, type_opti_t TYPE_OPTI){

	// img reel size
	int width  = 320; // correspond au nb de colonne  => indice boucle j
	int height = 240; // correspond au nb de ligne    => indice boucle i

	// chronometrie
	int iter, niter = 4;
	int run, nrun = 5;
	double t0, t1, dt, tmin, t;

	char *format = "%d ";

	// calcul cpp
	double cycles_step1 = 0, cycles_step1_mean = 0, cycles_step1_unroll = 0, cycles_step1_unroll_mean = 0;
	double cycles_step2 = 0, cycles_step2_mean = 0, cycles_step2_unroll = 0, cycles_step2_unroll_mean = 0;
	double cycles_step3 = 0, cycles_step3_mean = 0, cycles_step3_unroll = 0, cycles_step3_unroll_mean = 0;
	double cycles_step4 = 0, cycles_step4_mean = 0, cycles_step4_unroll = 0, cycles_step4_unroll_mean = 0;
   	double cycles_total = 0, cycles_total_mean = 0, cycles_total_unroll = 0, cycles_total_unroll_mean = 0; 
   	
   	double cycles_total_fusion = 0, cycles_total_fusion_mean = 0, cycles_total_fusion_unroll = 0, cycles_total_fusion_unroll_mean = 0;

   	double cycles_total_fusion_omp = 0, cycles_total_fusion_mean_omp = 0, cycles_total_fusion_unroll_omp = 0, cycles_total_fusion_unroll_mean_omp = 0;

	// calcul temps
   	double time_step1 = 0, time_step1_mean = 0, time_step1_unroll = 0, time_step1_unroll_mean = 0;
   	double time_step2 = 0, time_step2_mean = 0, time_step2_unroll = 0, time_step2_unroll_mean = 0;
   	double time_step3 = 0, time_step3_mean = 0, time_step3_unroll = 0, time_step3_unroll_mean = 0;
   	double time_step4 = 0, time_step4_mean = 0, time_step4_unroll = 0, time_step4_unroll_mean = 0;
	double time_total = 0, time_total_mean = 0, time_total_unroll = 0, time_total_unroll_mean = 0;

	double time_total_fusion = 0, time_total_fusion_mean = 0, time_total_fusion_unroll = 0, time_total_fusion_unroll_mean = 0;

	double time_total_fusion_omp = 0, time_total_fusion_mean_omp = 0, time_total_fusion_unroll_omp = 0, time_total_fusion_unroll_mean_omp = 0;

	// calcul debit
	double debit_step1 = 0, debit_step1_mean = 0, debit_step1_unroll = 0, debit_step1_unroll_mean = 0;
	double debit_step2 = 0, debit_step2_mean = 0, debit_step2_unroll = 0, debit_step2_unroll_mean = 0;
	double debit_step3 = 0, debit_step3_mean = 0, debit_step3_unroll = 0, debit_step3_unroll_mean = 0;
	double debit_step4 = 0, debit_step4_mean = 0, debit_step4_unroll = 0, debit_step4_unroll_mean = 0;
	double debit_total = 0, debit_total_mean = 0, debit_total_unroll = 0, debit_total_unroll_mean = 0;

	double debit_total_fusion = 0, debit_total_fusion_mean = 0, debit_total_fusion_unroll = 0, debit_total_fusion_unroll_mean = 0;

	double debit_total_fusion_omp = 0, debit_total_fusion_mean_omp = 0, debit_total_fusion_unroll_omp = 0, debit_total_fusion_unroll_mean_omp = 0;

	puts("========================================");
	puts("=== benchmark mouvement dataset SIMD ===");
	puts("========================================");

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

		/*---------------------------------------------------*/

		// --------------------------- //
    	// -- chargement de l'image -- //
    	// --------------------------- //

		MLoadPGM_ui8matrix(filename, mi0, mi1, mj0, mj1, img_temp);

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_temp, image);

		/*---------------------------------------------------*/

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

	    switch(TYPE_SD)
	    {
	    	case SD:
	    		switch(TYPE_OPTI)
	    		{
	    			case SIMD:

	    				// STEP 1
						CHRONO(SigmaDelta_step1_simd(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image), cycles_step1);
						time_step1  = (double)(cycles_step1/CLK_PROC);
						debit_step1 = (width*height) / time_step1;
						time_step1 	*= 1000;

						cycles_step1_mean 			+= cycles_step1;
						time_step1_mean  			+= time_step1;
						debit_step1_mean 			+= debit_step1;

						// STEP 2
						CHRONO(SigmaDelta_step2_simd(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff), cycles_step2);
						time_step2 	= (double)(cycles_step2/CLK_PROC);
						debit_step2 =  (width*height) / time_step2;
						time_step2 	*= 1000;

						cycles_step2_mean 			+= cycles_step2;
						time_step2_mean  			+= time_step2;
						debit_step2_mean 			+= debit_step2;

						// STEP 3
						CHRONO(SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff), cycles_step3);
						time_step3  = (double)(cycles_step3/CLK_PROC);
						debit_step3 =  (width*height) / time_step3;
						time_step3  *= 1000;

						cycles_step3_mean 			+= cycles_step3;
						time_step3_mean  			+= time_step3;
						debit_step3_mean 			+= debit_step3;

						// STEP 4
						CHRONO(SigmaDelta_step4_simd(vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin), cycles_step4);
						time_step4  = (double)(cycles_step4/CLK_PROC);
						debit_step4 =  (width*height) / time_step4;
						time_step4  *= 1000;

						cycles_step4_mean 			+= cycles_step4;
						time_step4_mean  			+= time_step4;
						debit_step4_mean 			+= debit_step4;

						cycles_total 				= cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
						time_total   				= time_step1   + time_step2   + time_step3   + time_step4;
						debit_total  				= (WIDTH*HEIGHT) / time_total;

						cycles_total_mean 			+= cycles_total;
						time_total_mean   			+= time_total;
						debit_total_mean  			+= debit_total;

	    				break;

	    			case SIMD_UNROLL:

		    			// STEP 1 UNROLL
						CHRONO(SigmaDelta_step1_simd_unroll(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image), cycles_step1_unroll);
						time_step1_unroll 	= (double)(cycles_step1_unroll/CLK_PROC);
						debit_step1_unroll 	= (width*height) / time_step1_unroll;
						time_step1_unroll 	*= 1000;

						cycles_step1_unroll_mean 		+= cycles_step1_unroll;
						time_step1_unroll_mean  		+= time_step1_unroll;
						debit_step1_unroll_mean 		+= debit_step1_unroll;

						// STEP 2 UNROLL
						CHRONO(SigmaDelta_step2_simd_unroll(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff), cycles_step2_unroll);
						time_step2_unroll  = (double)(cycles_step2_unroll/CLK_PROC);
						debit_step2_unroll = (width*height) / time_step2_unroll;
						time_step2_unroll  *= 1000;

						cycles_step2_unroll_mean 		+= cycles_step2_unroll;
						time_step2_unroll_mean  		+= time_step2_unroll;
						debit_step2_unroll_mean 		+= debit_step2_unroll;

						// STEP 3 UNROLL
						CHRONO(SigmaDelta_step3_simd_unroll(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff), cycles_step3_unroll);
						time_step3_unroll  = (double)(cycles_step3_unroll/CLK_PROC);
						debit_step3_unroll = (width*height) / time_step3_unroll;
						time_step3_unroll  *= 1000;

						cycles_step3_unroll_mean 		+= cycles_step3_unroll;
						time_step3_unroll_mean  		+= time_step3_unroll;
						debit_step3_unroll_mean 		+= debit_step3_unroll;

						// STEP 4 UNROLL
						CHRONO(SigmaDelta_step4_simd_unroll(vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin), cycles_step4_unroll);
						time_step4_unroll  = (double)(cycles_step4_unroll/CLK_PROC);
						debit_step4_unroll = (width*height) / time_step4_unroll;
						time_step4_unroll  *= 1000;

						cycles_step4_unroll_mean 		+= cycles_step4_unroll;
						time_step4_unroll_mean  		+= time_step4_unroll;
						debit_step4_unroll_mean 		+= debit_step4_unroll;

						cycles_total_unroll 			= cycles_step1_unroll + cycles_step2_unroll + cycles_step3_unroll + cycles_step4_unroll;
						time_total_unroll   			= time_step1_unroll   + time_step2_unroll   + time_step3_unroll   + time_step4_unroll;
						debit_total_unroll  			= (WIDTH*HEIGHT) / time_total_unroll;

						cycles_total_unroll_mean 		+= cycles_total_unroll; 
						time_total_unroll_mean 		+= time_total_unroll;
						debit_total_unroll_mean		+= debit_total_unroll;

	    				break;

	    			default:
	    				break;
	    		}
	    		break;

	    	case SD_FULL:
	    		switch(TYPE_OPTI)
	    		{
	    			case SIMD:

	    				// FULL
						CHRONO(SigmaDelta_simd_fusion(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_fusion);
						time_total_fusion  = (double)(cycles_total_fusion/CLK_PROC);
						debit_total_fusion = (width*height) / time_total_fusion;
						time_total_fusion  *= 1000;

						cycles_total_fusion_mean 		+= cycles_total_fusion;
						time_total_fusion_mean			+= time_total_fusion;
						debit_total_fusion_mean			+= debit_total_fusion;

	    				break;

	    			case SIMD_UNROLL:

	    				// FULL UNROLL
						CHRONO(SigmaDelta_simd_fusion_unroll(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_fusion_unroll);
						time_total_fusion_unroll  = (double)(cycles_total_fusion_unroll/CLK_PROC);
						debit_total_fusion_unroll = (width*height) / time_total_fusion_unroll;
						time_total_fusion_unroll  *= 1000;

						cycles_total_fusion_unroll_mean += cycles_total_fusion_unroll;
						time_total_fusion_unroll_mean 	+= time_total_fusion_unroll;
						debit_total_fusion_unroll_mean	+= debit_total_fusion_unroll;

	    				break;

	    			case SIMD_OMP:

	    				// FULL OPEN MP
						CHRONO(SigmaDelta_simd_fusion_openMP(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_fusion_omp);
						time_total_fusion_omp  = (double)(cycles_total_fusion_omp/CLK_PROC);
						debit_total_fusion_omp = (width*height) / time_total_fusion_omp;
						time_total_fusion_omp  *= 1000;

						cycles_total_fusion_mean_omp += cycles_total_fusion_omp;
						time_total_fusion_mean_omp 	+= time_total_fusion_omp;
						debit_total_fusion_mean_omp	+= debit_total_fusion_omp;

	    				break;

	    			case SIMD_UNROLL_OMP:

	    				// FULL UNROLL OPEN MP
						CHRONO(SigmaDelta_simd_fusion_unroll_openMP(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_fusion_unroll_omp);
						time_total_fusion_unroll_omp  = (double)(cycles_total_fusion_unroll_omp/CLK_PROC);
						debit_total_fusion_unroll_omp = (width*height) / time_total_fusion_unroll_omp;
						time_total_fusion_unroll_omp  *= 1000;

						cycles_total_fusion_unroll_mean_omp += cycles_total_fusion_unroll_omp;
						time_total_fusion_unroll_mean_omp 	+= time_total_fusion_unroll_omp;
						debit_total_fusion_unroll_mean_omp	+= debit_total_fusion_unroll_omp;

	    				break;

	    			default:
	    				break;
	    		}
	    		break;

	    	default:
	    		break;
	    }

		// rotate mean and std vmatrix
		copy_vui8matrix(vmi0, vmi1, vmj0, vmj1, mean1, mean0);
		copy_vui8matrix(vmi0, vmi1, vmj0, vmj1, std1 , std0 );
	}

	// --------------- //
    // -- affichage -- //
    // --------------- //

	BENCH(printf("Total Mesure Dataset :\n\n"));

	switch(TYPE_SD)
	    {
	    	case SD:
	    		switch(TYPE_OPTI)
	    		{
	    			case SIMD:

	    				BENCH(printf("step 1 :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_step1_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step1_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts("")); 
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

						BENCH(printf("step 2 :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_step2_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step2_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));
						
						BENCH(printf("step 3 :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_step3_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step3_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));
						
						BENCH(printf("step 4 :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_step4_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step4_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

						BENCH(printf("Total :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_total_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	    				break;

	    			case SIMD_UNROLL:

	    				BENCH(printf("step 1 unroll :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_step1_unroll_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step1_unroll_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts("")); 
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1_unroll_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

						BENCH(printf("step 2 unroll :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_step2_unroll_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step2_unroll_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts("")); 
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2_unroll_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

						BENCH(printf("step 3 unroll :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_step3_unroll_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step3_unroll_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts("")); 
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3_unroll_mean)); BENCH(puts("")); BENCH(puts(""));

						BENCH(printf("step 4 unroll :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_step4_unroll_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_step4_unroll_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4_unroll_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

						BENCH(printf("Total unroll :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_total_unroll_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_unroll_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_unroll_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	    				break;

	    			default:
	    				break;
	    		}
	    		break;

	    	case SD_FULL:
	    		switch(TYPE_OPTI)
	    		{
	    			case SIMD:

	    				BENCH(printf("Total fusion :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_total_fusion_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_fusion_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_fusion_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	    				break;

	    			case SIMD_UNROLL:

	    				BENCH(printf("Total fusion unroll :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_total_fusion_unroll_mean/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_fusion_unroll_mean/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_fusion_unroll_mean/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	    				break;

    				case SIMD_OMP:

	    				BENCH(printf("Total fusion Open MP :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_total_fusion_mean_omp/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_fusion_mean_omp/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_fusion_mean_omp/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

    				break;

	    			case SIMD_UNROLL_OMP:

	    				BENCH(printf("Total fusion optimisé Open MP :")); BENCH(puts(""));
						BENCH(printf("temps (ms) \t    = %0.6f", time_total_fusion_unroll_mean_omp/NB_IMG)); BENCH(puts(""));
						BENCH(printf("cpp   (cycle/pixel) = %0.6f", (cycles_total_fusion_unroll_mean_omp/(WIDTH*HEIGHT))/NB_IMG)); BENCH(puts(""));
						BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_fusion_unroll_mean_omp/NB_IMG)); BENCH(puts("")); BENCH(puts(""));

	    				break;

	    			default:
	    				break;
	    		}
	    		break;

	    	default:
	    		break;
	    }

	/*---------------------------------------------------*/

	// ---------- //
	// -- free -- //
	// ---------- //

	free_vui8matrix(image, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(mean0, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(mean1, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(std0 , vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(std1 , vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(img_diff, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(img_bin , vmi0, vmi1, vmj0, vmj1);
}

void bench_mouvement_SIMD_graphic(){

	// init fichier csv
	FILE* fichier_csv = fopen("csv_files/perf_SigmaDelta_SIMD.csv","w");
	fprintf(fichier_csv, "%s;;;;;%s;;;;;%s\n", "Sigma Delta", "Sigma Delta Optimise", "SigmaDelta Optimise OMP");
	fprintf(fichier_csv, "%s;%s;%s;%s;", "Taille (pixels)", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
	fprintf(fichier_csv, ";%s;%s;%s;", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
	fprintf(fichier_csv, ";%s;%s;%s\n", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int vmi0, vmi1, vmj0, vmj1;

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

    char *format = "%d ";

    // cardinalité des registres
	int card = card_vuint8(); // 16

    // calcul cpp
	double cycles_total = 0, cycles_step1 = 0, cycles_step2 = 0, cycles_step3 = 0, cycles_step4 = 0;
	double cycles_total_fusion_unroll = 0;
	double cycles_total_fusion_unroll_omp = 0;

	// calcul temps
	double time_total = 0, time_step1 = 0, time_step2 = 0, time_step3 = 0, time_step4 = 0;
	double time_total_fusion_unroll = 0;
	double time_total_fusion_unroll_omp = 0;

	// calcul debit
	double debit_total = 0;
	double debit_total_fusion_unroll = 0;
	double debit_total_fusion_unroll_omp = 0;

    puts("=========================================");
	puts("=== benchmark mouvement SIMD graphics ===");
	puts("=========================================");
    
    // Dimension initial des matrices générés
    int height = 32;
    int width  = 32;

    for (int i = 0; i < 300; ++i)
    {
    	// printf("\ni = %d\n", i);
    	// printf("\nheight = %d\n", height);

    	// commence a 8 x 8
    	height += 16;
    	width  += 16;

	    // ------------------------- //
	    // -- calculs des indices -- //
	    // ------------------------- //

    	// indices matrices
		mi0 = 0; mi1 = height-1;
		mj0 = 0; mj1 = width-1;

		// indices vectoriels matrices
		vmi0 = 0; vmi1 = (height)-1;
		vmj0 = 0; vmj1 = (width/card)-1;

		// ---------------- //
	    // -- allocation -- //
	    // ---------------- //

	    uint8 ** image_init 	= ui8matrix(mi0, mi1, mj0, mj1);
	   	uint8 ** img_temp 		= ui8matrix(mi0, mi1, mj0, mj1);

	   	// images
		vuint8** image 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);

		// moyennes
		vuint8** mean0 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);
		vuint8** mean1 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);

		// ecart-types
		vuint8** std0 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);
		vuint8** std1 			= vui8matrix(vmi0, vmi1, vmj0, vmj1);

		// image de différence
		vuint8 ** img_diff 		= vui8matrix(vmi0, vmi1, vmj0, vmj1);

		// image binaire (sortie)
		vuint8 ** img_bin 		= vui8matrix(vmi0, vmi1, vmj0, vmj1);

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
	   			img_temp[i][j]  	= val2; 
	   		}
	   	}

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, image_init, image);

	   	// initiate mean0 et std0 for first iteration
		for (int i = vmi0; i <= vmi1; ++i)
		{
			for (int j = vmj0; j <= vmj1; ++j)
			{
				mean0[i][j] = image[i][j];
				std0[i][j]  = init_vuint8(VMIN);
			}
		}

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_temp, image);

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		CHRONO(SigmaDelta_step1_simd(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image), cycles_step1);
		time_step1 = (double)(cycles_step1/CLK_PROC);

		CHRONO(SigmaDelta_step2_simd(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff), cycles_step2);
		time_step2 = (double)(cycles_step2/CLK_PROC);
		
		CHRONO(SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff), cycles_step3);
		time_step3 = (double)(cycles_step3/CLK_PROC);

		CHRONO(SigmaDelta_step4_simd( vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin), cycles_step4);
		time_step4 = (double)(cycles_step4/CLK_PROC);

		cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
		time_total   = time_step1   + time_step2   + time_step3   + time_step4;
		debit_total  = (width*height) / time_total;

		// FULL UNROLL
		CHRONO(SigmaDelta_simd_fusion_unroll(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_fusion_unroll);
		time_total_fusion_unroll = (double)(cycles_total_fusion_unroll/CLK_PROC);
		debit_total_fusion_unroll = (width*height) / time_total_fusion_unroll;

		// FULL UNROLL OMP
		CHRONO(SigmaDelta_simd_fusion_unroll_openMP(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_fusion_unroll_omp);
		time_total_fusion_unroll_omp = (double)(cycles_total_fusion_unroll_omp/CLK_PROC);
		debit_total_fusion_unroll_omp = (width*height) / time_total_fusion_unroll_omp;

		// ecrire les donnees dans un fichier csv
		fprintf(fichier_csv, "%d;", height);
		fprintf(fichier_csv, "%f;", time_total*1000);
		fprintf(fichier_csv, "%f;", cycles_total/(height*width));
		fprintf(fichier_csv, "%f;", debit_total);

		fprintf(fichier_csv, ";%f;" , time_total_fusion_unroll*1000);
		fprintf(fichier_csv, "%f;"  , cycles_total_fusion_unroll/(height*width));
		fprintf(fichier_csv, "%f;"  , debit_total_fusion_unroll);

		fprintf(fichier_csv, ";%f;" , time_total_fusion_unroll_omp * 1000);
		fprintf(fichier_csv, "%f;"  , cycles_total_fusion_unroll_omp / (height*width));
		fprintf(fichier_csv, "%f\n" , debit_total_fusion_unroll_omp);

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

    fclose(fichier_csv);
}

void main_bench_mouvement_SIMD(int argc, char *argv[]){

	// benchmark unitaire sur image du set
	// bench_mouvement_SIMD_car();

	// benchmark global sur tout le dataset

	// bench_mouvement_SIMD_dataset(SD, SIMD);

	// bench_mouvement_SIMD_dataset(SD_FULL, SIMD);

	// bench_mouvement_SIMD_dataset(SD_FULL, SIMD_UNROLL);

	// bench_mouvement_SIMD_dataset(SD_FULL, SIMD_OMP);

	// bench_mouvement_SIMD_dataset(SD_FULL, SIMD_UNROLL_OMP);

	// benchmark sur images generees pour fichier csv
	bench_mouvement_SIMD_graphic();
}
