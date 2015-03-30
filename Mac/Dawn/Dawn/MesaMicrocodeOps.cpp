// MesaMicrocode.cpp : implements Win32 specific Microcode for Mesa Processor
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

// #include "stdafx.h"
#include "MesaMicrocodeOps.h"
#include "MesaProcessor.h"
#include "../AppFramework/DawnApp.h"
#include "../AppFramework/DawnFrame.h"
#include "../MesaAgent/MesaAgent.h"
// #include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef __APPLE__
#define u64 unsigned
typedef char* CString;
#define LARGE_INTEGER long long
#else
#define u64 unsigned __int64
#endif 


/////////////////////////////////////////////////////////////////////////////
// module MesaMicrocodeOps static variables
static u64 s_nPulseFrequencyInCountsPerSecond = 0;
static u64 s_nPulseBase = 0;
static u16 s_nInterruptMask = 0;
static u32 s_nPollSkipCount = 0;
static const u32 s_nPollSkipThreshhold = 50;        // Poll Agents every n execution cycles

/////////////////////////////////////////////////////////////////////////////
// module MesaMicrocodeOps static function declarations
static void CollectStats(CString & message);
static CString GetCommaString(u32 nNum);

/////////////////////////////////////////////////////////////////////////////
// class MesaMicrocodeOps
bool
MesaMicrocodeOps::Initialize()
{
    s_nPulseFrequencyInCountsPerSecond = 0;
    s_nPulseBase = 0;
    LARGE_INTEGER frequency;
    LARGE_INTEGER count;
    if (::QueryPerformanceFrequency(&frequency) && ::QueryPerformanceCounter(&count))
    {
        s_nPulseFrequencyInCountsPerSecond = (u64)(frequency.QuadPart);
        s_nPulseBase = (u64)(count.QuadPart);
    }

    return true;
}

bool
MesaMicrocodeOps::Uninitialize()
{
    return true;
}

bool
MesaMicrocodeOps::LoadGerm()
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    bool bSuccess = false;
    u8 buff[MesaProcessor::kBytesPerPage];
    char fileBuff[MAX_PATH];
    
    ::GetModuleFileName(NULL, fileBuff, sizeof(fileBuff));
    CString fileName(fileBuff);
    int nChar = fileName.ReverseFind('\\');
    if (nChar >= 0) fileName = fileName.Left(nChar+1);
    fileName += "Dawn.germ";

    do
    {
        hFile = ::CreateFile(
            (const char *)fileName,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hFile == INVALID_HANDLE_VALUE) return LoadGermFromResources();
        
        DWORD size = ::GetFileSize(hFile, NULL);
        DWORD sizeRead = 0;

        if (-1 == size) break;
        if (size < MesaProcessor::kBytesPerPage) break;

        if (0 == ::ReadFile(hFile, buff, sizeof(buff), &sizeRead, NULL)) break;
        if (sizeRead != MesaProcessor::kBytesPerPage) break;
        if (0 == PrincOps_LoadGermData(buff, sizeRead, 0)) break;

        size -= sizeRead;
        u32 pageNum = 1;
        while (size > 0)
        {
            if (0 == ::ReadFile(hFile, buff, sizeof(buff), &sizeRead, NULL)) { bSuccess = true; break; }
            if ((sizeRead > size) || (sizeRead == 0)) { bSuccess = true; break; }
            if (0 == PrincOps_LoadGermData(buff, sizeRead, pageNum)) { bSuccess = true; break; }
            pageNum++;
            size -= sizeRead;
        }
        if (bSuccess == true) { bSuccess = false; break; }

        bSuccess = true;
    }
    while (false);
    
    if (hFile != INVALID_HANDLE_VALUE) ::CloseHandle(hFile);

    return bSuccess;
}

bool
MesaMicrocodeOps::LoadGermFromResources()
{
    HRSRC hResource = NULL;
    DWORD size = 0;
    DWORD sizeRead = 0;
    HGLOBAL hGlobal = NULL;
    u8 * pGerm = NULL;
    bool bSuccess = false;
    u32 pageNum = 0;
    
    do
    {
        if (NULL == (hResource = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_GERM), "GERM"))) break;
        if (0 == (size = ::SizeofResource(AfxGetResourceHandle(), hResource))) break;
        if (size < MesaProcessor::kBytesPerPage) break;
        if (NULL == (hGlobal = ::LoadResource(AfxGetResourceHandle(), hResource))) break;
        if (NULL == (pGerm = (u8 *)::LockResource(hGlobal))) break;

        while (size > 0)
        {
            sizeRead = (size >= MesaProcessor::kBytesPerPage) ? MesaProcessor::kBytesPerPage : size;
            if (0 == PrincOps_LoadGermData(pGerm, sizeRead, pageNum)) { bSuccess = true; break; }
            pGerm += sizeRead;
            pageNum++;
            size -= sizeRead;
        }

        if (bSuccess == true) { bSuccess = false; break; }
        bSuccess = true;
    }
    while (false);
    
    return bSuccess;
}

u32
MesaMicrocodeOps::GetPulseResolutionInMicroseconds()
{
    return 1;
}

u32
MesaMicrocodeOps::GetPulseCount()
{
    LARGE_INTEGER count;
    if ((0 != s_nPulseFrequencyInCountsPerSecond) && ::QueryPerformanceCounter(&count))
    {
        u64 currentPulseCount = (u64)(count.QuadPart) - s_nPulseBase;
        currentPulseCount *= 1000000;
        currentPulseCount /= s_nPulseFrequencyInCountsPerSecond;
        return (u32)currentPulseCount;
    }

    return 0;
}

void
MesaMicrocodeOps::SetMPCode(u16 mpCode)
{
#ifdef DAWN_IN_FRAME_WINDOW
    ((DawnFrame*)AfxGetMainWnd())->SetMPCode(mpCode);
#endif
}

void
MesaMicrocodeOps::SetIdleMessage()
{
#ifdef DAWN_IN_FRAME_WINDOW
    CString message("Mesa Processor Idle.");
    ((DawnFrame*)AfxGetMainWnd())->SetMessageString(message);
#endif
}

void
MesaMicrocodeOps::SetRunningMessage()
{
#ifdef DAWN_IN_FRAME_WINDOW
    CString message("Mesa Processor Running...");
    ((DawnFrame*)AfxGetMainWnd())->SetMessageString(message);
    DawnApp::GetDawnView()->InvalidateRect(NULL);
#endif
}

void
MesaMicrocodeOps::SetFailureMessage()
{
#ifdef DAWN_IN_FRAME_WINDOW
    CString message("Failed to run Mesa Processor.");
    ((DawnFrame*)AfxGetMainWnd())->SetMessageString(message);
#endif
    SetMPCode(0);
}

void
MesaMicrocodeOps::SetSuccessMessage()
{
#ifdef DAWN_IN_FRAME_WINDOW
    CString message;
    CollectStats(message);
    ((DawnFrame*)AfxGetMainWnd())->SetMessageString(message);
#endif
    SetMPCode(0);
}

u32
MesaMicrocodeOps::LookupSysColor(u16 nIndex)
{
	static const int colorTable[] =
    {
        COLOR_SCROLLBAR,									
        COLOR_BACKGROUND,
        COLOR_ACTIVECAPTION,	   
        COLOR_INACTIVECAPTION,	   
        COLOR_MENU,
        COLOR_WINDOW,
        COLOR_WINDOWFRAME,
        COLOR_MENUTEXT,
        COLOR_WINDOWTEXT,
        COLOR_CAPTIONTEXT,  	   
        COLOR_ACTIVEBORDER,
        COLOR_INACTIVEBORDER,
        COLOR_APPWORKSPACE,
        COLOR_HIGHLIGHT,
        COLOR_HIGHLIGHTTEXT,
        COLOR_BTNFACE,            
        COLOR_BTNSHADOW,          
        COLOR_GRAYTEXT,           
        COLOR_BTNTEXT,
        COLOR_INACTIVECAPTIONTEXT,
        COLOR_BTNHIGHLIGHT
    };

    return ::GetSysColor((nIndex > (sizeof(colorTable) / sizeof(colorTable[0]))) ? 0 : nIndex);
}

/////////////////////////////////////////////////////////////////////////////
// module MesaMicrocodeOps static functions
static void
CollectStats(CString & message)
{
    // collect total time

    u32 pulseCount = 0;
    LARGE_INTEGER count;
    if ((0 != s_nPulseFrequencyInCountsPerSecond) && ::QueryPerformanceCounter(&count))
    {
        u64 currentPulseCount = (u64)(count.QuadPart) - s_nPulseBase;
        currentPulseCount *= 1000000;
        currentPulseCount /= s_nPulseFrequencyInCountsPerSecond;
        pulseCount = (u32)currentPulseCount;
    }

    u32 milliseconds = pulseCount / 1000;
    u32 seconds = milliseconds / 1000;
    u32 minutes = seconds / 60;
    u32 minsec = seconds % 60;
    milliseconds = milliseconds % 1000;

    u32 numInstructions = MesaProcessor::GetNumExecutedInstructions();

    message.Format("Mesa Processor Finished.  "
                   "Total Runtime = %s microseconds, "
                   "%s.%03d seconds, "
                   "%d:%d.%03d minutes.  "
                   "Instructions Dispatched = %s.  ",
                    GetCommaString(pulseCount),
                    GetCommaString(seconds), milliseconds,
                    minutes, minsec, milliseconds,
                    GetCommaString(numInstructions));

    // compute the microseconds per instruction or instructions per microsecond
    CString additional;
    if (pulseCount > numInstructions)
    {
        // microseconds per instruction
        u64 microseconds = ((u64)pulseCount * 100) / numInstructions;
        u32 msi1 = (u32)(microseconds / 100);
        u32 msi2 = (u32)(microseconds % 100);
        additional.Format("%s.%02d microseconds per instruction.", GetCommaString(msi1), msi2);
    }
    else
    {
        // instructions per microsecond
        u64 instructions = ((u64)numInstructions * 100) / pulseCount;
        u32 ims1 = (u32)(instructions / 100);
        u32 ims2 = (u32)(instructions % 100);
        additional.Format("%s.%02d instructions per microsecond.", GetCommaString(ims1), ims2);
    }
    message += additional;
}

static CString
GetCommaString(u32 nNum)
{
    if (nNum == 0) return CString("0");
    
    CString commaString;
    bool bAddComma = false;

    while (nNum > 0)
    {
        u32 thisNum = nNum % 1000;
        nNum /= 1000;
        CString thisNumString;
        if (nNum == 0) thisNumString.Format("%d", thisNum);
        else thisNumString.Format("%03d", thisNum);
        if (bAddComma) commaString = "," + commaString;
        commaString = thisNumString + commaString;
        bAddComma = true;
    }

    return commaString;
}

u16
MesaMicrocodeOps::GetInterruptMask()
{
    // poll the agents and dispatch a windows message every so often
    if (++s_nPollSkipCount > s_nPollSkipThreshhold)
    {
        MSG msg;
        if (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message != WM_KICKIDLE && ! AfxGetApp()->PreTranslateMessage(&msg))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        s_nPollSkipCount = 0;
        MesaAgent::PollAgents();
    }

    return s_nInterruptMask;
}

void
MesaMicrocodeOps::SetInterruptMask(u16 nMask)
{
    s_nInterruptMask = nMask;
}

void
MesaMicrocodeOps::RaiseInterrupt(u16 nInterrupt)
{
    s_nInterruptMask |= nInterrupt;
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
