#include <stdio.h>
#include <string.h>
#include "drv_usart.h"
#include "drv_msp.h"
#include "log.h"
#include "br_config.h"

static void uart_isr(ESERIAL_DEV serial_dev);
static void dma_rx_done_isr(ESERIAL_DEV serial_dev);
static void dma_tx_done_isr(ESERIAL_DEV serial_dev);
static void DMA_RX_Configuration(struct ec_serial_device *serial);
static void DMA_TX_Configuration(struct ec_serial_device *serial);
static void drv_serial_isr(ESERIAL_DEV serial_dev, int event);

#if defined(BSP_USING_UART1)
	static struct serial_configure default_cfg1 = EC_SERIAL_CONFIG_DEFAULT;
#endif
#if defined(BSP_USING_UART2)
	static struct serial_configure default_cfg2 = EC_SERIAL_CONFIG_DEFAULT;
#endif
#if defined(BSP_USING_UART3)
	static struct serial_configure default_cfg3 = EC_SERIAL_CONFIG_DEFAULT;
#endif
#if defined(BSP_USING_UART4)
	static struct serial_configure default_cfg4 = EC_SERIAL_CONFIG_DEFAULT;
#endif
#if defined(BSP_USING_UART5)
	static struct serial_configure default_cfg5 = EC_SERIAL_CONFIG_DEFAULT;
#endif

/* UART1 device driver structure */
#if defined(BSP_USING_UART1)
	static uint8_t uart1_rx_data[EC_SERIAL_RB_BUFSZ];
	static uint8_t uart1_tx_data[EC_SERIAL_TX_BUFSZ];
	static structUsartData uart1_rxbuff = {.data = uart1_rx_data};
	static structUsartData uart1_txbuff = {.data = uart1_tx_data};
#endif
	
#if defined(BSP_USING_UART2)
	static uint8_t uart2_rx_data[EC_SERIAL_RB_BUFSZ];
	static structUsartData uart2_rxbuff = {.data = uart2_rx_data};
#endif
	
#if defined(BSP_USING_UART3)
	static uint8_t uart3_rx_data[EC_SERIAL_RB_BUFSZ];
	static structUsartData uart3_rxbuff = {.data = uart3_rx_data};
#endif
	
#if defined(BSP_USING_UART4)
	static uint8_t uart4_rx_data[EC_SERIAL_RB_BUFSZ];
	static structUsartData uart4_rxbuff = {.data = uart4_rx_data};
#endif
	
#if defined(BSP_USING_UART5)
	static uint8_t uart5_rx_data[EC_SERIAL_RB_BUFSZ];
	static structUsartData uart5_rxbuff = {.data = uart5_rx_data};
#endif	
	
enum
{
#ifdef BSP_USING_UART1
    UART1_INDEX,
#endif

#ifdef BSP_USING_UART2
    UART2_INDEX,
#endif

#ifdef BSP_USING_UART3
    UART3_INDEX,
#endif
	
#ifdef BSP_USING_UART4
    UART4_INDEX,
#endif

#ifdef BSP_USING_UART5
    UART5_INDEX,
#endif	
};	

static struct ec_serial_device serialx[] =
{
	#if defined(BSP_USING_UART1)
	{
		USART1,
		USART1_IRQn,
		{
			DMA_CH1,
			DMA,
			DMA_FLAG_GL1,
			DMA_Channel1_IRQn,
			0,
			0,
			DMA_CH7,
			DMA,
			DMA_FLAG_TC7,
			DMA_Channel7_IRQn,
			0,
			0,
		},
		&default_cfg1,
		&uart1_rxbuff,
		&uart1_txbuff,
		NULL
	},
	#endif
	#if defined(BSP_USING_UART2)
	{	
		USART2,
		USART2_IRQn,
		{
			DMA_CH2,
			DMA,
			DMA_FLAG_GL2,
			DMA_Channel2_IRQn,
			0,
			0
		},
		&default_cfg2,
		&uart2_rxbuff,
		NULL,
		NULL,
	},
	#endif
	#if defined(BSP_USING_UART3)
	{
		USART3,
		USART3_IRQn,
		{
			DMA_CH3,
			DMA,
			DMA_FLAG_GL3,
			DMA_Channel3_IRQn,
			0,
			0
		},
		&default_cfg3,
		&uart3_rxbuff,
		NULL
	},
	#endif
	#if defined(BSP_USING_UART4)
	{	
		UART4,
		UART4_IRQn,
		{
			DMA_CH4,
			DMA,
			DMA_FLAG_GL4,
			DMA_Channel4_IRQn,
			0,
			0,
		},
		&default_cfg4,
		&uart4_buff,
		NULL
	},
	#endif
	#if defined(BSP_USING_UART5)
	{
		UART5,
		UART5_IRQn,
		{
			DMA_CH5,
			DMA,
			DMA_FLAG_GL5,
			DMA_Channel5_IRQn,
			0,
			0
		},
		&default_cfg5,
		&uart5_buff,
		NULL
	}
	#endif
};

#if defined(BSP_USING_UART1)
void USART1_IRQHandler(void)
{
    /* enter interrupt */
    uart_isr(ESERIAL_1);
}

void DMA_Channel1_IRQHandler(void)
{
    /* enter interrupt */
    dma_rx_done_isr(ESERIAL_1);
}

void DMA_Channel7_IRQHandler(void)
{
    /* enter interrupt */
    dma_tx_done_isr(ESERIAL_1);
}
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
void USART2_IRQHandler(void)
{
    /* enter interrupt */
    uart_isr(ESERIAL_2);
}

void DMA_Channel2_IRQHandler(void)
{
    /* enter interrupt */
    dma_rx_done_isr(ESERIAL_2);
}
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
void USART3_IRQHandler(void)
{
    /* enter interrupt */
    uart_isr(ESERIAL_3);
}

void DMA_Channel3_IRQHandler(void)
{
    /* enter interrupt */
    dma_rx_done_isr(ESERIAL_3);
}
#endif /* BSP_USING_UART3 */

#if defined(BSP_USING_UART4)
void UART4_IRQHandler(void)
{
    /* enter interrupt */
    uart_isr(ESERIAL_4);
}

void DMA_Channel4_IRQHandler(void)
{
    /* enter interrupt */
    dma_rx_done_isr(ESERIAL_4);
}
#endif /* BSP_USING_UART4 */

#if defined(BSP_USING_UART5)
void UART5_IRQHandler(void)
{
    /* enter interrupt */
    uart_isr(ESERIAL_5);
}

void DMA_Channel5_IRQHandler(void)
{
    /* enter interrupt */
    dma_rx_done_isr(ESERIAL_5);
}
#endif /* BSP_USING_UART5 */

static struct ec_serial_device *serial_get_handle(ESERIAL_DEV serial_dev)
{
#if defined(BSP_USING_UART1)
	if(serial_dev == ESERIAL_1)	return &serialx[UART1_INDEX];
#endif
	
#if defined(BSP_USING_UART2)
	if(serial_dev == ESERIAL_2)	return &serialx[UART2_INDEX];
#endif
	
#if defined(BSP_USING_UART3)
	if(serial_dev == ESERIAL_3)	return &serialx[UART3_INDEX];
#endif

#if defined(BSP_USING_UART4)
	if(serial_dev == ESERIAL_4)	return &serialx[UART4_INDEX];
#endif
	
#if defined(BSP_USING_UART5)
	if(serial_dev == ESERIAL_5)	return &serialx[UART5_INDEX];
#endif	
	return NULL;
}

static int ec32_uart_configure(struct ec_serial_device *serial, struct serial_configure *cfg)
{
    USART_InitType USART_InitStructure;
	USART_StructInit(&USART_InitStructure);
    ec32_msp_usart_init(serial->uart_device);

    USART_InitStructure.BaudRate = cfg->baud_rate;

    if (cfg->data_bits == DATA_BITS_8)
    {
        USART_InitStructure.WordLength = USART_WL_8B;
    }
    else if (cfg->data_bits == DATA_BITS_9)
    {
        USART_InitStructure.WordLength = USART_WL_9B;
    }

    if (cfg->stop_bits == STOP_BITS_1)
    {
        USART_InitStructure.StopBits = USART_STPB_1;
    }
    else if (cfg->stop_bits == STOP_BITS_2)
    {
        USART_InitStructure.StopBits = USART_STPB_2;
    }

    if (cfg->parity == PARITY_NONE)
    {
        USART_InitStructure.Parity = USART_PE_NO;
    }
    else if (cfg->parity == PARITY_ODD)
    {
        USART_InitStructure.Parity = USART_PE_ODD;
    }
    else if (cfg->parity == PARITY_EVEN)
    {
        USART_InitStructure.Parity = USART_PE_EVEN;
    }

    if (cfg->flowcontrol == EC_SERIAL_FLOWCONTROL_NONE)
    {
        USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    }
    else if (cfg->flowcontrol == EC_SERIAL_FLOWCONTROL_CTSRTS)
    {
        USART_InitStructure.HardwareFlowControl = USART_HFCTRL_RTS_CTS;
    }
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
    USART_Init(serial->uart_device, &USART_InitStructure);
	
    /* Enable USART */
    USART_Enable(serial->uart_device,ENABLE);
    USART_ClrFlag(serial->uart_device, USART_FLAG_TXDE | USART_FLAG_TXC);

    return 0;
}

/**
	usart nvic configure
 */
static void ec32_usart_nvic_configure(struct ec_serial_device *serial,uint8_t level)
{
	NVIC_InitType NVIC_InitStructure;
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = serial->irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = level;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief Serial config interrupt and dma control mode,
  * @param serial_dev:  ESERIAL_1(usart1),
						ESERIAL_2(usart2),
						ESERIAL_3(usart3)
						ESERIAL_4(uart4),
                        ESERIAL_5(uart5)
* @param cmd control command:   EC_DEVICE_CTRL_CLR_INT
                                EC_DEVICE_CTRL_SET_INT
                                EC_DEVICE_CTRL_CONFIG
  * @param arg Transmit data buffer length.
  * @return Return status : 0.
  */
static int ec32_uart_control(struct ec_serial_device *serial, int cmd, void *arg)
{
    uint32_t ctrl_arg = (uint32_t)(arg);

    switch (cmd)
    {
    /* disable interrupt */
    case EC_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        NVIC_DisableIRQ(serial->irq);
        /* disable interrupt */
        if(ctrl_arg & EC_SERIAL_RX_INT)
            USART_ConfigInt(serial->uart_device, USART_INT_RXDNE, DISABLE);
        if(ctrl_arg & EC_SERIAL_TX_INT)
            USART_ConfigInt(serial->uart_device, USART_INT_TXC, DISABLE);
        if(ctrl_arg & EC_SERIAL_IDLEF_INT)
            USART_ConfigInt(serial->uart_device, USART_INT_IDLEF, DISABLE);
        break;
    /* enable interrupt */
    case EC_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
        NVIC_EnableIRQ(serial->irq);
        /* enable interrupt */
        if(ctrl_arg & EC_SERIAL_RX_INT)
            USART_ConfigInt(serial->uart_device, USART_INT_RXDNE ,ENABLE);
        if(ctrl_arg & EC_SERIAL_TX_INT)
            USART_ConfigInt(serial->uart_device, USART_INT_TXC ,ENABLE);
        if(ctrl_arg & EC_SERIAL_IDLEF_INT)
            USART_ConfigInt(serial->uart_device, USART_INT_IDLEF ,ENABLE);
        break;
    /* USART config */
    case EC_DEVICE_CTRL_CONFIG :
        if (ctrl_arg & EC_SERIAL_DMA_RX)
            DMA_RX_Configuration(serial);
		if (ctrl_arg & EC_SERIAL_DMA_TX)
            DMA_TX_Configuration(serial);
		break;
	case EC_DEVICE_CTRL_NVIC :	
		if(ctrl_arg > 15)
			ctrl_arg = 15;
		ec32_usart_nvic_configure(serial,ctrl_arg);
        break;
    }
    return 0;
}

static int ec32_uart_putc(struct ec_serial_device *serial, char c)
{
    serial->uart_device->DAT = c;
    while (!(serial->uart_device->STS & USART_FLAG_TXDE));
    return 1;
}

static int ec32_uart_getc(struct ec_serial_device *serial)
{
    int ch;
    ch = -1;
    if (serial->uart_device->STS & USART_FLAG_RXDNE)
    {
        ch = serial->uart_device->DAT & 0xff;
    }
    return ch;
}

/**
 * Serial port receive idle process. This need add to uart idle ISR.
 *
 * @param serial serial device
 */
static void dma_uart_rx_idle_isr(ESERIAL_DEV serial_dev)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    uint32_t recv_total_index, recv_len;

    recv_total_index = serial->dma.setting_recv_len - DMA_GetCurrDataCounter(serial->dma.rx_ch);
    recv_len = recv_total_index - serial->dma.last_recv_index;
    serial->dma.last_recv_index = recv_total_index;
	
    if (recv_len)
        drv_serial_isr(serial_dev, EC_SERIAL_EVENT_RX_DMADONE | (recv_len << 8));

    /* read a data for clear receive idle interrupt flag */
    USART_ReceiveData(serial->uart_device);
    DMA_ClearFlag( serial->dma.rx_gl_flag,serial->dma.rx_dma_type);
}

/**
 * DMA receive done process. This need add to DMA receive done ISR.
 *
 * @param serial serial device
 */
static void dma_rx_done_isr(ESERIAL_DEV serial_dev)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    uint32_t recv_len;

    recv_len = serial->dma.setting_recv_len - serial->dma.last_recv_index;
    /* reset last recv index */
    serial->dma.last_recv_index = 0;

    if (recv_len)
        drv_serial_isr(serial_dev, EC_SERIAL_EVENT_RX_DMADONE | (recv_len << 8));

    DMA_ClearFlag(serial->dma.rx_gl_flag,serial->dma.rx_dma_type);
}

static void dma_tx_done_isr(ESERIAL_DEV serial_dev)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    uint32_t trasmit_len;

    trasmit_len = serial->dma.setting_tx_len - serial->dma.last_tx_index;
    /* reset last recv index */
    serial->dma.last_recv_index = 0;

    if (trasmit_len)
        drv_serial_isr(serial_dev, EC_SERIAL_EVENT_TX_DMADONE | (trasmit_len << 8));

    DMA_ClearFlag(serial->dma.tx_gl_flag,serial->dma.tx_dma_type);
}

/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(ESERIAL_DEV serial_dev)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    if (USART_GetIntStatus(serial->uart_device, USART_INT_RXDNE) != RESET)
    {
        if (USART_GetIntStatus(serial->uart_device, USART_FLAG_PEF) == RESET)
        {
            drv_serial_isr(serial_dev, EC_SERIAL_EVENT_RX_IND);
        }
        /* clear interrupt */
        USART_ClrIntPendingBit(serial->uart_device, USART_INT_RXDNE);
    }
    if (USART_GetIntStatus(serial->uart_device, USART_INT_IDLEF) != RESET)
    {	
        dma_uart_rx_idle_isr(serial_dev);
    }
    if (USART_GetIntStatus(serial->uart_device, USART_INT_TXC) != RESET)
    {
        /* clear interrupt */
        drv_serial_isr(serial_dev, EC_SERIAL_EVENT_TX_DONE);
        USART_ConfigInt(serial->uart_device, USART_INT_TXC, ENABLE);
        USART_ClrIntPendingBit(serial->uart_device, USART_INT_TXC);
    }
    if (USART_GetFlagStatus(serial->uart_device, USART_FLAG_OREF) == SET)
    {
        ec32_uart_getc(serial);
    }
}

static void DMA_RX_Configuration(struct ec_serial_device *serial)
{
    DMA_InitType DMA_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    serial->dma.setting_recv_len = serial->config->bufsz;
	
    /* enable transmit idle interrupt */
    USART_ConfigInt(serial->uart_device, USART_INT_IDLEF ,ENABLE);

    /* DMA clock enable */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA,ENABLE);
	
	uint32_t dma_remap_rx = 0;							//add
	if(serial->uart_device == USART1)
		dma_remap_rx = DMA_REMAP_USART1_RX;
	if(serial->uart_device == USART2)
		dma_remap_rx = DMA_REMAP_USART2_RX;
	if(serial->uart_device == USART3)
		dma_remap_rx = DMA_REMAP_USART3_RX;
	if(serial->uart_device == UART4)
		dma_remap_rx = DMA_REMAP_UART4_RX;
	if(serial->uart_device == UART5)
		dma_remap_rx = DMA_REMAP_UART5_RX;
	
	DMA_RequestRemap(dma_remap_rx,serial->dma.rx_dma_type,serial->dma.rx_ch,ENABLE);

    /* rx dma config */
    DMA_DeInit(serial->dma.rx_ch);
    DMA_InitStructure.PeriphAddr = (uint32_t) &(serial->uart_device->DAT);
    DMA_InitStructure.MemAddr = (uint32_t)(serial->Rxbuffer->data);
    DMA_InitStructure.Direction = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize = serial->config->bufsz;
    DMA_InitStructure.PeriphInc = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
    DMA_InitStructure.MemDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.CircularMode = DMA_MODE_CIRCULAR;
    DMA_InitStructure.Priority = DMA_PRIORITY_HIGH;
    DMA_InitStructure.Mem2Mem = DMA_M2M_DISABLE;
    DMA_Init(serial->dma.rx_ch, &DMA_InitStructure);
    DMA_ClearFlag(serial->dma.rx_gl_flag,serial->dma.rx_dma_type);
    DMA_ConfigInt(serial->dma.rx_ch, DMA_INT_TXC,ENABLE);
    USART_EnableDMA(serial->uart_device, USART_DMAREQ_RX,ENABLE);
    DMA_EnableChannel(serial->dma.rx_ch,ENABLE);
	
    /* rx dma interrupt config */
    NVIC_InitStructure.NVIC_IRQChannel = serial->dma.rx_irq_ch;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void DMA_TX_Configuration(struct ec_serial_device *serial)
{
    uint32_t dma_remap_tx = 0;
	serial->dma.setting_tx_len = serial->config->txbufsz;
	
    if(serial->uart_device == USART1)
        dma_remap_tx = DMA_REMAP_USART1_TX;
    if(serial->uart_device == USART2)
        dma_remap_tx = DMA_REMAP_USART2_TX;
    if(serial->uart_device == USART3)
        dma_remap_tx = DMA_REMAP_USART3_TX;
    if(serial->uart_device == UART4)
        dma_remap_tx = DMA_REMAP_UART4_TX;
    if(serial->uart_device == UART5)
        dma_remap_tx = DMA_REMAP_UART5_TX;

    DMA_RequestRemap(dma_remap_tx,serial->dma.tx_dma_type,serial->dma.tx_ch,ENABLE);

    DMA_DeInit(serial->dma.tx_ch);
	DMA_ClearFlag(serial->dma.tx_gl_flag, serial->dma.tx_dma_type);
    USART_EnableDMA(serial->uart_device, USART_DMAREQ_TX, ENABLE);
}

static void ec32_uart_dma_tx_config(struct ec_serial_device *serial, uint8_t *buffer, uint16_t length)
{
	if(serial->Txbuffer->tail + length > serial->dma.setting_tx_len)
	{
//		if(DMA_GetCurrDataCounter(serial->dma.tx_ch) + length > serial->dma.setting_tx_len)
//		{
			while(DMA_GetFlagStatus(serial->dma.tx_gl_flag, serial->dma.tx_dma_type) == RESET){}
			DMA_ClearFlag(serial->dma.tx_gl_flag, serial->dma.tx_dma_type);
//		}
	}
	
    DMA_EnableChannel(serial->dma.tx_ch, DISABLE);
    DMA_InitType DMA_InitStructure;
	
	uint16_t dma_get_counts = DMA_GetCurrDataCounter(serial->dma.tx_ch);
    /* if no data waiting send*/
    if(dma_get_counts == 0)
    {
		serial->Txbuffer->lenth = length > serial->dma.setting_tx_len ? serial->dma.setting_tx_len : length;
		memcpy(serial->Txbuffer->data, buffer, serial->Txbuffer->lenth);
        DMA_InitStructure.MemAddr = (uint32_t)serial->Txbuffer->data;
        DMA_InitStructure.BufSize = serial->Txbuffer->lenth;
        serial->Txbuffer->tail = serial->Txbuffer->lenth;
    }else{	
//		if(serial->Txbuffer->tail + length > serial->dma.setting_tx_len)
//		{
//			serial->dma.last_tx_index = serial->Txbuffer->tail - dma_get_counts;
//			memcpy(serial->Txbuffer->data, serial->Txbuffer->data + serial->dma.last_tx_index, dma_get_counts);
//			memcpy(serial->Txbuffer->data + dma_get_counts, buffer, length);
//			DMA_InitStructure.MemAddr = (uint32_t)(serial->Txbuffer->data);
//			DMA_InitStructure.BufSize = length + dma_get_counts;
//			serial->Txbuffer->tail = length + dma_get_counts;
//		}else{
			memcpy(serial->Txbuffer->data + serial->Txbuffer->tail, buffer, length);
			DMA_InitStructure.MemAddr = (uint32_t)(serial->Txbuffer->data + serial->Txbuffer->tail - dma_get_counts);
			DMA_InitStructure.BufSize = length + dma_get_counts;
			serial->Txbuffer->tail = serial->Txbuffer->tail + length;
//		}
    }
	
	DMA_InitStructure.PeriphAddr = (uint32_t) &(serial->uart_device->DAT);
    DMA_InitStructure.Direction = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.PeriphInc = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
    DMA_InitStructure.MemDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.CircularMode = DMA_MODE_NORMAL;
    DMA_InitStructure.Priority = DMA_PRIORITY_MEDIUM;
    DMA_InitStructure.Mem2Mem = DMA_M2M_DISABLE;
    DMA_Init(serial->dma.tx_ch, &DMA_InitStructure);
	
	DMA_ClearFlag(serial->dma.tx_gl_flag, serial->dma.tx_dma_type);
    DMA_EnableChannel(serial->dma.tx_ch, ENABLE);
}

static uint16_t ec_serial_update_write_index(struct ec_serial_device *dev, uint16_t lenth)
{
	int fifo_size = dev->dma.setting_recv_len;
	structUsartData *rx_buff = dev->Rxbuffer;
	if(rx_buff->tail == rx_buff->head)
	{
		if(rx_buff->mirror)
			return 0;
	}
	int used_size = rx_buff->tail + rx_buff->mirror*fifo_size - rx_buff->head;
	int current_size = fifo_size - used_size;
	if(current_size < lenth)
		lenth = current_size;
	if(fifo_size-rx_buff->tail > lenth)
	{
		rx_buff->tail += lenth;
		return lenth;
	}
	rx_buff->mirror = ~rx_buff->mirror;
	rx_buff->tail = lenth-(fifo_size-rx_buff->tail);
	return lenth;
}

static uint16_t ec_fifo_data_len(struct ec_serial_device *dev)
{
	int fifo_size = dev->dma.setting_recv_len;
	structUsartData *rx_buff = dev->Rxbuffer;
	if(rx_buff->tail == rx_buff->head)
	{
		if(rx_buff->mirror)
			return fifo_size;
	}
	uint16_t rx_lenth = rx_buff->tail + rx_buff->mirror*fifo_size - rx_buff->head;
	return rx_lenth;
}

static uint16_t ec_serial_push_one_data(struct ec_serial_device *dev)
{
	int fifo_size = dev->dma.setting_recv_len;
	structUsartData *rx_buff = dev->Rxbuffer;
	int used_size = rx_buff->tail + rx_buff->mirror*fifo_size - rx_buff->head;
	int current_size = fifo_size - used_size;
	if(current_size)
		rx_buff->data[rx_buff->tail] = ec32_uart_getc(dev);
	else
		return 0;
	ec_serial_update_write_index(dev,1);
	return  current_size;
}

int drv_uart_configure(ESERIAL_DEV serial_dev,struct serial_configure *config)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
	return ec32_uart_configure(serial,config);
}


int	drv_uart_control(ESERIAL_DEV serial_dev,int cmd, void *arg)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
	return ec32_uart_control(serial,cmd,arg);  		
}

/**
  * @brief all of usart init.
  * @param none.
  * @return event RT_EOK .
  */
int drv_usart_init(ESERIAL_DEV serial_dev,uint8_t mode,struct serial_configure *config)
{
	struct serial_configure conf = EC_SERIAL_CONFIG_DEFAULT;
	if(config != NULL)
		drv_uart_configure(serial_dev,config);
	else
		drv_uart_configure(serial_dev,&conf);
	
    if(mode & ESERIAL_MODE_DMA_RX){
		drv_uart_control(serial_dev,EC_DEVICE_CTRL_NVIC,(void *)conf.nvic);
		drv_uart_control(serial_dev,EC_DEVICE_CTRL_CONFIG,(void *)EC_SERIAL_DMA_RX);  		//enable DMA 
	}
	
	if(mode & ESERIAL_MODE_DMA_TX){
		drv_uart_control(serial_dev,EC_DEVICE_CTRL_NVIC,(void *)conf.nvic);
		drv_uart_control(serial_dev,EC_DEVICE_CTRL_CONFIG,(void *)EC_SERIAL_DMA_TX);  		//enable DMA 
	}
	
	if(mode & ESERIAL_MODE_IT){
		drv_uart_control(serial_dev,EC_DEVICE_CTRL_NVIC,(void *)conf.nvic);
		drv_uart_control(serial_dev,EC_DEVICE_CTRL_SET_INT,(void *)EC_SERIAL_RX_INT);
	}
//	logDebug("usart[%d] init success!",serial_dev);
    return 0;
}

int drv_serial_read(ESERIAL_DEV serial_dev,void *buffer,uint32_t size)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    if (size == 0) return 0;
	uint32_t lenth = size;
	uint8_t *p_buff = (uint8_t *)buffer;
	if(serial->uart_device->STS & USART_FLAG_RXDNE)
	{
		while(lenth--)
		{
			*p_buff = ec32_uart_getc(serial);
			p_buff ++;
		}
		return size;
	}
	return 0;
}

int drv_serial_write(ESERIAL_DEV serial_dev,const void *buffer,uint32_t size)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    if (size == 0) return 0;
	uint32_t lenth = size;
	uint8_t *p_buff = (uint8_t *)buffer;
	while(lenth--)
	{
		ec32_uart_putc(serial,*p_buff);
		p_buff ++;
	}
	return size;
}

int drv_serial_dma_write(ESERIAL_DEV serial_dev,const void *buffer,uint32_t size)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    if (size == 0) 
		return 0;
	ec32_uart_dma_tx_config(serial,(uint8_t *)buffer, size);
	return size;
}

uint16_t drv_fifo_data_get(ESERIAL_DEV serial_dev,uint8_t *buff,uint16_t lenth)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
	if(!ec_fifo_data_len(serial))							//适配shell
		return 0;
	
	for(int i=0;i<lenth;i++)
	{
		buff[i] = serial->Rxbuffer->data[serial->Rxbuffer->head++];
		if(serial->Rxbuffer->head > serial->config->bufsz-1)
		{
			serial->Rxbuffer->mirror = ~serial->Rxbuffer->mirror;
			serial->Rxbuffer->head = 0;
		}
	}
	return lenth;
}

/**
  * @brief ISR for serial interrupt
  * @param serial RT-thread serial device.
  * @param event ISR event type.
  */
static void drv_serial_isr(ESERIAL_DEV serial_dev, int event)
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    switch (event & 0xff)
    {
        /* Interrupt receive event */
        case EC_SERIAL_EVENT_RX_IND:
        case EC_SERIAL_EVENT_RX_DMADONE:
        {
			structUsartData *rx_fifo = serial->Rxbuffer;
            uint16_t rx_lenth = 0;

            /* If the event is RT_SERIAL_EVENT_RX_IND, rx_length is equal to 0 */
            rx_lenth = (event & (~0xff)) >> 8;
            if (rx_lenth)
                ec_serial_update_write_index(serial, rx_lenth);
			else 
				ec_serial_push_one_data(serial);
            /* Get the length of the data from the ringbuffer */
            rx_lenth = ec_fifo_data_len(serial);
			/* update fifo arr lenth  */
			rx_fifo->lenth = rx_lenth;
            if (rx_lenth == 0) break;
            /* Trigger the receiving completion callback */
            if (serial->rx_indicate != NULL)
                serial->rx_indicate(serial_dev, rx_lenth);
            break;
        }

        /* Interrupt transmit event */
        case EC_SERIAL_EVENT_TX_DONE:
        {
            break;
        }

        case EC_SERIAL_EVENT_TX_DMADONE:
        {
            break;
        }
        default:
            break;
    }
}

int drv_device_set_rx_indicate(ESERIAL_DEV serial_dev,int(*rx_ind)(ESERIAL_DEV serial_dev, uint16_t size))
{
	struct ec_serial_device *serial = serial_get_handle(serial_dev);
    serial->rx_indicate = rx_ind;
    return 0;
}
