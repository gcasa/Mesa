# Microsoft Developer Studio Project File - Name="PrincOps" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PrincOps - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PrincOps.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PrincOps.mak" CFG="PrincOps - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PrincOps - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PrincOps - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "PrincOps"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PrincOps - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Targets\Release\Obj\PrincOps"
# PROP Intermediate_Dir "..\..\Targets\Release\Obj\PrincOps"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "..\..\Include" /D "_LIB" /D "_MP_BY_THE_BOOK" /D "_MP_INLINE_OFF" /D "WIN32" /D "NDEBUG" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Targets\Release\Lib\PrincOps.lib"

!ELSEIF  "$(CFG)" == "PrincOps - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Targets\Debug\Obj\PrincOps"
# PROP Intermediate_Dir "..\..\Targets\Debug\Obj\PrincOps"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "..\..\Include" /D "_LIB" /D "_MP_BY_THE_BOOK" /D "_MP_INLINE_OFF" /D "WIN32" /D "_DEBUG" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Targets\Debug\Lib\PrincOps.lib"

!ENDIF 

# Begin Target

# Name "PrincOps - Win32 Release"
# Name "PrincOps - Win32 Debug"
# Begin Group "Public"

# PROP Default_Filter ""
# Begin Group "MP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_AssignmentInstructions.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_AssignmentInstructions_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_BlockTransfers.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_BlockTransfers_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_ControlTransfers.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_ControlTransfers_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_DataTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_DataTypes_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_InstructionInterpreter.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_InstructionInterpreter_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_JumpInstructions.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_JumpInstructions_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_MemoryOrganization.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_MemoryOrganization_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_MesaTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_Processes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_Processes_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_ProcessorStruct.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_ProcessorStruct_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_StackInstructions.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_StackInstructions_Inline.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_Undocumented.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\MP\MP_Undocumented_Inline.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Include\PrincOps\PrincOps.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\PrincOps_Microcode.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PrincOps\PrincOps_PlatformTypes.h
# End Source File
# End Group
# Begin Group "Private"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Private\MP__InlineGlue.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_AssignmentInstructions.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_BlockTransfers.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_ControlTransfers.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_InstructionInterpreter.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_JumpInstructions.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_MemoryOrganization.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_Processes.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_ProcessorStruct.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_StackInstructions.c
# End Source File
# Begin Source File

SOURCE=.\Private\MP_Undocumented.c
# End Source File
# Begin Source File

SOURCE=.\Private\PrincOps.c
# End Source File
# End Group
# End Target
# End Project
