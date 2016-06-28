/*******************************************************************************
* File Name: USER_PORT_tx.c  
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
#include "USER_PORT_tx.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        USER_PORT_tx_PC =   (USER_PORT_tx_PC & \
                                (uint32)(~(uint32)(USER_PORT_tx_DRIVE_MODE_IND_MASK << (USER_PORT_tx_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (USER_PORT_tx_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: USER_PORT_tx_Write
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
void USER_PORT_tx_Write(uint8 value) 
{
    uint8 drVal = (uint8)(USER_PORT_tx_DR & (uint8)(~USER_PORT_tx_MASK));
    drVal = (drVal | ((uint8)(value << USER_PORT_tx_SHIFT) & USER_PORT_tx_MASK));
    USER_PORT_tx_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: USER_PORT_tx_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  USER_PORT_tx_DM_STRONG     Strong Drive 
*  USER_PORT_tx_DM_OD_HI      Open Drain, Drives High 
*  USER_PORT_tx_DM_OD_LO      Open Drain, Drives Low 
*  USER_PORT_tx_DM_RES_UP     Resistive Pull Up 
*  USER_PORT_tx_DM_RES_DWN    Resistive Pull Down 
*  USER_PORT_tx_DM_RES_UPDWN  Resistive Pull Up/Down 
*  USER_PORT_tx_DM_DIG_HIZ    High Impedance Digital 
*  USER_PORT_tx_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void USER_PORT_tx_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(USER_PORT_tx__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: USER_PORT_tx_Read
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
*  Macro USER_PORT_tx_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 USER_PORT_tx_Read(void) 
{
    return (uint8)((USER_PORT_tx_PS & USER_PORT_tx_MASK) >> USER_PORT_tx_SHIFT);
}


/*******************************************************************************
* Function Name: USER_PORT_tx_ReadDataReg
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
uint8 USER_PORT_tx_ReadDataReg(void) 
{
    return (uint8)((USER_PORT_tx_DR & USER_PORT_tx_MASK) >> USER_PORT_tx_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(USER_PORT_tx_INTSTAT) 

    /*******************************************************************************
    * Function Name: USER_PORT_tx_ClearInterrupt
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
    uint8 USER_PORT_tx_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(USER_PORT_tx_INTSTAT & USER_PORT_tx_MASK);
		USER_PORT_tx_INTSTAT = maskedStatus;
        return maskedStatus >> USER_PORT_tx_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
