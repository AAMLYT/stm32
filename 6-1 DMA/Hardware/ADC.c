#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//初始化gio
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//配置AD基础通道
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
	//初始化ADC单元
	//有转运DMA可以使用连续转换，扫描模式
//	ADC_InitTypeDef ADC_InitStruct;
//	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
//	ADC_InitStruct.ADC_ScanConvMode = ENABLE;
//	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
//	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
//	ADC_InitStruct.ADC_NbrOfChannel = 4;
//	ADC_Init(ADC1,&ADC_InitStruct);
	//无DMA只能要获得一个标志位后一个一个的手动转换
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1,&ADC_InitStruct);
	//开始采集
	ADC_Cmd(ADC1,ENABLE);
	//校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	
}

uint16_t Get_AD( uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}
