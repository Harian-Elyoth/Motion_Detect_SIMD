/*
==============================
========= test_morpho.c ======
==============================
*/

// ================= INCLUDES ========================

#include "test_morpho.h"

// ================= PROGRAM =========================

void test_erosion_3(){
    DEBUG(printf("\n===========================  TEST EROSION 3 =================================\n"));
    gen_img_bin_test(1, 3);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    erosion_3(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_dilatation_3(){
    DEBUG(printf("\n===========================  TEST DILATATION 3 =================================\n"));
    gen_img_bin_test(0, 3);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    dilatation_3(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image dilatée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_erosion_5(){
    DEBUG(printf("\n===========================  TEST EROSION 5 =================================\n"));
    gen_img_bin_test(1, 5);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    erosion_5(img_bin_test, img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_dilatation_5(){
    DEBUG(printf("\n===========================  TEST DILATATION 5 =================================\n"));
    gen_img_bin_test(0, 5);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    dilatation_5(img_bin_test, img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image dilatée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_morpho_3(){
    DEBUG(printf("\n===========================  TEST MORPHO 3 =================================\n"));
    gen_img_bin_test(1, 3);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    morpho_3(img_bin_test, img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image transformée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_morpho_5(){
    DEBUG(printf("\n===========================  TEST MORPHO 5 =================================\n"));
    gen_img_bin_test(1, 5);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));

    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    morpho_5(img_bin_test, img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test);


    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image transformée : ")); DEBUG(puts(""));

    void free_matrix();
}


void test_erosion_3_opti(){
    DEBUG(printf("\n===========================  TEST EROSION 3_opti =================================\n"));
    gen_img_bin_test(1, 3);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    erosion_3_opti(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_dilatation_3_opti(){
    DEBUG(printf("\n===========================  TEST DILATATION 3_opti =================================\n"));
    gen_img_bin_test(0, 3);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    dilatation_3_opti(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image dilatée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_erosion_5_opti(){
    DEBUG(printf("\n===========================  TEST EROSION 5_opti =================================\n"));
    gen_img_bin_test(1, 5);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    erosion_5_opti(img_bin_test, img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_dilatation_5_opti(){
    DEBUG(printf("\n===========================  TEST DILATATION 5_opti =================================\n"));
    gen_img_bin_test(0, 5);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    dilatation_5_opti(img_bin_test, img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image dilatée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_morpho_3_opti(){
    DEBUG(printf("\n===========================  TEST MORPHO 3_opti =================================\n"));
    gen_img_bin_test(1, 3);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    morpho_3_opti(img_bin_test, img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test);

    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image transformée : ")); DEBUG(puts(""));

    void free_matrix();
}

void test_morpho_5_opti(){
    DEBUG(printf("\n===========================  TEST MORPHO 5_opti =================================\n"));
    gen_img_bin_test(1, 5);

	char * format = "%d ";

    DEBUG(display_ui8matrix(img_bin_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image binaire : ")); DEBUG(puts(""));

    //On recupère img_bin de mouvement et on applique une erosion_3 dessus
    morpho_5_opti(img_bin_test, img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test);


    DEBUG(display_ui8matrix(img_filtered_test, mi0b_test, mi1b_test, mj0b_test, mj1b_test, format, "image transformée : ")); DEBUG(puts(""));

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
