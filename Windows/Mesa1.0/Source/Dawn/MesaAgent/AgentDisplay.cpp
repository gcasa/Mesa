// AgentDisplay.cpp : implementation of class AgentDisplay
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentDisplay.h"
#include "../MesaProcessor/MesaProcessor.h"
#include "../AppFramework/DawnApp.h"
#include "../AppFramework/DawnView.h"
#include "../Resource/Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentDisplay
AgentDisplay::AgentDisplay(MesaAgent::AgentIndex agentIndex, void * pFCB, u16 * pDisplayMem) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pDisplayFCB = (FCB *)pFCB;
    memset(pDisplayFCB, 0, sizeof(FCB));

    DawnView * pView = DawnApp::GetDawnView();
    CRect viewRect;
    pView->GetClientRect(&viewRect);

    // set up FCB
    pDisplayFCB->m_nDisplayType =   C_TO_MESA_UNSPECIFIED(kMonochrome);
    pDisplayFCB->m_nDisplayWidth =  C_TO_MESA_UNSPECIFIED(viewRect.Width());
    pDisplayFCB->m_nDisplayHeight = C_TO_MESA_UNSPECIFIED(viewRect.Height());
    pDisplayFCB->m_nInverseVideo =  C_TO_MESA_UNSPECIFIED(0);
    pDisplayFCB->m_n32mpBitmap =    C_TO_MESA_LONG_UNSPECIFIED((u32)pDisplayMem);

    // make sure the width is a multiple of eight
    ASSERT(0 == (viewRect.Width() % 8));
    
    // set up windows bitmap stuff
    //RGBQUAD black = { 96, 96, 96, 0 };
    //RGBQUAD white = { 192, 192, 192, 0 };
    RGBQUAD black = { 255, 0, 0, 0 };
    RGBQUAD white = { 255, 255, 255, 0 };

    m_colorBlack = black;
    m_colorWhite = white;
    m_pDisplayMemory = (u8 *)pDisplayMem;
    m_nDisplayBytesPerLine = viewRect.Width() / 8;

    m_bitmapInfo.bmiHeader.biSize = sizeof(m_bitmapInfo.bmiHeader); 
    m_bitmapInfo.bmiHeader.biWidth = viewRect.Width();
    m_bitmapInfo.bmiHeader.biHeight = -viewRect.Height(); 
    m_bitmapInfo.bmiHeader.biPlanes = 1; 
    m_bitmapInfo.bmiHeader.biBitCount = 1; 
    m_bitmapInfo.bmiHeader.biCompression = BI_RGB; 
    m_bitmapInfo.bmiHeader.biSizeImage = 0; 
    m_bitmapInfo.bmiHeader.biXPelsPerMeter = 0; 
    m_bitmapInfo.bmiHeader.biYPelsPerMeter = 0; 
    m_bitmapInfo.bmiHeader.biClrUsed = 0; 
    m_bitmapInfo.bmiHeader.biClrImportant = 0;
    m_bitmapInfo.bmiColors[0] = m_colorWhite;
    m_bitmapInfo.bmiColors[1] = m_colorBlack;

    // put a nice pattern into our bits
    unsigned char * pBits;
    for (int row = 0; row < viewRect.Height(); row++)
    {
        pBits = m_pDisplayMemory + row * m_nDisplayBytesPerLine;
        unsigned char byte = (row % 2) ? 0xAA : ~0xAA;
        //if (row < 20) byte = 0xFF; else byte = (row % 2) ? 0xFF : 0x00;
        //byte = 0x00;
        for (unsigned int col = 0; col < m_nDisplayBytesPerLine; col++) *pBits++ = byte;
    }

    // load our mp code bitmap for the cursor
    VERIFY(m_bitmapMP.LoadBitmap(IDB_BITMAP_MPCODE));

    m_hCursor = NULL;
    m_hCursorOrig = (HCURSOR)::GetClassLong(pView->GetSafeHwnd(), GCL_HCURSOR);
}

AgentDisplay::~AgentDisplay()
{
    DawnView * pView = DawnApp::GetDawnView();
    if (pView && m_hCursor && m_hCursorOrig)
    {
        ::SetClassLong(pView->GetSafeHwnd(), GCL_HCURSOR, (LONG)m_hCursorOrig);
        ::SetCursor(m_hCursorOrig);
        ::DestroyCursor(m_hCursor);
    }
}

/////////////////////////////////////////////////////////////////////////////
// class AgentDisplay public static functions
u32
AgentDisplay::GetDisplayMemoryPageRequirement()
{
    DawnView * pView = DawnApp::GetDawnView();
    CRect viewRect;
    pView->GetClientRect(&viewRect);

    u32 numPages = ((viewRect.Width() + 7) / 8) * viewRect.Height();  // start out as num bytes
    numPages += 511; // normalize up to the next whole page
    numPages /= 512; // convert to pages

    numPages++;     // add a page just for good measure 
    return numPages;
}

/////////////////////////////////////////////////////////////////////////////
// class AgentDisplay public virtual functions
bool
AgentDisplay::Initiate()
{
    FCB * pFCB = (FCB *)m_pFCB;

    switch (MESA_TO_C_UNSPECIFIED(pFCB->m_nCommand))
    {
        case kNop:
            pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
            break;
        case kSetCLTEntry:
            SetCLTEntry();
            break;
        case kGetCLTEntry:
            GetCLTEntry();
            break;
        case kSetInverseVideo:
            SetInverseVideo();
            break;
        case kSetCursorPattern:
            SetCursorPattern();
            break;
        case kUpdateRect:
            UpdateRect();
            break;
        case kMoveRect:
            MoveRect();
            break;
        case kPatternFillRect:
            PatternFillRect();
            break;
        default:
            pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kFailure);
            break;
    }

    return true;
}

bool
AgentDisplay::Poll()
{
    return false;
}

u32
AgentDisplay::GetFCBSize()
{
    return sizeof(FCB);
}

void
AgentDisplay::SetCLTEntry()
{
    FCB * pFCB = (FCB *)m_pFCB;
    pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kReadOnlyCLT);
}

void
AgentDisplay::GetCLTEntry()
{
    FCB * pFCB = (FCB *)m_pFCB;
    u16 nColorIndex = MESA_TO_C_UNSPECIFIED(pFCB->m_nColorIndex);
    if (nColorIndex > 1) pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kReadOnlyCLT);
    else
    {
        pFCB->m_nlteColor = C_TO_MESA_LONG_UNSPECIFIED(RGB(m_bitmapInfo.bmiColors[nColorIndex].rgbRed, m_bitmapInfo.bmiColors[nColorIndex].rgbGreen, m_bitmapInfo.bmiColors[nColorIndex].rgbBlue));
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
    }
}

void
AgentDisplay::SetInverseVideo()
{
    FCB * pFCB = (FCB *)m_pFCB;
    DawnView * pView = DawnApp::GetDawnView();

    if (MESA_TO_C_UNSPECIFIED(pFCB->m_nInverseVideo))
    {
        m_bitmapInfo.bmiColors[0] = m_colorBlack;
        m_bitmapInfo.bmiColors[1] = m_colorWhite;
    }
    else
    {
        m_bitmapInfo.bmiColors[1] = m_colorBlack;
        m_bitmapInfo.bmiColors[0] = m_colorWhite;
    }

    SetCursorPattern();
    pView->InvalidateRect(NULL);
    pView->UpdateWindow();
    pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
}

void
AgentDisplay::SetCursorMPCode(u16 nMPCode)
{
    u16 n1 = nMPCode / 1000;
    u16 n2 = nMPCode % 1000 / 100;
    u16 n3 = nMPCode % 100 / 10;
    u16 n4 = nMPCode % 10;

    FCB * pFCB = (FCB *)m_pFCB;
    bool bInverseVideo = (MESA_TO_C_UNSPECIFIED(pFCB->m_nInverseVideo) ? true : false);
    DawnView * pView = DawnApp::GetDawnView();
    if (NULL == pView) return;

    // do a bunch of Windows hocus
    u16 nCursorWidth = ::GetSystemMetrics(SM_CXCURSOR);
    u16 nCursorHeight = ::GetSystemMetrics(SM_CYCURSOR);
    u16 nCursorBytes = (nCursorWidth * nCursorHeight) >> 3;
    char bitsXOR[256];
    char bitsAND[256];
    CWindowDC dc(NULL);
    CDC dc1;
    CDC dc2;
    CBitmap bitmapAND;
    CBitmap bitmapXOR;
    CBitmap bitmapMP;
    CBitmap * pBitmapOld1 = NULL;
    CBitmap * pBitmapOld2 = NULL;
    HCURSOR hCursor = NULL;
    u16 nMPCursorWidth = 7 * 2 + 2;
    u16 nMPCursorHeight = 10 * 2;

    do
    {
        if (0 == dc1.CreateCompatibleDC(&dc)) break;
        if (0 == dc2.CreateCompatibleDC(&dc)) break;

        if (0 == bitmapAND.CreateCompatibleBitmap(&dc2, nCursorWidth, nCursorHeight)) break;
        if (0 == bitmapXOR.CreateCompatibleBitmap(&dc2, nCursorWidth, nCursorHeight)) break;
        if (0 == bitmapMP.CreateCompatibleBitmap(&dc1, nMPCursorWidth, nMPCursorHeight)) break;

        // select the mp resource bitmap into dc2
        if (NULL == (pBitmapOld2 = dc2.SelectObject(&m_bitmapMP))) break;

        // put the mp code cursor pattern in dc1
        if (NULL == (pBitmapOld1 = dc1.SelectObject(&bitmapMP))) break;
        // first fill with black, then blit the characters (inversed)
        dc1.FillSolidRect(0, 0, nMPCursorWidth, nMPCursorHeight, RGB(0, 0, 0));
        dc1.BitBlt(0,  0, 7, 10, &dc2, 7 * n1, 0, NOTSRCCOPY);
        dc1.BitBlt(7,  0, 7, 10, &dc2, 7 * n2, 0, NOTSRCCOPY);
        dc1.BitBlt(2, 10, 7, 10, &dc2, 7 * n3, 0, NOTSRCCOPY);
        dc1.BitBlt(9, 10, 7, 10, &dc2, 7 * n4, 0, NOTSRCCOPY);
        
        // clear the resource bitmap out of dc2
        dc2.SelectObject(pBitmapOld2); pBitmapOld2 = NULL;

        // Build the XOR cursor plane
        memset(bitsXOR, 0x00, nCursorBytes);
        if (bInverseVideo)
        {
            if (0 == bitmapXOR.SetBitmapBits(nCursorBytes, bitsXOR)) break;
            if (NULL == (pBitmapOld2 = dc2.SelectObject(&bitmapXOR))) break;
            if (0 == dc2.BitBlt(0, 0, nCursorWidth, nCursorHeight, &dc1, 0, 0, SRCCOPY)) break;
            dc2.SelectObject(pBitmapOld2); pBitmapOld2 = NULL;
            if (0 == bitmapXOR.GetBitmapBits(nCursorBytes, bitsXOR)) break;
        }

        // Build the AND cursor plane
        memset(bitsAND, 0xFF, nCursorBytes);
        if (0 == bitmapAND.SetBitmapBits(nCursorBytes, bitsAND)) break;
        if (NULL == (pBitmapOld2 = dc2.SelectObject(&bitmapAND))) break;
        if (0 == dc2.BitBlt(0, 0, nCursorWidth, nCursorHeight, &dc1, 0, 0, NOTSRCCOPY)) break;
        dc2.SelectObject(pBitmapOld2); pBitmapOld2 = NULL;
        if (0 == bitmapAND.GetBitmapBits(nCursorBytes, bitsAND)) break;

        // finish building the planes, clear the bitmap from dc1 and create the cursor
        dc1.SelectObject(pBitmapOld1); pBitmapOld1 = NULL;
        hCursor = ::CreateCursor(AfxGetInstanceHandle(), 0, 0, nCursorWidth, nCursorHeight, bitsAND, bitsXOR);
    }
    while (false);

    if (hCursor)
    {
        CRect clientRect;
        CPoint cursorPoint;

        pView->GetClientRect(&clientRect);
        ::GetCursorPos(&cursorPoint);

        ::SetClassLong(pView->GetSafeHwnd(), GCL_HCURSOR, (LONG)hCursor);
        if (pView == pView->WindowFromPoint(cursorPoint)) ::SetCursor(hCursor);
        if (m_hCursor) ::DestroyCursor(m_hCursor);
        m_hCursor = hCursor;
    }
    else
    {
        if (NULL != pBitmapOld1) dc1.SelectObject(pBitmapOld1);
        if (NULL != pBitmapOld2) dc2.SelectObject(pBitmapOld2);
    }
}

void
AgentDisplay::SetCursorPattern()
{
    FCB * pFCB = (FCB *)m_pFCB;
    const u32 nPatternSize = sizeof(pFCB->m_nCursorBits);
    u8 pattern[nPatternSize];
    u8 * pCursorBits = (u8 *)(&pFCB->m_nCursorBits);
    bool bInverseVideo = (MESA_TO_C_UNSPECIFIED(pFCB->m_nInverseVideo) ? true : false);
    DawnView * pView = DawnApp::GetDawnView();
    if (NULL == pView) return;

    // byte swap the pattern bits
    for (u32 i = 0; i < nPatternSize; i += 2, pCursorBits += 2)
    {
        pattern[i] = *(pCursorBits + 1);
        pattern[i+1] = *pCursorBits;
    }
    
    // now do a bunch of Windows hocus
    u16 nCursorWidth = ::GetSystemMetrics(SM_CXCURSOR);
    u16 nCursorHeight = ::GetSystemMetrics(SM_CYCURSOR);
    u16 nCursorBytes = (nCursorWidth * nCursorHeight) >> 3;
    char bitsXOR[256];
    char bitsAND[256];
    CWindowDC dc(NULL);
    CDC dc1;
    CDC dc2;
    CBitmap bitmapAND;
    CBitmap bitmapXOR;
    CBitmap bitmap16X16;
    CBitmap * pBitmapOld1 = NULL;
    CBitmap * pBitmapOld2 = NULL;
    HCURSOR hCursor = NULL;

    do
    {
        if (0 == dc1.CreateCompatibleDC(&dc)) break;
        if (0 == dc2.CreateCompatibleDC(&dc)) break;

        if (0 == bitmapAND.CreateCompatibleBitmap(&dc2, nCursorWidth, nCursorHeight)) break;
        if (0 == bitmapXOR.CreateCompatibleBitmap(&dc2, nCursorWidth, nCursorHeight)) break;
        if (0 == bitmap16X16.CreateCompatibleBitmap(&dc1, 16, 16)) break;

        // put the cursor pattern in dc1
        if (0 == bitmap16X16.SetBitmapBits(nPatternSize, pattern)) break;
        if (NULL == (pBitmapOld1 = dc1.SelectObject(&bitmap16X16))) break;
        
        // Build the XOR cursor plane
        memset(bitsXOR, 0x00, nCursorBytes);
        if (bInverseVideo)
        {
            if (0 == bitmapXOR.SetBitmapBits(nCursorBytes, bitsXOR)) break;
            if (NULL == (pBitmapOld2 = dc2.SelectObject(&bitmapXOR))) break;
            if (0 == dc2.BitBlt(0, 0, nCursorWidth, nCursorHeight, &dc1, 0, 0, SRCCOPY)) break;
            dc2.SelectObject(pBitmapOld2); pBitmapOld2 = NULL;
            if (0 == bitmapXOR.GetBitmapBits(nCursorBytes, bitsXOR)) break;
        }

        // Build the AND cursor plane
        memset(bitsAND, 0xFF, nCursorBytes);
        if (0 == bitmapAND.SetBitmapBits(nCursorBytes, bitsAND)) break;
        if (NULL == (pBitmapOld2 = dc2.SelectObject(&bitmapAND))) break;
        if (0 == dc2.BitBlt(0, 0, nCursorWidth, nCursorHeight, &dc1, 0, 0, NOTSRCCOPY)) break;
        dc2.SelectObject(pBitmapOld2); pBitmapOld2 = NULL;
        if (0 == bitmapAND.GetBitmapBits(nCursorBytes, bitsAND)) break;

        // finish building the planes, clear the bitmap from dc1 and create the cursor
        dc1.SelectObject(pBitmapOld1); pBitmapOld1 = NULL;
        hCursor = ::CreateCursor(AfxGetInstanceHandle(), 0, 0, nCursorWidth, nCursorHeight, bitsAND, bitsXOR);
    }
    while (false);

    if (hCursor)
    {
        CRect clientRect;
        CPoint cursorPoint;

        pView->GetClientRect(&clientRect);
        ::GetCursorPos(&cursorPoint);

        ::SetClassLong(pView->GetSafeHwnd(), GCL_HCURSOR, (LONG)hCursor);
        if (pView == pView->WindowFromPoint(cursorPoint)) ::SetCursor(hCursor);
        if (m_hCursor) ::DestroyCursor(m_hCursor);
        m_hCursor = hCursor;
    }
    else
    {
        if (NULL != pBitmapOld1) dc1.SelectObject(pBitmapOld1);
        if (NULL != pBitmapOld2) dc2.SelectObject(pBitmapOld2);
    }
        
    pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
}

void
AgentDisplay::UpdateRect()
{
    FCB * pFCB = (FCB *)m_pFCB;
    DawnView * pView = DawnApp::GetDawnView();
    s16 x = MESA_TO_C_UNSPECIFIED(pFCB->m_mesaRectDest.x);
    s16 y = MESA_TO_C_UNSPECIFIED(pFCB->m_mesaRectDest.y);
    s16 width = MESA_TO_C_UNSPECIFIED(pFCB->m_mesaRectDest.width);
    s16 height = MESA_TO_C_UNSPECIFIED(pFCB->m_mesaRectDest.height);
    CRect rect;
    pView->GetClientRect(&rect);

    if ((x < 0) || (y < 0) || (width <= 0) || (height <= 0) || ((x + width) > rect.right) || ((y + height) > rect.bottom))
    {
        pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kInvalidDestRect);
    }
    else if (! pView->IsIconic())
    {
        pView->InvalidateRect(CRect(x, y, x+width, y+height));
        pView->UpdateWindow();
    }

    pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
}

void
AgentDisplay::MoveRect()
{
    // move has already been performed in the mesa bitmap buffer
    UpdateRect();
}

void
AgentDisplay::PatternFillRect()
{
    // pattern fill has already been performed in the mesa bitmap buffer
    UpdateRect();
}

/////////////////////////////////////////////////////////////////////////////
// class AgentDisplay public functions
bool
AgentDisplay::BlitDisplayRectToWindow(HDC hDC, const CRect * pRect)
{
    if (m_pDisplayMemory)
    {
        ::SetDIBitsToDevice(
            hDC,                                                    // HDC
            pRect->left,                                            // xDEST
            pRect->top,                                             // yDEST
            pRect->Width(),                                         // width
            pRect->Height(),                                        // height
            pRect->left,                                            // xSRC
            0,                                                      // ySRC
            0,                                                      // starting scan line
            pRect->Height(),                                        // num scan lines
            m_pDisplayMemory + m_nDisplayBytesPerLine * pRect->top, // bits
            (BITMAPINFO *)&m_bitmapInfo,                            // bitmap info
            DIB_RGB_COLORS);                                        // color usage
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
//  18-Oct-03   dwoodward   added SetCursorMPCode()
