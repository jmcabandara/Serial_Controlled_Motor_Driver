/*******************************************************************************
* File Name: USER_PORT_BOOT.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the bootloader
*  communication support in the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USER_PORT_BOOT.h"


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_BTLDR_COMM_MODE_ENABLED)

/*******************************************************************************
* Function Name: USER_PORT_CyBtldrCommStart
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommStart function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void USER_PORT_CyBtldrCommStart(void)
{
    #if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
        if(USER_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            USER_PORT_I2CCyBtldrCommStart();
        }
        else if(USER_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            USER_PORT_SpiCyBtldrCommStart();
        }
        else if(USER_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            USER_PORT_UartCyBtldrCommStart();
        }
        else if(USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
             USER_PORT_EzI2CCyBtldrCommStart();
        }
        else
        {
            /* Unknown mode */
        }
    #elif(USER_PORT_SCB_MODE_I2C_CONST_CFG)
        USER_PORT_I2CCyBtldrCommStart();

    #elif(USER_PORT_SCB_MODE_SPI_CONST_CFG)
        USER_PORT_SpiCyBtldrCommStart();

    #elif(USER_PORT_SCB_MODE_UART_CONST_CFG)
        USER_PORT_UartCyBtldrCommStart();

    #elif(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)
        USER_PORT_EzI2CCyBtldrCommStart();

    #else
        /* Unknown mode */

    #endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: USER_PORT_CyBtldrCommStop
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommStop function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void USER_PORT_CyBtldrCommStop(void)
{
    #if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
        if(USER_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            USER_PORT_I2CCyBtldrCommStop();
        }
        else if(USER_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            USER_PORT_SpiCyBtldrCommStop();
        }
        else if(USER_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            USER_PORT_UartCyBtldrCommStop();
        }
        else if(USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            USER_PORT_EzI2CCyBtldrCommStop();
        }
        else
        {
            /* Unknown mode */
        }
    #elif(USER_PORT_SCB_MODE_I2C_CONST_CFG)
        USER_PORT_I2CCyBtldrCommStop();

    #elif(USER_PORT_SCB_MODE_SPI_CONST_CFG)
        USER_PORT_SpiCyBtldrCommStop();

    #elif(USER_PORT_SCB_MODE_UART_CONST_CFG)
        USER_PORT_UartCyBtldrCommStop();

    #elif(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)
        USER_PORT_EzI2CCyBtldrCommStop();

    #else
        /* Unknown mode */

    #endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: USER_PORT_CyBtldrCommReset
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommReset function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void USER_PORT_CyBtldrCommReset(void)
{
    #if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
        if(USER_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            USER_PORT_I2CCyBtldrCommReset();
        }
        else if(USER_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            USER_PORT_SpiCyBtldrCommReset();
        }
        else if(USER_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            USER_PORT_UartCyBtldrCommReset();
        }
        else if(USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            USER_PORT_EzI2CCyBtldrCommReset();
        }
        else
        {
            /* Unknown mode */
        }
    #elif(USER_PORT_SCB_MODE_I2C_CONST_CFG)
        USER_PORT_I2CCyBtldrCommReset();

    #elif(USER_PORT_SCB_MODE_SPI_CONST_CFG)
        USER_PORT_SpiCyBtldrCommReset();

    #elif(USER_PORT_SCB_MODE_UART_CONST_CFG)
        USER_PORT_UartCyBtldrCommReset();

    #elif(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)
        USER_PORT_EzI2CCyBtldrCommReset();

    #else
        /* Unknown mode */

    #endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: USER_PORT_CyBtldrCommRead
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommRead function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  pData:    Pointer to storage for the block of data to be read from the
*            bootloader host
*  size:     Number of bytes to be read.
*  count:    Pointer to the variable to write the number of bytes actually
*            read.
*  timeOut:  Number of units in 10 ms to wait before returning because of a
*            timeout.
*
* Return:
*  Returns CYRET_SUCCESS if no problem was encountered or returns the value
*  that best describes the problem.
*
*******************************************************************************/
cystatus USER_PORT_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    #if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
        if(USER_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            status = USER_PORT_I2CCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(USER_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            status = USER_PORT_SpiCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(USER_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            status = USER_PORT_UartCyBtldrCommRead(pData, size, count, timeOut);
        }
        else if(USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            status = USER_PORT_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
        }
        else
        {
            status = CYRET_INVALID_STATE; /* Unknown mode: return status */
        }

    #elif(USER_PORT_SCB_MODE_I2C_CONST_CFG)
        status = USER_PORT_I2CCyBtldrCommRead(pData, size, count, timeOut);

    #elif(USER_PORT_SCB_MODE_SPI_CONST_CFG)
        status = USER_PORT_SpiCyBtldrCommRead(pData, size, count, timeOut);

    #elif(USER_PORT_SCB_MODE_UART_CONST_CFG)
        status = USER_PORT_UartCyBtldrCommRead(pData, size, count, timeOut);

    #elif(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)
        status = USER_PORT_EzI2CCyBtldrCommRead(pData, size, count, timeOut);

    #else
        status = CYRET_INVALID_STATE; /* Unknown mode: return status */

    #endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

    return(status);
}


/*******************************************************************************
* Function Name: USER_PORT_CyBtldrCommWrite
********************************************************************************
*
* Summary:
*  Calls the CyBtldrCommWrite  function of the bootloader communication
*  component for the selected mode.
*
* Parameters:
*  pData:    Pointer to the block of data to be written to the bootloader host.
*  size:     Number of bytes to be written.
*  count:    Pointer to the variable to write the number of bytes actually
*            written.
*  timeOut:  Number of units in 10 ms to wait before returning because of a
*            timeout.
*
* Return:
*  Returns CYRET_SUCCESS if no problem was encountered or returns the value
*  that best describes the problem.
*
*******************************************************************************/
cystatus USER_PORT_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    #if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
        if(USER_PORT_SCB_MODE_I2C_RUNTM_CFG)
        {
            status = USER_PORT_I2CCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(USER_PORT_SCB_MODE_SPI_RUNTM_CFG)
        {
            status = USER_PORT_SpiCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(USER_PORT_SCB_MODE_UART_RUNTM_CFG)
        {
            status = USER_PORT_UartCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else if(USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            status = USER_PORT_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
        }
        else
        {
            status = CYRET_INVALID_STATE; /* Unknown mode */
        }
    #elif(USER_PORT_SCB_MODE_I2C_CONST_CFG)
        status = USER_PORT_I2CCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(USER_PORT_SCB_MODE_SPI_CONST_CFG)
        status = USER_PORT_SpiCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(USER_PORT_SCB_MODE_UART_CONST_CFG)
        status = USER_PORT_UartCyBtldrCommWrite(pData, size, count, timeOut);

    #elif(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)
        status = USER_PORT_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);

    #else
        status = CYRET_INVALID_STATE; /* Unknown mode */

    #endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_BTLDR_COMM_MODE_ENABLED) */


/* [] END OF FILE */
