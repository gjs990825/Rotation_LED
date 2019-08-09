#include "display.h"
#include "ledarray.h"
#include "delay.h"
#include "string.h"
#include "debug.h"
#include "stdio.h"
#include "stdlib.h"

// 控制常量↓

// 每个显示周期的分段个数
#define LandscapePixelNumber 120
// 显示周期占总周期的比例
const float DisplayCycleToFullCycleProportion = 120.0 / 360.0;
// 像素间隔时间
const int PixelSpacingInterval = 1;

// 控制变量↓

// 缩放比例控制
float scalingRatio = 1.0;

// 旋转时间计数
__IO uint32_t rotationCounter = 0;
// 当前单个像素像素时间周期
__IO uint16_t currentPixelCycle = 0;

// 当前帧计数
uint32_t frameCounter = 0;

bool displayBuffer[LandscapePixelNumber][16];

// 显示初始化
void Display_Init(void)
{
    uint16_t arr = 71;
    uint16_t psc = 9;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, DISABLE);

    // 清空缓冲数据
    Display_CLS();
}

// 清空数据
void Display_CLS(void)
{
    memset(displayBuffer, 0, sizeof(displayBuffer));
}

// 控制显示定时器
void Display_Control(FunctionalState status)
{
    TIM_Cmd(TIM3, status);
}

// 从数组到十六进制
uint16_t Display_ArrayToHex(bool pixArray[16])
{
    uint16_t outData = 0;
    for (uint8_t i = 0; i < 16; i++)
    {
        outData |= (pixArray[i]) ? (1 << i) : 0;
    }
    return outData;
}

// 从十六进制到数组
void Display_HexToArray(uint16_t hex, bool *pixArray)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        pixArray[i] = (hex >> i) & 0x1;
    }
}

// 写入一列
void Display_WriteARow(bool pixel[16], uint16_t x)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        displayBuffer[x][i] = pixel[i];
    }
}

// 输出前120列的缓存
void Display_PrintBuffer(void)
{
    for (uint8_t i = 0; i < LandscapePixelNumber; i++)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            printf("%d", displayBuffer[i][j]);
        }
        printf("\r\n");
    }
}

// 输出第x列的数据
static void Display_OutputBuffer(uint16_t x)
{
    // 防越界
    if (x >= LandscapePixelNumber)
        return;

    LEDArray_OutArray(displayBuffer[x]);
    // LEDArray_OutHex(Display_ArrayToHex(displayBuffer[x]));
}

// 变更显示颜色
void Display_Color(uint8_t color)
{
    LEDArray_Color(color);
}

// 变更显示横向缩放
void Display_Scaling(float scale)
{
    scalingRatio = scale;
}

// 自动显示内容
void Display_AutoDisplay(void)
{
    static uint32_t counter = 0;

    // 像素
    if ((++counter) > currentPixelCycle)
    {
        counter = 0;
        frameCounter++;
    }

    if (frameCounter >= LandscapePixelNumber)
        return;

    // 分割像素点， 未到达显示区间不亮灯
    if (counter >= PixelSpacingInterval)
    {
        Display_OutputBuffer(frameCounter);
    }
    else
    {
        LEDArray_OutHex(0);
    }
}

// 红外中断处理
void Display_InterruptHandle(void)
{
    // 上次中断时间戳
    static uint32_t lastInterrupt = 0;
    // 上次红外触发时间计数
    static uint32_t lastRoundStamp = 0;

    // 防红外输入抖动
    if (IsTimeOut(lastInterrupt, 2))
    {
        // 记录上次中断时间戳
        lastInterrupt = millis();

        printf("Cycle：%d\t", (rotationCounter - lastRoundStamp));

        // 当前像素周期时间
        uint16_t pixelCycle = (rotationCounter - lastRoundStamp) * // 总周期的时间 *
                              scalingRatio *                       // 缩放比例 *
                              DisplayCycleToFullCycleProportion /  // 显示周期与总周期比例 /
                              LandscapePixelNumber;                // 显示周期像素数目

        // 像素时间变动超过1时候更新，防止长度跳动
        if (abs((int16_t)currentPixelCycle - (int32_t)pixelCycle) > 1)
        {
            currentPixelCycle = pixelCycle;
        }

        // 更新上一次触发时间
        lastRoundStamp = rotationCounter;

        // 清空帧计数
        frameCounter = 0;

        printf("PixelCycle:%d\r\n", currentPixelCycle);
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        // 计数器
        rotationCounter++;
        // 刷新显示
        Display_AutoDisplay();
    }
}
