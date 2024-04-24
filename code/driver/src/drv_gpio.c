#include <stddef.h>
#include "drv_gpio.h"
#include "br_config.h" 
#include "n32l40x.h"

#ifdef BSP_USING_PIN

/* user define change */
#define EC32_PIN_NUMBERS 		48          //[ 32, 48, 64, 100, 144 ]

#define EC_GPIO_Pins_Set(gpio,pin)          GPIO_SetBits(gpio,pin)                  //rt32_set_pin
#define EC_GPIO_Pins_Reset(gpio,pin)        GPIO_ResetBits(gpio,pin)                //rt32_reset_pin
#define EC_ReadInputDataBit(gpio,pin)       GPIO_ReadInputDataBit(gpio,pin)         //rt32_pin_read
#define EC_RCC_Periph_Enable(rcc)           RCC_EnableAPB2PeriphClk(rcc,ENABLE)      //rt32_pin_mode

#define EC_BIT_RESET                        0                                       //bit0
#define EC_BIT_SET    						1                                       //bit1
//RCC_EnableAPB1PeriphClk,AHB,,,RCC_APB2_PERIPH_GPIOA
#define __EC32_PIN(index, rcc, gpio, gpio_index) \
{ \
0, RCC_##rcc##_PERIPH_GPIO##gpio, GPIO##gpio, GPIO_PIN_##gpio_index \
, GPIO##gpio##_PORT_SOURCE, GPIO_PIN_SOURCE##gpio_index \
}
#define __EC32_PIN_DEFAULT {-1, 0, 0, 0, 0, 0}

/* N32 GPIO driver */
struct pin_index
{
    int index;
    uint32_t rcc;
    GPIO_Module *gpio;
    uint32_t pin;
    uint8_t port_source;
    uint8_t pin_source;
};

static const struct pin_index pins[] =
{
#if (EC32_PIN_NUMBERS == 32)
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
    __EC32_PIN(2, APB2, D, 14),
    __EC32_PIN(3, APB2, D, 15),
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
    __EC32_PIN(6, APB2, A, 0),
    __EC32_PIN(7, APB2, A, 1),
    __EC32_PIN(8, APB2, A, 2),
    __EC32_PIN(9, APB2, A, 3),
    __EC32_PIN(10, APB2, A, 4),
    __EC32_PIN(11, APB2, A, 5),
    __EC32_PIN(12, APB2, A, 6),
    __EC32_PIN(13, APB2, A, 7),
    __EC32_PIN(14, APB2, B, 0),
    __EC32_PIN(15, APB2, B, 1),
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
    __EC32_PIN(18, APB2, A, 8),
    __EC32_PIN(19, APB2, A, 9),
    __EC32_PIN(20, APB2, A, 10),
    __EC32_PIN(21, APB2, A, 11),
    __EC32_PIN(22, APB2, A, 12),
    __EC32_PIN(23, APB2, A, 13),
    __EC32_PIN(24, APB2, A, 14),
    __EC32_PIN(25, APB2, A, 15),
    __EC32_PIN(26, APB2, B, 3),
    __EC32_PIN(27, APB2, B, 4),
    __EC32_PIN(28, APB2, B, 5),
    __EC32_PIN(29, APB2, B, 6),
    __EC32_PIN(30, APB2, B, 7),
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
#endif		
	
#if (EC32_PIN_NUMBERS == 48)
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
    __EC32_PIN(2, APB2, C, 13),
    __EC32_PIN(3, APB2, C, 14),
    __EC32_PIN(4, APB2, C, 15),
    __EC32_PIN(5, APB2, D, 14),
    __EC32_PIN(6, APB2, D, 15),
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
    __EC32_PIN(10, APB2, A, 0),
    __EC32_PIN(11, APB2, A, 1),
    __EC32_PIN(12, APB2, A, 2),
    __EC32_PIN(13, APB2, A, 3),
    __EC32_PIN(14, APB2, A, 4),
    __EC32_PIN(15, APB2, A, 5),
    __EC32_PIN(16, APB2, A, 6),
    __EC32_PIN(17, APB2, A, 7),
    __EC32_PIN(18, APB2, B, 0),
    __EC32_PIN(19, APB2, B, 1),
    __EC32_PIN(20, APB2, B, 2),
    __EC32_PIN(21, APB2, B, 10),
    __EC32_PIN(22, APB2, B, 11),
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
    __EC32_PIN(25, APB2, B, 12),
    __EC32_PIN(26, APB2, B, 13),
    __EC32_PIN(27, APB2, B, 14),
    __EC32_PIN(28, APB2, B, 15),
    __EC32_PIN(29, APB2, A, 8),
    __EC32_PIN(30, APB2, A, 9),
    __EC32_PIN(31, APB2, A, 10),
    __EC32_PIN(32, APB2, A, 11),
    __EC32_PIN(33, APB2, A, 12),
    __EC32_PIN(34, APB2, A, 13),
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,
    __EC32_PIN(37, APB2, A, 14),
    __EC32_PIN(38, APB2, A, 15),
    __EC32_PIN(39, APB2, B, 3),
    __EC32_PIN(40, APB2, B, 4),
    __EC32_PIN(41, APB2, B, 5),
    __EC32_PIN(42, APB2, B, 6),
    __EC32_PIN(43, APB2, B, 7),
    __EC32_PIN_DEFAULT,
    __EC32_PIN(45, APB2, B, 8),
    __EC32_PIN(46, APB2, B, 9),
    __EC32_PIN_DEFAULT,
    __EC32_PIN_DEFAULT,

#endif

};

struct pin_irq_map
{
    uint16_t             pinbit;
    uint32_t             irqbit;
    enum IRQn            irqno;
};

struct rt_pin_irq_hdr
{
    int16_t        pin;
    uint16_t       mode;
    void (*hdr)(void *args);
    void             *args;
};


#define ITEM_NUM(items) sizeof(items)/sizeof(items[0])
const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;
    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
            index = NULL;
    }
    else
    {
        index = NULL;
    }

    return index;
};

/**
 *\*\name   br32_pin_write.
 *\*\fun    Write GPIO Pin.
 *\*\param  pin : 0,1,2...BR32_PIN_NUMBERS
 *\*\param  value:PIN_LOW or PIN_HIGH
 *\*\return none
 */
void drv_pin_write(uint32_t pin, uint32_t value)
{
    const struct pin_index *index;
    index = get_pin(pin);
    if (index == NULL)
    {
        return;
    }
    if (value == PIN_LOW)
    {
        EC_GPIO_Pins_Reset(index->gpio, index->pin);
    }
    else
    {
        EC_GPIO_Pins_Set(index->gpio, index->pin);
    }
}

/**
 *\*\name   br32_pin_read.
 *\*\fun    Read GPIO Pin.
 *\*\param  pin : 0,1,2...BR32_PIN_NUMBERS
 *\*\return read PIN_LOW or PIN_HIGH
 */
int drv_pin_read(uint32_t pin)
{
    int value;
    const struct pin_index *index;

    value = PIN_LOW;

    index = get_pin(pin);
    if (index == NULL)
    {
        return value;
    }

    if (EC_ReadInputDataBit(index->gpio, index->pin) == EC_BIT_RESET)
    {
        value = PIN_LOW;
    }
    else
    {
        value = PIN_HIGH;
    }

    return value;
}

/**
 *\*\name   ec32_pin_mode.
 *\*\fun    Configure GPIO mode.
 *\*\param  pin : 0,1,2...BR32_PIN_NUMBERS
 *\*\param  mode
 *\*\          - PIN_MODE_OUTPUT
 *\*\          - PIN_MODE_INPUT
 *\*\          - PIN_MODE_INPUT_PULLUP
 *\*\          - PIN_MODE_INPUT_PULLDOWN
 *\*\          - PIN_MODE_OUTPUT_OD
 *\*\return none
 */
void drv_pin_mode(uint32_t pin, uint32_t mode)
{
    const struct pin_index *index;
    GPIO_InitType  GPIO_InitStructure;
	GPIO_InitStruct(&GPIO_InitStructure);
	
    index = get_pin(pin);
    if (index == NULL)
    {
        return;
    }

    /* GPIO Periph clock enable */
    EC_RCC_Periph_Enable(index->rcc);
	
//	/*开启复用的外设时钟使能*/
//	if(pin == 5)
//	{
//		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO,ENABLE);
//		GPIO_ConfigPinRemap(GPIOD_PORT_SOURCE,GPIO_PIN_SOURCE14,GPIO_NO_AF);	/*映射的使能*/
//	}
//	
//	if(pin == 6)
//	{
//		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO,ENABLE);
//		GPIO_ConfigPinRemap(GPIOD_PORT_SOURCE,GPIO_PIN_SOURCE15,GPIO_NO_AF);	/*映射的使能*/
//	}
	
    /* Configure GPIO_InitStructure */
    GPIO_InitStructure.Pin              = index->pin;
	
    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
        GPIO_InitStructure.GPIO_Pull  = GPIO_Pull_Up;
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull up. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
        GPIO_InitStructure.GPIO_Pull  = GPIO_Pull_Down;
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* input setting: pull up. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    }
    else
    {
        /* input setting:default. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
        GPIO_InitStructure.GPIO_Pull  = GPIO_Pull_Up;
    }
    GPIO_InitPeripheral(index->gpio, &GPIO_InitStructure);
}

#endif

