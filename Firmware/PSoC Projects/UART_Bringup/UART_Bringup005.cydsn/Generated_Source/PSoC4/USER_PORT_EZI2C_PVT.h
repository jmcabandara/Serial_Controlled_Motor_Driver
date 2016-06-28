/*******************************************************************************
* File Name: .h
* Version 2.0
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in _EZI2C mode.
*  Do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_EZI2C_PVT_USER_PORT_H)
#define CY_SCB_EZI2C_PVT_USER_PORT_H

#include "USER_PORT_EZI2C.h"


/***************************************
*      EZI2C Private Vars
***************************************/

extern volatile uint8 USER_PORT_curStatus;
extern uint8 USER_PORT_fsmState;

/* Variables intended to be used with Buffer 1: Primary slave address */
extern volatile uint8 * USER_PORT_dataBuffer1;
extern uint16 USER_PORT_bufSizeBuf1;
extern uint16 USER_PORT_protectBuf1;
extern uint16 USER_PORT_offsetBuf1;
extern uint16 USER_PORT_indexBuf1;

#if(USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST)
    extern uint8 USER_PORT_addrBuf1;
    extern uint8 USER_PORT_addrBuf2;

    /* Variables intended to be used with Buffer 2: Primary slave address */
    extern volatile uint8 * USER_PORT_dataBuffer2;
    extern uint16 USER_PORT_bufSizeBuf2;
    extern uint16 USER_PORT_protectBuf2;
    extern uint16 USER_PORT_offsetBuf2;
    extern uint16 USER_PORT_indexBuf2;
#endif /* (USER_PORT_SECONDARY_ADDRESS_ENABLE_CONST) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(USER_PORT_SCB_MODE_EZI2C_CONST_CFG)
    void USER_PORT_EzI2CInit(void);
#endif /* (USER_PORT_SCB_MODE_EZI2C_CONST_CFG) */

void USER_PORT_EzI2CStop(void);
#if(USER_PORT_EZI2C_WAKE_ENABLE_CONST)
    void USER_PORT_EzI2CSaveConfig(void);
    void USER_PORT_EzI2CRestoreConfig(void);
#endif /* (USER_PORT_EZI2C_WAKE_ENABLE_CONST) */

#endif /* (CY_SCB__EZI2C_PVT_USER_PORT_H) */


/* [] END OF FILE */
