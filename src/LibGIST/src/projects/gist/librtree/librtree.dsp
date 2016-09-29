# Microsoft Developer Studio Project File - Name="librtree" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=librtree - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "librtree.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "librtree.mak" CFG="librtree - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "librtree - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "librtree - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "librtree - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "$(LIBGISTHOME)/include" /I "$(JDKINSTALL)/include" /I "$(JDKINSTALL)/include/win32" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "AMDB" /YX /FD /TP /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "librtree - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "$(LIBGISTHOME)/include" /I "$(JDKINSTALL)/include" /I "$(JDKINSTALL)/include/win32" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "AMDB" /YX /FD /GZ /TP /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\lib\librtree.lib"

!ENDIF 

# Begin Target

# Name "librtree - Win32 Release"
# Name "librtree - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\librtree\amdb_rstartree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\amdb_rtree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_ball.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rrtree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rstartree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtpred.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtpred_point.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtpred_rect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtpred_rr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtpred_sphere.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtpred_sr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtpred_ss.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_rtreecext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_sptree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_srtree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\librtree\gist_sstree.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
