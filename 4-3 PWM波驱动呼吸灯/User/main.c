#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
/*
TIM输出比较功能输出PWM波驱动呼吸灯
GPIO_Pin_0
CCR/AAR就是设置输出PWM波占空比
通过不断改变CCR寄存器大小从而改变占空比大小从而改变LED亮度实现呼吸灯
ARR固定为100
0<= CCR <=99
*/
int main(void)
{
	OLED_Init();
	PWM_Init();
	while (1)
	{
		int8_t i = 0;
		for (i=0;i<100;i++)
		{
			PWM_SetCompare1(i);
			OLED_ShowSignedNum(1,1,TIM_GetCapture1(TIM2),4);
			Delay_ms(10);
		}
		for (i=100;i>=0;i--)
		{
			PWM_SetCompare1(i);
			OLED_ShowSignedNum(1,1,TIM_GetCapture1(TIM2),4);
			Delay_ms(10);
		}
	}
}

