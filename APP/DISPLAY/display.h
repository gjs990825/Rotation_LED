#if !defined(_DISPLAY_H_)
#define _DISPLAY_H_

#include "sys.h"

// 每个显示周期的分段个数
#define LandscapePixelNumber 90

// 缓冲

extern bool displayBuffer[][16];

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
void Display_WriteARow_Hex(uint16_t row, uint16_t x);

// 转换工具

uint16_t Display_ArrayToHex(bool pixArray[16]);
void Display_HexToArray(uint16_t hex, bool *pixArray);

// 获取数据或状态

bool Display_IsStablized(void);
void Display_PrintBuffer(void);

// 外部调用接口

void Display_InterruptHandle(void);
void Display_WaitTillStabilized(void);

#endif // _DISPLAY_H_
