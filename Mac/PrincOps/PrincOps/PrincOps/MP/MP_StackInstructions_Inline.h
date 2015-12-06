/*
    MP_StackInstructions_Inline.h - Mesa Processor Stack inline functions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_STACKINSTRUCTIONS_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_STACKINSTRUCTIONS_INLINE_H

/*******************************************************************************
** Inline Helper functions from StackInstructions chapter of PrincOps */
__mpinline void MP_Push(UNSPECIFIED data)
{
    if (MP_Mesa_Processor.registers.SP == cSS) MP_StackError();
    MP_Mesa_Processor.registers.stack[MP_Mesa_Processor.registers.SP] = C_TO_MESA_UNSPECIFIED(data);
    MP_Mesa_Processor.registers.SP++;
}

__mpinline UNSPECIFIED MP_Pop()
{
    if (MP_Mesa_Processor.registers.SP == 0) MP_StackError();
    return MESA_TO_C_UNSPECIFIED(MP_Mesa_Processor.registers.stack[--MP_Mesa_Processor.registers.SP]);
}

__mpinline void MP_PushLong(LONG_UNSPECIFIED data)
{
#ifdef _MP_BY_THE_BOOK
    MP_Push(MP_LowHalf(data));
    MP_Push(MP_HighHalf(data));
#else
    if (MP_Mesa_Processor.registers.SP > (cSS - 2)) MP_StackError();
    MP_Mesa_Processor.registers.stack[MP_Mesa_Processor.registers.SP] = C_TO_MESA_UNSPECIFIED((UNSPECIFIED)data);
    MP_Mesa_Processor.registers.stack[MP_Mesa_Processor.registers.SP+1] = C_TO_MESA_UNSPECIFIED((UNSPECIFIED)(data >> 16));
    MP_Mesa_Processor.registers.SP += 2;
#endif
}

__mpinline LONG_UNSPECIFIED MP_PopLong()
{
#ifdef _MP_BY_THE_BOOK
    LONG_UNSPECIFIED high = (LONG_UNSPECIFIED)MP_Pop();
    LONG_UNSPECIFIED low = (LONG_UNSPECIFIED)MP_Pop();
    return ((high << 16) | low);
#else
    LONG_UNSPECIFIED value;
    if (MP_Mesa_Processor.registers.SP < 2) MP_StackError();
    MP_Mesa_Processor.registers.SP -= 2;
    value =
        (((LONG_UNSPECIFIED)MESA_TO_C_UNSPECIFIED(MP_Mesa_Processor.registers.stack[MP_Mesa_Processor.registers.SP+1])) << 16) |
        ((LONG_UNSPECIFIED)MESA_TO_C_UNSPECIFIED(MP_Mesa_Processor.registers.stack[MP_Mesa_Processor.registers.SP]));
    return value;
#endif
}

__mpinline void MP_MinimalStack()
{
    if (MP_Mesa_Processor.registers.SP != 0) MP_StackError();
}

__mpinline void MP_Recover()
{
    if (MP_Mesa_Processor.registers.SP == cSS) MP_StackError();
    MP_Mesa_Processor.registers.SP++;
}

__mpinline void MP_Discard()
{
    if (MP_Mesa_Processor.registers.SP == 0) MP_StackError();
    MP_Mesa_Processor.registers.SP--;
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_STACKINSTRUCTIONS_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

