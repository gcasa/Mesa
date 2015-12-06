/*
    MP_InstructionInterpreter.h - internal header file of Mesa Processor Instruction Interpreter

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_INSTRUCTIONINTERPRETER_H
#define _INCLUDE_PRINCOPS_MP_MP_INSTRUCTIONINTERPRETER_H

/*******************************************************************************
** INSTRUCTION INTERPRETER FUNCTIONS */
void MP_Processor();        /* Mesa Processor main loop */

/*******************************************************************************
** INSTRUCTION INTERPRETER INLINE FUNCTIONS */
__mpinline BYTE MP_GetCodeByte();
__mpinline UNSPECIFIED MP_GetCodeWord();
__mpinline void MP_Dispatch(BYTE opcode);
__mpinline void MP_Execute();
__mpinline void MP_ErrorAbort();
__mpinline void MP_Error();

/*******************************************************************************
** INSTRUCTION INTERPRETER OPTIMIZATION FUNCTIONS */
#ifndef _MP_BY_THE_BOOK

void MP_Dispatch_EscOpcode();
void MP_Dispatch_EscLOpcode();
void MP_Dispatch_OpcodeTRAP();
void MP_Dispatch_EscOpcodeTRAP();

#endif

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_INSTRUCTIONINTERPRETER_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

