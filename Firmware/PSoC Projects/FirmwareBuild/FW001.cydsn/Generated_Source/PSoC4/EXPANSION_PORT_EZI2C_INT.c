/*******************************************************************************
* File Name: EXPANSION_PORT_EZI2C_INT.c
* Version 2.0
*
* Description:
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in EZI2C mode.
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
#include "EXPANSION_PORT_EZI2C_PVT.h"

#if(EXPANSION_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EZI2C_STRETCH_ISR
    ********************************************************************************
    *
    * Summary:
    *  Handles the Interrupt Service Routine for the SCB EZI2C mode. The clock stretching is
    *  used during operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    CY_ISR_PROTO(EXPANSION_PORT_EZI2C_STRETCH_ISR)
    {
        static uint16 locBufSize;
        uint32 locIndex;
        uint32 locStatus;

        uint32 endTransfer;
        uint32 fifoIndex;
        uint32 locByte;

        uint32 locIntrCause;
        uint32 locIntrSlave;

    #if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
        /* Variable intended to be used with either buffer */
        static volatile uint8 * EXPANSION_PORT_dataBuffer; /* Pointer to data buffer              */
        static uint16 EXPANSION_PORT_bufSizeBuf;           /* Size of buffer in bytes             */
        static uint16 EXPANSION_PORT_protectBuf;           /* Start index of write protected area */

        static uint8 activeAddress;
        uint32 ackResponse;

        ackResponse = EXPANSION_PORT_EZI2C_ACK_RECEIVED_ADDRESS;
    #endif /* (EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */

    #if !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER)
        if(NULL != EXPANSION_PORT_customIntrHandler)
        {
            EXPANSION_PORT_customIntrHandler();
        }
    #else
        CY_EXPANSION_PORT_CUSTOM_INTR_HANDLER();
    #endif /* !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER) */

        /* Make local copy of global variable */
        locIndex = EXPANSION_PORT_EZI2C_GET_INDEX(activeAddress);

        /* Get interrupt sources */
        locIntrSlave = EXPANSION_PORT_GetSlaveInterruptSource();
        locIntrCause = EXPANSION_PORT_GetInterruptCause();

        /* INTR_SLAVE.I2C_ARB_LOST and INTR_SLAVE_I2C.BUS_ERROR */
        /* Handles errors on the bus. There are cases when both bits are set.
        * The error recovery is common: re-enable the scb IP. The content of the RX FIFO is lost.
        */
        if(0u != (locIntrSlave & (EXPANSION_PORT_INTR_SLAVE_I2C_ARB_LOST |
                                  EXPANSION_PORT_INTR_SLAVE_I2C_BUS_ERROR)))
        {
            EXPANSION_PORT_CTRL_REG &= (uint32) ~EXPANSION_PORT_CTRL_ENABLED; /* Disable SCB block */

        #if(EXPANSION_PORT_CY_SCBIP_V0)
            if(0u != ((uint8) EXPANSION_PORT_EZI2C_STATUS_BUSY & EXPANSION_PORT_curStatus))
        #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */
            {
                EXPANSION_PORT_curStatus &= (uint8) ~EXPANSION_PORT_EZI2C_STATUS_BUSY;
                EXPANSION_PORT_curStatus |= (uint8)  EXPANSION_PORT_EZI2C_STATUS_ERR;

                /* INTR_TX_EMPTY is enabled in the address phase to receive data */
                if(0u == (EXPANSION_PORT_GetTxInterruptMode() & EXPANSION_PORT_INTR_TX_EMPTY))
                {
                    /* Write complete */
                    if(EXPANSION_PORT_indexBuf1 != EXPANSION_PORT_offsetBuf1)
                    {
                        EXPANSION_PORT_curStatus |= (uint8) EXPANSION_PORT_INTR_SLAVE_I2C_WRITE_STOP;
                    }
                }
                else
                {
                    /* Read complete */
                    EXPANSION_PORT_curStatus |= (uint8) EXPANSION_PORT_INTR_SLAVE_I2C_NACK;
                }
            }

            EXPANSION_PORT_DISABLE_SLAVE_AUTO_DATA;

            /* Disable TX and RX interrupt sources */
            EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
            EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

        #if(EXPANSION_PORT_CY_SCBIP_V0)
            /* Clear interrupt sources as they are not automatically cleared after SCB is disabled */
            EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_ALL);
            EXPANSION_PORT_ClearSlaveInterruptSource(EXPANSION_PORT_INTR_SLAVE_ALL);
        #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

            EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_IDLE;

            EXPANSION_PORT_CTRL_REG |= (uint32) EXPANSION_PORT_CTRL_ENABLED;  /* Enable SCB block */
        }
        else
        {
            /* INTR_I2C_EC_WAKE_UP */
            /* Wakes up device from deep sleep */
            if(0u != (locIntrCause & EXPANSION_PORT_INTR_CAUSE_I2C_EC))
            {
                /* Disables wakeup interrupt source but does not clear it. It is cleared in INTR_SLAVE_I2C_ADDR_MATCH */
                EXPANSION_PORT_SetI2CExtClkInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
            }

            if(0u != (locIntrCause & (EXPANSION_PORT_INTR_CAUSE_RX | EXPANSION_PORT_INTR_CAUSE_SLAVE)))
            {
                /* INTR_RX.NOT_EMPTY */
                /* Receives data byte-by-byte. Does not use RX FIFO capabilities */
                if (0u != (EXPANSION_PORT_GetRxInterruptSourceMasked() & EXPANSION_PORT_INTR_RX_NOT_EMPTY))
                {
                #if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
                    /* If I2C_STOP service is delayed to I2C_ADDR_MATCH the address byte is in the RX FIFO and
                    * RX_NOT_EMPTY is enabled. The address byte has to stay into RX FIFO therefore
                    * RX.NOT_EMPTY service has to be skipped. The address byte has to be read by I2C_ADDR_MATCH.
                    */
                    if (0u == (locIntrCause & EXPANSION_PORT_INTR_CAUSE_SLAVE))
                #endif /* (EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */
                    {
                        locByte = EXPANSION_PORT_RX_FIFO_RD_REG;

                        switch(EXPANSION_PORT_fsmState)
                        {
                        case EXPANSION_PORT_EZI2C_FSM_BYTE_WRITE:
                            if(0u != locBufSize)
                            {
                                /* Store data byte and ACK */
                                EXPANSION_PORT_I2C_SLAVE_GENERATE_ACK;

                                EXPANSION_PORT_dataBuffer[locIndex] = (uint8) locByte;
                                locIndex++;
                                locBufSize--;
                            }
                            else
                            {
                                /* Discard data byte and NACK */
                                EXPANSION_PORT_I2C_SLAVE_GENERATE_NACK;
                            }
                            break;

                    #if(EXPANSION_PORT_SUB_ADDRESS_SIZE16_CONST)
                        case EXPANSION_PORT_EZI2C_FSM_OFFSET_HI8:

                            EXPANSION_PORT_I2C_SLAVE_GENERATE_ACK;

                            /* Store offset most significant byre */
                            locBufSize = (uint16) ((uint8) locByte);

                            EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_OFFSET_LO8;

                            break;
                    #endif /* (EXPANSION_PORT_SUB_ADDRESS_SIZE16_CONST) */

                        case EXPANSION_PORT_EZI2C_FSM_OFFSET_LO8:

                            if (EXPANSION_PORT_SUB_ADDRESS_SIZE16)
                            {
                                /* Collect 2 bytes offset */
                                locByte = ((uint32) ((uint32) locBufSize << 8u)) | locByte;
                            }
                            

                            /* Check offset against buffer size */
                            if(locByte < (uint32) EXPANSION_PORT_bufSizeBuf)
                            {
                                EXPANSION_PORT_I2C_SLAVE_GENERATE_ACK;

                                /* Update local buffer index with new offset */
                                locIndex = locByte;

                                /* Get available buffer size to write */
                                locBufSize = (uint16) ((locByte < EXPANSION_PORT_protectBuf) ?
                                                       (EXPANSION_PORT_protectBuf - locByte) : (0u));

                            #if(EXPANSION_PORT_CY_SCBIP_V0)

                                if(locBufSize < EXPANSION_PORT_EZI2C_FIFO_SIZE)
                                {
                                    /* Set FSM state to receive byte by byte */
                                    EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_BYTE_WRITE;
                                }
                                /* Receive RX FIFO chunks */
                                else if(locBufSize == EXPANSION_PORT_EZI2C_FIFO_SIZE)
                                {
                                    EXPANSION_PORT_ENABLE_SLAVE_AUTO_DATA; /* NACK when RX FIFO is full */
                                    EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
                                }
                                else
                                {
                                    EXPANSION_PORT_ENABLE_SLAVE_AUTO_DATA_ACK; /* Stretch when RX FIFO is full */
                                    EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_INTR_RX_FULL);
                                }

                            #else

                                if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE)
                                {
                                    /* Set FSM state to receive byte by byte.
                                    * The byte by byte receive is always chosen for two addresses. Ticket ID#175559.
                                    */
                                    EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_BYTE_WRITE;
                                }
                                else
                                {
                                    if (locBufSize < EXPANSION_PORT_EZI2C_FIFO_SIZE)
                                    {
                                        /* Set FSM state to receive byte by byte */
                                        EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_BYTE_WRITE;
                                    }
                                    /* Receive RX FIFO chunks */
                                    else if (locBufSize == EXPANSION_PORT_EZI2C_FIFO_SIZE)
                                    {
                                        EXPANSION_PORT_ENABLE_SLAVE_AUTO_DATA; /* NACK when RX FIFO is full */
                                        EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
                                    }
                                    else
                                    {
                                        EXPANSION_PORT_ENABLE_SLAVE_AUTO_DATA_ACK; /* Stretch when RX FIFO is full */
                                        EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_INTR_RX_FULL);
                                    }
                                }
                                

                            #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

                                /* Store local offset into global variable */
                                EXPANSION_PORT_EZI2C_SET_OFFSET(activeAddress, locIndex);
                            }
                            else
                            {
                                /* Discard offset byte and NACK */
                                EXPANSION_PORT_I2C_SLAVE_GENERATE_NACK;
                            }
                            break;

                        default:
                            CYASSERT(0u != 0u); /* Should never get there */
                            break;
                        }

                        EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_NOT_EMPTY);
                    }
                }
                /* INTR_RX.FULL, INTR_SLAVE.I2C_STOP */
                /* Receive FIFO chunks: auto data ACK is enabled */
                else if (0u != (EXPANSION_PORT_I2C_CTRL_REG & EXPANSION_PORT_I2C_CTRL_S_READY_DATA_ACK))
                {
                    /* Slave interrupt (I2C_STOP or I2C_ADDR_MATCH) leads to completion of read.
                    * A completion event has a higher priority than the FIFO full.
                    * Read remaining data from RX FIFO.
                    */
                    if(0u != (locIntrCause & EXPANSION_PORT_INTR_CAUSE_SLAVE))
                    {
                        /* Read remaining bytes from RX FIFO */
                        fifoIndex = EXPANSION_PORT_GET_RX_FIFO_ENTRIES;

                        if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE)
                        {
                            /* Update with current address match */
                            if(EXPANSION_PORT_CHECK_INTR_SLAVE_MASKED(EXPANSION_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
                            {
                                /* Update RX FIFO entries as address byte is there now */
                                fifoIndex = EXPANSION_PORT_GET_RX_FIFO_ENTRIES;

                                /* If SR is valid, RX FIFO is full and address is in SHIFTER:
                                * read 8 entries and leave address in RX FIFO for further processing.
                                * If SR is invalid, address is already in RX FIFO: read (entries-1).
                                */
                                fifoIndex -= ((0u != EXPANSION_PORT_GET_RX_FIFO_SR_VALID) ? (0u) : (1u));
                            }
                        }
                        

                        EXPANSION_PORT_DISABLE_SLAVE_AUTO_DATA;
                        endTransfer = EXPANSION_PORT_EZI2C_CONTINUE_TRANSFER;
                    }
                    else
                    /* INTR_RX_FULL */
                    /* Continue transfer or disable INTR_RX_FULL to catch completion event. */
                    {
                        /* Calculate buffer size available to write data into */
                        locBufSize -= (uint16) EXPANSION_PORT_EZI2C_FIFO_SIZE;

                        if(locBufSize <= EXPANSION_PORT_EZI2C_FIFO_SIZE)
                        {
                            /* Send NACK when RX FIFO overflow */
                            fifoIndex   = locBufSize;
                            endTransfer = EXPANSION_PORT_EZI2C_COMPLETE_TRANSFER;
                        }
                        else
                        {
                            /* Continue  transaction */
                            fifoIndex   = EXPANSION_PORT_EZI2C_FIFO_SIZE;
                            endTransfer = EXPANSION_PORT_EZI2C_CONTINUE_TRANSFER;
                        }
                    }

                    for(; (0u != fifoIndex); fifoIndex--)
                    {
                        /* Store data in buffer */
                        EXPANSION_PORT_dataBuffer[locIndex] = (uint8) EXPANSION_PORT_RX_FIFO_RD_REG;
                        locIndex++;
                    }

                    /* Complete transfer sending NACK when RX FIFO overflow */
                    if(EXPANSION_PORT_EZI2C_COMPLETE_TRANSFER == endTransfer)
                    {
                        EXPANSION_PORT_ENABLE_SLAVE_AUTO_DATA_NACK;

                        /* Disable INTR_RX_FULL during last RX FIFO chunk reception */
                        EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
                    }

                    EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_FULL |
                                                            EXPANSION_PORT_INTR_RX_NOT_EMPTY);
                }
                else
                {
                    /* Exit for slave interrupts which are not active for RX direction:
                    * INTR_SLAVE.I2C_ADDR_MATCH and INTR_SLAVE.I2C_STOP while byte-by-byte reception.
                    */
                }
            }

            if(0u != (locIntrCause & EXPANSION_PORT_INTR_CAUSE_SLAVE))
            {
                /* INTR_SLAVE.I2C_STOP */
                /* Catch Stop condition: completion of write or read transfer */
            #if(!EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
                if(0u != (locIntrSlave & EXPANSION_PORT_INTR_SLAVE_I2C_STOP))
            #else
                /* Prevent triggering when matched address was NACKed */
                if((0u != (locIntrSlave & EXPANSION_PORT_INTR_SLAVE_I2C_STOP)) &&
                   (0u != ((uint8) EXPANSION_PORT_EZI2C_STATUS_BUSY & EXPANSION_PORT_curStatus)))
            #endif
                {
                    /* Disable TX and RX interrupt sources */
                    EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
                    EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

                    /* Set read completion mask */
                    locStatus = EXPANSION_PORT_INTR_SLAVE_I2C_NACK;

                    /* Check if buffer content was modified: the address phase resets the locIndex */
                    if(locIndex != EXPANSION_PORT_EZI2C_GET_OFFSET(activeAddress))
                    {
                        locStatus |= EXPANSION_PORT_INTR_SLAVE_I2C_WRITE_STOP;
                    }

                    /* Complete read or write transaction */
                    locStatus &= locIntrSlave;
                    EXPANSION_PORT_EZI2C_UPDATE_LOC_STATUS(activeAddress, locStatus);
                    locStatus |= (uint32)  EXPANSION_PORT_curStatus;
                    locStatus &= (uint32) ~EXPANSION_PORT_EZI2C_STATUS_BUSY;
                    EXPANSION_PORT_curStatus = (uint8) locStatus;

                    EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_IDLE;

                    if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE)
                    {
                        /* Store local index into global variable, before address phase */
                        EXPANSION_PORT_EZI2C_SET_INDEX(activeAddress, locIndex);
                    }
                    
                }

                /* INTR_SLAVE.I2C_ADDR_MATCH */
                /* The matched address is received: the slave starts its operation.
                * INTR_SLAVE_I2C_STOP updates the buffer index before the address phase for two addresses mode.
                * This is done to update buffer index correctly before the address phase changes it.
                */
                if(0u != (locIntrSlave & EXPANSION_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
                {
                    if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE)
                    {
                        /* Read address byte from RX FIFO */
                        locByte = EXPANSION_PORT_GET_I2C_7BIT_ADDRESS(EXPANSION_PORT_RX_FIFO_RD_REG);

                        EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_NOT_EMPTY);

                        /* Check received address against device addresses */
                        if(EXPANSION_PORT_addrBuf1 == locByte)
                        {
                            /* Set buffer exposed to primary slave address */
                            EXPANSION_PORT_dataBuffer = EXPANSION_PORT_dataBuffer1;
                            EXPANSION_PORT_bufSizeBuf = EXPANSION_PORT_bufSizeBuf1;
                            EXPANSION_PORT_protectBuf = EXPANSION_PORT_protectBuf1;

                            activeAddress = EXPANSION_PORT_EZI2C_ACTIVE_ADDRESS1;
                        }
                        else if(EXPANSION_PORT_addrBuf2 == locByte)
                        {
                            /* Set buffer exposed to secondary slave address */
                            EXPANSION_PORT_dataBuffer = EXPANSION_PORT_dataBuffer2;
                            EXPANSION_PORT_bufSizeBuf = EXPANSION_PORT_bufSizeBuf2;
                            EXPANSION_PORT_protectBuf = EXPANSION_PORT_protectBuf2;

                            activeAddress = EXPANSION_PORT_EZI2C_ACTIVE_ADDRESS2;
                        }
                        else
                        {
                            /* Address does not match */
                            ackResponse = EXPANSION_PORT_EZI2C_NACK_RECEIVED_ADDRESS;
                        }
                    }
                    

                #if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
                    if(EXPANSION_PORT_EZI2C_NACK_RECEIVED_ADDRESS == ackResponse)
                    {
                        /* Clear interrupt sources before NACK address */
                        EXPANSION_PORT_ClearI2CExtClkInterruptSource(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP);
                        EXPANSION_PORT_ClearSlaveInterruptSource(EXPANSION_PORT_INTR_SLAVE_ALL);

                    #if(!EXPANSION_PORT_CY_SCBIP_V0)
                        /* Disable INTR_I2C_STOP to not trigger after matched address is NACKed. Ticket ID#156094 */
                        EXPANSION_PORT_DISABLE_INTR_SLAVE(EXPANSION_PORT_INTR_SLAVE_I2C_STOP);
                    #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */

                        /* NACK address byte: it does not match neither primary nor secondary */
                        EXPANSION_PORT_I2C_SLAVE_GENERATE_NACK;
                    }
                    else
                #endif /* (EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */
                    {

                    #if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
                        if(!EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE)
                        {
                            /* Set buffer exposed to primary slave address */
                            EXPANSION_PORT_dataBuffer = EXPANSION_PORT_dataBuffer1;
                            EXPANSION_PORT_bufSizeBuf = EXPANSION_PORT_bufSizeBuf1;
                            EXPANSION_PORT_protectBuf = EXPANSION_PORT_protectBuf1;

                            activeAddress = EXPANSION_PORT_EZI2C_ACTIVE_ADDRESS1;
                        }
                    #endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

                        /* Bus becomes busy after address is received */
                        EXPANSION_PORT_curStatus |= (uint8) EXPANSION_PORT_EZI2C_STATUS_BUSY;

                        /* Slave is read or written: set current offset */
                        locIndex = EXPANSION_PORT_EZI2C_GET_OFFSET(activeAddress);

                        /* Check transaction direction */
                        if(EXPANSION_PORT_CHECK_I2C_STATUS(EXPANSION_PORT_I2C_STATUS_S_READ))
                        {
                            /* Calculate slave buffer size */
                            locBufSize = EXPANSION_PORT_bufSizeBuf - (uint16) locIndex;

                            /* Clear TX FIFO to start fill from offset */
                            EXPANSION_PORT_CLEAR_TX_FIFO;
                            EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_INTR_TX_EMPTY);
                        }
                        else
                        {
                            /* Master writes: enable reception interrupt. The FSM state was set in INTR_SLAVE_I2C_STOP*/
                            EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_INTR_RX_NOT_EMPTY);
                        }

                        /* Clear interrupt sources before ACK address */
                        EXPANSION_PORT_ClearI2CExtClkInterruptSource(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP);
                        EXPANSION_PORT_ClearSlaveInterruptSource(EXPANSION_PORT_INTR_SLAVE_ALL);

                    #if (!EXPANSION_PORT_CY_SCBIP_V0)
                        /* Enable STOP to trigger after address match is ACKed. Ticket ID#156094 */
                        EXPANSION_PORT_ENABLE_INTR_SLAVE(EXPANSION_PORT_INTR_SLAVE_I2C_STOP);
                    #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */

                        /* ACK the address byte */
                        EXPANSION_PORT_I2C_SLAVE_GENERATE_ACK;
                    }
                }

                /* Clear slave interrupt sources */
                EXPANSION_PORT_ClearSlaveInterruptSource(locIntrSlave);
            }

            /* INTR_TX.EMPTY */
            /* Transmits data to the master: loads data into the TX FIFO. The 0xFF sends out if the master reads
            * out the buffer. The address reception with a read flag clears the TX FIFO to be loaded with data.
            */
            if(0u != (EXPANSION_PORT_GetInterruptCause() & EXPANSION_PORT_INTR_CAUSE_TX))
            {
                /* Put data into TX FIFO until there is a room */
                do
                {
                    /* Check transmit buffer range: locBufSize calculates after address reception */
                    if(0u != locBufSize)
                    {
                        EXPANSION_PORT_TX_FIFO_WR_REG = (uint32) EXPANSION_PORT_dataBuffer[locIndex];
                        locIndex++;
                        locBufSize--;
                    }
                    else
                    {
                        EXPANSION_PORT_TX_FIFO_WR_REG = EXPANSION_PORT_EZI2C_OVFL_RETURN;
                    }
                }
                while(EXPANSION_PORT_EZI2C_FIFO_SIZE != EXPANSION_PORT_GET_TX_FIFO_ENTRIES);

                EXPANSION_PORT_ClearTxInterruptSource(EXPANSION_PORT_INTR_TX_EMPTY);
            }
        }

        /* Store local index copy into global variable */
        EXPANSION_PORT_EZI2C_SET_INDEX(activeAddress, locIndex);
    }
#endif /* (EXPANSION_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST) */


#if(EXPANSION_PORT_EZI2C_SCL_STRETCH_DISABLE_CONST)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EZI2C_NO_STRETCH_ISR
    ********************************************************************************
    *
    * Summary:
    *  Handles the Interrupt Service Routine for the SCB EZI2C mode. Clock stretching is
    *  NOT used during operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    CY_ISR_PROTO(EXPANSION_PORT_EZI2C_NO_STRETCH_ISR)
    {
    #if(EXPANSION_PORT_SUB_ADDRESS_SIZE16_CONST)
        static uint8 locOffset;
    #endif /* (EXPANSION_PORT_SUB_ADDRESS_SIZE16_CONST) */

        uint32 locByte;
        uint32 locStatus;
        uint32 locIntrSlave;
        uint32 locIntrCause;

    #if !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER)
        /* Calls registered customer routine to manage interrupt sources */
        if(NULL != EXPANSION_PORT_customIntrHandler)
        {
            EXPANSION_PORT_customIntrHandler();
        }
    #else
        CY_EXPANSION_PORT_CUSTOM_INTR_HANDLER();
    #endif /* !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER) */

        locByte = 0u;

        /* Get copy of triggered slave interrupt sources */
        locIntrSlave = EXPANSION_PORT_GetSlaveInterruptSource();
        locIntrCause = EXPANSION_PORT_GetInterruptCause();

        /* INTR_SLAVE.I2C_ARB_LOST and INTR_SLAVE.I2C_BUS_ERROR */
        /* Handles errors on the bus: There are cases when both bits are set.
        * The error recovery is common: re-enable the scb IP. The content of the RX FIFO is lost.
        */
        if(0u != (locIntrSlave & (EXPANSION_PORT_INTR_SLAVE_I2C_ARB_LOST |
                                  EXPANSION_PORT_INTR_SLAVE_I2C_BUS_ERROR)))
        {
            EXPANSION_PORT_CTRL_REG &= (uint32) ~EXPANSION_PORT_CTRL_ENABLED; /* Disable SCB block */

        #if (EXPANSION_PORT_CY_SCBIP_V0)
            if(0u != (locIntrSlave & EXPANSION_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
        #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */
            {
                EXPANSION_PORT_curStatus |= (uint8) EXPANSION_PORT_EZI2C_STATUS_ERR;

                if(0u != (EXPANSION_PORT_EZI2C_FSM_WRITE_MASK & EXPANSION_PORT_fsmState))
                {
                    /* Write complete */
                    if(EXPANSION_PORT_indexBuf1 != EXPANSION_PORT_offsetBuf1)
                    {
                        EXPANSION_PORT_curStatus |= (uint8) EXPANSION_PORT_INTR_SLAVE_I2C_WRITE_STOP;
                    }
                }
                else
                {
                    /* Read complete */
                    EXPANSION_PORT_curStatus |= (uint8) EXPANSION_PORT_INTR_SLAVE_I2C_NACK;
                }
            }

            /* Clean-up interrupt sources */
            EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

        #if (EXPANSION_PORT_CY_SCBIP_V0)
            /* Clear interrupt sources as they are not automatically cleared after SCB is disabled */
            EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_NOT_EMPTY);
            EXPANSION_PORT_ClearSlaveInterruptSource(EXPANSION_PORT_INTR_SLAVE_ALL);
        #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

            EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_IDLE;

            EXPANSION_PORT_CTRL_REG |= (uint32) EXPANSION_PORT_CTRL_ENABLED;  /* Enable SCB block */
        }
        else
        {
            /* INTR_RX.NOT_EMPTY */
            /* The slave receives data from the master: accepts into the RX FIFO. At least one entry is available to be
            * read. The offset is written first and all the following bytes are data (expected to be put in the buffer).
            * The slave ACKs all bytes, but it discards them if they do not match the write criteria.
            * The slave NACKs the bytes in the case of an RX FIFO overflow.
            */
            if(0u != (locIntrCause & EXPANSION_PORT_INTR_CAUSE_RX))
            {
                /* Read all entries available in RX FIFO */
                do
                {
                    locByte = EXPANSION_PORT_RX_FIFO_RD_REG;

                    switch(EXPANSION_PORT_fsmState)
                    {

                    case EXPANSION_PORT_EZI2C_FSM_BYTE_WRITE:
                        /* Check buffer index against protect area */
                        if(EXPANSION_PORT_indexBuf1 < EXPANSION_PORT_protectBuf1)
                        {
                            /* Stores received byte into buffer */
                            EXPANSION_PORT_dataBuffer1[EXPANSION_PORT_indexBuf1] = (uint8) locByte;
                            EXPANSION_PORT_indexBuf1++;
                        }
                        else
                        {
                            /* Discard current byte and sets FSM state to discard following bytes */
                            EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_WAIT_STOP;
                        }

                        break;

                #if(EXPANSION_PORT_SUB_ADDRESS_SIZE16_CONST)
                    case EXPANSION_PORT_EZI2C_FSM_OFFSET_HI8:

                        /* Store high byte of offset */
                        locOffset = (uint8) locByte;

                        EXPANSION_PORT_fsmState  = EXPANSION_PORT_EZI2C_FSM_OFFSET_LO8;

                        break;
                #endif /* (EXPANSION_PORT_SUB_ADDRESS_SIZE16_CONST) */

                    case EXPANSION_PORT_EZI2C_FSM_OFFSET_LO8:

                        if(EXPANSION_PORT_SUB_ADDRESS_SIZE16)
                        {
                            /* Append offset with high byte */
                            locByte = ((uint32) ((uint32) locOffset << 8u)) | locByte;
                        }
                        

                        /* Check if offset within buffer range */
                        if(locByte < (uint32) EXPANSION_PORT_bufSizeBuf1)
                        {
                            /* Store and sets received offset */
                            EXPANSION_PORT_offsetBuf1 = (uint16) locByte;
                            EXPANSION_PORT_indexBuf1  = (uint16) locByte;

                            /* Move FSM to data receive state */
                            EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_BYTE_WRITE;
                        }
                        else
                        {
                            /* Reset index due to TX FIFO fill */
                            EXPANSION_PORT_indexBuf1 = (uint16) EXPANSION_PORT_offsetBuf1;

                            /* Discard current byte and sets FSM state to default to discard following bytes */
                            EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_WAIT_STOP;
                        }

                        break;

                    case EXPANSION_PORT_EZI2C_FSM_WAIT_STOP:
                        /* Clear RX FIFO to discard all received data */
                        EXPANSION_PORT_CLEAR_RX_FIFO;

                        break;

                    default:
                        CYASSERT(0u != 0u); /* Should never get there */
                        break;
                    }

                }
                while(0u != EXPANSION_PORT_GET_RX_FIFO_ENTRIES);

                EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_NOT_EMPTY);
            }


            /* INTR_SLAVE.I2C_START */
            /* Catches start of transfer to trigger TX FIFO update event */
            if(0u != (locIntrSlave & EXPANSION_PORT_INTR_SLAVE_I2C_START))
            {
            #if(!EXPANSION_PORT_CY_SCBIP_V0)
                if(EXPANSION_PORT_EZI2C_EC_AM_ENABLE)
                {
                    /* Manage INTR_I2C_EC.WAKE_UP as slave busy status */
                    EXPANSION_PORT_ClearI2CExtClkInterruptSource(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP);
                }
                else
                {
                    /* Manage INTR_SLAVE.I2C_ADDR_MATCH as slave busy status */
                    EXPANSION_PORT_ClearSlaveInterruptSource(EXPANSION_PORT_INTR_SLAVE_I2C_ADDR_MATCH);
                }
                
            #else
                /* Manage INTR_SLAVE.I2C_ADDR_MATCH as slave busy status */
                EXPANSION_PORT_ClearSlaveInterruptSource(EXPANSION_PORT_INTR_SLAVE_I2C_ADDR_MATCH);
            #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

                /* Clear TX FIFO and put a byte */
                EXPANSION_PORT_CLEAR_TX_FIFO;
                EXPANSION_PORT_TX_FIFO_WR_REG = (uint32) EXPANSION_PORT_dataBuffer1[EXPANSION_PORT_offsetBuf1];

                /* Store buffer index to be handled by INTR_SLAVE.I2C_STOP */
                locByte = (uint32) EXPANSION_PORT_indexBuf1;

                /* Update index: one byte is already in the TX FIFO */
                EXPANSION_PORT_indexBuf1 = (uint16) EXPANSION_PORT_offsetBuf1 + 1u;

                /* Enable INTR_TX.NOT_FULL to load TX FIFO */
                EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_INTR_TX_TRIGGER);

                /* Clear locIntrSlave after INTR.TX_TRIGGER is enabled */
                EXPANSION_PORT_ClearSlaveInterruptSource(locIntrSlave);

                locIntrCause |= EXPANSION_PORT_INTR_CAUSE_TX;
            }


            /* INTR_TX.TRIGGER */
            /* Transmits data to the master: loads data into the TX FIFO. The TX FIFO is loaded with data
            *  until used entries are less than EXPANSION_PORT_TX_LOAD_SIZE. If index reaches end of the
            *  buffer the 0xFF is sent to the end of transfer.
            */
            if(0u != (locIntrCause & EXPANSION_PORT_INTR_CAUSE_TX))
            {
                /* Put data into TX FIFO until there is room */
                do
                {
                    /* Check transmit buffer range */
                    if(EXPANSION_PORT_indexBuf1 < EXPANSION_PORT_bufSizeBuf1)
                    {
                        EXPANSION_PORT_TX_FIFO_WR_REG = (uint32) EXPANSION_PORT_dataBuffer1[EXPANSION_PORT_indexBuf1];
                        EXPANSION_PORT_indexBuf1++;
                    }
                    else
                    {
                        EXPANSION_PORT_TX_FIFO_WR_REG = EXPANSION_PORT_EZI2C_OVFL_RETURN;
                    }

                }
                while(EXPANSION_PORT_TX_LOAD_SIZE != EXPANSION_PORT_GET_TX_FIFO_ENTRIES);

                EXPANSION_PORT_ClearTxInterruptSource(EXPANSION_PORT_INTR_TX_TRIGGER);
            }


            /* INTR_SLAVE.I2C_STOP */
            /* Catch completion of write or read transfer. */
            if(0u != (locIntrSlave & EXPANSION_PORT_INTR_SLAVE_I2C_STOP))
            {
                if(0u == (locIntrSlave & EXPANSION_PORT_INTR_SLAVE_I2C_START))
                {
                #if(!EXPANSION_PORT_CY_SCBIP_V0)
                    if(EXPANSION_PORT_EZI2C_EC_AM_ENABLE)
                    {
                        /* Manage INTR_I2C_EC.WAKE_UP as slave busy status */
                        EXPANSION_PORT_ClearI2CExtClkInterruptSource(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP);
                    }
                    
                #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */

                    /* Manage INTR_SLAVE.I2C_ADDR_MATCH as slave busy status */
                    EXPANSION_PORT_ClearSlaveInterruptSource(locIntrSlave);

                    /* Read current buffer index */
                    locByte = (uint32) EXPANSION_PORT_indexBuf1;
                }

                /* Set read completion mask */
                locStatus = EXPANSION_PORT_INTR_SLAVE_I2C_NACK;

                if((locByte != EXPANSION_PORT_offsetBuf1) &&
                   (0u != (EXPANSION_PORT_EZI2C_FSM_WRITE_MASK & EXPANSION_PORT_fsmState)))
                {
                    /* Set write completion mask */
                    locStatus |= EXPANSION_PORT_INTR_SLAVE_I2C_WRITE_STOP;
                }

                /* Set completion flags in the status variable */
                EXPANSION_PORT_curStatus |= (uint8) (locStatus & locIntrSlave);

                EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_IDLE;
            }


        #if(!EXPANSION_PORT_CY_SCBIP_V0)
            if(EXPANSION_PORT_EZI2C_EC_AM_ENABLE)
            {
                /* INTR_I2C_EC.WAKE_UP */
                /* Wake up device from deep sleep on address match event. The matched address is NACKed */
                if(0u != (locIntrCause & EXPANSION_PORT_INTR_CAUSE_I2C_EC))
                {
                    EXPANSION_PORT_I2C_SLAVE_GENERATE_NACK; /* NACK in active mode */
                    EXPANSION_PORT_ClearI2CExtClkInterruptSource(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP);
                }
            }
            
        #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */
        }
    }
#endif /* (EXPANSION_PORT_EZI2C_SCL_STRETCH_DISABLE_CONST) */


/* [] END OF FILE */
