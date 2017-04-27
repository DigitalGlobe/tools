# Microsoft Developer Studio Generated NMAKE File, Based on vb_dll.dsp
!IF "$(CFG)" == ""
CFG=vb_dll - Win32 Debug
!MESSAGE No configuration specified. Defaulting to vb_dll - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "vb_dll - Win32 Release" && "$(CFG)" != "vb_dll - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vb_dll.mak" CFG="vb_dll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vb_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vb_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "vb_dll - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vb_dll.dll"

!ELSE 

ALL : "$(OUTDIR)\vb_dll.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vb_dll.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\vb_dll.dll"
	-@erase "$(OUTDIR)\vb_dll.exp"
	-@erase "$(OUTDIR)\vb_dll.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\vb_dll.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vb_dll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=lmgr.lib lm_new.obj libsb.lib libcrvs.lib kernel32.lib\
 user32.lib gdi32.lib comdlg32.lib advapi32.lib netapi32.lib comctl32.lib\
 wsock32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\vb_dll.pdb" /machine:I386 /def:".\vb_dll.def"\
 /out:"$(OUTDIR)\vb_dll.dll" /implib:"$(OUTDIR)\vb_dll.lib" 
DEF_FILE= \
	".\vb_dll.def"
LINK32_OBJS= \
	"$(INTDIR)\vb_dll.obj"

"$(OUTDIR)\vb_dll.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "vb_dll - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\vb_dll.dll"

!ELSE 

ALL : "$(OUTDIR)\vb_dll.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vb_dll.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\vb_dll.dll"
	-@erase "$(OUTDIR)\vb_dll.exp"
	-@erase "$(OUTDIR)\vb_dll.ilk"
	-@erase "$(OUTDIR)\vb_dll.lib"
	-@erase "$(OUTDIR)\vb_dll.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\vb_dll.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\vb_dll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=lmgr.lib lm_new.obj libsb.lib libcrvs.lib kernel32.lib\
 user32.lib gdi32.lib comdlg32.lib advapi32.lib netapi32.lib comctl32.lib\
 wsock32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\vb_dll.pdb" /debug /machine:I386 /nodefaultlib:"libcmtd.lib"\
 /def:".\vb_dll.def" /out:"$(OUTDIR)\vb_dll.dll" /implib:"$(OUTDIR)\vb_dll.lib"\
 /pdbtype:sept 
DEF_FILE= \
	".\vb_dll.def"
LINK32_OBJS= \
	"$(INTDIR)\vb_dll.obj"

"$(OUTDIR)\vb_dll.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "vb_dll - Win32 Release" || "$(CFG)" == "vb_dll - Win32 Debug"
SOURCE=.\vb_dll.c

!IF  "$(CFG)" == "vb_dll - Win32 Release"

DEP_CPP_VB_DL=\
	".\lm_attr.h"\
	".\lmclient.h"\
	".\vb_dll.h"\
	{$(INCLUDE)}"lm_cro.h"\
	

"$(INTDIR)\vb_dll.obj" : $(SOURCE) $(DEP_CPP_VB_DL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "vb_dll - Win32 Debug"

DEP_CPP_VB_DL=\
	".\lm_attr.h"\
	".\lmclient.h"\
	".\vb_dll.h"\
	{$(INCLUDE)}"lm_cro.h"\
	

"$(INTDIR)\vb_dll.obj" : $(SOURCE) $(DEP_CPP_VB_DL) "$(INTDIR)"


!ENDIF 


!ENDIF 

