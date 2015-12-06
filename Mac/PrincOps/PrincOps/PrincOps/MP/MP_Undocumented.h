/*
    MP_Undocumented.h - internal header file of Mesa processor process instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_UNDOCUMENTED_H
#define _INCLUDE_PRINCOPS_MP_MP_UNDOCUMENTED_H

/*******************************************************************************
** UNDOCUMENTED TYPES */
#define                     VMDataRunIndexFirst      ((CARDINAL)1)
#define                     VMDataRunPointerFirst    ((POINTER)14)
#define                     VMDataRunSize            ((CARDINAL)14)

/*******************************************************************************
** UNDOCUMENTED FUNCTIONS */
__mpinline BOOLEAN MP_GetVMDataRunVMInterval(LONG_POINTER virtualAddressVMDataRun, LONG_CARDINAL * pIntervalPageNumberToSet, LONG_CARDINAL * pIntervalPageCountToSet);
__mpinline CARDINAL MP_VMDataRunIndexFromPointer(POINTER pVMDataRun);
__mpinline POINTER MP_VMDataRunPointerFromIndex(CARDINAL nIndexVMDataRun);
__mpinline LONG_POINTER MP_VMDataRunLongPointerFromIndex(LONG_POINTER nVMDataRunTableBase, CARDINAL nIndexVMDataRun);
__mpinline LONG_POINTER MP_VMDataRunLongPointerFromPointer(LONG_POINTER nVMDataRunTableBase, POINTER pVMDataRun);

/*******************************************************************************
** UNDOCUMENTED INSTRUCTIONS */

void MP_Instruction_LGA0();     /* Long Global Address 0 */
void MP_Instruction_LGAB();     /* Long Global Address Byte */
void MP_Instruction_LGAW();     /* Long Global Address Word */
void MP_Instruction_DESC();     /* Desc */

void MP_Instruction_VERSION();  /* Version */
void MP_Instruction_BYTESWAP(); /* Byte Swap */

void MP_Instruction_VMFIND();   /* VM Find */

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_UNDOCUMENTED_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

