/*******************************************************************************
* File Name: DEBUGPIN2.c  
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
#include "DEBUGPIN2.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        DEBUGPIN2_PC =   (DEBUGPIN2_PC & \
                                (uint32)(~(uint32)(DEBUGPIN2_DRIVE_MODE_IND_MASK << (DEBUGPIN2_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (DEBUGPIN2_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: DEBUGPIN2_Write
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
void DEBUGPIN2_Write(uint8 value) 
{
    uint8 drVal = (uint8)(DEBUGPIN2_DR & (uint8)(~DEBUGPIN2_MASK));
    drVal = (drVal | ((uint8)(value << DEBUGPIN2_SHIFT) & DEBUGPIN2_MASK));
    DEBUGPIN2_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: DEBUGPIN2_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  DEBUGPIN2_DM_STRONG     Strong Drive 
*  DEBUGPIN2_DM_OD_HI      Open Drain, Drives High 
*  DEBUGPIN2_DM_OD_LO      Open Drain, Drives Low 
*  DEBUGPIN2_DM_RES_UP     Resistive Pull Up 
*  DEBUGPIN2_DM_RES_DWN    Resistive Pull Down 
*  DEBUGPIN2_DM_RES_UPDWN  Resistive Pull Up/Down 
*  DEBUGPIN2_DM_DIG_HIZ    High Impedance Digital 
*  DEBUGPIN2_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void DEBUGPIN2_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(DEBUGPIN2__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: DEBUGPIN2_Read
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
*  Macro DEBUGPIN2_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 DEBUGPIN2_Read(void) 
{
    return (uint8)((DEBUGPIN2_PS & DEBUGPIN2_MASK) >> DEBUGPIN2_SHIFT);
}


/*******************************************************************************
* Function Name: DEBUGPIN2_ReadDataReg
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
uint8 DEBUGPIN2_ReadDataReg(void) 
{
    return (uint8)((DEBUGPIN2_DR & DEBUGPIN2_MASK) >> DEBUGPIN2_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(DEBUGPIN2_INTSTAT) 

    /*******************************************************************************
    * Function Name: DEBUGPIN2_ClearInterrupt
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
    uint8 DEBUGPIN2_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(DEBUGPIN2_INTSTAT & DEBUGPIN2_MASK);
		DEBUGPIN2_INTSTAT = maskedStatus;
        return maskedStatus >> DEBUGPIN2_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
