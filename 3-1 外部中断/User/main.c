#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "DSSensor.h"
/*
GPIOB_Pin_14:外部中断触发线

*/
int main(void)
{
	DsSensor_Init();
	OLED_Init();
	OLED_ShowString(1,1,"count:");
	while (1)
	{
		OLED_ShowNum(1,7,DS_count(),5);
	}
}

