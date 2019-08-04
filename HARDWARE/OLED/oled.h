#if !defined(_OLED_H_)
#define _OLED_H_

#include "sys.h"

#define OLE_ADRESS 0x78

void OLED_WriteCmd(uint8_t cmd);
void OLED_WriteDat(uint8_t dat);
void OLED_Init(void);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_Fill(uint8_t fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize, bool isHighlight);
// void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize);
void OLED_ShowCN(uint8_t x, uint8_t y, uint8_t number);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);

#endif // _OLED_H_
