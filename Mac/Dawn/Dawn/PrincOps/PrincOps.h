/*
    PrincOps.h - main public header file of PrincOps implementation

    Copyright (c) 2001, Don Woodward.  All rights reserved.

*******************************************************************************/

#ifndef _INCLUDE_PRINCOPS_PRINCOPS_H
#define _INCLUDE_PRINCOPS_PRINCOPS_H

// GJC: Turn off inlining of functions for now.
// #define _MP_INLINE_OFF

#ifdef __cplusplus
extern "C" {
#endif

    
    
#include "PrincOps_PlatformTypes.h"

/*******************************************************************************
** PRINCOPS TYPEDEFS */
typedef void (PrincOps_Instruction_Function)();
    /* PrincOps_Instruction_Function is the type of a PrincOps instruction function.
       Create your own PrincOps_Instruction_Functions and install them into the PrincOps
       implementation using PrincOps_InstallInstruction or one of its variants.
       NOTE: #include <PrincOps/PrincOps_Microcode.h> in your source files where you
       implement new instruction functions.
    */

typedef u16 (PrincOps_GetInterruptMask_Function)();
typedef void (PrincOps_SetInterruptMask_Function)(u16 nInterruptMask);
typedef void (PrincOps_WriteMPCode_Function)(u16 mpCode);
typedef u32 (PrincOps_GetPulseCount_Function)();
typedef struct _PrincOps_Configuration_Struct
{
    u16 *                                   pStartOfRealMemory;
    u32                                     nNumRealMemoryPages;
    u32                                     nNumVirtualMemoryPages;
    u32                                     nNumDeviceIOPages;
    u32 *                                   pVMMap;
    u32                                     nNumVMMapPages;
    PrincOps_GetInterruptMask_Function *    pGetInterruptMaskFunction;
    PrincOps_SetInterruptMask_Function *    pSetInterruptMaskFunction;
    PrincOps_WriteMPCode_Function *         pWriteMPCodeFunction;
    PrincOps_GetPulseCount_Function *       pGetPulseCountFunction;
    u32                                     nPulseResolutionInMicroseconds;

}      PrincOps_Configuration_Struct;
    /* PrincOps_Configuration_Struct contains the fields necessary to
       configure the PrincOps implementation.  The fields are as follows:
       pStartOfRealMemory           - The start of real memory for the PrincOps implementation.
                                      This must be a 16bit word aligned address of allocated memory.
       nNumRealMemoryPages          - The number of pages allocated at pStartOfRealMemory.
                                      Each page equals 512 bytes.  Valid values are from 1536
                                      to 65536. (768Kbytes to 32Mbytes).  This is the amount of real memory
                                      that will be available to the processor as RAM.
       nNumVirtualMemoryPages       - The number of virtual pages to be used by the processor.  Valid values
                                      are from nNumRealMemoryPages to 65536.
       nNumDeviceIOPages            - The number of actual pages of memory to be used for device
                                      IO in the processor - must be from 1 to 256.
       pVMMap                       - A 32bit word aligned pointer to at least 256K of memory that the
                                    - processor will use for its virtual memory map.
       nNumVMMapPages               - The number of 512 byte pages allocated at pVMMap.  This number must
                                      be at least 512.  (512 pages == 256K bytes).  NOTE: The processor
                                      will use exactly 512 pages.  You can pass in more, but the excess
                                      will not be used by the processor.
       pGetInterruptMaskFunction    - A pointer to a function which will return a 16 bit word with
                                      the current interrupt mask for the PrincOps processor.
                                      NOTE that you (the client) must ensure that setting of the
                                      interrupt mask and reading of the interrupt mask is performed
                                      atomically.
       pWriteMPCodeFunction         - A pointer to a function which will accept the current maintainence panel
                                      code.
       pGetPulseCountFunction       - A pointer to a function which will return the current pulse count
                                      as a 32 bit word.  Each pulse count represents an elapsed time of
                                      between 1 and 100 microseconds.
       nPulseResolutionInMicroseconds The number of microseconds each pulse count represents.
                                      This number must be between 1 and 100.
    */

typedef struct _PrincOps_EtherBootFileNumber
{
    u16 a;
    u16 b;
    u16 c;
} PrincOps_EtherBootFileNumber;

typedef struct _PrincOps_NetworkAddress
{
    struct { u16 a; u16 b; }                        net;
    struct { u16 a; u16 b; u16 c; }                 host;
    u16                                             socket;
} PrincOps_NetworkAddress;

typedef struct _PrincOps_PilotDiskAddress
{
    u16 cylinder;
    u16 sector;
    u16 head;
} PrincOps_PilotDiskAddress;

typedef struct _PrincOps_DiskFileID
{
    struct { u16 a; u16 b; u16 c; u16 d; u16 e; }   pilotDiskFileID;
    u32                                             firstFilePageNumber;
    PrincOps_PilotDiskAddress                       diskAddress;
} PrincOps_DiskFileID;

typedef union _PrincOps_BootLocation
{   struct
    {
		PrincOps_EtherBootFileNumber                bootFileNumber;
		PrincOps_NetworkAddress                     address;
	}                                   ethernet;
    PrincOps_DiskFileID                 disk;
} PrincOps_BootLocation;


#define PrincOps_CurrentRequestBasicVersion     (1838)
#define PrincOps_BootActionInLoad               (0)
#define PrincOps_BootActionOutLoad              (1)
#define PrincOps_BootActionBootPhysicalVolume   (2)
#define PrincOps_BootActionTeledebug            (3)
#define PrincOps_BootActionNoOp                 (4)
#define PrincOps_DeviceTypeEthernet             (6)
#define PrincOps_DeviceTypeAnyPilotDisk         (64)
#define PrincOps_DeviceTypeAnyFloppy            (17)
#define PrincOps_DeviceTypeStream               (4000)

typedef struct _PrincOps_BootRequest
{
	u16                                 requestBasicVersion;
	u16                                 bootAction;
	u16                                 deviceType;
	u16                                 deviceOrdinal;
	PrincOps_BootLocation               bootLocation;
	u16                                 requestExtensionVersion;
} PrincOps_BootRequest;

/*******************************************************************************
** PRINCOPS FUNCTIONS */
u8 PrincOps_Configure(PrincOps_Configuration_Struct * pConfigurationStruct);
    /*  Call PrincOps_Configure() to configure the PrincOps processor. 
        PrincOps_Configure() returns 1 if successful, zero otherwise.
        NOTE: PrincOps_Configure must be called PRIOR to calling
        PrincOps_LoadGermData() and then PrincOps_Run().
    */

PrincOps_Instruction_Function * PrincOps_InstallInstruction(u8 opcode, PrincOps_Instruction_Function * pInstructionFunction);
PrincOps_Instruction_Function * PrincOps_InstallInstructionEsc(u8 opcodeEsc, PrincOps_Instruction_Function * pInstructionFunction);
PrincOps_Instruction_Function * PrincOps_InstallInstructionEscL(u8 opcodeEscL, PrincOps_Instruction_Function * pInstructionFunction);
    /*  Call PrincOps_InstallInstruction() or one of its variants to substitute the instruction function
        located at opcode (opcodeEsc, opcodeEscL) with the instruction function passed in.
        PrincOps_InstallInstruction() and its variants return the pointer to the instruction function
        that previously existed at said opcode location.  NOTE: All calls to PrincOps_InstallInstruction()
        and its variants must be made PRIOR to calling PrincOps_Run().
    */

u8 PrincOps_LoadGermData(const u8 * pGermData, u32 nGermDataSize, u32 nGermPage);
    /* Call PrincOps_LoadGermData() to stuff pGermData into processor memory.
       Call PrincOps_LoadGermData() once for each page of germ data to be stuffed.
       Usually nGermDataSize will be 512 (the number of bytes in a page), except for
       the last page of germ data which may be less than 512.  nGermPage must be set
       to the page of the germ data being loaded (starting with 0).  Usually two calls
       will suffice: PrincOps_LoadGermData(pGermData, 512, 0) and
       PrincOps_LoadGermData(pGermData, 512 * (nGermPages-2) + nLastGermPageSize, 1).
       PrincOps_LoadGermData() must only be called after a successful call to PrincOps_Configure(). 
       PrincOps_LoadGermData() returns 1 if successful, zero otherwise.
    */

void PrincOps_MakeDiskBootRequest(PrincOps_BootRequest * pBootRequest);
void PrincOps_MakeFloppyBootRequest(PrincOps_BootRequest * pBootRequest);
void PrincOps_MakeEthernetBootRequest(PrincOps_BootRequest * pBootRequest);
void PrincOps_MakeStreamBootRequest(PrincOps_BootRequest * pBootRequest);
    /* PrincOps_MakeDiskBootRequest(), PrincOps_MakeFloppyBootRequest(),
       PrincOps_MakeEthernetBootRequest() and PrincOps_MakeStreamBootRequest()
       are convenience functions that initialize pBootRequest with default parameters
       specifying booting from Disk, Floppy, Ethernet and Stream respectively.  Use
       these functions to setup your boot request, customize your boot request to
       taste, then call PrincOps_SetGermBootRequest() to set your boot request.
    */

u8 PrincOps_SetGermBootRequest(PrincOps_BootRequest * pBootRequest);
    /* Call PrincOps_SetGermBootRequest() to setup the germ's boot request
       parameters.  PrincOps_SetGermBootRequest() must only be called after
       successfully loading the germ with PrincOps_LoadGermData().
       PrincOps_SetGermBootRequest() returns 1 if successful, zero otherwise.
    */

u8 PrincOps_SetGermBootSwitches(const char * pBootSwitches);
    /* Call PrincOps_SetGermBootSwitches() to set the boot switches
       for the germ.  PrincOps_SetBootRequest() must only be called after
       successfully loading the germ with PrincOps_LoadGermData().
       PrincOps_SetGermBootSwitches() returns 1 if successful, zero otherwise.
    */

u8 PrincOps_Run(); 
    /*  Call PrincOps_Run() to start execution of the PrincOps processor.
        PrincOps_Run() returns 1 if successful, zero otherwise.
    */

u8 PrincOps_Halt(); 
    /*  Call PrincOps_Halt() to halt execution of the PrincOps processor.
        PrincOps_Halt() returns 1 if successful, zero otherwise.
    */

u8 PrincOps_IsRunning();
    /* PrincOps_IsRunning() returns 1 if the PrincOps processor is currently executing,
       zero otherwise.
    */

#ifdef __cplusplus 
}
#endif

#endif /* #ifndef _INCLUDE_PRINCOPS_PRINCOPS_H */

/*******************************************************************************
    LOG
********************************************************************************
09-Aug-01   dwoodward   created

*/

