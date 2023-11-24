#include "fire.h"

void Fire_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None ;//软件触发，不使用硬件触发转化
	ADC_InitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	ADC_Cmd(ADC1,ENABLE);
}

void Fire_Init2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None ;//软件触发，不使用硬件触发转化
	ADC_InitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_239Cycles5);
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	ADC_Cmd(ADC1,ENABLE);
}

void Fire_Init3(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None ;//软件触发，不使用硬件触发转化
	ADC_InitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,1,ADC_SampleTime_239Cycles5);
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	ADC_Cmd(ADC1,ENABLE);
}


uint16_t Fire_Data(void)
{
	uint16_t adc_get;
	
	uint8_t i = 0;
	for(i = 0; i<50;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);
		adc_get = adc_get + ADC_GetConversionValue(ADC1);
	}
	return adc_get/50;
}

uint16_t Fire_Data2(void)
{
	uint16_t adc_get;
	uint8_t i = 0;
	for(i = 0; i<50;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);
		adc_get = adc_get + ADC_GetConversionValue(ADC1);
	}
	return adc_get/50;
}
uint16_t Fire_Data3(void)
{
	uint16_t adc_get;
	uint8_t i = 0;
	for(i = 0; i<50;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET);
		adc_get = adc_get + ADC_GetConversionValue(ADC1);
	}
	return adc_get/50;
}




