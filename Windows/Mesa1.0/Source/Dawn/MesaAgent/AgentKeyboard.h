// AgentKeyboard.h : interface of class AgentKeyboard
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTKEYBOARD_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTKEYBOARD_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentKeyboard
class AgentKeyboard : public MesaAgent::Agent
{
public:
    AgentKeyboard(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentKeyboard();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:

#pragma pack (1)

    struct FCB
    {
	    u16 m_keyState[7];
    };

#pragma pack ()

public: // public functions
    void SetMouseButtonState(int nVKButton, bool bDown, bool bOtherMouseButtonsDown);
    void SetKeyState(u32 nVKKey, u32 nFlags, bool bDown);
    void SetToggleKeyState();

public: // implementation
    class XKeyMapEntry
    {
    public:
        u8 m_nVK;
        u8 m_xKeyStandard;
        u8 m_xKeyExtended;
        bool m_bToggleKey;
        bool m_bUseOEM;
    };

    class XKeyStroke
    {
    public:
        XKeyStroke() { m_data = 0; }
    public:
        inline u8   GetKeyCode()                { return (u8)(m_data & 0xFF); }
        inline bool IsKeyToggle()               { return (m_data & (1 << 8)) ? true : false; }
        inline bool IsKeyDown()                 { return (m_data & (1 << 9)) ? true : false; }
        inline void SetKeyCode(u8 nKeyCode)     { m_data = (nKeyCode | (m_data & 0xFF00)); }
        inline void SetKeyToggle(bool bToggle)  { if (bToggle) m_data |= (1 << 8); else m_data &= (~(1 << 8)); }
        inline void SetKeyDown(bool bDown)      { if (bDown) m_data |= (1 << 9); else m_data &= (~(1 << 9));  }
    private:
        u16 m_data;
    };

    class XKeyMap; friend class XKeyMap; class XKeyMap
    {
    public:
        void InitializeMap();
        XKeyStroke Lookup(u32 nVKKey, u32 nFlags);
    private:
        enum { kMaxVK = 0xFF };
    private:
        XKeyMapEntry m_map[kMaxVK + 1];
        XKeyMapEntry m_oemMap[kMaxVK + 1];
    };

    class KeystrokeQueue; friend class KeystrokeQueue; class KeystrokeQueue
    {
    public:
        KeystrokeQueue() { Clear(); }
        inline void SetKeyboardAgent(AgentKeyboard * pAgent) { m_pAgent = pAgent; }
        inline void Clear() { m_nFirstKeystroke = m_nNumKeystrokes = 0; m_nLastKeystroke = 255; }
        void Add(XKeyStroke keystroke);
        void Flush();
        void FlushInstructionQueue();
        bool SawKeystroke(u32 nKeystroke);
    public:
        enum { kQueueSize = 256 };
    private:
        AgentKeyboard * m_pAgent;
        XKeyStroke m_keystrokes[kQueueSize];
        u32 m_nFirstKeystroke;
        u32 m_nLastKeystroke;
        u32 m_nNumKeystrokes;
    };

    void OnTimer();
private:
    void SetXeroxKeyStroke(XKeyStroke keystroke);

private:
    void * m_pTimer;
    KeystrokeQueue m_queue;

private:
    static XKeyMap s_xKeyMap;
    static XKeyMapEntry s_xKeyMapInitializer[];
    static XKeyMapEntry s_xKeyMapInitializerOEM[];
};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTKEYBOARD_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
