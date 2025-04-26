#ifndef INC_USART_H_
#define INC_USART_H_

#include "STM32F407xx.h"

/* @def_group MODE_Types */
#define USART_MODE_Tx					( (uint32_t)(0x00000008) )
#define USART_MODE_Rx					( (uint32_t)(0x00000004) )
#define USART_MODE_Tx_Rx				( (uint32_t)(0x0000000C) )

/* @def_group WordLenght_Types */
#define USART_WORDLENGHT_8Bits			( (uint32_t)(0x00000000) )
#define USART_WORDLENGHT_9Bits			( (uint32_t)(0x00001000) )

/* @def_group Parity_Modes */
#define USART_PARITY_NONE				( (uint32_t)(0x00000000) )
#define USART_PARITY_Even				( (uint32_t)(0x00000400) )
#define USART_PARITY_Odd				( (uint32_t)(0x00000600) )

/* @def_group Stop_Bits */
#define USART_STOPBITS_1				( (uint32_t)(0x00000000) )
#define USART_STOPBITS_Half				( (uint32_t)(0x00001000) )
#define USART_STOPBITS_2				( (uint32_t)(0x00002000) )
#define USART_STOPBITS_1_Half			( (uint32_t)(0x00003000) )

/* @def_group OverSampling_Modes */
#define USART_OVERSAMPLE_16				( (uint32_t)(0x00000000) )
#define USART_OVERSAMPLE_8				( (uint32_t)(0x00008000) )

/* @def_group HardWareFlowControl_Modes */
#define USART_HW_NONE					( (uint32_t)(0x00000000) )
#define USART_HW_CTS					( (uint32_t)(0x00000200) )
#define USART_HW_RTS					( (uint32_t)(0x00000100) )
#define USART_HW_CTS_RTS				( (uint32_t)(0x00000300) )

typedef enum
{
	USART_FLAG_RESET = 0x0U,
	USART_FLAG_SET = !USART_FLAG_RESET

}USART_FlagStatus_t;


#define __USART_DIV_VALUE_8(__CLOCK__, __BAUDRATE__)		( (25U * (uint32_t)(__CLOCK__)) / (4U * (uint32_t)(__BAUDRATE__)) )
#define __USART_DIV_VALUE_16(__CLOCK__, __BAUDRATE__)		( (25U * (uint32_t)(__CLOCK__)) / (2U * (uint32_t)(__BAUDRATE__)) )

typedef enum
{
	USART_BUS_FREE = 0x0U,
	USART_BUS_Tx = 0x1U,
	USART_BUS_Rx = 0x2U

}USART_BusState_t;


typedef struct
{
	uint32_t Mode;						/*!< Transmission and Reception Modes @def_group MODE_Types 		*/
	uint32_t BaudRate;					/*!< User Value for UART Baudrate							 		*/
	uint32_t WorldLenght;				/*!< 8 Bits & 9 Bits  Modes @def_group WordLenght_Types 			*/
	uint32_t Parity;					/*!< Even & Odd Parity Modes @def_group Parity_Modes 				*/
	uint32_t StopBits;					/*!< Stop Bits Modes @def_group Stop_Bits 							*/
	uint32_t OverSampling;				/*!< OverSampling Modes @def_group OverSampling_Modes 				*/
	uint32_t HardWareFlowControl;		/*!< HardWareFlowControl Modes @def_group HardWareFlowControl_Modes */

}USART_InitTypedef_t;

typedef struct __USART_HandleTypedef_t
{
	USART_TypeDef_t *Instance;
	USART_InitTypedef_t Init;
	uint8_t *pTxBuffer;
	uint16_t TxBufferSize;
	uint8_t TxStatus;
	void (*TxISR_Function)(struct __USART_HandleTypedef_t *USART_Handle);
	uint8_t *pRxBuffer;
	uint16_t RxBufferSize;
	uint8_t RxStatus;
	void(*RxISR_Function)(struct __USART_HandleTypedef_t *USART_Handle);

}USART_HandleTypedef_t;

void USART_Init(USART_HandleTypedef_t *USART_Handle);
void USART_TransmitData(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize);
void USART_ReceiveData(USART_HandleTypedef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize);
void USART_TransmitData_IT(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize);
void USART_ReceiveData_IT(USART_HandleTypedef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize);
void USART_PeriphCmd(USART_HandleTypedef_t *USART_Handle, FunctionalState_t stateOfUSART);
USART_FlagStatus_t USART_GetFlagStatus(USART_HandleTypedef_t *USART_Handle, uint16_t flagName);
void USART_InterruptHandler(USART_HandleTypedef_t *USART_Handle);


#endif /* INC_USART_H_ */
