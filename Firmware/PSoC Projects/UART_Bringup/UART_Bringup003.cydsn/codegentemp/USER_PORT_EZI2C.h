/*******************************************************************************
* File Name: USER_PORT_EZI2C.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the SCB Component in
*  the EZI2C mode.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_EZI2C_USER_PORT_H)
#define CY_SCB_EZI2C_USER_PORT_H

#include "USER_PORT.h"


/***************************************
*   Initial Parameter Constants
****************************************/

#define USER_PORT_EZI2C_CLOCK_STRETCHING         (1u)
#define USER_PORT_EZI2C_MEDIAN_FILTER_ENABLE     (1u)
#define USER_PORT_EZI2C_NUMBER_OF_ADDRESSES      (0u)
#define USER_PORT_EZI2C_PRIMARY_SLAVE_ADDRESS    (8u)
#define USER_PORT_EZI2C_SECONDARY_SLAVE_ADDRESS  (9u)
#define USER_PORT_EZI2C_SUB_ADDRESS_SIZE         (0u)
#define USER_PORT_EZI2C_WAKE_ENABLE              (0u)
#define USER_PORT_EZI2C_DATA_RATE                (100u)
#define USER_PORT_EZI2C_SLAVE_ADDRESS_MASK       (254u)
#define USER_PORT_EZI2C_BYTE_MODE_ENABLE         (0u)


/***************************************
*  Conditional Compilation Parameters
****************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    #define USER_PORT_SUB_ADDRESS_SIZE16             (0u != USER_PORT_subAddrSize)
    #define USER_PORT_SECONDARY_ADDRESS_ENABLE       (0u != USER_PORT_numberOfAddr)

    #define USER_PORT_EZI2C_EC_AM_ENABLE         (0u != (USER_PORT_CTRL_REG & \
                                                                USER_PORT_CTRL_EC_AM_MODE))
    #define USER_PORT_EZI2C_SCL_STRETCH_ENABLE   (0u != (USER_PORT_GetSlaveInterruptMode() & \
                                                                USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH))
    #define USER_PORT_EZI2C_SCL_STRETCH_DISABLE       (!USER_PORT_EZI2C_SCL_STRETCH_ENABLE)

    #define USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST  (1u)
    #define USER_PORT_SUB_ADDRESS_SIZE16_CONST        (1u)
    #define USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST  (1u)
    #define USER_PORT_EZI2C_SCL_STRETCH_DISABLE_CONST (1u)
    #define USER_PORT_EZI2C_WAKE_ENABLE_CONST         (1u)

    #if (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
        #define USER_PORT_EZI2C_FIFO_SIZE    (USER_PORT_FIFO_SIZE)
    #else
        #define USER_PORT_EZI2C_FIFO_SIZE    (USER_PORT_GET_FIFO_SIZE(USER_PORT_CTRL_REG & \
                                                                                    USER_PORT_CTRL_BYTE_MODE))
    #endif /* (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */

#else

    #define USER_PORT_SUB_ADDRESS_SIZE16         (0u != USER_PORT_EZI2C_SUB_ADDRESS_SIZE)
    #define USER_PORT_SUB_ADDRESS_SIZE16_CONST   (USER_PORT_SUB_ADDRESS_SIZE16)

    #define USER_PORT_SECONDARY_ADDRESS_ENABLE        (0u != USER_PORT_EZI2C_NUMBER_OF_ADDRESSES)
    #define USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST  (USER_PORT_SECONDARY_ADDRESS_ENABLE)

    #define USER_PORT_EZI2C_SCL_STRETCH_ENABLE        (0u != USER_PORT_EZI2C_CLOCK_STRETCHING)
    #define USER_PORT_EZI2C_SCL_STRETCH_DISABLE       (!USER_PORT_EZI2C_SCL_STRETCH_ENABLE)
    #define USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST  (USER_PORT_EZI2C_SCL_STRETCH_ENABLE)
    #define USER_PORT_EZI2C_SCL_STRETCH_DISABLE_CONST (USER_PORT_EZI2C_SCL_STRETCH_DISABLE)

    #define USER_PORT_EZI2C_WAKE_ENABLE_CONST         (0u != USER_PORT_EZI2C_WAKE_ENABLE)
    #define USER_PORT_EZI2C_EC_AM_ENABLE              (0u != USER_PORT_EZI2C_WAKE_ENABLE)

    #if (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
       #define USER_PORT_EZI2C_FIFO_SIZE    (USER_PORT_FIFO_SIZE)

    #else
        #define USER_PORT_EZI2C_FIFO_SIZE \
                                            USER_PORT_GET_FIFO_SIZE(USER_PORT_EZI2C_BYTE_MODE_ENABLE)
    #endif /* (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */

#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*       Type Definitions
***************************************/

typedef struct
{
    uint32 enableClockStretch;
    uint32 enableMedianFilter;
    uint32 numberOfAddresses;
    uint32 primarySlaveAddr;
    uint32 secondarySlaveAddr;
    uint32 subAddrSize;
    uint32 enableWake;
    uint8  enableByteMode;
} USER_PORT_EZI2C_INIT_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    void USER_PORT_EzI2CInit(const USER_PORT_EZI2C_INIT_STRUCT *config);
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

uint32 USER_PORT_EzI2CGetActivity(void);

void   USER_PORT_EzI2CSetAddress1(uint32 address);
uint32 USER_PORT_EzI2CGetAddress1(void);
void   USER_PORT_EzI2CSetBuffer1(uint32 bufSize, uint32 rwBoundary, volatile uint8 * buffer);
void   USER_PORT_EzI2CSetReadBoundaryBuffer1(uint32 rwBoundary);

#if(USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
    void   USER_PORT_EzI2CSetAddress2(uint32 address);
    uint32 USER_PORT_EzI2CGetAddress2(void);
    void   USER_PORT_EzI2CSetBuffer2(uint32 bufSize, uint32 rwBoundary, volatile uint8 * buffer);
    void   USER_PORT_EzI2CSetReadBoundaryBuffer2(uint32 rwBoundary);
#endif /* (USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */

#if(USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST)
    CY_ISR_PROTO(USER_PORT_EZI2C_STRETCH_ISR);
#endif /* (USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST) */

#if(USER_PORT_EZI2C_SCL_STRETCH_DISABLE_CONST)
    CY_ISR_PROTO(USER_PORT_EZI2C_NO_STRETCH_ISR);
#endif /* (USER_PORT_EZI2C_SCL_STRETCH_DISABLE) */


/***************************************
*            API Constants
***************************************/

/* Configuration structure constants */
#define USER_PORT_EZI2C_ONE_ADDRESS      (0u)
#define USER_PORT_EZI2C_TWO_ADDRESSES    (1u)

#define USER_PORT_EZI2C_PRIMARY_ADDRESS  (0u)
#define USER_PORT_EZI2C_BOTH_ADDRESSES   (1u)

#define USER_PORT_EZI2C_SUB_ADDR8_BITS   (0u)
#define USER_PORT_EZI2C_SUB_ADDR16_BITS  (1u)

/* USER_PORT_EzI2CGetActivity() return constants */
#define USER_PORT_EZI2C_STATUS_SECOND_OFFSET (1u)
#define USER_PORT_EZI2C_STATUS_READ1     ((uint32) (USER_PORT_INTR_SLAVE_I2C_NACK))        /* Bit [1]   */
#define USER_PORT_EZI2C_STATUS_WRITE1    ((uint32) (USER_PORT_INTR_SLAVE_I2C_WRITE_STOP))  /* Bit [3]   */
#define USER_PORT_EZI2C_STATUS_READ2     ((uint32) (USER_PORT_INTR_SLAVE_I2C_NACK >> \
                                                           USER_PORT_EZI2C_STATUS_SECOND_OFFSET)) /* Bit [0]   */
#define USER_PORT_EZI2C_STATUS_WRITE2    ((uint32) (USER_PORT_INTR_SLAVE_I2C_WRITE_STOP >> \
                                                           USER_PORT_EZI2C_STATUS_SECOND_OFFSET)) /* Bit [2]   */
#define USER_PORT_EZI2C_STATUS_ERR       ((uint32) (0x10u))                                       /* Bit [4]   */
#define USER_PORT_EZI2C_STATUS_BUSY      ((uint32) (0x20u))                                       /* Bit [5]   */
#define USER_PORT_EZI2C_CLEAR_STATUS     ((uint32) (0x1Fu))                                       /* Bits [0-4]*/
#define USER_PORT_EZI2C_CMPLT_INTR_MASK  ((uint32) (USER_PORT_INTR_SLAVE_I2C_NACK | \
                                                           USER_PORT_INTR_SLAVE_I2C_WRITE_STOP))


/***************************************
*     Vars with External Linkage
***************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    extern const USER_PORT_EZI2C_INIT_STRUCT USER_PORT_configEzI2C;
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*           FSM states
***************************************/

/* Returns to master when there is no data to transmit */
#define USER_PORT_EZI2C_OVFL_RETURN  (0xFFu)

/* States of EZI2C Slave FSM */
#define USER_PORT_EZI2C_FSM_OFFSET_HI8 ((uint8) (0x02u)) /* Idle state for 1 addr: waits for high byte offset */
#define USER_PORT_EZI2C_FSM_OFFSET_LO8 ((uint8) (0x00u)) /* Idle state for 2 addr: waits for low byte offset  */
#define USER_PORT_EZI2C_FSM_BYTE_WRITE ((uint8) (0x01u)) /* Data byte write state: byte by byte mode          */
#define USER_PORT_EZI2C_FSM_WAIT_STOP  ((uint8) (0x03u)) /* Discards written bytes as they do not match write
                                                                   criteria */
#define USER_PORT_EZI2C_FSM_WRITE_MASK ((uint8) (0x01u)) /* Incorporates write states: EZI2C_FSM_BYTE_WRITE and
                                                                   EZI2C_FSM_WAIT_STOP  */

#define USER_PORT_EZI2C_FSM_IDLE     ((USER_PORT_SUB_ADDRESS_SIZE16) ?      \
                                                (USER_PORT_EZI2C_FSM_OFFSET_HI8) : \
                                                (USER_PORT_EZI2C_FSM_OFFSET_LO8))

#define USER_PORT_EZI2C_CONTINUE_TRANSFER    (1u)
#define USER_PORT_EZI2C_COMPLETE_TRANSFER    (0u)

#define USER_PORT_EZI2C_NACK_RECEIVED_ADDRESS  (0u)
#define USER_PORT_EZI2C_ACK_RECEIVED_ADDRESS   (1u)

#define USER_PORT_EZI2C_ACTIVE_ADDRESS1  (0u)
#define USER_PORT_EZI2C_ACTIVE_ADDRESS2  (1u)


/***************************************
*       Macro Definitions
***************************************/

/* Access to global variables */
#if(USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST)

    #define USER_PORT_EZI2C_UPDATE_LOC_STATUS(activeAddress, locStatus) \
            do{ \
                (locStatus) >>= (activeAddress); \
            }while(0)

    #define USER_PORT_EZI2C_GET_INDEX(activeAddress) \
            ((USER_PORT_EZI2C_ACTIVE_ADDRESS1 == (activeAddress)) ? \
                ((uint32) USER_PORT_indexBuf1) :                    \
                ((uint32) USER_PORT_indexBuf2))

    #define USER_PORT_EZI2C_GET_OFFSET(activeAddress) \
            ((USER_PORT_EZI2C_ACTIVE_ADDRESS1 == (activeAddress)) ? \
                ((uint32) USER_PORT_offsetBuf1) :                   \
                ((uint32) USER_PORT_offsetBuf2))

    #define USER_PORT_EZI2C_SET_INDEX(activeAddress, locIndex) \
            do{ \
                if(USER_PORT_EZI2C_ACTIVE_ADDRESS1 == (activeAddress)) \
                {    \
                    USER_PORT_indexBuf1 = (uint16) (locIndex); \
                }    \
                else \
                {    \
                    USER_PORT_indexBuf2 = (uint16) (locIndex); \
                }    \
            }while(0)

    #define USER_PORT_EZI2C_SET_OFFSET(activeAddress, locOffset) \
            do{ \
                if(USER_PORT_EZI2C_ACTIVE_ADDRESS1 == (activeAddress)) \
                {    \
                    USER_PORT_offsetBuf1 = (uint16) (locOffset); \
                }    \
                else \
                {    \
                    USER_PORT_offsetBuf2 = (uint16) (locOffset); \
                }    \
            }while(0)
#else
    #define USER_PORT_EZI2C_UPDATE_LOC_STATUS(activeAddress, locStatus)  do{ /* Empty*/ }while(0)

    #define USER_PORT_EZI2C_GET_INDEX(activeAddress)  ((uint32) (USER_PORT_indexBuf1))

    #define USER_PORT_EZI2C_GET_OFFSET(activeAddress) ((uint32) (USER_PORT_offsetBuf1))

    #define USER_PORT_EZI2C_SET_INDEX(activeAddress, locIndex) \
            do{ \
                USER_PORT_indexBuf1 = (uint16) (locIndex); \
            }while(0)

    #define USER_PORT_EZI2C_SET_OFFSET(activeAddress, locOffset) \
            do{ \
                USER_PORT_offsetBuf1 = (uint16) (locOffset); \
            }while(0)

#endif  /* (USER_PORT_SUB_ADDRESS_SIZE16_CONST) */


/***************************************
*      Common Register Settings
***************************************/

#define USER_PORT_CTRL_EZI2C     (USER_PORT_CTRL_MODE_I2C)

#define USER_PORT_EZI2C_CTRL     (USER_PORT_I2C_CTRL_S_GENERAL_IGNORE | \
                                         USER_PORT_I2C_CTRL_SLAVE_MODE)

#define USER_PORT_EZI2C_CTRL_AUTO    (USER_PORT_I2C_CTRL_S_READY_ADDR_ACK      | \
                                             USER_PORT_I2C_CTRL_S_READY_DATA_ACK      | \
                                             USER_PORT_I2C_CTRL_S_NOT_READY_ADDR_NACK | \
                                             USER_PORT_I2C_CTRL_S_NOT_READY_DATA_NACK)

#define USER_PORT_EZI2C_RX_CTRL  ((USER_PORT_FIFO_SIZE - 1u)   | \
                                          USER_PORT_RX_CTRL_MSB_FIRST | \
                                          USER_PORT_RX_CTRL_ENABLED)

#define USER_PORT_EZI2C_TX_FIFO_CTRL (2u)
#define USER_PORT_TX_LOAD_SIZE       (2u)

#define USER_PORT_EZI2C_TX_CTRL  ((USER_PORT_FIFO_SIZE - 1u)   | \
                                          USER_PORT_TX_CTRL_MSB_FIRST | \
                                          USER_PORT_TX_CTRL_ENABLED)

#define USER_PORT_EZI2C_INTR_SLAVE_MASK  (USER_PORT_INTR_SLAVE_I2C_BUS_ERROR | \
                                                 USER_PORT_INTR_SLAVE_I2C_ARB_LOST  | \
                                                 USER_PORT_INTR_SLAVE_I2C_STOP)

#define USER_PORT_INTR_SLAVE_COMPLETE    (USER_PORT_INTR_SLAVE_I2C_STOP | \
                                                 USER_PORT_INTR_SLAVE_I2C_NACK | \
                                                 USER_PORT_INTR_SLAVE_I2C_WRITE_STOP)


/***************************************
*    Initialization Register Settings
***************************************/

#if(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)

    #define USER_PORT_EZI2C_DEFAULT_CTRL \
                                (USER_PORT_GET_CTRL_BYTE_MODE  (USER_PORT_EZI2C_BYTE_MODE_ENABLE)    | \
                                 USER_PORT_GET_CTRL_ADDR_ACCEPT(USER_PORT_EZI2C_NUMBER_OF_ADDRESSES) | \
                                 USER_PORT_GET_CTRL_EC_AM_MODE (USER_PORT_EZI2C_WAKE_ENABLE))

    #if(USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST)
        #define USER_PORT_EZI2C_DEFAULT_I2C_CTRL (USER_PORT_EZI2C_CTRL)
    #else
        #define USER_PORT_EZI2C_DEFAULT_I2C_CTRL (USER_PORT_EZI2C_CTRL_AUTO | USER_PORT_EZI2C_CTRL)
    #endif /* (USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST) */


    #define USER_PORT_EZI2C_DEFAULT_RX_MATCH \
                                (USER_PORT_GET_I2C_8BIT_ADDRESS(USER_PORT_EZI2C_PRIMARY_SLAVE_ADDRESS) | \
                                 USER_PORT_GET_RX_MATCH_MASK   (USER_PORT_EZI2C_SLAVE_ADDRESS_MASK))

    #define USER_PORT_EZI2C_DEFAULT_RX_CTRL  (USER_PORT_EZI2C_RX_CTRL)
    #define USER_PORT_EZI2C_DEFAULT_TX_CTRL  (USER_PORT_EZI2C_TX_CTRL)

    #define USER_PORT_EZI2C_DEFAULT_RX_FIFO_CTRL (0u)
    #if(USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST)
        #define USER_PORT_EZI2C_DEFAULT_TX_FIFO_CTRL (0u)
    #else
        #define USER_PORT_EZI2C_DEFAULT_TX_FIFO_CTRL (2u)
    #endif /* (USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST) */

    /* Interrupt sources */
    #define USER_PORT_EZI2C_DEFAULT_INTR_I2C_EC_MASK (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_EZI2C_DEFAULT_INTR_SPI_EC_MASK (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_EZI2C_DEFAULT_INTR_MASTER_MASK (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_EZI2C_DEFAULT_INTR_TX_MASK     (USER_PORT_NO_INTR_SOURCES)

    #if(USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST)
        #define USER_PORT_EZI2C_DEFAULT_INTR_RX_MASK     (USER_PORT_NO_INTR_SOURCES)
        #define USER_PORT_EZI2C_DEFAULT_INTR_SLAVE_MASK  (USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH | \
                                                                 USER_PORT_EZI2C_INTR_SLAVE_MASK)
    #else
        #define USER_PORT_EZI2C_DEFAULT_INTR_RX_MASK     (USER_PORT_INTR_RX_NOT_EMPTY)
        #define USER_PORT_EZI2C_DEFAULT_INTR_SLAVE_MASK  (USER_PORT_INTR_SLAVE_I2C_START | \
                                                                 USER_PORT_EZI2C_INTR_SLAVE_MASK)
    #endif /* (USER_PORT_EZI2C_SCL_STRETCH_ENABLE_CONST) */

#endif /* (USER_PORT_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* (CY_SCB_EZI2C_USER_PORT_H) */


/* [] END OF FILE */
