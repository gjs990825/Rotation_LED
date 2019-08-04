#include "gui.h"
#include "delay.h"
#include "oled.h"
#include <stdio.h>
#include "my_lib.h"
#include "key.h"

// 文字储存buffer
uint8_t angleString[10] = "";
uint8_t targetString[10] = "";
uint8_t target2String[10] = "";
uint8_t timeString[10] = "";
uint8_t stabilizedString[10] = "";

// Agnle文字
GUI_Component_t Text_Angle = {
    .x = 0,
    .y = 0,
    .currentHighlight = false,
    .needBlink = false,
    .text = "Angle:",
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 1000,
    .lastTimeFlash = 0,
};
// Angle数值
GUI_Component_t AngleValue = {
    .x = 48,
    .y = 0,
    .currentHighlight = false,
    .needBlink = false,
    .text = angleString,
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 90,
    .lastTimeFlash = 0,
};
// Target文字
GUI_Component_t Text_Target = {
    .x = 0,
    .y = 2,
    .currentHighlight = false,
    .needBlink = false,
    .text = "Goal:",
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 1000,
    .lastTimeFlash = 0,
};
// Target数值
GUI_Component_t TargetValue = {
    .x = 40,
    .y = 2,
    .currentHighlight = false,
    .needBlink = false,
    .text = targetString,
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 90,
    .lastTimeFlash = 0,
};
// Target2数值
GUI_Component_t Target2Value = {
    .x = 88,
    .y = 2,
    .currentHighlight = false,
    .needBlink = false,
    .text = target2String,
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 90,
    .lastTimeFlash = 0,
};
// Stabilized文字
GUI_Component_t StabilizedText = {
    .x = 0,
    .y = 4,
    .currentHighlight = false,
    .needBlink = false,
    .text = "Stabilized:",
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 1000,
    .lastTimeFlash = 0,
};
// Stabilized数值
GUI_Component_t StabilizedValue = {
    .x = 88,
    .y = 4,
    .currentHighlight = false,
    .needBlink = false,
    .text = stabilizedString,
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 90,
    .lastTimeFlash = 0,
};
// Time文字
GUI_Component_t TimeText = {
    .x = 0,
    .y = 6,
    .currentHighlight = false,
    .needBlink = false,
    .text = "Time:",
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 1000,
    .lastTimeFlash = 0,
};
// Time数值
GUI_Component_t TimeValue = {
    .x = 40,
    .y = 6,
    .currentHighlight = false,
    .needBlink = false,
    .text = timeString,
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 90,
    .lastTimeFlash = 0,
};

// 控件集合
GUI_Component_t *componentsSet[] = {
    &Text_Angle,
    &AngleValue,
    &Text_Target,
    &TargetValue,
    &Target2Value,
    &StabilizedText,
    &StabilizedValue,
    &TimeText,
    &TimeValue,
};

// 控件集合内容个数
uint16_t componentsNumber = GET_ARRAY_LENGEH(componentsSet);

// 确认控件
GUI_Component_t ConfirmText = {
    .x = 32,
    .y = 2,
    .currentHighlight = false,
    .needBlink = false,
    .text = "Confirm?",
    .size = 2,
    .lastTimeRefresh = 0,
    .refreshInterval = 90,
    .lastTimeFlash = 0,
};

// 确认控件集合
GUI_Component_t *confirmComponents[] = {
    &ConfirmText,
};

// 确认控件内容个数
uint16_t confirmComponentsNumber = GET_ARRAY_LENGEH(confirmComponents);

// 设置闪烁状态
void GUI_SetFlash(GUI_Component_t *component, bool status)
{
    if (status)
    {
        component->needBlink = true;
    }
    else
    {
        component->currentHighlight = true;
        GUI_FlashComponent(component);
        component->needBlink = false;
    }
}

// 更新可变数据
void GUI_UpdateData(void)
{
//    sprintf((char *)AngleValue.text, "%5.2f", Get_CurrentAngle());
//    sprintf((char *)TimeValue.text, "%5.2f", millis() / 1000.0);
//    sprintf((char *)TargetValue.text, "%5.1f", setAngle1);
//    sprintf((char *)Target2Value.text, "%5.1f", setAngle2);
//    sprintf((char *)StabilizedValue.text, "%s", Is_Stablilized() ? "YES" : "NO ");
}

// 闪烁控件
void GUI_FlashComponent(GUI_Component_t *components)
{
    components->lastTimeFlash = millis();
    components->currentHighlight = !components->currentHighlight;

    OLED_ShowStr(components->x,
                 components->y,
                 components->text,
                 components->size,
                 components->currentHighlight);
}

bool needRefreash = true;

// 刷新控件
void GUI_RefreashComponent(GUI_Component_t *components)
{
    components->lastTimeRefresh = millis();

    OLED_ShowStr(components->x,
                 components->y,
                 components->text,
                 components->size,
                 components->currentHighlight);
}

// 界面刷新任务
void GUI_RefreashInterface(GUI_Component_t *components[], uint16_t compomentNumber)
{
    if (needRefreash)
        GUI_UpdateData();

    for (uint16_t i = 0; i < compomentNumber; i++)
    {
        // 刷新任务
        if (IsTimeOut(components[i]->lastTimeRefresh, components[i]->refreshInterval))
        {
            GUI_RefreashComponent(components[i]);
        }
        // 闪烁任务
        if (components[i]->needBlink && IsTimeOut(components[i]->lastTimeFlash, flashInterval))
        {
            GUI_FlashComponent(components[i]);
        }
    }
}

uint8_t GUI_ConfirmPage(void)
{
    uint8_t key;

    needRefreash = false;
//    在刷新程序里面定义接口
//    GUI_ChangeDisplay(confirmComponents, confirmComponentsNumber);
    OLED_CLS();

    while (1)
    {
        key = KEY_Scan(1);
        if (key)
        {
            while (key == KEY_Scan(1))
                ;
            break;
        }
    }

//    GUI_ChangeDisplay(componentsSet, componentsNumber);
    OLED_CLS();
    needRefreash = true;

    return key;
}
