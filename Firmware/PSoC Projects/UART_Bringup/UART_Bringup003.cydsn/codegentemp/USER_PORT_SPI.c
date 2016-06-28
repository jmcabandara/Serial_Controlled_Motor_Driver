/*******************************************************************************
* File Name: USER_PORT_SPI.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI mode.
*
* Note:
*
*******************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USER_PORT_PVT.h"
#include "USER_PORT_SPI_UART_PVT.h"

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const USER_PORT_SPI_INIT_STRUCT USER_PORT_configSpi =
    {
        USER_PORT_SPI_MODE,
        USER_PORT_SPI_SUB_MODE,
        USER_PORT_SPI_CLOCK_MODE,
        USER_PORT_SPI_OVS_FACTOR,
        USER_PORT_SPI_MEDIAN_FILTER_ENABLE,
        USER_PORT_SPI_LATE_MISO_SAMPLE_ENABLE,
        USER_PORT_SPI_WAKE_ENABLE,
        USER_PORT_SPI_RX_DATA_BITS_NUM,
        USER_PORT_SPI_TX_DATA_BITS_NUM,
        USER_PORT_SPI_BITS_ORDER,
        USER_PORT_SPI_TRANSFER_SEPARATION,
        0u,
        NULL,
        0u,
        NULL,
        (uint32) USER_PORT_SCB_IRQ_INTERNAL,
        USER_PORT_SPI_INTR_RX_MASK,
        USER_PORT_SPI_RX_TRIGGER_LEVEL,
        USER_PORT_SPI_INTR_TX_MASK,
        USER_PORT_SPI_TX_TRIGGER_LEVEL,
        (uint8) USER_PORT_SPI_BYTE_MODE_ENABLE,
        (uint8) USER_PORT_SPI_FREE_RUN_SCLK_ENABLE,
        (uint8) USER_PORT_SPI_SS_POLARITY
    };


    /*******************************************************************************
    * Function Name: USER_PORT_SpiInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for the SPI operation.
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
    void USER_PORT_SpiInit(const USER_PORT_SPI_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            USER_PORT_SetPins(USER_PORT_SCB_MODE_SPI, config->mode, USER_PORT_DUMMY_PARAM);

            /* Store internal configuration */
            USER_PORT_scbMode       = (uint8) USER_PORT_SCB_MODE_SPI;
            USER_PORT_scbEnableWake = (uint8) config->enableWake;
            USER_PORT_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            USER_PORT_rxBuffer      =         config->rxBuffer;
            USER_PORT_rxDataBits    = (uint8) config->rxDataBits;
            USER_PORT_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            USER_PORT_txBuffer      =         config->txBuffer;
            USER_PORT_txDataBits    = (uint8) config->txDataBits;
            USER_PORT_txBufferSize  = (uint8) config->txBufferSize;

            /* Configure SPI interface */
            USER_PORT_CTRL_REG     = USER_PORT_GET_CTRL_OVS(config->oversample)           |
                                            USER_PORT_GET_CTRL_BYTE_MODE(config->enableByteMode) |
                                            USER_PORT_GET_CTRL_EC_AM_MODE(config->enableWake)    |
                                            USER_PORT_CTRL_SPI;

            USER_PORT_SPI_CTRL_REG = USER_PORT_GET_SPI_CTRL_CONTINUOUS    (config->transferSeperation)  |
                                            USER_PORT_GET_SPI_CTRL_SELECT_PRECEDE(config->submode &
                                                                          USER_PORT_SPI_MODE_TI_PRECEDES_MASK) |
                                            USER_PORT_GET_SPI_CTRL_SCLK_MODE     (config->sclkMode)            |
                                            USER_PORT_GET_SPI_CTRL_LATE_MISO_SAMPLE(config->enableLateSampling)|
                                            USER_PORT_GET_SPI_CTRL_SCLK_CONTINUOUS(config->enableFreeRunSclk)  |
                                            USER_PORT_GET_SPI_CTRL_SSEL_POLARITY (config->polaritySs)          |
                                            USER_PORT_GET_SPI_CTRL_SUB_MODE      (config->submode)             |
                                            USER_PORT_GET_SPI_CTRL_MASTER_MODE   (config->mode);

            /* Configure RX direction */
            USER_PORT_RX_CTRL_REG     =  USER_PORT_GET_RX_CTRL_DATA_WIDTH(config->rxDataBits)         |
                                                USER_PORT_GET_RX_CTRL_BIT_ORDER (config->bitOrder)           |
                                                USER_PORT_GET_RX_CTRL_MEDIAN    (config->enableMedianFilter) |
                                                USER_PORT_SPI_RX_CTRL;

            USER_PORT_RX_FIFO_CTRL_REG = USER_PORT_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure TX direction */
            USER_PORT_TX_CTRL_REG      = USER_PORT_GET_TX_CTRL_DATA_WIDTH(config->txDataBits) |
                                                USER_PORT_GET_TX_CTRL_BIT_ORDER (config->bitOrder)   |
                                                USER_PORT_SPI_TX_CTRL;

            USER_PORT_TX_FIFO_CTRL_REG = USER_PORT_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

            /* Configure interrupt with SPI handler but do not enable it */
            CyIntDisable    (USER_PORT_ISR_NUMBER);
            CyIntSetPriority(USER_PORT_ISR_NUMBER, USER_PORT_ISR_PRIORITY);
            (void) CyIntSetVector(USER_PORT_ISR_NUMBER, &USER_PORT_SPI_UART_ISR);

            /* Configure interrupt sources */
            USER_PORT_INTR_I2C_EC_MASK_REG = USER_PORT_NO_INTR_SOURCES;
            USER_PORT_INTR_SPI_EC_MASK_REG = USER_PORT_NO_INTR_SOURCES;
            USER_PORT_INTR_SLAVE_MASK_REG  = USER_PORT_GET_SPI_INTR_SLAVE_MASK(config->rxInterruptMask);
            USER_PORT_INTR_MASTER_MASK_REG = USER_PORT_GET_SPI_INTR_MASTER_MASK(config->txInterruptMask);
            USER_PORT_INTR_RX_MASK_REG     = USER_PORT_GET_SPI_INTR_RX_MASK(config->rxInterruptMask);
            USER_PORT_INTR_TX_MASK_REG     = USER_PORT_GET_SPI_INTR_TX_MASK(config->txInterruptMask);

            /* Set active SS0 */
            USER_PORT_SpiSetActiveSlaveSelect(USER_PORT_SPI_SLAVE_SELECT0);

            /* Clear RX buffer indexes */
            USER_PORT_rxBufferHead     = 0u;
            USER_PORT_rxBufferTail     = 0u;
            USER_PORT_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            USER_PORT_txBufferHead = 0u;
            USER_PORT_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: USER_PORT_SpiInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for the SPI operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void USER_PORT_SpiInit(void)
    {
        /* Configure SPI interface */
        USER_PORT_CTRL_REG     = USER_PORT_SPI_DEFAULT_CTRL;
        USER_PORT_SPI_CTRL_REG = USER_PORT_SPI_DEFAULT_SPI_CTRL;

        /* Configure TX and RX direction */
        USER_PORT_RX_CTRL_REG      = USER_PORT_SPI_DEFAULT_RX_CTRL;
        USER_PORT_RX_FIFO_CTRL_REG = USER_PORT_SPI_DEFAULT_RX_FIFO_CTRL;

        /* Configure TX and RX direction */
        USER_PORT_TX_CTRL_REG      = USER_PORT_SPI_DEFAULT_TX_CTRL;
        USER_PORT_TX_FIFO_CTRL_REG = USER_PORT_SPI_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with SPI handler but do not enable it */
    #if(USER_PORT_SCB_IRQ_INTERNAL)
            CyIntDisable    (USER_PORT_ISR_NUMBER);
            CyIntSetPriority(USER_PORT_ISR_NUMBER, USER_PORT_ISR_PRIORITY);
            (void) CyIntSetVector(USER_PORT_ISR_NUMBER, &USER_PORT_SPI_UART_ISR);
    #endif /* (USER_PORT_SCB_IRQ_INTERNAL) */

        /* Configure interrupt sources */
        USER_PORT_INTR_I2C_EC_MASK_REG = USER_PORT_SPI_DEFAULT_INTR_I2C_EC_MASK;
        USER_PORT_INTR_SPI_EC_MASK_REG = USER_PORT_SPI_DEFAULT_INTR_SPI_EC_MASK;
        USER_PORT_INTR_SLAVE_MASK_REG  = USER_PORT_SPI_DEFAULT_INTR_SLAVE_MASK;
        USER_PORT_INTR_MASTER_MASK_REG = USER_PORT_SPI_DEFAULT_INTR_MASTER_MASK;
        USER_PORT_INTR_RX_MASK_REG     = USER_PORT_SPI_DEFAULT_INTR_RX_MASK;
        USER_PORT_INTR_TX_MASK_REG     = USER_PORT_SPI_DEFAULT_INTR_TX_MASK;

        /* Set active SS0 for master */
    #if (USER_PORT_SPI_MASTER_CONST)
        USER_PORT_SpiSetActiveSlaveSelect(USER_PORT_SPI_SLAVE_SELECT0);
    #endif /* (USER_PORT_SPI_MASTER_CONST) */

    #if(USER_PORT_INTERNAL_RX_SW_BUFFER_CONST)
        USER_PORT_rxBufferHead     = 0u;
        USER_PORT_rxBufferTail     = 0u;
        USER_PORT_rxBufferOverflow = 0u;
    #endif /* (USER_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(USER_PORT_INTERNAL_TX_SW_BUFFER_CONST)
        USER_PORT_txBufferHead = 0u;
        USER_PORT_txBufferTail = 0u;
    #endif /* (USER_PORT_INTERNAL_TX_SW_BUFFER_CONST) */


    }
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (USER_PORT_SPI_MASTER_CONST)
    /*******************************************************************************
    * Function Name: USER_PORT_SetActiveSlaveSelect
    ********************************************************************************
    *
    * Summary:
    *  Selects one of the four slave select lines to be active during the transfer.
    *  After initialization the active slave select line is 0.
    *  The component should be in one of the following states to change the active
    *  slave select signal source correctly:
    *   - The component is disabled
    *   - The component has completed transfer (TX FIFO is empty and the
    *     SCB_INTR_MASTER_SPI_DONE status is set)
    *  This function does not check that these conditions are met.
    *  This function is only applicable to SPI Master mode of operation.
    *
    * Parameters:
    *  slaveSelect: slave select line which will be active while the following
    *               transfer.
    *   USER_PORT_SPI_SLAVE_SELECT0 - Slave select 0
    *   USER_PORT_SPI_SLAVE_SELECT1 - Slave select 1
    *   USER_PORT_SPI_SLAVE_SELECT2 - Slave select 2
    *   USER_PORT_SPI_SLAVE_SELECT3 - Slave select 3
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void USER_PORT_SpiSetActiveSlaveSelect(uint32 slaveSelect)
    {
        uint32 spiCtrl;

        spiCtrl = USER_PORT_SPI_CTRL_REG;

        spiCtrl &= (uint32) ~USER_PORT_SPI_CTRL_SLAVE_SELECT_MASK;
        spiCtrl |= (uint32)  USER_PORT_GET_SPI_CTRL_SS(slaveSelect);

        USER_PORT_SPI_CTRL_REG = spiCtrl;
    }
#endif /* (USER_PORT_SPI_MASTER_CONST) */


#if !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: USER_PORT_SpiSetSlaveSelectPolarity
    ********************************************************************************
    *
    * Summary:
    *  Sets active polarity for slave select line.
    *  The component should be in one of the following states to change the active
    *  slave select signal source correctly:
    *   - The component is disabled.
    *   - The component has completed transfer.
    *  This function does not check that these conditions are met.
    *
    * Parameters:
    *  slaveSelect: slave select line to change active polarity.
    *   USER_PORT_SPI_SLAVE_SELECT0 - Slave select 0
    *   USER_PORT_SPI_SLAVE_SELECT1 - Slave select 1
    *   USER_PORT_SPI_SLAVE_SELECT2 - Slave select 2
    *   USER_PORT_SPI_SLAVE_SELECT3 - Slave select 3
    *
    *  polarity: active polarity of slave select line.
    *   USER_PORT_SPI_SS_ACTIVE_LOW  - Slave select is active low
    *   USER_PORT_SPI_SS_ACTIVE_HIGH - Slave select is active high
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void USER_PORT_SpiSetSlaveSelectPolarity(uint32 slaveSelect, uint32 polarity)
    {
        uint32 ssPolarity;

        /* Get position of the polarity bit associated with slave select line */
        ssPolarity = USER_PORT_GET_SPI_CTRL_SSEL_POLARITY((uint32) 1u << slaveSelect);

        if (0u != polarity)
        {
            USER_PORT_SPI_CTRL_REG |= (uint32)  ssPolarity;
        }
        else
        {
            USER_PORT_SPI_CTRL_REG &= (uint32) ~ssPolarity;
        }
    }
#endif /* !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */


#if(USER_PORT_SPI_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: USER_PORT_SpiSaveConfig
    ********************************************************************************
    *
    * Summary:
    *  Clears INTR_SPI_EC.WAKE_UP and enables it. This interrupt
    *  source triggers when the master assigns the SS line and wakes up the device.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void USER_PORT_SpiSaveConfig(void)
    {
        USER_PORT_ClearSpiExtClkInterruptSource(USER_PORT_INTR_SPI_EC_WAKE_UP);
        USER_PORT_SetSpiExtClkInterruptMode(USER_PORT_INTR_SPI_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: USER_PORT_SpiRestoreConfig
    ********************************************************************************
    *
    * Summary:
    *  Disables the INTR_SPI_EC.WAKE_UP interrupt source. After wakeup
    *  slave does not drive the MISO line and the master receives 0xFF.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void USER_PORT_SpiRestoreConfig(void)
    {
        USER_PORT_SetSpiExtClkInterruptMode(USER_PORT_NO_INTR_SOURCES);
    }
#endif /* (USER_PORT_SPI_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
