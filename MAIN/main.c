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
#include "communication.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();
    USART1_Init(115200);
    printf("Usart OK\r\n");

    LEDArray_Init();
    Display_Init();

    Display_WaitTillStabilized();
    

    while (1)
    {
        // BasicTask_All(7, 8);
        Check_USARTMessage();

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
