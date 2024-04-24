#ifndef __DRV_IWDG_H
#define __DRV_IWDG_H

#include "n32l40x.h"

//user change start
typedef uint8_t  EC_IWDG_CONFIG_PRESCALER;

#define IWDG_PRES_4         IWDG_PRESCALER_DIV4
#define IWDG_PRES_8         IWDG_PRESCALER_DIV8
#define IWDG_PRES_16        IWDG_PRESCALER_DIV16
#define IWDG_PRES_32        IWDG_PRESCALER_DIV32
#define IWDG_PRES_64        IWDG_PRESCALER_DIV64
#define IWDG_PRES_128       IWDG_PRESCALER_DIV128
#define IWDG_PRES_256       IWDG_PRESCALER_DIV256
//user change end

#define EC_CTRL_IWDG_GET_TIMEOUT    (1) /* get timeout(in seconds) */
#define EC_CTRL_IWDG_SET_TIMEOUT    (2) /* set timeout(in seconds) */
#define EC_CTRL_IWDG_GET_TIMELEFT   (3) /* get the left time before reboot(in seconds) */
#define EC_CTRL_IWDG_KEEPALIVE      (4) /* refresh watchdog */
#define EC_CTRL_IWDG_START          (5) /* start watchdog */
#define EC_CTRL_IWDG_STOP           (6) /* stop watchdog */

#define DEV_CTRL_IWDG_GET_TIMEOUT    (1) /* get timeout(in seconds) */
#define DEV_CTRL_IWDG_SET_TIMEOUT    (2) /* set timeout(in seconds) */
#define DEV_CTRL_IWDG_GET_TIMELEFT   (3) /* get the left time before reboot(in seconds) */
#define DEV_CTRL_IWDG_KEEPALIVE      (4) /* refresh watchdog */
#define DEV_CTRL_IWDG_START          (5) /* start watchdog */
#define DEV_CTRL_IWDG_STOP           (6) /* stop watchdog */

int drv_iwdg_init(uint32_t *sec);
int drv_iwdg_control(int cmd, void *arg);
int drv_iwdg_get_timeout(uint32_t *sec);
int drv_iwdg_set_timeout(uint32_t *sec);
int drv_iwdg_refresh(void);
int drv_iwdg_start(void);

#endif
