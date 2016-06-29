/*******************************************************************************
* File Name: EXPANSION_PORT.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the SCB Component.
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

#if(EXPANSION_PORT_SCB_MODE_I2C_INC)
    #include "EXPANSION_PORT_I2C_PVT.h"
#endif /* (EXPANSION_PORT_SCB_MODE_I2C_INC) */

#if(EXPANSION_PORT_SCB_MODE_EZI2C_INC)
    #include "EXPANSION_PORT_EZI2C_PVT.h"
#endif /* (EXPANSION_PORT_SCB_MODE_EZI2C_INC) */

#if(EXPANSION_PORT_SCB_MODE_SPI_INC || EXPANSION_PORT_SCB_MODE_UART_INC)
    #include "EXPANSION_PORT_SPI_UART_PVT.h"
#endif /* (EXPANSION_PORT_SCB_MODE_SPI_INC || EXPANSION_PORT_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 EXPANSION_PORT_scbMode = EXPANSION_PORT_SCB_MODE_UNCONFIG;
    uint8 EXPANSION_PORT_scbEnableWake;
    uint8 EXPANSION_PORT_scbEnableIntr;

    /* I2C configuration variables */
    uint8 EXPANSION_PORT_mode;
    uint8 EXPANSION_PORT_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * EXPANSION_PORT_rxBuffer;
    uint8  EXPANSION_PORT_rxDataBits;
    uint32 EXPANSION_PORT_rxBufferSize;

    volatile uint8 * EXPANSION_PORT_txBuffer;
    uint8  EXPANSION_PORT_txDataBits;
    uint32 EXPANSION_PORT_txBufferSize;

    /* EZI2C configuration variables */
    uint8 EXPANSION_PORT_numberOfAddr;
    uint8 EXPANSION_PORT_subAddrSize;
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/

uint8 EXPANSION_PORT_initVar = 0u;

#if !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER)
    cyisraddress EXPANSION_PORT_customIntrHandler = NULL;
#endif /* !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER) */


/***************************************
*    Private Function Prototypes
***************************************/

static void EXPANSION_PORT_ScbEnableIntr(void);
static void EXPANSION_PORT_ScbModeStop(void);


/*******************************************************************************
* Function Name: EXPANSION_PORT_Init
********************************************************************************
*
* Summary:
*  Initializes the SCB component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  SCB_I2CInit, SCB_SpiInit, SCB_UartInit or SCB_EzI2CInit.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_Init(void)
{
#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    if(EXPANSION_PORT_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        EXPANSION_PORT_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif(EXPANSION_PORT_SCB_MODE_I2C_CONST_CFG)
    EXPANSION_PORT_I2CInit();

#elif(EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG)
    EXPANSION_PORT_SpiInit();

#elif(EXPANSION_PORT_SCB_MODE_UART_CONST_CFG)
    EXPANSION_PORT_UartInit();

#elif(EXPANSION_PORT_SCB_MODE_EZI2C_CONST_CFG)
    EXPANSION_PORT_EzI2CInit();

#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_Enable
********************************************************************************
*
* Summary:
*  Enables the SCB component operation.
*  The SCB configuration should be not changed when the component is enabled.
*  Any configuration changes should be made after disabling the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_Enable(void)
{
#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if(!EXPANSION_PORT_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        EXPANSION_PORT_CTRL_REG |= EXPANSION_PORT_CTRL_ENABLED;

        EXPANSION_PORT_ScbEnableIntr();
    }
#else
    EXPANSION_PORT_CTRL_REG |= EXPANSION_PORT_CTRL_ENABLED;

    EXPANSION_PORT_ScbEnableIntr();
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_Start
********************************************************************************
*
* Summary:
*  Invokes SCB_Init() and SCB_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZ I2C. Otherwise this function does not enable the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  EXPANSION_PORT_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void EXPANSION_PORT_Start(void)
{
    if(0u == EXPANSION_PORT_initVar)
    {
        EXPANSION_PORT_Init();
        EXPANSION_PORT_initVar = 1u; /* Component was initialized */
    }

    EXPANSION_PORT_Enable();
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_Stop
********************************************************************************
*
* Summary:
*  Disables the SCB component and its interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_Stop(void)
{
#if(EXPANSION_PORT_SCB_IRQ_INTERNAL)
    EXPANSION_PORT_DisableInt();
#endif /* (EXPANSION_PORT_SCB_IRQ_INTERNAL) */

    EXPANSION_PORT_CTRL_REG &= (uint32) ~EXPANSION_PORT_CTRL_ENABLED;  /* Disable scb IP */

#if(EXPANSION_PORT_SCB_IRQ_INTERNAL)
    EXPANSION_PORT_ClearPendingInt();
#endif /* (EXPANSION_PORT_SCB_IRQ_INTERNAL) */

    EXPANSION_PORT_ScbModeStop(); /* Calls scbMode specific Stop function */
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_SetRxFifoLevel
********************************************************************************
*
* Summary:
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
* Parameters:
*  level: Level in the RX FIFO to generate RX level interrupt.
*         The range of valid level values is between 0 and RX FIFO depth - 1.
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = EXPANSION_PORT_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~EXPANSION_PORT_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (EXPANSION_PORT_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    EXPANSION_PORT_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_SetTxFifoLevel
********************************************************************************
*
* Summary:
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has more entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
* Parameters:
*  level: Level in the TX FIFO to generate TX level interrupt.
*         The range of valid level values is between 0 and TX FIFO depth - 1.
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = EXPANSION_PORT_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~EXPANSION_PORT_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (EXPANSION_PORT_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    EXPANSION_PORT_TX_FIFO_CTRL_REG = txFifoCtrl;
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_SetCustomInterruptHandler
********************************************************************************
*
* Summary:
*  Registers a function to be called by the internal interrupt handler.
*  First the function that is registered is called, then the internal interrupt
*  handler performs any operation such as software buffer management functions
*  before the interrupt returns.  It is the user's responsibility not to break
*  the software buffer operations. Only one custom handler is supported, which
*  is the function provided by the most recent call.
*  At the initialization time no custom handler is registered.
*
* Parameters:
*  func: Pointer to the function to register.
*        The value NULL indicates to remove the current custom interrupt
*        handler.
*
* Return:
*  None
*
*******************************************************************************/
void EXPANSION_PORT_SetCustomInterruptHandler(cyisraddress func)
{
#if !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER)
    EXPANSION_PORT_customIntrHandler = func; /* Register interrupt handler */
#else
    if(NULL != func)
    {
        /* Suppress compiler warning */
    }
#endif /* !defined (CY_REMOVE_EXPANSION_PORT_CUSTOM_INTR_HANDLER) */
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Enables an interrupt for a specific mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void EXPANSION_PORT_ScbEnableIntr(void)
{
#if(EXPANSION_PORT_SCB_IRQ_INTERNAL)
    #if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Enable interrupt in NVIC */
        if(0u != EXPANSION_PORT_scbEnableIntr)
        {
            EXPANSION_PORT_EnableInt();
        }
    #else
        EXPANSION_PORT_EnableInt();

    #endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (EXPANSION_PORT_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_ScbModeStop
********************************************************************************
*
* Summary:
*  Calls the Stop function for a specific operation mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void EXPANSION_PORT_ScbModeStop(void)
{
#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    if(EXPANSION_PORT_SCB_MODE_I2C_RUNTM_CFG)
    {
        EXPANSION_PORT_I2CStop();
    }
    else if (EXPANSION_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        EXPANSION_PORT_EzI2CStop();
    }
#if (!EXPANSION_PORT_CY_SCBIP_V1)
    else if (EXPANSION_PORT_SCB_MODE_UART_RUNTM_CFG)
    {
        EXPANSION_PORT_UartStop();
    }
#endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */
    else
    {
        /* Do nothing for other modes */
    }
#elif(EXPANSION_PORT_SCB_MODE_I2C_CONST_CFG)
    EXPANSION_PORT_I2CStop();

#elif(EXPANSION_PORT_SCB_MODE_EZI2C_CONST_CFG)
    EXPANSION_PORT_EzI2CStop();

#elif(EXPANSION_PORT_SCB_MODE_UART_CONST_CFG)
    EXPANSION_PORT_UartStop();

#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_SetPins
    ********************************************************************************
    *
    * Summary:
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    * Parameters:
    *  mode:      Mode of SCB operation.
    *  subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 hsiomSel [EXPANSION_PORT_SCB_PINS_NUMBER];
        uint32 pinsDm   [EXPANSION_PORT_SCB_PINS_NUMBER];

    #if (!EXPANSION_PORT_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */

        uint32 i;

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for(i = 0u; i < EXPANSION_PORT_SCB_PINS_NUMBER; i++)
        {
            hsiomSel[i]  = EXPANSION_PORT_HSIOM_DEF_SEL;
            pinsDm[i]    = EXPANSION_PORT_PIN_DM_ALG_HIZ;
        }

        if((EXPANSION_PORT_SCB_MODE_I2C   == mode) ||
           (EXPANSION_PORT_SCB_MODE_EZI2C == mode))
        {
            hsiomSel[EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX] = EXPANSION_PORT_HSIOM_I2C_SEL;
            hsiomSel[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_HSIOM_I2C_SEL;

            pinsDm[EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_OD_LO;
            pinsDm[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_OD_LO;
        }
    #if (!EXPANSION_PORT_CY_SCBIP_V1)
        else if(EXPANSION_PORT_SCB_MODE_SPI == mode)
        {
            hsiomSel[EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX] = EXPANSION_PORT_HSIOM_SPI_SEL;
            hsiomSel[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_HSIOM_SPI_SEL;
            hsiomSel[EXPANSION_PORT_SCLK_PIN_INDEX]        = EXPANSION_PORT_HSIOM_SPI_SEL;

            if(EXPANSION_PORT_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_DIG_HIZ;
                pinsDm[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_STRONG;
                pinsDm[EXPANSION_PORT_SCLK_PIN_INDEX]        = EXPANSION_PORT_PIN_DM_DIG_HIZ;

            #if(EXPANSION_PORT_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[EXPANSION_PORT_SS0_PIN_INDEX] = EXPANSION_PORT_HSIOM_SPI_SEL;
                pinsDm  [EXPANSION_PORT_SS0_PIN_INDEX] = EXPANSION_PORT_PIN_DM_DIG_HIZ;
            #endif /* (EXPANSION_PORT_SS1_PIN) */

            #if(EXPANSION_PORT_MISO_SDA_TX_PIN)
                /* Disable input buffer */
                 pinsInBuf |= EXPANSION_PORT_MISO_SDA_TX_PIN_MASK;
            #endif /* (EXPANSION_PORT_MISO_SDA_TX_PIN_PIN) */
            }
            else /* (Master) */
            {
                pinsDm[EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_STRONG;
                pinsDm[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_DIG_HIZ;
                pinsDm[EXPANSION_PORT_SCLK_PIN_INDEX]        = EXPANSION_PORT_PIN_DM_STRONG;

            #if(EXPANSION_PORT_SS0_PIN)
                hsiomSel [EXPANSION_PORT_SS0_PIN_INDEX] = EXPANSION_PORT_HSIOM_SPI_SEL;
                pinsDm   [EXPANSION_PORT_SS0_PIN_INDEX] = EXPANSION_PORT_PIN_DM_STRONG;
                pinsInBuf                                |= EXPANSION_PORT_SS0_PIN_MASK;
            #endif /* (EXPANSION_PORT_SS0_PIN) */

            #if(EXPANSION_PORT_SS1_PIN)
                hsiomSel [EXPANSION_PORT_SS1_PIN_INDEX] = EXPANSION_PORT_HSIOM_SPI_SEL;
                pinsDm   [EXPANSION_PORT_SS1_PIN_INDEX] = EXPANSION_PORT_PIN_DM_STRONG;
                pinsInBuf                                |= EXPANSION_PORT_SS1_PIN_MASK;
            #endif /* (EXPANSION_PORT_SS1_PIN) */

            #if(EXPANSION_PORT_SS2_PIN)
                hsiomSel [EXPANSION_PORT_SS2_PIN_INDEX] = EXPANSION_PORT_HSIOM_SPI_SEL;
                pinsDm   [EXPANSION_PORT_SS2_PIN_INDEX] = EXPANSION_PORT_PIN_DM_STRONG;
                pinsInBuf                                |= EXPANSION_PORT_SS2_PIN_MASK;
            #endif /* (EXPANSION_PORT_SS2_PIN) */

            #if(EXPANSION_PORT_SS3_PIN)
                hsiomSel [EXPANSION_PORT_SS3_PIN_INDEX] = EXPANSION_PORT_HSIOM_SPI_SEL;
                pinsDm   [EXPANSION_PORT_SS3_PIN_INDEX] = EXPANSION_PORT_PIN_DM_STRONG;
                pinsInBuf                                |= EXPANSION_PORT_SS3_PIN_MASK;
            #endif /* (EXPANSION_PORT_SS2_PIN) */

                /* Disable input buffers */
            #if(EXPANSION_PORT_MOSI_SCL_RX_PIN)
                pinsInBuf |= EXPANSION_PORT_MOSI_SCL_RX_PIN_MASK;
            #endif /* (EXPANSION_PORT_MOSI_SCL_RX_PIN) */

             #if(EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN)
                pinsInBuf |= EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN_MASK;
            #endif /* (EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN) */

            #if(EXPANSION_PORT_SCLK_PIN)
                pinsInBuf |= EXPANSION_PORT_SCLK_PIN_MASK;
            #endif /* (EXPANSION_PORT_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if(EXPANSION_PORT_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
                hsiomSel[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_HSIOM_UART_SEL;
                pinsDm  [EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_OD_LO;
            }
            else /* Standard or IrDA */
            {
                if(0u != (EXPANSION_PORT_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                    hsiomSel[EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX] = EXPANSION_PORT_HSIOM_UART_SEL;
                    pinsDm  [EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_DIG_HIZ;
                }

                if(0u != (EXPANSION_PORT_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                    hsiomSel[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_HSIOM_UART_SEL;
                    pinsDm  [EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX] = EXPANSION_PORT_PIN_DM_STRONG;

                #if(EXPANSION_PORT_MISO_SDA_TX_PIN)
                     pinsInBuf |= EXPANSION_PORT_MISO_SDA_TX_PIN_MASK;
                #endif /* (EXPANSION_PORT_MISO_SDA_TX_PIN_PIN) */
                }

            #if !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
                if(EXPANSION_PORT_UART_MODE_STD == subMode)
                {
                    if(0u != (EXPANSION_PORT_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                        hsiomSel[EXPANSION_PORT_SCLK_PIN_INDEX] = EXPANSION_PORT_HSIOM_UART_SEL;
                        pinsDm  [EXPANSION_PORT_SCLK_PIN_INDEX] = EXPANSION_PORT_PIN_DM_DIG_HIZ;
                    }

                    if(0u != (EXPANSION_PORT_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                        hsiomSel[EXPANSION_PORT_SS0_PIN_INDEX] = EXPANSION_PORT_HSIOM_UART_SEL;
                        pinsDm  [EXPANSION_PORT_SS0_PIN_INDEX] = EXPANSION_PORT_PIN_DM_STRONG;

                    #if(EXPANSION_PORT_SS0_PIN)
                        /* Disable input buffer */
                        pinsInBuf |= EXPANSION_PORT_SS0_PIN_MASK;
                    #endif /* (EXPANSION_PORT_SS0_PIN) */
                    }
                }
            #endif /* !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */
            }
        }
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if(EXPANSION_PORT_MOSI_SCL_RX_PIN)
        EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_MOSI_SCL_RX_HSIOM_REG,
                                       EXPANSION_PORT_MOSI_SCL_RX_HSIOM_MASK,
                                       EXPANSION_PORT_MOSI_SCL_RX_HSIOM_POS,
                                       hsiomSel[EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX]);

        EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_SetDriveMode((uint8) pinsDm[EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX]);

    #if (!EXPANSION_PORT_CY_SCBIP_V1)
        EXPANSION_PORT_SET_INP_DIS(EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_INP_DIS,
                                     EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_MASK,
                                     (0u != (pinsInBuf & EXPANSION_PORT_MOSI_SCL_RX_PIN_MASK)));
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */
    #endif /* (EXPANSION_PORT_MOSI_SCL_RX_PIN) */

    #if(EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN)
        EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_REG,
                                       EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_MASK,
                                       EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_POS,
                                       hsiomSel[EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake_SetDriveMode((uint8)
                                                               pinsDm[EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        EXPANSION_PORT_SET_INP_DIS(EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS,
                                     EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake_MASK,
                                     (0u != (pinsInBuf & EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN_MASK)));

         /* Set interrupt on falling edge */
        EXPANSION_PORT_SET_INCFG_TYPE(EXPANSION_PORT_MOSI_SCL_RX_WAKE_INTCFG_REG,
                                        EXPANSION_PORT_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK,
                                        EXPANSION_PORT_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS,
                                        EXPANSION_PORT_INTCFG_TYPE_FALLING_EDGE);
    #endif /* (EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN) */

    #if(EXPANSION_PORT_MISO_SDA_TX_PIN)
        EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_MISO_SDA_TX_HSIOM_REG,
                                       EXPANSION_PORT_MISO_SDA_TX_HSIOM_MASK,
                                       EXPANSION_PORT_MISO_SDA_TX_HSIOM_POS,
                                       hsiomSel[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX]);

        EXPANSION_PORT_spi_miso_i2c_sda_uart_tx_SetDriveMode((uint8) pinsDm[EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX]);

    #if (!EXPANSION_PORT_CY_SCBIP_V1)
        EXPANSION_PORT_SET_INP_DIS(EXPANSION_PORT_spi_miso_i2c_sda_uart_tx_INP_DIS,
                                     EXPANSION_PORT_spi_miso_i2c_sda_uart_tx_MASK,
                                    (0u != (pinsInBuf & EXPANSION_PORT_MISO_SDA_TX_PIN_MASK)));
    #endif /* (!EXPANSION_PORT_CY_SCBIP_V1) */
    #endif /* (EXPANSION_PORT_MOSI_SCL_RX_PIN) */

    #if(EXPANSION_PORT_SCLK_PIN)
        EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_SCLK_HSIOM_REG, EXPANSION_PORT_SCLK_HSIOM_MASK,
                                       EXPANSION_PORT_SCLK_HSIOM_POS, hsiomSel[EXPANSION_PORT_SCLK_PIN_INDEX]);

        EXPANSION_PORT_spi_sclk_SetDriveMode((uint8) pinsDm[EXPANSION_PORT_SCLK_PIN_INDEX]);

        EXPANSION_PORT_SET_INP_DIS(EXPANSION_PORT_spi_sclk_INP_DIS,
                                     EXPANSION_PORT_spi_sclk_MASK,
                                     (0u != (pinsInBuf & EXPANSION_PORT_SCLK_PIN_MASK)));
    #endif /* (EXPANSION_PORT_SCLK_PIN) */

    #if(EXPANSION_PORT_SS0_PIN)
        EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_SS0_HSIOM_REG, EXPANSION_PORT_SS0_HSIOM_MASK,
                                       EXPANSION_PORT_SS0_HSIOM_POS, hsiomSel[EXPANSION_PORT_SS0_PIN_INDEX]);

        EXPANSION_PORT_spi_ss0_SetDriveMode((uint8) pinsDm[EXPANSION_PORT_SS0_PIN_INDEX]);

        EXPANSION_PORT_SET_INP_DIS(EXPANSION_PORT_spi_ss0_INP_DIS,
                                     EXPANSION_PORT_spi_ss0_MASK,
                                     (0u != (pinsInBuf & EXPANSION_PORT_SS0_PIN_MASK)));
    #endif /* (EXPANSION_PORT_SS1_PIN) */

    #if(EXPANSION_PORT_SS1_PIN)
        EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_SS1_HSIOM_REG, EXPANSION_PORT_SS1_HSIOM_MASK,
                                       EXPANSION_PORT_SS1_HSIOM_POS, hsiomSel[EXPANSION_PORT_SS1_PIN_INDEX]);

        EXPANSION_PORT_spi_ss1_SetDriveMode((uint8) pinsDm[EXPANSION_PORT_SS1_PIN_INDEX]);

        EXPANSION_PORT_SET_INP_DIS(EXPANSION_PORT_spi_ss1_INP_DIS,
                                     EXPANSION_PORT_spi_ss1_MASK,
                                     (0u != (pinsInBuf & EXPANSION_PORT_SS1_PIN_MASK)));
    #endif /* (EXPANSION_PORT_SS1_PIN) */

    #if(EXPANSION_PORT_SS2_PIN)
        EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_SS2_HSIOM_REG, EXPANSION_PORT_SS2_HSIOM_MASK,
                                       EXPANSION_PORT_SS2_HSIOM_POS, hsiomSel[EXPANSION_PORT_SS2_PIN_INDEX]);

        EXPANSION_PORT_spi_ss2_SetDriveMode((uint8) pinsDm[EXPANSION_PORT_SS2_PIN_INDEX]);

        EXPANSION_PORT_SET_INP_DIS(EXPANSION_PORT_spi_ss2_INP_DIS,
                                     EXPANSION_PORT_spi_ss2_MASK,
                                     (0u != (pinsInBuf & EXPANSION_PORT_SS2_PIN_MASK)));
    #endif /* (EXPANSION_PORT_SS2_PIN) */

    #if(EXPANSION_PORT_SS3_PIN)
        EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_SS3_HSIOM_REG,  EXPANSION_PORT_SS3_HSIOM_MASK,
                                       EXPANSION_PORT_SS3_HSIOM_POS, hsiomSel[EXPANSION_PORT_SS3_PIN_INDEX]);

        EXPANSION_PORT_spi_ss3_SetDriveMode((uint8) pinsDm[EXPANSION_PORT_SS3_PIN_INDEX]);

        EXPANSION_PORT_SET_INP_DIS(EXPANSION_PORT_spi_ss3_INP_DIS,
                                     EXPANSION_PORT_spi_ss3_MASK,
                                     (0u != (pinsInBuf & EXPANSION_PORT_SS3_PIN_MASK)));
    #endif /* (EXPANSION_PORT_SS3_PIN) */
    }

#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: EXPANSION_PORT_I2CSlaveNackGeneration
    ********************************************************************************
    *
    * Summary:
    *  Sets command to generate NACK to the address or data.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void EXPANSION_PORT_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (EXPANSION_PORT_CTRL_REG & EXPANSION_PORT_CTRL_EC_AM_MODE)) &&
            (0u == (EXPANSION_PORT_I2C_CTRL_REG & EXPANSION_PORT_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            EXPANSION_PORT_CTRL_REG &= ~EXPANSION_PORT_CTRL_EC_AM_MODE;
            EXPANSION_PORT_CTRL_REG |=  EXPANSION_PORT_CTRL_EC_AM_MODE;
        }

        EXPANSION_PORT_I2C_SLAVE_CMD_REG = EXPANSION_PORT_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */


/* [] END OF FILE */
