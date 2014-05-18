; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=DawnView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "dawn.h"
LastPage=0

ClassCount=4
Class1=DawnAboutBox
Class2=DawnApp
Class3=DawnFrame
Class4=DawnView

ResourceCount=2
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME

[CLS:DawnAboutBox]
Type=0
BaseClass=CDialog
HeaderFile=\Mesa\Source\Dawn\AppFramework\DawnAboutBox.h
ImplementationFile=\Mesa\Source\Dawn\AppFramework\DawnAboutBox.cpp
LastObject=DawnAboutBox

[CLS:DawnApp]
Type=0
BaseClass=CWinApp
HeaderFile=\Mesa\Source\Dawn\AppFramework\DawnApp.h
ImplementationFile=\Mesa\Source\Dawn\AppFramework\DawnApp.cpp
LastObject=DawnApp
Filter=N
VirtualFilter=AC

[CLS:DawnFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=\Mesa\Source\Dawn\AppFramework\DawnFrame.h
ImplementationFile=\Mesa\Source\Dawn\AppFramework\DawnFrame.cpp
LastObject=DawnFrame
Filter=T
VirtualFilter=fWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=DawnAboutBox
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_FILE_PRINT
Command5=ID_APP_BYTE_SWAP_VIRTUAL_DISK_FILE
Command6=ID_APP_START_DAWN
CommandCount=6

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_APP_EXIT
Command2=ID_EDIT_UNDO
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

