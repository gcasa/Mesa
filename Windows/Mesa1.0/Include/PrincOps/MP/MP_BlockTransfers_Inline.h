/*
    MP_BlockTransfers_Inline.h - header file of Mesa Processor block transfer inline functions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_BLOCKTRANSFERS_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_BLOCKTRANSFERS_INLINE_H

/*******************************************************************************
** BLOCK TRANSFER INLINE FUNCTIONS */
__mpinline CARDINAL MP_Checksum(CARDINAL cksum, CARDINAL data)
{
    CARDINAL temp = cksum + data;
    if (cksum > temp) temp++;
    if (temp >= 0x8000) temp = temp * 2 + 1; else temp *= 2;
    return temp; 
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_BLOCKTRANSFERS_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

