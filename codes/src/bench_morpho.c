/* ---------------------------------------------------------------- */
/* ---  Bench Algorithme Morpho pour le traitement d'image -------- */
/* ---------------------------------------------------------------- */

#include "bench_morpho.h"

void bench_erosion_3(){

    gen_img_bin_bench(1, 3);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    
	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    //On recupère img_bin_bench de mouvement et on applique une erosion_3 dessus

    CHRONO(erosion_3(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    
    time = (double)(cycles/CLK_PROC);
    debit = (WIDTH  * HEIGHT) / time;

    BENCH(printf("Bench erosion 3 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

}

void bench_erosion_5(){

    gen_img_bin_bench(1, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    //On recupère img_bin_bench de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_5(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    
    time = (double)(cycles/CLK_PROC);
    debit = (WIDTH  * HEIGHT) / time;

    BENCH(printf("Bench erosion 5 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

}

void bench_dilatation_3(){
    
    gen_img_bin_bench(0, 3);
    
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    //On recupère img_bin_bench de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_3(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    
    time = (double)(cycles/CLK_PROC);
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench dilatation 3 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_dilatation_5(){

    gen_img_bin_bench(0, 5);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    //On recupère img_bin_bench de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_5(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    
    time = (double)(cycles/CLK_PROC);
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench dilatation 5 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_morpho_3(){
    
    gen_img_bin_bench(1, 3);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    //On recupère img_bin_bench de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_3(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    
    time = (double)cycles/CLK_PROC;
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench morpho 3 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_morpho_5(){
    
    gen_img_bin_bench(1, 5);
    
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    CHRONO(morpho_5(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    
    time = (double)cycles/CLK_PROC;
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench morpho 5 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_erosion_3_opti(){
    
    gen_img_bin_bench(1, 3);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    //On recupère img_bin_bench de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_3_opti(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);

    time = (double)(cycles/CLK_PROC);
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench erosion 3 opti:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_erosion_5_opti(){

    gen_img_bin_bench(1, 5);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    //On recupère img_bin_bench de mouvement et on applique une erosion 5 dessus
    CHRONO(erosion_5_opti(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);

    time = (double)(cycles/CLK_PROC);
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench erosion 5 opti:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_dilatation_3_opti(){
    
    gen_img_bin_bench(0, 3);
    
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    CHRONO(dilatation_3_opti(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    
    time = (double)cycles/CLK_PROC;
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench dilatation 3 opti:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_dilatation_5_opti(){

    gen_img_bin_bench(0, 5);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    CHRONO(dilatation_5_opti(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);

    time = (double)cycles/CLK_PROC;
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench dilatation 5 opti:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_morpho_3_opti(){
    
    gen_img_bin_bench(1, 3);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    CHRONO(morpho_3_opti(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);

    time = (double)cycles/CLK_PROC;
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench morpho 3 opti:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void bench_morpho_5_opti(){
    
    gen_img_bin_bench(1, 5);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    double debit;
    
    CHRONO(morpho_5_opti(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);

    time = (double)cycles/CLK_PROC;
    debit = (WIDTH  * HEIGHT ) / time;

    BENCH(printf("Bench morpho 5 opti :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time * 1000)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH  * HEIGHT ))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));
}

void gen_img_bin_bench(int type, int kernel_size){
    

    if (kernel_size == 3)
    {
        b_bench = 1; 
    }
    else
    {
        b_bench = 2; 
    }

    // indices matrices
    mi0_bench = 0; mi1_bench = HEIGHT  - 1;
    mj0_bench = 0; mj1_bench = WIDTH   - 1;
    
	
	// indices matrices avec bord
	mi0b_bench = mi0_bench-b_bench; mi1b_bench = mi1_bench+b_bench;
	mj0b_bench = mj0_bench-b_bench; mj1b_bench = mj1_bench+b_bench;


	// ---------------- //
    // -- allocation -- //
    // ---------------- //
    
	uint8** image 		= ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);

	uint8** mean0 		= ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);
	uint8** mean1 		= ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);

	uint8** std0 		= ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);
	uint8** std1 		= ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);

	uint8** img_diff 	= ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);
	img_bin_bench 	    = ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);
	img_filtered_bench 	= ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);
   
	// -------------- //
    // -- prologue -- //
    // -------------- //


    MLoadPGM_ui8matrix("../car3/car_3037.pgm", mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench, image);

    duplicate_border(mi0_bench, mi1_bench, mj0_bench, mj1_bench, b_bench, image);

    // initiate mean0 et std0 for first iteration
    for (int i = mi0b_bench; i <= mi1b_bench; ++i)
    {
        for (int j = mj0b_bench; j <= mj1b_bench; ++j)
        {
            mean0[i][j] = image[i][j];
            std0[i][j]  = VMIN;
        }
    }

    MLoadPGM_ui8matrix("../car3/car_3038.pgm", mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench, image);

    duplicate_border(mi0_bench, mi1_bench, mj0_bench, mj1_bench, b_bench, image);
    

	// ----------------- //
    // -- traitements -- //
    // ----------------- //

    SigmaDelta_step1(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench, mean0, mean1, image);
	SigmaDelta_step2(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench, image, mean1, img_diff);
	SigmaDelta_step3(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench, std0, std1, img_diff);
	SigmaDelta_step4(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench, std1, img_diff, img_bin_bench);

	// convert binary img to pgm img
	//bin_to_pgm(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench, img_bin_bench,"SD_out.pgm");

	// ---------- //
    // -- free -- //
    // ---------- //

	free_ui8matrix(image, mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);

	free_ui8matrix(mean0, mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);
	free_ui8matrix(mean1, mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);

	free_ui8matrix(std0, mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);
	free_ui8matrix(std1, mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);

	free_ui8matrix(img_diff, mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);
}

void main_bench_morpho(int argc, char *argv[])
{
    bench_erosion_3();
    bench_dilatation_3();

    bench_erosion_5();
    bench_dilatation_5();

    bench_morpho_3();
    bench_morpho_5();

    bench_erosion_3_opti();
    bench_dilatation_3_opti();
    //bench_dilatation_3_opti();
    //bench_erosion_3_opti();
    bench_erosion_5_opti();
    bench_dilatation_5_opti();
    //bench_dilatation_5();
    bench_morpho_3_opti();
    bench_morpho_5_opti();

}
