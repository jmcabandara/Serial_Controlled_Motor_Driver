/*******************************************************************************
* File Name: EXPANSION_PORT_SPI_UART.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
*******************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EXPANSION_PORT_PVT.h"
#include "EXPANSION_PORT_SPI_UART_PVT.h"

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST)
    volatile uint32 EXPANSION_PORT_rxBufferHead;
    volatile uint32 EXPANSION_PORT_rxBufferTail;
    volatile uint8  EXPANSION_PORT_rxBufferOverflow;
#endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

#if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST)
    volatile uint32 EXPANSION_PORT_txBufferHead;
    volatile uint32 EXPANSION_PORT_txBufferTail;
#endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

#if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 EXPANSION_PORT_rxBufferInternal[EXPANSION_PORT_RX_BUFFER_SIZE];
#endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER) */

#if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER)
    volatile uint8 EXPANSION_PORT_txBufferInternal[EXPANSION_PORT_TX_BUFFER_SIZE];
#endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER) */


#if(EXPANSION_PORT_RX_DIRECTION)

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiUartReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Next data element from the receive buffer.
    *
    * Global Variables:
    *  Look into EXPANSION_PORT_SpiInit for description.
    *
    *******************************************************************************/
    uint32 EXPANSION_PORT_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

        #if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 locTail;
        #endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

        if(EXPANSION_PORT_CHECK_RX_SW_BUFFER)
        {
            if(EXPANSION_PORT_rxBufferHead != EXPANSION_PORT_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (EXPANSION_PORT_rxBufferTail + 1u);

                if(EXPANSION_PORT_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = EXPANSION_PORT_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                EXPANSION_PORT_rxBufferTail = locTail;
            }
        }
        else
        {
            rxData = EXPANSION_PORT_RX_FIFO_RD_REG; /* Read data from RX FIFO */
        }
        

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiUartGetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of received data elements
    *
    *******************************************************************************/
    uint32 EXPANSION_PORT_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
        #if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 locHead;
        #endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

        if(EXPANSION_PORT_CHECK_RX_SW_BUFFER)
        {
            locHead = EXPANSION_PORT_rxBufferHead;

            if(locHead >= EXPANSION_PORT_rxBufferTail)
            {
                size = (locHead - EXPANSION_PORT_rxBufferTail);
            }
            else
            {
                size = (locHead + (EXPANSION_PORT_RX_BUFFER_SIZE - EXPANSION_PORT_rxBufferTail));
            }
        }
        else
        {
            size = EXPANSION_PORT_GET_RX_FIFO_ENTRIES;
        }
        

        return(size);
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiUartClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receive buffer and RX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_SpiUartClearRxBuffer(void)
    {
        #if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 intSourceMask;
        #endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

        if(EXPANSION_PORT_CHECK_RX_SW_BUFFER)
        {
            intSourceMask = EXPANSION_PORT_SpiUartDisableIntRx();

            EXPANSION_PORT_CLEAR_RX_FIFO;

            /* Flush RX software buffer */
            EXPANSION_PORT_rxBufferHead     = EXPANSION_PORT_rxBufferTail;
            EXPANSION_PORT_rxBufferOverflow = 0u;

            /* End RX transfer */
            EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_ALL);

            EXPANSION_PORT_SpiUartEnableIntRx(intSourceMask);
        }
        else
        {
            EXPANSION_PORT_CLEAR_RX_FIFO;
        }
        
    }

#endif /* (EXPANSION_PORT_RX_DIRECTION) */


#if(EXPANSION_PORT_TX_DIRECTION)

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiUartWriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    * Parameters:
    *  txDataByte: the data to be transmitted.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_SpiUartWriteTxData(uint32 txData)
    {
        #if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 locHead;
            uint32 intSourceMask;
        #endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

        if(EXPANSION_PORT_CHECK_TX_SW_BUFFER)
        {
            /* Head index to put data */
            locHead = (EXPANSION_PORT_txBufferHead + 1u);

            /* Adjust TX software buffer index */
            if(EXPANSION_PORT_TX_BUFFER_SIZE == locHead)
            {
                locHead = 0u;
            }

            while(locHead == EXPANSION_PORT_txBufferTail)
            {
                /* Wait for space in TX software buffer */
            }

            /* TX software buffer has at least one room */

            if((EXPANSION_PORT_txBufferHead == EXPANSION_PORT_txBufferTail) &&
               (EXPANSION_PORT_SPI_UART_FIFO_SIZE != EXPANSION_PORT_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                EXPANSION_PORT_TX_FIFO_WR_REG = txData;
            }
            /* Put data in TX software buffer */
            else
            {
                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                EXPANSION_PORT_ClearTxInterruptSource(EXPANSION_PORT_INTR_TX_NOT_FULL);

                EXPANSION_PORT_PutWordInTxBuffer(locHead, txData);

                EXPANSION_PORT_txBufferHead = locHead;

                /* Enable interrupt to transmit */
                intSourceMask  = EXPANSION_PORT_INTR_TX_NOT_FULL;
                intSourceMask |= EXPANSION_PORT_GetTxInterruptMode();
                EXPANSION_PORT_SpiUartEnableIntTx(intSourceMask);
            }
        }
        else
        {
            while(EXPANSION_PORT_SPI_UART_FIFO_SIZE == EXPANSION_PORT_GET_TX_FIFO_ENTRIES)
            {
                /* Block while TX FIFO is FULL */
            }

            EXPANSION_PORT_TX_FIFO_WR_REG = txData;
        }
        
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiUartPutArray
    ********************************************************************************
    *
    * Summary:
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * Parameters:
    *  wrBuf:  pointer to an array with data to be placed in transmit buffer.
    *  count:  number of data elements to be placed in the transmit buffer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for(i=0u; i < count; i++)
        {
            EXPANSION_PORT_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiUartGetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    * Returns the number of elements currently in the transmit buffer.
    *  - TX software buffer is disabled: returns the number of used entries in
    *    TX FIFO.
    *  - TX software buffer is enabled: returns the number of elements currently
    *    used in the transmit buffer. This number does not include used entries in
    *    the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *    not full.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of data elements ready to transmit.
    *
    *******************************************************************************/
    uint32 EXPANSION_PORT_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
        #if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 locTail;
        #endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

        if(EXPANSION_PORT_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = EXPANSION_PORT_txBufferTail;

            if(EXPANSION_PORT_txBufferHead >= locTail)
            {
                size = (EXPANSION_PORT_txBufferHead - locTail);
            }
            else
            {
                size = (EXPANSION_PORT_txBufferHead + (EXPANSION_PORT_TX_BUFFER_SIZE - locTail));
            }
        }
        else
        {
            size = EXPANSION_PORT_GET_TX_FIFO_ENTRIES;
        }
        

        return(size);
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiUartClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the transmit buffer and TX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_SpiUartClearTxBuffer(void)
    {
        #if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 intSourceMask;
        #endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

        if(EXPANSION_PORT_CHECK_TX_SW_BUFFER)
        {
            intSourceMask = EXPANSION_PORT_SpiUartDisableIntTx();

            EXPANSION_PORT_CLEAR_TX_FIFO;

            /* Flush TX software buffer */
            EXPANSION_PORT_txBufferHead = EXPANSION_PORT_txBufferTail;

            /* End TX transfer if it is in progress */
            intSourceMask &= (uint32) ~EXPANSION_PORT_INTR_TX_NOT_FULL;

            EXPANSION_PORT_SpiUartEnableIntTx(intSourceMask);
        }
        else
        {
            EXPANSION_PORT_CLEAR_TX_FIFO;
        }
        
    }

#endif /* (EXPANSION_PORT_TX_DIRECTION) */


/*******************************************************************************
* Function Name: EXPANSION_PORT_SpiUartDisableIntRx
********************************************************************************
*
* Summary:
*  Disables the RX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 EXPANSION_PORT_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = EXPANSION_PORT_GetRxInterruptMode();

    EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

    return(intSource);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_SpiUartDisableIntTx
********************************************************************************
*
* Summary:
*  Disables TX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 EXPANSION_PORT_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = EXPANSION_PORT_GetTxInterruptMode();

    EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

    return(intSourceMask);
}


#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_PutWordInRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  index:      index to store data byte/word in the RX buffer.
    *  rxDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if(EXPANSION_PORT_ONE_BYTE_WIDTH == EXPANSION_PORT_rxDataBits)
        {
            EXPANSION_PORT_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            EXPANSION_PORT_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            EXPANSION_PORT_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_GetWordFromRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 EXPANSION_PORT_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if(EXPANSION_PORT_ONE_BYTE_WIDTH == EXPANSION_PORT_rxDataBits)
        {
            value = EXPANSION_PORT_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) EXPANSION_PORT_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)EXPANSION_PORT_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return(value);
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_PutWordInTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  idx:        index to store data byte/word in the TX buffer.
    *  txDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if(EXPANSION_PORT_ONE_BYTE_WIDTH == EXPANSION_PORT_txDataBits)
        {
            EXPANSION_PORT_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            EXPANSION_PORT_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            EXPANSION_PORT_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_GetWordFromTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    * Parameters:
    *  idx: index to get data byte/word from the TX buffer.
    *
    * Return:
    *  Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 EXPANSION_PORT_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if(EXPANSION_PORT_ONE_BYTE_WIDTH == EXPANSION_PORT_txDataBits)
        {
            value = (uint32) EXPANSION_PORT_txBuffer[idx];
        }
        else
        {
            value  = (uint32) EXPANSION_PORT_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) EXPANSION_PORT_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return(value);
    }

#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
