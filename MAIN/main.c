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
        Check_USARTMessage();

        // LEDArray_OutHex(0);

        // for (uint8_t i = 0; i < 16; i++)
        // {
        //     LEDArray_OutHex(1 << i);
        //     delay(300);
        //     printf("%d\r\n", i);
        // }
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
