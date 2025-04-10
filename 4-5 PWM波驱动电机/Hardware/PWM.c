#include "stm32f10x.h"                  // Device header

/*
输出PWM波

*/
void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//选择内部时钟模式
	TIM_InternalClockConfig(TIM2);
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	//配置时基单元 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstructe;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitstructe);
	TIM_TimeBaseInitstructe.TIM_Prescaler = 720 - 1;//预分频器值
	TIM_TimeBaseInitstructe.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式，向上计数
	TIM_TimeBaseInitstructe.TIM_Period = 2000 - 1;//自动重装器值
	TIM_TimeBaseInitstructe.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitstructe.TIM_RepetitionCounter = 0;//重复计数器的值，就是要重复计数器重复多少次，高级才有
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitstructe);
	//输出比较初始化
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;  //PWM1模式
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM2,&TIM_OCInitStruct);
	
	//启用
	TIM_Cmd(TIM2,ENABLE);
}

/*
设置比较值
*/
void PWM_SetCmp(uint16_t NUM)
{
	TIM_SetCompare1(TIM2,NUM-1);
}
