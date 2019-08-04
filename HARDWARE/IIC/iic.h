#if !defined(_IIC_H_)
#define _IIC_H_

#include "sys.h"

#define GPIO_I2C GPIOB
#define I2C_Pin_SCL GPIO_Pin_10
#define I2C_Pin_SDA GPIO_Pin_11
#define RCC_I2C RCC_APB2Periph_GPIOB

#define SCL_H GPIO_I2C->BSRR = I2C_Pin_SCL
#define SCL_L GPIO_I2C->BRR = I2C_Pin_SCL
#define SDA_H GPIO_I2C->BSRR = I2C_Pin_SDA
#define SDA_L GPIO_I2C->BRR = I2C_Pin_SDA
#define SCL_read GPIO_I2C->IDR &I2C_Pin_SCL
#define SDA_read GPIO_I2C->IDR &I2C_Pin_SDA

void I2C_Soft_Init(void);
int I2C_Single_Write(u8 SlaveAddress, u8 REG_Address, u8 REG_data);
int I2C_Single_Read(u8 SlaveAddress, u8 REG_Address);
uint8_t I2C_Mult_Read(u8 SlaveAddress, u8 REG_Address, u8 *ptChar, u8 size);

void I2C_delay(void);
int I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
int I2C_WaitAck(void); //返回为:=1有ACK,=0无ACK
void I2C_SendByte(u8 SendByte);
u8 I2C_ReadByte(void); //数据从高位到低位//

#endif // _IIC_H_
