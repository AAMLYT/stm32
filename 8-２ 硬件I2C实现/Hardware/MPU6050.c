#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h" 


/*
异常处理函数

I2Cx：I2C外设
I2C_EVENT：事件标志位
*/
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
}

/*
向MPU6050中对应寄存器写入值

Reg：寄存器地址
Byte：写入值
*/
void MPU6050_W_RegByte(uint8_t Reg,uint8_t Byte)
{
	I2C_GenerateSTART(I2C2, ENABLE);																					//起始
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);										//EV5事件标志位判断
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_W, I2C_Direction_Transmitter);  //寻找MPU6050是否可以通信
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);			//EV6发送事件标志位判断
	
	I2C_SendData(I2C2, Reg);																									//找到要发送数据的寄存器
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);							//EV8事件标志判断，移位寄存器非空，数据寄存器为空
	
	I2C_SendData(I2C2, Byte);																									//发送数据
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);								//EV8_2事件标志判断，移位寄存器空，数据寄存器为空
	
	I2C_GenerateSTOP(I2C2,ENABLE);																						//停止
	
}
/*
读取MPU6050中对应寄存器值

Reg：寄存器地址
return：返回对应寄存器值
*/
uint8_t MPU6050_R_RegByte(uint8_t Reg)
{
	uint8_t Data;

	I2C_GenerateSTART(I2C2, ENABLE);																					//起始
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);										//EV5事件判断
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_W, I2C_Direction_Transmitter);	//寻找MPU6050是否可以通信
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);			//EV6发送事件标志位判断
	
	I2C_SendData(I2C2, Reg);																									//找到要接收数据的寄存器
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);	              //EV8_2事件标志判断，移位寄存器空，数据寄存器为空
	
	I2C_GenerateSTART(I2C2, ENABLE);																					//起始
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);										//EV5事件标志位判断
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS_W, I2C_Direction_Receiver);			//寻找MPU6050是否可以通信
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);					//EV6发送事件标志位判断
	                                                                          //此时再次与MPU6050通信，地址指针指向上一次通信的寄存器
	I2C_AcknowledgeConfig(I2C2, DISABLE);							                        //要提前设置不应答与停止，因为事件7产生与数据传输同时完成，所以要提前设置停止条件												
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);            			//EV7事件判断，是否读取完成
	Data = I2C_ReceiveData(I2C2);																							//接收寄存器值
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);																			//应答位设置为高方便下次数据传输,有无都可以
	
	return Data;
}

/*
MPU初始化

*/
void MPU_Init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStrcture;
	GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStrcture.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStrcture);
	
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 50000;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStruct);
	
	I2C_Cmd(I2C2, ENABLE);
	//MPU6050内部寄存器配置
	MPU6050_W_RegByte(MPU6050_PWR_MGMT_1,0x01);
	MPU6050_W_RegByte(MPU6050_PWR_MGMT_2,0x00);
	MPU6050_W_RegByte(MPU6050_SMPLRT_DIV,0x09);
	MPU6050_W_RegByte(MPU6050_CONFIG, 0x06);
	MPU6050_W_RegByte(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_W_RegByte(MPU6050_ACCEL_CONFIG, 0x18);
}


/*
读取MPU6050加速度与角速度值

传入地址，把数据写入地址中
*(X.Y.Z)A：读取XYZ轴加速度值
*(X.Y.Z)G：读取XYZ轴角速度值
*/
void MPU6050_ReceiveData(int16_t *XA, int16_t *YA, int16_t *ZA, int16_t *XG, int16_t *YG, int16_t *ZG)
{
	uint8_t DataH, DataL;
	DataH = MPU6050_R_RegByte(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_R_RegByte(MPU6050_ACCEL_XOUT_L);
	*XA = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_RegByte(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_R_RegByte(MPU6050_ACCEL_YOUT_L);
	*YA = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_RegByte(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_R_RegByte(MPU6050_ACCEL_ZOUT_L);
	*ZA = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_RegByte(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_R_RegByte(MPU6050_GYRO_XOUT_L);
	*XG = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_RegByte(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_R_RegByte(MPU6050_GYRO_YOUT_L);
	*YG = (DataH << 8) | DataL;
	
	DataH = MPU6050_R_RegByte(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_R_RegByte(MPU6050_GYRO_ZOUT_L);
	*ZG = (DataH << 8) | DataL;
}
