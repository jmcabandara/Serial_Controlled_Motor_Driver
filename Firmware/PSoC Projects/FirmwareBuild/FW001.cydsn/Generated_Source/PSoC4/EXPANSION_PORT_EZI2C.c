/*******************************************************************************
* File Name: EXPANSION_PORT_EZI2C.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  EZI2C mode.
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
#include "EXPANSION_PORT_EZI2C_PVT.h"


/***************************************
*      EZI2C Private Vars
***************************************/

volatile uint8 EXPANSION_PORT_curStatus; /* Status byte */
uint8 EXPANSION_PORT_fsmState;           /* FSM state   */

/* Variables intended to be used with Buffer 1: Primary slave address */
volatile uint8 * EXPANSION_PORT_dataBuffer1; /* Pointer to data buffer 1 */
uint16 EXPANSION_PORT_bufSizeBuf1; /* Size of buffer 1 in bytes      */
uint16 EXPANSION_PORT_protectBuf1; /* Start index of write protected area buffer 1 */
uint16 EXPANSION_PORT_offsetBuf1; /* Current offset within buffer 1 */
uint16 EXPANSION_PORT_indexBuf1;  /* Current index within buffer 1  */

#if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
    uint8 EXPANSION_PORT_addrBuf1; /* Primary slave address. Used for software comparison   */
    uint8 EXPANSION_PORT_addrBuf2; /* Secondary slave address. Used for software comparison */

    /* Variables intended to be used with Buffer 2: Primary slave address */
    volatile uint8 * EXPANSION_PORT_dataBuffer2; /* Pointer to data buffer 2 */
    uint16 EXPANSION_PORT_bufSizeBuf2; /* Size of buffer 2 in bytes      */
    uint16 EXPANSION_PORT_protectBuf2; /* Start index of write protected area buffer 2 */
    uint16 EXPANSION_PORT_offsetBuf2; /* Current offset within buffer 2 */
    uint16 EXPANSION_PORT_indexBuf2;  /* Current index within buffer 2  */
#endif /* (EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */


/***************************************
*      EZI2C Private Functions
***************************************/

#if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
    static uint32 EXPANSION_PORT_EzI2CUpdateRxMatchReg(uint32 addr1, uint32 addr2);
#endif /* (EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of EZI2C */
    const EXPANSION_PORT_EZI2C_INIT_STRUCT EXPANSION_PORT_configEzI2C =
    {
        EXPANSION_PORT_EZI2C_CLOCK_STRETCHING,
        EXPANSION_PORT_EZI2C_MEDIAN_FILTER_ENABLE,
        EXPANSION_PORT_EZI2C_NUMBER_OF_ADDRESSES,
        EXPANSION_PORT_EZI2C_PRIMARY_SLAVE_ADDRESS,
        EXPANSION_PORT_EZI2C_SECONDARY_SLAVE_ADDRESS,
        EXPANSION_PORT_EZI2C_SUB_ADDRESS_SIZE,
        EXPANSION_PORT_EZI2C_WAKE_ENABLE,
        EXPANSION_PORT_EZI2C_BYTE_MODE_ENABLE
    };

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for the EZI2C operation.
    *
    * Parameters:
    *  config:  Pointer to a structure that contains the following ordered list of
    *           fields. These fields match the selections available in the
    *           customizer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_EzI2CInit(const EXPANSION_PORT_EZI2C_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            EXPANSION_PORT_SetPins(EXPANSION_PORT_SCB_MODE_EZI2C, EXPANSION_PORT_DUMMY_PARAM,
                                                                      EXPANSION_PORT_DUMMY_PARAM);

            /* Store internal configuration */
            EXPANSION_PORT_scbMode       = (uint8) EXPANSION_PORT_SCB_MODE_EZI2C;
            EXPANSION_PORT_scbEnableWake = (uint8) config->enableWake;
            EXPANSION_PORT_scbEnableIntr = (uint8) EXPANSION_PORT_SCB_IRQ_INTERNAL;

            EXPANSION_PORT_numberOfAddr  = (uint8) config->numberOfAddresses;
            EXPANSION_PORT_subAddrSize   = (uint8) config->subAddrSize;

        #if (EXPANSION_PORT_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            EXPANSION_PORT_SET_I2C_CFG_SDA_FILT_TRIM(EXPANSION_PORT_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings, AF = 1, DF = 0. Ticket ID#176179 */
            EXPANSION_PORT_I2C_CFG_ANALOG_FITER_ENABLE;

            /* Configure I2C interface */
            EXPANSION_PORT_CTRL_REG     = EXPANSION_PORT_GET_CTRL_BYTE_MODE  (config->enableByteMode)    |
                                            EXPANSION_PORT_GET_CTRL_ADDR_ACCEPT(config->numberOfAddresses) |
                                            EXPANSION_PORT_GET_CTRL_EC_AM_MODE (config->enableWake);

            EXPANSION_PORT_I2C_CTRL_REG = EXPANSION_PORT_EZI2C_CTRL;

            /* Configure RX direction */
            EXPANSION_PORT_RX_CTRL_REG = EXPANSION_PORT_EZI2C_RX_CTRL |
                                        EXPANSION_PORT_GET_RX_CTRL_MEDIAN(EXPANSION_PORT_DIGITAL_FILTER_DISABLE);
                                                ;
            EXPANSION_PORT_RX_FIFO_CTRL_REG = EXPANSION_PORT_CLEAR_REG;

            /* Set default address and mask */
            if(EXPANSION_PORT_EZI2C_PRIMARY_ADDRESS == config->numberOfAddresses)
            {
                EXPANSION_PORT_RX_MATCH_REG = EXPANSION_PORT_EzI2CUpdateRxMatchReg(config->primarySlaveAddr,
                                                                                       config->primarySlaveAddr);
            }
            else
            {
                EXPANSION_PORT_RX_MATCH_REG = EXPANSION_PORT_EzI2CUpdateRxMatchReg(config->primarySlaveAddr,
                                                                                       config->secondarySlaveAddr);
            }

            /* Configure TX direction */
            EXPANSION_PORT_TX_CTRL_REG      = EXPANSION_PORT_EZI2C_TX_CTRL;
            EXPANSION_PORT_TX_FIFO_CTRL_REG = ((0u != (config->enableClockStretch)) ?
                                                 EXPANSION_PORT_CLEAR_REG : EXPANSION_PORT_EZI2C_TX_FIFO_CTRL);

            /* Configure interrupt sources */
        #if (!EXPANSION_PORT_CY_SCBIP_V1)
           EXPANSION_PORT_INTR_SPI_EC_MASK_REG = EXPANSION_PORT_NO_INTR_SOURCES;
        #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */

            EXPANSION_PORT_INTR_I2C_EC_MASK_REG = EXPANSION_PORT_NO_INTR_SOURCES;
            EXPANSION_PORT_INTR_MASTER_MASK_REG = EXPANSION_PORT_NO_INTR_SOURCES;
            EXPANSION_PORT_INTR_SLAVE_MASK_REG  = EXPANSION_PORT_EZI2C_INTR_SLAVE_MASK;
            EXPANSION_PORT_INTR_TX_MASK_REG     = EXPANSION_PORT_NO_INTR_SOURCES;

            /* Configure interrupt with EZI2C handler but do not enable it */
            CyIntDisable    (EXPANSION_PORT_ISR_NUMBER);
            CyIntSetPriority(EXPANSION_PORT_ISR_NUMBER, EXPANSION_PORT_ISR_PRIORITY);
            (void) CyIntSetVector(EXPANSION_PORT_ISR_NUMBER, (0u != (config->enableClockStretch)) ?
                                                                      (&EXPANSION_PORT_EZI2C_STRETCH_ISR) :
                                                                      (&EXPANSION_PORT_EZI2C_NO_STRETCH_ISR));

            if(0u != (config->enableClockStretch))
            {
                /* Configure interrupt sources */
                EXPANSION_PORT_INTR_SLAVE_MASK_REG |= EXPANSION_PORT_INTR_SLAVE_I2C_ADDR_MATCH;
                EXPANSION_PORT_INTR_RX_MASK_REG     = EXPANSION_PORT_NO_INTR_SOURCES;
            }
            else
            {
                /* Enable Auto ACK/NACK features */
                EXPANSION_PORT_I2C_CTRL_REG |= EXPANSION_PORT_EZI2C_CTRL_AUTO;

                /* Configure RX interrupt source */
                EXPANSION_PORT_INTR_SLAVE_MASK_REG |= EXPANSION_PORT_INTR_SLAVE_I2C_START;
                EXPANSION_PORT_INTR_RX_MASK_REG     = EXPANSION_PORT_INTR_RX_NOT_EMPTY;
            }

            /* Configure global variables */
            EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_IDLE;

            EXPANSION_PORT_curStatus  = 0u;
            EXPANSION_PORT_indexBuf1  = 0u;
            EXPANSION_PORT_offsetBuf1 = 0u;
            EXPANSION_PORT_indexBuf2  = 0u;
            EXPANSION_PORT_offsetBuf2 = 0u;

            EXPANSION_PORT_addrBuf1 = (uint8) config->primarySlaveAddr;
            EXPANSION_PORT_addrBuf2 = (uint8) config->secondarySlaveAddr;
        }
    }

#else

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for the EZI2C operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_EzI2CInit(void)
    {
        /* Configure I2C interface */
        EXPANSION_PORT_CTRL_REG     = EXPANSION_PORT_EZI2C_DEFAULT_CTRL;
        EXPANSION_PORT_I2C_CTRL_REG = EXPANSION_PORT_EZI2C_DEFAULT_I2C_CTRL;

    #if (EXPANSION_PORT_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        EXPANSION_PORT_SET_I2C_CFG_SDA_FILT_TRIM(EXPANSION_PORT_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

        /* Configure RX direction */
        EXPANSION_PORT_RX_CTRL_REG      = EXPANSION_PORT_EZI2C_DEFAULT_RX_CTRL;
        EXPANSION_PORT_RX_FIFO_CTRL_REG = EXPANSION_PORT_EZI2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        EXPANSION_PORT_RX_MATCH_REG     = EXPANSION_PORT_EZI2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        EXPANSION_PORT_TX_CTRL_REG      = EXPANSION_PORT_EZI2C_DEFAULT_TX_CTRL;
        EXPANSION_PORT_TX_FIFO_CTRL_REG = EXPANSION_PORT_EZI2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with EZI2C handler but do not enable it */
    #if !defined (CY_EXTERNAL_INTERRUPT_CONFIG)
        CyIntDisable    (EXPANSION_PORT_ISR_NUMBER);
        CyIntSetPriority(EXPANSION_PORT_ISR_NUMBER, EXPANSION_PORT_ISR_PRIORITY);

    #if (EXPANSION_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST)
        (void) CyIntSetVector(EXPANSION_PORT_ISR_NUMBER, &EXPANSION_PORT_EZI2C_STRETCH_ISR);
    #else
        (void) CyIntSetVector(EXPANSION_PORT_ISR_NUMBER, &EXPANSION_PORT_EZI2C_NO_STRETCH_ISR);
    #endif /* (EXPANSION_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST) */

    #endif /* !defined (CY_EXTERNAL_INTERRUPT_CONFIG) */

        /* Configure interrupt sources */
    #if (!EXPANSION_PORT_CY_SCBIP_V1)
        EXPANSION_PORT_INTR_SPI_EC_MASK_REG = EXPANSION_PORT_EZI2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */

        EXPANSION_PORT_INTR_I2C_EC_MASK_REG = EXPANSION_PORT_EZI2C_DEFAULT_INTR_I2C_EC_MASK;
        EXPANSION_PORT_INTR_SLAVE_MASK_REG  = EXPANSION_PORT_EZI2C_DEFAULT_INTR_SLAVE_MASK;
        EXPANSION_PORT_INTR_MASTER_MASK_REG = EXPANSION_PORT_EZI2C_DEFAULT_INTR_MASTER_MASK;
        EXPANSION_PORT_INTR_RX_MASK_REG     = EXPANSION_PORT_EZI2C_DEFAULT_INTR_RX_MASK;
        EXPANSION_PORT_INTR_TX_MASK_REG     = EXPANSION_PORT_EZI2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_IDLE;

        EXPANSION_PORT_curStatus  = 0u;
        EXPANSION_PORT_indexBuf1  = 0u;
        EXPANSION_PORT_offsetBuf1 = 0u;

    #if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
        EXPANSION_PORT_indexBuf2  = 0u;
        EXPANSION_PORT_offsetBuf2 = 0u;

        EXPANSION_PORT_addrBuf1 = EXPANSION_PORT_EZI2C_PRIMARY_SLAVE_ADDRESS;
        EXPANSION_PORT_addrBuf2 = EXPANSION_PORT_EZI2C_SECONDARY_SLAVE_ADDRESS;
    #endif /* (EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */
    }
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: EXPANSION_PORT_EzI2CStop
********************************************************************************
*
* Summary:
*  Resets the EZI2C FSM into a default state and disables TX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_EzI2CStop(void)
{
    /* Disable TX interrupt sources in order not to  cause the false interrupts
    * trigger. The incoming transaction will enable an appropriate TX interrupt.
    */
    EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

#if(EXPANSION_PORT_CY_SCBIP_V0)
    /* Clear pending interrupt as after the block is disabled, the TX FIFO
    * becomes empty and the block does not gate the interrupt sources when
    * disabled.
    */
    EXPANSION_PORT_ClearPendingInt();
#endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

    EXPANSION_PORT_fsmState = EXPANSION_PORT_EZI2C_FSM_IDLE;
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_EzI2CGetActivity
********************************************************************************
*
* Summary:
*  Returns the EZI2C slave status.
*  The read, write and error status flags reset to zero after this function
*  call. The busy status flag is cleared when the transaction intended for
*  the EZI2C slave completes.
*
* Parameters:
*  None
*
* Return:
*  Returns the status of the EZI2C Slave activity.
*
*******************************************************************************/
uint32 EXPANSION_PORT_EzI2CGetActivity(void)
{
    uint32 status;

    EXPANSION_PORT_DisableInt();  /* Lock from interruption */

    status = EXPANSION_PORT_curStatus;

    /* Relay on address match event from HW as bus busy status */
    if(EXPANSION_PORT_EZI2C_SCL_STRETCH_DISABLE)
    {
        /* For EXPANSION_PORT_CY_SCBIP_V0 the wake is prohibited by customizer */
        if(EXPANSION_PORT_EZI2C_EC_AM_ENABLE)
        {
            status |= EXPANSION_PORT_CHECK_INTR_I2C_EC(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP) ?
                        EXPANSION_PORT_EZI2C_STATUS_BUSY : 0u;
        }
        else
        {
            status |= EXPANSION_PORT_CHECK_INTR_SLAVE(EXPANSION_PORT_INTR_SLAVE_I2C_ADDR_MATCH) ?
                        EXPANSION_PORT_EZI2C_STATUS_BUSY : 0u;
        }
        
    }
    

    EXPANSION_PORT_curStatus &= ((uint8) ~EXPANSION_PORT_EZI2C_CLEAR_STATUS);

    EXPANSION_PORT_EnableInt();   /* Release lock */

    return(status);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_EzI2CSetAddress1
********************************************************************************
*
* Summary:
*  Sets the primary I2C slave address.
*
* Parameters:
*  address: I2C slave address for the secondary device.
*           This address is the 7-bit right-justified slave address and does
*           not include the R/W bit.
*           This value may be any address between 0 and 127 (0x00 to 0x7F).
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_EzI2CSetAddress1(uint32 address)
{
    if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE)
    {
        EXPANSION_PORT_addrBuf1 = (uint8) address;

        EXPANSION_PORT_RX_MATCH_REG = EXPANSION_PORT_EzI2CUpdateRxMatchReg(address,
                                                                               (uint32) EXPANSION_PORT_addrBuf2);
    }
    else
    {
        uint32 matchReg;

        matchReg = EXPANSION_PORT_RX_MATCH_REG;

        matchReg &= ((uint32) ~EXPANSION_PORT_RX_MATCH_ADDR_MASK);            /* Clear address bits */
        matchReg |= ((uint32)  EXPANSION_PORT_GET_I2C_8BIT_ADDRESS(address)); /* Set mask */

        EXPANSION_PORT_RX_MATCH_REG = matchReg;
    }
    
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_EzI2CGetAddress1
********************************************************************************
*
* Summary:
*  Gets the primary I2C slave 7-bit address.
*
* Parameters:
*  None
*
* Return:
*  Returns I2C slave address for the primary device.
*  This address is the 7-bit right-justified slave address and does not
*  include the R/W bit.
*  This value may be any address between 0 and 127 (0x00 to 0x7F).
*
*******************************************************************************/
uint32 EXPANSION_PORT_EzI2CGetAddress1(void)
{
    uint32 address;

    if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE)
    {
        address = (uint32) EXPANSION_PORT_addrBuf1;
    }
    else
    {
        address = (EXPANSION_PORT_GET_RX_MATCH_ADDR(EXPANSION_PORT_RX_MATCH_REG) >>
                   EXPANSION_PORT_I2C_SLAVE_ADDR_POS);
    }
    

    return(address);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_EzI2CSetBuffer1
********************************************************************************
*
* Summary:
*  Sets up the data buffer to be exposed to the I2C master on a primary slave
*  address request.
*
* Parameters:
*  bufSize: Size of the buffer in bytes.
*  rwBoundary: Sets how many bytes are writeable in the beginning of the buffer.
*              This value must be less than or equal to the buffer size.
*              Data located at offset rwBoundry and greater are read only.
*  buffer: Pointer to the data buffer.
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_EzI2CSetBuffer1(uint32 bufSize, uint32 rwBoundary, volatile uint8 * buffer)
{
    if (NULL != buffer)
    {
        EXPANSION_PORT_DisableInt();  /* Lock from interruption */

        EXPANSION_PORT_dataBuffer1 =  buffer;
        EXPANSION_PORT_bufSizeBuf1 = (uint16) bufSize;
        EXPANSION_PORT_protectBuf1 = (uint16) rwBoundary;

        EXPANSION_PORT_EnableInt();   /* Release lock */
    }
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_EzI2CSetReadBoundaryBuffer1
********************************************************************************
*
* Summary:
*  Sets the read only boundary in the data buffer to be exposed to the I2C
*  master on a primary slave address request.
*
* Parameters:
*  rwBoundry: Sets how many bytes are writeable in the beginning of the buffer.
*             This value must be less than or equal to the buffer size.
*             Data located at offset rwBoundry and greater are read only.
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_EzI2CSetReadBoundaryBuffer1(uint32 rwBoundary)
{
    EXPANSION_PORT_protectBuf1 = (uint16) rwBoundary;
}


#if(EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CUpdateRxMatchReg
    ********************************************************************************
    *
    * Summary:
    *  Returns the value of the RX MATCH register for addr1 and addr2. The addr1 is
    *  accepted as the primary address and it is written to RX_MATCH.ADDRESS
    *  (addr1 << 0x01).
    *  The RX_MATCH.MASK is set as follow: addr1 and addr2 equal bits set to 1
    *  otherwise 0.
    *
    * Parameters:
    *  addr1: I2C slave address for the primary device.
    *  addr2: I2C slave address for the secondary device.
    *         This address is the 7-bit right-justified slave address and does
    *         not include the R/W bit.
    *         This value may be any address between 0 and 127 (0x00 to 0x7F).
    *
    * Return:
    *  Value of RX MATCH register.
    *
    *******************************************************************************/
    static uint32 EXPANSION_PORT_EzI2CUpdateRxMatchReg(uint32 addr1, uint32 addr2)
    {
        uint32 matchReg;

        matchReg  = ~(addr1 ^ addr2); /* If (addr1 and addr2) bit matches - mask bit equals 1, in other case 0 */

        matchReg  = (uint32) (EXPANSION_PORT_GET_I2C_8BIT_ADDRESS(matchReg) << EXPANSION_PORT_RX_MATCH_MASK_POS);
        matchReg |= EXPANSION_PORT_GET_I2C_8BIT_ADDRESS(addr1);

        return(matchReg);
    }

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CSetAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the secondary I2C slave address.
    *
    * Parameters:
    *  address: I2C slave address for the secondary device.
    *           This address is the 7-bit right-justified slave address and does
    *           not include the R/W bit.
    *           This value may be any address between 0 and 127 (0x00 to 0x7F).
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_EzI2CSetAddress2(uint32 address)
    {
        EXPANSION_PORT_addrBuf2 = (uint8) address;

        EXPANSION_PORT_RX_MATCH_REG = EXPANSION_PORT_EzI2CUpdateRxMatchReg((uint32) EXPANSION_PORT_addrBuf1,
                                                                               address);
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CGetAddress2
    ********************************************************************************
    *
    * Summary:
    *  Gets secondary the I2C slave 7-bit address.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Returns the I2C slave address for the secondary device.
    *  This address is a 7-bit right-justified slave address and does not
    *  include the R/W bit.
    *  This value may be any address between 0 and 127 (0x00 to 0x7F).
    *
    *******************************************************************************/
    uint32 EXPANSION_PORT_EzI2CGetAddress2(void)
    {
        return((uint32) EXPANSION_PORT_addrBuf2);
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CSetBuffer2
    ********************************************************************************
    *
    * Summary:
    *  Sets up the data buffer to be exposed to the I2C master on a secondary slave
    *  address request.
    *
    * Parameters:
    *  bufSize: Size of the buffer in bytes.
    *  rwBoundary: Sets how many bytes are writeable in the beginning of the buffer.
    *              This value must be less than or equal to the buffer size.
    *              Data located at offset rwBoundry and greater are read only.
    *  buffer: Pointer to the data buffer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_EzI2CSetBuffer2(uint32 bufSize, uint32 rwBoundary, volatile uint8 * buffer)
    {
        if (NULL != buffer)
        {
            EXPANSION_PORT_DisableInt();  /* Lock from interruption */

            EXPANSION_PORT_dataBuffer2 =  buffer;
            EXPANSION_PORT_bufSizeBuf2 = (uint16) bufSize;
            EXPANSION_PORT_protectBuf2 = (uint16) rwBoundary;

            EXPANSION_PORT_EnableInt();   /* Release lock */
        }
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CSetReadBoundaryBuffer2
    ********************************************************************************
    *
    * Summary:
    *  Sets the read only boundary in the data buffer to be exposed to the I2C
    *  master on a secondary address request.
    *
    * Parameters:
    *  rwBoundary: Sets how many bytes are writeable at the beginning of the buffer.
    *              This value must be less than or equal to the buffer size.
    *              Data located at offset rwBoundry and greater are read only.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_EzI2CSetReadBoundaryBuffer2(uint32 rwBoundary)
    {
        EXPANSION_PORT_protectBuf2 = (uint16) rwBoundary;
    }

#endif /* (EXPANSION_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */


#if(EXPANSION_PORT_EZI2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CSaveConfig
    ********************************************************************************
    *
    * Summary:
    *  Clock stretching is  enabled: Enables INTR_I2C_EC.WAKE_UP interrupt source.
    *  It triggers on the slave address match.
    *  Clock stretching is disabled: Waits until the I2C slave becomes free and
    *  disables the block to perform reconfiguration from the active mode operation
    *  to deep sleep with wake up on the address match: enables INTR_I2C_EC.WAKE_UP
    *  interrupt source and disables the INTR_S and INTR_TX interrupt sources.
    *  The block is disabled before reconfiguration and enabled when
    *  it is completed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_EzI2CSaveConfig(void)
    {
    #if(EXPANSION_PORT_CY_SCBIP_V0)

        if(EXPANSION_PORT_EZI2C_SCL_STRETCH_ENABLE)
        {
            /* Enable wakeup interrupt source on address match */
            EXPANSION_PORT_SetI2CExtClkInterruptMode(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP);
        }
        

    #else
        uint8 enableInt;

        enableInt = (uint8) EXPANSION_PORT_INTR_I2C_EC_WAKE_UP;

        if(EXPANSION_PORT_EZI2C_SCL_STRETCH_ENABLE)
        {
        #if (EXPANSION_PORT_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID #187931 */
            EXPANSION_PORT_SCBCLK_Stop();
        #endif /* (EXPANSION_PORT_SCB_CLK_INTERNAL) */

            /* Enable interrupt source to wakeup device */
            EXPANSION_PORT_SetI2CExtClkInterruptMode(enableInt);
        }
        else
        {
            for(;;) /* Wait for end of transaction intended to slave */
            {
                if(0u == (EXPANSION_PORT_GetI2CExtClkInterruptSource() & EXPANSION_PORT_INTR_I2C_EC_WAKE_UP))
                {
                    enableInt = CyEnterCriticalSection();

                    if(0u == (EXPANSION_PORT_GetI2CExtClkInterruptSource() & EXPANSION_PORT_INTR_I2C_EC_WAKE_UP))
                    {
                        /* Attempts to set NACK command before disable block */
                        EXPANSION_PORT_I2C_SLAVE_GENERATE_NACK;

                        if(0u == (EXPANSION_PORT_GetI2CExtClkInterruptSource() & EXPANSION_PORT_INTR_I2C_EC_WAKE_UP))
                        {
                            /* NACK command was set before. It is safe to disable block */
                            EXPANSION_PORT_CTRL_REG &= (uint32) ~EXPANSION_PORT_CTRL_ENABLED;
                            EXPANSION_PORT_DisableInt();

                            CyExitCriticalSection(enableInt);
                            break;
                        }
                        else
                        {
                            /* Clear NACK command to prevent data NACK */
                            EXPANSION_PORT_I2C_SLAVE_CLEAR_NACK;
                        }
                    }

                    CyExitCriticalSection(enableInt);
                }
            }

            /* Disable all active mode interrupt sources */
            EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
            EXPANSION_PORT_SetSlaveInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
            EXPANSION_PORT_ClearPendingInt();
            EXPANSION_PORT_EnableInt();

            /* Enable wakeup interrupt on address match */
            EXPANSION_PORT_SetI2CExtClkInterruptMode(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP);

            enableInt = CyEnterCriticalSection();

            EXPANSION_PORT_CTRL_REG |= (uint32) EXPANSION_PORT_CTRL_ENABLED;
            EXPANSION_PORT_I2C_SLAVE_GENERATE_NACK;

            CyExitCriticalSection(enableInt);
        }
        
    #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_EzI2CRestoreConfig
    ********************************************************************************
    *
    * Summary:
    *  Clock stretching is  enabled: Disables the INTR_I2C_EC.WAKE_UP interrupt
    *  source.
    *  Clock stretching is disabled: Reconfigures the EZI2C component from
    *  Deep Sleep (wake up on the address match) to active operation: disables
    *  the INTR_I2C_EC.WAKE_UP interrupt source and restores the INTR_S
    *  interrupt sources to operate in the active mode.
    *  The block is disabled before reconfiguration and enabled when
    *  it is completed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_EzI2CRestoreConfig(void)
    {
    #if(EXPANSION_PORT_CY_SCBIP_V0)

        if(EXPANSION_PORT_EZI2C_SCL_STRETCH_ENABLE)
        {
            /* Disable wakeup interrupt on address match */
            EXPANSION_PORT_SetI2CExtClkInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
        }
        

    #else

        if(EXPANSION_PORT_EZI2C_SCL_STRETCH_ENABLE)
        {
            /* Disable wakeup interrupt source on address match */
            EXPANSION_PORT_SetI2CExtClkInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

        #if (EXPANSION_PORT_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID #187931 */
            EXPANSION_PORT_SCBCLK_Start();
        #endif /* (EXPANSION_PORT_SCB_CLK_INTERNAL) */
        }
        else
        {
            /* NACK will be driven on the bus by wakeup or NACK command.
            * It is safe to disable block to restore active mode configuration.
            */
            EXPANSION_PORT_CTRL_REG &= (uint32) ~EXPANSION_PORT_CTRL_ENABLED;

            /* Restore active mode interrupt sources */
            EXPANSION_PORT_SetI2CExtClkInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
            EXPANSION_PORT_SetSlaveInterruptMode(EXPANSION_PORT_EZI2C_INTR_SLAVE_MASK |
                                                   EXPANSION_PORT_INTR_SLAVE_I2C_START);
            EXPANSION_PORT_ClearPendingInt();

            EXPANSION_PORT_CTRL_REG |= (uint32) EXPANSION_PORT_CTRL_ENABLED;
        }
        

    #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */
    }
#endif /* (EXPANSION_PORT_EZI2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
