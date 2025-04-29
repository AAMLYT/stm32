#ifndef __MyI2C_H
#define __MyI2C_H

void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Data);
uint8_t MyI2C_ReceiveByte(void);
void I2C_SendACK(uint8_t ACK);
uint8_t I2C_ReceiveACK(void);


#endif
