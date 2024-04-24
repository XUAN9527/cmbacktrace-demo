#include "drv_i2c.h"

#ifdef BSP_USING_SOFT_I2C
#include "n32l40x.h"

#include "module_pin.h"

/* USER'S CONFIG START */
#define i2c_pin_mode(pin,mode)		module_pin_mode(pin,mode)
#define i2c_pin_write(pin,level) 	module_pin_write(pin,level)
#define i2c_pin_read(pin) 			module_pin_read(pin)

#define I2C_PIN_HIGH				PIN_HIGH
#define I2C_PIN_LOW 				PIN_LOW
#define I2C_PIN_MODE_OUTPUT_OD		PIN_MODE_OUTPUT_OD
/* USER'S CONFIG END */

static const struct drv_soft_i2c_config i2c_pin_config[] =
{
#ifdef BSP_USING_SOFT_I2C1
    I2C1_SOFT_BUS_CONFIG,
#endif
#ifdef BSP_USING_SOFT_I2C2
    I2C2_SOFT_BUS_CONFIG,
#endif
};

static struct drv_i2c_bus_device i2c_bus_dev[sizeof(i2c_pin_config)/sizeof(struct drv_soft_i2c_config)];

static void drv_i2c_gpio_init(struct drv_soft_i2c_config *i2c)
{
    struct drv_soft_i2c_config* cfg = i2c;

    i2c_pin_mode(cfg->scl, I2C_PIN_MODE_OUTPUT_OD);				//PIN_MODE_OUTPUT
    i2c_pin_mode(cfg->sda, I2C_PIN_MODE_OUTPUT_OD);

    i2c_pin_write(cfg->scl, I2C_PIN_HIGH);
    i2c_pin_write(cfg->sda, I2C_PIN_HIGH);
}

/**
 * This function sets the sda pin.
 *
 * @param n32 config class.
 * @param The sda pin state.
 */
static void n32_set_sda(void *data, int state)
{
    struct drv_soft_i2c_config* cfg = (struct drv_soft_i2c_config*)data;
    if (state)
    {
        i2c_pin_write(cfg->sda, I2C_PIN_HIGH);
    }
    else
    {
        i2c_pin_write(cfg->sda, I2C_PIN_LOW);
    }
}

/**
 * This function sets the scl pin.
 *
 * @param n32 config class.
 * @param The scl pin state.
 */
static void n32_set_scl(void *data, int state)
{
    struct drv_soft_i2c_config* cfg = (struct drv_soft_i2c_config*)data;
    if (state)
    {
        i2c_pin_write(cfg->scl, I2C_PIN_HIGH);
    }
    else
    {
        i2c_pin_write(cfg->scl, I2C_PIN_LOW);
    }
}

/**
 * This function gets the sda pin state.
 *
 * @param The sda pin state.
 */
static int n32_get_sda(void *data)
{
    struct drv_soft_i2c_config* cfg = (struct drv_soft_i2c_config*)data;
    return i2c_pin_read(cfg->sda);
}

/**
 * This function gets the scl pin state.
 *
 * @param The scl pin state.
 */
static int n32_get_scl(void *data)
{
    struct drv_soft_i2c_config* cfg = (struct drv_soft_i2c_config*)data;
    return i2c_pin_read(cfg->scl);
}
/**
 * The time delay function.
 *
 * @param microseconds.
 */
static void n32_udelay(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    ticks = us * reload / (1000000 / DRV_TICK_PER_SECOND);
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

static int drv_i2c_bus_unlock(const struct drv_soft_i2c_config *cfg)
{
    int i = 0;

    if (I2C_PIN_LOW == i2c_pin_read(cfg->sda))
    {
        while (i++ < 9)
        {
            i2c_pin_write(cfg->scl, I2C_PIN_HIGH);
            n32_udelay(100);
            i2c_pin_write(cfg->scl, I2C_PIN_LOW);
            n32_udelay(100);
        }
    }
    if (I2C_PIN_LOW == i2c_pin_read(cfg->sda))
    {
        return -1;
    }

    return 0;
}

//static const struct drv_i2c_bit_ops drv_bit_ops_default =
//{
//    .data     = NULL,
//    .set_sda  = n32_set_sda,
//    .set_scl  = n32_set_scl,
//    .get_sda  = n32_get_sda,
//    .get_scl  = n32_get_scl,
//    .udelay   = n32_udelay,
//    .delay_us = 1,
//    .timeout  = 100
//};

struct drv_i2c_bus_device *get_i2c_device(EI2C_DEVICE dev_e)
{
	int size = sizeof(i2c_pin_config)/sizeof(struct drv_soft_i2c_config);
	uint8_t dev_num = (uint8_t)dev_e;
	for(int i=0;i<size;i++)
	{
		if(i2c_pin_config[i].i2c_num == dev_num)
			return &i2c_bus_dev[i];
	}
	return NULL;
}


/* I2C initialization function */
int drv_hw_i2c_init(EI2C_DEVICE dev_e)
{
	int size = sizeof(i2c_pin_config)/sizeof(struct drv_soft_i2c_config);
	struct drv_i2c_bus_device *dev = get_i2c_device(dev_e);
	uint8_t dev_num = (uint8_t)dev_e;
	struct drv_soft_i2c_config *pin_cfg = NULL;
	struct drv_i2c_bit_ops *ops = &dev->ops;
	
	for(int i=0;i<size;i++)
	{
		if(i2c_pin_config[i].i2c_num == dev_num)
		{
			pin_cfg = (struct drv_soft_i2c_config *)&i2c_pin_config[i];
			break;
		}
	}
	if(pin_cfg == NULL)
		return -1;
    ops->data  = (void*)pin_cfg;
    ops->set_sda  = n32_set_sda;
    ops->set_scl  = n32_set_scl;
    ops->get_sda  = n32_get_sda;
    ops->get_scl  = n32_get_scl;
    ops->udelay   = n32_udelay;
    ops->delay_us = 1;
    ops->timeout  = 5;
    
    drv_i2c_gpio_init(pin_cfg);
    drv_i2c_bus_unlock(pin_cfg);
    return 0;
}

#endif
