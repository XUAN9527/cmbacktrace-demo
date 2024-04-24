#ifndef __BOARD_H
#define __BOARD_H

#include "br_config.h"
#include "stdint.h"

#define VECTOR_TABLE_OFFSET         0x00000000      		//向量表偏移地址
#define SYSTICK_PER_SEC             EC_TICK_PER_SECOND      //systick interrupt(ms)

/* only for n32l40x,need to change */
#define FLASH_PAGE_SIZE                 (2 * 1024)
#define BR32_FLASH_START_ADRESS         ((uint32_t)0x08000000)
#define BR32_FLASH_SIZE                 (128 * 1024)
#define BR32_FLASH_END_ADDRESS          ((uint32_t)(BR32_FLASH_START_ADRESS + BR32_FLASH_SIZE))

/* Internal SRAM memory size[Kbytes] <48>, Default: 48*/
#define BR32_SRAM_SIZE                  (24)
#define BR32_SRAM_END                   (0x20000000 + BR32_SRAM_SIZE * 1024)

void System_board_init(void);                 //系统初始化

#endif

