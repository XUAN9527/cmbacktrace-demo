#include "drv_timer.h"
#include "drv_msp.h"
#include "br_config.h"

#ifdef  BSP_USING_TIMER

enum
{
#ifdef BSP_USING_TIMER1
    TIMER1_INDEX,
#endif

#ifdef BSP_USING_TIMER2
    TIMER2_INDEX,
#endif

#ifdef BSP_USING_TIMER3
    TIMER3_INDEX,
#endif

#ifdef BSP_USING_TIMER4
    TIMER4_INDEX,
#endif

#ifdef BSP_USING_TIMER5
    TIMER5_INDEX,
#endif

#ifdef BSP_USING_TIMER6
    TIMER6_INDEX,
#endif

#ifdef BSP_USING_TIMER7
    TIMER7_INDEX,
#endif

#ifdef BSP_USING_TIMER8
    TIMER8_INDEX,
#endif

#ifdef BSP_USING_TIMER9
    TIMER9_INDEX,
#endif
};

#ifdef BSP_USING_TIMER1
	static drv_timer_info_t _info1 = DRV_TIMER_CONFIG_DEFAULT;
#endif
#ifdef BSP_USING_TIMER2
	static drv_timer_info_t _info2 = DRV_TIMER_CONFIG_DEFAULT;
#endif
#ifdef BSP_USING_TIMER3
	static drv_timer_info_t _info3 = DRV_TIMER_CONFIG_DEFAULT;
#endif
#ifdef BSP_USING_TIMER4
	static drv_timer_info_t _info4 = DRV_TIMER_CONFIG_DEFAULT;
#endif
#ifdef BSP_USING_TIMER5
	static drv_timer_info_t _info5 = DRV_TIMER_CONFIG_DEFAULT;
#endif
#ifdef BSP_USING_TIMER6
	static drv_timer_info_t _info6 = DRV_TIMER_CONFIG_DEFAULT;
#endif
#ifdef BSP_USING_TIMER7
	static drv_timer_info_t _info7 = DRV_TIMER_CONFIG_DEFAULT;
#endif
#ifdef BSP_USING_TIMER8
	static drv_timer_info_t _info8 = DRV_TIMER_CONFIG_DEFAULT;
#endif
#ifdef BSP_USING_TIMER9
	static drv_timer_info_t _info9 = DRV_TIMER_CONFIG_DEFAULT;
#endif

static drv_timer_device_t timer_dev[] =
{
#ifdef BSP_USING_TIMER1
    TIMER1_CONFIG,
#endif

#ifdef BSP_USING_TIMER2
    TIMER2_CONFIG,
#endif

#ifdef BSP_USING_TIMER3
    TIMER3_CONFIG,
#endif

#ifdef BSP_USING_TIMER4
    TIMER4_CONFIG,
#endif

#ifdef BSP_USING_TIMER5
    TIMER5_CONFIG,
#endif

#ifdef BSP_USING_TIMER6
    TIMER6_CONFIG,
#endif

#ifdef BSP_USING_TIMER7
    TIMER7_CONFIG,
#endif

#ifdef BSP_USING_TIMER8
    TIMER8_CONFIG,
#endif

#ifdef BSP_USING_TIMER9
    TIMER9_CONFIG,
#endif
};

drv_timer_device_t *drv_timer_device_get(ETIMER_INDEX timx)
{	
#ifdef BSP_USING_TIMER1
	if(timx == ETIMER_DEV1)		return &timer_dev[TIMER1_INDEX];
#endif
#ifdef BSP_USING_TIMER2
	if(timx == ETIMER_DEV2)		return &timer_dev[TIMER2_INDEX];
#endif
#ifdef BSP_USING_TIMER3
	if(timx == ETIMER_DEV3)		return &timer_dev[TIMER3_INDEX];
#endif
#ifdef BSP_USING_TIMER4
	if(timx == ETIMER_DEV4)		return &timer_dev[TIMER4_INDEX];
#endif
#ifdef BSP_USING_TIMER5
	if(timx == ETIMER_DEV5)		return &timer_dev[TIMER5_INDEX];
#endif
#ifdef BSP_USING_TIMER6
	if(timx == ETIMER_DEV6)		return &timer_dev[TIMER6_INDEX];
#endif
#ifdef BSP_USING_TIMER7
	if(timx == ETIMER_DEV7)		return &timer_dev[TIMER7_INDEX];
#endif
#ifdef BSP_USING_TIMER8
	if(timx == ETIMER_DEV8) 	return &timer_dev[TIMER8_INDEX];
#endif
#ifdef BSP_USING_TIMER9
	if(timx == ETIMER_DEV9) 	return &timer_dev[TIMER9_INDEX];
#endif
	return NULL;
}


static void drv_timer_start(drv_timer_device_t *timer)
{
	uint32_t Period = timer->tim_info->period;
	uint32_t opmode = timer->tim_info->opmode;
	
    /* set tim cnt */
    TIM_SetCnt(timer->tim_handle, 0);
    /* set tim arr */
    TIM_SetAutoReload(timer->tim_handle, Period - 1);
    if (opmode == TIMER_MODE_ONESHOT)
    {
        /* set timer to single mode */
        TIM_SelectOnePulseMode(timer->tim_handle, TIM_OPMODE_SINGLE);
    }
    else
    {
        TIM_SelectOnePulseMode(timer->tim_handle, TIM_OPMODE_REPET);
    }
	
	TIM_ConfigInt(timer->tim_handle,TIM_INT_UPDATE,ENABLE);
    TIM_ClrIntPendingBit(timer->tim_handle, TIM_INT_UPDATE);
    /* start timer */
    TIM_Enable(timer->tim_handle,ENABLE);
}

static void drv_timer_stop(drv_timer_device_t *timer)
{
    /* stop timer */
    TIM_Enable(timer->tim_handle,DISABLE);
    /* set tim cnt */
    TIM_SetCnt(timer->tim_handle, 0);
	TIM_ConfigInt(timer->tim_handle,TIM_INT_UPDATE ,DISABLE);
    TIM_ClrIntPendingBit(timer->tim_handle,TIM_INT_UPDATE);
}

static uint32_t drv_timer_counter_get(drv_timer_device_t *timer)
{ 
    return timer->tim_handle->CNT;
}

int drv_timer_init(ETIMER_INDEX timx)
{
	drv_timer_device_t *timer = drv_timer_device_get(timx);
	RCC_ClocksType RCC_Clock;
    RCC_GetClocksFreqValue(&RCC_Clock);
    uint32_t input_clock;
    if ((TIM1 == timer->tim_handle) || (TIM8 == timer->tim_handle))
    {
        RCC_ConfigTim18Clk(RCC_TIM18CLK_SRC_SYSCLK);
        input_clock = RCC_Clock.SysclkFreq;
    }
    else
    {
        if (1 == (RCC_Clock.HclkFreq/RCC_Clock.Pclk1Freq))
            input_clock = RCC_Clock.Pclk1Freq;
        else
            input_clock = RCC_Clock.Pclk1Freq * 2;
    }

    /* timer clock enable */
    ec32_msp_timer_init(timer->tim_handle);
	
	uint32_t period = timer->tim_info->period;
    uint32_t psc = timer->tim_info->psc;
	
    timer->tim_info->int_clock = input_clock;
	timer->tim_info->cycles = input_clock/psc/period*timer->tim_info->timeout;
	timer->tim_info->cyc_reload = timer->tim_info->cycles;

	TIM_TimeBaseInitType TIM_TimeBaseStructure;
	TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period = period;
    TIM_TimeBaseStructure.Prescaler = psc-1;
    TIM_TimeBaseStructure.ClkDiv = TIM_CLK_DIV1;
    TIM_TimeBaseStructure.RepetCnt = 0;

    if (timer->tim_info->cntmode == TIMER_CNTMODE_UP)
    {
        TIM_TimeBaseStructure.CntMode = TIM_CNT_MODE_UP;
    }
    else
    {
        TIM_TimeBaseStructure.CntMode = TIM_CNT_MODE_DOWN;
    }
    TIM_InitTimeBase(timer->tim_handle, &TIM_TimeBaseStructure);
	
	NVIC_InitType NVIC_InitStructure;
    /* Enable the TIMx global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = timer->tim_irqn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	return 0;
}

int drv_timer_ctrl(ETIMER_INDEX timx,drv_timer_ctrl_t cmd, void *arg)
{
	drv_timer_device_t *timer = drv_timer_device_get(timx);
    switch(cmd)
    {
        case TIMER_CTRL_INFO_SET:
        {
           	drv_timer_info_t *tim_info = timer->tim_info;
			drv_timer_info_t *set_info = (drv_timer_info_t *)arg;

			if(set_info != NULL)
			{
				tim_info->int_clock  = 	set_info->int_clock;
				tim_info->psc		 =	set_info->psc;
				tim_info->period	 =	set_info->period;
				tim_info->timeout	 =	set_info->timeout;
				tim_info->cycles	 =	set_info->cycles;
				tim_info->cyc_reload =	set_info->cyc_reload;
				tim_info->overflow	 =	set_info->overflow;
				tim_info->opmode	 =	set_info->opmode;
				tim_info->cntmode	 =	set_info->cntmode;
			}
			drv_timer_init(timx);
			break;
        }
		case TIMER_CTRL_START:
		{
			drv_timer_start(timer);
			break;
		}
		case TIMER_CTRL_STOP:
		{
			drv_timer_stop(timer);
			break;
		}
		case TIMER_CTRL_INFO_GET:
		{
			drv_timer_info_t *tim_info = timer->tim_info;
			drv_timer_info_t *get_info = (drv_timer_info_t *)arg;

			if(get_info != NULL)
			{
				get_info->int_clock  = 	tim_info->int_clock;
				get_info->psc		 =	tim_info->psc;
				get_info->period	 =	tim_info->period;
				get_info->timeout	 =	tim_info->timeout;
				get_info->cycles	 =	tim_info->cycles;
				get_info->cyc_reload =	tim_info->cyc_reload;
				get_info->overflow	 =	tim_info->overflow;
				get_info->opmode	 =	tim_info->opmode;
				get_info->cntmode	 =	tim_info->cntmode;
			}
			break;
		}
		case TIMER_CTRL_CNT_GET:
		{
			drv_timer_counter_get(timer);
			break;
		}
        default:
        {
            return -1;
        }
    }
    return 0;
}

#ifdef BSP_USING_TIMER1
void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM1, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV1);
        TIM_ClrIntPendingBit(TIM1, TIM_INT_UPDATE);
    }
}
#endif

#ifdef BSP_USING_TIMER2
void TIM2_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM2, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV2);
        TIM_ClrIntPendingBit(TIM2, TIM_INT_UPDATE);
    }
}
#endif

#ifdef BSP_USING_TIMER3
void TIM3_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM3, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV3);
        TIM_ClrIntPendingBit(TIM3, TIM_INT_UPDATE);
    }
}
#endif

#ifdef BSP_USING_TIMER4
void TIM4_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM4, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV4);
        TIM_ClrIntPendingBit(TIM4, TIM_INT_UPDATE);
    }
}
#endif

#ifdef BSP_USING_TIMER5
void TIM5_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM5, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV5);
        TIM_ClrIntPendingBit(TIM5, TIM_INT_UPDATE);
    }
}
#endif

#ifdef BSP_USING_TIMER6
void TIM6_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM6, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV6);
        TIM_ClrIntPendingBit(TIM6, TIM_INT_UPDATE);
    }
}
#endif

#ifdef BSP_USING_TIMER7
void TIM7_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM7, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV7);
        TIM_ClrIntPendingBit(TIM7, TIM_INT_UPDATE);
    }
}
#endif

#ifdef BSP_USING_TIMER8
void TIM8_UP_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM8, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV8);
        TIM_ClrIntPendingBit(TIM8, TIM_INT_UPDATE);
    }
}
#endif

#ifdef BSP_USING_TIMER9
void TIM9_IRQHandler(void)
{
    if(TIM_GetIntStatus(TIM9, TIM_INT_UPDATE) == SET)
    {
        hal_timer_device_isr(ETIMER_DEV9);
        TIM_ClrIntPendingBit(TIM9, TIM_INT_UPDATE);
    }
}
#endif

#endif

