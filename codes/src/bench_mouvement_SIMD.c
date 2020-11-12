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

	// calcul temps
	double time_total, time_step1, time_step2, time_step3, time_step4;

	// calcul debit
	double debit_total, debit_step1, debit_step2, debit_step3, debit_step4;

	// taille noyau de convolution	
    int kernel_size = 3;

	puts("=========================================");
	puts("=== benchmark mouvement unitaire SIMD ===");
	puts("=========================================");

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

	cycles_total = cycles_step1 + cycles_step2 + cycles_step3 + cycles_step4;
	time_total   = time_step1   + time_step2   + time_step3   + time_step4;
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

void main_bench_mouvement_SIMD(int argc, char *argv[]){

	// benchmark unitaire sur petite images test generer
	// bench_mouvement_SIMD_car(true);

	// benchmark unitaire sur image du set
	bench_mouvement_SIMD_car(false);

	// benchmark global sur tout le dataset
	// bench_mouvement_SIMD_dataset();
}