#include "task.h"
#include "ledarray.h"
#include "display.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"
#include "display.h"

// 指定行显示两条线
void BasicTask_1(uint8_t line1, uint8_t line2)
{
    Display_Color(0xFF);
    Display_CLS();

    uint16_t outPutData = 0;

    outPutData |= 1 << line1;
    outPutData |= 1 << line2;

    for (uint8_t i = 0; i < (LandscapePixelNumber / 3); i++)
    {
        Display_WriteARow_Hex(outPutData, i);
    }

    Display_Control(ENABLE);
    delay(5000);
    Display_Control(DISABLE);
    Display_CLS();
}

// 水平亮线上下往复
void BasicTask_2(void)
{
    Display_Color(0);
    Display_CLS();
    Display_Control(ENABLE);

    uint16_t outPutData = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            outPutData = 0;
            outPutData |= 1 << (15 - j);
            outPutData |= 1 << j;

            // 等待间隔区间写数据以防断线
            while (!Display_IsInterval())
                ;

            for (uint8_t k = 0; k < (LandscapePixelNumber / 3); k++)
            {
                Display_WriteARow_Hex(outPutData, k);
            }
            delay(100);
        }
    }

    Display_Control(DISABLE);
    delay(500);
}

// 独立显示两个全亮16*16点阵，间隔点为4
void BasicTask_3(void)
{
    Display_CLS();
    Display_Color(0xFF);

    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            displayBuffer[j + (i * (16 + 4))] = 0xFFFF;
        }
    }

    Display_Control(ENABLE);
    delay(5000);
}

// 对矩形水平宽窄变换循环显示，宽窄变化不小于两个矩阵点距离，变化不少于5次
void BasicTask_4(void)
{
    Display_Color(0);

    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint8_t j = 1; j <= 50; j++)
        {
            Display_Scaling(1 + 0.04 * j);
            delay(50);
        }
        for (int8_t j = 49; j >= 0; j--)
        {
            Display_Scaling(1 + 0.04 * j);
            delay(50);
        }
    }

    Display_Scaling(1);

    Display_Control(DISABLE);
    Display_CLS();
    delay(500);
}

void Show_ImgData(uint16_t *img[16])
{
    Display_Color(0xFF);

    Display_CLS();

    for (uint8_t j = 0; j < 3; j++)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            Display_WriteARow_Hex(img[j][i], (j * (16 + 3)) + i);
        }
    }

    Display_Control(ENABLE);
}

// 按顺序在120度弧面内同时显示（三个点阵点间隔）接收到的图文信息
void PromotedTask_1(uint16_t *img[16])
{
    Show_ImgData(img);
    delay(8000);
}

// 使图文在显示的同时实现红色绿色橙黄色交替变色显示，变色样式不少于5种
void PromotedTask_2(uint16_t *img[16])
{
    Show_ImgData(img);
    for (uint8_t j = 0; j < 3; j++)
    {
        for (uint16_t i = 0; i <= 0xFF; i++)
        {
            Display_Color(i);
            delay(10);
        }
    }
    Display_Color(0xFF);
}

// 其它功能
void PromotedTask_3(void)
{
    Clock_Display();
}

// 显示上电之后的运行时间
// 串口接收到任意信息后退出
void Clock_Display(void)
{
    Display_Color(0xFF);

    const int clock2_offset = LandscapePixelNumber / 2;

    Display_Control(ENABLE);
    Display_CLS();

    // 两面显示
    Display_ShowStr(0, 0, "00H 00M 00S 00", Font_8x16, false);
    Display_ShowStr(0 + clock2_offset, 0, "00H 00M 00S 00", Font_8x16, false);

    // 时间记录
    uint32_t h = 0, m = 0, s = 0;
    // 时间字符显示缓冲
    uint8_t strH[3], strM[3], strS[3], strMS[3];

    USART_CLR_REC();

    for (;;)
    {
        if (USART_RECEIVED)
        {
            Display_CLS();
            break;
        }

        static uint32_t tms;
        if (!IsTimeOut(tms, 10))
            continue;

        tms = millis();
        sprintf((char *)strMS, "%02d", tms % 1000 / 10);
        Display_ShowStr(96, 0, strMS, Font_8x16, 0);
        Display_ShowStr(96 + clock2_offset, 0, strMS, Font_8x16, 0);

        uint32_t ts = tms / 1000;
        if (s != ts)
        {
            s = ts;
            sprintf((char *)strS, "%02d", s % 60);
            Display_ShowStr(64, 0, strS, Font_8x16, false);
            Display_ShowStr(64 + clock2_offset, 0, strS, Font_8x16, false);

            uint32_t tm = ts / 60;
            if (m != tm)
            {
                m = tm;
                sprintf((char *)strM, "%02d", m % 60);
                Display_ShowStr(32, 0, strM, Font_8x16, false);
                Display_ShowStr(32 + clock2_offset, 0, strM, Font_8x16, false);

                uint32_t th = ts / 3600;
                if (h != th)
                {
                    h = th;

                    sprintf((char *)strH, "%02d", h % 24);
                    Display_ShowStr(0, 0, strH, Font_8x16, false);
                    Display_ShowStr(0 + clock2_offset, 0, strH, Font_8x16, false);
                }
            }
        }
    }
}

#define WaitIntervalIs(sta)                                      \
    do                                                           \
    {                                                            \
        while (Display_IsInterval() != sta)                      \
        {                                                        \
            if (USART_RECEIVED || IsTimeOut(startTime, timeout)) \
                goto END;                                        \
        }                                                        \
    } while (0)

#define ENDLABLE()            \
    END:                      \
    Display_Control(DISABLE); \
    Display_CLS();            \
    return

// 转动的斜线
// 串口接收到消息或者运行一段时间后自动退出
void Running_Slashes(uint32_t timeout)
{
    Display_Color(0);
    Display_Control(ENABLE);

    uint32_t startTime = millis();

    for (;;)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            WaitIntervalIs(true);

            for (uint16_t i = 0; i < LandscapePixelNumber; i++)
            {
                Display_WriteARow_Hex(1 << (i + j) % 16, i);
            }

            WaitIntervalIs(false);
        }
    }

    ENDLABLE();
}

// 输出字幕（未显示区域不会更新图像）
void Display_OutputSubs(int16_t weight, uint16_t hight, const uint8_t *subs, int16_t offset)
{
    int16_t realPos;

    for (int16_t i = 0; i < weight; i++)
    {
        for (uint8_t j = 0; j < hight; j++)
        {
            realPos = i + offset;
            if (i < 0)
                continue;

            Display_WriteARow_Byte(subs[i + weight * j], (j == 1), realPos);
        }
    }
}

// 循环滚动显示字幕
void Subs_ScrollDisplay(int16_t weight, uint16_t hight, const uint8_t *subs, uint32_t timeout)
{
#define _SUBS_INTERVAL_ 16

    uint32_t startTime = millis();
    uint16_t aCircle = weight + _SUBS_INTERVAL_;

    // 满足循环显示需要重复的个数
    uint8_t number = LandscapePixelNumber / aCircle + 1;
    // 最少需要两个形成图形循环
    if (number == 1)
        number = 2;

    Display_Control(ENABLE);

    for (;;)
    {
        for (int16_t j = 0; j > -aCircle; j--)
        {
            WaitIntervalIs(false);

            for (uint8_t i = 0; i < number; i++)
            {
                Display_OutputSubs(weight, hight, subs, j + i * aCircle);
            }

            WaitIntervalIs(true);
        }
    }

    ENDLABLE();
}

void Video_Play(uint16_t width, uint8_t hight, uint16_t frames, const uint8_t *video, uint32_t timeout)
{
    bool whileParam;
    uint32_t startTime;

    if (timeout == 0)
    {
        // 播放一次
        whileParam = 0;
        startTime = 0x7FFFFFFF;
    }
    else
    {
        whileParam = 1;
        startTime = millis();
    }

#define _VIDEO_INTERVAL_ 16

    uint8_t number = LandscapePixelNumber / (width + _VIDEO_INTERVAL_);

    Display_Control(ENABLE);

    do
    {
        for (uint16_t f = 0; f < frames; f++)
        {
            WaitIntervalIs(true);

            for (uint16_t i = 0; i < width; i++)
            {
                for (uint16_t j = 0; j < hight; j++)
                {
                    for (uint8_t k = 0; k < number; k++)
                    {
                        Display_WriteARow_Byte(video[(f * width * hight) + (i + j * width)],
                                               (j == 1),
                                               i + k * (width + _VIDEO_INTERVAL_));
                    }
                }
            }

            WaitIntervalIs(false);
        }
    } while (whileParam);

    ENDLABLE();
}
