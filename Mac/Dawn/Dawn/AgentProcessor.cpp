// AgentProcessor.cpp : implementation of class AgentProcessor
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentProcessor.h"
#include "../AppFramework/XNSDriverClient.h"
#include "../MesaProcessor/MesaProcessor.h"
#include "../MesaProcessor/MesaMicrocodeOps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static SYSTEMTIME s_nClockSnapShot;
static double s_nCounterMultiplicand = 0;
static unsigned __int64 s_nUptimeMillisecondsAtSnapShot;

static unsigned __int64
GetUptimeMilliseconds()
{
    LARGE_INTEGER count;
    if ((0 == s_nCounterMultiplicand) && ::QueryPerformanceFrequency(&count)) s_nCounterMultiplicand = ((double)(1000i64)) / (double)(count.QuadPart);
    return ::QueryPerformanceCounter(&count) ? (unsigned __int64)(((double)count.QuadPart) * s_nCounterMultiplicand) : 0;
}


/////////////////////////////////////////////////////////////////////////////
// class AgentProcessor
AgentProcessor::AgentProcessor(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    SYSTEMTIME systemTime1900 = { 1901, 1, 0, 1, 0, 0, 0, 0 };
    FILETIME fileTime;
    if (::SystemTimeToFileTime(&systemTime1900, &fileTime))
    {
        m_nMesaEpoch = fileTime.dwHighDateTime;
        m_nMesaEpoch <<= 32;
        m_nMesaEpoch |= fileTime.dwLowDateTime;
    }
    else m_nMesaEpoch = 0;

    ::GetSystemTime(&s_nClockSnapShot);
    s_nUptimeMillisecondsAtSnapShot = ::GetUptimeMilliseconds();
}

AgentProcessor::~AgentProcessor()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentProcessor public virtual functions
bool
AgentProcessor::Initiate()
{
    FCB * pFCB = (FCB *)m_pFCB;
    switch (MESA_TO_C_UNSPECIFIED(pFCB->m_nCommand))
    {
        case kCommandInit:
            Init();
            break;
        case kCommandGetGMT:
            GetGMT();
            break;
        case kCommandSetGMT:
            SetGMT();
            break;
        default:
            break;
    }

    return true;
}

bool
AgentProcessor::Poll()
{
    return false;
}

u32
AgentProcessor::GetFCBSize()
{
    return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentProcessor private functions
void
AgentProcessor::Init()
{
    FCB * pFCB = (FCB *)m_pFCB;

    if (XNSDriverClient::GetXNSDriverClient().GetXNSProcID((char*)pFCB->m_procID))
    {
        // swap for mesa
        u8 temp = pFCB->m_procID[0];
        pFCB->m_procID[0] = pFCB->m_procID[1];
        pFCB->m_procID[1] = temp;

        temp = pFCB->m_procID[2];
        pFCB->m_procID[2] = pFCB->m_procID[3];
        pFCB->m_procID[3] = temp;

        temp = pFCB->m_procID[4];
        pFCB->m_procID[4] = pFCB->m_procID[5];
        pFCB->m_procID[5] = temp;
    }
    else
    {
        pFCB->m_procID[0] = 0;
        pFCB->m_procID[1] = 0;
        pFCB->m_procID[2] = 0;
        pFCB->m_procID[3] = 0;
        pFCB->m_procID[4] = 0;
        pFCB->m_procID[5] = 0;
    }

    pFCB->m_nMicrosecondsPerHundredPulses = C_TO_MESA_UNSPECIFIED(100 * (u16)MesaMicrocodeOps::GetPulseResolutionInMicroseconds());
    pFCB->m_nMillisecondsPerTick = C_TO_MESA_UNSPECIFIED(40);           // always 40

    pFCB->m_alignmentFiller = C_TO_MESA_UNSPECIFIED(0);
    pFCB->m_nRealMemoryPageCount = C_TO_MESA_LONG_UNSPECIFIED(MesaProcessor::GetRealMemoryPageCount());
    pFCB->m_nVirtualMemoryPageCount = C_TO_MESA_LONG_UNSPECIFIED(MesaProcessor::GetVirtualMemoryPageCount());
    pFCB->m_nMesaTimeGMT = C_TO_MESA_LONG_UNSPECIFIED(0);
    pFCB->m_nCommand = C_TO_MESA_UNSPECIFIED(kCommandInit);
    pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusSuccess);
}

void
AgentProcessor::GetGMT()
{
#if 0

    // reads the system clock
    FCB * pFCB = (FCB *)m_pFCB;
    SYSTEMTIME systemTime;
    FILETIME fileTime;
    ::GetSystemTime(&systemTime);
    
    if (::SystemTimeToFileTime(&systemTime, &fileTime))
    {
        unsigned __int64 mesaTime = fileTime.dwHighDateTime;
        mesaTime <<= 32;
        mesaTime |= fileTime.dwLowDateTime;
        mesaTime = mesaTime - m_nMesaEpoch;
        mesaTime /= 10000000;

        pFCB->m_nMesaTimeGMT = C_TO_MESA_LONG_UNSPECIFIED(((u32)mesaTime));
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusSuccess);
    }
    else
    {
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusFailure);
    }

#else
    
    // computes mesa time based on the stored snapshots plus elapsed milliseconds
    FCB * pFCB = (FCB *)m_pFCB;
    FILETIME fileTime;

    if (::SystemTimeToFileTime(&s_nClockSnapShot, &fileTime))
    {
        unsigned __int64 mesaTime = fileTime.dwHighDateTime;
        mesaTime <<= 32;
        mesaTime |= fileTime.dwLowDateTime;
        mesaTime = mesaTime - m_nMesaEpoch;
        mesaTime /= 10000000;

        mesaTime += ((::GetUptimeMilliseconds() - s_nUptimeMillisecondsAtSnapShot) / 1000);

        pFCB->m_nMesaTimeGMT = C_TO_MESA_LONG_UNSPECIFIED(((u32)mesaTime));
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusSuccess);
    }
    else
    {
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusFailure);
    }

#endif
}

void
AgentProcessor::SetGMT()
{
#if 0

    // sets the system clock
    FCB * pFCB = (FCB *)m_pFCB;
    SYSTEMTIME systemTime;
    FILETIME fileTime;
    unsigned __int64 mesaTime = MESA_TO_C_UNSPECIFIED(pFCB->m_nMesaTimeGMT);

    mesaTime *= 10000000;
    mesaTime += m_nMesaEpoch;
    fileTime.dwHighDateTime = (unsigned int)(mesaTime >> 32);
    fileTime.dwLowDateTime = (unsigned int)mesaTime;

    if (::FileTimeToSystemTime(&fileTime, &systemTime))
    {
        ::SetSystemTime(&systemTime);
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusSuccess);
    }
    else
    {
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusFailure);
    }

#else

    // sets a clock snapshot with uptime snapshot
    FCB * pFCB = (FCB *)m_pFCB;
    SYSTEMTIME systemTime;
    FILETIME fileTime;
    unsigned __int64 mesaTime = MESA_TO_C_UNSPECIFIED(pFCB->m_nMesaTimeGMT);

    mesaTime *= 10000000;
    mesaTime += m_nMesaEpoch;
    fileTime.dwHighDateTime = (unsigned int)(mesaTime >> 32);
    fileTime.dwLowDateTime = (unsigned int)mesaTime;

    if (::FileTimeToSystemTime(&fileTime, &systemTime))
    {
        s_nClockSnapShot = systemTime;
        s_nUptimeMillisecondsAtSnapShot = ::GetUptimeMilliseconds();
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusSuccess);
    }
    else
    {
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kStatusFailure);
    }

#endif
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
