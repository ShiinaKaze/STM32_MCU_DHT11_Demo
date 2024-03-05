#include "dht11.h"
#include "delay.h"

/* DHT11 configuration*/
#define DHT11_RCC_APB RCC_APB2Periph_GPIOA
#define DHT11_GPIOx GPIOA
#define DHT11_Pin GPIO_Pin_1
#define MCU_WriteBit(BitVal) GPIO_WriteBit(DHT11_GPIOx, DHT11_Pin, BitVal)
#define MCU_ReadBit() GPIO_ReadInputDataBit(DHT11_GPIOx, DHT11_Pin)
#define DHT11_Delay_us(us) Delay_us(us)
#define DHT11_Delay_ms(ms) Delay_ms(ms)
#define DHT11_Delay_s(s) Delay_s(s)
#define CONN_RETRY_MAX 200

/**
 * @brief  Bit_SET and Bit_RESET enumeration
 */

typedef enum
{
	DHT11_Offline = 0,
	DHT11_Online
} DHT11_Status;

// /**
//  * @brief  Set IO input mode
//  * @param  None
//  * @retval None
//  */
// void DHT11_SetInput(void)
// {

// 	GPIO_InitTypeDef GPIO_InitStructure;

// 	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(DHT11_GPIOx, &GPIO_InitStructure);
// 	GPIO_SetBits(DHT11_GPIOx, DHT11_Pin);
// }
// /**
//  * @brief  Set IO output mode
//  * @param  None
//  * @retval None
//  */
// void DHT11_SetOutput(void)
// {

// 	GPIO_InitTypeDef GPIO_InitStructure;

// 	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(DHT11_GPIOx, &GPIO_InitStructure);
// 	GPIO_SetBits(DHT11_GPIOx, DHT11_Pin);
// }

/**
 * @brief  DHT11 Start
 * @param  None
 * @retval None
 */
void DHT11_Start(void)
{
	MCU_WriteBit(Bit_SET); // From high level start
	// DHT11_SetOutput(); 	// Set OUTPUT
	MCU_WriteBit(Bit_RESET);
	DHT11_Delay_ms(20); // MCU pull down 20 ms
	MCU_WriteBit(Bit_SET);
	DHT11_Delay_us(30); // MCU pull up 30 ms
}

/**
 * @brief  DHT11 Check: Check whether the device is online
 * @param  None
 * @retval DHT11 status: 1 indicates online and 0 indicates offline
 */
DHT11_Status DHT11_Check(void)
{
	uint8_t retry = 0;
	// DHT11_SetInput();	  // SET INPUT
	while (MCU_ReadBit()) // DHT11 will pull down 40-80 us
	{
		retry++;
		if (retry >= CONN_RETRY_MAX)
		{
			return DHT11_Offline;
		}
	}
	retry = 0;
	while (!MCU_ReadBit()) // DHT11 will pull up 40-80 us
	{
		retry++;
		if (retry >= CONN_RETRY_MAX)
		{
			return DHT11_Offline;
		}
	}

	return DHT11_Online;
}

/**
 * @brief  Read one bit from DHT11
 * @param  None
 * @retval One bit data
 */
uint8_t DHT11_ReadBit(void)
{
	uint8_t retry = 0;
	while (MCU_ReadBit() && retry < CONN_RETRY_MAX) // start to transmit 1 bit (50 us)
	{
		retry++;
	}
	retry = 0;
	while (!MCU_ReadBit() && retry < CONN_RETRY_MAX) // voltage-length decide data 0 or 1
	{
		retry++;
	}
	DHT11_Delay_us(40); // wait for 40 us, if high level, data is 1, otherwise it is 0
	if (MCU_ReadBit())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief  Read one byte from DHT11
 * @param  None
 * @retval One byte data
 */
uint8_t DHT11_ReadByte(void)
{
	uint8_t data = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		data <<= 1;
		data |= DHT11_ReadBit();
	}
	return data;
}

/**
 * @brief  Read data from DHT11, temperature range (0-50), humidity range (20-90)
 * @param  DHT11: DHT11  structure
 * @retval DHT11 status: 1 indicates online and 0 indicates offline
 */
DHT11_Status DHT11_ReadData(DHT11 *dht11)
{
	DHT11_Start();
	if (DHT11_Check() == 1)
	{
		dht11->hum_int = DHT11_ReadByte();
		dht11->hum_dec = DHT11_ReadByte();
		dht11->temp_int = DHT11_ReadByte();
		dht11->temp_dec = DHT11_ReadByte();
		dht11->check_sum = DHT11_ReadByte();
	}
	else
	{
		return DHT11_Offline;
	}
	DHT11_Delay_s(1);
	return DHT11_Online;
}

/**
 * @brief  Initializes the DHT11
 * @param  None
 * @retval None
 */
void DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DHT11_RCC_APB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_GPIOx, &GPIO_InitStructure);
}
