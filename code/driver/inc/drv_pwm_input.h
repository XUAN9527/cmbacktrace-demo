#ifndef __DRV_PWM_INPUT_H
#define __DRV_PWM_INPUT_H
#include <stdint.h>
#include "n32l40x.h"

#define INPUT_SUCCESS_TIMES		0x00000001		//0xFFFFFFFF , INPUT_SUCCESS_TIMES+1=2
#define INPUT_FAILED_TIMEOUT 	0x00000800		//0xFFFFFFFF , 超时1s/20K=0.05ms一次，2000次，100ms

/*	GPIO映射表 ：
		 ETR_REMAP_0	(AFIO)			ETR_REMAP_1		(AFIO)			ETR_REMAP_2		(AFIO)

TIM1_ETR 	
TIM1_CH1 																	
TIM1_CH2 	
TIM1_CH3 	
TIM1_CH4 									
TIM1_BKIN 	
TIM1_CH1N 	
TIM1_CH2N 	
TIM1_CH3N 	

TIM8_ETR 	
TIM8_CH1 	
TIM8_CH2 	
TIM8_CH3 	
TIM8_CH4 	
TIM8_BKIN 	
TIM8_CH1N 	
TIM8_CH2N 	
TIM8_CH3N 	
																		
TIM2_CH1 		PA15   ----	 AF5
TIM2_CH2 									PB3   ----   AF2
TIM2_CH3 	
TIM2_CH4 	

TIM3_CH1 	
TIM3_CH2 									PB5   ----	 AF4
TIM3_CH3 	
TIM3_CH4 	

TIM4_CH1 		PB6   ----	 AF2
TIM4_CH2 
TIM4_CH3 	
TIM4_CH4 	

TIM5_CH1 	
TIM5_CH2 	
TIM5_CH3 	
TIM5_CH4 	

TIM9_CH1 		PB12  ----  AF1
*/

typedef enum{
	ETIMER_1 = 1,
	ETIMER_2,
	ETIMER_3,
	ETIMER_4,
	ETIMER_5,
	ETIMER_6,
	ETIMER_7,
	ETIMER_8,
	ETIMER_9
}ETIMER_SOURSE;

typedef enum{		//bit[11:10]
	ETR_REMAP_0 = 0,	
	ETR_REMAP_1,
	ETR_REMAP_2,
	ETR_REMAP_3
}ETIMER_REMAP;

typedef enum{		
	ETIMER_CH_1 = 1,	
	ETIMER_CH_2,
	ETIMER_CH_3,
	ETIMER_CH_4
}ETIMER_CH;
	
typedef enum{		
	ETIMER_MODE_NONE = 0X00,
	ETIMER_MODE_PWM = 0x01,
	ETIMER_MODE_INPUT = 0x10
}ETIMER_MODE;

typedef enum{	 
	EC_CMD_SET = 1,			
	EC_CMD_GET			
}ECONTROL_CMD;

typedef enum{	 
	EINPUT_NONE = 0,
	EINPUT_DONE = 1,
	EINPUT_TIMEOUT = 2
}EINPUTEVENT;

struct ec_pwm_input_config
{
    uint8_t channel; 	/* channel 1~4 */
	uint32_t psc;		/* 预分频 1~65536 */
    uint32_t period; 	/* 计数值    0~0xFFFF */
    uint32_t value;   	/* pwm模式下pluse：在period范围内，input模式下FREQ */
};

struct ec_pi_config
{
	uint32_t psc;				/* 预分频 0~0xFFFF */
    uint32_t period; 			/* 计数值  0~0xFFFF */
    ETIMER_MODE channel[4]; 	/* channel 1~4；参考ETIMER_MODE选择模式 */
    uint32_t value[4];   		/* pwm模式下pluse，input模式下FREQ */
};

struct  ec_tim_device
{
    TIM_Module* tim_handle;
	uint32_t int_clock;
	struct ec_pi_config pwm_input;
};

struct capture_list
{
    uint16_t ic_readvalue1;
	uint16_t ic_readvalue2;
	uint32_t captureflag;
	uint32_t capture;
	uint32_t timfreq;
	uint32_t timenterirqcnt;
	uint8_t first_input;
	uint8_t input_done;
	uint8_t timeout_flag;
	uint8_t timeout_status;
	uint32_t timeout;
};

int drv_hw_pwm_input_init(ETIMER_SOURSE tim,ETIMER_REMAP remap,ETIMER_CH channel,ETIMER_MODE mode);
int drv_pwm_input_control(ETIMER_SOURSE tim, ETIMER_MODE mode, ECONTROL_CMD cmd,struct ec_pwm_input_config *pi);
int drv_pwm_input_enabled(ETIMER_SOURSE timx, ETIMER_CH channel,ETIMER_MODE mode,bool enable);
int drv_set_pwm_pulse_percent(ETIMER_SOURSE timx, ETIMER_CH channel,uint8_t percent);
int drv_set_pwm_pulse_Thousandths(ETIMER_SOURSE timx, ETIMER_CH channel,uint16_t Thousandths);
int tim_user_get_callback(ETIMER_SOURSE tim,ETIMER_CH channel,EINPUTEVENT event,uint32_t timfreq);

#endif

