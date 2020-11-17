/* --------------------------------------------------------------------- */
/* ---  Bench Algorithme Sigma Delta SIMD pour le traitement d'image --- */
/* --------------------------------------------------------------------- */

#include "bench_mouvement_SIMD.h"

void bench_mouvement_SIMD_car(bool is_visual){

	int width, height;

	if (is_visual)
	{
		// img test size
		width  = 32; // correspond au nb de colonne  => indice boucle j
		height = 16; // correspond au nb de ligne    => indice boucle i
	}
	else{

		// img reel size
		width  = 320; // correspond au nb de colonne  => indice boucle j
		height = 240; // correspond au nb de ligne    => indice boucle i
	}

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

	puts("=========================================");
	puts("=== benchmark mouvement unitaire SIMD ===");
	puts("=========================================");

	// BORD
	// 1 for 3x3 
	int b = 1; 

	// 2 for 5x5
	//int b = 2;

	// cardinalité des registres
	int card = card_vuint8(); // = 16 ici

	// ------------------------- //
	// -- calculs des indices -- //
	// ------------------------- //

	// indices scalaires matrices
	int mi0 = 0; int mi1 = height-1;
	int mj0 = 0; int mj1 = width-1;

	// indices scalaires matrices avec bord
	int mi0b = mi0-b; int mi1b = mi1+b;
	int mj0b = mj0-b; int mj1b = mj1+b;

	// indices vectoriels matrices
	int vmi0 = 0; int vmi1 = (height)-1;
	int vmj0 = 0; int vmj1 = (width/card)-1;

	// indices vectoriels matrices avec bord
	int vmi0b = vmi0-b; int vmi1b = vmi1+b;
	int vmj0b = vmj0-1; int vmj1b = vmj1+1;

	// images
	vuint8** image = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	// moyennes
	vuint8** mean0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	vuint8** mean1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);		

	// ecart-types
	vuint8** std0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	vuint8** std1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);			

	// image de différence
	vuint8 ** img_diff = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);	

	// image binaire (sortie)
	vuint8 ** img_bin = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);


	/*---------------------------------------------------*/

	// ------------------------------ //
	// -- chargement et conversion -- //
	// ------------------------------ //

	uint8 ** img_temp = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	if (is_visual)
    {
    	// gen img visuel
		gen_pgm_img_simd();

		MLoadPGM_ui8matrix("pgm_imgs/my_pgm1.pgm", mi0b, mi1b, mj0b, mj1b, img_temp);

		duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp, image);

		// initiate mean0 et std0 for first iteration
		for (int i = vmi0b; i <= vmi1b; ++i)
		{
			for (int j = vmj0b; j <= vmj1b; ++j)
			{
				mean0[i][j] = image[i][j];
				std0[i][j]  = init_vuint8(VMIN);
			}
		}

		MLoadPGM_ui8matrix("pgm_imgs/my_pgm2.pgm", mi0b, mi1b, mj0b, mj1b, img_temp);

		duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);
    }
    else
    {
		MLoadPGM_ui8matrix("../car3/car_3037.pgm", mi0b, mi1b, mj0b, mj1b, img_temp);

		duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);

		// transfert ui8matrix à vui8matrix init

		ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp, image);

		// initiate mean0 et std0 for first iteration
		for (int i = vmi0b; i <= vmi1b; ++i)
		{
			for (int j = vmj0b; j <= vmj1b; ++j)
			{
				mean0[i][j] = image[i][j];
				std0[i][j]  = init_vuint8(VMIN);
			}
		}

		MLoadPGM_ui8matrix("../car3/car_3038.pgm", mi0b, mi1b, mj0b, mj1b, img_temp);

		duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);
    }

	// transfert ui8matrix à vui8matrix real
	ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp, image);


	/*---------------------------------------------------*/

	// ----------------- //
	// -- traitements -- //
	// ----------------- //

	BENCH(printf("Sigma Delta SIMD :\n\n"));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 1
	CHRONO(SigmaDelta_step1_simd(vmi0b, vmi1b, vmj0b, vmj1b, mean0, mean1, image), cycles_step1);
	time_step1 = (double)(cycles_step1/CLK_PROC);
	debit_step1 = (width*height) / time_step1;	

	BENCH(printf("step 1 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1)); BENCH(puts("")); BENCH(puts(""));

	// STEP 1 OPTI
	CHRONO(SigmaDelta_step1_simd_opti(vmi0b, vmi1b, vmj0b, vmj1b, mean0, mean1, image), cycles_step1_opti);
	time_step1_opti = (double)(cycles_step1_opti/CLK_PROC);
	debit_step1_opti = (WIDTH*HEIGHT) / time_step1_opti;

	BENCH(printf("step 1 optimisée :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1_opti*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1_opti/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 2
	CHRONO(SigmaDelta_step2_simd(vmi0b, vmi1b, vmj0b, vmj1b, image, mean1, img_diff), cycles_step2);
	time_step2 = (double)(cycles_step2/CLK_PROC);
	debit_step2 = (width*height) / time_step2;

	BENCH(printf("step 2 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2)); BENCH(puts("")); BENCH(puts(""));

	// STEP 2 OPTI
	CHRONO(SigmaDelta_step2_simd_opti(vmi0b, vmi1b, vmj0b, vmj1b, image, mean1, img_diff), cycles_step2_opti);
	time_step2_opti = (double)(cycles_step2_opti/CLK_PROC);
	debit_step2_opti = (WIDTH*HEIGHT) / time_step2_opti;

	BENCH(printf("step 2 optimisée :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2_opti*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2_opti/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 3
	CHRONO(SigmaDelta_step3_simd(vmi0b, vmi1b, vmj0b, vmj1b, std0, std1, img_diff), cycles_step3);
	time_step3 = (double)(cycles_step3/CLK_PROC);
	debit_step3 = (width*height) / time_step3;

	BENCH(printf("step 3 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3)); BENCH(puts("")); BENCH(puts(""));

	// STEP 3 OPTI
	CHRONO(SigmaDelta_step3_simd_opti(vmi0b, vmi1b, vmj0b, vmj1b, std0, std1, img_diff), cycles_step3_opti);
	time_step3_opti = (double)(cycles_step3_opti/CLK_PROC);
	debit_step3_opti = (WIDTH*HEIGHT) / time_step3_opti;

	BENCH(printf("step 3 optimisée :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3_opti*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3_opti/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// STEP 4
	CHRONO(SigmaDelta_step4_simd( vmi0b, vmi1b, vmj0b, vmj1b, std1, img_diff, img_bin), cycles_step4);
	time_step4 = (double)(cycles_step4/CLK_PROC);
	debit_step4 = (width*height) / time_step4;

	BENCH(printf("step 4 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4)); BENCH(puts("")); BENCH(puts(""));

	// STEP 4 OPTI
	CHRONO(SigmaDelta_step4_simd_opti(vmi0b, vmi1b, vmj0b, vmj1b, std1, img_diff, img_bin), cycles_step4_opti);
	time_step4_opti = (double)(cycles_step4_opti/CLK_PROC);
	debit_step4_opti = (WIDTH*HEIGHT) / time_step4_opti;

	BENCH(printf("step 4 optimisée :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4_opti*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4_opti/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4_opti)); BENCH(puts("")); BENCH(puts(""));

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

	// TOTAL OPTI
	cycles_total_opti = cycles_step1_opti + cycles_step2_opti + cycles_step3_opti + cycles_step4_opti;
	time_total_opti   = time_step1_opti   + time_step2_opti   + time_step3_opti   + time_step4_opti;
	debit_total_opti  = (WIDTH*HEIGHT) / time_total_opti;

	BENCH(printf("Total optimisé :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_opti*1000)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_opti/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_opti)); BENCH(puts("")); BENCH(puts(""));

	/*----------------------------------------------------------------------------------------------*/
	/*----------------------------------------------------------------------------------------------*/

	// FULL
	CHRONO(SigmaDelta_simd_full(vmi0b, vmi1b, vmj0b, vmj1b, image, mean0, std0, img_bin), cycles_total_full);
	time_total_full = (double)(cycles_total_full/CLK_PROC);
	debit_total_full = (WIDTH*HEIGHT) / time_total_full;
	time_total_full *= 1000;

	BENCH(printf("Total full :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_full)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_full/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_full)); BENCH(puts("")); BENCH(puts(""));	

	// FULL OPTI
	CHRONO(SigmaDelta_simd_full_opti(vmi0b, vmi1b, vmj0b, vmj1b, image, mean0, std0, img_bin), cycles_total_full_opti);
	time_total_full_opti = (double)(cycles_total_full_opti/CLK_PROC);
	debit_total_full_opti = (WIDTH*HEIGHT) / time_total_full_opti;
	time_total_full_opti *= 1000;

	BENCH(printf("Total full optimisé :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total_full_opti)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total_full_opti/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total_full_opti)); BENCH(puts("")); BENCH(puts(""));

	/*---------------------------------------------------*/

	// ---------- //
	// -- free -- //
	// ---------- //

	free_vui8matrix(image, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(mean0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(mean1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(std0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(std1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(img_diff, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(img_bin, vmi0b, vmi1b, vmj0b, vmj1b);
}

void bench_mouvement_SIMD_dataset(){

	// img reel size
	int width  = 320; // correspond au nb de colonne  => indice boucle j
	int height = 240; // correspond au nb de ligne    => indice boucle i

	// chronometrie
	int iter, niter = 4;
	int run, nrun = 5;
	double t0, t1, dt, tmin, t;

	char *format = "%d ";

    // calcul cpp
	double cycles_dataset, cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4;

	// calcul temps
	double time_dataset, time_total, time_step1, time_step2, time_step3, time_step4;

	// calcul debit
	double debit_dataset, debit_total;

	// taille noyau de convolution	
    int kernel_size = 3;

	puts("========================================");
	puts("=== benchmark mouvement dataset SIMD ===");
	puts("========================================");

	// BORD
	// 1 for 3x3 
	int b = 1; 

	// 2 for 5x5
	//int b = 2;

	// cardinalité des registres
	int card = card_vuint8(); // 16

	/*---------------------------------------------------*/

	// ------------------------- //
	// -- calculs des indices -- //
	// ------------------------- //

	// indices scalaires matrices
	int mi0 = 0; int mi1 = height-1;
	int mj0 = 0; int mj1 = width-1;

	// indices scalaires matrices avec bord
	int mi0b = mi0-b; int mi1b = mi1+b;
	int mj0b = mj0-b; int mj1b = mj1+b;

	// indices vectoriels matrices
	int vmi0 = 0; int vmi1 = (height)-1;
	int vmj0 = 0; int vmj1 = (width/card)-1;

	// indices vectoriels matrices avec bord
	int vmi0b = vmi0-b; int vmi1b = vmi1+b;
	int vmj0b = vmj0-1; int vmj1b = vmj1+1;

	// images
	vuint8** image = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	// moyennes
	vuint8** mean0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	vuint8** mean1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);		

	// ecart-types
	vuint8** std0 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	vuint8** std1 = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);			

	// image de différence
	vuint8 ** img_diff = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);	

	// image binaire (sortie)
	vuint8 ** img_bin = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	/*---------------------------------------------------*/

	// -------------- //
    // -- prologue -- //
    // -------------- //

	uint8 ** img_temp = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	MLoadPGM_ui8matrix("../car3/car_3000.pgm", mi0b, mi1b, mj0b, mj1b, img_temp);

	duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);

	// transfert ui8matrix à vui8matrix init
	ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp, image);

	// initiate mean0 et std0 for first iteration
	for (int i = vmi0b; i <= vmi1b; ++i)
	{
		for (int j = vmj0b; j <= vmj1b; ++j)
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

		MLoadPGM_ui8matrix(filename, mi0b, mi1b, mj0b, mj1b, img_temp);

		duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp, image);

		/*---------------------------------------------------*/

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		CHRONO(SigmaDelta_step1_simd(vmi0b, vmi1b, vmj0b, vmj1b, mean0, mean1, image), cycles_step1);
		time_step1 = (double)(cycles_step1/CLK_PROC);
		time_step1 *= 1000;

		CHRONO(SigmaDelta_step2_simd(vmi0b, vmi1b, vmj0b, vmj1b, image, mean1, img_diff), cycles_step2);
		time_step2 = (double)(cycles_step2/CLK_PROC);
		time_step2 *= 1000;

		CHRONO(SigmaDelta_step3_simd(vmi0b, vmi1b, vmj0b, vmj1b, std0, std1, img_diff), cycles_step3);
		time_step3 = (double)(cycles_step3/CLK_PROC);
		time_step3 *= 1000;

		CHRONO(SigmaDelta_step4_simd( vmi0b, vmi1b, vmj0b, vmj1b, std1, img_diff, img_bin), cycles_step4);
		time_step4 = (double)(cycles_step4/CLK_PROC);
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

	/*---------------------------------------------------*/

	// ---------- //
	// -- free -- //
	// ---------- //

	free_vui8matrix(image, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(mean0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(mean1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(std0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(std1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(img_diff, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(img_bin, vmi0b, vmi1b, vmj0b, vmj1b);
}

void bench_mouvement_SIMD_graphic(){

	// init fichier csv
	FILE* fichier_csv = fopen("csv_files/perf_SigmaDelta_SIMD.csv","w");
	fprintf(fichier_csv, "%s;;;;;%s\n", "Sigma Delta", "Sigma Delta Optimise");
	fprintf(fichier_csv, "%s;%s;%s;%s;", "Taille (pixels)", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
	fprintf(fichier_csv, ";%s;%s;%s\n", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");

	// BORD
	int b;

	int mi0, mi1, mj0, mj1; 	// indices scalaire
	int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord


	int vmi0, vmi1, vmj0, vmj1;
	int vmi0b, vmi1b, vmj0b, vmj1b;

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

    char *format = "%d ";

    // cardinalité des registres
	int card = card_vuint8(); // 16

    // calcul cpp
	double cycles_dataset, cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4, cycles_total_full_opti;

	// calcul temps
	double time_total, time_step1, time_step2, time_step3, time_step4, time_total_full_opti;

	// calcul debit
	double debit_total, debit_step1, debit_step2, debit_step3, debit_step4, debit_total_full_opti;

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
    	printf("\ni = %d\n", i);

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

		// indices vectoriels matrices
		vmi0 = 0; vmi1 = (height)-1;
		vmj0 = 0; vmj1 = (width/card)-1;

		// indices vectoriels matrices avec bord
		vmi0b = vmi0-b; vmi1b = vmi1+b;
		vmj0b = vmj0-1; vmj1b = vmj1+1;

		// DEBUG(printf("mi0b = %d, mi1b = %d, mj0b = %d, mj1b = %d\n", mi0b, mi1b, mj0b, mj1b));
		// DEBUG(printf("vmi0b = %d, vmi1b = %d, vmj0b = %d, vmj1b = %d\n", vmi0b, vmi1b, vmj0b, vmj1b));

		// ---------------- //
	    // -- allocation -- //
	    // ---------------- //

	    uint8 ** image_init 	= ui8matrix(mi0b, mi1b, mj0b, mj1b);
	   	uint8 ** img_temp 		= ui8matrix(mi0b, mi1b, mj0b, mj1b);

	   	// images
		vuint8** image 			= vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

		// moyennes
		vuint8** mean0 			= vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
		vuint8** mean1 			= vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);		

		// ecart-types
		vuint8** std0 			= vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
		vuint8** std1 			= vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);			

		// image de différence
		vuint8 ** img_diff 		= vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);	

		// image binaire (sortie)
		vuint8 ** img_bin 		= vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	   	for (int i = mi0b; i <= mi1b; ++i)
	   	{
	   		for (int j = mj0b; j <= mj1b; ++j)
	   		{
	   			// Générer des valeurs de pixels avec un motif qui evoluent avec la taille
	   			// Génére 2 images censé etre tres similaire (seul quelques pixels doivent varier)
	   			image_init [i][j]   	= (i + j)*10 + j;
	   			img_temp[i][j]  		= (i + j)*10 + j + i; 
	   		}
	   	}

	   	duplicate_border(mi0, mi1, mj0, mj1, b, image_init);
	   	duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, image_init, image);

	   	// initiate mean0 et std0 for first iteration
		for (int i = vmi0b; i <= vmi1b; ++i)
		{
			for (int j = vmj0b; j <= vmj1b; ++j)
			{
				mean0[i][j] = image[i][j];
				std0[i][j]  = init_vuint8(VMIN);
			}
		}

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp, image);

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		CHRONO(SigmaDelta_step1_simd(vmi0b, vmi1b, vmj0b, vmj1b, mean0, mean1, image), cycles_step1);
		time_step1 = (double)(cycles_step1/CLK_PROC);
		debit_step1 = (WIDTH*HEIGHT) / time_step1;

		CHRONO(SigmaDelta_step2_simd(vmi0b, vmi1b, vmj0b, vmj1b, image, mean1, img_diff), cycles_step2);
		time_step2 = (double)(cycles_step2/CLK_PROC);
		debit_step2 = (WIDTH*HEIGHT) / time_step2;
		
		CHRONO(SigmaDelta_step3_simd(vmi0b, vmi1b, vmj0b, vmj1b, std0, std1, img_diff), cycles_step3);
		time_step3 = (double)(cycles_step3/CLK_PROC);
		debit_step3 = (WIDTH*HEIGHT) / time_step3;

		CHRONO(SigmaDelta_step4_simd( vmi0b, vmi1b, vmj0b, vmj1b, std1, img_diff, img_bin), cycles_step4);
		time_step4 = (double)(cycles_step4/CLK_PROC);
		debit_step4 = (WIDTH*HEIGHT) / time_step4;

		cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
		time_total   = time_step1   + time_step2   + time_step3   + time_step4;
		debit_total  = (WIDTH*HEIGHT) / time_total;

		// FULL OPTI
		CHRONO(SigmaDelta_simd_full_opti(vmi0b, vmi1b, vmj0b, vmj1b, image, mean0, std0, img_bin), cycles_total_full_opti);
		time_total_full_opti = (double)(cycles_total_full_opti/CLK_PROC);
		debit_total_full_opti = (WIDTH*HEIGHT) / time_total_full_opti;

		// ecrire les donnees dans un fichier csv
		fprintf(fichier_csv, "%d;", height);
		fprintf(fichier_csv, "%f;", time_total*1000);
		fprintf(fichier_csv, "%f;", cycles_total/(height*width));
		fprintf(fichier_csv, "%f;", debit_total);

		fprintf(fichier_csv, ";%f;", time_total_full_opti*1000);
		fprintf(fichier_csv, "%f;", cycles_total_full_opti/(height*width));
		fprintf(fichier_csv, "%f\n", debit_total_full_opti);

		// ---------- //
		// -- free -- //
		// ---------- //

		free_vui8matrix(image, vmi0b, vmi1b, vmj0b, vmj1b);

		free_vui8matrix(mean0, vmi0b, vmi1b, vmj0b, vmj1b);
		free_vui8matrix(mean1, vmi0b, vmi1b, vmj0b, vmj1b);

		free_vui8matrix(std0, vmi0b, vmi1b, vmj0b, vmj1b);
		free_vui8matrix(std1, vmi0b, vmi1b, vmj0b, vmj1b);

		free_vui8matrix(img_diff, vmi0b, vmi1b, vmj0b, vmj1b);
		free_vui8matrix(img_bin, vmi0b, vmi1b, vmj0b, vmj1b);
    }

    fclose(fichier_csv);
}

void main_bench_mouvement_SIMD(int argc, char *argv[]){

	// benchmark unitaire sur petite images test generer
	// bench_mouvement_SIMD_car(true);

	// benchmark unitaire sur image du set
	bench_mouvement_SIMD_car(false);

	// benchmark global sur tout le dataset
	// bench_mouvement_SIMD_dataset();

	// benchmark sur images generees pour fichier csv
	//bench_mouvement_SIMD_graphic();
}
