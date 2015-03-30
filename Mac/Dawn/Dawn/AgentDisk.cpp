// AgentDisk.cpp : implementation of class AgentDisk
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentDisk.h"
#include "MesaProcessor.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BOGUS: default install of VC++ doesn't define INVALID_SET_FILE_POINTER; you
// must install the PlatformSDK to get it.  #define it here if necessary
#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentDisk
AgentDisk::AgentDisk(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pDiskFCB = (FCB *)pFCB;
    memset(pDiskFCB, 0, sizeof(FCB));
    DCB * pDCB = &pDiskFCB->dcbs[0];
    HANDLE hFile = INVALID_HANDLE_VALUE;

    char fileBuff[MAX_PATH];
    
    ::GetModuleFileName(NULL, fileBuff, sizeof(fileBuff));
    CString fileName(fileBuff);
    int nChar = fileName.ReverseFind('\\');
    if (nChar >= 0) fileName = fileName.Left(nChar+1);
    fileName += "Dawn.dsk";

    // open the hFile read/write and get the file size
    hFile = ::CreateFile(
                (const char *)fileName,
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
//                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
                NULL);

    if (hFile == INVALID_HANDLE_VALUE) return;
    DWORD nFileSize = ::GetFileSize(hFile, NULL);
    if (nFileSize == -1)
    {
        ::CloseHandle(hFile);
        return;
    }

    // set up pDCB
    pDCB->deviceType = C_TO_MESA_UNSPECIFIED(kAnyPilotDisk);
    pDCB->numberOfCylinders = C_TO_MESA_UNSPECIFIED((u16)(nFileSize / (kPilotHeadsPerCylinder * kPilotSectorsPerTrack * 512)));
    pDCB->numberOfHeads = C_TO_MESA_UNSPECIFIED(kPilotHeadsPerCylinder);
    pDCB->pagesPerTrack = C_TO_MESA_UNSPECIFIED(kPilotSectorsPerTrack);
    *((HANDLE *)&pDCB->agentData[0]) = hFile;

    // mark that we have 1 successful disk
    pDiskFCB->numberOfDCBs = C_TO_MESA_UNSPECIFIED(1);
}

AgentDisk::~AgentDisk()
{
    FCB * pDiskFCB = (FCB *)m_pFCB;
    if (pDiskFCB->numberOfDCBs == 1)
    {
        // close the file
        DCB * pDCB = &pDiskFCB->dcbs[0];
        HANDLE hFile = *((HANDLE *)&pDCB->agentData[0]);
        ::CloseHandle(hFile);
    }
}

/////////////////////////////////////////////////////////////////////////////
// class AgentDisk public virtual functions
bool
AgentDisk::Initiate()
{
    FCB * pFCB = (FCB*)m_pFCB;

    u32 virtualAddressIOCB = MESA_TO_C_LONG_UNSPECIFIED(pFCB->virtualAddressIOCB);
    u16 interruptSelector = MESA_TO_C_UNSPECIFIED(pFCB->interruptSelector);
    u16 stopAgent = MESA_TO_C_UNSPECIFIED(pFCB->stopAgent);
    u16 agentStopped = MESA_TO_C_UNSPECIFIED(pFCB->agentStopped);
    u16 numberOfDCBs = MESA_TO_C_UNSPECIFIED(pFCB->numberOfDCBs);
    
    if (stopAgent)
    {
        pFCB->agentStopped = C_TO_MESA_UNSPECIFIED(1);
        return true;
    }

    if (0 == virtualAddressIOCB) return true;

    u32 nStartPage = 0;
    while (0 != virtualAddressIOCB)
    {
        IOCB * pIOCB = (IOCB *)MesaProcessor::GetRealAddress(virtualAddressIOCB, true);
        if (0 == pIOCB) break;

        // prepare virtualAddressIOCB for the next loop iteration
        virtualAddressIOCB = MESA_TO_C_LONG_UNSPECIFIED(pIOCB->virtualAddressNextIOCB);

        // make sure the parameters are correct
        if (! ValidateIOCB(pIOCB)) continue;

        // parameters are correct, preposition the file
        HANDLE hFile = *((HANDLE *)(&pFCB->dcbs[MESA_TO_C_UNSPECIFIED(pIOCB->diskIndex)].agentData[0]));
        u32 nSeekPosition = PilotDiskAddressToPage(pIOCB->diskAddress) * 512;
        if (INVALID_SET_FILE_POINTER == ::SetFilePointer(hFile, nSeekPosition, 0, FILE_BEGIN))
        {
            pIOCB->status = C_TO_MESA_UNSPECIFIED(seekTimeout);
            continue;
        }

        // now commence with the io on the file
        u32                 dataVirtualAddress = MESA_TO_C_LONG_UNSPECIFIED(pIOCB->dataVirtualAddress);
        u16                 incrementDataPtr = MESA_TO_C_UNSPECIFIED(pIOCB->incrementDataPtr);
        PilotDiskCommand    command = MESA_TO_C_UNSPECIFIED(pIOCB->command);
        u16                 pageCount = MESA_TO_C_UNSPECIFIED(pIOCB->pageCount);

        #if 0
            u32 nRealAddress = (u32)MesaProcessor::GetRealAddress(dataVirtualAddress, (command == read ? true : false));
            nRealAddress -= (u32)MesaProcessor::GetStartOfRealMemory();

            TRACE("%s: PSB: %03d, CHS: %03d, %d, %02d. Page: %05d to %05d. Count: %d, VirtualPage: %d to %d, RealPage: %d\n",
                 (command == read ? " READ" : (command == write ? " WRITE" : (command == verify ? "VERIFY" : "   ???"))),
                 MesaProcessor::GetPSB(),
                 (u32)pIOCB->diskAddress.cylinder, (u32)pIOCB->diskAddress.head, (u32)pIOCB->diskAddress.sector,
                 PilotDiskAddressToPage(pIOCB->diskAddress), PilotDiskAddressToPage(pIOCB->diskAddress) + pageCount - 1,
                 (u32)pageCount, dataVirtualAddress / 256, dataVirtualAddress / 256 + pageCount -1, nRealAddress / 512);

            if (0 != (dataVirtualAddress % 256))
            {
                TRACE("BOGUS VIRTUAL ADDRESS: %d\n", dataVirtualAddress);
            }

            if (0 != (nRealAddress % 512))
            {
                TRACE("   BOGUS REAL ADDRESS: %d\n", nRealAddress);
            }
        #endif

        // do the reading or writing one destination virtual page at a time since contiguous virtual pages
        // don't map to contiguous real pages
        while (pageCount > 0)
        {
            pIOCB->status = C_TO_MESA_UNSPECIFIED(DoPrePositionedSinglePageDiskIO(hFile, command, dataVirtualAddress));
            if (pIOCB->status != MESA_TO_C_UNSPECIFIED(goodCompletion)) break;

            #if 0
                        u32 nEndingPage = PilotDiskAddressToPage(pIOCB->diskAddress) + pageCount;
                        u32 nTotalTracks = nEndingPage / kPilotSectorsPerTrack;

                        pIOCB->diskAddress.sector = (u8)(nEndingPage % kPilotSectorsPerTrack);
                        pIOCB->diskAddress.head = (u8)(nTotalTracks % kPilotHeadsPerCylinder);
                        pIOCB->diskAddress.cylinder = MESA_TO_C_UNSPECIFIED((u16)(nTotalTracks / kPilotHeadsPerCylinder));
            #endif
            
            pageCount--;
            pIOCB->pageCount = C_TO_MESA_UNSPECIFIED(pageCount);

            dataVirtualAddress += 256;
            if (incrementDataPtr)
            {
                pIOCB->dataVirtualAddress = C_TO_MESA_UNSPECIFIED(dataVirtualAddress);
            }
        }

        // io completed successfully, partially, or failed.  Raise the interrupt
        RaiseInterrupt(interruptSelector);
    }

    return true;
}

bool
AgentDisk::Poll()
{
    return false;
}

u32
AgentDisk::GetFCBSize()
{
    return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentDisk private functions
inline bool
AgentDisk::ValidateIOCB(IOCB * pIOCB)
{        
    bool bSuccess = false;
    do
    {
        if (MESA_TO_C_UNSPECIFIED(pIOCB->status) != inProgress)
        {
            // should never happen
            pIOCB->status = C_TO_MESA_UNSPECIFIED(clientError);
            break;
        }

        if (MESA_TO_C_UNSPECIFIED(pIOCB->diskIndex) != 0)
        {
            // should never happen
            pIOCB->status = C_TO_MESA_UNSPECIFIED(clientError);
            break;
        }

        if ( MESA_TO_C_UNSPECIFIED(pIOCB->command) == nop)
        {
            // no op, no problem
            pIOCB->status = C_TO_MESA_UNSPECIFIED(goodCompletion);
            break;
        }

        if (MESA_TO_C_UNSPECIFIED(pIOCB->pageCount) == 0)
        {
            // should never happen, but I guess it's ok
            pIOCB->status = C_TO_MESA_UNSPECIFIED(goodCompletion);
            break;
        }

        bSuccess = true;
    }
    while (false);

    return bSuccess;
}

AgentDisk::PilotDiskStatus
AgentDisk::DoPrePositionedSinglePageDiskIO(HANDLE hFile, PilotDiskCommand command, u32 dataVirtualAddress)
{
    // ok, we're going to do a read, write, or verify; get the parameters
    u8 * pData = (u8 *)MesaProcessor::GetRealAddress(dataVirtualAddress, (command == read ? true : false));
    if (0 == pData)
    {
        TRACE("AGENTDISK...MEMORYFAULT..  dataVirtualAddress = %d\n", (int)dataVirtualAddress);
        return memoryFault;
    }

    DWORD numBytesIO = 0;
    switch (command)
    {
        case read:
            //TRACE("READ...");
            if ((0 == ::ReadFile(hFile, pData, 512, &numBytesIO, NULL)) || (512 != numBytesIO))
            {
                TRACE("READ FAILED!\n");
                return otherError;
            }
            //TRACE("SUCCEEDED!\n");
            return goodCompletion;
            break;
        case write:
            //TRACE("WRITE...");
            if ((0 == ::WriteFile(hFile, pData, 512, &numBytesIO, NULL)) || (512 != numBytesIO))
            {
                TRACE("WRITE FAILED!\n");
                return writeFault;
            }
            //TRACE("SUCCEEDED!\n");
            return goodCompletion;
            break;
        case verify:
            {
                //TRACE("VERIFY...");
                char buff[512];
                if ((0 == ::ReadFile(hFile, buff, sizeof(buff), &numBytesIO, NULL)) || (sizeof(buff) != numBytesIO))
                {
                    //TRACE("VERIFY READ FAILED!\n");
                    return dataVerifyError;
                }
                if (0 != memcmp(buff, pData, sizeof(buff)))
                {
                    //TRACE("VERIFY COMPARE FAILED!\n");
                    return dataVerifyError;
                }
                //TRACE("VERIFY SUCCEEDED!\n");
                return goodCompletion;
            }
            break;
        default:
            return otherError;
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
