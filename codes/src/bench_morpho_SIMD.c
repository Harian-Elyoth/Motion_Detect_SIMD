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

void gen_vimg_bin_bench_SIMD(int type, int kernel_size){
    
    int seuil;
    if(type){
       seuil = 95; 
    }
    else {
        seuil = 5;
    }
    if(kernel_size == 3){
        b_bench = 1;
    }
    else if(kernel_size == 5){
        b_bench = 2;
    }
    else {
        fprintf(stderr, "Error wrong kernel_size must be 3 or 5 and it's %d\n", kernel_size);
        exit(1);
    }
    // indices matrices
	vmi0_bench = 0; vmi1_bench = HEIGHT_BENCH - 1;
	vmj0_bench = 0; vmj1_bench = WIDTH_BENCH/16 - 1;
	
	// indices matrices avec bord
	vmi0b_bench = vmi0_bench-b_bench; vmi1b_bench = vmi1_bench+b_bench;
	vmj0b_bench = vmj0_bench-b_bench + 1; vmj1b_bench = vmj1_bench+b_bench - 1;

    vimg_bin_bench = vui8matrix(vmi0b_bench, vmi1b_bench, vmj0b_bench, vmj1b_bench);

	// image filtrée
	vimg_filtered_bench = vui8matrix(vmi0b_bench, vmi1b_bench, vmj0b_bench, vmj1b_bench);

    srand(time(NULL));
    for(int i = vmi0_bench ; i <= vmi1_bench ; i++){
        for(int j = vmj0_bench ; j <= vmj1_bench ; j++){
            VEC_STORE_2D_EPI8(init_vuint8_all(rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, \
            rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, \
            rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil,\
            rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil), i, j, vimg_bin_bench);
            
        }
    }

	
}

void main_bench_morpho_SIMD(int argc, char *argv[])
{


    bench_erosion_3_SIMD();
    //bench_erosion_5();
    bench_dilatation_3_SIMD();
    bench_dilatation_3_SIMD();

    bench_erosion_3_SIMD();
    //bench_dilatation_5();
    bench_morpho_3_SIMD();
    //bench_morpho_SIMD_5();
    
}
