 /*
    MP_MesaTypes.h - Mesa types of Mesa PrincOps implementation

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_MESATYPES_H
#define _INCLUDE_PRINCOPS_MP_MP_MESATYPES_H

/*******************************************************************************
** Mesa Processor defs and types */
#define                 WordSize              (16)
#define                 READONLY 
typedef u8              BYTE;
typedef u16             UNSPECIFIED;
typedef u32             LONG_UNSPECIFIED;
typedef u16             POINTER;
typedef u32             LONG_POINTER;
typedef u16             CARDINAL;
typedef u32             LONG_CARDINAL;
typedef s16             INTEGER;
typedef s32             LONG_INTEGER;
typedef u32             LONG;
typedef UNSPECIFIED     BOOLEAN;    /* not really, but C doesn't support bit variables.
                                       we'll only use BOOLEAN wisely - to check for
                                       results, not for structure members */

#define MESA_TRUE       (1)
#define MESA_FALSE      (0)

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_MESATYPES_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

