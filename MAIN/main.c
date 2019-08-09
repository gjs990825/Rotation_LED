/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "ledarray.h"
#include "display.h"
#include "debug.h"
#include "task.h"

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

    LEDArray_OutHex(0xFFFF);

    TIM_Cmd(TIM3, ENABLE);

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

    while (1)
    {
        // for (uint16_t i = 0; i < 64; i++)
        // {
        //     LEDArray_OutArray(displayBuffer[i]);
        //     delay(100);
        // }
        
        // BasicTask_1(2, 13);
        // BasicTask_2();

        // LEDArray_OutHex(0);
        // BasicTask_3();
        // Display_CLS();
        // delay(1000);

        // for (uint8_t i = 0; i < 16; i++)
        // {
        //     LEDArray_OutHex(1 << i);
        //     delay(300);
        //     printf("%d\r\n", i);
        // }
        // PeriodicTask(500, printf("2333\r\n"));
        // PeriodicTask(500, Display_Color())
        // delay(500);
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
