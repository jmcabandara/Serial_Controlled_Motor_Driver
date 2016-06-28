/*******************************************************************************
* File Name: DEBUGPIN2.h  
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

#if !defined(CY_PINS_DEBUGPIN2_H) /* Pins DEBUGPIN2_H */
#define CY_PINS_DEBUGPIN2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DEBUGPIN2_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    DEBUGPIN2_Write(uint8 value) ;
void    DEBUGPIN2_SetDriveMode(uint8 mode) ;
uint8   DEBUGPIN2_ReadDataReg(void) ;
uint8   DEBUGPIN2_Read(void) ;
uint8   DEBUGPIN2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define DEBUGPIN2_DRIVE_MODE_BITS        (3)
#define DEBUGPIN2_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DEBUGPIN2_DRIVE_MODE_BITS))

#define DEBUGPIN2_DM_ALG_HIZ         (0x00u)
#define DEBUGPIN2_DM_DIG_HIZ         (0x01u)
#define DEBUGPIN2_DM_RES_UP          (0x02u)
#define DEBUGPIN2_DM_RES_DWN         (0x03u)
#define DEBUGPIN2_DM_OD_LO           (0x04u)
#define DEBUGPIN2_DM_OD_HI           (0x05u)
#define DEBUGPIN2_DM_STRONG          (0x06u)
#define DEBUGPIN2_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define DEBUGPIN2_MASK               DEBUGPIN2__MASK
#define DEBUGPIN2_SHIFT              DEBUGPIN2__SHIFT
#define DEBUGPIN2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DEBUGPIN2_PS                     (* (reg32 *) DEBUGPIN2__PS)
/* Port Configuration */
#define DEBUGPIN2_PC                     (* (reg32 *) DEBUGPIN2__PC)
/* Data Register */
#define DEBUGPIN2_DR                     (* (reg32 *) DEBUGPIN2__DR)
/* Input Buffer Disable Override */
#define DEBUGPIN2_INP_DIS                (* (reg32 *) DEBUGPIN2__PC2)


#if defined(DEBUGPIN2__INTSTAT)  /* Interrupt Registers */

    #define DEBUGPIN2_INTSTAT                (* (reg32 *) DEBUGPIN2__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define DEBUGPIN2_DRIVE_MODE_SHIFT       (0x00u)
#define DEBUGPIN2_DRIVE_MODE_MASK        (0x07u << DEBUGPIN2_DRIVE_MODE_SHIFT)


#endif /* End Pins DEBUGPIN2_H */


/* [] END OF FILE */
