// MesaProcessor.h : header file for Win32 specific Mesa Processor initialization
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAPROCESSOR_MESAPROCESSOR_H
#define _SOURCE_MESADLL_MESAPROCESSOR_MESAPROCESSOR_H

#include <PrincOps/PrincOps.h>

/////////////////////////////////////////////////////////////////////////////
// class MesaProcessor
class MesaProcessor
{
public:
    static bool Initialize();
    static bool Run();
    static u32  GetPulseCount();

public:
    static u32    GetRealMemoryPageCount();
    static u32    GetVirtualMemoryPageCount();
    static u32    GetNumExecutedInstructions();
    static u16 *  GetRealAddress(u32 mesaVirtualAddress, bool bReadWrite);
        // GetRealAddress returns the real address represented by mesaVirtualAddress.
        // It returns 0 if mesaVirtualAddress is not mapped or if the page is readOnly
        // and bReadWrite is true.
    static u16 *  GetStartOfRealMemory();
    static u32    GetPSB();

private:
    MesaProcessor(u32 nSizeOfRealMemoryInPages, u32 nSizeOfVirtualMemoryInPages);
    ~MesaProcessor();

private:
    static MesaProcessor * s_pThis;
public:
    enum { kBytesPerPage = 512, kWordsPerPage = 256, kDWordsPerPage = 128 };

private:
    u16 * m_pRealMemory;
    u16 * m_pDisplayMemory;
    u32 * m_pVMMap;
};

#endif // #ifndef _SOURCE_MESADLL_MESAPROCESSOR_MESAPROCESSOR_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
