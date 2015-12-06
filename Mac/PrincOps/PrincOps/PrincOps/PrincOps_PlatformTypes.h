/*
    PrincOps_PlatformTypes.h - platform dependent types of the Mesa PrincOps implementation

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_PRINCOPS_PLATFORMTYPES_H
#define _INCLUDE_PRINCOPS_PRINCOPS_PLATFORMTYPES_H

/*******************************************************************************
** BEGIN WIN32 */
#if defined (WIN32)

/*******************************************************************************
** WIN32 typedefs */
typedef unsigned long   u32;
typedef unsigned short  u16;
typedef unsigned char   u8;
typedef signed long     s32;
typedef signed short    s16;
typedef signed char     s8;

/*******************************************************************************
** WIN32 #defines */
#define __mpinline  __inline

/*******************************************************************************
** WIN32 type conversion
   WordLen  Number      MESA        WIN32
   16       0xDEAD      0xDEAD      0xADDE
   32       0xDEADC0ED  0xC0EDDEAD  0xEDC0ADDE   
*/

#if 0
    /* This is how I think it should work, but apparently for the sake of performance data on the GVWin virtual
       disk is stored in PC format.
    */
    #define MESA_TO_C_UNSPECIFIED(data) (((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8))
    #define C_TO_MESA_UNSPECIFIED(data) (MESA_TO_C_UNSPECIFIED(data))
    #define MESA_TO_C_LONG_UNSPECIFIED(data) ((MESA_TO_C_UNSPECIFIED(data >> 16) << 16) | MESA_TO_C_UNSPECIFIED(data))
    #define C_TO_MESA_LONG_UNSPECIFIED(data) (MESA_TO_C_LONG_UNSPECIFIED(data))
        #if 0
            #define MESA_TO_C_LONG_UNSPECIFIED(data) ( \
                ((data & 0xFF000000) >> 8) | \
                ((data & 0x00FF0000) << 8) | \
                ((data & 0x0000FF00) >> 8) | \
                ((data & 0x000000FF) << 8))
        #endif
#else
    /* This is what seems to work with GVWin virtual disks.  */
    #define MESA_TO_C_UNSPECIFIED(data)         (data)
    #define C_TO_MESA_UNSPECIFIED(data)         (data)
    #define MESA_TO_C_LONG_UNSPECIFIED(data)    (data)
    #define C_TO_MESA_LONG_UNSPECIFIED(data)    (data)
#endif

/* END WIN32
*******************************************************************************/

/*******************************************************************************
** BEGIN X86LINUX */
#elif defined (X86LINUX) || defined (__APPLE__)

/*******************************************************************************
** X86LINUX typedefs */
typedef unsigned long   u32;
typedef unsigned short  u16;
typedef unsigned char   u8;
typedef signed long     s32;
typedef signed short    s16;
typedef signed char     s8;

/*******************************************************************************
** X86LINUX #defines */
#define __mpinline __inline__

/*******************************************************************************
** X86LINUX type conversion
   WordLen  Number      MESA        X86LINUX
   16       0xDEAD      0xDEAD      0xADDE
   32       0xDEADC0ED  0xC0EDDEAD  0xEDC0ADDE   
*/

/* This is what seems to work with GVWin virtual disks.  */
#define MESA_TO_C_UNSPECIFIED(data)         (data)
#define C_TO_MESA_UNSPECIFIED(data)         (data)
#define MESA_TO_C_LONG_UNSPECIFIED(data)    (data)
#define C_TO_MESA_LONG_UNSPECIFIED(data)    (data)

/* END X86LINUX */

#else
#error "Unsupported Platform"
#endif

/*******************************************************************************
** Platform independent declaration specifiers */
#if defined (_MP_INLINE_OFF)
#undef  __mpinline
#define __mpinline
#endif

#endif // #ifndef _INCLUDE_PRINCOPS_PRINCOPS_PLATFORMTYPES_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created
24-Jul-03   dwoodward   added X86LINUX
*/

