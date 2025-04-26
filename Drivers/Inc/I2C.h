#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "STM32F407xx.h"

/* Clock Speeds */
#define I2C_SPEED_Standart    (100000U)
#define I2C_SPEED_FastMode    (400000U)

/* ACK State */
#define I2C_ACK_DISABLE       (0x00000000U)
#define I2C_ACK_ENABLE        (0x00000400U)

/* Clock Stretching */
#define I2C_STRETCH_ENABLE    (0x00000000U)
#define I2C_STRETCH_DISABLE   (0x00000080U)

/* Address Modes */
#define I2C_ADDRMODE_7        (0x00004000U)
#define I2C_ADDRMODE_10       (0x0000C000U)

/* Duty Cycle Modes */
#define I2C_DUTY_Standart     (0x00000000U)
#define I2C_DUTY_FM_2         (0x00008000U)
#define I2C_DUTY_FM_16_9      (0x0000C000U)

/* Structures */
typedef struct
{
    uint32_t ClockSpeed;
    uint32_t ACK_State;
    uint32_t ClockStretch;
    uint32_t AddressingMode;
    uint32_t DutyCycle;
    uint32_t MyOwnAddress;
} I2C_InitTypeDef_t;

typedef struct
{
    I2C_TypeDef_t *Instance;
    I2C_InitTypeDef_t Init;
} I2C_HandleTypeDef_t;

/* Function Prototypes */
void I2C_Init(I2C_HandleTypeDef_t *I2C_Handle);
void I2C_PeriphCmd(I2C_TypeDef_t *I2Cx, FunctionalState_t stateOfI2C);

void I2C_Master_Transmit(I2C_HandleTypeDef_t *I2C_Handle, uint8_t slaveAddr, uint8_t *pData, uint16_t size);
void I2C_Master_Receive(I2C_HandleTypeDef_t *I2C_Handle, uint8_t slaveAddr, uint8_t *pData, uint16_t size);

#endif /* INC_I2C_H_ */
