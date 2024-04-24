#include "drv_msp.h"
#include "n32l40x.h"

#include "br_config.h"

#ifdef BSP_USING_UART
void ec32_msp_usart_init(void *Instance)
{
    GPIO_InitType GPIO_InitStructure;
    USART_Module *USARTx = (USART_Module *)Instance;

    GPIO_InitStruct(&GPIO_InitStructure);
	EC_APBxClkCmd_AFIO(EC_PERIPHClk_AFIO,ENABLE);                  //时钟复用
	
#ifdef BSP_USING_UART1
    if(USART1 == USARTx)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1,ENABLE);
        RCC_EnableAPB2PeriphClk(EC_USART1_RCC_GPIOx,ENABLE);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Alternate =  EC_USART1_Tx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_USART1_TxPin;
        GPIO_InitPeripheral(EC_USART1_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Alternate =  EC_USART1_Rx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_USART1_RxPin;
        GPIO_InitPeripheral(EC_USART1_GPIO, &GPIO_InitStructure);
    }
#endif
#ifdef BSP_USING_UART2
    if(USART2 == USARTx)
    {
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2,ENABLE);
        RCC_EnableAPB2PeriphClk(EC_USART2_RCC_GPIOx,ENABLE);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Alternate =  EC_USART2_Tx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_USART2_TxPin;
        GPIO_InitPeripheral(EC_USART2_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Alternate =  EC_USART2_Rx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_USART2_RxPin;
        GPIO_InitPeripheral(EC_USART2_GPIO, &GPIO_InitStructure);
    }
#endif
#ifdef BSP_USING_UART3
    if(USART3 == USARTx)
    {
		RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART3,ENABLE);
        RCC_EnableAPB2PeriphClk(EC_USART3_RCC_GPIOx,ENABLE);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Alternate =  EC_USART3_Tx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_USART3_TxPin;
        GPIO_InitPeripheral(EC_USART3_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Alternate =  EC_USART3_Rx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_USART3_RxPin;
        GPIO_InitPeripheral(EC_USART3_GPIO, &GPIO_InitStructure);
    }
#endif
#ifdef BSP_USING_UART4
    if(UART4 == USARTx)
    {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_UART4,ENABLE);
        RCC_EnableAPB2PeriphClk(EC_UART4_RCC_GPIOx,ENABLE);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Alternate =  EC_UART4_Tx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_UART4_TxPin;
        GPIO_InitPeripheral(EC_UART4_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Alternate =  EC_UART4_Rx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_UART4_RxPin;
        GPIO_InitPeripheral(EC_UART4_GPIO, &GPIO_InitStructure);
    }
#endif
#ifdef BSP_USING_UART5
    if(UART5 == USARTx)
    {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_UART5,ENABLE);
        RCC_EnableAPB2PeriphClk(EC_UART5_RCC_GPIOx,ENABLE);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Alternate =  EC_UART5_Tx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_UART5_TxPin;
        GPIO_InitPeripheral(EC_UART5_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Alternate =  EC_UART5_Rx_GPIO_AF;
        GPIO_InitStructure.Pin = EC_UART5_RxPin;
        GPIO_InitPeripheral(EC_UART5_GPIO, &GPIO_InitStructure);
    }
#endif
    /* Add others */
}
#endif /* BSP_USING_SERIAL */

#ifdef BSP_USING_ADC
void ec32_msp_adc_init(void *Instance)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStruct(&GPIO_InitStructure);
    ADC_Module *ADCx = (ADC_Module *)Instance;

    if(ADCx == ADC)
    {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA|RCC_APB2_PERIPH_GPIOB,ENABLE);
		RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA|RCC_AHB_PERIPH_ADC,ENABLE);
		
		/* RCC_ADCHCLK_DIV16*/
		ADC_ConfigClk(ADC_CTRL3_CKMOD_AHB, RCC_ADCHCLK_DIV16);
		RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8);  //selsect HSE as RCC ADC1M CLK Source	   

		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin       = BAT_AD_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
		GPIO_InitPeripheral(BAT_AD_PORT, &GPIO_InitStructure);
		
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin       = BAT_NTC_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
		GPIO_InitPeripheral(BAT_NTC_PORT, &GPIO_InitStructure);
		
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin       = MOTOR_AD_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
		GPIO_InitPeripheral(MOTOR_AD_PORT, &GPIO_InitStructure);
		
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin       = HEAT1_AD_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
		GPIO_InitPeripheral(HEAT1_AD_PORT, &GPIO_InitStructure);

        GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin       = HEAT2_AD_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
		GPIO_InitPeripheral(HEAT2_AD_PORT, &GPIO_InitStructure);

        GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin       = HEAT3_AD_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
		GPIO_InitPeripheral(HEAT3_AD_PORT, &GPIO_InitStructure);
    }
}
#endif /* BSP_USING_ADC */

#ifdef BSP_USING_TIMER
void ec32_msp_timer_init(void* Instance)
{
	TIM_Module *TIMx = (TIM_Module *)Instance;

#ifdef BSP_USING_TIMER1
	if(TIMx == TIM1)
	{
		/* TIM1 clock enable */
		EC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1,ENABLE);
	}
#endif	
	
#ifdef BSP_USING_TIMER2
	if(TIMx == TIM2)
	{
		/* TIM2 clock enable */
		EC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM2,ENABLE);
	}
#endif

#ifdef BSP_USING_TIMER3
    if(TIMx == TIM3)
    {
        /* TIM3 clock enable */
        EC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3,ENABLE);
    }
#endif

#ifdef BSP_USING_TIMER4
    if(TIMx == TIM4)
    {
        /* TIM4 clock enable */
        EC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM4,ENABLE);
    }
#endif

#ifdef BSP_USING_TIMER5
    if(TIMx == TIM5)
    {
        /* TIM5 clock enable */
        EC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM5,ENABLE);
    }
#endif

#ifdef BSP_USING_TIMER6
        if(TIMx == TIM6)
        {
            /* TIM6 clock enable */
            EC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM6,ENABLE);
        }
#endif

#ifdef BSP_USING_TIMER7
        if(TIMx == TIM7)
        {
            /* TIM7 clock enable */
            EC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM7,ENABLE);
        }
#endif
		
#ifdef BSP_USING_TIMER8
	if(TIMx == TIM8)
	{
		/* TIM1 clock enable */
		EC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM8,ENABLE);
	}
#endif
		
#ifdef BSP_USING_TIMER9
	if(TIMx == TIM9)
	{
		/* TIM9 clock enable */
		EC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM9,ENABLE);
	}
#endif	
}
#endif	/* BSP_USING_ADC */

#ifdef BSP_USING_SPI
void ec32_msp_spi_init(void *Instance)
{
    GPIO_InitType GPIO_InitStructure;
    SPI_Module *SPIx = (SPI_Module *)Instance;

    GPIO_Structure_Initialize(&GPIO_InitStructure);
    EC_APBxClkCmd_AFIO(EC_PERIPHClk_AFIO);                    //时钟复用
#ifdef BSP_USING_SPI1
    if(SPI1 == SPIx)
    {
        RCC_APB2_Peripheral_Clock_Enable(EC_SPI1_PERIPH);
        RCC_AHB_Peripheral_Clock_Enable(EC_SPI1_PERIPH_GPIO);

        GPIO_Structure_Initialize(&GPIO_InitStructure);
        /* Configure master pins: NSS, SCK, MISO and MOSI */
        /* Confugure SPI pins as Alternate Function Push Pull */
        GPIO_InitStructure.Pin        = EC_SPI1_NSS_PIN | EC_SPI1_CLK_PIN | EC_SPI1_MISO_PIN | EC_SPI1_MOSI_PIN;
        GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
        GPIO_InitStructure.GPIO_Alternate = EC_SPI1_GPIO_ALTERNATE;
        GPIO_Peripheral_Initialize(EC_SPI1_GPIO, &GPIO_InitStructure);
    }
#endif
#ifdef BSP_USING_SPI2
    if(SPI2 == SPIx)
    {
        RCC_APB2_Peripheral_Clock_Enable(EC_SPI2_PERIPH);
        RCC_AHB_Peripheral_Clock_Enable(EC_SPI2_PERIPH_GPIO);

        GPIO_Structure_Initialize(&GPIO_InitStructure);
        /* Configure master pins: NSS, SCK, MISO and MOSI */
        /* Confugure SPI pins as Alternate Function Push Pull */
        GPIO_InitStructure.Pin        = EC_SPI2_NSS_PIN | EC_SPI2_CLK_PIN | EC_SPI2_MISO_PIN | EC_SPI2_MOSI_PIN;
        GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
        GPIO_InitStructure.GPIO_Alternate = EC_SPI2_GPIO_ALTERNATE;
        GPIO_Peripheral_Initialize(EC_SPI2_GPIO, &GPIO_InitStructure);
    }
#endif
    /* Add others */
}
#endif /* BSP_USING_SPI */

#ifdef BSP_USING_SDIO
void ec32_msp_sdio_init(void *Instance)
{
    GPIO_InitType GPIO_InitCtlStructure;
    SDIO_Module *SDIOx = (SDIO_Module *)Instance;

    GPIO_InitStruct(&GPIO_InitCtlStructure);
    GPIO_InitCtlStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if(SDIO == SDIOx)
    {
        /* if used dma ... */
        RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA2, ENABLE);

        RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_SDIO, ENABLE);
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC | RCC_APB2_PERIPH_GPIOD, ENABLE);
        GPIO_InitCtlStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitCtlStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitPeripheral(GPIOC, &GPIO_InitCtlStructure);

        GPIO_InitCtlStructure.Pin = GPIO_PIN_2;
        GPIO_InitCtlStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitPeripheral(GPIOD, &GPIO_InitCtlStructure);
    }
}
#endif /* BSP_USING_SDIO */

#ifdef BSP_USING_CAN
void ec32_msp_can_init(void *Instance)
{
    GPIO_InitType GPIO_InitCtlStruct;
    CAN_Module *CANx = (CAN_Module *)Instance;

    GPIO_InitStruct(&GPIO_InitCtlStruct);
    GPIO_InitCtlStruct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef BSP_USING_CAN1
    if(CAN1 == CANx)
    {
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_CAN1, ENABLE);
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
        GPIO_InitCtlStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitCtlStruct.Pin = GPIO_PIN_12;
        GPIO_InitPeripheral(GPIOA, &GPIO_InitCtlStruct);

        GPIO_InitCtlStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitCtlStruct.Pin = GPIO_PIN_11;
        GPIO_InitPeripheral(GPIOA, &GPIO_InitCtlStruct);
    }
#endif
#ifdef BSP_USING_CAN2
    if(CAN2 == CANx)
    {
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_CAN2, ENABLE);
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
        //        GPIO_PinsRemapConfig(AFIO_MAP6_CAN2_0001, ENABLE);
        GPIO_InitCtlStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitCtlStruct.Pin = GPIO_PIN_6;
        GPIO_InitPeripheral(GPIOB, &GPIO_InitCtlStruct);

        GPIO_InitCtlStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitCtlStruct.Pin = GPIO_PIN_5;
        GPIO_InitPeripheral(GPIOB, &GPIO_InitCtlStruct);
    }
#endif
}
#endif /* BSP_USING_CAN */

#ifdef BSP_USING_EXTI

void ec32_msp_exti_init(void)
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /* Enable the GPIO Clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
    
    /*Configure the GPIO pin as input floating*/
	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.Pin        = HALL_INPUT_PIN;
	GPIO_InitStructure.GPIO_Pull  = GPIO_Pull_Up;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
	GPIO_InitPeripheral(HALL_INPUT_PORT, &GPIO_InitStructure);
	
    GPIO_ConfigEXTILine(HALL_INPUT_PORT_SOURCE, HALL_INPUT_PIN_SOURCE);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = HALL_INPUT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = HALL_INPUT_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x07;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

#endif

