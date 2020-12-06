/* ------------------------------------------------------------------------------ */
/* ---  Bench Algorithme Sigma Delta + Morpho SIMD pour le traitement d'image --- */
/* ------------------------------------------------------------------------------ */

#include "bench_mouvement_morpho_SIMD.h"


void bench_mouvement_morpho_SIMD_car(bool is_visual){

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
	double cycles_total, cycles_step1, cycles_step2, cycles_step3, cycles_step4, cycles_morpho;

	// calcul temps
	double time_total, time_step1, time_step2, time_step3, time_step4, time_morpho;

	// calcul debit
	double debit_total, debit_step1, debit_step2, debit_step3, debit_step4, debit_morpho;


	puts("==================================================");
	puts("=== benchmark mouvement + morpho unitaire SIMD ===");
	puts("==================================================");

	// BORD
	// 1 for 3x3 
	int b = 1; 

	// 2 for 5x5
	//int b = 2;

	// cardinalité des registres
	int card = card_vuint8(); // 16

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

	// image filtrée après morpho
	vuint8 ** img_filtered = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	vuint8** tmp1_SIMD	= vui8matrix(mi0b, mi1b, mj0b, mj1b);
	vuint8** tmp2_SIMD	= vui8matrix(mi0b, mi1b, mj0b, mj1b);

	/*---------------------------------------------------*/

	DEBUG(puts("================================"));
	DEBUG(puts("=== chargement et conversion ==="));
	DEBUG(puts("================================"));

	// ------------------------------ //
	// -- chargement et conversion -- //
	// ------------------------------ //

	uint8 ** img_temp = ui8matrix(mi0b, mi1b, mj0b, mj1b);

	if (is_visual)
    {
    	// gen img visuel
		// gen_pgm_img_simd();

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

		DEBUG(printf("Before duplicate_border : \n")); DEBUG(puts(""));
		DEBUG(display_ui8matrix(img_temp, mi0b, mi1b, mj0b, mj1b, "%d ", "img_temp : ")); DEBUG(puts(""));

		duplicate_border(mi0, mi1, mj0, mj1, b, img_temp);

		DEBUG(printf("After duplicate_vborder : \n")); DEBUG(puts(""));
		DEBUG(display_ui8matrix(img_temp, mi0b, mi1b, mj0b, mj1b, "%d ", "img_temp : ")); DEBUG(puts(""));
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

	DEBUG(printf("After conversion : \n"));DEBUG(puts(""));

	/*---------------------------------------------------*/

	DEBUG(puts("==================="));
	DEBUG(puts("=== traitements ==="));
	DEBUG(puts("==================="));

	// ----------------- //
	// -- traitements -- //
	// ----------------- //

	BENCH(printf("Sigma Delta :\n\n"));

	CHRONO(SigmaDelta_step1_simd(vmi0b, vmi1b, vmj0b, vmj1b, mean0, mean1, image), cycles_step1);
	time_step1 = (double)(cycles_step1/CLK_PROC);
	debit_step1 = (width*height) / time_step1;
	time_step1 *= 1000;

	BENCH(printf("step 1 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step1)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step1/(WIDTH*HEIGHT))); BENCH(puts("")); 
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step1)); BENCH(puts("")); BENCH(puts(""));

	CHRONO(SigmaDelta_step2_simd(vmi0b, vmi1b, vmj0b, vmj1b, image, mean1, img_diff), cycles_step2);
	time_step2 = (double)(cycles_step2/CLK_PROC);
	debit_step2 = (width*height) / time_step2;
	time_step2 *= 1000;

	BENCH(printf("step 2 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step2)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step2/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step2)); BENCH(puts("")); BENCH(puts(""));

	CHRONO(SigmaDelta_step3_simd(vmi0b, vmi1b, vmj0b, vmj1b, std0, std1, img_diff), cycles_step3);
	time_step3 = (double)(cycles_step3/CLK_PROC);
	debit_step3 = (width*height) / time_step3;
	time_step3 *= 1000;

	BENCH(printf("step 3 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step3)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step3/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step3)); BENCH(puts("")); BENCH(puts(""));

	
	CHRONO(SigmaDelta_step4_simd( vmi0b, vmi1b, vmj0b, vmj1b, std1, img_diff, img_bin), cycles_step4);
	time_step4 = (double)(cycles_step4/CLK_PROC);
	debit_step4 = (width*height) / time_step4;
	time_step4 *= 1000;

	BENCH(printf("step 4 :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_step4)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_step4/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_step4)); BENCH(puts("")); BENCH(puts(""));

	CHRONO(morpho_3_SIMD(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
	time_morpho = (double)(cycles_morpho/CLK_PROC);
	debit_morpho = (width*height) / time_morpho;
	time_morpho *= 1000;

	BENCH(printf("Morphologie :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));

	cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4 + cycles_morpho;
	time_total   = time_step1   + time_step2   + time_step3   + time_step4 + time_morpho;
	debit_total  = (width*height) / time_total;;

	BENCH(printf("Total :")); BENCH(puts(""));
	BENCH(printf("temps (ms) \t    = %0.6f", time_total)); BENCH(puts(""));
	BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total/(WIDTH*HEIGHT))); BENCH(puts(""));
	BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total)); BENCH(puts("")); BENCH(puts(""));

	/*---------------------------------------------------*/

	DEBUG(puts("============"));
	DEBUG(puts("=== free ==="));
	DEBUG(puts("============"));

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
	free_vui8matrix(img_filtered, vmi0b, vmi1b, vmj0b, vmj1b);
}

void bench_mouvement_morpho_SIMD_dataset(){

	// img reel size
	int width  = 320; // correspond au nb de colonne  => indice boucle j
	int height = 240; // correspond au nb de ligne    => indice boucle i

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
	double debit_dataset, debit_total;

	// taille noyau de convolution	
    int kernel_size = 3;

	puts("=================================================");
	puts("=== benchmark mouvement + morpho dataset SIMD ===");
	puts("=================================================");

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

	// image filtrée après morpho
	vuint8 ** img_filtered = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	vuint8** tmp1_SIMD	= vui8matrix(mi0b, mi1b, mj0b, mj1b);
	vuint8** tmp2_SIMD	= vui8matrix(mi0b, mi1b, mj0b, mj1b);

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

		CHRONO(morpho_3_SIMD(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD,vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
		time_morpho = (double)(cycles_morpho/CLK_PROC);
		time_morpho *= 1000;

		cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4 + cycles_morpho;
		time_total   = time_step1   + time_step2   + time_step3   + time_step4 + time_morpho;
		debit_total  = (width*height) / time_total;

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
	free_vui8matrix(img_filtered, vmi0b, vmi1b, vmj0b, vmj1b);
}

void bench_mouvement_morpho_SIMD_graphic(){

	// init fichier csv
	FILE* fichier_csv = fopen("csv_files/perf_mouvement_morpho_SIMD.csv","w");
	fprintf(fichier_csv, ";%s;;;;%s\n", "Traitement SIMD", "Traitement SIMD OPTI");
	fprintf(fichier_csv, "%s;%s;%s;%s;", "Taille (pixels)", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
	fprintf(fichier_csv, "%s;%s;%s\n", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");

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
	double cycles_total, cycles_total_full_opti, cycles_step1, cycles_step2, cycles_step3, cycles_step4, cycles_morpho_3, cycles_morpho_3_opti;
	// calcul temps
	double time_total, time_total_full_opti, time_step1, time_step2, time_step3, time_step4, time_morpho_3, time_morpho_3_opti;

	// calcul debit
	double debit_total, debit_total_full_opti, debit_morpho_3, debit_morpho_3_opti;

    puts("=========================================");
	puts("=== benchmark mouvement SIMD graphics ===");
	puts("=========================================");

    // 1 for 3x3 / 2 for 5x5
    b = 2; 
    
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
        vuint8 ** tmp1_SIMD     = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
        vuint8 ** tmp2_SIMD     = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
        vuint8 ** tmp3_SIMD     = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
		// image binaire (sortie)
		vuint8 ** img_bin 		= vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
        vuint8 ** img_filtered  = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

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

	   			image_init [i][j]   	= val1;
	   			img_temp[i][j]  		= val2; 
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

		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_temp, image);

		CHRONO(SigmaDelta_step1_simd(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image), cycles_step1);
		time_step1 = (double)(cycles_step1/CLK_PROC);

		CHRONO(SigmaDelta_step2_simd(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff), cycles_step2);
		time_step2 = (double)(cycles_step2/CLK_PROC);
		
		CHRONO(SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff), cycles_step3);
		time_step3 = (double)(cycles_step3/CLK_PROC);

		CHRONO(SigmaDelta_step4_simd( vmi0, vmi0, vmj0, vmj1, std1, img_diff, img_bin), cycles_step4);
		time_step4 = (double)(cycles_step4/CLK_PROC);

       	duplicate_vborder(vmi0, vmi1, vmj0, vmj1, b, img_bin);
		
	    CHRONO(morpho_3_SIMD(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho_3);
        time_morpho_3 = (double)(cycles_morpho_3/CLK_PROC);
		

		cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4 + cycles_morpho_3;
		time_total   = time_step1   + time_step2   + time_step3   + time_step4 + time_morpho_3;
		debit_total  = (width*height) / time_total;

		fprintf(fichier_csv, "%d;", height);
		fprintf(fichier_csv, "%f;", time_total*1000);
		fprintf(fichier_csv, "%f;", cycles_total/(height*width));
		fprintf(fichier_csv, "%f;", debit_total);

		// FULL OPTI
		CHRONO(SigmaDelta_simd_full_opti(vmi0, vmi1, vmj0, vmj1, image, mean0, mean1, std0, std1, img_bin), cycles_total_full_opti);
		time_total_full_opti = (double)(cycles_total_full_opti/CLK_PROC);

		duplicate_vborder(vmi0, vmi1, vmj0, vmj1, b, img_bin);
		CHRONO(morpho_3_SIMD_opti(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho_3_opti);
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
		free_ui8matrix(img_temp, mi0, mi1, mj0, mj1);
		free_vui8matrix(image, vmi0, vmi1, vmj0, vmj1);
		
		free_vui8matrix(mean0, vmi0, vmi1, vmj0, vmj1);
		free_vui8matrix(mean1, vmi0, vmi1, vmj0, vmj1);
		
		free_vui8matrix(std0, vmi0, vmi1, vmj0, vmj1);
		free_vui8matrix(std1, vmi0, vmi1, vmj0, vmj1);
		
		free_vui8matrix(img_diff, vmi0, vmi1, vmj0, vmj1);
		
		free_vui8matrix(img_bin, vmi0, vmi1, vmj0, vmj1);

		free_vui8matrix(img_filtered, vmi0, vmi1, vmj0, vmj1);
		free_vui8matrix(tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
		free_vui8matrix(tmp2_SIMD, vmi0, vmi1, vmj0, vmj1);
		free_vui8matrix(tmp3_SIMD, vmi0, vmi1, vmj0, vmj1);
    }

    fclose(fichier_csv);
}

void main_bench_mouvement_morpho_SIMD(int argc, char *argv[]){

	// Affiche les métriques de performance

	// benchmark unitaire sur petite images test generer
	// bench_mouvement_morpho_SIMD_car(true);

	// benchmark unitaire sur image du set
	// bench_mouvement_morpho_SIMD_car(false);
	   bench_mouvement_morpho_SIMD_graphic();
	// benchmark global sur tout le dataset
	// bench_mouvement_morpho_SIMD_dataset();
}