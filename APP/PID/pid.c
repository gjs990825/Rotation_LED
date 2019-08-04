#include "pid.h"
#include "my_lib.h"
#include "usart.h"
#include "stdio.h"

// float Kp = 18, Ki = 0.21, Kd = 75;
// 重物 // 25.0, 0.25, 365.0
// 无重物 // 25.0, 0.05, 165.0

float Kp = 25.0, Ki = 0.50, Kd = 175.0;

float error = 0, P = 0, I = 0, D = 0, PID_Value = 0;
float previous_error = 0;

// 通过串口接收PID参数设置
void Set_PIDParamFromUSART(void)
{
    if (USART_RECEIVED)
    {
        USART_CLR_REC();

        printf("%s\r\n", USART_RX_BUF);

        float p, i, d;
        if (sscanf((char *)USART_RX_BUF, "%f, %f, %f", &p, &i, &d) == 3)
        {
            printf("P:%f\tI:%f\tD:%f\t\r\n", p, i, d);
            PID_SetParam(p, i, d);
        }
        else
        {
            printf("Error\r\n");
        }
    }
}

// 清除PID数据
void PID_DataClear(void)
{
    I = 0;
    D = 0;
    PID_Value = 0;
    previous_error = 0;
}

// 设置PID参数
void PID_SetParam(float p, float i, float d)
{
    Kp = p;
    Ki = i;
    Kd = d;
    PID_DataClear();
}

// 设置PID数据
void PID_DataSetting(float error, float value)
{
    I = 0;
    D = 0;
    PID_Value = value;
    previous_error = error;
}

// 根据输入误差计算PID值并输出
float Calculate_PID(float inputError)
{
    error = inputError;
    P = error;
    I = I + error;
    D = error - previous_error;

    I = constrain_float(I, -200.0, 200.0); // 积分限幅

    PID_Value = (Kp * P) + (Ki * I) + (Kd * D);
    PID_Value = constrain_float(PID_Value, -700, 700);

    previous_error = error;

    return PID_Value;
}
