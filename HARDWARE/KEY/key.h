#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY1 PAin(4)
#define KEY2 PAin(2)
#define KEY3 PAin(1)
#define KEY4 PAin(0)
#define KEY5 PAin(3)

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t);
void Key_GetSetValue(float *val);

#endif
