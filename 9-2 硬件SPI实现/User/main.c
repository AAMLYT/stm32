#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

/*
********************************************************************************
硬件SPI通信

外设SPI1
NSS:多主机模式主机选择选择线  Pin4
SCK:同步时钟									Pin5
MOSI:主机输出从机输入线				Pin7
MISO:主机输入从机输出线				Pin6
CS:主机控制从机线							Pin3



********************************************************************************
*/

uint8_t MID;
uint16_t DID;
uint8_t Array1[] = {0x00, 0x01, 0x02, 0x03};
uint8_t Array2[4];
int main(void)
{
	int i;
  OLED_Init();	
	W25Q64_Init();
	//读取ID
	W25Q64_ReadID(&MID,&DID);
	OLED_ShowString(1,1,"ID1:");
	OLED_ShowString(2,1,"ID1:");
	OLED_ShowString(3,1,"W:");
	OLED_ShowString(4,1,"R:");
	OLED_ShowNum(1,5,MID,2);
	OLED_ShowNum(2,5,DID,4);

	while (1)
	{
		Array1[0] ++;
		Array1[1] ++;
		Array1[2] ++;
		Array1[3] ++;
		//写入数据,W25Q64每次写入时需要擦除掉数据写入
		Clean4KB(0x000000);
		WriteData(0x000100,Array1,4);
		//读取数据
		ReadData(0x000100,Array2,4);
		
		OLED_ShowNum(3,3,Array1[0],2);
		OLED_ShowNum(3,6,Array1[1],2);
		OLED_ShowNum(3,9,Array1[2],2);
		OLED_ShowNum(3,12,Array1[3],2);
		OLED_ShowNum(4,3,Array2[0],2);
		OLED_ShowNum(4,6,Array2[1],2);
		OLED_ShowNum(4,9,Array2[2],2);
		OLED_ShowNum(4,12,Array2[3],2);
	}
}
 
