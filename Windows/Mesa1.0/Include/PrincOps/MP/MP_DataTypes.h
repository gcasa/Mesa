 /*
    MP_DataTypes.h - Mesa types of Mesa Processor implementation

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_DATATYPES_H
#define _INCLUDE_PRINCOPS_MP_MP_DATATYPES_H

/*******************************************************************************
** Mesa Processor type manipulation functions */
__mpinline BYTE MP_HighByte(UNSPECIFIED data);
__mpinline BYTE MP_LowByte(UNSPECIFIED data);
__mpinline UNSPECIFIED MP_Not(UNSPECIFIED data);
__mpinline BOOLEAN MP_Odd(UNSPECIFIED data);
__mpinline UNSPECIFIED MP_And(UNSPECIFIED data1, UNSPECIFIED data2);
__mpinline UNSPECIFIED MP_Or(UNSPECIFIED data1, UNSPECIFIED data2);
__mpinline UNSPECIFIED MP_Xor(UNSPECIFIED data1, UNSPECIFIED data2);
__mpinline UNSPECIFIED MP_Shift(UNSPECIFIED data, INTEGER count);
__mpinline UNSPECIFIED MP_Rotate(UNSPECIFIED data, INTEGER count);
/*
__mpinline UNSPECIFIED MP_ArithShift(UNSPECIFIED data, INTEGER count);
*/
__mpinline UNSPECIFIED MP_HighHalf(LONG_UNSPECIFIED data);
__mpinline UNSPECIFIED MP_LowHalf(LONG_UNSPECIFIED data);
__mpinline LONG_UNSPECIFIED MP_LongAnd(LONG_UNSPECIFIED data1, LONG_UNSPECIFIED data2);
__mpinline LONG_UNSPECIFIED MP_LongOr(LONG_UNSPECIFIED data1, LONG_UNSPECIFIED data2);
__mpinline LONG_UNSPECIFIED MP_LongXor(LONG_UNSPECIFIED data1, LONG_UNSPECIFIED data2);
__mpinline LONG_UNSPECIFIED MP_LongShift(LONG_UNSPECIFIED data, INTEGER count);
/*
__mpinline LONG_UNSPECIFIED MP_LongArithShift(LONG_UNSPECIFIED data, INTEGER count);
*/
__mpinline INTEGER MP_SignExtend(BYTE z);

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_DATATYPES_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

