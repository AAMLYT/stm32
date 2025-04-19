#include "stm32f10x.h"   


// Device header
uint16_t Arr_Value[4];
void AD_Init(void)
{
	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                          //ADC时钟必须小于14Mhz
	//初始化gio
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                              //ADC专用输入模式ＡＩＮ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//配置AD基础通道
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
	//初始化ADC单元
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                            //独立模式
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;                                  //是否扫描
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                            //是否连续
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //外部触发设置
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;												 //对齐方式
	ADC_InitStruct.ADC_NbrOfChannel = 4;																			 //采集通道数道数
	ADC_Init(ADC1,&ADC_InitStruct);
	//初始化DMA数据转运单元
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             //读取外设地址地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设是否递增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Arr_Value;                 //接收内存地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //内存是否递增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //外设是发数据函数收数据
	DMA_InitStructure.DMA_BufferSize = 4;                                       //传输次数
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //传输次数是否重装
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       //通道优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //软硬件触发选择
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	//开始采集
	//开始转运
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);   //ADC触发DMA转运
	//上电校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发ADC采集
}

uint16_t Get_AD(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,4);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

