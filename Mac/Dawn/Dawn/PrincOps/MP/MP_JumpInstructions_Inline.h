/*
    MP_JumpInstructions_Inline.h - internal header file of Mesa Processor Jump instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_JUMPINSTRUCTIONS_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_JUMPINSTRUCTIONS_INLINE_H

/*******************************************************************************
** INLINE JUMP FUNCTIONS */
__mpinline void MP_JumpN(CARDINAL n)
{
    MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + n;
}

__mpinline void MP_JumpZeroN(CARDINAL n)
{
    UNSPECIFIED u = MP_Pop();
    if (0 == u) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + n;
}

__mpinline void MP_JumpNotZeroN(CARDINAL n)
{
    UNSPECIFIED u = MP_Pop();
    if (0 != u) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + n;
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_JUMPINSTRUCTIONS_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

