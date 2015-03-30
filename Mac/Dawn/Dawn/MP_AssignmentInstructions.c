/*
    MP_AssignmentInstructions.c - implementation of Mesa processor Assignment instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include "PrincOps/PrincOps_Microcode.h"

/*******************************************************************************
** ASSIGNMENT VARIABLES */
UNSPECIFIED MP_MaskTable[] =
{
      0x1,   0x3,   0x7,   0xF,   0x1F,   0x3F,   0x7F,   0xFF,
    0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF
};

/*******************************************************************************
** ASSIGNMENT INSTRUCTIONS */

/* Immediate Instructions */
void
MP_Instruction_LIN1()
{
    /* Load Immediate Negative One */
    MP_Push(0xFFFF); 
}

void
MP_Instruction_LINI()
{
    /* Load Immediate Negative Infinity */
    MP_Push(0x8000);
}

void
MP_Instruction_LI00()
{
    /* Load Immediate Double Zero */
    MP_PushLong(0);
}

void
MP_Instruction_LI0()
{
    /* Load Immediate 0 */
    MP_LoadImmediateN(0);
}

void
MP_Instruction_LI1()
{
    /* Load Immediate 1 */
    MP_LoadImmediateN(1);
}

void
MP_Instruction_LI2()
{
    /* Load Immediate 2 */
    MP_LoadImmediateN(2);
}

void
MP_Instruction_LI3()
{
    /* Load Immediate 3 */
    MP_LoadImmediateN(3);
}

void
MP_Instruction_LI4()
{
    /* Load Immediate 4 */
    MP_LoadImmediateN(4);
}

void
MP_Instruction_LI5()
{
    /* Load Immediate 5 */
    MP_LoadImmediateN(5);
}

void
MP_Instruction_LI6()
{
    /* Load Immediate 6 */
    MP_LoadImmediateN(6);
}

void
MP_Instruction_LI7()
{
    /* Load Immediate 7 */
    MP_LoadImmediateN(7);
}

void
MP_Instruction_LI8()
{
    /* Load Immediate 8 */
    MP_LoadImmediateN(8);
}

void
MP_Instruction_LI9()
{
    /* Load Immediate 9 */
    MP_LoadImmediateN(9);
}

void
MP_Instruction_LI10()
{
    /* Load Immediate 10 */
    MP_LoadImmediateN(10);
}

void
MP_Instruction_LIB()
{
    /* Load Immediate Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push(alpha);
}

void
MP_Instruction_LINB()
{
    /* Load Immediate Negative Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push((UNSPECIFIED)((0xFF << 8) | alpha));
}

void
MP_Instruction_LIHB()
{
    /* Load Immediate High Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push((UNSPECIFIED)(alpha << 8));
}

void
MP_Instruction_LIW()
{
    /* Load Immediate Word */
    UNSPECIFIED u = MP_GetCodeWord();
    MP_Push(u);
}

/* Frame Instructions */
void
MP_Instruction_LA0()
{
    /* Local Address 0 */
    MP_LocalAddressN(0);
}

void
MP_Instruction_LA1()
{
    /* Local Address 1 */
    MP_LocalAddressN(1);
}

void
MP_Instruction_LA2()
{
    /* Local Address 2 */
    MP_LocalAddressN(2);
}

void
MP_Instruction_LA3()
{
    /* Local Address 3 */
    MP_LocalAddressN(3);
}

void
MP_Instruction_LA6()
{
    /* Local Address 6 */
    MP_LocalAddressN(6);
}

void
MP_Instruction_LA8()
{
    /* Local Address 8 */
    MP_LocalAddressN(8);
}

void
MP_Instruction_LAB()
{
    /* Local Address Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push((UNSPECIFIED)(MP_Mesa_Processor.registers.LF + alpha));
}

void
MP_Instruction_LAW()
{
    /* Local Address Word */
    UNSPECIFIED word = MP_GetCodeWord();
    MP_Push((UNSPECIFIED)(MP_Mesa_Processor.registers.LF + word));
}

void
MP_Instruction_LL0()
{
    /* Load Local 0 */
    MP_LoadLocalN(0);
}

void
MP_Instruction_LL1()
{
    /* Load Local 1 */
    MP_LoadLocalN(1);
}

void
MP_Instruction_LL2()
{
    /* Load Local 2 */
    MP_LoadLocalN(2);
}

void
MP_Instruction_LL3()
{
    /* Load Local 3 */
    MP_LoadLocalN(3);
}

void
MP_Instruction_LL4()
{
    /* Load Local 4 */
    MP_LoadLocalN(4);
}

void
MP_Instruction_LL5()
{
    /* Load Local 5 */
    MP_LoadLocalN(5);
}

void
MP_Instruction_LL6()
{
    /* Load Local 6 */
    MP_LoadLocalN(6);
}

void
MP_Instruction_LL7()
{
    /* Load Local 7 */
    MP_LoadLocalN(7);
}

void
MP_Instruction_LL8()
{
    /* Load Local 8 */
    MP_LoadLocalN(8);
}

void
MP_Instruction_LL9()
{
    /* Load Local 9 */
    MP_LoadLocalN(9);
}

void
MP_Instruction_LL10()
{
    /* Load Local 10 */
    MP_LoadLocalN(10);
}

void
MP_Instruction_LL11()
{
    /* Load Local 11 */
    MP_LoadLocalN(11);
}

void
MP_Instruction_LLB()
{
    /* Load Local Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + alpha))));
}

void
MP_Instruction_LLD0()
{
    /* Load Local Double 0 */
    MP_LoadLocalDoubleN(0);
}

void
MP_Instruction_LLD1()
{
    /* Load Local Double 1 */
    MP_LoadLocalDoubleN(1);
}

void
MP_Instruction_LLD2()
{
    /* Load Local Double 2 */
    MP_LoadLocalDoubleN(2);
}

void
MP_Instruction_LLD3()
{
    /* Load Local Double 3 */
    MP_LoadLocalDoubleN(3);
}

void
MP_Instruction_LLD4()
{
    /* Load Local Double 4 */
    MP_LoadLocalDoubleN(4);
}

void
MP_Instruction_LLD5()
{
    /* Load Local Double 5 */
    MP_LoadLocalDoubleN(5);
}

void
MP_Instruction_LLD6()
{
    /* Load Local Double 6 */
    MP_LoadLocalDoubleN(6);
}

void
MP_Instruction_LLD7()
{
    /* Load Local Double 7 */
    MP_LoadLocalDoubleN(7);
}

void
MP_Instruction_LLD8()
{
    /* Load Local Double 8 */
    MP_LoadLocalDoubleN(8);
}

void
MP_Instruction_LLD10()
{
    /* Load Local Double 10 */
    MP_LoadLocalDoubleN(10);
}

void
MP_Instruction_LLDB()
{
    /* Load Local Double Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + alpha))));
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + alpha + 1))));
}

void
MP_Instruction_SL0()
{
    /* Store Local 0 */
    MP_StoreLocalN(0);
}

void
MP_Instruction_SL1()
{
    /* Store Local 1 */
    MP_StoreLocalN(1);
}

void
MP_Instruction_SL2()
{
    /* Store Local 2 */
    MP_StoreLocalN(2);
}

void
MP_Instruction_SL3()
{
    /* Store Local 3 */
    MP_StoreLocalN(3);
}

void
MP_Instruction_SL4()
{
    /* Store Local 4 */
    MP_StoreLocalN(4);
}

void
MP_Instruction_SL5()
{
    /* Store Local 5 */
    MP_StoreLocalN(5);
}

void
MP_Instruction_SL6()
{
    /* Store Local 6 */
    MP_StoreLocalN(6);
}

void
MP_Instruction_SL7()
{
    /* Store Local 7 */
    MP_StoreLocalN(7);
}

void
MP_Instruction_SL8()
{
    /* Store Local 8 */
    MP_StoreLocalN(8);
}

void
MP_Instruction_SL9()
{
    /* Store Local 9 */
    MP_StoreLocalN(9);
}

void
MP_Instruction_SL10()
{
    /* Store Local 10 */
    MP_StoreLocalN(10);
}

void
MP_Instruction_SLB()
{
    /* Store Local Byte */
    MP_StoreLocalByte();
}

void
MP_Instruction_SLD0()
{
    /* Store Local Double 0 */
    MP_StoreLocalDoubleN(0);
}

void
MP_Instruction_SLD1()
{
    /* Store Local Double 1 */
    MP_StoreLocalDoubleN(1);
}

void
MP_Instruction_SLD2()
{
    /* Store Local Double 2 */
    MP_StoreLocalDoubleN(2);
}

void
MP_Instruction_SLD3()
{
    /* Store Local Double 3 */
    MP_StoreLocalDoubleN(3);
}

void
MP_Instruction_SLD4()
{
    /* Store Local Double 4 */
    MP_StoreLocalDoubleN(4);
}

void
MP_Instruction_SLD5()
{
    /* Store Local Double 5 */
    MP_StoreLocalDoubleN(5);
}

void
MP_Instruction_SLD6()
{
    /* Store Local Double 0 */
    MP_StoreLocalDoubleN(6);
}

void
MP_Instruction_SLD8()
{
    /* Store Local Double 8 */
    MP_StoreLocalDoubleN(8);
}

void
MP_Instruction_SLDB()
{
    /* Store Local Double Byte */
    MP_StoreLocalDoubleByte();
}

void
MP_Instruction_PL0()
{
    /* Put Local 0 */
    MP_PutLocalN(0);
}

void
MP_Instruction_PL1()
{
    /* Put Local 1 */
    MP_PutLocalN(1);
}

void
MP_Instruction_PL2()
{
    /* Put Local 2 */
    MP_PutLocalN(2);
}

void
MP_Instruction_PL3()
{
    /* Put Local 3 */
    MP_PutLocalN(3);
}

void
MP_Instruction_PLB()
{
    /* Put Local Byte */
    MP_StoreLocalByte();
    MP_Recover();
}

void
MP_Instruction_PLD0()
{
    /* Put Local Double 0 */
    MP_StoreLocalDoubleN(0);
    MP_Recover();
    MP_Recover();
}

void
MP_Instruction_PLDB()
{
    /* Put Local Double Byte */
    MP_StoreLocalDoubleByte();
    MP_Recover();
    MP_Recover();
}

void
MP_Instruction_AL0IB()
{
    /* Add Local Zero to Immediate Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push((UNSPECIFIED)(MP_ReadMesaRealAddress(MP_FetchMds(MP_Mesa_Processor.registers.LF)) + alpha));
}

void
MP_Instruction_GA0()
{
    /* Global Address 0 */
    MP_GlobalAddressN(0);
}

void
MP_Instruction_GA1()
{
    /* Global Address 1 */
    MP_GlobalAddressN(1);
}

void
MP_Instruction_GAB()
{
    /* Global Address Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push((UNSPECIFIED)(MP_Mesa_Processor.registers.GF + alpha));
}

void
MP_Instruction_GAW()
{
    /* Global Address Word */
    UNSPECIFIED word = MP_GetCodeWord();
    MP_Push((UNSPECIFIED)(MP_Mesa_Processor.registers.GF + word));
}

void
MP_Instruction_LG0()
{
    /* Load Global 0 */
    MP_LoadGlobalN(0);
}

void
MP_Instruction_LG1()
{
    /* Load Global 1 */
    MP_LoadGlobalN(1);
}

void
MP_Instruction_LG2()
{
    /* Load Global 2 */
    MP_LoadGlobalN(2);
}

void
MP_Instruction_LGB()
{
    /* Load Global Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(MP_Mesa_Processor.registers.GF + alpha)));
}

void
MP_Instruction_LGD0()
{
    /* Load Global Double 0 */
    MP_LoadGlobalDoubleN(0);
}

void
MP_Instruction_LGD2()
{
    /* Load Global Double 2 */
    MP_LoadGlobalDoubleN(2);
}

void
MP_Instruction_LGDB()
{
    /* Load Global Double Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(MP_Mesa_Processor.registers.GF + alpha)));
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(MP_Mesa_Processor.registers.GF + alpha + 1)));
}

void
MP_Instruction_SGB()
{
    /* Store Global Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_WriteMesaRealAddress(MP_Store(MP_Mesa_Processor.registers.GF + alpha), MP_Pop());
}

void
MP_Instruction_SGDB()
{
    /* Store Global Double Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_WriteMesaRealAddress(MP_Store(MP_Mesa_Processor.registers.GF + alpha + 1), MP_Pop());
    MP_WriteMesaRealAddress(MP_Store(MP_Mesa_Processor.registers.GF + alpha), MP_Pop());
}

/* Pointer Instructions */
void
MP_Instruction_R0()
{
    /* Read 0 */
    MP_ReadN(0);
}

void
MP_Instruction_R1()
{
    /* Read 1 */
    MP_ReadN(1);
}

void
MP_Instruction_RB()
{
    /* Read Byte */
    BYTE alpha = MP_GetCodeByte();
    POINTER ptr = MP_Pop();
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + alpha))));
}

void
MP_Instruction_RL0()
{
    /* Read Long Zero */
    LONG_POINTER ptr = MP_PopLong();
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(ptr)));
}

void
MP_Instruction_RLB()
{
    /* Read Long Byte */
    BYTE alpha = MP_GetCodeByte();
    LONG_POINTER ptr = MP_PopLong();
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)alpha))));
}

void
MP_Instruction_RD0()
{
    /* Read Double Zero */
    POINTER ptr = MP_Pop();
    UNSPECIFIED u = MP_ReadMesaRealAddress(MP_FetchMds(ptr));
    UNSPECIFIED v = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + 1)));
    MP_Push(u);
    MP_Push(v);
}

void
MP_Instruction_RDB()
{
    /* Read Double Byte */
    BYTE alpha = MP_GetCodeByte();
    POINTER ptr = MP_Pop();
    UNSPECIFIED u = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + alpha)));
    UNSPECIFIED v = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + alpha + 1)));
    MP_Push(u);
    MP_Push(v);
}

void
MP_Instruction_RDL0()
{
    /* Read Double Long Zero */
    LONG_POINTER ptr = MP_PopLong();
    UNSPECIFIED u = MP_ReadMesaRealAddress(MP_Fetch(ptr));
    UNSPECIFIED v = MP_ReadMesaRealAddress(MP_Fetch(ptr + 1));
    MP_Push(u);
    MP_Push(v);
}

void
MP_Instruction_RDLB()
{
    /* Read Double Long Byte */
    BYTE alpha = MP_GetCodeByte();
    LONG_POINTER ptr = MP_PopLong();
    UNSPECIFIED u = MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)alpha)));
    UNSPECIFIED v = MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)alpha) + 1));
    MP_Push(u);
    MP_Push(v);
}

void
MP_Instruction_RC()
{
    /* Read Code */
    BYTE alpha = MP_GetCodeByte();
    CARDINAL offset = MP_Pop();
    MP_Push(MP_ReadCode((UNSPECIFIED)(offset + alpha)));
}

void
MP_Instruction_W0()
{
    /* Write Zero */
    POINTER ptr = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds(ptr), MP_Pop());
}

void
MP_Instruction_WB()
{
    /* Write Byte */
    BYTE alpha = MP_GetCodeByte();
    POINTER ptr = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + alpha)), MP_Pop());
}

void
MP_Instruction_WLB()
{
    /* Write Long Byte */
    BYTE alpha = MP_GetCodeByte();
    LONG_POINTER ptr = MP_PopLong();
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)alpha)), MP_Pop());
}

void
MP_Instruction_WDB()
{
    /* Write Double Byte */
    BYTE alpha = MP_GetCodeByte();
    POINTER ptr = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + alpha + 1)), MP_Pop());
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + alpha)), MP_Pop());
}

void
MP_Instruction_WDLB()
{
    /* Write Double Long Byte */
    BYTE alpha = MP_GetCodeByte();
    LONG_POINTER ptr = MP_PopLong();
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)alpha) + 1), MP_Pop());
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)alpha)), MP_Pop());
}

void
MP_Instruction_PSB()
{
    /* Put Swapped Byte */
    BYTE alpha = MP_GetCodeByte();
    UNSPECIFIED u = MP_Pop();
    POINTER ptr = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + alpha)), u);
    MP_Recover();
}

void
MP_Instruction_PSD0()
{
    /* Put Swapped Double Zero */
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    POINTER ptr = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + 1)), v);
    MP_WriteMesaRealAddress(MP_StoreMds(ptr), u);
    MP_Recover();
}

void
MP_Instruction_PSDB()
{
    /* Put Swapped Double Byte */
    BYTE alpha = MP_GetCodeByte();
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    POINTER ptr = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + alpha + 1)), v);
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + alpha)), u);
    MP_Recover();
}

void
MP_Instruction_PSLB()
{
    /* Put Swapped Long Byte */
    BYTE alpha = MP_GetCodeByte();
    UNSPECIFIED u = MP_Pop();
    LONG_POINTER ptr = MP_PopLong();
    MP_WriteMesaRealAddress(MP_Store(ptr + (LONG_POINTER)alpha), u);
    MP_Recover();
    MP_Recover();
}

void
MP_Instruction_PSDLB()
{
    /* Put Swapped Double Long Byte */
    BYTE alpha = MP_GetCodeByte();
    UNSPECIFIED v = MP_Pop();
    UNSPECIFIED u = MP_Pop();
    LONG_POINTER ptr = MP_PopLong();
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)alpha) + 1), v);
    MP_WriteMesaRealAddress(MP_Store(ptr + (LONG_POINTER)alpha), u);
    MP_Recover();
    MP_Recover();
}

void
MP_Instruction_RLI00()
{
    /* Read Local Indirect Zero 0 */
    MP_ReadLocalIndirectZeroN(0);
}

void
MP_Instruction_RLI01()
{
    /* Read Local Indirect Zero 1 */
    MP_ReadLocalIndirectZeroN(1);
}

void
MP_Instruction_RLI02()
{
    /* Read Local Indirect Zero 2 */
    MP_ReadLocalIndirectZeroN(2);
}

void
MP_Instruction_RLI03()
{
    /* Read Local Indirect Zero 3 */
    MP_ReadLocalIndirectZeroN(3);
}

void
MP_Instruction_RLIP()
{
    /* Read Local Indirect Pair */
    BYTE pair = MP_GetCodeByte();
    POINTER ptr = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair))));
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + MP_NibbleRight(pair)))));
}

void
MP_Instruction_RLILP()
{
    /* Read Local Indirect Long Pair */
    BYTE pair = MP_GetCodeByte();
    LONG_POINTER ptr = MP_ReadDblMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair)));
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_NibbleRight(pair)))));
}

void
MP_Instruction_RGIP()
{
    /* Read Global Indirect Pair */
    BYTE pair = MP_GetCodeByte();
    POINTER ptr = MP_ReadMesaRealAddress(MP_Fetch(MP_Mesa_Processor.registers.GF + MP_NibbleLeft(pair)));
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + MP_NibbleRight(pair)))));
}

void
MP_Instruction_RGILP()
{
    /* Read Global Indirect Long Pair */
    BYTE pair = MP_GetCodeByte();
    LONG_POINTER ptr = MP_ReadDbl(MP_Mesa_Processor.registers.GF + MP_NibbleLeft(pair));
    MP_Push(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_NibbleRight(pair)))));
}

void
MP_Instruction_RLDI00()
{
    /* Read Local Double Indirect Zero Zero */
    POINTER ptr = MP_ReadMesaRealAddress(MP_FetchMds(MP_Mesa_Processor.registers.LF));
    UNSPECIFIED u = MP_ReadMesaRealAddress(MP_FetchMds(ptr));
    UNSPECIFIED v = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + 1)));
    MP_Push(u);
    MP_Push(v);
}

void
MP_Instruction_RLDIP()
{
    /* Read Local Double Indirect Pair */
    BYTE pair = MP_GetCodeByte();
    POINTER ptr = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair))));
    UNSPECIFIED u = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + MP_NibbleRight(pair))));
    UNSPECIFIED v = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + MP_NibbleRight(pair) + 1)));
    MP_Push(u);
    MP_Push(v);
}

void
MP_Instruction_RLDILP()
{
    /* Read Local Double Indirect Long Pair */
    BYTE pair = MP_GetCodeByte();
    LONG_POINTER ptr = MP_ReadDblMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair)));
    UNSPECIFIED u = MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_NibbleRight(pair))));
    UNSPECIFIED v = MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_NibbleRight(pair)) + 1));
    MP_Push(u);
    MP_Push(v);
}

void
MP_Instruction_WLIP()
{
    /* Write Local Indirect Pair */
    BYTE pair = MP_GetCodeByte();
    POINTER ptr = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair))));
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + MP_NibbleRight(pair))), MP_Pop());
}

void
MP_Instruction_WLILP()
{
    /* Write Local Indirect Long Pair */
    BYTE pair = MP_GetCodeByte();
    LONG_POINTER ptr = MP_ReadDblMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair)));
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)MP_NibbleRight(pair))), MP_Pop());
}

void
MP_Instruction_WLDILP()
{
    /* Write Local Double Indirect Long Pair */
    BYTE pair = MP_GetCodeByte();
    LONG_POINTER ptr = MP_ReadDblMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair)));
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)MP_NibbleRight(pair)) + 1), MP_Pop());
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)MP_NibbleRight(pair))), MP_Pop());
}

/* String Instructions */
void
MP_Instruction_RS()
{
    /* Read String */
    BYTE alpha = MP_GetCodeByte();
    CARDINAL index = MP_Pop();
    POINTER ptr = MP_Pop();
    MP_Push(MP_FetchByte(MP_LengthenPointer(ptr), alpha + index));
}

void
MP_Instruction_RLS()
{
    /* Read Long String */
    BYTE alpha = MP_GetCodeByte();
    CARDINAL index = MP_Pop();
    LONG_POINTER ptr = MP_PopLong();
    MP_Push(MP_FetchByte(ptr, alpha + index));
}

void
MP_Instruction_WS()
{
    /* Write String */
    BYTE alpha = MP_GetCodeByte();
    CARDINAL index = MP_Pop();
    POINTER ptr = MP_Pop();
    BYTE data = MP_LowByte(MP_Pop());
    MP_StoreByte(MP_LengthenPointer(ptr), alpha + index, data);
}

void
MP_Instruction_WLS()
{
    /* Write Long String */
    BYTE alpha = MP_GetCodeByte();
    CARDINAL index = MP_Pop();
    LONG_POINTER ptr = MP_PopLong();
    BYTE data = MP_LowByte(MP_Pop());
    MP_StoreByte(ptr, alpha + index, data);
}

/* Field Instructions */
void
MP_Instruction_RF()
{
    /* Read Field */
    FieldDesc desc = MP_GetCodeWord();
    POINTER ptr = MP_Pop();
    MP_Push(MP_ReadField(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + MP_GetFieldDescOffset(desc)))), MP_GetFieldDescFieldSpec(desc)));
}

void
MP_Instruction_R0F()
{
    /* Read Zero Field */
    FieldSpec spec = MP_GetCodeByte();
    POINTER ptr = MP_Pop();
    MP_Push(MP_ReadField(MP_ReadMesaRealAddress(MP_FetchMds(ptr)), spec));
}

void
MP_Instruction_RLF()
{
    /* Read Long Field */
    FieldDesc desc = MP_GetCodeWord();
    LONG_POINTER ptr = MP_PopLong();
    MP_Push(MP_ReadField(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_GetFieldDescOffset(desc)))), MP_GetFieldDescFieldSpec(desc)));
}

void
MP_Instruction_RL0F()
{
    /* Read Long Zero Field */
    FieldSpec spec = MP_GetCodeByte();
    LONG_POINTER ptr = MP_PopLong();
    MP_Push(MP_ReadField(MP_ReadMesaRealAddress(MP_Fetch(ptr)), spec));
}

void
MP_Instruction_RLFS()
{
    /* Read Long Field Stack */
    FieldDesc desc = MP_Pop();
    LONG_POINTER ptr = MP_PopLong();
    MP_Push(MP_ReadField(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_GetFieldDescOffset(desc)))), MP_GetFieldDescFieldSpec(desc)));
}

void
MP_Instruction_RCFS()
{
    /* Read Code Field Stack */
    FieldDesc desc = MP_Pop();
    CARDINAL offset = MP_Pop();
    MP_Push(MP_ReadField(MP_ReadCode((CARDINAL)(offset + MP_GetFieldDescOffset(desc))), MP_GetFieldDescFieldSpec(desc)));
}

void
MP_Instruction_RLIPF()
{
    /* Read Local Indirect Pair Field */
    BYTE pair = MP_GetCodeByte();
    FieldSpec spec = MP_GetCodeByte();
    POINTER ptr = MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair))));
    MP_Push(MP_ReadField(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + MP_NibbleRight(pair)))), spec));
}

void
MP_Instruction_RLILPF()
{
    /* Read Local Indirect Long Pair Field */
    BYTE pair = MP_GetCodeByte();
    FieldSpec spec = MP_GetCodeByte();
    LONG_POINTER ptr = MP_ReadDblMds((POINTER)(MP_Mesa_Processor.registers.LF + MP_NibbleLeft(pair)));
    MP_Push(MP_ReadField(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_NibbleRight(pair)))), spec));
}

void
MP_Instruction_WF()
{
    /* Write Field */
    FieldDesc desc = MP_GetCodeWord();
    POINTER ptr = MP_Pop();
    UNSPECIFIED data = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + MP_GetFieldDescOffset(desc))), MP_WriteField(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + MP_GetFieldDescOffset(desc)))), MP_GetFieldDescFieldSpec(desc), data));
}

void
MP_Instruction_W0F()
{
    /* Write Zero Field */
    FieldSpec spec = MP_GetCodeByte();
    POINTER ptr = MP_Pop();
    UNSPECIFIED data = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds(ptr), MP_WriteField(MP_ReadMesaRealAddress(MP_FetchMds(ptr)), spec, data));
}

void
MP_Instruction_WLF()
{
    /* Write Long Field */
    FieldDesc desc = MP_GetCodeWord();
    LONG_POINTER ptr = MP_PopLong();
    UNSPECIFIED data = MP_Pop();
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)MP_GetFieldDescOffset(desc))), MP_WriteField(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_GetFieldDescOffset(desc)))), MP_GetFieldDescFieldSpec(desc), data));
}

void
MP_Instruction_WL0F()
{
    /* Write Long Zero Field */
    FieldSpec spec = MP_GetCodeByte();
    LONG_POINTER ptr = MP_PopLong();
    UNSPECIFIED data = MP_Pop();
    MP_WriteMesaRealAddress(MP_Store(ptr), MP_WriteField(MP_ReadMesaRealAddress(MP_Fetch(ptr)), spec, data));
}

void
MP_Instruction_WLFS()
{
    /* Write Long Field Stack */
    FieldDesc desc = MP_Pop();
    LONG_POINTER ptr = MP_PopLong();
    UNSPECIFIED data = MP_Pop();
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)MP_GetFieldDescOffset(desc))), MP_WriteField(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_GetFieldDescOffset(desc)))), MP_GetFieldDescFieldSpec(desc), data));
}

void
MP_Instruction_WS0F()
{
    /* Write Swapped Zero Field */
    MP_WriteSwappedZeroField();
}

void
MP_Instruction_PS0F()
{
    /* Put Swapped Zero Field */
    MP_WriteSwappedZeroField();
    MP_Recover();
}

void
MP_Instruction_PSF()
{
    /* Put Swapped Field */
    FieldDesc desc = MP_GetCodeWord();
    UNSPECIFIED data = MP_Pop();
    POINTER ptr = MP_Pop();
    MP_WriteMesaRealAddress(MP_StoreMds((POINTER)(ptr + MP_GetFieldDescOffset(desc))), MP_WriteField(MP_ReadMesaRealAddress(MP_FetchMds((POINTER)(ptr + MP_GetFieldDescOffset(desc)))), MP_GetFieldDescFieldSpec(desc), data));
    MP_Recover();
}

void
MP_Instruction_PSLF()
{
    /* Put Swapped Long Field */
    FieldDesc desc = MP_GetCodeWord();
    UNSPECIFIED data = MP_Pop();
    LONG_POINTER ptr = MP_PopLong();
    MP_WriteMesaRealAddress(MP_Store(ptr + ((LONG_POINTER)MP_GetFieldDescOffset(desc))), MP_WriteField(MP_ReadMesaRealAddress(MP_Fetch(ptr + ((LONG_POINTER)MP_GetFieldDescOffset(desc)))), MP_GetFieldDescFieldSpec(desc), data));
    MP_Recover();
    MP_Recover();
}

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
