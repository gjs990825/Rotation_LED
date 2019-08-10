#include "communication.h"
#include "usart.h"
#include "stdio.h"
#include "task.h"
#include "string.h"

char img1DataKey[] = "IMG1_DATA:";
char img2DataKey[] = "IMG2_DATA:";
char img3DataKey[] = "IMG3_DATA:";
char controlBasicKey[] = "CONTROL_Basic:";
char controlPromotedKey[] = "CONTROL_Promote:";

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
                printf("Basic_1:%d, %d\r\n", line1, line2);
                BasicTask_1(line1, line2);
            }
            break;
        case 2:
            printf("Basic_2\r\n");
            BasicTask_2();
            break;
        case 3:
            printf("Basic_3\r\n");
            BasicTask_3();
            break;
        case 4:
            printf("Basic_4\r\n");
            BasicTask_4();
            break;
        case 5:
            printf("Basic_ALL\r\n");
            if (sscanf(str, "CONTROL_Basic:[5][%d][%d]", &line1, &line2) == 2)
            {
                printf("Basic_ALL:%d, %d\r\n", line1, line2);
                BasicTask_All(line1, line2);
            }
            break;
        default:
            printf("ERROR\r\n");
            break;
        }
    }
}

// 检查串口消息
void Check_USARTMessage(void)
{
    if (USART_RECEIVED)
    {
        printf("%s\r\n", USART_RX_BUF);

        if (strstr((char *)USART_RX_BUF, "CONTROL_Basic:") != NULL)
        {
            CONN_ExcuteCMD((char *)USART_RX_BUF);
        }
        USART_CLR_REC();
    }
}
