#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "LightSensor.h"
#include "LED.h"

/*

OLED:
	GPIOB:Pin8,Pin9
	
LED:
	GPIOA:Pin0

LightSensor:
	GPIOA:Pin2


*/
int main(void)
{
	//初始化
	OLED_Init();
	LED_Init();
	LightSensor_Init();
	while (1)
	{
		//检测光强开关灯
		if (Read_Ligh())       
		{ 
			LED_On(GPIO_Pin_0);
		}
		else
		{
			LED_Off(GPIO_Pin_0);
		}
	}
}


