/*
    MP_ProcessorStruct_Inline.h - header file of Mesa Processor state inline functions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_PROCESSORSTRUCT_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_PROCESSORSTRUCT_INLINE_H

/*******************************************************************************
** Mesa ProcessorStruct inline functions */
__mpinline UNSPECIFIED MP_ReadMesaRealAddress(LONG_POINTER nRealAddress)
{
    return MESA_TO_C_UNSPECIFIED(*(MP_Mesa_Processor.config.m_pRealMemoryBaseAddress + nRealAddress));
}

__mpinline void MP_WriteMesaRealAddress(LONG_POINTER nRealAddress, UNSPECIFIED data)
{
    *(MP_Mesa_Processor.config.m_pRealMemoryBaseAddress + nRealAddress) = C_TO_MESA_UNSPECIFIED(data);
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_PROCESSORSTRUCT_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

