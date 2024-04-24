#ifndef __DRV_FLASH_H
#define __DRV_FLASH_H

#include "stdint.h"
#include "br_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AF_PAGE_SIZE 		FLASH_PAGE_SIZE
#define AF_END_ADDRESS		BR32_FLASH_END_ADDRESS

/* EasyFlash error code */
typedef enum
{
    AF_NO_ERR,
    AF_ERASE_ERR,
    AF_READ_ERR,
    AF_WRITE_ERR
} AfErrCode;

void af_flash_unlock(void);
void af_flash_lock(void);
AfErrCode af_flash_earse_one_page(uint32_t Page_Address);
AfErrCode af_flash_program_word(uint32_t Address, uint32_t Data);

AfErrCode af_port_read(uint32_t addr, uint32_t *buf, uint32_t size);
AfErrCode af_port_erase(uint32_t addr, uint32_t size);
AfErrCode af_port_write(uint32_t addr, const uint32_t *buf, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
