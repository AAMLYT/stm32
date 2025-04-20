#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "USART.h"

int  main(void)
{
	uint8_t arrary[] = {0xAF,0x22,0x46,0x4c};
	uint8_t String[] = "hello word";
	Usart_Init();
	OLED_Init();
	/*
	发送
	*/
	//TX_bit(0x41);
	//TX_array(arrary,4);
	//TX_string(String);
	//TX_NUM(543542,6);
	/*
	接收
	*/
	uint8_t Num;
	uint8_t Num2;
	while(1)
		{
			if (GetRxFlag())
			{
				Num = ITRE_bit();
				OLED_ShowHexNum(1,1,Num,2);
			}
		}
}


