#if !defined(_DISPLAY_H_)
#define _DISPLAY_H_

#include "sys.h"

void Display_Init(void);
void Display_Control(FunctionalState status);
void Display_OutPutBuffer(uint16_t x);
void Display_Color(uint8_t color);
void Display_AutoDisplay(void);
void Display_CLS(void);

#endif // _DISPLAY_H_
