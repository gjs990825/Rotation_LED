#ifndef __MY_LIB_H__
#define __MY_LIB_H__

#include "sys.h"

#define GET_ARRAY_LENGEH(array) (sizeof(array) / sizeof(array[0]))

int constrain_int(int x, int a, int b);
float constrain_float(float x, float a, float b);
uint8_t BCD2HEX(uint8_t bcd_data);
uint8_t HEX2BCD(uint8_t hex_data);
void bubble_sort(uint16_t arr[], uint16_t len);
int MinimumAbsOf(int *array, uint16_t length);
double Variance(double* x, uint16_t number);
double Average(double *x, uint16_t number);
float map(float x, float in_min, float in_max, float out_min, float out_max);

void infinity_loop(void);

#endif //__MY_LIB_H__
