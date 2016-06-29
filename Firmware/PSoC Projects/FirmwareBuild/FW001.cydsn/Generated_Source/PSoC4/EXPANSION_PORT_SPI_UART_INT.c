/*******************************************************************************
* File Name: EXPANSION_PORT_SPI_UART_INT.c
* Version 2.0
*
* Description:
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EXPANSION_PORT_PVT.h"
#include "EXPANSION_PORT_SPI_UART_PVT.h"


/*******************************************************************************
* Function Name: EXPANSION_PORT_SPI_UART_ISR
********************************************************************************
*
* Summary:
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(EXPANSION_PORT_SPI_UART_ISR)
{
#if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
    uint32 dataRx;
#endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

#if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

    if(NULL != EXPANSION_PORT_customIntrHandler)
    {
        EXPANSION_PORT_customIntrHandler();
    }

    if(EXPANSION_PORT_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        EXPANSION_PORT_ClearSpiExtClkInterruptSource(EXPANSION_PORT_INTR_SPI_EC_WAKE_UP);
    }
    

    if(EXPANSION_PORT_CHECK_RX_SW_BUFFER)
    {
        if(EXPANSION_PORT_CHECK_INTR_RX_MASKED(EXPANSION_PORT_INTR_RX_NOT_EMPTY))
        {
            while(0u != EXPANSION_PORT_GET_RX_FIFO_ENTRIES)
            {
                /* Get data from RX FIFO */
                dataRx = EXPANSION_PORT_RX_FIFO_RD_REG;

                /* Move local head index */
                locHead = (EXPANSION_PORT_rxBufferHead + 1u);

                /* Adjust local head index */
                if(EXPANSION_PORT_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if(locHead == EXPANSION_PORT_rxBufferTail)
                {
                    /* Overflow: through away new data */
                    EXPANSION_PORT_rxBufferOverflow = (uint8) EXPANSION_PORT_INTR_RX_OVERFLOW;
                }
                else
                {
                    /* Store received data */
                    EXPANSION_PORT_PutWordInRxBuffer(locHead, dataRx);

                    /* Move head index */
                    EXPANSION_PORT_rxBufferHead = locHead;
                }
            }

            EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_NOT_EMPTY);
        }
    }
    


    if(EXPANSION_PORT_CHECK_TX_SW_BUFFER)
    {
        if(EXPANSION_PORT_CHECK_INTR_TX_MASKED(EXPANSION_PORT_INTR_TX_NOT_FULL))
        {
            /* Put data into TX FIFO */
            while(EXPANSION_PORT_SPI_UART_FIFO_SIZE != EXPANSION_PORT_GET_TX_FIFO_ENTRIES)
            {
                /* Check for room in TX software buffer */
                if(EXPANSION_PORT_txBufferHead != EXPANSION_PORT_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (EXPANSION_PORT_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if(EXPANSION_PORT_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    EXPANSION_PORT_TX_FIFO_WR_REG = EXPANSION_PORT_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    EXPANSION_PORT_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    EXPANSION_PORT_DISABLE_INTR_TX(EXPANSION_PORT_INTR_TX_NOT_FULL);
                    break;
                }
            }

            EXPANSION_PORT_ClearTxInterruptSource(EXPANSION_PORT_INTR_TX_NOT_FULL);
        }
    }
    
}


/* [] END OF FILE */
