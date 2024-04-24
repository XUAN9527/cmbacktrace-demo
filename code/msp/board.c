#include <stdio.h>
#include "board.h"
#include "n32l40x.h"

#define MAIN_SYSCLK_FREQ    64000000
#define SET_HSE				0x11
#define SET_HSI				0x22

static uint32_t systemcoreclock = MAIN_SYSCLK_FREQ;           //主频

#if 1
/*
* System Clock Config Init 128M 
* @param clksourse：	SET_HSE
*						SET_HSI
* @return 
*/			
#define HSX_STARTUP_TIMEOUT ((uint16_t)0x0500)
static void System_Clock_Config(uint8_t clksourse)              // 倍频64M
{
    uint32_t pllsrc;
    uint32_t pllmul;
    uint32_t latency;
    uint32_t sysclkdiv,pclk1div, pclk2div;
	uint16_t StartUpCounter = 0;
    ErrorStatus StartUpStatus = SUCCESS;

    // Reset the RCC clock configuration to default reset state
    RCC_DeInit();
	
	if(clksourse == SET_HSE)
	{
		RCC_ConfigHse(RCC_HSE_ENABLE);
		StartUpStatus = RCC_WaitHseStable();
		pllsrc = RCC_PLL_SRC_HSE_DIV1;
	}else if(clksourse == SET_HSI)
	{
		RCC_EnableHsi(ENABLE);
		StartUpStatus = RCC_WaitHsiStable();
		pllsrc = RCC_PLL_HSI_PRE_DIV2;
	}else
		return;
	
    if(StartUpStatus == SUCCESS)
    {
        /* must use HSE as PLL source */
		
        latency  = FLASH_LATENCY_1;         //Freq>=100M,选2，Freq<50M，选0，否则1
        sysclkdiv = RCC_SYSCLK_DIV1;
        pllmul   = RCC_PLL_MUL_8;
        pclk1div = RCC_HCLK_DIV4;
        pclk2div = RCC_HCLK_DIV2;

        /* Flash wait state
        0: HCLK <= 32M
        1: HCLK <= 64M
		*/
	    FLASH->AC &= (uint32_t)((uint32_t)~FLASH_AC_LATENCY);
	    FLASH->AC |= (uint32_t)latency;
		RCC_ConfigHclk(sysclkdiv);
		RCC_ConfigPclk2(pclk2div);
		RCC_ConfigPclk1(pclk1div);
		RCC_ConfigPll(pllsrc, pllmul,RCC_PLLDIVCLK_DISABLE);
		RCC_EnablePll(ENABLE);
		while (RCC_GetFlagStatus(RCC_CTRL_FLAG_PLLRDF) == RESET);
		/* Wait till HSI is ready and if Time out is reached exit */
		do{
			StartUpCounter ++;
		} while ((!RCC_GetFlagStatus(RCC_CTRL_FLAG_PLLRDF)) && (StartUpCounter < HSX_STARTUP_TIMEOUT));
		RCC_ConfigSysclk(RCC_SYSCLK_SRC_PLLCLK);
		StartUpCounter = 0;
		do{
			StartUpCounter ++;
		}while ((RCC_GetSysclkSrc() != 0x0C) && (StartUpCounter < HSX_STARTUP_TIMEOUT));
    }
}
#endif

static void NVIC_Config(void)
{
#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
    SCB->VTOR = NVIC_VectTab_FLASH | VECTOR_TABLE_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);      //设置中断优先级分组，全是Sub_Priority
//	__set_FAULTMASK(0);
}

static void SetupSysTick(void)
{
  SysTick_Config(systemcoreclock/SYSTICK_PER_SEC);
}

void System_board_init(void)
{
    System_Clock_Config(SET_HSI);
    NVIC_Config();
    SetupSysTick();
}

