/*
    MP_Undocumented.c - implementation of Mesa processor undocumented instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include <PrincOps/PrincOps_Microcode.h>

/*******************************************************************************
** UNDOCUMENTED INSTRUCTIONS */
void
MP_Instruction_LGA0()
{
    /* Long Global Address 0 */
    MP_PushLong(MP_Mesa_Processor.registers.GF);
}

void
MP_Instruction_LGAB()
{
    /* Long Global Address Byte */
    BYTE alpha = MP_GetCodeByte();
    MP_PushLong(MP_Mesa_Processor.registers.GF + alpha);
}

void
MP_Instruction_LGAW()
{
    /* Long Global Address Word */
    UNSPECIFIED word = MP_GetCodeWord();
    MP_PushLong(MP_Mesa_Processor.registers.GF + word);
}

void
MP_Instruction_DESC()
{
    /* Descriptor */
    UNSPECIFIED word = MP_GetCodeWord();
    MP_Push((UNSPECIFIED)(MP_Mesa_Processor.registers.GFI | 3));
    MP_Push(word);
}

void
MP_Instruction_VERSION()
{
    /* Version */
    /*
        Not in PrincOps manual; from 6085 microcode.
        1. Push word 0 of VersionResult.
        2. Push word 1 of VersionResult.
        VersionResult: TYPE = MACHINE DEPENDENT RECORD [
           machineType (0: 0..3): MachineType,
           majorVersion (0: 4..7): [0..17B],  -- incremented by incompatible changes
           unused (0: 8..13): [0..77B],
           floatingPoint (0: 14..14): BOOLEAN,
           cedar (0: 15..15): BOOLEAN,
           releaseDate (1): CARDINAL];  -- days since January 1, 1901
    */
    MP_Push(0x8002);    
    MP_Push(0x8482);     /* Jan 1 1993 */

}

void
MP_Instruction_BYTESWAP()
{
    /* Byte Swap */
    UNSPECIFIED u = MP_Pop();
    u = ((u << 8) | (u >> 8));
    MP_Push(u);
}

void
MP_Instruction_VMFIND()
{
    /* VM Find */
    POINTER pRunTop = MP_Pop();
    LONG_POINTER lpRunBase = MP_PopLong();
    LONG_CARDINAL page = MP_PopLong();
    BOOLEAN bFound = MESA_FALSE;
    POINTER pRunFound = 0;

    // need to go from 0 to pRunTop looking for page
    // in the interval
    CARDINAL nRunIndexLow = VMDataRunIndexFirst;
    CARDINAL nRunIndexHigh = MP_VMDataRunIndexFromPointer((CARDINAL)(pRunTop - VMDataRunPointerFirst));
    CARDINAL nRunIndexCurrent = 0;
    LONG_CARDINAL nIntervalPageCurrent = 0;
    LONG_CARDINAL nIntervalPageCountCurrent = 0;

    while (0 == pRunFound)
    {
        nRunIndexCurrent = (nRunIndexLow + nRunIndexHigh) / 2;
        if (MESA_FALSE == MP_GetVMDataRunVMInterval(MP_VMDataRunLongPointerFromIndex(lpRunBase, nRunIndexCurrent), &nIntervalPageCurrent, &nIntervalPageCountCurrent))
        {
            MP_Error();
        }

        if (nIntervalPageCurrent > page)
        {
            nRunIndexHigh = nRunIndexCurrent - 1;
        }
        else
        if (page > nIntervalPageCurrent)
        {
            nRunIndexLow = nRunIndexCurrent + 1;
        }
        else
        {
            // EXACT
            bFound = MESA_TRUE;
            pRunFound = MP_VMDataRunPointerFromIndex(nRunIndexCurrent);
            break;
        }

        if (nRunIndexHigh < nRunIndexLow)
        {
            // NOT EXACT
            bFound = MESA_FALSE;
            if (nRunIndexLow == VMDataRunIndexFirst) pRunFound = MP_VMDataRunPointerFromIndex(VMDataRunIndexFirst);
            else
            {
                pRunFound = MP_VMDataRunPointerFromIndex(nRunIndexHigh);
                if (MESA_FALSE == MP_GetVMDataRunVMInterval(MP_VMDataRunLongPointerFromPointer(lpRunBase, pRunFound), &nIntervalPageCurrent, &nIntervalPageCountCurrent))
                {
                    MP_Error();
                }
                if (page < (nIntervalPageCurrent + nIntervalPageCountCurrent))
                {
                    bFound = MESA_TRUE;
                }
                else
                {
                    pRunFound += VMDataRunSize;
                }
            }
        }
    }

    MP_Push(bFound);
    MP_Push(pRunFound);
}

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
