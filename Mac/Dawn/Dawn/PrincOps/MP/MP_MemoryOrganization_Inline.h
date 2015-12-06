/*
    MP_MemoryOrganization_Inline.h - internal header file of Mesa Processor Memory Organization inline functions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_MEMORYORGANIZATION_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_MEMORYORGANIZATION_INLINE_H

/*******************************************************************************
** VIRTUAL MEMORY INLINE FUNCTIONS */
__mpinline void
MP_ReadMap(VirtualPageNumber nVirtualPageNumber, MapFlags * pMapFlagsToSet, RealPageNumber * pRealPageNumberToSet)
{    
    if (nVirtualPageNumber < MP_Mesa_Processor.config.m_nNumVirtualMemoryPages)
    {
        MapEntry * pMapEntry = (MapEntry *)(MP_Mesa_Processor.config.m_pVMMap + nVirtualPageNumber);
        *pMapFlagsToSet = (UNSPECIFIED)(*pMapEntry & MapFlagsVacantMask);
        *pRealPageNumberToSet = (*pMapEntry >> 8);
    }
    else
    {
        *pMapFlagsToSet = 0;
        *pRealPageNumberToSet = 0;
    }
}

__mpinline void
MP_WriteMap(VirtualPageNumber nVirtualPageNumber, MapFlags nMapFlags, RealPageNumber nRealPageNumber)
{
    if (nVirtualPageNumber < MP_Mesa_Processor.config.m_nNumVirtualMemoryPages)
    {
        MapEntry * pMapEntry = (MapEntry *)(MP_Mesa_Processor.config.m_pVMMap + nVirtualPageNumber);
        *pMapEntry = (nMapFlags & MapFlagsVacantMask) | (nRealPageNumber << 8);
    }
}

__mpinline BOOLEAN
MP_Vacant(MapFlags nMapFlags)
{
    return ((MapFlagsVacantValue == (nMapFlags & MapFlagsVacantMask)) ? MESA_TRUE : MESA_FALSE);
}

__mpinline RealAddress
MP_Map(VirtualAddress nVirtualAddress, MapOp op)
{
    MapFlags nMapFlags;
    RealPageNumber nRealPageNumber;
    VirtualPageNumber nVirtualPageNumber = nVirtualAddress / PageSize;
    RealAddress nAddressOffset = nVirtualAddress % PageSize;
    MP_ReadMap(nVirtualPageNumber, &nMapFlags, &nRealPageNumber);
    if (MP_Vacant(nMapFlags)) MP_PageFault(nVirtualAddress);
    if (op == MapOp_Write)
    {
        if (nMapFlags & MapFlagsProtected) MP_WriteProtectFault(nVirtualAddress);
        else nMapFlags |= MapFlagsDirty;
    }
    nMapFlags |= MapFlagsReferenced;
    MP_WriteMap(nVirtualPageNumber, nMapFlags, nRealPageNumber);
    return nRealPageNumber * PageSize + nAddressOffset;
}

__mpinline RealAddress MP_Fetch(VirtualAddress nVirtualAddress)
{
    return MP_Map(nVirtualAddress, MapOp_Read);
}

__mpinline RealAddress MP_Store(VirtualAddress nVirtualAddress)
{
    return MP_Map(nVirtualAddress, MapOp_Write);
}

__mpinline LONG_UNSPECIFIED MP_ReadDbl(VirtualAddress nVirtualAddress)
{
    UNSPECIFIED nValLo = MP_ReadMesaRealAddress(MP_Fetch(nVirtualAddress));
    UNSPECIFIED nValHi = MP_ReadMesaRealAddress(MP_Fetch(nVirtualAddress + 1));
    return (((LONG_UNSPECIFIED)nValHi) << 16) | ((LONG_UNSPECIFIED)nValLo);
}

__mpinline UNSPECIFIED MP_ReadCode(CARDINAL nOffset)
{
    return MP_ReadMesaRealAddress(MP_Fetch(MP_Mesa_Processor.registers.CB + ((LONG_CARDINAL)nOffset)));
}

__mpinline LONG_POINTER MP_LengthenPointer(POINTER pointer)
{
    return MP_Mesa_Processor.registers.MDS + ((LONG_POINTER)pointer);
}

__mpinline LONG_POINTER MP_FetchMds(POINTER pointer)
{
    return MP_Fetch(MP_LengthenPointer(pointer));
}

__mpinline LONG_POINTER MP_StoreMds(POINTER pointer)
{
    return MP_Store(MP_LengthenPointer(pointer));
}

__mpinline LONG_UNSPECIFIED MP_ReadDblMds(POINTER pointer)
{
    return MP_ReadDbl(MP_LengthenPointer(pointer));
}

__mpinline LONG_POINTER MP_GlobalBase(LONG_POINTER pointerToGlobalFrame)
{
    return pointerToGlobalFrame - GLOBALOVERHEADSIZE;
}

__mpinline LONG_POINTER MP_GlobalBaseGlobalWordOffset(LONG_POINTER pointerToGlobalFrame)
{
    return MP_GlobalBase(pointerToGlobalFrame) + GLOBALWORDOFFSET;
}

__mpinline CARDINAL MP_GlobalWordGFI(GlobalWord globalWord)
{
    return globalWord >> 2;
}

__mpinline BOOLEAN MP_GlobalWordCodeLinks(GlobalWord globalWord)
{
    return ((globalWord & 1) ? MESA_TRUE : MESA_FALSE);
}

__mpinline BOOLEAN MP_GlobalWordTrapXfers(GlobalWord globalWord)
{
    return ((globalWord & 2) ? MESA_TRUE : MESA_FALSE);
}

__mpinline POINTER MP_LocalBase(POINTER pointerToLocalFrame)
{
    return pointerToLocalFrame - LOCALOVERHEADSIZE;
}

__mpinline POINTER MP_LocalBaseWordOffset(POINTER pointerToLocalFrame)
{
    return (POINTER)(MP_LocalBase(pointerToLocalFrame) + LOCALWORDOFFSET);
}

__mpinline POINTER MP_LocalBaseReturnLinkOffset(POINTER pointerToLocalFrame)
{
    return (POINTER)(MP_LocalBase(pointerToLocalFrame) + LOCALRETURNLINKOFFSET);
}

__mpinline POINTER MP_LocalBaseGlobalLinkOffset(POINTER pointerToLocalFrame)
{
    return (POINTER)(MP_LocalBase(pointerToLocalFrame) + LOCALGLOBALLINKOFFSET);
}

__mpinline POINTER MP_LocalBaseProgramCounterOffset(POINTER pointerToLocalFrame)
{
    return (POINTER)(MP_LocalBase(pointerToLocalFrame) + LOCALPCOFFSET);
}

__mpinline BYTE MP_LocalWordFSI(LocalWord localWord)
{
    return ((BYTE)localWord);
}

__mpinline POINTER MP_Handle(CARDINAL index)
{
    return (POINTER)(index * PROCESSSTATEBLOCKSIZE);
}

__mpinline CARDINAL MP_Index(POINTER handle)
{
    return (CARDINAL)(handle / PROCESSSTATEBLOCKSIZE);
}

__mpinline BYTE MP_NibbleLeft(BYTE pair)
{
    return (pair >> 4);
}

__mpinline BYTE MP_NibbleRight(BYTE pair)
{
    return (pair & 0xF);
}

__mpinline BYTE MP_Left(UNSPECIFIED word)
{
    return ((BYTE)(word >> 8));
}

__mpinline BYTE MP_Right(UNSPECIFIED word)
{
    return ((BYTE)word);
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_MEMORYORGANIZATION_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

