#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "ADC.h"
#include "OLED.h"

/*
ADC外设数据采集

*/
int  main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowString(1,1,"AD:");
	while(1)
		{
			OLED_ShowNum(1,4,Get_AD(),5);
		}
}

