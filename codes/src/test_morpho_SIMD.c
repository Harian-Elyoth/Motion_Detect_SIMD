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
#include "morpho_SIMD.h"

void main_test_morpho_SIMD(int argc, char const *argv[])
{
    vuint8 ** X = vui8matrix(-1, 24, -1, 32);
    vuint8 ** Y = vui8matrix(-1, 24, -1, 32);
    DEBUG(display_vui8matrix(X, -1, 24, -1, 32, "%d ", "X : "));
    erosion_3_SIMD(X, Y, 0, 0, 24, 32);

}
