#ifndef __DRV_DELAY_H
#define __DRV_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "br_def.h"

void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_DELAY_H__ */

