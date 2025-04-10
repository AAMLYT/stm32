#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*
按键初始化
GPIO_Pin_7

*/
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIOStrcture;
	GPIOStrcture.GPIO_Mode = GPIO_Mode_IPD ;
	GPIOStrcture.GPIO_Pin = GPIO_Pin_7;
	GPIOStrcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOStrcture);
}

/*
按键检测
return:按键按下返回1，未按下返回0

*/
uint16_t ReadKey(void)
{
	if (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
	{
		Delay_ms(20);
		while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
		Delay_ms(20);
		return 1;
	}
	return 0;

}
