#include "STM32F407xx.h"

GPIO_InitTypeDef_t GPIO_InitStruct;


static void GPIO_Config(void);




int main(void)
{
	GPIO_Config();
	USART_Config();

	while(1)
	{
		GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	}
}




static void GPIO_Config(void)
{
	RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
	GPIO_InitStruct.pinNumber = GPIO_PIN_12;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_VERY;

	GPIO_Init(GPIOD, &GPIO_InitStruct);
}
