/*******************************************************************************
* File Name: EXPANSION_PORT_I2C_BOOT.c
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

#include "EXPANSION_PORT_BOOT.h"
#include "EXPANSION_PORT_I2C_PVT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED)

/***************************************
*    Private I/O Component Vars
***************************************/

/* Writes to this buffer */
static uint8 EXPANSION_PORT_slReadBuf[EXPANSION_PORT_I2C_BTLDR_SIZEOF_READ_BUFFER];

/* Reads from this buffer */
static uint8 EXPANSION_PORT_slWriteBuf[EXPANSION_PORT_I2C_BTLDR_SIZEOF_WRITE_BUFFER];

/* Flag to release buffer to be read */
static uint32 EXPANSION_PORT_applyBuffer;


/***************************************
*    Private Function Prototypes
***************************************/

static void EXPANSION_PORT_I2CResposeInsert(void);


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CCyBtldrCommStart
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
void EXPANSION_PORT_I2CCyBtldrCommStart(void)
{
    EXPANSION_PORT_I2CSlaveInitWriteBuf(EXPANSION_PORT_slWriteBuf, EXPANSION_PORT_I2C_BTLDR_SIZEOF_WRITE_BUFFER);
    EXPANSION_PORT_I2CSlaveInitReadBuf (EXPANSION_PORT_slReadBuf, 0u);

    EXPANSION_PORT_SetCustomInterruptHandler(&EXPANSION_PORT_I2CResposeInsert);
    EXPANSION_PORT_applyBuffer = 0u;

    EXPANSION_PORT_Start();
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CCyBtldrCommStop
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
void EXPANSION_PORT_I2CCyBtldrCommStop(void)
{
    EXPANSION_PORT_Stop();
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CCyBtldrCommReset
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
void EXPANSION_PORT_I2CCyBtldrCommReset(void)
{
    /* Make the read buffer full */
    EXPANSION_PORT_slRdBufSize = 0u;

    /* Reset write buffer and Read buffer */
    EXPANSION_PORT_slRdBufIndex = 0u;
    EXPANSION_PORT_slWrBufIndex = 0u;

    /* Clear read and write status */
    EXPANSION_PORT_slStatus = 0u;
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CCyBtldrCommRead
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
*  EXPANSION_PORT_slWriteBuf - used to store received command.
*
*******************************************************************************/
cystatus EXPANSION_PORT_I2CCyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
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
            if(0u != (EXPANSION_PORT_I2C_SSTAT_WR_CMPLT & EXPANSION_PORT_slStatus))
            {
                /* Copy command into bootloader buffer */
                (void) memcpy((void *) pData, (const void *) EXPANSION_PORT_slWriteBuf,
                                                EXPANSION_PORT_I2C_MIN_UINT16(EXPANSION_PORT_slWrBufIndex, size));

                /* Copy number of written bytes */
                *count = (uint16) EXPANSION_PORT_slWrBufIndex;

                /* Clear write buffer and status */
                EXPANSION_PORT_slStatus     = 0u;
                EXPANSION_PORT_slWrBufIndex = 0u;

                status = CYRET_SUCCESS;
                break;
            }

            CyDelay(EXPANSION_PORT_WAIT_1_MS);
            timeoutMs--;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CCyBtldrCommWrite
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
*  EXPANSION_PORT_slReadBuf - used to store response.
*  EXPANSION_PORT_slRdBufIndex - used to store current index within slave
*  read buffer.
*
*******************************************************************************/
cystatus EXPANSION_PORT_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;
    uint32 timeoutMs;

    status = CYRET_BAD_PARAM;

    if((NULL != pData) && (size > 0u))
    {
        status = CYRET_TIMEOUT;
        timeoutMs = ((uint32) 10u * timeOut); /* Convert from 10mS checks to 1mS checks */

        /* Copy response into read buffer */
        (void) memcpy((void *) EXPANSION_PORT_slReadBuf, (const void *) pData, (uint32) size);
        *count = size; /* Buffer was copied to I2C buffer */

        /* Read buffer is ready to be released to host */
        EXPANSION_PORT_applyBuffer = (uint32) size;

        while(0u != timeoutMs)
        {
            /* Check if response has been read */
            if(EXPANSION_PORT_slRdBufIndex == (uint32) size)
            {
                /* Invalidate read buffer */
                EXPANSION_PORT_slRdBufSize  = 0u;
                EXPANSION_PORT_slRdBufIndex = 0u;

                status = CYRET_SUCCESS;
                break;
            }

            CyDelay(EXPANSION_PORT_WAIT_1_MS);
            timeoutMs--;
        }
    }

    return(status);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CResposeInsert
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
*  EXPANSION_PORT_slRdBufIndex - used to store current index within slave
*  read buffer.
*  EXPANSION_PORT_applyBuffer - flag to release buffer with response to be
*  read
*
*******************************************************************************/
static void EXPANSION_PORT_I2CResposeInsert(void)
{
    if(EXPANSION_PORT_CHECK_INTR_SLAVE_MASKED(EXPANSION_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
    {
        if(0u != EXPANSION_PORT_applyBuffer)
        {
            /* Response was copied into buffer: release buffer to host */
            EXPANSION_PORT_slRdBufSize = EXPANSION_PORT_applyBuffer;
            EXPANSION_PORT_applyBuffer = 0u;
        }
    }
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (EXPANSION_PORT_I2C_BTLDR_COMM_ENABLED) */


/* [] END OF FILE */
