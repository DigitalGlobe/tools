# Microsoft Developer Studio Project File - Name="libgist" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libgist - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libgist.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libgist.mak" CFG="libgist - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libgist - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libgist - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libgist - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "$(LIBGISTHOME)\include" /I "$(JDKINSTALL)\include" /I "$(JDKINSTALL)\include\win32" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "AMDB" /YX /FD /TP /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libgist - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "$(LIBGISTHOME)\include" /I "$(JDKINSTALL)\include" /I "$(JDKINSTALL)\include\win32" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "AMDB" /FR /YX /FD /GZ /TP /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\lib\libgist.lib"

!ENDIF 

# Begin Target

# Name "libgist - Win32 Release"
# Name "libgist - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\libgist\amdb_analysis.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_clustering.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_defs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_idxstruct.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_itemset.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_penaltystats.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_splitstats.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_support.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_treemap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_wkldprofile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\amdb_wkldstats.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_amdb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_cursor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_cursorext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_disppredcursor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_htab.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_p.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_query.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_support.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_unordered.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_unorderedn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\gist_ustk.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\libgist\vec_t.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
