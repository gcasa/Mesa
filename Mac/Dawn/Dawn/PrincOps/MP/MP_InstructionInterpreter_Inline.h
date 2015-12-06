/*
    MP_InstructionInterpreter.h - internal header file of Mesa Processor Instruction Interpreter

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_INSTRUCTIONINTERPRETER_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_INSTRUCTIONINTERPRETER_INLINE_H

/*******************************************************************************
** INSTRUCTION INTERPRETER INLINE FUNCTIONS */

#ifdef _MP_BY_THE_BOOK

__mpinline BYTE MP_GetCodeByte()
{
    CARDINAL pc = MP_Mesa_Processor.registers.PC;
    UNSPECIFIED code = MP_ReadCode((CARDINAL)(pc / 2));
    MP_Mesa_Processor.registers.PC++;
    return ((pc & 1) ? MP_Right(code) : MP_Left(code));
}

#else

__mpinline BYTE MP_GetCodeByte()
{
    VirtualAddress nVirtualAddress = MP_Mesa_Processor.registers.CB + (MP_Mesa_Processor.registers.PC >> 1);
    VirtualPageNumber nVirtualCodePage = nVirtualAddress >> 8;

    if ((MP_Mesa_Processor.registers.CB != MP_Mesa_Processor.state.cachedCB) || (nVirtualCodePage != MP_Mesa_Processor.state.cachedVirtualCodePage))
    {
        // cache miss, reload the cache
        RealAddress nRealAddress = MP_Map(nVirtualCodePage << 8, MapOp_Read);
        MP_Mesa_Processor.state.cachedVirtualCodePage = nVirtualCodePage;
        MP_Mesa_Processor.state.cachedCB = MP_Mesa_Processor.registers.CB;
        MP_Mesa_Processor.state.pCodePageCache = (BYTE *)(MP_Mesa_Processor.config.m_pRealMemoryBaseAddress + nRealAddress);
   }
    
    // read the cache
    // NOTE: we increment the PC *first* because byte ordering is backwards in raw 16 bit words
    //       incrementing first is equivalent to adding (~MP_Mesa_Processor.registers.PC & 1) which is the intent.
    // ISSUE: this is Platform Dependent!!! (for platform independent code #define _MP_BY_THE_BOOK)
    MP_Mesa_Processor.registers.PC++;
    return *(MP_Mesa_Processor.state.pCodePageCache + ((nVirtualAddress & 0xFF) * 2) + (MP_Mesa_Processor.registers.PC & 1));
}

#endif

__mpinline UNSPECIFIED MP_GetCodeWord()
{
    UNSPECIFIED word = ((UNSPECIFIED)MP_GetCodeByte()) << 8;
    word |= (UNSPECIFIED)MP_GetCodeByte();
    return word;
}

__mpinline void MP_Dispatch(BYTE opcode)
{
    InstructionFunction * pInstructionFunction;

    switch (opcode)
    {
        case 0xF8:   // zESC
            opcode = MP_GetCodeByte();
            pInstructionFunction = MP_Mesa_OpcodeEsc_Table[opcode];
            if (pInstructionFunction) (*pInstructionFunction)(); else MP_EscOpcodeTrap(opcode);
            break;
        case 0xF9:
            opcode = MP_GetCodeByte();
            pInstructionFunction = MP_Mesa_OpcodeEscL_Table[opcode];
            if (pInstructionFunction) (*pInstructionFunction)(); else MP_EscOpcodeTrap(opcode);
            break;
        default:
            pInstructionFunction = MP_Mesa_Opcode_Table[opcode];
            if (pInstructionFunction) (*pInstructionFunction)(); else MP_OpcodeTrap(opcode);
            break;
    }

    MP_Mesa_Processor.state.instructionsDispatched++;
}

__mpinline void MP_Execute()
{
    MP_Mesa_Processor.state.savedPC = MP_Mesa_Processor.registers.PC;    
    MP_Mesa_Processor.state.savedSP = MP_Mesa_Processor.registers.SP;
    MP_Dispatch(MP_GetCodeByte());  
}

__mpinline void MP_ErrorAbort()
{
    longjmp(MP_Mesa_Processor.state.jmp_buf_ABORT, -1);
}

__mpinline void MP_Error()
{
    MP_Mesa_Processor.state.terminate = MESA_TRUE;
    MP_ErrorAbort();  
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_INSTRUCTIONINTERPRETER_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

