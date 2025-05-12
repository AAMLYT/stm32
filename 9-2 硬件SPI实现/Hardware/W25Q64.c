#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64-INS.h"


/*
********************************************************************************
W25Q64初始化
空间分配；
一块64kb   一扇4kb    一页256Byte
一扇有16页
一块有16扇
共128块

********************************************************************************
*/
void W25Q64_Init(void)
{
	MySPI_Init();
}

/*
********************************************************************************
写使能

********************************************************************************
*/
void WriteEnable(void)
{
	SPI_Start();
	SPI_SwapByte(W25Q64_WRITE_ENABLE);
	SPI_Stop();
}
/*
********************************************************************************
W25Q64忙碌等待

********************************************************************************
*/
void WaitBusy(void)
{
	uint32_t Count = 100000;
	SPI_Start();
	SPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	while ((SPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Count --;
		if (Count == 0)
		{
			break;
		}
	}
	SPI_Stop();
}

/*
********************************************************************************
W25Q64扇区擦除

RegAddress：要擦除扇区地址
********************************************************************************
*/
void Clean4KB(uint32_t Address)
{
	WriteEnable();
	SPI_Start();
	SPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);//擦除最小4kb
	SPI_SwapByte(Address >> 16);   //16---23位移动到低8位
	SPI_SwapByte(Address >> 8);		//8---15移动到低8位
	SPI_SwapByte(Address);					//0---7位
	SPI_Stop();
	WaitBusy();	
}

/*
********************************************************************************
W25Q64读取ID

MID：8位商家地址
DID：16位设备地址
********************************************************************************
*/
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	SPI_Start();
	SPI_SwapByte(W25Q64_JEDEC_ID);
	*MID = SPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = SPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= SPI_SwapByte(W25Q64_DUMMY_BYTE);
	SPI_Stop();
}

/*
********************************************************************************
W25Q64写入数据
要使能
写数据需要擦除最小单位一扇4kb
只能从1变0，不能0变1
写数据最多只能连续写入一页数据，如果多写人会返回页首覆盖数据
写数据会把数据写入RAM区中不会立即写入到flash中
所以完成写入操作时会进入忙碌状态，会影响下一步读写操作

RegAddress：24位寄存器地址
DataArray：要写入数据，数组
NUM：数组数量，最大256
********************************************************************************
*/
void WriteData(uint32_t Address, uint8_t *DataArray, uint16_t NUM)
{
	uint16_t i;
	//使能
	WriteEnable();
	
	SPI_Start();
	SPI_SwapByte(W25Q64_PAGE_PROGRAM);			//交换发送写入数据的指令
	SPI_SwapByte(Address >> 16);				    //交换发送地址23~16位
	SPI_SwapByte(Address >> 8);				      //交换发送地址15~8位
	SPI_SwapByte(Address);	
	//传输数据
	for (i = 0; i < NUM; i ++)
	{
		SPI_SwapByte(DataArray[i]);
	}
	SPI_Stop();
  //忙碌等待
	WaitBusy();
}

/*
********************************************************************************
W25Q64读取数据

RegAddress：24位寄存器地址
DataArray：读取数据存入地址
NUM：读取数量
********************************************************************************
*/
void ReadData(uint32_t Address, uint8_t *DataArray, uint32_t NUM)
{
	uint32_t i;
	//使能
	SPI_Start();
	SPI_SwapByte(W25Q64_READ_DATA);			//交换发送读取数据的指令
	SPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	SPI_SwapByte(Address >> 8);				  //交换发送地址15~8位
	SPI_SwapByte(Address);	
	//传输数据
	for (i = 0; i < NUM; i ++)
	{
		DataArray[i] = SPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	SPI_Stop();
}
