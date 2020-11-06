/*
===============================================================
==================== Test SD + Morpho =========================
===============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"

#include "mouvement.h"
#include "morpho.h"
#include "test_mouvement_morpho.h"


void test_mouvement_morpho(){

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;

    char *format = "%d ";
    int kernel_size = 3;

    // alloue les matrices images, moyennes, ecart-types, diff, binaire
	allocate_matrix(kernel_size);

	int count = 3000;

	for (int i = 1; i < NB_IMG ; ++i)
	{

		char filename0[25] = "";

		snprintf(filename0, 25, "../car3/car_%d.pgm", count);

		count += 1;

		char filename1[25] = "";

		snprintf(filename1, 25, "../car3/car_%d.pgm", count);

		// CHARGER LES IMAGES PUIS FAIRE TRAITEMENTS

		MLoadPGM_ui8matrix(filename0, mi0b, mi1b, mj0b, mj1b, image0);
		MLoadPGM_ui8matrix(filename1, mi0b, mi1b, mj0b, mj1b, image1);

		// Fait uniquement au 1er tour de boucle	
		if (i == 1){

			// initiate mean0 et std0
			for (int i = mi0b; i <= mi1b; ++i)
			{
				for (int j = mj0b; j <= mj1b; ++j)
				{
					mean0[i][j] = image0[i][j];
					std0[i][j]  = VMIN;
				}
			}
		}

		duplicate_border();

		CHRONO(SigmaDelta(),cycles);

		BENCH(printf("SIGMA DELTA : "));
		BENCH(printf("it = %d, cycles/X*Y = %0.6f", i, cycles/(WIDTH * HEIGHT))); BENCH(puts(""));

		// DEBUG(display_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b, format, "img_bin : ")); DEBUG(puts(""));

		CHRONO(morpho_3(img_bin, img_filtered, mi0, mj0, mi1, mj1),cycles);

		// DEBUG(display_ui8matrix(img_filtered, mi0b, mi1b, mj0b, mj1b, format, "img_filtered : ")); DEBUG(puts(""));

		BENCH(printf("MORPHOLOGIE : "));
		BENCH(printf("it = %d, cycles/X*Y = %0.6f", i, cycles/(WIDTH * HEIGHT))); BENCH(puts(""));

		// built pgm filename out
		char filename_out[25] = "";
		snprintf(filename_out, 25, "SD_out_%d.pgm", i);

		filtered_to_pgm(filename_out);
	}

	// free all matrix
	void free_matrix();
}

void main_test_mouvement_morpho(int argc, char *argv[])
{
	DEBUG(test_mouvement_morpho());
	BENCH(test_mouvement_morpho());
}