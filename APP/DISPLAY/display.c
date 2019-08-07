#include "display.h"
#include "ledarray.h"
#include "delay.h"
#include "string.h"
#include "debug.h"

// 每个周期的分段个数
#define LandScapePixelNumber 360
// 像素间隔比例
#define PixelIntervalScale 0.1f

// 旋转时间计数
__IO uint32_t rotationCounter = 0;
// 当前单个像素时间间隔
__IO uint32_t currentInterval = 0;
// 当前像素显示间隔
__IO uint32_t currentPixelInterval = 0;

// 当前帧计数
uint16_t frameCounter = 0;

bool displayBuffer[LandScapePixelNumber][16];

// 显示初始化
void Display_Init(void)
{
    uint16_t arr = 71;
    uint16_t psc = 99;

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
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
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

// 输出第x列的数据
static void Display_OutPutBuffer(uint16_t x)
{
    // 防越界
    if (x > LandScapePixelNumber)
        return;

    uint16_t outData = 0;
    for (uint8_t i = 0; i < 16; i++)
    {
        outData |= (displayBuffer[x][i]) ? (1 << i) : 0;
    }
    LEDArray_Out(outData);
}

// 变更显示颜色
void Display_Color(uint8_t color)
{
    LEDArray_Color(color);
}

// 自动显示内容
static void Display_AutoDisplay(void)
{
    static uint32_t counter = 0;

    if ((++counter) > currentInterval)
    {
        counter = 0;
        frameCounter++;
    }

    // 分割像素点， 未到达显示区间不亮灯
    if (counter > currentPixelInterval)
    {
        Display_OutPutBuffer(frameCounter);
    }
    else
    {
        LEDArray_Out(0);
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

        // 计算当前周期和像素间隔
        currentInterval = (rotationCounter - lastRoundStamp) / LandScapePixelNumber;
        currentPixelInterval = currentInterval * PixelIntervalScale;

        lastRoundStamp = rotationCounter;

        // 清空帧计数
        frameCounter = 0;
    }
}

void TIM3_IRQHandler(void)
{

    DEBUG_PIN_1_SET();

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        // 计数器
        rotationCounter++;
        // 刷新显示
        Display_AutoDisplay();
    }

    DEBUG_PIN_1_RESET();
}
