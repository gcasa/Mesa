/*
    MP_Processes.c - implementation of Mesa processor process instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include <PrincOps/PrincOps_Microcode.h>

/*******************************************************************************
** PROCESS INSTRUCTIONS */

/* Process Instructions */
void
MP_Instruction_ME()
{
    /* Monitor Entry */
    LONG_POINTER m = MP_PopLong();
    Monitor mon;
    MP_MinimalStack();
    mon = MP_ReadMesaRealAddress(MP_Fetch(m));
    if (! MP_GetMonitorLocked(mon))
    {
        MP_SetMonitorLocked(&mon, MESA_TRUE);
        MP_WriteMesaRealAddress(MP_Store(m), mon);
        MP_Push(MESA_TRUE);
    }
    else MP_EnterFailed(m);
}

void
MP_Instruction_MX()
{
    /* Monitor Exit */
    LONG_POINTER m = MP_PopLong();
    MP_MinimalStack();
    if (MP_Exit(m)) MP_Reschedule(MESA_FALSE);    
}

void
MP_Instruction_MW()
{
    /* Monitor Wait */
    Ticks t = MP_Pop();
    LONG_POINTER c = MP_PopLong();
    LONG_POINTER m = MP_PopLong();
    PsbFlags flags;
    Condition cond;
    BOOLEAN requeue;
    MP_MinimalStack();
    MP_CleanupCondition(c);
    requeue = MP_Exit(m);
    flags = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))));
    cond = MP_ReadMesaRealAddress(MP_Fetch(c));
    if ((! MP_GetPsbFlagsAbort(flags)) || (! MP_GetConditionAbortable(cond)))
    {
        if (MP_GetConditionWakeup(cond))
        {
            MP_SetConditionWakeup(&cond, MESA_FALSE);
            MP_WriteMesaRealAddress(MP_Store(c), cond);
        }
        else
        {
            Ticks newTicks = 0;
            if (t > 0)
            {
                newTicks = MP_LowHalf(((LONG)MP_Mesa_Processor.registers.PTC) + ((LONG)t));
                if (newTicks < 1) newTicks = 1;
            }
            MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockTimeoutOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), newTicks);
            MP_SetPsbFlagsWaiting(&flags, MESA_TRUE);
            MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), flags);
            MP_Requeue(MP_GetProcessDataAreaHeaderReadyOffset(PDA), c, MP_Mesa_Processor.registers.PSB);
            requeue = MESA_TRUE;
        }
    }
    if (requeue) MP_Reschedule(MESA_FALSE);
}

void
MP_Instruction_MR()
{
    /* Monitor Reentry */
    LONG_POINTER c = MP_PopLong();
    LONG_POINTER m = MP_PopLong();
    Monitor mon;
    MP_MinimalStack();
    mon = MP_ReadMesaRealAddress(MP_Fetch(m));
    if (! MP_GetMonitorLocked(mon))
    {
        PsbFlags flags;
        MP_CleanupCondition(c);
        flags = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))));
        MP_SetPsbFlagsCleanup(&flags, PsbNull);
        MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockFlagsOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), flags);
        if (MP_GetPsbFlagsAbort(flags))
        {
            Condition cond = MP_ReadMesaRealAddress(MP_Fetch(c));
            if (MP_GetConditionAbortable(cond)) MP_ProcessTrap();
        }

        MP_SetMonitorLocked(&mon, MESA_TRUE);
        MP_WriteMesaRealAddress(MP_Store(m), mon);
        MP_Push(MESA_TRUE);
   }
   else MP_EnterFailed(m);
}

void
MP_Instruction_NC()
{
    /* Notify Condition */
    LONG_POINTER c = MP_PopLong();
    Condition cond;
    MP_MinimalStack();
    MP_CleanupCondition(c);
    cond = MP_ReadMesaRealAddress(MP_Fetch(c));
    if (MP_GetConditionTail(cond) != PsbNull)
    {
        MP_WakeHead(c);
        MP_Reschedule(MESA_FALSE);
    }
}

void
MP_Instruction_BC()
{
    /* Broadcast Condition */
    LONG_POINTER c = MP_PopLong();
    BOOLEAN requeue = MESA_FALSE;
    Condition cond;
    MP_MinimalStack();
    MP_CleanupCondition(c);
    cond = MP_ReadMesaRealAddress(MP_Fetch(c));
    while (MP_GetConditionTail(cond) != PsbNull)
    {
        MP_WakeHead(c);
        requeue = MESA_TRUE;
        cond = MP_ReadMesaRealAddress(MP_Fetch(c));
    }
    if (requeue) MP_Reschedule(MESA_FALSE);
}

void
MP_Instruction_REQ()
{
    /* Requeue */
    PsbHandle psb = MP_Pop();
    QueueHandle dstque = MP_PopLong();
    QueueHandle srcque = MP_PopLong();
    MP_MinimalStack();
    MP_Requeue(srcque, dstque, MP_Index(psb));
    MP_Reschedule(MESA_FALSE);
}

void
MP_Instruction_SPP()
{
    /* Set Process Priority */
    Priority priority = (Priority)MP_Pop();
    PsbLink link;
    MP_MinimalStack();
    link = MP_ReadMesaRealAddress(MP_Fetch(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))));
    MP_SetPsbLinkPriority(&link, priority);
    MP_WriteMesaRealAddress(MP_Store(MP_GetProcessStateBlockLinkOffset(GetProcessDataAreaBlockOffset(MP_Mesa_Processor.registers.PSB))), link);
    MP_Requeue(MP_GetProcessDataAreaHeaderReadyOffset(PDA), MP_GetProcessDataAreaHeaderReadyOffset(PDA), MP_Mesa_Processor.registers.PSB);
    MP_Reschedule(MESA_FALSE);
}

/* Scheduling */
void
MP_Instruction_DI()
{
    /* Disable Interrupts */
    if (MP_Mesa_Processor.registers.WDC == WdcMax) MP_InterruptError();
    MP_DisableInterrupts();
}

void
MP_Instruction_EI()
{
    /* Enable Interrupts */
    if (MP_Mesa_Processor.registers.WDC == 0) MP_InterruptError();
    MP_EnableInterrupts();
}

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
