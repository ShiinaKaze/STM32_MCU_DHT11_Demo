#include "stm32f10x.h"
#include "oled.h"
#include "dht11.h"
#include "delay.h"
int main()
{
	DHT11 dht11 = {0};
	OLED_Init();
	OLED_ShowNum(1, 1, SystemCoreClock, 10);
	DHT11_Init();

	while (1)
	{
		OLED_ShowNum(2, 1, DHT11_ReadData(&dht11), 1);
		OLED_ShowNum(3, 1, dht11.temp_int, 2);
		OLED_ShowNum(4, 1, dht11.hum_int, 2);
	}
}
