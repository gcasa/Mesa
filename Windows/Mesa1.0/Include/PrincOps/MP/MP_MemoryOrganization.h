/*
    MP_MemoryOrganization.h - internal header file of Mesa Processor Memory Organization

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_MEMORYORGANIZATION_H
#define _INCLUDE_PRINCOPS_MP_MP_MEMORYORGANIZATION_H

/*******************************************************************************
** VIRTUAL MEMORY TYPES AND CONSTANTS */
#define                     PageSize              ((CARDINAL)256)  /* 1 Page is 256 words or 512 bytes */
typedef LONG_CARDINAL       PageNumber;                       /* There are 2^32 pages of VM       */
typedef PageNumber          VirtualPageNumber;
typedef PageNumber          RealPageNumber;
typedef LONG_POINTER        VirtualAddress;
typedef LONG_POINTER        RealAddress;
typedef POINTER             LocalFrameHandle;
typedef POINTER             GlobalFrameHandle;
typedef LONG_UNSPECIFIED    MapEntry;             /* not from PrincOps, what we put in our VM Map */
typedef UNSPECIFIED         MapFlags;
#define                     MapFlagsReserved      ((CARDINAL)65528)
#define                     MapFlagsProtected     ((CARDINAL)4)
#define                     MapFlagsDirty         ((CARDINAL)2)
#define                     MapFlagsReferenced    ((CARDINAL)1)
#define                     MapFlagsVacantMask    ((CARDINAL)7)
#define                     MapFlagsVacantValue   ((CARDINAL)6)

typedef enum
{
    MapOp_Read = 0,
    MapOp_Write = 1,
} MapOp;

typedef UNSPECIFIED         LocalWord;
typedef UNSPECIFIED         GlobalWord;
#define                     GLOBALOVERHEADSIZE      ((CARDINAL)2)   // sizeof(PrincOpsExtras2.GlobalOverhead) == 2 words
#define                     GLOBALWORDOFFSET        (1)
#define                     LOCALOVERHEADSIZE       ((CARDINAL)4)   // sizeof(LocalOverhead) == 4 words
#define                     LOCALWORDOFFSET         (0)
#define                     LOCALRETURNLINKOFFSET   (1)
#define                     LOCALGLOBALLINKOFFSET   (2)
#define                     LOCALPCOFFSET           (3)
#define                     PROCESSSTATEBLOCKSIZE   ((CARDINAL)8)

/*******************************************************************************
** VIRTUAL MEMORY INLINE FUNCTIONS */
__mpinline void MP_ReadMap(VirtualPageNumber nVirtualPageNumber, MapFlags * pMapFlagsToSet, RealPageNumber * pRealPageNumberToSet);
__mpinline void MP_WriteMap(VirtualPageNumber nVirtualPageNumber, MapFlags nMapFlags, RealPageNumber nRealPageNumber);
__mpinline BOOLEAN MP_Vacant(MapFlags nMapFlags);
__mpinline RealAddress MP_Map(VirtualAddress nVirtualAddress, MapOp op);
__mpinline RealAddress MP_Fetch(VirtualAddress nVirtualAddress);
__mpinline RealAddress MP_Store(VirtualAddress nVirtualAddress);
__mpinline LONG_UNSPECIFIED MP_ReadDbl(VirtualAddress nVirtualAddress);
__mpinline UNSPECIFIED MP_ReadCode(CARDINAL nOffset);
__mpinline LONG_POINTER MP_LengthenPointer(POINTER pointer);
__mpinline LONG_POINTER MP_FetchMds(POINTER pointer);
__mpinline LONG_POINTER MP_StoreMds(POINTER pointer);
__mpinline LONG_UNSPECIFIED MP_ReadDblMds(POINTER pointer);
__mpinline LONG_POINTER MP_GlobalBase(LONG_POINTER pointerToGlobalFrame);
__mpinline LONG_POINTER MP_GlobalBaseGlobalWordOffset(LONG_POINTER pointerToGlobalFrame);
__mpinline CARDINAL MP_GlobalWordGFI(GlobalWord globalWord);
__mpinline BOOLEAN MP_GlobalWordCodeLinks(GlobalWord globalWord);
__mpinline BOOLEAN MP_GlobalWordTrapXfers(GlobalWord globalWord);
__mpinline POINTER MP_LocalBase(POINTER pointerToLocalFrame);
__mpinline POINTER MP_LocalBaseWordOffset(POINTER pointerToLocalFrame);
__mpinline POINTER MP_LocalBaseReturnLinkOffset(POINTER pointerToLocalFrame);
__mpinline POINTER MP_LocalBaseGlobalLinkOffset(POINTER pointerToLocalFrame);
__mpinline POINTER MP_LocalBaseProgramCounterOffset(POINTER pointerToLocalFrame);
__mpinline BYTE MP_LocalWordFSI(LocalWord localWord);
__mpinline POINTER MP_Handle(CARDINAL index);
__mpinline CARDINAL MP_Index(POINTER handle);
__mpinline BYTE MP_NibbleLeft(BYTE pair);
__mpinline BYTE MP_NibbleRight(BYTE pair);
__mpinline BYTE MP_Left(UNSPECIFIED word);
__mpinline BYTE MP_Right(UNSPECIFIED word);

/*******************************************************************************
** VIRTUAL MEMORY INSTRUCTIONS */
void MP_Instruction_SM();       /* Set Map       */
void MP_Instruction_SMF();      /* Set Map Flags */
void MP_Instruction_GMF();      /* Get Map Flags */
void MP_Instruction_LP();       /* Lengthen Pointer */
void MP_Instruction_ROB();      /* Read Overhead Byte */
void MP_Instruction_WOB();      /* Write Overhead Byte */
void MP_Instruction_RRIT();     /* Read Register IT */
void MP_Instruction_RRMDS();    /* Read Register MDS */
void MP_Instruction_RRPSB();    /* Read Register PSB */
void MP_Instruction_RRPTC();    /* Read Register PTC */
void MP_Instruction_RRWDC();    /* Read Register WDC */
void MP_Instruction_RRWP();     /* Read Register WP */
void MP_Instruction_RRXTS();    /* Read Register XTS */
void MP_Instruction_WRIT();     /* Write Register IT */
void MP_Instruction_WRMDS();    /* Write Register MDS */
void MP_Instruction_WRMP();     /* Write Register MP */
void MP_Instruction_WRPSB();    /* Write Register PSB */
void MP_Instruction_WRPTC();    /* Write Register PTC */
void MP_Instruction_WRWDC();    /* Write Register WDC */
void MP_Instruction_WRWP();     /* Write Register WP */
void MP_Instruction_WRXTS();    /* Write Register XTS */

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_MEMORYORGANIZATION_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

