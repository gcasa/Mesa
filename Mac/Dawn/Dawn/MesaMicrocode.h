// MesaMicrocode.h : header file for Win32 specific Microcode for Mesa Processor
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAPROCESSOR_MESAMICROCODE_H
#define _SOURCE_MESADLL_MESAPROCESSOR_MESAMICROCODE_H

#include <PrincOps/PrincOps_PlatformTypes.h>

/////////////////////////////////////////////////////////////////////////////
// MesaMicrocode processor support functions
extern "C" u16  MesaMicrocode_GetInterruptMaskFunction();
extern "C" void MesaMicrocode_SetInterruptMaskFunction(u16 nMask);
extern "C" void MesaMicrocode_WriteMPCodeFunction(u16 mpCode);
extern "C" u32  MesaMicrocode_PulseCountFunction();

/////////////////////////////////////////////////////////////////////////////
// MesaMicrocode microcode functions
extern "C" void MesaMicrocode_GetRGB_MVM();
extern "C" void MesaMicrocode_Snap_Toggle_State();
extern "C" void MesaMicrocode_CALLAGENT();
extern "C" void MesaMicrocode_MAPDISPLAY();
extern "C" void MesaMicrocode_STOPEMULATOR();
extern "C" void MesaMicrocode_SUSPEND();
extern "C" void MesaMicrocode_COLORBLT();
extern "C" void MesaMicrocode_WRITEPIXEL();
extern "C" void MesaMicrocode_BITBLTX();
extern "C" void MesaMicrocode_BITBLT();

extern "C" unsigned int MesaMicrocode_Util_GetNumExecutedInstructions();

#endif // #ifndef _SOURCE_MESADLL_MESAPROCESSOR_MESAMICROCODE_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
