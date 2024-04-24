/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
*\*\file n32l40x_it.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
**/

#ifndef __N32L40X_IT_H__
#define __N32L40X_IT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32l40x.h"

#define system_it_1ms		systick_flag_get(SYS_INT_1MS)
#define system_it_10ms		systick_flag_get(SYS_INT_10MS)
#define system_it_50ms		systick_flag_get(SYS_INT_50MS)
#define system_it_100ms		systick_flag_get(SYS_INT_100MS)
#define system_it_500ms		systick_flag_get(SYS_INT_500MS)
#define system_it_1s		systick_flag_get(SYS_INT_1S)

typedef enum{
	SYS_INT_1MS = 1,
	SYS_INT_10MS,
	SYS_INT_50MS,
	SYS_INT_100MS,
	SYS_INT_500MS,
	SYS_INT_1S,
}ESYSITNUM;

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);

void SysTick_Handler(void);
void delay_ms(__IO uint32_t nTime); //delay ms
uint32_t sys_tick_get(void);		//depend systick
uint32_t sys_hour_get(void);		//depend systick
uint32_t sys_minute_get(void);		//depend systick
uint32_t sys_second_get(void);		//depend systick
uint32_t sys_msecond_get(void);		//depend systick
bool systick_flag_get(ESYSITNUM iflag);

#ifdef __cplusplus
}
#endif

#endif /* __N32G430_IT_H__ */

