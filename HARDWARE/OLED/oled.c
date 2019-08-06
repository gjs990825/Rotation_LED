#include "oled.h"
#include "delay.h"
#include "font.h"

#if __OLED_SPI__

#include "spi.h"

#define OLED_PIN_RES PBout(12)
#define OLED_PIN_DC PBout(11)
#define OLED_PIN_CS PBout(10)

void __OLED_Delay(void)
{
	uint16_t i = 1000;
	while (i--)
		;
}

void OLED_SPIWrite(uint8_t data)
{
	uint8_t retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if (retry > 0xFE)
			return;
	}

	SPI_I2S_SendData(SPI2, data);

	// 按情况增加延时
	__OLED_Delay();
}

#else

#include "iic.h"

#endif

fontInfo_t Font_6x8 = {
	.data = (uint8_t *)F6X8,
	.xSize = 6,
	.ySize = 8,
};

fontInfo_t Font_6x12 = {
	.data = (uint8_t *)F6X12,
	.xSize = 6,
	.ySize = 12,
};

fontInfo_t Font_8x16 = {
	.data = (uint8_t *)F8X16,
	.xSize = 8,
	.ySize = 16,
};

fontInfo_t Font_12x24 = {
	.data = (uint8_t *)F12X24,
	.xSize = 12,
	.ySize = 24,
};

void OLED_WriteCmd(uint8_t cmd) //写命令
{
#if __OLED_SPI__

	OLED_PIN_DC = 0;
	OLED_PIN_CS = 0;
	OLED_SPIWrite(cmd);
	OLED_PIN_DC = 1;
	OLED_PIN_CS = 1;

#else

	I2C_Single_Write(OLE_ADRESS, 0x00, cmd);

#endif
}

void OLED_WriteDat(uint8_t dat)
{
#if __OLED_SPI__

	OLED_PIN_DC = 1;
	OLED_PIN_CS = 0;
	OLED_SPIWrite(dat);
	OLED_PIN_DC = 1;
	OLED_PIN_CS = 1;

#else

	I2C_Single_Write(OLE_ADRESS, 0x40, dat);

#endif
}

void OLED_Init(void)
{
#if __OLED_SPI__

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10);

#endif

	delay_ms(100); //这里的延时很重要

	OLED_WriteCmd(0xAE); //display off
	OLED_WriteCmd(0x20); //Set Memory Addressing Mode
	OLED_WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	OLED_WriteCmd(0xc8); //Set COM Output Scan Direction
	OLED_WriteCmd(0x00); //---set low column address
	OLED_WriteCmd(0x10); //---set high column address
	OLED_WriteCmd(0x40); //--set start line address
	OLED_WriteCmd(0x81); //--set contrast control register
	OLED_WriteCmd(0xff); //亮度调节 0x00~0xff
	OLED_WriteCmd(0xa1); //--set segment re-map 0 to 127
	OLED_WriteCmd(0xa6); //--set normal display
	OLED_WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	OLED_WriteCmd(0x3F); //
	OLED_WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_WriteCmd(0xd3); //-set display offset
	OLED_WriteCmd(0x00); //-not offset
	OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_WriteCmd(0xf0); //--set divide ratio
	OLED_WriteCmd(0xd9); //--set pre-charge period
	OLED_WriteCmd(0x22); //
	OLED_WriteCmd(0xda); //--set com pins hardware configuration
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb); //--set vcomh
	OLED_WriteCmd(0x20); //0x20,0.77xVcc
	OLED_WriteCmd(0x8d); //--set DC-DC enable
	OLED_WriteCmd(0x14); //
	OLED_WriteCmd(0xaf); //--turn on oled panel

	OLED_CLS();
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{
	OLED_WriteCmd(0xb0 + y);
	OLED_WriteCmd(((x & 0xf0) >> 4) | 0x10);
	//OLED_WriteCmd(x&0x0f);;
	OLED_WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char fill_Data)
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		OLED_WriteCmd(0xb0 + m); //page0-page1
		OLED_WriteCmd(0x00);	 //low column start address
		OLED_WriteCmd(0x10);	 //high column start address
		for (n = 0; n < 132; n++)
		{
			OLED_WriteDat(fill_Data);
		}
	}
}

void OLED_CLS(void) //清屏
{
	OLED_Fill(0x00);
}

void OLED_ON(void)
{
	OLED_WriteCmd(0X8D); //设置电荷泵
	OLED_WriteCmd(0X14); //开启电荷泵
	OLED_WriteCmd(0XAF); //OLED唤醒
}

void OLED_OFF(void)
{
	OLED_WriteCmd(0X8D); //设置电荷泵
	OLED_WriteCmd(0X10); //关闭电荷泵
	OLED_WriteCmd(0XAE); //OLED休眠
}

void OLED_ShowSingleChar(uint8_t x, uint8_t y, uint8_t ch, fontInfo_t font, uint8_t yByteSize, bool isHightlight)
{
	ch -= 32;

	for (uint8_t i = 0; i < yByteSize; i++)
	{
		OLED_SetPos(x, y + i);
		for (uint8_t j = 0; j < font.xSize; j++)
		{
			OLED_WriteDat(font.data[(ch * font.xSize + j) * yByteSize + i]);
		}
	}
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, fontInfo_t font, bool isHightlight)
{
	uint8_t yByteSize = font.ySize / 8 + ((font.ySize % 8) ? 1 : 0);

	OLED_ShowSingleChar(x, y, ch, font, yByteSize, isHightlight);
}

void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], fontInfo_t font, bool isHighlight)
{
	uint8_t count = 0;
	while (ch[count] != '\0')
	{
		OLED_ShowChar(x + count * font.xSize, y, ch[count], font, isHighlight);
		count++;
	}
}

void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_SetPos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WriteDat(BMP[j++]);
		}
	}
}
