
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

	int b; 

    switch(MORPHO){

        case MORPHO3:
        case EROSION3:
        case DILATATION3:
            b = 1; 
            break;

        case MORPHO5 :
        case EROSION5 :
        case DILATATION5 :
            b = 2; 
            break;

        default : 
            break;
    }

	puts("======================================");
	puts("=== benchmark morpho unitaire SIMD ===");
	puts("=======================================");


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
	vuint8** image                 = vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// moyennes
	vuint8** mean0                 = vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** mean1                 = vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// ecart-types
	vuint8** std0                  = vui8matrix(vmi0, vmi1, vmj0, vmj1);
	vuint8** std1                  = vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// image de différence
	vuint8 ** img_diff             = vui8matrix(vmi0, vmi1, vmj0, vmj1);

	// image binaire (sortie)
	vuint8 ** img_bin              = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	// image filtrée après morpho
    vuint8 ** img_filtered         = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

    // matrice temporaires
	vuint8** tmp1_SIMD	           = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
	vuint8** tmp2_SIMD	           = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);
    vuint8** tmp3_SIMD             = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	/*---------------------------------------------------*/

	DEBUG(puts("================================"));
	DEBUG(puts("=== chargement et conversion ==="));
	DEBUG(puts("================================"));

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

	DEBUG(puts("==================="));
	DEBUG(puts("=== traitements ==="));
	DEBUG(puts("==================="));

	SigmaDelta_step1_simd(vmi0, vmi1, vmj0, vmj1, mean0, mean1, image);
	SigmaDelta_step2_simd(vmi0, vmi1, vmj0, vmj1, image, mean1, img_diff);
	SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0, std1, img_diff);
	SigmaDelta_step4_simd(vmi0, vmi1, vmj0, vmj1, std1, img_diff, img_bin);

    duplicate_vborder(mi0, mi1, mj0, mj1, b, img_bin);

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

    else if(OPTI == SIMD_OPTI) {
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
                    BENCH(printf("====== Morphologie 3 fractionnée : ======")); BENCH(puts(""));
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

    else if (OPTI == PIPELINE){
        switch(MORPHO){

            case MORPHO3:

                CHRONO(morpho_3_SIMD_pipeline(img_bin, tmp1_SIMD, tmp2_SIMD, tmp3_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
                time_morpho = (double)(cycles_morpho/CLK_PROC);
                debit_morpho = (WIDTH*HEIGHT) / time_morpho;
                time_morpho *= 1000;
                BENCH(printf("Morphologie 3 Pipeline:")); BENCH(puts(""));  
                BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));
                break;
        }
    }

    else if (OPTI == PIPELINE_OPTI){
        switch(MORPHO){

            case MORPHO5:

                CHRONO(morpho_3_SIMD_pipeline_opti(img_bin, tmp1_SIMD, tmp2_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
                time_morpho = (double)(cycles_morpho/CLK_PROC);
                debit_morpho = (WIDTH*HEIGHT) / time_morpho;
                time_morpho *= 1000;
                BENCH(printf("Morphologie 3 Pipeline Optimisé:")); BENCH(puts(""));  
                BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));
                break;
        }
    }

    else if(OPTI == SIMD_OMP) {

        switch(MORPHO){
            case EROSION3 :
                CHRONO(erosion_3_SIMD_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 3 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_3_SIMD_omp(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1);
                dilatation_3_SIMD_omp(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_3_SIMD_omp(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case DILATATION3 :
                erosion_3_SIMD_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
                CHRONO(dilatation_3_SIMD_omp(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_3_SIMD_omp(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_3_SIMD_omp(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case MORPHO3 :
                if(fract){
                    BENCH(printf("====== Morphologie 3 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_3_SIMD_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_SIMD_omp(tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_SIMD_omp(tmp2_SIMD, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_3_SIMD_omp(tmp1_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_erosion_2); 
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
                    BENCH(printf("====== Morphologie 3 OMP : ======")); BENCH(puts(""));
                    CHRONO(morpho_3_SIMD_omp(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
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
                CHRONO(erosion_5_SIMD_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 5 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_SIMD_omp(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1);
                dilatation_5_SIMD_omp(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_5_SIMD_omp(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case DILATATION5 :
                erosion_5_SIMD_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
                CHRONO(dilatation_5_SIMD_omp(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_SIMD_omp(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_5_SIMD_omp(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case MORPHO5 :
                if(fract){
                    BENCH(printf("====== Morphologie 5 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_5_SIMD_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_SIMD_omp(tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_SIMD_omp(tmp2_SIMD, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_5_SIMD_omp(tmp1_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_erosion_2); 
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
                    CHRONO(morpho_5_SIMD_omp(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
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

    else if (OPTI == SIMD_OPTI_OMP){
        switch(MORPHO){
            case EROSION3 :
                CHRONO(erosion_3_SIMD_opti_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 3 Déroulée:")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_3_SIMD_opti_omp(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1);
                dilatation_3_SIMD_opti_omp(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_3_SIMD_opti_omp(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case DILATATION3 :
                erosion_3_SIMD_opti_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
                CHRONO(dilatation_3_SIMD_opti_omp(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000;
                BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts("")); 
                dilatation_3_SIMD_opti_omp(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_3_SIMD_opti_omp(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case MORPHO3 :
                if(fract){
                    BENCH(printf("====== Morphologie 3 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_3_SIMD_opti_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_SIMD_opti_omp(tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_SIMD_opti_omp(tmp2_SIMD, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_3_SIMD_opti_omp(tmp1_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_erosion_2); 
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
                    CHRONO(morpho_3_SIMD_opti_omp(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
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
                CHRONO(erosion_5_SIMD_opti_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 5 Déroulée :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_SIMD_opti_omp(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1);
                dilatation_5_SIMD_opti_omp(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_5_SIMD_opti_omp(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case DILATATION5 :
                erosion_5_SIMD_opti_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
                CHRONO(dilatation_5_SIMD_opti_omp(tmp1_SIMD, tmp2_SIMD,  vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                BENCH(printf("Dilatation 5 Déroulée :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_SIMD_opti_omp(tmp2_SIMD, tmp1_SIMD,  vmi0, vmi1, vmj0, vmj1);
                erosion_5_SIMD_opti_omp(tmp1_SIMD, img_filtered,  vmi0, vmi1, vmj0, vmj1); 
                break;
            case MORPHO5 :
                if(fract){
                    BENCH(printf("====== Morphologie 5 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_5_SIMD_opti_omp(img_bin, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_SIMD_opti_omp(tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_SIMD_opti_omp(tmp2_SIMD, tmp1_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_5_SIMD_opti_omp(tmp1_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_erosion_2); 
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
                    CHRONO(morpho_5_SIMD_opti_omp(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
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

    else if (OPTI == PIPELINE_OMP){

        switch(MORPHO){
            case MORPHO3:

                CHRONO(morpho_3_SIMD_pipeline_omp(img_bin, tmp1_SIMD, tmp2_SIMD, tmp3_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
                time_morpho = (double)(cycles_morpho/CLK_PROC);
                debit_morpho = (WIDTH*HEIGHT) / time_morpho;
                time_morpho *= 1000;
                BENCH(printf("Morphologie 3 Pipeline OMP:")); BENCH(puts(""));  
                BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));
                break;
        }
    }

    else if (OPTI == PIPELINE_OPTI_OMP){
        switch(MORPHO){
            case MORPHO5:

                CHRONO(morpho_3_SIMD_pipeline_opti_omp(img_bin, tmp1_SIMD, tmp2_SIMD, img_filtered, vmi0, vmi1, vmj0, vmj1), cycles_morpho); 
                time_morpho = (double)(cycles_morpho/CLK_PROC);
                debit_morpho = (WIDTH*HEIGHT) / time_morpho;
                time_morpho *= 1000;
                BENCH(printf("Morphologie 3 Pipeline Optimisé OMP:")); BENCH(puts(""));  
                BENCH(printf("temps (ms) \t    = %0.6f", time_morpho)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_morpho/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_morpho)); BENCH(puts("")); BENCH(puts(""));
                break;
        }
    }


	/*---------------------------------------------------*/

	DEBUG(puts("============"));
	DEBUG(puts("=== free ==="));
	DEBUG(puts("============"));


	free_vui8matrix(image, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(mean0, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(mean1, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(std0, vmi0, vmi1, vmj0, vmj1);
	free_vui8matrix(std1, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(img_diff, vmi0, vmi1, vmj0, vmj1);

	free_vui8matrix(img_bin, vmi0b, vmi1b, vmj0b, vmj1b);

    // free_vui8matrix(img_filtered, vmi0b, vmi1b, vmj0b, vmj1b);

    free_vui8matrix(tmp1_SIMD, vmi0b, vmi1b, vmj0b, vmj1b);
    free_vui8matrix(tmp2_SIMD, vmi0b, vmi1b, vmj0b, vmj1b);

    free_vui8matrix(tmp3_SIMD, vmi0b, vmi1b, vmj0b, vmj1b);
}

void bench_morpho_SIMD_graphic(){

	// init fichier csv
	FILE* fichier_csv = fopen("csv_files/perf_morpho_SIMD.csv","w");
	fprintf(fichier_csv, ";%s;;;;;%s;;;;;%s;;;;;%s;;;;;%s;;;;;%s;;;;;%s;;;;;%s\n", "Morpho 3 SIMD", "Morpho 3 SIMD OMP", "Morpho 3 SIMD OPTI", "Morpho 3 SIMD OPTI OMP", "Morpho 3 SIMD PIPELINE", "Morpho 3 SIMD PIPELINE OMP", "Morpho 3 SIMD PIPELINE OPTI", "Morpho 3 SIMD PIPELINE OPTI OMP");
	fprintf(fichier_csv, "%s;%s;%s;%s;", "Taille (pixels)", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
	fprintf(fichier_csv, ";%s;%s;%s;", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
    fprintf(fichier_csv, ";%s;%s;%s;", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
    fprintf(fichier_csv, ";%s;%s;%s;", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
    fprintf(fichier_csv, ";%s;%s;%s;", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
    fprintf(fichier_csv, ";%s;%s;%s;", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
    fprintf(fichier_csv, ";%s;%s;%s;", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
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
	double cycles_dataset, cycles_morpho_3, cycles_morpho_3_opti, cycles_morpho_3_pipeline, cycles_morpho_3_pipeline_opti, cycles_morpho_3_omp, cycles_morpho_3_opti_omp, cycles_morpho_3_pipeline_omp, cycles_morpho_3_pipeline_opti_omp;

	// calcul temps
	double time_dataset, time_morpho_3, time_morpho_3_opti, time_morpho_3_pipeline, time_morpho_3_pipeline_opti, time_morpho_3_omp, time_morpho_3_opti_omp, time_morpho_3_pipeline_omp, time_morpho_3_pipeline_opti_omp;

	// calcul debit
	double debit_dataset, debit_morpho_3, debit_morpho_3_opti, debit_morpho_3_pipeline, debit_morpho_3_pipeline_opti, debit_morpho_3_omp, debit_morpho_3_opti_omp, debit_morpho_3_pipeline_omp, debit_morpho_3_pipeline_opti_omp;

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

		// transfert ui8matrix à vui8matrix init
		ui8matrix_to_vui8matrix(card, vmi0, vmi1, vmj0, vmj1, img_temp, image);

        SigmaDelta_step1_simd(vmi0, vmi1, vmj0, vmj1, mean0, mean1   , image);
        SigmaDelta_step2_simd(vmi0, vmi1, vmj0, vmj1, image, mean1   , img_diff);
        SigmaDelta_step3_simd(vmi0, vmi1, vmj0, vmj1, std0 , std1    , img_diff);
        SigmaDelta_step4_simd(vmi0, vmi1, vmj0, vmj1, std1 , img_diff, img_bin);

        duplicate_vborder(vmi0, vmi1, vmj0, vmj1, b, img_bin);
		// ----------------- //
	    // -- traitements -- //
	    // ----------------- //

        CHRONO(morpho_3_SIMD(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho_3);
        time_morpho_3 = (double)(cycles_morpho_3/CLK_PROC);
        debit_morpho_3 = width*height / time_morpho_3;

        CHRONO(morpho_3_SIMD_omp(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho_3_omp);
        time_morpho_3_omp = (double)(cycles_morpho_3_omp/CLK_PROC);
        debit_morpho_3_omp = width*height / time_morpho_3_omp;

        CHRONO(morpho_3_SIMD_opti(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho_3_opti);
        time_morpho_3_opti = (double)(cycles_morpho_3_opti/CLK_PROC);
        debit_morpho_3_opti = width*height / time_morpho_3_opti;

        CHRONO(morpho_3_SIMD_opti_omp(img_bin, img_filtered, tmp1_SIMD, tmp2_SIMD, vmi0, vmi1, vmj0, vmj1), cycles_morpho_3_opti_omp);
        time_morpho_3_opti_omp = (double)(cycles_morpho_3_opti_omp/CLK_PROC);
        debit_morpho_3_opti_omp = width*height / time_morpho_3_opti_omp;

        CHRONO(morpho_3_SIMD_pipeline(img_bin, tmp1_SIMD, tmp2_SIMD, tmp3_SIMD, img_filtered,vmi0, vmi1, vmj0, vmj1), cycles_morpho_3_pipeline);
        time_morpho_3_pipeline = (double)(cycles_morpho_3_pipeline/CLK_PROC);
        debit_morpho_3_pipeline = width*height / time_morpho_3_pipeline;

        CHRONO(morpho_3_SIMD_pipeline_omp(img_bin, tmp1_SIMD, tmp2_SIMD, tmp3_SIMD, img_filtered,vmi0, vmi1, vmj0, vmj1), cycles_morpho_3_pipeline_omp);
        time_morpho_3_pipeline_omp = (double)(cycles_morpho_3_pipeline_omp/CLK_PROC);
        debit_morpho_3_pipeline_omp = width*height / time_morpho_3_pipeline_omp;

        CHRONO(morpho_3_SIMD_pipeline_opti(img_bin, tmp1_SIMD, tmp2_SIMD, img_filtered,vmi0, vmi1, vmj0, vmj1), cycles_morpho_3_pipeline_opti);
        time_morpho_3_pipeline_opti = (double)(cycles_morpho_3_pipeline_opti/CLK_PROC);
        debit_morpho_3_pipeline_opti = width*height / time_morpho_3_pipeline_opti;

        CHRONO(morpho_3_SIMD_pipeline_opti_omp(img_bin, tmp1_SIMD, tmp2_SIMD, img_filtered,vmi0, vmi1, vmj0, vmj1), cycles_morpho_3_pipeline_opti_omp);
        time_morpho_3_pipeline_opti_omp = (double)(cycles_morpho_3_pipeline_opti_omp/CLK_PROC);
        debit_morpho_3_pipeline_opti_omp = width*height / time_morpho_3_pipeline_opti_omp;
		// ecrire les donnees dans un fichier csv
		fprintf(fichier_csv, "%d;", height);
		fprintf(fichier_csv, "%f;", time_morpho_3*1000);
		fprintf(fichier_csv, "%f;", cycles_morpho_3/(height*width));
		fprintf(fichier_csv, "%f;", debit_morpho_3);

		fprintf(fichier_csv, ";%f;", time_morpho_3_omp*1000);
		fprintf(fichier_csv, "%f;", cycles_morpho_3_omp/(height*width));
		fprintf(fichier_csv, "%f;", debit_morpho_3_omp);

        fprintf(fichier_csv, ";%f;", time_morpho_3_opti*1000);
		fprintf(fichier_csv, "%f;", cycles_morpho_3_opti/(height*width));
		fprintf(fichier_csv, "%f;", debit_morpho_3_opti);

        fprintf(fichier_csv, ";%f;", time_morpho_3_opti_omp*1000);
		fprintf(fichier_csv, "%f;", cycles_morpho_3_opti_omp/(height*width));
		fprintf(fichier_csv, "%f;", debit_morpho_3_opti_omp);

        fprintf(fichier_csv, ";%f;", time_morpho_3_pipeline*1000);
		fprintf(fichier_csv, "%f;", cycles_morpho_3_pipeline/(height*width));
		fprintf(fichier_csv, "%f;", debit_morpho_3_pipeline);

        fprintf(fichier_csv, ";%f;", time_morpho_3_pipeline_omp*1000);
		fprintf(fichier_csv, "%f;", cycles_morpho_3_pipeline_omp/(height*width));
		fprintf(fichier_csv, "%f;", debit_morpho_3_pipeline_omp);

        fprintf(fichier_csv, ";%f;", time_morpho_3_pipeline_opti*1000);
		fprintf(fichier_csv, "%f;", cycles_morpho_3_pipeline_opti/(height*width));
		fprintf(fichier_csv, "%f;", debit_morpho_3_pipeline_opti);

        fprintf(fichier_csv, ";%f;", time_morpho_3_pipeline_opti_omp*1000);
		fprintf(fichier_csv, "%f;", cycles_morpho_3_pipeline_opti_omp/(height*width));
		fprintf(fichier_csv, "%f\n;", debit_morpho_3_pipeline_opti_omp);

		// ---------- //
		// -- free -- //
		// ---------- //
		free_ui8matrix(image_init, mi0, mi1, mj0, mj1);
		free_ui8matrix(img_temp, mi0, mi1, mj0, mj1);
		free_vui8matrix(image, vmi0, vmi1, vmj0, vmj1);
		//printf("Bonjour !\n\n");
		free_vui8matrix(mean0, vmi0, vmi1, vmj0, vmj1);
		free_vui8matrix(mean1, vmi0, vmi1, vmj0, vmj1);
		//printf("Bonjour !\n\n");
		free_vui8matrix(std0, vmi0, vmi1, vmj0, vmj1);
		free_vui8matrix(std1, vmi0, vmi1, vmj0, vmj1);
		
		free_vui8matrix(img_diff, vmi0, vmi1, vmj0, vmj1);
		
		//free_vui8matrix(img_bin, vmi0, vmi1, vmj0, vmj1);
        
		//free_vui8matrix(img_filtered, vmi0, vmi1, vmj0, vmj1);
        //printf("Bonjour !\n\n");
		// free_vui8matrix(tmp1_SIMD, vmi0, vmi1, vmj0, vmj1);
        // printf("Bonjour !\n\n");
		// free_vui8matrix(tmp2_SIMD, vmi0, vmi1, vmj0, vmj1);
        // printf("Bonjour !\n\n");
		// free_vui8matrix(tmp3_SIMD, vmi0, vmi1, vmj0, vmj1);

    }

    fclose(fichier_csv);
}

void main_bench_morpho_SIMD(int argc, char *argv[]){

    
    // bench_morpho_SIMD_car(false, EROSION3, SIMD, 0);
    // bench_morpho_SIMD_car(false, EROSION5, SIMD, 0);
    // bench_morpho_SIMD_car(false, DILATATION3, SIMD, 0);
    // bench_morpho_SIMD_car(false, DILATATION5, SIMD, 0);
    // bench_morpho_SIMD_car(false, MORPHO3, SIMD, 0);
    // bench_morpho_SIMD_car(false, MORPHO3, SIMD, 1);
    // bench_morpho_SIMD_car(false, MORPHO5, SIMD, 0);
    // bench_morpho_SIMD_car(false, MORPHO5, SIMD, 1);

    // bench_morpho_SIMD_car(false, EROSION3, SIMD_OPTI, 0);
    // bench_morpho_SIMD_car(false, EROSION5, SIMD_OPTI, 0);
    // bench_morpho_SIMD_car(false, DILATATION3, SIMD_OPTI, 0);
    // bench_morpho_SIMD_car(false, DILATATION5, SIMD_OPTI, 0);
    // bench_morpho_SIMD_car(false, MORPHO3, SIMD_OPTI, 0);
    // bench_morpho_SIMD_car(false, MORPHO3, SIMD_OPTI, 1);
    // bench_morpho_SIMD_car(false, MORPHO5, SIMD_OPTI, 0);
    // bench_morpho_SIMD_car(false, MORPHO5, SIMD_OPTI, 1);
    // printf("===============================\n");
    // bench_morpho_SIMD_car(false, MORPHO3, PIPELINE, 0);
    // bench_morpho_SIMD_car(false, MORPHO5, PIPELINE_OPTI, 0);
    // printf("===============================\n");
    bench_morpho_SIMD_graphic();

}

