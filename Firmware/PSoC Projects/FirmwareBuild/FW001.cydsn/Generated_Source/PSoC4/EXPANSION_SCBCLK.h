/*******************************************************************************
* File Name: EXPANSION_SCBCLK.h
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

#if !defined(CY_CLOCK_EXPANSION_SCBCLK_H)
#define CY_CLOCK_EXPANSION_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void EXPANSION_SCBCLK_StartEx(uint32 alignClkDiv);
#define EXPANSION_SCBCLK_Start() \
    EXPANSION_SCBCLK_StartEx(EXPANSION_SCBCLK__PA_DIV_ID)

#else

void EXPANSION_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void EXPANSION_SCBCLK_Stop(void);

void EXPANSION_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 EXPANSION_SCBCLK_GetDividerRegister(void);
uint8  EXPANSION_SCBCLK_GetFractionalDividerRegister(void);

#define EXPANSION_SCBCLK_Enable()                         EXPANSION_SCBCLK_Start()
#define EXPANSION_SCBCLK_Disable()                        EXPANSION_SCBCLK_Stop()
#define EXPANSION_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    EXPANSION_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define EXPANSION_SCBCLK_SetDivider(clkDivider)           EXPANSION_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define EXPANSION_SCBCLK_SetDividerValue(clkDivider)      EXPANSION_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define EXPANSION_SCBCLK_DIV_ID     EXPANSION_SCBCLK__DIV_ID

#define EXPANSION_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define EXPANSION_SCBCLK_CTRL_REG   (*(reg32 *)EXPANSION_SCBCLK__CTRL_REGISTER)
#define EXPANSION_SCBCLK_DIV_REG    (*(reg32 *)EXPANSION_SCBCLK__DIV_REGISTER)

#define EXPANSION_SCBCLK_CMD_DIV_SHIFT          (0u)
#define EXPANSION_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define EXPANSION_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define EXPANSION_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define EXPANSION_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << EXPANSION_SCBCLK_CMD_DISABLE_SHIFT))
#define EXPANSION_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << EXPANSION_SCBCLK_CMD_ENABLE_SHIFT))

#define EXPANSION_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define EXPANSION_SCBCLK_DIV_FRAC_SHIFT (3u)
#define EXPANSION_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define EXPANSION_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define EXPANSION_SCBCLK_DIV_REG        (*(reg32 *)EXPANSION_SCBCLK__REGISTER)
#define EXPANSION_SCBCLK_ENABLE_REG     EXPANSION_SCBCLK_DIV_REG
#define EXPANSION_SCBCLK_DIV_FRAC_MASK  EXPANSION_SCBCLK__FRAC_MASK
#define EXPANSION_SCBCLK_DIV_FRAC_SHIFT (16u)
#define EXPANSION_SCBCLK_DIV_INT_MASK   EXPANSION_SCBCLK__DIVIDER_MASK
#define EXPANSION_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_EXPANSION_SCBCLK_H) */

/* [] END OF FILE */
