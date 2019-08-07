#include "display.h"
#include "ledarray.h"
#include "delay.h"
#include "string.h"

// 每个周期的分段个数
#define numberOfInterval 360

// 旋转时间计数
__IO uint32_t rotationCounter = 0;
// 上次触发时间
__IO uint32_t lastRoundStamp = 0;
__IO uint32_t currentInterval = 0;

uint16_t colorControl = 0xFF;

bool displayBuffer[numberOfInterval][16];

// 显示初始化
void Display_Init(void)
{
    uint16_t arr = 0;
    uint16_t psc = 0;

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

    TIM_Cmd(TIM3, ENABLE);

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
void Display_OutPutBuffer(uint16_t x)
{
    LEDArray_Color(colorControl);

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
    colorControl = color;
}

void Display_AutoDisplay(void)
{
    // 计算输出当前列
    Display_OutPutBuffer((rotationCounter - lastRoundStamp) / currentInterval);
}

void TIM3_IRQHandler(void)
{
    static bool infraredStatus = false;

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        rotationCounter++;

        bool currentStatus = Infrared_IsTiggered();

        // 未触发，红外未触发
        if (!infraredStatus && !currentStatus)
        {
            // do nothing
        }
        // 已触发， 红外触发
        else if (infraredStatus && currentStatus)
        {
            // do nothing
        }
        // 未触发， 红外触发
        else if (!infraredStatus && currentStatus)
        {
            infraredStatus = true;
            // 计算当前周期
            currentInterval = (rotationCounter - lastRoundStamp) / numberOfInterval;
            lastRoundStamp = rotationCounter;
        }
        // 已触发， 红外未触发
        else if (infraredStatus && !currentStatus)
        {
            infraredStatus = false;
        }

        Display_AutoDisplay();
    }
}
