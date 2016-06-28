/*******************************************************************************
* File Name: USER_PORT_I2C.c
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

#include "USER_PORT_PVT.h"
#include "USER_PORT_I2C_PVT.h"


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 USER_PORT_state;  /* Current state of I2C FSM */

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const USER_PORT_I2C_INIT_STRUCT USER_PORT_configI2C =
    {
        USER_PORT_I2C_MODE,
        USER_PORT_I2C_OVS_FACTOR_LOW,
        USER_PORT_I2C_OVS_FACTOR_HIGH,
        USER_PORT_I2C_MEDIAN_FILTER_ENABLE,
        USER_PORT_I2C_SLAVE_ADDRESS,
        USER_PORT_I2C_SLAVE_ADDRESS_MASK,
        USER_PORT_I2C_ACCEPT_ADDRESS,
        USER_PORT_I2C_WAKE_ENABLE,
        USER_PORT_I2C_BYTE_MODE_ENABLE,
        USER_PORT_I2C_DATA_RATE,
    };

    /*******************************************************************************
    * Function Name: USER_PORT_I2CInit
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
    void USER_PORT_I2CInit(const USER_PORT_I2C_INIT_STRUCT *config)
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
            USER_PORT_SetPins(USER_PORT_SCB_MODE_I2C, USER_PORT_DUMMY_PARAM,
                                                                    USER_PORT_DUMMY_PARAM);

            /* Store internal configuration */
            USER_PORT_scbMode       = (uint8) USER_PORT_SCB_MODE_I2C;
            USER_PORT_scbEnableWake = (uint8) config->enableWake;
            USER_PORT_scbEnableIntr = (uint8) USER_PORT_SCB_IRQ_INTERNAL;

            USER_PORT_mode          = (uint8) config->mode;
            USER_PORT_acceptAddr    = (uint8) config->acceptAddr;

        #if (USER_PORT_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            USER_PORT_SET_I2C_CFG_SDA_FILT_TRIM(USER_PORT_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (USER_PORT_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((USER_PORT_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= USER_PORT_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (USER_PORT_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                USER_PORT_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = USER_PORT_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                USER_PORT_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = USER_PORT_DIGITAL_FILTER_ENABLE;
            }

        #if (!USER_PORT_CY_SCBIP_V0)
            locEnableWake = (USER_PORT_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!USER_PORT_CY_SCBIP_V0) */

            /* Configure I2C interface */
            USER_PORT_CTRL_REG     = USER_PORT_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            USER_PORT_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            USER_PORT_GET_CTRL_EC_AM_MODE (locEnableWake);

            USER_PORT_I2C_CTRL_REG = USER_PORT_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                                            USER_PORT_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)  |
                                            USER_PORT_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode)           |
                                            USER_PORT_I2C_CTRL;



            /* Configure RX direction */
            USER_PORT_RX_CTRL_REG      = USER_PORT_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                USER_PORT_I2C_RX_CTRL;
            USER_PORT_RX_FIFO_CTRL_REG = USER_PORT_CLEAR_REG;

            /* Set default address and mask */
            USER_PORT_RX_MATCH_REG    = ((USER_PORT_I2C_SLAVE) ?
                                                (USER_PORT_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 USER_PORT_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (USER_PORT_CLEAR_REG));


            /* Configure TX direction */
            USER_PORT_TX_CTRL_REG      = USER_PORT_I2C_TX_CTRL;
            USER_PORT_TX_FIFO_CTRL_REG = USER_PORT_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (USER_PORT_ISR_NUMBER);
            CyIntSetPriority(USER_PORT_ISR_NUMBER, USER_PORT_ISR_PRIORITY);
            (void) CyIntSetVector(USER_PORT_ISR_NUMBER, &USER_PORT_I2C_ISR);

            /* Configure interrupt sources */
        #if(!USER_PORT_CY_SCBIP_V1)
            USER_PORT_INTR_SPI_EC_MASK_REG = USER_PORT_NO_INTR_SOURCES;
        #endif /* (!USER_PORT_CY_SCBIP_V1) */

            USER_PORT_INTR_I2C_EC_MASK_REG = USER_PORT_NO_INTR_SOURCES;
            USER_PORT_INTR_RX_MASK_REG     = USER_PORT_NO_INTR_SOURCES;
            USER_PORT_INTR_TX_MASK_REG     = USER_PORT_NO_INTR_SOURCES;

            USER_PORT_INTR_SLAVE_MASK_REG  = ((USER_PORT_I2C_SLAVE) ?
                                                     (USER_PORT_I2C_INTR_SLAVE_MASK) :
                                                     (USER_PORT_CLEAR_REG));

            USER_PORT_INTR_MASTER_MASK_REG = ((USER_PORT_I2C_MASTER) ?
                                                     (USER_PORT_I2C_INTR_MASTER_MASK) :
                                                     (USER_PORT_CLEAR_REG));

            /* Configure global variables */
            USER_PORT_state = USER_PORT_I2C_FSM_IDLE;

            /* Internal slave variables */
            USER_PORT_slStatus        = 0u;
            USER_PORT_slRdBufIndex    = 0u;
            USER_PORT_slWrBufIndex    = 0u;
            USER_PORT_slOverFlowCount = 0u;

            /* Internal master variables */
            USER_PORT_mstrStatus     = 0u;
            USER_PORT_mstrRdBufIndex = 0u;
            USER_PORT_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: USER_PORT_I2CInit
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
    void USER_PORT_I2CInit(void)
    {
    #if(USER_PORT_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        USER_PORT_SET_I2C_CFG_SDA_FILT_TRIM(USER_PORT_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (USER_PORT_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        USER_PORT_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        USER_PORT_CTRL_REG     = USER_PORT_I2C_DEFAULT_CTRL;
        USER_PORT_I2C_CTRL_REG = USER_PORT_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        USER_PORT_RX_CTRL_REG      = USER_PORT_I2C_DEFAULT_RX_CTRL;
        USER_PORT_RX_FIFO_CTRL_REG = USER_PORT_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        USER_PORT_RX_MATCH_REG     = USER_PORT_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        USER_PORT_TX_CTRL_REG      = USER_PORT_I2C_DEFAULT_TX_CTRL;
        USER_PORT_TX_FIFO_CTRL_REG = USER_PORT_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (USER_PORT_ISR_NUMBER);
        CyIntSetPriority(USER_PORT_ISR_NUMBER, USER_PORT_ISR_PRIORITY);
    #if(!USER_PORT_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(USER_PORT_ISR_NUMBER, &USER_PORT_I2C_ISR);
    #endif /* (USER_PORT_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!USER_PORT_CY_SCBIP_V1)
        USER_PORT_INTR_SPI_EC_MASK_REG = USER_PORT_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!USER_PORT_CY_SCBIP_V1) */

        USER_PORT_INTR_I2C_EC_MASK_REG = USER_PORT_I2C_DEFAULT_INTR_I2C_EC_MASK;
        USER_PORT_INTR_SLAVE_MASK_REG  = USER_PORT_I2C_DEFAULT_INTR_SLAVE_MASK;
        USER_PORT_INTR_MASTER_MASK_REG = USER_PORT_I2C_DEFAULT_INTR_MASTER_MASK;
        USER_PORT_INTR_RX_MASK_REG     = USER_PORT_I2C_DEFAULT_INTR_RX_MASK;
        USER_PORT_INTR_TX_MASK_REG     = USER_PORT_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        USER_PORT_state = USER_PORT_I2C_FSM_IDLE;

    #if(USER_PORT_I2C_SLAVE)
        /* Internal slave variable */
        USER_PORT_slStatus        = 0u;
        USER_PORT_slRdBufIndex    = 0u;
        USER_PORT_slWrBufIndex    = 0u;
        USER_PORT_slOverFlowCount = 0u;
    #endif /* (USER_PORT_I2C_SLAVE) */

    #if(USER_PORT_I2C_MASTER)
    /* Internal master variable */
        USER_PORT_mstrStatus     = 0u;
        USER_PORT_mstrRdBufIndex = 0u;
        USER_PORT_mstrWrBufIndex = 0u;
    #endif /* (USER_PORT_I2C_MASTER) */
    }
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: USER_PORT_I2CStop
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
void USER_PORT_I2CStop(void)
{
    /* Disable TX interrupt sources in order not to cause a false trigger.
    * The incoming transaction will enable an appropriate TX interrupt.
    */
    USER_PORT_SetTxInterruptMode(USER_PORT_NO_INTR_SOURCES);

#if(USER_PORT_CY_SCBIP_V0)
    /* Clear a pending interrupt as the TX FIFO becomes empty and the block does not gate
    * an interrupt trigger when disabled.
    */
    USER_PORT_ClearPendingInt();
#endif /* (USER_PORT_CY_SCBIP_V0) */

    USER_PORT_state = USER_PORT_I2C_FSM_IDLE;
}


#if(USER_PORT_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: USER_PORT_I2CSaveConfig
    ********************************************************************************
    *
    * Summary:
    *  Enables USER_PORT_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void USER_PORT_I2CSaveConfig(void)
    {
    #if (!USER_PORT_CY_SCBIP_V0)
        #if (USER_PORT_I2C_MULTI_MASTER_SLAVE_CONST && USER_PORT_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (USER_PORT_CTRL_REG & USER_PORT_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                USER_PORT_Stop();
                USER_PORT_CTRL_REG |= USER_PORT_CTRL_EC_AM_MODE;
                USER_PORT_Enable();
            }
        #endif /* (USER_PORT_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (USER_PORT_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            USER_PORT_SCBCLK_Stop();
        #endif /* (USER_PORT_SCB_CLK_INTERNAL) */
    #endif /* (!USER_PORT_CY_SCBIP_V0) */

        USER_PORT_SetI2CExtClkInterruptMode(USER_PORT_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: USER_PORT_I2CRestoreConfig
    ********************************************************************************
    *
    * Summary:
    *  Disables USER_PORT_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void USER_PORT_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        USER_PORT_SetI2CExtClkInterruptMode(USER_PORT_NO_INTR_SOURCES);

    #if (!USER_PORT_CY_SCBIP_V0)
        #if (USER_PORT_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            USER_PORT_SCBCLK_Start();
        #endif /* (USER_PORT_SCB_CLK_INTERNAL) */
    #endif /* (!USER_PORT_CY_SCBIP_V0) */
    }
#endif /* (USER_PORT_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
