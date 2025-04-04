#include "stm32f10x.h"                  // Device header

//LED初始化
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //打开GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

//开灯
void LED_On(uint16_t GPIO_Pinx)
{
	GPIO_ResetBits(GPIOA,GPIO_Pinx);
}
//关灯
void LED_Off(uint16_t GPIO_Pinx)
{
	GPIO_SetBits(GPIOA,GPIO_Pinx);
}

//LED反转
void LED_TurnOn(uint16_t GPIO_Pinx)
{
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pinx))//读取寄存器值是0还是1，如果是0则置1，若是1则置0，同一个寄存器
	{
		GPIO_ResetBits(GPIOA,GPIO_Pinx);
	}
	else
	{
		GPIO_SetBits(GPIOA,GPIO_Pinx);
	}
}
