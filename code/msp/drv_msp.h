#ifndef __DRV_MSP_H
#define __DRV_MSP_H

#define EC_APBxClkCmd_AFIO      RCC_EnableAPB2PeriphClk
#define EC_PERIPHClk_AFIO       RCC_APB2_PERIPH_AFIO

#define EC_EnableAPB2PeriphClk	RCC_EnableAPB2PeriphClk
#define EC_EnableAPB1PeriphClk	RCC_EnableAPB1PeriphClk
#define EC_EnableAHBPeriphClk	RCC_EnableAHBPeriphClk

//USART:
//MCU_TX_RX USART1
#define EC_USART1_GPIO              GPIOA
#define EC_USART1_RCC_GPIOx 		RCC_APB2_PERIPH_GPIOA
#define EC_USART1_TxPin             GPIO_PIN_9
#define EC_USART1_RxPin             GPIO_PIN_10
#define EC_USART1_Tx_GPIO_AF        GPIO_AF4_USART1
#define EC_USART1_Rx_GPIO_AF        GPIO_AF4_USART1

//MCU_TX_RX USART2
#define EC_USART2_GPIO              GPIOD
#define EC_USART2_RCC_GPIOx 		RCC_APB2_PERIPH_GPIOD
#define EC_USART2_TxPin             GPIO_PIN_14
#define EC_USART2_RxPin             GPIO_PIN_15
#define EC_USART2_Tx_GPIO_AF        GPIO_AF4_USART2
#define EC_USART2_Rx_GPIO_AF        GPIO_AF4_USART2

/* User's usart define start */
//MCU_TX_RX USART3
#define EC_USART3_GPIO              GPIOB
#define EC_USART3_RCC_GPIOx 		RCC_APB2_PERIPH_GPIOB
#define EC_USART3_TxPin             GPIO_PIN_10
#define EC_USART3_RxPin             GPIO_PIN_11
#define EC_USART3_Tx_GPIO_AF        GPIO_AF0_USART3
#define EC_USART3_Rx_GPIO_AF        GPIO_AF5_USART3

//MCU_TX_RX UART4
#define EC_UART4_GPIO              	GPIOB
#define EC_UART4_RCC_GPIOx 		   	RCC_APB2_PERIPH_GPIOB
#define EC_UART4_TxPin             	GPIO_PIN_0
#define EC_UART4_RxPin             	GPIO_PIN_1
#define EC_UART4_Tx_GPIO_AF        	GPIO_AF6_UART4
#define EC_UART4_Rx_GPIO_AF        	GPIO_AF6_UART4

//MCU_TX_RX UART5
#define EC_UART5_GPIO              	GPIOB
#define EC_UART5_RCC_GPIOx 		   	RCC_APB2_PERIPH_GPIOB
#define EC_UART5_TxPin             	GPIO_PIN_4
#define EC_UART5_RxPin             	GPIO_PIN_5
#define EC_UART5_Tx_GPIO_AF        	GPIO_AF6_UART5
#define EC_UART5_Rx_GPIO_AF        	GPIO_AF7_UART5


/* User's usart define end */

//ADC:
//MCU ADC；
#define HEAT1_AD_PORT				GPIOA		
#define	HEAT1_AD_PIN                GPIO_PIN_3

#define HEAT2_AD_PORT				GPIOB		
#define	HEAT2_AD_PIN                GPIO_PIN_0

#define HEAT3_AD_PORT				GPIOB		
#define	HEAT3_AD_PIN                GPIO_PIN_1

#define	BAT_AD_PORT					GPIOA				//电池AD采集
#define	BAT_AD_PIN					GPIO_PIN_4

#define	BAT_NTC_PORT				GPIOA				//电池NTC采集
#define	BAT_NTC_PIN					GPIO_PIN_5

#define MOTOR_AD_PORT				GPIOA				//电机电流检测I=AD/0.22;
#define	MOTOR_AD_PIN				GPIO_PIN_6



//SPI:
//MCU SPI1
//#define EC_SPI1                 SPI1
//#define EC_SPI1_PERIPH          RCC_APB2_PERIPH_SPI1
//#define EC_SPI1_PERIPH_GPIO     RCC_AHB_PERIPH_GPIOA
//#define EC_SPI1_GPIO            GPIOA
//#define EC_SPI1_GPIO_ALTERNATE  GPIO_AF1_SPI1
//#define EC_SPI1_NSS_PIN         GPIO_PIN_4
//#define EC_SPI1_CLK_PIN         GPIO_PIN_5
//#define EC_SPI1_MISO_PIN        GPIO_PIN_6
//#define EC_SPI1_MOSI_PIN        GPIO_PIN_7

//TEST SPI2
//#define EC_SPI2                 SPI2
//#define EC_SPI2_PERIPH          RCC_APB2_PERIPH_SPI2
//#define EC_SPI2_PERIPH_GPIO     RCC_AHB_PERIPH_GPIOB
//#define EC_SPI2_GPIO            GPIOB
//#define EC_SPI2_GPIO_ALTERNATE  GPIO_AF1_SPI2
//#define EC_SPI2_NSS_PIN         GPIO_PIN_12
//#define EC_SPI2_CLK_PIN         GPIO_PIN_13
//#define EC_SPI2_MISO_PIN        GPIO_PIN_14
//#define EC_SPI2_MOSI_PIN        GPIO_PIN_15

/* User's spi define start */

/* User's spi define end */

//#define HALL_INPUT_PORT        GPIOB
//#define HALL_INPUT_PIN         GPIO_PIN_2
//#define HALL_INPUT_EXTI_LINE   EXTI_LINE2
//#define HALL_INPUT_PORT_SOURCE GPIOB_PORT_SOURCE
//#define HALL_INPUT_PIN_SOURCE  GPIO_PIN_SOURCE2
//#define HALL_INPUT_IRQn        EXTI2_IRQn

void ec32_msp_usart_init(void *Instance);
void ec32_msp_timer_init(void* Instance);
void ec32_msp_adc_init(void *Instance);
//void ec32_msp_exti_init(void);


//void ec32_msp_spi_init(void *Instance);
//void ec32_msp_sdio_init(void *Instance);
//void ec32_msp_can_init(void *Instance);

#endif
