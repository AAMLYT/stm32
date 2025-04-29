#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

/* 
硬件I2C通信MPU6050实现
 

*/
int16_t AX, AY, AZ, GX, GY, GZ;	
int main(void)
{
  OLED_Init();	
	MPU_Init();
	while (1)
	{
		MPU6050_ReceiveData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}
 
