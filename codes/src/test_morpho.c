/*
==============================
========= test_morpho.c ======
==============================
*/

// ================= INCLUDES ========================

#include "test_morpho.h"

// ================= PROGRAM =========================


void gen_img_bin_test(type_morpho_t type){
    
    switch(type){
        case EROSION3:
        case DILATATION3:
        case MORPHO3:
            b_test = 1;
            break;
        case EROSION5:
        case DILATATION5:
        case MORPHO5:
            b_test = 2;
            break;
        default :
            break;
    }

    int height_test = 24;
    int width_test = 32;

    // indices matrices
	mi0_test = 0; mi1_test = height_test-1;
	mj0_test = 0; mj1_test = width_test-1;
	
	// indices matrices avec bord
	mi0b_test = mi0_test-b_test; mi1b_test = mi1_test+b_test;
	mj0b_test = mj0_test-b_test; mj1b_test = mj1_test+b_test;
    if(type == MORPHO3 || type == MORPHO5){
        tmp1 = ui8matrix(mi0b_test, mi1b_test, mj0b_test, mj1b_test);
        tmp2 = ui8matrix(mi0b_test, mi1b_test, mj0b_test, mj1b_test);
    }
    assertion = ui8matrix(mi0_test, mi1_test, mj0_test, mj1_test);
    img_bin_test = ui8matrix(mi0b_test, mi1b_test, mj0b_test, mj1b_test);

	// image filtrée
	img_filtered_test = ui8matrix(mi0b_test, mi1b_test, mj0b_test, mj1b_test);

    srand(time(NULL));
    for(int i = mi0_test ; i <= mi1_test ; i++){
        for(int j = mj0_test ; j <= mj1_test ; j++){
            if((i >= HEIGHT_TEST/4 && i <= HEIGHT_TEST*3/4) || (j >= WIDTH_TEST/4 && j <= WIDTH_TEST*3/4)){
                img_bin_test[i][j] = 1;
            }
            else {
                img_bin_test[i][j] = 0;
            }
        }
    }

    switch(type){
        case EROSION3 :
        case EROSION5 :
            for(int i = mi0_test + b_test; i <= mi1_test - b_test; i++){
                for(int j = mj0_test + b_test; j <= mj1_test - b_test; j++){
                    if((i >= (HEIGHT_TEST/4 + b_test) && i <= (HEIGHT_TEST*3/4 - b_test) || (j >= WIDTH_TEST/4 + b_test && j <= (WIDTH_TEST*3/4 - b_test)))){
                        assertion[i][j] = 1;
                    }
                    else {
                        assertion[i][j] = 0;
                    }
                }
            }
            break;
        case DILATATION3 :
        case DILATATION5 :
            for(int i = mi0_test ; i <= mi1_test ; i++){
                for(int j = mj0_test ; j <= mj1_test ; j++){
                    if((i >= (HEIGHT_TEST/4 - b_test) && i <= (HEIGHT_TEST*3/4 + b_test) || (j >= WIDTH_TEST/4 - b_test && j <= (WIDTH_TEST*3/4 + b_test)))){
                        assertion[i][j] = 1;
                    }
                    else {
                        assertion[i][j] = 0;
                    }
                }
            }
            break;
        case MORPHO3 :
            for(int i = mi0_test ; i <= mi1_test ; i++){
                for(int j = mj0_test ; j <= mj1_test ; j++){
                    if(((i == mi0_test) || (i== mi1_test)) || ((j == mj0_test) || (j == mj1_test))){
                        assertion[i][j] = 0;
                    }
                    else {
                        assertion[i][j] = img_bin_test[i][j];
                    }
                }
            }
            break;
        case MORPHO5 :
        
            for(int i = mi0_test ; i <= mi1_test ; i++){
                for(int j = mj0_test ; j <= mj1_test ; j++){
                    if(((i <= mi0_test + 1) || (i >= mi1_test - 1)) || ((j <= mj0_test + 1) || (j >= mj1_test - 1))){
                        assertion[i][j] = 0;
                    }
                    else {
                        assertion[i][j] = img_bin_test[i][j];
                    }
                }
            }
            break;
        default :
            break;
    }

}

void test_unitaire(type_morpho_t MORPHO, type_opti_t OPTI){
    gen_img_bin_test(MORPHO);
    char * format = " %d";
    DEBUG(display_ui8matrix(img_bin_test, mi0_test, mi1_test, mj0_test, mj1_test, format, "image binaire : ")); DEBUG(puts(""));
    switch(OPTI){
        case SCALAIRE :
            switch(MORPHO){
                case EROSION3 : 
                    erosion_3(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case EROSION5 :
                    erosion_5(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case DILATATION3 : 
                    dilatation_3(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case DILATATION5 :
                    dilatation_5(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case MORPHO3 :
                    morpho_3(img_bin_test, img_filtered_test, tmp1, tmp2, mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case MORPHO5 :
                    morpho_5(img_bin_test, img_filtered_test, tmp1, tmp2, mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                default :
                    break;
            }
            break;
        case SCALAIRE_OPTI :
            switch(MORPHO){
                case EROSION3 : 
                    erosion_3_opti(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case EROSION5 :
                    erosion_5_opti(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case DILATATION3 : 
                    dilatation_3_opti(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case DILATATION5 :
                    dilatation_5_opti(img_bin_test, img_filtered_test , mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case MORPHO3 :
                    morpho_3_opti(img_bin_test, img_filtered_test, tmp1, tmp2, mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                case MORPHO5 :
                    morpho_5_opti(img_bin_test, img_filtered_test, tmp1, tmp2, mi0_test, mi1_test, mj0_test, mj1_test);
                    break;
                default :
                    break;
            }
    }
    display_type_morpho(MORPHO);
    if(equal(img_filtered_test, assertion, mi0_test, mi1_test, mj0_test, mj1_test)){
        printf(" :\n\x1B[32mOK\x1B[0m\n\n");
    }
    else {
        printf(" :\n\x1B[31mKO\x1B[0m\n\n");
        DEBUG(display_ui8matrix(assertion, mi0_test, mi1_test, mj0_test, mj1_test, format, "assertion matrix : ")); DEBUG(puts(""));
    }
    DEBUG(display_ui8matrix(img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test, format, "returned matrix : ")); DEBUG(puts(""));
}

//Compare deux matrices de meme taille renvoi 1 si elles sont égales 0 sinon
int equal(uint8 ** A, uint8 ** B, int mi0, int mi1, int mj0, int mj1){
    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0; j <= mj1 ; j++){
            if(A[i][j] != B[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

void test_unitaires(){
    printf("===============================\n");
    display_type_opti(SCALAIRE);puts("");
    test_unitaire(EROSION5, SCALAIRE);
    test_unitaire(EROSION5, SCALAIRE);
    test_unitaire(DILATATION3, SCALAIRE);
    test_unitaire(DILATATION5, SCALAIRE);
    test_unitaire(MORPHO3, SCALAIRE);
    test_unitaire(MORPHO5, SCALAIRE);
    printf("===============================\n");
    display_type_opti(SCALAIRE_OPTI);puts("");
    test_unitaire(EROSION5, SCALAIRE_OPTI);
    test_unitaire(EROSION5, SCALAIRE_OPTI);
    test_unitaire(DILATATION3, SCALAIRE_OPTI);
    test_unitaire(DILATATION5, SCALAIRE_OPTI);
    test_unitaire(MORPHO3, SCALAIRE_OPTI);
    test_unitaire(MORPHO5, SCALAIRE_OPTI);
    printf("===============================\n");
}


void main_test_morpho(int argc, char *argv[])
{
    test_unitaires();
}
