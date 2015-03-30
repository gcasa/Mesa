// AgentDisplay.h : interface of class AgentDisplay
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTDISPLAY_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTDISPLAY_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentDisplay
class AgentDisplay : public MesaAgent::Agent
{
public:
    AgentDisplay(MesaAgent::AgentIndex agentIndex, void * pFCB, u16 * pDisplayMem);
    virtual ~AgentDisplay();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
public:
    static u32 GetDisplayMemoryPageRequirement();
private:
    void SetCLTEntry();
    void GetCLTEntry();
    void SetInverseVideo();
    void SetCursorPattern();
    void UpdateRect();
    void MoveRect();
    void PatternFillRect();
private:

    enum Command
    {
        kNop                = 0,
        kSetCLTEntry        = 1,
        kGetCLTEntry        = 2,
        kSetInverseVideo    = 3,
        kSetCursorPattern   = 4,
        kUpdateRect         = 5,
        kMoveRect           = 6,
        kPatternFillRect    = 7,
    };

    enum Status
    {
        kSuccess            = 0,
        kFailure            = 1,
        kInvalidCLTIndex    = 2,
        kReadOnlyCLT        = 3,
        kInvalidDestRect    = 4,
        kInvalidSourceRect  = 5,
    };

    enum DisplayType
    {
        kMonochrome         = 0,
        k16Color            = 1,
        k256Color           = 2,
    };

    enum PatternFillMode
    {
        kCopy               = 0,
        kAnd                = 1,
        kOr                 = 2,
        kXor                = 3,
    };

#pragma pack (1)

    struct MesaRect
    {
        s16         x;
        s16         y;
        s16         width;
        s16         height;
    };

    struct FCB
    {
	    u16         m_nCommand;
        u16         m_nStatus;
        u32         m_n32mpBitmap;
        u32         m_nlteColor;
        u16         m_nPatternZeroColorIndex;
        u16         m_nPatternOneColorIndex;
        MesaRect    m_mesaRectDest;
        u16         m_nSourceX;
        u16         m_nSourceY;
        u16         m_nCursorBits[16];
        u16         m_nPatternBits[4];
        u16         m_nPatternFillMode;
        u16         m_nInverseVideo;
        u16         m_nColorIndex;
        u16         m_nDisplayType;
        u16         m_nDisplayWidth;
        u16         m_nDisplayHeight;
    };

#pragma pack ()

private:
    // windows bitmap stuff
    typedef struct _MONOBITMAPINFO
    {
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD bmiColors[2];
    } MONOBITMAPINFO;

    MONOBITMAPINFO m_bitmapInfo;
    RGBQUAD m_colorBlack;
    RGBQUAD m_colorWhite;
    u8 * m_pDisplayMemory;
    u32 m_nDisplayBytesPerLine;

    HCURSOR m_hCursor;
    HCURSOR m_hCursorOrig;

    CBitmap m_bitmapMP;

public:
    bool BlitDisplayRectToWindow(HDC hDC, const CRect * pRect);
    void SetCursorMPCode(u16 nMPCode);
};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTDISPLAY_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
//  18-Oct-03   dwoodward   added SetCursorMPCode()
