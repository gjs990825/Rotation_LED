/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "ledarray.h"
#include "display.h"
#include "debug.h"

// 指定行显示两条线
void BasicTask_1(uint8_t line1, uint8_t line2)
{
    uint16_t outPutData = 0;

    outPutData |= 1 << line1;
    outPutData |= 1 << line2;
    Display_Control(DISABLE);
    LEDArray_Out(outPutData);
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
            LEDArray_Out(outPutData);
            delay(100);
        }
    }
}

// 独立显示两个全亮16*16点阵，间隔点为4
void BasicTask_3(void)
{
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
void BasicTask(uint8_t line1, uint8_t line2)
{
    BasicTask_1(line1, line2);
    BasicTask_2();
    BasicTask_3();
    BasicTask_4();
    Display_CLS();
}

// 提高任务
void PromotedTask(void)
{
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();
    USART1_Init(115200);
    printf("Usart OK\r\n");


    DebugPin_Init();
    LEDArray_Init();
    Display_Init();

    Display_Color(0x7F);

    TIM_Cmd(TIM3, ENABLE);

    while (1)
    {
        // DEBUG_PIN_3_TOGGLE();
    }
}

void assert_failed(uint8_t *file, uint32_t line)
{
    while (1)
    {
        printf("File: %s, line: %d\r\n", file, line);
        delay_ms(500);
    }
}
