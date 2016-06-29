/*******************************************************************************
* File Name: EXPANSION_PORT_PINS.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_EXPANSION_PORT_H)
#define CY_SCB_PINS_EXPANSION_PORT_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define EXPANSION_PORT_REMOVE_MOSI_SCL_RX_WAKE_PIN    (1u)
#define EXPANSION_PORT_REMOVE_MOSI_SCL_RX_PIN         (0u)
#define EXPANSION_PORT_REMOVE_MISO_SDA_TX_PIN         (0u)
#define EXPANSION_PORT_REMOVE_SCLK_PIN                (1u)
#define EXPANSION_PORT_REMOVE_SS0_PIN                 (1u)
#define EXPANSION_PORT_REMOVE_SS1_PIN                 (1u)
#define EXPANSION_PORT_REMOVE_SS2_PIN                 (1u)
#define EXPANSION_PORT_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define EXPANSION_PORT_REMOVE_I2C_PINS                (1u)
#define EXPANSION_PORT_REMOVE_SPI_MASTER_PINS         (1u)
#define EXPANSION_PORT_REMOVE_SPI_SLAVE_PINS          (1u)
#define EXPANSION_PORT_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define EXPANSION_PORT_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define EXPANSION_PORT_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define EXPANSION_PORT_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define EXPANSION_PORT_REMOVE_UART_TX_PIN             (1u)
#define EXPANSION_PORT_REMOVE_UART_RX_TX_PIN          (1u)
#define EXPANSION_PORT_REMOVE_UART_RX_PIN             (1u)
#define EXPANSION_PORT_REMOVE_UART_RX_WAKE_PIN        (1u)
#define EXPANSION_PORT_REMOVE_UART_RTS_PIN            (1u)
#define EXPANSION_PORT_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN   (0u == EXPANSION_PORT_REMOVE_MOSI_SCL_RX_WAKE_PIN)
#define EXPANSION_PORT_MOSI_SCL_RX_PIN        (0u == EXPANSION_PORT_REMOVE_MOSI_SCL_RX_PIN)
#define EXPANSION_PORT_MISO_SDA_TX_PIN        (0u == EXPANSION_PORT_REMOVE_MISO_SDA_TX_PIN)
#define EXPANSION_PORT_SCLK_PIN               (0u == EXPANSION_PORT_REMOVE_SCLK_PIN)
#define EXPANSION_PORT_SS0_PIN                (0u == EXPANSION_PORT_REMOVE_SS0_PIN)
#define EXPANSION_PORT_SS1_PIN                (0u == EXPANSION_PORT_REMOVE_SS1_PIN)
#define EXPANSION_PORT_SS2_PIN                (0u == EXPANSION_PORT_REMOVE_SS2_PIN)
#define EXPANSION_PORT_SS3_PIN                (0u == EXPANSION_PORT_REMOVE_SS3_PIN)

/* Mode defined pins */
#define EXPANSION_PORT_I2C_PINS               (0u == EXPANSION_PORT_REMOVE_I2C_PINS)
#define EXPANSION_PORT_SPI_MASTER_PINS        (0u == EXPANSION_PORT_REMOVE_SPI_MASTER_PINS)
#define EXPANSION_PORT_SPI_SLAVE_PINS         (0u == EXPANSION_PORT_REMOVE_SPI_SLAVE_PINS)
#define EXPANSION_PORT_SPI_MASTER_SS0_PIN     (0u == EXPANSION_PORT_REMOVE_SPI_MASTER_SS0_PIN)
#define EXPANSION_PORT_SPI_MASTER_SS1_PIN     (0u == EXPANSION_PORT_REMOVE_SPI_MASTER_SS1_PIN)
#define EXPANSION_PORT_SPI_MASTER_SS2_PIN     (0u == EXPANSION_PORT_REMOVE_SPI_MASTER_SS2_PIN)
#define EXPANSION_PORT_SPI_MASTER_SS3_PIN     (0u == EXPANSION_PORT_REMOVE_SPI_MASTER_SS3_PIN)
#define EXPANSION_PORT_UART_TX_PIN            (0u == EXPANSION_PORT_REMOVE_UART_TX_PIN)
#define EXPANSION_PORT_UART_RX_TX_PIN         (0u == EXPANSION_PORT_REMOVE_UART_RX_TX_PIN)
#define EXPANSION_PORT_UART_RX_PIN            (0u == EXPANSION_PORT_REMOVE_UART_RX_PIN)
#define EXPANSION_PORT_UART_RX_WAKE_PIN       (0u == EXPANSION_PORT_REMOVE_UART_RX_WAKE_PIN)
#define EXPANSION_PORT_UART_RTS_PIN           (0u == EXPANSION_PORT_REMOVE_UART_RTS_PIN)
#define EXPANSION_PORT_UART_CTS_PIN           (0u == EXPANSION_PORT_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if(EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN)
    #include "EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake.h"
#endif /* (EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN) */

#if(EXPANSION_PORT_MOSI_SCL_RX_PIN)
    #include "EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx.h"
#endif /* (EXPANSION_PORT_MOSI_SCL_RX_PIN) */

#if(EXPANSION_PORT_MISO_SDA_TX_PIN)
    #include "EXPANSION_PORT_spi_miso_i2c_sda_uart_tx.h"
#endif /* (EXPANSION_PORT_MISO_SDA_TX_PIN_PIN) */

#if(EXPANSION_PORT_SCLK_PIN)
    #include "EXPANSION_PORT_spi_sclk.h"
#endif /* (EXPANSION_PORT_SCLK_PIN) */

#if(EXPANSION_PORT_SS0_PIN)
    #include "EXPANSION_PORT_spi_ss0.h"
#endif /* (EXPANSION_PORT_SS1_PIN) */

#if(EXPANSION_PORT_SS1_PIN)
    #include "EXPANSION_PORT_spi_ss1.h"
#endif /* (EXPANSION_PORT_SS1_PIN) */

#if(EXPANSION_PORT_SS2_PIN)
    #include "EXPANSION_PORT_spi_ss2.h"
#endif /* (EXPANSION_PORT_SS2_PIN) */

#if(EXPANSION_PORT_SS3_PIN)
    #include "EXPANSION_PORT_spi_ss3.h"
#endif /* (EXPANSION_PORT_SS3_PIN) */

#if(EXPANSION_PORT_I2C_PINS)
    #include "EXPANSION_PORT_scl.h"
    #include "EXPANSION_PORT_sda.h"
#endif /* (EXPANSION_PORT_I2C_PINS) */

#if(EXPANSION_PORT_SPI_MASTER_PINS)
    #include "EXPANSION_PORT_sclk_m.h"
    #include "EXPANSION_PORT_mosi_m.h"
    #include "EXPANSION_PORT_miso_m.h"
#endif /* (EXPANSION_PORT_SPI_MASTER_PINS) */

#if(EXPANSION_PORT_SPI_SLAVE_PINS)
    #include "EXPANSION_PORT_sclk_s.h"
    #include "EXPANSION_PORT_mosi_s.h"
    #include "EXPANSION_PORT_miso_s.h"
    #include "EXPANSION_PORT_ss_s.h"
#endif /* (EXPANSION_PORT_SPI_SLAVE_PINS) */

#if(EXPANSION_PORT_SPI_MASTER_SS0_PIN)
    #include "EXPANSION_PORT_ss0_m.h"
#endif /* (EXPANSION_PORT_SPI_MASTER_SS0_PIN) */

#if(EXPANSION_PORT_SPI_MASTER_SS1_PIN)
    #include "EXPANSION_PORT_ss1_m.h"
#endif /* (EXPANSION_PORT_SPI_MASTER_SS1_PIN) */

#if(EXPANSION_PORT_SPI_MASTER_SS2_PIN)
    #include "EXPANSION_PORT_ss2_m.h"
#endif /* (EXPANSION_PORT_SPI_MASTER_SS2_PIN) */

#if(EXPANSION_PORT_SPI_MASTER_SS3_PIN)
    #include "EXPANSION_PORT_ss3_m.h"
#endif /* (EXPANSION_PORT_SPI_MASTER_SS3_PIN) */

#if(EXPANSION_PORT_UART_TX_PIN)
    #include "EXPANSION_PORT_tx.h"
#endif /* (EXPANSION_PORT_UART_TX_PIN) */

#if(EXPANSION_PORT_UART_RX_TX_PIN)
    #include "EXPANSION_PORT_rx_tx.h"
#endif /* (EXPANSION_PORT_UART_RX_TX_PIN) */

#if(EXPANSION_PORT_UART_RX_PIN)
    #include "EXPANSION_PORT_rx.h"
#endif /* (EXPANSION_PORT_UART_RX_PIN) */

#if(EXPANSION_PORT_UART_RX_WAKE_PIN)
    #include "EXPANSION_PORT_rx_wake.h"
#endif /* (EXPANSION_PORT_UART_RX_WAKE_PIN) */

#if(EXPANSION_PORT_UART_RTS_PIN)
    #include "EXPANSION_PORT_rts.h"
#endif /* (EXPANSION_PORT_UART_RTS_PIN) */

#if(EXPANSION_PORT_UART_CTS_PIN)
    #include "EXPANSION_PORT_cts.h"
#endif /* (EXPANSION_PORT_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if(EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN)
    #define EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_REG  \
                                                (*(reg32 *) EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_PTR  \
                                                ( (reg32 *) EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_MASK \
                                                (EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_MASK)
    #define EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_POS  \
                                                (EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_SHIFT)

    #define EXPANSION_PORT_MOSI_SCL_RX_WAKE_INTCFG_REG    (*(reg32 *) \
                                                              EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)
    #define EXPANSION_PORT_MOSI_SCL_RX_WAKE_INTCFG_PTR    ( (reg32 *) \
                                                              EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)

    #define EXPANSION_PORT_INTCFG_TYPE_MASK                  (0x03u)
    #define EXPANSION_PORT_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS  (EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake__SHIFT)
    #define EXPANSION_PORT_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK ((uint32)                                           \
                                                                    ((uint32) EXPANSION_PORT_INTCFG_TYPE_MASK << \
                                                                    EXPANSION_PORT_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS))
#endif /* (EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN) */

#if(EXPANSION_PORT_MOSI_SCL_RX_PIN)
    #define EXPANSION_PORT_MOSI_SCL_RX_HSIOM_REG      (*(reg32 *) EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define EXPANSION_PORT_MOSI_SCL_RX_HSIOM_PTR      ( (reg32 *) EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define EXPANSION_PORT_MOSI_SCL_RX_HSIOM_MASK     (EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx__0__HSIOM_MASK)
    #define EXPANSION_PORT_MOSI_SCL_RX_HSIOM_POS      (EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx__0__HSIOM_SHIFT)
#endif /* (EXPANSION_PORT_MOSI_SCL_RX_PIN) */

#if(EXPANSION_PORT_MISO_SDA_TX_PIN)
    #define EXPANSION_PORT_MISO_SDA_TX_HSIOM_REG      (*(reg32 *) EXPANSION_PORT_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define EXPANSION_PORT_MISO_SDA_TX_HSIOM_PTR      ( (reg32 *) EXPANSION_PORT_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define EXPANSION_PORT_MISO_SDA_TX_HSIOM_MASK     (EXPANSION_PORT_spi_miso_i2c_sda_uart_tx__0__HSIOM_MASK)
    #define EXPANSION_PORT_MISO_SDA_TX_HSIOM_POS      (EXPANSION_PORT_spi_miso_i2c_sda_uart_tx__0__HSIOM_SHIFT)
#endif /* (EXPANSION_PORT_MISO_SDA_TX_PIN_PIN) */

#if(EXPANSION_PORT_SCLK_PIN)
    #define EXPANSION_PORT_SCLK_HSIOM_REG     (*(reg32 *) EXPANSION_PORT_spi_sclk__0__HSIOM)
    #define EXPANSION_PORT_SCLK_HSIOM_PTR     ( (reg32 *) EXPANSION_PORT_spi_sclk__0__HSIOM)
    #define EXPANSION_PORT_SCLK_HSIOM_MASK    (EXPANSION_PORT_spi_sclk__0__HSIOM_MASK)
    #define EXPANSION_PORT_SCLK_HSIOM_POS     (EXPANSION_PORT_spi_sclk__0__HSIOM_SHIFT)
#endif /* (EXPANSION_PORT_SCLK_PIN) */

#if(EXPANSION_PORT_SS0_PIN)
    #define EXPANSION_PORT_SS0_HSIOM_REG      (*(reg32 *) EXPANSION_PORT_spi_ss0__0__HSIOM)
    #define EXPANSION_PORT_SS0_HSIOM_PTR      ( (reg32 *) EXPANSION_PORT_spi_ss0__0__HSIOM)
    #define EXPANSION_PORT_SS0_HSIOM_MASK     (EXPANSION_PORT_spi_ss0__0__HSIOM_MASK)
    #define EXPANSION_PORT_SS0_HSIOM_POS      (EXPANSION_PORT_spi_ss0__0__HSIOM_SHIFT)
#endif /* (EXPANSION_PORT_SS1_PIN) */

#if(EXPANSION_PORT_SS1_PIN)
    #define EXPANSION_PORT_SS1_HSIOM_REG      (*(reg32 *) EXPANSION_PORT_spi_ss1__0__HSIOM)
    #define EXPANSION_PORT_SS1_HSIOM_PTR      ( (reg32 *) EXPANSION_PORT_spi_ss1__0__HSIOM)
    #define EXPANSION_PORT_SS1_HSIOM_MASK     (EXPANSION_PORT_spi_ss1__0__HSIOM_MASK)
    #define EXPANSION_PORT_SS1_HSIOM_POS      (EXPANSION_PORT_spi_ss1__0__HSIOM_SHIFT)
#endif /* (EXPANSION_PORT_SS1_PIN) */

#if(EXPANSION_PORT_SS2_PIN)
    #define EXPANSION_PORT_SS2_HSIOM_REG     (*(reg32 *) EXPANSION_PORT_spi_ss2__0__HSIOM)
    #define EXPANSION_PORT_SS2_HSIOM_PTR     ( (reg32 *) EXPANSION_PORT_spi_ss2__0__HSIOM)
    #define EXPANSION_PORT_SS2_HSIOM_MASK    (EXPANSION_PORT_spi_ss2__0__HSIOM_MASK)
    #define EXPANSION_PORT_SS2_HSIOM_POS     (EXPANSION_PORT_spi_ss2__0__HSIOM_SHIFT)
#endif /* (EXPANSION_PORT_SS2_PIN) */

#if(EXPANSION_PORT_SS3_PIN)
    #define EXPANSION_PORT_SS3_HSIOM_REG     (*(reg32 *) EXPANSION_PORT_spi_ss3__0__HSIOM)
    #define EXPANSION_PORT_SS3_HSIOM_PTR     ( (reg32 *) EXPANSION_PORT_spi_ss3__0__HSIOM)
    #define EXPANSION_PORT_SS3_HSIOM_MASK    (EXPANSION_PORT_spi_ss3__0__HSIOM_MASK)
    #define EXPANSION_PORT_SS3_HSIOM_POS     (EXPANSION_PORT_spi_ss3__0__HSIOM_SHIFT)
#endif /* (EXPANSION_PORT_SS3_PIN) */

#if(EXPANSION_PORT_I2C_PINS)
    #define EXPANSION_PORT_SCL_HSIOM_REG     (*(reg32 *) EXPANSION_PORT_scl__0__HSIOM)
    #define EXPANSION_PORT_SCL_HSIOM_PTR     ( (reg32 *) EXPANSION_PORT_scl__0__HSIOM)
    #define EXPANSION_PORT_SCL_HSIOM_MASK    (EXPANSION_PORT_scl__0__HSIOM_MASK)
    #define EXPANSION_PORT_SCL_HSIOM_POS     (EXPANSION_PORT_scl__0__HSIOM_SHIFT)

    #define EXPANSION_PORT_SDA_HSIOM_REG     (*(reg32 *) EXPANSION_PORT_sda__0__HSIOM)
    #define EXPANSION_PORT_SDA_HSIOM_PTR     ( (reg32 *) EXPANSION_PORT_sda__0__HSIOM)
    #define EXPANSION_PORT_SDA_HSIOM_MASK    (EXPANSION_PORT_sda__0__HSIOM_MASK)
    #define EXPANSION_PORT_SDA_HSIOM_POS     (EXPANSION_PORT_sda__0__HSIOM_SHIFT)
#endif /* (EXPANSION_PORT_I2C_PINS) */


/***************************************
*        Registers Constants
***************************************/

/* Pins constants */
#define EXPANSION_PORT_HSIOM_DEF_SEL      (0x00u)
#define EXPANSION_PORT_HSIOM_GPIO_SEL     (0x00u)
#define EXPANSION_PORT_HSIOM_UART_SEL     (0x09u)
#define EXPANSION_PORT_HSIOM_I2C_SEL      (0x0Eu)
#define EXPANSION_PORT_HSIOM_SPI_SEL      (0x0Fu)

#define EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX      (0u) /* RX pins without interrupt */
#define EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN_INDEX (0u) /* RX pin with interrupt     */
#define EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX      (1u)
#define EXPANSION_PORT_SCLK_PIN_INDEX             (2u)
#define EXPANSION_PORT_SS0_PIN_INDEX              (3u)
#define EXPANSION_PORT_SS1_PIN_INDEX              (4u)
#define EXPANSION_PORT_SS2_PIN_INDEX              (5u)
#define EXPANSION_PORT_SS3_PIN_INDEX              (6u)

#define EXPANSION_PORT_MOSI_SCL_RX_PIN_MASK      ((uint32) 0x01u << EXPANSION_PORT_MOSI_SCL_RX_PIN_INDEX)
#define EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN_MASK ((uint32) 0x01u << EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN_INDEX)
#define EXPANSION_PORT_MISO_SDA_TX_PIN_MASK      ((uint32) 0x01u << EXPANSION_PORT_MISO_SDA_TX_PIN_INDEX)
#define EXPANSION_PORT_SCLK_PIN_MASK             ((uint32) 0x01u << EXPANSION_PORT_SCLK_PIN_INDEX)
#define EXPANSION_PORT_SS0_PIN_MASK              ((uint32) 0x01u << EXPANSION_PORT_SS0_PIN_INDEX)
#define EXPANSION_PORT_SS1_PIN_MASK              ((uint32) 0x01u << EXPANSION_PORT_SS1_PIN_INDEX)
#define EXPANSION_PORT_SS2_PIN_MASK              ((uint32) 0x01u << EXPANSION_PORT_SS2_PIN_INDEX)
#define EXPANSION_PORT_SS3_PIN_MASK              ((uint32) 0x01u << EXPANSION_PORT_SS3_PIN_INDEX)

#define EXPANSION_PORT_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin DM defines */
#define EXPANSION_PORT_PIN_DM_ALG_HIZ  (0u)
#define EXPANSION_PORT_PIN_DM_DIG_HIZ  (1u)
#define EXPANSION_PORT_PIN_DM_OD_LO    (4u)
#define EXPANSION_PORT_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Set bits-mask in register */
#define EXPANSION_PORT_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define EXPANSION_PORT_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define EXPANSION_PORT_SET_HSIOM_SEL(reg, mask, pos, sel) EXPANSION_PORT_SET_REGISTER_BITS(reg, mask, pos, sel)
#define EXPANSION_PORT_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        EXPANSION_PORT_SET_REGISTER_BITS(reg, mask, pos, intType)
#define EXPANSION_PORT_SET_INP_DIS(reg, mask, val) EXPANSION_PORT_SET_REGISTER_BIT(reg, mask, val)

/* EXPANSION_PORT_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  EXPANSION_PORT_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if(EXPANSION_PORT_I2C_PINS)
    #define EXPANSION_PORT_SET_I2C_SCL_DR(val) EXPANSION_PORT_scl_Write(val)

    #define EXPANSION_PORT_SET_I2C_SCL_HSIOM_SEL(sel) \
                          EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_SCL_HSIOM_REG,  \
                                                         EXPANSION_PORT_SCL_HSIOM_MASK, \
                                                         EXPANSION_PORT_SCL_HSIOM_POS,  \
                                                         (sel))
    #define EXPANSION_PORT_WAIT_SCL_SET_HIGH  (0u == EXPANSION_PORT_scl_Read())

/* Unconfigured SCB: scl signal */
#elif(EXPANSION_PORT_MOSI_SCL_RX_WAKE_PIN)
    #define EXPANSION_PORT_SET_I2C_SCL_DR(val) \
                            EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake_Write(val)

    #define EXPANSION_PORT_SET_I2C_SCL_HSIOM_SEL(sel) \
                    EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_REG,  \
                                                   EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_MASK, \
                                                   EXPANSION_PORT_MOSI_SCL_RX_WAKE_HSIOM_POS,  \
                                                   (sel))

    #define EXPANSION_PORT_WAIT_SCL_SET_HIGH  (0u == EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_wake_Read())

#elif(EXPANSION_PORT_MOSI_SCL_RX_PIN)
    #define EXPANSION_PORT_SET_I2C_SCL_DR(val) \
                            EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_Write(val)


    #define EXPANSION_PORT_SET_I2C_SCL_HSIOM_SEL(sel) \
                            EXPANSION_PORT_SET_HSIOM_SEL(EXPANSION_PORT_MOSI_SCL_RX_HSIOM_REG,  \
                                                           EXPANSION_PORT_MOSI_SCL_RX_HSIOM_MASK, \
                                                           EXPANSION_PORT_MOSI_SCL_RX_HSIOM_POS,  \
                                                           (sel))

    #define EXPANSION_PORT_WAIT_SCL_SET_HIGH  (0u == EXPANSION_PORT_spi_mosi_i2c_scl_uart_rx_Read())

#else
    #define EXPANSION_PORT_SET_I2C_SCL_DR(val) \
                                                    do{ /* Does nothing */ }while(0)
    #define EXPANSION_PORT_SET_I2C_SCL_HSIOM_SEL(sel) \
                                                    do{ /* Does nothing */ }while(0)

    #define EXPANSION_PORT_WAIT_SCL_SET_HIGH  (0u)
#endif /* (EXPANSION_PORT_MOSI_SCL_RX_PIN) */

/* SCB I2C: sda signal */
#if(EXPANSION_PORT_I2C_PINS)
    #define EXPANSION_PORT_WAIT_SDA_SET_HIGH  (0u == EXPANSION_PORT_sda_Read())

/* Unconfigured SCB: sda signal */
#elif(EXPANSION_PORT_MISO_SDA_TX_PIN)
    #define EXPANSION_PORT_WAIT_SDA_SET_HIGH  (0u == EXPANSION_PORT_spi_miso_i2c_sda_uart_tx_Read())

#else
    #define EXPANSION_PORT_WAIT_SDA_SET_HIGH  (0u)
#endif /* (EXPANSION_PORT_MOSI_SCL_RX_PIN) */

#endif /* (CY_SCB_PINS_EXPANSION_PORT_H) */


/* [] END OF FILE */
