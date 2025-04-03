#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"
int main(void)
{
	
	uint8_t a;
	while (1)
	{
		KEY_Init();
		LED_Init();
		a = KEY_D();
		if ( a==1)
		{
			LED1_Turn();
		}
		if ( a==2)                                                                      
		{
			LED2_Turn();
		} 
	
	}
}

