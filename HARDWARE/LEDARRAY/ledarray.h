#if !defined(_LEDARRAY_H_)
#define _LEDARRAY_H_

#include "sys.h"

#define Infrared_In PAin(2)
#define Infrared_IsTiggered() (Infrared_In)

void LEDArray_Init(void);
void LEDArray_Out(uint16_t ledStatus);
void LEDArray_ALLOFF(void);
void LEDArray_ALLON(void);
void LEDArray_Color(uint8_t clolor);

#endif // _LEDARRAY_H_
