#include "drv_adc.h"
#include "br_config.h"
#include "drv_msp.h"
#include "log.h"

#ifdef BSP_USING_ADC

static volatile uint16_t wADC_ConvertedValue[ADC_SUM_NUM] = {0};
static drv_adc_dev user_adc[] = {ADC};
static const drv_adch_config adch_conf[] = {
	{AD_CHANNEL_1, 1},
	{AD_CHANNEL_2, 2},
	{AD_CHANNEL_3, 3},
	{AD_CHANNEL_4, 4},
	{AD_CHANNEL_5, 5},
	{AD_CHANNEL_6, 6},
	{AD_CHANNEL_7, 7},
};
	
static drv_adc_dev *drv_get_adc_device(EADC_DEVICE adc_dev)
{
#ifdef BSP_USING_ADC
	if(adc_dev == EADC_DEV1)	return &user_adc[0];
#endif
	return NULL;
}

static DMA_ChannelType *drv_get_dma_channel(EDMA_CHANNEL dma_ch)
{
	if(dma_ch == EDMA_CH1)	return DMA_CH1;
	if(dma_ch == EDMA_CH2)	return DMA_CH2;
	if(dma_ch == EDMA_CH3)	return DMA_CH3;
	if(dma_ch == EDMA_CH4)	return DMA_CH4;
	if(dma_ch == EDMA_CH5)	return DMA_CH5;
	if(dma_ch == EDMA_CH6)	return DMA_CH6;
	if(dma_ch == EDMA_CH7)	return DMA_CH7;
	if(dma_ch == EDMA_CH8)	return DMA_CH8;
	return NULL;	
}

static void drv_dma_configuration(EADC_DEVICE adc_dev,EDMA_CHANNEL dma_ch)
{
	DMA_InitType DMA_InitStructure;
	DMA_ChannelType * dma_chx = drv_get_dma_channel(dma_ch);
	
	DMA_DeInit(dma_chx);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&ADC->DAT;
    DMA_InitStructure.MemAddr        = (uint32_t)&wADC_ConvertedValue;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;
    DMA_InitStructure.BufSize        = ADC_SUM_NUM;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_HALFWORD;
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.CircularMode   = DMA_MODE_CIRCULAR;
    DMA_InitStructure.Priority       = DMA_PRIORITY_MEDIUM;
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(dma_chx, &DMA_InitStructure);
	
	//DMA REMAP ADC
	DMA_RequestRemap(DMA_REMAP_ADC1, ADC_DMAy, dma_chx, ENABLE);
	
    /* Enable DMA channel1 */
    DMA_EnableChannel(dma_chx,ENABLE);
}

static uint32_t adc_get_chsnumber(uint8_t num)
{
	return num;
}

static int drv_adc_configuration(EADC_DEVICE adc_dev)
{	
	ADC_Module *adc_handler = drv_get_adc_device(adc_dev)->ADC_Handler;
	uint32_t adc_ch_num = adc_get_chsnumber((uint8_t)ADC_CH_NUM);
    ADC_InitType ADC_InitStructure;
	
    ec32_msp_adc_init(adc_handler);

	ADC_InitStruct(&ADC_InitStructure);
    ADC_InitStructure.MultiChEn      = ENABLE;
    ADC_InitStructure.ContinueConvEn = ENABLE;
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;
    ADC_InitStructure.ChsNumber      = adc_ch_num;
    ADC_Init(adc_handler,&ADC_InitStructure);
    	
	for(int i=0;i<adc_ch_num;i++)
	{
		ADC_ConfigRegularChannel(adc_handler,adch_conf[i].ad_channel,adch_conf[i].ad_regular_num,ADC_SAMP_TIME_239CYCLES5);
		if ((adch_conf[i].ad_channel == ADC_CH_VREFINT) || (adch_conf[i].ad_channel == ADC_CH_TEMP_SENSOR))
		{
			ADC_EnableTempSensorVrefint(ENABLE);
		}
	}
	
    /* Enable ADC DMA */
    ADC_EnableDMA(adc_handler, ENABLE);
    return 0;
}

void drv_adc_enable(EADC_DEVICE adc_dev, bool enabled)
{
	ADC_Module *adc_handler = drv_get_adc_device(adc_dev)->ADC_Handler;
	if (enabled)
    {
        /* Enable ADCx */
         ADC_Enable(adc_handler, ENABLE);
		/* Check ADC Ready */
		while(ADC_GetFlagStatusNew(adc_handler,ADC_FLAG_RDY) == RESET)
        ;
		/* Start ADCx calibration */
         ADC_StartCalibration(adc_handler);
		/* Check the end of ADCx calibration */
         while(ADC_GetCalibrationStatus(adc_handler));
		
		 ADC_EnableSoftwareStartConv(adc_handler, ENABLE);
    }
    else
    {
        /* Enable ADCx */
        ADC_Enable(adc_handler, DISABLE);
		DMA_ClearFlag(DMAy_FLAG_TCz,ADC_DMAy);
    }
}

/* 
*@function  		get_adc_convertedValue
*@param channel    	1~ADC_CH_NUM
*@return 			adc channel value
*/
uint16_t drv_get_adc_convertedcnts(uint32_t channel)			
{
	if((channel < 1)||(channel > ADC_CH_NUM))
		return 0xFFFF;
	
	if(DMA_GetFlagStatus(DMAy_FLAG_TCz,ADC_DMAy))
    {
		uint32_t adc_value = 0;
		uint32_t value_max = wADC_ConvertedValue[channel-1];
		uint32_t value_min = wADC_ConvertedValue[channel-1];
		for(int i=channel-1;i<ADC_SUM_NUM;i+=ADC_CH_NUM)
		{
			if(wADC_ConvertedValue[i]>value_max)	value_max = wADC_ConvertedValue[i];
			if(wADC_ConvertedValue[i]<value_min)	value_min = wADC_ConvertedValue[i];
			adc_value+=wADC_ConvertedValue[i];
		}
		adc_value = adc_value - value_max - value_min;
		adc_value = adc_value/(ADC_EQ_NUM-2);		
		
		if(adc_value > 0xFFFF)
			return 0xFFFE;
		
		return 	adc_value;
	}
	return 0xFFFD;
}

int drv_adc_init(EADC_DEVICE adc_dev,EDMA_CHANNEL dma_ch)
{
	drv_adc_enable(adc_dev,DISABLE);
	drv_adc_configuration(adc_dev);
	drv_dma_configuration(adc_dev,dma_ch);
	return 0;
}

uint16_t drv_get_vcc_mv_value(void)
{
	uint32_t vref_cnt = drv_get_adc_convertedcnts(VREF_INT_CHANNEL);		//Vref
	uint32_t vcc_ad = VREF_INT * ADC_RESOLUTION / vref_cnt;
	return vcc_ad;
}

uint16_t drv_get_convertedValue(uint8_t channel)
{
	uint32_t vcc_mv = drv_get_vcc_mv_value();
	uint32_t cnt = drv_get_adc_convertedcnts(channel);
	uint32_t value = cnt * vcc_mv / ADC_RESOLUTION;
	return value;
}

int hal_adc_init_start(void)
{
	if(drv_adc_init(EADC_DEV1,EDMA_CH6) != 0)
	{
		logError();
		return -1;
	}
	drv_adc_enable(EADC_DEV1,ENABLE);
	return 0;
}


#endif /* BSP_USING_ADC */


