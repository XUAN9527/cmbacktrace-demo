#ifndef __DRV_I2C_H
#define __DRV_I2C_H

#include "drv_i2c_bit_ops.h"
#include "module_i2c.h"
#include "br_config.h"

#ifdef BSP_USING_SOFT_I2C1
#define I2C1_SOFT_BUS_CONFIG                             \
    {                                                    \
        .scl = I2C1_SOFT_SCL_PIN,                        \
        .sda = I2C1_SOFT_SDA_PIN,                        \
        .i2c_num = 1                        			 \
    }
#endif

#ifdef BSP_USING_SOFT_I2C2
#define I2C2_SOFT_BUS_CONFIG                             \
    {                                                    \
        .scl = BSP_I2C2_SCL_PIN,                         \
        .sda = BSP_I2C2_SDA_PIN,                         \
        .i2c_num = 2                        			 \
    }
#endif

int drv_hw_i2c_init(EI2C_DEVICE dev_e);
struct drv_i2c_bus_device *get_i2c_device(EI2C_DEVICE dev_e);

#endif

