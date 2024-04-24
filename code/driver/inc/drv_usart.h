#ifndef __DRV_USART_H
#define __DRV_USART_H

#include "n32l40x.h"

#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
#define BAUD_RATE_2000000               2000000
#define BAUD_RATE_3000000               3000000

#define DATA_BITS_5                     5
#define DATA_BITS_6                     6
#define DATA_BITS_7                     7
#define DATA_BITS_8                     8
#define DATA_BITS_9                     9

#define STOP_BITS_1                     0
#define STOP_BITS_2                     1
#define STOP_BITS_3                     2
#define STOP_BITS_4                     3

#define PARITY_NONE                     0
#define PARITY_ODD                      1
#define PARITY_EVEN                     2

#define BIT_ORDER_LSB                   0
#define BIT_ORDER_MSB                   1

#define EC_SERIAL_FLOWCONTROL_CTSRTS    1
#define EC_SERIAL_FLOWCONTROL_NONE      0

#define EC_SERIAL_NVIC_DEFAULT     		7

#define NRZ_NORMAL                      0       /* Non Return to Zero : normal mode */
#define NRZ_INVERTED                    1       /* Non Return to Zero : inverted mode */

#ifndef EC_SERIAL_RB_BUFSZ
	#define EC_SERIAL_RB_BUFSZ          128
#endif

#ifndef EC_SERIAL_TX_BUFSZ
	#define EC_SERIAL_TX_BUFSZ          1024
#endif

#define EC_SERIAL_EVENT_RX_IND          0x01    /* Rx indication */
#define EC_SERIAL_EVENT_TX_DONE         0x02    /* Tx complete   */
#define EC_SERIAL_EVENT_RX_DMADONE      0x03    /* Rx DMA transfer done */
#define EC_SERIAL_EVENT_TX_DMADONE      0x04    /* Tx DMA transfer done */
#define EC_SERIAL_EVENT_RX_TIMEOUT      0x05    /* Rx timeout    */
		
#define EC_SERIAL_RX_INT                0x01
#define EC_SERIAL_TX_INT                0x02
#define EC_SERIAL_IDLEF_INT             0x04
		
#define EC_SERIAL_DMA_RX                0x01
#define EC_SERIAL_DMA_TX                0x02

#define EC_SERIAL_ERR_OVERRUN           0x01
#define EC_SERIAL_ERR_FRAMING           0x02
#define EC_SERIAL_ERR_PARITY            0x04
		
#define EC_DEVICE_CTRL_SET_INT          0x10            /**< set interrupt */
#define EC_DEVICE_CTRL_CLR_INT          0x11            /**< clear interrupt */
#define EC_DEVICE_CTRL_GET_INT          0x12            /**< get interrupt status */
#define EC_DEVICE_CTRL_CONFIG           0x13            /**< configure device */
#define EC_DEVICE_CTRL_NVIC          	0x14			/**< configure nvic */

#define	ESERIAL_MODE_NORMAL				0x01			//不开启中断
#define	ESERIAL_MODE_DMA_RX				0x02			//DMA IDLE 中断接收
#define	ESERIAL_MODE_IT					0x04			//普通中断接收
#define	ESERIAL_MODE_DMA_TX				0x08			//DMA发送

/* Default config for serial_configure structure */
#define EC_SERIAL_CONFIG_DEFAULT           \
{                                          \
    BAUD_RATE_115200, /* 115200 bits/s */  \
    DATA_BITS_8,      /* 8 databits */     \
    STOP_BITS_1,      /* 1 stopbit */      \
    PARITY_NONE,      /* No parity  */     \
    BIT_ORDER_LSB,    /* LSB first sent */ \
    NRZ_NORMAL,       /* Normal mode */    \
    EC_SERIAL_RB_BUFSZ, /* Buffer size */  \
	EC_SERIAL_TX_BUFSZ, /* Buffer size */  \
    EC_SERIAL_FLOWCONTROL_NONE, /* Off flowcontrol */ \
	EC_SERIAL_NVIC_DEFAULT,		/* nvic defalut */ \
    0                                      \
}

typedef enum{
	ESERIAL_1 = 1,
	ESERIAL_2,
	ESERIAL_3,
	ESERIAL_4,
	ESERIAL_5
}ESERIAL_DEV; 

typedef struct{
    uint16_t head;
    uint16_t tail;
	uint16_t mirror:1;
    uint16_t lenth:15;
	uint8_t *data;
} structUsartData;

struct serial_configure
{
    uint32_t baud_rate;

    uint32_t data_bits               :4;
    uint32_t stop_bits               :2;
    uint32_t parity                  :2;
    uint32_t bit_order               :1;
    uint32_t invert                  :1;
    uint32_t bufsz                   :16;
	uint32_t txbufsz                 :16;
    uint32_t flowcontrol             :1;
	uint32_t nvic					 :4;
    uint32_t reserved                :5;
};

struct ec_serial_device
{
    USART_Module *uart_device;
    IRQn_Type irq;
    struct ec32_uart_dma
    {
        /* dma channel */
        DMA_ChannelType *rx_ch;
        DMA_Module *rx_dma_type;
        uint32_t rx_gl_flag;			//dma global flag
        uint8_t rx_irq_ch;				//dma irq channel
        uint32_t setting_recv_len;		//setting receive len
        uint32_t last_recv_index;		//last receive index
		DMA_ChannelType *tx_ch;
        DMA_Module *tx_dma_type;
        uint32_t tx_gl_flag;			//dma global flag
        uint8_t tx_irq_ch;				//dma irq channel
        uint32_t setting_tx_len;		//setting transmit len
        uint32_t last_tx_index;			//last receive index
    } dma;
    struct serial_configure *config;
    structUsartData *Rxbuffer;
	structUsartData *Txbuffer;
	int (*rx_indicate)(ESERIAL_DEV serial_dev, uint16_t size);
};

int drv_usart_init(ESERIAL_DEV serial_dev,uint8_t mode,struct serial_configure *config);
int drv_uart_configure(ESERIAL_DEV serial_dev,struct serial_configure *config);
int	drv_uart_control(ESERIAL_DEV serial_dev,int cmd, void *arg);
uint16_t drv_fifo_data_get(ESERIAL_DEV serial_dev,uint8_t *buff,uint16_t lenth);
int drv_serial_read(ESERIAL_DEV serial_dev,void *buffer,uint32_t size);
int drv_serial_write(ESERIAL_DEV serial_dev,const void *buffer,uint32_t size);
int drv_serial_dma_write(ESERIAL_DEV serial_dev,const void *buffer,uint32_t size);
int drv_device_set_rx_indicate(ESERIAL_DEV serial_dev,int(*rx_ind)(ESERIAL_DEV serial_dev, uint16_t size));

#endif
