// MesaMicrocode.cpp : implements Win32 specific Microcode for Mesa Processor
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "MesaMicrocode.h"
#include "MesaMicrocodeOps.h"
#include "../MesaAgent/MesaAgent.h"
#include <PrincOps/PrincOps_Microcode.h>

/////////////////////////////////////////////////////////////////////////////
// MesaMicrocode
// This module implements machine specific Microcode for the Win32 version
// of our Mesa processor.  Most of the instructions are quite simple.
// However, we're going to do a major number on BITBLT and the new BITBLTX.
// In the book, a virgin BITBLT and a restarted BITBLT are distinguished by
// a stack pointer of 1 (virgin BitBlt) containing a short pointer to BitBltArgs
// and a stack pointer of SIZE[BitBltArgs] indicating a restart of the instruction
// due to a pending interrupt.
// In our implementation we'll use the implementation of BitBltX to perform BitBlt's job.
// So in BITBLT, the short pointer will be converted to a private decoded BitBltX argument
// structure, whose Win32 real address we'll place at the beginning of the stack as a
// LONG_UNSPECIFIED and set savedSP to 2.  On BTBLT entry we'll use a SP value of 2
// (versus 1) to distinguish
// between an instruction restart and virgin execution.  This will have the added
// performance benefit of picking up where we left off due to a page fault, which in the book
// would cause an entire restart of the intruction using the original short pointer.
// BITBLTX will do the same thing, decoding its packed BITBLTX args into our private BitBltX argument
// structure which we'll also put at the base of the stack for instruction restarting.

/////////////////////////////////////////////////////////////////////////////
// MesaMicrocode types
class BitBltXArgs
{
public:
    // constructor for BITBLT
    BitBltXArgs(
        LONG_POINTER dstWord, CARDINAL dstBit, INTEGER dstBpl, LONG_POINTER srcWord,
        CARDINAL srcBit, INTEGER srcBpl, CARDINAL width, CARDINAL height, UNSPECIFIED flags);

    // constructor for BITBLTX
    BitBltXArgs(UNSPECIFIED flags, CARDINAL nHeight, CARDINAL nWidth, INTEGER nSourcePixelsPerLine,
        CARDINAL nSourcePixelStart, LONG_POINTER nSourcePixelWordAddress, INTEGER nDestPixelsPerLine,
        CARDINAL nDestPixelStart, LONG_POINTER nDestPixelWordAddress); 
public:
    // start or resume the BitBltX
    void BitBltX();
protected:
    enum Direction  { kForward = 0, kBackward };
    enum PixelType  { kDisplay = 0, kBit };
    enum SourceFunc { kComplement = 0, kNull };
    enum DestFunc   { kSrc = 0, kSrcIfDstLE1, kSrcIf0, kSrcIfDstNot0, kSrcIfNot0, kSrcIfDst0, kSrcPixelXor, kSrcXorDst,
                      kBBNull, kBBAnd, kBBOr, kBBXor };
protected:
    u16 m_nHeight;
    u16 m_nWidth;
    s16 m_nSourcePixelsPerLine;
    u16 m_nSourcePixelStart;
    u32 m_nSourcePixelWordAddress;
    s16 m_nDestPixelsPerLine;
    u16 m_nDestPixelStart;
    u32 m_nDestPixelWordAddress;
    Direction m_direction;
    PixelType m_sourceType;
    PixelType m_destType;
    bool m_bPattern;
    SourceFunc m_sourceFunc;
    DestFunc m_destFunc;
    bool m_bPatternPacked;
    u16 m_nPatternYOffset;
    u16 m_nPatternWidth;
    u16 m_nPatternHeight;
protected:
    bool m_bInitialized;
    u16 m_patternLines[16]; // patterns can be a max of 16 bits wide by 16 lines high
    u32 m_nCurrentLine;
    u32 m_nCurrentPixel;

protected:
    bool BoundsCheckAndInitializePattern();
    bool PatternLine();
    bool BlitLine();
    void ReadAndTransformPatternPixel(u16 & sourcePixel);
    void ReadAndTransformSourcePixel(u16 & sourcePixel);
    void ReadAndTransformDestPixel(const u16 & sourcePixel, u16 & destPixel);
    void WriteDestPixel(u16 & destPixel);
    inline void BumpBit(const u32 & baseAddress, u32 nNumBits, u32 & pixelWord, u32 & pixelOffset);
    void BumpLine();
};

/////////////////////////////////////////////////////////////////////////////
// class BitBltXArgs implementation
BitBltXArgs::BitBltXArgs(
    LONG_POINTER dstWord, CARDINAL dstBit, INTEGER dstBpl, LONG_POINTER srcWord,
    CARDINAL srcBit, INTEGER srcBpl, CARDINAL width, CARDINAL height, UNSPECIFIED flags)
{
    // constructor for BITBLT
    m_nHeight = height;
    m_nWidth = width;
    m_nSourcePixelsPerLine = srcBpl;        // OVERLAID with BitBlt.GrayParm
    m_nSourcePixelStart = srcBit;
    m_nSourcePixelWordAddress = srcWord;
    m_nDestPixelsPerLine = dstBpl;
    m_nDestPixelStart = dstBit;
    m_nDestPixelWordAddress = dstWord;

    // normalize the pixel starts and word addresses
    //while (m_nSourcePixelStart > 16) { m_nSourcePixelStart -= 16; m_nSourcePixelWordAddress++; }
    //while (m_nDestPixelStart > 16) { m_nDestPixelStart -= 16; m_nDestPixelWordAddress++; }

    // decode the flags - top 8
    // 00000000
    // direction (0 = forward, 1 = backward)
    //  disjoint (ignore this flag, always set m_sourceType to kBit)
    //   disjointItems (ignore this flag, always set m_destType to kBit)
    //    gray (0 = false, 1 = true) (same as pattern)
    //     srcFunc (0 = null, 1 = complement)
    //      dstFunc (0 = null, 1 = and, 2 = or, 3 = xor) (2 bits)
    m_direction = ((flags & (1 << 15)) ? kBackward : kForward);
    m_sourceType = kBit;
    m_destType = kBit;
    m_bPattern = ((flags & (1 << 12)) ? true : false);
    m_sourceFunc = ((flags & (1 << 11)) ? kComplement : kNull);
    switch ((flags >> 9) & 0x3)
    {
        case 0:
            m_destFunc = kBBNull;
            break;
        case 1:
            m_destFunc = kBBAnd;
            break;
        case 2:
            m_destFunc = kBBOr;
            break;
        case 3:
            m_destFunc = kBBXor;
            break;
    }

    if (m_bPattern)
    {
        // bitBltX.m_nSourcePixelsPerLine is actually a BitBlt.GrayParm
        // A pattern parm is a packed word as follows:
        // 00000000 00000000
        // Reserved (4 bits) (unpacked is always false)
        //     yOffset (4 bits [0..16))
        //          widthMinusOne (4 bits [0..16))
        //              heightMinusOne (4 bits [0..16))
        m_bPatternPacked = true;
        m_nPatternYOffset = (m_nSourcePixelsPerLine >> 8) & 0xF;
        m_nPatternWidth = ((m_nSourcePixelsPerLine >> 4) & 0xF) + 1;
        m_nPatternHeight = (m_nSourcePixelsPerLine & 0xF) + 1;
    }

    m_bInitialized = false;
}

BitBltXArgs::BitBltXArgs(
    UNSPECIFIED flags, CARDINAL nHeight, CARDINAL nWidth, INTEGER nSourcePixelsPerLine,
    CARDINAL nSourcePixelStart, LONG_POINTER nSourcePixelWordAddress, INTEGER nDestPixelsPerLine,
    CARDINAL nDestPixelStart, LONG_POINTER nDestPixelWordAddress)
{
    // constructor for BITBLTX

    m_nHeight = nHeight;
    m_nWidth = nWidth;

    m_nSourcePixelsPerLine = nSourcePixelsPerLine;  // this is a ColorBlt.SrcDesc which is an OVERLAID
                                                    // pixelsPerLine and a PatternParm which we'll set
                                                    // later if the flags so indicate
    m_nSourcePixelStart = nSourcePixelStart;
    m_nSourcePixelWordAddress = nSourcePixelWordAddress;

    m_nDestPixelsPerLine = nDestPixelsPerLine;
    m_nDestPixelStart = nDestPixelStart;
    m_nDestPixelWordAddress = nDestPixelWordAddress;

    // normalize the pixel starts and word addresses
    //while (m_nSourcePixelStart > 16) { m_nSourcePixelStart -= 16; m_nSourcePixelWordAddress++; }
    //while (m_nDestPixelStart > 16) { m_nDestPixelStart -= 16; m_nDestPixelWordAddress++; }

    // decode the flags - top 8
    // 00000000
    // direction  (0 = forward, 1 = backward)
    //  sourceType (0 = bit, 1 = display)
    //   destType (0 = bit, 1 = display)
    //    pattern (0 = false, 1 = true)
    //     sourceFunc (0 = null, 1 = complement)
    //      dstFunc (0 = src, 1 = srcIfDstLE1, 2 = srcIf0, 3 = srcIfDstNot0, 4 = srcIfNot0,
    //               5 = srcIfDst0, 6 = pixelXor, srcXorDst)
    m_direction = ((flags & (1 << 15)) ? kBackward : kForward);
    m_sourceType = ((flags & (1 << 14)) ? kDisplay : kBit);
    m_destType = ((flags & (1 << 13)) ? kDisplay : kBit);
    m_bPattern = ((flags & (1 << 12)) ? true : false);
    m_sourceFunc = ((flags & (1 << 11)) ? kComplement : kNull);
    switch ((flags >> 8) & 0x7)
    {
        case 0:
            m_destFunc = kSrc;
            break;
        case 1:
            m_destFunc = kSrcIfDstLE1;
            break;
        case 2:
            m_destFunc = kSrcIf0;
            break;
        case 3:
            m_destFunc = kSrcIfDstNot0;
            break;
        case 4:
            m_destFunc = kSrcIfNot0;
            break;
        case 5:
            m_destFunc = kSrcIfDst0;
            break;
        case 6:
            m_destFunc = kSrcPixelXor;
            break;
        case 7:
            m_destFunc = kSrcXorDst;
            break;
    }

    if (m_bPattern)
    {
        // bitBltX.m_nSourcePixelsPerLine is actually a ColorBlt.PatternParm
        // A pattern parm is a packed word as follows:
        // 00000000 00000000
        // Reserved (3 bits)
        //    unpacked (0 = false, 1 = true)
        //     yOffset (4 bits [0..16))
        //          widthMinusOne (4 bits [0..16))
        //              heightMinusOne (4 bits [0..16))
        m_bPatternPacked = (m_nSourcePixelsPerLine & (1 << 12)) ? false : true;
        m_nPatternYOffset = (m_nSourcePixelsPerLine >> 8) & 0xF;
        m_nPatternWidth = ((m_nSourcePixelsPerLine >> 4) & 0xF) + 1;
        m_nPatternHeight = (m_nSourcePixelsPerLine & 0xF) + 1;
    }

    m_bInitialized = false;
}

extern "C" void DebugPrint(const char *);
#include <stdio.h>
bool
BitBltXArgs::BoundsCheckAndInitializePattern()
{
    // we do the initialization outside of the constructor due to the possibility of
    // a page fault reading the pattern bits
    if (m_bInitialized) return true;
    
    // normalize parameters and set current line and pixel
    if (m_nDestPixelStart > 15)
    {
        m_nDestPixelWordAddress += (m_nDestPixelStart / 16);
        m_nDestPixelStart = (m_nDestPixelStart % 16);
    }
    if ((! m_bPattern) && (m_nSourcePixelStart > 15))
    {
        m_nSourcePixelWordAddress += (m_nSourcePixelStart / 16);
        m_nSourcePixelStart = (m_nSourcePixelStart % 16);
    }

#if 0
    if (! m_bPattern)
    {
        const char * pType = (m_sourceType != m_destType) ? "MIXED" : ((m_sourceType == kBit) ? "BIT" : "DISPLAY");
        char buff[256];
        sprintf(buff, "%s %s BLIT!\n", (m_nDestPixelStart == m_nSourcePixelStart) ? "ALIGNED" : "UNALIGNED", pType);
        DebugPrint(buff);
    }
#endif

    m_nCurrentLine = 0;
    if (m_direction == kBackward)
    {
        m_nSourcePixelsPerLine = - m_nSourcePixelsPerLine;
        m_nDestPixelsPerLine = - m_nDestPixelsPerLine;
        m_nCurrentPixel = m_nWidth - 1;
    }
    else m_nCurrentPixel = 0;
    
    // bounds check the parameters
    if (m_nDestPixelsPerLine < 1)
    {
        return false;
    }

    if (m_bPattern)
    {
        if (m_direction == kBackward)
        {
            return false;
        }
    }
    else
    {
        if (m_nSourcePixelsPerLine < 1)
        {
            return false;
        }
        m_bInitialized = true;
        return true;
    }
    
    // m_bPattern == true
    if ((m_nPatternWidth != 1) || (m_nPatternHeight > 16) || (m_bPatternPacked && (m_nPatternYOffset > (m_nPatternHeight-1))))
    {
        // we only support 16 bit wide patterns by 16 line high in the forward direction
        // with a max offset of the patternHeight -1 if the pattern is packed
        return false;
    }

    if (m_bPatternPacked)
    {
        // unpack the pattern into our pattern array
        u32 patternOriginAddress = m_nSourcePixelWordAddress - m_nPatternYOffset;
        for (u32 i = 0; i < m_nPatternHeight; i++)
        {
            m_patternLines[i] = MP_ReadMesaRealAddress(MP_Fetch(patternOriginAddress + i));
        }
        m_bInitialized = true;
    }
    else
    {
        // stick the 1 width, 1 height pattern word into our pattern array
        if ((m_nPatternWidth == 1) && (m_nPatternHeight == 1))
        {
            m_patternLines[0] = MP_ReadMesaRealAddress(MP_Fetch(m_nSourcePixelWordAddress));
            if (m_patternLines[0]) m_patternLines[0] = 0xFFFF;
            m_bInitialized = true;
        }
    }

    if (! m_bInitialized)
    {
        return false;
    }

    return true;
}

void
BitBltXArgs::BitBltX()
{
    // first bounds check the parms and initialize the pattern if any
    if (! BoundsCheckAndInitializePattern())
    {
        delete this;
        return;
    }

    if (m_bPattern)
    {
        while (PatternLine())
        {
            if (MP_InterruptPending())
            {
                MP_Mesa_Processor.registers.SP = MP_Mesa_Processor.state.savedSP;
                MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
                return;
            }
        }
    }
    else
    {
        while (BlitLine())
        {
            if (MP_InterruptPending())
            {
                MP_Mesa_Processor.registers.SP = MP_Mesa_Processor.state.savedSP;
                MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
                return;
            }
        }
    }

    delete this;
}

bool
BitBltXArgs::PatternLine()
{
    u16 pixelSource;
    u16 pixelDest;

    // only have to deal with the forward case
    for (; m_nCurrentPixel < m_nWidth; m_nCurrentPixel++)
    {
        ReadAndTransformPatternPixel(pixelSource);
        ReadAndTransformDestPixel(pixelSource, pixelDest);
        WriteDestPixel(pixelDest);
    }
    m_nCurrentPixel = 0;
    BumpLine();
    return (m_nCurrentLine == m_nHeight) ? false : true;
}

bool
BitBltXArgs::BlitLine()
{
    u16 pixelSource;
    u16 pixelDest;

    for (;;)
    {
        ReadAndTransformSourcePixel(pixelSource);
        ReadAndTransformDestPixel(pixelSource, pixelDest);
        WriteDestPixel(pixelDest);
        if (m_direction == kForward)
        {
            m_nCurrentPixel++;
            if (m_nCurrentPixel == m_nWidth) { m_nCurrentPixel = 0; break; }
        }
        else
        {
            if (m_nCurrentPixel == 0) { m_nCurrentPixel = m_nWidth - 1; break; }
            m_nCurrentPixel--;
        }
    }
    BumpLine();
    return (m_nCurrentLine == m_nHeight) ? false : true;
}

void
BitBltXArgs::BumpLine()
{
    u32 bumpAmount;

    if (m_direction == kForward)
    {
        // first do dest
        bumpAmount = m_nDestPixelsPerLine + m_nDestPixelStart;
        m_nDestPixelWordAddress += (bumpAmount / 16);
        m_nDestPixelStart = (u16)(bumpAmount % 16);

        if (! m_bPattern)
        {
            // and now source
            bumpAmount = m_nSourcePixelsPerLine + m_nSourcePixelStart;
            m_nSourcePixelWordAddress += (bumpAmount / 16);
            m_nSourcePixelStart = (u16)(bumpAmount % 16);
        }
    }
    else
    {
        // back is trickier; use bumpAmount as wordsBack
        u16 bitsBack;

        // do dest
        bumpAmount = m_nDestPixelsPerLine / 16;
        bitsBack = m_nDestPixelsPerLine % 16;
        if (bitsBack > m_nDestPixelStart)
        {
            bumpAmount++;
            m_nDestPixelStart = 16 + m_nDestPixelStart - bitsBack;
        }
        else m_nDestPixelStart -= bitsBack;
        m_nDestPixelWordAddress -= bumpAmount;

        // do source
        bumpAmount = m_nSourcePixelsPerLine / 16;
        bitsBack = m_nSourcePixelsPerLine % 16;
        if (bitsBack > m_nSourcePixelStart)
        {
            bumpAmount++;
            m_nSourcePixelStart = 16 + m_nSourcePixelStart - bitsBack;
        }
        else m_nSourcePixelStart -= bitsBack;
        m_nSourcePixelWordAddress -= bumpAmount;

        #if 0
        // do dest
        bumpAmount = m_nDestPixelsPerLine + m_nDestPixelStart;
        m_nDestPixelWordAddress -= (bumpAmount / 16);
        m_nDestPixelStart = (u16)(bumpAmount % 16);

        // do source
        bumpAmount = m_nSourcePixelsPerLine + m_nSourcePixelStart;
        m_nSourcePixelWordAddress -= (bumpAmount / 16);
        m_nSourcePixelStart = (u16)(bumpAmount % 16);
        #endif
    }
    m_nCurrentLine++;
}

void
BitBltXArgs::ReadAndTransformPatternPixel(u16 & sourcePixel)
{
    // m_nSourcePixelStart is the xOffset into the pattern for (m_nCurrentPixel at the start of the line)
    // m_nPatternYOffset is the yOffset into the pattern for packed patterns
    u32 patternLine = (m_bPatternPacked ? ((m_nCurrentLine + m_nPatternYOffset) % m_nPatternHeight) : 0);
    u32 patternBit = (m_nCurrentPixel + m_nSourcePixelStart) % 16;
    sourcePixel = m_patternLines[patternLine];
    if (m_sourceType == kDisplay)
    {
        sourcePixel = (sourcePixel << 8) | (sourcePixel >> 8);
    }
    sourcePixel = (sourcePixel >> (15 - patternBit)) & 1;
    if (m_sourceFunc == kComplement) sourcePixel = (sourcePixel ? 0 : 1);
}

void
BitBltXArgs::ReadAndTransformSourcePixel(u16 & sourcePixel)
{
    u32 pixelWord;
    u32 pixelOffset;

    BumpBit(m_nSourcePixelWordAddress, m_nSourcePixelStart + m_nCurrentPixel, pixelWord, pixelOffset);
    sourcePixel = MP_ReadMesaRealAddress(MP_Fetch(pixelWord));

    if (m_sourceType == kDisplay)
    {
        sourcePixel = (sourcePixel << 8) | (sourcePixel >> 8);
    }

    sourcePixel = (sourcePixel >> (15 - pixelOffset)) & 1;

    if (m_sourceFunc == kComplement) sourcePixel = (sourcePixel ? 0 : 1);
}

void
BitBltXArgs::ReadAndTransformDestPixel(const u16 & sourcePixel, u16 & destPixel)
{
    u32 pixelWord;
    u32 pixelOffset;

    BumpBit(m_nDestPixelWordAddress, m_nDestPixelStart + m_nCurrentPixel, pixelWord, pixelOffset);
    destPixel = MP_ReadMesaRealAddress(MP_Fetch(pixelWord));

    if (m_destType == kDisplay)
    {
        destPixel = (destPixel << 8) | (destPixel >> 8);
    }
    destPixel = (destPixel >> (15 - pixelOffset)) & 1;

    switch (m_destFunc)
    {
        case kSrc:
            destPixel = sourcePixel;
            break;
        case kSrcIfDstLE1:
            destPixel &= sourcePixel;
            break;
        case kSrcIf0:
            if (0 == sourcePixel) destPixel = 0;
            break;
        case kSrcIfDstNot0:
            if (destPixel) destPixel = sourcePixel;
            break;
        case kSrcIfNot0:
            if (sourcePixel) destPixel = 1;
            break;
        case kSrcIfDst0:
            if (0 == destPixel) destPixel = sourcePixel;
            break;
        case kSrcPixelXor:
            if (0 == destPixel) destPixel = sourcePixel;
            else if (sourcePixel) destPixel = 0;
            break;
        case kSrcXorDst:
            destPixel = ((destPixel ^ sourcePixel)) & 1;
            break;
        case kBBNull:
            destPixel = sourcePixel;
            break;
        case kBBAnd:
            destPixel &= sourcePixel;
            break;
        case kBBOr:
            destPixel |= sourcePixel;
            break;
        case kBBXor:
            destPixel = ((destPixel ^ sourcePixel)) & 1;
            break;
        default:
            break;
    }
}

void
BitBltXArgs::WriteDestPixel(u16 & destPixel)
{
    u32 pixelWord;
    u32 pixelOffset;
    u16 destPixelOrig;

    BumpBit(m_nDestPixelWordAddress, m_nDestPixelStart + m_nCurrentPixel, pixelWord, pixelOffset);
    destPixelOrig = MP_ReadMesaRealAddress(MP_Fetch(pixelWord));

    if (m_destType == kDisplay)
    {
        destPixelOrig = (destPixelOrig << 8) | (destPixelOrig >> 8);
    }

    destPixelOrig = destPixelOrig & (~ (1 << (15 - pixelOffset)));
    destPixelOrig |= (destPixel << (15 - pixelOffset));

    if (m_destType == kDisplay)
    {
        destPixelOrig = (destPixelOrig << 8) | (destPixelOrig >> 8);
    }
    MP_WriteMesaRealAddress(MP_Store(pixelWord), destPixelOrig);
}

inline void
BitBltXArgs::BumpBit(const u32 & baseAddress, u32 nNumBits, u32 & pixelWord, u32 & pixelOffset)
{
    pixelWord = baseAddress + (nNumBits / 16);
    pixelOffset = nNumBits % 16;
}

/////////////////////////////////////////////////////////////////////////////
// MesaMicrocode processor support functions
extern "C" u16
MesaMicrocode_GetInterruptMaskFunction()
{
    return MesaMicrocodeOps::GetInterruptMask();
}

extern "C" void
MesaMicrocode_SetInterruptMaskFunction(u16 nMask)
{
    MesaMicrocodeOps::SetInterruptMask(nMask);
}

extern "C" void
MesaMicrocode_WriteMPCodeFunction(u16 mpCode)
{
    MesaMicrocodeOps::SetMPCode(mpCode);
}

extern "C" u32
MesaMicrocode_PulseCountFunction()
{
    return MesaMicrocodeOps::GetPulseCount();
}

/////////////////////////////////////////////////////////////////////////////
// MesaMicrocode microcode functions
extern "C" void
MesaMicrocode_GetRGB_MVM()
{
    UNSPECIFIED colorIndex = MP_Pop();
    LONG_UNSPECIFIED color = MesaMicrocodeOps::LookupSysColor(colorIndex);
    MP_PushLong(color);
}

extern "C" void
MesaMicrocode_Snap_Toggle_State()
{
    // trap to software, don't know if there is a software implementation
    MP_EscOpcodeTrap(0x82);
}

extern "C" void
MesaMicrocode_CALLAGENT()
{
    UNSPECIFIED agentIndex = MP_Pop();
    if (0 != MesaAgent::CallAgent((MesaAgent::AgentIndex)agentIndex))
    {
        MP_Error();     // This stops the emulator!i
    }
}

extern "C" void
MesaMicrocode_MAPDISPLAY()
{
    CARDINAL pagesToMapInEachBlock = MP_Pop();
    CARDINAL totalNumPagesToMap = MP_Pop();
    LONG_CARDINAL realByteAddressToStartMapping = MP_PopLong();
    LONG_CARDINAL virtualPageAtWhichToStartMapping = MP_PopLong();

    // only do a limited implementation, based on 1 block for a mono bitmap
    if (pagesToMapInEachBlock != totalNumPagesToMap)
    {
        MP_Error();
    }

    // assume graphics memory starts exactly at the top of real memory
    if (realByteAddressToStartMapping != (((u32)MP_Mesa_Processor.config.m_pRealMemoryBaseAddress) + (MP_Mesa_Processor.config.m_nNumRealMemoryPages * 512)))
    {
        MP_Error();
    }

    // start mapping
    u32 firstRealPage = MP_Mesa_Processor.config.m_nNumRealMemoryPages;
    u32 lastRealPage = firstRealPage + totalNumPagesToMap;
    for (u32 realPage = firstRealPage; realPage < lastRealPage; realPage++, virtualPageAtWhichToStartMapping++)
    {
        *(MP_Mesa_Processor.config.m_pVMMap + virtualPageAtWhichToStartMapping) = (realPage << 8);
    }
}

extern "C" void
MesaMicrocode_STOPEMULATOR()
{
    LONG_CARDINAL timeToRestart = MP_PopLong();
    MP_Error();     // This stops the emulator!
}

extern "C" void
MesaMicrocode_SUSPEND()
{
    // trap to software, probably unimplemented instruction
    MP_EscOpcodeTrap(0x8D);
}

extern "C" void
MesaMicrocode_COLORBLT()
{
    // trap to software ok
    MP_EscOpcodeTrap(0xC0);
}

extern "C" void
MesaMicrocode_WRITEPIXEL()
{
    // trap to software ok
    MP_EscOpcodeTrap(0xC1);
}

extern "C" void
MesaMicrocode_BITBLTX()
{
    if (MP_Mesa_Processor.registers.SP == 11)
    {
        // starting the instruction from scratch
        UNSPECIFIED flags = MP_Pop();
        CARDINAL nHeight = MP_Pop();
        CARDINAL nWidth = MP_Pop();
        INTEGER nSourcePixelsPerLine = MP_Pop();
        CARDINAL nSourcePixelStart = MP_Pop();
        LONG_POINTER nSourcePixelWordAddress = MP_PopLong();
        INTEGER nDestPixelsPerLine = MP_Pop();
        CARDINAL nDestPixelStart = MP_Pop();
        LONG_POINTER nDestPixelWordAddress = MP_PopLong();
        BitBltXArgs * pBBXArgs = new BitBltXArgs(
            flags, nHeight, nWidth,
            nSourcePixelsPerLine, nSourcePixelStart, nSourcePixelWordAddress,
            nDestPixelsPerLine, nDestPixelStart, nDestPixelWordAddress);
        if (pBBXArgs)
        {
            // setup for instruction restart
            MP_PushLong((LONG_UNSPECIFIED)pBBXArgs);
            MP_Mesa_Processor.registers.SP = 0;
            MP_Mesa_Processor.state.savedSP = 2;
            // start the instruction
            pBBXArgs->BitBltX();
        }
    }
    else
    if (MP_Mesa_Processor.registers.SP == 2)
    {
        // resuming the instruction
        BitBltXArgs * pBBXArgs = (BitBltXArgs *)MP_PopLong();
        pBBXArgs->BitBltX();
    }
    else MP_StackError();
}

extern "C" void
MesaMicrocode_BITBLT()
{
    if (MP_Mesa_Processor.registers.SP == 1)
    {
        // starting the instruction from scratch
        POINTER pBitBltArgs = MP_Pop();
        /* BitBltArgs: 
            dst: BitAddress,
            dstBpl: INTEGER,
            src: BitAddress,
            srcDesc: SrcDesc,
            width: CARDINAL,
            height: CARDINAL,
            flags: BitBltFlags,
            reserved: UNSPECIFIED,
        */

        LONG_POINTER dstWord = MP_ReadDblMds(pBitBltArgs);
        CARDINAL dstBit = MP_ReadMesaRealAddress(MP_FetchMds(pBitBltArgs + 2));
        INTEGER dstBpl = MP_ReadMesaRealAddress(MP_FetchMds(pBitBltArgs + 3));
        LONG_POINTER srcWord = MP_ReadDblMds(pBitBltArgs + 4);
        CARDINAL srcBit = MP_ReadMesaRealAddress(MP_FetchMds(pBitBltArgs + 6));
        INTEGER srcBpl = MP_ReadMesaRealAddress(MP_FetchMds(pBitBltArgs + 7));     /* overlaid with grap: GrayParm */
        CARDINAL width = MP_ReadMesaRealAddress(MP_FetchMds(pBitBltArgs + 8));
        CARDINAL height = MP_ReadMesaRealAddress(MP_FetchMds(pBitBltArgs + 9));
        UNSPECIFIED flags = MP_ReadMesaRealAddress(MP_FetchMds(pBitBltArgs + 10));

        BitBltXArgs * pBBXArgs = new BitBltXArgs(dstWord, dstBit, dstBpl, srcWord, srcBit, srcBpl, width, height, flags);

        if (pBBXArgs)
        {
            // setup for instruction restart
            MP_PushLong((LONG_UNSPECIFIED)pBBXArgs);
            MP_Mesa_Processor.registers.SP = 0;
            MP_Mesa_Processor.state.savedSP = 2;
            // start the instruction
            pBBXArgs->BitBltX();
        }
    }
    else
    if (MP_Mesa_Processor.registers.SP == 2)
    {
        // resuming the instruction
        BitBltXArgs * pBBXArgs = (BitBltXArgs *)MP_PopLong();
        pBBXArgs->BitBltX();
    }
    else MP_StackError();
}
   
////////////////////////////////////////////////////////////////////////////////
// DEBUG CODE
#if 0

static bool s_bTurnedOff = false;
extern "C" void TurnOffReadyQueueDumping()
{
    s_bTurnedOff = true;
}

extern "C" void MP_DumpReadyQueue(int preemption)
{
// [READYQUEUE DISABLED]->[87]->[END]
// [READYQUEUE DISABLED]->[87]->[END]
// [READYQUEUE DISABLED]->[EMPTY] -> Monitor Entry reschedule error
// Question: Where did 86 go?

    char buff[80];
    PsbLink link;
    PsbIndex psb;
    Queue queue;
    if (s_bTurnedOff) return;
    queue = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessDataAreaHeaderReadyOffset(PDA)));
    
    // [READYQUEUE PREEMPT DISABLED BEFORE]->
    sprintf(buff, "[READYQUEUE%s%s]->",
        (preemption ? " PREEMPT" : ""),
        (MP_InterruptsEnabled() ? "" : " DISABLED"));

    DebugPrint(buff);
    if (MP_GetQueueTail(queue) == PsbNull)
    {
        DebugPrint("[EMPTY]\n");
        return;
    }
    link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_GetQueueTail(queue)))));
    while (1)
    {
        psb = MP_GetPsbLinkNext(link);
        link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(psb))));
        if (psb == MP_GetQueueTail(queue))
        {
            sprintf(buff, "[%d%s]->[END]\n", (int)psb, (psb == MP_Mesa_Processor.registers.PSB) ? "*" : "");
            DebugPrint(buff);
            break;
        }
        else
        {
            sprintf(buff, "[%d%s]->", (int)psb, (psb == MP_Mesa_Processor.registers.PSB) ? "*" : "");
            DebugPrint(buff);
        }
    }
}

#include <stdio.h>
#include <string.h>

extern "C" void DebugPrint(const char * pString);

class InstructionLog
{
public:
    InstructionLog(u8 opcode, bool bEsc);
public:
    u8 m_nOpcode;
    bool m_bEsc;
    u16 m_stack[14];
    u16 m_nSP;
    u16 m_nPSB;
    InstructionLog * m_pNext;
public:
    void Dump();
public:
    static void DumpLog();
    static void CleanLog();
};

int s_bLogInstructions = 0;
u16 s_nProcessToLog = 90;
InstructionLog * s_pFirstInstructionLog = 0;
InstructionLog * s_pLastInstructionLog = 0;


#if 1

bool s_bLoggerInitialized = false;
bool s_bLogging = true;

typedef struct _LogTableEntry
{
    u32 count;
    u32 newCount;
} LogTableEntry;

LogTableEntry s_table[256];
LogTableEntry s_tableEsc[256];

extern "C"
void LogInstruction(u8 opcode, int nEsc)
{
    if (! s_bLogging) return;
    if (! s_bLoggerInitialized)
    {
        s_bLoggerInitialized = true;
        memset(s_table, 0, sizeof(s_table));
        memset(s_tableEsc, 0, sizeof(s_tableEsc));
    }

    if (s_bLogInstructions)
    {
        // doing the new thing
        if (s_nProcessToLog == MP_Mesa_Processor.registers.PSB)
        {
            if (nEsc) s_tableEsc[opcode].newCount++;
            else s_table[opcode].newCount++;
        }
    }
    else
    {
        if (nEsc) s_tableEsc[opcode].count++;
        else s_table[opcode].count++;
    }
}
extern "C" void LogInstructionStart() { s_bLogInstructions = 1; }
extern "C" void LogInstructionStop()
{
    s_bLogInstructions = 0;
    s_bLogging = false;

    // dump the data
    char buff[256];
    DebugPrint("INSTRUCTION DUMP BEGIN\n");
    DebugPrint("----------------------\n");
    u32 i;
    for (i = 0; i < 256; i++)
    {
        if (0 == s_table[i].newCount) continue;
        sprintf(buff, "    %03d | % 8d % 8d\n", i, s_table[i].count, s_table[i].newCount);
        DebugPrint(buff);
    }
    for (i = 0; i < 256; i++)
    {
        if (0 == s_tableEsc[i].newCount) continue;
        sprintf(buff, "ESC %03d | % 8d % 8d\n", i, s_tableEsc[i].count, s_tableEsc[i].newCount);
        DebugPrint(buff);
    }
    DebugPrint("--------------------\n");
    DebugPrint("INSTRUCTION DUMP END\n");
}

#else

extern "C"
void LogInstruction(u8 opcode, int nEsc)
{
    if (! s_bLogInstructions) return;
    if (MP_Mesa_Processor.registers.MP == 938)
    {
        s_bLogInstructions = 0;
        InstructionLog::DumpLog();
        InstructionLog::CleanLog();
        return;
    }
    //if (s_nProcessToLog != MP_Mesa_Processor.registers.PSB) return;
    new InstructionLog(opcode, (nEsc ? true : false));
}
extern "C" void LogInstructionStart() { s_bLogInstructions = 1; }
extern "C" void LogInstructionStop()
{
    s_bLogInstructions = 0;
    InstructionLog::DumpLog();
    InstructionLog::CleanLog();
}
#endif


InstructionLog::InstructionLog(u8 opcode, bool bEsc)
{
    m_nOpcode = opcode;
    m_bEsc = bEsc;
    m_pNext = 0;
    m_nSP = MP_Mesa_Processor.registers.SP;
    m_nPSB = MP_Mesa_Processor.registers.PSB;
    for (u32 i = 0; i < m_nSP; i++)
    {
        m_stack[i] = MP_Mesa_Processor.registers.stack[i];
    }
    if (s_pFirstInstructionLog == 0)
    {
        s_pFirstInstructionLog = s_pLastInstructionLog = this;
    }
    else
    {
        s_pLastInstructionLog->m_pNext = this;
        s_pLastInstructionLog = this;
    }
}

void
InstructionLog::DumpLog()
{
    InstructionLog * pLog = s_pFirstInstructionLog;
    DebugPrint("INSTRUCTION DUMP BEGIN\n");
    DebugPrint("----------------------\n");
    while (pLog)
    {
        pLog->Dump();
        pLog = pLog->m_pNext;
    } 
    DebugPrint("--------------------\n");
    DebugPrint("INSTRUCTION DUMP END\n");
}

void
InstructionLog::Dump()
{
    char buff[256];
    sprintf(buff, "%04d | %s %03d | ", m_nPSB, (m_bEsc ? "ESC" : "   "), m_nOpcode);
    DebugPrint(buff);
    for (u32 i = 0; i < m_nSP; i++)
    {
        sprintf(buff, "%05d ", m_stack[i]);
        DebugPrint(buff);
    }
    DebugPrint("\n");
}

void
InstructionLog::CleanLog()
{
    InstructionLog * pLog = s_pFirstInstructionLog;
    while (pLog)
    {
        s_pLastInstructionLog = pLog;
        pLog = pLog->m_pNext;
        delete s_pLastInstructionLog;
    }
    s_pFirstInstructionLog = s_pLastInstructionLog = 0;
}
#endif

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
