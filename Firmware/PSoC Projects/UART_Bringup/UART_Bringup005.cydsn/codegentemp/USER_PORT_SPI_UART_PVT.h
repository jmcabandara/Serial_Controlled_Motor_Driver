/*******************************************************************************
* File Name: USER_PORT_SPI_UART_PVT.h
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

#if !defined(CY_SCB_SPI_UART_PVT_USER_PORT_H)
#define CY_SCB_SPI_UART_PVT_USER_PORT_H

#include "USER_PORT_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if(USER_PORT_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  USER_PORT_rxBufferHead;
    extern volatile uint32  USER_PORT_rxBufferTail;
    extern volatile uint8   USER_PORT_rxBufferOverflow;
#endif /* (USER_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

#if(USER_PORT_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  USER_PORT_txBufferHead;
    extern volatile uint32  USER_PORT_txBufferTail;
#endif /* (USER_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

#if(USER_PORT_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 USER_PORT_rxBufferInternal[USER_PORT_RX_BUFFER_SIZE];
#endif /* (USER_PORT_INTERNAL_RX_SW_BUFFER) */

#if(USER_PORT_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 USER_PORT_txBufferInternal[USER_PORT_TX_BUFFER_SIZE];
#endif /* (USER_PORT_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(USER_PORT_SCB_MODE_SPI_CONST_CFG)
    void USER_PORT_SpiInit(void);
#endif /* (USER_PORT_SCB_MODE_SPI_CONST_CFG) */

#if(USER_PORT_SPI_WAKE_ENABLE_CONST)
    void USER_PORT_SpiSaveConfig(void);
    void USER_PORT_SpiRestoreConfig(void);
#endif /* (USER_PORT_SPI_WAKE_ENABLE_CONST) */

#if(USER_PORT_SCB_MODE_UART_CONST_CFG)
    void USER_PORT_UartInit(void);
#endif /* (USER_PORT_SCB_MODE_UART_CONST_CFG) */

#if(USER_PORT_UART_WAKE_ENABLE_CONST)
    void USER_PORT_UartSaveConfig(void);
    void USER_PORT_UartRestoreConfig(void);
    #define USER_PORT_UartStop() \
        do{                             \
            USER_PORT_UART_RX_CTRL_REG &= ~USER_PORT_UART_RX_CTRL_SKIP_START; \
        }while(0)
#else
        #define USER_PORT_UartStop() do{ /* Does nothing */ }while(0)

#endif /* (USER_PORT_UART_WAKE_ENABLE_CONST) */

/* Interrupt processing */
#define USER_PORT_SpiUartEnableIntRx(intSourceMask)  USER_PORT_SetRxInterruptMode(intSourceMask)
#define USER_PORT_SpiUartEnableIntTx(intSourceMask)  USER_PORT_SetTxInterruptMode(intSourceMask)
uint32  USER_PORT_SpiUartDisableIntRx(void);
uint32  USER_PORT_SpiUartDisableIntTx(void);


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in USER_PORT_SetPins() */
#define USER_PORT_UART_RX_PIN_ENABLE    (USER_PORT_UART_RX)
#define USER_PORT_UART_TX_PIN_ENABLE    (USER_PORT_UART_TX)

/* UART RTS and CTS position to be used in  USER_PORT_SetPins() */
#define USER_PORT_UART_RTS_PIN_ENABLE    (0x10u)
#define USER_PORT_UART_CTS_PIN_ENABLE    (0x20u)

#endif /* (CY_SCB_SPI_UART_PVT_USER_PORT_H) */


/* [] END OF FILE */
