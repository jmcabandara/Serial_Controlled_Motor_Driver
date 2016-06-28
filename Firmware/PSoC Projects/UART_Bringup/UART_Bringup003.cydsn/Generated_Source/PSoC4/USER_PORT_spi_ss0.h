/*******************************************************************************
* File Name: USER_PORT_spi_ss0.h  
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

#if !defined(CY_PINS_USER_PORT_spi_ss0_H) /* Pins USER_PORT_spi_ss0_H */
#define CY_PINS_USER_PORT_spi_ss0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "USER_PORT_spi_ss0_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    USER_PORT_spi_ss0_Write(uint8 value) ;
void    USER_PORT_spi_ss0_SetDriveMode(uint8 mode) ;
uint8   USER_PORT_spi_ss0_ReadDataReg(void) ;
uint8   USER_PORT_spi_ss0_Read(void) ;
uint8   USER_PORT_spi_ss0_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define USER_PORT_spi_ss0_DRIVE_MODE_BITS        (3)
#define USER_PORT_spi_ss0_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - USER_PORT_spi_ss0_DRIVE_MODE_BITS))

#define USER_PORT_spi_ss0_DM_ALG_HIZ         (0x00u)
#define USER_PORT_spi_ss0_DM_DIG_HIZ         (0x01u)
#define USER_PORT_spi_ss0_DM_RES_UP          (0x02u)
#define USER_PORT_spi_ss0_DM_RES_DWN         (0x03u)
#define USER_PORT_spi_ss0_DM_OD_LO           (0x04u)
#define USER_PORT_spi_ss0_DM_OD_HI           (0x05u)
#define USER_PORT_spi_ss0_DM_STRONG          (0x06u)
#define USER_PORT_spi_ss0_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define USER_PORT_spi_ss0_MASK               USER_PORT_spi_ss0__MASK
#define USER_PORT_spi_ss0_SHIFT              USER_PORT_spi_ss0__SHIFT
#define USER_PORT_spi_ss0_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define USER_PORT_spi_ss0_PS                     (* (reg32 *) USER_PORT_spi_ss0__PS)
/* Port Configuration */
#define USER_PORT_spi_ss0_PC                     (* (reg32 *) USER_PORT_spi_ss0__PC)
/* Data Register */
#define USER_PORT_spi_ss0_DR                     (* (reg32 *) USER_PORT_spi_ss0__DR)
/* Input Buffer Disable Override */
#define USER_PORT_spi_ss0_INP_DIS                (* (reg32 *) USER_PORT_spi_ss0__PC2)


#if defined(USER_PORT_spi_ss0__INTSTAT)  /* Interrupt Registers */

    #define USER_PORT_spi_ss0_INTSTAT                (* (reg32 *) USER_PORT_spi_ss0__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define USER_PORT_spi_ss0_DRIVE_MODE_SHIFT       (0x00u)
#define USER_PORT_spi_ss0_DRIVE_MODE_MASK        (0x07u << USER_PORT_spi_ss0_DRIVE_MODE_SHIFT)


#endif /* End Pins USER_PORT_spi_ss0_H */


/* [] END OF FILE */
