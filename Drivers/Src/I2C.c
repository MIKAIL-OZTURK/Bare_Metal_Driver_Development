#include "I2C.h"


/**
 * @brief  Generates a START condition on the I2C bus
 * @param  I2Cx: Pointer to I2C peripheral
 * @retval None
 */
void I2C_GenerateStart(I2C_TypeDef_t *I2Cx)
{
	I2Cx->CR1 |= (1U << 8);	// START bit set

	// Wait until SB (Start Bit) flag is set
	while (!(I2Cx->SR1 & (1U << 0)));
}




/**
 * @brief  Enables or disables the I2C peripheral
 * @param  I2Cx: Pointer to I2C peripheral (I2C1, I2C2, I2C3)
 * @param  stateOfI2C: ENABLE or DISABLE
 * @retval None
 */
void I2C_PeriphCmd(I2C_TypeDef_t *I2Cx, FunctionalState_t stateOfI2C)
{
	if (stateOfI2C == ENABLE)
	{
		I2Cx->CR1 |= (1U << 0);		// PE bit -> Peripheral Enable
	}
	else
	{
		I2Cx->CR1 &= ~(1U << 0);	// Peripheral Disable
	}
}


/**
 * @brief  Initializes the I2C peripheral according to the specified parameters
 *         in the I2C_HandleTypeDef_t.
 * @param  I2C_Handle: pointer to a I2C_HandleTypeDef_t structure
 * @retval None
 */
void I2C_Init(I2C_HandleTypeDef_t *I2C_Handle)
{
	uint32_t tempReg = 0;

	// 1. Peripheral'i Disable et (konfigürasyon yapabilmek için)
	I2C_PeriphCmd(I2C_Handle->Instance, DISABLE);

	// 2. CR2 Register'ına APB1 Clock Frequency değerini yaz (MHz cinsinden)
	tempReg = I2C_Handle->Instance->CR2;
	tempReg &= ~(0x3F);	// FREQ[5:0] bitlerini temizle
	tempReg |= (I2C_Get_FreqValue(SystemCoreClock/4) & 0x3F); // APB1 Clock -> SystemCoreClock/4
	I2C_Handle->Instance->CR2 = tempReg;

	// 3. Own Address Register (OAR1) ayarla
	tempReg = 0;
	tempReg |= (I2C_Handle->Init.MyOwnAddress << 1);	// Slave adresi sol kaydırılmalı
	tempReg |= (1U << 14);	// Bit14 -> Should always be kept at 1 in 7-bit addressing
	I2C_Handle->Instance->OAR1 = tempReg;

	// 4. CCR Register - Clock Control ayarları
	uint16_t ccrValue = 0;
	tempReg = 0;

	if (I2C_Handle->Init.ClockSpeed <= I2C_SPEED_Standart)
	{
		// Standard Mode
		ccrValue = (SystemCoreClock/4) / (I2C_Handle->Init.ClockSpeed << 1);
		tempReg |= (ccrValue & 0xFFF);
	}
	else
	{
		// Fast Mode
		tempReg |= (1U << 15);	// F/S bit -> Fast Mode
		tempReg |= (I2C_Handle->Init.DutyCycle); // Duty Cycle seçimi

		if (I2C_Handle->Init.DutyCycle == I2C_DUTY_FM_2)
		{
			ccrValue = (SystemCoreClock/4) / (I2C_Handle->Init.ClockSpeed * 3);
		}
		else
		{
			ccrValue = (SystemCoreClock/4) / (I2C_Handle->Init.ClockSpeed * 25);
		}
		tempReg |= (ccrValue & 0xFFF);
	}
	I2C_Handle->Instance->CCR = tempReg;

	// 5. TRISE Register ayarlanması (Rise time)
	if (I2C_Handle->Init.ClockSpeed <= I2C_SPEED_Standart)
	{
		// Standard Mode
		I2C_Handle->Instance->TRISE = (I2C_Get_FreqValue(SystemCoreClock/4)) + 1;
	}
	else
	{
		// Fast Mode
		I2C_Handle->Instance->TRISE = ( (I2C_Get_FreqValue(SystemCoreClock/4)) * 300 / 1000 ) + 1;
	}

	// 6. CR1 Register ayarları (ACK ve Stretching)
	tempReg = I2C_Handle->Instance->CR1;
	tempReg |= (I2C_Handle->Init.ACK_State);
	tempReg |= (I2C_Handle->Init.ClockStretch);
	I2C_Handle->Instance->CR1 = tempReg;

	// 7. I2C Peripheral Enable
	I2C_PeriphCmd(I2C_Handle->Instance, ENABLE);
}

/**
 * @brief  Sends slave address over I2C bus
 * @param  I2Cx: Pointer to I2C peripheral
 * @param  address: 7-bit slave address
 * @param  direction: 0 for write, 1 for read
 * @retval None
 */
void I2C_SendAddress(I2C_TypeDef_t *I2Cx, uint8_t address, uint8_t direction)
{
	uint8_t addr = (address << 1);

	if (direction == 1)
	{
		addr |= 1U; // Read Mode
	}

	I2Cx->DR = addr;

	// Wait until ADDR flag is set
	while (!(I2Cx->SR1 & (1U << 1)));

	// Read SR1 and SR2 to clear ADDR flag
	(void)I2Cx->SR1;
	(void)I2Cx->SR2;
}

/**
 * @brief  Writes single byte to the I2C bus
 * @param  I2Cx: Pointer to I2C peripheral
 * @param  data: byte to send
 * @retval None
 */
void I2C_WriteData(I2C_TypeDef_t *I2Cx, uint8_t data)
{
	I2Cx->DR = data;

	// Wait until Byte Transfer Finished (BTF) or TxE flag is set
	while (!(I2Cx->SR1 & (1U << 7))); // TxE flag
}

/**
 * @brief  Reads single byte from the I2C bus
 * @param  I2Cx: Pointer to I2C peripheral
 * @retval Received byte
 */
uint8_t I2C_ReadData(I2C_TypeDef_t *I2Cx)
{
	// Wait until RxNE (Data Register Not Empty)
	while (!(I2Cx->SR1 & (1U << 6)));

	return (uint8_t)(I2Cx->DR);
}


/**
 * @brief  Generates a STOP condition on the I2C bus
 * @param  I2Cx: Pointer to I2C peripheral
 * @retval None
 */
void I2C_GenerateStop(I2C_TypeDef_t *I2Cx)
{
	I2Cx->CR1 |= (1U << 9);	// STOP bit set
}


/**
 * @brief  Master mode - Transmit data to I2C slave
 * @param  I2C_Handle: Pointer to I2C handle
 * @param  pData: Data buffer to transmit
 * @param  size: Number of bytes to transmit
 * @retval None
 */
void I2C_Master_Transmit(I2C_HandleTypeDef_t *I2C_Handle, uint8_t slaveAddr, uint8_t *pData, uint16_t size)
{
	I2C_GenerateStart(I2C_Handle->Instance);

	I2C_SendAddress(I2C_Handle->Instance, slaveAddr, 0); // 0 -> Write

	for (uint16_t i = 0; i < size; i++)
	{
		I2C_WriteData(I2C_Handle->Instance, pData[i]);
	}

	I2C_GenerateStop(I2C_Handle->Instance);
}


/**
 * @brief  Master mode - Receive data from I2C slave
 * @param  I2C_Handle: Pointer to I2C handle
 * @param  pData: Buffer to store received data
 * @param  size: Number of bytes to receive
 * @retval None
 */
void I2C_Master_Receive(I2C_HandleTypeDef_t *I2C_Handle, uint8_t slaveAddr, uint8_t *pData, uint16_t size)
{
	I2C_GenerateStart(I2C_Handle->Instance);

	I2C_SendAddress(I2C_Handle->Instance, slaveAddr, 1); // 1 -> Read

	for (uint16_t i = 0; i < size; i++)
	{
		pData[i] = I2C_ReadData(I2C_Handle->Instance);
	}

	I2C_GenerateStop(I2C_Handle->Instance);
}
