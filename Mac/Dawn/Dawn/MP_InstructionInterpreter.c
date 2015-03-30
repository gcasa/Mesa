/*
    MP_InstructionInterpreter.c - implementation of Mesa processor instruction interpreter

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include "PrincOps/PrincOps_Microcode.h"

/*******************************************************************************
** module Instruction Interpreter static variables */

/*******************************************************************************
** Instruction Interpreter FUNCTIONS */

static __mpinline void
MP_Initialize()
{
    /* State */
    MP_Mesa_Processor.state.terminate = MESA_FALSE;


    /* PrincOps initialization */

    /* Process registers */
    MP_Mesa_Processor.registers.WP = 0;
    (*MP_Mesa_Processor.config.m_pSetInterruptMaskFunction)(MP_Mesa_Processor.registers.WP);
    MP_Mesa_Processor.registers.WDC = 1; 
    MP_Mesa_Processor.registers.XTS = 0;
    MP_Mesa_Processor.state.time = (MP_Mesa_Processor.registers.IT = (*MP_Mesa_Processor.config.m_pGetPulseCountFunction)());
    MP_Mesa_Processor.state.running = MESA_TRUE;
    
    /* Context initialization */
    MP_Mesa_Processor.registers.SP = 0;
    MP_Mesa_Processor.state.breakbyte = 0;
    MP_Mesa_Processor.registers.PSB = 0;
    MP_Mesa_Processor.registers.MDS = 0;

    /* GFI initialization */
    MP_Mesa_Processor.registers.GFI = 0;

    /* First XFER */
    MP_XFER(MP_GetSDOffset(sBoot), 0, XFERType_call, MESA_FALSE);
}

#ifdef _MP_BY_THE_BOOK

void
MP_Processor()
{
    MP_Initialize();
    while (MESA_FALSE == MP_Mesa_Processor.state.terminate)
    {
        int errorABORT = setjmp(MP_Mesa_Processor.state.jmp_buf_ABORT);
        if (0 == errorABORT)
        {
            while (1)
            {
                BOOLEAN interrupt = MP_CheckForInterrupts();
                BOOLEAN timeout = MP_CheckForTimeouts();
                if (interrupt || timeout) MP_Reschedule(MESA_TRUE);
                else if (MP_Mesa_Processor.state.running) MP_Execute();
            }
        }
    }
    MP_Mesa_Processor.state.running = MESA_FALSE;
}

#else

void
MP_Processor()
{
    BOOLEAN interrupt;
    BOOLEAN timeout;

    MP_Initialize();

    while (MESA_FALSE == MP_Mesa_Processor.state.terminate)
    {
        int errorABORT = setjmp(MP_Mesa_Processor.state.jmp_buf_ABORT);
        if (0 == errorABORT)
        {
            while (1)
            {
                interrupt = MP_CheckForInterrupts();
                timeout = MP_CheckForTimeouts();
                if (interrupt || timeout) MP_Reschedule(MESA_TRUE);
                else if (MP_Mesa_Processor.state.running)
                {
                    MP_Mesa_Processor.state.savedPC = MP_Mesa_Processor.registers.PC;    
                    MP_Mesa_Processor.state.savedSP = MP_Mesa_Processor.registers.SP;
                    MP_Mesa_Processor.state.savedOpcode = MP_GetCodeByte();
                    MP_Mesa_Opcode_Table[MP_Mesa_Processor.state.savedOpcode]();
                    ++MP_Mesa_Processor.state.instructionsDispatched;
                }
            }
        }
    }
    MP_Mesa_Processor.state.running = MESA_FALSE;
}

void
MP_Dispatch_EscOpcode()
{
    MP_Mesa_Processor.state.savedOpcode = MP_GetCodeByte();
    MP_Mesa_OpcodeEsc_Table[MP_Mesa_Processor.state.savedOpcode]();
}

void
MP_Dispatch_EscLOpcode()
{
    MP_Mesa_Processor.state.savedOpcode = MP_GetCodeByte();
    MP_Mesa_OpcodeEscL_Table[MP_Mesa_Processor.state.savedOpcode]();
}

void
MP_Dispatch_OpcodeTRAP()
{
    MP_OpcodeTrap(MP_Mesa_Processor.state.savedOpcode);
}

void
MP_Dispatch_EscOpcodeTRAP()
{
    MP_EscOpcodeTrap(MP_Mesa_Processor.state.savedOpcode);
}

#endif

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
