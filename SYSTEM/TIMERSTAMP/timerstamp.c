#include "timerstamp.h"

// 毫秒时间戳
__IO uint32_t global_times = 0;

// 获取当前时间戳
uint32_t millis(void)
{
	return global_times;
}

// 返回是否超时
bool IsTimeOut(uint32_t setTimeStamp, uint32_t timeOutLimit)
{
	return millis() > (setTimeStamp + timeOutLimit);
}

void delay(uint32_t ms)
{
    uint32_t startStamp = millis();
    for (;;)
    {
        if (IsTimeOut(startStamp, ms))
            break;
    }
}

// 时间戳初始化
void Timestamp_Init(void)
{
	// 一个毫秒需要的周期
	uint32_t total = SystemCoreClock / 1000;
	
	uint16_t arr = 1000 - 1;
	uint16_t psc = total / 1000 - 1;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM4, ENABLE);
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

		global_times++;
	}
}
