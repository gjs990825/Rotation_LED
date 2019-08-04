#ifndef _TIMERSTAMP_H_
#define _TIMERSTAMP_H_

#include "sys.h"

// 等待某个标志位，超时则忽略
#define WaitForFlagInMs(flag, status, timeout)                        \
    do                                                                \
    {                                                                 \
        uint32_t startStamp = millis();                               \
        while ((!IsTimeOut(startStamp, timeout)) && (flag != status)) \
        {                                                             \
        };                                                            \
    } while (0)

// 周期性任务
// 用于主循环中定期执行的任务
#define PeriodicTask(period, content)            \
    do                                           \
    {                                            \
        static uint32_t LastProcessStamp = 0;    \
        if (IsTimeOut(LastProcessStamp, period)) \
        {                                        \
            LastProcessStamp = millis();         \
            {                                    \
                content;                         \
            }                                    \
        }                                        \
    } while (0)

// 通过时间戳延时（精度1ms）可长时间延时
inline void delay(uint32_t ms);
// 获取当前时间戳
inline uint32_t millis(void);
// 返回是否超时（单位ms）
inline bool IsTimeOut(uint32_t setTimeStamp, uint32_t timeOutLimit);
// 初始化时间戳记录定时器
void Timestamp_Init(void);

#endif
