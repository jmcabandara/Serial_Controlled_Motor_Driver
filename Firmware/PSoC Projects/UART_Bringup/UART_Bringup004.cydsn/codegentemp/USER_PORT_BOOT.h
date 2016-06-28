/*******************************************************************************
* File Name: USER_PORT_BOOT.h
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

#if !defined(CY_SCB_BOOT_USER_PORT_H)
#define CY_SCB_BOOT_USER_PORT_H

#include "USER_PORT_PVT.h"

#if (USER_PORT_SCB_MODE_I2C_INC)
    #include "USER_PORT_I2C.h"
#endif /* (USER_PORT_SCB_MODE_I2C_INC) */

#if (USER_PORT_SCB_MODE_EZI2C_INC)
    #include "USER_PORT_EZI2C.h"
#endif /* (USER_PORT_SCB_MODE_EZI2C_INC) */

#if (USER_PORT_SCB_MODE_SPI_INC || USER_PORT_SCB_MODE_UART_INC)
    #include "USER_PORT_SPI_UART.h"
#endif /* (USER_PORT_SCB_MODE_SPI_INC || USER_PORT_SCB_MODE_UART_INC) */


/***************************************
*        Function Prototypes
***************************************/

/* Bootloader communication interface enable */
#define USER_PORT_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USER_PORT) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

#if (USER_PORT_SCB_MODE_I2C_INC)

    #define USER_PORT_I2C_BTLDR_COMM_ENABLED     (USER_PORT_BTLDR_COMM_ENABLED && \
                                                            (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             USER_PORT_I2C_SLAVE_CONST))

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_I2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void USER_PORT_I2CCyBtldrCommStart(void);
    void USER_PORT_I2CCyBtldrCommStop (void);
    void USER_PORT_I2CCyBtldrCommReset(void);
    cystatus USER_PORT_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus USER_PORT_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Size of Read/Write buffers for I2C bootloader  */
    #define USER_PORT_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
    #define USER_PORT_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)
    #define USER_PORT_I2C_MIN_UINT16(a, b)           ( ((uint16)(a) < (uint16) (b)) ? \
                                                                    ((uint32) (a)) : ((uint32) (b)) )
    #define USER_PORT_WAIT_1_MS                      (1u)
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_I2C_BTLDR_COMM_ENABLED) */

#endif /* (USER_PORT_SCB_MODE_I2C_INC) */


#if (USER_PORT_SCB_MODE_EZI2C_INC)

    /* Provide EMPTY bootloader communication functions. EZI2C is NOT supported yet */
    #define USER_PORT_EZI2C_BTLDR_COMM_ENABLED   (USER_PORT_BTLDR_COMM_ENABLED && \
                                                         USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void USER_PORT_EzI2CCyBtldrCommStart(void);
    void USER_PORT_EzI2CCyBtldrCommStop (void);
    void USER_PORT_EzI2CCyBtldrCommReset(void);
    cystatus USER_PORT_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus USER_PORT_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_EZI2C_BTLDR_COMM_ENABLED) */

#endif /* (USER_PORT_SCB_MODE_EZI2C_INC) */

#if (USER_PORT_SCB_MODE_SPI_INC || USER_PORT_SCB_MODE_UART_INC)
    /* Provide EMPTY bootloader communication functions. SPI and UART is NOT supported yet */
    #define USER_PORT_SPI_BTLDR_COMM_ENABLED     (USER_PORT_BTLDR_COMM_ENABLED && \
                                                        USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    #define USER_PORT_UART_BTLDR_COMM_ENABLED    (USER_PORT_BTLDR_COMM_ENABLED && \
                                                        USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void USER_PORT_SpiCyBtldrCommStart(void);
    void USER_PORT_SpiCyBtldrCommStop (void);
    void USER_PORT_SpiCyBtldrCommReset(void);
    cystatus USER_PORT_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus USER_PORT_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void USER_PORT_UartCyBtldrCommStart(void);
    void USER_PORT_UartCyBtldrCommStop (void);
    void USER_PORT_UartCyBtldrCommReset(void);
    cystatus USER_PORT_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus USER_PORT_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_UART_BTLDR_COMM_ENABLED) */

#endif /* (USER_PORT_SCB_MODE_SPI_INC || USER_PORT_SCB_MODE_UART_INC) */

#if !defined (USER_PORT_I2C_BTLDR_COMM_ENABLED)
    #define USER_PORT_I2C_BTLDR_COMM_ENABLED     (0u)
#endif /* (USER_PORT_I2C_BTLDR_COMM_ENABLED) */

#if !defined (USER_PORT_EZI2C_BTLDR_COMM_ENABLED)
    #define USER_PORT_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (USER_PORT_EZI2C_BTLDR_COMM_ENABLED) */

#if !defined (USER_PORT_SPI_BTLDR_COMM_ENABLED)
    #define USER_PORT_SPI_BTLDR_COMM_ENABLED     (0u)
#endif /* (USER_PORT_SPI_BTLDR_COMM_ENABLED) */

#if !defined (USER_PORT_UART_BTLDR_COMM_ENABLED)
    #define USER_PORT_UART_BTLDR_COMM_ENABLED    (0u)
#endif /* (USER_PORT_UART_BTLDR_COMM_ENABLED) */

/* Bootloader enabled condition for each mode */
#define USER_PORT_BTLDR_COMM_MODE_ENABLED    (USER_PORT_I2C_BTLDR_COMM_ENABLED   || \
                                                     USER_PORT_EZI2C_BTLDR_COMM_ENABLED || \
                                                     USER_PORT_SPI_BTLDR_COMM_ENABLED   || \
                                                     USER_PORT_UART_BTLDR_COMM_ENABLED)

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_BTLDR_COMM_ENABLED)
    #if (USER_PORT_BTLDR_COMM_MODE_ENABLED)
        /* Bootloader physical layer functions */
        void USER_PORT_CyBtldrCommStart(void);
        void USER_PORT_CyBtldrCommStop (void);
        void USER_PORT_CyBtldrCommReset(void);
        cystatus USER_PORT_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus USER_PORT_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (USER_PORT_BTLDR_COMM_MODE_ENABLED) */

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USER_PORT)
        #define CyBtldrCommStart    USER_PORT_CyBtldrCommStart
        #define CyBtldrCommStop     USER_PORT_CyBtldrCommStop
        #define CyBtldrCommReset    USER_PORT_CyBtldrCommReset
        #define CyBtldrCommWrite    USER_PORT_CyBtldrCommWrite
        #define CyBtldrCommRead     USER_PORT_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_USER_PORT) */
#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_BTLDR_COMM_ENABLED) */

#endif /* (CY_SCB_BOOT_USER_PORT_H) */

/* [] END OF FILE */
