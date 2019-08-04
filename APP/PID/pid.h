#if !defined(__PID_H_)
#define __PID_H_

#include "sys.h"

// 计算出的PID值
extern float PID_Value;

// 从串口设定PID参数
void Set_PIDParamFromUSART(void);
// 调整PID参数值
void PID_SetParam(float p, float i, float d);

// 根据当前误差计算
float Calculate_PID(float inputError);
// 清空PID数据
void PID_DataClear(void);
// 设定上一次误差值
void PID_DataSetting(float error, float value);

#endif // __PID_H_
