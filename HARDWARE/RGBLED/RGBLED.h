#if !defined(_RGBLED_H_)
#define _RGBLED_H_

#include "sys.h"

void RGBLED_Init(uint32_t initColor);
void RGBLED_SetRGB(uint8_t R, uint8_t G, uint8_t B);
void RGBLED_Set(uint32_t clolor);

#endif // _RGBLED_H_
