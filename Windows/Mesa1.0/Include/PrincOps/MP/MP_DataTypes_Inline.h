 /*
    MP_DataTypes_Inline.h - Mesa type inline helper functions of Mesa Processor implementation

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_DATATYPES_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_DATATYPES_INLINE_H

/*******************************************************************************
** Mesa Processor type manipulation functions */
__mpinline BYTE MP_HighByte(UNSPECIFIED data)
{
    return (BYTE)(data >> 8);
}

__mpinline BYTE MP_LowByte(UNSPECIFIED data)
{
    return (BYTE)data;
}

__mpinline UNSPECIFIED MP_Not(UNSPECIFIED data)
{
    return ~data;
}

__mpinline BOOLEAN MP_Odd(UNSPECIFIED data)
{
    return (data & 1) ? MESA_TRUE : MESA_FALSE;
}

__mpinline UNSPECIFIED MP_And(UNSPECIFIED data1, UNSPECIFIED data2)
{
    return data1 & data2;
}

__mpinline UNSPECIFIED MP_Or(UNSPECIFIED data1, UNSPECIFIED data2)
{
    return data1 | data2;
}

__mpinline UNSPECIFIED MP_Xor(UNSPECIFIED data1, UNSPECIFIED data2)
{
    return data1 ^ data2;
}

__mpinline UNSPECIFIED MP_Shift(UNSPECIFIED data, INTEGER count)
{
    if ((count >15) || (count < -15)) return 0;
    return ((count >= 0) ? (data << count) : (data >> (0-count)));
}

__mpinline UNSPECIFIED MP_Rotate(UNSPECIFIED data, INTEGER count)
{
    /* ISSUE: only need to rotate abs(count) mod 16 */
    if (count > 0)
    {
        while (count-- > 0)
        {
            BOOLEAN highBit = (data & 0x8000) ? MESA_TRUE : MESA_FALSE;
            data <<= 1;
            if (highBit) data |= 1;
        }
    }
    else
    if (count < 0)
    {
        while (count++ < 0)
        {
            BOOLEAN lowBit = (data & 1) ? MESA_TRUE : MESA_FALSE;
            data >>= 1;
            if (lowBit) data |= 0x8000;
        }
    }

    return data;
}

/*
__mpinline UNSPECIFIED MP_ArithShift(UNSPECIFIED data, INTEGER count)
{
    // WRONG
    return ((count > 0) ? (data << count) : (data >> count));
}
*/

__mpinline UNSPECIFIED MP_HighHalf(LONG_UNSPECIFIED data)
{
    return ((UNSPECIFIED)(data >> 16));
}

__mpinline UNSPECIFIED MP_LowHalf(LONG_UNSPECIFIED data)
{
    return ((UNSPECIFIED)data);
}

__mpinline LONG_UNSPECIFIED MP_LongAnd(LONG_UNSPECIFIED data1, LONG_UNSPECIFIED data2)
{
    return data1 & data2;
}

__mpinline LONG_UNSPECIFIED MP_LongOr(LONG_UNSPECIFIED data1, LONG_UNSPECIFIED data2)
{
    return data1 | data2;
}

__mpinline LONG_UNSPECIFIED MP_LongXor(LONG_UNSPECIFIED data1, LONG_UNSPECIFIED data2)
{
    return data1 ^ data2;
}

__mpinline LONG_UNSPECIFIED MP_LongShift(LONG_UNSPECIFIED data, INTEGER count)
{
    if ((count >31) || (count < -31)) return 0;
    return ((count >= 0) ? (data << count) : (data >> (0-count)));
}

/*
__mpinline LONG_UNSPECIFIED MP_LongArithShift(LONG_UNSPECIFIED data, INTEGER count)
{
    // WRONG
    return ((count > 0) ? (data << count) : (data >> count));
}
*/

__mpinline INTEGER MP_SignExtend(BYTE z)
{
    return ((z < 128) ? ((INTEGER)z) : ((INTEGER)(((INTEGER)z) - 256)));
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_DATATYPES_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

