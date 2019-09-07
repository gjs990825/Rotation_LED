#include "display.h"
#include "ledarray.h"
#include "delay.h"
#include "string.h"
#include "debug.h"
#include "stdio.h"
#include "stdlib.h"
#include "my_lib.h"
#include "font.h"

#define RefreshTIM TIM3

fontInfo_t Font_6x8 = {
    .data = (uint8_t *)F6X8,
    .xSize = 6,
    .ySize = 8,
};

fontInfo_t Font_8x16 = {
    .data = (uint8_t *)F8X16,
    .xSize = 8,
    .ySize = 16,
};

// 控制常量↓

// 显示周期占总周期的比例
const float DisplayCycleToFullCycleProportion = 345.0 / 360.0;

// 测量的稳定周期区间
#define _DISPLAY_TEST_CYCLE_ 255013

// 设定稳定周期区间
const uint32_t stableUpperLimit = _DISPLAY_TEST_CYCLE_ * 1.15;

// 控制变量↓

// 缩放比例控制
float scalingRatio = 1.0;

// 缩放比例是否已提交
bool scalingCommitted = false;

// 当前输出列计数
uint32_t pixelCounter = 0;

// 当前速度是否稳定
bool isStablized = false;

// 显示控制
bool displayEnable = false;

// 输出缓冲区
uint16_t displayBuffer[LandscapePixelNumber];

// 显示初始化
void Display_Init(void)
{
    // 此数值会在第一次进中断之后被静态变量数值取代，但不可设定为0
    // 自动重载寄存器为0时计数器不工作
    uint16_t arr = 666;

    // 分频系数，控制精度
    // 可设当增大降低填充数值
    // ——在填充值超出0xFFFF的情况下
    uint16_t psc = 10;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(RefreshTIM, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(RefreshTIM, TIM_IT_Update);

    TIM_ITConfig(RefreshTIM, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(RefreshTIM, ENABLE);

    // 清空缓冲数据
    Display_CLS();
}

// 显示扫描和速度检测服务
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(RefreshTIM, TIM_IT_Update))
    {
        TIM_ClearITPendingBit(RefreshTIM, TIM_IT_Update);

        if (displayEnable)
        {
            if (isStablized)
            {
                Display_OutputBuffer(pixelCounter);
            }
            else
            {
                Display_UnstableHandle();
            }
        }
        pixelCounter++;
    }
}

// 设定定时器重载值
void Display_PixelControlTimSet(uint16_t arr)
{
    RefreshTIM->ARR = arr;
}

// 返回是否稳定转速
bool Display_IsStablized(void)
{
    return isStablized;
}

// 是否在间隔（无图像显示区间）内
// 写运动的图像数据时在此区间内可避免图像撕裂
bool Display_IsInterval(void)
{
    return pixelCounter >= LandscapePixelNumber;
}

// 清空数据
void Display_CLS(void)
{
    memset(displayBuffer, 0, sizeof(displayBuffer));
}

// 控制显示定时器
void Display_Control(FunctionalState status)
{
    displayEnable = status;

    // 失能之后变暗
    if (status == DISABLE)
        LEDArray_ALLOFF();
}

// 从数组到十六进制
uint16_t Display_ArrayToHex(bool pixArray[16])
{
    uint16_t outData = 0;
    for (uint8_t i = 0; i < 16; i++)
    {
        outData |= (pixArray[i]) ? (1 << i) : 0;
    }
    return outData;
}

// 从十六进制到数组
void Display_HexToArray(uint16_t hex, bool *pixArray)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        pixArray[i] = (hex >> i) & 0x1;
    }
}

// 写入一列
void Display_WriteARow(bool pixel[16], uint16_t x)
{
    if (x >= LandscapePixelNumber)
        return;

    displayBuffer[x] = Display_ArrayToHex(pixel);
}

// 写入一列（HEX）
void Display_WriteARow_Hex(uint16_t HEXData, uint16_t x)
{
    if (x >= LandscapePixelNumber)
        return;

    displayBuffer[x] = HEXData;
}

// 写入半列（一个字节）
void Display_WriteARow_Byte(uint8_t byte, uint8_t isHighByte, uint16_t x)
{
    if (x >= LandscapePixelNumber)
        return;

    if (isHighByte)
    {
        displayBuffer[x] &= 0x00FF;
        displayBuffer[x] |= (uint16_t)byte << 8;
    }
    else
    {
        displayBuffer[x] &= 0xFF00;
        displayBuffer[x] |= (uint16_t)byte;
    }
}

// 填充象棋棋盘到buffer测试
void Display_FillWith(uint16_t data)
{
    for (uint16_t i = 0; i < LandscapePixelNumber; i++)
    {
        Display_WriteARow_Hex((i % 2 == 0) ? data : ~data, i);
    }
}

// 输出缓存
void Display_PrintBuffer(void)
{
    for (uint16_t i = 0; i < LandscapePixelNumber; i++)
    {
        printf("%X\r\n", displayBuffer[i]);
    }
}

// 输出第x列的数据到LED
static void Display_OutputBuffer(uint16_t x)
{
    // 防越界
    if (x >= LandscapePixelNumber)
    {
        LEDArray_OutHex(0);
        return;
    }
    LEDArray_OutHex(displayBuffer[x]);
}

// 变更显示颜色，数值越高越红，0-0xFF
void Display_Color(uint8_t color)
{
    LEDArray_Color(color);
}

// 变更显示横向缩放
void Display_Scaling(float scale)
{
    scalingRatio = scale;
    scalingCommitted = false;
}

// 红外中断处理
void Display_InterruptHandle(void)
{
    // 计算出的像素周期（定时器填充值）
    // 此初始值决定了未稳定前的扫描中断时间间隔
    static float pixelCycle = 666;

    // 上次中断时间戳
    static uint32_t lastInterrupt = 0;
    // 最后一次检测到不稳定的时间戳
    static uint32_t unstableTimeStamp = 0;

    // 防抖（不知道有没有抖动，反正打个戳能用上）
    if (IsTimeOut(lastInterrupt, 10))
    {
        // 记录上次中断时间戳
        lastInterrupt = millis();

        uint32_t currentCycle = pixelCounter * RefreshTIM->ARR + RefreshTIM->CNT;
        // 清空像素列计数
        pixelCounter = 0;

        // 判断当前是否稳定
        isStablized = currentCycle <= stableUpperLimit;

        if (!isStablized)
        {
            RefreshTIM->CNT = 0;
            unstableTimeStamp = lastInterrupt;
            return;
        }

        // 稳定数秒后不再调整间距
        if (!IsTimeOut(unstableTimeStamp, 6000))
        {
            pixelCycle = currentCycle *                      // 当前总周期的时间 *
                         DisplayCycleToFullCycleProportion / // 显示周期与总周期比例 /
                         LandscapePixelNumber;               // 显示周期像素数目

            // 清空标志位
            scalingCommitted = false;
            // printf("C:%d\tPC:%d\r\n", currentCycle, (uint16_t)pixelCycle);
        }

        // 检查是否需要提交周期或缩放更改
        if (!scalingCommitted)
        {
            scalingCommitted = true;
            Display_PixelControlTimSet((uint16_t)(pixelCycle * scalingRatio + 0.5));
        }

        // 清空当前定时器计数值，清除标志位
        RefreshTIM->CNT = 0;
        TIM_ClearITPendingBit(RefreshTIM, TIM_IT_Update);
        TIM_ClearFlag(RefreshTIM, TIM_FLAG_Update);
    }
}

// 不稳定handle
void Display_UnstableHandle(void)
{
    // 300毫秒横线循环显示
    LEDArray_OutHex(1 << (millis() / 300 % 16));
}

// 等待稳定
void Display_WaitTillStabilized(void)
{
    Display_Control(ENABLE);
    for (;;)
    {
        if (Display_IsStablized())
            break;
    }
    Display_Control(DISABLE);
}

// 输出一个字符
void Display_ShowSingleChar(uint16_t x, uint8_t y, uint8_t ch, fontInfo_t font, uint8_t yByteSize, bool isHightlight)
{
    ch -= 32;

    for (uint8_t i = 0; i < yByteSize; i++)
    {
        if (isHightlight)
        {
            for (uint8_t j = 0; j < font.xSize; j++)
            {
                Display_WriteARow_Byte(~font.data[(ch * font.xSize + j) * yByteSize + i], (i == 1) || (y != 0), j + x);
            }
        }
        else
        {
            for (uint8_t j = 0; j < font.xSize; j++)
            {
                Display_WriteARow_Byte(font.data[(ch * font.xSize + j) * yByteSize + i], (i == 1) || (y != 0), j + x);
            }
        }
    }
}

void Display_ShowChar(uint16_t x, uint8_t y, uint8_t ch, fontInfo_t font, bool isHightlight)
{
    uint8_t yByteSize = font.ySize / 8 + ((font.ySize % 8) ? 1 : 0);

    Display_ShowSingleChar(x, y, ch, font, yByteSize, isHightlight);
}

// 输出字符串：
// 8 * 16字体Y只能为0，其它数值无意义
// 6 * 8字体Y为0-1，为第0-1行
void Display_ShowStr(uint16_t x, uint8_t y, uint8_t ch[], fontInfo_t font, bool isHighlight)
{
    uint16_t count = 0;
    while (ch[count] != '\0')
    {
        Display_ShowChar(x + count * font.xSize, y, ch[count], font, isHighlight);
        count++;
    }
}
