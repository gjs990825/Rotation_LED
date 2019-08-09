#if !defined(_TASK_H_)
#define _TASK_H_

#include "sys.h"

void BasicTask_1(uint8_t line1, uint8_t line2);
void BasicTask_2(void);
void BasicTask_3(void);
void BasicTask_4(void);
void BasicTask_All(uint8_t line1, uint8_t line2);

void PromotedTask_1(uint16_t img1[16], uint16_t img2[16], uint16_t img3[16]);
void PromotedTask_2(void);
void PromotedTask_3(void);
void PromotedTask_All(uint16_t img1[16], uint16_t img2[16], uint16_t img3[16]);

#endif // _TASK_H_
