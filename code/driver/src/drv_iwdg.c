#include "drv_iwdg.h"
#include "br_config.h" 

#ifdef BSP_USING_IWDG

#define LSI_VALUE       40000
#define PRESCAL_VALUE   256

struct ec32_iwdg_cfg{
    uint8_t is_start;
    EC_IWDG_CONFIG_PRESCALER prescaler;
    uint16_t reload;
};

static struct ec32_iwdg_cfg ec32_iwdg;

/* USER DRIVER CHANGE START */
static void EC_IWDG_Prescaler_DivSet(EC_IWDG_CONFIG_PRESCALER IWDG_prescaler)
{
    IWDG_WriteConfig(IWDG_WRITE_ENABLE);
    IWDG_SetPrescalerDiv(IWDG_prescaler);
    IWDG_WriteConfig(IWDG_WRITE_DISABLE);
}

static void EC_IWDG_Reload_Counter(uint16_t reload_value)
{
	IWDG_WriteConfig(IWDG_WRITE_ENABLE);
    IWDG_CntReload(reload_value);
    IWDG_WriteConfig(IWDG_WRITE_DISABLE);
}

static void EC_IWDG_Refresh(void)
{
    IWDG_ReloadKey();
}

static void EC_IWDG_Enable(void)
{
	IWDG_ReloadKey();
    IWDG_Enable();
}
/* USER DRIVER CHANGE END */

/**
 * @brief  Init IWDG
 */
int drv_iwdg_init(uint32_t *sec)
{
    ec32_iwdg.is_start = 0;
    ec32_iwdg.prescaler = IWDG_PRES_256;
    ec32_iwdg.reload = 0x00000FFF;
    EC_IWDG_Prescaler_DivSet(ec32_iwdg.prescaler);
	if(*sec)
		drv_iwdg_control(EC_CTRL_IWDG_SET_TIMEOUT,(void *)sec);
	else
		EC_IWDG_Reload_Counter(ec32_iwdg.reload);
    return 0;
}

/**
 * @brief  Contorl IWDG
 * @param cmd
 *  @argEC_CTRL_IWDG_GET_TIMEOUT
 *  @argEC_CTRL_IWDG_SET_TIMEOUT
 *  @argEC_CTRL_IWDG_KEEPALIVE
 *  @argEC_CTRL_IWDG_START
 * @param arg
 */
int drv_iwdg_control(int cmd, void *arg)
{
    switch (cmd)
    {
		/* feed the watchdog */
		case EC_CTRL_IWDG_KEEPALIVE:
			EC_IWDG_Refresh();
			break;
		
		/* set watchdog timeout */
		case EC_CTRL_IWDG_SET_TIMEOUT:              
			if(LSI_VALUE)
			{
				ec32_iwdg.reload = (*((uint32_t*)arg)) * LSI_VALUE / PRESCAL_VALUE ;
				if (ec32_iwdg.reload > 0xFFF)
					return -1;
				EC_IWDG_Reload_Counter(ec32_iwdg.reload);
			}
			break;
			
		case EC_CTRL_IWDG_GET_TIMEOUT:
			if(LSI_VALUE)
				(*((uint32_t*)arg)) = ec32_iwdg.reload * PRESCAL_VALUE / LSI_VALUE;
			break;

		case EC_CTRL_IWDG_START:
			if (ec32_iwdg.is_start != 1)
			{
				ec32_iwdg.is_start = 1;
				EC_IWDG_Enable();
			}
			break;
		default:
			return -1;
    }
    return 0;
}

int drv_iwdg_get_timeout(uint32_t *sec)
{
	return drv_iwdg_control(DEV_CTRL_IWDG_GET_TIMEOUT,(void *)sec);
}

int drv_iwdg_set_timeout(uint32_t *sec)				//the most times set 27s				
{
	return drv_iwdg_control(DEV_CTRL_IWDG_SET_TIMEOUT,(void *)sec);
}

int drv_iwdg_refresh(void)
{
	return drv_iwdg_control(DEV_CTRL_IWDG_KEEPALIVE,(void *)0);
}

int drv_iwdg_start(void)
{
	return drv_iwdg_control(DEV_CTRL_IWDG_START,(void *)0);
}


/* demo
int iwdg_init_start(void)
{
	int result = 0;
	uint32_t sec = 2;
	drv_iwdg_init(&sec);
	result = hal_iwdg_start();
	if(result != 0)
		return -1;
	return result;
}
*/

#endif /* RT_USING_WDT */
