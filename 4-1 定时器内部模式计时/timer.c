#include "stm32f10x.h"                  // Device header

/*
定时器计时
t = (arr+1)*(psc+1)/72000000

*/
/* 
定时器初始化
*/
uint16_t NUM;
void Timer_Init(void)
{
	//初始化时钟TIM2在APB1总线上
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//选择内部时钟模式
	TIM_InternalClockConfig(TIM2);
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstructe;
	TIM_TimeBaseInitstructe.TIM_Prescaler = 7200 - 1;//预分频器值
	TIM_TimeBaseInitstructe.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式，向上计数
	TIM_TimeBaseInitstructe.TIM_Period = 10000 - 1;//自动重装器值
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
/*
返回计时数
*/
uint16_t RE_Num(void)
{
	return NUM;
}
/*
计时中断
*/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update))
	{
		NUM++;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

