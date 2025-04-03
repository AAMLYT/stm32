#include "stm32f10x.h"                  // Device header
#include "Delay.h"


void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //打开GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

}

//led点亮简单实现

void led1_on(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

void led2_on(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}

void led1_off(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

void led2_off(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}


//led点亮
//GPIOx ：x可以是A......G
//GPIO_Pin ： 第几个I/O口
void LED_ON(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	GPIO_ResetBits(GPIOx,GPIO_Pin_x);
}
//led熄灭
//GPIOx ：x可以是A......G
//GPIO_Pin ： 第几个I/O口
void LED_OFF(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	GPIO_SetBits(GPIOx,GPIO_Pin_x);
}

//led复位 调用后会先点亮然后后熄灭
//第一个参数：读取的输入端口设置
//第二个：读取的输入I/O口
//输出操作的GPIO设置
//输出的I/O口设置
//读取按键的操作，然后根据读取到的数据对led的i/o口进行置高低电平的操作
void LED_Turn(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	if (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x)==0)
	{
		GPIO_SetBits(GPIOx,GPIO_Pin_x);
	}
	else
	{
		GPIO_ResetBits(GPIOx,GPIO_Pin_x);
	}
}

void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);					//则设置PA1引脚为高电平
	}
	else													//否则，即当前引脚输出高电平
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);					//则设置PA1引脚为低电平
	}
}
void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{                                                                                                                                                       
		GPIO_SetBits(GPIOA, GPIO_Pin_2);					//则设置PA1引脚为高电平
	}
	else													//否则，即当前引脚输出高电平
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);					//则设置PA1引脚为低电平
	}
}
