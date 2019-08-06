#if !defined(_OLED_H_)
#define _OLED_H_

#include "sys.h"

#define __OLED_SPI__ 1

#if !__OLED_SPI__

#define OLE_ADRESS 0x78

#endif

typedef struct fontInfo
{
    uint8_t *data; // 字体数据指针（二维强转一维即可）
    uint8_t xSize; // 字体宽像素个数
    uint8_t ySize; // 字体高度字节数
} fontInfo_t;

// 字体
extern fontInfo_t Font_6x8, Font_6x12, Font_8x16, Font_12x24;

void OLED_Init(void);
void OLED_Fill(uint8_t fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, fontInfo_t font, bool isHightlight);
void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], fontInfo_t font, bool isHighlight);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);


#endif // _OLED_H_
