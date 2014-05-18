# Microsoft Developer Studio Project File - Name="Dawn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Dawn - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Dawn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Dawn.mak" CFG="Dawn - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Dawn - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Dawn - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Dawn"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Dawn - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Targets\Release\Obj\Dawn"
# PROP Intermediate_Dir "..\..\Targets\Release\Obj\Dawn"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "Resource" /I "../../Include" /D "_WINDOWS" /D "_MP_BY_THE_BOOK" /D "_MP_INLINE_OFF" /D "WIN32" /D "NDEBUG" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\Targets\Release\Lib\PrincOps.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\Targets\Release\Bin\Dawn.exe"

!ELSEIF  "$(CFG)" == "Dawn - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Targets\Debug\Obj\Dawn"
# PROP Intermediate_Dir "..\..\Targets\Debug\Obj\Dawn"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "Resource" /I "../../Include" /D "_WINDOWS" /D "_MP_BY_THE_BOOK" /D "_MP_INLINE_OFF" /D "WIN32" /D "_DEBUG" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\Targets\Debug\Lib\PrincOps.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\Targets\Debug\Bin\Dawn.exe"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "Dawn - Win32 Release"
# Name "Dawn - Win32 Debug"
# Begin Group "AppFramework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppFramework\DawnAboutBox.cpp
# End Source File
# Begin Source File

SOURCE=.\AppFramework\DawnAboutBox.h
# End Source File
# Begin Source File

SOURCE=.\AppFramework\DawnApp.cpp
# End Source File
# Begin Source File

SOURCE=.\AppFramework\DawnApp.h
# End Source File
# Begin Source File

SOURCE=.\AppFramework\DawnFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AppFramework\DawnFrame.h
# End Source File
# Begin Source File

SOURCE=.\AppFramework\DawnView.cpp
# End Source File
# Begin Source File

SOURCE=.\AppFramework\DawnView.h
# End Source File
# Begin Source File

SOURCE=.\AppFramework\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\AppFramework\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\AppFramework\XNSDriverClient.cpp
# End Source File
# Begin Source File

SOURCE=.\AppFramework\XNSDriverClient.h
# End Source File
# End Group
# Begin Group "MesaAgent"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MesaAgent\AgentBeep.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentBeep.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentDisk.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentDisk.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentDisplay.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentFloppy.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentFloppy.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentMouse.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentNetwork.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentParallel.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentParallel.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentProcessor.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentSerial.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentSerial.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentStream.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentStream.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentTTY.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\AgentTTY.h
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\MesaAgent.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=.\MesaAgent\MesaAgent.h
# End Source File
# End Group
# Begin Group "MesaProcessor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MesaProcessor\MesaMicrocode.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\MesaProcessor\MesaMicrocode.h
# End Source File
# Begin Source File

SOURCE=.\MesaProcessor\MesaMicrocodeOps.cpp
# End Source File
# Begin Source File

SOURCE=.\MesaProcessor\MesaMicrocodeOps.h
# End Source File
# Begin Source File

SOURCE=.\MesaProcessor\MesaProcessor.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\MesaProcessor\MesaProcessor.h
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource\Dawn.germ
# End Source File
# Begin Source File

SOURCE=.\Resource\Dawn.ico
# End Source File
# Begin Source File

SOURCE=.\Resource\Dawn.rc
# End Source File
# Begin Source File

SOURCE=.\Resource\Dawn.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource\mpcode.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource\Toolbar.bmp
# End Source File
# End Group
# End Target
# End Project
