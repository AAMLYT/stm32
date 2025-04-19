#include "stm32f10x.h"                  // Device header

void MyDMA_Init(uint32_t AddrA,uint32_t AddrB)
{
	//DMA时钟在APB总线上
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//初始化DMA数据转运
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;//读取外设地址地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//外设是否递增
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;//接收内存地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable   ;//内存是否递增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设是发数据函数收数据
	DMA_InitStructure.DMA_BufferSize = 0;//传输次数
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//传输次数是否重装
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//通道优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//软硬件触发选择
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	//开始转运
	DMA_Cmd(DMA1_Channel1,DISABLE);
	
}

void Get_DataValue(uint16_t Size)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,Size);
	DMA_Cmd(DMA1_Channel1,ENABLE);

	while (DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

