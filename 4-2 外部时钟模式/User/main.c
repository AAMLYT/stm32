#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "timer.h"
/*
外接红外对射模块作为外部时钟
GPIO_Pin_0

*/
uint16_t NUM;//计数值
int main(void)
{
	EXTimer_Init();
	OLED_Init();
	OLED_ShowString(1,1,"count:");
	while (1)
	{
		OLED_ShowNum(1,7,NUM,5);
		OLED_ShowNum(2,1,TIM_GetCounter(TIM2),5);
		
	}
}
/*
重装次数记录

*/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update))
	{
		NUM++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	
}
