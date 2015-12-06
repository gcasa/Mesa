/*
    MP_Processes.h - internal header file of Mesa processor process instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_PROCESSES_H
#define _INCLUDE_PRINCOPS_MP_MP_PROCESSES_H

/*******************************************************************************
** PROCESS TYPES */
typedef LONG_POINTER                            ProcessDataAreaHeader;
typedef LONG_POINTER                            StateAllocationTable;
typedef LONG_POINTER                            FaultVector;
typedef BYTE                                    FaultIndex;
typedef LONG_POINTER                            InterruptVector;
typedef BYTE                                    InterruptLevel;
#define PDAAVAILABLESIZE                        (5)
#define PDASTATEALLOCATIONTABLESIZE             (8)
#define PDAINTERRUPTVECTORSIZE                  (8 * 4)
#define PDAFAULTVECTORSIZE                      (8 * 2)
#define PROCESSDATAAREAHEADERREADYOFFSET        (0)
#define PROCESSDATAAREAHEADERCOUNTOFFSET        (1)
#define PROCESSDATAAREAHEADERUNUSEDOFFSET       (2)
#define PROCESSDATAAREAHEADERAVAILOFFSET        (3)
#define PROCESSDATAAREAHEADERSTATEOFFSET        (PROCESSDATAAREAHEADERAVAILOFFSET + PDAAVAILABLESIZE)
#define PROCESSDATAAREAHEADERINTERRUPTOFFSET    (PROCESSDATAAREAHEADERSTATEOFFSET + PDASTATEALLOCATIONTABLESIZE)
#define PROCESSDATAAREAHEADERFAULTOFFSET        (PROCESSDATAAREAHEADERINTERRUPTOFFSET + PDAINTERRUPTVECTORSIZE)
#define PROCESSDATAAREAHEADERSIZE               (PROCESSDATAAREAHEADERFAULTOFFSET + PDAFAULTVECTORSIZE)

typedef LONG_POINTER                    ProcessStateBlock;
#define PROCESSSTATEBLOCKLINKOFFSET     (0)
#define PROCESSSTATEBLOCKFLAGSOFFSET    (1)
#define PROCESSSTATEBLOCKCONTEXTOFFSET  (2)
#define PROCESSSTATEBLOCKTIMEOUTOFFSET  (3)
#define PROCESSSTATEBLOCKMDSOFFSET      (4)
#define PROCESSSTATEBLOCKAVAILOFFSET    (5)
#define PROCESSSTATEBLOCKSTICKYOFFSET   (6)

#define PDA                             ((LONG_POINTER)mPDA)
typedef CARDINAL                        PsbIndex;
#define PsbNull                         ((PsbIndex)0)
#define StartPsb                        ((PsbIndex)((PROCESSDATAAREAHEADERSIZE + PROCESSSTATEBLOCKSIZE - 1) / PROCESSSTATEBLOCKSIZE))

typedef BYTE                            Priority;
typedef POINTER                         PsbHandle;
typedef UNSPECIFIED                     PsbLink;
typedef UNSPECIFIED                     PsbFlags;
typedef UNSPECIFIED                     Monitor;
typedef UNSPECIFIED                     Condition;
typedef LONG_POINTER                    QueueHandle;
typedef UNSPECIFIED                     Queue;
typedef CARDINAL                        Ticks;

#define FAULTVECTORSIZE                 (2)
#define FAULTVECTORQUEUEOFFSET          (0)
#define FAULTVECTORCONDITIONOFFSET      (1)

#define INTERRUPTVECTORSIZE             (2)
#define INTERRUPTVECTORCONDITIONOFFSET  (0)
#define INTERRUPTVECTORAVAILABLEOFFEST  (1)

#define WdcMax                          ((CARDINAL)cWDC)

/*******************************************************************************
** PROCESS FUNCTIONS */
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderReadyOffset(ProcessDataAreaHeader pdah);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderCountOffset(ProcessDataAreaHeader pdah);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderUnusedOffset(ProcessDataAreaHeader pdah);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderAvailableOffset(ProcessDataAreaHeader pdah);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderStateOffset(ProcessDataAreaHeader pdah);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderStateVector(ProcessDataAreaHeader pdah, Priority pri);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderInterruptOffset(ProcessDataAreaHeader pdah);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderInterruptVector(ProcessDataAreaHeader pdah, InterruptLevel level);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderInterruptVectorCondition(ProcessDataAreaHeader pdah, InterruptLevel level);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderInterruptVectorAvailable(ProcessDataAreaHeader pdah, InterruptLevel level);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderFaultOffset(ProcessDataAreaHeader pdah);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderFaultVector(ProcessDataAreaHeader pdah, FaultIndex index);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderFaultVectorQueue(ProcessDataAreaHeader pdah, FaultIndex index);
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderFaultVectorCondition(ProcessDataAreaHeader pdah, FaultIndex index);
__mpinline LONG_POINTER MP_GetProcessStateBlockLinkOffset(ProcessStateBlock psb);
__mpinline LONG_POINTER MP_GetProcessStateBlockFlagsOffset(ProcessStateBlock psb);
__mpinline LONG_POINTER MP_GetProcessStateBlockContextOffset(ProcessStateBlock psb);
__mpinline LONG_POINTER MP_GetProcessStateBlockTimeoutOffset(ProcessStateBlock psb);
__mpinline LONG_POINTER MP_GetProcessStateBlockMdsOffset(ProcessStateBlock psb);
__mpinline LONG_POINTER MP_GetProcessStateBlockAvailableOffset(ProcessStateBlock psb);
__mpinline LONG_POINTER MP_GetProcessStateBlockStickyOffset(ProcessStateBlock psb);
__mpinline LONG_POINTER MP_LengthenPdaPtr(POINTER ptr);
__mpinline POINTER MP_OffsetPda(LONG_POINTER ptr);
__mpinline LONG_POINTER MP_FetchPda(POINTER ptr);
__mpinline LONG_POINTER MP_StorePda(POINTER ptr);
__mpinline Priority MP_GetPsbLinkPriority(PsbLink link);
__mpinline PsbIndex MP_GetPsbLinkNext(PsbLink link);
__mpinline BOOLEAN MP_GetPsbLinkFailed(PsbLink link);
__mpinline BOOLEAN MP_GetPsbLinkPermanent(PsbLink link);
__mpinline BOOLEAN MP_GetPsbLinkPreempted(PsbLink link);
__mpinline void MP_SetPsbLinkPriority(PsbLink * pLink, Priority priority);
__mpinline void MP_SetPsbLinkNext(PsbLink * pLink, PsbIndex index);
__mpinline void MP_SetPsbLinkFailed(PsbLink * pLink, BOOLEAN failed);
__mpinline void MP_SetPsbLinkPermanent(PsbLink * pLink, BOOLEAN permanent);
__mpinline void MP_SetPsbLinkPreempted(PsbLink * pLink, BOOLEAN preempted);
__mpinline BYTE MP_GetPsbFlagsAvailable(PsbFlags flags);
__mpinline PsbIndex MP_GetPsbFlagsCleanup(PsbFlags flags);
__mpinline BOOLEAN MP_GetPsbFlagsReserved(PsbFlags flags);
__mpinline BOOLEAN MP_GetPsbFlagsWaiting(PsbFlags flags);
__mpinline BOOLEAN MP_GetPsbFlagsAbort(PsbFlags flags);
__mpinline void MP_SetPsbFlagsAvailable(PsbFlags * pFlags, BYTE available);
__mpinline void MP_SetPsbFlagsCleanup(PsbFlags * pFlags, PsbIndex cleanup);
__mpinline void MP_SetPsbFlagsReserved(PsbFlags * pFlags, BOOLEAN reserved);
__mpinline void MP_SetPsbFlagsWaiting(PsbFlags * pFlags, BOOLEAN waiting);
__mpinline void MP_SetPsbFlagsAbort(PsbFlags * pFlags, BOOLEAN abort);
__mpinline BYTE MP_GetMonitorReserved(Monitor monitor);
__mpinline PsbIndex MP_GetMonitorTail(Monitor monitor);
__mpinline BYTE MP_GetMonitorAvailable(Monitor monitor);
__mpinline BOOLEAN MP_GetMonitorLocked(Monitor monitor);
__mpinline void MP_SetMonitorReserved(Monitor * pMonitor, BYTE available);
__mpinline void MP_SetMonitorTail(Monitor * pMonitor, PsbIndex tail);
__mpinline void MP_SetMonitorAvailable(Monitor * pMonitor, BYTE available);
__mpinline void MP_SetMonitorLocked(Monitor * pMonitor, BOOLEAN locked);
__mpinline BYTE MP_GetConditionReserved(Condition condition);
__mpinline PsbIndex MP_GetConditionTail(Condition condition);
__mpinline BOOLEAN MP_GetConditionAvailable(Condition condition);
__mpinline BOOLEAN MP_GetConditionAbortable(Condition condition);
__mpinline BOOLEAN MP_GetConditionWakeup(Condition condition);
__mpinline void MP_SetConditionReserved(Condition * pCondition, BYTE reserved);
__mpinline void MP_SetConditionTail(Condition * pCondition, PsbIndex tail);
__mpinline void MP_SetConditionAvailable(Condition * pCondition, BOOLEAN available);
__mpinline void MP_SetConditionAbortable(Condition * pCondition, BOOLEAN abortable);
__mpinline void MP_SetConditionWakeup(Condition * pCondition, BOOLEAN wakeup);
__mpinline BYTE MP_GetQueueReserved1(Queue queue);
__mpinline PsbIndex MP_GetQueueTail(Queue queue);
__mpinline BYTE MP_GetQueueReserved2(Queue queue);
__mpinline void MP_SetQueueReserved1(Queue * pQueue, BYTE reserved1);
__mpinline void MP_SetQueueTail(Queue * pQueue, PsbIndex tail);
__mpinline void MP_SetQueueReserved2(Queue * pQueue, BYTE reserved2);
__mpinline LONG_POINTER GetProcessDataAreaBlockOffset(LONG_POINTER blockNum);
__mpinline void MP_Dequeue(LONG_POINTER src, PsbIndex psb);
__mpinline void MP_Enqueue(LONG_POINTER dst, PsbIndex psb);
__mpinline void MP_CleanupCondition(LONG_POINTER c);
__mpinline void MP_Requeue(LONG_POINTER src, LONG_POINTER dst, PsbIndex psb);
__mpinline void MP_EnterFailed(LONG_POINTER m);
__mpinline BOOLEAN MP_Exit(LONG_POINTER m);
__mpinline void MP_WakeHead(LONG_POINTER c);
__mpinline StateHandle MP_AllocState(Priority pri);
__mpinline void MP_FreeState(Priority pri, StateHandle state);
__mpinline void MP_SaveProcess(BOOLEAN preemption);
__mpinline LocalFrameHandle MP_LoadProcess();
__mpinline BOOLEAN MP_EmptyState(Priority pri);
__mpinline void MP_Reschedule(BOOLEAN preemption);
__mpinline BOOLEAN MP_NotifyWakeup(LONG_POINTER c);
__mpinline PsbIndex MP_Fault(FaultIndex fi);
__mpinline void MP_FaultOne(FaultIndex fi, UNSPECIFIED parameter);
__mpinline void MP_FaultTwo(FaultIndex fi, LONG_UNSPECIFIED parameter);
__mpinline void MP_FrameFault(FSIndex fsi);
__mpinline void MP_PageFault(LONG_POINTER ptr);
__mpinline void MP_WriteProtectFault(LONG_POINTER page);
__mpinline BOOLEAN MP_InterruptPending();
__mpinline BOOLEAN MP_Interrupt();
__mpinline BOOLEAN MP_CheckForInterrupts();
__mpinline BOOLEAN MP_InterruptsEnabled();
__mpinline void MP_DisableInterrupts();
__mpinline void MP_EnableInterrupts();
__mpinline BOOLEAN MP_TimeoutScan();
__mpinline BOOLEAN MP_CheckForTimeouts();

/*******************************************************************************
** PROCESS INSTRUCTIONS */

/* Process Instructions */
void MP_Instruction_ME();       /* Monitor Entry */
void MP_Instruction_MX();       /* Monitor Exit */
void MP_Instruction_MW();       /* Monitor Wait */
void MP_Instruction_MR();       /* Monitor Reentry */
void MP_Instruction_NC();       /* Notify Condition */
void MP_Instruction_BC();       /* Broadcast Condition */
void MP_Instruction_REQ();      /* Requeue */
void MP_Instruction_SPP();      /* Set Process Priority */

/* Scheduling */
void MP_Instruction_DI();       /* Disable Interrupts */
void MP_Instruction_EI();       /* Enable Interrupts */

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_PROCESSES_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

