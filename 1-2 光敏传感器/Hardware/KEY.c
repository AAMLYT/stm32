#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*
按键初始化
GPIO:
	Pin:2,3
*/
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //打开GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

uint16_t Get_KeyValue(uint16_t GPIO_Pinx)
{
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pinx))//读取pinx寄存器的值，初始寄存器值为0，按下为1
	{
		Delay_ms(20);//消抖
		while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pinx));//判断是否松手
		Delay_ms(20);
		return 1;
	}
	else
	{
		return 0;
	}
}

