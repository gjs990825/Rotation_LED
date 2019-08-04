#if !defined(_GUI_H_)
#define _GUI_H_

#include "sys.h"

#define flashInterval 300

typedef struct GUI_Compoment
{
    uint8_t x;                // x坐标
    uint8_t y;                // y坐标
    bool currentHighlight;    // 当前高亮状态
    bool needBlink;           // 需要闪烁
    uint8_t *text;            // 文字内容
    uint8_t size;             // 文字大小
    uint32_t lastTimeRefresh; // 上次刷新时间
    uint16_t refreshInterval; // 刷新间隔
    uint32_t lastTimeFlash;   // 上次闪烁时间
    // uint16_t flashInterval;   // 闪烁间隔 // 取消，默认值
} GUI_Component_t;

extern GUI_Component_t *componentsSet[];
extern uint16_t componentsNumber;

extern GUI_Component_t Text_Angle,
    AngleValue,
    Text_Target,
    TargetValue,
    Target2Value,
    StabilizedText,
    StabilizedValue,
    TimeText,
    TimeValue;

void GUI_SetFlash(GUI_Component_t *component, bool status);
void GUI_FlashComponent(GUI_Component_t *components);
void GUI_RefreashComponent(GUI_Component_t *components);
void GUI_RefreashInterface(GUI_Component_t *components[], uint16_t compomentNumber);
void OLED_GUI(void);

uint8_t GUI_ConfirmPage(void);

#endif // _GUI_H_
