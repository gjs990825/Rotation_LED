#include "communication.h"
#include "usart.h"
#include "stdio.h"
#include "task.h"
#include "string.h"
#include "display.h"
#include "delay.h"
#include "cxk.h"

uint16_t img1[16] = {
    0xF000,
    0xF000,
    0xF000,
    0xF000,
    0x0F00,
    0x0F00,
    0x0F00,
    0x0F00,
    0x00F0,
    0x00F0,
    0x00F0,
    0x00F0,
    0x000F,
    0x000F,
    0x000F,
    0x000F,
};
uint16_t img2[16] = {
    0x00F0,
    0x00F0,
    0x00F0,
    0x00F0,
    0x000F,
    0x000F,
    0x000F,
    0x000F,
    0xF000,
    0xF000,
    0xF000,
    0xF000,
    0x0F00,
    0x0F00,
    0x0F00,
    0x0F00,
};
uint16_t img3[16] = {
    0x000F,
    0x000F,
    0x000F,
    0x000F,
    0x00F0,
    0x00F0,
    0x00F0,
    0x00F0,
    0x0F00,
    0x0F00,
    0x0F00,
    0x0F00,
    0xF000,
    0xF000,
    0xF000,
    0xF000,
};

// 集合
uint16_t *img[3] = {img1, img2, img3};

void CONN_ExcuteCMD(char *str)
{
    int basicN, line1, line2;
    int num = sscanf(str, "CONTROL_Basic:[%d]", &basicN);

    if (num == 1)
    {
        switch (basicN)
        {
        case 1:
            if (sscanf(str, "CONTROL_Basic:[1][%d][%d]", &line1, &line2) == 2)
            {
                BasicTask_1(line1, line2);
            }
            break;
        case 2:
            BasicTask_2();
            break;
        case 3:
            // 3/4任务在一块
            BasicTask_3();
            BasicTask_4();
            break;
        default:
            printf("ERROR\r\n");
            break;
        }
    }
}

void CONN_ProcessData(char *str)
{
    uint16_t *pImgN = NULL;
    int imgN, tempData, i;
    if (sscanf(str, "IMG_DATA_%d:[%d](%X)", &imgN, &i, &tempData) == 3)
    {
        if (imgN <= 3 && imgN > 0)
        {
            pImgN = img[imgN - 1];

            if (pImgN != NULL && i < 16 && i >= 0)
            {
                pImgN[i] = (uint16_t)tempData;
            }
        }
    }
}

void CONN_Replay(char *str)
{
    int id;
    if (sscanf(str, "Replay:[%d]", &id) == 1)
    {
        if (id == 1)
        {
            PromotedTask_1(img);
        }
        else if (id == 2)
        {
            PromotedTask_2(img);
        }
    }
}

uint8_t msgBuf[50];

// 检查串口消息
// 返回是否有消息
bool Check_USARTMessage(void)
{
    if (USART_RECEIVED)
    {
        memset(msgBuf, 0, 50);
        memcpy(msgBuf, USART_RX_BUF, USART_MSG_LEN);

        USART_CLR_REC();

        if (strstr((char *)msgBuf, "IMG_DATA_") != NULL)
        {
            CONN_ProcessData((char *)msgBuf);
        }
        else if (strstr((char *)msgBuf, "CXK") != NULL)
        {
            Subs_ScrollDisplay(112, 2, LitigationWarning, 4000);
            Video_Play(16, 2, 874, ChickenYouAreSoBeatutiful, 40000);
        }
        else
        {
            Display_CLS();
            Display_Color(0);
            Display_Control(ENABLE);
            Display_ShowStr(0, 0, msgBuf, Font_8x16, false);

            if (strstr((char *)msgBuf, "CONTROL_Basic:") != NULL)
            {
                // 显示指令内容的时间
                delay(2500);
                CONN_ExcuteCMD((char *)msgBuf);
            }
            else if (strstr((char *)msgBuf, "Replay:") != NULL)
            {
                // 显示指令内容的时间
                delay(2500);
                CONN_Replay((char *)msgBuf);
            }
        }

        return true;
    }
    return false;
}
