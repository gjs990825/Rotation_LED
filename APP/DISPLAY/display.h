#if !defined(_DISPLAY_H_)
#define _DISPLAY_H_

#include "sys.h"

extern bool displayBuffer[][16];

void Display_Init(void);
void Display_Control(FunctionalState status);
void Display_OutputBuffer(uint16_t x);
void Display_Color(uint8_t color);
void Display_AutoDisplay(void);
void Display_CLS(void);
void Display_Scaling(float scale);
uint16_t Display_ArrayToHex(bool pixArray[16]);
void Display_HexToArray(uint16_t hex, bool *pixArray);
void Display_WriteARow(bool pixel[16], uint16_t x);

void Display_PrintBuffer(void);

void Display_InterruptHandle(void);

#endif // _DISPLAY_H_
