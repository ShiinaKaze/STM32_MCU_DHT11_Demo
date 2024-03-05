#ifndef __SW_I2C_H
#define __SW_I2C_H

#include "stm32f10x.h"

/*Pin configuration*/
#define SW_I2C_RCC_APB
#define SW_I2C_GPIOx GPIOB
#define SW_SCL_Pin GPIO_Pin_6
#define SW_SDA_Pin GPIO_Pin_7
#define SW_I2C_ACK 0
#define SW_I2C_None_ACK 1
#define SW_MCU_SCL(PinState) GPIO_WriteBit(SW_I2C_GPIOx, SW_SCL_Pin, PinState)
#define SW_MCU_SDA(PinState) GPIO_WriteBit(SW_I2C_GPIOx, SW_SDA_Pin, PinState)
#define SW_MCU_ReadBit() GPIO_ReadInputDataBit(SW_I2C_GPIOx, SW_SDA_Pin)

void SW_I2C_Init(void);
void SW_I2C_Start(void);
void SW_I2C_Stop(void);
uint8_t SW_I2C_TransmitByte(uint8_t Byte);

#endif // __SW_I2C_H
