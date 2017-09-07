# Microsoft Developer Studio Project File - Name="MM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MM.mak" CFG="MM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MM - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MM - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib glut32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MM - Win32 Release"
# Name "MM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClosedSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\Cone.cpp
# End Source File
# Begin Source File

SOURCE=.\Cuboid.cpp
# End Source File
# Begin Source File

SOURCE=.\Cylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\Knife.cpp
# End Source File
# Begin Source File

SOURCE=.\MM.cpp
# End Source File
# Begin Source File

SOURCE=.\MM.rc
# End Source File
# Begin Source File

SOURCE=.\MMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelModify.cpp
# End Source File
# Begin Source File

SOURCE=.\Pipe.cpp
# End Source File
# Begin Source File

SOURCE=.\Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Search.cpp
# End Source File
# Begin Source File

SOURCE=.\Sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\StatePanel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Torus.cpp
# End Source File
# Begin Source File

SOURCE=.\Triangulation.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClosedSurface.h
# End Source File
# Begin Source File

SOURCE=.\Cone.h
# End Source File
# Begin Source File

SOURCE=.\Cuboid.h
# End Source File
# Begin Source File

SOURCE=.\Cylinder.h
# End Source File
# Begin Source File

SOURCE=.\Knife.h
# End Source File
# Begin Source File

SOURCE=.\MM.h
# End Source File
# Begin Source File

SOURCE=.\MMDlg.h
# End Source File
# Begin Source File

SOURCE=.\OutputDlg.h
# End Source File
# Begin Source File

SOURCE=.\PanelCreate.h
# End Source File
# Begin Source File

SOURCE=.\PanelModify.h
# End Source File
# Begin Source File

SOURCE=.\Pipe.h
# End Source File
# Begin Source File

SOURCE=.\Queue.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Search.h
# End Source File
# Begin Source File

SOURCE=.\Sphere.h
# End Source File
# Begin Source File

SOURCE=.\StatePanel.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Torus.h
# End Source File
# Begin Source File

SOURCE=.\Triangulation.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\frame.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\MM.rc2
# End Source File
# Begin Source File

SOURCE=.\res\new.ico
# End Source File
# Begin Source File

SOURCE=.\res\open.ico
# End Source File
# Begin Source File

SOURCE=.\res\output.ico
# End Source File
# Begin Source File

SOURCE=.\res\save.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
