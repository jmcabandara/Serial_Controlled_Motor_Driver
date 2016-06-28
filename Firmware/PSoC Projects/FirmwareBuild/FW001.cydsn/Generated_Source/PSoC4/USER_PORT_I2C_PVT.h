/*******************************************************************************
* File Name: .h
* Version 2.0
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_I2C_PVT_USER_PORT_H)
#define CY_SCB_I2C_PVT_USER_PORT_H

#include "USER_PORT_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 USER_PORT_state; /* Current state of I2C FSM */

#if(USER_PORT_I2C_SLAVE_CONST)
    extern volatile uint8 USER_PORT_slStatus;          /* Slave Status */

    /* Receive buffer variables */
    extern volatile uint8 * USER_PORT_slWrBufPtr;      /* Pointer to Receive buffer  */
    extern volatile uint32  USER_PORT_slWrBufSize;     /* Slave Receive buffer size  */
    extern volatile uint32  USER_PORT_slWrBufIndex;    /* Slave Receive buffer Index */

    /* Transmit buffer variables */
    extern volatile uint8 * USER_PORT_slRdBufPtr;      /* Pointer to Transmit buffer  */
    extern volatile uint32  USER_PORT_slRdBufSize;     /* Slave Transmit buffer size  */
    extern volatile uint32  USER_PORT_slRdBufIndex;    /* Slave Transmit buffer Index */
    extern volatile uint32  USER_PORT_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
    extern volatile uint8   USER_PORT_slOverFlowCount; /* Slave Transmit Overflow counter */
#endif /* (USER_PORT_I2C_SLAVE_CONST) */

#if(USER_PORT_I2C_MASTER_CONST)
    extern volatile uint16 USER_PORT_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  USER_PORT_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * USER_PORT_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  USER_PORT_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  USER_PORT_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * USER_PORT_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  USER_PORT_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  USER_PORT_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  USER_PORT_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (USER_PORT_I2C_MASTER_CONST) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(USER_PORT_SCB_MODE_I2C_CONST_CFG)
    void USER_PORT_I2CInit(void);
#endif /* (USER_PORT_SCB_MODE_I2C_CONST_CFG) */

void USER_PORT_I2CStop(void);
void USER_PORT_I2CSaveConfig(void);
void USER_PORT_I2CRestoreConfig(void);

#if(USER_PORT_I2C_MASTER_CONST)
    void USER_PORT_I2CReStartGeneration(void);
#endif /* (USER_PORT_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_USER_PORT_H) */


/* [] END OF FILE */
