/*
==========================================================
============= fichier test de morpho_SIMD.c ==============
==========================================================

*/

#include "test_morpho_SIMD.h"



void test_erosion_3_SIMD(){
    
    DEBUG(printf("\n===========================  TEST EROSION 3 =================================\n"));

    gen_vimg_bin_test_SIMD(1, 3); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    erosion_3_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_erosion_5_SIMD(){

    DEBUG(printf("\n===========================  TEST EROSION 5 =================================\n"));

    gen_vimg_bin_test_SIMD(1, 5); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    erosion_5_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_dilatation_3_SIMD(){

    DEBUG(printf("\n===========================  TEST DILATATION 3 =================================\n"));

    gen_vimg_bin_test_SIMD(0, 3); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    dilatation_3_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image dilatée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_dilatation_5_SIMD(){

    DEBUG(printf("\n===========================  TEST DILATATION 5 =================================\n"));

    gen_vimg_bin_test_SIMD(0, 5); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    dilatation_5_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image dilatée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_morpho_3_SIMD(){

    DEBUG(printf("\n===========================  TEST MORPHO 3 SIMD =================================\n"));

    gen_vimg_bin_test_SIMD(1, 3); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    morpho_3_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image transformée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_morpho_5_SIMD(){

    DEBUG(printf("\n===========================  TEST MORPHO 5 =================================\n"));

    gen_vimg_bin_test_SIMD(1, 5); //erosion kernel_3


	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    morpho_5_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image transformée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_erosion_3_SIMD_opti(){
    
    DEBUG(printf("\n===========================  TEST EROSION 3 =================================\n"));

    gen_vimg_bin_test_SIMD(1, 3); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    erosion_3_SIMD_opti(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_erosion_5_SIMD_opti(){

    DEBUG(printf("\n===========================  TEST EROSION 5 =================================\n"));

    gen_vimg_bin_test_SIMD(1, 5); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    erosion_5_SIMD_opti(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image érodée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_dilatation_3_SIMD_opti(){

    DEBUG(printf("\n===========================  TEST DILATATION 3 =================================\n"));

    gen_vimg_bin_test_SIMD(0, 3); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    dilatation_3_SIMD_opti(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image dilatée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_dilatation_5_SIMD_opti(){

    DEBUG(printf("\n===========================  TEST DILATATION 5 =================================\n"));

    gen_vimg_bin_test_SIMD(0, 5); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    dilatation_5_SIMD_opti(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image dilatée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_morpho_3_SIMD_opti(){

    DEBUG(printf("\n===========================  TEST MORPHO 3 =================================\n"));

    gen_vimg_bin_test_SIMD(1, 3); //erosion kernel_3

	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    morpho_3_SIMD_opti(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image transformée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_morpho_5_SIMD_opti(){

    DEBUG(printf("\n===========================  TEST MORPHO 5 =================================\n"));

    gen_vimg_bin_test_SIMD(1, 5); //erosion kernel_3


	char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    morpho_5_SIMD_opti(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image transformée : ")); DEBUG(puts(""));

    void free_vmatrix();
}

void test_morpho_3_SIMD_pipeline(){

    DEBUG(printf("\n===========================  TEST MORPHO 3 SIMD PIPELINE =================================\n"));

    gen_vimg_bin_test_SIMD(1, 3); //erosion kernel_3

    char * format = "%d ";

    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    //On recupère vimg_bin de mouvement et on applique une erosion_3 dessus
    morpho_3_SIMD_pipeline(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);

    DEBUG(display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image transformée : ")); DEBUG(puts(""));

    void free_vmatrix();
}


void gen_vimg_bin_test_SIMD(int type, int kernel_size){
    
    int seuil;
    if(type){
       seuil = 95; 
    }
    else {
        seuil = 5;
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
	vmi0_test = 0; vmi1_test = HEIGHT_TEST - 1;
	vmj0_test = 0; vmj1_test = WIDTH_TEST/16 - 1;
	
	// indices matrices avec bord
	vmi0b_test = vmi0_test-b_test; vmi1b_test = vmi1_test+b_test;
	vmj0b_test = vmj0_test-b_test + 1; vmj1b_test = vmj1_test+b_test - 1;

    vimg_bin_test = vui8matrix(vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test);

	// image filtrée
	vimg_filtered_test = vui8matrix(vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test);

    // srand(time(NULL));
    // for(int i = vmi0_test ; i <= vmi1_test ; i++){
    //     for(int j = vmj0_test ; j <= vmj1_test ; j++){
    //         VEC_STORE_2D_EPI8(init_vuint8_all(rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, \
    //         rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, \
    //         rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil,\
    //         rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil, rand()%100 < seuil), i, j, vimg_bin_test);
            
    //     }
    // }

    for(int i = vmi0_test ; i <= vmi1_test ; i++){
        for(int j = vmj0_test ; j <= vmj1_test ; j++){
                
            if((i > 1 && i < 7)  && (j > 0 && j < 3))
            {
                vimg_bin_test[i][j] = init_vuint8(1); 
            }
            else
            {
                vimg_bin_test[i][j] = init_vuint8(0); 
            }      
        }
    }    

	
}


void main_test_morpho_SIMD(int argc, char *argv[])
{
    // test_erosion_3_SIMD_opti();
    // test_erosion_5_SIMD_opti();
    // test_dilatation_3_SIMD_opti();
    // test_dilatation_5_SIMD_opti();
    //test_erosion_3_SIMD();
    //test_erosion_5_SIMD();
    //test_dilatation_3_SIMD();
    //test_dilatation_5_SIMD();
    // test_morpho_3_SIMD_opti();
    // test_morpho_5_SIMD_opti();
    test_morpho_3_SIMD();
    test_morpho_3_SIMD_pipeline();


}
