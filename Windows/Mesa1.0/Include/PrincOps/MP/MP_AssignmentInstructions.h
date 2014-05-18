/*
    MP_AssignmentInstructions.h - internal header file of Mesa Processor Assignment instructions

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_MP_MP_ASSIGNMENTINSTRUCTIONS_H
#define _INCLUDE_PRINCOPS_MP_MP_ASSIGNMENTINSTRUCTIONS_H

/*******************************************************************************
** ASSIGNMENT TYPES */
typedef BYTE            FieldSpec;
typedef UNSPECIFIED     FieldDesc;
extern  UNSPECIFIED     MP_MaskTable[];

/*******************************************************************************
** ASSIGNMENT FUNCTIONS */
__mpinline void MP_LoadImmediateN(CARDINAL n);
__mpinline void MP_LocalAddressN(CARDINAL n);
__mpinline void MP_LoadLocalN(CARDINAL n);
__mpinline void MP_LoadLocalDoubleN(CARDINAL n);
__mpinline void MP_StoreLocalN(CARDINAL n);
__mpinline void MP_StoreLocalDoubleN(CARDINAL n);
__mpinline void MP_PutLocalN(CARDINAL n);
__mpinline void MP_StoreLocalByte();
__mpinline void MP_StoreLocalDoubleByte();
__mpinline void MP_GlobalAddressN(CARDINAL n);
__mpinline void MP_LoadGlobalN(CARDINAL n);
__mpinline void MP_LoadGlobalDoubleN(CARDINAL n);
__mpinline void MP_ReadN(CARDINAL n);
__mpinline void MP_ReadLocalIndirectZeroN(CARDINAL n);
__mpinline BYTE MP_FetchByte(LONG_POINTER ptr, LONG_CARDINAL offset);
__mpinline void MP_StoreByte(LONG_POINTER ptr, LONG_CARDINAL offset, BYTE data);
__mpinline BYTE MP_GetFieldSpecPos(FieldSpec fieldSpec);
__mpinline BYTE MP_GetFieldSpecSize(FieldSpec fieldSpec);
__mpinline BYTE MP_GetFieldDescOffset(FieldDesc fieldDesc);
__mpinline FieldSpec MP_GetFieldDescFieldSpec(FieldDesc fieldDesc);
__mpinline UNSPECIFIED MP_ReadField(UNSPECIFIED source, FieldSpec spec);
__mpinline UNSPECIFIED MP_WriteField(UNSPECIFIED dest, FieldSpec spec, UNSPECIFIED data);
__mpinline void MP_WriteSwappedZeroField();

/*******************************************************************************
** ASSIGNMENT INSTRUCTIONS */

/* Immediate Instructions */
void MP_Instruction_LIN1();     /* Load Immediate Negative One */
void MP_Instruction_LINI();     /* Load Immediate Negative Infinity */
void MP_Instruction_LI00();     /* Load Immediate Double Zero */
void MP_Instruction_LI0();      /* Load Immediate 0 */
void MP_Instruction_LI1();      /* Load Immediate 1 */
void MP_Instruction_LI2();      /* Load Immediate 2 */
void MP_Instruction_LI3();      /* Load Immediate 3 */
void MP_Instruction_LI4();      /* Load Immediate 4 */
void MP_Instruction_LI5();      /* Load Immediate 5 */
void MP_Instruction_LI6();      /* Load Immediate 6 */
void MP_Instruction_LI7();      /* Load Immediate 7 */
void MP_Instruction_LI8();      /* Load Immediate 8 */
void MP_Instruction_LI9();      /* Load Immediate 9 */
void MP_Instruction_LI10();     /* Load Immediate 10 */
void MP_Instruction_LIB();      /* Load Immediate Byte */
void MP_Instruction_LINB();     /* Load Immediate Negative Byte */
void MP_Instruction_LIHB();     /* Load Immediate High Byte */
void MP_Instruction_LIW();      /* Load Immediate Word */

/* Frame Instructions */
void MP_Instruction_LA0();      /* Local Address 0 */
void MP_Instruction_LA1();      /* Local Address 1 */
void MP_Instruction_LA2();      /* Local Address 2 */
void MP_Instruction_LA3();      /* Local Address 3 */
void MP_Instruction_LA6();      /* Local Address 6 */
void MP_Instruction_LA8();      /* Local Address 8 */
void MP_Instruction_LAB();      /* Local Address Byte */
void MP_Instruction_LAW();      /* Local Address Word */
void MP_Instruction_LL0();      /* Load Local 0 */
void MP_Instruction_LL1();      /* Load Local 1 */
void MP_Instruction_LL2();      /* Load Local 2 */
void MP_Instruction_LL3();      /* Load Local 3 */
void MP_Instruction_LL4();      /* Load Local 4 */
void MP_Instruction_LL5();      /* Load Local 5 */
void MP_Instruction_LL6();      /* Load Local 6 */
void MP_Instruction_LL7();      /* Load Local 7 */
void MP_Instruction_LL8();      /* Load Local 8 */
void MP_Instruction_LL9();      /* Load Local 9 */
void MP_Instruction_LL10();     /* Load Local 10 */
void MP_Instruction_LL11();     /* Load Local 11 */
void MP_Instruction_LLB();      /* Load Local Byte */
void MP_Instruction_LLD0();     /* Load Local Double 0 */
void MP_Instruction_LLD1();     /* Load Local Double 1 */
void MP_Instruction_LLD2();     /* Load Local Double 2 */
void MP_Instruction_LLD3();     /* Load Local Double 3 */
void MP_Instruction_LLD4();     /* Load Local Double 4 */
void MP_Instruction_LLD5();     /* Load Local Double 5 */
void MP_Instruction_LLD6();     /* Load Local Double 6 */
void MP_Instruction_LLD7();     /* Load Local Double 7 */
void MP_Instruction_LLD8();     /* Load Local Double 8 */
void MP_Instruction_LLD10();    /* Load Local Double 10 */
void MP_Instruction_LLDB();     /* Load Local Double Byte */
void MP_Instruction_SL0();      /* Store Local 0 */
void MP_Instruction_SL1();      /* Store Local 1 */
void MP_Instruction_SL2();      /* Store Local 2 */
void MP_Instruction_SL3();      /* Store Local 3 */
void MP_Instruction_SL4();      /* Store Local 4 */
void MP_Instruction_SL5();      /* Store Local 5 */
void MP_Instruction_SL6();      /* Store Local 6 */
void MP_Instruction_SL7();      /* Store Local 7 */
void MP_Instruction_SL8();      /* Store Local 8 */
void MP_Instruction_SL9();      /* Store Local 9 */
void MP_Instruction_SL10();     /* Store Local 10 */
void MP_Instruction_SLB();      /* Store Local Byte */
void MP_Instruction_SLD0();     /* Store Local Double 0 */
void MP_Instruction_SLD1();     /* Store Local Double 1 */
void MP_Instruction_SLD2();     /* Store Local Double 2 */
void MP_Instruction_SLD3();     /* Store Local Double 3 */
void MP_Instruction_SLD4();     /* Store Local Double 4 */
void MP_Instruction_SLD5();     /* Store Local Double 5 */
void MP_Instruction_SLD6();     /* Store Local Double 6 */
void MP_Instruction_SLD8();     /* Store Local Double 8 */
void MP_Instruction_SLDB();     /* Store Local Double Byte */
void MP_Instruction_PL0();      /* Put Local 0 */
void MP_Instruction_PL1();      /* Put Local 1 */
void MP_Instruction_PL2();      /* Put Local 2 */
void MP_Instruction_PL3();      /* Put Local 3 */
void MP_Instruction_PLB();      /* Put Local Byte */
void MP_Instruction_PLD0();     /* Put Local Double 0 */
void MP_Instruction_PLDB();     /* Put Local Double Byte */
void MP_Instruction_AL0IB();    /* Add Local Zero to Immediate Byte */
void MP_Instruction_GA0();      /* Global Address 0 */
void MP_Instruction_GA1();      /* Global Address 1 */
void MP_Instruction_GAB();      /* Global Address Byte */
void MP_Instruction_GAW();      /* Global Address Word */
void MP_Instruction_LG0();      /* Load Global 0 */
void MP_Instruction_LG1();      /* Load Global 1 */
void MP_Instruction_LG2();      /* Load Global 2 */
void MP_Instruction_LGB();      /* Load Global Byte */
void MP_Instruction_LGD0();     /* Load Global Double 0 */
void MP_Instruction_LGD2();     /* Load Global Double 2 */
void MP_Instruction_LGDB();     /* Load Global Double Byte */
void MP_Instruction_SGB();      /* Store Global Byte */
void MP_Instruction_SGDB();     /* Store Global Double Byte */

/* Pointer Instructions */
void MP_Instruction_R0();       /* Read 0 */
void MP_Instruction_R1();       /* Read 1 */
void MP_Instruction_RB();       /* Read Byte */
void MP_Instruction_RL0();      /* Read Long Zero */
void MP_Instruction_RLB();      /* Read Long Byte */
void MP_Instruction_RD0();      /* Read Double Zero */
void MP_Instruction_RDB();      /* Read Double Byte */
void MP_Instruction_RDL0();     /* Read Double Long Zero */
void MP_Instruction_RDLB();     /* Read Double Long Byte */
void MP_Instruction_RC();       /* Read Code */
void MP_Instruction_W0();       /* Write Zero */
void MP_Instruction_WB();       /* Write Byte */
void MP_Instruction_WLB();      /* Write Long Byte */
void MP_Instruction_WDB();      /* Write Double Byte */
void MP_Instruction_WDLB();     /* Write Double Long Byte */
void MP_Instruction_PSB();      /* Put Swapped Byte */
void MP_Instruction_PSD0();     /* Put Swapped Double Zero */
void MP_Instruction_PSDB();     /* Put Swapped Double Byte */
void MP_Instruction_PSLB();     /* Put Swapped Long Byte */
void MP_Instruction_PSDLB();    /* Put Swapped Double Long Byte */
void MP_Instruction_RLI00();    /* Read Local Indirect Zero 0 */
void MP_Instruction_RLI01();    /* Read Local Indirect Zero 1 */
void MP_Instruction_RLI02();    /* Read Local Indirect Zero 2 */
void MP_Instruction_RLI03();    /* Read Local Indirect Zero 3 */
void MP_Instruction_RLIP();     /* Read Local Indirect Pair */
void MP_Instruction_RLILP();    /* Read Local Indirect Long Pair */
void MP_Instruction_RGIP();     /* Read Global Indirect Pair */
void MP_Instruction_RGILP();    /* Read Global Indirect Long Pair */
void MP_Instruction_RLDI00();   /* Read Local Double Indirect Zero Zero */
void MP_Instruction_RLDIP();    /* Read Local Double Indirect Pair */
void MP_Instruction_RLDILP();   /* Read Local Double Indirect Long Pair */
void MP_Instruction_WLIP();     /* Write Local Indirect Pair */
void MP_Instruction_WLILP();    /* Write Local Indirect Long Pair */
void MP_Instruction_WLDILP();   /* Write Local Double Indirect Long Pair */

/* String Instructions */
void MP_Instruction_RS();       /* Read String */
void MP_Instruction_RLS();      /* Read Long String */
void MP_Instruction_WS();       /* Write String */
void MP_Instruction_WLS();      /* Write Long String */

/* Field Instructions */
void MP_Instruction_RF();       /* Read Field */
void MP_Instruction_R0F();      /* Read Zero Field */
void MP_Instruction_RLF();      /* Read Long Field */
void MP_Instruction_RL0F();     /* Read Long Zero Field */
void MP_Instruction_RLFS();     /* Read Long Field Stack */
void MP_Instruction_RCFS();     /* Read Code Field Stack */
void MP_Instruction_RLIPF();    /* Read Local Indirect Pair Field */
void MP_Instruction_RLILPF();   /* Read Local Indirect Long Pair Field */
void MP_Instruction_WF();       /* Write Field */
void MP_Instruction_W0F();      /* Write Zero Field */
void MP_Instruction_WLF();      /* Write Long Field */
void MP_Instruction_WL0F();     /* Write Long Zero Field */
void MP_Instruction_WLFS();     /* Write Long Field Stack */
void MP_Instruction_WS0F();     /* Write Swapped Zero Field */
void MP_Instruction_PS0F();     /* Put Swapped Zero Field */
void MP_Instruction_PSF();      /* Put Swapped Field */
void MP_Instruction_PSLF();     /* Put Swapped Long Field */

#endif // #ifndef _INCLUDE_PRINCOPS_MP_MP_ASSIGNMENTINSTRUCTIONS_H

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

