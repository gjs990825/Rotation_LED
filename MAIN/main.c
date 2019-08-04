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

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();
    USART1_Init(115200);
    printf("Usart OK\r\n");

    LED_Init();

    I2C_Soft_Init();

    // 下面依次是 OLED / MPU6050 / FDC2214 / 舵机 / RGBLED 的初始化
    // OLED写了简单的GUI可以通过定时器定期刷新的方式显示，可定制显示控件 感兴趣可以自己研究
    // ADS1115 不需要初始化直接调用读取函数

    OLED_Init();
    MPU6050_Init();

    bool capOk = FDC2214_begin(0xF, 0x6, 0x5, false);
    //setup all four channels, autoscan with 4 channels, deglitch at 10MHz, external oscillator
    printf("Sensor %s\r\n", capOk ? "OK" : "Fail");

    Servo_Init(0.0, 180);
    RGBLED_Init(0x7F7F7F);

    while (1)
    {
        PeriodicTask(500, LED_Blink(0));
    }
}

void ServoTest(void)
{
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
}

void RGBLED_Test(void)
{
    for (;;)
    {
        RGBLED_Set(0xFF0000);
        delay(500);
        RGBLED_Set(0xFFFF00);
        delay(500);
        RGBLED_Set(0x00FF00);
        delay(500);
        RGBLED_Set(0x00FFFF);
        delay(500);
        RGBLED_Set(0x0000FF);
        delay(500);
        RGBLED_Set(0xFF00FF);
        delay(500);
        RGBLED_Set(0x800000);
        delay(500);
        RGBLED_Set(0x808000);
        delay(500);
        RGBLED_Set(0x008000);
        delay(500);
        RGBLED_Set(0x008080);
        delay(500);
        RGBLED_Set(0x000080);
        delay(500);
        RGBLED_Set(0x800080);
        delay(500);
        RGBLED_Set(0xFFFFFF);
        delay(500);
        RGBLED_Set(0xC0C0C0);
        delay(500);
        RGBLED_Set(0x808080);
        delay(500);
        RGBLED_Set(0x000000);
        delay(500);
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
