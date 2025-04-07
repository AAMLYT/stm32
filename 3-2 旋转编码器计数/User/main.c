#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "Encoding.h"

/*
编码器计数
编码器:
	GPIOA_Pin_0,1
OLED:
	GPIOB_Pin_8,9
	
*/
int main(void)
{
	uint16_t NUM = 0;
	//初始化
	Encode_Init();
	OLED_Init();
	OLED_ShowString(1,1,"count:");
	while (1)
	{
		NUM += RE_Num();
		OLED_ShowSignedNum(1,7,NUM,5);
	}
}

