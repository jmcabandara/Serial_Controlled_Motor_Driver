/*******************************************************************************
* File Name: .h
* Version 2.0
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component.
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

#if !defined(CY_SCB_PVT_USER_PORT_H)
#define CY_SCB_PVT_USER_PORT_H

#include "USER_PORT.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define USER_PORT_SetI2CExtClkInterruptMode(interruptMask) USER_PORT_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define USER_PORT_ClearI2CExtClkInterruptSource(interruptMask) USER_PORT_CLEAR_INTR_I2C_EC(interruptMask)
#define USER_PORT_GetI2CExtClkInterruptSource()                (USER_PORT_INTR_I2C_EC_REG)
#define USER_PORT_GetI2CExtClkInterruptMode()                  (USER_PORT_INTR_I2C_EC_MASK_REG)
#define USER_PORT_GetI2CExtClkInterruptSourceMasked()          (USER_PORT_INTR_I2C_EC_MASKED_REG)

#if (!USER_PORT_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define USER_PORT_SetSpiExtClkInterruptMode(interruptMask) \
                                                                USER_PORT_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define USER_PORT_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                USER_PORT_CLEAR_INTR_SPI_EC(interruptMask)
    #define USER_PORT_GetExtSpiClkInterruptSource()                 (USER_PORT_INTR_SPI_EC_REG)
    #define USER_PORT_GetExtSpiClkInterruptMode()                   (USER_PORT_INTR_SPI_EC_MASK_REG)
    #define USER_PORT_GetExtSpiClkInterruptSourceMasked()           (USER_PORT_INTR_SPI_EC_MASKED_REG)
#endif /* (!USER_PORT_CY_SCBIP_V1) */

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void USER_PORT_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER)
    extern cyisraddress USER_PORT_customIntrHandler;
#endif /* !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER) */

extern USER_PORT_BACKUP_STRUCT USER_PORT_backup;

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 USER_PORT_scbMode;
    extern uint8 USER_PORT_scbEnableWake;
    extern uint8 USER_PORT_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 USER_PORT_mode;
    extern uint8 USER_PORT_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * USER_PORT_rxBuffer;
    extern uint8   USER_PORT_rxDataBits;
    extern uint32  USER_PORT_rxBufferSize;

    extern volatile uint8 * USER_PORT_txBuffer;
    extern uint8   USER_PORT_txDataBits;
    extern uint32  USER_PORT_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 USER_PORT_numberOfAddr;
    extern uint8 USER_PORT_subAddrSize;
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*        Conditional Macro
****************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define USER_PORT_SCB_MODE_I2C_RUNTM_CFG     (USER_PORT_SCB_MODE_I2C      == USER_PORT_scbMode)
    #define USER_PORT_SCB_MODE_SPI_RUNTM_CFG     (USER_PORT_SCB_MODE_SPI      == USER_PORT_scbMode)
    #define USER_PORT_SCB_MODE_UART_RUNTM_CFG    (USER_PORT_SCB_MODE_UART     == USER_PORT_scbMode)
    #define USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG   (USER_PORT_SCB_MODE_EZI2C    == USER_PORT_scbMode)
    #define USER_PORT_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (USER_PORT_SCB_MODE_UNCONFIG == USER_PORT_scbMode)

    /* Defines wakeup enable */
    #define USER_PORT_SCB_WAKE_ENABLE_CHECK       (0u != USER_PORT_scbEnableWake)
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!USER_PORT_CY_SCBIP_V1)
    #define USER_PORT_SCB_PINS_NUMBER    (7u)
#else
    #define USER_PORT_SCB_PINS_NUMBER    (2u)
#endif /* (!USER_PORT_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_USER_PORT_H) */


/* [] END OF FILE */
