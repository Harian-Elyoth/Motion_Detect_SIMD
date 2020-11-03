/* --------------------------------------------------------------- */
/* ---  Test Algorithme Sigma Delta pour le traitement d'image --- */
/* --------------------------------------------------------------- */

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
#include "test_mouvement.h"

void test_SD_car(int is_test){

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;

	char *format = "%d ";
    double cycles;
    int kernel_size = 3;

	// alloue les matrices images, moyennes, ecart-types, diff, binaire
	allocate_matrix(kernel_size);

	// charge un couple d'images du set car3 dans image0 et image1
	load_imgs();

	// affiche les images initiales avec bord vide
	DEBUG(display_ui8matrix(image0, mi0b, mi1b, mj0b, mj1b, format, "1ere image (bord a 0) : ")); DEBUG(puts(""));
	DEBUG(display_ui8matrix(image1, mi0b, mi1b, mj0b, mj1b, format, "2eme image (bord a 0) : ")); DEBUG(puts(""));

	duplicate_border();

	// affiche les images initiales avec bord dupliques
	DEBUG(display_ui8matrix(image0, mi0b, mi1b, mj0b, mj1b, format, "1ere image : ")); DEBUG(puts(""));
	DEBUG(display_ui8matrix(image1, mi0b, mi1b, mj0b, mj1b, format, "2eme image : ")); DEBUG(puts(""));

	CHRONO(SigmaDelta(),cycles);

	BENCH(printf("cycles = %0.6f", cycles)); BENCH(puts(""));

	BENCH(printf("cycles/X*Y = %0.6f", cycles/(WIDTH*HEIGHT))); BENCH(puts(""));

	// affiche l'image binaire resultante
	DEBUG(display_ui8matrix(img_bin, mi0b, mi1b, mj0b, mj1b, format, "image binaire : ")); DEBUG(puts(""));

	bin_to_pgm("SD_out.pgm");

	// free all matrix
	void free_matrix();
}

void test_SD_dataset(){

	// chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;
    int kernel_size = 3;

    char *format = "%d ";

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

		// DEBUG(printf("file 0 : %s\n", filename0));
		// DEBUG(printf("file 1 : %s\n", filename1));

		// CHARGER LES IMAGES PUIS FAIRE TRAITEMENTS

		MLoadPGM_ui8matrix(filename0, mi0b, mi1b, mj0b, mj1b, image0);
		MLoadPGM_ui8matrix(filename1, mi0b, mi1b, mj0b, mj1b, image1);

		if (i == 1){

			// initiate mean0 et std0
			for (int i = mi0b; i < mi1b; ++i)
			{
				for (int j = mj0b; j < mj1b; ++j)
				{
					mean0[i][j] = image0[i][j];
					std0[i][j]  = VMIN;
				}
			}
		}

		duplicate_border();

		CHRONO(SigmaDelta(),cycles);

		BENCH(printf("it : %d, cycles/X*Y = %0.6f", i, cycles/(WIDTH*HEIGHT))); BENCH(puts(""));

		// built pgm filename out
		char filename_out[25] = "";
		snprintf(filename_out, 25, "SD_out_%d.pgm", i);

		bin_to_pgm(filename_out);
	}

	// free all matrix
	void free_matrix();
}


void main_test_mouvement(int argc, char *argv[])
{
	DEBUG(test_SD_dataset());

	BENCH(test_SD_dataset());	
}