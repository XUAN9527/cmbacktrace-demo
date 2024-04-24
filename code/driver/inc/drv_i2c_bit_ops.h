/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2012-04-25     weety         first version
 */

#ifndef __DRV_I2C_BIT_OPS_H
#define __DRV_I2C_BIT_OPS_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct drv_soft_i2c_config
{
	uint8_t scl;
	uint8_t sda;
	uint8_t i2c_num;
};

struct drv_i2c_msg
{
    uint16_t addr;
    uint16_t flags;
    uint16_t len;
	uint8_t reg_addr;
    uint8_t  *buf;
};

struct drv_i2c_bit_ops
{
    void *data;            /* private data for lowlevel routines */
    void (*set_sda)(void *data, int state);
    void (*set_scl)(void *data, int state);
    int (*get_sda)(void *data);
    int (*get_scl)(void *data);

    void (*udelay)(uint32_t us);

    uint32_t delay_us;  /* scl and sda line delay */
    uint32_t timeout;   /* in tick */
};

struct drv_i2c_bus_device
{
	struct drv_i2c_msg msg;
	struct drv_i2c_bit_ops ops;
};

int drv_i2c_bit_xfer(struct drv_i2c_bit_ops *bus,
                 struct drv_i2c_msg msgs[],
                 uint32_t num);

int drv_aw_i2c_bit_xfer(struct drv_i2c_bit_ops *bus,
                 struct drv_i2c_msg msgs[],
                 uint32_t num);
				 
#ifdef __cplusplus
}
#endif

#endif
