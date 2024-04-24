#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "drv_pwm_input.h"
#include "drv_pwm_gpio.h"
#include "br_config.h"

#ifdef BSP_USING_PWM_INPUT

static struct ec_tim_device timer_dev[]={{TIM1},{TIM2},{TIM3},{TIM4},{TIM5},{TIM6},{TIM7},{TIM8},{TIM9}};
static struct capture_list Tim2Cap[4]={0};
static struct capture_list Tim3Cap[4]={0};
static struct capture_list Tim4Cap[4]={0};
static struct capture_list Tim5Cap[4]={0};

static int drv_pwm_get(struct ec_tim_device *dev,struct ec_pwm_input_config *pwm)
{
    uint32_t ar, div, cc1, cc2, cc3, cc4;
    uint32_t channel = pwm->channel;
	
    ar   = dev->tim_handle->AR;
    div  = dev->tim_handle->PSC;
    cc1  = dev->tim_handle->CCDAT1;
    cc2  = dev->tim_handle->CCDAT2;
    cc3  = dev->tim_handle->CCDAT3;
    cc4  = dev->tim_handle->CCDAT4;
	
    pwm->period = ar ;
	pwm->psc = (div + 1);
	
    if(channel == 1)
        pwm->value = (cc1 + 1) ;
    if(channel == 2)
        pwm->value = (cc2 + 1) ;
    if(channel == 3)
        pwm->value = (cc3 + 1) ;
    if(channel == 4)
        pwm->value = (cc4 + 1) ;
    return 0;
}

static int drv_input_get(struct ec_tim_device *dev,struct ec_pwm_input_config *inputcap)
{	
	uint8_t ch = inputcap->channel-1;
	struct capture_list *cap = NULL;
	
	if(dev == &timer_dev[1])			cap = &Tim2Cap[ch];
	else if(dev == &timer_dev[2])		cap = &Tim3Cap[ch];
	else if(dev == &timer_dev[3])		cap = &Tim4Cap[ch];
	else if(dev == &timer_dev[4])		cap = &Tim5Cap[ch];
	else
		return -1;
	
	inputcap->value = cap->timfreq;
	return 0;
}

static int drv_pwm_set(struct ec_tim_device *dev,struct ec_pwm_input_config *pwm)
{
    /* Init timer pin and enable clock */

    RCC_ClocksType RCC_Clock;
    RCC_GetClocksFreqValue(&RCC_Clock);
    uint32_t input_clock;
    if ((TIM1 == dev->tim_handle) || (TIM8 == dev->tim_handle))
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

	/* TIM clock Freq */
	dev->int_clock = input_clock;
	
    /* Convert nanosecond to frequency and duty cycle. */
    uint32_t period = (uint32_t)pwm->period ;
    uint32_t psc = (uint32_t)pwm->psc;

    /* TIMe base configuration */
    TIM_TimeBaseInitType TIM_TIMeBaseStructure;
    TIM_InitTimBaseStruct(&TIM_TIMeBaseStructure);
    TIM_TIMeBaseStructure.Period = period;
    TIM_TIMeBaseStructure.Prescaler = psc - 1;
    TIM_TIMeBaseStructure.ClkDiv = 0;
    TIM_TIMeBaseStructure.CntMode = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(dev->tim_handle, &TIM_TIMeBaseStructure);

    uint32_t pulse = (uint32_t)pwm->value;
    /* PWM1 Mode configuration */
    OCInitType  TIM_OCInitStructure;
    TIM_InitOcStruct(&TIM_OCInitStructure);
    TIM_OCInitStructure.OcMode = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse = pulse;
    TIM_OCInitStructure.OcPolarity = TIM_OC_POLARITY_HIGH;

    uint8_t channel = pwm->channel;
    if(channel == 1)
    {
        TIM_InitOc1(dev->tim_handle, &TIM_OCInitStructure);
		TIM_ConfigOc1Preload(dev->tim_handle, TIM_OC_PRE_LOAD_ENABLE);
    }
    else if(channel == 2)
    {
        TIM_InitOc2(dev->tim_handle, &TIM_OCInitStructure);
		TIM_ConfigOc2Preload(dev->tim_handle, TIM_OC_PRE_LOAD_ENABLE);
    }
    else if(channel == 3)
    {
        TIM_InitOc3(dev->tim_handle, &TIM_OCInitStructure);
		TIM_ConfigOc3Preload(dev->tim_handle, TIM_OC_PRE_LOAD_ENABLE);
    }
    else if(channel == 4)
    {
        TIM_InitOc4(dev->tim_handle, &TIM_OCInitStructure);
		TIM_ConfigOc4Preload(dev->tim_handle, TIM_OC_PRE_LOAD_ENABLE);
    }
	else
	{
		return -1;
	}
	
	TIM_ConfigArPreload(dev->tim_handle, ENABLE);
	TIM_EnableCtrlPwmOutputs(dev->tim_handle, ENABLE);
	
    return 0;
}

uint8_t input_get_ch(uint8_t ch)
{
	if(ch == 1)	return TIM_CH_1;
	if(ch == 2)	return TIM_CH_2;
	if(ch == 3)	return TIM_CH_3;
	if(ch == 4)	return TIM_CH_4;
	return 0x80;
}

static uint8_t input_get_irqn(struct ec_tim_device *dev)
{
	if(dev->tim_handle == TIM1)	return TIM1_UP_IRQn;
	if(dev->tim_handle == TIM2)	return TIM2_IRQn;
	if(dev->tim_handle == TIM3)	return TIM3_IRQn;
	if(dev->tim_handle == TIM4)	return TIM4_IRQn;
	if(dev->tim_handle == TIM5)	return TIM5_IRQn;
	if(dev->tim_handle == TIM6)	return TIM6_IRQn;
	if(dev->tim_handle == TIM7)	return TIM7_IRQn;
	if(dev->tim_handle == TIM8)	return TIM8_UP_IRQn;
	if(dev->tim_handle == TIM9)	return TIM9_IRQn;
	return 0x80;
}

static int drv_input_set(struct ec_tim_device *dev,struct ec_pwm_input_config *inputcap)
{
	/* Init timer pin and enable clock */	
    RCC_ClocksType RCC_Clock;
    RCC_GetClocksFreqValue(&RCC_Clock);
    uint32_t input_clock;
    if ((TIM1 == dev->tim_handle) || (TIM8 == dev->tim_handle))
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

	/* TIM clock Freq */
	dev->int_clock = input_clock;
    /* Convert nanosecond to frequency and duty cycle. */
    uint32_t period = (uint32_t)inputcap->period ;
    uint32_t psc = inputcap->psc;

	uint8_t tim_irqn = input_get_irqn(dev);
	if(tim_irqn == 0x80)
		return -1;
	
	NVIC_InitType NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel					 = tim_irqn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIMe base configuration */
    TIM_TimeBaseInitType TIM_TIMeBaseStructure;
    TIM_InitTimBaseStruct(&TIM_TIMeBaseStructure);
    TIM_TIMeBaseStructure.Period = period;
    TIM_TIMeBaseStructure.Prescaler = psc - 1;
    TIM_TIMeBaseStructure.ClkDiv = 0;
    TIM_TIMeBaseStructure.CntMode = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(dev->tim_handle, &TIM_TIMeBaseStructure);
	
    uint8_t ch = inputcap->channel;
	uint8_t channel = input_get_ch(ch);
	if(channel == 0x80)
		return -1;
		
	TIM_ICInitType TIM_ICInitStructure;
	TIM_InitIcStruct(&TIM_ICInitStructure);
	TIM_ICInitStructure.Channel     = channel;
    TIM_ICInitStructure.IcPolarity  = TIM_IC_POLARITY_RISING;
    TIM_ICInitStructure.IcSelection = TIM_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.IcPrescaler = TIM_IC_PSC_DIV1;
    TIM_ICInitStructure.IcFilter    = 0x0;
    TIM_ICInit(dev->tim_handle, &TIM_ICInitStructure);
	return 0;
}


/*
*ec pwm control function
*@param cmd:EC_CMD_SET			
*			EC_CMD_GET			
*@return 0:success;-1:fail;
*/
int drv_pwm_control(ETIMER_SOURSE tim, ECONTROL_CMD cmd,struct ec_pwm_input_config *pwm)
{
	if((tim < ETIMER_1)||(tim > ETIMER_9))
			return -1;
	struct ec_tim_device *dev = &timer_dev[tim-1];
    switch (cmd)
    {
        case EC_CMD_SET:
			dev->pwm_input.psc	 					= pwm->psc;
			dev->pwm_input.period 					= pwm->period;
			dev->pwm_input.channel[pwm->channel-1] 	= ETIMER_MODE_PWM;
			dev->pwm_input.value[pwm->channel-1] 	= pwm->value;
            return drv_pwm_set(dev,pwm);
        case EC_CMD_GET:
            return drv_pwm_get(dev,pwm);
        default:
            return -1;
    }
}

/*
*ec pwm control function
*@param cmd:EC_CMD_SET			
*			EC_CMD_GET			
*@return 0:success;-1:fail;
*/
int drv_input_control(ETIMER_SOURSE tim, ECONTROL_CMD cmd,struct ec_pwm_input_config *inputcap)
{
	if((tim < ETIMER_1)||(tim > ETIMER_9))
			return -1;
	struct ec_tim_device *dev = &timer_dev[tim-1];
    switch (cmd)
    {
        case EC_CMD_SET:
			dev->pwm_input.psc 								= inputcap->psc;
			dev->pwm_input.period 							= inputcap->period;
			dev->pwm_input.channel[inputcap->channel-1] 	= ETIMER_MODE_INPUT;
            return drv_input_set(dev,inputcap);
        case EC_CMD_GET:
            return drv_input_get(dev,inputcap);
        default:
            return -1;
    }
}


int drv_pwm_enable(ETIMER_SOURSE tim,ETIMER_CH channel, bool enable)
{
	struct ec_tim_device *dev = &timer_dev[tim-1];
	uint8_t ch = (uint8_t)channel;
    /* Get the value of channel */
    if (!enable)
    {
        if(ch == 1)
        {
			TIM_EnableCapCmpCh(dev->tim_handle, TIM_CH_1, TIM_CAP_CMP_DISABLE);
        }
        else if(ch == 2)
        {
            TIM_EnableCapCmpCh(dev->tim_handle, TIM_CH_2, TIM_CAP_CMP_DISABLE);
        }
        else if(ch == 3)
        {
            TIM_EnableCapCmpCh(dev->tim_handle, TIM_CH_3, TIM_CAP_CMP_DISABLE);
        }
        else if(ch == 4)
        {
            TIM_EnableCapCmpCh(dev->tim_handle, TIM_CH_4, TIM_CAP_CMP_DISABLE);
        }
		else
		{
			return -1;
		}
    }
    else
    {
        if(ch == 1)
        {
			TIM_EnableCapCmpCh(dev->tim_handle, TIM_CH_1, TIM_CAP_CMP_ENABLE);
        }
        else if(ch == 2)
        {
            TIM_EnableCapCmpCh(dev->tim_handle, TIM_CH_2, TIM_CAP_CMP_ENABLE);
        }
        else if(ch == 3)
        {
            TIM_EnableCapCmpCh(dev->tim_handle, TIM_CH_3, TIM_CAP_CMP_ENABLE);
        }
        else if(ch == 4)
        {
            TIM_EnableCapCmpCh(dev->tim_handle, TIM_CH_4, TIM_CAP_CMP_ENABLE);
        }
		else
		{
			return -1;
		}
    }
    TIM_Enable(dev->tim_handle, ENABLE);
    return 0;
}


int drv_input_enable(ETIMER_SOURSE tim,ETIMER_CH channel, bool enable)
{
	struct ec_tim_device *dev 	= &timer_dev[tim-1];
	struct capture_list *cap 	= NULL;
	uint8_t ch = (uint8_t)channel;	
	
	if(tim == ETIMER_2)			cap = &Tim2Cap[ch-1];
	else if(tim == ETIMER_3)	cap = &Tim3Cap[ch-1];
	else if(tim == ETIMER_4)	cap = &Tim4Cap[ch-1];
	else if(tim == ETIMER_5)	cap = &Tim5Cap[ch-1];
	else
		return -1;
	if (!enable)
    {
		if(ch == 1)
			TIM_ConfigInt(dev->tim_handle, TIM_INT_CC1 | TIM_INT_UPDATE, DISABLE);
		else if(ch == 2)
			TIM_ConfigInt(dev->tim_handle, TIM_INT_CC2 | TIM_INT_UPDATE, DISABLE);
		else if(ch == 3)
			TIM_ConfigInt(dev->tim_handle, TIM_INT_CC3 | TIM_INT_UPDATE, DISABLE);
		else if(ch == 4)
			TIM_ConfigInt(dev->tim_handle, TIM_INT_CC4 | TIM_INT_UPDATE, DISABLE);
		else
			return -1;		
		cap->timeout_flag = 0;
	}else
	{	
		if(ch == 1)
			TIM_ConfigInt(dev->tim_handle, TIM_INT_CC1 | TIM_INT_UPDATE, ENABLE);
		else if(ch == 2)
			TIM_ConfigInt(dev->tim_handle, TIM_INT_CC2 | TIM_INT_UPDATE, ENABLE);
		else if(ch == 3)
			TIM_ConfigInt(dev->tim_handle, TIM_INT_CC3 | TIM_INT_UPDATE, ENABLE);
		else if(ch == 4)
			TIM_ConfigInt(dev->tim_handle, TIM_INT_CC4 | TIM_INT_UPDATE, ENABLE);
		else
			return -1;
		cap->timeout_flag = 1;
	}
	TIM_Enable(dev->tim_handle, ENABLE);
    return 0;
}		

//pwm set pulse...
int drv_pwm_compare_set(ETIMER_SOURSE tim,ETIMER_CH channel,uint16_t compare)
{
	struct ec_tim_device *dev = &timer_dev[tim-1];
	uint8_t ch = (uint8_t)channel;
	if(ch == 1)
		TIM_SetCmp1(dev->tim_handle, compare);
	else if(ch == 2)
		TIM_SetCmp2(dev->tim_handle, compare);
	else if(ch == 3)
		TIM_SetCmp3(dev->tim_handle, compare);
	else if(ch == 4)
		TIM_SetCmp4(dev->tim_handle, compare);
	else
		return -1;
	return 0;
}

/*
*ec hw pwm init function
*@param tim:ETIMER_SOURSE_1 ~ ETIMER_SOURSE_8		
*@param tim:ETIMER_REMAP_0 ~ ETIMER_REMAP_3	 			
*@param tim:ETIMER_CH_1 ~ ETIMER_CH_4		
*@param tim:ETIMER_MODE_PWM
*			ETIMER_MODE_INPUT
*@return 0:success;-1:fail;
*/
int drv_hw_pwm_input_init(ETIMER_SOURSE tim,ETIMER_REMAP remap,ETIMER_CH channel,ETIMER_MODE mode)
{
	uint8_t get_ch 				= (uint8_t)channel;
	timer_gpio_init(tim,remap,channel,mode);
	if(mode == ETIMER_MODE_PWM)
	{	
		/* Init pwm data */	
		struct ec_pwm_input_config pwm 		= {get_ch,32,999,0};		//init 1K
		return drv_pwm_control(tim,EC_CMD_SET,&pwm);
	}else if(mode == ETIMER_MODE_INPUT)
	{
		/* Init inputcapture data */
		struct ec_pwm_input_config inputcap 	= {get_ch,1,0xFFFF};
		return drv_input_control(tim,EC_CMD_SET,&inputcap);
	}else
		return -1;
}

static int tim_InputCapture_callback(struct ec_tim_device *dev,const signed char ic_num,const uint16_t cap_read)
{
	struct capture_list *cap = NULL;
	
	if(dev->tim_handle == TIM2)			cap = &Tim2Cap[ic_num];
	else if(dev->tim_handle == TIM3)	cap = &Tim3Cap[ic_num];
	else if(dev->tim_handle == TIM4)	cap = &Tim4Cap[ic_num];
	else if(dev->tim_handle == TIM5)	cap = &Tim5Cap[ic_num];
	else
		return -1;
	
	if(cap->first_input == 0)
	{
		cap->first_input = 1;
		return 0;
	}
		
	if (cap->captureflag == 0)
    {
        cap->ic_readvalue1 = cap_read;
        cap->captureflag = 1;
		cap->timenterirqcnt = 0;
    }
    else if (cap->captureflag == 1)
    {
        cap->ic_readvalue2 = cap_read;
		uint32_t in_clock = dev->int_clock;
        if((cap->ic_readvalue2 > cap->ic_readvalue1)&&(cap->timenterirqcnt == 0))
        {
            cap->capture = (cap->ic_readvalue2 - cap->ic_readvalue1);
        }
        else
        {
            cap->capture = ((dev->pwm_input.period+1) * cap->timenterirqcnt + cap->ic_readvalue2 - cap->ic_readvalue1);
        }
        /* Frequency computation */
        cap->timfreq  = (uint32_t)in_clock/dev->pwm_input.psc/cap->capture;	
		cap->input_done = 1;		
        cap->captureflag  = 0;
		cap->timeout_flag = 0;
		cap->timeout = 0;
    }
	return 0;
}

#ifdef BSP_USING_INPUT_TIM2

void TIM2_IRQHandler(void)
{
	signed char ic_num 				= -1;
	uint16_t cap_read 			= 0;
	struct ec_tim_device *dev 	= &timer_dev[1];
	TIM_Module* TIMx 			= TIM2;
	
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC1) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC1);
		cap_read = TIM_GetCap1(TIMx);
		ic_num = 0;	
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC2) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC2);
		cap_read = TIM_GetCap2(TIMx);
		ic_num = 1;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC3) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC3);
		cap_read = TIM_GetCap3(TIMx);
		ic_num = 2;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
    if (TIM_GetIntStatus(TIMx, TIM_INT_CC4) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC4);
		cap_read = TIM_GetCap4(TIMx);
		ic_num = 3;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	
	if (TIM_GetIntStatus(TIMx, TIM_INT_UPDATE) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_UPDATE);
		for(int i=0;i<4;i++)
		{
			struct capture_list *cap = &Tim2Cap[i];
			cap->timenterirqcnt++;
			if(cap->timeout_flag)
			{
				cap->timeout++;
			}
		}
    }
	
	for(int i=0;i<4;i++)
	{
		struct capture_list *cap = &Tim2Cap[i];
		if(cap->input_done == 1)
		{
			static uint32_t times = 0;
			times++;
			cap->input_done 	= 0;
			cap->timeout_flag 	= 1;
			cap->timeout 		= 0;
			if(times > INPUT_SUCCESS_TIMES)
			{
				times = 0;
				tim_user_get_callback(ETIMER_2,(ETIMER_CH)(i+1),EINPUT_DONE,cap->timfreq);
			}
		}
		else
		{
			if(cap->timeout > INPUT_FAILED_TIMEOUT)
			{
				cap->timeout_status = 1;
				cap->timeout_flag 	= 0;
				cap->timeout 		= 0;
			}
			
			if(cap->timeout_status == 1)
			{
				cap->timeout_flag 	= 1;
				cap->timeout_status = 0;
				cap->captureflag  = 0;
				cap->timfreq = 0;
				tim_user_get_callback(ETIMER_2,(ETIMER_CH)(i+1),EINPUT_TIMEOUT,cap->timfreq);
			}
		}
	}
}
#endif

#ifdef BSP_USING_INPUT_TIM3
void TIM3_IRQHandler(void)
{
	signed char ic_num 				= -1;
	uint16_t cap_read 			= 0;
	struct ec_tim_device *dev 	= &timer_dev[2];
	TIM_Module* TIMx = TIM3;
	
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC1) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC1);
		cap_read = TIM_GetCap1(TIMx);
		ic_num = 0;	
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC2) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC2);
		cap_read = TIM_GetCap2(TIMx);
		ic_num = 1;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC3) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC3);
		cap_read = TIM_GetCap3(TIMx);
		ic_num = 2;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
    if (TIM_GetIntStatus(TIMx, TIM_INT_CC4) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC4);
		cap_read = TIM_GetCap4(TIMx);
		ic_num = 3;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	
	if (TIM_GetIntStatus(TIMx, TIM_INT_UPDATE) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_UPDATE);
		for(int i=0;i<4;i++)
		{
			struct capture_list *cap = &Tim3Cap[i];
			cap->timenterirqcnt++;
			if(cap->timeout_flag)
			{
				cap->timeout++;
			}
		}
    }
	
	for(int i=0;i<4;i++)
	{
		struct capture_list *cap = &Tim3Cap[i];
		if(cap->input_done == 1)
		{
			static uint32_t times = 0;
			times++;
			cap->input_done 	= 0;
			cap->timeout_flag 	= 1;
			cap->timeout 		= 0;
			if(times > INPUT_SUCCESS_TIMES)
			{
				times = 0;
				tim_user_get_callback(ETIMER_3,(ETIMER_CH)(i+1),EINPUT_DONE,cap->timfreq);
			}
		}
		else
		{
			if(cap->timeout > INPUT_FAILED_TIMEOUT)
			{
				cap->timeout_status = 1;
				cap->timeout_flag 	= 0;
				cap->timeout 		= 0;
			}
			
			if(cap->timeout_status == 1)
			{
				cap->timeout_flag 	= 1;
				cap->timeout_status = 0;
				cap->captureflag  = 0;
				cap->timfreq = 0;
				tim_user_get_callback(ETIMER_3,(ETIMER_CH)(i+1),EINPUT_TIMEOUT,cap->timfreq);
			}
		}
	}
}
#endif

#ifdef BSP_USING_INPUT_TIM4
void TIM4_IRQHandler(void)
{
	signed char ic_num 			= -1;
	uint16_t cap_read 			= 0;
	struct ec_tim_device *dev 	= &timer_dev[3];
	TIM_Module* TIMx = TIM4;
	
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC1) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC1);
		cap_read = TIM_GetCap1(TIMx);
		ic_num = 0;	
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC2) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC2);
		cap_read = TIM_GetCap2(TIMx);
		ic_num = 1;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC3) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC3);
		cap_read = TIM_GetCap3(TIMx);
		ic_num = 2;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
    if (TIM_GetIntStatus(TIMx, TIM_INT_CC4) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC4);
		cap_read = TIM_GetCap4(TIMx);
		ic_num = 3;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	
	if (TIM_GetIntStatus(TIMx, TIM_INT_UPDATE) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_UPDATE);
		for(int i=0;i<4;i++)
		{
			struct capture_list *cap = &Tim4Cap[i];
			cap->timenterirqcnt++;
			if(cap->timeout_flag)
			{
				cap->timeout++;
			}
		}
    }
	
	for(int i=0;i<4;i++)
	{
		struct capture_list *cap = &Tim4Cap[i];
		if(cap->input_done == 1)
		{
			static uint32_t times = 0;
			times++;
			cap->input_done 	= 0;
			cap->timeout_flag 	= 1;
			cap->timeout 		= 0;
			if(times > INPUT_SUCCESS_TIMES)
			{
				times = 0;
				tim_user_get_callback(ETIMER_4,(ETIMER_CH)(i+1),EINPUT_DONE,cap->timfreq);
			}
		}
		else
		{
			if(cap->timeout > INPUT_FAILED_TIMEOUT)
			{
				cap->timeout_status = 1;
				cap->timeout_flag 	= 0;
				cap->timeout 		= 0;
			}
			
			if(cap->timeout_status == 1)
			{
				cap->timeout_flag 	= 1;
				cap->timeout_status = 0;
				cap->captureflag  = 0;
				cap->timfreq = 0;
				tim_user_get_callback(ETIMER_4,(ETIMER_CH)(i+1),EINPUT_TIMEOUT,cap->timfreq);
			}
		}
	}
}
#endif

#ifdef BSP_USING_INPUT_TIM5
void TIM5_IRQHandler(void)
{
	signed char ic_num 			= -1;
	uint16_t cap_read 			= 0;
	struct ec_tim_device *dev 	= &timer_dev[4];
	TIM_Module* TIMx = TIM5;
	
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC1) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC1);
		cap_read = TIM_GetCap1(TIMx);
		ic_num = 0;	
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC2) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC2);
		cap_read = TIM_GetCap2(TIMx);
		ic_num = 1;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	if (TIM_GetIntStatus(TIMx, TIM_INT_CC3) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC3);
		cap_read = TIM_GetCap3(TIMx);
		ic_num = 2;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
    if (TIM_GetIntStatus(TIMx, TIM_INT_CC4) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_CC4);
		cap_read = TIM_GetCap4(TIMx);
		ic_num = 3;
		tim_InputCapture_callback(dev,ic_num,cap_read);
    }
	
	if (TIM_GetIntStatus(TIMx, TIM_INT_UPDATE) == SET)
    {
        TIM_ClrIntPendingBit(TIMx, TIM_INT_UPDATE);
		for(int i=0;i<4;i++)
		{
			struct capture_list *cap = &Tim5Cap[i];
			cap->timenterirqcnt++;
			if(cap->timeout_flag)
			{
				cap->timeout++;
			}
		}
    }
	
	for(int i=0;i<4;i++)
	{
		struct capture_list *cap = &Tim5Cap[i];
		if(cap->input_done == 1)
		{
			static uint32_t times = 0;
			times++;
			cap->input_done 	= 0;
			cap->timeout_flag 	= 1;
			cap->timeout 		= 0;
			if(times > INPUT_SUCCESS_TIMES)
			{
				times = 0;
				tim_user_get_callback(ETIMER_5,(ETIMER_CH)(i+1),EINPUT_DONE,cap->timfreq);
			}
		}
		else
		{
			if(cap->timeout > INPUT_FAILED_TIMEOUT)
			{
				cap->timeout_status = 1;
				cap->timeout_flag 	= 0;
				cap->timeout 		= 0;
			}
			
			if(cap->timeout_status == 1)
			{
				cap->timeout_flag 	= 1;
				cap->timeout_status = 0;
				cap->captureflag  = 0;
				cap->timfreq = 0;
				tim_user_get_callback(ETIMER_5,(ETIMER_CH)(i+1),EINPUT_TIMEOUT,cap->timfreq);
			}
		}
	}
}
#endif

int tim_user_get_callback(ETIMER_SOURSE tim,ETIMER_CH channel,EINPUTEVENT event,uint32_t timfreq) __attribute__((weak));
int tim_user_get_callback(ETIMER_SOURSE tim,ETIMER_CH channel,EINPUTEVENT event,uint32_t timfreq)
{
	return 0;
}

int drv_pwm_input_enabled(ETIMER_SOURSE timx, ETIMER_CH channel,ETIMER_MODE mode,bool enable)
{
	if(mode == ETIMER_MODE_PWM)
		return drv_pwm_enable(timx, channel, enable);
	else if(mode == ETIMER_MODE_INPUT)
		return drv_input_enable(timx, channel, enable);
	return -1;
}

int drv_pwm_input_control(ETIMER_SOURSE tim, ETIMER_MODE mode, ECONTROL_CMD cmd,struct ec_pwm_input_config *pi)
{
	if(mode == ETIMER_MODE_PWM)
		return drv_pwm_control(tim, cmd, pi);
	else if(mode == ETIMER_MODE_INPUT)
		return drv_input_control(tim, cmd, pi);
	return -1;
}

int drv_set_pwm_pulse_percent(ETIMER_SOURSE timx, ETIMER_CH channel,uint8_t percent)
{
	struct ec_pwm_input_config pwm = {0};
	pwm.channel = channel;
	drv_pwm_control(timx, EC_CMD_GET,&pwm);
	pwm.value = pwm.period*percent/100;
	if(percent == 100)										//n32的bug
		pwm.value += 1;
	drv_pwm_compare_set(timx,channel,pwm.value);			//只设置pulse值
	return 0;
}

int drv_set_pwm_pulse_Thousandths(ETIMER_SOURSE timx, ETIMER_CH channel,uint16_t Thousandths)
{
	struct ec_pwm_input_config pwm = {0};
	pwm.channel = channel;
	drv_pwm_control(timx, EC_CMD_GET,&pwm);
	pwm.value = pwm.period*Thousandths/1000;
	if(Thousandths == 1000)										//n32的bug
		pwm.value += 1;
	drv_pwm_compare_set(timx,channel,pwm.value);			//只设置pulse值
	return 0;
}


/*  
 *	pwm and inputcapture samples 
 */

/*
int my_pwm_input_sample(ETIMER_SOURSE timx,ETIMER_CH ch,ETIMER_REMAP remap,ETIMER_MODE mode)
{
	struct ec_pwm_input_config info = {ch,64,999,499};
	drv_hw_pwm_input_init(timx,ch,remap,mode);
	if(mode == MTIMER_MODE_PWM)
		drv_pwm_input_control(timx,PWM_CMD_SET,&info);
	else if(mode == MTIMER_MODE_INPUT)
		drv_pwm_input_control(timx,INPUT_CMD_SET,&info);
	else
		return -1;
	drv_pwm_input_enabled(timx,ch,mode,MC_CMD_ENABLE);
	if(mode == MTIMER_MODE_PWM)
	{
		logDebug("tim[%d] channel[%d] pwm init success!",timx,ch);
	}else{
		logDebug("tim[%d] channel[%d] inputcapture init success!",timx,ch);
	}
	return 0;
}

int my_get_pwm_data(ETIMER_SOURSE timx,ETIMER_CH ch)
{
	struct ec_pwm_input_config pwm = {ch};
	drv_pwm_input_control(timx,PWM_CMD_SET,&pwm);
	logDebug("pre = %d,channel= %d,value = %d",pwm.period,pwm.channel,pwm.value);
	return 0;
}

int my_get_input_data(ETIMER_SOURSE timx,ETIMER_CH ch)
{
	struct ec_pwm_input_config inputc = {ch};
	drv_pwm_input_control(timx,INPUT_CMD_SET,&inputc);
	logDebug("channel= %d，value = %d",inputc.channel,inputc.value);
	return 0;
}
*/

/* other file defined */
/*
int tim_user_get_callback(ETIMER_SOURSE tim,ETIMER_CH channel,MINPUTEVENT event,uint32_t timfreq)
{
	if(tim == MTIMER_1)
	{
		if(event == EINPUT_DONE)
		{				
			logDebug("T1 channel=%d,timfreq=%d",channel,timfreq);
		}
		else if(event == EINPUT_TIMEOUT)
		{
			logDebug("T1 inputcapture failed!");
		}
	}
	
	if(tim == MTIMER_2)
	{
		if(event == EINPUT_DONE)
		{				
			logDebug("T2 channel=%d,timfreq=%d",channel,timfreq);
		}
		else if(event == EINPUT_TIMEOUT)
		{
			logDebug("T2 inputcapture failed!");
		}
	}
	
	if(tim == MTIMER_3)
	{
		if(event == EINPUT_DONE)
		{				
			logDebug("T3 channel=%d,timfreq=%d",channel,timfreq);
		}
		else if(event == EINPUT_TIMEOUT)
		{
			logDebug("T3 inputcapture failed!");
		}
	}

	if(tim == MTIMER_4)
	{
		if(event == EINPUT_DONE)
		{				
			logDebug("T4 channel=%d,timfreq=%d",channel,timfreq);
		}
		else if(event == EINPUT_TIMEOUT)
		{
			logDebug("T4 inputcapture failed!");
		}
	}
	
	if(tim == MTIMER_5)
	{
		if(event == EINPUT_DONE)
		{				
			logDebug("T5 channel=%d,timfreq=%d",channel,timfreq);
		}
		else if(event == EINPUT_TIMEOUT)
		{
			logDebug("T5 inputcapture failed!");
		}
	}
	
	return 0;
}
*/

#endif
