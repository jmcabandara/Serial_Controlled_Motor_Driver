/*******************************************************************************
* File Name: USER_PORT_SPI_UART.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_USER_PORT_H)
#define CY_SCB_SPI_UART_USER_PORT_H

#include "USER_PORT.h"


/***************************************
*   SPI Initial Parameter Constants
****************************************/

#define USER_PORT_SPI_MODE                   (0u)
#define USER_PORT_SPI_SUB_MODE               (0u)
#define USER_PORT_SPI_CLOCK_MODE             (0u)
#define USER_PORT_SPI_OVS_FACTOR             (16u)
#define USER_PORT_SPI_MEDIAN_FILTER_ENABLE   (0u)
#define USER_PORT_SPI_LATE_MISO_SAMPLE_ENABLE (0u)
#define USER_PORT_SPI_RX_DATA_BITS_NUM       (8u)
#define USER_PORT_SPI_TX_DATA_BITS_NUM       (8u)
#define USER_PORT_SPI_WAKE_ENABLE            (0u)
#define USER_PORT_SPI_BITS_ORDER             (1u)
#define USER_PORT_SPI_TRANSFER_SEPARATION    (1u)
#define USER_PORT_SPI_NUMBER_OF_SS_LINES     (1u)
#define USER_PORT_SPI_RX_BUFFER_SIZE         (8u)
#define USER_PORT_SPI_TX_BUFFER_SIZE         (8u)

#define USER_PORT_SPI_INTERRUPT_MODE         (0u)

#define USER_PORT_SPI_INTR_RX_MASK           (0u)
#define USER_PORT_SPI_INTR_TX_MASK           (0u)

#define USER_PORT_SPI_RX_TRIGGER_LEVEL       (7u)
#define USER_PORT_SPI_TX_TRIGGER_LEVEL       (0u)

#define USER_PORT_SPI_BYTE_MODE_ENABLE       (0u)
#define USER_PORT_SPI_FREE_RUN_SCLK_ENABLE   (0u)
#define USER_PORT_SPI_SS0_POLARITY           (0u)
#define USER_PORT_SPI_SS1_POLARITY           (0u)
#define USER_PORT_SPI_SS2_POLARITY           (0u)
#define USER_PORT_SPI_SS3_POLARITY           (0u)


/***************************************
*   UART Initial Parameter Constants
****************************************/

#define USER_PORT_UART_SUB_MODE              (0u)
#define USER_PORT_UART_DIRECTION             (3u)
#define USER_PORT_UART_DATA_BITS_NUM         (8u)
#define USER_PORT_UART_PARITY_TYPE           (2u)
#define USER_PORT_UART_STOP_BITS_NUM         (2u)
#define USER_PORT_UART_OVS_FACTOR            (12u)
#define USER_PORT_UART_IRDA_LOW_POWER        (0u)
#define USER_PORT_UART_MEDIAN_FILTER_ENABLE  (0u)
#define USER_PORT_UART_RETRY_ON_NACK         (0u)
#define USER_PORT_UART_IRDA_POLARITY         (0u)
#define USER_PORT_UART_DROP_ON_FRAME_ERR     (0u)
#define USER_PORT_UART_DROP_ON_PARITY_ERR    (0u)
#define USER_PORT_UART_WAKE_ENABLE           (0u)
#define USER_PORT_UART_RX_BUFFER_SIZE        (8u)
#define USER_PORT_UART_TX_BUFFER_SIZE        (8u)
#define USER_PORT_UART_MP_MODE_ENABLE        (0u)
#define USER_PORT_UART_MP_ACCEPT_ADDRESS     (0u)
#define USER_PORT_UART_MP_RX_ADDRESS         (2u)
#define USER_PORT_UART_MP_RX_ADDRESS_MASK    (255u)

#define USER_PORT_UART_INTERRUPT_MODE        (2u)

#define USER_PORT_UART_INTR_RX_MASK          (0u)
#define USER_PORT_UART_INTR_TX_MASK          (0u)

#define USER_PORT_UART_RX_TRIGGER_LEVEL      (7u)
#define USER_PORT_UART_TX_TRIGGER_LEVEL      (0u)

#define USER_PORT_UART_BYTE_MODE_ENABLE      (0u)
#define USER_PORT_UART_CTS_ENABLE            (0u)
#define USER_PORT_UART_CTS_POLARITY          (0u)
#define USER_PORT_UART_RTS_POLARITY          (0u)
#define USER_PORT_UART_RTS_FIFO_LEVEL        (4u)

/* SPI mode enum */
#define USER_PORT_SPI_SLAVE  (0u)
#define USER_PORT_SPI_MASTER (1u)

/* UART direction enum */
#define USER_PORT_UART_RX    (1u)
#define USER_PORT_UART_TX    (2u)
#define USER_PORT_UART_TX_RX (3u)


/***************************************
*   Conditional Compilation Parameters
****************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    /* Mode */
    #define USER_PORT_SPI_MASTER_CONST       (1u)

    /* Direction */
    #define USER_PORT_RX_DIRECTION           (1u)
    #define USER_PORT_TX_DIRECTION           (1u)
    #define USER_PORT_UART_RX_DIRECTION      (1u)
    #define USER_PORT_UART_TX_DIRECTION      (1u)

    /* Only external RX and TX buffer for Uncofigured mode */
    #define USER_PORT_INTERNAL_RX_SW_BUFFER   (0u)
    #define USER_PORT_INTERNAL_TX_SW_BUFFER   (0u)

    /* Get RX and TX buffer size */
    #define USER_PORT_RX_BUFFER_SIZE (USER_PORT_rxBufferSize)
    #define USER_PORT_TX_BUFFER_SIZE (USER_PORT_txBufferSize)

    /* Return true if buffer is provided */
    #define USER_PORT_CHECK_RX_SW_BUFFER (NULL != USER_PORT_rxBuffer)
    #define USER_PORT_CHECK_TX_SW_BUFFER (NULL != USER_PORT_txBuffer)

    /* Always provide global variables to support RX and TX buffers */
    #define USER_PORT_INTERNAL_RX_SW_BUFFER_CONST    (1u)
    #define USER_PORT_INTERNAL_TX_SW_BUFFER_CONST    (1u)

    /* Get wakeup enable option */
    #define USER_PORT_SPI_WAKE_ENABLE_CONST  (1u)
    #define USER_PORT_CHECK_SPI_WAKE_ENABLE  (0u != USER_PORT_scbEnableWake)
    #define USER_PORT_UART_WAKE_ENABLE_CONST (1u)

    /* SPI/UART: TX or RX FIFO size */
    #if (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
        #define USER_PORT_SPI_UART_FIFO_SIZE (USER_PORT_FIFO_SIZE)
    #else
        #define USER_PORT_SPI_UART_FIFO_SIZE (USER_PORT_GET_FIFO_SIZE(USER_PORT_CTRL_REG & \
                                                                                    USER_PORT_CTRL_BYTE_MODE))
    #endif /* (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */

#else

    /* Internal RX and TX buffer: for SPI or UART */
    #if (USER_PORT_SCB_MODE_SPI_CONST_CFG)

        /* SPI Direction */
        #define USER_PORT_SPI_RX_DIRECTION (1u)
        #define USER_PORT_SPI_TX_DIRECTION (1u)

        /* Get FIFO size */
        #if (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
            #define USER_PORT_SPI_UART_FIFO_SIZE    (USER_PORT_FIFO_SIZE)
        #else
            #define USER_PORT_SPI_UART_FIFO_SIZE \
                                           USER_PORT_GET_FIFO_SIZE(USER_PORT_SPI_BYTE_MODE_ENABLE)

        #endif /* (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */

        /* SPI internal RX and TX buffers */
        #define USER_PORT_INTERNAL_SPI_RX_SW_BUFFER  (USER_PORT_SPI_RX_BUFFER_SIZE > \
                                                                USER_PORT_SPI_UART_FIFO_SIZE)
        #define USER_PORT_INTERNAL_SPI_TX_SW_BUFFER  (USER_PORT_SPI_TX_BUFFER_SIZE > \
                                                                USER_PORT_SPI_UART_FIFO_SIZE)

        /* Internal SPI RX and TX buffer */
        #define USER_PORT_INTERNAL_RX_SW_BUFFER  (USER_PORT_INTERNAL_SPI_RX_SW_BUFFER)
        #define USER_PORT_INTERNAL_TX_SW_BUFFER  (USER_PORT_INTERNAL_SPI_TX_SW_BUFFER)

        /* Internal SPI RX and TX buffer size */
        #define USER_PORT_RX_BUFFER_SIZE         (USER_PORT_SPI_RX_BUFFER_SIZE + 1u)
        #define USER_PORT_TX_BUFFER_SIZE         (USER_PORT_SPI_TX_BUFFER_SIZE)

        /* Get wakeup enable option */
        #define USER_PORT_SPI_WAKE_ENABLE_CONST  (0u != USER_PORT_SPI_WAKE_ENABLE)
        #define USER_PORT_UART_WAKE_ENABLE_CONST (0u)

    #else

        /* UART Direction */
        #define USER_PORT_UART_RX_DIRECTION (0u != (USER_PORT_UART_DIRECTION & USER_PORT_UART_RX))
        #define USER_PORT_UART_TX_DIRECTION (0u != (USER_PORT_UART_DIRECTION & USER_PORT_UART_TX))

        /* Get FIFO size */
        #if (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
            #define USER_PORT_SPI_UART_FIFO_SIZE    (USER_PORT_FIFO_SIZE)
        #else
            #define USER_PORT_SPI_UART_FIFO_SIZE \
                                           USER_PORT_GET_FIFO_SIZE(USER_PORT_UART_BYTE_MODE_ENABLE)
        #endif /* (USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */

        /* UART internal RX and TX buffers */
        #define USER_PORT_INTERNAL_UART_RX_SW_BUFFER  (USER_PORT_UART_RX_BUFFER_SIZE > \
                                                                USER_PORT_SPI_UART_FIFO_SIZE)
        #define USER_PORT_INTERNAL_UART_TX_SW_BUFFER  (USER_PORT_UART_TX_BUFFER_SIZE > \
                                                                    USER_PORT_SPI_UART_FIFO_SIZE)

        /* Internal UART RX and TX buffer */
        #define USER_PORT_INTERNAL_RX_SW_BUFFER  (USER_PORT_INTERNAL_UART_RX_SW_BUFFER)
        #define USER_PORT_INTERNAL_TX_SW_BUFFER  (USER_PORT_INTERNAL_UART_TX_SW_BUFFER)

        /* Internal UART RX and TX buffer size */
        #define USER_PORT_RX_BUFFER_SIZE         (USER_PORT_UART_RX_BUFFER_SIZE + 1u)
        #define USER_PORT_TX_BUFFER_SIZE         (USER_PORT_UART_TX_BUFFER_SIZE)

        /* Get wakeup enable option */
        #define USER_PORT_SPI_WAKE_ENABLE_CONST  (0u)
        #define USER_PORT_UART_WAKE_ENABLE_CONST (0u != USER_PORT_UART_WAKE_ENABLE)

    #endif /* (USER_PORT_SCB_MODE_SPI_CONST_CFG) */

    /* Mode */
    #define USER_PORT_SPI_MASTER_CONST   (USER_PORT_SPI_MODE == USER_PORT_SPI_MASTER)

    /* Direction */
    #define USER_PORT_RX_DIRECTION ((USER_PORT_SCB_MODE_SPI_CONST_CFG) ? \
                                            (USER_PORT_SPI_RX_DIRECTION) : (USER_PORT_UART_RX_DIRECTION))

    #define USER_PORT_TX_DIRECTION ((USER_PORT_SCB_MODE_SPI_CONST_CFG) ? \
                                            (USER_PORT_SPI_TX_DIRECTION) : (USER_PORT_UART_TX_DIRECTION))

    /* Internal RX and TX buffer: for SPI or UART. Used in conditional compilation check */
    #define USER_PORT_CHECK_RX_SW_BUFFER (USER_PORT_INTERNAL_RX_SW_BUFFER)
    #define USER_PORT_CHECK_TX_SW_BUFFER (USER_PORT_INTERNAL_TX_SW_BUFFER)

    /* Provide global variables to support RX and TX buffers */
    #define USER_PORT_INTERNAL_RX_SW_BUFFER_CONST    (USER_PORT_INTERNAL_RX_SW_BUFFER)
    #define USER_PORT_INTERNAL_TX_SW_BUFFER_CONST    (USER_PORT_INTERNAL_TX_SW_BUFFER)

    /* Wakeup for SPI */
    #define USER_PORT_CHECK_SPI_WAKE_ENABLE  (USER_PORT_SPI_WAKE_ENABLE_CONST)

#endif /* End (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*       Type Definitions
***************************************/

/* USER_PORT_SPI_INIT_STRUCT */
typedef struct
{
    uint32 mode;
    uint32 submode;
    uint32 sclkMode;
    uint32 oversample;
    uint32 enableMedianFilter;
    uint32 enableLateSampling;
    uint32 enableWake;
    uint32 rxDataBits;
    uint32 txDataBits;
    uint32 bitOrder;
    uint32 transferSeperation;
    uint32 rxBufferSize;
    uint8* rxBuffer;
    uint32 txBufferSize;
    uint8* txBuffer;
    uint32 enableInterrupt;
    uint32 rxInterruptMask;
    uint32 rxTriggerLevel;
    uint32 txInterruptMask;
    uint32 txTriggerLevel;
    uint8 enableByteMode;
    uint8 enableFreeRunSclk;
    uint8 polaritySs;
} USER_PORT_SPI_INIT_STRUCT;

/* USER_PORT_UART_INIT_STRUCT */
typedef struct
{
    uint32 mode;
    uint32 direction;
    uint32 dataBits;
    uint32 parity;
    uint32 stopBits;
    uint32 oversample;
    uint32 enableIrdaLowPower;
    uint32 enableMedianFilter;
    uint32 enableRetryNack;
    uint32 enableInvertedRx;
    uint32 dropOnParityErr;
    uint32 dropOnFrameErr;
    uint32 enableWake;
    uint32 rxBufferSize;
    uint8* rxBuffer;
    uint32 txBufferSize;
    uint8* txBuffer;
    uint32 enableMultiproc;
    uint32 multiprocAcceptAddr;
    uint32 multiprocAddr;
    uint32 multiprocAddrMask;
    uint32 enableInterrupt;
    uint32 rxInterruptMask;
    uint32 rxTriggerLevel;
    uint32 txInterruptMask;
    uint32 txTriggerLevel;
    uint8 enableByteMode;
    uint8 enableCts;
    uint8 ctsPolarity;
    uint8 rtsRxFifoLevel;
    uint8 rtsPolarity;
} USER_PORT_UART_INIT_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

/* SPI specific functions */
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    void USER_PORT_SpiInit(const USER_PORT_SPI_INIT_STRUCT *config);
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(USER_PORT_SCB_MODE_SPI_INC)
    #define USER_PORT_SpiIsBusBusy() ((uint32) (0u != (USER_PORT_SPI_STATUS_REG & \
                                                              USER_PORT_SPI_STATUS_BUS_BUSY)))

    #if (USER_PORT_SPI_MASTER_CONST)
        void USER_PORT_SpiSetActiveSlaveSelect(uint32 slaveSelect);
    #endif /*(USER_PORT_SPI_MASTER_CONST) */

    #if !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
        void USER_PORT_SpiSetSlaveSelectPolarity(uint32 slaveSelect, uint32 polarity);
    #endif /* !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */
#endif /* (USER_PORT_SCB_MODE_SPI_INC) */

/* UART specific functions */
#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    void USER_PORT_UartInit(const USER_PORT_UART_INIT_STRUCT *config);
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(USER_PORT_SCB_MODE_UART_INC)
    void USER_PORT_UartSetRxAddress(uint32 address);
    void USER_PORT_UartSetRxAddressMask(uint32 addressMask);

    /* UART RX direction APIs */
    #if(USER_PORT_UART_RX_DIRECTION)
        uint32 USER_PORT_UartGetChar(void);
        uint32 USER_PORT_UartGetByte(void);

        #if !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
            /* UART APIs for Flow Control */
            void USER_PORT_UartSetRtsPolarity(uint32 polarity);
            void USER_PORT_UartSetRtsFifoLevel(uint32 level);
        #endif /* !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */
    #endif /* (USER_PORT_UART_RX_DIRECTION) */

    /* UART TX direction APIs */
    #if(USER_PORT_UART_TX_DIRECTION)
        #define USER_PORT_UartPutChar(ch)    USER_PORT_SpiUartWriteTxData((uint32)(ch))
        void USER_PORT_UartPutString(const char8 string[]);
        void USER_PORT_UartPutCRLF(uint32 txDataByte);

        #if !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1)
            /* UART APIs for Flow Control */
            void USER_PORT_UartEnableCts(void);
            void USER_PORT_UartDisableCts(void);
            void USER_PORT_UartSetCtsPolarity(uint32 polarity);
        #endif /* !(USER_PORT_CY_SCBIP_V0 || USER_PORT_CY_SCBIP_V1) */
    #endif /* (USER_PORT_UART_TX_DIRECTION) */
#endif /* (USER_PORT_SCB_MODE_UART_INC) */

/* Common APIs RX direction */
#if(USER_PORT_RX_DIRECTION)
    uint32 USER_PORT_SpiUartReadRxData(void);
    uint32 USER_PORT_SpiUartGetRxBufferSize(void);
    void   USER_PORT_SpiUartClearRxBuffer(void);
#endif /* (USER_PORT_RX_DIRECTION) */

/* Common APIs TX direction */
#if(USER_PORT_TX_DIRECTION)
    void   USER_PORT_SpiUartWriteTxData(uint32 txData);
    void   USER_PORT_SpiUartPutArray(const uint8 wrBuf[], uint32 count);
    void   USER_PORT_SpiUartClearTxBuffer(void);
    uint32 USER_PORT_SpiUartGetTxBufferSize(void);
#endif /* (USER_PORT_TX_DIRECTION) */

CY_ISR_PROTO(USER_PORT_SPI_UART_ISR);

#if(USER_PORT_UART_RX_WAKEUP_IRQ)
    CY_ISR_PROTO(USER_PORT_UART_WAKEUP_ISR);
#endif /* (USER_PORT_UART_RX_WAKEUP_IRQ) */


/***************************************
*     Buffer Access Macro Definitions
***************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* RX direction */
    void   USER_PORT_PutWordInRxBuffer  (uint32 idx, uint32 rxDataByte);
    uint32 USER_PORT_GetWordFromRxBuffer(uint32 idx);

    /* TX direction */
    void   USER_PORT_PutWordInTxBuffer  (uint32 idx, uint32 txDataByte);
    uint32 USER_PORT_GetWordFromTxBuffer(uint32 idx);

#else
    /* RX direction */
    #if(USER_PORT_INTERNAL_RX_SW_BUFFER_CONST)
        #define USER_PORT_PutWordInRxBuffer(idx, rxDataByte) \
                do{                                                 \
                    USER_PORT_rxBufferInternal[(idx)] = ((uint8) (rxDataByte)); \
                }while(0)

        #define USER_PORT_GetWordFromRxBuffer(idx) USER_PORT_rxBufferInternal[(idx)]

    #endif /* (USER_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

    /* TX direction */
    #if(USER_PORT_INTERNAL_TX_SW_BUFFER_CONST)
        #define USER_PORT_PutWordInTxBuffer(idx, txDataByte) \
                    do{                                             \
                        USER_PORT_txBufferInternal[(idx)] = ((uint8) (txDataByte)); \
                    }while(0)

        #define USER_PORT_GetWordFromTxBuffer(idx) USER_PORT_txBufferInternal[(idx)]

    #endif /* (USER_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

#endif /* (USER_PORT_TX_SW_BUFFER_ENABLE) */


/***************************************
*         SPI API Constants
***************************************/

/* SPI sub mode enum */
#define USER_PORT_SPI_MODE_MOTOROLA      (0x00u)
#define USER_PORT_SPI_MODE_TI_COINCIDES  (0x01u)
#define USER_PORT_SPI_MODE_TI_PRECEDES   (0x11u)
#define USER_PORT_SPI_MODE_NATIONAL      (0x02u)
#define USER_PORT_SPI_MODE_MASK          (0x03u)
#define USER_PORT_SPI_MODE_TI_PRECEDES_MASK  (0x10u)
#define USER_PORT_SPI_MODE_NS_MICROWIRE  (USER_PORT_SPI_MODE_NATIONAL)

/* SPI phase and polarity mode enum */
#define USER_PORT_SPI_SCLK_CPHA0_CPOL0   (0x00u)
#define USER_PORT_SPI_SCLK_CPHA0_CPOL1   (0x02u)
#define USER_PORT_SPI_SCLK_CPHA1_CPOL0   (0x01u)
#define USER_PORT_SPI_SCLK_CPHA1_CPOL1   (0x03u)

/* SPI bits order enum */
#define USER_PORT_BITS_ORDER_LSB_FIRST   (0u)
#define USER_PORT_BITS_ORDER_MSB_FIRST   (1u)

/* SPI transfer separation enum */
#define USER_PORT_SPI_TRANSFER_SEPARATED     (0u)
#define USER_PORT_SPI_TRANSFER_CONTINUOUS    (1u)

/* SPI slave select constants */
#define USER_PORT_SPI_SLAVE_SELECT0    (USER_PORT_SCB__SS0_POSISTION)
#define USER_PORT_SPI_SLAVE_SELECT1    (USER_PORT_SCB__SS1_POSISTION)
#define USER_PORT_SPI_SLAVE_SELECT2    (USER_PORT_SCB__SS2_POSISTION)
#define USER_PORT_SPI_SLAVE_SELECT3    (USER_PORT_SCB__SS3_POSISTION)

/* SPI slave select polarity settings */
#define USER_PORT_SPI_SS_ACTIVE_LOW  (0u)
#define USER_PORT_SPI_SS_ACTIVE_HIGH (1u)


/***************************************
*         UART API Constants
***************************************/

/* UART sub-modes enum */
#define USER_PORT_UART_MODE_STD          (0u)
#define USER_PORT_UART_MODE_SMARTCARD    (1u)
#define USER_PORT_UART_MODE_IRDA         (2u)

/* UART direction enum */
#define USER_PORT_UART_RX    (1u)
#define USER_PORT_UART_TX    (2u)
#define USER_PORT_UART_TX_RX (3u)

/* UART parity enum */
#define USER_PORT_UART_PARITY_EVEN   (0u)
#define USER_PORT_UART_PARITY_ODD    (1u)
#define USER_PORT_UART_PARITY_NONE   (2u)

/* UART stop bits enum */
#define USER_PORT_UART_STOP_BITS_1   (2u)
#define USER_PORT_UART_STOP_BITS_1_5 (3u)
#define USER_PORT_UART_STOP_BITS_2   (4u)

/* UART IrDA low power OVS enum */
#define USER_PORT_UART_IRDA_LP_OVS16     (16u)
#define USER_PORT_UART_IRDA_LP_OVS32     (32u)
#define USER_PORT_UART_IRDA_LP_OVS48     (48u)
#define USER_PORT_UART_IRDA_LP_OVS96     (96u)
#define USER_PORT_UART_IRDA_LP_OVS192    (192u)
#define USER_PORT_UART_IRDA_LP_OVS768    (768u)
#define USER_PORT_UART_IRDA_LP_OVS1536   (1536u)

/* Uart MP: mark (address) and space (data) bit definitions */
#define USER_PORT_UART_MP_MARK       (0x100u)
#define USER_PORT_UART_MP_SPACE      (0x000u)

/* UART CTS/RTS polarity settings */
#define USER_PORT_UART_CTS_ACTIVE_LOW    (0u)
#define USER_PORT_UART_CTS_ACTIVE_HIGH   (1u)
#define USER_PORT_UART_RTS_ACTIVE_LOW    (0u)
#define USER_PORT_UART_RTS_ACTIVE_HIGH   (1u)

/* Sources of RX errors */
#define USER_PORT_INTR_RX_ERR        (USER_PORT_INTR_RX_OVERFLOW    | \
                                             USER_PORT_INTR_RX_UNDERFLOW   | \
                                             USER_PORT_INTR_RX_FRAME_ERROR | \
                                             USER_PORT_INTR_RX_PARITY_ERROR)

/* Shifted INTR_RX_ERR defines ONLY for USER_PORT_UartGetByte() */
#define USER_PORT_UART_RX_OVERFLOW       (USER_PORT_INTR_RX_OVERFLOW << 8u)
#define USER_PORT_UART_RX_UNDERFLOW      (USER_PORT_INTR_RX_UNDERFLOW << 8u)
#define USER_PORT_UART_RX_FRAME_ERROR    (USER_PORT_INTR_RX_FRAME_ERROR << 8u)
#define USER_PORT_UART_RX_PARITY_ERROR   (USER_PORT_INTR_RX_PARITY_ERROR << 8u)
#define USER_PORT_UART_RX_ERROR_MASK     (USER_PORT_UART_RX_OVERFLOW    | \
                                                 USER_PORT_UART_RX_UNDERFLOW   | \
                                                 USER_PORT_UART_RX_FRAME_ERROR | \
                                                 USER_PORT_UART_RX_PARITY_ERROR)


/***************************************
*     Vars with External Linkage
***************************************/

#if(USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    extern const USER_PORT_SPI_INIT_STRUCT  USER_PORT_configSpi;
    extern const USER_PORT_UART_INIT_STRUCT USER_PORT_configUart;
#endif /* (USER_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*    Specific SPI Macro Definitions
***************************************/

#define USER_PORT_GET_SPI_INTR_SLAVE_MASK(sourceMask)  ((sourceMask) & USER_PORT_INTR_SLAVE_SPI_BUS_ERROR)
#define USER_PORT_GET_SPI_INTR_MASTER_MASK(sourceMask) ((sourceMask) & USER_PORT_INTR_MASTER_SPI_DONE)
#define USER_PORT_GET_SPI_INTR_RX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~USER_PORT_INTR_SLAVE_SPI_BUS_ERROR)

#define USER_PORT_GET_SPI_INTR_TX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~USER_PORT_INTR_MASTER_SPI_DONE)


/***************************************
*    Specific UART Macro Definitions
***************************************/

#define USER_PORT_UART_GET_CTRL_OVS_IRDA_LP(oversample) \
        ((USER_PORT_UART_IRDA_LP_OVS16   == (oversample)) ? USER_PORT_CTRL_OVS_IRDA_LP_OVS16 : \
         ((USER_PORT_UART_IRDA_LP_OVS32   == (oversample)) ? USER_PORT_CTRL_OVS_IRDA_LP_OVS32 : \
          ((USER_PORT_UART_IRDA_LP_OVS48   == (oversample)) ? USER_PORT_CTRL_OVS_IRDA_LP_OVS48 : \
           ((USER_PORT_UART_IRDA_LP_OVS96   == (oversample)) ? USER_PORT_CTRL_OVS_IRDA_LP_OVS96 : \
            ((USER_PORT_UART_IRDA_LP_OVS192  == (oversample)) ? USER_PORT_CTRL_OVS_IRDA_LP_OVS192 : \
             ((USER_PORT_UART_IRDA_LP_OVS768  == (oversample)) ? USER_PORT_CTRL_OVS_IRDA_LP_OVS768 : \
              ((USER_PORT_UART_IRDA_LP_OVS1536 == (oversample)) ? USER_PORT_CTRL_OVS_IRDA_LP_OVS1536 : \
                                                                          USER_PORT_CTRL_OVS_IRDA_LP_OVS16)))))))

#define USER_PORT_GET_UART_RX_CTRL_ENABLED(direction) ((0u != (USER_PORT_UART_RX & (direction))) ? \
                                                                     (USER_PORT_RX_CTRL_ENABLED) : (0u))

#define USER_PORT_GET_UART_TX_CTRL_ENABLED(direction) ((0u != (USER_PORT_UART_TX & (direction))) ? \
                                                                     (USER_PORT_TX_CTRL_ENABLED) : (0u))


/***************************************
*        SPI Register Settings
***************************************/

#define USER_PORT_CTRL_SPI      (USER_PORT_CTRL_MODE_SPI)
#define USER_PORT_SPI_RX_CTRL   (USER_PORT_RX_CTRL_ENABLED)
#define USER_PORT_SPI_TX_CTRL   (USER_PORT_TX_CTRL_ENABLED)


/***************************************
*       SPI Init Register Settings
***************************************/

#define USER_PORT_SPI_SS_POLARITY \
             (((uint32) USER_PORT_SPI_SS0_POLARITY << USER_PORT_SPI_SLAVE_SELECT0) | \
              ((uint32) USER_PORT_SPI_SS1_POLARITY << USER_PORT_SPI_SLAVE_SELECT1) | \
              ((uint32) USER_PORT_SPI_SS2_POLARITY << USER_PORT_SPI_SLAVE_SELECT2) | \
              ((uint32) USER_PORT_SPI_SS3_POLARITY << USER_PORT_SPI_SLAVE_SELECT3))

#if(USER_PORT_SCB_MODE_SPI_CONST_CFG)

    /* SPI Configuration */
    #define USER_PORT_SPI_DEFAULT_CTRL \
                    (USER_PORT_GET_CTRL_OVS(USER_PORT_SPI_OVS_FACTOR) | \
                     USER_PORT_GET_CTRL_BYTE_MODE (USER_PORT_SPI_BYTE_MODE_ENABLE) | \
                     USER_PORT_GET_CTRL_EC_AM_MODE(USER_PORT_SPI_WAKE_ENABLE)      | \
                     USER_PORT_CTRL_SPI)

    #define USER_PORT_SPI_DEFAULT_SPI_CTRL \
                    (USER_PORT_GET_SPI_CTRL_CONTINUOUS    (USER_PORT_SPI_TRANSFER_SEPARATION)       | \
                     USER_PORT_GET_SPI_CTRL_SELECT_PRECEDE(USER_PORT_SPI_SUB_MODE &                   \
                                                                  USER_PORT_SPI_MODE_TI_PRECEDES_MASK)     | \
                     USER_PORT_GET_SPI_CTRL_SCLK_MODE     (USER_PORT_SPI_CLOCK_MODE)                | \
                     USER_PORT_GET_SPI_CTRL_LATE_MISO_SAMPLE(USER_PORT_SPI_LATE_MISO_SAMPLE_ENABLE) | \
                     USER_PORT_GET_SPI_CTRL_SCLK_CONTINUOUS(USER_PORT_SPI_FREE_RUN_SCLK_ENABLE)     | \
                     USER_PORT_GET_SPI_CTRL_SSEL_POLARITY (USER_PORT_SPI_SS_POLARITY)               | \
                     USER_PORT_GET_SPI_CTRL_SUB_MODE      (USER_PORT_SPI_SUB_MODE)                  | \
                     USER_PORT_GET_SPI_CTRL_MASTER_MODE   (USER_PORT_SPI_MODE))

    /* RX direction */
    #define USER_PORT_SPI_DEFAULT_RX_CTRL \
                    (USER_PORT_GET_RX_CTRL_DATA_WIDTH(USER_PORT_SPI_RX_DATA_BITS_NUM)     | \
                     USER_PORT_GET_RX_CTRL_BIT_ORDER (USER_PORT_SPI_BITS_ORDER)           | \
                     USER_PORT_GET_RX_CTRL_MEDIAN    (USER_PORT_SPI_MEDIAN_FILTER_ENABLE) | \
                     USER_PORT_SPI_RX_CTRL)

    #define USER_PORT_SPI_DEFAULT_RX_FIFO_CTRL \
                    USER_PORT_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(USER_PORT_SPI_RX_TRIGGER_LEVEL)

    /* TX direction */
    #define USER_PORT_SPI_DEFAULT_TX_CTRL \
                    (USER_PORT_GET_TX_CTRL_DATA_WIDTH(USER_PORT_SPI_TX_DATA_BITS_NUM) | \
                     USER_PORT_GET_TX_CTRL_BIT_ORDER (USER_PORT_SPI_BITS_ORDER)       | \
                     USER_PORT_SPI_TX_CTRL)

    #define USER_PORT_SPI_DEFAULT_TX_FIFO_CTRL \
                    USER_PORT_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(USER_PORT_SPI_TX_TRIGGER_LEVEL)

    /* Interrupt sources */
    #define USER_PORT_SPI_DEFAULT_INTR_SPI_EC_MASK   (USER_PORT_NO_INTR_SOURCES)

    #define USER_PORT_SPI_DEFAULT_INTR_I2C_EC_MASK   (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_SPI_DEFAULT_INTR_SLAVE_MASK \
                    (USER_PORT_SPI_INTR_RX_MASK & USER_PORT_INTR_SLAVE_SPI_BUS_ERROR)

    #define USER_PORT_SPI_DEFAULT_INTR_MASTER_MASK \
                    (USER_PORT_SPI_INTR_TX_MASK & USER_PORT_INTR_MASTER_SPI_DONE)

    #define USER_PORT_SPI_DEFAULT_INTR_RX_MASK \
                    (USER_PORT_SPI_INTR_RX_MASK & (uint32) ~USER_PORT_INTR_SLAVE_SPI_BUS_ERROR)

    #define USER_PORT_SPI_DEFAULT_INTR_TX_MASK \
                    (USER_PORT_SPI_INTR_TX_MASK & (uint32) ~USER_PORT_INTR_MASTER_SPI_DONE)

#endif /* (USER_PORT_SCB_MODE_SPI_CONST_CFG) */


/***************************************
*        UART Register Settings
***************************************/

#define USER_PORT_CTRL_UART      (USER_PORT_CTRL_MODE_UART)
#define USER_PORT_UART_RX_CTRL   (USER_PORT_RX_CTRL_LSB_FIRST) /* LSB for UART goes first */
#define USER_PORT_UART_TX_CTRL   (USER_PORT_TX_CTRL_LSB_FIRST) /* LSB for UART goes first */


/***************************************
*      UART Init Register Settings
***************************************/

#if(USER_PORT_SCB_MODE_UART_CONST_CFG)

    /* UART configuration */
    #if(USER_PORT_UART_MODE_IRDA == USER_PORT_UART_SUB_MODE)

        #define USER_PORT_DEFAULT_CTRL_OVS   ((0u != USER_PORT_UART_IRDA_LOW_POWER) ?              \
                                (USER_PORT_UART_GET_CTRL_OVS_IRDA_LP(USER_PORT_UART_OVS_FACTOR)) : \
                                (USER_PORT_CTRL_OVS_IRDA_OVS16))

    #else

        #define USER_PORT_DEFAULT_CTRL_OVS   USER_PORT_GET_CTRL_OVS(USER_PORT_UART_OVS_FACTOR)

    #endif /* (USER_PORT_UART_MODE_IRDA == USER_PORT_UART_SUB_MODE) */

    #define USER_PORT_UART_DEFAULT_CTRL \
                                (USER_PORT_GET_CTRL_BYTE_MODE  (USER_PORT_UART_BYTE_MODE_ENABLE)  | \
                                 USER_PORT_GET_CTRL_ADDR_ACCEPT(USER_PORT_UART_MP_ACCEPT_ADDRESS) | \
                                 USER_PORT_DEFAULT_CTRL_OVS                                              | \
                                 USER_PORT_CTRL_UART)

    #define USER_PORT_UART_DEFAULT_UART_CTRL \
                                    (USER_PORT_GET_UART_CTRL_MODE(USER_PORT_UART_SUB_MODE))

    /* RX direction */
    #define USER_PORT_UART_DEFAULT_RX_CTRL_PARITY \
                                ((USER_PORT_UART_PARITY_NONE != USER_PORT_UART_PARITY_TYPE) ?      \
                                  (USER_PORT_GET_UART_RX_CTRL_PARITY(USER_PORT_UART_PARITY_TYPE) | \
                                   USER_PORT_UART_RX_CTRL_PARITY_ENABLED) : (0u))

    #define USER_PORT_UART_DEFAULT_UART_RX_CTRL \
                    (USER_PORT_GET_UART_RX_CTRL_MODE(USER_PORT_UART_STOP_BITS_NUM)                    | \
                     USER_PORT_GET_UART_RX_CTRL_POLARITY(USER_PORT_UART_IRDA_POLARITY)                | \
                     USER_PORT_GET_UART_RX_CTRL_MP_MODE(USER_PORT_UART_MP_MODE_ENABLE)                | \
                     USER_PORT_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(USER_PORT_UART_DROP_ON_PARITY_ERR) | \
                     USER_PORT_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(USER_PORT_UART_DROP_ON_FRAME_ERR)   | \
                     USER_PORT_UART_DEFAULT_RX_CTRL_PARITY)

    #define USER_PORT_UART_DEFAULT_RX_CTRL \
                                (USER_PORT_GET_RX_CTRL_DATA_WIDTH(USER_PORT_UART_DATA_BITS_NUM)        | \
                                 USER_PORT_GET_RX_CTRL_MEDIAN    (USER_PORT_UART_MEDIAN_FILTER_ENABLE) | \
                                 USER_PORT_GET_UART_RX_CTRL_ENABLED(USER_PORT_UART_DIRECTION))

    #define USER_PORT_UART_DEFAULT_RX_FIFO_CTRL \
                                USER_PORT_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(USER_PORT_UART_RX_TRIGGER_LEVEL)

    #define USER_PORT_UART_DEFAULT_RX_MATCH_REG  ((0u != USER_PORT_UART_MP_MODE_ENABLE) ?          \
                                (USER_PORT_GET_RX_MATCH_ADDR(USER_PORT_UART_MP_RX_ADDRESS) | \
                                 USER_PORT_GET_RX_MATCH_MASK(USER_PORT_UART_MP_RX_ADDRESS_MASK)) : (0u))

    /* TX direction */
    #define USER_PORT_UART_DEFAULT_TX_CTRL_PARITY (USER_PORT_UART_DEFAULT_RX_CTRL_PARITY)

    #define USER_PORT_UART_DEFAULT_UART_TX_CTRL \
                                (USER_PORT_GET_UART_TX_CTRL_MODE(USER_PORT_UART_STOP_BITS_NUM)       | \
                                 USER_PORT_GET_UART_TX_CTRL_RETRY_NACK(USER_PORT_UART_RETRY_ON_NACK) | \
                                 USER_PORT_UART_DEFAULT_TX_CTRL_PARITY)

    #define USER_PORT_UART_DEFAULT_TX_CTRL \
                                (USER_PORT_GET_TX_CTRL_DATA_WIDTH(USER_PORT_UART_DATA_BITS_NUM) | \
                                 USER_PORT_GET_UART_TX_CTRL_ENABLED(USER_PORT_UART_DIRECTION))

    #define USER_PORT_UART_DEFAULT_TX_FIFO_CTRL \
                                USER_PORT_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(USER_PORT_UART_TX_TRIGGER_LEVEL)

    #define USER_PORT_UART_DEFAULT_FLOW_CTRL \
                        (USER_PORT_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(USER_PORT_UART_RTS_FIFO_LEVEL) | \
                         USER_PORT_GET_UART_FLOW_CTRL_RTS_POLARITY (USER_PORT_UART_RTS_POLARITY)   | \
                         USER_PORT_GET_UART_FLOW_CTRL_CTS_POLARITY (USER_PORT_UART_CTS_POLARITY)   | \
                         USER_PORT_GET_UART_FLOW_CTRL_CTS_ENABLE   (USER_PORT_UART_CTS_ENABLE))

    /* Interrupt sources */
    #define USER_PORT_UART_DEFAULT_INTR_I2C_EC_MASK  (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_UART_DEFAULT_INTR_SPI_EC_MASK  (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_UART_DEFAULT_INTR_SLAVE_MASK   (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_UART_DEFAULT_INTR_MASTER_MASK  (USER_PORT_NO_INTR_SOURCES)
    #define USER_PORT_UART_DEFAULT_INTR_RX_MASK      (USER_PORT_UART_INTR_RX_MASK)
    #define USER_PORT_UART_DEFAULT_INTR_TX_MASK      (USER_PORT_UART_INTR_TX_MASK)

#endif /* (USER_PORT_SCB_MODE_UART_CONST_CFG) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

#define USER_PORT_SPIM_ACTIVE_SS0    (USER_PORT_SPI_SLAVE_SELECT0)
#define USER_PORT_SPIM_ACTIVE_SS1    (USER_PORT_SPI_SLAVE_SELECT1)
#define USER_PORT_SPIM_ACTIVE_SS2    (USER_PORT_SPI_SLAVE_SELECT2)
#define USER_PORT_SPIM_ACTIVE_SS3    (USER_PORT_SPI_SLAVE_SELECT3)

#endif /* CY_SCB_SPI_UART_USER_PORT_H */


/* [] END OF FILE */
