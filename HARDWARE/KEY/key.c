#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"

void KEY_Init(void) //IO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

#define KEY1_PRES 1
#define KEY2_PRES 2
#define KEY3_PRES 3
#define KEY4_PRES 4
#define KEY5_PRES 5

#define IfKey(n)     \
	if (KEY##n == 0) \
		return KEY##n##_PRES;

#define ElifKey(n)   \
	if (KEY##n == 0) \
		return KEY##n##_PRES;

// 0为不支持连按
uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up = 1; //按键按松开标志
	if (mode)
	{
		key_up = 1; //支持连按
	}

	if (key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0 || KEY5 == 0))
	{
		delay_ms(10); //去抖动
		key_up = 0;

		IfKey(1);
		ElifKey(2);
		ElifKey(3);
		ElifKey(4);
		ElifKey(5);
	}
	else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 && KEY5 == 1)
	{
		key_up = 1;
	}
	return 0; // 无按键按下
}

// 获取数值并取整
void Key_GetSetValue(float *val)
{
	uint8_t key;
	int8_t quitOption = 0;
	float initVal = *val;

	while (1)
	{
		key = KEY_Scan(1);

		switch (key)
		{
		case 2:
			*val -= 0.1;
			break;
		case 3:
			*val += 0.1;
			break;
		case 1:
			quitOption = -1;
			break;
		case 4:
			quitOption = -1;
			break;
		case 5:
			quitOption = 1;
			break;

		default:
			break;
		}

		if (quitOption)
			break;
	}

	while (key == KEY_Scan(1))
		;

	if (quitOption != 1)
		*val = (int)(initVal + 0.5);
	else
		*val = (int)(*val + 0.5);
}
