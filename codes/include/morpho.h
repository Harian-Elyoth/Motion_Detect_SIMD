/* ----------------------------- */
/* ------- morpho.h -------- */
/* ----------------------------- */

#ifndef __MORPHO_H__
#define __MORPHO_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif


void erosion_3(uint8 ** X, uint8 ** Y);
void erosion_5(uint8 ** X, uint8 ** Y);

void dilatation_3(uint8 ** X, uint8 ** Y);
void dilatation_5(uint8 ** X, uint8 ** Y);

void morpho_3(uint8 ** X, uint8 ** Y);
void morpho_5(uint8 ** X, uint8 ** Y);


#ifdef __cplusplus
}
#endif

#endif