/*
    MP_BlockTransfers.c - implementation of Mesa processor block transfer instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include "PrincOps/PrincOps_Microcode.h"

/*******************************************************************************
** BLOCK TRANSFER INSTRUCTIONS */
/* Word Boundary Block Transfers */
void
MP_Instruction_BLT()
{
    /* Block Transfer */
    POINTER dest;
    CARDINAL count;
    POINTER source;
    do
    {
        dest = MP_Pop();
        count = MP_Pop();
        source = MP_Pop();
        if (count == 0) break;
        MP_WriteMesaRealAddress(MP_StoreMds(dest), MP_ReadMesaRealAddress(MP_FetchMds(source)));
        MP_Push((POINTER)(source+1));
        MP_Push((CARDINAL)(count-1));
        MP_Push((POINTER)(dest+1));
        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}

void
MP_Instruction_BLTL()
{
    /* Block Transfer Long */
    LONG_POINTER dest;
    CARDINAL count;
    LONG_POINTER source;
    do
    {
        dest = MP_PopLong();
        count = MP_Pop();
        source = MP_PopLong();
        if (count == 0) break;
        MP_WriteMesaRealAddress(MP_Store(dest), MP_ReadMesaRealAddress(MP_Fetch(source)));
        MP_PushLong((LONG_POINTER)(source+1));
        MP_Push((CARDINAL)(count-1));
        MP_PushLong((LONG_POINTER)(dest+1));
        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}

void
MP_Instruction_BLTLR()
{
    /* Block Transfer Long Reversed */
    LONG_POINTER dest;
    CARDINAL count;
    LONG_POINTER source;
    do
    {
        dest = MP_PopLong();
        count = MP_Pop();
        source = MP_PopLong();
        if (count == 0) break;
        count--;
        MP_WriteMesaRealAddress(MP_Store(dest + count), MP_ReadMesaRealAddress(MP_Fetch(source + count)));
        MP_PushLong(source);
        MP_Push(count);
        MP_PushLong(dest);
        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}

void
MP_Instruction_BLTC()
{
    /* Block Transfer Code */
    POINTER dest;
    CARDINAL count;
    POINTER source;
    do
    {
        dest = MP_Pop();
        count = MP_Pop();
        source = MP_Pop();
        if (count == 0) break;
        MP_WriteMesaRealAddress(MP_StoreMds(dest), MP_ReadCode(source));
        MP_Push((POINTER)(source+1));
        MP_Push((CARDINAL)(count-1));
        MP_Push((POINTER)(dest+1));
        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}

void
MP_Instruction_BLTCL()
{
    /* Block Transfer Code Long */
    LONG_POINTER dest;
    CARDINAL count;
    POINTER source;
    do
    {
        dest = MP_PopLong();
        count = MP_Pop();
        source = MP_Pop();
        if (count == 0) break;
        MP_WriteMesaRealAddress(MP_Store(dest), MP_ReadCode(source));
        MP_Push((POINTER)(source+1));
        MP_Push((CARDINAL)(count-1));
        MP_PushLong((LONG_POINTER)(dest+1));
        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}

void
MP_Instruction_CKSUM()
{
    /* Checksum */
    LONG_POINTER source;
    CARDINAL count;
    CARDINAL cksum;
    do
    {
        source = MP_PopLong();
        count = MP_Pop();
        cksum = MP_Pop();
        if (count == 0) break;
        cksum = MP_Checksum(cksum, MP_ReadMesaRealAddress(MP_Fetch(source)));
        MP_Push((CARDINAL)cksum);
        MP_Push((CARDINAL)(count-1));
        MP_PushLong((LONG_POINTER)(source+1));
        if (MP_InterruptPending())
        {
            MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
            return;
        }
    }
    while (1);
    if (cksum == 0xFFFF) cksum = 0;
    MP_Push(cksum);
}
 
/* Block Comparisons */
void
MP_Instruction_BLEL()
{
    /* Block Equal Long */
    LONG_POINTER ptr1;
    CARDINAL count;
    LONG_POINTER ptr2;
    do
    {
        ptr1 = MP_PopLong();
        count = MP_Pop();
        ptr2 = MP_PopLong();
        if (count == 0) { MP_Push(MESA_TRUE); break; }
        if (MP_ReadMesaRealAddress(MP_Fetch(ptr1)) != MP_ReadMesaRealAddress(MP_Fetch(ptr2)))
        {
            MP_Push(MESA_FALSE);
            break;
        }
        MP_PushLong((LONG_POINTER)(ptr2 + 1));
        MP_Push((CARDINAL)(count - 1));
        MP_PushLong((LONG_POINTER)(ptr1 + 1));
        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}

void
MP_Instruction_BLECL()
{
    /* Block Equal Code Long */
    LONG_POINTER ptr;
    CARDINAL count;
    CARDINAL offset;
    do
    {
        ptr = MP_PopLong();
        count = MP_Pop();
        offset = MP_Pop();
        if (count == 0) { MP_Push(MESA_TRUE); break; }
        if (MP_ReadMesaRealAddress(MP_Fetch(ptr)) != MP_ReadCode(offset))
        {
            MP_Push(MESA_FALSE);
            break;
        }
        MP_Push((CARDINAL)(offset + 1));
        MP_Push((CARDINAL)(count - 1));
        MP_PushLong((LONG_POINTER)(ptr + 1));
        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}

/* Byte Boundary Block Transfers */
void
MP_Instruction_BYTBLT()
{
    /* Byte Block Transfer */
    CARDINAL sourceOffset;
    LONG_POINTER sourceBase;
    CARDINAL count;
    CARDINAL destOffset;
    LONG_POINTER destBase;
    do
    {
        sourceOffset = MP_Pop();
        sourceBase = MP_PopLong();
        count = MP_Pop();
        destOffset = MP_Pop();
        destBase = MP_PopLong();
        if (count == 0) break;
        sourceBase = sourceBase + ((LONG_POINTER)(sourceOffset / 2));
        sourceOffset &= 1;  // same as sourceOffset = sourceOffset % 2
        destBase = destBase + ((LONG_POINTER)(destOffset / 2));
        destOffset &= 1;    // same as destOffset = destOffset % 2

        MP_StoreByte(destBase, (LONG_POINTER)destOffset, MP_FetchByte(sourceBase, (LONG_POINTER)sourceOffset));
        if (sourceOffset == 1) { sourceBase++; sourceOffset = 0; } else sourceOffset = 1;
        if (destOffset == 1) { destBase++; destOffset = 0; } else destOffset = 1;
        MP_PushLong(destBase);
        MP_Push(destOffset);
        MP_Push((CARDINAL)(count - 1));
        MP_PushLong(sourceBase);
        MP_Push(sourceOffset);
        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}


void
MP_Instruction_BYTBLTR()
{
    CARDINAL sourceOffset;
    LONG_POINTER sourceBase;
    CARDINAL count;
    CARDINAL destOffset;
    LONG_POINTER destBase;

    do
    {
        sourceOffset = MP_Pop();
        sourceBase = MP_PopLong();
        count = MP_Pop();
        destOffset = MP_Pop();
        destBase = MP_PopLong();

        if (count == 0) break;

        MP_StoreByte(destBase + ((LONG_POINTER)((destOffset + (count - 1)) / 2)), (LONG_POINTER)((destOffset + (count - 1)) % 2), MP_FetchByte(sourceBase + ((LONG_POINTER)((sourceOffset + (count - 1)) / 2)), (LONG_POINTER)((sourceOffset + (count - 1)) & 1)));

        MP_PushLong(destBase);
        MP_Push(destOffset);
        MP_Push((CARDINAL)(count-1));
        MP_PushLong(sourceBase);
        MP_Push(sourceOffset);

        if (MP_InterruptPending())
        {
           MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
           break;
        }
    }
    while (1);
}

/* Bit Boundary Block Transfers */
void MP_Instruction_BITBLT()
{
    /* Bit Block Transfer */
    /* NOTE: In our PrincOps processor, we'll leave this instruction unimplemented
             and have it trap to software.  Clients of this processor should probably
             implement their own BitBlt instruction and call PrincOps_InstallInstructionFunction
             to install it and override this implementation. */
    
    MP_EscOpcodeTrap(0x2B);
}

void MP_Instruction_TXTBLT()
{
    /* Text Block Transfer */
    /* Just trap to a sofware implementation. */
    MP_EscOpcodeTrap(0x2C);
}


/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
