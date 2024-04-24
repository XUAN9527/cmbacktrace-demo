/**
 * @brief shell移植到STM32L431时的接口实现
 * @author mculover666
 * @date 2020/03/27 
*/

#include "shell.h"
#include "n32l40x.h"
#include "drv_usart.h"
#include "shell_port.h"
#include "log.h"

Shell shell;
Log uartLog;
char shell_buffer[512];

void uartLogWrite(char *buffer, short len)
{
    if (uartLog.shell)
    {
        shellWriteEndLine(uartLog.shell, buffer, len);
    }
}

Log uartLog = {
    .write = uartLogWrite,
    .active = true,
    .level = LOG_VERBOSE
};

//shell写函数原型：signed short (*write)(char *, unsigned short);
signed short User_Shell_Write(char* buff,unsigned short len)
{
	return drv_serial_dma_write(ESERIAL_1, buff, len);
	// return drv_serial_write(ESERIAL_1, buff, len);
}

//shell读函数原型：signed short (*read)(char *, unsigned short);
signed short User_Shell_Read(char* buff,unsigned short len)
{
	return drv_fifo_data_get(ESERIAL_1, (uint8_t *)buff, len);
}

/* 3. 编写初始化函数 */
void User_Shell_Init(void)
{
    shell.write = User_Shell_Write;
	shell.read = User_Shell_Read;
    shellInit(&shell, shell_buffer, sizeof(shell_buffer));
	logRegister(&uartLog, &shell);
}

//SHELL_EXPORT_USER(SHELL_CMD_PERMISSION(0xFF), root, breo, root user);

/*
	LOG_NONE = 0,                                  < 无级别
    LOG_ERROR = 1,                                 < 错误
    LOG_WRANING = 2,                               < 警告
    LOG_INFO = 3,                                  < 消息
    LOG_DEBUG = 4,                                 < 调试
    LOG_VERBOSE = 5,                               < 冗余
    LOG_ALL = 6,  
*/
static void log_usart(uint8_t level)
{
	logSetLevel(&uartLog,(LogLevel)level);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN, log_usart, log_usart, log set level none->vebose:0->6);

void shellTaskPort(void)
{
	shellTask((void *)&shell);
}

