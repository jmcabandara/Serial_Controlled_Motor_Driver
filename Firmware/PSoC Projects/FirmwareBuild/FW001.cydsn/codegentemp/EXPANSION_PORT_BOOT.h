/*******************************************************************************
* File Name: EXPANSION_PORT_BOOT.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the bootloader
*  communication interface of SCB component.
*
* Note:
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_EXPANSION_PORT_H)
#define CY_SCB_BOOT_EXPANSION_PORT_H

#include "EXPANSION_PORT_PVT.h"

#if (EXPANSION_PORT_SCB_MODE_I2C_INC)
    #include "EXPANSION_PORT_I2C.h"
#endif /* (EXPANSION_PORT_SCB_MODE_I2C_INC) */

#if (EXPANSION_PORT_SCB_MODE_EZI2C_INC)
    #include "EXPANSION_PORT_EZI2C.h"
#endif /* (EXPANSION_PORT_SCB_MODE_EZI2C_INC) */

#if (EXPANSION_PORT_SCB_MODE_SPI_INC || EXPANSION_PORT_SCB_MODE_UART_INC)
    #include "EXPANSION_PORT_SPI_UART.h"
#endif /* (EXPANSION_PORT_SCB_MODE_SPI_INC || EXPANSION_PORT_SCB_MODE_UART_INC) */


/***************************************
*        Function Prototypes
***************************************/

/* Bootloader communication interface enable */
#define EXPANSION_PORT_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_EXPANSION_PORT) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

#if (EXPANSION_PORT_SCB_MODE_I2C_INC)

    #define EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED     (EXPANSION_PORT_BTLDR_COMM_ENABLED && \
                                                            (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             EXPANSION_PORT_I2C_SLAVE_CONST))

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void EXPANSION_PORT_I2CCyBtldrCommStart(void);
    void EXPANSION_PORT_I2CCyBtldrCommStop (void);
    void EXPANSION_PORT_I2CCyBtldrCommReset(void);
    cystatus EXPANSION_PORT_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus EXPANSION_PORT_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Size of Read/Write buffers for I2C bootloader  */
    #define EXPANSION_PORT_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
    #define EXPANSION_PORT_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)
    #define EXPANSION_PORT_I2C_MIN_UINT16(a, b)           ( ((uint16)(a) < (uint16) (b)) ? \
                                                                    ((uint32) (a)) : ((uint32) (b)) )
    #define EXPANSION_PORT_WAIT_1_MS                      (1u)
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED) */

#endif /* (EXPANSION_PORT_SCB_MODE_I2C_INC) */


#if (EXPANSION_PORT_SCB_MODE_EZI2C_INC)

    /* Provide EMPTY bootloader communication functions. EZI2C is NOT supported yet */
    #define EXPANSION_PORT_EZI2C_BTLDR_COMM_ENABLED   (EXPANSION_PORT_BTLDR_COMM_ENABLED && \
                                                         EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void EXPANSION_PORT_EzI2CCyBtldrCommStart(void);
    void EXPANSION_PORT_EzI2CCyBtldrCommStop (void);
    void EXPANSION_PORT_EzI2CCyBtldrCommReset(void);
    cystatus EXPANSION_PORT_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus EXPANSION_PORT_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_EZI2C_BTLDR_COMM_ENABLED) */

#endif /* (EXPANSION_PORT_SCB_MODE_EZI2C_INC) */

#if (EXPANSION_PORT_SCB_MODE_SPI_INC || EXPANSION_PORT_SCB_MODE_UART_INC)
    /* Provide EMPTY bootloader communication functions. SPI and UART is NOT supported yet */
    #define EXPANSION_PORT_SPI_BTLDR_COMM_ENABLED     (EXPANSION_PORT_BTLDR_COMM_ENABLED && \
                                                        EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    #define EXPANSION_PORT_UART_BTLDR_COMM_ENABLED    (EXPANSION_PORT_BTLDR_COMM_ENABLED && \
                                                        EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void EXPANSION_PORT_SpiCyBtldrCommStart(void);
    void EXPANSION_PORT_SpiCyBtldrCommStop (void);
    void EXPANSION_PORT_SpiCyBtldrCommReset(void);
    cystatus EXPANSION_PORT_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus EXPANSION_PORT_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void EXPANSION_PORT_UartCyBtldrCommStart(void);
    void EXPANSION_PORT_UartCyBtldrCommStop (void);
    void EXPANSION_PORT_UartCyBtldrCommReset(void);
    cystatus EXPANSION_PORT_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus EXPANSION_PORT_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_UART_BTLDR_COMM_ENABLED) */

#endif /* (EXPANSION_PORT_SCB_MODE_SPI_INC || EXPANSION_PORT_SCB_MODE_UART_INC) */

#if !defined (EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED)
    #define EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED     (0u)
#endif /* (EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED) */

#if !defined (EXPANSION_PORT_EZI2C_BTLDR_COMM_ENABLED)
    #define EXPANSION_PORT_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (EXPANSION_PORT_EZI2C_BTLDR_COMM_ENABLED) */

#if !defined (EXPANSION_PORT_SPI_BTLDR_COMM_ENABLED)
    #define EXPANSION_PORT_SPI_BTLDR_COMM_ENABLED     (0u)
#endif /* (EXPANSION_PORT_SPI_BTLDR_COMM_ENABLED) */

#if !defined (EXPANSION_PORT_UART_BTLDR_COMM_ENABLED)
    #define EXPANSION_PORT_UART_BTLDR_COMM_ENABLED    (0u)
#endif /* (EXPANSION_PORT_UART_BTLDR_COMM_ENABLED) */

/* Bootloader enabled condition for each mode */
#define EXPANSION_PORT_BTLDR_COMM_MODE_ENABLED    (EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED   || \
                                                     EXPANSION_PORT_EZI2C_BTLDR_COMM_ENABLED || \
                                                     EXPANSION_PORT_SPI_BTLDR_COMM_ENABLED   || \
                                                     EXPANSION_PORT_UART_BTLDR_COMM_ENABLED)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_BTLDR_COMM_ENABLED)
    #if (EXPANSION_PORT_BTLDR_COMM_MODE_ENABLED)
        /* Bootloader physical layer functions */
        void EXPANSION_PORT_CyBtldrCommStart(void);
        void EXPANSION_PORT_CyBtldrCommStop (void);
        void EXPANSION_PORT_CyBtldrCommReset(void);
        cystatus EXPANSION_PORT_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus EXPANSION_PORT_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (EXPANSION_PORT_BTLDR_COMM_MODE_ENABLED) */

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_EXPANSION_PORT)
        #define CyBtldrCommStart    EXPANSION_PORT_CyBtldrCommStart
        #define CyBtldrCommStop     EXPANSION_PORT_CyBtldrCommStop
        #define CyBtldrCommReset    EXPANSION_PORT_CyBtldrCommReset
        #define CyBtldrCommWrite    EXPANSION_PORT_CyBtldrCommWrite
        #define CyBtldrCommRead     EXPANSION_PORT_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_EXPANSION_PORT) */
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_BTLDR_COMM_ENABLED) */

#endif /* (CY_SCB_BOOT_EXPANSION_PORT_H) */

/* [] END OF FILE */
