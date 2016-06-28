/*******************************************************************************
* File Name: USER_PORT_spi_sclk.h  
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

#if !defined(CY_PINS_USER_PORT_spi_sclk_H) /* Pins USER_PORT_spi_sclk_H */
#define CY_PINS_USER_PORT_spi_sclk_H

#include "cytypes.h"
#include "cyfitter.h"
#include "USER_PORT_spi_sclk_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    USER_PORT_spi_sclk_Write(uint8 value) ;
void    USER_PORT_spi_sclk_SetDriveMode(uint8 mode) ;
uint8   USER_PORT_spi_sclk_ReadDataReg(void) ;
uint8   USER_PORT_spi_sclk_Read(void) ;
uint8   USER_PORT_spi_sclk_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define USER_PORT_spi_sclk_DRIVE_MODE_BITS        (3)
#define USER_PORT_spi_sclk_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - USER_PORT_spi_sclk_DRIVE_MODE_BITS))

#define USER_PORT_spi_sclk_DM_ALG_HIZ         (0x00u)
#define USER_PORT_spi_sclk_DM_DIG_HIZ         (0x01u)
#define USER_PORT_spi_sclk_DM_RES_UP          (0x02u)
#define USER_PORT_spi_sclk_DM_RES_DWN         (0x03u)
#define USER_PORT_spi_sclk_DM_OD_LO           (0x04u)
#define USER_PORT_spi_sclk_DM_OD_HI           (0x05u)
#define USER_PORT_spi_sclk_DM_STRONG          (0x06u)
#define USER_PORT_spi_sclk_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define USER_PORT_spi_sclk_MASK               USER_PORT_spi_sclk__MASK
#define USER_PORT_spi_sclk_SHIFT              USER_PORT_spi_sclk__SHIFT
#define USER_PORT_spi_sclk_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define USER_PORT_spi_sclk_PS                     (* (reg32 *) USER_PORT_spi_sclk__PS)
/* Port Configuration */
#define USER_PORT_spi_sclk_PC                     (* (reg32 *) USER_PORT_spi_sclk__PC)
/* Data Register */
#define USER_PORT_spi_sclk_DR                     (* (reg32 *) USER_PORT_spi_sclk__DR)
/* Input Buffer Disable Override */
#define USER_PORT_spi_sclk_INP_DIS                (* (reg32 *) USER_PORT_spi_sclk__PC2)


#if defined(USER_PORT_spi_sclk__INTSTAT)  /* Interrupt Registers */

    #define USER_PORT_spi_sclk_INTSTAT                (* (reg32 *) USER_PORT_spi_sclk__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define USER_PORT_spi_sclk_DRIVE_MODE_SHIFT       (0x00u)
#define USER_PORT_spi_sclk_DRIVE_MODE_MASK        (0x07u << USER_PORT_spi_sclk_DRIVE_MODE_SHIFT)


#endif /* End Pins USER_PORT_spi_sclk_H */


/* [] END OF FILE */
