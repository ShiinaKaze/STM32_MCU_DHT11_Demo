#include "stm32f10x.h"
#include "sw_i2c.h"

/**
 * @brief  Initializes I2C peripheral
 * @param  None
 * @retval None
 */
void SW_I2C_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SW_SCL_Pin | SW_SDA_Pin;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SW_MCU_SCL(Bit_SET);
	SW_MCU_SDA(Bit_SET);
}

/**
 * @brief  I2C start condition
 * @param  None
 * @retval None
 */
void SW_I2C_Start(void)
{
	SW_MCU_SDA(Bit_SET);
	SW_MCU_SCL(Bit_SET);
	SW_MCU_SDA(Bit_RESET);
	SW_MCU_SCL(Bit_RESET);
}

/**
 * @brief  I2C stop condition
 * @param  None
 * @retval None
 */
void SW_I2C_Stop(void)
{
	SW_MCU_SDA(Bit_RESET);
	SW_MCU_SCL(Bit_SET);
	SW_MCU_SDA(Bit_SET);
}

/**
 * @brief  Transmit one byte using I2C
 * @param  Byte
 * @retval None
 */
uint8_t SW_I2C_TransmitByte(uint8_t Byte)
{
	uint8_t ack;
	for (uint8_t i = 0; i < 8; i++)
	{
		if (Byte & 0x80)
		{
			SW_MCU_SDA(Bit_SET);
		}
		else
		{
			SW_MCU_SDA(Bit_RESET);
		}

		Byte <<= 1;

		SW_MCU_SCL(Bit_SET);
		SW_MCU_SCL(Bit_RESET);
	}

	// receive ACK
	ack = SW_MCU_ReadBit();
	SW_MCU_SCL(Bit_SET);
	SW_MCU_SCL(Bit_RESET);
	return ack;
}
