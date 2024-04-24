#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

#include <stdint.h>

#define PIN_LOW                         0x00        //pin_read,pin_write
#define PIN_HIGH                        0x01

#define PIN_MODE_OUTPUT                 0x00        //pin_mode
#define PIN_MODE_INPUT                  0x01
#define PIN_MODE_INPUT_PULLUP           0x02
#define PIN_MODE_INPUT_PULLDOWN         0x03
#define PIN_MODE_OUTPUT_OD              0x04

void drv_pin_mode(uint32_t pin, uint32_t mode);
void drv_pin_write(uint32_t pin, uint32_t value);
int drv_pin_read(uint32_t pin);

#endif
