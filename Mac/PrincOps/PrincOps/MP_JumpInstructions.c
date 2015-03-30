/*
    MP_JumpInstructions.c - implementation of Mesa processor Jump instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include <PrincOps/PrincOps_Microcode.h>

/*******************************************************************************
** JUMP INSTRUCTIONS */
void
MP_Instruction_J2()
{
    /* Jump 2 */
    MP_JumpN(2);
}

void
MP_Instruction_J3()
{
    /* Jump 3 */
    MP_JumpN(3);
}

void
MP_Instruction_J4()
{
    /* Jump 4 */
    MP_JumpN(4);
}

void
MP_Instruction_J5()
{
    /* Jump 5 */
    MP_JumpN(5);
}

void
MP_Instruction_J6()
{
    /* Jump 6 */
    MP_JumpN(6);
}

void
MP_Instruction_J7()
{
    /* Jump 7 */
    MP_JumpN(7);
}

void
MP_Instruction_J8()
{
    /* Jump 8 */
    MP_JumpN(8);
}

void
MP_Instruction_JB()
{
    /* Jump Byte */
    BYTE disp = MP_GetCodeByte();
    MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JW()
{
    /* Jump Word */
    INTEGER disp = MP_GetCodeWord();
    MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + disp;
}

void
MP_Instruction_JS()
{
    /* Jump Stack */
    MP_Mesa_Processor.registers.PC = MP_Pop();
}

void
MP_Instruction_CATCH()
{
    /* BYTE alpha = */ MP_GetCodeByte();
}

void
MP_Instruction_JZ3()
{
    /* Jump Zero 3 */
    MP_JumpZeroN(3);
}

void
MP_Instruction_JZ4()
{
    /* Jump Zero 4 */
    MP_JumpZeroN(4);
}

void
MP_Instruction_JNZ3()
{
    /* Jump Not Zero 3 */
    MP_JumpNotZeroN(3);
}

void
MP_Instruction_JNZ4()
{
    /* Jump Not Zero 4 */
    MP_JumpNotZeroN(4);
}

void
MP_Instruction_JZB()
{
    /* Jump Zero Byte */
    BYTE disp = MP_GetCodeByte();
    UNSPECIFIED data = MP_Pop();
    if (0 == data) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JNZB()
{
    /* Jump Not Zero Byte */
    BYTE disp = MP_GetCodeByte();
    UNSPECIFIED data = MP_Pop();
    if (0 != data) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JEB()
{
    /* Jump Equal Byte */
    BYTE disp = MP_GetCodeByte();
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    if (u == v) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JNEB()
{
    /* Jump Not Equal Byte */
    BYTE disp = MP_GetCodeByte();
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    if (u != v) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JDEB()
{
    /* Jump Double Equal Byte */
    BYTE disp = MP_GetCodeByte();
    LONG_UNSPECIFIED v = MP_PopLong();
    LONG_UNSPECIFIED u = MP_PopLong();
    if (u == v) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JDNEB()
{
    /* Jump Double Not Equal Byte */
    BYTE disp = MP_GetCodeByte();
    LONG_UNSPECIFIED v = MP_PopLong();
    LONG_UNSPECIFIED u = MP_PopLong();
    if (u != v) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JEP()
{
    /* Jump Equal Pair */
    BYTE pair = MP_GetCodeByte();
    UNSPECIFIED data = MP_Pop();
    if (data == (pair >> 4)) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + (pair & 0xF) + 4;
}

void
MP_Instruction_JNEP()
{
    /* Jump Not Equal Pair */
    BYTE pair = MP_GetCodeByte();
    UNSPECIFIED data = MP_Pop();
    if (data != (pair >> 4)) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + (pair & 0xF) + 4;
}

void
MP_Instruction_JEBB()
{
    /* Jump Equal Byte Byte */
    UNSPECIFIED byte = MP_GetCodeByte();
    BYTE disp = MP_GetCodeByte();
    UNSPECIFIED data = MP_Pop();
    if (data == byte) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JNEBB()
{
    /* Jump Not Equal Byte Byte */
    UNSPECIFIED byte = MP_GetCodeByte();
    BYTE disp = MP_GetCodeByte();
    UNSPECIFIED data = MP_Pop();
    if (data != byte) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JLB()
{
    /* Jump Less Byte */
    BYTE disp = MP_GetCodeByte();
    INTEGER k = MP_Pop();
    INTEGER j = MP_Pop();
    if (j < k) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JLEB()
{
    /* Jump Less Equal Byte */
    BYTE disp = MP_GetCodeByte();
    INTEGER k = MP_Pop();
    INTEGER j = MP_Pop();
    if (j <= k) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JGB()
{
    /* Jump Greater Byte */
    BYTE disp = MP_GetCodeByte();
    INTEGER k = MP_Pop();
    INTEGER j = MP_Pop();
    if (j > k) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JGEB()
{
    /* Jump Greater Equal Byte */
    BYTE disp = MP_GetCodeByte();
    INTEGER k = MP_Pop();
    INTEGER j = MP_Pop();
    if (j >= k) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JULB()
{
    /* Jump Unsigned Less Byte */
    BYTE disp = MP_GetCodeByte();
    CARDINAL v = MP_Pop();
    CARDINAL u = MP_Pop();
    if (u < v) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JULEB()
{
    /* Jump Unsigned Less Equal Byte */
    BYTE disp = MP_GetCodeByte();
    CARDINAL v = MP_Pop();
    CARDINAL u = MP_Pop();
    if (u <= v) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JUGB()
{
    /* Jump Unsigned Greater Byte */
    BYTE disp = MP_GetCodeByte();
    CARDINAL v = MP_Pop();
    CARDINAL u = MP_Pop();
    if (u > v) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JUGEB()
{
    /* Jump Unsigned Greater Equal Byte */
    BYTE disp = MP_GetCodeByte();
    CARDINAL v = MP_Pop();
    CARDINAL u = MP_Pop();
    if (u >= v) MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + MP_SignExtend(disp);
}

void
MP_Instruction_JIB()
{
    /* Jump Indexed Byte */
    UNSPECIFIED disp;
    CARDINAL base = MP_GetCodeWord();
    CARDINAL limit = MP_Pop();
    CARDINAL index = MP_Pop();
    if (index < limit)
    {
        disp = MP_ReadCode(((CARDINAL)(base + index / 2)));
        MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + ((0 == (index % 2)) ? (disp >> 8) : (disp & 0xFF));
    }
}

void
MP_Instruction_JIW()
{
    /* Jump Indexed Word */
    CARDINAL disp;
    CARDINAL base = MP_GetCodeWord();
    CARDINAL limit = MP_Pop();
    CARDINAL index = MP_Pop();
    if (index < limit)
    {
        disp = MP_ReadCode(((CARDINAL)(base + index)));
        MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC + disp;
    }
}

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
