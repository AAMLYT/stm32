#ifndef __MPU6050_H
#define __MPU6050_H

void MPU_Init(void);
uint8_t GET_ID(void);
void MPU6050_ReceiveData(int16_t *XA, int16_t *YA, int16_t *ZA, int16_t *XG, int16_t *YG, int16_t *ZG);

#endif
