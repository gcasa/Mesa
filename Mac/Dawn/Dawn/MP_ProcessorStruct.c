/*
    MP_ProcessorStruct.c - implementation of Mesa processor state

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#include "PrincOps/PrincOps_Microcode.h"

/*******************************************************************************
** module MP_ProcessorStruct public variables */
Mesa_Processor MP_Mesa_Processor;

/*******************************************************************************
** MP_ProcessorStruct functions */
int
MP_ProcessorInitialize()
{
    return 0;
}

int
MP_ProcessorUninitialize()
{
    return 0;
}

/*******************************************************************************
** MP_ProcessorStruct global variables */
Mesa_Processor MP_Mesa_Processor;

/*******************************************************************************
** MP_ProcessorStruct opcode tables */
#ifdef _MP_BY_THE_BOOK
#define MP_Dispatch_EscOpcode       0
#define MP_Dispatch_EscLOpcode      0
#define MP_Dispatch_OpcodeTRAP      0
#define MP_Dispatch_EscOpcodeTRAP   0
#endif

InstructionFunction * MP_Mesa_Opcode_Table[256] =
{
    /* 000B | 000 | 0x00 */         MP_Dispatch_OpcodeTRAP,
    /* 001B | 001 | 0x01 */         MP_Instruction_LL0,
    /* 002B | 002 | 0x02 */         MP_Instruction_LL1,
    /* 003B | 003 | 0x03 */         MP_Instruction_LL2,
    /* 004B | 004 | 0x04 */         MP_Instruction_LL3,
    /* 005B | 005 | 0x05 */         MP_Instruction_LL4,
    /* 006B | 006 | 0x06 */         MP_Instruction_LL5,
    /* 007B | 007 | 0x07 */         MP_Instruction_LL6,
    /* 010B | 008 | 0x08 */         MP_Instruction_LL7,
    /* 011B | 009 | 0x09 */         MP_Instruction_LL8,
    /* 012B | 010 | 0x0A */         MP_Instruction_LL9,
    /* 013B | 011 | 0x0B */         MP_Instruction_LL10,
    /* 014B | 012 | 0x0C */         MP_Instruction_LL11,
    /* 015B | 013 | 0x0D */         MP_Instruction_LLB,
    /* 016B | 014 | 0x0E */         MP_Instruction_LLD0,
    /* 017B | 015 | 0x0F */         MP_Instruction_LLD1,

    /* 020B | 016 | 0x10 */         MP_Instruction_LLD2,
    /* 021B | 017 | 0x11 */         MP_Instruction_LLD3,
    /* 022B | 018 | 0x12 */         MP_Instruction_LLD4,
    /* 023B | 019 | 0x13 */         MP_Instruction_LLD5,
    /* 024B | 020 | 0x14 */         MP_Instruction_LLD6,
    /* 025B | 021 | 0x15 */         MP_Instruction_LLD7,
    /* 026B | 022 | 0x16 */         MP_Instruction_LLD8,
    /* 027B | 023 | 0x17 */         MP_Instruction_LLD10,
    /* 030B | 024 | 0x18 */         MP_Instruction_LLDB,
    /* 031B | 025 | 0x19 */         MP_Instruction_SL0,
    /* 032B | 026 | 0x1A */         MP_Instruction_SL1,
    /* 033B | 027 | 0x1B */         MP_Instruction_SL2,
    /* 034B | 028 | 0x1C */         MP_Instruction_SL3,
    /* 035B | 029 | 0x1D */         MP_Instruction_SL4,
    /* 036B | 030 | 0x1E */         MP_Instruction_SL5,
    /* 037B | 031 | 0x1F */         MP_Instruction_SL6,

    /* 040B | 032 | 0x20 */         MP_Instruction_SL7,
    /* 041B | 033 | 0x21 */         MP_Instruction_SL8,
    /* 042B | 034 | 0x22 */         MP_Instruction_SL9,
    /* 043B | 035 | 0x23 */         MP_Instruction_SL10,
    /* 044B | 036 | 0x24 */         MP_Instruction_SLB,
    /* 045B | 037 | 0x25 */         MP_Instruction_SLD0,
    /* 046B | 038 | 0x26 */         MP_Instruction_SLD1,
    /* 047B | 039 | 0x27 */         MP_Instruction_SLD2,
    /* 050B | 040 | 0x28 */         MP_Instruction_SLD3,
    /* 051B | 041 | 0x29 */         MP_Instruction_SLD4,
    /* 052B | 042 | 0x2A */         MP_Instruction_SLD5,
    /* 053B | 043 | 0x2B */         MP_Instruction_SLD6,
    /* 054B | 044 | 0x2C */         MP_Instruction_SLD8,
    /* 055B | 045 | 0x2D */         MP_Instruction_PL0,
    /* 056B | 046 | 0x2E */         MP_Instruction_PL1,
    /* 057B | 047 | 0x2F */         MP_Instruction_PL2,

    /* 060B | 048 | 0x30 */         MP_Instruction_PL3,
    /* 061B | 049 | 0x31 */         MP_Instruction_PLB,
    /* 062B | 050 | 0x32 */         MP_Instruction_PLD0,
    /* 063B | 051 | 0x33 */         MP_Instruction_PLDB,
    /* 064B | 052 | 0x34 */         MP_Instruction_LG0,
    /* 065B | 053 | 0x35 */         MP_Instruction_LG1,
    /* 066B | 054 | 0x36 */         MP_Instruction_LG2,
    /* 067B | 055 | 0x37 */         MP_Instruction_LGB,
    /* 070B | 056 | 0x38 */         MP_Instruction_LGD0,
    /* 071B | 057 | 0x39 */         MP_Instruction_LGD2,
    /* 072B | 058 | 0x3A */         MP_Instruction_LGDB,
    /* 073B | 059 | 0x3B */         MP_Instruction_SGB,
    /* 074B | 060 | 0x3C */         MP_Instruction_BNDCK,
    /* 075B | 061 | 0x3D */         MP_Instruction_BRK,
    /* 076B | 062 | 0x3E */         MP_Dispatch_OpcodeTRAP,
    /* 077B | 063 | 0x3F */         MP_Dispatch_OpcodeTRAP, /* zSTC - MopcodesExtras.mesa : not implemented here */

    /* 100B | 064 | 0x40 */         MP_Instruction_R0,
    /* 101B | 065 | 0x41 */         MP_Instruction_R1,
    /* 102B | 066 | 0x42 */         MP_Instruction_RB,
    /* 103B | 067 | 0x43 */         MP_Instruction_RL0,
    /* 104B | 068 | 0x44 */         MP_Instruction_RLB,
    /* 105B | 069 | 0x45 */         MP_Instruction_RD0,
    /* 106B | 070 | 0x46 */         MP_Instruction_RDB,
    /* 107B | 071 | 0x47 */         MP_Instruction_RDL0,
    /* 110B | 072 | 0x48 */         MP_Instruction_RDLB,
    /* 111B | 073 | 0x49 */         MP_Instruction_W0,
    /* 112B | 074 | 0x4A */         MP_Instruction_WB,
    /* 113B | 075 | 0x4B */         MP_Instruction_PSB,
    /* 114B | 076 | 0x4C */         MP_Instruction_WLB,
    /* 115B | 077 | 0x4D */         MP_Instruction_PSLB,
    /* 116B | 078 | 0x4E */         MP_Instruction_WDB,
    /* 117B | 079 | 0x4F */         MP_Instruction_PSD0,

    /* 120B | 080 | 0x50 */         MP_Instruction_PSDB,
    /* 121B | 081 | 0x51 */         MP_Instruction_WDLB,
    /* 122B | 082 | 0x52 */         MP_Instruction_PSDLB,
    /* 123B | 083 | 0x53 */         MP_Instruction_RLI00,
    /* 124B | 084 | 0x54 */         MP_Instruction_RLI01,
    /* 125B | 085 | 0x55 */         MP_Instruction_RLI02,
    /* 126B | 086 | 0x56 */         MP_Instruction_RLI03,
    /* 127B | 087 | 0x57 */         MP_Instruction_RLIP,
    /* 130B | 088 | 0x58 */         MP_Instruction_RLILP,
    /* 131B | 089 | 0x59 */         MP_Instruction_RLDI00,
    /* 132B | 090 | 0x5A */         MP_Instruction_RLDIP,
    /* 133B | 091 | 0x5B */         MP_Instruction_RLDILP,
    /* 134B | 092 | 0x5C */         MP_Instruction_RGIP,
    /* 135B | 093 | 0x5D */         MP_Instruction_RGILP,
    /* 136B | 094 | 0x5E */         MP_Instruction_WLIP,
    /* 137B | 095 | 0x5F */         MP_Instruction_WLILP,

    /* 140B | 096 | 0x60 */         MP_Instruction_WLDILP,
    /* 141B | 097 | 0x61 */         MP_Instruction_RS,
    /* 142B | 098 | 0x62 */         MP_Instruction_RLS,
    /* 143B | 099 | 0x63 */         MP_Instruction_WS,
    /* 144B | 100 | 0x64 */         MP_Instruction_WLS,
    /* 145B | 101 | 0x65 */         MP_Instruction_R0F,
    /* 146B | 102 | 0x66 */         MP_Instruction_RF,
    /* 147B | 103 | 0x67 */         MP_Instruction_RL0F,
    /* 150B | 104 | 0x68 */         MP_Instruction_RLF,
    /* 151B | 105 | 0x69 */         MP_Instruction_RLFS,
    /* 152B | 106 | 0x6A */         MP_Instruction_RLIPF,
    /* 153B | 107 | 0x6B */         MP_Instruction_RLILPF,
    /* 154B | 108 | 0x6C */         MP_Instruction_W0F,
    /* 155B | 109 | 0x6D */         MP_Instruction_WF,
    /* 156B | 110 | 0x6E */         MP_Instruction_PSF,
    /* 157B | 111 | 0x6F */         MP_Instruction_PS0F,

    /* 160B | 112 | 0x70 */         MP_Instruction_WS0F,
    /* 161B | 113 | 0x71 */         MP_Instruction_WL0F,
    /* 162B | 114 | 0x72 */         MP_Instruction_WLF,
    /* 163B | 115 | 0x73 */         MP_Instruction_PSLF,
    /* 164B | 116 | 0x74 */         MP_Instruction_WLFS,
    /* 165B | 117 | 0x75 */         MP_Instruction_SLDB,
    /* 166B | 118 | 0x76 */         MP_Instruction_SGDB,
    /* 167B | 119 | 0x77 */         MP_Instruction_LLKB,
    /* 170B | 120 | 0x78 */         MP_Instruction_RKIB,
    /* 171B | 121 | 0x79 */         MP_Instruction_RKDIB,
    /* 172B | 122 | 0x7A */         MP_Instruction_LKB,
    /* 173B | 123 | 0x7B */         MP_Instruction_SHIFT,
    /* 174B | 124 | 0x7C */         MP_Instruction_SHIFTSB,
    /* 175B | 125 | 0x7D */         MP_Dispatch_OpcodeTRAP,     /* 3 Pilot 15 Opcodes added by Fuji; */
    /* 176B | 126 | 0x7E */         MP_Dispatch_OpcodeTRAP,     /* Not implemented here              */
    /* 177B | 127 | 0x7F */         MP_Dispatch_OpcodeTRAP,

    /* 200B | 128 | 0x80 */         MP_Instruction_CATCH,
    /* 201B | 129 | 0x81 */         MP_Instruction_J2,
    /* 202B | 130 | 0x82 */         MP_Instruction_J3,
    /* 203B | 131 | 0x83 */         MP_Instruction_J4,
    /* 204B | 132 | 0x84 */         MP_Instruction_J5,
    /* 205B | 133 | 0x85 */         MP_Instruction_J6,
    /* 206B | 134 | 0x86 */         MP_Instruction_J7,
    /* 207B | 135 | 0x87 */         MP_Instruction_J8,
    /* 210B | 136 | 0x88 */         MP_Instruction_JB,
    /* 211B | 137 | 0x89 */         MP_Instruction_JW,
    /* 212B | 138 | 0x8A */         MP_Instruction_JEP,
    /* 213B | 139 | 0x8B */         MP_Instruction_JEB,
    /* 214B | 140 | 0x8C */         MP_Instruction_JEBB,
    /* 215B | 141 | 0x8D */         MP_Instruction_JNEP,
    /* 216B | 142 | 0x8E */         MP_Instruction_JNEB,
    /* 217B | 143 | 0x8F */         MP_Instruction_JNEBB,

    /* 220B | 144 | 0x90 */         MP_Instruction_JLB,
    /* 221B | 145 | 0x91 */         MP_Instruction_JGEB,
    /* 222B | 146 | 0x92 */         MP_Instruction_JGB,
    /* 223B | 147 | 0x93 */         MP_Instruction_JLEB,
    /* 224B | 148 | 0x94 */         MP_Instruction_JULB,
    /* 225B | 149 | 0x95 */         MP_Instruction_JUGEB,
    /* 226B | 150 | 0x96 */         MP_Instruction_JUGB,
    /* 227B | 151 | 0x97 */         MP_Instruction_JULEB,
    /* 230B | 152 | 0x98 */         MP_Instruction_JZ3,
    /* 231B | 153 | 0x99 */         MP_Instruction_JZ4,
    /* 232B | 154 | 0x9A */         MP_Instruction_JZB,
    /* 233B | 155 | 0x9B */         MP_Instruction_JNZ3,
    /* 234B | 156 | 0x9C */         MP_Instruction_JNZ4,
    /* 235B | 157 | 0x9D */         MP_Instruction_JNZB,
    /* 236B | 158 | 0x9E */         MP_Instruction_JDEB,
    /* 237B | 159 | 0x9F */         MP_Instruction_JDNEB,

    /* 240B | 160 | 0xA0 */         MP_Instruction_JIB,
    /* 241B | 161 | 0xA1 */         MP_Instruction_JIW,
    /* 242B | 162 | 0xA2 */         MP_Instruction_REC,
    /* 243B | 163 | 0xA3 */         MP_Instruction_REC2,
    /* 244B | 164 | 0xA4 */         MP_Instruction_DIS,
    /* 245B | 165 | 0xA5 */         MP_Instruction_DIS2,
    /* 246B | 166 | 0xA6 */         MP_Instruction_EXCH,
    /* 247B | 167 | 0xA7 */         MP_Instruction_DEXCH,
    /* 250B | 168 | 0xA8 */         MP_Instruction_DUP,
    /* 251B | 169 | 0xA9 */         MP_Instruction_DDUP,
    /* 252B | 170 | 0xAA */         MP_Instruction_EXDIS,
    /* 253B | 171 | 0xAB */         MP_Instruction_NEG,
    /* 254B | 172 | 0xAC */         MP_Instruction_INC,
    /* 255B | 173 | 0xAD */         MP_Instruction_DEC,
    /* 256B | 174 | 0xAE */         MP_Instruction_DINC,
    /* 257B | 175 | 0xAF */         MP_Instruction_DBL,

    /* 260B | 176 | 0xB0 */         MP_Instruction_DDBL,
    /* 261B | 177 | 0xB1 */         MP_Instruction_TRPL,
    /* 262B | 178 | 0xB2 */         MP_Instruction_AND,
    /* 263B | 179 | 0xB3 */         MP_Instruction_IOR,
    /* 264B | 180 | 0xB4 */         MP_Instruction_ADDSB,
    /* 265B | 181 | 0xB5 */         MP_Instruction_ADD,
    /* 266B | 182 | 0xB6 */         MP_Instruction_SUB,
    /* 267B | 183 | 0xB7 */         MP_Instruction_DADD,
    /* 270B | 184 | 0xB8 */         MP_Instruction_DSUB,
    /* 271B | 185 | 0xB9 */         MP_Instruction_ADC,
    /* 272B | 186 | 0xBA */         MP_Instruction_ACD,
    /* 273B | 187 | 0xBB */         MP_Instruction_AL0IB,
    /* 274B | 188 | 0xBC */         MP_Instruction_MUL,
    /* 275B | 189 | 0xBD */         MP_Instruction_DCMP,
    /* 276B | 190 | 0xBE */         MP_Instruction_UDCMP,
    /* 277B | 191 | 0xBF */         MP_Instruction_VMFIND,

    /* 300B | 192 | 0xC0 */         MP_Instruction_LI0,
    /* 301B | 193 | 0xC1 */         MP_Instruction_LI1,
    /* 302B | 194 | 0xC2 */         MP_Instruction_LI2,
    /* 303B | 195 | 0xC3 */         MP_Instruction_LI3,
    /* 304B | 196 | 0xC4 */         MP_Instruction_LI4,
    /* 305B | 197 | 0xC5 */         MP_Instruction_LI5,
    /* 306B | 198 | 0xC6 */         MP_Instruction_LI6,
    /* 307B | 199 | 0xC7 */         MP_Instruction_LI7,
    /* 310B | 200 | 0xC8 */         MP_Instruction_LI8,
    /* 311B | 201 | 0xC9 */         MP_Instruction_LI9,
    /* 312B | 202 | 0xCA */         MP_Instruction_LI10,
    /* 313B | 203 | 0xCB */         MP_Instruction_LIN1,
    /* 314B | 204 | 0xCC */         MP_Instruction_LINI,
    /* 315B | 205 | 0xCD */         MP_Instruction_LIB,
    /* 316B | 206 | 0xCE */         MP_Instruction_LIW,
    /* 317B | 207 | 0xCF */         MP_Instruction_LINB,

    /* 320B | 208 | 0xD0 */         MP_Instruction_LIHB,
    /* 321B | 209 | 0xD1 */         MP_Instruction_LI00,   /* ISSUE: Is this LI00 or LID0 ? */
    /* 322B | 210 | 0xD2 */         MP_Instruction_LA0,
    /* 323B | 211 | 0xD3 */         MP_Instruction_LA1,
    /* 324B | 212 | 0xD4 */         MP_Instruction_LA2,
    /* 325B | 213 | 0xD5 */         MP_Instruction_LA3,
    /* 326B | 214 | 0xD6 */         MP_Instruction_LA6,
    /* 327B | 215 | 0xD7 */         MP_Instruction_LA8,
    /* 330B | 216 | 0xD8 */         MP_Instruction_LAB,
    /* 331B | 217 | 0xD9 */         MP_Instruction_LAW,
    /* 332B | 218 | 0xDA */         MP_Instruction_GA0,
    /* 333B | 219 | 0xDB */         MP_Instruction_GA1,
    /* 334B | 220 | 0xDC */         MP_Instruction_GAB,
    /* 335B | 221 | 0xDD */         MP_Instruction_GAW,
    /* 336B | 222 | 0xDE */         MP_Dispatch_OpcodeTRAP,                       /* zCAW */
    /* 337B | 223 | 0xDF */         MP_Instruction_EFC0,

    /* 340B | 224 | 0xE0 */         MP_Instruction_EFC1,
    /* 341B | 225 | 0xE1 */         MP_Instruction_EFC2,
    /* 342B | 226 | 0xE2 */         MP_Instruction_EFC3,
    /* 343B | 227 | 0xE3 */         MP_Instruction_EFC4,
    /* 344B | 228 | 0xE4 */         MP_Instruction_EFC5,
    /* 345B | 229 | 0xE5 */         MP_Instruction_EFC6,
    /* 346B | 230 | 0xE6 */         MP_Instruction_EFC7,
    /* 347B | 231 | 0xE7 */         MP_Instruction_EFC8,
    /* 350B | 232 | 0xE8 */         MP_Instruction_EFC9,
    /* 351B | 233 | 0xE9 */         MP_Instruction_EFC10,
    /* 352B | 234 | 0xEA */         MP_Instruction_EFC11,
    /* 353B | 235 | 0xEB */         MP_Instruction_EFC12,
    /* 354B | 236 | 0xEC */         MP_Instruction_EFCB,
    /* 355B | 237 | 0xED */         MP_Instruction_LFC,
    /* 356B | 238 | 0xEE */         MP_Instruction_SFC,
    /* 357B | 239 | 0xEF */         MP_Instruction_RET,

    /* 360B | 240 | 0xF0 */         MP_Instruction_KFCB,
    /* 361B | 241 | 0xF1 */         MP_Instruction_ME,
    /* 362B | 242 | 0xF2 */         MP_Instruction_MX,
    /* 363B | 243 | 0xF3 */         MP_Instruction_BLT,
    /* 364B | 244 | 0xF4 */         MP_Instruction_BLTL,
    /* 365B | 245 | 0xF5 */         MP_Instruction_BLTC,
    /* 366B | 246 | 0xF6 */         MP_Instruction_BLTCL,
    /* 367B | 247 | 0xF7 */         MP_Instruction_LP,
    /* 370B | 248 | 0xF8 */         MP_Dispatch_EscOpcode,      /* zESC */
    /* 371B | 249 | 0xF9 */         MP_Dispatch_EscLOpcode,     /* zESCL */

        /* zLGA0, zLGAB, zLGAW, zDESC - MOpcodesExtras.mesa */
        /* Defined in 7.2.2 of changed chapters document    */
        /* zLGA1 in document, but not in microcode          */

    /* 372B | 250 | 0xFA */         MP_Instruction_LGA0,
    /* 373B | 251 | 0xFB */         MP_Instruction_LGAB,
    /* 374B | 252 | 0xFC */         MP_Instruction_LGAW,
    /* 375B | 253 | 0xFD */         MP_Instruction_DESC,
    
    /* 376B | 254 | 0xFE */         MP_Dispatch_OpcodeTRAP,
    /* 377B | 255 | 0xFF */         MP_Dispatch_OpcodeTRAP
};

InstructionFunction * MP_Mesa_OpcodeEsc_Table[256] =
{
    /* 000B | 000 | 0x00 */         MP_Dispatch_EscOpcodeTRAP,
    /* 001B | 001 | 0x01 */         MP_Dispatch_EscOpcodeTRAP,
    /* 002B | 002 | 0x02 */         MP_Instruction_MW,
    /* 003B | 003 | 0x03 */         MP_Instruction_MR,
    /* 004B | 004 | 0x04 */         MP_Instruction_NC,
    /* 005B | 005 | 0x05 */         MP_Instruction_BC,
    /* 006B | 006 | 0x06 */         MP_Instruction_REQ,
    /* 007B | 007 | 0x07 */         MP_Instruction_SM,
    /* 010B | 008 | 0x08 */         MP_Instruction_SMF,
    /* 011B | 009 | 0x09 */         MP_Instruction_GMF,
    /* 012B | 010 | 0x0A */         MP_Instruction_AF,
    /* 013B | 011 | 0x0B */         MP_Instruction_FF,
    /* 014B | 012 | 0x0C */         MP_Instruction_PI,
    /* 015B | 013 | 0x0D */         MP_Instruction_PO,
    /* 016B | 014 | 0x0E */         MP_Instruction_POR,
    /* 017B | 015 | 0x0F */         MP_Instruction_SPP,

    /* 020B | 016 | 0x10 */         MP_Instruction_DI,
    /* 021B | 017 | 0x11 */         MP_Instruction_EI,
    /* 022B | 018 | 0x12 */         MP_Instruction_XOR,
    /* 023B | 019 | 0x13 */         MP_Instruction_DAND,
    /* 024B | 020 | 0x14 */         MP_Instruction_DIOR,
    /* 025B | 021 | 0x15 */         MP_Instruction_DXOR,
    /* 026B | 022 | 0x16 */         MP_Instruction_ROTATE,
    /* 027B | 023 | 0x17 */         MP_Instruction_DSHIFT,
    /* 030B | 024 | 0x18 */         MP_Instruction_LINT,
    /* 031B | 025 | 0x19 */         MP_Instruction_JS,
    /* 032B | 026 | 0x1A */         MP_Instruction_RCFS,
    /* 033B | 027 | 0x1B */         MP_Instruction_RC,
    /* 034B | 028 | 0x1C */         MP_Instruction_UDIV,
    /* 035B | 029 | 0x1D */         MP_Instruction_LUDIV,
    /* 036B | 030 | 0x1E */         MP_Instruction_ROB,
    /* 037B | 031 | 0x1F */         MP_Instruction_WOB,

    /* 040B | 032 | 0x20 */         MP_Instruction_DSK,
    /* 041B | 033 | 0x21 */         MP_Instruction_XE,
    /* 042B | 034 | 0x22 */         MP_Instruction_XF,
    /* 043B | 035 | 0x23 */         MP_Instruction_LSK,
    /* 044B | 036 | 0x24 */         MP_Instruction_BNDCKL,
    /* 045B | 037 | 0x25 */         MP_Instruction_NILCK,
    /* 046B | 038 | 0x26 */         MP_Instruction_NILCKL,
    /* 047B | 039 | 0x27 */         MP_Instruction_BLTLR,
    /* 050B | 040 | 0x28 */         MP_Instruction_BLEL,
    /* 051B | 041 | 0x29 */         MP_Instruction_BLECL,
    /* 052B | 042 | 0x2A */         MP_Instruction_CKSUM,
    /* 053B | 043 | 0x2B */         MP_Instruction_BITBLT,
    /* 054B | 044 | 0x2C */         MP_Instruction_TXTBLT,
    /* 055B | 045 | 0x2D */         MP_Instruction_BYTBLT,
    /* 056B | 046 | 0x2E */         MP_Instruction_BYTBLTR,
    /* 057B | 047 | 0x2F */         MP_Instruction_VERSION,

    /* 060B | 048 | 0x30 */         MP_Instruction_DMUL,
    /* 061B | 049 | 0x31 */         MP_Instruction_SDIV,
    /* 062B | 050 | 0x32 */         MP_Instruction_SDDIV,
    /* 063B | 051 | 0x33 */         MP_Instruction_UDDIV,

        /* CompressAssist - 4045 uCode instructions */

    /* 064B | 052 | 0x34 */         MP_Dispatch_EscOpcodeTRAP,  /* CompressAssist.NextMatchAndDiff          */
    /* 065B | 053 | 0x35 */         MP_Dispatch_EscOpcodeTRAP,  /* CompressAssist.PushByte                  */
    /* 066B | 054 | 0x36 */         MP_Dispatch_EscOpcodeTRAP,  /* CompressAssist.PushByteToWordBoundary    */
    /* 067B | 055 | 0x37 */         MP_Dispatch_EscOpcodeTRAP,  /* CompressAssist.Clear                     */

    /* 070B | 056 | 0x38 */         MP_Dispatch_EscOpcodeTRAP,

        /* ST80MiscOps uCode instructions */

    /* 071B | 057 | 0x39 */         MP_Dispatch_EscOpcodeTRAP,
    /* 072B | 058 | 0x3A */         MP_Dispatch_EscOpcodeTRAP,
    /* 073B | 059 | 0x3B */         MP_Dispatch_EscOpcodeTRAP,
    /* 074B | 060 | 0x3C */         MP_Dispatch_EscOpcodeTRAP,
    /* 075B | 061 | 0x3D */         MP_Dispatch_EscOpcodeTRAP,
    /* 076B | 062 | 0x3E */         MP_Dispatch_EscOpcodeTRAP,
    /* 077B | 063 | 0x3F */         MP_Dispatch_EscOpcodeTRAP,

        /* Floating Point Instructions */

    /* 100B | 064 | 0x40 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FADD                      */
    /* 101B | 065 | 0x41 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FSUB                      */
    /* 102B | 066 | 0x42 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FMUL                      */
    /* 103B | 067 | 0x43 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FDIV                      */
    /* 104B | 068 | 0x44 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FCOMP                     */
    /* 105B | 069 | 0x45 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FIX                       */
    /* 106B | 070 | 0x46 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FLOAT                     */
    /* 107B | 071 | 0x47 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FIXI                      */
    /* 110B | 072 | 0x48 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FIXC                      */
    /* 111B | 073 | 0x49 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FSTICKY                   */
    /* 112B | 074 | 0x4A */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FREM                      */
    /* 113B | 075 | 0x4B */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_ROUND                     */
    /* 114B | 076 | 0x4C */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_ROUNDI                    */
    /* 115B | 077 | 0x4D */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FROUNDC                   */
    /* 116B | 078 | 0x4E */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FSQRT                     */
    /* 117B | 079 | 0x4F */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_FSC                       */

        /* Unused group that may be assigned to floating point */
    
    /* 120B | 080 | 0x50 */         MP_Dispatch_EscOpcodeTRAP,
    /* 121B | 081 | 0x51 */         MP_Dispatch_EscOpcodeTRAP,
    /* 122B | 082 | 0x52 */         MP_Dispatch_EscOpcodeTRAP,
    /* 123B | 083 | 0x53 */         MP_Dispatch_EscOpcodeTRAP,
    /* 124B | 084 | 0x54 */         MP_Dispatch_EscOpcodeTRAP,
    /* 125B | 085 | 0x55 */         MP_Dispatch_EscOpcodeTRAP,
    /* 126B | 086 | 0x56 */         MP_Dispatch_EscOpcodeTRAP,
    /* 127B | 087 | 0x57 */         MP_Dispatch_EscOpcodeTRAP,
    /* 130B | 088 | 0x58 */         MP_Dispatch_EscOpcodeTRAP,
    /* 131B | 089 | 0x59 */         MP_Dispatch_EscOpcodeTRAP,
    /* 132B | 090 | 0x5A */         MP_Dispatch_EscOpcodeTRAP,
    /* 133B | 091 | 0x5B */         MP_Dispatch_EscOpcodeTRAP,
    /* 134B | 092 | 0x5C */         MP_Dispatch_EscOpcodeTRAP,
    /* 135B | 093 | 0x5D */         MP_Dispatch_EscOpcodeTRAP,
    /* 136B | 094 | 0x5E */         MP_Dispatch_EscOpcodeTRAP,
    /* 137B | 095 | 0x5F */         MP_Dispatch_EscOpcodeTRAP,

        /* Cedar Ops */

    /* 140B | 096 | 0x60 */         MP_Dispatch_EscOpcodeTRAP,
    /* 141B | 097 | 0x61 */         MP_Dispatch_EscOpcodeTRAP,
    /* 142B | 098 | 0x62 */         MP_Dispatch_EscOpcodeTRAP,
    /* 143B | 099 | 0x63 */         MP_Dispatch_EscOpcodeTRAP,
    /* 144B | 100 | 0x64 */         MP_Dispatch_EscOpcodeTRAP,
    /* 145B | 101 | 0x65 */         MP_Dispatch_EscOpcodeTRAP,
    /* 146B | 102 | 0x66 */         MP_Dispatch_EscOpcodeTRAP,
    /* 147B | 103 | 0x67 */         MP_Dispatch_EscOpcodeTRAP,
    /* 150B | 104 | 0x68 */         MP_Dispatch_EscOpcodeTRAP,
    /* 151B | 105 | 0x69 */         MP_Dispatch_EscOpcodeTRAP,
    /* 152B | 106 | 0x6A */         MP_Dispatch_EscOpcodeTRAP,
    /* 153B | 107 | 0x6B */         MP_Dispatch_EscOpcodeTRAP,
    /* 154B | 108 | 0x6C */         MP_Dispatch_EscOpcodeTRAP,
    /* 155B | 109 | 0x6D */         MP_Dispatch_EscOpcodeTRAP,
    /* 156B | 110 | 0x6E */         MP_Dispatch_EscOpcodeTRAP,
    /* 157B | 111 | 0x6F */         MP_Dispatch_EscOpcodeTRAP,

    /* 160B | 112 | 0x70 */         MP_Instruction_WRPSB,
    /* 161B | 113 | 0x71 */         MP_Instruction_WRMDS,
    /* 162B | 114 | 0x72 */         MP_Instruction_WRWP,
    /* 163B | 115 | 0x73 */         MP_Instruction_WRWDC,
    /* 164B | 116 | 0x74 */         MP_Instruction_WRPTC,
    /* 165B | 117 | 0x75 */         MP_Instruction_WRIT,
    /* 166B | 118 | 0x76 */         MP_Instruction_WRXTS,
    /* 167B | 119 | 0x77 */         MP_Instruction_WRMP,
    /* 170B | 120 | 0x78 */         MP_Instruction_RRPSB,
    /* 171B | 121 | 0x79 */         MP_Instruction_RRMDS,
    /* 172B | 122 | 0x7A */         MP_Instruction_RRWP,
    /* 173B | 123 | 0x7B */         MP_Instruction_RRWDC,
    /* 174B | 124 | 0x7C */         MP_Instruction_RRPTC,
    /* 175B | 125 | 0x7D */         MP_Instruction_RRIT,
    /* 176B | 126 | 0x7E */         MP_Instruction_RRXTS,
    /* 177B | 127 | 0x7F */         MP_Dispatch_EscOpcodeTRAP,  /* a177 in Misc.mc */

        /* Processor dependent uCode instructions */

    /* 200B | 128 | 0x80 */         MP_Dispatch_EscOpcodeTRAP,
    /* 201B | 129 | 0x81 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_GetRGB_MVM */
    /* 202B | 130 | 0x82 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_Snap_Toggle_State */
    /* 203B | 131 | 0x83 */         MP_Dispatch_EscOpcodeTRAP,  /* @BANDBLT */
    /* 204B | 132 | 0x84 */         MP_Dispatch_EscOpcodeTRAP,
    /* 205B | 133 | 0x85 */         MP_Dispatch_EscOpcodeTRAP,
    /* 206B | 134 | 0x86 */         MP_Dispatch_EscOpcodeTRAP,
    /* 207B | 135 | 0x87 */         MP_Instruction_BYTESWAP,
    /* 210B | 136 | 0x88 */         MP_Dispatch_EscOpcodeTRAP,
    /* 211B | 137 | 0x89 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_CALLAGENT */
    /* 212B | 138 | 0x8A */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_MAPDISPLAY */
    /* 213B | 139 | 0x8B */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_STOPEMULATOR */
    /* 214B | 140 | 0x8C */         MP_Dispatch_EscOpcodeTRAP,
    /* 215B | 141 | 0x8D */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_SUSPEND */
    /* 216B | 142 | 0x8E */         MP_Dispatch_EscOpcodeTRAP,
    /* 217B | 143 | 0x8F */         MP_Dispatch_EscOpcodeTRAP,

        /* Dicentra specific uCode instructions */

    /* 220B | 144 | 0x90 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_DTPrint   */
    /* 221B | 145 | 0x91 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_DTFile    */
    /* 222B | 146 | 0x92 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_DTFileX   */
    /* 223B | 147 | 0x93 */         MP_Dispatch_EscOpcodeTRAP,  /* MP_Instruction_DTxx      */
    /* 224B | 148 | 0x94 */         MP_Dispatch_EscOpcodeTRAP,
    /* 225B | 149 | 0x95 */         MP_Dispatch_EscOpcodeTRAP,
    /* 226B | 150 | 0x96 */         MP_Dispatch_EscOpcodeTRAP,
    /* 227B | 151 | 0x97 */         MP_Dispatch_EscOpcodeTRAP,
    /* 230B | 152 | 0x98 */         MP_Dispatch_EscOpcodeTRAP,
    /* 231B | 153 | 0x99 */         MP_Dispatch_EscOpcodeTRAP,
    /* 232B | 154 | 0x9A */         MP_Dispatch_EscOpcodeTRAP,
    /* 233B | 155 | 0x9B */         MP_Dispatch_EscOpcodeTRAP,
    /* 234B | 156 | 0x9C */         MP_Dispatch_EscOpcodeTRAP,
    /* 235B | 157 | 0x9D */         MP_Dispatch_EscOpcodeTRAP,
    /* 236B | 158 | 0x9E */         MP_Dispatch_EscOpcodeTRAP,
    /* 237B | 159 | 0x9F */         MP_Dispatch_EscOpcodeTRAP,

    /* 240B | 160 | 0xA0 */         MP_Dispatch_EscOpcodeTRAP,  /* FLOYD */
    /* 241B | 161 | 0xA1 */         MP_Dispatch_EscOpcodeTRAP,  /* GRAYBLT */
    /* 242B | 162 | 0xA2 */         MP_Dispatch_EscOpcodeTRAP,  /* GRAYSUM */
    /* 243B | 163 | 0xA3 */         MP_Dispatch_EscOpcodeTRAP,  /* GRAYTHRSHLD */
    /* 244B | 164 | 0xA4 */         MP_Dispatch_EscOpcodeTRAP,  /* TRAPZBLT */
    /* 245B | 165 | 0xA5 */         MP_Dispatch_EscOpcodeTRAP,  /* COUNTBITSON */
    /* 246B | 166 | 0xA6 */         MP_Dispatch_EscOpcodeTRAP,  /* COUNTBITSOFF */
    /* 247B | 167 | 0xA7 */         MP_Dispatch_EscOpcodeTRAP,  /* BLTLINEGRAY */
    /* 250B | 168 | 0xA8 */         MP_Dispatch_EscOpcodeTRAP,  /* HALFTONE */
    /* 251B | 169 | 0xA9 */         MP_Dispatch_EscOpcodeTRAP,  /* CHAINBLT */
    /* 252B | 170 | 0xAA */         MP_Dispatch_EscOpcodeTRAP,  /* BUMPBITADDRESS */
    /* 253B | 171 | 0xAB */         MP_Dispatch_EscOpcodeTRAP,  /* FLIPXBITS */
    /* 254B | 172 | 0xAC */         MP_Dispatch_EscOpcodeTRAP,  /* FLIPXGRAY */
    /* 255B | 173 | 0xAD */         MP_Dispatch_EscOpcodeTRAP,  /* ROTATEBITS */
    /* 256B | 174 | 0xAE */         MP_Dispatch_EscOpcodeTRAP,  /* LINEBREAK */
    /* 257B | 175 | 0xAF */         MP_Dispatch_EscOpcodeTRAP,  /* SCALEBITSTOGRAY */

    /* 260B | 176 | 0xB0 */         MP_Dispatch_EscOpcodeTRAP,  /* GETRUNS */
    /* 261B | 177 | 0xB1 */         MP_Dispatch_EscOpcodeTRAP,  /* PREDICT */
    /* 262B | 178 | 0xB2 */         MP_Dispatch_EscOpcodeTRAP,  /* ENCODE */
    /* 263B | 179 | 0xB3 */         MP_Dispatch_EscOpcodeTRAP,  /* RIPPLEXOR */
    /* 264B | 180 | 0xB4 */         MP_Dispatch_EscOpcodeTRAP,
    /* 265B | 181 | 0xB5 */         MP_Dispatch_EscOpcodeTRAP,
    /* 266B | 182 | 0xB6 */         MP_Dispatch_EscOpcodeTRAP,
    /* 267B | 183 | 0xB7 */         MP_Dispatch_EscOpcodeTRAP,
    /* 270B | 184 | 0xB8 */         MP_Dispatch_EscOpcodeTRAP,
    /* 271B | 185 | 0xB9 */         MP_Dispatch_EscOpcodeTRAP,
    /* 272B | 186 | 0xBA */         MP_Dispatch_EscOpcodeTRAP,
    /* 273B | 187 | 0xBB */         MP_Dispatch_EscOpcodeTRAP,
    /* 274B | 188 | 0xBC */         MP_Dispatch_EscOpcodeTRAP,
    /* 275B | 189 | 0xBD */         MP_Dispatch_EscOpcodeTRAP,
    /* 276B | 190 | 0xBE */         MP_Dispatch_EscOpcodeTRAP,
    /* 277B | 191 | 0xBF */         MP_Dispatch_EscOpcodeTRAP,

        /* More machine dependent instructions - required for Tajo */

    /* 300B | 192 | 0xC0 */         MP_Dispatch_EscOpcodeTRAP,  /* COLORBLT */
    /* 301B | 193 | 0xC1 */         MP_Dispatch_EscOpcodeTRAP,  /* WRITEPIXEL */
    /* 302B | 194 | 0xC2 */         MP_Dispatch_EscOpcodeTRAP,  /* BITBLTX */
    /* 303B | 195 | 0xC3 */         MP_Dispatch_EscOpcodeTRAP,
    /* 304B | 196 | 0xC4 */         MP_Dispatch_EscOpcodeTRAP,
    /* 305B | 197 | 0xC5 */         MP_Dispatch_EscOpcodeTRAP,
    /* 306B | 198 | 0xC6 */         MP_Dispatch_EscOpcodeTRAP,
    /* 307B | 199 | 0xC7 */         MP_Dispatch_EscOpcodeTRAP,
    /* 310B | 200 | 0xC8 */         MP_Dispatch_EscOpcodeTRAP,
    /* 311B | 201 | 0xC9 */         MP_Dispatch_EscOpcodeTRAP,
    /* 312B | 202 | 0xCA */         MP_Dispatch_EscOpcodeTRAP,
    /* 313B | 203 | 0xCB */         MP_Dispatch_EscOpcodeTRAP,
    /* 314B | 204 | 0xCC */         MP_Dispatch_EscOpcodeTRAP,
    /* 315B | 205 | 0xCD */         MP_Dispatch_EscOpcodeTRAP,
    /* 316B | 206 | 0xCE */         MP_Dispatch_EscOpcodeTRAP,
    /* 317B | 207 | 0xCF */         MP_Dispatch_EscOpcodeTRAP,

    /* 320B | 208 | 0xD0 */         MP_Dispatch_EscOpcodeTRAP,
    /* 321B | 209 | 0xD1 */         MP_Dispatch_EscOpcodeTRAP,
    /* 322B | 210 | 0xD2 */         MP_Dispatch_EscOpcodeTRAP,
    /* 323B | 211 | 0xD3 */         MP_Dispatch_EscOpcodeTRAP,
    /* 324B | 212 | 0xD4 */         MP_Dispatch_EscOpcodeTRAP,
    /* 325B | 213 | 0xD5 */         MP_Dispatch_EscOpcodeTRAP,
    /* 326B | 214 | 0xD6 */         MP_Dispatch_EscOpcodeTRAP,
    /* 327B | 215 | 0xD7 */         MP_Dispatch_EscOpcodeTRAP,
    /* 330B | 216 | 0xD8 */         MP_Dispatch_EscOpcodeTRAP,
    /* 331B | 217 | 0xD9 */         MP_Dispatch_EscOpcodeTRAP,
    /* 332B | 218 | 0xDA */         MP_Dispatch_EscOpcodeTRAP,
    /* 333B | 219 | 0xDB */         MP_Dispatch_EscOpcodeTRAP,
    /* 334B | 220 | 0xDC */         MP_Dispatch_EscOpcodeTRAP,
    /* 335B | 221 | 0xDD */         MP_Dispatch_EscOpcodeTRAP,
    /* 336B | 222 | 0xDE */         MP_Dispatch_EscOpcodeTRAP,
    /* 337B | 223 | 0xDF */         MP_Dispatch_EscOpcodeTRAP,

    /* 340B | 224 | 0xE0 */         MP_Dispatch_EscOpcodeTRAP,
    /* 341B | 225 | 0xE1 */         MP_Dispatch_EscOpcodeTRAP,  /* Unpack10p - Additional FP */
    /* 342B | 226 | 0xE2 */         MP_Dispatch_EscOpcodeTRAP,  /* DeNorm - Additional FP */
    /* 343B | 227 | 0xE3 */         MP_Dispatch_EscOpcodeTRAP,  /* UsualPack - Additional FP */
    /* 344B | 228 | 0xE4 */         MP_Dispatch_EscOpcodeTRAP,
    /* 345B | 229 | 0xE5 */         MP_Dispatch_EscOpcodeTRAP,
    /* 346B | 230 | 0xE6 */         MP_Dispatch_EscOpcodeTRAP,
    /* 347B | 231 | 0xE7 */         MP_Dispatch_EscOpcodeTRAP,
    /* 350B | 232 | 0xE8 */         MP_Dispatch_EscOpcodeTRAP,
    /* 351B | 233 | 0xE9 */         MP_Dispatch_EscOpcodeTRAP,
    /* 352B | 234 | 0xEA */         MP_Dispatch_EscOpcodeTRAP,
    /* 353B | 235 | 0xEB */         MP_Dispatch_EscOpcodeTRAP,
    /* 354B | 236 | 0xEC */         MP_Dispatch_EscOpcodeTRAP,
    /* 355B | 237 | 0xED */         MP_Dispatch_EscOpcodeTRAP,
    /* 356B | 238 | 0xEE */         MP_Dispatch_EscOpcodeTRAP,
    /* 357B | 239 | 0xEF */         MP_Dispatch_EscOpcodeTRAP,

    /* 360B | 240 | 0xF0 */         MP_Dispatch_EscOpcodeTRAP,
    /* 361B | 241 | 0xF1 */         MP_Dispatch_EscOpcodeTRAP,
    /* 362B | 242 | 0xF2 */         MP_Dispatch_EscOpcodeTRAP,
    /* 363B | 243 | 0xF3 */         MP_Dispatch_EscOpcodeTRAP,
    /* 364B | 244 | 0xF4 */         MP_Dispatch_EscOpcodeTRAP,
    /* 365B | 245 | 0xF5 */         MP_Dispatch_EscOpcodeTRAP,
    /* 366B | 246 | 0xF6 */         MP_Dispatch_EscOpcodeTRAP,
    /* 367B | 247 | 0xF7 */         MP_Dispatch_EscOpcodeTRAP,
    /* 370B | 248 | 0xF8 */         MP_Dispatch_EscOpcodeTRAP,
    /* 371B | 249 | 0xF9 */         MP_Dispatch_EscOpcodeTRAP,
    /* 372B | 250 | 0xFA */         MP_Dispatch_EscOpcodeTRAP,
    /* 373B | 251 | 0xFB */         MP_Dispatch_EscOpcodeTRAP,
    /* 374B | 252 | 0xFC */         MP_Dispatch_EscOpcodeTRAP,
    /* 375B | 253 | 0xFD */         MP_Dispatch_EscOpcodeTRAP,
    /* 376B | 254 | 0xFE */         MP_Dispatch_EscOpcodeTRAP,
    /* 377B | 255 | 0xFF */         MP_Dispatch_EscOpcodeTRAP
};

#if 0
InstructionFunction * MP_Mesa_OpcodeEscL_Table[256] =
{
    /* 000B | 000 | 0x00 */         0,
    /* 001B | 001 | 0x01 */         0,
    /* 002B | 002 | 0x02 */         0,
    /* 003B | 003 | 0x03 */         0,
    /* 004B | 004 | 0x04 */         0,
    /* 005B | 005 | 0x05 */         0,
    /* 006B | 006 | 0x06 */         0,
    /* 007B | 007 | 0x07 */         0,
    /* 010B | 008 | 0x08 */         0,
    /* 011B | 009 | 0x09 */         0,
    /* 012B | 010 | 0x0A */         0,
    /* 013B | 011 | 0x0B */         0,
    /* 014B | 012 | 0x0C */         0,
    /* 015B | 013 | 0x0D */         0,
    /* 016B | 014 | 0x0E */         0,
    /* 017B | 015 | 0x0F */         0,

    /* 020B | 016 | 0x10 */         0,
    /* 021B | 017 | 0x11 */         0,
    /* 022B | 018 | 0x12 */         0,
    /* 023B | 019 | 0x13 */         0,
    /* 024B | 020 | 0x14 */         0,
    /* 025B | 021 | 0x15 */         0,
    /* 026B | 022 | 0x16 */         0,
    /* 027B | 023 | 0x17 */         0,
    /* 030B | 024 | 0x18 */         0,
    /* 031B | 025 | 0x19 */         0,
    /* 032B | 026 | 0x1A */         0,
    /* 033B | 027 | 0x1B */         0,
    /* 034B | 028 | 0x1C */         0,
    /* 035B | 029 | 0x1D */         0,
    /* 036B | 030 | 0x1E */         0,
    /* 037B | 031 | 0x1F */         0,

    /* 040B | 032 | 0x20 */         0,
    /* 041B | 033 | 0x21 */         0,
    /* 042B | 034 | 0x22 */         0,
    /* 043B | 035 | 0x23 */         0,
    /* 044B | 036 | 0x24 */         0,
    /* 045B | 037 | 0x25 */         0,
    /* 046B | 038 | 0x26 */         0,
    /* 047B | 039 | 0x27 */         0,
    /* 050B | 040 | 0x28 */         0,
    /* 051B | 041 | 0x29 */         0,
    /* 052B | 042 | 0x2A */         0,
    /* 053B | 043 | 0x2B */         0,
    /* 054B | 044 | 0x2C */         0,
    /* 055B | 045 | 0x2D */         0,
    /* 056B | 046 | 0x2E */         0,
    /* 057B | 047 | 0x2F */         0,

    /* 060B | 048 | 0x30 */         0,
    /* 061B | 049 | 0x31 */         0,
    /* 062B | 050 | 0x32 */         0,
    /* 063B | 051 | 0x33 */         0,
    /* 064B | 052 | 0x34 */         0,
    /* 065B | 053 | 0x35 */         0,
    /* 066B | 054 | 0x36 */         0,
    /* 067B | 055 | 0x37 */         0,
    /* 070B | 056 | 0x38 */         0,
    /* 071B | 057 | 0x39 */         0,
    /* 072B | 058 | 0x3A */         0,
    /* 073B | 059 | 0x3B */         0,
    /* 074B | 060 | 0x3C */         0,
    /* 075B | 061 | 0x3D */         0,
    /* 076B | 062 | 0x3E */         0,
    /* 077B | 063 | 0x3F */         0,

    /* 100B | 064 | 0x40 */         0,
    /* 101B | 065 | 0x41 */         0,
    /* 102B | 066 | 0x42 */         0,
    /* 103B | 067 | 0x43 */         0,
    /* 104B | 068 | 0x44 */         0,
    /* 105B | 069 | 0x45 */         0,
    /* 106B | 070 | 0x46 */         0,
    /* 107B | 071 | 0x47 */         0,
    /* 110B | 072 | 0x48 */         0,
    /* 111B | 073 | 0x49 */         0,
    /* 112B | 074 | 0x4A */         0,
    /* 113B | 075 | 0x4B */         0,
    /* 114B | 076 | 0x4C */         0,
    /* 115B | 077 | 0x4D */         0,
    /* 116B | 078 | 0x4E */         0,
    /* 117B | 079 | 0x4F */         0,

    /* 120B | 080 | 0x50 */         0,
    /* 121B | 081 | 0x51 */         0,
    /* 122B | 082 | 0x52 */         0,
    /* 123B | 083 | 0x53 */         0,
    /* 124B | 084 | 0x54 */         0,
    /* 125B | 085 | 0x55 */         0,
    /* 126B | 086 | 0x56 */         0,
    /* 127B | 087 | 0x57 */         0,
    /* 130B | 088 | 0x58 */         0,
    /* 131B | 089 | 0x59 */         0,
    /* 132B | 090 | 0x5A */         0,
    /* 133B | 091 | 0x5B */         0,
    /* 134B | 092 | 0x5C */         0,
    /* 135B | 093 | 0x5D */         0,
    /* 136B | 094 | 0x5E */         0,
    /* 137B | 095 | 0x5F */         0,

    /* 140B | 096 | 0x60 */         0,
    /* 141B | 097 | 0x61 */         0,
    /* 142B | 098 | 0x62 */         0,
    /* 143B | 099 | 0x63 */         0,
    /* 144B | 100 | 0x64 */         0,
    /* 145B | 101 | 0x65 */         0,
    /* 146B | 102 | 0x66 */         0,
    /* 147B | 103 | 0x67 */         0,
    /* 150B | 104 | 0x68 */         0,
    /* 151B | 105 | 0x69 */         0,
    /* 152B | 106 | 0x6A */         0,
    /* 153B | 107 | 0x6B */         0,
    /* 154B | 108 | 0x6C */         0,
    /* 155B | 109 | 0x6D */         0,
    /* 156B | 110 | 0x6E */         0,
    /* 157B | 111 | 0x6F */         0,

    /* 160B | 112 | 0x70 */         0,
    /* 161B | 113 | 0x71 */         0,
    /* 162B | 114 | 0x72 */         0,
    /* 163B | 115 | 0x73 */         0,
    /* 164B | 116 | 0x74 */         0,
    /* 165B | 117 | 0x75 */         0,
    /* 166B | 118 | 0x76 */         0,
    /* 167B | 119 | 0x77 */         0,
    /* 170B | 120 | 0x78 */         0,
    /* 171B | 121 | 0x79 */         0,
    /* 172B | 122 | 0x7A */         0,
    /* 173B | 123 | 0x7B */         0,
    /* 174B | 124 | 0x7C */         0,
    /* 175B | 125 | 0x7D */         0,
    /* 176B | 126 | 0x7E */         0,
    /* 177B | 127 | 0x7F */         0,

    /* 200B | 128 | 0x80 */         0,
    /* 201B | 129 | 0x81 */         0,
    /* 202B | 130 | 0x82 */         0,
    /* 203B | 131 | 0x83 */         0,
    /* 204B | 132 | 0x84 */         0,
    /* 205B | 133 | 0x85 */         0,
    /* 206B | 134 | 0x86 */         0,
    /* 207B | 135 | 0x87 */         0,
    /* 210B | 136 | 0x88 */         0,
    /* 211B | 137 | 0x89 */         0,
    /* 212B | 138 | 0x8A */         0,
    /* 213B | 139 | 0x8B */         0,
    /* 214B | 140 | 0x8C */         0,
    /* 215B | 141 | 0x8D */         0,
    /* 216B | 142 | 0x8E */         0,
    /* 217B | 143 | 0x8F */         0,

    /* 220B | 144 | 0x90 */         0,
    /* 221B | 145 | 0x91 */         0,
    /* 222B | 146 | 0x92 */         0,
    /* 223B | 147 | 0x93 */         0,
    /* 224B | 148 | 0x94 */         0,
    /* 225B | 149 | 0x95 */         0,
    /* 226B | 150 | 0x96 */         0,
    /* 227B | 151 | 0x97 */         0,
    /* 230B | 152 | 0x98 */         0,
    /* 231B | 153 | 0x99 */         0,
    /* 232B | 154 | 0x9A */         0,
    /* 233B | 155 | 0x9B */         0,
    /* 234B | 156 | 0x9C */         0,
    /* 235B | 157 | 0x9D */         0,
    /* 236B | 158 | 0x9E */         0,
    /* 237B | 159 | 0x9F */         0,

    /* 240B | 160 | 0xA0 */         0,
    /* 241B | 161 | 0xA1 */         0,
    /* 242B | 162 | 0xA2 */         0,
    /* 243B | 163 | 0xA3 */         0,
    /* 244B | 164 | 0xA4 */         0,
    /* 245B | 165 | 0xA5 */         0,
    /* 246B | 166 | 0xA6 */         0,
    /* 247B | 167 | 0xA7 */         0,
    /* 250B | 168 | 0xA8 */         0,
    /* 251B | 169 | 0xA9 */         0,
    /* 252B | 170 | 0xAA */         0,
    /* 253B | 171 | 0xAB */         0,
    /* 254B | 172 | 0xAC */         0,
    /* 255B | 173 | 0xAD */         0,
    /* 256B | 174 | 0xAE */         0,
    /* 257B | 175 | 0xAF */         0,

    /* 260B | 176 | 0xB0 */         0,
    /* 261B | 177 | 0xB1 */         0,
    /* 262B | 178 | 0xB2 */         0,
    /* 263B | 179 | 0xB3 */         0,
    /* 264B | 180 | 0xB4 */         0,
    /* 265B | 181 | 0xB5 */         0,
    /* 266B | 182 | 0xB6 */         0,
    /* 267B | 183 | 0xB7 */         0,
    /* 270B | 184 | 0xB8 */         0,
    /* 271B | 185 | 0xB9 */         0,
    /* 272B | 186 | 0xBA */         0,
    /* 273B | 187 | 0xBB */         0,
    /* 274B | 188 | 0xBC */         0,
    /* 275B | 189 | 0xBD */         0,
    /* 276B | 190 | 0xBE */         0,
    /* 277B | 191 | 0xBF */         0,

    /* 300B | 192 | 0xC0 */         0,
    /* 301B | 193 | 0xC1 */         0,
    /* 302B | 194 | 0xC2 */         0,
    /* 303B | 195 | 0xC3 */         0,
    /* 304B | 196 | 0xC4 */         0,
    /* 305B | 197 | 0xC5 */         0,
    /* 306B | 198 | 0xC6 */         0,
    /* 307B | 199 | 0xC7 */         0,
    /* 310B | 200 | 0xC8 */         0,
    /* 311B | 201 | 0xC9 */         0,
    /* 312B | 202 | 0xCA */         0,
    /* 313B | 203 | 0xCB */         0,
    /* 314B | 204 | 0xCC */         0,
    /* 315B | 205 | 0xCD */         0,
    /* 316B | 206 | 0xCE */         0,
    /* 317B | 207 | 0xCF */         0,

    /* 320B | 208 | 0xD0 */         0,
    /* 321B | 209 | 0xD1 */         0,
    /* 322B | 210 | 0xD2 */         0,
    /* 323B | 211 | 0xD3 */         0,
    /* 324B | 212 | 0xD4 */         0,
    /* 325B | 213 | 0xD5 */         0,
    /* 326B | 214 | 0xD6 */         0,
    /* 327B | 215 | 0xD7 */         0,
    /* 330B | 216 | 0xD8 */         0,
    /* 331B | 217 | 0xD9 */         0,
    /* 332B | 218 | 0xDA */         0,
    /* 333B | 219 | 0xDB */         0,
    /* 334B | 220 | 0xDC */         0,
    /* 335B | 221 | 0xDD */         0,
    /* 336B | 222 | 0xDE */         0,
    /* 337B | 223 | 0xDF */         0,

    /* 340B | 224 | 0xE0 */         0,
    /* 341B | 225 | 0xE1 */         0,
    /* 342B | 226 | 0xE2 */         0,
    /* 343B | 227 | 0xE3 */         0,
    /* 344B | 228 | 0xE4 */         0,
    /* 345B | 229 | 0xE5 */         0,
    /* 346B | 230 | 0xE6 */         0,
    /* 347B | 231 | 0xE7 */         0,
    /* 350B | 232 | 0xE8 */         0,
    /* 351B | 233 | 0xE9 */         0,
    /* 352B | 234 | 0xEA */         0,
    /* 353B | 235 | 0xEB */         0,
    /* 354B | 236 | 0xEC */         0,
    /* 355B | 237 | 0xED */         0,
    /* 356B | 238 | 0xEE */         0,
    /* 357B | 239 | 0xEF */         0,

    /* 360B | 240 | 0xF0 */         0,
    /* 361B | 241 | 0xF1 */         0,
    /* 362B | 242 | 0xF2 */         0,
    /* 363B | 243 | 0xF3 */         0,
    /* 364B | 244 | 0xF4 */         0,
    /* 365B | 245 | 0xF5 */         0,
    /* 366B | 246 | 0xF6 */         0,
    /* 367B | 247 | 0xF7 */         0,
    /* 370B | 248 | 0xF8 */         0,
    /* 371B | 249 | 0xF9 */         0,
    /* 372B | 250 | 0xFA */         0,
    /* 373B | 251 | 0xFB */         0,
    /* 374B | 252 | 0xFC */         0,
    /* 375B | 253 | 0xFD */         0,
    /* 376B | 254 | 0xFE */         0,
    /* 377B | 255 | 0xFF */         0
};
#endif

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/
