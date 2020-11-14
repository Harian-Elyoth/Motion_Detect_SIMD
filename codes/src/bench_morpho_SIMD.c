/* ---------------------------------------------------------------- */
/* ---  Bench Algorithme morpho_SIMD pour le traitement d'image -------- */
/* ---------------------------------------------------------------- */

#include "bench_morpho_SIMD.h"


void bench_erosion_3_SIMD(){
    
    gen_vimg_bin_bench_SIMD(1, 3);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_3_SIMD(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench erosion 3 SIMD :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_erosion_5_SIMD(){

    gen_vimg_bin_bench_SIMD(1, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_5_SIMD(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench erosion 5 SIMD:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));


    void free_matrix();
}

void bench_dilatation_3_SIMD(){
    gen_vimg_bin_bench_SIMD(0, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_3_SIMD(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench dilatation 3 SIMD:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_dilatation_5_SIMD(){

    gen_vimg_bin_bench_SIMD(0, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_5_SIMD(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench dilatation 5 SIMD:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_morpho_3_SIMD(){
    gen_vimg_bin_bench_SIMD(1, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_3_SIMD(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench morpho SIMD 3 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_morpho_5_SIMD(){
    gen_vimg_bin_bench_SIMD(1, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_5_SIMD(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench morpho SIMD 5 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_erosion_3_SIMD_opti(){
    
    gen_vimg_bin_bench_SIMD(1, 3);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_3_SIMD_opti(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench erosion 3 SIMD :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_erosion_5_SIMD_opti(){

    gen_vimg_bin_bench_SIMD(1, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_5_SIMD_opti(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench erosion 5 SIMD:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));


    void free_matrix();
}

void bench_dilatation_3_SIMD_opti(){
    gen_vimg_bin_bench_SIMD(0, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_3_SIMD_opti(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench dilatation 3 SIMD:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_dilatation_5_SIMD_opti(){

    gen_vimg_bin_bench_SIMD(0, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_5_SIMD_opti(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench dilatation 5 SIMD:\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_morpho_3_SIMD_opti(){
    gen_vimg_bin_bench_SIMD(1, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_3_SIMD_opti(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench morpho SIMD 3 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_morpho_5_SIMD_opti(){
    gen_vimg_bin_bench_SIMD(1, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_5_SIMD_opti(vimg_bin_bench, vimg_filtered_bench , vmi0_bench, vmi1_bench, vmj0_bench, vmj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench morpho SIMD 5 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}


void gen_vimg_bin_bench_SIMD(int type, int kernel_size){


	char *format = "%d ";
	

	puts("====================================");
	puts("=== test mouvement unitaire SIMD ===");
	puts("====================================");

	// BORD
	// 1 for 3x3 
    if (kernel_size == 3)
    {
        b_bench = 1; 
    }
    else
    {
        b_bench = 2; 
    }

	// 2 for 5x5
	//int b_bench = 2;

	// cardinalité des registres
	int card = card_vuint8(); // 16

	// ------------------------- //
	// -- calculs des indices -- //
	// ------------------------- //

	// indices scalaires matrices
	int mi0 = 0; int mi1 = HEIGHT_BENCH-1;
	int mj0 = 0; int mj1 = WIDTH_BENCH-1;

	// indices scalaires matrices avec bord
	int mi0b = mi0-b_bench; int mi1b = mi1+b_bench;
	int mj0b = mj0-b_bench; int mj1b = mj1+b_bench;

	// indices vectoriels matrices
	int vmi0 = 0; int vmi1 = (HEIGHT_BENCH)-1;
	int vmj0 = 0; int vmj1 = (WIDTH_BENCH/card)-1;

	// indices vectoriels matrices avec bord
	int vmi0b = vmi0-b_bench; int vmi1b = vmi1+b_bench;
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
	vimg_bin_bench = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	// image filtré par morpho
	vimg_filtered_bench = vui8matrix(vmi0b, vmi1b, vmj0b, vmj1b);

	/*---------------------------------------------------*/

	// ------------------------------ //
	// -- chargement et conversion -- //
	// ------------------------------ //

	uint8 ** img_temp = ui8matrix(mi0b, mi1b, mj0b, mj1b);

    MLoadPGM_ui8matrix("../car3/car_3037.pgm", mi0b, mi1b, mj0b, mj1b, img_temp);

    duplicate_border(mi0, mi1, mj0, mj1, b_bench, img_temp);

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

    duplicate_border(mi0, mi1, mj0, mj1, b_bench, img_temp);
    

	// transfert ui8matrix à vui8matrix real
	ui8matrix_to_vui8matrix(card, vmi0b, vmi1b, vmj0b, vmj1b, img_temp, image);


	// ----------------- //
	// -- traitements -- //
	// ----------------- //

	// SIGMA DELA

	SigmaDelta_step1_simd(vmi0b, vmi1b, vmj0b, vmj1b, mean0, mean1, image);
	SigmaDelta_step2_simd(vmi0b, vmi1b, vmj0b, vmj1b, image, mean1, img_diff);
	SigmaDelta_step3_simd(vmi0b, vmi1b, vmj0b, vmj1b, std0, std1, img_diff);
	SigmaDelta_step4_simd( vmi0b, vmi1b, vmj0b, vmj1b, std1, img_diff, vimg_bin_bench);



	// ---------- //
	// -- free -- //
	// ---------- //

	free_vui8matrix(image, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(mean0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(mean1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(std0, vmi0b, vmi1b, vmj0b, vmj1b);
	free_vui8matrix(std1, vmi0b, vmi1b, vmj0b, vmj1b);

	free_vui8matrix(img_diff, vmi0b, vmi1b, vmj0b, vmj1b);

   
	
}

void main_bench_morpho_SIMD(int argc, char *argv[])
{


    //bench_erosion_3_SIMD_opti();
    //bench_erosion_5();
    //bench_dilatation_3_SIMD_opti();
    //bench_dilatation_3_SIMD_opti();

    //bench_erosion_3_SIMD_opti();
    //bench_dilatation_5();
    bench_morpho_3_SIMD_opti();
    bench_morpho_3_SIMD();
    //bench_morpho_SIMD_5();
    
}
