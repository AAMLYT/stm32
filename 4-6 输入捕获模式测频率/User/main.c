#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
/*
输入捕获测频率
TIM2：发出矩形帮我
TIM3：测量频率

*/
int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	OLED_ShowString(1, 1, "Freq:00000Hz");
	PWM_SetPrescaler(719);
	PWM_SetCompare1(50);
	while (1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
	}
}
