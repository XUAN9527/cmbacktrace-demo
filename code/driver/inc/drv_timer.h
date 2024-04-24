#ifndef __DRV_TIMER_H
#define __DRV_TIMER_H

#include <stddef.h>
#include <stdint.h>
#include "n32l40x.h"
#include "br_config.h"
#include "hal_timer.h"

#ifdef BSP_USING_TIMER1
#ifndef TIMER1_CONFIG
#define TIMER1_CONFIG               	  \
		{								  \
		   .tim_handle	  = TIM1,		  \
		   .tim_irqn	  = TIM1_UP_IRQn, \
		   .tim_info	  = &_info1, 	  \
		   .rx_indicate   = NULL		  \
		}
#endif /* TIM1_CONFIG */
#endif /* BSP_USING_TIMER1 */

#ifdef BSP_USING_TIMER2
#ifndef TIMER2_CONFIG
#define TIMER2_CONFIG                 \
    {                                 \
       .tim_handle    = TIM2,         \
       .tim_irqn      = TIM2_IRQn,    \
       .tim_info	  = &_info2,   	  \
       .rx_indicate   = NULL		  \
    }
#endif /* TIM2_CONFIG */
#endif /* BSP_USING_TIMER2 */

#ifdef BSP_USING_TIMER3
#ifndef TIMER3_CONFIG
#define TIMER3_CONFIG              		  \
		{								  \
		   .tim_handle	  = TIM3,		  \
		   .tim_irqn	  = TIM3_IRQn,	  \
		   .tim_info	  = &_info3, 	  \
		   .rx_indicate   = NULL		  \
		}
#endif /* TIM3_CONFIG */
#endif /* BSP_USING_TIMER3 */

#ifdef BSP_USING_TIMER4
#ifndef TIMER4_CONFIG
#define TIMER4_CONFIG               	  \
		{								  \
		   .tim_handle	  = TIM4,		  \
		   .tim_irqn	  = TIM4_IRQn,	  \
		   .tim_info	  = &_info4, 	  \
		   .rx_indicate   = NULL		  \
		}
#endif /* TIM4_CONFIG */
#endif /* BSP_USING_TIMER4 */

#ifdef BSP_USING_TIMER5
#ifndef TIMER5_CONFIG
#define TIMER5_CONFIG                	  \
		{								  \
		   .tim_handle	  = TIM5,		  \
		   .tim_irqn	  = TIM5_IRQn,	  \
		   .tim_info	  = &_info5, 	  \
		   .rx_indicate   = NULL		  \
		}
#endif /* TIM5_CONFIG */
#endif /* BSP_USING_TIMER5 */

#ifdef BSP_USING_TIMER6
#ifndef TIMER6_CONFIG
#define TIMER6_CONFIG                	  \
		{								  \
		   .tim_handle	  = TIM6,		  \
		   .tim_irqn	  = TIM6_IRQn,	  \
		   .tim_info	  = &_info6, 	  \
		   .rx_indicate   = NULL		  \
		}
#endif /* TIM6_CONFIG */
#endif /* BSP_USING_TIMER6 */

#ifdef BSP_USING_TIMER7
#ifndef TIMER7_CONFIG
#define TIMER7_CONFIG               	  \
		{								  \
		   .tim_handle	  = TIM7,		  \
		   .tim_irqn	  = TIM7_IRQn,	  \
		   .tim_info	  = &_info7, 	  \
		   .rx_indicate   = NULL		  \
		}
#endif /* TIM7_CONFIG */
#endif /* BSP_USING_TIMER7 */

#ifdef BSP_USING_TIMER8
#ifndef TIMER8_CONFIG
#define TIMER8_CONFIG               	  \
		{								  \
		   .tim_handle	  = TIM8,		  \
		   .tim_irqn	  = TIM8_UP_IRQn, \
		   .tim_info	  = &_info8, 	  \
		   .rx_indicate   = NULL		  \
		}
#endif /* TIM8_CONFIG */
#endif /* BSP_USING_TIMER8 */
		
#ifdef BSP_USING_TIMER9
#ifndef TIMER9_CONFIG
#define TIMER9_CONFIG               	  \
		{								  \
		   .tim_handle	  = TIM9,		  \
		   .tim_irqn	  = TIM9_IRQn,	  \
		   .tim_info	  = &_info9, 	  \
		   .rx_indicate   = NULL		  \
		}
#endif /* TIM9_CONFIG */
#endif /* BSP_USING_TIMER9 */		
		
typedef struct  drv_timer_device
{
    TIM_Module* tim_handle;
    IRQn_Type   tim_irqn;
	drv_timer_info_t *tim_info;
	int (*rx_indicate)(ETIMER_INDEX timx,uint16_t size);
}drv_timer_device_t;

int drv_timer_init(ETIMER_INDEX timx);
int drv_timer_ctrl(ETIMER_INDEX timx,drv_timer_ctrl_t cmd, void *arg);
drv_timer_device_t *drv_timer_device_get(ETIMER_INDEX timx);

#endif
