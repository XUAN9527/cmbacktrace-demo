#include "drv_flash.h"
#include "board.h"
#include "n32l40x.h"

#ifdef BSP_USING_ON_CHIP_FLASH

void af_flash_unlock(void)
{
	FLASH_Unlock();	
}

void af_flash_lock(void)
{
	FLASH_Lock();	
}

AfErrCode af_flash_earse_one_page(uint32_t Page_Address)
{
	AfErrCode result = AF_NO_ERR;
	FLASH_STS flash_sts = FLASH_EraseOnePage(Page_Address);
	if(flash_sts != FLASH_COMPL)
		result = AF_ERASE_ERR;
	return result;
}
	
AfErrCode af_flash_program_word(uint32_t Address, uint32_t Data)
{
	AfErrCode result = AF_NO_ERR;
	FLASH_STS flash_sts = FLASH_ProgramWord(Address,Data);
	if(flash_sts != FLASH_COMPL)
		result = AF_WRITE_ERR;
	return result;
}

AfErrCode af_port_read(uint32_t addr, uint32_t *buf, uint32_t size) {
	AfErrCode result = AF_NO_ERR;
    uint8_t *buf_8 = (uint8_t *)buf;
    uint32_t i;
	
	if(addr +  size > BR32_FLASH_END_ADDRESS)
	{
		result = AF_READ_ERR;
		return result;
	}
    /*copy from flash to ram */
    for (i = 0; i < size; i++, addr ++, buf_8++) {
        *buf_8 = *(uint8_t *) addr;
    }
	return result;
}

AfErrCode af_port_erase(uint32_t addr, uint32_t size)
{
	AfErrCode result = AF_NO_ERR;
    uint32_t erase_pages, i;

	if(addr +  size > AF_END_ADDRESS)
	{
		result = AF_ERASE_ERR;
		return result;
	}
    /* calculate pages */
    erase_pages = size / AF_PAGE_SIZE;
    if (size % AF_PAGE_SIZE != 0)
        erase_pages++;

    /* start erase */
    af_flash_unlock();
    for (i = 0; i < erase_pages; i++)
    {
        if(AF_NO_ERR != af_flash_earse_one_page(addr + (AF_PAGE_SIZE * i)))
        {
        	result = AF_ERASE_ERR;
			return result;
        }
    }
    af_flash_lock();
	return result;
}

AfErrCode af_port_write(uint32_t addr, const uint32_t *buf, uint32_t size)
{
	AfErrCode result = AF_NO_ERR;
    uint32_t i;
    uint32_t read_data;

	if(addr +  size > BR32_FLASH_END_ADDRESS)
	{
		result = AF_WRITE_ERR;
		return result;
	}
    af_flash_unlock();
    for (i = 0; i < size; i += 4, buf++, addr += 4)
    {
        /* write data */
        if(AF_NO_ERR != af_flash_program_word(addr, *buf))
        {
        	result = AF_WRITE_ERR;
			return result;
        }
        read_data = *(uint32_t *)addr;
        /* check data */
        if (read_data != *buf)
        {
        	result = AF_WRITE_ERR;
            return result;
        }
    }
    af_flash_lock();
	return result;
}

#endif /* BSP_USING_ON_CHIP_FLASH */
