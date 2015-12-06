/*
    MP_JumpInstructions.h - internal header file of Mesa Processor Jump instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_JUMPINSTRUCTIONS_H
#define _INCLUDE_PRINCOPS_MP_MP_JUMPINSTRUCTIONS_H

/*******************************************************************************
** JUMP FUNCTIONS */
__mpinline void MP_JumpN(CARDINAL n);
__mpinline void MP_JumpZeroN(CARDINAL n);
__mpinline void MP_JumpNotZeroN(CARDINAL n);

/*******************************************************************************
** JUMP INSTRUCTIONS */

/* Unconditional Jumps */
void MP_Instruction_J2();      /* Jump 2 */
void MP_Instruction_J3();      /* Jump 3 */
void MP_Instruction_J4();      /* Jump 4 */
void MP_Instruction_J5();      /* Jump 5 */
void MP_Instruction_J6();      /* Jump 6 */
void MP_Instruction_J7();      /* Jump 7 */
void MP_Instruction_J8();      /* Jump 8 */
void MP_Instruction_JB();      /* Jump Byte */
void MP_Instruction_JW();      /* Jump Word */
void MP_Instruction_JS();      /* Jump Stack */
void MP_Instruction_CATCH();   /* Catch */

/* Equality Jumps */
void MP_Instruction_JZ3();      /* Jump Zero 3 */
void MP_Instruction_JZ4();      /* Jump Zero 4 */
void MP_Instruction_JNZ3();     /* Jump Not Zero 3 */
void MP_Instruction_JNZ4();     /* Jump Not Zero 4 */
void MP_Instruction_JZB();      /* Jump Zero Byte */
void MP_Instruction_JNZB();     /* Jump Not Zero Byte */
void MP_Instruction_JEB();      /* Jump Equal Byte */
void MP_Instruction_JNEB();     /* Jump Not Equal Byte */
void MP_Instruction_JDEB();     /* Jump Double Equal Byte */
void MP_Instruction_JDNEB();    /* Jump Double Not Equal Byte */
void MP_Instruction_JEP();      /* Jump Equal Pair */
void MP_Instruction_JNEP();     /* Jump Not Equal Pair */
void MP_Instruction_JEBB();     /* Jump Equal Byte Byte */
void MP_Instruction_JNEBB();    /* Jump Not Equal Byte Byte */

/* Signed Jumps */
void MP_Instruction_JLB();      /* Jump Less Byte */
void MP_Instruction_JLEB();     /* Jump Less Equal Byte */
void MP_Instruction_JGB();      /* Jump Greater Byte */
void MP_Instruction_JGEB();     /* Jump Greater Equal Byte */

/* Unsigned Jumps */
void MP_Instruction_JULB();     /* Jump Unsigned Less Byte */
void MP_Instruction_JULEB();    /* Jump Unsigned Less Equal Byte */
void MP_Instruction_JUGB();     /* Jump Unsigned Greater Byte */
void MP_Instruction_JUGEB();    /* Jump Unsigned Greater Equal Byte */

/* Indexed Jumps */
void MP_Instruction_JIB();      /* Jump Indexed Byte */
void MP_Instruction_JIW();      /* Jump Indexed Word */

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_JUMPINSTRUCTIONS_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

