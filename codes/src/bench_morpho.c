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
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_3(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench erosion 3 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_erosion_5(){

    gen_img_bin_bench(1, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_5(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench erosion 5 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));


    void free_matrix();
}

void bench_dilatation_3(){
    gen_img_bin_bench(0, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_3(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench dilatation 3 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_dilatation_5(){

    gen_img_bin_bench(0, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_5(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench dilatation 5 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_morpho_3(){
    gen_img_bin_bench(1, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_3(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench morpho 3 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void bench_morpho_5(){
    gen_img_bin_bench(1, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;
    double time;
    clock_t start, finish;
    double debit;
    
    start = clock();
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_5(img_bin_bench, img_filtered_bench , mi0_bench, mi1_bench, mj0_bench, mj1_bench), cycles);
    finish = clock();
    time = (double)(finish - start)/CLOCKS_PER_SEC;
    debit = (WIDTH_BENCH * HEIGHT_BENCH) / time;
    BENCH(printf("Bench morpho 5 :\n"));
    BENCH(printf("temps (ms) \t    = %0.6f", time)); BENCH(puts(""));
    BENCH(printf("cpp   (cycle/pixel) = %0.6f", cycles/(WIDTH_BENCH * HEIGHT_BENCH))); BENCH(puts(""));
    BENCH(printf("debit (pixel/sec)   = %0.2f", debit)); BENCH(puts("")); BENCH(puts(""));

    void free_matrix();
}

void gen_img_bin_bench(int type, int kernel_size){
    
    int seuil;
    if(type){
       seuil = 80; 
    }
    else {
        seuil = 20;
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
	mi0_bench = 0; mi1_bench = HEIGHT_BENCH-1;
	mj0_bench = 0; mj1_bench = WIDTH_BENCH-1;
	
	// indices matrices avec bord
	mi0b_bench = mi0_bench-b_bench; mi1b_bench = mi1_bench+b_bench;
	mj0b_bench = mj0_bench-b_bench; mj1b_bench = mj1_bench+b_bench;

    img_bin_bench = ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);

	// image filtrée
	img_filtered_bench = ui8matrix(mi0b_bench, mi1b_bench, mj0b_bench, mj1b_bench);

    srand(time(NULL));
    for(int i = mi0_bench ; i <= mi1_bench ; i++){
        for(int j = mj0_bench ; j <= mj1_bench ; j++){
            if(rand() % 100 < seuil){
                img_bin_bench[i][j] = 1;
            }
            else {
                img_bin_bench[i][j] = 0;
            }
        }
    }
}

void main_bench_morpho(int argc, char *argv[])
{


    bench_erosion_3();
    //bench_erosion_5();
    bench_dilatation_3();
    bench_dilatation_3();

    bench_erosion_3();
    //bench_dilatation_5();
    bench_morpho_3();
    //bench_morpho_5();
    
}
