/*
    MP_AssignmentInstructions_Inline.h - internal header file of Mesa Processor Assignment inline functions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_ASSIGNMENTINSTRUCTIONS_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_ASSIGNMENTINSTRUCTIONS_INLINE_H

/*******************************************************************************
** ASSIGNMENT INLINE FUNCTIONS */
__mpinline void MP_LoadImmediateN(CARDINAL n)
{
    MP_Push(n);
}

__mpinline void MP_LocalAddressN(CARDINAL n)
{
    MP_Push((UNSPECIFIED)(MP_Mesa_Processor.registers.LF + n));
}

__mpinline void MP_LoadLocalN(CARDINAL n)
{
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + n))));
}

__mpinline void MP_LoadLocalDoubleN(CARDINAL n)
{
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + n))));
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + n + 1))));
}

__mpinline void MP_StoreLocalN(CARDINAL n)
{
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + n)), MP_Pop());
}

__mpinline void MP_StoreLocalDoubleN(CARDINAL n)
{
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + n + 1)), MP_Pop());
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + n)), MP_Pop());
}

__mpinline void MP_PutLocalN(CARDINAL n)
{
    MP_StoreLocalN(n);
    MP_Recover();
}

__mpinline void MP_StoreLocalByte()
{
    BYTE alpha = MP_GetCodeByte();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + alpha)), MP_Pop());
}

__mpinline void MP_StoreLocalDoubleByte()
{
    BYTE alpha = MP_GetCodeByte();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + alpha + 1)), MP_Pop());
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + alpha)), MP_Pop());
}

__mpinline void MP_GlobalAddressN(CARDINAL n)
{
    MP_Push((UNSPECIFIED)(MP_Mesa_Processor.registers.GF + n));
}

__mpinline void MP_LoadGlobalN(CARDINAL n)
{
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(MP_Mesa_Processor.registers.GF + n)));
}

__mpinline void MP_LoadGlobalDoubleN(CARDINAL n)
{
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(MP_Mesa_Processor.registers.GF + n)));
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(MP_Mesa_Processor.registers.GF + n + 1)));
}

__mpinline void MP_ReadN(CARDINAL n)
{
    POINTER ptr = MP_Pop();
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + n))));
}

__mpinline void MP_ReadLocalIndirectZeroN(CARDINAL n)
{
    POINTER ptr = MP_ReadMesaRealAddress(MP_FetchMds(MP_Mesa_Processor.registers.LF));
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + n))));
}

__mpinline BYTE MP_FetchByte(LONG_POINTER ptr, LONG_CARDINAL offset)
{
    UNSPECIFIED word = MP_ReadMesaRealAddress(MP_Fetch(ptr + offset / 2));
    return (0 == (offset & 1)) ? ((BYTE)(word >> 8)) : ((BYTE)word);
}

__mpinline void MP_StoreByte(LONG_POINTER ptr, LONG_CARDINAL offset, BYTE data)
{
    UNSPECIFIED word = MP_ReadMesaRealAddress(MP_Fetch(ptr + offset / 2));
    MP_WriteMesaRealAddress(MP_Store(ptr + offset / 2), (UNSPECIFIED)((0 == (offset & 1)) ? ((((UNSPECIFIED)data) << 8) | (word & 0xFF)) : ((word & 0xFF00) | ((UNSPECIFIED)data))));
}

__mpinline BYTE MP_GetFieldSpecPos(FieldSpec fieldSpec)
{
    return (fieldSpec >> 4);
}

__mpinline BYTE MP_GetFieldSpecSize(FieldSpec fieldSpec)
{
    return (fieldSpec & 0xF);
}

__mpinline BYTE MP_GetFieldDescOffset(FieldDesc fieldDesc)
{
    return ((BYTE)(fieldDesc >> 8));
}

__mpinline FieldSpec MP_GetFieldDescFieldSpec(FieldDesc fieldDesc)
{
    return ((FieldSpec)fieldDesc);
}

__mpinline UNSPECIFIED MP_ReadField(UNSPECIFIED source, FieldSpec spec)
{
    BYTE pos = MP_GetFieldSpecPos(spec);
    BYTE size = MP_GetFieldSpecSize(spec);
    INTEGER shift = WordSize - (pos + size + 1);
    if (shift < 0) MP_Error();
    return (MP_Shift(source, (INTEGER)-shift) & MP_MaskTable[size]);
}

__mpinline UNSPECIFIED MP_WriteField(UNSPECIFIED dest, FieldSpec spec, UNSPECIFIED data)
{
    BYTE pos = MP_GetFieldSpecPos(spec);
    BYTE size = MP_GetFieldSpecSize(spec);
    INTEGER shift = WordSize - (pos + size + 1);
    UNSPECIFIED mask;
    if (shift < 0) MP_Error();
    mask = MP_Shift(MP_MaskTable[size], shift);
    data = MP_Shift(data, shift) & mask;
    return ((dest & ~mask) | data);
}

__mpinline void MP_WriteSwappedZeroField()
{
    FieldSpec spec = MP_GetCodeByte();
    UNSPECIFIED data = MP_Pop();
    POINTER ptr = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds(ptr), MP_WriteField(MP_ReadMesaRealAddress(MP_FetchMds(ptr)), spec, data));
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_ASSIGNMENTINSTRUCTIONS_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

