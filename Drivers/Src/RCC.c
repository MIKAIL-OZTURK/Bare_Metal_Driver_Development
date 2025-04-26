#include "RCC.h"

const uint8_t AHB_Prescalers[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APB_Prescalers[] = {0, 0, 0, 0, 1, 2, 3, 4};



/*
 * @brief	RCC_GetSystemClock, find system core clock
 *
 * @retval	uint32_t
 */
uint32_t RCC_GetSystemClock(void)
{
	uint32_t SystemCoreClock = 0;
	uint8_t clkSource = 0;

	clkSource = ((RCC->CFGR >> 2U) & 0X3U);

	switch(clkSource)
	{
		case 0:  SystemCoreClock = 16000000;	break;
		case 1:  SystemCoreClock = 8000000;		break;
		default: SystemCoreClock = 16000000;
	}
	return SystemCoreClock;
}



/*
 * @brief	RCC_GetHClock is find AHB clock
 *
 * @retval	uint32_t
 */
uint32_t RCC_GetHClock(void)
{
	uint32_t AHB_PeriphClock = 0;
	uint32_t SystemCoreClock = 0;
	uint8_t HPRE_Value = 0;
	uint8_t tempValue = 0;

	SystemCoreClock = RCC_GetSystemClock();

	HPRE_Value = ((RCC->CFGR >> 4U) & 0xFU);
	tempValue = AHB_Prescalers[HPRE_Value];
	AHB_PeriphClock = (SystemCoreClock >> tempValue);

	return AHB_PeriphClock;
}



/*
 * @brief	RCC_GetPClock1 is find APB1 clock
 *
 * @retval	uint32_t
 */
uint32_t RCC_GetPClock1(void)
{
	uint32_t APB1_PeriphClock = 0;
	uint32_t Hclock = 0;
	uint8_t HPRE1_Value = 0;
	uint8_t tempValue = 0;

	Hclock = RCC_GetHClock();
	HPRE1_Value = ((RCC->CFGR >> 10U) & 0x7U);
	tempValue = APB_Prescalers[HPRE1_Value];
	APB1_PeriphClock = (Hclock >> tempValue);

	return APB1_PeriphClock;
}



/*
 * @brief	RCC_GetPClock2 is find APB2 clock
 *
 * @retval	uint32_t
 */
uint32_t RCC_GetPClock2(void)
{
	uint32_t APB2_PeriphClock = 0;
	uint32_t Hclock = 0;
	uint8_t HPRE2_Value = 0;
	uint8_t tempValue = 0;

	Hclock = RCC_GetHClock();
	HPRE2_Value = ((RCC->CFGR >> 13U) & 0x7U);
	tempValue = APB_Prescalers[HPRE2_Value];
	APB2_PeriphClock = (Hclock >> tempValue);

	return APB2_PeriphClock;
}
