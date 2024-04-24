#ifndef __DRV_ADC_H
#define __DRV_ADC_H

#include <stdint.h>
#include <stddef.h>
#include "n32l40x.h"

#define VREF_INT            1200 			// 1200mv 基准电压值
#define ADC_RESOLUTION      4095
#define VREF_INT_CHANNEL    1

#define	ADC_CH_NUM          7
#define	ADC_EQ_NUM         	12
#define	ADC_SUM_NUM			(ADC_CH_NUM*ADC_EQ_NUM)	

#define ADC_DMAy			DMA
#define	DMAy_FLAG_TCz		DMA_FLAG_TC6

#define AD_CHANNEL_1		ADC_CH_VREFINT			//基准电压检测
#define AD_CHANNEL_2  		ADC_CH_4				//heat1,PA3
#define AD_CHANNEL_3  		ADC_CH_5				//bat_AD,PA4
#define AD_CHANNEL_4  		ADC_CH_6				//bat_NTC,PA5
#define AD_CHANNEL_5		ADC_CH_7				//motor_ad,PA6
#define AD_CHANNEL_6		ADC_CH_9				//heat2,PB0
#define AD_CHANNEL_7		ADC_CH_10				//heat3,PB1

typedef enum{	 
	EDMA_CH1 = 1,		
	EDMA_CH2,
	EDMA_CH3,
	EDMA_CH4,		
	EDMA_CH5,
	EDMA_CH6,
	EDMA_CH7,		
	EDMA_CH8,
}EDMA_CHANNEL;

typedef struct{
	uint8_t ad_channel;
	uint8_t ad_regular_num;
}drv_adch_config;

typedef enum{	 
	EADC_DEV1 = 1
}EADC_DEVICE;

typedef struct _drv_adc
{
    ADC_Module *ADC_Handler;		
}drv_adc_dev;

int drv_adc_init(EADC_DEVICE adc_dev,EDMA_CHANNEL dma_ch);
void drv_adc_enable(EADC_DEVICE adc_dev, bool enabled);
uint16_t drv_get_adc_convertedcnts(uint32_t channel);		
uint16_t drv_get_convertedValue(uint8_t channel);
uint16_t drv_get_vcc_mv_value(void);

#endif
