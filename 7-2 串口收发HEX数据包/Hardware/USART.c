#include "stm32f10x.h"                  // Device header

/*
USART2:
	GPIOA_Pin_2发送TX
	GPIOA_Pin_3接收RX
	



*/
uint8_t TX_DataPacket[4];//发送数据包缓存
uint8_t RX_DataPacket[4];//接收数据包缓存
uint8_t RX_Flag;//标志位
uint8_t RX_Data;//数据返回
void Usart_Init(void)
{
	//时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;									//pin2数据输出口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;									//pin3数据接收口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//USART初始化
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStruct);
	//串口中断配置
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	//中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	//EVIC配置
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	
	//使能
	USART_Cmd(USART2,ENABLE);

}

/*
发送数字
*/
void TX_bit(uint8_t num)
{
		USART_SendData(USART2,num);
		while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
}
/*
发送数组
*/
void TX_array(uint8_t *arrary,uint8_t lenght)
{
	uint16_t i = 0;
	for (i = 0; i<lenght; i++)
	{
		TX_bit(arrary[i]);
	}
}
/*
发送字符串
*/
void TX_string(uint8_t *string)
{
	uint16_t i = 0;
	for (i = 0; string[i]!='\0'; i++)
	{
		TX_bit(string[i]);
	}
}
uint16_t Get_Pow(uint16_t X,uint16_t Y)
{
	uint32_t Pow = 1;
	while(Y--)
	{
		 Pow *=X;
	}
	return Pow;
}
/*
发送十进制数
*/
void TX_NUM(uint32_t num,uint8_t lenght)
{
	uint16_t i;
	for (i = 0;i<lenght;i++)
	{
		TX_bit(num/Get_Pow(10,lenght-i-1)%10 + '0');
	}
}
/*
接收数据占用主程序
*/
uint8_t RE_bit(void)
{
	while(!USART_GetFlagStatus(USART2,USART_FLAG_RXNE));
	return USART_ReceiveData(USART2);
}
/*
获取中断执行标志位
*/
uint8_t GetRxFlag(void)
{
	if (RX_Flag)
	{
		RX_Flag = 0;
		return 1;
	}
	return 0;
}
/*
接收数据
*/
uint8_t ITRE_bit(void)
{
	return RX_Data;			//返回接收的数据变量
}




/*
发送HEX数据包
使用一个变量判断在进行什么操作
FF 4个HEX数据 AF
*/

void Send_DataPacket(void)
{
	TX_bit(0xFF);
	TX_array(TX_DataPacket,4);
	TX_bit(0xFE);
}

/*
接收HEX数据包

*/


/*
中断函数
状态机接收数据包

*/
void USART2_IRQHandler(void)
{
	static uint8_t i = 0;//记录接收数据数
	static uint8_t S = 0;//状态值
	if (USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		//状态0，如果接收到0xFF，状态转移到1
		if (S == 0)
		{
			if (USART_ReceiveData(USART2) == 0xFF)
			{
				S = 1;
				i = 0;
			}
		}
		//状态1，接收HEX数据，如果接收到4个数据状态转移到2
		else if (S == 1)
		{
			RX_DataPacket[i] = USART_ReceiveData(USART2);
			i++;
			if(i>=4)
			{
				S = 2;
			}
		}
		//状态2，如果接收到0xFE，状态转移到0，如果不是0xFE，则不接收
		else if (S == 2)
		{
			if (USART_ReceiveData(USART2) == 0xFE)
			{
				S = 0;
			}
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}

