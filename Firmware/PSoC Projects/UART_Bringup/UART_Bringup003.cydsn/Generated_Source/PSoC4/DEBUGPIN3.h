/*******************************************************************************
* File Name: DEBUGPIN3.h  
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

#if !defined(CY_PINS_DEBUGPIN3_H) /* Pins DEBUGPIN3_H */
#define CY_PINS_DEBUGPIN3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DEBUGPIN3_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    DEBUGPIN3_Write(uint8 value) ;
void    DEBUGPIN3_SetDriveMode(uint8 mode) ;
uint8   DEBUGPIN3_ReadDataReg(void) ;
uint8   DEBUGPIN3_Read(void) ;
uint8   DEBUGPIN3_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define DEBUGPIN3_DRIVE_MODE_BITS        (3)
#define DEBUGPIN3_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DEBUGPIN3_DRIVE_MODE_BITS))

#define DEBUGPIN3_DM_ALG_HIZ         (0x00u)
#define DEBUGPIN3_DM_DIG_HIZ         (0x01u)
#define DEBUGPIN3_DM_RES_UP          (0x02u)
#define DEBUGPIN3_DM_RES_DWN         (0x03u)
#define DEBUGPIN3_DM_OD_LO           (0x04u)
#define DEBUGPIN3_DM_OD_HI           (0x05u)
#define DEBUGPIN3_DM_STRONG          (0x06u)
#define DEBUGPIN3_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define DEBUGPIN3_MASK               DEBUGPIN3__MASK
#define DEBUGPIN3_SHIFT              DEBUGPIN3__SHIFT
#define DEBUGPIN3_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DEBUGPIN3_PS                     (* (reg32 *) DEBUGPIN3__PS)
/* Port Configuration */
#define DEBUGPIN3_PC                     (* (reg32 *) DEBUGPIN3__PC)
/* Data Register */
#define DEBUGPIN3_DR                     (* (reg32 *) DEBUGPIN3__DR)
/* Input Buffer Disable Override */
#define DEBUGPIN3_INP_DIS                (* (reg32 *) DEBUGPIN3__PC2)


#if defined(DEBUGPIN3__INTSTAT)  /* Interrupt Registers */

    #define DEBUGPIN3_INTSTAT                (* (reg32 *) DEBUGPIN3__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define DEBUGPIN3_DRIVE_MODE_SHIFT       (0x00u)
#define DEBUGPIN3_DRIVE_MODE_MASK        (0x07u << DEBUGPIN3_DRIVE_MODE_SHIFT)


#endif /* End Pins DEBUGPIN3_H */


/* [] END OF FILE */
