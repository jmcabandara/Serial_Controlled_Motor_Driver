/*******************************************************************************
* File Name: EXPANSION_PORT_SPI.c
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

#include "EXPANSION_PORT_PVT.h"
#include "EXPANSION_PORT_SPI_UART_PVT.h"

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const EXPANSION_PORT_SPI_INIT_STRUCT EXPANSION_PORT_configSpi =
    {
        EXPANSION_PORT_SPI_MODE,
        EXPANSION_PORT_SPI_SUB_MODE,
        EXPANSION_PORT_SPI_CLOCK_MODE,
        EXPANSION_PORT_SPI_OVS_FACTOR,
        EXPANSION_PORT_SPI_MEDIAN_FILTER_ENABLE,
        EXPANSION_PORT_SPI_LATE_MISO_SAMPLE_ENABLE,
        EXPANSION_PORT_SPI_WAKE_ENABLE,
        EXPANSION_PORT_SPI_RX_DATA_BITS_NUM,
        EXPANSION_PORT_SPI_TX_DATA_BITS_NUM,
        EXPANSION_PORT_SPI_BITS_ORDER,
        EXPANSION_PORT_SPI_TRANSFER_SEPARATION,
        0u,
        NULL,
        0u,
        NULL,
        (uint32) EXPANSION_PORT_SCB_IRQ_INTERNAL,
        EXPANSION_PORT_SPI_INTR_RX_MASK,
        EXPANSION_PORT_SPI_RX_TRIGGER_LEVEL,
        EXPANSION_PORT_SPI_INTR_TX_MASK,
        EXPANSION_PORT_SPI_TX_TRIGGER_LEVEL,
        (uint8) EXPANSION_PORT_SPI_BYTE_MODE_ENABLE,
        (uint8) EXPANSION_PORT_SPI_FREE_RUN_SCLK_ENABLE,
        (uint8) EXPANSION_PORT_SPI_SS_POLARITY
    };


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiInit
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
    void EXPANSION_PORT_SpiInit(const EXPANSION_PORT_SPI_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            EXPANSION_PORT_SetPins(EXPANSION_PORT_SCB_MODE_SPI, config->mode, EXPANSION_PORT_DUMMY_PARAM);

            /* Store internal configuration */
            EXPANSION_PORT_scbMode       = (uint8) EXPANSION_PORT_SCB_MODE_SPI;
            EXPANSION_PORT_scbEnableWake = (uint8) config->enableWake;
            EXPANSION_PORT_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            EXPANSION_PORT_rxBuffer      =         config->rxBuffer;
            EXPANSION_PORT_rxDataBits    = (uint8) config->rxDataBits;
            EXPANSION_PORT_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            EXPANSION_PORT_txBuffer      =         config->txBuffer;
            EXPANSION_PORT_txDataBits    = (uint8) config->txDataBits;
            EXPANSION_PORT_txBufferSize  = (uint8) config->txBufferSize;

            /* Configure SPI interface */
            EXPANSION_PORT_CTRL_REG     = EXPANSION_PORT_GET_CTRL_OVS(config->oversample)           |
                                            EXPANSION_PORT_GET_CTRL_BYTE_MODE(config->enableByteMode) |
                                            EXPANSION_PORT_GET_CTRL_EC_AM_MODE(config->enableWake)    |
                                            EXPANSION_PORT_CTRL_SPI;

            EXPANSION_PORT_SPI_CTRL_REG = EXPANSION_PORT_GET_SPI_CTRL_CONTINUOUS    (config->transferSeperation)  |
                                            EXPANSION_PORT_GET_SPI_CTRL_SELECT_PRECEDE(config->submode &
                                                                          EXPANSION_PORT_SPI_MODE_TI_PRECEDES_MASK) |
                                            EXPANSION_PORT_GET_SPI_CTRL_SCLK_MODE     (config->sclkMode)            |
                                            EXPANSION_PORT_GET_SPI_CTRL_LATE_MISO_SAMPLE(config->enableLateSampling)|
                                            EXPANSION_PORT_GET_SPI_CTRL_SCLK_CONTINUOUS(config->enableFreeRunSclk)  |
                                            EXPANSION_PORT_GET_SPI_CTRL_SSEL_POLARITY (config->polaritySs)          |
                                            EXPANSION_PORT_GET_SPI_CTRL_SUB_MODE      (config->submode)             |
                                            EXPANSION_PORT_GET_SPI_CTRL_MASTER_MODE   (config->mode);

            /* Configure RX direction */
            EXPANSION_PORT_RX_CTRL_REG     =  EXPANSION_PORT_GET_RX_CTRL_DATA_WIDTH(config->rxDataBits)         |
                                                EXPANSION_PORT_GET_RX_CTRL_BIT_ORDER (config->bitOrder)           |
                                                EXPANSION_PORT_GET_RX_CTRL_MEDIAN    (config->enableMedianFilter) |
                                                EXPANSION_PORT_SPI_RX_CTRL;

            EXPANSION_PORT_RX_FIFO_CTRL_REG = EXPANSION_PORT_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure TX direction */
            EXPANSION_PORT_TX_CTRL_REG      = EXPANSION_PORT_GET_TX_CTRL_DATA_WIDTH(config->txDataBits) |
                                                EXPANSION_PORT_GET_TX_CTRL_BIT_ORDER (config->bitOrder)   |
                                                EXPANSION_PORT_SPI_TX_CTRL;

            EXPANSION_PORT_TX_FIFO_CTRL_REG = EXPANSION_PORT_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

            /* Configure interrupt with SPI handler but do not enable it */
            CyIntDisable    (EXPANSION_PORT_ISR_NUMBER);
            CyIntSetPriority(EXPANSION_PORT_ISR_NUMBER, EXPANSION_PORT_ISR_PRIORITY);
            (void) CyIntSetVector(EXPANSION_PORT_ISR_NUMBER, &EXPANSION_PORT_SPI_UART_ISR);

            /* Configure interrupt sources */
            EXPANSION_PORT_INTR_I2C_EC_MASK_REG = EXPANSION_PORT_NO_INTR_SOURCES;
            EXPANSION_PORT_INTR_SPI_EC_MASK_REG = EXPANSION_PORT_NO_INTR_SOURCES;
            EXPANSION_PORT_INTR_SLAVE_MASK_REG  = EXPANSION_PORT_GET_SPI_INTR_SLAVE_MASK(config->rxInterruptMask);
            EXPANSION_PORT_INTR_MASTER_MASK_REG = EXPANSION_PORT_GET_SPI_INTR_MASTER_MASK(config->txInterruptMask);
            EXPANSION_PORT_INTR_RX_MASK_REG     = EXPANSION_PORT_GET_SPI_INTR_RX_MASK(config->rxInterruptMask);
            EXPANSION_PORT_INTR_TX_MASK_REG     = EXPANSION_PORT_GET_SPI_INTR_TX_MASK(config->txInterruptMask);

            /* Set active SS0 */
            EXPANSION_PORT_SpiSetActiveSlaveSelect(EXPANSION_PORT_SPI_SLAVE_SELECT0);

            /* Clear RX buffer indexes */
            EXPANSION_PORT_rxBufferHead     = 0u;
            EXPANSION_PORT_rxBufferTail     = 0u;
            EXPANSION_PORT_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            EXPANSION_PORT_txBufferHead = 0u;
            EXPANSION_PORT_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiInit
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
    void EXPANSION_PORT_SpiInit(void)
    {
        /* Configure SPI interface */
        EXPANSION_PORT_CTRL_REG     = EXPANSION_PORT_SPI_DEFAULT_CTRL;
        EXPANSION_PORT_SPI_CTRL_REG = EXPANSION_PORT_SPI_DEFAULT_SPI_CTRL;

        /* Configure TX and RX direction */
        EXPANSION_PORT_RX_CTRL_REG      = EXPANSION_PORT_SPI_DEFAULT_RX_CTRL;
        EXPANSION_PORT_RX_FIFO_CTRL_REG = EXPANSION_PORT_SPI_DEFAULT_RX_FIFO_CTRL;

        /* Configure TX and RX direction */
        EXPANSION_PORT_TX_CTRL_REG      = EXPANSION_PORT_SPI_DEFAULT_TX_CTRL;
        EXPANSION_PORT_TX_FIFO_CTRL_REG = EXPANSION_PORT_SPI_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with SPI handler but do not enable it */
    #if(EXPANSION_PORT_SCB_IRQ_INTERNAL)
            CyIntDisable    (EXPANSION_PORT_ISR_NUMBER);
            CyIntSetPriority(EXPANSION_PORT_ISR_NUMBER, EXPANSION_PORT_ISR_PRIORITY);
            (void) CyIntSetVector(EXPANSION_PORT_ISR_NUMBER, &EXPANSION_PORT_SPI_UART_ISR);
    #endif /* (EXPANSION_PORT_SCB_IRQ_INTERNAL) */

        /* Configure interrupt sources */
        EXPANSION_PORT_INTR_I2C_EC_MASK_REG = EXPANSION_PORT_SPI_DEFAULT_INTR_I2C_EC_MASK;
        EXPANSION_PORT_INTR_SPI_EC_MASK_REG = EXPANSION_PORT_SPI_DEFAULT_INTR_SPI_EC_MASK;
        EXPANSION_PORT_INTR_SLAVE_MASK_REG  = EXPANSION_PORT_SPI_DEFAULT_INTR_SLAVE_MASK;
        EXPANSION_PORT_INTR_MASTER_MASK_REG = EXPANSION_PORT_SPI_DEFAULT_INTR_MASTER_MASK;
        EXPANSION_PORT_INTR_RX_MASK_REG     = EXPANSION_PORT_SPI_DEFAULT_INTR_RX_MASK;
        EXPANSION_PORT_INTR_TX_MASK_REG     = EXPANSION_PORT_SPI_DEFAULT_INTR_TX_MASK;

        /* Set active SS0 for master */
    #if (EXPANSION_PORT_SPI_MASTER_CONST)
        EXPANSION_PORT_SpiSetActiveSlaveSelect(EXPANSION_PORT_SPI_SLAVE_SELECT0);
    #endif /* (EXPANSION_PORT_SPI_MASTER_CONST) */

    #if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST)
        EXPANSION_PORT_rxBufferHead     = 0u;
        EXPANSION_PORT_rxBufferTail     = 0u;
        EXPANSION_PORT_rxBufferOverflow = 0u;
    #endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST)
        EXPANSION_PORT_txBufferHead = 0u;
        EXPANSION_PORT_txBufferTail = 0u;
    #endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST) */


    }
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (EXPANSION_PORT_SPI_MASTER_CONST)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SetActiveSlaveSelect
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
    *   EXPANSION_PORT_SPI_SLAVE_SELECT0 - Slave select 0
    *   EXPANSION_PORT_SPI_SLAVE_SELECT1 - Slave select 1
    *   EXPANSION_PORT_SPI_SLAVE_SELECT2 - Slave select 2
    *   EXPANSION_PORT_SPI_SLAVE_SELECT3 - Slave select 3
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_SpiSetActiveSlaveSelect(uint32 slaveSelect)
    {
        uint32 spiCtrl;

        spiCtrl = EXPANSION_PORT_SPI_CTRL_REG;

        spiCtrl &= (uint32) ~EXPANSION_PORT_SPI_CTRL_SLAVE_SELECT_MASK;
        spiCtrl |= (uint32)  EXPANSION_PORT_GET_SPI_CTRL_SS(slaveSelect);

        EXPANSION_PORT_SPI_CTRL_REG = spiCtrl;
    }
#endif /* (EXPANSION_PORT_SPI_MASTER_CONST) */


#if !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiSetSlaveSelectPolarity
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
    *   EXPANSION_PORT_SPI_SLAVE_SELECT0 - Slave select 0
    *   EXPANSION_PORT_SPI_SLAVE_SELECT1 - Slave select 1
    *   EXPANSION_PORT_SPI_SLAVE_SELECT2 - Slave select 2
    *   EXPANSION_PORT_SPI_SLAVE_SELECT3 - Slave select 3
    *
    *  polarity: active polarity of slave select line.
    *   EXPANSION_PORT_SPI_SS_ACTIVE_LOW  - Slave select is active low
    *   EXPANSION_PORT_SPI_SS_ACTIVE_HIGH - Slave select is active high
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_SpiSetSlaveSelectPolarity(uint32 slaveSelect, uint32 polarity)
    {
        uint32 ssPolarity;

        /* Get position of the polarity bit associated with slave select line */
        ssPolarity = EXPANSION_PORT_GET_SPI_CTRL_SSEL_POLARITY((uint32) 1u << slaveSelect);

        if (0u != polarity)
        {
            EXPANSION_PORT_SPI_CTRL_REG |= (uint32)  ssPolarity;
        }
        else
        {
            EXPANSION_PORT_SPI_CTRL_REG &= (uint32) ~ssPolarity;
        }
    }
#endif /* !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */


#if(EXPANSION_PORT_SPI_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiSaveConfig
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
    void EXPANSION_PORT_SpiSaveConfig(void)
    {
        EXPANSION_PORT_ClearSpiExtClkInterruptSource(EXPANSION_PORT_INTR_SPI_EC_WAKE_UP);
        EXPANSION_PORT_SetSpiExtClkInterruptMode(EXPANSION_PORT_INTR_SPI_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SpiRestoreConfig
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
    void EXPANSION_PORT_SpiRestoreConfig(void)
    {
        EXPANSION_PORT_SetSpiExtClkInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);
    }
#endif /* (EXPANSION_PORT_SPI_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
