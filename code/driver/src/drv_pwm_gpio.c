#include "drv_pwm_gpio.h"
#include "n32l40x.h"
#include "br_config.h"

#ifdef BSP_USING_PWM_INPUT
int timer_gpio_init(ETIMER_SOURSE tim,ETIMER_REMAP remap,ETIMER_CH channel,ETIMER_MODE mode)
{
    GPIO_InitType GPIO_InitStructure;
	GPIO_InitStruct(&GPIO_InitStructure);		//RESET GPIO
	uint32_t gpio_mode = 0;
	uint8_t ch = (uint8_t)channel;
	if(mode == ETIMER_MODE_PWM)	
	{
		gpio_mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pull = GPIO_No_Pull;
	}
	else if(mode == ETIMER_MODE_INPUT)
	{
		gpio_mode = GPIO_Mode_Input;
//		gpio_mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;			//fix.....
	}
	else
		return -1;
	
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = (GPIO_ModeType)gpio_mode;
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_High;
	GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
	
	switch (tim)
	{
		case ETIMER_1:
			RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1,ENABLE);
			if(remap == ETR_REMAP_0)
			{
				if(ch == 1)													
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
					GPIO_InitStructure.Pin = GPIO_PIN_8;
					GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
				}
				if(ch == 2)													
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
					GPIO_InitStructure.Pin = GPIO_PIN_9;
					GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
				}
				if(ch == 3)													
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
					GPIO_InitStructure.Pin = GPIO_PIN_10;
					GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
				}
			}
			break;
		case ETIMER_2:	
			RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM2,ENABLE);
			if(remap == ETR_REMAP_0)
			{
				if(ch == 1)
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM2;
					GPIO_InitStructure.Pin = GPIO_PIN_0;
					GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
				}

				if(ch == 2)
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM2;
					GPIO_InitStructure.Pin = GPIO_PIN_1;
					GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
				}
				
				if(ch == 3)
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM2;
					GPIO_InitStructure.Pin = GPIO_PIN_2;
					GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
				}
			}
			
			if(remap == ETR_REMAP_1)
			{
				if(ch == 1)													
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_TIM2;
					GPIO_InitStructure.Pin = GPIO_PIN_15;
					GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
				}
				
				if(ch == 2)													
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM2;
					GPIO_InitStructure.Pin = GPIO_PIN_3;
					GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
				}
			}
			break;
		case ETIMER_3:
			// RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3,ENABLE);
			// if(remap == ETR_REMAP_0)
			// {
			// 	if(ch == 3)
			// 	{
			// 		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
			// 		GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
			// 		GPIO_InitStructure.Pin = GPIO_PIN_0;
			// 		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
			// 	}
				
			// 	if(ch == 4)
			// 	{
			// 		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
			// 		GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM3;
			// 		GPIO_InitStructure.Pin = GPIO_PIN_1;
			// 		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
			// 	}
			// }
		
			// if(remap == ETR_REMAP_1)
			// {
			// 	if(ch == 2)
			// 	{
			// 		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
			// 		GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_TIM3;
			// 		GPIO_InitStructure.Pin = GPIO_PIN_5;
			// 		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
			// 	}
			// }
			break;
		case ETIMER_4:		
			RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM4,ENABLE);
			if(remap == ETR_REMAP_0)
			{
				if(ch == 1)
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM4;
					GPIO_InitStructure.Pin = GPIO_PIN_6;
					GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
				}
				
				if(ch == 2)
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM4;
					GPIO_InitStructure.Pin = GPIO_PIN_7;
					GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
				}

				if(ch == 3)
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM4;
					GPIO_InitStructure.Pin = GPIO_PIN_8;
					GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
				}
				
				if(ch == 4)
				{
					RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
					GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM4;
					GPIO_InitStructure.Pin = GPIO_PIN_9;
					GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
				}
			}
			break;
		case ETIMER_5:
			// if(remap == ETR_REMAP_0)										//pb6,motor input
			// {
			// 	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM5,ENABLE);
			// 	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
			// 	if(ch == 1)
			// 	{
			// 		GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM5;
			// 		GPIO_InitStructure.Pin = GPIO_PIN_0;
			// 	}
			
			// 	if(ch == 2)
			// 	{
			// 		GPIO_InitStructure.GPIO_Alternate = GPIO_AF7_TIM5;
			// 		GPIO_InitStructure.Pin = GPIO_PIN_1;
			// 	}
				
			// 	if(ch == 3)
			// 	{
			// 		GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_TIM5;
			// 		GPIO_InitStructure.Pin = GPIO_PIN_2;
			// 	}
				
			// 	if(ch == 4)
			// 	{
			// 		GPIO_InitStructure.GPIO_Alternate = GPIO_AF7_TIM5;
			// 		GPIO_InitStructure.Pin = GPIO_PIN_3;
			// 	}
			// 	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
			// }
			break;
		case ETIMER_6:
			break;
		case ETIMER_7:
			break;
		case ETIMER_8:
			break;
		case ETIMER_9:
			RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM9,ENABLE);
			RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB,ENABLE);
			if(ch == 1)
			{
				GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM9;
				GPIO_InitStructure.Pin = GPIO_PIN_12;
				GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
			}
			
			if(ch == 2)
			{
				GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM9;
				GPIO_InitStructure.Pin = GPIO_PIN_13;
				GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
			}
			
			// if(ch == 3)
			// {
			// 	GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM9;
			// 	GPIO_InitStructure.Pin = GPIO_PIN_14;
			// 	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
			// }
			
			// if(ch == 4)
			// {
			// 	GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM9;
			// 	GPIO_InitStructure.Pin = GPIO_PIN_15;
			// 	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
			// }
			
			break;
		default:
		{
			break;
		}
	}
	return 0;
}
#endif
