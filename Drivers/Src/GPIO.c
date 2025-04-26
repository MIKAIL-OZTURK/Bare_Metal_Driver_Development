#include "GPIO.h"



/*
 * @brief	GPIO_Init, configures the port and pin
 *
 * @param	GPIOx 				= GPIO Port Base Address
 * @param	GPIO_ConfigStruct 	= User Config Structures
 *
 * @retval	void
 */
void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct)
{
	uint32_t position;
	uint32_t fakePosition = 0x00U;
	uint32_t lastPosition = 0x00U;
	uint32_t tempValue = 0x00U;

	for(position = 0U; position < GPIO_MAX_PIN_NUMBER; position++)
	{
		fakePosition = (0x1U << position);
		lastPosition = ((uint32_t)(GPIO_ConfigStruct->pinNumber) & fakePosition);

		if(fakePosition == lastPosition)
		{
			/* MODE CONFIG */
			tempValue = GPIOx->MODER;
			tempValue &= ~(0x3U << (position * 2));
			tempValue |= (GPIO_ConfigStruct->Mode << (position * 2));
			GPIOx->MODER = tempValue;

			if(GPIO_ConfigStruct->Mode == GPIO_MODE_OUTPUT || GPIO_ConfigStruct->Mode == GPIO_MODE_AF)
			{
				/* Output Type CONFIG */
				tempValue = GPIOx->OTYPER;
				tempValue &= ~(0x1U << position);
				tempValue |= (GPIO_ConfigStruct->Otype << position);
				GPIOx->OTYPER = tempValue;

				/* Output Speed CONFIG */
				tempValue = GPIOx->OSPEEDR;
				tempValue &= ~(0x3U << (position * 2));
				tempValue |= (GPIO_ConfigStruct->Speed << (position * 2));
				GPIOx->OSPEEDR = tempValue;
			}
			/* Push Pull CONFIG */
			tempValue = GPIOx->PUPDR;
			tempValue &= ~(0x3U << (position * 2));
			tempValue |= (GPIO_ConfigStruct->PuPd << (position * 2));
			GPIOx->PUPDR = tempValue;

			/* Alternate Mode CONFIG */
			if(GPIO_ConfigStruct->Mode == GPIO_MODE_AF)
			{
				tempValue = GPIOx->AFR[position >> 3U];
				tempValue &= ~(0xFU << ((position & 0x7U) * 4));
				tempValue |= (GPIO_ConfigStruct->Alternate << ((position & 0x7U) * 4));
				GPIOx->AFR[position >> 3U] = tempValue;
			}
		}
	}
}



/*
 * @brief	GPIO_ReadPin, reads the pin of GPIOx Port
 *
 * @param	GPIOx 		= GPIO Port Base Address
 * @param	pinNumber 	= GPIO Pin Numbers 0 - 15
 *
 * @retval	GPIO_PinState_t
 */
GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	GPIO_PinState_t bitStatus = GPIO_PIN_RESET;

	if((GPIOx->IDR & pinNumber) != GPIO_PIN_RESET)
	{
		bitStatus = GPIO_PIN_SET;
	}
	else
	{
		bitStatus = GPIO_PIN_RESET;
	}
	return bitStatus;
}



/*
 * @brief	GPIO_WritePin, makes pin high or Low
 *
 * @param	GPIOx 		= GPIO Port Base Address
 * @param	pinNumber 	= GPIO Pin Numbers 0 - 15
 * @param	pinState 	= GPIO_Pin_Set or GPIO_Pin_Reset
 *
 * @retval	void
 */
void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber, GPIO_PinState_t pinState)
{
	if(pinState == GPIO_PIN_SET)
	{
		GPIOx->BSRR = pinNumber;
	}
	else
	{
		GPIOx->BSRR = (pinNumber << 16U);
	}
}



/*
 * @brief	GPIO_TogglePin, toggles the pin of GPIOx Port
 *
 * @param	GPIOx 		= GPIO Port Base Address
 * @param	pinNumber 	= GPIO Pin Numbers 0 - 15
 *
 * @retval	void
 */
void GPIO_TogglePin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber)
{
	uint32_t tempODRRegister = GPIOx->ODR;
	GPIOx->BSRR = ((tempODRRegister & pinNumber ) << 16U ) | (~tempODRRegister & pinNumber);
}



/*
 * @brief	GPIO_LockPin, lock the pin of GPIOx Port
 *
 * @param	GPIOx 		= GPIO Port Base Address
 * @param	pinNumber 	= GPIO Pin Numbers 0 - 15
 *
 * @retval	void
 */
void GPIO_LockPin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber)
{
	uint32_t tempValue = (0x1 << 16U) | pinNumber;
	GPIOx->LCKR = tempValue;
	GPIOx->LCKR = pinNumber;
	GPIOx->LCKR = tempValue;
	tempValue = GPIOx->LCKR;
}
