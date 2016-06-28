/*******************************************************************************
* File Name: USER_PORT_PM.c
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

#include "USER_PORT.h"
#include "USER_PORT_PVT.h"

#if(USER_PORT_SCB_MODE_I2C_INC)
    #include "USER_PORT_I2C_PVT.h"
#endif /* (USER_PORT_SCB_MODE_I2C_INC) */

#if(USER_PORT_SCB_MODE_EZI2C_INC)
    #include "USER_PORT_EZI2C_PVT.h"
#endif /* (USER_PORT_SCB_MODE_EZI2C_INC) */

#if(USER_PORT_SCB_MODE_SPI_INC || USER_PORT_SCB_MODE_UART_INC)
    #include "USER_PORT_SPI_UART_PVT.h"
#endif /* (USER_PORT_SCB_MODE_SPI_INC || USER_PORT_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG || \
   (USER_PORT_SCB_MODE_I2C_CONST_CFG   && (!USER_PORT_I2C_WAKE_ENABLE_CONST))   || \
   (USER_PORT_SCB_MODE_EZI2C_CONST_CFG && (!USER_PORT_EZI2C_WAKE_ENABLE_CONST)) || \
   (USER_PORT_SCB_MODE_SPI_CONST_CFG   && (!USER_PORT_SPI_WAKE_ENABLE_CONST))   || \
   (USER_PORT_SCB_MODE_UART_CONST_CFG  && (!USER_PORT_UART_WAKE_ENABLE_CONST)))

    USER_PORT_BACKUP_STRUCT USER_PORT_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: USER_PORT_Sleep
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
void USER_PORT_Sleep(void)
{
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    if(USER_PORT_SCB_WAKE_ENABLE_CHECK)
    {
        if(USER_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            USER_PORT_I2CSaveConfig();
        }
        else if(USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            USER_PORT_EzI2CSaveConfig();
        }
    #if(!USER_PORT_CY_SCBIP_V1)
        else if(USER_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            USER_PORT_SpiSaveConfig();
        }
        else if(USER_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            USER_PORT_UartSaveConfig();
        }
    #endif /* (!USER_PORT_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        USER_PORT_backup.enableState = (uint8) USER_PORT_GET_CTRL_ENABLED;

        if(0u != USER_PORT_backup.enableState)
        {
            USER_PORT_Stop();
        }
    }

#else

    #if (USER_PORT_SCB_MODE_I2C_CONST_CFG && USER_PORT_I2C_WAKE_ENABLE_CONST)
        USER_PORT_I2CSaveConfig();

    #elif (USER_PORT_SCB_MODE_EZI2C_CONST_CFG && USER_PORT_EZI2C_WAKE_ENABLE_CONST)
        USER_PORT_EzI2CSaveConfig();

    #elif (USER_PORT_SCB_MODE_SPI_CONST_CFG && USER_PORT_SPI_WAKE_ENABLE_CONST)
        USER_PORT_SpiSaveConfig();

    #elif (USER_PORT_SCB_MODE_UART_CONST_CFG && USER_PORT_UART_WAKE_ENABLE_CONST)
        USER_PORT_UartSaveConfig();

    #else

        USER_PORT_backup.enableState = (uint8) USER_PORT_GET_CTRL_ENABLED;

        if(0u != USER_PORT_backup.enableState)
        {
            USER_PORT_Stop();
        }

    #endif /* defined (USER_PORT_SCB_MODE_I2C_CONST_CFG) && (USER_PORT_I2C_WAKE_ENABLE_CONST) */

#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: USER_PORT_Wakeup
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
void USER_PORT_Wakeup(void)
{
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    if(USER_PORT_SCB_WAKE_ENABLE_CHECK)
    {
        if(USER_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            USER_PORT_I2CRestoreConfig();
        }
        else if(USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            USER_PORT_EzI2CRestoreConfig();
        }
    #if(!USER_PORT_CY_SCBIP_V1)
        else if(USER_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            USER_PORT_SpiRestoreConfig();
        }
        else if(USER_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            USER_PORT_UartRestoreConfig();
        }
    #endif /* (!USER_PORT_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != USER_PORT_backup.enableState)
        {
            USER_PORT_Enable();
        }
    }

#else

    #if (USER_PORT_SCB_MODE_I2C_CONST_CFG  && USER_PORT_I2C_WAKE_ENABLE_CONST)
        USER_PORT_I2CRestoreConfig();

    #elif (USER_PORT_SCB_MODE_EZI2C_CONST_CFG && USER_PORT_EZI2C_WAKE_ENABLE_CONST)
        USER_PORT_EzI2CRestoreConfig();

    #elif (USER_PORT_SCB_MODE_SPI_CONST_CFG && USER_PORT_SPI_WAKE_ENABLE_CONST)
        USER_PORT_SpiRestoreConfig();

    #elif (USER_PORT_SCB_MODE_UART_CONST_CFG && USER_PORT_UART_WAKE_ENABLE_CONST)
        USER_PORT_UartRestoreConfig();

    #else

        if(0u != USER_PORT_backup.enableState)
        {
            USER_PORT_Enable();
        }

    #endif /* (USER_PORT_I2C_WAKE_ENABLE_CONST) */

#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
