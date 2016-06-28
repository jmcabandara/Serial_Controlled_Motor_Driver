/*******************************************************************************
* File Name: USER_PORT_I2C.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the SCB Component in
*  the I2C mode.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_I2C_USER_PORT_H)
#define CY_SCB_I2C_USER_PORT_H

#include "USER_PORT.h"


/***************************************
*   Initial Parameter Constants
****************************************/

#define USER_PORT_I2C_MODE                   (1u)
#define USER_PORT_I2C_OVS_FACTOR_LOW         (8u)
#define USER_PORT_I2C_OVS_FACTOR_HIGH        (8u)
#define USER_PORT_I2C_MEDIAN_FILTER_ENABLE   (1u)
#define USER_PORT_I2C_SLAVE_ADDRESS          (8u)
#define USER_PORT_I2C_SLAVE_ADDRESS_MASK     (254u)
#define USER_PORT_I2C_ACCEPT_ADDRESS         (0u)
#define USER_PORT_I2C_WAKE_ENABLE            (0u)
#define USER_PORT_I2C_DATA_RATE              (100u)
#define USER_PORT_I2C_DATA_RATE_ACTUAL       (u)
#define USER_PORT_I2C_CLOCK_FROM_TERM        (0u)
#define USER_PORT_I2C_EXTERN_INTR_HANDLER    (0u)
#define USER_PORT_I2C_BYTE_MODE_ENABLE       (0u)
#define USER_PORT_I2C_MANUAL_OVS_CONTROL     (1u)


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* I2C sub mode enum */
#define USER_PORT_I2C_MODE_SLAVE              (0x01u)
#define USER_PORT_I2C_MODE_MASTER             (0x02u)
#define USER_PORT_I2C_MODE_MULTI_MASTER       (0x06u)
#define USER_PORT_I2C_MODE_MULTI_MASTER_SLAVE (0x07u)
#define USER_PORT_I2C_MODE_MULTI_MASTER_MASK  (0x04u)

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    /* Returns true if slave mode is enabled */
    #define USER_PORT_I2C_SLAVE  (0u != (USER_PORT_I2C_MODE_SLAVE & USER_PORT_mode))

    /* Returns true if master mode is enabled */
    #define USER_PORT_I2C_MASTER (0u != (USER_PORT_I2C_MODE_MASTER & USER_PORT_mode))

    /* Returns true if multi-master mode is enabled */
    #define USER_PORT_I2C_MULTI_MASTER \
                            (0u != (USER_PORT_I2C_MODE_MULTI_MASTER_MASK & USER_PORT_mode))

    /* Returns true if mode is multi-master-slave */
    #define USER_PORT_I2C_MULTI_MASTER_SLAVE \
                            (USER_PORT_I2C_MODE_MULTI_MASTER_SLAVE == USER_PORT_mode)

    /* Returns true if address accepts in RX FIFO */
    #define USER_PORT_CHECK_I2C_ACCEPT_ADDRESS   (0u != USER_PORT_acceptAddr)

    #define USER_PORT_I2C_WAKE_ENABLE_CONST          (1u)
    #define USER_PORT_I2C_SLAVE_CONST                (1u)
    #define USER_PORT_I2C_MASTER_CONST               (1u)
    #define USER_PORT_I2C_MULTI_MASTER_SLAVE_CONST   (1u)
    #define USER_PORT_CHECK_I2C_ACCEPT_ADDRESS_CONST (1u)

    /* Returns FIFO size */
    #if (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
        #define USER_PORT_I2C_FIFO_SIZE      (USER_PORT_FIFO_SIZE)
    #else
        #define USER_PORT_I2C_FIFO_SIZE      (USER_PORT_GET_FIFO_SIZE(USER_PORT_CTRL_REG & \
                                                                                    USER_PORT_CTRL_BYTE_MODE))
    #endif /* (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */

#else

    /* Returns true if slave mode is enabled */
    #define USER_PORT_I2C_SLAVE   (0u != (USER_PORT_I2C_MODE_SLAVE & USER_PORT_I2C_MODE))

    /* Returns true if master mode is enabled */
    #define USER_PORT_I2C_MASTER  (0u != (USER_PORT_I2C_MODE_MASTER & USER_PORT_I2C_MODE))

    /* Returns true if multi-master mode is enabled */
    #define USER_PORT_I2C_MULTI_MASTER \
                                    (0u != (USER_PORT_I2C_MODE_MULTI_MASTER_MASK & USER_PORT_I2C_MODE))

    /* Returns true if mode is multi-master-slave */
    #define USER_PORT_I2C_MULTI_MASTER_SLAVE \
                                    (USER_PORT_I2C_MODE_MULTI_MASTER_SLAVE == USER_PORT_I2C_MODE)

    /* Returns true if address accepts in RX FIFO */
    #define USER_PORT_CHECK_I2C_ACCEPT_ADDRESS   (0u != USER_PORT_I2C_ACCEPT_ADDRESS)

    /* Returns true if wakeup on address match is enabled */
    #define USER_PORT_I2C_WAKE_ENABLE_CONST  (0u != USER_PORT_I2C_WAKE_ENABLE)

    #define USER_PORT_I2C_SLAVE_CONST    (USER_PORT_I2C_SLAVE)
    #define USER_PORT_I2C_MASTER_CONST   (USER_PORT_I2C_MASTER)
    #define USER_PORT_I2C_MULTI_MASTER_SLAVE_CONST (USER_PORT_I2C_MULTI_MASTER_SLAVE)
    #define USER_PORT_CHECK_I2C_ACCEPT_ADDRESS_CONST (USER_PORT_CHECK_I2C_ACCEPT_ADDRESS)

    /* I2C: TX or RX FIFO size */
    #if (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
        #define USER_PORT_I2C_FIFO_SIZE  (USER_PORT_FIFO_SIZE)
    #else
        #define USER_PORT_I2C_FIFO_SIZE  USER_PORT_GET_FIFO_SIZE(USER_PORT_I2C_BYTE_MODE_ENABLE)
    #endif /* ((USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */

    /* Adjust AF and DF filter settings. Ticket ID#176179 */
    #if ((USER_PORT_I2C_MODE_SLAVE == USER_PORT_I2C_MODE) ||     \
            ((USER_PORT_I2C_MODE_SLAVE != USER_PORT_I2C_MODE) && \
             (USER_PORT_I2C_DATA_RATE_ACTUAL <= USER_PORT_I2C_DATA_RATE_FS_MODE_MAX)))

        #define USER_PORT_I2C_MEDIAN_FILTER_ENABLE_ADJ       (0u)
        #define USER_PORT_I2C_CFG_ANALOG_FITER_ENABLE_ADJ    do{;}while(0)
    #else
        #define USER_PORT_I2C_MEDIAN_FILTER_ENABLE_ADJ       (1u)
        #define USER_PORT_I2C_CFG_ANALOG_FITER_ENABLE_ADJ    USER_PORT_I2C_CFG_ANALOG_FITER_DISABLE
    #endif

    /* Select oversampling factor low and high */
    #define USER_PORT_I2C_OVS_FACTOR_LOW_MIN     ((0u != USER_PORT_I2C_MANUAL_OVS_CONTROL) ? \
                                                            (8u) : (8u))

    #define USER_PORT_I2C_OVS_FACTOR_HIGH_MIN    ((0u != USER_PORT_I2C_MANUAL_OVS_CONTROL) ? \
                                                            (8u) : (8u))

#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*       Type Definitions
***************************************/

typedef struct
{
    uint32 mode;
    uint32 oversampleLow;
    uint32 oversampleHigh;
    uint32 enableMedianFilter;
    uint32 slaveAddr;
    uint32 slaveAddrMask;
    uint32 acceptAddr;
    uint32 enableWake;
    uint8  enableByteMode;
    uint16 dataRate;
} USER_PORT_I2C_INIT_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

/* Common functions */
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    void USER_PORT_I2CInit(const USER_PORT_I2C_INIT_STRUCT *config);
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

/* I2C Master functions prototypes */
#if(USER_PORT_I2C_MASTER_CONST)
    /* Read and Clear status functions */
    uint32 USER_PORT_I2CMasterStatus(void);
    uint32 USER_PORT_I2CMasterClearStatus(void);

    /* Interrupt based operation functions */
    uint32 USER_PORT_I2CMasterWriteBuf(uint32 slaveAddress, uint8 * wrData, uint32 cnt, uint32 mode);
    uint32 USER_PORT_I2CMasterReadBuf(uint32 slaveAddress, uint8 * rdData, uint32 cnt, uint32 mode);
    uint32 USER_PORT_I2CMasterGetReadBufSize(void);
    uint32 USER_PORT_I2CMasterGetWriteBufSize(void);
    void   USER_PORT_I2CMasterClearReadBuf(void);
    void   USER_PORT_I2CMasterClearWriteBuf(void);

    /* Manual operation functions */
    uint32 USER_PORT_I2CMasterSendStart(uint32 slaveAddress, uint32 bitRnW);
    uint32 USER_PORT_I2CMasterSendRestart(uint32 slaveAddress, uint32 bitRnW);
    uint32 USER_PORT_I2CMasterSendStop(void);
    uint32 USER_PORT_I2CMasterWriteByte(uint32 theByte);
    uint32 USER_PORT_I2CMasterReadByte(uint32 ackNack);
#endif /* (USER_PORT_I2C_MASTER_CONST) */

/* I2C Slave functions prototypes */
#if(USER_PORT_I2C_SLAVE_CONST)
    /* Read and Clear status functions */
    uint32 USER_PORT_I2CSlaveStatus(void);
    uint32 USER_PORT_I2CSlaveClearReadStatus(void);
    uint32 USER_PORT_I2CSlaveClearWriteStatus(void);

    /* Set Slave address and mask */
    void   USER_PORT_I2CSlaveSetAddress(uint32 address);
    void   USER_PORT_I2CSlaveSetAddressMask(uint32 addressMask);

    /* Interrupt based operation functions */
    void   USER_PORT_I2CSlaveInitReadBuf(uint8 * rdBuf, uint32 bufSize);
    void   USER_PORT_I2CSlaveInitWriteBuf(uint8 * wrBuf, uint32 bufSize);
    uint32 USER_PORT_I2CSlaveGetReadBufSize(void);
    uint32 USER_PORT_I2CSlaveGetWriteBufSize(void);
    void   USER_PORT_I2CSlaveClearReadBuf(void);
    void   USER_PORT_I2CSlaveClearWriteBuf(void);
#endif /* (USER_PORT_I2C_SLAVE_CONST) */

CY_ISR_PROTO(USER_PORT_I2C_ISR);


/***************************************
*            API Constants
***************************************/

/* I2C sub mode enum */
#define USER_PORT_I2C_MODE_SLAVE              (0x01u)
#define USER_PORT_I2C_MODE_MASTER             (0x02u)
#define USER_PORT_I2C_MODE_MULTI_MASTER       (0x06u)
#define USER_PORT_I2C_MODE_MULTI_MASTER_SLAVE (0x07u)
#define USER_PORT_I2C_MODE_MULTI_MASTER_MASK  (0x04u)

/* Master/Slave control constants */
#define USER_PORT_I2C_WRITE_XFER_MODE    (0u)    /* Write    */
#define USER_PORT_I2C_READ_XFER_MODE     (1u)    /* Read     */
#define USER_PORT_I2C_ACK_DATA           (0u)    /* Send ACK */
#define USER_PORT_I2C_NAK_DATA           (1u)    /* Send NAK */

/* "Mode" constants for MasterWriteBuf() or MasterReadBuf() function */
#define USER_PORT_I2C_MODE_COMPLETE_XFER     (0x00u)    /* Full transfer with Start and Stop       */
#define USER_PORT_I2C_MODE_REPEAT_START      (0x01u)    /* Begin with a ReStart instead of a Start */
#define USER_PORT_I2C_MODE_NO_STOP           (0x02u)    /* Complete the transfer without a Stop    */

/* Master status */
#define USER_PORT_I2C_MSTAT_CLEAR            ((uint16) 0x00u)   /* Clear (init) status value */

#define USER_PORT_I2C_MSTAT_RD_CMPLT         ((uint16) 0x01u)   /* Read complete               */
#define USER_PORT_I2C_MSTAT_WR_CMPLT         ((uint16) 0x02u)   /* Write complete              */
#define USER_PORT_I2C_MSTAT_XFER_INP         ((uint16) 0x04u)   /* Master transfer in progress */
#define USER_PORT_I2C_MSTAT_XFER_HALT        ((uint16) 0x08u)   /* Transfer is halted          */

#define USER_PORT_I2C_MSTAT_ERR_MASK         ((uint16) 0x3F0u) /* Mask for all errors                          */
#define USER_PORT_I2C_MSTAT_ERR_SHORT_XFER   ((uint16) 0x10u)  /* Master NAKed before end of packet            */
#define USER_PORT_I2C_MSTAT_ERR_ADDR_NAK     ((uint16) 0x20u)  /* Slave did not ACK                            */
#define USER_PORT_I2C_MSTAT_ERR_ARB_LOST     ((uint16) 0x40u)  /* Master lost arbitration during communication */
#define USER_PORT_I2C_MSTAT_ERR_ABORT_XFER   ((uint16) 0x80u)  /* The Slave was addressed before the Start gen */
#define USER_PORT_I2C_MSTAT_ERR_BUS_ERROR    ((uint16) 0x100u) /* The misplaced Start or Stop was occurred     */
#define USER_PORT_I2C_MSTAT_ERR_XFER         ((uint16) 0x200u) /* Error during transfer                        */

/* Master API returns */
#define USER_PORT_I2C_MSTR_NO_ERROR          (0x00u)  /* Function complete without error                       */
#define USER_PORT_I2C_MSTR_ERR_ARB_LOST      (0x01u)  /* Master lost arbitration: INTR_MASTER_I2C_ARB_LOST     */
#define USER_PORT_I2C_MSTR_ERR_LB_NAK        (0x02u)  /* Last Byte Naked: INTR_MASTER_I2C_NACK                 */
#define USER_PORT_I2C_MSTR_NOT_READY         (0x04u)  /* Master on the bus or Slave operation is in progress   */
#define USER_PORT_I2C_MSTR_BUS_BUSY          (0x08u)  /* Bus is busy, process not started                      */
#define USER_PORT_I2C_MSTR_ERR_ABORT_START   (0x10u)  /* Slave was addressed before master begin Start gen     */
#define USER_PORT_I2C_MSTR_ERR_BUS_ERR       (0x100u) /* Bus error has: INTR_MASTER_I2C_BUS_ERROR              */

/* Slave Status Constants */
#define USER_PORT_I2C_SSTAT_RD_CMPLT         (0x01u)    /* Read transfer complete                        */
#define USER_PORT_I2C_SSTAT_RD_BUSY          (0x02u)    /* Read transfer in progress                     */
#define USER_PORT_I2C_SSTAT_RD_OVFL          (0x04u)    /* Read overflow: master reads above buffer size */
#define USER_PORT_I2C_SSTAT_RD_ERR           (0x08u)    /* Read was interrupted by misplaced Start/Stop  */
#define USER_PORT_I2C_SSTAT_RD_MASK          (0x0Fu)    /* Read Status Mask                              */
#define USER_PORT_I2C_SSTAT_RD_NO_ERR        (0x00u)    /* Read no Error                                 */

#define USER_PORT_I2C_SSTAT_WR_CMPLT         (0x10u)    /* Write transfer complete                         */
#define USER_PORT_I2C_SSTAT_WR_BUSY          (0x20u)    /* Write transfer in progress                      */
#define USER_PORT_I2C_SSTAT_WR_OVFL          (0x40u)    /* Write overflow: master writes above buffer size */
#define USER_PORT_I2C_SSTAT_WR_ERR           (0x80u)    /* Write was interrupted by misplaced Start/Stop   */
#define USER_PORT_I2C_SSTAT_WR_MASK          (0xF0u)    /* Write Status Mask                               */
#define USER_PORT_I2C_SSTAT_WR_NO_ERR        (0x00u)    /* Write no Error                                  */

#define USER_PORT_I2C_SSTAT_RD_CLEAR         (0x0Du)    /* Read Status clear: do not clear */
#define USER_PORT_I2C_SSTAT_WR_CLEAR         (0xD0u)    /* Write Status Clear */

/* Internal I2C component constants */
#define USER_PORT_I2C_READ_FLAG              (0x01u)     /* Read flag of the Address */
#define USER_PORT_I2C_SLAVE_OVFL_RETURN      (0xFFu)     /* Return by slave when overflow */

#define USER_PORT_I2C_RESET_ERROR            (0x01u)     /* Flag to re-enable SCB IP */


/***************************************
*     Vars with External Linkage
***************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    extern const USER_PORT_I2C_INIT_STRUCT USER_PORT_configI2C;
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*           FSM states
***************************************/

#define USER_PORT_I2C_FSM_EXIT_IDLE      (0x00u) /* Master and Slave are not active, need to exit to IDLE */

/* Slave mode FSM states */
#define USER_PORT_I2C_FSM_IDLE           (0x10u) /* Idle I2C state                */
#define USER_PORT_I2C_FSM_SLAVE          (0x10u) /* Slave mask for all its states */

#define USER_PORT_I2C_FSM_SL_WR          (0x11u) /* Slave write states */
#define USER_PORT_I2C_FSM_SL_RD          (0x12u) /* Slave read  states */

/* Master mode FSM states */
#define USER_PORT_I2C_FSM_MASTER         (0x20u) /* Master mask for all its states */
#define USER_PORT_I2C_FSM_MSTR_ADDR      (0x08u) /* Master address phase           */
#define USER_PORT_I2C_FSM_MSTR_DATA      (0x04u) /* Master data phase              */
#define USER_PORT_I2C_FSM_MSTR_RD        (0x01u) /* Master read phase              */

#define USER_PORT_I2C_FSM_MSTR_WR_ADDR   (0x28u) /* FSM master transmit address with write          */
#define USER_PORT_I2C_FSM_MSTR_RD_ADDR   (0x29u) /* FSM master transmit address with read           */
#define USER_PORT_I2C_FSM_MSTR_WR_DATA   (0x24u) /* FSM master writes data into the slave           */
#define USER_PORT_I2C_FSM_MSTR_RD_DATA   (0x25u) /* FSM master reads data from the slave            */
#define USER_PORT_I2C_FSM_MSTR_HALT      (0x60u) /* FSM master halt state: wait for Stop or ReStart */

/* Requests to complete any on-going transfer */
#define USER_PORT_I2C_CMPLT_ANY_TRANSFER     (0x01u)

/* Returns true if FSM is in any Master state */
#define USER_PORT_CHECK_I2C_FSM_MASTER   (0u != (USER_PORT_I2C_FSM_MASTER & USER_PORT_state))

/* Returns true if FSM is in any Slave state */
#define USER_PORT_CHECK_I2C_FSM_SLAVE    (0u != (USER_PORT_I2C_FSM_SLAVE  & USER_PORT_state))

/* Returns true if FSM is in Master send address state */
#define USER_PORT_CHECK_I2C_FSM_ADDR (0u != (USER_PORT_I2C_FSM_MSTR_ADDR & USER_PORT_state))

/* Returns true if FSM is in Master send or receive data state */
#define USER_PORT_CHECK_I2C_FSM_DATA (0u != (USER_PORT_I2C_FSM_MSTR_DATA  & USER_PORT_state))

/* Returns true if FSM is in any of read states: applied for Slave and Master */
#define USER_PORT_CHECK_I2C_FSM_RD   (0u != (USER_PORT_I2C_FSM_MSTR_RD  & USER_PORT_state))

/* Returns true if FSM is in IDLE state */
#define USER_PORT_CHECK_I2C_FSM_IDLE (USER_PORT_I2C_FSM_IDLE == USER_PORT_state)

/* Returns true if FSM is HALT state */
#define USER_PORT_CHECK_I2C_FSM_HALT (USER_PORT_I2C_FSM_MSTR_HALT == USER_PORT_state)

/* Set Master read or write completion depends on state */
#define USER_PORT_GET_I2C_MSTAT_CMPLT (USER_PORT_CHECK_I2C_FSM_RD ?           \
                                                    (USER_PORT_I2C_MSTAT_RD_CMPLT) : \
                                                    (USER_PORT_I2C_MSTAT_WR_CMPLT))


/***************************************
*       Macro Definitions
***************************************/

/* Returns TRUE if sourceMask is set in USER_PORT_I2C_MASTER_CMD_REG: used to check if Start was generated */
#define USER_PORT_CHECK_I2C_MASTER_CMD(sourceMask)   (0u != (USER_PORT_I2C_MASTER_CMD_REG & (sourceMask)))

/* Returns TRUE if USER_PORT_MODE_NO_STOP is set in USER_PORT_mstrControl: detects NoStop condition */
#define USER_PORT_CHECK_I2C_MODE_NO_STOP(mode)   (0u != (USER_PORT_I2C_MODE_NO_STOP & (mode)))

/* Returns TRUE if USER_PORT_MODE_REPEAT_START is set: used to detect when generate ReStart condition */
#define USER_PORT_CHECK_I2C_MODE_RESTART(mode)   (0u != (USER_PORT_I2C_MODE_REPEAT_START  & (mode)))

/* Returns TRUE if USER_PORT_state is in one of master states */
#define USER_PORT_CHECK_I2C_MASTER_ACTIVE    (USER_PORT_CHECK_I2C_FSM_MASTER)


/***************************************
*      Common Register Settings
***************************************/

#define USER_PORT_CTRL_I2C       (USER_PORT_CTRL_MODE_I2C)

#define USER_PORT_I2C_CTRL       (USER_PORT_I2C_CTRL_S_GENERAL_IGNORE)

#define USER_PORT_I2C_RX_CTRL    ((USER_PORT_FIFO_SIZE - 1u)  | \
                                         USER_PORT_RX_CTRL_MSB_FIRST | \
                                         USER_PORT_RX_CTRL_ENABLED)

#define USER_PORT_I2C_TX_CTRL    ((USER_PORT_FIFO_SIZE - 1u)  | \
                                         USER_PORT_TX_CTRL_MSB_FIRST | \
                                         USER_PORT_TX_CTRL_ENABLED)

#define USER_PORT_I2C_INTR_SLAVE_MASK    (USER_PORT_INTR_SLAVE_I2C_ADDR_MATCH | \
                                                 USER_PORT_INTR_SLAVE_I2C_NACK       | \
                                                 USER_PORT_INTR_SLAVE_I2C_WRITE_STOP | \
                                                 USER_PORT_INTR_SLAVE_I2C_BUS_ERROR  | \
                                                 USER_PORT_INTR_SLAVE_I2C_ARB_LOST)

#define USER_PORT_I2C_INTR_MASTER_MASK   (USER_PORT_INTR_MASTER_I2C_ARB_LOST | \
                                                 USER_PORT_INTR_MASTER_I2C_NACK     | \
                                                 USER_PORT_INTR_MASTER_I2C_STOP     | \
                                                 USER_PORT_INTR_MASTER_I2C_BUS_ERROR)

/* Calculates tLOW in uS units */
#define USER_PORT_I2C_TLOW_TIME  ((1000u / USER_PORT_I2C_DATA_RATE) + \
                                            ((0u != (1000u % USER_PORT_I2C_DATA_RATE)) ? (1u) : (0u)))
/* tHIGH = tLOW */
#define USER_PORT_I2C_THIGH_TIME (USER_PORT_I2C_TLOW_TIME)

#define USER_PORT_I2C_SCL_LOW    (0u)
#define USER_PORT_I2C_SCL_HIGH   (1u)


/***************************************
*    Initialization Register Settings
***************************************/

#if(USER_PORT_SCB_MODE_I2C_CONST_CFG)

    #if (!USER_PORT_CY_SCBIP_V0)
        #define USER_PORT_I2C_WAKE_ENABLE_ADJ  (USER_PORT_I2C_MULTI_MASTER_SLAVE ? \
                                                            (0u) : (USER_PORT_I2C_WAKE_ENABLE))
    #else
        #define USER_PORT_I2C_WAKE_ENABLE_ADJ    (USER_PORT_I2C_WAKE_ENABLE)
    #endif /* (!USER_PORT_CY_SCBIP_V0) */

    #define USER_PORT_I2C_MODE_MASKED    (USER_PORT_I2C_MODE & \
                                                (USER_PORT_I2C_MODE_SLAVE | USER_PORT_I2C_MODE_MASTER))

    #define USER_PORT_I2C_DEFAULT_CTRL \
                                (USER_PORT_GET_CTRL_BYTE_MODE  (USER_PORT_I2C_BYTE_MODE_ENABLE) | \
                                 USER_PORT_GET_CTRL_ADDR_ACCEPT(USER_PORT_I2C_ACCEPT_ADDRESS)   | \
                                 USER_PORT_GET_CTRL_EC_AM_MODE (USER_PORT_I2C_WAKE_ENABLE_ADJ))

    #define USER_PORT_I2C_DEFAULT_I2C_CTRL \
                            (USER_PORT_GET_I2C_CTRL_HIGH_PHASE_OVS(USER_PORT_I2C_OVS_FACTOR_HIGH_MIN) | \
                             USER_PORT_GET_I2C_CTRL_LOW_PHASE_OVS (USER_PORT_I2C_OVS_FACTOR_LOW_MIN)  | \
                             USER_PORT_GET_I2C_CTRL_SL_MSTR_MODE  (USER_PORT_I2C_MODE_MASKED)         | \
                             USER_PORT_I2C_CTRL)

    #define USER_PORT_I2C_DEFAULT_RX_MATCH ((USER_PORT_I2C_SLAVE) ? \
                                (USER_PORT_GET_I2C_8BIT_ADDRESS(USER_PORT_I2C_SLAVE_ADDRESS) | \
                                 USER_PORT_GET_RX_MATCH_MASK   (USER_PORT_I2C_SLAVE_ADDRESS_MASK)) : (0u))

    #define USER_PORT_I2C_DEFAULT_RX_CTRL \
                                (USER_PORT_GET_RX_CTRL_MEDIAN(USER_PORT_I2C_MEDIAN_FILTER_ENABLE_ADJ) | \
                                 USER_PORT_I2C_RX_CTRL)

    #define USER_PORT_I2C_DEFAULT_TX_CTRL  (USER_PORT_I2C_TX_CTRL)

    #define USER_PORT_I2C_DEFAULT_RX_FIFO_CTRL (0u)
    #define USER_PORT_I2C_DEFAULT_TX_FIFO_CTRL (0u)

    /* Interrupt sources */
    #define USER_PORT_I2C_DEFAULT_INTR_I2C_EC_MASK   (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_I2C_DEFAULT_INTR_SPI_EC_MASK   (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_I2C_DEFAULT_INTR_RX_MASK       (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_I2C_DEFAULT_INTR_TX_MASK       (USER_PORT_NO_INTR_SOURCES)

    #define USER_PORT_I2C_DEFAULT_INTR_SLAVE_MASK    ((USER_PORT_I2C_SLAVE) ? \
                                                                (USER_PORT_I2C_INTR_SLAVE_MASK) : (0u))

    #define USER_PORT_I2C_DEFAULT_INTR_MASTER_MASK   ((USER_PORT_I2C_MASTER) ? \
                                                                (USER_PORT_I2C_INTR_MASTER_MASK) : (0u))

#endif /* (USER_PORT_SCB_MODE_I2C_CONST_CFG) */

#endif /* (CY_SCB_I2C_USER_PORT_H) */


/* [] END OF FILE */
