// MesaMicrocodeOps.h : header file for Win32 Microcode ops for Mesa Processor
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAPROCESSOR_MESAMICROCODEOPS_H
#define _SOURCE_MESADLL_MESAPROCESSOR_MESAMICROCODEOPS_H

#include <PrincOps/PrincOps_PlatformTypes.h>

/////////////////////////////////////////////////////////////////////////////
// class MesaMicrocodeOps
class MesaMicrocodeOps
{
public:
    static bool Initialize();
    static bool Uninitialize();

    static bool LoadGerm();

    static u32  GetPulseResolutionInMicroseconds();
    static u32  GetPulseCount();

    static void SetMPCode(u16 mpCode);
    static void SetIdleMessage();
    static void SetRunningMessage();
    static void SetFailureMessage();
    static void SetSuccessMessage();
    static u16  GetInterruptMask();
    static void SetInterruptMask(u16 nMask);
    static void RaiseInterrupt(u16 nInterrupt);

    static u32  LookupSysColor(u16 colorIndex);

private:
    static bool LoadGermFromResources();

};

#endif // #ifndef _SOURCE_MESADLL_MESAPROCESSOR_MESAMICROCODEOPS_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
