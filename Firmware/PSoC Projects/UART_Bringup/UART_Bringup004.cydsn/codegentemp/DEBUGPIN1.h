/*******************************************************************************
* File Name: DEBUGPIN1.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_DEBUGPIN1_H) /* Pins DEBUGPIN1_H */
#define CY_PINS_DEBUGPIN1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DEBUGPIN1_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    DEBUGPIN1_Write(uint8 value) ;
void    DEBUGPIN1_SetDriveMode(uint8 mode) ;
uint8   DEBUGPIN1_ReadDataReg(void) ;
uint8   DEBUGPIN1_Read(void) ;
uint8   DEBUGPIN1_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define DEBUGPIN1_DRIVE_MODE_BITS        (3)
#define DEBUGPIN1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DEBUGPIN1_DRIVE_MODE_BITS))

#define DEBUGPIN1_DM_ALG_HIZ         (0x00u)
#define DEBUGPIN1_DM_DIG_HIZ         (0x01u)
#define DEBUGPIN1_DM_RES_UP          (0x02u)
#define DEBUGPIN1_DM_RES_DWN         (0x03u)
#define DEBUGPIN1_DM_OD_LO           (0x04u)
#define DEBUGPIN1_DM_OD_HI           (0x05u)
#define DEBUGPIN1_DM_STRONG          (0x06u)
#define DEBUGPIN1_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define DEBUGPIN1_MASK               DEBUGPIN1__MASK
#define DEBUGPIN1_SHIFT              DEBUGPIN1__SHIFT
#define DEBUGPIN1_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DEBUGPIN1_PS                     (* (reg32 *) DEBUGPIN1__PS)
/* Port Configuration */
#define DEBUGPIN1_PC                     (* (reg32 *) DEBUGPIN1__PC)
/* Data Register */
#define DEBUGPIN1_DR                     (* (reg32 *) DEBUGPIN1__DR)
/* Input Buffer Disable Override */
#define DEBUGPIN1_INP_DIS                (* (reg32 *) DEBUGPIN1__PC2)


#if defined(DEBUGPIN1__INTSTAT)  /* Interrupt Registers */

    #define DEBUGPIN1_INTSTAT                (* (reg32 *) DEBUGPIN1__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define DEBUGPIN1_DRIVE_MODE_SHIFT       (0x00u)
#define DEBUGPIN1_DRIVE_MODE_MASK        (0x07u << DEBUGPIN1_DRIVE_MODE_SHIFT)


#endif /* End Pins DEBUGPIN1_H */


/* [] END OF FILE */
