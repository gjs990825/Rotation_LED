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
#include "chs_character.h"

int main(void)
{
    // 系统初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    Timestamp_Init();

    // 串口初始化
    USART1_Init(115200);
    printf("Usart OK\r\n");

    // 初始化LED线阵和红外反馈
    LEDArray_Init();
    // 初始化显示刷新和速度检测
    Display_Init();

    // 等待运行稳定
    Display_WaitTillStabilized();

    // 显示运动的斜线
    Running_Slashes(3000);

    // 显示字幕
    Subs_ScrollDisplay(392, 2, chs_character, 10000);

    // 上次处理串口消息的时间
    uint32_t lastProcess = 0;

    while (1)
    {
        // 检查串口消息
        if (Check_USARTMessage())
        {
            lastProcess = millis();
        }

        // 数秒内无动作进入计时模式，任意操作退出
        if (IsTimeOut(lastProcess, 2000))
        {
            Clock_Display();
        }
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
