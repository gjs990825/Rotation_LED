/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "servo.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();
    USART1_Init(115200);
    printf("Usart OK\r\n");
    // 基本外设初始化

    LED_Init();

    Servo_Init(0.0, 180);

    for (;;)
    {
        for (size_t i = 0; i < 180; i++)
        {
            Servo_To(i);
            delay(10);
        }
        for (size_t i = 180; i > 0; i--)
        {
            Servo_To(i);
            delay(10);
        }
    }

    while (1)
    {
        PeriodicTask(500, LED_Blink(0));
        PeriodicTask(1000, LED_Blink(2));
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
