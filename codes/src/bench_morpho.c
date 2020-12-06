/* ------------------------------------------------------------------------- */
/* ---  Bench Algorithme Morpho pour le traitement d'image --- */
/* ------------------------------------------------------------------------- */

#include "bench_morpho.h"

//si fract = 1 alors la morpho est fractionnées entre toutes ses opérations
//si fract = 0 alors le benchmark est uniquement fait sur la morpho seule
void bench_morpho_car(bool is_visual, type_morpho_t MORPHO, type_opti_t OPTI, int fract){

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
	double cycles_total = 0, cycles_erosion_1 = 0, cycles_dilatation_1 = 0, cycles_erosion_2 = 0, cycles_dilatation_2 = 0, cycles_morpho = 0;

	// calcul temps
	double time_total = 0, time_erosion_1 = 0, time_dilatation_1 = 0, time_erosion_2 = 0, time_dilatation_2 = 0, time_morpho = 0;

	// calcul debit
	double debit_total = 0, debit_erosion_1 = 0, debit_dilatation_1 = 0, debit_erosion_2 = 0, debit_dilatation_2 = 0, debit_morpho = 0;

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

    puts("=============================================");
    puts("======== benchmark  morpho unitaire =========");
    puts("=============================================");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //    

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

	uint8** image 			= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** mean0 			= ui8matrix(mi0, mi1, mj0, mj1);
	uint8** mean1 			= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** std0 			= ui8matrix(mi0, mi1, mj0, mj1);
	uint8** std1 			= ui8matrix(mi0, mi1, mj0, mj1);

	uint8** img_diff 		= ui8matrix(mi0, mi1, mj0, mj1);

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
		MLoadPGM_ui8matrix("../car3/car_3012.pgm", mi0, mi1, mj0, mj1, image);

		// initiate mean0 et std0 for first iteration
		for (int i = mi0; i <= mi1; ++i)
		{
			for (int j = mj0; j <= mj1; ++j)
			{
				mean0[i][j] = image[i][j];
				std0[i][j]  = VMIN;
			}
		}

		MLoadPGM_ui8matrix("../car3/car_3013.pgm", mi0, mi1, mj0, mj1, image);
    }

	// ----------------- //
    // -- traitements -- //
    // ----------------- //

	SigmaDelta_step1(mi0, mi1, mj0, mj1, mean0, mean1, image);
	SigmaDelta_step2(mi0, mi1, mj0, mj1, image, mean1, img_diff);
    SigmaDelta_step3(mi0, mi1, mj0, mj1, std0 , std1, img_diff);
    SigmaDelta_step4(mi0, mi1, mj0, mj1, std1 , img_diff, img_bin);

    duplicate_border(mi0, mi1, mj0, mj1, b, img_bin);


    // BENCH 
    if(OPTI == SCALAIRE){
        
        switch(MORPHO){

            case EROSION3 :


                CHRONO(erosion_3(img_bin, tmp1, mi0, mi1, mj0, mj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 

                BENCH(printf("Erosion 3 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));

                dilatation_3(tmp1, tmp2,  mi0, mi1, mj0, mj1);
                dilatation_3(tmp2, tmp1,  mi0, mi1, mj0, mj1);
                erosion_3(tmp1, img_filtered,  mi0, mi1, mj0, mj1); 

                break;

            case DILATATION3 :
                
                erosion_3(img_bin, tmp1, mi0, mi1, mj0, mj1);
                
                CHRONO(dilatation_3(tmp1, tmp2,  mi0, mi1, mj0, mj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                
                BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));

                dilatation_3(tmp2, tmp1,  mi0, mi1, mj0, mj1);
                erosion_3(tmp1, img_filtered,  mi0, mi1, mj0, mj1); 

                break;

            case MORPHO3 :

                if(fract){
                    BENCH(printf("====== Morphologie 3 fractionnée : ======")); BENCH(puts(""));
                    
                    CHRONO(erosion_3(img_bin, tmp1, mi0, mi1, mj0, mj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    
                    BENCH(printf("Erosion 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));

                    CHRONO(dilatation_3(tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;

                    BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));

                    CHRONO(dilatation_3(tmp2, tmp1, mi0, mi1, mj0, mj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;

                    BENCH(printf("Dilatation 3 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));

                    CHRONO(erosion_3(tmp1, img_filtered, mi0, mi1, mj0, mj1), cycles_erosion_2); 
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
                    CHRONO(morpho_3(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho); 
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
                CHRONO(erosion_5(img_bin, tmp1, mi0, mi1, mj0, mj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 5 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5(tmp1, tmp2,  mi0, mi1, mj0, mj1);
                dilatation_5(tmp2, tmp1,  mi0, mi1, mj0, mj1);
                erosion_5(tmp1, img_filtered,  mi0, mi1, mj0, mj1); 
                break;

            case DILATATION5 :
                erosion_5(img_bin, tmp1, mi0, mi1, mj0, mj1);
                CHRONO(dilatation_5(tmp1, tmp2,  mi0, mi1, mj0, mj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5(tmp2, tmp1,  mi0, mi1, mj0, mj1);
                erosion_5(tmp1, img_filtered,  mi0, mi1, mj0, mj1); 
                break;

            case MORPHO5 :
                if(fract){
                    BENCH(printf("====== Morphologie 5 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_5(img_bin, tmp1, mi0, mi1, mj0, mj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5(tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5(tmp2, tmp1, mi0, mi1, mj0, mj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 5 :")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_5(tmp1, img_filtered, mi0, mi1, mj0, mj1), cycles_erosion_2); 
                    time_erosion_2 = (double)(cycles_erosion_2/CLK_PROC);
                    debit_erosion_2 = (WIDTH*HEIGHT) / time_erosion_2;
                    time_erosion_2 *= 1000;
                    BENCH(printf("Erosion 5 :")); BENCH(puts(""));
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
                    CHRONO(morpho_5(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho); 
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
                CHRONO(erosion_3_unroll(img_bin, tmp1, mi0, mi1, mj0, mj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 3 Déroulée:")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_3_unroll(tmp1, tmp2,  mi0, mi1, mj0, mj1);
                dilatation_3_unroll(tmp2, tmp1,  mi0, mi1, mj0, mj1);
                erosion_3_unroll(tmp1, img_filtered,  mi0, mi1, mj0, mj1); 
                break;
            case DILATATION3 :
                erosion_3_unroll(img_bin, tmp1, mi0, mi1, mj0, mj1);
                CHRONO(dilatation_3_unroll(tmp1, tmp2,  mi0, mi1, mj0, mj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000;
                BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts("")); 
                dilatation_3_unroll(tmp2, tmp1,  mi0, mi1, mj0, mj1);
                erosion_3_unroll(tmp1, img_filtered,  mi0, mi1, mj0, mj1); 
                break;
            case MORPHO3 :
                if(fract){
                    BENCH(printf("====== Morphologie 5 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_3_unroll(img_bin, tmp1, mi0, mi1, mj0, mj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_unroll(tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_3_unroll(tmp2, tmp1, mi0, mi1, mj0, mj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 3 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_3_unroll(tmp1, img_filtered, mi0, mi1, mj0, mj1), cycles_erosion_2); 
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
                    CHRONO(morpho_3_unroll(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho); 
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
                CHRONO(erosion_5_unroll(img_bin, tmp1, mi0, mi1, mj0, mj1), cycles_erosion_1); 
                time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                time_erosion_1 *= 1000; 
                BENCH(printf("Erosion 5 Déroulée :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_unroll(tmp1, tmp2,  mi0, mi1, mj0, mj1);
                dilatation_5_unroll(tmp2, tmp1,  mi0, mi1, mj0, mj1);
                erosion_5_unroll(tmp1, img_filtered,  mi0, mi1, mj0, mj1); 
                break;
            case DILATATION5 :
                erosion_5_unroll(img_bin, tmp1, mi0, mi1, mj0, mj1);
                CHRONO(dilatation_5_unroll(tmp1, tmp2,  mi0, mi1, mj0, mj1), cycles_dilatation_1); 
                time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                time_dilatation_1 *= 1000; 
                BENCH(printf("Dilatation 5 Déroulée :")); BENCH(puts(""));
                BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                dilatation_5_unroll(tmp2, tmp1,  mi0, mi1, mj0, mj1);
                erosion_5_unroll(tmp1, img_filtered,  mi0, mi1, mj0, mj1); 
                break;
            case MORPHO5 :
                if(fract){
                    BENCH(printf("====== Morphologie 5 fractionnée : ======")); BENCH(puts(""));
                    CHRONO(erosion_5_unroll(img_bin, tmp1, mi0, mi1, mj0, mj1), cycles_erosion_1); 
                    time_erosion_1 = (double)(cycles_erosion_1/CLK_PROC);
                    debit_erosion_1 = (WIDTH*HEIGHT) / time_erosion_1;
                    time_erosion_1 *= 1000;
                    BENCH(printf("Erosion 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_erosion_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_erosion_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_erosion_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_unroll(tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_dilatation_1); 
                    time_dilatation_1 = (double)(cycles_dilatation_1/CLK_PROC);
                    debit_dilatation_1 = (WIDTH*HEIGHT) / time_dilatation_1;
                    time_dilatation_1 *= 1000;
                    BENCH(printf("Dilatation 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_1)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_1/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_1)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(dilatation_5_unroll(tmp2, tmp1, mi0, mi1, mj0, mj1), cycles_dilatation_2); 
                    time_dilatation_2 = (double)(cycles_dilatation_2/CLK_PROC);
                    debit_dilatation_2 = (WIDTH*HEIGHT) / time_dilatation_2;
                    time_dilatation_2 *= 1000;
                    BENCH(printf("Dilatation 5 Déroulée:")); BENCH(puts(""));
                    BENCH(printf("temps (ms) \t    = %0.6f", time_dilatation_2)); BENCH(puts(""));
                    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles_dilatation_2/(WIDTH*HEIGHT))); BENCH(puts(""));
                    BENCH(printf("debit (pixel/sec)   = %0.2f", debit_dilatation_2)); BENCH(puts("")); BENCH(puts(""));
                    CHRONO(erosion_5_unroll(tmp1, img_filtered, mi0, mi1, mj0, mj1), cycles_erosion_2); 
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
                    CHRONO(morpho_5_unroll(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho); 
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

	// ---------- //
    // -- free -- //
    // ---------- //

	free_ui8matrix(image, mi0, mi1, mj0, mj1);

	free_ui8matrix(mean0, mi0, mi1, mj0, mj1);
	free_ui8matrix(mean1, mi0, mi1, mj0, mj1);

	free_ui8matrix(std0, mi0, mi1, mj0, mj1);
	free_ui8matrix(std1, mi0, mi1, mj0, mj1);

	free_ui8matrix(img_diff, mi0, mi1, mj0, mj1);

	free_ui8matrix(img_bin     , mi0b, mi1b, mj0b, mj1b);
    free_ui8matrix(img_filtered, mi0b, mi1b, mj0b, mj1b);

    free_ui8matrix(tmp1, mi0b, mi1b, mj0b, mj1b);
    free_ui8matrix(tmp2, mi0b, mi1b, mj0b, mj1b);
}

void bench_morpho_dataset(){

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

void bench_morpho_graphic(){

    // init fichier csv
    FILE* fichier_csv = fopen("csv_files/perf_morpho.csv","w");
    fprintf(fichier_csv, ";%s;;;;;%s\n", "Morpho 3", "Morpho 3 OPTI");
    fprintf(fichier_csv, "%s;%s;%s;%s;", "Taille (pixels)", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");
    fprintf(fichier_csv, ";%s;%s;%s\n", "Temps (ms)", "Cycle par point (cpp)", "Debit (pixel/seconde)");

    // BORD
    int b;

    int mi0, mi1, mj0, mj1;     // indices scalaire
    int mi0b, mi1b, mj0b, mj1b; // indices scalaires avec bord

    // chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

    char *format = "%d ";

    // calcul cpp
    double cycles_morpho_3, cycles_morpho_3_unroll;

    // calcul temps
    double time_morpho_3, time_morpho_3_unroll;

    // calcul debit
    double debit_morpho_3, debit_morpho_3_unroll;

    puts("====================================");
    puts("=== benchmark mouvement graphics ===");
    puts("====================================");

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

        uint8 ** image_init     = ui8matrix(mi0, mi1, mj0, mj1);

        // images
        uint8** image           = ui8matrix(mi0, mi1, mj0, mj1);

        // moyennes
        uint8** mean0           = ui8matrix(mi0, mi1, mj0, mj1);
        uint8** mean1           = ui8matrix(mi0, mi1, mj0, mj1);

        // ecart-types
        uint8** std0            = ui8matrix(mi0, mi1, mj0, mj1);
        uint8** std1            = ui8matrix(mi0, mi1, mj0, mj1);

        // image de différence
        uint8 ** img_diff       = ui8matrix(mi0, mi1, mj0, mj1);
        
        // image binaire (sortie)
        uint8 ** img_bin        = ui8matrix(mi0b, mi1b, mj0b, mj1b);
        uint8 ** img_filtered   = ui8matrix(mi0b, mi1b, mj0b, mj1b);

        uint8 ** tmp1           = ui8matrix(mi0b, mi1b, mj0b, mj1b);
        uint8 ** tmp2           = ui8matrix(mi0b, mi1b, mj0b, mj1b);

        for (int i = mi0; i <= mi1; ++i)
        {
            for (int j = mj0; j <= mj1; ++j)
            {
                // Générer des valeurs de pixels avec un motif qui evoluent avec la taille
                // Génére 2 images censé etre tres similaire (seul quelques pixels doivent varier)
                int val1 = (i + j)*10 + j;
                int val2 = (i + j)*10 + j + i;
                if(val1 > 255)  val1 = val1 / 255;
                if(val2 > 255)  val2 = val2 / 255;

                image_init [i][j]       = val1;
                image[i][j]             = val2; 
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

        SigmaDelta_full_unroll(mi0, mi1, mj0, mj1, image, mean0, mean1, std0, std1, img_bin);

        duplicate_border(mi0, mi1, mj0, mj1, b, img_bin);

        // ----------------- //
        // -- traitements -- //
        // ----------------- //

        CHRONO(morpho_3(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho_3);
        time_morpho_3 = (double)(cycles_morpho_3/CLK_PROC);
        debit_morpho_3 = width*height / time_morpho_3;

        CHRONO(morpho_3_unroll(img_bin, img_filtered, tmp1, tmp2, mi0, mi1, mj0, mj1), cycles_morpho_3_unroll);
        time_morpho_3_unroll = (double)(cycles_morpho_3_unroll/CLK_PROC);
        debit_morpho_3_unroll = width*height / time_morpho_3_unroll;

        // ecrire les donnees dans un fichier csv
        fprintf(fichier_csv, "%d;", height);
        fprintf(fichier_csv, "%f;", time_morpho_3 * 1000);
        fprintf(fichier_csv, "%f;", cycles_morpho_3 / (height*width));
        fprintf(fichier_csv, "%f;", debit_morpho_3);

        fprintf(fichier_csv, ";%f;", time_morpho_3_unroll * 1000);
        fprintf(fichier_csv, "%f;" , cycles_morpho_3_unroll / (height*width));
        fprintf(fichier_csv, "%f\n", debit_morpho_3_unroll);

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

void main_bench_morpho(int argc, char *argv[]){

	// Affiche les métriques de performance

	// benchmark unitaire sur petite images test generer
	// bench_mouvement_morpho_car(true);

	// benchmark unitaire sur image du set
	// bench_morpho_car(false);

    // bench_morpho_car(false, EROSION3, SCALAIRE, 1);
    
    // bench_morpho_car(false, MORPHO3, SCALAIRE, 0);
    // bench_morpho_car(false, MORPHO3, SCALAIRE_OPTI, 0);

    // bench_morpho_car(false, MORPHO5, SCALAIRE, 0);
    // bench_morpho_car(false, MORPHO5, SCALAIRE_OPTI, 0);

    // bench_morpho_car(false, MORPHO5, SCALAIRE_OPTI, 1);

	// benchmark global sur tout le dataset
	// bench_morpho_dataset();

    bench_morpho_graphic();
}