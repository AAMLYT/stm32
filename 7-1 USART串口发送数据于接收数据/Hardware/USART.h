#ifndef __USART_H
#define __USART_H

void Usart_Init(void);
void TX_bit(uint8_t num);
void TX_array(uint8_t *arrary,uint8_t lenght);
void TX_string(uint8_t *string);
void TX_NUM(uint32_t num,uint8_t lenght);
uint8_t RE_bit(void);
uint8_t GetRxFlag(void);
uint8_t ITRE_bit(void);

#endif

