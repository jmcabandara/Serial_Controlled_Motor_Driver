/*******************************************************************************
* File Name: USER_PORT_SCBCLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_USER_PORT_SCBCLK_H)
#define CY_CLOCK_USER_PORT_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void USER_PORT_SCBCLK_StartEx(uint32 alignClkDiv);
#define USER_PORT_SCBCLK_Start() \
    USER_PORT_SCBCLK_StartEx(USER_PORT_SCBCLK__PA_DIV_ID)

#else

void USER_PORT_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void USER_PORT_SCBCLK_Stop(void);

void USER_PORT_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 USER_PORT_SCBCLK_GetDividerRegister(void);
uint8  USER_PORT_SCBCLK_GetFractionalDividerRegister(void);

#define USER_PORT_SCBCLK_Enable()                         USER_PORT_SCBCLK_Start()
#define USER_PORT_SCBCLK_Disable()                        USER_PORT_SCBCLK_Stop()
#define USER_PORT_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    USER_PORT_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define USER_PORT_SCBCLK_SetDivider(clkDivider)           USER_PORT_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define USER_PORT_SCBCLK_SetDividerValue(clkDivider)      USER_PORT_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define USER_PORT_SCBCLK_DIV_ID     USER_PORT_SCBCLK__DIV_ID

#define USER_PORT_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define USER_PORT_SCBCLK_CTRL_REG   (*(reg32 *)USER_PORT_SCBCLK__CTRL_REGISTER)
#define USER_PORT_SCBCLK_DIV_REG    (*(reg32 *)USER_PORT_SCBCLK__DIV_REGISTER)

#define USER_PORT_SCBCLK_CMD_DIV_SHIFT          (0u)
#define USER_PORT_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define USER_PORT_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define USER_PORT_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define USER_PORT_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << USER_PORT_SCBCLK_CMD_DISABLE_SHIFT))
#define USER_PORT_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << USER_PORT_SCBCLK_CMD_ENABLE_SHIFT))

#define USER_PORT_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define USER_PORT_SCBCLK_DIV_FRAC_SHIFT (3u)
#define USER_PORT_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define USER_PORT_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define USER_PORT_SCBCLK_DIV_REG        (*(reg32 *)USER_PORT_SCBCLK__REGISTER)
#define USER_PORT_SCBCLK_ENABLE_REG     USER_PORT_SCBCLK_DIV_REG
#define USER_PORT_SCBCLK_DIV_FRAC_MASK  USER_PORT_SCBCLK__FRAC_MASK
#define USER_PORT_SCBCLK_DIV_FRAC_SHIFT (16u)
#define USER_PORT_SCBCLK_DIV_INT_MASK   USER_PORT_SCBCLK__DIVIDER_MASK
#define USER_PORT_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_USER_PORT_SCBCLK_H) */

/* [] END OF FILE */
