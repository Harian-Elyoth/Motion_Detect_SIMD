/*
==============================
========= test_morpho.c ======
==============================
*/

// ================= INCLUDES ========================

#include "test_morpho.h"

// ================= PROGRAM =========================

void test_erosion_3(){
    gen_img_bin_test(1, 3);

    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_3(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test), cycles);

    BENCH(printf("cycles = %0.6f", cycles)); BENCH(puts(""));

	BENCH(printf("cycles/X*Y = %0.6f", cycles/(WIDTH_TEST * HEIGHT_TEST))); BENCH(puts(""));

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_dilatation_3(){
    gen_img_bin_test(0, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_3(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test), cycles);

    BENCH(printf("cycles = %0.6f", cycles)); BENCH(puts(""));

	BENCH(printf("cycles/X*Y = %0.6f", cycles/(WIDTH_TEST * HEIGHT_TEST))); BENCH(puts(""));

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_erosion_5(){
    gen_img_bin_test(1, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(erosion_5(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test), cycles);

    BENCH(printf("cycles = %0.6f", cycles)); BENCH(puts(""));

	BENCH(printf("cycles/X*Y = %0.6f", cycles/(WIDTH_TEST * HEIGHT_TEST))); BENCH(puts(""));

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_dilatation_5(){
    gen_img_bin_test(0, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(dilatation_5(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test), cycles);

    BENCH(printf("cycles = %0.6f", cycles)); BENCH(puts(""));

	BENCH(printf("cycles/X*Y = %0.6f", cycles/(WIDTH_TEST * HEIGHT_TEST))); BENCH(puts(""));

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_morpho_3(){
    gen_img_bin_test(1, 3);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_3(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test), cycles);

    BENCH(printf("cycles = %0.6f", cycles)); BENCH(puts(""));

	BENCH(printf("cycles/X*Y = %0.6f", cycles/(WIDTH_TEST * HEIGHT_TEST))); BENCH(puts(""));

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_morpho_5(){
    gen_img_bin_test(1, 5);
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char * format = "%d ";
    double cycles;

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));

    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    CHRONO(morpho_5(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test), cycles);

    BENCH(printf("cycles = %0.6f", cycles)); BENCH(puts(""));

	BENCH(printf("cycles/X*Y = %0.6f", cycles/(WIDTH_TEST * HEIGHT_TEST))); BENCH(puts(""));

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

//type = 1 : test pour érosion (majorité de 1) ///// type = 0 : test pour dilatation majourité de 0
void gen_img_bin_test(int type, int kernel_size){
    
    int seuil;
    if(type){
       seuil = 80; 
    }
    else {
        seuil = 20;
    }
    if(kernel_size == 3){
        b_test = 1;
    }
    else if(kernel_size == 5){
        b_test = 2;
    }
    else {
        fprintf(stderr, "Error wrong kernel_size must be 3 or 5 and it's %d\n", kernel_size);
        exit(1);
    }
    // indices matrices
	mi0_test = 0; mi1_test = HEIGHT_TEST-1;
	mj0_test = 0; mj1_test = WIDTH_TEST-1;
	
	// indices matrices avec bord
	mi0b_test = mi0_test-b_test; mi1b_test = mi1_test+b_test;
	mj0b_test = mj0_test-b_test; mj1b_test = mj1_test+b_test;

    img_bin_test = ui8matrix(mi0b_test, mi1b_test, mj0b_test, mj1b_test);

	// image filtrée
	img_filtered_test = ui8matrix(mi0b_test, mi1b_test, mj0b_test, mj1b_test);

    srand(time(NULL));
    for(int i = mi0_test ; i <= mi1_test ; i++){
        for(int j = mj0_test ; j <= mj1_test ; j++){
            if(rand() % 100 < seuil){
                img_bin_test[i][j] = 1;
            }
            else {
                img_bin_test[i][j] = 0;
            }
        }
    }
}

void main_test_morpho(int argc, char *argv[])
{


    //test_erosion_3();
    //test_erosion_5();
    //test_dilatation_3();
    //test_dilatation_5();
    test_morpho_3();
    // test_morpho_5();
    
}
