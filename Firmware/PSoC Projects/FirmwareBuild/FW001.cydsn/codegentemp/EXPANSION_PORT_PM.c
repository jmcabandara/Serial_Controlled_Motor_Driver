/*******************************************************************************
* File Name: EXPANSION_PORT_PM.c
* Version 2.0
*
* Description:
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EXPANSION_PORT.h"
#include "EXPANSION_PORT_PVT.h"

#if(EXPANSION_PORT_SCB_MODE_I2C_INC)
    #include "EXPANSION_PORT_I2C_PVT.h"
#endif /* (EXPANSION_PORT_SCB_MODE_I2C_INC) */

#if(EXPANSION_PORT_SCB_MODE_EZI2C_INC)
    #include "EXPANSION_PORT_EZI2C_PVT.h"
#endif /* (EXPANSION_PORT_SCB_MODE_EZI2C_INC) */

#if(EXPANSION_PORT_SCB_MODE_SPI_INC || EXPANSION_PORT_SCB_MODE_UART_INC)
    #include "EXPANSION_PORT_SPI_UART_PVT.h"
#endif /* (EXPANSION_PORT_SCB_MODE_SPI_INC || EXPANSION_PORT_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG || \
   (EXPANSION_PORT_SCB_MODE_I2C_CONST_CFG   && (!EXPANSION_PORT_I2C_WAKE_ENABLE_CONST))   || \
   (EXPANSION_PORT_SCB_MODE_EZI2C_CONST_CFG && (!EXPANSION_PORT_EZI2C_WAKE_ENABLE_CONST)) || \
   (EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG   && (!EXPANSION_PORT_SPI_WAKE_ENABLE_CONST))   || \
   (EXPANSION_PORT_SCB_MODE_UART_CONST_CFG  && (!EXPANSION_PORT_UART_WAKE_ENABLE_CONST)))

    EXPANSION_PORT_BACKUP_STRUCT EXPANSION_PORT_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: EXPANSION_PORT_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component to enter Deep Sleep.
*  The "Enable wakeup from Sleep Mode" selection has an influence on
*  this function implementation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_Sleep(void)
{
#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    if(EXPANSION_PORT_SCB_WAKE_ENABLE_CHECK)
    {
        if(EXPANSION_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            EXPANSION_PORT_I2CSaveConfig();
        }
        else if(EXPANSION_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            EXPANSION_PORT_EzI2CSaveConfig();
        }
    #if(!EXPANSION_PORT_CY_SCBIP_V1)
        else if(EXPANSION_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            EXPANSION_PORT_SpiSaveConfig();
        }
        else if(EXPANSION_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            EXPANSION_PORT_UartSaveConfig();
        }
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        EXPANSION_PORT_backup.enableState = (uint8) EXPANSION_PORT_GET_CTRL_ENABLED;

        if(0u != EXPANSION_PORT_backup.enableState)
        {
            EXPANSION_PORT_Stop();
        }
    }

#else

    #if (EXPANSION_PORT_SCB_MODE_I2C_CONST_CFG && EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
        EXPANSION_PORT_I2CSaveConfig();

    #elif (EXPANSION_PORT_SCB_MODE_EZI2C_CONST_CFG && EXPANSION_PORT_EZI2C_WAKE_ENABLE_CONST)
        EXPANSION_PORT_EzI2CSaveConfig();

    #elif (EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG && EXPANSION_PORT_SPI_WAKE_ENABLE_CONST)
        EXPANSION_PORT_SpiSaveConfig();

    #elif (EXPANSION_PORT_SCB_MODE_UART_CONST_CFG && EXPANSION_PORT_UART_WAKE_ENABLE_CONST)
        EXPANSION_PORT_UartSaveConfig();

    #else

        EXPANSION_PORT_backup.enableState = (uint8) EXPANSION_PORT_GET_CTRL_ENABLED;

        if(0u != EXPANSION_PORT_backup.enableState)
        {
            EXPANSION_PORT_Stop();
        }

    #endif /* defined (EXPANSION_PORT_SCB_MODE_I2C_CONST_CFG) && (EXPANSION_PORT_I2C_WAKE_ENABLE_CONST) */

#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_Wakeup
********************************************************************************
*
* Summary:
*  Prepares the component for the Active mode operation after exiting
*  Deep Sleep. The "Enable wakeup from Sleep Mode" option has an influence
*  on this function implementation.
*  This function should not be called after exiting Sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_Wakeup(void)
{
#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    if(EXPANSION_PORT_SCB_WAKE_ENABLE_CHECK)
    {
        if(EXPANSION_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            EXPANSION_PORT_I2CRestoreConfig();
        }
        else if(EXPANSION_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            EXPANSION_PORT_EzI2CRestoreConfig();
        }
    #if(!EXPANSION_PORT_CY_SCBIP_V1)
        else if(EXPANSION_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            EXPANSION_PORT_SpiRestoreConfig();
        }
        else if(EXPANSION_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            EXPANSION_PORT_UartRestoreConfig();
        }
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != EXPANSION_PORT_backup.enableState)
        {
            EXPANSION_PORT_Enable();
        }
    }

#else

    #if (EXPANSION_PORT_SCB_MODE_I2C_CONST_CFG  && EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
        EXPANSION_PORT_I2CRestoreConfig();

    #elif (EXPANSION_PORT_SCB_MODE_EZI2C_CONST_CFG && EXPANSION_PORT_EZI2C_WAKE_ENABLE_CONST)
        EXPANSION_PORT_EzI2CRestoreConfig();

    #elif (EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG && EXPANSION_PORT_SPI_WAKE_ENABLE_CONST)
        EXPANSION_PORT_SpiRestoreConfig();

    #elif (EXPANSION_PORT_SCB_MODE_UART_CONST_CFG && EXPANSION_PORT_UART_WAKE_ENABLE_CONST)
        EXPANSION_PORT_UartRestoreConfig();

    #else

        if(0u != EXPANSION_PORT_backup.enableState)
        {
            EXPANSION_PORT_Enable();
        }

    #endif /* (EXPANSION_PORT_I2C_WAKE_ENABLE_CONST) */

#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
