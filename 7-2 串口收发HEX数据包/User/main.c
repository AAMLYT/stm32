#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "USART.h"

int  main(void)
{
	TX_DataPacket[0] = 0x00;
	TX_DataPacket[1] = 0x01;
	TX_DataPacket[2] = 0x02;
	TX_DataPacket[3] = 0x03;
	Usart_Init();
	OLED_Init();
	Send_DataPacket();

	while(1)
		{
			OLED_ShowHexNum(1,1,RX_DataPacket[0],2);
			OLED_ShowHexNum(1,4,RX_DataPacket[1],2);
			OLED_ShowHexNum(1,7,RX_DataPacket[2],2);
			OLED_ShowHexNum(1,10,RX_DataPacket[3],2);
		}
}


