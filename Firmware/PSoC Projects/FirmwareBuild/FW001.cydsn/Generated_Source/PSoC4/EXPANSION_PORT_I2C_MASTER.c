/*******************************************************************************
* File Name: EXPANSION_PORT_I2C_MASTER.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  I2C Master mode.
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
#include "EXPANSION_PORT_I2C_PVT.h"

#if(EXPANSION_PORT_I2C_MASTER_CONST)

/***************************************
*      I2C Master Private Vars
***************************************/

/* Master variables */
volatile uint16 EXPANSION_PORT_mstrStatus;      /* Master Status byte  */
volatile uint8  EXPANSION_PORT_mstrControl;     /* Master Control byte */

/* Receive buffer variables */
volatile uint8 * EXPANSION_PORT_mstrRdBufPtr;   /* Pointer to Master Read buffer */
volatile uint32  EXPANSION_PORT_mstrRdBufSize;  /* Master Read buffer size       */
volatile uint32  EXPANSION_PORT_mstrRdBufIndex; /* Master Read buffer Index      */

/* Transmit buffer variables */
volatile uint8 * EXPANSION_PORT_mstrWrBufPtr;   /* Pointer to Master Write buffer */
volatile uint32  EXPANSION_PORT_mstrWrBufSize;  /* Master Write buffer size       */
volatile uint32  EXPANSION_PORT_mstrWrBufIndex; /* Master Write buffer Index      */
volatile uint32  EXPANSION_PORT_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */

#if (!EXPANSION_PORT_CY_SCBIP_V0 && \
    EXPANSION_PORT_I2C_MULTI_MASTER_SLAVE_CONST && EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
    static void EXPANSION_PORT_I2CMasterDisableEcAm(void);
#endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterWriteBuf
********************************************************************************
*
* Summary:
* Automatically writes an entire buffer of data to a slave device.
* Once the data transfer is initiated by this function, further data transfer
* is handled by the included ISR.
* Enables the I2C interrupt and clears SCB_ I2C_MSTAT_WR_CMPLT status.
*
* Parameters:
*  slaveAddr: 7-bit slave address.
*  xferData:  Pointer to buffer of data to be sent.
*  cnt:       Size of buffer to send.
*  mode:      Transfer mode defines: start or restart condition generation at
*             begin of the transfer and complete the transfer or halt before
*             generating a stop.
*
* Return:
*  Error status.
*
* Global variables:
*  EXPANSION_PORT_mstrStatus  - used to store current status of I2C Master.
*  EXPANSION_PORT_state       - used to store current state of software FSM.
*  EXPANSION_PORT_mstrControl - used to control master end of transaction with
*  or without the Stop generation.
*  EXPANSION_PORT_mstrWrBufPtr - used to store pointer to master write buffer.
*  EXPANSION_PORT_mstrWrBufIndex - used to current index within master write
*  buffer.
*  EXPANSION_PORT_mstrWrBufSize - used to store master write buffer size.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterWriteBuf(uint32 slaveAddress, uint8 * wrData, uint32 cnt, uint32 mode)
{
    uint32 errStatus;

    errStatus = EXPANSION_PORT_I2C_MSTR_NOT_READY;

    if(NULL != wrData)  /* Check buffer pointer */
    {
        /* Check FSM state and bus before generating Start/ReStart condition */
        if(EXPANSION_PORT_CHECK_I2C_FSM_IDLE)
        {
            EXPANSION_PORT_DisableInt();  /* Lock from interruption */

            /* Check bus state */
            errStatus = EXPANSION_PORT_CHECK_I2C_STATUS(EXPANSION_PORT_I2C_STATUS_BUS_BUSY) ?
                            EXPANSION_PORT_I2C_MSTR_BUS_BUSY : EXPANSION_PORT_I2C_MSTR_NO_ERROR;
        }
        else if(EXPANSION_PORT_CHECK_I2C_FSM_HALT)
        {
            EXPANSION_PORT_mstrStatus &= (uint16) ~EXPANSION_PORT_I2C_MSTAT_XFER_HALT;
                              errStatus  = EXPANSION_PORT_I2C_MSTR_NO_ERROR;
        }
        else
        {
            /* Unexpected FSM state: exit */
        }
    }

    /* Check if master is ready to start  */
    if(EXPANSION_PORT_I2C_MSTR_NO_ERROR == errStatus) /* No error proceed */
    {
    #if (!EXPANSION_PORT_CY_SCBIP_V0 && \
        EXPANSION_PORT_I2C_MULTI_MASTER_SLAVE_CONST && EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
            EXPANSION_PORT_I2CMasterDisableEcAm();
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */

        /* Set up write transaction */
        EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_MSTR_WR_ADDR;
        EXPANSION_PORT_mstrWrBufIndexTmp = 0u;
        EXPANSION_PORT_mstrWrBufIndex    = 0u;
        EXPANSION_PORT_mstrWrBufSize     = cnt;
        EXPANSION_PORT_mstrWrBufPtr      = (volatile uint8 *) wrData;
        EXPANSION_PORT_mstrControl       = (uint8) mode;

        slaveAddress = EXPANSION_PORT_GET_I2C_8BIT_ADDRESS(slaveAddress);

        EXPANSION_PORT_mstrStatus &= (uint16) ~EXPANSION_PORT_I2C_MSTAT_WR_CMPLT;

        EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_ALL);
        EXPANSION_PORT_ClearTxInterruptSource(EXPANSION_PORT_INTR_TX_UNDERFLOW);
        /* The TX and RX FIFO have to be EMPTY */

        /* Generate Start or ReStart */
        if(EXPANSION_PORT_CHECK_I2C_MODE_RESTART(mode))
        {
            EXPANSION_PORT_I2C_MASTER_GENERATE_RESTART;
            EXPANSION_PORT_TX_FIFO_WR_REG = slaveAddress;
        }
        else
        {
            EXPANSION_PORT_TX_FIFO_WR_REG = slaveAddress;
            EXPANSION_PORT_I2C_MASTER_GENERATE_START;
        }

         /* Catch when address is sent */
        EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_INTR_TX_UNDERFLOW);
    }

    EXPANSION_PORT_EnableInt();   /* Release lock */

    return(errStatus);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterReadBuf
********************************************************************************
*
* Summary:
*  Automatically reads an entire buffer of data from a slave device.
*  Once the data transfer is initiated by this function, further data transfer
*  is handled by the included ISR.
* Enables the I2C interrupt and clears SCB_ I2C_MSTAT_RD_CMPLT status.
*
* Parameters:
*  slaveAddr: 7-bit slave address.
*  xferData:  Pointer to buffer where to put data from slave.
*  cnt:       Size of buffer to read.
*  mode:      Transfer mode defines: start or restart condition generation at
*             begin of the transfer and complete the transfer or halt before
*             generating a stop.
*
* Return:
*  Error status.
*
* Global variables:
*  EXPANSION_PORT_mstrStatus  - used to store current status of I2C Master.
*  EXPANSION_PORT_state       - used to store current state of software FSM.
*  EXPANSION_PORT_mstrControl - used to control master end of transaction with
*  or without the Stop generation.
*  EXPANSION_PORT_mstrRdBufPtr - used to store pointer to master write buffer.
*  EXPANSION_PORT_mstrRdBufIndex - used to current index within master write
*  buffer.
*  EXPANSION_PORT_mstrRdBufSize - used to store master write buffer size.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterReadBuf(uint32 slaveAddress, uint8 * rdData, uint32 cnt, uint32 mode)
{
    uint32 errStatus;

    errStatus = EXPANSION_PORT_I2C_MSTR_NOT_READY;

    if(NULL != rdData)
    {
        /* Check FSM state and bus before generating Start/ReStart condition */
        if(EXPANSION_PORT_CHECK_I2C_FSM_IDLE)
        {
            EXPANSION_PORT_DisableInt();  /* Lock from interruption */

            /* Check bus state */
            errStatus = EXPANSION_PORT_CHECK_I2C_STATUS(EXPANSION_PORT_I2C_STATUS_BUS_BUSY) ?
                            EXPANSION_PORT_I2C_MSTR_BUS_BUSY : EXPANSION_PORT_I2C_MSTR_NO_ERROR;
        }
        else if(EXPANSION_PORT_CHECK_I2C_FSM_HALT)
        {
            EXPANSION_PORT_mstrStatus &= (uint16) ~EXPANSION_PORT_I2C_MSTAT_XFER_HALT;
                              errStatus  =  EXPANSION_PORT_I2C_MSTR_NO_ERROR;
        }
        else
        {
            /* Unexpected FSM state: exit */
        }
    }

    /* Check master ready to proceed */
    if(EXPANSION_PORT_I2C_MSTR_NO_ERROR == errStatus) /* No error proceed */
    {
        #if (!EXPANSION_PORT_CY_SCBIP_V0 && \
        EXPANSION_PORT_I2C_MULTI_MASTER_SLAVE_CONST && EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
            EXPANSION_PORT_I2CMasterDisableEcAm();
        #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */

        /* Set up read transaction */
        EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_MSTR_RD_ADDR;
        EXPANSION_PORT_mstrRdBufIndex = 0u;
        EXPANSION_PORT_mstrRdBufSize  = cnt;
        EXPANSION_PORT_mstrRdBufPtr   = (volatile uint8 *) rdData;
        EXPANSION_PORT_mstrControl    = (uint8) mode;

        slaveAddress = (EXPANSION_PORT_GET_I2C_8BIT_ADDRESS(slaveAddress) | EXPANSION_PORT_I2C_READ_FLAG);

        EXPANSION_PORT_mstrStatus &= (uint16) ~EXPANSION_PORT_I2C_MSTAT_RD_CMPLT;

        EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_ALL);

        /* TX and RX FIFO have to be EMPTY */

        /* Generate Start or ReStart */
        if(EXPANSION_PORT_CHECK_I2C_MODE_RESTART(mode))
        {
            EXPANSION_PORT_I2C_MASTER_GENERATE_RESTART;
            EXPANSION_PORT_TX_FIFO_WR_REG = (slaveAddress);
        }
        else
        {
            EXPANSION_PORT_TX_FIFO_WR_REG = (slaveAddress);
            EXPANSION_PORT_I2C_MASTER_GENERATE_START;
        }

        /* Prepare reading */
        if(EXPANSION_PORT_mstrRdBufSize < EXPANSION_PORT_I2C_FIFO_SIZE) /* Reading byte-by-byte */
        {
            EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_INTR_RX_NOT_EMPTY);
        }
        else /* Receive RX FIFO chunks */
        {
            EXPANSION_PORT_ENABLE_MASTER_AUTO_DATA_ACK;
            EXPANSION_PORT_SetRxInterruptMode(EXPANSION_PORT_INTR_RX_FULL);
        }
    }

    EXPANSION_PORT_EnableInt();   /* Release lock */

    return(errStatus);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterSendStart
********************************************************************************
*
* Summary:
*  Generates Start condition and sends slave address with read/write bit.
*  Disables the I2C interrupt.
*  This function is blocking and does not return until start condition and
*  address byte are sent and ACK/NACK response is received or errors occurred.
*
* Parameters:
*  slaveAddress: Right justified 7-bit Slave address (valid range 8 to 120).
*  bitRnW:       Direction of the following transfer. It is defined by
*                read/write bit within address byte.
*
* Return:
*  Erorr status.
*
* Global variables:
*  EXPANSION_PORT_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterSendStart(uint32 slaveAddress, uint32 bitRnW)
{
    uint32  resetIp;
    uint32 errStatus;

    resetIp   = 0u;
    errStatus = EXPANSION_PORT_I2C_MSTR_NOT_READY;

    /* Check FSM state before generating Start condition */
    if(EXPANSION_PORT_CHECK_I2C_FSM_IDLE)
    {
        /* If bus is free, generate Start condition */
        if(EXPANSION_PORT_CHECK_I2C_STATUS(EXPANSION_PORT_I2C_STATUS_BUS_BUSY))
        {
            errStatus = EXPANSION_PORT_I2C_MSTR_BUS_BUSY;
        }
        else
        {
            EXPANSION_PORT_DisableInt();  /* Lock from interruption */

        #if (!EXPANSION_PORT_CY_SCBIP_V0 && \
            EXPANSION_PORT_I2C_MULTI_MASTER_SLAVE_CONST && EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
            EXPANSION_PORT_I2CMasterDisableEcAm();
        #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */

            slaveAddress = EXPANSION_PORT_GET_I2C_8BIT_ADDRESS(slaveAddress);

            if(0u == bitRnW) /* Write direction */
            {
                EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_MSTR_WR_DATA;
            }
            else /* Read direction */
            {
                EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_MSTR_RD_DATA;
                         slaveAddress |= EXPANSION_PORT_I2C_READ_FLAG;
            }

            /* TX and RX FIFO have to be EMPTY */

            EXPANSION_PORT_TX_FIFO_WR_REG = slaveAddress; /* Put address in TX FIFO */
            EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_ALL);

            EXPANSION_PORT_I2C_MASTER_GENERATE_START;


            while(!EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ACK      |
                                                      EXPANSION_PORT_INTR_MASTER_I2C_NACK     |
                                                      EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                      EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR))
            {
                /*
                * Write: wait until address has been transferred
                * Read : wait until address has been transferred, data byte is going to RX FIFO as well.
                */
            }

            /* Check the results of the address phase */
            if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ACK))
            {
                errStatus = EXPANSION_PORT_I2C_MSTR_NO_ERROR;
            }
            else if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_NACK))
            {
                errStatus = EXPANSION_PORT_I2C_MSTR_ERR_LB_NAK;
            }
            else if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST))
            {
                EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;
                             errStatus = EXPANSION_PORT_I2C_MSTR_ERR_ARB_LOST;
                             resetIp   = EXPANSION_PORT_I2C_RESET_ERROR;
            }
            else /* EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR set is else condition */
            {
                EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;
                             errStatus = EXPANSION_PORT_I2C_MSTR_ERR_BUS_ERR;
                             resetIp   = EXPANSION_PORT_I2C_RESET_ERROR;
            }

            EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_I2C_ACK      |
                                                        EXPANSION_PORT_INTR_MASTER_I2C_NACK     |
                                                        EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                        EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR);

            /* Reset block in case of: LOST_ARB or BUS_ERR */
            if(0u != resetIp)
            {
                EXPANSION_PORT_SCB_SW_RESET;
            }
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterSendRestart
********************************************************************************
*
* Summary:
*  Generates Restart condition and sends slave address with read/write bit.
*  This function is blocking and does not return until start condition and
*  address are sent and ACK/NACK response is received or errors occurred.
*
* Parameters:
*  slaveAddress: Right justified 7-bit Slave address (valid range 8 to 120).
*  bitRnW:       Direction of the following transfer. It is defined by
*                read/write bit within address byte.
*
* Return:
*  Error status
*
*
* Global variables:
*  EXPANSION_PORT_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterSendRestart(uint32 slaveAddress, uint32 bitRnW)
{
    uint32 resetIp;
    uint32 errStatus;

    resetIp   = 0u;
    errStatus = EXPANSION_PORT_I2C_MSTR_NOT_READY;

    /* Check FSM state before generating ReStart condition */
    if(EXPANSION_PORT_CHECK_I2C_MASTER_ACTIVE)
    {
        slaveAddress = EXPANSION_PORT_GET_I2C_8BIT_ADDRESS(slaveAddress);

        if(0u == bitRnW) /* Write direction */
        {
            EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_MSTR_WR_DATA;
        }
        else  /* Read direction */
        {
            EXPANSION_PORT_state  = EXPANSION_PORT_I2C_FSM_MSTR_RD_DATA;
                      slaveAddress |= EXPANSION_PORT_I2C_READ_FLAG;
        }

        /* TX and RX FIFO have to be EMPTY */

        /* Clean-up interrupt status */
        EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_ALL);

        /* A proper ReStart sequence is: generate ReStart, then put an address byte in the TX FIFO.
        * Otherwise the master treats the address in the TX FIFO as a data byte if a previous transfer is write.
        * The write transfer continues instead of ReStart.
        */
        EXPANSION_PORT_I2C_MASTER_GENERATE_RESTART;

        while(EXPANSION_PORT_CHECK_I2C_MASTER_CMD(EXPANSION_PORT_I2C_MASTER_CMD_M_START))
        {
            /* Wait until ReStart has been generated */
        }

        /* Put address into TX FIFO */
        EXPANSION_PORT_TX_FIFO_WR_REG = slaveAddress;

        /* Wait for address to be transferred */
        while(!EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ACK      |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_NACK     |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR))
        {
            /* Wait until address has been transferred */
        }

        /* Check results of address phase */
        if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ACK))
        {
            errStatus = EXPANSION_PORT_I2C_MSTR_NO_ERROR;
        }
        else if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_NACK))
        {
             errStatus = EXPANSION_PORT_I2C_MSTR_ERR_LB_NAK;
        }
        else if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST))
        {
            EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;
                         errStatus = EXPANSION_PORT_I2C_MSTR_ERR_ARB_LOST;
                         resetIp   = EXPANSION_PORT_I2C_RESET_ERROR;
        }
        else /* EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR set is else condition */
        {
            EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;
                         errStatus = EXPANSION_PORT_I2C_MSTR_ERR_BUS_ERR;
                         resetIp   = EXPANSION_PORT_I2C_RESET_ERROR;
        }

        EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_I2C_ACK      |
                                                    EXPANSION_PORT_INTR_MASTER_I2C_NACK     |
                                                    EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                    EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR);

        /* Reset block in case of: LOST_ARB or BUS_ERR */
        if(0u != resetIp)
        {
            EXPANSION_PORT_SCB_SW_RESET;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterSendStop
********************************************************************************
*
* Summary:
*  Generates Stop condition on the bus.
*  At least one byte has to be read if start or restart condition with read
*  direction was generated before.
*  This function is blocking and does not return until a stop condition
*  is generated or error occurred.
*
* Parameters:
*  None
*
* Return:
*  Error status
*
* Side Effects:
*  A valid Start or ReStart condition must be generated before calling
*  this function. This function does nothing if Start or ReStart condition
*  failed before this function was called.
*  For read transfer, at least one byte has to be read before Stop generation.
*
* Global variables:
*  EXPANSION_PORT_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterSendStop(void)
{
    uint32 resetIp;
    uint32 errStatus;

    resetIp   = 0u;
    errStatus = EXPANSION_PORT_I2C_MSTR_NOT_READY;

    /* Check FSM state before generating Stop condition */
    if(EXPANSION_PORT_CHECK_I2C_MASTER_ACTIVE)
    {
        /*
        * Write direction: generates Stop
        * Read  direction: generates NACK and Stop
        */
        EXPANSION_PORT_I2C_MASTER_GENERATE_STOP;

        while(!EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_STOP     |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR))
        {
            /* Wait until Stop has been generated */
        }

        /* Check Stop generation */
        if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_STOP))
        {
            errStatus = EXPANSION_PORT_I2C_MSTR_NO_ERROR;
        }
        else if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST))
        {
            errStatus = EXPANSION_PORT_I2C_MSTR_ERR_ARB_LOST;
            resetIp   = EXPANSION_PORT_I2C_RESET_ERROR;
        }
        else /* EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR is set */
        {
            errStatus = EXPANSION_PORT_I2C_MSTR_ERR_BUS_ERR;
            resetIp   = EXPANSION_PORT_I2C_RESET_ERROR;
        }

        EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_I2C_STOP     |
                                                    EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                    EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR);

        EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;

        /* Reset block in case of: LOST_ARB or BUS_ERR */
        if(0u != resetIp)
        {
            EXPANSION_PORT_SCB_SW_RESET;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterWriteByte
********************************************************************************
*
* Summary:
*  Sends one byte to a slave.
*  This function is blocking and does not return until byte is transmitted
*  or error occurred.
*
* Parameters:
*  data: The data byte to send to the slave.
*
* Return:
*  Error status
*
* Side Effects:
*  A valid Start or ReStart condition must be generated before calling
*  this function. This function does nothing if Start or ReStart condition
*  failed before this function was called.
*
* Global variables:
*  EXPANSION_PORT_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterWriteByte(uint32 theByte)
{
    uint32 resetIp;
    uint32 errStatus;

    resetIp   = 0u;
    errStatus = EXPANSION_PORT_I2C_MSTR_NOT_READY;

    /* Check FSM state before write byte */
    if(EXPANSION_PORT_CHECK_I2C_MASTER_ACTIVE)
    {
        EXPANSION_PORT_TX_FIFO_WR_REG = theByte;

        while(!EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ACK      |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_NACK     |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR))
        {
            /* Wait until byte has been transferred */
        }

        /* Check results after byte was sent */
        if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ACK))
        {
            EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_MSTR_HALT;
                         errStatus = EXPANSION_PORT_I2C_MSTR_NO_ERROR;
        }
        else if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_NACK))
        {
            EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_MSTR_HALT;
                         errStatus = EXPANSION_PORT_I2C_MSTR_ERR_LB_NAK;
        }
        else if(EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST))
        {
            EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;
                         errStatus = EXPANSION_PORT_I2C_MSTR_ERR_ARB_LOST;
                         resetIp   = EXPANSION_PORT_I2C_RESET_ERROR;
        }
        else /* EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR set is */
        {
            EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;
                         errStatus = EXPANSION_PORT_I2C_MSTR_ERR_BUS_ERR;
                         resetIp   = EXPANSION_PORT_I2C_RESET_ERROR;
        }

        EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_I2C_ACK      |
                                                    EXPANSION_PORT_INTR_MASTER_I2C_NACK     |
                                                    EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                    EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR);

        /* Reset block in case of: LOST_ARB or BUS_ERR */
        if(0u != resetIp)
        {
            EXPANSION_PORT_SCB_SW_RESET;
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterReadByte
********************************************************************************
*
* Summary:
*  Reads one byte from a slave and ACKs or NAKs received byte.
*  This function does not generate NAK explicitly. The following call
*  SCB_I2CMasterSendStop() or SCB_I2CMasterSendRestart() will generate NAK and
*  Stop or ReStart condition appropriately.
*  This function is blocking and does not return until byte is received
*  or error occurred.
*
* Parameters:
*  ackNack: Response to received byte.
*
* Return:
*  Byte read from the slave. In case of error the MSB of returned data
*  is set to 1.
*
* Side Effects:
*  A valid Start or ReStart condition must be generated before calling this
*  function. This function does nothing and returns invalid byte value
*  if Start or ReStart conditions failed before this function was called.
*
* Global variables:
*  EXPANSION_PORT_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterReadByte(uint32 ackNack)
{
    uint32 theByte;

    /* Return invalid byte in case BUS_ERR happen during receiving */
    theByte = EXPANSION_PORT_I2C_INVALID_BYTE;

    /* Check FSM state before read byte */
    if(EXPANSION_PORT_CHECK_I2C_MASTER_ACTIVE)
    {
        while((!EXPANSION_PORT_CHECK_INTR_RX(EXPANSION_PORT_INTR_RX_NOT_EMPTY)) &&
              (!EXPANSION_PORT_CHECK_INTR_MASTER(EXPANSION_PORT_INTR_MASTER_I2C_ARB_LOST |
                                                  EXPANSION_PORT_INTR_MASTER_I2C_BUS_ERROR)))
        {
            /* Wait until byte has been received */
        }

        /* Check the results after the byte was sent */
        if(EXPANSION_PORT_CHECK_INTR_RX(EXPANSION_PORT_INTR_RX_NOT_EMPTY))
        {
            theByte = EXPANSION_PORT_RX_FIFO_RD_REG;

            EXPANSION_PORT_ClearRxInterruptSource(EXPANSION_PORT_INTR_RX_NOT_EMPTY);

            if(0u == ackNack)
            {
                EXPANSION_PORT_I2C_MASTER_GENERATE_ACK;
            }
            else
            {
                /* NACK is generated by Stop or ReStart command */
                EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_MSTR_HALT;
            }
        }
        else
        {
            EXPANSION_PORT_ClearMasterInterruptSource(EXPANSION_PORT_INTR_MASTER_ALL);

            /* Reset block in case of: LOST_ARB or BUS_ERR */
            EXPANSION_PORT_SCB_SW_RESET;
        }
    }

    return(theByte);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterGetReadBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes that has been transferred with an
*  SCB_I2CMasterReadBuf() function.
*
* Parameters:
*  None
*
* Return:
*  Byte count of transfer. If the transfer is not yet complete, it returns
*  the byte count transferred so far.
*
* Side Effects:
*  This function returns not valid value if SCB_I2C_MSTAT_ERR_ARB_LOST
*  or SCB_I2C_MSTAT_ERR_BUS_ERROR occurred while read transfer.
*
* Global variables:
*  EXPANSION_PORT_mstrRdBufIndex - used to current index within master read
*  buffer.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterGetReadBufSize(void)
{
    return(EXPANSION_PORT_mstrRdBufIndex);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterGetWriteBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes that have been transferred with an
*  SCB_I2CMasterWriteBuf() function.
*
* Parameters:
*  None
*
* Return:
*  Byte count of transfer. If the transfer is not yet complete, it returns
*  zero unit transfer completion.
*
* Side Effects:
*  This function returns not valid value if SCB_I2C_MSTAT_ERR_ARB_LOST
*  or SCB_I2C_MSTAT_ERR_BUS_ERROR occurred while read transfer.
*
* Global variables:
*  EXPANSION_PORT_mstrWrBufIndex - used to current index within master write
*  buffer.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterGetWriteBufSize(void)
{
    return(EXPANSION_PORT_mstrWrBufIndex);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterClearReadBuf
********************************************************************************
*
* Summary:
*  Resets the read buffer pointer back to the first byte in the buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  EXPANSION_PORT_mstrRdBufIndex - used to current index within master read
*   buffer.
*  EXPANSION_PORT_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
void EXPANSION_PORT_I2CMasterClearReadBuf(void)
{
    EXPANSION_PORT_DisableInt();  /* Lock from interruption */

    EXPANSION_PORT_mstrRdBufIndex = 0u;
    EXPANSION_PORT_mstrStatus    &= (uint16) ~EXPANSION_PORT_I2C_MSTAT_RD_CMPLT;

    EXPANSION_PORT_EnableInt();   /* Release lock */
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterClearWriteBuf
********************************************************************************
*
* Summary:
*  Resets the write buffer pointer back to the first byte in the buffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  EXPANSION_PORT_mstrRdBufIndex - used to current index within master read
*   buffer.
*  EXPANSION_PORT_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
void EXPANSION_PORT_I2CMasterClearWriteBuf(void)
{
    EXPANSION_PORT_DisableInt();  /* Lock from interruption */

    EXPANSION_PORT_mstrWrBufIndex = 0u;
    EXPANSION_PORT_mstrStatus    &= (uint16) ~EXPANSION_PORT_I2C_MSTAT_WR_CMPLT;

    EXPANSION_PORT_EnableInt();   /* Release lock */
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterStatus
********************************************************************************
*
* Summary:
*  Returns the master's communication status.
*
* Parameters:
*  None
*
* Return:
*  Current status of I2C master.
*
* Global variables:
*  EXPANSION_PORT_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterStatus(void)
{
    uint32 status;

    EXPANSION_PORT_DisableInt();  /* Lock from interruption */

    status = (uint32) EXPANSION_PORT_mstrStatus;

    if (EXPANSION_PORT_CHECK_I2C_MASTER_ACTIVE)
    {
        /* Add status of master pending transaction: MSTAT_XFER_INP */
        status |= (uint32) EXPANSION_PORT_I2C_MSTAT_XFER_INP;
    }

    EXPANSION_PORT_EnableInt();   /* Release lock */

    return(status);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CMasterClearStatus
********************************************************************************
*
* Summary:
*  Clears all status flags and returns the master status.
*
* Parameters:
*  None
*
* Return:
*  Current status of I2C master.
*
* Global variables:
*  EXPANSION_PORT_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
uint32 EXPANSION_PORT_I2CMasterClearStatus(void)
{
    uint32 status;

    EXPANSION_PORT_DisableInt();  /* Lock from interruption */

    /* Read and clear master status */
    status = (uint32) EXPANSION_PORT_mstrStatus;
    EXPANSION_PORT_mstrStatus = EXPANSION_PORT_I2C_MSTAT_CLEAR;

    EXPANSION_PORT_EnableInt();   /* Release lock */

    return(status);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CReStartGeneration
********************************************************************************
*
* Summary:
*  Generates a ReStart condition:
*  SCB IP V1 and later: Generates ReStart using the scb IP functionality
*    Sets the I2C_MASTER_CMD_M_START and I2C_MASTER_CMD_M_NACK (if the previous
*    transaction was read) bits in the SCB.I2C_MASTER_CMD register.
*    This combination forces the master to generate ReStart.
*
*  SCB IP V0: Generates Restart using the GPIO and scb IP functionality.
*   After the master completes write or read, the SCL is stretched.
*   The master waits until SDA line is released by the slave. Then the GPIO
*   function is enabled and the scb IP disabled as it already does not drive
*   the bus. In case of the previous transfer was read, the NACK is generated
*   by the GPIO. The delay of tLOW is added to manage the hold time.
*   Set I2C_M_CMD.START and enable the scb IP. The ReStart generation
*   is started after the I2C function is enabled for the SCL.
*   Note1: the scb IP due re-enable generates Start but on the I2C bus it
*          appears as ReStart.
*   Note2: the I2C_M_CMD.START is queued if scb IP is disabled.
*   Note3: the I2C_STATUS_M_READ is cleared is address was NACKed before.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  SCB IP V0: The NACK generation by the GPIO may cause a greater SCL period
*             than expected for the selected master data rate.
*
*******************************************************************************/
void EXPANSION_PORT_I2CReStartGeneration(void)
{
#if(EXPANSION_PORT_CY_SCBIP_V0)
    /* Generates Restart use GPIO and scb IP functionality. Ticket ID#143715,
    ID#145238 and ID#173656 */
    uint32 status = EXPANSION_PORT_I2C_STATUS_REG;

    while(EXPANSION_PORT_WAIT_SDA_SET_HIGH)
    {
        /* Wait when slave release SDA line: SCL tHIGH is complete */
    }

    /* Prepare DR register to drive SCL line */
    EXPANSION_PORT_SET_I2C_SCL_DR(EXPANSION_PORT_I2C_SCL_LOW);

    /* Switch HSIOM to GPIO: SCL goes low */
    EXPANSION_PORT_SET_I2C_SCL_HSIOM_SEL(EXPANSION_PORT_HSIOM_GPIO_SEL);

    /* Disable SCB block */
    EXPANSION_PORT_CTRL_REG &= (uint32) ~EXPANSION_PORT_CTRL_ENABLED;

    if(0u != (status & EXPANSION_PORT_I2C_STATUS_M_READ))
    {
        /* Generate NACK use GPIO functionality */
        EXPANSION_PORT_SET_I2C_SCL_DR(EXPANSION_PORT_I2C_SCL_LOW);
        CyDelayUs(EXPANSION_PORT_I2C_TLOW_TIME); /* Count tLOW */

        EXPANSION_PORT_SET_I2C_SCL_DR(EXPANSION_PORT_I2C_SCL_HIGH);
        while(EXPANSION_PORT_WAIT_SCL_SET_HIGH)
        {
            /* Wait until slave releases SCL in case if it stretches */
        }
        CyDelayUs(EXPANSION_PORT_I2C_THIGH_TIME); /* Count tHIGH */
    }

    /* Count tLOW as hold time for write and read */
    EXPANSION_PORT_SET_I2C_SCL_DR(EXPANSION_PORT_I2C_SCL_LOW);
    CyDelayUs(EXPANSION_PORT_I2C_TLOW_TIME); /* Count tLOW */

    /* Set command for Start generation: it will appear */
    EXPANSION_PORT_I2C_MASTER_CMD_REG = EXPANSION_PORT_I2C_MASTER_CMD_M_START;

    /* Enable SCB block */
    EXPANSION_PORT_CTRL_REG |= (uint32) EXPANSION_PORT_CTRL_ENABLED;

    /* Switch HSIOM to I2C: */
    EXPANSION_PORT_SET_I2C_SCL_HSIOM_SEL(EXPANSION_PORT_HSIOM_I2C_SEL);

    /* Revert SCL DR register */
    EXPANSION_PORT_SET_I2C_SCL_DR(EXPANSION_PORT_I2C_SCL_HIGH);
#else
    uint32 cmd;

    /* Generates ReStart use scb IP functionality */
    cmd  = EXPANSION_PORT_I2C_MASTER_CMD_M_START;
    cmd |= EXPANSION_PORT_CHECK_I2C_STATUS(EXPANSION_PORT_I2C_STATUS_M_READ) ?
                (EXPANSION_PORT_I2C_MASTER_CMD_M_NACK) : (0u);

    EXPANSION_PORT_I2C_MASTER_CMD_REG = cmd;
#endif /* (EXPANSION_PORT_CY_SCBIP_V1) */
}

#endif /* (EXPANSION_PORT_I2C_MASTER_CONST) */


#if (!EXPANSION_PORT_CY_SCBIP_V0 && \
    EXPANSION_PORT_I2C_MULTI_MASTER_SLAVE_CONST && EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_I2CMasterDisableEcAm
    ********************************************************************************
    *
    * Summary:
    *  Disables externally clocked address match to enable master operation
    *  in active mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    static void EXPANSION_PORT_I2CMasterDisableEcAm(void)
    {
        /* Disables externally clocked address match to enable master operation in active mode.
        * This applicable only for Multi-Master-Slave with wakeup enabled. Ticket ID#192742 */
        if (0u != (EXPANSION_PORT_CTRL_REG & EXPANSION_PORT_CTRL_EC_AM_MODE))
        {
            /* Enable external address match logic */
            EXPANSION_PORT_Stop();
            EXPANSION_PORT_CTRL_REG &= (uint32) ~EXPANSION_PORT_CTRL_EC_AM_MODE;
            EXPANSION_PORT_Enable();
        }
    }
#endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */


/* [] END OF FILE */
