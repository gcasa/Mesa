// AgentSerial.h : interface of class AgentSerial
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTSERIAL_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTSERIAL_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentSerial
class AgentSerial : public MesaAgent::Agent
{
public:
    AgentSerial(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentSerial();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:

    enum WhatCompleted
    {
        kSend                   = 0,
        kGet                    = 1,
        kBoth                   = 2,
        kDont                   = 3,
    };

    enum SerialCommand          // SerialIOFaceGuam.Command
    {
        kNop                    = 0,
        kAbortReceive           = 1,
        kAbortTransmit          = 2,
        kBreakOff               = 3,
        kBreakOn                = 4,
        kGetDeviceStatus        = 5,
        kOff                    = 6,
        kOn                     = 7,
        kReceive                = 8,
        kSetControlBits         = 9,
        kSetParameters          = 10,
        kTransmit               = 11,
    };

    enum XFerStatus             // NewRS232Face.mesa
    {
        kXFerInProgress         = 0,
        kXFerSuccess            = 1,
        kXFerAborted            = 2,
        kXFerAsyncFramingError  = 3,
        kXFerChecksumErrpr      = 4,
        kXFerDataLost           = 5,
        kXFerDeviceError        = 6,
        kXFerDisaster           = 7,
        kXFerFrameTimeout       = 8,
        kXFerInvalidCharacter   = 9,
        kXFerInvalidFrame       = 10,
        kXFerParityError        = 11,
    };

    enum ParameterStatus
    {
        kParamInProgress        = 0,
        kParamCompleted         = 1,
        kParamUnimplemented     = 2,
        kParamRejected          = 3,
    };

#pragma pack (1)

    struct PilotSerialOperation
    {
        u32 m_nVirtualAddressDataBuffer;
        u16 m_nBufferByteLength;
    };

    struct SerialIOCB
    {
        PilotSerialOperation m_operation;
        u16 m_dequeueIOCBType;
        u16 m_nActualLength;
        u16 m_nXFerStatus;
        u32 m_nVirtualAddressNextIOCB;
    };

    struct FlowControl
    {
        u16 m_nUseXonXoff;
        u16 m_nXonChar;
        u16 m_nXoffChar;
    };

    struct SerialParameter
    {
        u16 m_nRS232Chars;
        u8  m_charSync;
        u8  m_nSyncCount;
        u8  m_correspondent;
        u8  m_typeEcho;
        u16 m_nFrameTimeout;
        FlowControl m_flowControl;
    };

    struct FCB
    {
	    u32 m_nVirtualAddressTxIOCB;
	    u32 m_nVirtualAddressRxIOCB;
        u16 m_nStopAgent;
        u16 m_nAgentStopped;
        u16 m_nInterruptSelector;
        u16 m_nCommand;
        SerialParameter m_parameterBlock;
        u16 m_controlFlags;
        u16 m_deviceStatus;
        u16 m_nCommandStatus;
        u16 m_nAgentBlockSize;
    };

#pragma pack ()

};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTSERIAL_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
