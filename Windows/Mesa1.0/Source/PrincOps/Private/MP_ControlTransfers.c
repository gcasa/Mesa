/*
    MP_ControlTransfers.c - implementation of Mesa processor control transfer instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include <PrincOps/PrincOps_Microcode.h>

/*******************************************************************************
** CONTROL TRANSFER INSTRUCTIONS */

/* Frame Allocation Instructions */
void
MP_Instruction_AF()
{
    /* Allocate Frame */
    FSIndex fsi = (BYTE)MP_Pop();
    MP_Push(MP_Alloc(fsi));
}

void
MP_Instruction_FF()
{
    /* Free Frame */
    LocalFrameHandle frame = MP_Pop();
    MP_Free(frame);
}

/* Control transfer instructions */
void
MP_Instruction_LFC()
{
    /* Local Function Call */
    UNSPECIFIED word;
    LocalFrameHandle nLF;
    /* NOTE: Unlike in the book we call GetCodeWord() before stashing the PC in the LF */
    CARDINAL nPC = MP_GetCodeWord();
    MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseProgramCounterOffset(MP_Mesa_Processor.registers.LF)), MP_Mesa_Processor.registers.PC);
    if (0 == nPC) MP_UnboundTrap(0);
    word = MP_ReadCode((CARDINAL)(nPC / 2));
    nLF = MP_Alloc((BYTE)((nPC & 1) ? MP_Right(word) : MP_Left(word)));
    nPC++;
    MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseGlobalLinkOffset(nLF)), MP_Mesa_Processor.registers.GFI);
    MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseReturnLinkOffset(nLF)), MP_Mesa_Processor.registers.LF);
    MP_Mesa_Processor.registers.LF = nLF;
    MP_Mesa_Processor.registers.PC = nPC;
    MP_CheckForXferTraps(((ShortProcDesc)(((MP_Mesa_Processor.registers.GF | 1) << 16) | (MP_Mesa_Processor.registers.PC - 1))), XFERType_local);
}

void
MP_Instruction_EFC0()
{
    /* External Function Call 0 */
    MP_Call(MP_FetchLink(0));
}

void
MP_Instruction_EFC1()
{
    /* External Function Call 1 */
    MP_Call(MP_FetchLink(1));
}

void
MP_Instruction_EFC2()
{
    /* External Function Call 2 */
    MP_Call(MP_FetchLink(2));
}

void
MP_Instruction_EFC3()
{
    /* External Function Call 3 */
    MP_Call(MP_FetchLink(3));
}

void
MP_Instruction_EFC4()
{
    /* External Function Call 4 */
    MP_Call(MP_FetchLink(4));
}

void
MP_Instruction_EFC5()
{
    /* External Function Call 5 */
    MP_Call(MP_FetchLink(5));
}

void
MP_Instruction_EFC6()
{
    /* External Function Call 6 */
    MP_Call(MP_FetchLink(6));
}

void
MP_Instruction_EFC7()
{
    /* External Function Call 7 */
    MP_Call(MP_FetchLink(7));
}

void
MP_Instruction_EFC8()
{
    /* External Function Call 8 */
    MP_Call(MP_FetchLink(8));
}

void
MP_Instruction_EFC9()
{
    /* External Function Call 9 */
    MP_Call(MP_FetchLink(9));
}

void
MP_Instruction_EFC10()
{
    /* External Function Call 10 */
    MP_Call(MP_FetchLink(10));
}

void
MP_Instruction_EFC11()
{
    /* External Function Call 11 */
    MP_Call(MP_FetchLink(11));
}

void
MP_Instruction_EFC12()
{
    /* External Function Call 12 */
    MP_Call(MP_FetchLink(12));
}

void
MP_Instruction_EFCB()
{
    /* External Function Call Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Call(MP_FetchLink(alpha));
}

void
MP_Instruction_SFC()
{
    /* Stack Function Call */
    ControlLink link = MP_PopLong();
    MP_Call(link);
}

void
MP_Instruction_KFCB()
{
    /* Kernel Function Call Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Call(MP_ReadDblMds(MP_GetSDOffset(alpha)));
}

void
MP_Instruction_LKB()
{
    /* Link Byte */
    BYTE alpha = MP_GetCodeByte();
    ShortControlLink link;
    MP_Recover();
    link = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds(MP_Mesa_Processor.registers.LF), (UNSPECIFIED)(link - alpha));
}

void
MP_Instruction_RET()
{
    /* Return */
    ControlLink dst = (LONG_UNSPECIFIED)MP_ReadMesaRealAddress(MP_FetchMds(MP_LocalBaseReturnLinkOffset(MP_Mesa_Processor.registers.LF)));
    MP_XFER(dst, 0, XFERType_return, MESA_TRUE);
}

void
MP_Instruction_PO()
{
    /* Port Out */
    MP_PortOut();
}

void
MP_Instruction_POR()
{
    /* Port Out Responding */
    MP_PortOut();
}

void
MP_Instruction_PI()
{
    /* Port In */
    PortLink port;
    ShortControlLink src;
    MP_Recover();
    MP_Recover();
    src = MP_Pop();
    port = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds(MP_GetPortLinkInPortOffset(port)), 0);
    if (src != 0) MP_WriteMesaRealAddress(MP_StoreMds(MP_GetPortLinkOutPortOffset(port)), src);
}

void
MP_Instruction_LLKB()
{
    /* Load Link Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_PushLong(MP_FetchLink(alpha));
}

void
MP_Instruction_RKIB()
{
    /* Read Link Indirect Byte */
    BYTE alpha = MP_GetCodeByte();
    LONG_POINTER ptr = MP_FetchLink(alpha);
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(ptr)));
}

void
MP_Instruction_RKDIB()
{
    /* Read Link Double Indirect Byte */
    BYTE alpha = MP_GetCodeByte();
    LONG_POINTER ptr = MP_FetchLink(alpha);
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(ptr)));
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(ptr + 1)));
}

/* Traps */
void
MP_Instruction_DSK()
{
    /* Dump Stack */
    BYTE alpha = MP_GetCodeByte();
    POINTER state = (POINTER)(MP_Mesa_Processor.registers.LF + alpha);
    MP_SaveStack(MP_LengthenPointer(state));
}

void
MP_Instruction_LSK()
{
    /* Load Stack */
    BYTE alpha = MP_GetCodeByte();
    POINTER state = (POINTER)(MP_Mesa_Processor.registers.LF + alpha);
    MP_LoadStack(MP_LengthenPointer(state));
}

void
MP_Instruction_XF()
{
    /* XFER and Free */
#if 0
    /* book implementation */
    BYTE alpha = MP_GetCodeByte();
    POINTER ptr = (POINTER)(MP_Mesa_Processor.registers.LF + alpha);
    MP_XFER(MP_ReadDblMds(MP_GetTransferDescriptorDstOffset(ptr)), MP_ReadMesaRealAddress(MP_FetchMds(MP_GetTransferDescriptorSrcOffset(ptr))), XFERType_xfer, MESA_TRUE);
#else
    /*
       ajax tests require that the PC of the next instruction be stored in the local frame about to be freed
       we do this before any traps can happen - don't know why, but that's what the gvwin mvm does
    */
    BYTE alpha;
    POINTER ptr;

    MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseProgramCounterOffset(MP_Mesa_Processor.registers.LF)), (CARDINAL)(MP_Mesa_Processor.registers.PC+1));
    
    alpha = MP_GetCodeByte();
    ptr = (POINTER)(MP_Mesa_Processor.registers.LF + alpha);
    MP_XFER(MP_ReadDblMds(MP_GetTransferDescriptorDstOffset(ptr)), MP_ReadMesaRealAddress(MP_FetchMds(MP_GetTransferDescriptorSrcOffset(ptr))), XFERType_xfer, MESA_TRUE);

#endif
}

void
MP_Instruction_XE()
{
    /* XFER and Enable */
#if 0
    /* This is the book version */
    BYTE alpha;
    POINTER ptr;

    MP_Mesa_Processor.state.terminate = MESA_TRUE; /* Equivalant to BEGIN ENABLE Abort => ERROR; */

    alpha = MP_GetCodeByte();
    ptr = (POINTER)(MP_Mesa_Processor.registers.LF + alpha);
    MP_XFER(MP_ReadDblMds(MP_GetTransferDescriptorDstOffset(ptr)), MP_ReadMesaRealAddress(MP_FetchMds(MP_GetTransferDescriptorSrcOffset(ptr))), XFERType_xfer, MESA_FALSE);
    MP_EnableInterrupts();

    MP_Mesa_Processor.state.terminate = MESA_FALSE; /* Equivalant to END ENABLE; */
#else
    /* Actual MC version  - ajax */
    BYTE alpha;
    POINTER ptr;

    /* The actual microcode checks for interrupt error and enables BEFORE the XFER. */
    if (MP_Mesa_Processor.registers.WDC == 0) MP_InterruptError();
    MP_EnableInterrupts();

    /* The actual microcode writes the next instruction into the local frame's program counter */
    MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseProgramCounterOffset(MP_Mesa_Processor.registers.LF)), (CARDINAL)(MP_Mesa_Processor.registers.PC+1));

    MP_Mesa_Processor.state.terminate = MESA_TRUE; /* Equivalant to BEGIN ENABLE Abort => ERROR; */

    alpha = MP_GetCodeByte();
    ptr = (POINTER)(MP_Mesa_Processor.registers.LF + alpha);
    MP_XFER(MP_ReadDblMds(MP_GetTransferDescriptorDstOffset(ptr)), MP_ReadMesaRealAddress(MP_FetchMds(MP_GetTransferDescriptorSrcOffset(ptr))), XFERType_xfer, MESA_FALSE);

    MP_Mesa_Processor.state.terminate = MESA_FALSE; /* Equivalant to END ENABLE; */

#endif
}

void
MP_Instruction_BRK()
{
    /* Break */
    if (MP_Mesa_Processor.state.breakbyte == 0) MP_BreakTrap();
    else
    {
        MP_Dispatch(MP_Mesa_Processor.state.breakbyte);
        MP_Mesa_Processor.state.breakbyte = 0;
    }
}

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
