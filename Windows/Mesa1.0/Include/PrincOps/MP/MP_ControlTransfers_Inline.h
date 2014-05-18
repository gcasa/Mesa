/*
    MP_ControlTransfers.h - header file of Mesa Processor control transfer inline functions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_CONTROLTRANSFERS_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_CONTROLTRANSFERS_INLINE_H

/*******************************************************************************
** CONTROL TRANSFER INLINE FUNCTIONS */

__mpinline LONG_POINTER MP_GetGFTEntryOffset(CARDINAL gftHandle)
{
    return mGFT + ((LONG_POINTER)gftHandle);
}

__mpinline LONG_POINTER MP_GetGFTEntryGlobalFrameOffset(LONG_POINTER gftEntry)
{
    return gftEntry + 0;
}

__mpinline LONG_POINTER MP_GetGFTEntryCodebaseOffset(LONG_POINTER gftEntry)
{
    return gftEntry + 2;
}

__mpinline POINTER MP_GetSDOffset(SDIndex index)
{
    return ((((CARDINAL)index) * 2) + SD);
}

__mpinline UNSPECIFIED MP_GetTaggedControlLinkData(ControlLink controlLink)
{
    return (UNSPECIFIED)(MP_LowHalf(controlLink) >> 2);
}

__mpinline LinkType MP_GetTaggedControlLinkType(ControlLink controlLink)
{
    switch (MP_LowHalf(controlLink) & 0x3)
    {
        case 0:     return LinkType_frame;
        case 1:     return LinkType_shortProc;
        case 2:     return LinkType_indirect;
        default:    return LinkType_longProc;
    }
}

__mpinline UNSPECIFIED MP_GetTaggedControlLinkFill(ControlLink controlLink)
{
    return (UNSPECIFIED)(MP_HighHalf(controlLink));
}

__mpinline LinkType MP_ControlLinkType(ControlLink controlLink)
{
    return MP_GetTaggedControlLinkType(controlLink);
}

__mpinline FrameLink MP_MakeFrameLink(ControlLink controlLink)
{
    if (MP_ControlLinkType(controlLink) != LinkType_frame) MP_Error();
    return (FrameLink)(MP_LowHalf(controlLink));
}

__mpinline IndirectLink MP_MakeIndirectLink(ControlLink controlLink)
{
    if (MP_ControlLinkType(controlLink) != LinkType_indirect) MP_Error();
    return (IndirectLink)(MP_LowHalf(controlLink));
}

__mpinline UNSPECIFIED MP_GetTaggedShortProcDescGF(ShortProcDesc shortProcDesc)
{
    return (UNSPECIFIED)(MP_LowHalf(shortProcDesc));
}

__mpinline CARDINAL MP_GetTaggedShortProcDescPC(ShortProcDesc shortProcDesc)
{
    return (CARDINAL)(MP_HighHalf(shortProcDesc));
}

__mpinline ShortProcDesc MP_MakeShortProcDesc(ControlLink controlLink)
{
    if (MP_ControlLinkType(controlLink) != LinkType_shortProc) MP_Error();
    return ((ShortProcDesc)controlLink);
}

__mpinline UNSPECIFIED MP_GetTaggedLongProcDescGFTHandle(LongProcDesc longProcDesc)
{
    return (UNSPECIFIED)(MP_LowHalf(longProcDesc));
}

__mpinline CARDINAL MP_GetTaggedLongProcDescPC(LongProcDesc longProcDesc)
{
    return (CARDINAL)(MP_HighHalf(longProcDesc));
}

__mpinline LongProcDesc MP_MakeLongProcDesc(ControlLink controlLink)
{
    if (MP_ControlLinkType(controlLink) != LinkType_longProc) MP_Error();
    return ((LongProcDesc)controlLink);
}

__mpinline UNSPECIFIED MP_GetTaggedAVItemData(AVItem avi)
{
    return (UNSPECIFIED)(avi >> 2);
}

__mpinline AVTag MP_GetTaggedAVItemTag(AVItem avi)
{
    return (AVTag)(avi & 0x3);
}

__mpinline LocalFrameHandle MP_AVFrame(AVItem avi)
{
    if (MP_GetTaggedAVItemTag(avi) != AVTag_frame) MP_Error();
    return (LocalFrameHandle)avi;
}

__mpinline POINTER MP_AVLink(AVItem avi)
{
    if (MP_GetTaggedAVItemTag(avi) != AVTag_frame) MP_Error();
    return (POINTER)avi;
}

__mpinline LocalFrameHandle MP_Alloc(FSIndex fsi)
{
    AVItem item;
    FSIndex slot = fsi;
    while (1)
    {
        item = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(AV + slot)));
        if (MP_GetTaggedAVItemTag(item) != AVTag_indirect) break;
        if (MP_GetTaggedAVItemData(item) > 255) MP_Error();
        slot = (FSIndex)MP_GetTaggedAVItemData(item);
    }
    if (MP_GetTaggedAVItemTag(item) == AVTag_empty) MP_FrameFault(fsi);
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(AV + slot)), MP_ReadMesaRealAddress(MP_FetchMds(MP_AVLink(item))));
    return MP_AVFrame(item);
}

__mpinline void MP_Free(LocalFrameHandle frame)
{
    LocalWord word = MP_ReadMesaRealAddress(MP_FetchMds(MP_LocalBaseWordOffset(frame)));
    AVItem item = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(AV + MP_LocalWordFSI(word))));
    MP_WriteMesaRealAddress(MP_StoreMds(frame), item);
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(AV + MP_LocalWordFSI(word))), frame);
}

__mpinline void MP_XFER(ControlLink dst, ShortControlLink src, XFERType type, BOOLEAN free)
{
    CARDINAL nPC;
    LocalFrameHandle nLF;
    BOOLEAN push = MESA_FALSE;
    ControlLink nDst = dst;
    if ((type == XFERType_trap) && (MESA_TRUE == free)) MP_Error();
    while (MP_ControlLinkType(nDst) == LinkType_indirect)
    {
        IndirectLink link = MP_MakeIndirectLink(nDst);
        if (type == XFERType_trap) MP_Error();
        nDst = MP_ReadDblMds(link);
        push = MESA_TRUE;
    }
    switch (MP_ControlLinkType(nDst))
    {
        case LinkType_frame:
            {
                FrameLink frame = MP_MakeFrameLink(nDst);
                if (0 == frame) MP_ControlTrap(src);
                nLF = frame;
                MP_Mesa_Processor.registers.GFI = MP_ReadMesaRealAddress(MP_FetchMds(MP_LocalBaseGlobalLinkOffset(nLF)));
                if (0 == MP_Mesa_Processor.registers.GFI) MP_UnboundTrap(dst);
                MP_Mesa_Processor.registers.GF = MP_ReadDbl(MP_GetGFTEntryGlobalFrameOffset(MP_GetGFTEntryOffset(MP_Mesa_Processor.registers.GFI)));
                if (0 == MP_Mesa_Processor.registers.GF) MP_UnboundTrap(dst);
                MP_Mesa_Processor.registers.CB = MP_ReadDbl(MP_GetGFTEntryCodebaseOffset(MP_GetGFTEntryOffset(MP_Mesa_Processor.registers.GFI)));
                if (MP_Odd(MP_LowHalf(MP_Mesa_Processor.registers.CB))) MP_CodeTrap(MP_Mesa_Processor.registers.GFI);
                nPC = MP_ReadMesaRealAddress(MP_FetchMds(MP_LocalBaseProgramCounterOffset(nLF)));
                if (0 == nPC) MP_UnboundTrap(dst);
                if (type == XFERType_trap)
                {
                    MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseReturnLinkOffset(nLF)), src);
                    MP_DisableInterrupts();
                }
            }
            break;
        case LinkType_shortProc:
            {
                GlobalWord globalWord;
                UNSPECIFIED word;
                ShortProcDesc proc = MP_MakeShortProcDesc(nDst);
                MP_Mesa_Processor.registers.GF = MP_And(MP_GetTaggedShortProcDescGF(proc), 0xFFFC);
                if (0 == MP_Mesa_Processor.registers.GF) MP_UnboundTrap(dst);
                globalWord = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_GlobalBaseGlobalWordOffset(MP_Mesa_Processor.registers.GF))));
                MP_Mesa_Processor.registers.GFI = MP_And(globalWord, 0xFFFC);
                
                if (0 == MP_Mesa_Processor.registers.GFI) MP_UnboundTrap(dst);
                MP_Mesa_Processor.registers.GF = MP_ReadDbl(MP_GetGFTEntryGlobalFrameOffset(MP_GetGFTEntryOffset(MP_Mesa_Processor.registers.GFI)));
                if (0 == MP_Mesa_Processor.registers.GF) MP_UnboundTrap(dst);
                MP_Mesa_Processor.registers.CB = MP_ReadDbl(MP_GetGFTEntryCodebaseOffset(MP_GetGFTEntryOffset(MP_Mesa_Processor.registers.GFI)));
                if (MP_Odd(MP_LowHalf(MP_Mesa_Processor.registers.CB))) MP_CodeTrap(MP_Mesa_Processor.registers.GFI);
                nPC = MP_GetTaggedShortProcDescPC(proc);
                if (0 == nPC) MP_UnboundTrap(dst);
                word = MP_ReadCode((CARDINAL)(nPC / 2));
                nLF = MP_Alloc((BYTE)((nPC & 1) ? MP_Right(word) : MP_Left(word)));
                nPC++;
                MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseGlobalLinkOffset(nLF)), MP_Mesa_Processor.registers.GFI);
                MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseReturnLinkOffset(nLF)), src);
            }
            break;
        case LinkType_longProc:
            {
                /* Note GFI is actually a GFT Handle not a GFT Index */
                UNSPECIFIED word;
                LongProcDesc proc = MP_MakeLongProcDesc(nDst);
                MP_Mesa_Processor.registers.GFI = MP_And(MP_GetTaggedLongProcDescGFTHandle(proc), 0xFFFC);
                if (0 == MP_Mesa_Processor.registers.GFI) MP_UnboundTrap(dst);
                MP_Mesa_Processor.registers.GF = MP_ReadDbl(MP_GetGFTEntryGlobalFrameOffset(MP_GetGFTEntryOffset(MP_Mesa_Processor.registers.GFI)));
                if (0 == MP_Mesa_Processor.registers.GF) MP_UnboundTrap(dst);
                MP_Mesa_Processor.registers.CB = MP_ReadDbl(MP_GetGFTEntryCodebaseOffset(MP_GetGFTEntryOffset(MP_Mesa_Processor.registers.GFI)));
                if (MP_Odd(MP_LowHalf(MP_Mesa_Processor.registers.CB))) MP_CodeTrap(MP_Mesa_Processor.registers.GFI);
                nPC = MP_GetTaggedLongProcDescPC(proc);
                if (0 == nPC) MP_UnboundTrap(dst);
                word = MP_ReadCode((CARDINAL)(nPC / 2));
                nLF = MP_Alloc((BYTE)((nPC & 1) ? MP_Right(word) : MP_Left(word)));
                nPC++;
                MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseGlobalLinkOffset(nLF)), MP_Mesa_Processor.registers.GFI);
                MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseReturnLinkOffset(nLF)), src);
            }
            break;
        default:
            MP_Error();
            break;
    }
    if (MESA_TRUE == push)
    {
        MP_Push(MP_LowHalf(dst));
        MP_Push(src);
        MP_Discard();
        MP_Discard();
    }
    if (MESA_TRUE == free) MP_Free(MP_Mesa_Processor.registers.LF);
    MP_Mesa_Processor.registers.LF = nLF;
    MP_Mesa_Processor.registers.PC = nPC;
    MP_CheckForXferTraps(dst, type);
}

__mpinline ControlLink MP_FetchLink(BYTE offset)
{
    GlobalWord word = MP_ReadMesaRealAddress(MP_Fetch(MP_GlobalBaseGlobalWordOffset(MP_Mesa_Processor.registers.GF)));
    if (MP_GlobalWordCodeLinks(word)) return MP_ReadDbl(MP_Mesa_Processor.registers.CB - ((LONG_POINTER)((offset + 1) * 2)));
    else return MP_ReadDbl(MP_GlobalBase(MP_Mesa_Processor.registers.GF) - ((offset + 1) * 2));
}

__mpinline void MP_Call(ControlLink dst)
{
    MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseProgramCounterOffset(MP_Mesa_Processor.registers.LF)), MP_Mesa_Processor.registers.PC);
    MP_XFER(dst, MP_Mesa_Processor.registers.LF, XFERType_call, MESA_FALSE);
}

__mpinline POINTER MP_GetPortLinkInPortOffset(PortLink portLink)
{
    return (POINTER)(portLink + PORTLINKINPORTOFFSET);
}

__mpinline POINTER MP_GetPortLinkOutPortOffset(PortLink portLink)
{
    return (POINTER)(portLink + PORTLINKOUTPORTOFFSET);
}

__mpinline void MP_PortOut()
{
#if 0
	UNSPECIFIED reserved = MP_Pop();
	PortLink port = MP_Pop();
#else
	PortLink port;
	MP_Pop();
	port = MP_Pop();
#endif

	MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseProgramCounterOffset(MP_Mesa_Processor.registers.LF)), MP_Mesa_Processor.registers.PC);
    MP_WriteMesaRealAddress(MP_StoreMds(MP_GetPortLinkInPortOffset(port)), MP_Mesa_Processor.registers.LF);
    MP_XFER(MP_ReadDblMds(MP_GetPortLinkOutPortOffset(port)), port, XFERType_port, MESA_FALSE);
}

__mpinline BOOLEAN MP_ValidContext()
{
    return ((MP_Mesa_Processor.registers.PC >= (CODESEGMENTSIZE * 2)) ? MESA_TRUE : MESA_FALSE);
}

__mpinline void MP_Trap(POINTER ptr)
{
    ControlLink handler = MP_ReadDblMds(ptr);
    MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
    MP_Mesa_Processor.registers.SP = MP_Mesa_Processor.state.savedSP;
    if (MP_ValidContext()) MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseProgramCounterOffset(MP_Mesa_Processor.registers.LF)), MP_Mesa_Processor.registers.PC);
    MP_XFER(handler, MP_Mesa_Processor.registers.LF, XFERType_trap, MESA_FALSE);
}

__mpinline void MP_TrapZero(POINTER ptr)
{
    MP_Trap(ptr);
    MP_ErrorAbort();
}

__mpinline void MP_TrapOne(POINTER ptr, UNSPECIFIED parameter)
{
    MP_Trap(ptr);
    MP_WriteMesaRealAddress(MP_StoreMds(MP_Mesa_Processor.registers.LF), parameter);
    MP_ErrorAbort();
}

__mpinline void MP_TrapTwo(POINTER ptr, LONG_UNSPECIFIED parameter)
{
    MP_Trap(ptr);
    MP_WriteMesaRealAddress(MP_StoreMds(MP_Mesa_Processor.registers.LF), MP_LowHalf(parameter));
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + 1)), MP_HighHalf(parameter));
    MP_ErrorAbort();
}

__mpinline void MP_BoundsTrap()
{
    MP_TrapZero(MP_GetSDOffset(sBoundsTrap));
}

__mpinline void MP_BreakTrap()
{
    MP_TrapZero(MP_GetSDOffset(sBreakTrap));
}

__mpinline void MP_CodeTrap(CARDINAL gfi)
{
    MP_TrapOne(MP_GetSDOffset(sCodeTrap), gfi);
}

__mpinline void MP_ControlTrap(ShortControlLink src)
{
    MP_TrapOne(MP_GetSDOffset(sControlTrap), src);
}

__mpinline void MP_DivCheckTrap()
{
    MP_TrapZero(MP_GetSDOffset(sDivCheckTrap));
}

__mpinline void MP_DivZeroTrap()
{
    MP_TrapZero(MP_GetSDOffset(sDivZeroTrap));
}

__mpinline void MP_EscOpcodeTrap(BYTE opcode)
{
    MP_TrapOne((POINTER)(ETT + ((POINTER)opcode * ETTEntrySize)), opcode);
}

__mpinline void MP_InterruptError()
{
    MP_TrapZero(MP_GetSDOffset(sInterruptError));
}

__mpinline void MP_OpcodeTrap(BYTE opcode)
{
    MP_TrapOne(MP_GetSDOffset(sOpcodeTrap), opcode);
}

__mpinline void MP_PointerTrap()
{
    MP_TrapZero(MP_GetSDOffset(sPointerTrap));
}

__mpinline void MP_ProcessTrap()
{
    MP_TrapZero(MP_GetSDOffset(sProcessTrap));
}

__mpinline void MP_RescheduleError()
{
    MP_TrapZero(MP_GetSDOffset(sRescheduleError));
}

__mpinline void MP_StackError()
{
    MP_TrapZero(MP_GetSDOffset(sStackError));
}

__mpinline void MP_UnboundTrap(ControlLink dst)
{
    MP_TrapTwo(MP_GetSDOffset(sUnboundTrap), dst);
}

__mpinline void MP_HardwareError()
{
    MP_TrapZero(MP_GetSDOffset(sHardwareError));
}

__mpinline LONG_POINTER MP_GetStateHandleStackOffset(StateHandle stateHandle)
{
    return (stateHandle + STATEVECTORSTACKOFFSET);
}

__mpinline LONG_POINTER MP_GetStateHandleStateWordOffset(StateHandle stateHandle)
{
    return (stateHandle + STATEVECTORSTATEWORDOFFSET);
}

__mpinline LONG_POINTER MP_GetStateHandleFrameOffset(StateHandle stateHandle)
{
    return (stateHandle + STATEVECTORFRAMEOFFSET);
}

__mpinline LONG_POINTER MP_GetStateHandleDataZeroOffset(StateHandle stateHandle)
{
    return (stateHandle + STATEVECTORDATAZEROOFFSET);
}

__mpinline LONG_POINTER MP_GetStateHandleDataOneOffset(StateHandle stateHandle)
{
    return (stateHandle + STATEVECTORDATAONEOFFSET);
}

__mpinline StateWord MP_MakeStateWord(BYTE breakByte, CARDINAL sp)
{
    return ((((UNSPECIFIED)breakByte) << 8) | (sp & 0xF));
}

__mpinline CARDINAL MP_GetStateWordStackPointer(StateWord stateWord)
{
    return (stateWord & 0xF);
}

__mpinline BYTE MP_GetStateWordBreakByte(StateWord stateWord)
{
    return (BYTE)(stateWord >> 8);
}

__mpinline void MP_SaveStack(StateHandle state)
{
    CARDINAL sp;
    CARDINAL depth = MP_Mesa_Processor.registers.SP + 2;
    if (depth > StackDepth) depth = StackDepth;
    for (sp = 0; sp < depth; sp++)
    {
        MP_WriteMesaRealAddress(MP_Store(MP_GetStateHandleStackOffset(state) + sp), MP_Mesa_Processor.registers.stack[sp]);
    }
    MP_WriteMesaRealAddress(MP_Store(MP_GetStateHandleStateWordOffset(state)), MP_MakeStateWord(MP_Mesa_Processor.state.breakbyte, MP_Mesa_Processor.registers.SP));
    MP_Mesa_Processor.registers.SP =  MP_Mesa_Processor.state.savedSP = 0;
    MP_Mesa_Processor.state.breakbyte = 0;
}

__mpinline void MP_LoadStack(StateHandle state)
{
    CARDINAL sp;
    StateWord stateWord = MP_ReadMesaRealAddress(MP_Fetch(MP_GetStateHandleStateWordOffset(state)));
    CARDINAL depth = MP_GetStateWordStackPointer(stateWord) + 2;
    if (depth > StackDepth) depth = StackDepth;
    for (sp = 0; sp < depth; sp++)
    {
        MP_Mesa_Processor.registers.stack[sp] = MP_ReadMesaRealAddress(MP_Fetch(MP_GetStateHandleStackOffset(state) + sp));
    }
    MP_Mesa_Processor.registers.SP =  MP_Mesa_Processor.state.savedSP = MP_GetStateWordStackPointer(stateWord);
    MP_Mesa_Processor.state.breakbyte = MP_GetStateWordBreakByte(stateWord);
}

__mpinline POINTER MP_GetTransferDescriptorSrcOffset(TransferDescriptor transferDescriptor)
{
    return (transferDescriptor + TRANSFERDESCRIPTORSRCOFFSET);
}

__mpinline POINTER MP_GetTransferDescriptorDstOffset(TransferDescriptor transferDescriptor)
{
    return (transferDescriptor + TRANSFERDESCRIPTORDSTOFFSET);
}

__mpinline void MP_CheckForXferTraps(ControlLink dst, XFERType type)
{
    if (MP_Odd(MP_Mesa_Processor.registers.XTS))
    {
        GlobalWord word = MP_ReadMesaRealAddress(MP_Fetch(MP_GlobalBaseGlobalWordOffset(MP_Mesa_Processor.registers.GF)));
        if (MP_GlobalWordTrapXfers(word))
        {
            MP_Mesa_Processor.registers.XTS = MP_Shift(MP_Mesa_Processor.registers.XTS, -1);

            MP_Mesa_Processor.state.terminate = MESA_TRUE; /* Equivalant to BEGIN ENABLE Abort => ERROR; */
            MP_Trap(MP_GetSDOffset(sXferTrap));    
            MP_Mesa_Processor.state.terminate = MESA_FALSE; /* Equivalant to END ENABLE; */

            MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + 0)), MP_LowHalf(dst));
            MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + 1)), MP_HighHalf(dst));
            MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(MP_Mesa_Processor.registers.LF + 2)), ((UNSPECIFIED)type));

            MP_ErrorAbort();
        }
    }
    else MP_Mesa_Processor.registers.XTS = MP_Shift(MP_Mesa_Processor.registers.XTS, -1);
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_CONTROLTRANSFERS_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

