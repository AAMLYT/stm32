#include "stm32f10x.h"                  // Device header

/*
********************************************************************************
SPI软件通信
CLK；时钟线Pin5
MOSI：主机输出从机输入Pin7
MISO：主机输入从机输出Pin6
CS：片选Pin4

********************************************************************************
*/
/*
********************************************************************************
SPI改变CS下降沿或者下降沿

BitVal：0/1
********************************************************************************
*/
void SPI_W_CS(uint8_t BitVal)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)BitVal);
}

/*
********************************************************************************
SPI改变CLK下降沿或者下降沿

BitVal：0/1

********************************************************************************
*/
void SPI_W_CLK(uint8_t BitVal)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitVal);
}

/*
********************************************************************************
SPI改变MOSI

BitVal：0/1

********************************************************************************
*/
void SPI_W_MOSI(uint8_t BitVal)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitVal);
}

/*
********************************************************************************
SPI读取MISO

return:MISO电平0/1

********************************************************************************
*/
uint8_t SPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}

/*
********************************************************************************
SPI软件通信初始化

模式0
********************************************************************************
*/
void MySPI_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//cs,clk,mosi
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//miso
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//模式0
	SPI_W_CS(1);
	SPI_W_CLK(0);
	
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
uint8_t SPI_SwapByte(uint8_t SwapByte)
{
	uint8_t i,Byte = 0x00;
	for (i = 0; i < 8; i ++)
	{
		SPI_W_MOSI(SwapByte & (0x80 >> i));
		SPI_W_CLK(1);
		if (SPI_R_MISO() == 1){Byte |= (0x80 >> i);}
		SPI_W_CLK(0);
	}
	return Byte;
}

