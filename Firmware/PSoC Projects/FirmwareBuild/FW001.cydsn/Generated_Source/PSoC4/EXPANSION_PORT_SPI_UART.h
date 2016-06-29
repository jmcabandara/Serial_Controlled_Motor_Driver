/*******************************************************************************
* File Name: EXPANSION_PORT_SPI_UART.h
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

#if !defined(CY_SCB_SPI_UART_EXPANSION_PORT_H)
#define CY_SCB_SPI_UART_EXPANSION_PORT_H

#include "EXPANSION_PORT.h"


/***************************************
*   SPI Initial Parameter Constants
****************************************/

#define EXPANSION_PORT_SPI_MODE                   (0u)
#define EXPANSION_PORT_SPI_SUB_MODE               (0u)
#define EXPANSION_PORT_SPI_CLOCK_MODE             (0u)
#define EXPANSION_PORT_SPI_OVS_FACTOR             (16u)
#define EXPANSION_PORT_SPI_MEDIAN_FILTER_ENABLE   (0u)
#define EXPANSION_PORT_SPI_LATE_MISO_SAMPLE_ENABLE (0u)
#define EXPANSION_PORT_SPI_RX_DATA_BITS_NUM       (8u)
#define EXPANSION_PORT_SPI_TX_DATA_BITS_NUM       (8u)
#define EXPANSION_PORT_SPI_WAKE_ENABLE            (0u)
#define EXPANSION_PORT_SPI_BITS_ORDER             (1u)
#define EXPANSION_PORT_SPI_TRANSFER_SEPARATION    (1u)
#define EXPANSION_PORT_SPI_NUMBER_OF_SS_LINES     (1u)
#define EXPANSION_PORT_SPI_RX_BUFFER_SIZE         (8u)
#define EXPANSION_PORT_SPI_TX_BUFFER_SIZE         (8u)

#define EXPANSION_PORT_SPI_INTERRUPT_MODE         (0u)

#define EXPANSION_PORT_SPI_INTR_RX_MASK           (0u)
#define EXPANSION_PORT_SPI_INTR_TX_MASK           (0u)

#define EXPANSION_PORT_SPI_RX_TRIGGER_LEVEL       (7u)
#define EXPANSION_PORT_SPI_TX_TRIGGER_LEVEL       (0u)

#define EXPANSION_PORT_SPI_BYTE_MODE_ENABLE       (0u)
#define EXPANSION_PORT_SPI_FREE_RUN_SCLK_ENABLE   (0u)
#define EXPANSION_PORT_SPI_SS0_POLARITY           (0u)
#define EXPANSION_PORT_SPI_SS1_POLARITY           (0u)
#define EXPANSION_PORT_SPI_SS2_POLARITY           (0u)
#define EXPANSION_PORT_SPI_SS3_POLARITY           (0u)


/***************************************
*   UART Initial Parameter Constants
****************************************/

#define EXPANSION_PORT_UART_SUB_MODE              (0u)
#define EXPANSION_PORT_UART_DIRECTION             (3u)
#define EXPANSION_PORT_UART_DATA_BITS_NUM         (8u)
#define EXPANSION_PORT_UART_PARITY_TYPE           (2u)
#define EXPANSION_PORT_UART_STOP_BITS_NUM         (2u)
#define EXPANSION_PORT_UART_OVS_FACTOR            (12u)
#define EXPANSION_PORT_UART_IRDA_LOW_POWER        (0u)
#define EXPANSION_PORT_UART_MEDIAN_FILTER_ENABLE  (0u)
#define EXPANSION_PORT_UART_RETRY_ON_NACK         (0u)
#define EXPANSION_PORT_UART_IRDA_POLARITY         (0u)
#define EXPANSION_PORT_UART_DROP_ON_FRAME_ERR     (0u)
#define EXPANSION_PORT_UART_DROP_ON_PARITY_ERR    (0u)
#define EXPANSION_PORT_UART_WAKE_ENABLE           (0u)
#define EXPANSION_PORT_UART_RX_BUFFER_SIZE        (8u)
#define EXPANSION_PORT_UART_TX_BUFFER_SIZE        (8u)
#define EXPANSION_PORT_UART_MP_MODE_ENABLE        (0u)
#define EXPANSION_PORT_UART_MP_ACCEPT_ADDRESS     (0u)
#define EXPANSION_PORT_UART_MP_RX_ADDRESS         (2u)
#define EXPANSION_PORT_UART_MP_RX_ADDRESS_MASK    (255u)

#define EXPANSION_PORT_UART_INTERRUPT_MODE        (2u)

#define EXPANSION_PORT_UART_INTR_RX_MASK          (0u)
#define EXPANSION_PORT_UART_INTR_TX_MASK          (0u)

#define EXPANSION_PORT_UART_RX_TRIGGER_LEVEL      (7u)
#define EXPANSION_PORT_UART_TX_TRIGGER_LEVEL      (0u)

#define EXPANSION_PORT_UART_BYTE_MODE_ENABLE      (0u)
#define EXPANSION_PORT_UART_CTS_ENABLE            (0u)
#define EXPANSION_PORT_UART_CTS_POLARITY          (0u)
#define EXPANSION_PORT_UART_RTS_POLARITY          (0u)
#define EXPANSION_PORT_UART_RTS_FIFO_LEVEL        (4u)

/* SPI mode enum */
#define EXPANSION_PORT_SPI_SLAVE  (0u)
#define EXPANSION_PORT_SPI_MASTER (1u)

/* UART direction enum */
#define EXPANSION_PORT_UART_RX    (1u)
#define EXPANSION_PORT_UART_TX    (2u)
#define EXPANSION_PORT_UART_TX_RX (3u)


/***************************************
*   Conditional Compilation Parameters
****************************************/

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)

    /* Mode */
    #define EXPANSION_PORT_SPI_MASTER_CONST       (1u)

    /* Direction */
    #define EXPANSION_PORT_RX_DIRECTION           (1u)
    #define EXPANSION_PORT_TX_DIRECTION           (1u)
    #define EXPANSION_PORT_UART_RX_DIRECTION      (1u)
    #define EXPANSION_PORT_UART_TX_DIRECTION      (1u)

    /* Only external RX and TX buffer for Uncofigured mode */
    #define EXPANSION_PORT_INTERNAL_RX_SW_BUFFER   (0u)
    #define EXPANSION_PORT_INTERNAL_TX_SW_BUFFER   (0u)

    /* Get RX and TX buffer size */
    #define EXPANSION_PORT_RX_BUFFER_SIZE (EXPANSION_PORT_rxBufferSize)
    #define EXPANSION_PORT_TX_BUFFER_SIZE (EXPANSION_PORT_txBufferSize)

    /* Return true if buffer is provided */
    #define EXPANSION_PORT_CHECK_RX_SW_BUFFER (NULL != EXPANSION_PORT_rxBuffer)
    #define EXPANSION_PORT_CHECK_TX_SW_BUFFER (NULL != EXPANSION_PORT_txBuffer)

    /* Always provide global variables to support RX and TX buffers */
    #define EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST    (1u)
    #define EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST    (1u)

    /* Get wakeup enable option */
    #define EXPANSION_PORT_SPI_WAKE_ENABLE_CONST  (1u)
    #define EXPANSION_PORT_CHECK_SPI_WAKE_ENABLE  (0u != EXPANSION_PORT_scbEnableWake)
    #define EXPANSION_PORT_UART_WAKE_ENABLE_CONST (1u)

    /* SPI/UART: TX or RX FIFO size */
    #if (EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
        #define EXPANSION_PORT_SPI_UART_FIFO_SIZE (EXPANSION_PORT_FIFO_SIZE)
    #else
        #define EXPANSION_PORT_SPI_UART_FIFO_SIZE (EXPANSION_PORT_GET_FIFO_SIZE(EXPANSION_PORT_CTRL_REG & \
                                                                                    EXPANSION_PORT_CTRL_BYTE_MODE))
    #endif /* (EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */

#else

    /* Internal RX and TX buffer: for SPI or UART */
    #if (EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG)

        /* SPI Direction */
        #define EXPANSION_PORT_SPI_RX_DIRECTION (1u)
        #define EXPANSION_PORT_SPI_TX_DIRECTION (1u)

        /* Get FIFO size */
        #if (EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
            #define EXPANSION_PORT_SPI_UART_FIFO_SIZE    (EXPANSION_PORT_FIFO_SIZE)
        #else
            #define EXPANSION_PORT_SPI_UART_FIFO_SIZE \
                                           EXPANSION_PORT_GET_FIFO_SIZE(EXPANSION_PORT_SPI_BYTE_MODE_ENABLE)

        #endif /* (EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */

        /* SPI internal RX and TX buffers */
        #define EXPANSION_PORT_INTERNAL_SPI_RX_SW_BUFFER  (EXPANSION_PORT_SPI_RX_BUFFER_SIZE > \
                                                                EXPANSION_PORT_SPI_UART_FIFO_SIZE)
        #define EXPANSION_PORT_INTERNAL_SPI_TX_SW_BUFFER  (EXPANSION_PORT_SPI_TX_BUFFER_SIZE > \
                                                                EXPANSION_PORT_SPI_UART_FIFO_SIZE)

        /* Internal SPI RX and TX buffer */
        #define EXPANSION_PORT_INTERNAL_RX_SW_BUFFER  (EXPANSION_PORT_INTERNAL_SPI_RX_SW_BUFFER)
        #define EXPANSION_PORT_INTERNAL_TX_SW_BUFFER  (EXPANSION_PORT_INTERNAL_SPI_TX_SW_BUFFER)

        /* Internal SPI RX and TX buffer size */
        #define EXPANSION_PORT_RX_BUFFER_SIZE         (EXPANSION_PORT_SPI_RX_BUFFER_SIZE + 1u)
        #define EXPANSION_PORT_TX_BUFFER_SIZE         (EXPANSION_PORT_SPI_TX_BUFFER_SIZE)

        /* Get wakeup enable option */
        #define EXPANSION_PORT_SPI_WAKE_ENABLE_CONST  (0u != EXPANSION_PORT_SPI_WAKE_ENABLE)
        #define EXPANSION_PORT_UART_WAKE_ENABLE_CONST (0u)

    #else

        /* UART Direction */
        #define EXPANSION_PORT_UART_RX_DIRECTION (0u != (EXPANSION_PORT_UART_DIRECTION & EXPANSION_PORT_UART_RX))
        #define EXPANSION_PORT_UART_TX_DIRECTION (0u != (EXPANSION_PORT_UART_DIRECTION & EXPANSION_PORT_UART_TX))

        /* Get FIFO size */
        #if (EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
            #define EXPANSION_PORT_SPI_UART_FIFO_SIZE    (EXPANSION_PORT_FIFO_SIZE)
        #else
            #define EXPANSION_PORT_SPI_UART_FIFO_SIZE \
                                           EXPANSION_PORT_GET_FIFO_SIZE(EXPANSION_PORT_UART_BYTE_MODE_ENABLE)
        #endif /* (EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */

        /* UART internal RX and TX buffers */
        #define EXPANSION_PORT_INTERNAL_UART_RX_SW_BUFFER  (EXPANSION_PORT_UART_RX_BUFFER_SIZE > \
                                                                EXPANSION_PORT_SPI_UART_FIFO_SIZE)
        #define EXPANSION_PORT_INTERNAL_UART_TX_SW_BUFFER  (EXPANSION_PORT_UART_TX_BUFFER_SIZE > \
                                                                    EXPANSION_PORT_SPI_UART_FIFO_SIZE)

        /* Internal UART RX and TX buffer */
        #define EXPANSION_PORT_INTERNAL_RX_SW_BUFFER  (EXPANSION_PORT_INTERNAL_UART_RX_SW_BUFFER)
        #define EXPANSION_PORT_INTERNAL_TX_SW_BUFFER  (EXPANSION_PORT_INTERNAL_UART_TX_SW_BUFFER)

        /* Internal UART RX and TX buffer size */
        #define EXPANSION_PORT_RX_BUFFER_SIZE         (EXPANSION_PORT_UART_RX_BUFFER_SIZE + 1u)
        #define EXPANSION_PORT_TX_BUFFER_SIZE         (EXPANSION_PORT_UART_TX_BUFFER_SIZE)

        /* Get wakeup enable option */
        #define EXPANSION_PORT_SPI_WAKE_ENABLE_CONST  (0u)
        #define EXPANSION_PORT_UART_WAKE_ENABLE_CONST (0u != EXPANSION_PORT_UART_WAKE_ENABLE)

    #endif /* (EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG) */

    /* Mode */
    #define EXPANSION_PORT_SPI_MASTER_CONST   (EXPANSION_PORT_SPI_MODE == EXPANSION_PORT_SPI_MASTER)

    /* Direction */
    #define EXPANSION_PORT_RX_DIRECTION ((EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG) ? \
                                            (EXPANSION_PORT_SPI_RX_DIRECTION) : (EXPANSION_PORT_UART_RX_DIRECTION))

    #define EXPANSION_PORT_TX_DIRECTION ((EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG) ? \
                                            (EXPANSION_PORT_SPI_TX_DIRECTION) : (EXPANSION_PORT_UART_TX_DIRECTION))

    /* Internal RX and TX buffer: for SPI or UART. Used in conditional compilation check */
    #define EXPANSION_PORT_CHECK_RX_SW_BUFFER (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER)
    #define EXPANSION_PORT_CHECK_TX_SW_BUFFER (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER)

    /* Provide global variables to support RX and TX buffers */
    #define EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST    (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER)
    #define EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST    (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER)

    /* Wakeup for SPI */
    #define EXPANSION_PORT_CHECK_SPI_WAKE_ENABLE  (EXPANSION_PORT_SPI_WAKE_ENABLE_CONST)

#endif /* End (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*       Type Definitions
***************************************/

/* EXPANSION_PORT_SPI_INIT_STRUCT */
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
} EXPANSION_PORT_SPI_INIT_STRUCT;

/* EXPANSION_PORT_UART_INIT_STRUCT */
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
} EXPANSION_PORT_UART_INIT_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

/* SPI specific functions */
#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    void EXPANSION_PORT_SpiInit(const EXPANSION_PORT_SPI_INIT_STRUCT *config);
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(EXPANSION_PORT_SCB_MODE_SPI_INC)
    #define EXPANSION_PORT_SpiIsBusBusy() ((uint32) (0u != (EXPANSION_PORT_SPI_STATUS_REG & \
                                                              EXPANSION_PORT_SPI_STATUS_BUS_BUSY)))

    #if (EXPANSION_PORT_SPI_MASTER_CONST)
        void EXPANSION_PORT_SpiSetActiveSlaveSelect(uint32 slaveSelect);
    #endif /*(EXPANSION_PORT_SPI_MASTER_CONST) */

    #if !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
        void EXPANSION_PORT_SpiSetSlaveSelectPolarity(uint32 slaveSelect, uint32 polarity);
    #endif /* !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */
#endif /* (EXPANSION_PORT_SCB_MODE_SPI_INC) */

/* UART specific functions */
#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    void EXPANSION_PORT_UartInit(const EXPANSION_PORT_UART_INIT_STRUCT *config);
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(EXPANSION_PORT_SCB_MODE_UART_INC)
    void EXPANSION_PORT_UartSetRxAddress(uint32 address);
    void EXPANSION_PORT_UartSetRxAddressMask(uint32 addressMask);

    /* UART RX direction APIs */
    #if(EXPANSION_PORT_UART_RX_DIRECTION)
        uint32 EXPANSION_PORT_UartGetChar(void);
        uint32 EXPANSION_PORT_UartGetByte(void);

        #if !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
            /* UART APIs for Flow Control */
            void EXPANSION_PORT_UartSetRtsPolarity(uint32 polarity);
            void EXPANSION_PORT_UartSetRtsFifoLevel(uint32 level);
        #endif /* !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */
    #endif /* (EXPANSION_PORT_UART_RX_DIRECTION) */

    /* UART TX direction APIs */
    #if(EXPANSION_PORT_UART_TX_DIRECTION)
        #define EXPANSION_PORT_UartPutChar(ch)    EXPANSION_PORT_SpiUartWriteTxData((uint32)(ch))
        void EXPANSION_PORT_UartPutString(const char8 string[]);
        void EXPANSION_PORT_UartPutCRLF(uint32 txDataByte);

        #if !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1)
            /* UART APIs for Flow Control */
            void EXPANSION_PORT_UartEnableCts(void);
            void EXPANSION_PORT_UartDisableCts(void);
            void EXPANSION_PORT_UartSetCtsPolarity(uint32 polarity);
        #endif /* !(EXPANSION_PORT_CY_SCBIP_V0 || EXPANSION_PORT_CY_SCBIP_V1) */
    #endif /* (EXPANSION_PORT_UART_TX_DIRECTION) */
#endif /* (EXPANSION_PORT_SCB_MODE_UART_INC) */

/* Common APIs RX direction */
#if(EXPANSION_PORT_RX_DIRECTION)
    uint32 EXPANSION_PORT_SpiUartReadRxData(void);
    uint32 EXPANSION_PORT_SpiUartGetRxBufferSize(void);
    void   EXPANSION_PORT_SpiUartClearRxBuffer(void);
#endif /* (EXPANSION_PORT_RX_DIRECTION) */

/* Common APIs TX direction */
#if(EXPANSION_PORT_TX_DIRECTION)
    void   EXPANSION_PORT_SpiUartWriteTxData(uint32 txData);
    void   EXPANSION_PORT_SpiUartPutArray(const uint8 wrBuf[], uint32 count);
    void   EXPANSION_PORT_SpiUartClearTxBuffer(void);
    uint32 EXPANSION_PORT_SpiUartGetTxBufferSize(void);
#endif /* (EXPANSION_PORT_TX_DIRECTION) */

CY_ISR_PROTO(EXPANSION_PORT_SPI_UART_ISR);

#if(EXPANSION_PORT_UART_RX_WAKEUP_IRQ)
    CY_ISR_PROTO(EXPANSION_PORT_UART_WAKEUP_ISR);
#endif /* (EXPANSION_PORT_UART_RX_WAKEUP_IRQ) */


/***************************************
*     Buffer Access Macro Definitions
***************************************/

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    /* RX direction */
    void   EXPANSION_PORT_PutWordInRxBuffer  (uint32 idx, uint32 rxDataByte);
    uint32 EXPANSION_PORT_GetWordFromRxBuffer(uint32 idx);

    /* TX direction */
    void   EXPANSION_PORT_PutWordInTxBuffer  (uint32 idx, uint32 txDataByte);
    uint32 EXPANSION_PORT_GetWordFromTxBuffer(uint32 idx);

#else
    /* RX direction */
    #if(EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST)
        #define EXPANSION_PORT_PutWordInRxBuffer(idx, rxDataByte) \
                do{                                                 \
                    EXPANSION_PORT_rxBufferInternal[(idx)] = ((uint8) (rxDataByte)); \
                }while(0)

        #define EXPANSION_PORT_GetWordFromRxBuffer(idx) EXPANSION_PORT_rxBufferInternal[(idx)]

    #endif /* (EXPANSION_PORT_INTERNAL_RX_SW_BUFFER_CONST) */

    /* TX direction */
    #if(EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST)
        #define EXPANSION_PORT_PutWordInTxBuffer(idx, txDataByte) \
                    do{                                             \
                        EXPANSION_PORT_txBufferInternal[(idx)] = ((uint8) (txDataByte)); \
                    }while(0)

        #define EXPANSION_PORT_GetWordFromTxBuffer(idx) EXPANSION_PORT_txBufferInternal[(idx)]

    #endif /* (EXPANSION_PORT_INTERNAL_TX_SW_BUFFER_CONST) */

#endif /* (EXPANSION_PORT_TX_SW_BUFFER_ENABLE) */


/***************************************
*         SPI API Constants
***************************************/

/* SPI sub mode enum */
#define EXPANSION_PORT_SPI_MODE_MOTOROLA      (0x00u)
#define EXPANSION_PORT_SPI_MODE_TI_COINCIDES  (0x01u)
#define EXPANSION_PORT_SPI_MODE_TI_PRECEDES   (0x11u)
#define EXPANSION_PORT_SPI_MODE_NATIONAL      (0x02u)
#define EXPANSION_PORT_SPI_MODE_MASK          (0x03u)
#define EXPANSION_PORT_SPI_MODE_TI_PRECEDES_MASK  (0x10u)
#define EXPANSION_PORT_SPI_MODE_NS_MICROWIRE  (EXPANSION_PORT_SPI_MODE_NATIONAL)

/* SPI phase and polarity mode enum */
#define EXPANSION_PORT_SPI_SCLK_CPHA0_CPOL0   (0x00u)
#define EXPANSION_PORT_SPI_SCLK_CPHA0_CPOL1   (0x02u)
#define EXPANSION_PORT_SPI_SCLK_CPHA1_CPOL0   (0x01u)
#define EXPANSION_PORT_SPI_SCLK_CPHA1_CPOL1   (0x03u)

/* SPI bits order enum */
#define EXPANSION_PORT_BITS_ORDER_LSB_FIRST   (0u)
#define EXPANSION_PORT_BITS_ORDER_MSB_FIRST   (1u)

/* SPI transfer separation enum */
#define EXPANSION_PORT_SPI_TRANSFER_SEPARATED     (0u)
#define EXPANSION_PORT_SPI_TRANSFER_CONTINUOUS    (1u)

/* SPI slave select constants */
#define EXPANSION_PORT_SPI_SLAVE_SELECT0    (EXPANSION_PORT_SCB__SS0_POSISTION)
#define EXPANSION_PORT_SPI_SLAVE_SELECT1    (EXPANSION_PORT_SCB__SS1_POSISTION)
#define EXPANSION_PORT_SPI_SLAVE_SELECT2    (EXPANSION_PORT_SCB__SS2_POSISTION)
#define EXPANSION_PORT_SPI_SLAVE_SELECT3    (EXPANSION_PORT_SCB__SS3_POSISTION)

/* SPI slave select polarity settings */
#define EXPANSION_PORT_SPI_SS_ACTIVE_LOW  (0u)
#define EXPANSION_PORT_SPI_SS_ACTIVE_HIGH (1u)


/***************************************
*         UART API Constants
***************************************/

/* UART sub-modes enum */
#define EXPANSION_PORT_UART_MODE_STD          (0u)
#define EXPANSION_PORT_UART_MODE_SMARTCARD    (1u)
#define EXPANSION_PORT_UART_MODE_IRDA         (2u)

/* UART direction enum */
#define EXPANSION_PORT_UART_RX    (1u)
#define EXPANSION_PORT_UART_TX    (2u)
#define EXPANSION_PORT_UART_TX_RX (3u)

/* UART parity enum */
#define EXPANSION_PORT_UART_PARITY_EVEN   (0u)
#define EXPANSION_PORT_UART_PARITY_ODD    (1u)
#define EXPANSION_PORT_UART_PARITY_NONE   (2u)

/* UART stop bits enum */
#define EXPANSION_PORT_UART_STOP_BITS_1   (2u)
#define EXPANSION_PORT_UART_STOP_BITS_1_5 (3u)
#define EXPANSION_PORT_UART_STOP_BITS_2   (4u)

/* UART IrDA low power OVS enum */
#define EXPANSION_PORT_UART_IRDA_LP_OVS16     (16u)
#define EXPANSION_PORT_UART_IRDA_LP_OVS32     (32u)
#define EXPANSION_PORT_UART_IRDA_LP_OVS48     (48u)
#define EXPANSION_PORT_UART_IRDA_LP_OVS96     (96u)
#define EXPANSION_PORT_UART_IRDA_LP_OVS192    (192u)
#define EXPANSION_PORT_UART_IRDA_LP_OVS768    (768u)
#define EXPANSION_PORT_UART_IRDA_LP_OVS1536   (1536u)

/* Uart MP: mark (address) and space (data) bit definitions */
#define EXPANSION_PORT_UART_MP_MARK       (0x100u)
#define EXPANSION_PORT_UART_MP_SPACE      (0x000u)

/* UART CTS/RTS polarity settings */
#define EXPANSION_PORT_UART_CTS_ACTIVE_LOW    (0u)
#define EXPANSION_PORT_UART_CTS_ACTIVE_HIGH   (1u)
#define EXPANSION_PORT_UART_RTS_ACTIVE_LOW    (0u)
#define EXPANSION_PORT_UART_RTS_ACTIVE_HIGH   (1u)

/* Sources of RX errors */
#define EXPANSION_PORT_INTR_RX_ERR        (EXPANSION_PORT_INTR_RX_OVERFLOW    | \
                                             EXPANSION_PORT_INTR_RX_UNDERFLOW   | \
                                             EXPANSION_PORT_INTR_RX_FRAME_ERROR | \
                                             EXPANSION_PORT_INTR_RX_PARITY_ERROR)

/* Shifted INTR_RX_ERR defines ONLY for EXPANSION_PORT_UartGetByte() */
#define EXPANSION_PORT_UART_RX_OVERFLOW       (EXPANSION_PORT_INTR_RX_OVERFLOW << 8u)
#define EXPANSION_PORT_UART_RX_UNDERFLOW      (EXPANSION_PORT_INTR_RX_UNDERFLOW << 8u)
#define EXPANSION_PORT_UART_RX_FRAME_ERROR    (EXPANSION_PORT_INTR_RX_FRAME_ERROR << 8u)
#define EXPANSION_PORT_UART_RX_PARITY_ERROR   (EXPANSION_PORT_INTR_RX_PARITY_ERROR << 8u)
#define EXPANSION_PORT_UART_RX_ERROR_MASK     (EXPANSION_PORT_UART_RX_OVERFLOW    | \
                                                 EXPANSION_PORT_UART_RX_UNDERFLOW   | \
                                                 EXPANSION_PORT_UART_RX_FRAME_ERROR | \
                                                 EXPANSION_PORT_UART_RX_PARITY_ERROR)


/***************************************
*     Vars with External Linkage
***************************************/

#if(EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG)
    extern const EXPANSION_PORT_SPI_INIT_STRUCT  EXPANSION_PORT_configSpi;
    extern const EXPANSION_PORT_UART_INIT_STRUCT EXPANSION_PORT_configUart;
#endif /* (EXPANSION_PORT_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*    Specific SPI Macro Definitions
***************************************/

#define EXPANSION_PORT_GET_SPI_INTR_SLAVE_MASK(sourceMask)  ((sourceMask) & EXPANSION_PORT_INTR_SLAVE_SPI_BUS_ERROR)
#define EXPANSION_PORT_GET_SPI_INTR_MASTER_MASK(sourceMask) ((sourceMask) & EXPANSION_PORT_INTR_MASTER_SPI_DONE)
#define EXPANSION_PORT_GET_SPI_INTR_RX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~EXPANSION_PORT_INTR_SLAVE_SPI_BUS_ERROR)

#define EXPANSION_PORT_GET_SPI_INTR_TX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~EXPANSION_PORT_INTR_MASTER_SPI_DONE)


/***************************************
*    Specific UART Macro Definitions
***************************************/

#define EXPANSION_PORT_UART_GET_CTRL_OVS_IRDA_LP(oversample) \
        ((EXPANSION_PORT_UART_IRDA_LP_OVS16   == (oversample)) ? EXPANSION_PORT_CTRL_OVS_IRDA_LP_OVS16 : \
         ((EXPANSION_PORT_UART_IRDA_LP_OVS32   == (oversample)) ? EXPANSION_PORT_CTRL_OVS_IRDA_LP_OVS32 : \
          ((EXPANSION_PORT_UART_IRDA_LP_OVS48   == (oversample)) ? EXPANSION_PORT_CTRL_OVS_IRDA_LP_OVS48 : \
           ((EXPANSION_PORT_UART_IRDA_LP_OVS96   == (oversample)) ? EXPANSION_PORT_CTRL_OVS_IRDA_LP_OVS96 : \
            ((EXPANSION_PORT_UART_IRDA_LP_OVS192  == (oversample)) ? EXPANSION_PORT_CTRL_OVS_IRDA_LP_OVS192 : \
             ((EXPANSION_PORT_UART_IRDA_LP_OVS768  == (oversample)) ? EXPANSION_PORT_CTRL_OVS_IRDA_LP_OVS768 : \
              ((EXPANSION_PORT_UART_IRDA_LP_OVS1536 == (oversample)) ? EXPANSION_PORT_CTRL_OVS_IRDA_LP_OVS1536 : \
                                                                          EXPANSION_PORT_CTRL_OVS_IRDA_LP_OVS16)))))))

#define EXPANSION_PORT_GET_UART_RX_CTRL_ENABLED(direction) ((0u != (EXPANSION_PORT_UART_RX & (direction))) ? \
                                                                     (EXPANSION_PORT_RX_CTRL_ENABLED) : (0u))

#define EXPANSION_PORT_GET_UART_TX_CTRL_ENABLED(direction) ((0u != (EXPANSION_PORT_UART_TX & (direction))) ? \
                                                                     (EXPANSION_PORT_TX_CTRL_ENABLED) : (0u))


/***************************************
*        SPI Register Settings
***************************************/

#define EXPANSION_PORT_CTRL_SPI      (EXPANSION_PORT_CTRL_MODE_SPI)
#define EXPANSION_PORT_SPI_RX_CTRL   (EXPANSION_PORT_RX_CTRL_ENABLED)
#define EXPANSION_PORT_SPI_TX_CTRL   (EXPANSION_PORT_TX_CTRL_ENABLED)


/***************************************
*       SPI Init Register Settings
***************************************/

#define EXPANSION_PORT_SPI_SS_POLARITY \
             (((uint32) EXPANSION_PORT_SPI_SS0_POLARITY << EXPANSION_PORT_SPI_SLAVE_SELECT0) | \
              ((uint32) EXPANSION_PORT_SPI_SS1_POLARITY << EXPANSION_PORT_SPI_SLAVE_SELECT1) | \
              ((uint32) EXPANSION_PORT_SPI_SS2_POLARITY << EXPANSION_PORT_SPI_SLAVE_SELECT2) | \
              ((uint32) EXPANSION_PORT_SPI_SS3_POLARITY << EXPANSION_PORT_SPI_SLAVE_SELECT3))

#if(EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG)

    /* SPI Configuration */
    #define EXPANSION_PORT_SPI_DEFAULT_CTRL \
                    (EXPANSION_PORT_GET_CTRL_OVS(EXPANSION_PORT_SPI_OVS_FACTOR) | \
                     EXPANSION_PORT_GET_CTRL_BYTE_MODE (EXPANSION_PORT_SPI_BYTE_MODE_ENABLE) | \
                     EXPANSION_PORT_GET_CTRL_EC_AM_MODE(EXPANSION_PORT_SPI_WAKE_ENABLE)      | \
                     EXPANSION_PORT_CTRL_SPI)

    #define EXPANSION_PORT_SPI_DEFAULT_SPI_CTRL \
                    (EXPANSION_PORT_GET_SPI_CTRL_CONTINUOUS    (EXPANSION_PORT_SPI_TRANSFER_SEPARATION)       | \
                     EXPANSION_PORT_GET_SPI_CTRL_SELECT_PRECEDE(EXPANSION_PORT_SPI_SUB_MODE &                   \
                                                                  EXPANSION_PORT_SPI_MODE_TI_PRECEDES_MASK)     | \
                     EXPANSION_PORT_GET_SPI_CTRL_SCLK_MODE     (EXPANSION_PORT_SPI_CLOCK_MODE)                | \
                     EXPANSION_PORT_GET_SPI_CTRL_LATE_MISO_SAMPLE(EXPANSION_PORT_SPI_LATE_MISO_SAMPLE_ENABLE) | \
                     EXPANSION_PORT_GET_SPI_CTRL_SCLK_CONTINUOUS(EXPANSION_PORT_SPI_FREE_RUN_SCLK_ENABLE)     | \
                     EXPANSION_PORT_GET_SPI_CTRL_SSEL_POLARITY (EXPANSION_PORT_SPI_SS_POLARITY)               | \
                     EXPANSION_PORT_GET_SPI_CTRL_SUB_MODE      (EXPANSION_PORT_SPI_SUB_MODE)                  | \
                     EXPANSION_PORT_GET_SPI_CTRL_MASTER_MODE   (EXPANSION_PORT_SPI_MODE))

    /* RX direction */
    #define EXPANSION_PORT_SPI_DEFAULT_RX_CTRL \
                    (EXPANSION_PORT_GET_RX_CTRL_DATA_WIDTH(EXPANSION_PORT_SPI_RX_DATA_BITS_NUM)     | \
                     EXPANSION_PORT_GET_RX_CTRL_BIT_ORDER (EXPANSION_PORT_SPI_BITS_ORDER)           | \
                     EXPANSION_PORT_GET_RX_CTRL_MEDIAN    (EXPANSION_PORT_SPI_MEDIAN_FILTER_ENABLE) | \
                     EXPANSION_PORT_SPI_RX_CTRL)

    #define EXPANSION_PORT_SPI_DEFAULT_RX_FIFO_CTRL \
                    EXPANSION_PORT_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(EXPANSION_PORT_SPI_RX_TRIGGER_LEVEL)

    /* TX direction */
    #define EXPANSION_PORT_SPI_DEFAULT_TX_CTRL \
                    (EXPANSION_PORT_GET_TX_CTRL_DATA_WIDTH(EXPANSION_PORT_SPI_TX_DATA_BITS_NUM) | \
                     EXPANSION_PORT_GET_TX_CTRL_BIT_ORDER (EXPANSION_PORT_SPI_BITS_ORDER)       | \
                     EXPANSION_PORT_SPI_TX_CTRL)

    #define EXPANSION_PORT_SPI_DEFAULT_TX_FIFO_CTRL \
                    EXPANSION_PORT_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(EXPANSION_PORT_SPI_TX_TRIGGER_LEVEL)

    /* Interrupt sources */
    #define EXPANSION_PORT_SPI_DEFAULT_INTR_SPI_EC_MASK   (EXPANSION_PORT_NO_INTR_SOURCES)

    #define EXPANSION_PORT_SPI_DEFAULT_INTR_I2C_EC_MASK   (EXPANSION_PORT_NO_INTR_SOURCES)
    #define EXPANSION_PORT_SPI_DEFAULT_INTR_SLAVE_MASK \
                    (EXPANSION_PORT_SPI_INTR_RX_MASK & EXPANSION_PORT_INTR_SLAVE_SPI_BUS_ERROR)

    #define EXPANSION_PORT_SPI_DEFAULT_INTR_MASTER_MASK \
                    (EXPANSION_PORT_SPI_INTR_TX_MASK & EXPANSION_PORT_INTR_MASTER_SPI_DONE)

    #define EXPANSION_PORT_SPI_DEFAULT_INTR_RX_MASK \
                    (EXPANSION_PORT_SPI_INTR_RX_MASK & (uint32) ~EXPANSION_PORT_INTR_SLAVE_SPI_BUS_ERROR)

    #define EXPANSION_PORT_SPI_DEFAULT_INTR_TX_MASK \
                    (EXPANSION_PORT_SPI_INTR_TX_MASK & (uint32) ~EXPANSION_PORT_INTR_MASTER_SPI_DONE)

#endif /* (EXPANSION_PORT_SCB_MODE_SPI_CONST_CFG) */


/***************************************
*        UART Register Settings
***************************************/

#define EXPANSION_PORT_CTRL_UART      (EXPANSION_PORT_CTRL_MODE_UART)
#define EXPANSION_PORT_UART_RX_CTRL   (EXPANSION_PORT_RX_CTRL_LSB_FIRST) /* LSB for UART goes first */
#define EXPANSION_PORT_UART_TX_CTRL   (EXPANSION_PORT_TX_CTRL_LSB_FIRST) /* LSB for UART goes first */


/***************************************
*      UART Init Register Settings
***************************************/

#if(EXPANSION_PORT_SCB_MODE_UART_CONST_CFG)

    /* UART configuration */
    #if(EXPANSION_PORT_UART_MODE_IRDA == EXPANSION_PORT_UART_SUB_MODE)

        #define EXPANSION_PORT_DEFAULT_CTRL_OVS   ((0u != EXPANSION_PORT_UART_IRDA_LOW_POWER) ?              \
                                (EXPANSION_PORT_UART_GET_CTRL_OVS_IRDA_LP(EXPANSION_PORT_UART_OVS_FACTOR)) : \
                                (EXPANSION_PORT_CTRL_OVS_IRDA_OVS16))

    #else

        #define EXPANSION_PORT_DEFAULT_CTRL_OVS   EXPANSION_PORT_GET_CTRL_OVS(EXPANSION_PORT_UART_OVS_FACTOR)

    #endif /* (EXPANSION_PORT_UART_MODE_IRDA == EXPANSION_PORT_UART_SUB_MODE) */

    #define EXPANSION_PORT_UART_DEFAULT_CTRL \
                                (EXPANSION_PORT_GET_CTRL_BYTE_MODE  (EXPANSION_PORT_UART_BYTE_MODE_ENABLE)  | \
                                 EXPANSION_PORT_GET_CTRL_ADDR_ACCEPT(EXPANSION_PORT_UART_MP_ACCEPT_ADDRESS) | \
                                 EXPANSION_PORT_DEFAULT_CTRL_OVS                                              | \
                                 EXPANSION_PORT_CTRL_UART)

    #define EXPANSION_PORT_UART_DEFAULT_UART_CTRL \
                                    (EXPANSION_PORT_GET_UART_CTRL_MODE(EXPANSION_PORT_UART_SUB_MODE))

    /* RX direction */
    #define EXPANSION_PORT_UART_DEFAULT_RX_CTRL_PARITY \
                                ((EXPANSION_PORT_UART_PARITY_NONE != EXPANSION_PORT_UART_PARITY_TYPE) ?      \
                                  (EXPANSION_PORT_GET_UART_RX_CTRL_PARITY(EXPANSION_PORT_UART_PARITY_TYPE) | \
                                   EXPANSION_PORT_UART_RX_CTRL_PARITY_ENABLED) : (0u))

    #define EXPANSION_PORT_UART_DEFAULT_UART_RX_CTRL \
                    (EXPANSION_PORT_GET_UART_RX_CTRL_MODE(EXPANSION_PORT_UART_STOP_BITS_NUM)                    | \
                     EXPANSION_PORT_GET_UART_RX_CTRL_POLARITY(EXPANSION_PORT_UART_IRDA_POLARITY)                | \
                     EXPANSION_PORT_GET_UART_RX_CTRL_MP_MODE(EXPANSION_PORT_UART_MP_MODE_ENABLE)                | \
                     EXPANSION_PORT_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(EXPANSION_PORT_UART_DROP_ON_PARITY_ERR) | \
                     EXPANSION_PORT_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(EXPANSION_PORT_UART_DROP_ON_FRAME_ERR)   | \
                     EXPANSION_PORT_UART_DEFAULT_RX_CTRL_PARITY)

    #define EXPANSION_PORT_UART_DEFAULT_RX_CTRL \
                                (EXPANSION_PORT_GET_RX_CTRL_DATA_WIDTH(EXPANSION_PORT_UART_DATA_BITS_NUM)        | \
                                 EXPANSION_PORT_GET_RX_CTRL_MEDIAN    (EXPANSION_PORT_UART_MEDIAN_FILTER_ENABLE) | \
                                 EXPANSION_PORT_GET_UART_RX_CTRL_ENABLED(EXPANSION_PORT_UART_DIRECTION))

    #define EXPANSION_PORT_UART_DEFAULT_RX_FIFO_CTRL \
                                EXPANSION_PORT_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(EXPANSION_PORT_UART_RX_TRIGGER_LEVEL)

    #define EXPANSION_PORT_UART_DEFAULT_RX_MATCH_REG  ((0u != EXPANSION_PORT_UART_MP_MODE_ENABLE) ?          \
                                (EXPANSION_PORT_GET_RX_MATCH_ADDR(EXPANSION_PORT_UART_MP_RX_ADDRESS) | \
                                 EXPANSION_PORT_GET_RX_MATCH_MASK(EXPANSION_PORT_UART_MP_RX_ADDRESS_MASK)) : (0u))

    /* TX direction */
    #define EXPANSION_PORT_UART_DEFAULT_TX_CTRL_PARITY (EXPANSION_PORT_UART_DEFAULT_RX_CTRL_PARITY)

    #define EXPANSION_PORT_UART_DEFAULT_UART_TX_CTRL \
                                (EXPANSION_PORT_GET_UART_TX_CTRL_MODE(EXPANSION_PORT_UART_STOP_BITS_NUM)       | \
                                 EXPANSION_PORT_GET_UART_TX_CTRL_RETRY_NACK(EXPANSION_PORT_UART_RETRY_ON_NACK) | \
                                 EXPANSION_PORT_UART_DEFAULT_TX_CTRL_PARITY)

    #define EXPANSION_PORT_UART_DEFAULT_TX_CTRL \
                                (EXPANSION_PORT_GET_TX_CTRL_DATA_WIDTH(EXPANSION_PORT_UART_DATA_BITS_NUM) | \
                                 EXPANSION_PORT_GET_UART_TX_CTRL_ENABLED(EXPANSION_PORT_UART_DIRECTION))

    #define EXPANSION_PORT_UART_DEFAULT_TX_FIFO_CTRL \
                                EXPANSION_PORT_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(EXPANSION_PORT_UART_TX_TRIGGER_LEVEL)

    #define EXPANSION_PORT_UART_DEFAULT_FLOW_CTRL \
                        (EXPANSION_PORT_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(EXPANSION_PORT_UART_RTS_FIFO_LEVEL) | \
                         EXPANSION_PORT_GET_UART_FLOW_CTRL_RTS_POLARITY (EXPANSION_PORT_UART_RTS_POLARITY)   | \
                         EXPANSION_PORT_GET_UART_FLOW_CTRL_CTS_POLARITY (EXPANSION_PORT_UART_CTS_POLARITY)   | \
                         EXPANSION_PORT_GET_UART_FLOW_CTRL_CTS_ENABLE   (EXPANSION_PORT_UART_CTS_ENABLE))

    /* Interrupt sources */
    #define EXPANSION_PORT_UART_DEFAULT_INTR_I2C_EC_MASK  (EXPANSION_PORT_NO_INTR_SOURCES)
    #define EXPANSION_PORT_UART_DEFAULT_INTR_SPI_EC_MASK  (EXPANSION_PORT_NO_INTR_SOURCES)
    #define EXPANSION_PORT_UART_DEFAULT_INTR_SLAVE_MASK   (EXPANSION_PORT_NO_INTR_SOURCES)
    #define EXPANSION_PORT_UART_DEFAULT_INTR_MASTER_MASK  (EXPANSION_PORT_NO_INTR_SOURCES)
    #define EXPANSION_PORT_UART_DEFAULT_INTR_RX_MASK      (EXPANSION_PORT_UART_INTR_RX_MASK)
    #define EXPANSION_PORT_UART_DEFAULT_INTR_TX_MASK      (EXPANSION_PORT_UART_INTR_TX_MASK)

#endif /* (EXPANSION_PORT_SCB_MODE_UART_CONST_CFG) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

#define EXPANSION_PORT_SPIM_ACTIVE_SS0    (EXPANSION_PORT_SPI_SLAVE_SELECT0)
#define EXPANSION_PORT_SPIM_ACTIVE_SS1    (EXPANSION_PORT_SPI_SLAVE_SELECT1)
#define EXPANSION_PORT_SPIM_ACTIVE_SS2    (EXPANSION_PORT_SPI_SLAVE_SELECT2)
#define EXPANSION_PORT_SPIM_ACTIVE_SS3    (EXPANSION_PORT_SPI_SLAVE_SELECT3)

#endif /* CY_SCB_SPI_UART_EXPANSION_PORT_H */


/* [] END OF FILE */
