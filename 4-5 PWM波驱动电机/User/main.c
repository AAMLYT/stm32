#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "DianJi.h"
#include "KEY.h"
#include "PWM.h"
/*
PWM波驱动电机转动
按键1按下正转
按键2按下反转
按键3按下停止
KEY:
GPIOA:Pin1,Pin2,Pin3
MOTOR:
GPIOA:Pin4,Pin5
PWM:
GPIOA:Pin0
*/
int main(void)                 
{
	OLED_Init();
	KEY_Init();
	MOTOR_Init();
	while (1)
	{
		if (Get_KeyValue(GPIO_Pin_1))
		{
			PWM_SetCmp(1999);
			FWD();
		}
		else if (Get_KeyValue(GPIO_Pin_2))
		{
			PWM_SetCmp(1999);
			REV();
		}
		else if (Get_KeyValue(GPIO_Pin_3))
		{
			PWM_SetCmp(0);
			STP();
		}
	}
}
