#include "task.h"
#include "ledarray.h"
#include "display.h"
#include "delay.h"
#include "stdio.h"

// 指定行显示两条线
void BasicTask_1(uint8_t line1, uint8_t line2)
{
    uint16_t outPutData = 0;

    outPutData |= 1 << line1;
    outPutData |= 1 << line2;
    Display_Control(DISABLE);
    LEDArray_OutHex(outPutData);
    delay(1000);
}

// 水平亮线上下往复
void BasicTask_2(void)
{
    uint16_t outPutData = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            outPutData = 0;
            outPutData |= 1 << (15 - j);
            outPutData |= 1 << j;
            LEDArray_OutHex(outPutData);
            delay(100);
        }
    }
}

// 独立显示两个全亮16*16点阵，间隔点为4
void BasicTask_3(void)
{
    // Display_PrintBuffer();

    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            for (uint8_t k = 0; k < 16; k++)
            {
                displayBuffer[j + (i * (16 + 4))][k] = 1;
            }
        }
    }

    // Display_PrintBuffer();

    delay(3000);
}

// 对矩形水平宽窄变换循环显示，宽窄变化不小于两个矩阵点距离，变化不少于5次
void BasicTask_4(void)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        for (uint8_t j = 0; j < 10; j++)
        {
            Display_Scaling(1 + 0.01 * j);
            delay(100);
        }
        for (int8_t j = 9; j >= 0; j--)
        {
            Display_Scaling(1 + 0.01 * j);
            delay(100);
        }
    }
}

// 基础任务
void BasicTask_All(uint8_t line1, uint8_t line2)
{
    BasicTask_1(line1, line2);
    BasicTask_2();
    BasicTask_3();
    BasicTask_4();
    Display_CLS();
}

// 按顺序在120度弧面内同时显示（三个点阵点间隔）接收到的图文信息
void PromotedTask_1(uint16_t img1[16], uint16_t img2[16], uint16_t img3[16])
{
    bool pixel[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        Display_HexToArray(img1[i], pixel);
        Display_WriteARow(pixel, ((16 + 3) * 0) + i);
    }
    for (uint8_t i = 0; i < 16; i++)
    {
        Display_HexToArray(img2[i], pixel);
        Display_WriteARow(pixel, ((16 + 3) * 1) + i);
    }
    for (uint8_t i = 0; i < 16; i++)
    {
        Display_HexToArray(img3[i], pixel);
        Display_WriteARow(pixel, ((16 + 3) * 2) + i);
    }

    delay(8000);
}

// 使图文在显示的同时实现红色绿色橙黄色交替变色显示，变色样式不少于5种
void PromotedTask_2(void)
{
}

// 其它（花里胡哨就完事了）
void PromotedTask_3(void)
{
}

// 发挥部分任务
void PromotedTask_All(uint16_t img1[16], uint16_t img2[16], uint16_t img3[16])
{
    uint16_t PIXEL[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        PIXEL[i] = 0xAAAA;
    }

    Display_CLS();
    Display_Control(ENABLE);
    PromotedTask_1(PIXEL, PIXEL, PIXEL);
    PromotedTask_2();
    PromotedTask_3();
    Display_Control(DISABLE);
}
