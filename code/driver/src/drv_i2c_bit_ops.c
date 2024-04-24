/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2012-04-25     weety         first version
 */
#include <stdio.h>
#include "drv_i2c_bit_ops.h"
#include "module_i2c.h"
#include "log.h"

#define SET_SDA(ops, val)   ops->set_sda(ops->data, val)
#define SET_SCL(ops, val)   ops->set_scl(ops->data, val)
#define GET_SDA(ops)        ops->get_sda(ops->data)
#define GET_SCL(ops)        ops->get_scl(ops->data)

static void i2c_delay(struct drv_i2c_bit_ops *ops)
{
    ops->udelay((ops->delay_us + 1) >> 1);
}

static void i2c_delay2(struct drv_i2c_bit_ops *ops)
{
    ops->udelay(ops->delay_us);
}

#define SDA_L(ops)          SET_SDA(ops, 0)
#define SDA_H(ops)          SET_SDA(ops, 1)
#define SCL_L(ops)          SET_SCL(ops, 0)

/**
 * release scl line, and wait scl line to high.
 */
static int SCL_H(struct drv_i2c_bit_ops *ops)
{
    uint64_t start;

    SET_SCL(ops, 1);
    if (!ops->get_scl)
        goto done;
    start = i2c_tick_get();
    while (!GET_SCL(ops))
    {
        if ((i2c_tick_get() - start) > ops->timeout)
            return -2;
        i2c_delay(ops);
    }
done:
    i2c_delay(ops);
    return 0;
}

static void i2c_start(struct drv_i2c_bit_ops *ops)
{
    SDA_L(ops);
    i2c_delay(ops);
    SCL_L(ops);
}

static void i2c_restart(struct drv_i2c_bit_ops *ops)
{
    SDA_H(ops);
    SCL_H(ops);
    i2c_delay(ops);
    SDA_L(ops);
    i2c_delay(ops);
    SCL_L(ops);
}

static void i2c_stop(struct drv_i2c_bit_ops *ops)
{
    SDA_L(ops);
    i2c_delay(ops);
    SCL_H(ops);
    i2c_delay(ops);
    SDA_H(ops);
    i2c_delay2(ops);
}

int i2c_waitack(struct drv_i2c_bit_ops *ops)
{
    int ack;

    SDA_H(ops);
    i2c_delay(ops);

    if (SCL_H(ops) < 0)
    {
//        logVerbose("wait ack timeout");
        return -2;
    }

    ack = !GET_SDA(ops);    /* ACK : SDA pin is pulled low */
//    logVerbose("%s", ack ? "ACK" : "NACK");

    SCL_L(ops);

    return ack;
}

static int i2c_writeb(struct drv_i2c_bit_ops *bus, uint8_t data)
{
    int i;
    uint8_t bit;

    struct drv_i2c_bit_ops *ops = bus;
//	logVerbose("data = 0x%02X ",data);
    for (i = 7; i >= 0; i--)
    {
        SCL_L(ops);
        bit = (data >> i) & 1;
        SET_SDA(ops, bit);
        i2c_delay(ops);
        if (SCL_H(ops) < 0)
        {
//            logVerbose("i2c_writeb: 0x%02x, "
//                  "wait scl pin high timeout at bit %d",
//                  data, i);
            return -2;
        }
    }
    SCL_L(ops);
    i2c_delay(ops);

    return i2c_waitack(ops);
}

static int i2c_readb(struct drv_i2c_bit_ops *bus)
{
    uint8_t i;
    uint8_t data = 0;
    struct drv_i2c_bit_ops *ops = bus;

    SDA_H(ops);
    i2c_delay(ops);
    for (i = 0; i < 8; i++)
    {
        data <<= 1;

        if (SCL_H(ops) < 0)
        {
//            logVerbose("i2c_readb: wait scl pin high "
//                  "timeout at bit %d", 7 - i);

            return -2;
        }

        if (GET_SDA(ops))
            data |= 1;
        SCL_L(ops);
        i2c_delay2(ops);
    }

    return data;
}

static int i2c_send_bytes(struct drv_i2c_bit_ops *bus,
                          struct drv_i2c_msg *msg)
{
    uint32_t ret;
    uint32_t bytes = 0;
    const uint8_t *ptr = msg->buf;
    uint32_t count = msg->len;
    uint16_t ignore_nack = msg->flags & DRV_I2C_IGNORE_NACK;

    while (count > 0)
    {
        ret = i2c_writeb(bus, *ptr);

        if ((ret > 0) || (ignore_nack && (ret == 0)))
        {
            count --;
            ptr ++;
            bytes ++;
        }
        else if (ret == 0)
        {
//            logVerbose("send bytes: NACK.");
            return 0;
        }
        else
        {
//            logVerbose("send bytes: error %d", ret);
            return ret;
        }
    }

    return bytes;
}

static int i2c_send_ack_or_nack(struct drv_i2c_bit_ops *bus, int ack)
{
    struct drv_i2c_bit_ops *ops = bus;

    if (ack)
        SET_SDA(ops, 0);
    i2c_delay(ops);
    if (SCL_H(ops) < 0)
    {
//        logVerbose("ACK or NACK timeout.");
        return -2;
    }
    SCL_L(ops);

    return 0;
}

static int i2c_recv_bytes(struct drv_i2c_bit_ops *bus,
                          struct drv_i2c_msg        *msg)
{
    int val;
    uint32_t bytes = 0;   /* actual bytes */
    uint8_t *ptr = msg->buf;
    uint32_t count = msg->len;
    const uint32_t flags = msg->flags;

    while (count > 0)
    {
        val = i2c_readb(bus);
        if (val >= 0)
        {
            *ptr = val;
            bytes ++;
        }
        else
        {
            break;
        }

        ptr ++;
        count --;

//        logVerbose("recieve bytes: 0x%02x, %s",
//              val, (flags & DRV_I2C_NO_READ_ACK) ?
//              "(No ACK/NACK)" : (count ? "ACK" : "NACK"));

        if (!(flags & DRV_I2C_NO_READ_ACK))
        {
            val = i2c_send_ack_or_nack(bus, count);
            if (val < 0)
                return val;
        }
    }

    return bytes;
}

static int i2c_send_address(struct drv_i2c_bit_ops *bus,
                            uint8_t                addr,
                            uint32_t                retries)
{
    struct drv_i2c_bit_ops *ops = bus;
    uint32_t i;
    int ret = 0;

    for (i = 0; i <= retries; i++)
    {
        ret = i2c_writeb(bus, addr);
        if (ret == 1 || i == retries)
            break;
//        logVerbose("send stop condition");
        i2c_stop(ops);
        i2c_delay2(ops);
//        logVerbose("send start condition");
        i2c_start(ops);
    }

    return ret;
}

static int i2c_send_reg_addr(struct drv_i2c_bit_ops *bus,
                            uint8_t                reg_addr,
                            uint32_t                retries)
{
    struct drv_i2c_bit_ops *ops = bus;
    uint32_t i;
    int ret = 0;

    for (i = 0; i <= retries; i++)
    {
        ret = i2c_writeb(bus, reg_addr);
        if (ret == 1 || i == retries)
            break;
//        logVerbose("send stop condition");
        i2c_stop(ops);
        i2c_delay2(ops);
//        logVerbose("send start condition");
        i2c_start(ops);
    }

    return ret;
}

/* 适配7bit，未适配10bit的reg_addr */
static int i2c_bit_send_address(struct drv_i2c_bit_ops *bus,
                                struct drv_i2c_msg        *msg)		
{
    uint16_t flags = msg->flags;
    uint16_t ignore_nack = msg->flags & DRV_I2C_IGNORE_NACK;
    struct drv_i2c_bit_ops *ops = bus;

    uint8_t addr1, addr2 ,addr3;
    uint32_t retries;
    int ret;

    retries = ignore_nack ? 0 : 3;

    if (flags & DRV_I2C_ADDR_10BIT)
    {
        addr1 = 0xf0 | ((msg->addr >> 7) & 0x06);
        addr2 = msg->addr & 0xff;

//        logVerbose("addr1: %d, addr2: %d", addr1, addr2);

        ret = i2c_send_address(bus, addr1, retries);
        if ((ret != 1) && !ignore_nack)
        {
            return -8;
        }

        ret = i2c_writeb(bus, addr2);
        if ((ret != 1) && !ignore_nack)
        {
            return -8;
        }
        if (flags & DRV_I2C_RD)
        {
            i2c_restart(ops);
            addr1 |= 0x01;
            ret = i2c_send_address(bus, addr1, retries);
            if ((ret != 1) && !ignore_nack)
            {
                return -8;
            }
        }
    }
    else
    {
        /* 7-bit addr */
        addr1 = msg->addr << 1;
		addr3 = msg->reg_addr;
        if (flags & DRV_I2C_RD)
            addr1 |= 1;
        ret = i2c_send_address(bus, addr1, retries);
		if ((ret != 1) && !ignore_nack)
            return -8;
		ret = i2c_send_reg_addr(bus, addr3, retries);
        if ((ret != 1) && !ignore_nack)
            return -8;
    }
    return 0;
}

int drv_i2c_bit_xfer(struct drv_i2c_bit_ops *bus,
                 struct drv_i2c_msg msgs[],
                 uint32_t num)
{
    struct drv_i2c_msg *msg = NULL;
    struct drv_i2c_bit_ops *ops = bus;
    int i, ret;
    uint16_t ignore_nack;

    i2c_start(ops);
//    logVerbose("send start condition");
	
    for (i = 0; i < num; i++)
    {
        msg = &msgs[i];
        ignore_nack = msg->flags & DRV_I2C_IGNORE_NACK;
        if (!(msg->flags & DRV_I2C_NO_START))
        {
            if (i)
            {
                i2c_restart(ops);
            }
            ret = i2c_bit_send_address(bus, msg);
            if ((ret != 0) && !ignore_nack)
            {
                goto out;
            }
        }
        if (msg->flags & DRV_I2C_RD)
        {
            ret = i2c_recv_bytes(bus, msg);
            if (ret >= 1){
//			    logVerbose("read %d byte%s", ret, ret == 1 ? "" : "s");
			}
            if (ret < msg->len)
            {
                if (ret >= 0)
                    ret = -8;
                goto out;
            }
        }
        else
        {
            ret = i2c_send_bytes(bus, msg);
            if (ret >= 1){
//                logVerbose("write %d byte%s", ret, ret == 1 ? "" : "s");
			}
            if (ret < msg->len)
            {
                if (ret >= 0)
                    ret = -1;
                goto out;
            }
        }
    }
    ret = i;

out:	
//    logVerbose("send stop condition");
    i2c_stop(ops);

    return ret;
}



