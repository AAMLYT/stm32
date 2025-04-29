#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
/*
I2C通信设计


*/
/*
软件I2C时钟变换实现

x：上拉 1 ，下拉 0
*/
void I2C_W_SCL(uint8_t x)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)x);
	Delay_us(10);
}

/*
软件I2C位写入实现

x：上拉 1 ，下拉 0
*/
void I2C_W_SDA(uint8_t x)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)x);
	Delay_us(10);
}

/*
软件I2C位读取实现

return：一位数据  0或1
*/
uint8_t I2C_R_SDA(void)
{
	uint8_t Value;
	Value = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return Value;
}

/*
软件I2C初始化

*/
void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}

//I2C传输规则
/*
I2C起始条件

*/
void MyI2C_Start(void)
{
	//SCL于SDA都处于上拉状态，判断是否开始传输的条件
	I2C_W_SCL(1);
	I2C_W_SDA(1);
	//SDA下拉后SCL下拉，开始传输
	I2C_W_SDA(0);
	I2C_W_SCL(0);
}

/*
I2C停止条件

*/
void MyI2C_Stop(void)
{
	//SCL先下拉然后SDA下拉，创造停止基础条件
	I2C_W_SDA(0);
	//停止条件，SCL先上拉然后SDA上拉
	I2C_W_SCL(1);
	I2C_W_SDA(1);
}

/*
发送一个字节数据

Data：发送要给我字节值
*/
void MyI2C_SendByte(uint8_t Data)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		I2C_W_SDA(Data & (0x80 >> i));
		I2C_W_SCL(1);
		I2C_W_SCL(0);
	}
}

/*
接收一个字节数据

return：返回一个字节数据
*/
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t Data;
	uint8_t i;
	//释放SDA，让从机发送数据
	I2C_W_SDA(1);
	//读取SDA
	for (i = 0; i < 8; i ++)
	{
		I2C_W_SCL(1);
		Data = ((Data << 1) | I2C_R_SDA());
		I2C_W_SCL(0);
	}
	return Data;
}

/*
发送应答

ACK：发送应答值  0 应答，1 不应答
*/
void I2C_SendACK(uint8_t ACK)
{
	//发送接收应答
	I2C_W_SDA(ACK);
	//从机读取时钟
	I2C_W_SCL(1);
	I2C_W_SCL(0);
}

/*
接收应答

return 接收应答值  0 应答，1 不应答
*/
uint8_t I2C_ReceiveACK(void)
{
	uint8_t Status;
	//SDA上拉，接收应答
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	Status = I2C_R_SDA();
	//从机读取时钟
	I2C_W_SCL(0);
	return Status;
}

