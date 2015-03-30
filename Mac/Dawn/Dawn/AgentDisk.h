// AgentDisk.h : interface of class AgentDisk
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTDISK_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTDISK_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentDisk
class AgentDisk : public MesaAgent::Agent
{
public:
    AgentDisk(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentDisk();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();

public:
    enum
    {
        kMaxNumEmulatedDisks    = 1,
        kPilotHeadsPerCylinder  = 2,
        kPilotSectorsPerTrack   = 16,
        kAnyPilotDisk           = 64,
        kDosSectorSize          = 512,
        kIONotFinished          = 0,
        kIOFinished             = 1,
        kSectorsPerBuffer       = 32,
    };
    enum PilotDiskCommands
    {
        nop                     = 0,
        read,
        write,
        verify
    };
    enum PilotDiskStati
    {
        inProgress              = 0,
        goodCompletion,
        notReady,
        recalibrateError,
        seekTimeout,
        headerCRCError,
        dataCRCError,
        headerNotFound,
        dataVerifyError,
        overrunError,
        writeFault,
        memoryError,
        memoryFault,
        clientError,
        operationReset,
        otherError
    };
           

#pragma pack (1)

    struct DCB
    {
        u16 deviceType;
        u16 numberOfCylinders;
        u16 numberOfHeads;
        u16 pagesPerTrack;
        u8  agentData[12];
    };

    struct FCB
    {
        u32     virtualAddressIOCB;
        u16     interruptSelector;
        u16		stopAgent;
        u16		agentStopped;
        u16		numberOfDCBs;
        DCB     dcbs[kMaxNumEmulatedDisks];
    };

    struct PilotDiskOperation
    {
        u16     pilotData[13];
    };

    struct PilotDiskAddress
    {
        u16     cylinder;
        u8      sector;
        u8      head;
    };

    struct AgentOperationData
    {
        u32     ioFinished;
        u32     pagesThisIO;
        void *  pNBOperationBlock;
        u32     spareA;
        u32     spareB; 
    };

    typedef u16 PilotDiskCommand;
    typedef u16 PilotDiskStatus;

    struct IOCB
    {
        PilotDiskOperation  operation;
        u16                 diskIndex;
        PilotDiskAddress    diskAddress;
        u32                 dataVirtualAddress;
        u16                 incrementDataPtr;
        PilotDiskCommand    command;
        u16                 pageCount;
        PilotDiskStatus     status;
        u32                 virtualAddressNextIOCB;
        AgentOperationData  aod;
    };

#pragma pack ()

private:
    inline u32 PilotDiskAddressToPage(const PilotDiskAddress & diskAddress);
    inline bool ValidateIOCB(IOCB * pIOCB);
private:
    PilotDiskStatus DoPrePositionedSinglePageDiskIO(HANDLE hFile, PilotDiskCommand command, u32 dataVirtualAddress);
};

inline u32
AgentDisk::PilotDiskAddressToPage(const PilotDiskAddress & diskAddress)
{
    return (((u32)MESA_TO_C_UNSPECIFIED(diskAddress.cylinder)) * kPilotHeadsPerCylinder + diskAddress.head) * kPilotSectorsPerTrack + diskAddress.sector;
}

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTDISK_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
