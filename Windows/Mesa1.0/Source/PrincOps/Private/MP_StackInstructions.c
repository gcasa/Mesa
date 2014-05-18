/*
    MP_StackInstructions.c - implementation of Mesa processor stack

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include <PrincOps/PrincOps_Microcode.h>

/*******************************************************************************
** STACK FUNCTIONS */

/*******************************************************************************
** STACK INSTRUCTIONS */

/* Stack Primitives */
void
MP_Instruction_REC()
{
    /* Recover */
    MP_Recover();
}

void
MP_Instruction_REC2()
{
    /* Recover 2 */
    MP_Recover();
    MP_Recover();
}

void
MP_Instruction_DIS()
{
    /* Discard */
    MP_Discard();
}

void
MP_Instruction_DIS2()
{
    /* Discard 2 */
    MP_Discard();
    MP_Discard();
}

void
MP_Instruction_EXCH()
{
    /* Exchange */
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    MP_Push(v);
    MP_Push(u);
}

void
MP_Instruction_DEXCH()
{
    /* Double Exchange */
    LONG_UNSPECIFIED v = MP_PopLong();
    LONG_UNSPECIFIED u = MP_PopLong();
    MP_PushLong(v);
    MP_PushLong(u); 
}

void
MP_Instruction_DUP()
{
    /* Duplicate */
    UNSPECIFIED u = MP_Pop();
    MP_Push(u);
    MP_Push(u);
}

void
MP_Instruction_DDUP()
{
    /* Double Duplicate */
    LONG_UNSPECIFIED u = MP_PopLong();
    MP_PushLong(u);
    MP_PushLong(u);
}

void
MP_Instruction_EXDIS()
{
    /* Exchange Discard */
    UNSPECIFIED u = MP_Pop();
    /* UNSPECIFIED v = */ MP_Pop();
    MP_Push(u);
}

void
MP_Instruction_BNDCK()
{
    /* Bounds Check */
    CARDINAL range = MP_Pop();
    CARDINAL index = MP_Pop();
    MP_Push(index);
    if (index >= range) MP_BoundsTrap();
}

void
MP_Instruction_BNDCKL()
{
    /* Bounds Check Long */
    LONG_CARDINAL range = MP_PopLong();
    LONG_CARDINAL index = MP_PopLong();
    MP_PushLong(index);
    if (index >= range) MP_BoundsTrap();
}

void
MP_Instruction_NILCK()
{
    /* Nil Check */
    POINTER pointer = MP_Pop();
    MP_Push(pointer);
    if (pointer == 0) MP_PointerTrap();
}

void
MP_Instruction_NILCKL()
{
    /* Nil Check Long */
    LONG_POINTER pointer = MP_PopLong();
    MP_PushLong(pointer);
    if (pointer == 0) MP_PointerTrap();
}

void
MP_Instruction_NEG()
{
    /* Negate */
    INTEGER i = MP_Pop();
    MP_Push(((UNSPECIFIED)-i));
}

void
MP_Instruction_INC()
{
    /* Increment */
    CARDINAL s = MP_Pop();
    MP_Push(((UNSPECIFIED)(s+1)));
}

void
MP_Instruction_DINC()
{
    /* Double Increment */
    LONG_CARDINAL s = MP_PopLong();
    MP_PushLong(s+1);
}

void
MP_Instruction_DEC()
{
    /* Decrement */
    CARDINAL s = MP_Pop();
    MP_Push(((UNSPECIFIED)(s-1)));
}

void
MP_Instruction_ADDSB()
{
    /* Add Signed Byte */
    BYTE alpha = MP_GetCodeByte();
    INTEGER i = MP_Pop();
    MP_Push(((UNSPECIFIED)(i + MP_SignExtend(alpha))));
}

void
MP_Instruction_DBL()
{
    /* Double */
    UNSPECIFIED u = MP_Pop();
    MP_Push(MP_Shift(u, 1));
}

void
MP_Instruction_DDBL()
{
    /* Double Double */
    LONG_UNSPECIFIED u = MP_PopLong();
    MP_PushLong(MP_LongShift(u, 1));
}

void
MP_Instruction_TRPL()
{
    /* Triple */
    CARDINAL s = MP_Pop();
    MP_Push(((UNSPECIFIED)(s * 3)));
}

void
MP_Instruction_LINT()
{
    /* Lengthen Integer */
    INTEGER i = MP_Pop();
    MP_Push(i);
    MP_Push(((UNSPECIFIED)((i < 0) ? -1 : 0)));
}

void
MP_Instruction_SHIFTSB()
{
    /* Shift Signed Byte */
    BYTE alpha = MP_GetCodeByte();
    UNSPECIFIED u = MP_Pop();
    INTEGER shift = MP_SignExtend(alpha);
    if ((shift < -15) || (shift > 15)) MP_Error();
    MP_Push(MP_Shift(u, shift));
}

/* Logical Operations */
void
MP_Instruction_AND()
{
    /* And */
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    MP_Push(MP_And(u, v));
}

void
MP_Instruction_DAND()
{
    /* Double And */
    LONG_UNSPECIFIED v = MP_PopLong();
    LONG_UNSPECIFIED u = MP_PopLong();
    MP_PushLong(MP_LongAnd(u, v));
}

void
MP_Instruction_IOR()
{
    /* Inclusive Or */
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    MP_Push(MP_Or(u, v));
}

void
MP_Instruction_DIOR()
{
    /* Double Inclusive Or */
    LONG_UNSPECIFIED v = MP_PopLong();
    LONG_UNSPECIFIED u = MP_PopLong();
    MP_PushLong(MP_LongOr(u, v));
}

void
MP_Instruction_XOR()
{
    /* Exclusive Or */
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    MP_Push(MP_Xor(u, v));
}

void
MP_Instruction_DXOR()
{
    /* Double Exclusive Or */
    LONG_UNSPECIFIED v = MP_PopLong();
    LONG_UNSPECIFIED u = MP_PopLong();
    MP_PushLong(MP_LongXor(u, v));
}

void
MP_Instruction_SHIFT()
{
    /* Shift */
    INTEGER shift = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    MP_Push(MP_Shift(u, shift));
}

void
MP_Instruction_DSHIFT()
{
    /* Double Shift */
    INTEGER shift = MP_Pop();
    LONG_UNSPECIFIED u = MP_PopLong();
    MP_PushLong(MP_LongShift(u, shift));
}

void
MP_Instruction_ROTATE()
{
    /* Rotate */
    INTEGER rotate = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    MP_Push(MP_Rotate(u, rotate));
}

void
MP_Instruction_ADD()
{
    /* Add */
    CARDINAL t = MP_Pop();
    CARDINAL s = MP_Pop();
    MP_Push(((UNSPECIFIED)(s + t)));
}

void
MP_Instruction_SUB()
{
    /* Subtract */
    CARDINAL t = MP_Pop();
    CARDINAL s = MP_Pop();
    MP_Push(((UNSPECIFIED)(s - t)));
}

void
MP_Instruction_DADD()
{
    /* Double Add */
    LONG_CARDINAL t = MP_PopLong();
    LONG_CARDINAL s = MP_PopLong();
    MP_PushLong(s + t);
}

void
MP_Instruction_DSUB()
{
    /* Double Subtract */
    LONG_CARDINAL t = MP_PopLong();
    LONG_CARDINAL s = MP_PopLong();
    MP_PushLong(s - t);
}

void
MP_Instruction_ADC()
{
#if 0
    /* This is the book version; but it's switched with ACD */
    /* Add Double to Cardinal */
    LONG_CARDINAL t = MP_PopLong();
    CARDINAL s = MP_Pop();
    MP_PushLong(((LONG_CARDINAL)s) + t);
#else
    CARDINAL t = MP_Pop();
    LONG_CARDINAL s = MP_PopLong();
    MP_PushLong(s + ((LONG_CARDINAL)t));
#endif
}

void
MP_Instruction_ACD()
{
#if 0
    /* This is the book version; but it's switched with ADC */
    /* Add Cardinal to Double */
    CARDINAL t = MP_Pop();
    LONG_CARDINAL s = MP_PopLong();
    MP_PushLong(s + ((LONG_CARDINAL)t));
#else
    LONG_CARDINAL t = MP_PopLong();
    CARDINAL s = MP_Pop();
    MP_PushLong(((LONG_CARDINAL)s) + t);
#endif
}

void
MP_Instruction_MUL()
{
    /* Multiply */
    CARDINAL t = MP_Pop();
    CARDINAL s = MP_Pop();
    MP_PushLong(((LONG_CARDINAL)s) * t);
    MP_Discard();
}

void
MP_Instruction_DMUL()
{
    /* Double Multiply */
    LONG_CARDINAL t = MP_PopLong();
    LONG_CARDINAL s = MP_PopLong();
    MP_PushLong(s * t);
}

void
MP_Instruction_SDIV()
{
    /* Signed Divide */
    INTEGER k = MP_Pop();
    INTEGER j = MP_Pop();
    if (0 == k) MP_DivZeroTrap();
    MP_Push(((UNSPECIFIED)(j / k)));
    MP_Push(((UNSPECIFIED)(j % k)));
    MP_Discard();
}

void
MP_Instruction_UDIV()
{
    /* Unsigned Divide */
    CARDINAL t = MP_Pop();
    CARDINAL s = MP_Pop();
    if (0 == t) MP_DivZeroTrap();
    MP_Push(((UNSPECIFIED)(s / t)));
    MP_Push(((UNSPECIFIED)(s % t)));
    MP_Discard();
}

void
MP_Instruction_LUDIV()
{
    /* Long Unsigned Divide */
    CARDINAL t = MP_Pop();
    LONG_CARDINAL s = MP_PopLong();
    if (0 == t) MP_DivZeroTrap();
    if (MP_HighHalf(s) >= t) MP_DivCheckTrap();
    MP_Push(MP_LowHalf(s / ((LONG_CARDINAL)t)));
    MP_Push(MP_LowHalf(s % ((LONG_CARDINAL)t)));
    MP_Discard();
}

void
MP_Instruction_SDDIV()
{
    /* Signed Double Divide */
    LONG_INTEGER k = MP_PopLong();
    LONG_INTEGER j = MP_PopLong();
    if (0 == k) MP_DivZeroTrap();
    MP_PushLong(j / k);
    MP_PushLong(j % k);
    MP_Discard();
    MP_Discard();
}

void
MP_Instruction_UDDIV()
{
    /* Unsigned Double Divide */
    LONG_CARDINAL t = MP_PopLong();
    LONG_CARDINAL s = MP_PopLong();
    if (0 == t) MP_DivZeroTrap();
    MP_PushLong(s / t);
    MP_PushLong(s % t);
    MP_Discard();
    MP_Discard();
}

void
MP_Instruction_DCMP()
{
    /* Double Compare */
    LONG_INTEGER k = MP_PopLong();
    LONG_INTEGER j = MP_PopLong();
    MP_Push(((UNSPECIFIED)  ((j > k) ? 1 : ((j < k) ? -1 : 0))));
}

void
MP_Instruction_UDCMP()
{
    /* Unsigned Double Compare */
    LONG_CARDINAL t = MP_PopLong();
    LONG_CARDINAL s = MP_PopLong();
    MP_Push(((UNSPECIFIED)((s > t) ? 1 : ((s < t) ? -1 : 0))));
}

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
