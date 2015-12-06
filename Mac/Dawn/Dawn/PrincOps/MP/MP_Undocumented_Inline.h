/*
    MP_Undocumented_Inline.h - Mesa Processor Undocumented inline functions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_UNDOCUMENTED_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_UNDOCUMENTED_INLINE_H

/*******************************************************************************
** Inline Helper functions that are undocumented */
__mpinline BOOLEAN
MP_GetVMDataRunVMInterval(LONG_POINTER virtualAddressVMDataRun, LONG_CARDINAL * pIntervalPageNumberToSet, LONG_CARDINAL * pIntervalPageCountToSet)
{
    VirtualPageNumber nVirtualPageNumber = virtualAddressVMDataRun / PageSize;
    RealAddress nAddressOffset = virtualAddressVMDataRun % PageSize;
    RealPageNumber nRealPageNumber;
    RealAddress nRealAddressPageNumber;
    RealAddress nRealAddressPageCount;
    MapFlags nMapFlags;

    MP_ReadMap(nVirtualPageNumber, &nMapFlags, &nRealPageNumber);
    if (MP_Vacant(nMapFlags)) return MESA_FALSE;
    nRealAddressPageNumber = nRealPageNumber * PageSize + nAddressOffset;
    
    virtualAddressVMDataRun += 2;
    nVirtualPageNumber = virtualAddressVMDataRun / PageSize;
    nAddressOffset = virtualAddressVMDataRun % PageSize;
    MP_ReadMap(nVirtualPageNumber, &nMapFlags, &nRealPageNumber);
    if (MP_Vacant(nMapFlags)) return MESA_FALSE;
    nRealAddressPageCount = nRealPageNumber * PageSize + nAddressOffset;

    *pIntervalPageNumberToSet = MP_ReadMesaRealAddress(nRealAddressPageNumber);
    *pIntervalPageCountToSet = MP_ReadMesaRealAddress(nRealAddressPageCount);

    return MESA_TRUE;
}

__mpinline CARDINAL
MP_VMDataRunIndexFromPointer(POINTER pVMDataRun)
{
    return (pVMDataRun / VMDataRunSize);
}

__mpinline POINTER
MP_VMDataRunPointerFromIndex(CARDINAL nIndexVMDataRun)
{
    return (nIndexVMDataRun * VMDataRunSize);
}

__mpinline LONG_POINTER
MP_VMDataRunLongPointerFromIndex(LONG_POINTER nVMDataRunTableBase, CARDINAL nIndexVMDataRun)
{
    return (nVMDataRunTableBase + (nIndexVMDataRun * VMDataRunSize));
}

__mpinline LONG_POINTER
MP_VMDataRunLongPointerFromPointer(LONG_POINTER nVMDataRunTableBase, POINTER pVMDataRun)
{
    return (nVMDataRunTableBase + pVMDataRun);
}



#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_UNDOCUMENTED_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

