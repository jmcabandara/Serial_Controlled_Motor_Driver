/*******************************************************************************
* File Name: SCBCLK.h
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

#if !defined(CY_CLOCK_SCBCLK_H)
#define CY_CLOCK_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void SCBCLK_StartEx(uint32 alignClkDiv);
#define SCBCLK_Start() \
    SCBCLK_StartEx(SCBCLK__PA_DIV_ID)

#else

void SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void SCBCLK_Stop(void);

void SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 SCBCLK_GetDividerRegister(void);
uint8  SCBCLK_GetFractionalDividerRegister(void);

#define SCBCLK_Enable()                         SCBCLK_Start()
#define SCBCLK_Disable()                        SCBCLK_Stop()
#define SCBCLK_SetDividerRegister(clkDivider, reset)  \
    SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define SCBCLK_SetDivider(clkDivider)           SCBCLK_SetDividerRegister((clkDivider), 1u)
#define SCBCLK_SetDividerValue(clkDivider)      SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define SCBCLK_DIV_ID     SCBCLK__DIV_ID

#define SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define SCBCLK_CTRL_REG   (*(reg32 *)SCBCLK__CTRL_REGISTER)
#define SCBCLK_DIV_REG    (*(reg32 *)SCBCLK__DIV_REGISTER)

#define SCBCLK_CMD_DIV_SHIFT          (0u)
#define SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << SCBCLK_CMD_DISABLE_SHIFT))
#define SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << SCBCLK_CMD_ENABLE_SHIFT))

#define SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define SCBCLK_DIV_FRAC_SHIFT (3u)
#define SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define SCBCLK_DIV_REG        (*(reg32 *)SCBCLK__REGISTER)
#define SCBCLK_ENABLE_REG     SCBCLK_DIV_REG
#define SCBCLK_DIV_FRAC_MASK  SCBCLK__FRAC_MASK
#define SCBCLK_DIV_FRAC_SHIFT (16u)
#define SCBCLK_DIV_INT_MASK   SCBCLK__DIVIDER_MASK
#define SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_SCBCLK_H) */

/* [] END OF FILE */
