/* -------------- */
/* --- main.c --- */
/* -------------- */

#include <stdio.h>
#include <stdlib.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "simd1D.h"
#include "simd2D.h"

#include "mouvement_SIMD.h"
#include "test_mouvement.h"
#include "test_mouvement_morpho.h"
#include "test_morpho.h"

void info(void)
{
#ifdef ENABLE_BENCHMARK
    puts("mode Benchmark ON");
    puts("DEBUG OFF");
#else
    puts("mode Benchmark OFF");
    puts("DEBUG ON");
#endif
}

int main(int argc, char *argv[])
{
    info();

    // main_test_mouvement(argc,argv);
    main_test_mouvement_morpho(argc, argv);
    // main_test_morpho(argc, argv);
    // main_mouvement_simd(argc, argv);

    return 0;   
}