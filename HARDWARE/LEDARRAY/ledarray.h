#if !defined(_LEDARRAY_H_)
#define _LEDARRAY_H_

#include "sys.h"

// LED阵列是否在同一端口
#define _LED_IN_DIFFERENT_PORT_ 1

#define Infrared_In PCin(13)
#define Infrared_IsTiggered() (Infrared_In)

void LEDArray_Init(void);
void LEDArray_OutHex(uint16_t ledStatus);
void LEDArray_OutArray(bool array[16]);
void LEDArray_ALLOFF(void);
void LEDArray_ALLON(void);
void LEDArray_Color(uint8_t clolor);

#endif // _LEDARRAY_H_
