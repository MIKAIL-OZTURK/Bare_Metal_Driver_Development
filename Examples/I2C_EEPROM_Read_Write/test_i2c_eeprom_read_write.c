#include "STM32F407xx.h"

#define EEPROM_ADDR  0x50  // 24C02 EEPROM I2C address (A0, A1, A2 -> GND)

I2C_HandleTypeDef_t hi2c1;

static void I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = I2C_SPEED_Standart;
    hi2c1.Init.ACK_State = I2C_ACK_ENABLE;
    hi2c1.Init.ClockStretch = I2C_STRETCH_ENABLE;
    hi2c1.Init.AddressingMode = I2C_ADDRMODE_7;
    hi2c1.Init.DutyCycle = I2C_DUTY_Standart;
    hi2c1.Init.MyOwnAddress = 0x61;

    I2C_Init(&hi2c1);
    I2C_PeriphCmd(hi2c1.Instance, ENABLE);
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

void EEPROM_WriteByte(uint8_t memAddress, uint8_t data)
{
    uint8_t buffer[2];
    buffer[0] = memAddress;  
    buffer[1] = data;        

    I2C_Master_Transmit(&hi2c1, EEPROM_ADDR, buffer, 2);

    for (volatile int i = 0; i < 50000; i++); 
}

uint8_t EEPROM_ReadByte(uint8_t memAddress)
{
    uint8_t data;

    I2C_Master_Transmit(&hi2c1, EEPROM_ADDR, &memAddress, 1);
    I2C_Master_Receive(&hi2c1, EEPROM_ADDR, &data, 1);

    return data;
}

int main(void)
{
    I2C1_Init();

    uint8_t myData = 0xAB;
    EEPROM_WriteByte(0x10, myData);

    uint8_t receivedData = EEPROM_ReadByte(0x10);

    while (1)
    {
        if (receivedData == myData)
        {
            GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        }
    }
}
