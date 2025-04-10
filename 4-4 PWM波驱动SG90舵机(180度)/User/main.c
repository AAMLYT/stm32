#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "Key.h"
/*
PWM波驱动舵机转向
180度版本舵机SG90
脉冲周期20ms
脉冲高电平时间：0.5ms   1ms   1.5ms   2ms   2.5ms
旋转角度        0       45    90      135   180
CCR             0       100   150     200   250
*/
uint16_t NUM = 0;//设置输出比较CCR寄存器值
int main(void)
{
	Key_Init();
	OLED_Init();
	PWM_Init();
	while (1)
	{
		if (ReadKey())																		//按键按下舵机正转180度后反转180度
		{
			for (NUM=0;NUM<250;NUM+=50)
			{
				TIM_SetCompare1(TIM2,NUM);
				OLED_ShowNum(1,1,TIM_GetCounter(TIM2),6);
				OLED_ShowNum(2,1,TIM_GetCapture1(TIM2),6);
				Delay_ms(500);
			}
			for (NUM=250;NUM>0;NUM-=50)
			{
				TIM_SetCompare1(TIM2,NUM);
				OLED_ShowNum(1,1,TIM_GetCounter(TIM2),6);
				OLED_ShowNum(2,1,TIM_GetCapture1(TIM2),6);
				Delay_ms(500);
			}
			
		}
	}
}
