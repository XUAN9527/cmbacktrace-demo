#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "br_config.h"
#include "n32l40x_it.h"
#include "drv_iwdg.h"
#include "shell_port.h"
#include "soft_timer.h"
#include "log.h"
#include "board.h"
#include "cm_backtrace.h"
#include "drv_usart.h"

#define HARDWARE_VERSION               "V1.0.0"
#define SOFTWARE_VERSION               "V0.1.0"

static int user_board_uart_init(void);
static void user_board_system_info(void);

extern void fault_test_by_unalign(void);
extern void fault_test_by_div0(void);

int main(void)
{
	System_board_init();
	user_board_uart_init();
	user_board_system_info();
	
	cm_backtrace_init("CmBacktrace", HARDWARE_VERSION, SOFTWARE_VERSION);
	fault_test_by_unalign();
    fault_test_by_div0();

    while(1)
    {

	}
}

static void version_info_print(void)
{
    logPrintln("Thread Operating System");
	logPrintln("build  %s %s",__DATE__, __TIME__);
	logPrintln("Project Name     =  [%s]",STRING_PROJECT_NAME);
	logPrintln("Project Version  =  [%s]",STRING_VERSION);
    logPrintln("2022 - 2023 Copyright by xuan\r\n");
}

static uint32_t rcc_clocks_arr[4];
static void RCC_Clocks_Freq_Get(void)
{
	RCC_ClocksType rcc_clocks;
	RCC_GetClocksFreqValue(&rcc_clocks);
	rcc_clocks_arr[0] = rcc_clocks.SysclkFreq;
	rcc_clocks_arr[1] = rcc_clocks.HclkFreq;
	rcc_clocks_arr[2] = rcc_clocks.Pclk1Freq;
	rcc_clocks_arr[3] = rcc_clocks.Pclk2Freq;
}

static uint32_t *rcc_clocks_arr_get(void)
{
	return rcc_clocks_arr;
}

static void System_Clock_print(void)
{
	uint32_t *rcc_arr = rcc_clocks_arr_get();
	logPrintln("Rcc Clock Dump:");
	logPrintln("SysclkFreq = %d Hz",*rcc_arr);
	logPrintln("HclkFreq   = %d Hz",*(rcc_arr+1));
	logPrintln("Pclk1Freq  = %d Hz",*(rcc_arr+2));
	logPrintln("Pclk2Freq  = %d Hz\r\n",*(rcc_arr+3));
}

static int user_board_uart_init(void)
{
	drv_usart_init(ESERIAL_1,ESERIAL_MODE_DMA_RX | ESERIAL_MODE_DMA_TX,RT_NULL);
//	drv_usart_init(ESERIAL_1,ESERIAL_MODE_DMA_RX,RT_NULL);
	User_Shell_Init();
    return 0;
}
static void user_board_system_info(void)
{
	RCC_Clocks_Freq_Get();
	version_info_print();
	System_Clock_print();
}
