/*
==========================================================
============= fichier test de morpho_SIMD.c ==============
==========================================================
*/

#include "test_morpho_SIMD.h"

int b_test;

vuint8 ** vimg_filtered_test;
vuint8 ** tmp1_SIMD;
vuint8 ** tmp2_SIMD;
vuint8 ** tmp3_SIMD;
vuint8 ** vimg_bin_test;

uint8  ** img_bin_test;
uint8  ** img_filtered_test;
uint8  ** assertion;

int mi0_test  , mi1_test  , mj0_test  , mj1_test  ;
int mi0b_test , mi1b_test , mj0b_test , mj1b_test ;
int vmi0_test , vmi1_test , vmj0_test , vmj1_test ;      // indices scalaire
int vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test;     // indices scalaires avec bord

void gen_img_bin_test_SIMD(type_morpho_t type, type_opti_t OPTI){
    int k;
    switch(type){
        case EROSION3:
        case DILATATION3:
        case MORPHO3:
            b_test = 1;
            k = 0;
            break;
        case EROSION5:
        case DILATATION5:
        case MORPHO5:
            b_test = 2;
            k = 1;
            break;
        default :
            break;
    }
    if(type == MORPHO3 && OPTI == PIPELINE_FUSION){
        b_test = 2;
    }
    // indices matrices
	vmi0_test = 0; vmi1_test = HEIGHT_TEST_SIMD-1;
	vmj0_test = 0; vmj1_test = WIDTH_TEST_SIMD/16-1;
	mi0_test = 0; mi1_test = HEIGHT_TEST_SIMD-1;
	mj0_test = 0; mj1_test = WIDTH_TEST_SIMD-1;
	// indices matrices avec bord
	vmi0b_test = vmi0_test-b_test; vmi1b_test = vmi1_test+b_test;
	vmj0b_test = vmj0_test-b_test + k; vmj1b_test = vmj1_test+b_test - k;

    mi0b_test = mi0_test-b_test; mi1b_test = mi1_test+b_test;
	mj0b_test = mj0_test-b_test; mj1b_test = mj1_test+b_test;
    printf("vmi0b_test = %d, vmi1b_test = %d, vmj0b_test = %d, vmj1b_test = %d\n", vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test);
    if(type == MORPHO3 || type == MORPHO5){
        tmp1_SIMD = vui8matrix(vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test);
        tmp2_SIMD = vui8matrix(vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test);
        tmp3_SIMD = vui8matrix(vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test);

    }


    assertion = ui8matrix(mi0_test, mi1_test, mj0_test, mj1_test);
    img_bin_test = ui8matrix(mi0b_test, mi1b_test, mj0b_test, mj1b_test);



    
    vimg_bin_test = vui8matrix(vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test);

	// image filtrée
	vimg_filtered_test = vui8matrix(vmi0_test, vmi1_test, vmj0_test, vmj1_test);
    img_filtered_test = ui8matrix(mi0_test, mi1_test, mj0_test, mj1_test);

    srand(time(NULL));
    for(int i = mi0_test ; i <= mi1_test ; i++){
        for(int j = mj0_test ; j <= mj1_test ; j++){
            if((i >= HEIGHT_TEST_SIMD/4 && i <= HEIGHT_TEST_SIMD*3/4) || (j >= WIDTH_TEST_SIMD/4 && j <= WIDTH_TEST_SIMD*3/4)){
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
                    if((i >= (HEIGHT_TEST_SIMD/4 + b_test) && i <= (HEIGHT_TEST_SIMD*3/4 - b_test) || (j >= WIDTH_TEST_SIMD/4 + b_test && j <= (WIDTH_TEST_SIMD*3/4 - b_test)))){
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
                    if((i >= (HEIGHT_TEST_SIMD/4 - b_test) && i <= (HEIGHT_TEST_SIMD*3/4 + b_test) || (j >= WIDTH_TEST_SIMD/4 - b_test && j <= (WIDTH_TEST_SIMD*3/4 + b_test)))){
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
    
    // BENCH(display_ui8matrix(img_bin_test, mi0_test, mi1_test, mj0_test, mj1_test, format, "image binaire : ")); DEBUG(puts(""));
    ui8matrix_to_vui8matrix_wb_morpho(16, vmi0b_test, vmi1b_test, vmj0b_test, vmj1b_test, img_bin_test, vimg_bin_test);

}

void test_unitaire_SIMD(type_morpho_t MORPHO, type_opti_t OPTI){
    gen_img_bin_test_SIMD(MORPHO, OPTI);
    char * format = " %d";
    DEBUG(display_vui8matrix(vimg_bin_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "image binaire : ")); DEBUG(puts(""));
    switch(OPTI){
        case SIMD :
            switch(MORPHO){
                case EROSION3 : 
                    erosion_3_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case EROSION5 :
                    erosion_5_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case DILATATION3 : 
                    dilatation_3_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case DILATATION5 :
                    dilatation_5_SIMD(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case MORPHO3 :
                    morpho_3_SIMD(vimg_bin_test, vimg_filtered_test, tmp1_SIMD, tmp2_SIMD, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case MORPHO5 :
                    morpho_5_SIMD(vimg_bin_test, vimg_filtered_test, tmp1_SIMD, tmp2_SIMD, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                default :
                    break;
            }
            break;
        case SIMD_OMP :
            switch(MORPHO){
                case EROSION3 : 
                    erosion_3_SIMD_omp(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case EROSION5 :
                    erosion_5_SIMD_omp(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case DILATATION3 : 
                    dilatation_3_SIMD_omp(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case DILATATION5 :
                    dilatation_5_SIMD_omp(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case MORPHO3 :
                    morpho_3_SIMD_omp(vimg_bin_test, vimg_filtered_test, tmp1_SIMD, tmp2_SIMD, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case MORPHO5 :
                    morpho_5_SIMD_omp(vimg_bin_test, vimg_filtered_test, tmp1_SIMD, tmp2_SIMD, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                default :
                    break;
            }
            break;
        case SIMD_UNROLL :
            switch(MORPHO){
                case EROSION3 : 
                    erosion_3_SIMD_unroll(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case EROSION5 :
                    erosion_5_SIMD_unroll(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case DILATATION3 : 
                    dilatation_3_SIMD_unroll(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case DILATATION5 :
                    dilatation_5_SIMD_unroll(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case MORPHO3 :
                    morpho_3_SIMD_unroll(vimg_bin_test, vimg_filtered_test, tmp1_SIMD, tmp2_SIMD, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case MORPHO5 :
                    morpho_5_SIMD_unroll(vimg_bin_test, vimg_filtered_test, tmp1_SIMD, tmp2_SIMD, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                default :
                    break;
            }
            break;
        case SIMD_UNROLL_OMP :
            switch(MORPHO){
                case EROSION3 : 
                    erosion_3_SIMD_unroll_omp(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case EROSION5 :
                    erosion_5_SIMD_unroll_omp(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case DILATATION3 : 
                    dilatation_3_SIMD_unroll_omp(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case DILATATION5 :
                    dilatation_5_SIMD_unroll_omp(vimg_bin_test, vimg_filtered_test , vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case MORPHO3 :
                    morpho_3_SIMD_unroll_omp(vimg_bin_test, vimg_filtered_test, tmp1_SIMD, tmp2_SIMD, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                case MORPHO5 :
                    morpho_5_SIMD_unroll_omp(vimg_bin_test, vimg_filtered_test, tmp1_SIMD, tmp2_SIMD, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
                    break;
                default :
                    break;
            }
            break;
        case PIPELINE :
            morpho_3_SIMD_pipeline(vimg_bin_test, tmp1_SIMD, tmp2_SIMD, tmp3_SIMD, vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
            break;
        case PIPELINE_FUSION :
            morpho_3_SIMD_pipeline_fusion(vimg_bin_test, tmp1_SIMD, tmp2_SIMD, vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
            break;
        case PIPELINE_OMP :
            morpho_3_SIMD_pipeline_omp(vimg_bin_test, tmp1_SIMD, tmp2_SIMD, tmp3_SIMD, vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
            break;
        case PIPELINE_FUSION_OMP :
            morpho_3_SIMD_pipeline_fusion_omp(vimg_bin_test, tmp1_SIMD, tmp2_SIMD, vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test);
            break;
        default :
            break;
    }
    display_type_morpho(MORPHO);
    //display_vui8matrix(vimg_filtered_test, vmi0_test, vmi1_test, vmj0_test, vmj1_test, format, "TEST\n");
    vui8matrix_to_ui8matrix_morpho(16, vmi0_test, vmi1_test, vmj0_test, vmj1_test, img_filtered_test, vimg_filtered_test);
    if(equal_SIMD(img_filtered_test, assertion, mi0_test, mi1_test, mj0_test, mj1_test)){
        printf(" :\n\x1B[32mOK\x1B[0m\n\n");
    }
    
    else {
        printf(" :\n\x1B[31mKO\x1B[0m\n\n");
        DEBUG(display_ui8matrix(assertion, mi0_test, mi1_test, mj0_test, mj1_test, format, "assertion matrix : ")); DEBUG(puts(""));
    }
    DEBUG(display_ui8matrix(img_filtered_test, mi0_test, mi1_test, mj0_test, mj1_test, format, "returned matrix : ")); DEBUG(puts(""));

}





//Compare deux matrices de meme taille renvoi 1 si elles sont égales 0 sinon
int equal_SIMD(uint8 ** A, uint8 ** B, int mi0, int mi1, int mj0, int mj1){
    for(int i = mi0 ; i <= mi1 ; i++){
        for(int j = mj0; j <= mj1 ; j++){
            if(A[i][j] != B[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

void tests_unitaires_SIMD(){
    printf("===============================\n");
    display_type_unroll(SIMD);puts("");
    test_unitaire_SIMD(EROSION3, SIMD);
    test_unitaire_SIMD(EROSION5, SIMD);
    test_unitaire_SIMD(DILATATION3, SIMD);
    test_unitaire_SIMD(DILATATION5, SIMD);
    test_unitaire_SIMD(MORPHO3, SIMD);
    test_unitaire_SIMD(MORPHO5, SIMD);
    printf("===============================\n");
    display_type_unroll(SIMD_UNROLL);puts("");
    test_unitaire_SIMD(EROSION3, SIMD_UNROLL);
    test_unitaire_SIMD(EROSION5, SIMD_UNROLL);
    test_unitaire_SIMD(DILATATION3, SIMD_UNROLL);
    test_unitaire_SIMD(DILATATION5, SIMD_UNROLL);
    test_unitaire_SIMD(MORPHO3, SIMD_UNROLL);
    test_unitaire_SIMD(MORPHO5, SIMD_UNROLL);
    printf("===============================\n");
    display_type_unroll(PIPELINE);puts("");
    test_unitaire_SIMD(MORPHO3, PIPELINE);
    printf("===============================\n");
    display_type_unroll(PIPELINE_FUSION);puts("");
    test_unitaire_SIMD(MORPHO3, PIPELINE_FUSION);
    printf("===============================\n");

}


// conversion sans bord
void ui8matrix_to_vui8matrix_morpho(int card, int vmi0, int vmi1, int vmj0, int vmj1, uint8 **img, vuint8 **img_simd){

    for (int i = vmi0; i <= vmi1; ++i)
    {
        for (int j = vmj0; j <= vmj1; ++j)
        {

            img_simd[i][j] = init_vuint8_all(    img[i][(j * card) + 0 ], img[i][(j * card) + 1 ], img[i][(j * card) + 2 ], img[i][(j * card) + 3 ], 
                                                img[i][(j * card) + 4 ], img[i][(j * card) + 5 ], img[i][(j * card) + 6 ], img[i][(j * card) + 7 ], 
                                                img[i][(j * card) + 8 ], img[i][(j * card) + 9 ], img[i][(j * card) + 10], img[i][(j * card) + 11], 
                                                img[i][(j * card) + 12], img[i][(j * card) + 13], img[i][(j * card) + 14], img[i][(j * card) + 15]);
        } 
    }
}

void vui8matrix_to_ui8matrix_morpho(int card, int vmi0, int vmi1, int vmj0, int vmj1, uint8** img, vuint8** img_simd){

	for (int i = vmi0; i <= vmi1; ++i)
	{
		for (int j = vmj0; j <= vmj1; ++j)
		{

            img[i][j * card + 0 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 0 ));
            img[i][j * card + 1 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 1 ));
            img[i][j * card + 2 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 2 ));
            img[i][j * card + 3 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 3 ));
            img[i][j * card + 4 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 4 ));
            img[i][j * card + 5 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 5 ));
            img[i][j * card + 6 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 6 ));
            img[i][j * card + 7 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 7 ));
            img[i][j * card + 8 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 8 ));
            img[i][j * card + 9 ] = (uint8)(_mm_extract_epi8(img_simd[i][j], 9 ));
            img[i][j * card + 10] = (uint8)(_mm_extract_epi8(img_simd[i][j], 10));
            img[i][j * card + 11] = (uint8)(_mm_extract_epi8(img_simd[i][j], 11));
            img[i][j * card + 12] = (uint8)(_mm_extract_epi8(img_simd[i][j], 12));
            img[i][j * card + 13] = (uint8)(_mm_extract_epi8(img_simd[i][j], 13));
            img[i][j * card + 14] = (uint8)(_mm_extract_epi8(img_simd[i][j], 14));
            img[i][j * card + 15] = (uint8)(_mm_extract_epi8(img_simd[i][j], 15));
		} 
	}

}

// conversion avec bord
void ui8matrix_to_vui8matrix_wb_morpho(int card, int vmi0b, int vmi1b, int vmj0b, int vmj1b, uint8 **img, vuint8 **img_simd){

    for (int i = vmi0b; i <= vmi1b; ++i)
    {
        for (int j = vmj0b; j <= vmj1b; ++j)
        {
            if (j == vmj0b)
            {
                img_simd[i][j] = init_vuint8(img[i][j]);
            }
            else if (j == vmj1b)
            {
                img_simd[i][j] = init_vuint8(img[i][j*card]);
            }
            else{
                img_simd[i][j] = init_vuint8_all(    img[i][(j * card) + 0 ], img[i][(j * card) + 1 ], img[i][(j * card) + 2 ], img[i][(j * card) + 3 ], 
                                                    img[i][(j * card) + 4 ], img[i][(j * card) + 5 ], img[i][(j * card) + 6 ], img[i][(j * card) + 7 ], 
                                                    img[i][(j * card) + 8 ], img[i][(j * card) + 9 ], img[i][(j * card) + 10], img[i][(j * card) + 11], 
                                                    img[i][(j * card) + 12], img[i][(j * card) + 13], img[i][(j * card) + 14], img[i][(j * card) + 15]);
            }
        } 
    }

}

void main_test_morpho_SIMD(int argc, char *argv[])
{
    test_unitaire_SIMD(MORPHO3, PIPELINE_FUSION);
    //test_unitaire_SIMD(DILATATION5, SIMD_UNROLL);
    //tests_unitaires_SIMD();
    
}
