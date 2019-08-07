#include "debug.h"
#include "stdarg.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"

// 与printf的用法一样
// 输出格式化字符串到通讯显示屏幕上
void print_info(char *str, ...)
{
 //    uint16_t len;
 //    uint8_t buf[50];
 //    va_list ap;
 //    va_start(ap, str);
 //    vsprintf((char *)buf, str, ap);
 //    va_end(ap);
 //    len = strlen((char *)buf);
 //   Send_InfoData_To_Fifo(buf, len);
}

void Dump_Array(uint8_t *name, uint8_t *array, uint8_t length)
{
    print_info("%s:", name);
    for (uint8_t i = 0; i < length; i++)
    {
        print_info("0x%02X ", array[i]);
        delay_ms(10);
    }
    print_info("\r\n");
}

// 初始化调试用引脚
// 操作引脚电平高低即可通过示波器或者LED观察到运行状态、中断进入情况、
// 中断或者任务处理时间等，请善用此工具
void DebugPin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    DEBUG_PIN_1_RESET();
    DEBUG_PIN_2_RESET();
    DEBUG_PIN_3_RESET();
}
