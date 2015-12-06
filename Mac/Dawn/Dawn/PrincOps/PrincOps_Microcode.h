/*
    PrincOps_Microcode.h - main include file for developing microcode
                         - instructions for the Mesa Processor

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_PRINCOPS_MICROCODE_H
#define _INCLUDE_PRINCOPS_PRINCOPS_MICROCODE_H

/*******************************************************************************
** NOTE: Include this file only if you are going to be developing
         microcode instructions for the Mesa Processor.
*/

#ifdef __cplusplus 
extern "C" {
#endif

#include "PrincOps.h"
#include "MP/MP_MesaTypes.h"
#include "MP/MP_ProcessorStruct.h"
#include "MP/MP_DataTypes.h"
#include "MP/MP_MemoryOrganization.h"
#include "MP/MP_InstructionInterpreter.h"
#include "MP/MP_StackInstructions.h"
#include "MP/MP_JumpInstructions.h"
#include "MP/MP_AssignmentInstructions.h"
#include "MP/MP_BlockTransfers.h"
#include "MP/MP_ControlTransfers.h"
#include "MP/MP_Processes.h"
#include "MP/MP_Undocumented.h"


// GJC : Removed "PrincOps/" from the beginning to make sure they compile.
#if !defined (_MP_INLINE_OFF)
#include "MP/MP_ProcessorStruct_Inline.h"
#include "MP/MP_DataTypes_Inline.h"
#include "MP/MP_MemoryOrganization_Inline.h"
#include "MP/MP_InstructionInterpreter_Inline.h"
#include "MP/MP_StackInstructions_Inline.h"
#include "MP/MP_JumpInstructions_Inline.h"
#include "MP/MP_AssignmentInstructions_Inline.h"
#include "MP/MP_BlockTransfers_Inline.h"
#include "MP/MP_ControlTransfers_Inline.h"
#include "MP/MP_Processes_Inline.h"
#include "MP/MP_Undocumented_Inline.h"
#endif


#ifdef __cplusplus 
}
#endif

#endif // #ifndef _INCLUDE_PRINCOPS_PRINCOPS_MICROCODE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

