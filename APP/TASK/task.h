#if !defined(_TASK_H_)
#define _TASK_H_

#include "sys.h"

// 发挥部分

void Clock_Display(void);
void Running_Slashes(void);

// 基础基础

void BasicTask_1(uint8_t line1, uint8_t line2);
void BasicTask_2(void);
void BasicTask_3(void);
void BasicTask_4(void);

// 提高部分

void PromotedTask_1(uint16_t img1[16], uint16_t img2[16], uint16_t img3[16]);
void PromotedTask_2(uint16_t img1[16], uint16_t img2[16], uint16_t img3[16]);
void PromotedTask_3(void);

#endif // _TASK_H_
