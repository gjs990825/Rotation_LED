/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include <stdio.h>
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "servo.h"
#include "RGBLED.h"
#include "iic.h"
#include "mpu6050.h"
#include "ads1115.h"
#include "FDC2214.h"
#include "oled.h"
#include "spi.h"
#include "gui.h"
#include "image.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();
    USART1_Init(115200);
    printf("Usart OK\r\n");

    LED_Init();

    HardwareSPI_Init();
    OLED_Init();

    OLED_DrawBMP(0, 0, 128, 8, (uint8_t *)guaiqiao);

    // uint8_t str[10];
    while (1)
    {
        // PeriodicTask(500, LED_Blink(0));
        // GUI_RefreashInterface(componentsSet, componentsNumber);
        // sprintf((char *)str, "%5.2f", millis() / 1000.0);
        // OLED_ShowStr(0, 0, str, Font_12x24, false);
        // delay(10);
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
