/*******************************************************************************
* File Name: EXPANSION_PORT_I2C.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  I2C mode.
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


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 EXPANSION_PORT_state;  /* Current state of I2C FSM */

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const EXPANSION_PORT_I2C_INIT_STRUCT EXPANSION_PORT_configI2C =
    {
        EXPANSION_PORT_I2C_MODE,
        EXPANSION_PORT_I2C_OVS_FACTOR_LOW,
        EXPANSION_PORT_I2C_OVS_FACTOR_HIGH,
        EXPANSION_PORT_I2C_MEDIAN_FILTER_ENABLE,
        EXPANSION_PORT_I2C_SLAVE_ADDRESS,
        EXPANSION_PORT_I2C_SLAVE_ADDRESS_MASK,
        EXPANSION_PORT_I2C_ACCEPT_ADDRESS,
        EXPANSION_PORT_I2C_WAKE_ENABLE,
        EXPANSION_PORT_I2C_BYTE_MODE_ENABLE,
        EXPANSION_PORT_I2C_DATA_RATE,
    };

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_I2CInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for I2C operation.
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
    void EXPANSION_PORT_I2CInit(const EXPANSION_PORT_I2C_INIT_STRUCT *config)
    {
        uint32 medianFilter;
        uint32 locEnableWake;

        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            EXPANSION_PORT_SetPins(EXPANSION_PORT_SCB_MODE_I2C, EXPANSION_PORT_DUMMY_PARAM,
                                                                    EXPANSION_PORT_DUMMY_PARAM);

            /* Store internal configuration */
            EXPANSION_PORT_scbMode       = (uint8) EXPANSION_PORT_SCB_MODE_I2C;
            EXPANSION_PORT_scbEnableWake = (uint8) config->enableWake;
            EXPANSION_PORT_scbEnableIntr = (uint8) EXPANSION_PORT_SCB_IRQ_INTERNAL;

            EXPANSION_PORT_mode          = (uint8) config->mode;
            EXPANSION_PORT_acceptAddr    = (uint8) config->acceptAddr;

        #if (EXPANSION_PORT_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            EXPANSION_PORT_SET_I2C_CFG_SDA_FILT_TRIM(EXPANSION_PORT_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((EXPANSION_PORT_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= EXPANSION_PORT_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (EXPANSION_PORT_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                EXPANSION_PORT_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = EXPANSION_PORT_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                EXPANSION_PORT_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = EXPANSION_PORT_DIGITAL_FILTER_ENABLE;
            }

        #if (!EXPANSION_PORT_CY_SCBIP_V0)
            locEnableWake = (EXPANSION_PORT_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */

            /* Configure I2C interface */
            EXPANSION_PORT_CTRL_REG     = EXPANSION_PORT_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            EXPANSION_PORT_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            EXPANSION_PORT_GET_CTRL_EC_AM_MODE (locEnableWake);

            EXPANSION_PORT_I2C_CTRL_REG = EXPANSION_PORT_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                                            EXPANSION_PORT_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)  |
                                            EXPANSION_PORT_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode)           |
                                            EXPANSION_PORT_I2C_CTRL;



            /* Configure RX direction */
            EXPANSION_PORT_RX_CTRL_REG      = EXPANSION_PORT_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                EXPANSION_PORT_I2C_RX_CTRL;
            EXPANSION_PORT_RX_FIFO_CTRL_REG = EXPANSION_PORT_CLEAR_REG;

            /* Set default address and mask */
            EXPANSION_PORT_RX_MATCH_REG    = ((EXPANSION_PORT_I2C_SLAVE) ?
                                                (EXPANSION_PORT_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 EXPANSION_PORT_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (EXPANSION_PORT_CLEAR_REG));


            /* Configure TX direction */
            EXPANSION_PORT_TX_CTRL_REG      = EXPANSION_PORT_I2C_TX_CTRL;
            EXPANSION_PORT_TX_FIFO_CTRL_REG = EXPANSION_PORT_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (EXPANSION_PORT_ISR_NUMBER);
            CyIntSetPriority(EXPANSION_PORT_ISR_NUMBER, EXPANSION_PORT_ISR_PRIORITY);
            (void) CyIntSetVector(EXPANSION_PORT_ISR_NUMBER, &EXPANSION_PORT_I2C_ISR);

            /* Configure interrupt sources */
        #if(!EXPANSION_PORT_CY_SCBIP_V1)
            EXPANSION_PORT_INTR_SPI_EC_MASK_REG = EXPANSION_PORT_NO_INTR_SOURCES;
        #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */

            EXPANSION_PORT_INTR_I2C_EC_MASK_REG = EXPANSION_PORT_NO_INTR_SOURCES;
            EXPANSION_PORT_INTR_RX_MASK_REG     = EXPANSION_PORT_NO_INTR_SOURCES;
            EXPANSION_PORT_INTR_TX_MASK_REG     = EXPANSION_PORT_NO_INTR_SOURCES;

            EXPANSION_PORT_INTR_SLAVE_MASK_REG  = ((EXPANSION_PORT_I2C_SLAVE) ?
                                                     (EXPANSION_PORT_I2C_INTR_SLAVE_MASK) :
                                                     (EXPANSION_PORT_CLEAR_REG));

            EXPANSION_PORT_INTR_MASTER_MASK_REG = ((EXPANSION_PORT_I2C_MASTER) ?
                                                     (EXPANSION_PORT_I2C_INTR_MASTER_MASK) :
                                                     (EXPANSION_PORT_CLEAR_REG));

            /* Configure global variables */
            EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;

            /* Internal slave variables */
            EXPANSION_PORT_slStatus        = 0u;
            EXPANSION_PORT_slRdBufIndex    = 0u;
            EXPANSION_PORT_slWrBufIndex    = 0u;
            EXPANSION_PORT_slOverFlowCount = 0u;

            /* Internal master variables */
            EXPANSION_PORT_mstrStatus     = 0u;
            EXPANSION_PORT_mstrRdBufIndex = 0u;
            EXPANSION_PORT_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_I2CInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for the I2C operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_I2CInit(void)
    {
    #if(EXPANSION_PORT_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        EXPANSION_PORT_SET_I2C_CFG_SDA_FILT_TRIM(EXPANSION_PORT_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        EXPANSION_PORT_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        EXPANSION_PORT_CTRL_REG     = EXPANSION_PORT_I2C_DEFAULT_CTRL;
        EXPANSION_PORT_I2C_CTRL_REG = EXPANSION_PORT_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        EXPANSION_PORT_RX_CTRL_REG      = EXPANSION_PORT_I2C_DEFAULT_RX_CTRL;
        EXPANSION_PORT_RX_FIFO_CTRL_REG = EXPANSION_PORT_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        EXPANSION_PORT_RX_MATCH_REG     = EXPANSION_PORT_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        EXPANSION_PORT_TX_CTRL_REG      = EXPANSION_PORT_I2C_DEFAULT_TX_CTRL;
        EXPANSION_PORT_TX_FIFO_CTRL_REG = EXPANSION_PORT_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (EXPANSION_PORT_ISR_NUMBER);
        CyIntSetPriority(EXPANSION_PORT_ISR_NUMBER, EXPANSION_PORT_ISR_PRIORITY);
    #if(!EXPANSION_PORT_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(EXPANSION_PORT_ISR_NUMBER, &EXPANSION_PORT_I2C_ISR);
    #endif /* (EXPANSION_PORT_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!EXPANSION_PORT_CY_SCBIP_V1)
        EXPANSION_PORT_INTR_SPI_EC_MASK_REG = EXPANSION_PORT_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */

        EXPANSION_PORT_INTR_I2C_EC_MASK_REG = EXPANSION_PORT_I2C_DEFAULT_INTR_I2C_EC_MASK;
        EXPANSION_PORT_INTR_SLAVE_MASK_REG  = EXPANSION_PORT_I2C_DEFAULT_INTR_SLAVE_MASK;
        EXPANSION_PORT_INTR_MASTER_MASK_REG = EXPANSION_PORT_I2C_DEFAULT_INTR_MASTER_MASK;
        EXPANSION_PORT_INTR_RX_MASK_REG     = EXPANSION_PORT_I2C_DEFAULT_INTR_RX_MASK;
        EXPANSION_PORT_INTR_TX_MASK_REG     = EXPANSION_PORT_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;

    #if(EXPANSION_PORT_I2C_SLAVE)
        /* Internal slave variable */
        EXPANSION_PORT_slStatus        = 0u;
        EXPANSION_PORT_slRdBufIndex    = 0u;
        EXPANSION_PORT_slWrBufIndex    = 0u;
        EXPANSION_PORT_slOverFlowCount = 0u;
    #endif /* (EXPANSION_PORT_I2C_SLAVE) */

    #if(EXPANSION_PORT_I2C_MASTER)
    /* Internal master variable */
        EXPANSION_PORT_mstrStatus     = 0u;
        EXPANSION_PORT_mstrRdBufIndex = 0u;
        EXPANSION_PORT_mstrWrBufIndex = 0u;
    #endif /* (EXPANSION_PORT_I2C_MASTER) */
    }
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: EXPANSION_PORT_I2CStop
********************************************************************************
*
* Summary:
*  Resets the I2C FSM into the default state and disables TX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_I2CStop(void)
{
    /* Disable TX interrupt sources in order not to cause a false trigger.
    * The incoming transaction will enable an appropriate TX interrupt.
    */
    EXPANSION_PORT_SetTxInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

#if(EXPANSION_PORT_CY_SCBIP_V0)
    /* Clear a pending interrupt as the TX FIFO becomes empty and the block does not gate
    * an interrupt trigger when disabled.
    */
    EXPANSION_PORT_ClearPendingInt();
#endif /* (EXPANSION_PORT_CY_SCBIP_V0) */

    EXPANSION_PORT_state = EXPANSION_PORT_I2C_FSM_IDLE;
}


#if(EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_I2CSaveConfig
    ********************************************************************************
    *
    * Summary:
    *  Enables EXPANSION_PORT_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_I2CSaveConfig(void)
    {
    #if (!EXPANSION_PORT_CY_SCBIP_V0)
        #if (EXPANSION_PORT_I2C_MULTI_MASTER_SLAVE_CONST && EXPANSION_PORT_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (EXPANSION_PORT_CTRL_REG & EXPANSION_PORT_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                EXPANSION_PORT_Stop();
                EXPANSION_PORT_CTRL_REG |= EXPANSION_PORT_CTRL_EC_AM_MODE;
                EXPANSION_PORT_Enable();
            }
        #endif /* (EXPANSION_PORT_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (EXPANSION_PORT_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            EXPANSION_PORT_SCBCLK_Stop();
        #endif /* (EXPANSION_PORT_SCB_CLK_INTERNAL) */
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */

        EXPANSION_PORT_SetI2CExtClkInterruptMode(EXPANSION_PORT_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: EXPANSION_PORT_I2CRestoreConfig
    ********************************************************************************
    *
    * Summary:
    *  Disables EXPANSION_PORT_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        EXPANSION_PORT_SetI2CExtClkInterruptMode(EXPANSION_PORT_NO_INTR_SOURCES);

    #if (!EXPANSION_PORT_CY_SCBIP_V0)
        #if (EXPANSION_PORT_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            EXPANSION_PORT_SCBCLK_Start();
        #endif /* (EXPANSION_PORT_SCB_CLK_INTERNAL) */
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V0) */
    }
#endif /* (EXPANSION_PORT_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
