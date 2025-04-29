#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h" 
#include "MyI2C.h" 

/*
配置MPU6050模块

*/
/*
向MPU6050中对应寄存器写入值

Reg：寄存器地址
Byte：写入值
*/
void MPU6050_W_RegByte(uint8_t Reg,uint8_t Byte)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS_W);
	I2C_ReceiveACK();
	MyI2C_SendByte(Reg);
	I2C_ReceiveACK();
	MyI2C_SendByte(Byte);
	I2C_ReceiveACK();
	MyI2C_Stop();
	
}
/*
读取MPU6050中对应寄存器值

Reg：寄存器地址
return：返回对应寄存器值
*/
uint8_t MPU6050_R_RegByte(uint8_t Reg)
{
	uint8_t Data;
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS_W);
	I2C_ReceiveACK();
	MyI2C_SendByte(Reg);
	I2C_ReceiveACK();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS_R);
	I2C_ReceiveACK();
	Data = MyI2C_ReceiveByte();
	I2C_SendACK(1);                      //只接收一个字节，所以不应答
	MyI2C_Stop();
	return Data;
}

/*
MPU初始化

*/
void MPU_Init(void)
{
	MyI2C_Init();
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
