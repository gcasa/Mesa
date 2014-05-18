/*
    MP_BlockTransfers.h - internal header file of Mesa Processor block transfer instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_BLOCKTRANSFERS_H
#define _INCLUDE_PRINCOPS_MP_MP_BLOCKTRANSFERS_H

/*******************************************************************************
** BLOCK TRANSFER TYPES */

/*******************************************************************************
** BLOCK TRANSFER FUNCTIONS */
__mpinline CARDINAL MP_Checksum(CARDINAL cksum, CARDINAL data);

/*******************************************************************************
** BLOCK TRANSFER INSTRUCTIONS */

/* Word Boundary Block Transfers */
void MP_Instruction_BLT();      /* Block Transfer */
void MP_Instruction_BLTL();     /* Block Transfer Long */
void MP_Instruction_BLTLR();    /* Block Transfer Long Reversed */
void MP_Instruction_BLTC();     /* Block Transfer Code */
void MP_Instruction_BLTCL();    /* Block Transfer Code Long */
void MP_Instruction_CKSUM();    /* Checksum */

/* Block Comparisons */
void MP_Instruction_BLEL();     /* Block Equal Long */
void MP_Instruction_BLECL();    /* Block Equal Code Long */

/* Byte Boundary Block Transfers */
void MP_Instruction_BYTBLT();   /* Byte Block Transfer */
void MP_Instruction_BYTBLTR();  /* Byte Block Transfer Reversed */

/* Bit Boundary Block Transfers */
void MP_Instruction_BITBLT();   /* Bit Block Transfer */
void MP_Instruction_TXTBLT();   /* Text Block Transfer */

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_BLOCKTRANSFERS_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

