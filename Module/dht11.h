#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

typedef struct DHT11 {
	uint8_t hum_int;
	uint8_t hum_dec;
	uint8_t temp_int;
	uint8_t temp_dec;
	uint8_t check_sum;
} DHT11;
                          
uint8_t DHT11_ReadData(DHT11 *dht11);
uint8_t DHT11_Check(void);                    
void DHT11_Init(void);
#endif
