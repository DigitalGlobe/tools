## Makefile for uconvmsg (uconvmsg) created by pkgdata
## from ICU Version 3.0

NAME=uconvmsg
CNAME=uconvmsg
LIBNAME=uconvmsg
SRCDIR=.
TARGETDIR=.\Win32\Release
TEMP_DIR=.\Win32\Release
MODE=static
MAKEFILE=.\Win32\Release\uconvmsg_static.mak
ENTRYPOINT=uconvmsg
TARGET_VERSION=(null)
MKINSTALLDIRS=mkdir



## List files [1] containing data files to process (note: - means stdin)
LISTFILES= pkgdatain.txt


## Data Files [2]
DATAFILES= resources\\root.res resources\\fr.res


## Data File Paths [2]
DATAFILEPATHS= ".\resources\\root.res" ".\resources\\fr.res"


ICUROOT=C:\tools\src\firebird\extern\icu\Win32\Release\bin

GENCMN = $(ICUROOT)\gencmn.exe
# LIB file to make:
DLLTARGET=uconvmsg.LIB

LINK32 = LIB.exe
LINK32_FLAGS = /nologo /out:"$(TARGETDIR)\$(DLLTARGET)" /EXPORT:"uconvmsg"
GENCCODE = $(ICUROOT)\genccode.exe
# intermediate obj file
CMNOBJTARGET=$(NAME)_dat.obj

# common file to make:
CMNTARGET=uconvmsg.dat

all: "$(TARGETDIR)\$(DLLTARGET)"

"$(TARGETDIR)\$(DLLTARGET)": "$(TEMP_DIR)\$(CMNOBJTARGET)"
	$(LINK32) $(LINK32_FLAGS) "$(TEMP_DIR)\$(CMNOBJTARGET)" $(DATA_VER_INFO)

"$(TEMP_DIR)\$(CMNOBJTARGET)": "$(TARGETDIR)\$(CMNTARGET)"
	@"$(GENCCODE)" $(GENCOPTIONS) -e $(ENTRYPOINT) -o -d "$(TEMP_DIR)" "$(TARGETDIR)\$(CMNTARGET)"

clean:
	-@erase "$(TARGETDIR)\$(DLLTARGET)"
	-@erase "$(TARGETDIR)\$(CMNOBJTARGET)"
	-@erase "$(TARGETDIR)\$(CMNTARGET)"

install: "$(TARGETDIR)\$(DLLTARGET)"
	copy "$(TARGETDIR)\$(DLLTARGET)" "$(INSTALLTO)\$(DLLTARGET)"

rebuild: clean all

"$(TARGETDIR)\$(CMNTARGET)" : $(DATAFILEPATHS)
	"$(GENCMN)" -C " Copyright (C) 2004, International Business Machines Corporation and others. All Rights Reserved. " -d "$(TARGETDIR)"  -s "$(SRCDIR)" -n "$(NAME)" 0 <<
resources\\root.res
resources\\fr.res
<<


# End of makefile for uconvmsg [static mode]

