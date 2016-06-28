/*******************************************************************************
* File Name: USER_PORT_EZI2C_INT.c
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

#include "USER_PORT_PVT.h"
#include "USER_PORT_EZI2C_PVT.h"

#if(USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: USER_PORT_EZI2C_STRETCH_ISR
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
    CY_ISR_PROTO(USER_PORT_EZI2C_STRETCH_ISR)
    {
        static uint16 locBufSize;
        uint32 locIndex;
        uint32 locStatus;

        uint32 endTransfer;
        uint32 fifoIndex;
        uint32 locByte;

        uint32 locIntrCause;
        uint32 locIntrSlave;

    #if(USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
        /* Variable intended to be used with either buffer */
        static volatile uint8 * USER_PORT_dataBuffer; /* Pointer to data buffer              */
        static uint16 USER_PORT_bufSizeBuf;           /* Size of buffer in bytes             */
        static uint16 USER_PORT_protectBuf;           /* Start index of write protected area */

        static uint8 activeAddress;
        uint32 ackResponse;

        ackResponse = USER_PORT_EZI2C_ACK_RECEIVED_ADDRESS;
    #endif /* (USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */

    #if !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER)
        if(NULL != USER_PORT_customIntrHandler)
        {
            USER_PORT_customIntrHandler();
        }
    #else
        CY_USER_PORT_CUSTOM_INTR_HANDLER();
    #endif /* !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER) */

        /* Make local copy of global variable */
        locIndex = USER_PORT_EZI2C_GET_INDEX(activeAddress);

        /* Get interrupt sources */
        locIntrSlave = USER_PORT_GetSlaveInterruptSource();
        locIntrCause = USER_PORT_GetInterruptCause();

        /* INTR_SLAVE.I2C_ARB_LOST and INTR_SLAVE_I2C.BUS_ERROR */
        /* Handles errors on the bus. There are cases when both bits are set.
        * The error recovery is common: re-enable the scb IP. The content of the RX FIFO is lost.
        */
        if(0u != (locIntrSlave & (USER_PORT_INTR_SLAVE_I2C_ARB_LOST |
                                  USER_PORT_INTR_SLAVE_I2C_BUS_ERROR)))
        {
            USER_PORT_CTRL_REG &= (uint32) ~USER_PORT_CTRL_ENABLED; /* Disable SCB block */

        #if(USER_PORT_CY_SCBIP_V0)
            if(0u != ((uint8) USER_PORT_EZI2C_STATUS_BUSY & USER_PORT_curStatus))
        #endif /* (USER_PORT_CY_SCBIP_V0) */
            {
                USER_PORT_curStatus &= (uint8) ~USER_PORT_EZI2C_STATUS_BUSY;
                USER_PORT_curStatus |= (uint8)  USER_PORT_EZI2C_STATUS_ERR;

                /* INTR_TX_EMPTY is enabled in the address phase to receive data */
                if(0u == (USER_PORT_GetTxInterruptMode() & USER_PORT_INTR_TX_EMPTY))
                {
                    /* Write complete */
                    if(USER_PORT_indexBuf1 != USER_PORT_offsetBuf1)
                    {
                        USER_PORT_curStatus |= (uint8) USER_PORT_INTR_SLAVE_I2C_WRITE_STOP;
                    }
                }
                else
                {
                    /* Read complete */
                    USER_PORT_curStatus |= (uint8) USER_PORT_INTR_SLAVE_I2C_NACK;
                }
            }

            USER_PORT_DISABLE_SLAVE_AUTO_DATA;

            /* Disable TX and RX interrupt sources */
            USER_PORT_SetRxInterruptMode(USER_PORT_NO_INTR_SOURCES);
            USER_PORT_SetTxInterruptMode(USER_PORT_NO_INTR_SOURCES);

        #if(USER_PORT_CY_SCBIP_V0)
            /* Clear interrupt sources as they are not automatically cleared after SCB is disabled */
            USER_PORT_ClearRxInterruptSource(USER_PORT_INTR_RX_ALL);
            USER_PORT_ClearSlaveInterruptSource(USER_PORT_INTR_SLAVE_ALL);
        #endif /* (USER_PORT_CY_SCBIP_V0) */

            USER_PORT_fsmState = USER_PORT_EZI2C_FSM_IDLE;

            USER_PORT_CTRL_REG |= (uint32) USER_PORT_CTRL_ENABLED;  /* Enable SCB block */
        }
        else
        {
            /* INTR_I2C_EC_WAKE_UP */
            /* Wakes up device from deep sleep */
            if(0u != (locIntrCause & USER_PORT_INTR_CAUSE_I2C_EC))
            {
                /* Disables wakeup interrupt source but does not clear it. It is cleared in INTR_SLAVE_I2C_ADDR_MATCH */
                USER_PORT_SetI2CExtClkInterruptMode(USER_PORT_NO_INTR_SOURCES);
            }

            if(0u != (locIntrCause & (USER_PORT_INTR_CAUSE_RX | USER_PORT_INTR_CAUSE_SLAVE)))
            {
                /* INTR_RX.NOT_EMPTY */
                /* Receives data byte-by-byte. Does not use RX FIFO capabilities */
                if (0u != (USER_PORT_GetRxInterruptSourceMasked() & USER_PORT_INTR_RX_NOT_EMPTY))
                {
                #if(USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
                    /* If I2C_STOP service is delayed to I2C_ADDR_MATCH the address byte is in the RX FIFO and
                    * RX_NOT_EMPTY is enabled. The address byte has to stay into RX FIFO therefore
                    * RX.NOT_EMPTY service has to be skipped. The address byte has to be read by I2C_ADDR_MATCH.
                    */
                    if (0u == (locIntrCause & USER_PORT_INTR_CAUSE_SLAVE))
                #endif /* (USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */
                    {
                        locByte = USER_PORT_RX_FIFO_RD_REG;

                        switch(USER_PORT_fsmState)
                        {
                        case USER_PORT_EZI2C_FSM_BYTE_WRITE:
                            if(0u != locBufSize)
                            {
                                /* Store data byte and ACK */
                                USER_PORT_I2C_SLAVE_GENERATE_ACK;

                                USER_PORT_dataBuffer[locIndex] = (uint8) locByte;
                                locIndex++;
                                locBufSize--;
                            }
                            else
                            {
                                /* Discard data byte and NACK */
                                USER_PORT_I2C_SLAVE_GENERATE_NACK;
                            }
                            break;

                    #if(USER_PORT_SUB_ADDRESS_SIZE16_CONST)
                        case USER_PORT_EZI2C_FSM_OFFSET_HI8:

                            USER_PORT_I2C_SLAVE_GENERATE_ACK;

                            /* Store offset most significant byre */
                            locBufSize = (uint16) ((uint8) locByte);

                            USER_PORT_fsmState = USER_PORT_EZI2C_FSM_OFFSET_LO8;

                            break;
                    #endif /* (USER_PORT_SUB_ADDRESS_SIZE16_CONST) */

                        case USER_PORT_EZI2C_FSM_OFFSET_LO8:

                            if (USER_PORT_SUB_ADDRESS_SIZE16)
                            {
                                /* Collect 2 bytes offset */
                                locByte = ((uint32) ((uint32) locBufSize << 8u)) | locByte;
                            }
                            

                            /* Check offset against buffer size */
                            if(locByte < (uint32) USER_PORT_bufSizeBuf)
                            {
                                USER_PORT_I2C_SLAVE_GENERATE_ACK;

                                /* Update local buffer index with new offset */
                                locIndex = locByte;

                                /* Get available buffer size to write */
                                locBufSize = (uint16) ((locByte < USER_PORT_protectBuf) ?
                                                       (USER_PORT_protectBuf - locByte) : (0u));

                            #if(USER_PORT_CY_SCBIP_V0)

                                if(locBufSize < USER_PORT_EZI2C_FIFO_SIZE)
                                {
                                    /* Set FSM state to receive byte by byte */
                                    USER_PORT_fsmState = USER_PORT_EZI2C_FSM_BYTE_WRITE;
                                }
                                /* Receive RX FIFO chunks */
                                else if(locBufSize == USER_PORT_EZI2C_FIFO_SIZE)
                                {
                                    USER_PORT_ENABLE_SLAVE_AUTO_DATA; /* NACK when RX FIFO is full */
                                    USER_PORT_SetRxInterruptMode(USER_PORT_NO_INTR_SOURCES);
                                }
                                else
                                {
                                    USER_PORT_ENABLE_SLAVE_AUTO_DATA_ACK; /* Stretch when RX FIFO is full */
                                    USER_PORT_SetRxInterruptMode(USER_PORT_INTR_RX_FULL);
                                }

                            #else

                                if(USER_PORT_SECONDARY_ADDRESS_ENABLE)
                                {
                                    /* Set FSM state to receive byte by byte.
                                    * The byte by byte receive is always chosen for two addresses. Ticket ID#175559.
                                    */
                                    USER_PORT_fsmState = USER_PORT_EZI2C_FSM_BYTE_WRITE;
                                }
                                else
                                {
                                    if (locBufSize < USER_PORT_EZI2C_FIFO_SIZE)
                                    {
                                        /* Set FSM state to receive byte by byte */
                                        USER_PORT_fsmState = USER_PORT_EZI2C_FSM_BYTE_WRITE;
                                    }
                                    /* Receive RX FIFO chunks */
                                    else if (locBufSize == USER_PORT_EZI2C_FIFO_SIZE)
                                    {
                                        USER_PORT_ENABLE_SLAVE_AUTO_DATA; /* NACK when RX FIFO is full */
                                        USER_PORT_SetRxInterruptMode(USER_PORT_NO_INTR_SOURCES);
                                    }
                                    else
                                    {
                                        USER_PORT_ENABLE_SLAVE_AUTO_DATA_ACK; /* Stretch when RX FIFO is full */
                                        USER_PORT_SetRxInterruptMode(USER_PORT_INTR_RX_FULL);
                                    }
                                }
                                

                            #endif /* (USER_PORT_CY_SCBIP_V0) */

                                /* Store local offset into global variable */
                                USER_PORT_EZI2C_SET_OFFSET(activeAddress, locIndex);
                            }
                            else
                            {
                                /* Discard offset byte and NACK */
                                USER_PORT_I2C_SLAVE_GENERATE_NACK;
                            }
                            break;

                        default:
                            CYASSERT(0u != 0u); /* Should never get there */
                            break;
                        }

                        USER_PORT_ClearRxInterruptSource(USER_PORT_INTR_RX_NOT_EMPTY);
                    }
                }
                /* INTR_RX.FULL, INTR_SLAVE.I2C_STOP */
                /* Receive FIFO chunks: auto data ACK is enabled */
                else if (0u != (USER_PORT_I2C_CTRL_REG & USER_PORT_I2C_CTRL_S_READY_DATA_ACK))
                {
                    /* Slave interrupt (I2C_STOP or I2C_ADDR_MATCH) leads to completion of read.
                    * A completion event has a higher priority than the FIFO full.
                    * Read remaining data from RX FIFO.
                    */
                    if(0u != (locIntrCause & USER_PORT_INTR_CAUSE_SLAVE))
                    {
                        /* Read remaining bytes from RX FIFO */
                        fifoIndex = USER_PORT_GET_RX_FIFO_ENTRIES;

                        if(USER_PORT_SECONDARY_ADDRESS_ENABLE)
                        {
                            /* Update with current address match */
                            if(USER_PORT_CHECK_INTR_SLAVE_MASKED(USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
                            {
                                /* Update RX FIFO entries as address byte is there now */
                                fifoIndex = USER_PORT_GET_RX_FIFO_ENTRIES;

                                /* If SR is valid, RX FIFO is full and address is in SHIFTER:
                                * read 8 entries and leave address in RX FIFO for further processing.
                                * If SR is invalid, address is already in RX FIFO: read (entries-1).
                                */
                                fifoIndex -= ((0u != USER_PORT_GET_RX_FIFO_SR_VALID) ? (0u) : (1u));
                            }
                        }
                        

                        USER_PORT_DISABLE_SLAVE_AUTO_DATA;
                        endTransfer = USER_PORT_EZI2C_CONTINUE_TRANSFER;
                    }
                    else
                    /* INTR_RX_FULL */
                    /* Continue transfer or disable INTR_RX_FULL to catch completion event. */
                    {
                        /* Calculate buffer size available to write data into */
                        locBufSize -= (uint16) USER_PORT_EZI2C_FIFO_SIZE;

                        if(locBufSize <= USER_PORT_EZI2C_FIFO_SIZE)
                        {
                            /* Send NACK when RX FIFO overflow */
                            fifoIndex   = locBufSize;
                            endTransfer = USER_PORT_EZI2C_COMPLETE_TRANSFER;
                        }
                        else
                        {
                            /* Continue  transaction */
                            fifoIndex   = USER_PORT_EZI2C_FIFO_SIZE;
                            endTransfer = USER_PORT_EZI2C_CONTINUE_TRANSFER;
                        }
                    }

                    for(; (0u != fifoIndex); fifoIndex--)
                    {
                        /* Store data in buffer */
                        USER_PORT_dataBuffer[locIndex] = (uint8) USER_PORT_RX_FIFO_RD_REG;
                        locIndex++;
                    }

                    /* Complete transfer sending NACK when RX FIFO overflow */
                    if(USER_PORT_EZI2C_COMPLETE_TRANSFER == endTransfer)
                    {
                        USER_PORT_ENABLE_SLAVE_AUTO_DATA_NACK;

                        /* Disable INTR_RX_FULL during last RX FIFO chunk reception */
                        USER_PORT_SetRxInterruptMode(USER_PORT_NO_INTR_SOURCES);
                    }

                    USER_PORT_ClearRxInterruptSource(USER_PORT_INTR_RX_FULL |
                                                            USER_PORT_INTR_RX_NOT_EMPTY);
                }
                else
                {
                    /* Exit for slave interrupts which are not active for RX direction:
                    * INTR_SLAVE.I2C_ADDR_MATCH and INTR_SLAVE.I2C_STOP while byte-by-byte reception.
                    */
                }
            }

            if(0u != (locIntrCause & USER_PORT_INTR_CAUSE_SLAVE))
            {
                /* INTR_SLAVE.I2C_STOP */
                /* Catch Stop condition: completion of write or read transfer */
            #if(!USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
                if(0u != (locIntrSlave & USER_PORT_INTR_SLAVE_I2C_STOP))
            #else
                /* Prevent triggering when matched address was NACKed */
                if((0u != (locIntrSlave & USER_PORT_INTR_SLAVE_I2C_STOP)) &&
                   (0u != ((uint8) USER_PORT_EZI2C_STATUS_BUSY & USER_PORT_curStatus)))
            #endif
                {
                    /* Disable TX and RX interrupt sources */
                    USER_PORT_SetRxInterruptMode(USER_PORT_NO_INTR_SOURCES);
                    USER_PORT_SetTxInterruptMode(USER_PORT_NO_INTR_SOURCES);

                    /* Set read completion mask */
                    locStatus = USER_PORT_INTR_SLAVE_I2C_NACK;

                    /* Check if buffer content was modified: the address phase resets the locIndex */
                    if(locIndex != USER_PORT_EZI2C_GET_OFFSET(activeAddress))
                    {
                        locStatus |= USER_PORT_INTR_SLAVE_I2C_WRITE_STOP;
                    }

                    /* Complete read or write transaction */
                    locStatus &= locIntrSlave;
                    USER_PORT_EZI2C_UPDATE_LOC_STATUS(activeAddress, locStatus);
                    locStatus |= (uint32)  USER_PORT_curStatus;
                    locStatus &= (uint32) ~USER_PORT_EZI2C_STATUS_BUSY;
                    USER_PORT_curStatus = (uint8) locStatus;

                    USER_PORT_fsmState = USER_PORT_EZI2C_FSM_IDLE;

                    if(USER_PORT_SECONDARY_ADDRESS_ENABLE)
                    {
                        /* Store local index into global variable, before address phase */
                        USER_PORT_EZI2C_SET_INDEX(activeAddress, locIndex);
                    }
                    
                }

                /* INTR_SLAVE.I2C_ADDR_MATCH */
                /* The matched address is received: the slave starts its operation.
                * INTR_SLAVE_I2C_STOP updates the buffer index before the address phase for two addresses mode.
                * This is done to update buffer index correctly before the address phase changes it.
                */
                if(0u != (locIntrSlave & USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
                {
                    if(USER_PORT_SECONDARY_ADDRESS_ENABLE)
                    {
                        /* Read address byte from RX FIFO */
                        locByte = USER_PORT_GET_I2C_7BIT_ADDRESS(USER_PORT_RX_FIFO_RD_REG);

                        USER_PORT_ClearRxInterruptSource(USER_PORT_INTR_RX_NOT_EMPTY);

                        /* Check received address against device addresses */
                        if(USER_PORT_addrBuf1 == locByte)
                        {
                            /* Set buffer exposed to primary slave address */
                            USER_PORT_dataBuffer = USER_PORT_dataBuffer1;
                            USER_PORT_bufSizeBuf = USER_PORT_bufSizeBuf1;
                            USER_PORT_protectBuf = USER_PORT_protectBuf1;

                            activeAddress = USER_PORT_EZI2C_ACTIVE_ADDRESS1;
                        }
                        else if(USER_PORT_addrBuf2 == locByte)
                        {
                            /* Set buffer exposed to secondary slave address */
                            USER_PORT_dataBuffer = USER_PORT_dataBuffer2;
                            USER_PORT_bufSizeBuf = USER_PORT_bufSizeBuf2;
                            USER_PORT_protectBuf = USER_PORT_protectBuf2;

                            activeAddress = USER_PORT_EZI2C_ACTIVE_ADDRESS2;
                        }
                        else
                        {
                            /* Address does not match */
                            ackResponse = USER_PORT_EZI2C_NACK_RECEIVED_ADDRESS;
                        }
                    }
                    

                #if(USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
                    if(USER_PORT_EZI2C_NACK_RECEIVED_ADDRESS == ackResponse)
                    {
                        /* Clear interrupt sources before NACK address */
                        USER_PORT_ClearI2CExtClkInterruptSource(USER_PORT_INTR_I2C_EC_WAKE_UP);
                        USER_PORT_ClearSlaveInterruptSource(USER_PORT_INTR_SLAVE_ALL);

                    #if(!USER_PORT_CY_SCBIP_V0)
                        /* Disable INTR_I2C_STOP to not trigger after matched address is NACKed. Ticket ID#156094 */
                        USER_PORT_DISABLE_INTR_SLAVE(USER_PORT_INTR_SLAVE_I2C_STOP);
                    #endif /* (!USER_PORT_CY_SCBIP_V0) */

                        /* NACK address byte: it does not match neither primary nor secondary */
                        USER_PORT_I2C_SLAVE_GENERATE_NACK;
                    }
                    else
                #endif /* (USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */
                    {

                    #if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
                        if(!USER_PORT_SECONDARY_ADDRESS_ENABLE)
                        {
                            /* Set buffer exposed to primary slave address */
                            USER_PORT_dataBuffer = USER_PORT_dataBuffer1;
                            USER_PORT_bufSizeBuf = USER_PORT_bufSizeBuf1;
                            USER_PORT_protectBuf = USER_PORT_protectBuf1;

                            activeAddress = USER_PORT_EZI2C_ACTIVE_ADDRESS1;
                        }
                    #endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

                        /* Bus becomes busy after address is received */
                        USER_PORT_curStatus |= (uint8) USER_PORT_EZI2C_STATUS_BUSY;

                        /* Slave is read or written: set current offset */
                        locIndex = USER_PORT_EZI2C_GET_OFFSET(activeAddress);

                        /* Check transaction direction */
                        if(USER_PORT_CHECK_I2C_STATUS(USER_PORT_I2C_STATUS_S_READ))
                        {
                            /* Calculate slave buffer size */
                            locBufSize = USER_PORT_bufSizeBuf - (uint16) locIndex;

                            /* Clear TX FIFO to start fill from offset */
                            USER_PORT_CLEAR_TX_FIFO;
                            USER_PORT_SetTxInterruptMode(USER_PORT_INTR_TX_EMPTY);
                        }
                        else
                        {
                            /* Master writes: enable reception interrupt. The FSM state was set in INTR_SLAVE_I2C_STOP*/
                            USER_PORT_SetRxInterruptMode(USER_PORT_INTR_RX_NOT_EMPTY);
                        }

                        /* Clear interrupt sources before ACK address */
                        USER_PORT_ClearI2CExtClkInterruptSource(USER_PORT_INTR_I2C_EC_WAKE_UP);
                        USER_PORT_ClearSlaveInterruptSource(USER_PORT_INTR_SLAVE_ALL);

                    #if (!USER_PORT_CY_SCBIP_V0)
                        /* Enable STOP to trigger after address match is ACKed. Ticket ID#156094 */
                        USER_PORT_ENABLE_INTR_SLAVE(USER_PORT_INTR_SLAVE_I2C_STOP);
                    #endif /* (!USER_PORT_CY_SCBIP_V0) */

                        /* ACK the address byte */
                        USER_PORT_I2C_SLAVE_GENERATE_ACK;
                    }
                }

                /* Clear slave interrupt sources */
                USER_PORT_ClearSlaveInterruptSource(locIntrSlave);
            }

            /* INTR_TX.EMPTY */
            /* Transmits data to the master: loads data into the TX FIFO. The 0xFF sends out if the master reads
            * out the buffer. The address reception with a read flag clears the TX FIFO to be loaded with data.
            */
            if(0u != (USER_PORT_GetInterruptCause() & USER_PORT_INTR_CAUSE_TX))
            {
                /* Put data into TX FIFO until there is a room */
                do
                {
                    /* Check transmit buffer range: locBufSize calculates after address reception */
                    if(0u != locBufSize)
                    {
                        USER_PORT_TX_FIFO_WR_REG = (uint32) USER_PORT_dataBuffer[locIndex];
                        locIndex++;
                        locBufSize--;
                    }
                    else
                    {
                        USER_PORT_TX_FIFO_WR_REG = USER_PORT_EZI2C_OVFL_RETURN;
                    }
                }
                while(USER_PORT_EZI2C_FIFO_SIZE != USER_PORT_GET_TX_FIFO_ENTRIES);

                USER_PORT_ClearTxInterruptSource(USER_PORT_INTR_TX_EMPTY);
            }
        }

        /* Store local index copy into global variable */
        USER_PORT_EZI2C_SET_INDEX(activeAddress, locIndex);
    }
#endif /* (USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST) */


#if(USER_PORT_EZI2C_SCL_STRETCH_DISABLE_CONST)
    /*******************************************************************************
    * Function Name: USER_PORT_EZI2C_NO_STRETCH_ISR
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
    CY_ISR_PROTO(USER_PORT_EZI2C_NO_STRETCH_ISR)
    {
    #if(USER_PORT_SUB_ADDRESS_SIZE16_CONST)
        static uint8 locOffset;
    #endif /* (USER_PORT_SUB_ADDRESS_SIZE16_CONST) */

        uint32 locByte;
        uint32 locStatus;
        uint32 locIntrSlave;
        uint32 locIntrCause;

    #if !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER)
        /* Calls registered customer routine to manage interrupt sources */
        if(NULL != USER_PORT_customIntrHandler)
        {
            USER_PORT_customIntrHandler();
        }
    #else
        CY_USER_PORT_CUSTOM_INTR_HANDLER();
    #endif /* !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER) */

        locByte = 0u;

        /* Get copy of triggered slave interrupt sources */
        locIntrSlave = USER_PORT_GetSlaveInterruptSource();
        locIntrCause = USER_PORT_GetInterruptCause();

        /* INTR_SLAVE.I2C_ARB_LOST and INTR_SLAVE.I2C_BUS_ERROR */
        /* Handles errors on the bus: There are cases when both bits are set.
        * The error recovery is common: re-enable the scb IP. The content of the RX FIFO is lost.
        */
        if(0u != (locIntrSlave & (USER_PORT_INTR_SLAVE_I2C_ARB_LOST |
                                  USER_PORT_INTR_SLAVE_I2C_BUS_ERROR)))
        {
            USER_PORT_CTRL_REG &= (uint32) ~USER_PORT_CTRL_ENABLED; /* Disable SCB block */

        #if (USER_PORT_CY_SCBIP_V0)
            if(0u != (locIntrSlave & USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
        #endif /* (USER_PORT_CY_SCBIP_V0) */
            {
                USER_PORT_curStatus |= (uint8) USER_PORT_EZI2C_STATUS_ERR;

                if(0u != (USER_PORT_EZI2C_FSM_WRITE_MASK & USER_PORT_fsmState))
                {
                    /* Write complete */
                    if(USER_PORT_indexBuf1 != USER_PORT_offsetBuf1)
                    {
                        USER_PORT_curStatus |= (uint8) USER_PORT_INTR_SLAVE_I2C_WRITE_STOP;
                    }
                }
                else
                {
                    /* Read complete */
                    USER_PORT_curStatus |= (uint8) USER_PORT_INTR_SLAVE_I2C_NACK;
                }
            }

            /* Clean-up interrupt sources */
            USER_PORT_SetTxInterruptMode(USER_PORT_NO_INTR_SOURCES);

        #if (USER_PORT_CY_SCBIP_V0)
            /* Clear interrupt sources as they are not automatically cleared after SCB is disabled */
            USER_PORT_ClearRxInterruptSource(USER_PORT_INTR_RX_NOT_EMPTY);
            USER_PORT_ClearSlaveInterruptSource(USER_PORT_INTR_SLAVE_ALL);
        #endif /* (USER_PORT_CY_SCBIP_V0) */

            USER_PORT_fsmState = USER_PORT_EZI2C_FSM_IDLE;

            USER_PORT_CTRL_REG |= (uint32) USER_PORT_CTRL_ENABLED;  /* Enable SCB block */
        }
        else
        {
            /* INTR_RX.NOT_EMPTY */
            /* The slave receives data from the master: accepts into the RX FIFO. At least one entry is available to be
            * read. The offset is written first and all the following bytes are data (expected to be put in the buffer).
            * The slave ACKs all bytes, but it discards them if they do not match the write criteria.
            * The slave NACKs the bytes in the case of an RX FIFO overflow.
            */
            if(0u != (locIntrCause & USER_PORT_INTR_CAUSE_RX))
            {
                /* Read all entries available in RX FIFO */
                do
                {
                    locByte = USER_PORT_RX_FIFO_RD_REG;

                    switch(USER_PORT_fsmState)
                    {

                    case USER_PORT_EZI2C_FSM_BYTE_WRITE:
                        /* Check buffer index against protect area */
                        if(USER_PORT_indexBuf1 < USER_PORT_protectBuf1)
                        {
                            /* Stores received byte into buffer */
                            USER_PORT_dataBuffer1[USER_PORT_indexBuf1] = (uint8) locByte;
                            USER_PORT_indexBuf1++;
                        }
                        else
                        {
                            /* Discard current byte and sets FSM state to discard following bytes */
                            USER_PORT_fsmState = USER_PORT_EZI2C_FSM_WAIT_STOP;
                        }

                        break;

                #if(USER_PORT_SUB_ADDRESS_SIZE16_CONST)
                    case USER_PORT_EZI2C_FSM_OFFSET_HI8:

                        /* Store high byte of offset */
                        locOffset = (uint8) locByte;

                        USER_PORT_fsmState  = USER_PORT_EZI2C_FSM_OFFSET_LO8;

                        break;
                #endif /* (USER_PORT_SUB_ADDRESS_SIZE16_CONST) */

                    case USER_PORT_EZI2C_FSM_OFFSET_LO8:

                        if(USER_PORT_SUB_ADDRESS_SIZE16)
                        {
                            /* Append offset with high byte */
                            locByte = ((uint32) ((uint32) locOffset << 8u)) | locByte;
                        }
                        

                        /* Check if offset within buffer range */
                        if(locByte < (uint32) USER_PORT_bufSizeBuf1)
                        {
                            /* Store and sets received offset */
                            USER_PORT_offsetBuf1 = (uint16) locByte;
                            USER_PORT_indexBuf1  = (uint16) locByte;

                            /* Move FSM to data receive state */
                            USER_PORT_fsmState = USER_PORT_EZI2C_FSM_BYTE_WRITE;
                        }
                        else
                        {
                            /* Reset index due to TX FIFO fill */
                            USER_PORT_indexBuf1 = (uint16) USER_PORT_offsetBuf1;

                            /* Discard current byte and sets FSM state to default to discard following bytes */
                            USER_PORT_fsmState = USER_PORT_EZI2C_FSM_WAIT_STOP;
                        }

                        break;

                    case USER_PORT_EZI2C_FSM_WAIT_STOP:
                        /* Clear RX FIFO to discard all received data */
                        USER_PORT_CLEAR_RX_FIFO;

                        break;

                    default:
                        CYASSERT(0u != 0u); /* Should never get there */
                        break;
                    }

                }
                while(0u != USER_PORT_GET_RX_FIFO_ENTRIES);

                USER_PORT_ClearRxInterruptSource(USER_PORT_INTR_RX_NOT_EMPTY);
            }


            /* INTR_SLAVE.I2C_START */
            /* Catches start of transfer to trigger TX FIFO update event */
            if(0u != (locIntrSlave & USER_PORT_INTR_SLAVE_I2C_START))
            {
            #if(!USER_PORT_CY_SCBIP_V0)
                if(USER_PORT_EZI2C_EC_AM_ENABLE)
                {
                    /* Manage INTR_I2C_EC.WAKE_UP as slave busy status */
                    USER_PORT_ClearI2CExtClkInterruptSource(USER_PORT_INTR_I2C_EC_WAKE_UP);
                }
                else
                {
                    /* Manage INTR_SLAVE.I2C_ADDR_MATCH as slave busy status */
                    USER_PORT_ClearSlaveInterruptSource(USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH);
                }
                
            #else
                /* Manage INTR_SLAVE.I2C_ADDR_MATCH as slave busy status */
                USER_PORT_ClearSlaveInterruptSource(USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH);
            #endif /* (USER_PORT_CY_SCBIP_V0) */

                /* Clear TX FIFO and put a byte */
                USER_PORT_CLEAR_TX_FIFO;
                USER_PORT_TX_FIFO_WR_REG = (uint32) USER_PORT_dataBuffer1[USER_PORT_offsetBuf1];

                /* Store buffer index to be handled by INTR_SLAVE.I2C_STOP */
                locByte = (uint32) USER_PORT_indexBuf1;

                /* Update index: one byte is already in the TX FIFO */
                USER_PORT_indexBuf1 = (uint16) USER_PORT_offsetBuf1 + 1u;

                /* Enable INTR_TX.NOT_FULL to load TX FIFO */
                USER_PORT_SetTxInterruptMode(USER_PORT_INTR_TX_TRIGGER);

                /* Clear locIntrSlave after INTR.TX_TRIGGER is enabled */
                USER_PORT_ClearSlaveInterruptSource(locIntrSlave);

                locIntrCause |= USER_PORT_INTR_CAUSE_TX;
            }


            /* INTR_TX.TRIGGER */
            /* Transmits data to the master: loads data into the TX FIFO. The TX FIFO is loaded with data
            *  until used entries are less than USER_PORT_TX_LOAD_SIZE. If index reaches end of the
            *  buffer the 0xFF is sent to the end of transfer.
            */
            if(0u != (locIntrCause & USER_PORT_INTR_CAUSE_TX))
            {
                /* Put data into TX FIFO until there is room */
                do
                {
                    /* Check transmit buffer range */
                    if(USER_PORT_indexBuf1 < USER_PORT_bufSizeBuf1)
                    {
                        USER_PORT_TX_FIFO_WR_REG = (uint32) USER_PORT_dataBuffer1[USER_PORT_indexBuf1];
                        USER_PORT_indexBuf1++;
                    }
                    else
                    {
                        USER_PORT_TX_FIFO_WR_REG = USER_PORT_EZI2C_OVFL_RETURN;
                    }

                }
                while(USER_PORT_TX_LOAD_SIZE != USER_PORT_GET_TX_FIFO_ENTRIES);

                USER_PORT_ClearTxInterruptSource(USER_PORT_INTR_TX_TRIGGER);
            }


            /* INTR_SLAVE.I2C_STOP */
            /* Catch completion of write or read transfer. */
            if(0u != (locIntrSlave & USER_PORT_INTR_SLAVE_I2C_STOP))
            {
                if(0u == (locIntrSlave & USER_PORT_INTR_SLAVE_I2C_START))
                {
                #if(!USER_PORT_CY_SCBIP_V0)
                    if(USER_PORT_EZI2C_EC_AM_ENABLE)
                    {
                        /* Manage INTR_I2C_EC.WAKE_UP as slave busy status */
                        USER_PORT_ClearI2CExtClkInterruptSource(USER_PORT_INTR_I2C_EC_WAKE_UP);
                    }
                    
                #endif /* (!USER_PORT_CY_SCBIP_V0) */

                    /* Manage INTR_SLAVE.I2C_ADDR_MATCH as slave busy status */
                    USER_PORT_ClearSlaveInterruptSource(locIntrSlave);

                    /* Read current buffer index */
                    locByte = (uint32) USER_PORT_indexBuf1;
                }

                /* Set read completion mask */
                locStatus = USER_PORT_INTR_SLAVE_I2C_NACK;

                if((locByte != USER_PORT_offsetBuf1) &&
                   (0u != (USER_PORT_EZI2C_FSM_WRITE_MASK & USER_PORT_fsmState)))
                {
                    /* Set write completion mask */
                    locStatus |= USER_PORT_INTR_SLAVE_I2C_WRITE_STOP;
                }

                /* Set completion flags in the status variable */
                USER_PORT_curStatus |= (uint8) (locStatus & locIntrSlave);

                USER_PORT_fsmState = USER_PORT_EZI2C_FSM_IDLE;
            }


        #if(!USER_PORT_CY_SCBIP_V0)
            if(USER_PORT_EZI2C_EC_AM_ENABLE)
            {
                /* INTR_I2C_EC.WAKE_UP */
                /* Wake up device from deep sleep on address match event. The matched address is NACKed */
                if(0u != (locIntrCause & USER_PORT_INTR_CAUSE_I2C_EC))
                {
                    USER_PORT_I2C_SLAVE_GENERATE_NACK; /* NACK in active mode */
                    USER_PORT_ClearI2CExtClkInterruptSource(USER_PORT_INTR_I2C_EC_WAKE_UP);
                }
            }
            
        #endif /* (!USER_PORT_CY_SCBIP_V0) */
        }
    }
#endif /* (USER_PORT_EZI2C_SCL_STRETCH_DISABLE_CONST) */


/* [] END OF FILE */
