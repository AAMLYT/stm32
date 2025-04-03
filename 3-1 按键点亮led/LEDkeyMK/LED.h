#ifndef __LED_H
#define __LED_H

void LED_Init(void);
void led1_on(void);
void led2_on(void);
void led1_off(void);
void led2_off(void);
void LED_ON(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void LED_OFF(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void LED_Turn(GPIO_TypeDef* GPIO2x,uint16_t GPIO_Pin_2x);
void LED2_Turn(void);
void LED1_Turn(void);

#endif

