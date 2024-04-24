#include "n32l40x_it.h"
#include "soft_timer.h"

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/


static uint32_t sys_ticks 	= 0;
static bool Interrupt_1ms 	= false;
static bool Interrupt_10ms 	= false;
static bool Interrupt_50ms 	= false;
static bool Interrupt_100ms = false;
static bool Interrupt_500ms = false;
static bool Interrupt_1s 	= false;

/**
 *\*\name   SysTick_Handler.
 *\*\fun    This function handles SysTick Handler.
 *\*\param  none.
 *\*\return none.
 */
void SysTick_Handler(void)		//depend systick
{
	sys_ticks += 1;
	Interrupt_1ms = true;
	if(!(sys_ticks%10))
		Interrupt_10ms = true;
	
	if(!(sys_ticks%50))
		Interrupt_50ms = true;
	
	if(!(sys_ticks%100))
		Interrupt_100ms = true;
	
	if(!(sys_ticks%500))
		Interrupt_500ms = true;
	
	if(!(sys_ticks%1000))
		Interrupt_1s = true;
	
	stimer_ticks();
}

bool systick_flag_get(ESYSITNUM iflag)
{
	if((iflag == SYS_INT_1MS)&&(Interrupt_1ms))
	{
		Interrupt_1ms = false;
		return true;
	}
	
	if((iflag == SYS_INT_10MS)&&(Interrupt_10ms))
	{
		Interrupt_10ms = false;
		return true;
	}
	
	if((iflag == SYS_INT_50MS)&&(Interrupt_50ms))
	{
		Interrupt_50ms = false;
		return true;
	}
	
	if((iflag == SYS_INT_100MS)&&(Interrupt_100ms))
	{
		Interrupt_100ms = false;
		return true;
	}
	
	if((iflag == SYS_INT_500MS)&&(Interrupt_500ms))
	{
		Interrupt_500ms = false;
		return true;
	}
	
	if((iflag == SYS_INT_1S)&&(Interrupt_1s))
	{
		Interrupt_1s = false;
		return true;
	}
	return false;
}

uint32_t sys_tick_get(void)		//depend systick
{
	return sys_ticks;
}

uint32_t sys_hour_get(void)		//depend systick
{
	return (sys_ticks/3600000);
}

uint32_t sys_minute_get(void)		//depend systick
{
	return (sys_ticks/60000%60);
}

uint32_t sys_second_get(void)		//depend systick
{
	return (sys_ticks/1000%60);
}

uint32_t sys_msecond_get(void)		//depend systick
{
	return (sys_ticks%1000);
}





/************************************************
 *\*\name   NMI_Handler.
 *\*\fun    This function handles NMI exception.
 *\*\param  none.
 *\*\return none.
 */
void NMI_Handler(void)
{
}


/**
 *\*\name   HardFault_Handler.
 *\*\fun    This function handles Hard Fault exception.
 *\*\param  none.
 *\*\return none.
 */
// void HardFault_Handler(void)
// {
//     /* Go to infinite loop when Hard Fault exception occurs */
//     while (1)
//     {
//     }
// }


/**
 *\*\name   MemManage_Handler.
 *\*\fun    This function handles Memory Manage exception.
 *\*\param  none.
 *\*\return none.
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}


/**
 *\*\name   BusFault_Handler.
 *\*\fun    This function handles Bus Fault exception.
 *\*\param  none.
 *\*\return none.
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}


/**
 *\*\name   UsageFault_Handler.
 *\*\fun    This function handles Usage Fault exception.
 *\*\param  none.
 *\*\return none.
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}


/**
 *\*\name   SVC_Handler.
 *\*\fun    This function handles SVCall exception.
 *\*\param  none.
 *\*\return none.
 */
void SVC_Handler(void)
{
}


/**
 *\*\name   DebugMon_Handler.
 *\*\fun    This function handles Debug Monitor exception.
 *\*\param  none.
 *\*\return none.
 */
void DebugMon_Handler(void)
{
}





