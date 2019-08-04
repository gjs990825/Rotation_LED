#include "oled.h"
#include "delay.h"
#include "font.h"
#include "iic.h"

void OLED_WriteCmd(uint8_t cmd) //写命令
{
	I2C_Single_Write(OLE_ADRESS, 0x00, cmd);
}

void OLED_WriteDat(uint8_t dat)
{
	I2C_Single_Write(OLE_ADRESS, 0x40, dat);
}

void OLED_Init(void)
{
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

void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize, bool isHighlight)
{
	unsigned char c = 0, i = 0, j = 0;
	switch (TextSize)
	{
	case 1:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 126)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			if (isHighlight)
			{
				for (i = 0; i < 6; i++)
					OLED_WriteDat(~F6x8[c][i]);
			}
			else
			{
				for (i = 0; i < 6; i++)
					OLED_WriteDat(F6x8[c][i]);
			}

			x += 6;
			j++;
		}
	}
	break;
	case 2:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 120)
			{
				x = 0;
				y++;
			}

			if (isHighlight)
			{
				OLED_SetPos(x, y);
				for (i = 0; i < 8; i++)
					OLED_WriteDat(~F8X16[c * 16 + i]);
				OLED_SetPos(x, y + 1);
				for (i = 0; i < 8; i++)
					OLED_WriteDat(~F8X16[c * 16 + i + 8]);
			}
			else
			{
				OLED_SetPos(x, y);
				for (i = 0; i < 8; i++)
					OLED_WriteDat(F8X16[c * 16 + i]);
				OLED_SetPos(x, y + 1);
				for (i = 0; i < 8; i++)
					OLED_WriteDat(F8X16[c * 16 + i + 8]);
			}

			x += 8;
			j++;
		}
	}
	break;
	}
}

// void OLED_ShowStr(uint8_t x, uint8_t y, uint8_t ch[], uint8_t TextSize)
// {
// 	unsigned char c = 0, i = 0, j = 0;
// 	switch (TextSize)
// 	{
// 	case 1:
// 	{
// 		while (ch[j] != '\0')
// 		{
// 			c = ch[j] - 32;
// 			if (x > 126)
// 			{
// 				x = 0;
// 				y++;
// 			}
// 			OLED_SetPos(x, y);
// 			for (i = 0; i < 6; i++)
// 				OLED_WriteDat(F6x8[c][i]);
// 			x += 6;
// 			j++;
// 		}
// 	}
// 	break;
// 	case 2:
// 	{
// 		while (ch[j] != '\0')
// 		{
// 			c = ch[j] - 32;
// 			if (x > 120)
// 			{
// 				x = 0;
// 				y++;
// 			}
// 			OLED_SetPos(x, y);
// 			for (i = 0; i < 8; i++)
// 				OLED_WriteDat(F8X16[c * 16 + i]);
// 			OLED_SetPos(x, y + 1);
// 			for (i = 0; i < 8; i++)
// 				OLED_WriteDat(F8X16[c * 16 + i + 8]);
// 			x += 8;
// 			j++;
// 		}
// 	}
// 	break;
// 	}
// }

void OLED_ShowCN(uint8_t x, uint8_t y, uint8_t number)
{
	unsigned char wm = 0;
	unsigned int adder = 32 * number;
	OLED_SetPos(x, y);
	for (wm = 0; wm < 16; wm++)
	{
		OLED_WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x, y + 1);
	for (wm = 0; wm < 16; wm++)
	{
		OLED_WriteDat(F16x16[adder]);
		adder += 1;
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
