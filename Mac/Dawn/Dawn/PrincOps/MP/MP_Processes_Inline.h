/*
    MP_Processes_Inline.h - internal header file of Mesa processor process inline functions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_PROCESSES_INLINE_H
#define _INCLUDE_PRINCOPS_MP_MP_PROCESSES_INLINE_H

/*******************************************************************************
** PROCESS INLINE FUNCTIONS */
__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderReadyOffset(ProcessDataAreaHeader pdah)
{
    return (pdah + PROCESSDATAAREAHEADERREADYOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderCountOffset(ProcessDataAreaHeader pdah)
{
    return (pdah + PROCESSDATAAREAHEADERCOUNTOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderUnusedOffset(ProcessDataAreaHeader pdah)
{
    return (pdah + PROCESSDATAAREAHEADERUNUSEDOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderAvailableOffset(ProcessDataAreaHeader pdah)
{
    return (pdah + PROCESSDATAAREAHEADERAVAILOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderStateOffset(ProcessDataAreaHeader pdah)
{
    return (pdah + PROCESSDATAAREAHEADERSTATEOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderStateVector(ProcessDataAreaHeader pdah, Priority pri)
{
    return (pdah + PROCESSDATAAREAHEADERSTATEOFFSET + ((LONG_POINTER)pri));
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderInterruptOffset(ProcessDataAreaHeader pdah)
{
    return (pdah + PROCESSDATAAREAHEADERINTERRUPTOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderInterruptVector(ProcessDataAreaHeader pdah, InterruptLevel level)
{
    return MP_GetProcessDataAreaHeaderInterruptOffset(pdah) + ((LONG_POINTER)(level * INTERRUPTVECTORSIZE));
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderInterruptVectorCondition(ProcessDataAreaHeader pdah, InterruptLevel level)
{
    return MP_GetProcessDataAreaHeaderInterruptVector(pdah, level) + INTERRUPTVECTORCONDITIONOFFSET;
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderInterruptVectorAvailable(ProcessDataAreaHeader pdah, InterruptLevel level)
{
    return MP_GetProcessDataAreaHeaderInterruptVector(pdah, level) + INTERRUPTVECTORAVAILABLEOFFEST;
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderFaultOffset(ProcessDataAreaHeader pdah)
{
    return (pdah + PROCESSDATAAREAHEADERFAULTOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderFaultVector(ProcessDataAreaHeader pdah, FaultIndex index)
{
    return MP_GetProcessDataAreaHeaderFaultOffset(pdah) + ((LONG_POINTER)(index * FAULTVECTORSIZE));
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderFaultVectorQueue(ProcessDataAreaHeader pdah, FaultIndex index)
{
    return MP_GetProcessDataAreaHeaderFaultVector(pdah, index) + FAULTVECTORQUEUEOFFSET;
}

__mpinline LONG_POINTER MP_GetProcessDataAreaHeaderFaultVectorCondition(ProcessDataAreaHeader pdah, FaultIndex index)
{
    return MP_GetProcessDataAreaHeaderFaultVector(pdah, index) + FAULTVECTORCONDITIONOFFSET;
}

__mpinline LONG_POINTER MP_GetProcessStateBlockLinkOffset(ProcessStateBlock psb)
{
    return (psb + PROCESSSTATEBLOCKLINKOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessStateBlockFlagsOffset(ProcessStateBlock psb)
{
    return (psb + PROCESSSTATEBLOCKFLAGSOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessStateBlockContextOffset(ProcessStateBlock psb)
{
    return (psb + PROCESSSTATEBLOCKCONTEXTOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessStateBlockTimeoutOffset(ProcessStateBlock psb)
{
    return (psb + PROCESSSTATEBLOCKTIMEOUTOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessStateBlockMdsOffset(ProcessStateBlock psb)
{
    return (psb + PROCESSSTATEBLOCKMDSOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessStateBlockAvailableOffset(ProcessStateBlock psb)
{
    return (psb + PROCESSSTATEBLOCKAVAILOFFSET);
}

__mpinline LONG_POINTER MP_GetProcessStateBlockStickyOffset(ProcessStateBlock psb)
{
    return (psb + PROCESSSTATEBLOCKSTICKYOFFSET);
}

__mpinline LONG_POINTER MP_LengthenPdaPtr(POINTER ptr)
{
    return (PDA + ((LONG_POINTER)ptr));
}

__mpinline POINTER MP_OffsetPda(LONG_POINTER ptr)
{
    if (MP_HighHalf(ptr - PDA) != 0) MP_Error();
    return MP_LowHalf(ptr - PDA);
}

__mpinline LONG_POINTER MP_FetchPda(POINTER ptr)
{
    return MP_Fetch(MP_LengthenPdaPtr(ptr));
}

__mpinline LONG_POINTER MP_StorePda(POINTER ptr)
{
    return MP_Store(MP_LengthenPdaPtr(ptr));
}

__mpinline Priority MP_GetPsbLinkPriority(PsbLink link)
{
    return (Priority)(link >> 13);
}

__mpinline PsbIndex MP_GetPsbLinkNext(PsbLink link)
{
    return (PsbIndex)((link >> 3) & 0x3FF);
}

__mpinline BOOLEAN MP_GetPsbLinkFailed(PsbLink link)
{
    return ((link & 4) ? MESA_TRUE : MESA_FALSE);
}

__mpinline BOOLEAN MP_GetPsbLinkPermanent(PsbLink link)
{
    return ((link & 2) ? MESA_TRUE : MESA_FALSE);
}

__mpinline BOOLEAN MP_GetPsbLinkPreempted(PsbLink link)
{
    return ((link & 1) ? MESA_TRUE : MESA_FALSE);
}

__mpinline void MP_SetPsbLinkPriority(PsbLink * pLink, Priority priority)
{
    *pLink = ((*pLink & 0x1FFF) | (((UNSPECIFIED)priority) << 13));
}

__mpinline void MP_SetPsbLinkNext(PsbLink * pLink, PsbIndex index)
{
    *pLink = ((*pLink & 0xE007) | (((UNSPECIFIED)index) << 3));
}

__mpinline void MP_SetPsbLinkFailed(PsbLink * pLink, BOOLEAN failed)
{
    *pLink = ((*pLink & 0xFFFB) | (failed ? 4 : 0));
}

__mpinline void MP_SetPsbLinkPermanent(PsbLink * pLink, BOOLEAN permanent)
{
    *pLink = ((*pLink & 0xFFFD) | (permanent ? 2 : 0));
}

__mpinline void MP_SetPsbLinkPreempted(PsbLink * pLink, BOOLEAN preempted)
{
    *pLink = ((*pLink & 0xFFFE) | (preempted ? 1 : 0));
}

__mpinline BYTE MP_GetPsbFlagsAvailable(PsbFlags flags)
{
    return (BYTE)(flags >> 13);
}

__mpinline PsbIndex MP_GetPsbFlagsCleanup(PsbFlags flags)
{
    return (PsbIndex)((flags >> 3) & 0x3FF);
}

__mpinline BOOLEAN MP_GetPsbFlagsReserved(PsbFlags flags)
{
    return ((flags & 4) ? MESA_TRUE : MESA_FALSE);
}

__mpinline BOOLEAN MP_GetPsbFlagsWaiting(PsbFlags flags)
{
    return ((flags & 2) ? MESA_TRUE : MESA_FALSE);
}

__mpinline BOOLEAN MP_GetPsbFlagsAbort(PsbFlags flags)
{
    return ((flags & 1) ? MESA_TRUE : MESA_FALSE);
}

__mpinline void MP_SetPsbFlagsAvailable(PsbFlags * pFlags, BYTE available)
{
    *pFlags = ((*pFlags & 0x1FFF) | (((UNSPECIFIED)available) << 13));
}

__mpinline void MP_SetPsbFlagsCleanup(PsbFlags * pFlags, PsbIndex cleanup)
{
    *pFlags = ((*pFlags & 0xE007) | (((UNSPECIFIED)cleanup) << 3));
}

__mpinline void MP_SetPsbFlagsReserved(PsbFlags * pFlags, BOOLEAN reserved)
{
    *pFlags = ((*pFlags & 0xFFFB) | (reserved ? 4 : 0));
}

__mpinline void MP_SetPsbFlagsWaiting(PsbFlags * pFlags, BOOLEAN waiting)
{
    *pFlags = ((*pFlags & 0xFFFD) | (waiting ? 2 : 0));
}

__mpinline void MP_SetPsbFlagsAbort(PsbFlags * pFlags, BOOLEAN abort)
{
    *pFlags = ((*pFlags & 0xFFFE) | (abort ? 1 : 0));
}

__mpinline BYTE MP_GetMonitorReserved(Monitor monitor)
{
    return (BYTE)(monitor >> 13);
}

__mpinline PsbIndex MP_GetMonitorTail(Monitor monitor)
{
    return (PsbIndex)((monitor >> 3) & 0x3FF);
}

__mpinline BYTE MP_GetMonitorAvailable(Monitor monitor)
{
    return ((BYTE)((monitor >> 1) & 3));
}

__mpinline BOOLEAN MP_GetMonitorLocked(Monitor monitor)
{
    return ((monitor & 1) ? MESA_TRUE : MESA_FALSE);
}

__mpinline void MP_SetMonitorReserved(Monitor * pMonitor, BYTE available)
{
    *pMonitor = ((*pMonitor & 0x1FFF) | (((UNSPECIFIED)available) << 13));
}

__mpinline void MP_SetMonitorTail(Monitor * pMonitor, PsbIndex tail)
{
    *pMonitor = ((*pMonitor & 0xE007) | (((UNSPECIFIED)tail) << 3));
}

__mpinline void MP_SetMonitorAvailable(Monitor * pMonitor, BYTE available)
{
    *pMonitor = ((*pMonitor & 0xFFF9) | ((((UNSPECIFIED)available) & 3) << 1));
}

__mpinline void MP_SetMonitorLocked(Monitor * pMonitor, BOOLEAN locked)
{
    *pMonitor = ((*pMonitor & 0xFFFE) | (locked ? 1 : 0));
}

__mpinline BYTE MP_GetConditionReserved(Condition condition)
{
    return (BYTE)(condition >> 13);
}

__mpinline PsbIndex MP_GetConditionTail(Condition condition)
{
    return (PsbIndex)((condition >> 3) & 0x3FF);
}

__mpinline BOOLEAN MP_GetConditionAvailable(Condition condition)
{
    return ((condition & 4) ? MESA_TRUE : MESA_FALSE);
}

__mpinline BOOLEAN MP_GetConditionAbortable(Condition condition)
{
    return ((condition & 2) ? MESA_TRUE : MESA_FALSE);
}

__mpinline BOOLEAN MP_GetConditionWakeup(Condition condition)
{
    return ((condition & 1) ? MESA_TRUE : MESA_FALSE);
}

__mpinline void MP_SetConditionReserved(Condition * pCondition, BYTE reserved)
{
    *pCondition = ((*pCondition & 0x1FFF) | (((UNSPECIFIED)reserved) << 13));
}

__mpinline void MP_SetConditionTail(Condition * pCondition, PsbIndex tail)
{
    *pCondition = ((*pCondition & 0xE007) | (((UNSPECIFIED)tail) << 3));
}

__mpinline void MP_SetConditionAvailable(Condition * pCondition, BOOLEAN available)
{
    *pCondition = ((*pCondition & 0xFFFB) | (available ? 4 : 0));
}

__mpinline void MP_SetConditionAbortable(Condition * pCondition, BOOLEAN abortable)
{
    *pCondition = ((*pCondition & 0xFFFD) | (abortable ? 2 : 0));
}

__mpinline void MP_SetConditionWakeup(Condition * pCondition, BOOLEAN wakeup)
{
    *pCondition = ((*pCondition & 0xFFFE) | (wakeup ? 1 : 0));
}

__mpinline BYTE MP_GetQueueReserved1(Queue queue)
{
    return (BYTE)(queue >> 13);
}

__mpinline PsbIndex MP_GetQueueTail(Queue queue)
{
    return (PsbIndex)((queue >> 3) & 0x3FF);
}

__mpinline BYTE MP_GetQueueReserved2(Queue queue)
{
    return (BYTE)(queue & 7);
}

__mpinline void MP_SetQueueReserved1(Queue * pQueue, BYTE reserved1)
{
    *pQueue = ((*pQueue & 0x1FFF) | (((UNSPECIFIED)reserved1) << 13));
}

__mpinline void MP_SetQueueTail(Queue * pQueue, PsbIndex tail)
{
    *pQueue = ((*pQueue & 0xE007) | (((UNSPECIFIED)tail) << 3));
}

__mpinline void MP_SetQueueReserved2(Queue * pQueue, BYTE reserved2)
{
    *pQueue = ((*pQueue & 0xFFF8) | (((UNSPECIFIED)reserved2) & 7));
}

__mpinline LONG_POINTER GetProcessDataAreaBlockOffset(LONG_POINTER blockNum)
{
    return (PDA + (blockNum * PROCESSSTATEBLOCKSIZE));
}

__mpinline void MP_Dequeue(LONG_POINTER src, PsbIndex psb)
{
    PsbLink link;
    PsbIndex prev;
    Queue queue = 0;
    QueueHandle que = src;
    if (que != 0) queue = MP_ReadMesaRealAddress(MP_Fetch(que));
    link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(psb))));
    
    if (MP_GetPsbLinkNext(link) == psb) prev = PsbNull;
    else
    {
        PsbLink temp;
        prev = ((que == 0) ? psb : MP_GetQueueTail(queue));
        temp = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(prev))));
        while (MP_GetPsbLinkNext(temp) != psb)
        {
            prev = MP_GetPsbLinkNext(temp);
            temp = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(prev))));
        }
        MP_SetPsbLinkNext(&temp, MP_GetPsbLinkNext(link));
        MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(prev))), temp);
    }

    if (que == 0)
    {
        PsbFlags flags = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(psb))));
        MP_SetPsbFlagsCleanup(&flags, MP_GetPsbLinkNext(link));
        MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(psb))), flags);
    }
    else if (MP_GetQueueTail(queue) == psb)
    {
        MP_SetQueueTail(&queue, prev);
        MP_WriteMesaRealAddress(MP_Store(que), queue);
    }
}

__mpinline void MP_Enqueue(LONG_POINTER dst, PsbIndex psb)
{
    QueueHandle que = dst;
    Queue queue = MP_ReadMesaRealAddress(MP_Fetch(que));
    PsbLink link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(psb))));
    if (MP_GetQueueTail(queue) == PsbNull)
    {
        MP_SetPsbLinkNext(&link, psb);
        MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(psb))), link);
        MP_SetQueueTail(&queue, psb);
        MP_WriteMesaRealAddress(MP_Store(que), queue);
    }
    else
    {
        PsbLink currentlink;
        PsbLink nextlink;
        PsbIndex prev = MP_GetQueueTail(queue);
        currentlink = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(prev))));
        if (MP_GetPsbLinkPriority(currentlink) >= MP_GetPsbLinkPriority(link))
        {
            MP_SetQueueTail(&queue, psb);
            MP_WriteMesaRealAddress(MP_Store(que), queue);
        }
        else
        {
            nextlink = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_GetPsbLinkNext(currentlink)))));
            while (MP_GetPsbLinkPriority(link) <= MP_GetPsbLinkPriority(nextlink))
            {
                prev = MP_GetPsbLinkNext(currentlink);
                currentlink = nextlink;
                nextlink = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_GetPsbLinkNext(currentlink)))));
            }
        }
        MP_SetPsbLinkNext(&link, MP_GetPsbLinkNext(currentlink));
        MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(psb))), link);
        MP_SetPsbLinkNext(&currentlink, psb);
        MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(prev))), currentlink);
    }
}

__mpinline void MP_CleanupCondition(LONG_POINTER c)
{
    PsbLink link;
    PsbFlags flags;
    PsbIndex psb;
    PsbIndex head;
    Condition cond = MP_ReadMesaRealAddress(MP_Fetch(c));
    if (PsbNull != (psb = MP_GetConditionTail(cond)))
    {
        flags = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(psb))));
        if (MP_GetPsbFlagsCleanup(flags) != PsbNull)
        {
            while (MP_GetPsbFlagsCleanup(flags) != PsbNull)
            {
                if (MP_GetPsbFlagsCleanup(flags) == psb)
                {
                    MP_SetConditionWakeup(&cond, MESA_FALSE);
                    MP_SetConditionTail(&cond, PsbNull);
                    MP_WriteMesaRealAddress(MP_Store(c), cond);
                    return;
                }
                psb = MP_GetPsbFlagsCleanup(flags);
                flags = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(psb))));
            }
            head = psb;
            link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(psb))));
            while (MP_GetPsbLinkNext(link) != head)
            {
                psb = MP_GetPsbLinkNext(link);
                link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(psb))));
            }
            MP_SetConditionTail(&cond, psb);
            MP_WriteMesaRealAddress(MP_Store(c), cond);
        }
    }
}

__mpinline void MP_Requeue(LONG_POINTER src, LONG_POINTER dst, PsbIndex psb)
{
    if (psb == PsbNull) MP_Error();
    MP_Dequeue(src, psb);
    MP_Enqueue(dst, psb);
}

__mpinline void MP_EnterFailed(LONG_POINTER m)
{
    PsbLink link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))));
    MP_SetPsbLinkFailed(&link, MESA_TRUE);
    MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), link);
    MP_Requeue(MP_GetProcessDataAreaHeaderReadyOffset(PDA), m, MP_Mesa_Processor.registers.PSB);
    MP_Reschedule(MESA_FALSE);
}

__mpinline BOOLEAN MP_Exit(LONG_POINTER m)
{
    Monitor mon = MP_ReadMesaRealAddress(MP_Fetch(m));
    if (MESA_FALSE == MP_GetMonitorLocked(mon)) MP_Error();
    MP_SetMonitorLocked(&mon, MESA_FALSE);
    MP_WriteMesaRealAddress(MP_Store(m), mon);
    if (PsbNull != MP_GetMonitorTail(mon))
    {
        PsbLink link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_GetMonitorTail(mon)))));
        MP_Requeue(m, MP_GetProcessDataAreaHeaderReadyOffset(PDA), MP_GetPsbLinkNext(link));
        return MESA_TRUE;
    }
    else return MESA_FALSE;
}

__mpinline void MP_WakeHead(LONG_POINTER c)
{
    Condition cond = MP_ReadMesaRealAddress(MP_Fetch(c));
    PsbLink link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_GetConditionTail(cond)))));
    PsbFlags flags = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(MP_GetPsbLinkNext(link)))));
    MP_SetPsbFlagsWaiting(&flags, MESA_FALSE);
    MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(MP_GetPsbLinkNext(link)))), flags);
    MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockTimeoutOffset(GetProcessDataAreaBlockOffset(MP_GetPsbLinkNext(link)))), 0);
    MP_Requeue(c, MP_GetProcessDataAreaHeaderReadyOffset(PDA), MP_GetPsbLinkNext(link));
}

__mpinline StateHandle MP_AllocState(Priority pri)
{
    StateHandle state;
    POINTER offset = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessDataAreaHeaderStateVector(PDA, pri)));
    if (offset == 0) MP_Error();
    state = MP_LengthenPdaPtr(offset);
    MP_WriteMesaRealAddress(MP_Store(MP_GetProcessDataAreaHeaderStateVector(PDA, pri)), MP_ReadMesaRealAddress(MP_Fetch(state)));
    return state;
}

__mpinline void MP_FreeState(Priority pri, StateHandle state)
{
    MP_WriteMesaRealAddress(MP_Store(state), MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessDataAreaHeaderStateVector(PDA, pri))));
    MP_WriteMesaRealAddress(MP_Store(MP_GetProcessDataAreaHeaderStateVector(PDA, pri)), MP_OffsetPda(state));
}

__mpinline void MP_SaveProcess(BOOLEAN preemption)
{
    PsbLink link;
    
    MP_Mesa_Processor.state.terminate = MESA_TRUE;   /* Equavalent to BEGIN ENABLE ABORT => ERROR */

    link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))));
    if (MP_ValidContext()) MP_WriteMesaRealAddress(MP_StoreMds(MP_LocalBaseProgramCounterOffset(MP_Mesa_Processor.registers.LF)), MP_Mesa_Processor.registers.PC);
    MP_SetPsbLinkPreempted(&link, preemption);
    if (preemption)
    {
        StateHandle state;
        if (! MP_GetPsbLinkPermanent(link)) state = MP_AllocState(MP_GetPsbLinkPriority(link));
        else state = MP_LengthenPdaPtr(MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockContextOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB)))));
        MP_SaveStack(state);
        MP_WriteMesaRealAddress(MP_Store(MP_GetStateHandleFrameOffset(state)), MP_Mesa_Processor.registers.LF);
        if (! MP_GetPsbLinkPermanent(link)) MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockContextOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), MP_OffsetPda(state));
    }
    else
    {
        if (! MP_GetPsbLinkPermanent(link)) MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockContextOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), MP_Mesa_Processor.registers.LF);
        else
        {
            StateHandle state = MP_LengthenPdaPtr(MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockContextOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB)))));
            MP_WriteMesaRealAddress(MP_Store(MP_GetStateHandleFrameOffset(state)), MP_Mesa_Processor.registers.LF);
        }
    }
    MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), link);
    
    MP_Mesa_Processor.state.terminate = MESA_FALSE;   /* Equavalent to END ENABLE */
}

__mpinline LocalFrameHandle MP_LoadProcess()
{
    LocalFrameHandle frame;
    CARDINAL mds;
    PsbLink link;

    MP_Mesa_Processor.state.terminate = MESA_TRUE;   /* Equavalent to BEGIN ENABLE ABORT => ERROR */
   
    link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))));
    frame = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockContextOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))));   
    if (MP_GetPsbLinkPreempted(link))
    {
        StateHandle state = MP_LengthenPdaPtr(frame);
        MP_LoadStack(state);
        frame = MP_ReadMesaRealAddress(MP_Fetch(MP_GetStateHandleFrameOffset(state)));
        if (! MP_GetPsbLinkPermanent(link)) MP_FreeState(MP_GetPsbLinkPriority(link), state);
    }
    else
    {
        if (MP_GetPsbLinkFailed(link))
        {
            MP_Push(MESA_FALSE);
            MP_SetPsbLinkFailed(&link, MESA_FALSE);
            MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), link);
        }
        if (MP_GetPsbLinkPermanent(link))
        {
            StateHandle state = MP_LengthenPdaPtr(frame);
            frame = MP_ReadMesaRealAddress(MP_Fetch(MP_GetStateHandleFrameOffset(state)));
        }
    }

    mds = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockMdsOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))));
    MP_Mesa_Processor.registers.MDS = MP_LongShift(((LONG_UNSPECIFIED)mds), WordSize);

    MP_Mesa_Processor.state.terminate = MESA_FALSE;   /* Equavalent to END ENABLE */
    
    return frame;
}

__mpinline BOOLEAN MP_EmptyState(Priority pri)
{
    POINTER state = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessDataAreaHeaderStateVector(PDA, pri)));
    return ((state == 0) ? MESA_TRUE : MESA_FALSE);
}

__mpinline void MP_Reschedule(BOOLEAN preemption)
{
    PsbLink link;
    PsbIndex psb;
    Queue queue;
    if (MP_Mesa_Processor.state.running) MP_SaveProcess(preemption);
    queue = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessDataAreaHeaderReadyOffset(PDA)));
    if (MP_GetQueueTail(queue) == PsbNull) goto BusyWait;
    link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_GetQueueTail(queue)))));
    while (1)
    {
        psb = MP_GetPsbLinkNext(link);
        link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(psb))));
        if (MP_GetPsbLinkPermanent(link) || MP_GetPsbLinkPreempted(link) || (! MP_EmptyState(MP_GetPsbLinkPriority(link)))) break;
        if (psb == MP_GetQueueTail(queue)) goto BusyWait;
    }
    
    MP_Mesa_Processor.registers.PSB = psb;
    MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC = 0;
    MP_Mesa_Processor.registers.LF = MP_LoadProcess();
    MP_Mesa_Processor.state.running = MESA_TRUE;
    MP_XFER((LONG_UNSPECIFIED)MP_Mesa_Processor.registers.LF, 0, XFERType_processSwitch, MESA_FALSE);
    
    return;
BusyWait:
    if (! MP_InterruptsEnabled()) MP_RescheduleError();
    MP_Mesa_Processor.state.running = MESA_FALSE;
}

__mpinline BOOLEAN MP_NotifyWakeup(LONG_POINTER c)
{
    Condition cond;
    BOOLEAN requeue = MESA_FALSE;
    MP_CleanupCondition(c);
    cond = MP_ReadMesaRealAddress(MP_Fetch(c));
    if (MP_GetConditionTail(cond) == PsbNull)
    {
        MP_SetConditionWakeup(&cond, MESA_TRUE);
        MP_WriteMesaRealAddress(MP_Store(c), cond);
    }
    else
    {
        MP_WakeHead(c);
        requeue = MESA_TRUE;
    }

    return requeue;
}

__mpinline PsbIndex MP_Fault(FaultIndex fi)
{
    PsbIndex faulted = MP_Mesa_Processor.registers.PSB;
    MP_Requeue(MP_GetProcessDataAreaHeaderReadyOffset(PDA), MP_GetProcessDataAreaHeaderFaultVectorQueue(PDA, fi), faulted);
    MP_NotifyWakeup(MP_GetProcessDataAreaHeaderFaultVectorCondition(PDA, fi));
    MP_Mesa_Processor.registers.PC = MP_Mesa_Processor.state.savedPC;
    MP_Mesa_Processor.registers.SP = MP_Mesa_Processor.state.savedSP;
    MP_Reschedule(MESA_TRUE);
    return faulted;
}

__mpinline void MP_FaultOne(FaultIndex fi, UNSPECIFIED parameter)
{
    PsbIndex psb = MP_Fault(fi);
    POINTER state = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockContextOffset(GetProcessDataAreaBlockOffset(psb))));
    MP_WriteMesaRealAddress(MP_StorePda((POINTER)MP_GetStateHandleDataZeroOffset(state)), parameter);
    MP_ErrorAbort();
}

__mpinline void MP_FaultTwo(FaultIndex fi, LONG_UNSPECIFIED parameter)
{
    PsbIndex psb = MP_Fault(fi);
    POINTER state = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockContextOffset(GetProcessDataAreaBlockOffset(psb))));
    MP_WriteMesaRealAddress(MP_StorePda((POINTER)MP_GetStateHandleDataZeroOffset(state)), MP_LowHalf(parameter));
    MP_WriteMesaRealAddress(MP_StorePda((POINTER)MP_GetStateHandleDataOneOffset(state)), MP_HighHalf(parameter));
    MP_ErrorAbort();
}

__mpinline void MP_FrameFault(FSIndex fsi)
{
    MP_FaultOne(qFrameFault, fsi);
}

__mpinline void MP_PageFault(LONG_POINTER ptr)
{
    MP_FaultTwo(qPageFault, ptr);
}

__mpinline void MP_WriteProtectFault(LONG_POINTER page)
{
    MP_FaultTwo(qWriteProtectFault, page);
}

__mpinline BOOLEAN MP_InterruptPending()
{
    MP_Mesa_Processor.registers.WP = (*MP_Mesa_Processor.config.m_pGetInterruptMaskFunction)();
    return (((MP_Mesa_Processor.registers.WP != 0) && MP_InterruptsEnabled()) ? MESA_TRUE : MESA_FALSE);
}

__mpinline BOOLEAN MP_Interrupt()
{
    UNSPECIFIED mask = 1;
    UNSPECIFIED wakeups;
    BOOLEAN requeue = MESA_FALSE;
    InterruptLevel level = WordSize;
    MP_Mesa_Processor.registers.WP = (*MP_Mesa_Processor.config.m_pGetInterruptMaskFunction)();
    wakeups = MP_Mesa_Processor.registers.WP;
    MP_Mesa_Processor.registers.WP = 0;
    (*MP_Mesa_Processor.config.m_pSetInterruptMaskFunction)(MP_Mesa_Processor.registers.WP);
    while (1)
    {
        level--;
        if (0 != MP_And(wakeups, mask))
        {
            if (MP_NotifyWakeup(MP_GetProcessDataAreaHeaderInterruptVectorCondition(PDA, level))) requeue = MESA_TRUE;
        }
        if (level == 0) break;
        mask = MP_Shift(mask, 1);
    }
    return requeue;
}

__mpinline BOOLEAN MP_CheckForInterrupts()
{
    if (MP_InterruptPending()) return MP_Interrupt();
    else return MESA_FALSE;
}

__mpinline BOOLEAN MP_InterruptsEnabled()
{
    return ((MP_Mesa_Processor.registers.WDC == 0) ? MESA_TRUE : MESA_FALSE);
}

__mpinline void MP_DisableInterrupts()
{
    MP_Mesa_Processor.registers.WDC++;
}

__mpinline void MP_EnableInterrupts()
{
    MP_Mesa_Processor.registers.WDC--;
}

__mpinline BOOLEAN MP_TimeoutScan()
{
    BOOLEAN requeue = MESA_FALSE;
    CARDINAL count = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessDataAreaHeaderCountOffset(PDA)));
    PsbIndex size = StartPsb + count;
    PsbIndex psb;
    for (psb = StartPsb; psb < size; psb++)
    {
        Ticks timeout = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockTimeoutOffset(GetProcessDataAreaBlockOffset(psb))));
        if ((timeout != 0) && (timeout == MP_Mesa_Processor.registers.PTC))
        {
            PsbFlags flags = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(psb))));
            MP_SetPsbFlagsWaiting(&flags, MESA_FALSE);
            MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(psb))), flags);
            MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockTimeoutOffset(GetProcessDataAreaBlockOffset(psb))), 0);
            MP_Requeue(0, MP_GetProcessDataAreaHeaderReadyOffset(PDA), psb);
            requeue = MESA_TRUE;
        }
    }
    return requeue;
}

__mpinline BOOLEAN MP_CheckForTimeouts()
{
    LONG_CARDINAL temp = (MP_Mesa_Processor.registers.IT = (*MP_Mesa_Processor.config.m_pGetPulseCountFunction)());
    if (MP_InterruptsEnabled() && ((temp - MP_Mesa_Processor.state.time) >= MP_Mesa_Processor.config.TimeOutInterval))
    {
        MP_Mesa_Processor.state.time = temp;
        MP_Mesa_Processor.registers.PTC++;
        if (0 == MP_Mesa_Processor.registers.PTC) MP_Mesa_Processor.registers.PTC++;
        return MP_TimeoutScan();
    }
    else return MESA_FALSE;
}

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_PROCESSES_INLINE_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

