#include "stm32f10x.h"                  // Device header


/*
********************************************************************************
SPI改变CS下降沿或者下降沿

BitVal：0/1
********************************************************************************
*/
void SPI_W_CS(uint8_t BitVal)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction)BitVal);
}

/*
********************************************************************************
SPI硬件通信初始化

SPI软件通信
CLK；时钟线Pin5
MOSI：主机输出从机输入Pin7
MISO：主机输入从机输出Pin6
CS：片选Pin3

********************************************************************************
*/
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//CS
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//MISO,MOSI,CLK
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//SPI初始化
	SPI_InitTypeDef SPIStructure;
	SPIStructure.SPI_Mode = SPI_Mode_Master;
	SPIStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPIStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPIStructure.SPI_CPOL = SPI_CPOL_Low;
	SPIStructure.SPI_CRCPolynomial = 7;
	SPIStructure.SPI_DataSize = SPI_DataSize_8b;
	SPIStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPIStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPIStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1,&SPIStructure);
	//使能
	SPI_Cmd(SPI1,ENABLE);

}

/*
********************************************************************************
SPI通信起始条件

CS：下降沿
********************************************************************************
*/
void SPI_Start(void)
{
	SPI_W_CS(0);
}

/*
********************************************************************************
SPI通信停止条件

CS：下降沿
********************************************************************************
*/
void SPI_Stop(void)
{
	SPI_W_CS(1);
}

/*
********************************************************************************
SPI通信交换字节,mosi发送高位,miso接收高位


SwapByte：传入交换字节
return：SwapByte，读取交换字节
********************************************************************************
*/
uint16_t SPI_SwapByte(uint8_t SwapByte)
{
	while (!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,SwapByte);
	
	while (!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI1);
}

