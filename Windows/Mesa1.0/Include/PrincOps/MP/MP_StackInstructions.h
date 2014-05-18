/*
    MP_StackInstructions.h - internal header file of Mesa Processor Stack

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_STACKINSTRUCTIONS_H
#define _INCLUDE_PRINCOPS_MP_MP_STACKINSTRUCTIONS_H

/*******************************************************************************
** STACK TYPES AND CONSTANTS */
 
/*******************************************************************************
** STACK FUNCTIONS */
__mpinline void MP_Push(UNSPECIFIED data);
__mpinline UNSPECIFIED MP_Pop();
__mpinline void MP_PushLong(LONG_UNSPECIFIED data);
__mpinline LONG_UNSPECIFIED MP_PopLong();
__mpinline void MP_MinimalStack();
__mpinline void MP_Recover();
__mpinline void MP_Discard();

/*******************************************************************************
** STACK INSTRUCTIONS */
/* Stack Primitives */
void MP_Instruction_REC();      /* Recover */
void MP_Instruction_REC2();     /* Recover 2 */
void MP_Instruction_DIS();      /* Discard */
void MP_Instruction_DIS2();     /* Discard 2 */
void MP_Instruction_EXCH();     /* Exchange */
void MP_Instruction_DEXCH();    /* Double Exchange */
void MP_Instruction_DUP();      /* Duplicate */
void MP_Instruction_DDUP();     /* Double Duplicate */
void MP_Instruction_EXDIS();    /* Exchange Discard */

/* Checking Instructions */
void MP_Instruction_BNDCK();    /* Bounds Check */
void MP_Instruction_BNDCKL();   /* Bounds Check Long */
void MP_Instruction_NILCK();    /* Nil Check */
void MP_Instruction_NILCKL();   /* Nil Check Long */

/* Unary Operations */
void MP_Instruction_NEG();      /* Negate */
void MP_Instruction_INC();      /* Increment */
void MP_Instruction_DINC();     /* Double Increment */
void MP_Instruction_DEC();      /* Decrement */
void MP_Instruction_ADDSB();    /* Add Signed Byte */
void MP_Instruction_DBL();      /* Double */
void MP_Instruction_DDBL();     /* Double Double */
void MP_Instruction_TRPL();     /* Triple */
void MP_Instruction_LINT();     /* Lengthen Integer */
void MP_Instruction_SHIFTSB();  /* Shift Signed Byte */

/* Logical Operations */
void MP_Instruction_AND();      /* And */
void MP_Instruction_DAND();     /* Double And */
void MP_Instruction_IOR();      /* Inclusive Or */
void MP_Instruction_DIOR();     /* Double Inclusive Or */
void MP_Instruction_XOR();      /* Exclusive Or */
void MP_Instruction_DXOR();     /* Double Exclusive Or */
void MP_Instruction_SHIFT();    /* Shift */
void MP_Instruction_DSHIFT();   /* Double Shift */
void MP_Instruction_ROTATE();   /* Rotate */

/* Arithmetic Operations */
void MP_Instruction_ADD();      /* Add */
void MP_Instruction_SUB();      /* Subtract */
void MP_Instruction_DADD();     /* Double Add */
void MP_Instruction_DSUB();     /* Double Subtract */
void MP_Instruction_ADC();      /* Add Double to Cardinal */
void MP_Instruction_ACD();      /* Add Cardinal to Double */
void MP_Instruction_MUL();      /* Multiply */
void MP_Instruction_DMUL();     /* Double Multiply */
void MP_Instruction_SDIV();     /* Signed Divide */
void MP_Instruction_UDIV();     /* Unsigned Divide */
void MP_Instruction_LUDIV();    /* Long Unsigned Divide */
void MP_Instruction_SDDIV();    /* Signed Double Divide */
void MP_Instruction_UDDIV();    /* Unsigned Double Divide */

/* Comparison Operations */
void MP_Instruction_DCMP();     /* Double Compare */
void MP_Instruction_UDCMP();    /* Unsigned Double Compare */

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_STACKINSTRUCTIONS_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

