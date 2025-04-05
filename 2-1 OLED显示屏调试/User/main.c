#include "stm32f10x.h"  // Device header
#include "OLED.h"
/*
测试OLED显示字符、字符串、数字功能
OLED:
	GPIOB_Pin:8、9
*/
int main(void)
{
	OLED_Init();
	while (1)
	{
		OLED_ShowChar(1,1,'s');
		OLED_ShowChar(1,2,'t');
		OLED_ShowChar(1,3,'m');
		OLED_ShowString(2,1,"stm");
		OLED_ShowNum(1,4,32,2);
		OLED_ShowNum(2,4,32,2);
		OLED_ShowSignedNum(3,1,666,3);
		OLED_ShowHexNum(4,1,10,6);
		OLED_ShowBinNum(4,8,10,4);
	}
}

