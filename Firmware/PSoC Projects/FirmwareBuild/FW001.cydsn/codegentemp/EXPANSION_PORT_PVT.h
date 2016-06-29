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

#if !defined(CY_SCB_PVT_EXPANSION_PORT_H)
#define CY_SCB_PVT_EXPANSION_PORT_H

#include "EXPANSION_PORT.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define EXPANSION_PORT_SetI2CExtClkInterruptMode(interruptMask) EXPANSION_PORT_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define EXPANSION_PORT_ClearI2CExtClkInterruptSource(interruptMask) EXPANSION_PORT_CLEAR_INTR_I2C_EC(interruptMask)
#define EXPANSION_PORT_GetI2CExtClkInterruptSource()                (EXPANSION_PORT_INTR_I2C_EC_REG)
#define EXPANSION_PORT_GetI2CExtClkInterruptMode()                  (EXPANSION_PORT_INTR_I2C_EC_MASK_REG)
#define EXPANSION_PORT_GetI2CExtClkInterruptSourceMasked()          (EXPANSION_PORT_INTR_I2C_EC_MASKED_REG)

#if (!EXPANSION_PORT_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define EXPANSION_PORT_SetSpiExtClkInterruptMode(interruptMask) \
                                                                EXPANSION_PORT_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define EXPANSION_PORT_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                EXPANSION_PORT_CLEAR_INTR_SPI_EC(interruptMask)
    #define EXPANSION_PORT_GetExtSpiClkInterruptSource()                 (EXPANSION_PORT_INTR_SPI_EC_REG)
    #define EXPANSION_PORT_GetExtSpiClkInterruptMode()                   (EXPANSION_PORT_INTR_SPI_EC_MASK_REG)
    #define EXPANSION_PORT_GetExtSpiClkInterruptSourceMasked()           (EXPANSION_PORT_INTR_SPI_EC_MASKED_REG)
#endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void EXPANSION_PORT_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER)
    extern cyisraddress EXPANSION_PORT_customIntrHandler;
#endif /* !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER) */

extern EXPANSION_PORT_BACKUP_STRUCT EXPANSION_PORT_backup;

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 EXPANSION_PORT_scbMode;
    extern uint8 EXPANSION_PORT_scbEnableWake;
    extern uint8 EXPANSION_PORT_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 EXPANSION_PORT_mode;
    extern uint8 EXPANSION_PORT_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * EXPANSION_PORT_rxBuffer;
    extern uint8   EXPANSION_PORT_rxDataBits;
    extern uint32  EXPANSION_PORT_rxBufferSize;

    extern volatile uint8 * EXPANSION_PORT_txBuffer;
    extern uint8   EXPANSION_PORT_txDataBits;
    extern uint32  EXPANSION_PORT_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 EXPANSION_PORT_numberOfAddr;
    extern uint8 EXPANSION_PORT_subAddrSize;
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*        Conditional Macro
****************************************/

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define EXPANSION_PORT_SCB_MODE_I2C_RUNTM_CFG     (EXPANSION_PORT_SCB_MODE_I2C      == EXPANSION_PORT_scbMode)
    #define EXPANSION_PORT_SCB_MODE_SPI_RUNTM_CFG     (EXPANSION_PORT_SCB_MODE_SPI      == EXPANSION_PORT_scbMode)
    #define EXPANSION_PORT_SCB_MODE_UART_RUNTM_CFG    (EXPANSION_PORT_SCB_MODE_UART     == EXPANSION_PORT_scbMode)
    #define EXPANSION_PORT_SCB_MODE_EZI2C_RUNTM_CFG   (EXPANSION_PORT_SCB_MODE_EZI2C    == EXPANSION_PORT_scbMode)
    #define EXPANSION_PORT_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (EXPANSION_PORT_SCB_MODE_UNCONFIG == EXPANSION_PORT_scbMode)

    /* Defines wakeup enable */
    #define EXPANSION_PORT_SCB_WAKE_ENABLE_CHECK       (0u != EXPANSION_PORT_scbEnableWake)
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!EXPANSION_PORT_CY_SCBIP_V1)
    #define EXPANSION_PORT_SCB_PINS_NUMBER    (7u)
#else
    #define EXPANSION_PORT_SCB_PINS_NUMBER    (2u)
#endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_EXPANSION_PORT_H) */


/* [] END OF FILE */
