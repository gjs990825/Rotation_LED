#ifndef _USART_H_
#define _USART_H_

#include "sys.h"

#define USART_REC_LEN 200         //�����������ֽ��� 200
#define USART1_ENABLE_INTERRUPT 1 //�ж�ʹ��(ʹ��scanf��������ʹ�ܣ����ܻᶪʧ����)

#define USART_RECEIVED (USART_RX_STA & 0x8000)
#define USART_CLR_REC() (USART_RX_STA = 0)
#define USART_MSG_LEN (USART_RX_STA & 0X3FFF)

extern u8 USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART_RX_STA;               //����״̬���

extern char UART_read(void);
extern void UART_write(char);

void USART1_Init(uint32_t baudrate);

#endif //_USART_H_
