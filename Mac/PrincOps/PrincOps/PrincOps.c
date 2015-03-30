/*
    PrincOps.c - main implementation of PrincOps processor

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include <PrincOps/PrincOps_Microcode.h>
#include <setjmp.h>

/*******************************************************************************
** module PrincOps.c private static variables */
static u8 s_bRunning = 0;

/*******************************************************************************
** module PrincOps.c private static functions */
static void
PrincOps_ClearMem(void * pMem, u32 nSize)
{
    u8 * pMemory = (u8 *)pMem;
    while (nSize--) *pMemory++ = 0;
}

static u32
PrincOps_OctalStringToDecimal(const char * pOctal)
{
    u8 digit1 = *pOctal - '0';
    u8 digit2 = *(pOctal + 1) - '0';
    u8 digit3 = *(pOctal + 2) - '0';

    return ((digit1 * 64) + (digit2 * 8) + digit3);
}


/*******************************************************************************
** PrincOps.h public functions */
u8
PrincOps_Configure(PrincOps_Configuration_Struct * pConfigurationStruct)
{
    /* 1.  Need to ensure pStartOfRealMemory and pVMMap are aligned on a 16 bit address
       2.  Need to make sure real mem at least 1536 words; virtual mem is at least real mem; device mem is at least 1 page.
       3.  Need to make sure pVMMap is big enough for 65536 map entries
       4.  Need to initialize VMMap
       5.  Need to perform all other initialization.
       6.  Only when successful, set up my variables and return 1.
    */
    
    u8 nRetVal = 0;

    LONG_CARDINAL nTimeoutInterval = 0;
    u32 nRealPage;
    u32 nVirtualPage;
    u32 nFirstDevicePage;

    /* determine if external memory byte swapping on mesa words is required */
    u16  mesaNum;
    u16  mpNum = (u16)C_TO_MESA_UNSPECIFIED(0xDEAD);
    u8 * pMesaNum = (u8 *)&mesaNum;
    *pMesaNum++ = 0xDE;
    *pMesaNum = 0xAD;

    /* zero MP_Mesa_Processor without runtime lib */
    PrincOps_ClearMem(&MP_Mesa_Processor, sizeof(MP_Mesa_Processor));    

    MP_Mesa_Processor.config.m_bByteSwapExternalMemory = ((mesaNum == mpNum) ? MESA_FALSE : MESA_TRUE);
    
    do
    {
        /* first check configuration struct */

        /* check real memory */
        /* make sure we have the real memory and that it starts on a word boundary */
        if (0 == pConfigurationStruct->pStartOfRealMemory) break;
        if (0 != ((u32)(pConfigurationStruct->pStartOfRealMemory) & 1)) break;

        /* make sure we have [1536..65536] pages of real memory. */
        if (pConfigurationStruct->nNumRealMemoryPages < 1536) break;
        if (pConfigurationStruct->nNumRealMemoryPages > 65536) break;

        /* make sure we have [nNumRealMemoryPages..65536] pages of virtual memory */
        if (pConfigurationStruct->nNumVirtualMemoryPages < pConfigurationStruct->nNumRealMemoryPages) break;
        if (pConfigurationStruct->nNumVirtualMemoryPages > 65536) break;

        /* make sure we have [1..256] device pages */
        if (pConfigurationStruct->nNumDeviceIOPages < 1) break;
        if (pConfigurationStruct->nNumDeviceIOPages > 256) break;

        /* make sure we have a big enough vmmap and that it starts on a word boundary */
        if (0 == pConfigurationStruct->pVMMap) break;
        if (0 != ((u32)(pConfigurationStruct->pVMMap) & 1)) break;
        if ((pConfigurationStruct->nNumVMMapPages * 512) < (65536 * sizeof(MapEntry))) break;
        
        /* make sure we have our callbacks */
        if (0 == pConfigurationStruct->pGetPulseCountFunction) break;
        if (0 == pConfigurationStruct->pWriteMPCodeFunction) break;
        if (0 == pConfigurationStruct->pGetInterruptMaskFunction) break;
        if (0 == pConfigurationStruct->pSetInterruptMaskFunction) break;
        
        /* make sure we a valid pulse resolution in [1..100] */
        if (pConfigurationStruct->nPulseResolutionInMicroseconds < 1) break;
        if (pConfigurationStruct->nPulseResolutionInMicroseconds > 100) break;
        
        /* ok, everything cool.  setup our processor */

        /* first figure out how many pulses there are in 40 milliseconds */
        nTimeoutInterval = 40 * 1000; /* 40 milliseconds in microseconds */
        nTimeoutInterval /= (LONG_CARDINAL)pConfigurationStruct->nPulseResolutionInMicroseconds;

        /* Ok, now set up the configuration of the mesa processor */
        MP_Mesa_Processor.config.m_pGetPulseCountFunction = pConfigurationStruct->pGetPulseCountFunction;
        MP_Mesa_Processor.config.m_pWriteMPCodeFunction = pConfigurationStruct->pWriteMPCodeFunction;
        MP_Mesa_Processor.config.m_pGetInterruptMaskFunction = pConfigurationStruct->pGetInterruptMaskFunction;
        MP_Mesa_Processor.config.m_pSetInterruptMaskFunction = pConfigurationStruct->pSetInterruptMaskFunction;
        MP_Mesa_Processor.config.m_nNumRealMemoryPages = pConfigurationStruct->nNumRealMemoryPages;
        MP_Mesa_Processor.config.m_pRealMemoryBaseAddress = pConfigurationStruct->pStartOfRealMemory;
        MP_Mesa_Processor.config.m_nNumVirtualMemoryPages = pConfigurationStruct->nNumVirtualMemoryPages;
        MP_Mesa_Processor.config.m_pVMMap = pConfigurationStruct->pVMMap;
        MP_Mesa_Processor.config.TimeOutInterval = 40 * 1000 / pConfigurationStruct->nPulseResolutionInMicroseconds;
        
        /* Initialize the virtual memory map */
        nFirstDevicePage = 256 - pConfigurationStruct->nNumDeviceIOPages;
        nRealPage = 0;

        /* 1.  First map the used device memory from real 0 up to the end of the virtual device area. */
        for (nVirtualPage = nFirstDevicePage; nVirtualPage < 256; nVirtualPage++, nRealPage++)
        {
            *(MP_Mesa_Processor.config.m_pVMMap + nVirtualPage) = nRealPage << 8;
        }

        /* 2.  Continue mapping the unused device area starting at virtual 0. */
        for (nVirtualPage = 0; nVirtualPage < nFirstDevicePage; nVirtualPage++, nRealPage++)
        {
            *(MP_Mesa_Processor.config.m_pVMMap + nVirtualPage) = nRealPage << 8;
        }

        /* 3.  The device area (first 256 virtual pages) is now fully mapped; map the rest of real memory. */
        /* ASSERT(nRealPage == 256); */
        for (nVirtualPage = 256; nVirtualPage < MP_Mesa_Processor.config.m_nNumRealMemoryPages; nVirtualPage++, nRealPage++)
        {
            *(MP_Mesa_Processor.config.m_pVMMap + nVirtualPage) = nRealPage << 8;
        }

        /* 4.  Now mark the rest of virtual memory vacant. */
        for (; nVirtualPage < 65536; nVirtualPage++)
        {
            *(MP_Mesa_Processor.config.m_pVMMap + nVirtualPage) = MapFlagsVacantValue;
        }

        /* Finally, we're done! */
        nRetVal = 1;
    }
    while (0);

    return nRetVal;
}

PrincOps_Instruction_Function *
PrincOps_InstallInstruction(u8 opcode, PrincOps_Instruction_Function * pInstructionFunction)
{
    PrincOps_Instruction_Function * pInstructionFunctionOld;
    if ((opcode == 0) || (opcode == 0xF8) || (opcode == 0xF9) || (opcode == 0xFF)) return 0;
    pInstructionFunctionOld = MP_Mesa_Opcode_Table[opcode];
    MP_Mesa_Opcode_Table[opcode] = pInstructionFunction;
    return pInstructionFunctionOld;
}

PrincOps_Instruction_Function *
PrincOps_InstallInstructionEsc(u8 opcodeEsc, PrincOps_Instruction_Function * pInstructionFunction)
{
    PrincOps_Instruction_Function * pInstructionFunctionOld;
    if ((opcodeEsc == 0) || (opcodeEsc == 0xFF)) return 0;
    pInstructionFunctionOld = MP_Mesa_OpcodeEsc_Table[opcodeEsc];
    MP_Mesa_OpcodeEsc_Table[opcodeEsc] = pInstructionFunction;
    return pInstructionFunctionOld;
}

PrincOps_Instruction_Function *
PrincOps_InstallInstructionEscL(u8 opcodeEscL, PrincOps_Instruction_Function * pInstructionFunction)
{
    PrincOps_Instruction_Function * pInstructionFunctionOld;
    if ((opcodeEscL == 0) || (opcodeEscL == 0xFF)) return 0;
    pInstructionFunctionOld = MP_Mesa_OpcodeEscL_Table[opcodeEscL];
    MP_Mesa_OpcodeEscL_Table[opcodeEscL] = pInstructionFunction;
    return pInstructionFunctionOld;
}

u8
PrincOps_LoadGermData(const u8 * pGermData, u32 nGermDataSize, u32 nGermPage)
{
    /* The first page of the germ file goes at virtual page 512 */
    VirtualAddress germVirtualPage = (nGermPage == 0 ? 512 : nGermPage);
    RealAddress germRealPage = 0;
    MapFlags nMapFlags = 0;
    u8 * germRealAddress = (u8 *)MP_Mesa_Processor.config.m_pRealMemoryBaseAddress;

    if ((germRealAddress == 0) || (pGermData == 0) || (nGermDataSize == 0) || (nGermDataSize & 1)) return 0;
    if ((nGermPage == 0) && (nGermDataSize > 512)) return 0;

    MP_ReadMap(germVirtualPage, &nMapFlags, &germRealPage);
    if (MP_Vacant(nMapFlags)) return 0;
    
    germRealAddress += (germRealPage * 512);
    
    if (MP_Mesa_Processor.config.m_bByteSwapExternalMemory == MESA_TRUE)
    {
        while (nGermDataSize > 0)
        {
            *germRealAddress++ = *(pGermData + 1);
            *germRealAddress++ = *pGermData;
            pGermData += 2;
            nGermDataSize -= 2;
        }
    }
    else
    {
        while (nGermDataSize-- > 0) *germRealAddress++ = *pGermData++;
    }

    return 1;
}

void
PrincOps_MakeDiskBootRequest(PrincOps_BootRequest * pBootRequest)
{
    if (0 == pBootRequest) return;
    PrincOps_ClearMem(pBootRequest, sizeof(PrincOps_BootRequest));
	pBootRequest->requestBasicVersion = PrincOps_CurrentRequestBasicVersion;
	pBootRequest->bootAction = PrincOps_BootActionBootPhysicalVolume;
	pBootRequest->deviceType = PrincOps_DeviceTypeAnyPilotDisk;
}

void
PrincOps_MakeFloppyBootRequest(PrincOps_BootRequest * pBootRequest)
{
    if (0 == pBootRequest) return;
    PrincOps_ClearMem(pBootRequest, sizeof(PrincOps_BootRequest));
	pBootRequest->requestBasicVersion = PrincOps_CurrentRequestBasicVersion;
	pBootRequest->bootAction = PrincOps_BootActionInLoad;
	pBootRequest->deviceType = PrincOps_DeviceTypeAnyFloppy;
}

void
PrincOps_MakeEthernetBootRequest(PrincOps_BootRequest * pBootRequest)
{
    if (0 == pBootRequest) return;
    PrincOps_ClearMem(pBootRequest, sizeof(PrincOps_BootRequest));
	pBootRequest->requestBasicVersion = PrincOps_CurrentRequestBasicVersion;
	pBootRequest->bootAction = PrincOps_BootActionInLoad;
	pBootRequest->deviceType = PrincOps_DeviceTypeEthernet;
	pBootRequest->bootLocation.ethernet.bootFileNumber.a = 0x0000;
	pBootRequest->bootLocation.ethernet.bootFileNumber.b = 0xAA00;
	pBootRequest->bootLocation.ethernet.bootFileNumber.c = 0x0E60;
	pBootRequest->bootLocation.ethernet.address.net.a =    0x0000;
	pBootRequest->bootLocation.ethernet.address.net.b =    0x0000;
	pBootRequest->bootLocation.ethernet.address.host.a =   0xFFFF;
	pBootRequest->bootLocation.ethernet.address.host.b =   0xFFFF;
	pBootRequest->bootLocation.ethernet.address.host.c =   0xFFFF;
	pBootRequest->bootLocation.ethernet.address.socket =   0x000A;
}

void
PrincOps_MakeStreamBootRequest(PrincOps_BootRequest * pBootRequest)
{
    if (0 == pBootRequest) return;
    PrincOps_ClearMem(pBootRequest, sizeof(PrincOps_BootRequest));
	pBootRequest->requestBasicVersion = PrincOps_CurrentRequestBasicVersion;
	pBootRequest->bootAction = PrincOps_BootActionInLoad;
	pBootRequest->deviceType = PrincOps_DeviceTypeStream;
}

u8
PrincOps_SetGermBootRequest(PrincOps_BootRequest * pBootRequest)
{
    /* Get the real address of the germ boot request area */
    /* Use MP_FetchMds instead of MP_StoreMds because we don't want to mark the virtual
       page as dirty at this point */
    LONG_POINTER nRealAddress = MP_FetchMds(MP_GetSDOffset(sFirstGermRequest));
    u16 * pRealAddress = MP_Mesa_Processor.config.m_pRealMemoryBaseAddress;
    if (pRealAddress == 0) return 0;
    pRealAddress += nRealAddress;

    /* Do it to it */
    *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->requestBasicVersion);
    *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootAction);

    /* now pRealAddress is pointing to the beginning of Boot.Location */
    *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->deviceType);
    *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->deviceOrdinal);

    /* now pRealAddress is pointing to the variant part of Boot.Location */
    /* The variant part of Boot.Location is 9 words */
    if (pBootRequest->deviceType == PrincOps_DeviceTypeEthernet)
    {
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.bootFileNumber.a);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.bootFileNumber.b);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.bootFileNumber.c);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.address.net.a);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.address.net.b);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.address.host.a);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.address.host.b);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.address.host.c);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->bootLocation.ethernet.address.socket);
    }
    else
    {
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
        *pRealAddress++ = C_TO_MESA_UNSPECIFIED(0);
    }

    /* now pRealAddress points to the requestExtensionVersion member of Boot.Location */
    *pRealAddress++ = C_TO_MESA_UNSPECIFIED(pBootRequest->requestExtensionVersion);

    /* now pRealAddress points to the System.Switches part of Boot.Location */

    return 1;
}


u8
PrincOps_SetGermBootSwitches(const char * pBootSwitches)
{
	u32 nBitIndex = 0;
    u32 nWordIndex = 0;
    u32 nBitInWord = 0;
    u16 switches[16];

    /* Get the real address of the germ boot request area */
    /* Use MP_FetchMds instead of MP_StoreMds because we don't want to mark the virtual
       page as dirty at this point */
    LONG_POINTER nRealAddress = MP_FetchMds(MP_GetSDOffset(sFirstGermRequest));
    u16 * pRealAddress = MP_Mesa_Processor.config.m_pRealMemoryBaseAddress;
    if (pRealAddress == 0) return 0;
    pRealAddress += nRealAddress;
    pRealAddress += 14; /* offset to location of switches */

    for (nWordIndex = 0; nWordIndex < 16; nWordIndex++) switches[nWordIndex] = 0;

    if (pBootSwitches != 0)
    {
        while (*pBootSwitches)
        {
		    nBitIndex = 256;

            if (*pBootSwitches == '\\')
            {
			    // octal value
                pBootSwitches++;
			    nBitIndex = PrincOps_OctalStringToDecimal(pBootSwitches);
			    pBootSwitches += 3;
		    }
		    else
            {
			    // literal value
                nBitIndex = *pBootSwitches;
			    pBootSwitches++;
		    }

	        if (nBitIndex < 255)
            {		
	            nWordIndex = nBitIndex / 16;
	            nBitInWord = nBitIndex % 16;
                switches[nWordIndex] |= (1 << (15 - nBitInWord));
	        }
        }
    }

    for (nWordIndex = 0; nWordIndex < 16; nWordIndex++)
    {
        *(pRealAddress + nWordIndex) = switches[nWordIndex];
    }

    return 1;
}

u8
PrincOps_Run()
{
    if (s_bRunning) return 0;
    if (MP_Mesa_Processor.config.m_pRealMemoryBaseAddress == 0) return 0;

    s_bRunning = 1;
    MP_Processor();
    s_bRunning = 0;

    return 1;
}

u8
PrincOps_Halt()
{
    if (0 == s_bRunning) return 0;

    MP_Error();

    return 1;
}

u8
PrincOps_IsRunning()
{
    return s_bRunning;
}

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
