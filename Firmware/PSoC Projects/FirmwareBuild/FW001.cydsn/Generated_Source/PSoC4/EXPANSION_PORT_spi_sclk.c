/*******************************************************************************
* File Name: EXPANSION_PORT_spi_sclk.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "EXPANSION_PORT_spi_sclk.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        EXPANSION_PORT_spi_sclk_PC =   (EXPANSION_PORT_spi_sclk_PC & \
                                (uint32)(~(uint32)(EXPANSION_PORT_spi_sclk_DRIVE_MODE_IND_MASK << (EXPANSION_PORT_spi_sclk_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (EXPANSION_PORT_spi_sclk_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: EXPANSION_PORT_spi_sclk_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void EXPANSION_PORT_spi_sclk_Write(uint8 value) 
{
    uint8 drVal = (uint8)(EXPANSION_PORT_spi_sclk_DR & (uint8)(~EXPANSION_PORT_spi_sclk_MASK));
    drVal = (drVal | ((uint8)(value << EXPANSION_PORT_spi_sclk_SHIFT) & EXPANSION_PORT_spi_sclk_MASK));
    EXPANSION_PORT_spi_sclk_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_spi_sclk_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  EXPANSION_PORT_spi_sclk_DM_STRONG     Strong Drive 
*  EXPANSION_PORT_spi_sclk_DM_OD_HI      Open Drain, Drives High 
*  EXPANSION_PORT_spi_sclk_DM_OD_LO      Open Drain, Drives Low 
*  EXPANSION_PORT_spi_sclk_DM_RES_UP     Resistive Pull Up 
*  EXPANSION_PORT_spi_sclk_DM_RES_DWN    Resistive Pull Down 
*  EXPANSION_PORT_spi_sclk_DM_RES_UPDWN  Resistive Pull Up/Down 
*  EXPANSION_PORT_spi_sclk_DM_DIG_HIZ    High Impedance Digital 
*  EXPANSION_PORT_spi_sclk_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void EXPANSION_PORT_spi_sclk_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(EXPANSION_PORT_spi_sclk__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_spi_sclk_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro EXPANSION_PORT_spi_sclk_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 EXPANSION_PORT_spi_sclk_Read(void) 
{
    return (uint8)((EXPANSION_PORT_spi_sclk_PS & EXPANSION_PORT_spi_sclk_MASK) >> EXPANSION_PORT_spi_sclk_SHIFT);
}


/*******************************************************************************
* Function Name: EXPANSION_PORT_spi_sclk_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 EXPANSION_PORT_spi_sclk_ReadDataReg(void) 
{
    return (uint8)((EXPANSION_PORT_spi_sclk_DR & EXPANSION_PORT_spi_sclk_MASK) >> EXPANSION_PORT_spi_sclk_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(EXPANSION_PORT_spi_sclk_INTSTAT) 

    /*******************************************************************************
    * Function Name: EXPANSION_PORT_spi_sclk_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 EXPANSION_PORT_spi_sclk_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(EXPANSION_PORT_spi_sclk_INTSTAT & EXPANSION_PORT_spi_sclk_MASK);
		EXPANSION_PORT_spi_sclk_INTSTAT = maskedStatus;
        return maskedStatus >> EXPANSION_PORT_spi_sclk_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
