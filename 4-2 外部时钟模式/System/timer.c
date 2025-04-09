#include "stm32f10x.h"                  // Device header

/*
TIM2外设的外部时钟模式计数
对射红外模块作为外部时钟接入

*/

void EXTimer_Init(void)
{
	//外部时钟接入PA0口，打开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//外部时钟模式2配置,固定引脚为PA0不能变
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_Inverted,0x00);
	//初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//配置时基单元 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstructe;
	TIM_TimeBaseInitstructe.TIM_Prescaler = 10 - 1;//预分频器值
	TIM_TimeBaseInitstructe.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式，向上计数
	TIM_TimeBaseInitstructe.TIM_Period = 10 - 1;//自动重装器值
	TIM_TimeBaseInitstructe.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitstructe.TIM_RepetitionCounter = 0;//重复计数器的值，就是要重复计数器重复多少次，高级才有
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitstructe);
	//时基单元初始化
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitstructe);
	//启用时基单元
	TIM_Cmd(TIM2,ENABLE);
	//中断输出设置
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//NVIC配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_Initstructe;
	NVIC_Initstructe.NVIC_IRQChannel = TIM2_IRQn;//TIM2_IRQHandler
	NVIC_Initstructe.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_Initstructe.NVIC_IRQChannelSubPriority = 1;
	NVIC_Initstructe.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstructe);
}

