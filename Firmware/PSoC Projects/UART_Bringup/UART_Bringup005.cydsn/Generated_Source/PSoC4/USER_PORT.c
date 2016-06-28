/*******************************************************************************
* File Name: USER_PORT.c
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

#include "USER_PORT_PVT.h"

#if(USER_PORT_SCB_MODE_I2C_INC)
    #include "USER_PORT_I2C_PVT.h"
#endif /* (USER_PORT_SCB_MODE_I2C_INC) */

#if(USER_PORT_SCB_MODE_EZI2C_INC)
    #include "USER_PORT_EZI2C_PVT.h"
#endif /* (USER_PORT_SCB_MODE_EZI2C_INC) */

#if(USER_PORT_SCB_MODE_SPI_INC || USER_PORT_SCB_MODE_UART_INC)
    #include "USER_PORT_SPI_UART_PVT.h"
#endif /* (USER_PORT_SCB_MODE_SPI_INC || USER_PORT_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 USER_PORT_scbMode = USER_PORT_SCB_MODE_UNCONFIG;
    uint8 USER_PORT_scbEnableWake;
    uint8 USER_PORT_scbEnableIntr;

    /* I2C configuration variables */
    uint8 USER_PORT_mode;
    uint8 USER_PORT_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * USER_PORT_rxBuffer;
    uint8  USER_PORT_rxDataBits;
    uint32 USER_PORT_rxBufferSize;

    volatile uint8 * USER_PORT_txBuffer;
    uint8  USER_PORT_txDataBits;
    uint32 USER_PORT_txBufferSize;

    /* EZI2C configuration variables */
    uint8 USER_PORT_numberOfAddr;
    uint8 USER_PORT_subAddrSize;
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/

uint8 USER_PORT_initVar = 0u;

#if !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER)
    cyisraddress USER_PORT_customIntrHandler = NULL;
#endif /* !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER) */


/***************************************
*    Private Function Prototypes
***************************************/

static void USER_PORT_ScbEnableIntr(void);
static void USER_PORT_ScbModeStop(void);


/*******************************************************************************
* Function Name: USER_PORT_Init
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
void USER_PORT_Init(void)
{
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    if(USER_PORT_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        USER_PORT_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif(USER_PORT_SCB_MODE_I2C_CONST_CFG)
    USER_PORT_I2CInit();

#elif(USER_PORT_SCB_MODE_SPI_CONST_CFG)
    USER_PORT_SpiInit();

#elif(USER_PORT_SCB_MODE_UART_CONST_CFG)
    USER_PORT_UartInit();

#elif(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)
    USER_PORT_EzI2CInit();

#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: USER_PORT_Enable
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
void USER_PORT_Enable(void)
{
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if(!USER_PORT_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        USER_PORT_CTRL_REG |= USER_PORT_CTRL_ENABLED;

        USER_PORT_ScbEnableIntr();
    }
#else
    USER_PORT_CTRL_REG |= USER_PORT_CTRL_ENABLED;

    USER_PORT_ScbEnableIntr();
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: USER_PORT_Start
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
*  USER_PORT_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void USER_PORT_Start(void)
{
    if(0u == USER_PORT_initVar)
    {
        USER_PORT_Init();
        USER_PORT_initVar = 1u; /* Component was initialized */
    }

    USER_PORT_Enable();
}


/*******************************************************************************
* Function Name: USER_PORT_Stop
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
void USER_PORT_Stop(void)
{
#if(USER_PORT_SCB_IRQ_INTERNAL)
    USER_PORT_DisableInt();
#endif /* (USER_PORT_SCB_IRQ_INTERNAL) */

    USER_PORT_CTRL_REG &= (uint32) ~USER_PORT_CTRL_ENABLED;  /* Disable scb IP */

#if(USER_PORT_SCB_IRQ_INTERNAL)
    USER_PORT_ClearPendingInt();
#endif /* (USER_PORT_SCB_IRQ_INTERNAL) */

    USER_PORT_ScbModeStop(); /* Calls scbMode specific Stop function */
}


/*******************************************************************************
* Function Name: USER_PORT_SetRxFifoLevel
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
void USER_PORT_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = USER_PORT_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~USER_PORT_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (USER_PORT_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    USER_PORT_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: USER_PORT_SetTxFifoLevel
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
void USER_PORT_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = USER_PORT_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~USER_PORT_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (USER_PORT_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    USER_PORT_TX_FIFO_CTRL_REG = txFifoCtrl;
}


/*******************************************************************************
* Function Name: USER_PORT_SetCustomInterruptHandler
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
void USER_PORT_SetCustomInterruptHandler(cyisraddress func)
{
#if !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER)
    USER_PORT_customIntrHandler = func; /* Register interrupt handler */
#else
    if(NULL != func)
    {
        /* Suppress compiler warning */
    }
#endif /* !defined (CY_REMOVE_USER_PORT_CUSTOM_INTR_HANDLER) */
}


/*******************************************************************************
* Function Name: USER_PORT_ScbModeEnableIntr
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
static void USER_PORT_ScbEnableIntr(void)
{
#if(USER_PORT_SCB_IRQ_INTERNAL)
    #if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Enable interrupt in NVIC */
        if(0u != USER_PORT_scbEnableIntr)
        {
            USER_PORT_EnableInt();
        }
    #else
        USER_PORT_EnableInt();

    #endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (USER_PORT_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: USER_PORT_ScbModeStop
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
static void USER_PORT_ScbModeStop(void)
{
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    if(USER_PORT_SCB_MODE_I2C_RUNTM_CFG)
    {
        USER_PORT_I2CStop();
    }
    else if (USER_PORT_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        USER_PORT_EzI2CStop();
    }
#if (!USER_PORT_CY_SCBIP_V1)
    else if (USER_PORT_SCB_MODE_UART_RUNTM_CFG)
    {
        USER_PORT_UartStop();
    }
#endif /* (!USER_PORT_CY_SCBIP_V1) */
    else
    {
        /* Do nothing for other modes */
    }
#elif(USER_PORT_SCB_MODE_I2C_CONST_CFG)
    USER_PORT_I2CStop();

#elif(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)
    USER_PORT_EzI2CStop();

#elif(USER_PORT_SCB_MODE_UART_CONST_CFG)
    USER_PORT_UartStop();

#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: USER_PORT_SetPins
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
    void USER_PORT_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 hsiomSel [USER_PORT_SCB_PINS_NUMBER];
        uint32 pinsDm   [USER_PORT_SCB_PINS_NUMBER];

    #if (!USER_PORT_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!USER_PORT_CY_SCBIP_V1) */

        uint32 i;

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for(i = 0u; i < USER_PORT_SCB_PINS_NUMBER; i++)
        {
            hsiomSel[i]  = USER_PORT_HSIOM_DEF_SEL;
            pinsDm[i]    = USER_PORT_PIN_DM_ALG_HIZ;
        }

        if((USER_PORT_SCB_MODE_I2C   == mode) ||
           (USER_PORT_SCB_MODE_EZI2C == mode))
        {
            hsiomSel[USER_PORT_MOSI_SCL_RX_PIN_INDEX] = USER_PORT_HSIOM_I2C_SEL;
            hsiomSel[USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_HSIOM_I2C_SEL;

            pinsDm[USER_PORT_MOSI_SCL_RX_PIN_INDEX] = USER_PORT_PIN_DM_OD_LO;
            pinsDm[USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_PIN_DM_OD_LO;
        }
    #if (!USER_PORT_CY_SCBIP_V1)
        else if(USER_PORT_SCB_MODE_SPI == mode)
        {
            hsiomSel[USER_PORT_MOSI_SCL_RX_PIN_INDEX] = USER_PORT_HSIOM_SPI_SEL;
            hsiomSel[USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_HSIOM_SPI_SEL;
            hsiomSel[USER_PORT_SCLK_PIN_INDEX]        = USER_PORT_HSIOM_SPI_SEL;

            if(USER_PORT_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[USER_PORT_MOSI_SCL_RX_PIN_INDEX] = USER_PORT_PIN_DM_DIG_HIZ;
                pinsDm[USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_PIN_DM_STRONG;
                pinsDm[USER_PORT_SCLK_PIN_INDEX]        = USER_PORT_PIN_DM_DIG_HIZ;

            #if(USER_PORT_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[USER_PORT_SS0_PIN_INDEX] = USER_PORT_HSIOM_SPI_SEL;
                pinsDm  [USER_PORT_SS0_PIN_INDEX] = USER_PORT_PIN_DM_DIG_HIZ;
            #endif /* (USER_PORT_SS1_PIN) */

            #if(USER_PORT_MISO_SDA_TX_PIN)
                /* Disable input buffer */
                 pinsInBuf |= USER_PORT_MISO_SDA_TX_PIN_MASK;
            #endif /* (USER_PORT_MISO_SDA_TX_PIN_PIN) */
            }
            else /* (Master) */
            {
                pinsDm[USER_PORT_MOSI_SCL_RX_PIN_INDEX] = USER_PORT_PIN_DM_STRONG;
                pinsDm[USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_PIN_DM_DIG_HIZ;
                pinsDm[USER_PORT_SCLK_PIN_INDEX]        = USER_PORT_PIN_DM_STRONG;

            #if(USER_PORT_SS0_PIN)
                hsiomSel [USER_PORT_SS0_PIN_INDEX] = USER_PORT_HSIOM_SPI_SEL;
                pinsDm   [USER_PORT_SS0_PIN_INDEX] = USER_PORT_PIN_DM_STRONG;
                pinsInBuf                                |= USER_PORT_SS0_PIN_MASK;
            #endif /* (USER_PORT_SS0_PIN) */

            #if(USER_PORT_SS1_PIN)
                hsiomSel [USER_PORT_SS1_PIN_INDEX] = USER_PORT_HSIOM_SPI_SEL;
                pinsDm   [USER_PORT_SS1_PIN_INDEX] = USER_PORT_PIN_DM_STRONG;
                pinsInBuf                                |= USER_PORT_SS1_PIN_MASK;
            #endif /* (USER_PORT_SS1_PIN) */

            #if(USER_PORT_SS2_PIN)
                hsiomSel [USER_PORT_SS2_PIN_INDEX] = USER_PORT_HSIOM_SPI_SEL;
                pinsDm   [USER_PORT_SS2_PIN_INDEX] = USER_PORT_PIN_DM_STRONG;
                pinsInBuf                                |= USER_PORT_SS2_PIN_MASK;
            #endif /* (USER_PORT_SS2_PIN) */

            #if(USER_PORT_SS3_PIN)
                hsiomSel [USER_PORT_SS3_PIN_INDEX] = USER_PORT_HSIOM_SPI_SEL;
                pinsDm   [USER_PORT_SS3_PIN_INDEX] = USER_PORT_PIN_DM_STRONG;
                pinsInBuf                                |= USER_PORT_SS3_PIN_MASK;
            #endif /* (USER_PORT_SS2_PIN) */

                /* Disable input buffers */
            #if(USER_PORT_MOSI_SCL_RX_PIN)
                pinsInBuf |= USER_PORT_MOSI_SCL_RX_PIN_MASK;
            #endif /* (USER_PORT_MOSI_SCL_RX_PIN) */

             #if(USER_PORT_MOSI_SCL_RX_WAKE_PIN)
                pinsInBuf |= USER_PORT_MOSI_SCL_RX_WAKE_PIN_MASK;
            #endif /* (USER_PORT_MOSI_SCL_RX_WAKE_PIN) */

            #if(USER_PORT_SCLK_PIN)
                pinsInBuf |= USER_PORT_SCLK_PIN_MASK;
            #endif /* (USER_PORT_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if(USER_PORT_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
                hsiomSel[USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_HSIOM_UART_SEL;
                pinsDm  [USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_PIN_DM_OD_LO;
            }
            else /* Standard or IrDA */
            {
                if(0u != (USER_PORT_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                    hsiomSel[USER_PORT_MOSI_SCL_RX_PIN_INDEX] = USER_PORT_HSIOM_UART_SEL;
                    pinsDm  [USER_PORT_MOSI_SCL_RX_PIN_INDEX] = USER_PORT_PIN_DM_DIG_HIZ;
                }

                if(0u != (USER_PORT_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                    hsiomSel[USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_HSIOM_UART_SEL;
                    pinsDm  [USER_PORT_MISO_SDA_TX_PIN_INDEX] = USER_PORT_PIN_DM_STRONG;

                #if(USER_PORT_MISO_SDA_TX_PIN)
                     pinsInBuf |= USER_PORT_MISO_SDA_TX_PIN_MASK;
                #endif /* (USER_PORT_MISO_SDA_TX_PIN_PIN) */
                }

            #if !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
                if(USER_PORT_UART_MODE_STD == subMode)
                {
                    if(0u != (USER_PORT_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                        hsiomSel[USER_PORT_SCLK_PIN_INDEX] = USER_PORT_HSIOM_UART_SEL;
                        pinsDm  [USER_PORT_SCLK_PIN_INDEX] = USER_PORT_PIN_DM_DIG_HIZ;
                    }

                    if(0u != (USER_PORT_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                        hsiomSel[USER_PORT_SS0_PIN_INDEX] = USER_PORT_HSIOM_UART_SEL;
                        pinsDm  [USER_PORT_SS0_PIN_INDEX] = USER_PORT_PIN_DM_STRONG;

                    #if(USER_PORT_SS0_PIN)
                        /* Disable input buffer */
                        pinsInBuf |= USER_PORT_SS0_PIN_MASK;
                    #endif /* (USER_PORT_SS0_PIN) */
                    }
                }
            #endif /* !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */
            }
        }
    #endif /* (!USER_PORT_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if(USER_PORT_MOSI_SCL_RX_PIN)
        USER_PORT_SET_HSIOM_SEL(USER_PORT_MOSI_SCL_RX_HSIOM_REG,
                                       USER_PORT_MOSI_SCL_RX_HSIOM_MASK,
                                       USER_PORT_MOSI_SCL_RX_HSIOM_POS,
                                       hsiomSel[USER_PORT_MOSI_SCL_RX_PIN_INDEX]);

        USER_PORT_spi_mosi_i2c_scl_uart_rx_SetDriveMode((uint8) pinsDm[USER_PORT_MOSI_SCL_RX_PIN_INDEX]);

    #if (!USER_PORT_CY_SCBIP_V1)
        USER_PORT_SET_INP_DIS(USER_PORT_spi_mosi_i2c_scl_uart_rx_INP_DIS,
                                     USER_PORT_spi_mosi_i2c_scl_uart_rx_MASK,
                                     (0u != (pinsInBuf & USER_PORT_MOSI_SCL_RX_PIN_MASK)));
    #endif /* (!USER_PORT_CY_SCBIP_V1) */
    #endif /* (USER_PORT_MOSI_SCL_RX_PIN) */

    #if(USER_PORT_MOSI_SCL_RX_WAKE_PIN)
        USER_PORT_SET_HSIOM_SEL(USER_PORT_MOSI_SCL_RX_WAKE_HSIOM_REG,
                                       USER_PORT_MOSI_SCL_RX_WAKE_HSIOM_MASK,
                                       USER_PORT_MOSI_SCL_RX_WAKE_HSIOM_POS,
                                       hsiomSel[USER_PORT_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        USER_PORT_spi_mosi_i2c_scl_uart_rx_wake_SetDriveMode((uint8)
                                                               pinsDm[USER_PORT_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        USER_PORT_SET_INP_DIS(USER_PORT_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS,
                                     USER_PORT_spi_mosi_i2c_scl_uart_rx_wake_MASK,
                                     (0u != (pinsInBuf & USER_PORT_MOSI_SCL_RX_WAKE_PIN_MASK)));

         /* Set interrupt on falling edge */
        USER_PORT_SET_INCFG_TYPE(USER_PORT_MOSI_SCL_RX_WAKE_INTCFG_REG,
                                        USER_PORT_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK,
                                        USER_PORT_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS,
                                        USER_PORT_INTCFG_TYPE_FALLING_EDGE);
    #endif /* (USER_PORT_MOSI_SCL_RX_WAKE_PIN) */

    #if(USER_PORT_MISO_SDA_TX_PIN)
        USER_PORT_SET_HSIOM_SEL(USER_PORT_MISO_SDA_TX_HSIOM_REG,
                                       USER_PORT_MISO_SDA_TX_HSIOM_MASK,
                                       USER_PORT_MISO_SDA_TX_HSIOM_POS,
                                       hsiomSel[USER_PORT_MISO_SDA_TX_PIN_INDEX]);

        USER_PORT_spi_miso_i2c_sda_uart_tx_SetDriveMode((uint8) pinsDm[USER_PORT_MISO_SDA_TX_PIN_INDEX]);

    #if (!USER_PORT_CY_SCBIP_V1)
        USER_PORT_SET_INP_DIS(USER_PORT_spi_miso_i2c_sda_uart_tx_INP_DIS,
                                     USER_PORT_spi_miso_i2c_sda_uart_tx_MASK,
                                    (0u != (pinsInBuf & USER_PORT_MISO_SDA_TX_PIN_MASK)));
    #endif /* (!USER_PORT_CY_SCBIP_V1) */
    #endif /* (USER_PORT_MOSI_SCL_RX_PIN) */

    #if(USER_PORT_SCLK_PIN)
        USER_PORT_SET_HSIOM_SEL(USER_PORT_SCLK_HSIOM_REG, USER_PORT_SCLK_HSIOM_MASK,
                                       USER_PORT_SCLK_HSIOM_POS, hsiomSel[USER_PORT_SCLK_PIN_INDEX]);

        USER_PORT_spi_sclk_SetDriveMode((uint8) pinsDm[USER_PORT_SCLK_PIN_INDEX]);

        USER_PORT_SET_INP_DIS(USER_PORT_spi_sclk_INP_DIS,
                                     USER_PORT_spi_sclk_MASK,
                                     (0u != (pinsInBuf & USER_PORT_SCLK_PIN_MASK)));
    #endif /* (USER_PORT_SCLK_PIN) */

    #if(USER_PORT_SS0_PIN)
        USER_PORT_SET_HSIOM_SEL(USER_PORT_SS0_HSIOM_REG, USER_PORT_SS0_HSIOM_MASK,
                                       USER_PORT_SS0_HSIOM_POS, hsiomSel[USER_PORT_SS0_PIN_INDEX]);

        USER_PORT_spi_ss0_SetDriveMode((uint8) pinsDm[USER_PORT_SS0_PIN_INDEX]);

        USER_PORT_SET_INP_DIS(USER_PORT_spi_ss0_INP_DIS,
                                     USER_PORT_spi_ss0_MASK,
                                     (0u != (pinsInBuf & USER_PORT_SS0_PIN_MASK)));
    #endif /* (USER_PORT_SS1_PIN) */

    #if(USER_PORT_SS1_PIN)
        USER_PORT_SET_HSIOM_SEL(USER_PORT_SS1_HSIOM_REG, USER_PORT_SS1_HSIOM_MASK,
                                       USER_PORT_SS1_HSIOM_POS, hsiomSel[USER_PORT_SS1_PIN_INDEX]);

        USER_PORT_spi_ss1_SetDriveMode((uint8) pinsDm[USER_PORT_SS1_PIN_INDEX]);

        USER_PORT_SET_INP_DIS(USER_PORT_spi_ss1_INP_DIS,
                                     USER_PORT_spi_ss1_MASK,
                                     (0u != (pinsInBuf & USER_PORT_SS1_PIN_MASK)));
    #endif /* (USER_PORT_SS1_PIN) */

    #if(USER_PORT_SS2_PIN)
        USER_PORT_SET_HSIOM_SEL(USER_PORT_SS2_HSIOM_REG, USER_PORT_SS2_HSIOM_MASK,
                                       USER_PORT_SS2_HSIOM_POS, hsiomSel[USER_PORT_SS2_PIN_INDEX]);

        USER_PORT_spi_ss2_SetDriveMode((uint8) pinsDm[USER_PORT_SS2_PIN_INDEX]);

        USER_PORT_SET_INP_DIS(USER_PORT_spi_ss2_INP_DIS,
                                     USER_PORT_spi_ss2_MASK,
                                     (0u != (pinsInBuf & USER_PORT_SS2_PIN_MASK)));
    #endif /* (USER_PORT_SS2_PIN) */

    #if(USER_PORT_SS3_PIN)
        USER_PORT_SET_HSIOM_SEL(USER_PORT_SS3_HSIOM_REG,  USER_PORT_SS3_HSIOM_MASK,
                                       USER_PORT_SS3_HSIOM_POS, hsiomSel[USER_PORT_SS3_PIN_INDEX]);

        USER_PORT_spi_ss3_SetDriveMode((uint8) pinsDm[USER_PORT_SS3_PIN_INDEX]);

        USER_PORT_SET_INP_DIS(USER_PORT_spi_ss3_INP_DIS,
                                     USER_PORT_spi_ss3_MASK,
                                     (0u != (pinsInBuf & USER_PORT_SS3_PIN_MASK)));
    #endif /* (USER_PORT_SS3_PIN) */
    }

#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: USER_PORT_I2CSlaveNackGeneration
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
    void USER_PORT_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (USER_PORT_CTRL_REG & USER_PORT_CTRL_EC_AM_MODE)) &&
            (0u == (USER_PORT_I2C_CTRL_REG & USER_PORT_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            USER_PORT_CTRL_REG &= ~USER_PORT_CTRL_EC_AM_MODE;
            USER_PORT_CTRL_REG |=  USER_PORT_CTRL_EC_AM_MODE;
        }

        USER_PORT_I2C_SLAVE_CMD_REG = USER_PORT_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */


/* [] END OF FILE */
