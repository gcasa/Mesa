// MesaProcessor.cpp : implements Win32 specific initialization for Mesa Processor
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include <PrincOps/PrincOps_Microcode.h>

#include "MesaProcessor.h"
#include "MesaMicrocode.h"
#include "MesaMicrocodeOps.h"
#include "../MesaAgent/MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class MesaProcessor static variables
MesaProcessor * MesaProcessor::s_pThis = 0;

/////////////////////////////////////////////////////////////////////////////
// class MesaProcessor #defines and static constants
#define BOOT_TAJO
//#define BOOT_TAJO
//#define BOOT_OTHELLO
//#define BOOT_VIEWPOINT
//#define BOOT_VIEWPOINT_8N_SWITCH

static const u32 s_kSizeOfVMMapInPages = 512;

#if defined(BOOT_TAJO)
    static const u32 s_kSizeOfRealMemoryInPages = 2048;
    static const u32 s_kSizeOfVirtualMemoryInPages = 32768;
    static const char * s_kpBootSwitches = "8W";
#elif defined (BOOT_OTHELLO)
    static const u32 s_kSizeOfRealMemoryInPages = 2048;
    static const u32 s_kSizeOfVirtualMemoryInPages = 32768;
    static const char * s_kpBootSwitches = ".58\\364\\373\\375";
#elif defined (BOOT_VIEWPOINT)
    static const u32 s_kSizeOfRealMemoryInPages = 8192;
    static const u32 s_kSizeOfVirtualMemoryInPages = 65536;
    //static const char * s_kpBootSwitches = "Oy{|}\\346\\347\\350\\377";
    static const char * s_kpBootSwitches = "Oy{|}\\350";
#elif defined (BOOT_VIEWPOINT_8N_SWITCH)
    static const u32 s_kSizeOfRealMemoryInPages = 8192;
    static const u32 s_kSizeOfVirtualMemoryInPages = 65536;
    static const char * s_kpBootSwitches = "8Ny{|}\\346\\347\\350\\377";
#else
    static const u32 s_kSizeOfRealMemoryInPages = 4096;
    static const u32 s_kSizeOfVirtualMemoryInPages = 32768;
    static const char * s_kpBootSwitches = "8W";
#endif

/////////////////////////////////////////////////////////////////////////////
// class MesaProcessor public functions
bool
MesaProcessor::Initialize()
{
    bool bSuccess = false;
    do
    {
        if (s_pThis != 0) return false;

        /* Allocate the processor and its ram */
        if (0 == (s_pThis = new MesaProcessor(s_kSizeOfRealMemoryInPages, s_kSizeOfVMMapInPages))) break;
        if (0 == s_pThis->m_pRealMemory) break;
        if (0 == s_pThis->m_pVMMap) break;
        if (0 == s_pThis->m_pDisplayMemory) break;

        /* Initialize our microcode operations and agents */
        if (! MesaMicrocodeOps::Initialize()) break;
        if (! MesaAgent::Initialize(s_pThis->m_pRealMemory, s_pThis->m_pDisplayMemory)) break;

        /* Configure the processor */
        PrincOps_Configuration_Struct poStruct;
        poStruct.pStartOfRealMemory = s_pThis->m_pRealMemory;
        poStruct.nNumRealMemoryPages = s_kSizeOfRealMemoryInPages;
        poStruct.nNumVirtualMemoryPages = s_kSizeOfVirtualMemoryInPages;
        poStruct.nNumDeviceIOPages = MesaAgent::GetNumDevicePages();
        poStruct.pVMMap = s_pThis->m_pVMMap;
        poStruct.nNumVMMapPages = s_kSizeOfVMMapInPages;
        poStruct.pGetInterruptMaskFunction = &MesaMicrocode_GetInterruptMaskFunction;
        poStruct.pSetInterruptMaskFunction = &MesaMicrocode_SetInterruptMaskFunction;
        poStruct.pWriteMPCodeFunction = &MesaMicrocode_WriteMPCodeFunction;
        poStruct.pGetPulseCountFunction = &MesaMicrocode_PulseCountFunction;
        poStruct.nPulseResolutionInMicroseconds = MesaMicrocodeOps::GetPulseResolutionInMicroseconds();

        if (0 == PrincOps_Configure(&poStruct)) break;

        /* Install our processor specific instructions */
        PrincOps_InstallInstructionEsc(0x81, MesaMicrocode_GetRGB_MVM);
        PrincOps_InstallInstructionEsc(0x82, MesaMicrocode_Snap_Toggle_State);
        PrincOps_InstallInstructionEsc(0x89, MesaMicrocode_CALLAGENT);
        PrincOps_InstallInstructionEsc(0x8A, MesaMicrocode_MAPDISPLAY);
        PrincOps_InstallInstructionEsc(0x8B, MesaMicrocode_STOPEMULATOR);
        PrincOps_InstallInstructionEsc(0x8D, MesaMicrocode_SUSPEND);

        PrincOps_InstallInstructionEsc(0xC0, MesaMicrocode_COLORBLT);
        PrincOps_InstallInstructionEsc(0xC1, MesaMicrocode_WRITEPIXEL);
        PrincOps_InstallInstructionEsc(0xC2, MesaMicrocode_BITBLTX);
        PrincOps_InstallInstructionEsc(0x2B, MesaMicrocode_BITBLT);

        /* Load the germ */
        if (! MesaMicrocodeOps::LoadGerm()) break;

        /* Set the boot request */
        PrincOps_BootRequest bootRequest;
        PrincOps_MakeDiskBootRequest(&bootRequest);
        if (0 == PrincOps_SetGermBootRequest(&bootRequest)) break;

        if (0 == PrincOps_SetGermBootSwitches(s_kpBootSwitches)) break;

        bSuccess = true;
    }
    while (false);
    
    if ((! bSuccess) && s_pThis) { delete s_pThis; s_pThis = 0; }

    return bSuccess;
}

bool
MesaProcessor::Run()
{
    MesaMicrocodeOps::SetRunningMessage();
    u8 retVal = (s_pThis ? PrincOps_Run() : 0);

    if (retVal == 0) MesaMicrocodeOps::SetFailureMessage();
    else MesaMicrocodeOps::SetSuccessMessage();

    if (s_pThis) { delete s_pThis; s_pThis = 0; }

    return (retVal == 0) ? false : true;
}

u32
MesaProcessor::GetNumExecutedInstructions()
{
   return MP_Mesa_Processor.state.instructionsDispatched;
}

u32
MesaProcessor::GetRealMemoryPageCount()
{
    return MP_Mesa_Processor.config.m_nNumRealMemoryPages;
}

u32
MesaProcessor::GetVirtualMemoryPageCount()
{
    return MP_Mesa_Processor.config.m_nNumVirtualMemoryPages;
}

u16 *
MesaProcessor::GetRealAddress(u32 mesaVirtualAddress, bool bReadWrite)
{
    // returns NULL if not mapped or page is write protected and writing is requested
    MapFlags nMapFlags;
    RealPageNumber nRealPageNumber;
    VirtualPageNumber nVirtualPageNumber = mesaVirtualAddress / PageSize;
    RealAddress nAddressOffset = mesaVirtualAddress % PageSize;
    MP_ReadMap(nVirtualPageNumber, &nMapFlags, &nRealPageNumber);
    if (MP_Vacant(nMapFlags)) return 0;
    if (bReadWrite && (nMapFlags & MapFlagsProtected)) return 0;
    return MP_Mesa_Processor.config.m_pRealMemoryBaseAddress + (nRealPageNumber * PageSize + nAddressOffset);
}

u16 *
MesaProcessor::GetStartOfRealMemory()
{
    return MP_Mesa_Processor.config.m_pRealMemoryBaseAddress;
}

u32
MesaProcessor::GetPSB()
{
    return MP_Mesa_Processor.registers.PSB;
}

/////////////////////////////////////////////////////////////////////////////
// class MesaProcessor private functions
MesaProcessor::MesaProcessor(u32 nSizeOfRealMemoryInPages, u32 nSizeOfVirtualMemoryInPages)
{
    u32 nNumDisplayPages = MesaAgent::GetDisplayMemoryPageRequirement();
    
    m_pRealMemory = new u16[nSizeOfRealMemoryInPages * kWordsPerPage + nNumDisplayPages * kWordsPerPage];
    m_pDisplayMemory = m_pRealMemory + nSizeOfRealMemoryInPages * kWordsPerPage;
    m_pVMMap = new u32[nSizeOfVirtualMemoryInPages * kDWordsPerPage];
}

MesaProcessor::~MesaProcessor()
{
    MesaAgent::Uninitialize();
    MesaMicrocodeOps::Uninitialize();
    if (m_pRealMemory != 0) delete [] m_pRealMemory;
    if (m_pVMMap != 0) delete [] m_pVMMap;
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
