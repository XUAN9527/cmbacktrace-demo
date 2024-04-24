#ifndef __SHELL_PORT_H
#define __SHELL_PORT_H

#include "shell.h"

#define SYSTEM_LOG_SHELL	0				//part cmd on/off

/* 将shell定义为外部变量，在串口中断回调函数中还要使用 */
//extern Shell shell;

/* 声明自己编写的初始化函数 */
void User_Shell_Init(void);
void shellTaskPort(void);

#endif /* __SHELL_PORT_H */
