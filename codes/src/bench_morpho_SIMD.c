/* ------------------------------------------------------------------------------ */
/* ---  Bench Algorithme Morpho SIMD pour le traitement d'image --- */
/* ------------------------------------------------------------------------------ */

#include "bench_morpho_SIMD.h"

void bench_morpho_SIMD_car(bool is_visual, type_morpho_t MORPHO, type_opti_t OPTI, int fract){

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
	double cycles_total, cycles_erosion_1, cycles_dilatation_1, cycles_erosion_2, cycles_dilatation_2, cycles_morpho;

	// calcul temps
	double time_total, time_erosion_1, time_dilatation_1, time_erosion_2, time_dilatation_2, time_morpho;

	// calcul debit
	double debit_total, debit_erosion_1, debit_dilatation_1, debit_erosion_2, debit_dilatation_2, debit_morpho;

	// taille noyau de convolution	
    int kernel_size;
	int b; 
    switch(MORPHO){
        case MORPHO3:
        case EROSION3:
        case DILATATION3:
            kernel_size = 3;
            b = 1; 
            break;
        case MORPHO5 :
        case EROSION5 :
        case DILATATION5 :
            kernel_size = 5;
            b = 2; 
            break;
        default : 
            break;
    }

	puts("==================================================");
	puts("=== benchmark morpho unitaire SIMD ===");
	puts("==================================================");

	// BORD
	// 1 for 3x3 


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


	SigmaDelta_step1_simd(vmi0b, vmi1b, vmj0b, vmj1b, mean0, mean1, image);
	SigmaDelta_step2_simd(vmi0b, vmi1b, vmj0b, vmj1b, image, mean1, img_diff);
	SigmaDelta_step3_simd(vmi0b, vmi1b, vmj0b, vmj1b, std0, std1, img_diff);
	SigmaDelta_step4_simd( vmi0b, vmi1b, vmj0b, vmj1b, std1, img_diff, img_bin);

    if(OPTI == SIMD){
        switch(MORPHO){
            case EROSION3 :
                CHRONO(erosion_3_SIMD(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 3 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_3_SIMD(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1);
                dilatation_3_SIMD(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_3_SIMD(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case DILATATION3 :
                erosion_3_SIMD(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
                CHRONO(dilatation_3_SIMD(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_3_SIMD(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_3_SIMD(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case MORPHO3 :
                if(fract){
                    BENCH(printf("====== Morphologie 3 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_3_SIMD(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_SIMD(tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_SIMD(tmp2_SIMD, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_3_SIMD(tmp1_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_erosion_2); 
                    time_erosion_2 = (double)(cycles_erosion_2/CLK_PROC);
                    debit_erosion_2 = (WIDTH*HEIGHT) / time_erosion_2;
                    time_erosion_2 *= 1000;
                    BENCH(printf("Erosion 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_2)); BENCH(puts("")); BENCH(puts(""));
                    BENCH(printf("=========================================")); BENCH(puts(""));
                }
                else {
                    CHRONO(morpho_3_SIMD(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
                    time_morpho = (double)(cycles_morpho/CLK_PROC);
                    debit_morpho = (WIDTH*HEIGHT) / time_morpho;
                    time_morpho *= 1000;
                    BENCH(printf("Morphologie 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));
                }
                break;
            case EROSION5 :
                CHRONO(erosion_5_SIMD(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 5 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_SIMD(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1);
                dilatation_5_SIMD(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_5_SIMD(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case DILATATION5 :
                erosion_5_SIMD(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
                CHRONO(dilatation_5_SIMD(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_SIMD(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_5_SIMD(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case MORPHO5 :
                if(fract){
                    BENCH(printf("====== Morphologie 5 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_5_SIMD(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_SIMD(tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_SIMD(tmp2_SIMD, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_5_SIMD(tmp1_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_erosion_2); 
                    time_erosion_2 = (double)(cycles_erosion_2/CLK_PROC);
                    debit_erosion_2 = (WIDTH*HEIGHT) / time_erosion_2;
                    time_erosion_2 *= 1000;
                    BENCH(printf("Erosion 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_2)); BENCH(puts("")); BENCH(puts(""));
                    BENCH(printf("=========================================")); BENCH(puts(""));
                }
                else {
                    CHRONO(morpho_5_SIMD(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
                    time_morpho = (double)(cycles_morpho/CLK_PROC);
                    debit_morpho = (WIDTH*HEIGHT) / time_morpho;
                    time_morpho *= 1000;
                    BENCH(printf("Morphologie 5:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));
                }
                break;
            default :
                break;
        }
    }
    else {
        switch(MORPHO){
            case EROSION3 :
                CHRONO(erosion_3_SIMD_opti(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 3 Déroulée:")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_3_SIMD_opti(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1);
                dilatation_3_SIMD_opti(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_3_SIMD_opti(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case DILATATION3 :
                erosion_3_SIMD_opti(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
                CHRONO(dilatation_3_SIMD_opti(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000;
                BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts("")); 
                dilatation_3_SIMD_opti(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_3_SIMD_opti(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case MORPHO3 :
                if(fract){
                    BENCH(printf("====== Morphologie 5 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_3_SIMD_opti(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_SIMD_opti(tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_SIMD_opti(tmp2_SIMD, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_3_SIMD_opti(tmp1_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_erosion_2); 
                    time_erosion_2 = (double)(cycles_erosion_2/CLK_PROC);
                    debit_erosion_2 = (WIDTH*HEIGHT) / time_erosion_2;
                    time_erosion_2 *= 1000;
                    BENCH(printf("Erosion 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_2)); BENCH(puts("")); BENCH(puts(""));
                    BENCH(printf("=========================================")); BENCH(puts(""));
                }
                else {
                    CHRONO(morpho_3_SIMD_opti(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
                    time_morpho = (double)(cycles_morpho/CLK_PROC);
                    debit_morpho = (WIDTH*HEIGHT) / time_morpho;
                    time_morpho *= 1000;
                    BENCH(printf("Morphologie 3 Déroulée:")); BENCH(puts(""));  
                    BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));
                }
                break;
            case EROSION5 :
                CHRONO(erosion_5_SIMD_opti(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 5 Déroulée :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_SIMD_opti(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1);
                dilatation_5_SIMD_opti(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_5_SIMD_opti(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case DILATATION5 :
                erosion_5_SIMD_opti(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
                CHRONO(dilatation_5_SIMD_opti(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                BENCH(printf("Dilatation 5 Déroulée :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_SIMD_opti(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_5_SIMD_opti(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case MORPHO5 :
                if(fract){
                    BENCH(printf("====== Morphologie 5 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_5_SIMD_opti(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_SIMD_opti(tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_SIMD_opti(tmp2_SIMD, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_5_SIMD_opti(tmp1_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_erosion_2); 
                    time_erosion_2 = (double)(cycles_erosion_2/CLK_PROC);
                    debit_erosion_2 = (WIDTH*HEIGHT) / time_erosion_2;
                    time_erosion_2 *= 1000;
                    BENCH(printf("Erosion 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_2)); BENCH(puts("")); BENCH(puts(""));
                    cycles_total = cycles_erosion_1 + cycles_erosion_2 + cycles_dilatation_1 + cycles_dilatation_2;
                    time_total = time_erosion_1 + time_erosion_2 + time_dilatation_1 + time_dilatation_2;
                    debit_total  = (WIDTH*HEIGHT) / time_total;

                    BENCH(printf("Total :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_total)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_total/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_total)); BENCH(puts("")); BENCH(puts(""));
                    BENCH(printf("=========================================")); BENCH(puts(""));
                }
                else {
                    CHRONO(morpho_5_SIMD_opti(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
                    time_morpho = (double)(cycles_morpho/CLK_PROC);
                    debit_morpho = (WIDTH*HEIGHT) / time_morpho;
                    time_morpho *= 1000;
                    BENCH(printf("Morphologie 5 déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));
                }
                break;
            default :
                break;
        }
    }


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


void main_bench_morpho_SIMD(int argc, char *argv[]){

    printf("===============================\n");
    display_type_opti(SIMD);puts("");
    bench_morpho_SIMD_car(false, EROSION3, SIMD, 0);
    bench_morpho_SIMD_car(false, EROSION5, SIMD, 0);
    bench_morpho_SIMD_car(false, DILATATION3, SIMD, 0);
    bench_morpho_SIMD_car(false, DILATATION5, SIMD, 0);
    bench_morpho_SIMD_car(false, MORPHO3, SIMD, 0);
    bench_morpho_SIMD_car(false, MORPHO3, SIMD, 1);
    bench_morpho_SIMD_car(false, MORPHO5, SIMD, 0);
    bench_morpho_SIMD_car(false, MORPHO5, SIMD, 1);
    printf("===============================\n");
    display_type_opti(SIMD_OPTI);puts("");
    bench_morpho_SIMD_car(false, EROSION3, SIMD_OPTI, 0);
    bench_morpho_SIMD_car(false, EROSION5, SIMD_OPTI, 0);
    bench_morpho_SIMD_car(false, DILATATION3, SIMD_OPTI, 0);
    bench_morpho_SIMD_car(false, DILATATION5, SIMD_OPTI, 0);
    bench_morpho_SIMD_car(false, MORPHO3, SIMD_OPTI, 0);
    bench_morpho_SIMD_car(false, MORPHO3, SIMD_OPTI, 1);
    bench_morpho_SIMD_car(false, MORPHO5, SIMD_OPTI, 0);
    bench_morpho_SIMD_car(false, MORPHO5, SIMD_OPTI, 1);
    printf("===============================\n");
}