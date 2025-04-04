#include "stm32f10x.h"                  // Device header

/*
光敏传感器初始化
GPIO：
	Pin:2
*/
void LightSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //打开GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}


/*  
需要通过光传感器读取IO口的操作
若光传感器检测强度大于光照阈值则Do引脚输出0，
若没有达到光照阈值do口输出1， 
*/
uint16_t Read_Ligh(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);//返回Pin2寄存器值若大于阈值则返回0.小于阈值返回1
}
