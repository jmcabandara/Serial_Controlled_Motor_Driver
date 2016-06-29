/*******************************************************************************
* File Name: EXPANSION_PORT_SPI_UART_PVT.h
* Version 2.0
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_EXPANSION_PORT_H)
#define CY_SCB_SPI_UART_PVT_EXPANSION_PORT_H

#include "EXPANSION_PORT_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  EXPANSION_PORT_rxBufferHead;
    extern volatile uint32  EXPANSION_PORT_rxBufferTail;
    extern volatile uint8   EXPANSION_PORT_rxBufferOverflow;
#endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

#if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  EXPANSION_PORT_txBufferHead;
    extern volatile uint32  EXPANSION_PORT_txBufferTail;
#endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

#if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 EXPANSION_PORT_rxBufferInternal[EXPANSION_PORT_RX_BUFFER_SIZE];
#endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER) */

#if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 EXPANSION_PORT_txBufferInternal[EXPANSION_PORT_TX_BUFFER_SIZE];
#endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG)
    void EXPANSION_PORT_SpiInit(void);
#endif /* (EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG) */

#if(EXPANSION_PORT_SPI_WAKE_ENABLE_CONST)
    void EXPANSION_PORT_SpiSaveConfig(void);
    void EXPANSION_PORT_SpiRestoreConfig(void);
#endif /* (EXPANSION_PORT_SPI_WAKE_ENABLE_CONST) */

#if(EXPANSION_PORT_SCB_MODE_UART_CONST_CFG)
    void EXPANSION_PORT_UartInit(void);
#endif /* (EXPANSION_PORT_SCB_MODE_UART_CONST_CFG) */

#if(EXPANSION_PORT_UART_WAKE_ENABLE_CONST)
    void EXPANSION_PORT_UartSaveConfig(void);
    void EXPANSION_PORT_UartRestoreConfig(void);
    #define EXPANSION_PORT_UartStop() \
        do{                             \
            EXPANSION_PORT_UART_RX_CTRL_REG &= ~EXPANSION_PORT_UART_RX_CTRL_SKIP_START; \
        }while(0)
#else
        #define EXPANSION_PORT_UartStop() do{ /* Does nothing */ }while(0)

#endif /* (EXPANSION_PORT_UART_WAKE_ENABLE_CONST) */

/* Interrupt processing */
#define EXPANSION_PORT_SpiUartEnableIntRx(intSourceMask)  EXPANSION_PORT_SetRxInterruptMode(intSourceMask)
#define EXPANSION_PORT_SpiUartEnableIntTx(intSourceMask)  EXPANSION_PORT_SetTxInterruptMode(intSourceMask)
uint32  EXPANSION_PORT_SpiUartDisableIntRx(void);
uint32  EXPANSION_PORT_SpiUartDisableIntTx(void);


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in EXPANSION_PORT_SetPins() */
#define EXPANSION_PORT_UART_RX_PIN_ENABLE    (EXPANSION_PORT_UART_RX)
#define EXPANSION_PORT_UART_TX_PIN_ENABLE    (EXPANSION_PORT_UART_TX)

/* UART RTS and CTS position to be used in  EXPANSION_PORT_SetPins() */
#define EXPANSION_PORT_UART_RTS_PIN_ENABLE    (0x10u)
#define EXPANSION_PORT_UART_CTS_PIN_ENABLE    (0x20u)

#endif /* (CY_SCB_SPI_UART_PVT_EXPANSION_PORT_H) */


/* [] END OF FILE */
