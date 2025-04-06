#ifndef __LED_H
#define __LED_H

void LED_Init(GPIO_TypeDef* GPIOx,uint32_t RCC_APB2Periph_GPIOx , uint16_t GPIO_Pin_x);
void led1_on(void);
void led2_on(void);
void led1_off(void);
void led2_off(void);
void LED_ON(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void LED_OFF(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void LED_Turn(GPIO_TypeDef* GPIO2x,uint16_t GPIO_Pin_2x);

#endif

