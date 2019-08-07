#if !defined(__DEBUG_H_)
#define __DEBUG_H_

#include "sys.h"

// 调试引脚定义

#define DEBUG_PIN_1 PAout(4)
#define DEBUG_PIN_2 PAout(5)
#define DEBUG_PIN_3 PAout(6)

#define DEBUG_PIN_1_SET() DEBUG_PIN_1 = 1
#define DEBUG_PIN_1_RESET() DEBUG_PIN_1 = 0
#define DEBUG_PIN_1_TOGGLE() DEBUG_PIN_1 = !DEBUG_PIN_1

#define DEBUG_PIN_2_SET() DEBUG_PIN_2 = 1
#define DEBUG_PIN_2_RESET() DEBUG_PIN_2 = 0
#define DEBUG_PIN_2_TOGGLE() DEBUG_PIN_2 = !DEBUG_PIN_2

#define DEBUG_PIN_3_SET() DEBUG_PIN_3 = 1
#define DEBUG_PIN_3_RESET() DEBUG_PIN_3 = 0
#define DEBUG_PIN_3_TOGGLE() DEBUG_PIN_3 = !DEBUG_PIN_3

void DebugPin_Init(void);
void DebugTimer_Init(uint16_t arr, uint16_t psc);
void print_info(char *str, ...);
void Dump_Array(uint8_t *name, uint8_t *array, uint8_t length);

// 显示数据

#define dump_array(array, length) Dump_Array(#array, array, length)
#define print_var(var) print_info("%s = %d\r\n", #var, var)
#define print_str(str) print_info("%s:%s\r\n", #str, str)

#endif // __DEBUG_H_
