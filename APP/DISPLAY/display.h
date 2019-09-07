#if !defined(_DISPLAY_H_)
#define _DISPLAY_H_

#include "sys.h"

// 每个显示周期的分段个数
#define LandscapePixelNumber 256

typedef struct fontInfo
{
    uint8_t *data; // 字体数据指针（二维强转一维即可）
    uint8_t xSize; // 字体宽像素个数
    uint8_t ySize; // 字体高度字节数
} fontInfo_t;

// 字体
extern fontInfo_t Font_6x8, Font_8x16;

// 缓冲
extern uint16_t displayBuffer[LandscapePixelNumber];

// 初始化

void Display_Init(void);

// 控制

void Display_Control(FunctionalState status);
void Display_Color(uint8_t color);
void Display_Scaling(float scale);
void Display_CLS(void);

// 输出

void Display_OutputBuffer(uint16_t x);
void Display_WriteARow(bool pixel[16], uint16_t x);
void Display_WriteARow_Hex(uint16_t HEXData, uint16_t x);

// 转换工具

uint16_t Display_ArrayToHex(bool pixArray[16]);
void Display_HexToArray(uint16_t hex, bool *pixArray);

// 获取数据或状态

bool Display_IsInterval(void);
bool Display_IsStablized(void);
void Display_PrintBuffer(void);

// 外部调用接口

void Display_InterruptHandle(void);
void Display_WaitTillStabilized(void);
void Display_UnstableHandle(void);

// 调试工具

void Display_FillWith(uint16_t data);

// 文字显示

void Display_ShowStr(uint16_t x, uint8_t y, uint8_t ch[], fontInfo_t font, bool isHighlight);

#endif // _DISPLAY_H_
