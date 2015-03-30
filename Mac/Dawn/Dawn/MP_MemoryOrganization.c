/*
    MP_MemoryOrganization.c - implementation of Mesa processor Memory Organization

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include "PrincOps/PrincOps_Microcode.h"

/*******************************************************************************
** VIRTUAL MEMORY INSTRUCTIONS */
void
MP_Instruction_SM()              /* Set Map       */
{
    MapFlags nMapFlags = MP_Pop();
    RealPageNumber nRealPageNumber = MP_PopLong();
    VirtualPageNumber nVirtualPageNumber = MP_PopLong();

    /* not in the book but ajax tests require it */
    if (MP_Vacant(nMapFlags)) nRealPageNumber = 0;

    MP_WriteMap(nVirtualPageNumber, nMapFlags, nRealPageNumber);
}

void
MP_Instruction_SMF() /* Set Map Flags */
{
    MapFlags nMapFlags;
    RealPageNumber nRealPageNumber;
    MapFlags nNewMapFlags = MP_Pop();
    VirtualPageNumber nVirtualPageNumber = MP_PopLong();
    MP_ReadMap(nVirtualPageNumber, &nMapFlags, &nRealPageNumber);
    MP_Push(nMapFlags);
    MP_PushLong(nRealPageNumber);
    if (! MP_Vacant(nMapFlags))
    {
       /* not in the book but ajax tests require it */
       /* if (MP_Vacant(nNewMapFlags)) nRealPageNumber = 0; */

       MP_WriteMap(nVirtualPageNumber, nNewMapFlags, nRealPageNumber);
    }
}

void
MP_Instruction_GMF()
{
    /* Get Map Flags */
    MapFlags nMapFlags;
    RealPageNumber nRealPageNumber;
    VirtualPageNumber nVirtualPageNumber = MP_PopLong();
    MP_ReadMap(nVirtualPageNumber, &nMapFlags, &nRealPageNumber);
    MP_Push(nMapFlags);
    MP_PushLong(nRealPageNumber);
}

void
MP_Instruction_LP()
{
    /* Lengthen Pointer */
    POINTER pointer = MP_Pop();
    MP_PushLong((pointer == 0) ? 0 : MP_LengthenPointer(pointer));
}

void
MP_Instruction_ROB()
{
    /* Read Overhead Byte */
    BYTE alpha = MP_GetCodeByte();
    POINTER pointer = MP_Pop();
    if ((alpha < 1) || (alpha > 4)) MP_Error();
    MP_Push(MP_ReadMesaRealAddress(MP_FetchMds(((POINTER)(pointer - alpha)))));
}

void
MP_Instruction_WOB()
{
    /* Write Overhead Byte */
    BYTE alpha = MP_GetCodeByte();
    POINTER pointer = MP_Pop();
    if ((alpha < 1) || (alpha > 4)) MP_Error();
    MP_WriteMesaRealAddress(MP_StoreMds(((POINTER)(pointer - alpha))), MP_Pop());
}

void
MP_Instruction_RRIT()
{
    /* Read Register IT */
    MP_Mesa_Processor.registers.IT = (*MP_Mesa_Processor.config.m_pGetPulseCountFunction)();
    MP_PushLong(MP_Mesa_Processor.registers.IT);
}

void
MP_Instruction_RRMDS()
{
    /* Read Register MDS */
    MP_Push(MP_HighHalf(MP_Mesa_Processor.registers.MDS));
}

void
MP_Instruction_RRPSB()
{
    /* Read Register PSB */
    MP_Push(MP_Handle(MP_Mesa_Processor.registers.PSB));
}

void
MP_Instruction_RRPTC()
{
    /* Read Register PTC */
    MP_Push(MP_Mesa_Processor.registers.PTC);
}

void
MP_Instruction_RRWDC()
{
    /* Read Register WDC */
    MP_Push(MP_Mesa_Processor.registers.WDC);
}

void
MP_Instruction_RRWP()
{
    /* Read Register WP */
    MP_Mesa_Processor.registers.WP = (*MP_Mesa_Processor.config.m_pGetInterruptMaskFunction)();
    MP_Push(MP_Mesa_Processor.registers.WP);
}

void
MP_Instruction_RRXTS()
{
    /* Read Register XTS */
    MP_Push(MP_Mesa_Processor.registers.XTS);
}

void
MP_Instruction_WRIT()
{
    /* Write Register IT */
    /* ISSUE: Apparantly this is a no-op although it is NOT a no-op in the book! */
    /*  By the book:
        MP_Mesa_Processor.registers.IT = MP_PopLong();
    */
    /* LONG_UNSPECIFIED u = */ MP_PopLong();
}

void
MP_Instruction_WRMDS()
{
    /* Write Register MDS */
    MP_Mesa_Processor.registers.MDS = MP_LongShift(((LONG_UNSPECIFIED)MP_Pop()), WordSize);
}

void
MP_Instruction_WRMP()
{
    /* Write Register MP */
    MP_Mesa_Processor.registers.MP = MP_Pop();
    (*(MP_Mesa_Processor.config.m_pWriteMPCodeFunction))(MP_Mesa_Processor.registers.MP);
}

void
MP_Instruction_WRPSB()
{
    /* Write Register PSB */
    MP_Mesa_Processor.registers.PSB = MP_Index(MP_Pop());
}

void
MP_Instruction_WRPTC()
{
    /* Write Register PTC */
    MP_Mesa_Processor.registers.PTC = MP_Pop();
    MP_Mesa_Processor.state.time = (MP_Mesa_Processor.registers.IT = (*MP_Mesa_Processor.config.m_pGetPulseCountFunction)());
}

void
MP_Instruction_WRWDC()
{
    /* Write Register WDC */
    MP_Mesa_Processor.registers.WDC = MP_Pop();
}

void
MP_Instruction_WRWP()
{
    /* Write Register WP */
    MP_Mesa_Processor.registers.WP = MP_Pop();
    (*MP_Mesa_Processor.config.m_pSetInterruptMaskFunction)(MP_Mesa_Processor.registers.WP);
}

void
MP_Instruction_WRXTS()
{
    /* Write Register XTS */
    MP_Mesa_Processor.registers.XTS = MP_Pop();
}

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
