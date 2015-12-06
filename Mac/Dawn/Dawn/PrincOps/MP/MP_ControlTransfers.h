/*
    MP_ControlTransfers.h - internal header file of Mesa Processor control transfer instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_CONTROLTRANSFERS_H
#define _INCLUDE_PRINCOPS_MP_MP_CONTROLTRANSFERS_H

/*******************************************************************************
** CONTROL TRANSFER TYPES */
typedef LONG_UNSPECIFIED                ControlLink;
typedef UNSPECIFIED                     ShortControlLink;
typedef enum { LinkType_frame = 0,
               LinkType_shortProc,
               LinkType_indirect,
               LinkType_longProc }      LinkType;

typedef LocalFrameHandle                FrameLink;
typedef POINTER                         IndirectLink;
typedef LONG_UNSPECIFIED                ShortProcDesc;
typedef LONG_UNSPECIFIED                LongProcDesc;

typedef BYTE                            FSIndex;
typedef UNSPECIFIED                     AVItem;
typedef POINTER                         AllocationVector;
typedef enum { AVTag_frame = 0,
               AVTag_empty,
               AVTag_indirect,
               AVTag_unused }           AVTag;
#define AV                              ((AllocationVector)mAV)

typedef enum { XFERType_return = 0,
               XFERType_call = 1,
               XFERType_local = 2,
               XFERType_port = 3,
               XFERType_xfer = 4,
               XFERType_trap = 5,
               XFERType_processSwitch = 6,
               XFERType_unused = 7 }    XFERType;

typedef BYTE                            SDIndex;
typedef POINTER                         SystemData;
#define SD                              ((SystemData)mSD)

typedef POINTER                         PortLink;
#define PORTLINKINPORTOFFSET            (0)
#define PORTLINKOUTPORTOFFSET           (2)

typedef POINTER                         EscTrapTable;
#define ETT                             ((EscTrapTable)mETT)
#define ETTEntrySize                    (2)

typedef LONG_POINTER                    StateHandle;
typedef UNSPECIFIED                     StateWord;
typedef POINTER                         StateVector;
#define STATEVECTORSTACKOFFSET          (0)
#define STATEVECTORSTATEWORDOFFSET      (14)
#define STATEVECTORFRAMEOFFSET          (15)
#define STATEVECTORDATAZEROOFFSET       (16)
#define STATEVECTORDATAONEOFFSET        (17)

typedef POINTER                         TransferDescriptor;
#define TRANSFERDESCRIPTORSRCOFFSET     (0)
#define TRANSFERDESCRIPTORDSTOFFSET     (2)

#define CODESEGMENTSIZE                 (4)

/*******************************************************************************
** CONTROL TRANSFER FUNCTIONS */
__mpinline POINTER MP_GetSDOffset(SDIndex index);
__mpinline UNSPECIFIED MP_GetTaggedControlLinkData(ControlLink controlLink);
__mpinline LinkType MP_GetTaggedControlLinkType(ControlLink controlLink);
__mpinline UNSPECIFIED MP_GetTaggedControlLinkFill(ControlLink controlLink);
__mpinline LinkType MP_ControlLinkType(ControlLink controlLink);
__mpinline FrameLink MP_MakeFrameLink(ControlLink controlLink);
__mpinline IndirectLink MP_MakeIndirectLink(ControlLink controlLink);
__mpinline UNSPECIFIED MP_GetTaggedShortProcDescGF(ShortProcDesc shortProcDesc);
__mpinline CARDINAL MP_GetTaggedShortProcDescPC(ShortProcDesc shortProcDesc);
__mpinline ShortProcDesc MP_MakeShortProcDesc(ControlLink controlLink);
__mpinline UNSPECIFIED MP_GetTaggedLongProcDescGFTHandle(LongProcDesc longProcDesc);
__mpinline CARDINAL MP_GetTaggedLongProcDescPC(LongProcDesc longProcDesc);
__mpinline LongProcDesc MP_MakeLongProcDesc(ControlLink controlLink);
__mpinline UNSPECIFIED MP_GetTaggedAVItemData(AVItem avi);
__mpinline AVTag MP_GetTaggedAVItemTag(AVItem avi);
__mpinline LocalFrameHandle MP_AVFrame(AVItem avi);
__mpinline POINTER MP_AVLink(AVItem avi);
__mpinline LocalFrameHandle MP_Alloc(FSIndex fsi);
__mpinline void MP_Free(LocalFrameHandle frame);
__mpinline void MP_XFER(ControlLink dst, ShortControlLink src, XFERType type, BOOLEAN free);
__mpinline ControlLink MP_FetchLink(BYTE offset);
__mpinline void MP_Call(ControlLink dst);
__mpinline POINTER MP_GetPortLinkInPortOffset(PortLink portLink);
__mpinline POINTER MP_GetPortLinkOutPortOffset(PortLink portLink);
__mpinline void MP_PortOut();
__mpinline BOOLEAN MP_ValidContext();
__mpinline void MP_Trap(POINTER ptr);
__mpinline void MP_TrapZero(POINTER ptr);
__mpinline void MP_TrapOne(POINTER ptr, UNSPECIFIED parameter);
__mpinline void MP_TrapTwo(POINTER ptr, LONG_UNSPECIFIED parameter);
__mpinline void MP_BoundsTrap();
__mpinline void MP_BreakTrap();
__mpinline void MP_CodeTrap(CARDINAL gfi);
__mpinline void MP_ControlTrap(ShortControlLink src);
__mpinline void MP_DivCheckTrap();
__mpinline void MP_DivZeroTrap();
__mpinline void MP_EscOpcodeTrap(BYTE opcode);
__mpinline void MP_InterruptError();
__mpinline void MP_OpcodeTrap(BYTE opcode);
__mpinline void MP_PointerTrap();
__mpinline void MP_ProcessTrap();
__mpinline void MP_RescheduleError();
__mpinline void MP_StackError();
__mpinline void MP_UnboundTrap(ControlLink dst);
__mpinline void MP_HardwareError();
__mpinline LONG_POINTER MP_GetStateHandleStackOffset(StateHandle stateHandle);
__mpinline LONG_POINTER MP_GetStateHandleStateWordOffset(StateHandle stateHandle);
__mpinline LONG_POINTER MP_GetStateHandleFrameOffset(StateHandle stateHandle);
__mpinline LONG_POINTER MP_GetStateHandleDataZeroOffset(StateHandle stateHandle);
__mpinline LONG_POINTER MP_GetStateHandleDataOneOffset(StateHandle stateHandle);
__mpinline StateWord MP_MakeStateWord(BYTE breakByte, CARDINAL sp);
__mpinline CARDINAL MP_GetStateWordStackPointer(StateWord stateWord);
__mpinline BYTE MP_GetStateWordBreakByte(StateWord stateWord);
__mpinline void MP_SaveStack(StateHandle state);
__mpinline void MP_LoadStack(StateHandle state);
__mpinline POINTER MP_GetTransferDescriptorSrcOffset(TransferDescriptor transferDescriptor);
__mpinline POINTER MP_GetTransferDescriptorDstOffset(TransferDescriptor transferDescriptor);
__mpinline void MP_CheckForXferTraps(ControlLink dst, XFERType type);

/*******************************************************************************
** CONTROL TRANSFER INSTRUCTIONS */

/* Frame Allocation Instructions */
void MP_Instruction_AF();       /* Allocate Frame */
void MP_Instruction_FF();       /* Free Frame */

/* Control transfer instructions */
void MP_Instruction_LFC();      /* Local Function Call */
void MP_Instruction_EFC0();     /* External Function Call 0 */
void MP_Instruction_EFC1();     /* External Function Call 1 */
void MP_Instruction_EFC2();     /* External Function Call 2 */
void MP_Instruction_EFC3();     /* External Function Call 3 */
void MP_Instruction_EFC4();     /* External Function Call 4 */
void MP_Instruction_EFC5();     /* External Function Call 5 */
void MP_Instruction_EFC6();     /* External Function Call 6 */
void MP_Instruction_EFC7();     /* External Function Call 7 */
void MP_Instruction_EFC8();     /* External Function Call 8 */
void MP_Instruction_EFC9();     /* External Function Call 9 */
void MP_Instruction_EFC10();    /* External Function Call 10 */
void MP_Instruction_EFC11();    /* External Function Call 11 */
void MP_Instruction_EFC12();    /* External Function Call 12 */
void MP_Instruction_EFCB();     /* External Function Call Byte */
void MP_Instruction_SFC();      /* Stack Function Call */
void MP_Instruction_KFCB();     /* Kernel Function Call Byte */
void MP_Instruction_LKB();      /* Link Byte */
void MP_Instruction_RET();      /* Return */
void MP_Instruction_PO();       /* Port Out */
void MP_Instruction_POR();      /* Port Out Responding */
void MP_Instruction_PI();       /* Port In */
void MP_Instruction_LLKB();     /* Load Link Byte */
void MP_Instruction_RKIB();     /* Read Link Indirect Byte */
void MP_Instruction_RKDIB();    /* Read Link Double Indirect Byte */

/* Traps */
void MP_Instruction_DSK();      /* Dump Stack */
void MP_Instruction_LSK();      /* Load Stack */
void MP_Instruction_XF();       /* XFER and Free */
void MP_Instruction_XE();       /* XFER and Enable */
void MP_Instruction_BRK();      /* Break */


#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_CONTROLTRANSFERS_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

