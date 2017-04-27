#**********************************************************************
#* Copyright (C) 1999-2003, International Business Machines Corporation
#* and others.  All Rights Reserved.
#**********************************************************************
# nmake file for creating data files on win32
# invoke with
# nmake /f makedata.mak icup=<path_to_icu_instalation> [Debug|Release]
#
#	12/10/1999	weiv	Created

#If no config, we default to debug
!IF "$(CFG)" == ""
CFG=Debug
!MESSAGE No configuration specified. Defaulting to common - Win32 Debug.
!ENDIF


# this test is pointless in an automated build system. It might make sense if
# this make file was run stand-alone, but as part of the Firebird/ICU component
# it will probably never be built alone. CFG errors, if any, will be picked up
# long before. However, as we need to diff against the original ICU code
# we probably need to keep this code in place
!if "$(CFG)" == "xyz"
#Here we test if a valid configuration is given
!IF "$(CFG)" != "Release" && "$(CFG)" != "release" && "$(CFG)" != "Debug" && "$(CFG)" != "debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "makedata.mak" CFG="Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "Release"
!MESSAGE "Debug"
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF
!ENDIF

#Let's see if user has given us a path to ICU
#This could be found according to the path to makefile, but for now it is this way
!IF "$(ICUP)"==""
!ERROR Can't find path!
!ENDIF
!MESSAGE ICU path is $(ICUP)
!MESSAGE CFG is $(CFG)
RESNAME=uconvmsg
RESDIR=.
RESFILES=resfiles.mk
ICUDATA=$(ICUP)\data

DLL_OUTPUT=.\$(CFG)
# set the following to 'static' or 'dll' depending
PKGMODE=static


ICD=$(ICUDATA)^\
DATA_PATH=$(ICUP)\data^\

!IF "$(CFG)" == "Release" || "$(CFG)" == "release"  || "$(CFG)" == "Debug" || "$(CFG)" == "debug"
ICUTOOLS=$(ICUP)\bin
!ELSE
ICUTOOLS=$(ICUP)\$(CFG)\bin
!ENDIF

PATH = $(PATH);$(ICUP)\bin

# Suffixes for data files
.SUFFIXES : .ucm .cnv .dll .dat .res .txt .c

# We're including a list of resource files.
FILESEPCHAR=\\

!IF EXISTS("$(RESFILES)")
!INCLUDE "$(RESFILES)"
!ELSE
!ERROR ERROR: cannot find "$(RESFILES)"
!ENDIF
RB_FILES = $(RESSRC:.txt=.res)

# This target should build all the data files
!IF "$(PKGMODE)" == "dll"
OUTPUT = "$(DLL_OUTPUT)\$(RESNAME).dll"
!ELSE
OUTPUT = "$(DLL_OUTPUT)\$(RESNAME).lib"
!ENDIF

ALL : $(OUTPUT)
	@echo All targets are up to date (mode $(PKGMODE))


# invoke pkgdata - static
"$(DLL_OUTPUT)\$(RESNAME).lib" : $(RB_FILES) $(RESFILES)
	@echo Building $(RESNAME).lib
	@"$(ICUTOOLS)\pkgdata" -f -v -m static -c -p $(RESNAME) -d "$(DLL_OUTPUT)" -s "$(RESDIR)" <<pkgdatain.txt
$(RB_FILES:.res =.res
)
<<KEEP

# This is to remove all the data files
CLEAN :
    -@erase "$(RB_FILES)"
	-@erase "$(RESDIR)\uconvmsg*.*"
	-@erase "$(CFG)\*uconvmsg*.*"
    -@"$(ICUTOOLS)\pkgdata" -f --clean -v -m static -c -p $(RESNAME) -d "$(DLL_OUTPUT)" -s "$(RESDIR)" pkgdatain.txt

# Inference rule for creating resource bundles
.txt.res:
	@echo Making Resource Bundle files
	"$(ICUTOOLS)\genrb" -t -p $(RESNAME) -s $(@D) -d $(@D) $(?F)


$(RESSRC) : {"$(ICUTOOLS)"}genrb.exe

