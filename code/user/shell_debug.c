#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "n32l40x.h"
#include "shell_debug.h"
#include "drv_usart.h"
#include "log.h"
#include "br_config.h"

//redefine
// static int is_lr_sent = 0;
// int fputc(int ch, FILE* f)
// {
//     if (ch == '\r')
//     {
//         is_lr_sent = 1;
//     }
//     else if (ch == '\n')
//     {
//         if (!is_lr_sent)
//         {
//             drv_serial_dma_write(ESERIAL_1,(uint8_t *)'\r',1);
//         }
//         is_lr_sent = 0;
//     }
//     else
//     {
//         is_lr_sent = 0;
//     }
//     drv_serial_dma_write(ESERIAL_1,(uint8_t *)&ch,1);
//     return ch;
// }

int _write(int fd, char* pBuffer, int size)
{
    // for (int i = 0; i < size; i++)
    // {
    //     USART_SendData(USART1, pBuffer[i]);
    //     while (USART_GetFlagStatus(USART1, USART_FLAG_TXDE) == RESET);
    // }
	// return size;
	
	return drv_serial_dma_write(ESERIAL_1, pBuffer, size);
}

#if 1										//普通方式

static void version(void)
{
	logPrintln("Thread Operating System");
	logPrintln("build  %s %s",__DATE__, __TIME__);
	logPrintln("Project Name     =  [%s]",STRING_PROJECT_NAME);
	logPrintln("Project Version  =  [%s]",STRING_VERSION);
    logPrintln("2022 - 2023 Copyright by xuan");
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN, version, version,version information);

//fix clear console,add front on \r\n
static void reboot(void)
{
#if 1
	__set_FAULTMASK(1);                                 //关闭所有中断
    NVIC_SystemReset();                                 //复位,以上两个函数位于 core_cm4.h 文件中
#else
	__set_FAULTMASK(1);
	iap_load_app(0x8000000);
#endif
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN, reboot, reboot,system software reset);

#endif
