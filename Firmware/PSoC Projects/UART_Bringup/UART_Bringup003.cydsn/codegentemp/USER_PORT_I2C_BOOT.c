/*******************************************************************************
* File Name: USER_PORT_I2C_BOOT.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the bootloader
*  communication support in SCB Component I2C mode.
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
#include "USER_PORT_I2C_PVT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_I2C_BTLDR_COMM_ENABLED)

/***************************************
*    Private I/O Component Vars
***************************************/

/* Writes to this buffer */
static uint8 USER_PORT_slReadBuf[USER_PORT_I2C_BTLDR_SIZEOF_READ_BUFFER];

/* Reads from this buffer */
static uint8 USER_PORT_slWriteBuf[USER_PORT_I2C_BTLDR_SIZEOF_WRITE_BUFFER];

/* Flag to release buffer to be read */
static uint32 USER_PORT_applyBuffer;


/***************************************
*    Private Function Prototypes
***************************************/

static void USER_PORT_I2CResposeInsert(void);


/*******************************************************************************
* Function Name: USER_PORT_I2CCyBtldrCommStart
********************************************************************************
*
* Summary:
*  Starts the I2C component and enables its interrupt.
*  Every incoming I2C write transaction is treated as a command for the
*  bootloader.
*  Every incoming I2C read transaction returns 0xFF until the bootloader
*  provides a response to the executed command.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void USER_PORT_I2CCyBtldrCommStart(void)
{
    USER_PORT_I2CSlaveInitWriteBuf(USER_PORT_slWriteBuf, USER_PORT_I2C_BTLDR_SIZEOF_WRITE_BUFFER);
    USER_PORT_I2CSlaveInitReadBuf (USER_PORT_slReadBuf, 0u);

    USER_PORT_SetCustomInterruptHandler(&USER_PORT_I2CResposeInsert);
    USER_PORT_applyBuffer = 0u;

    USER_PORT_Start();
}


/*******************************************************************************
* Function Name: USER_PORT_I2CCyBtldrCommStop
********************************************************************************
*
* Summary:
*  Disables the I2C component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void USER_PORT_I2CCyBtldrCommStop(void)
{
    USER_PORT_Stop();
}


/*******************************************************************************
* Function Name: USER_PORT_I2CCyBtldrCommReset
********************************************************************************
*
* Summary:
*  Sets read and write I2C buffers to the initial state and resets the slave
*  status.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*
*
*******************************************************************************/
void USER_PORT_I2CCyBtldrCommReset(void)
{
    /* Make the read buffer full */
    USER_PORT_slRdBufSize = 0u;

    /* Reset write buffer and Read buffer */
    USER_PORT_slRdBufIndex = 0u;
    USER_PORT_slWrBufIndex = 0u;

    /* Clear read and write status */
    USER_PORT_slStatus = 0u;
}


/*******************************************************************************
* Function Name: USER_PORT_I2CCyBtldrCommRead
********************************************************************************
*
* Summary:
*  Allows the caller to read data from the bootloader host.
*  The function handles polling to allow a block of data to be completely
*  received from the host device.
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
* Global variables:
*  USER_PORT_slWriteBuf - used to store received command.
*
*******************************************************************************/
cystatus USER_PORT_I2CCyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    uint32 timeoutMs;

    status = CYRET_BAD_PARAM;

    if((NULL != pData) && (size > 0u))
    {
        status = CYRET_TIMEOUT;
        timeoutMs = ((uint32) 10u * timeOut); /* Convert from 10mS check to 1mS checks */

        while(0u != timeoutMs)
        {
            /* Check if host complete write */
            if(0u != (USER_PORT_I2C_SSTAT_WR_CMPLT & USER_PORT_slStatus))
            {
                /* Copy command into bootloader buffer */
                (void) memcpy((void *) pData, (const void *) USER_PORT_slWriteBuf,
                                                USER_PORT_I2C_MIN_UINT16(USER_PORT_slWrBufIndex, size));

                /* Copy number of written bytes */
                *count = (uint16) USER_PORT_slWrBufIndex;

                /* Clear write buffer and status */
                USER_PORT_slStatus     = 0u;
                USER_PORT_slWrBufIndex = 0u;

                status = CYRET_SUCCESS;
                break;
            }

            CyDelay(USER_PORT_WAIT_1_MS);
            timeoutMs--;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: USER_PORT_I2CCyBtldrCommWrite
********************************************************************************
*
* Summary:
*  Allows the caller to write data to the bootloader host.
*  The function handles polling to allow a block of data to be completely sent
*  to the host device.
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
* Global variables:
*  USER_PORT_slReadBuf - used to store response.
*  USER_PORT_slRdBufIndex - used to store current index within slave
*  read buffer.
*
*******************************************************************************/
cystatus USER_PORT_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    uint32 timeoutMs;

    status = CYRET_BAD_PARAM;

    if((NULL != pData) && (size > 0u))
    {
        status = CYRET_TIMEOUT;
        timeoutMs = ((uint32) 10u * timeOut); /* Convert from 10mS checks to 1mS checks */

        /* Copy response into read buffer */
        (void) memcpy((void *) USER_PORT_slReadBuf, (const void *) pData, (uint32) size);
        *count = size; /* Buffer was copied to I2C buffer */

        /* Read buffer is ready to be released to host */
        USER_PORT_applyBuffer = (uint32) size;

        while(0u != timeoutMs)
        {
            /* Check if response has been read */
            if(USER_PORT_slRdBufIndex == (uint32) size)
            {
                /* Invalidate read buffer */
                USER_PORT_slRdBufSize  = 0u;
                USER_PORT_slRdBufIndex = 0u;

                status = CYRET_SUCCESS;
                break;
            }

            CyDelay(USER_PORT_WAIT_1_MS);
            timeoutMs--;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: USER_PORT_I2CResposeInsert
********************************************************************************
*
* Summary:
*  Releases the read buffer to be read when a response is copied to the buffer
*  and a new read transaction starts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  USER_PORT_slRdBufIndex - used to store current index within slave
*  read buffer.
*  USER_PORT_applyBuffer - flag to release buffer with response to be
*  read
*
*******************************************************************************/
static void USER_PORT_I2CResposeInsert(void)
{
    if(USER_PORT_CHECK_INTR_SLAVE_MASKED(USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
    {
        if(0u != USER_PORT_applyBuffer)
        {
            /* Response was copied into buffer: release buffer to host */
            USER_PORT_slRdBufSize = USER_PORT_applyBuffer;
            USER_PORT_applyBuffer = 0u;
        }
    }
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (USER_PORT_I2C_BTLDR_COMM_ENABLED) */


/* [] END OF FILE */
