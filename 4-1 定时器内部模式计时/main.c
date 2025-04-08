#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "timer.h"

/*
定时器内部时钟模式计时


*/

int main(void)
{
	Timer_Init();
	OLED_Init();
	OLED_ShowString(1,1,"count:");
	while (1)
	{
		OLED_ShowSignedNum(2,1,TIM_GetCounter(TIM2),5);
		OLED_ShowSignedNum(1,7,RE_Num(),5);
	}
}



