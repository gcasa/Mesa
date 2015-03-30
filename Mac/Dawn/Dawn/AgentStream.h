// AgentStream.h : interface of class AgentStream
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTSTREAM_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTSTREAM_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentStream
class AgentStream : public MesaAgent::Agent
{
public:
    AgentStream(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentStream();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:

    enum Command
    {
        kIdle       = 0,
        kAccept     = 1,
        kConnect    = 2,
        kDelete     = 3,
        kRead       = 4,
        kWrite      = 5,
    };

    enum Result
    {
        kCompleted  = 0,
        kInProgress = 1,
        kError      = 2,
    };

#pragma pack (1)

    struct BufferBlock
    {
        u32 m_nSubSequenceType;
        u32 m_nEndFlags;
        u32 m_nBytesWritten;
        u32 m_nBytesRead;
        u32 m_hTask;
        u16 m_nInterruptMesa;
        u32 m_nVirtualAddressBuffer;
        u32 m_nBufferSize;
        u16 m_nWriteLockedByMesa;
        u16 m_nPadding;
    };

    struct Stream
    {
        u32         m_nServerID;
        u32         m_nConnectionStateMesa;
        u32         m_nConnectionStatePC;
        BufferBlock bufferBlockToPC;
        BufferBlock bufferBlockToMesa;
        u32         m_nVirtualAddressNextStream;
        u16         m_nMesaIsServer;
        u16         m_nPadding;
    };

    struct FCB
    {
	    u32 m_nVirtualAddressFirstStream;
	    u32 m_nVirtualAddressStreamToManipulate;
	    u32 m_nCommand;
	    u32 m_nResult;
	    u16 m_nInterruptSelector;
	    u16 m_nStopAgent;
	    u16 m_nAgentStopped;
        u16 m_nStreamWordSize;
    };

#pragma pack ()

};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTSTREAM_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
