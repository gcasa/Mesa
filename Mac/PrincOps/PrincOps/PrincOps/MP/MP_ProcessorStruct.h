/*
    MP_ProcessorStruct.h - header file of Mesa Processor state implementation

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_PROCESSORSTRUCT_H
#define _INCLUDE_PRINCOPS_MP_MP_PROCESSORSTRUCT_H

#include <setjmp.h>

/*******************************************************************************
** PROCEDURE DECLARATIONS */
int MP_ProcessorInitialize();
int MP_ProcessorUninitialize(); 

/*******************************************************************************
** Mesa Processor constants */

/* Miscellaneous Constants */
#define     cSS                 ((CARDINAL)14)          /* Stack Size */
#define     cSV                 ((CARDINAL)0)           /* State Vector Size ?? Processor Dependent */
#define     cWM                 ((CARDINAL)0)           /* Wakeup Mask ?? Processor Dependent */ 

#if 0
    #define     cWDC                ((CARDINAL)7)           /* Wakeup Disable Count - by the book */
#else
    #define     cWDC                ((CARDINAL)255)           /* Wakeup Disable Count from FastXfer.mc */
#endif

#define     cTickMin            ((CARDINAL)15)          /* Min size of tick in milliseconds */
#define     cTickMax            ((CARDINAL)60)          /* Max size of tick in milliseconds */
#define     StackDepth          (cSS)

/* Constant Memory locations */
#define     mPDA                ((LONG_CARDINAL)65536)  /* Process Data Area */
#define     mAV                 ((LONG_CARDINAL)256)    /* Allocation Vector */
#define     mSD                 ((LONG_CARDINAL)512)    /* System Data Table */
#define     mETT                ((LONG_CARDINAL)1024)   /* ESC Trap Table */
#define     mGFT                ((LONG_CARDINAL)131072) /* Global Frame Table */

/* Fault Queue Indexes */
#define     qFrameFault         ((CARDINAL)0)
#define     qPageFault          ((CARDINAL)1)
#define     qWriteProtectFault  ((CARDINAL)2)

/* System Data Table Indexes */
#define     sBoot               ((CARDINAL)1)
#define     sBreakTrap          ((CARDINAL)0)
#define     sControlTrap        ((CARDINAL)6)
#define     sDivZeroTrap        ((CARDINAL)10)
#define     sOpcodeTrap         ((CARDINAL)5)
#define     sProcessTrap        ((CARDINAL)13)
#define     sStackError         ((CARDINAL)2)
#define     sXferTrap           ((CARDINAL)4)
#define     sBoundsTrap         ((CARDINAL)14)
#define     sCodeTrap           ((CARDINAL)7)
#define     sDivCheckTrap       ((CARDINAL)11)
#define     sInterruptError     ((CARDINAL)12)
#define     sPointerTrap        ((CARDINAL)15)
#define     sRescheduleError    ((CARDINAL)3)
#define     sUnboundTrap        ((CARDINAL)9)
#define     sHardwareError      ((CARDINAL)8)
#define     sFirstGermRequest   ((CARDINAL)208)


/*******************************************************************************
** Mesa Processor types and global variables */
typedef void (InstructionFunction)();

typedef struct
{
    PrincOps_GetPulseCount_Function *       m_pGetPulseCountFunction;
    PrincOps_WriteMPCode_Function *         m_pWriteMPCodeFunction;
    PrincOps_GetInterruptMask_Function *    m_pGetInterruptMaskFunction;
    PrincOps_SetInterruptMask_Function *    m_pSetInterruptMaskFunction;
    
    LONG_CARDINAL                           m_nNumRealMemoryPages;
    UNSPECIFIED *                           m_pRealMemoryBaseAddress;
    
    LONG_CARDINAL                           m_nNumVirtualMemoryPages;
    LONG_UNSPECIFIED *                      m_pVMMap;

    BOOLEAN                                 m_bByteSwapExternalMemory;

    LONG_CARDINAL                           TimeOutInterval;

} ProcessorConfigStruct;

typedef struct
{
    /* Control Registers */
    CARDINAL                PSB;        /* Processor State Block index [0..1024) */
    LONG_POINTER            MDS;        /* Main Data Space virtual address */
    POINTER                 LF;         /* Local Frame Handle */
    LONG_POINTER            GF;         /* Global Frame Handle (now long pointer, in or out of MDS) */
    CARDINAL                GFI;        /* Global Frame Index */
    LONG_POINTER            CB;         /* Code Base */  
    CARDINAL                PC;         /* Program Counter */  
    
    /* Evaluation Stack */
    CARDINAL                SP;         /* Stack Pointer [0..cSS) */
    UNSPECIFIED             stack[cSS]; /* Stack */
    
    /* Data and Status Registers */
    READONLY UNSPECIFIED    PID[4];     /* Processor ID */
    CARDINAL                MP;         /* Maintenance Panel */
    LONG_CARDINAL           IT;         /* Interval Timer */
    READONLY CARDINAL       WM;         /* Wakeup Mask (for Interrupts) */
    CARDINAL                WP;         /* Wakeup Pending */
    CARDINAL                WDC;        /* Wakeup Disable Counter */
    CARDINAL                PTC;        /* Process Timeout Counter */
    CARDINAL                XTS;        /* Xfer Trap Status */
} ProcessorRegisters;

typedef struct
{
    /* From PrincOps */
    LONG_CARDINAL           time;       /* used for timing out processes */
    BOOLEAN                 running;
    BYTE                    breakbyte;  /* called break in PrincOps, but break is reserved in C */
    CARDINAL                savedPC;
    CARDINAL                savedSP;

    /* State for this implementation of PrincOps */
    jmp_buf                 jmp_buf_ABORT;
    BOOLEAN                 terminate;
    LONG_CARDINAL           instructionsDispatched;

    /* Performance enhancement */
    #ifndef _MP_BY_THE_BOOK

        /* for optimized instruction table jumping */
        BYTE                savedOpcode;

        /* for the simple instruction cache */
        LONG_POINTER        cachedCB;
        u32                 cachedVirtualCodePage;
        BYTE *              pCodePageCache;
    #endif

} ProcessorState;

typedef struct
{
    ProcessorConfigStruct config;
    ProcessorRegisters registers;
    ProcessorState state;
} Mesa_Processor;

/*******************************************************************************
** Mesa ProcessorStruct extern variables */
extern Mesa_Processor MP_Mesa_Processor;
extern InstructionFunction * MP_Mesa_Opcode_Table[256];
extern InstructionFunction * MP_Mesa_OpcodeEsc_Table[256];
#define MP_Mesa_OpcodeEscL_Table MP_Mesa_OpcodeEsc_Table   /* extern InstructionFunction * MP_Mesa_OpcodeEscL_Table[256]; */

/*******************************************************************************
** Mesa ProcessorStruct functions */
__mpinline UNSPECIFIED MP_ReadMesaRealAddress(LONG_POINTER nRealAddress);
__mpinline void MP_WriteMesaRealAddress(LONG_POINTER nRealAddress, UNSPECIFIED data);

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_PROCESSORSTRUCT_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

