#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "KEY.h"
#include "LED.h"

/*
OLED:
	GPIOB:Pin8,Pin9
	
LED:
	GPIOA:Pin0,Pin1

KEY:
	GPIOA:Pin2,Pin3


*/
int main(void)
{
	OLED_Init();
	KEY_Init();
	LED_Init();
	while (1)
	{
		if (Get_KeyValue(GPIO_Pin_2))
		{
			LED_TurnOn(GPIO_Pin_0);
		}
		if (Get_KeyValue(GPIO_Pin_3))
		{
			LED_TurnOn(GPIO_Pin_1);
		}
		OLED_ShowNum(1,1,GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0),2);
		OLED_ShowNum(2,1,GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1),2);
	}
}


