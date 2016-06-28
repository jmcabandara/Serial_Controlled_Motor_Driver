/*******************************************************************************
* File Name: USER_PORT_SPI_UART_INT.c
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

#include "USER_PORT_PVT.h"
#include "USER_PORT_SPI_UART_PVT.h"


/*******************************************************************************
* Function Name: USER_PORT_SPI_UART_ISR
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
CY_ISR(USER_PORT_SPI_UART_ISR)
{
#if(USER_PORT_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
    uint32 dataRx;
#endif /* (USER_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

#if(USER_PORT_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (USER_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

    if(NULL != USER_PORT_customIntrHandler)
    {
        USER_PORT_customIntrHandler();
    }

    if(USER_PORT_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        USER_PORT_ClearSpiExtClkInterruptSource(USER_PORT_INTR_SPI_EC_WAKE_UP);
    }
    

    if(USER_PORT_CHECK_RX_SW_BUFFER)
    {
        if(USER_PORT_CHECK_INTR_RX_MASKED(USER_PORT_INTR_RX_NOT_EMPTY))
        {
            while(0u != USER_PORT_GET_RX_FIFO_ENTRIES)
            {
                /* Get data from RX FIFO */
                dataRx = USER_PORT_RX_FIFO_RD_REG;

                /* Move local head index */
                locHead = (USER_PORT_rxBufferHead + 1u);

                /* Adjust local head index */
                if(USER_PORT_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if(locHead == USER_PORT_rxBufferTail)
                {
                    /* Overflow: through away new data */
                    USER_PORT_rxBufferOverflow = (uint8) USER_PORT_INTR_RX_OVERFLOW;
                }
                else
                {
                    /* Store received data */
                    USER_PORT_PutWordInRxBuffer(locHead, dataRx);

                    /* Move head index */
                    USER_PORT_rxBufferHead = locHead;
                }
            }

            USER_PORT_ClearRxInterruptSource(USER_PORT_INTR_RX_NOT_EMPTY);
        }
    }
    


    if(USER_PORT_CHECK_TX_SW_BUFFER)
    {
        if(USER_PORT_CHECK_INTR_TX_MASKED(USER_PORT_INTR_TX_NOT_FULL))
        {
            /* Put data into TX FIFO */
            while(USER_PORT_SPI_UART_FIFO_SIZE != USER_PORT_GET_TX_FIFO_ENTRIES)
            {
                /* Check for room in TX software buffer */
                if(USER_PORT_txBufferHead != USER_PORT_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (USER_PORT_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if(USER_PORT_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    USER_PORT_TX_FIFO_WR_REG = USER_PORT_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    USER_PORT_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    USER_PORT_DISABLE_INTR_TX(USER_PORT_INTR_TX_NOT_FULL);
                    break;
                }
            }

            USER_PORT_ClearTxInterruptSource(USER_PORT_INTR_TX_NOT_FULL);
        }
    }
    
}


/* [] END OF FILE */
