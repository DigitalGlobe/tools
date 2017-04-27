#########################################################################
#	Copyright (c) 2003-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved
#        This software has been provided pursuant to a License Agreement
#        containing restrictions on its use.  This software contains
#        valuable trade secrets and proprietary information of 
#        Macrovision Corporation and is protected by law.  It may 
#        not be copied or distributed in any form or medium, disclosed 
#        to third parties, reverse engineered or used in any manner not 
#        provided for in said License Agreement except with the prior 
#        written authorization from Macrovision Corporation.
##########################################################################
#
# FLEXLM makefile for PCs
# If you have a real kit, change VENDORNAME=demo below to your real
# vendor daemon name
# Use nmake DLL=1 for dll clients
# Use nmake MD=1 for dynamic runtime library: msvcrt.lib
#

VENDORNAME = sanzew
DAEMON = $(VENDORNAME).exe

SRCDIR = ..\machind
!ifdef DEBUG
XTRACFLAG=/Zi /Od
LDFLAGS=/DEBUG 
CNAME=d
!else
XTRACFLAG =  /O1
LDFLAGS = 
CNAME=
!endif

LD = LINK /nologo /NODEFAULTLIB /OPT:NOREF $(LDFLAGS)
RC = RC

INCFLAGS = /I$(SRCDIR) /I.

ACTSTUB = .\activation\lib\libnoact$(SUFFIX).lib


!ifdef DLL

LMGRNAME=11
LIBSUFFIX=
DLL_FLAG=/DFLEXLM_DLL
CRT_LIB=libcmt$(CNAME).lib
LMNEW_OBJ=lm_new.obj
LMNEW_OBJ_CLIENT=
CRT_FLAG=/MT$(CNAME)

!else
!ifdef MD

LMGRNAME=_md
LIBSUFFIX=_md
LMNEW_OBJ=lm_new_md.obj 
CRT_FLAG=/MD$(CNAME)
CRT_LIB=msvcrt$(CNAME).lib 
LMNEW_OBJ_CLIENT=$(LMNEW_OBJ)

!else

LMGRNAME=
LIBSUFFIX=
CRT_FLAG=/MT$(CNAME)
LMNEW_OBJ=lm_new.obj
LMNEW_OBJ_CLIENT=$(LMNEW_OBJ)
CRT_LIB=libcmt$(CNAME).lib

!endif

SUFFIX=$(LIBSUFFIX)

!endif

LMGR_LIB=lmgr$(LMGRNAME).lib

XTRALIB1 =  oldnames.lib kernel32.lib user32.lib netapi32.lib \
        advapi32.lib  gdi32.lib comdlg32.lib  comctl32.lib wsock32.lib

XTRALIB =  $(XTRALIB1) $(CRT_LIB)

XTRACFLAG=$(XTRACFLAG) $(DLL_FLAG)

#
#	Use XTRAOBJS to define your object files for vendor daemon
#	initialization routines, checkout filter, checkin callback, etc.
#
XTRAOBJS = 

CFLAGS_NOOPT = /nologo /c  $(INCFLAGS) $(CRT_FLAG) $(XTRACFLAG)
CFLAGS = $(CFLAGS_NOOPT)  /DNO_ACTIVATION_SUPPORT
LMNEW_CFLAGS = /nologo /c  $(INCFLAGS) $(CRT_FLAG) $(DLL_FLAG)

SRCS	= $(SRCDIR)\lsvendor.c

EXECS =  lmflex.exe lmcrypt.exe installs.exe

CLIENTLIB        = $(LMGR_LIB) libsb$(SUFFIX).lib libcrvs$(SUFFIX).lib 
STATIC_CLIENTLIB = lmgr$(SUFFIX).lib libsb$(SUFFIX).lib libcrvs$(SUFFIX).lib 
LIBS             = lmgras$(SUFFIX).lib lmgrs$(SUFFIX).lib $(STATIC_CLIENTLIB)
DAEMONLIBS       = lmgras$(SUFFIX).lib lmgrs$(SUFFIX).lib lmgr$(SUFFIX).lib libsb$(SUFFIX).lib libcrvs$(SUFFIX).lib
EVENTLOGMESSAGEFILE = VendorLicenseServerMsgs
EVENTLOGMESSAGEFILE2 = MvsnLicenseServerMsgs

all: 	\
!ifdef DLL
     lmgr11.dll \
!endif
     $(EXECS) $(DAEMON) utils

daemon:	$(DAEMON)

$(DAEMON): $(XTRAOBJS) $(DAEMONLIBS) lsvendor.obj $(SRCDIR)\lsserver.h $(LMNEW_OBJ)
           $(LD) /subsystem:console /out:$(DAEMON) lsvendor.obj $(LMNEW_OBJ) \
           	$(XTRAOBJS) $(DAEMONLIBS) $(CRT_LIB) $(XTRALIB1) $(ACTSTUB)

lsvendor.obj: lm_new.obj
        $(CC) $(CFLAGS) -I../h /Fo$@ lsrvend.c 
 
$(LMNEW_OBJ): $(SRCDIR)\lsvendor.c $(SRCDIR)\lm_code.h 
	lmrand1 -i $(SRCDIR)\lsvendor.c
	$(CC) /c $(LMNEW_CFLAGS) -I../h lmcode.c
	$(LD) /subsystem:CONSOLE lmnewgen$(SUFFIX).obj lmcode.obj $(ACTSTUB) \
        lmgr$(SUFFIX).lib libcrvs$(SUFFIX).lib libsb$(SUFFIX).lib $(XTRALIB1) $(CRT_LIB) /out:lmnewgen.exe
	if exist lm_new.c del lm_new.c
	lmnewgen.exe $(VENDORNAME) -o lm_new.c
    	$(CC) $(LMNEW_CFLAGS) /Fo$(LMNEW_OBJ) lm_new.c 

lmcrypt.exe: $(SRCDIR)\lmcrypt.c \
		$(SRCDIR)\lmclient.h $(SRCDIR)\lm_code.h $(STATIC_CLIENTLIB) 
	$(CC) $(CFLAGS) $(SRCDIR)\lmcrypt.c
	$(LD) /out:lmcrypt.exe lmcrypt.obj $(STATIC_CLIENTLIB) $(ACTSTUB) $(XTRALIB)

lmflex.exe: $(SRCDIR)\lmflex.c  $(LMNEW_OBJ) \
            $(CLIENTLIB) 
	$(CC) $(CFLAGS) $(SRCDIR)\lmflex.c
	$(LD) /out:lmflex.exe lmflex.obj $(LMNEW_OBJ_CLIENT) $(CLIENTLIB) $(ACTSTUB) $(XTRALIB)
	if exist lmflex.obj del lmflex.obj
   
installs.exe: $(SRCDIR)\installs.c
        $(CC) $(CFLAGS) $(SRCDIR)\installs.c
        $(LD) /out:installs.exe installs.obj $(XTRALIB)
	if exist installs.obj del installs.obj

lmgr11.dll: $(LMNEW_OBJ) $(SRCDIR)\lmgr11.c
	$(CC) $(CFLAGS) $(SRCDIR)\lmgr11.c
        $(LD) /def:"$(SRCDIR)\lmgr11.def" \
              /out:lmgr11.dll lmgr11.obj /implib:"lmgr11.lib" /dll \
              $(LMNEW_OBJ) \
	      lmgr$(SUFFIX).lib libsb$(SUFFIX).lib libcrvs$(SUFFIX).lib \
              $(ACTSTUB) $(XTRALIB)
             

buildmsgfile:;
	if exist $(EVENTLOGMESSAGEFILE).dll copy $(EVENTLOGMESSAGEFILE).dll $(EVENTLOGMESSAGEFILE).dll_bak
	mc -u -U $(SRCDIR)\$(EVENTLOGMESSAGEFILE).mc
	$(RC) -r -fo $(EVENTLOGMESSAGEFILE).res $(EVENTLOGMESSAGEFILE).rc
	$(LD) /dll /noentry /machine:x86 /out:$(EVENTLOGMESSAGEFILE).dll $(EVENTLOGMESSAGEFILE).res
	if exist $(EVENTLOGMESSAGEFILE2).dll copy $(EVENTLOGMESSAGEFILE2).dll $(EVENTLOGMESSAGEFILE2).dll_bak
	mc -u -U $(SRCDIR)\$(EVENTLOGMESSAGEFILE2).mc
	$(RC) -r -fo $(EVENTLOGMESSAGEFILE2).res $(EVENTLOGMESSAGEFILE2).rc
	$(LD) /dll /noentry /machine:x86 /out:$(EVENTLOGMESSAGEFILE2).dll $(EVENTLOGMESSAGEFILE2).res

clean:;	
        if exist lmnewgen.obj ren lmnewgen.obj lmnewgen.sav
        if exist lmnewgen_md.obj ren lmnewgen_md.obj lmnewgen_md.sav
        if exist asrgen.obj copy asrgen.obj asrgen_obj.bak
        if exist *.obj del *.obj
        if exist lmnewgen_md.sav ren lmnewgen_md.sav lmnewgen_md.obj
        if exist lmnewgen.sav ren lmnewgen.sav lmnewgen.obj
        if exist asrgen_obj.bak move asrgen_obj.bak asrgen.obj
	if exist lmnewgen.exe del lmnewgen.exe 
        if exist lmcrypt.exe del lmcrypt.exe  
        if exist $(DAEMON) del $(DAEMON)
        if exist lmcode.c del lmcode.c 
        if exist lmcode.obj del lmcode.obj 
        if exist lm_new.c del lm_new*.c 
        if exist lm_new.obj del lm_new*.obj 
        if exist lsrvend.c del lsrvend.c 
        if exist lmflex.exe del lmflex.exe
	if exist *.ilk del *.ilk
	if exist vc60.pdb rename vc60.pdb vc60.sav
	if exist *.p* del *.p*
	if exist vc60.sav rename vc60.sav vc60.pdb
	if exist $(VENDORNAME)$(LMGRNAME).lib del $(VENDORNAME)$(LMGRNAME).lib
	if exist $(VENDORNAME).dll del $(VENDORNAME).dll
	if exist $(VENDORNAME).exp del $(VENDORNAME).exp
	if exist lmgr11.dll del lmgr11.dll
	if exist lmgr11.exp del lmgr11.exp
	if exist lmgr11.lib del lmgr11.lib
	if exist *.bin del *.bin
	if exist $(EVENTLOGMESSAGEFILE).h del $(EVENTLOGMESSAGEFILE).h
	if exist $(EVENTLOGMESSAGEFILE).res del $(EVENTLOGMESSAGEFILE).res
	if exist $(EVENTLOGMESSAGEFILE).rc del $(EVENTLOGMESSAGEFILE).rc
	if exist $(EVENTLOGMESSAGEFILE2).h del $(EVENTLOGMESSAGEFILE2).h
	if exist $(EVENTLOGMESSAGEFILE2).res del $(EVENTLOGMESSAGEFILE2).res
	if exist $(EVENTLOGMESSAGEFILE2).rc del $(EVENTLOGMESSAGEFILE2).rc

veryclean: clean
	if exist lmhostid.exe del lmhostid.exe
	if exist lmver.exe del lmver.exe
	if exist lmcksum.exe del lmcksum.exe
	if exist lmdown.exe del lmdown.exe
	if exist lmremove.exe del lmremove.exe
	if exist lmreread.exe del lmreread.exe
	if exist lmswitchr.exe del lmswitchr.exe
	if exist lmstat.exe del lmstat.exe
	if exist lmborrow.exe del lmborrow.exe
	if exist lmdiag.exe del lmdiag.exe
	if exist lminstall.exe del lminstall.exe

kitclean:
	if exist lmcode.c del lmcode.c
	if exist lmcode.obj del lmcode.obj
	if exist lmcrypt.obj del lmcrypt.obj
	if exist lm_new.c del lm_new.c
	if exist lm_new.obj del lm_new.obj
	if exist lmnewgen.exe del lmnewgen.exe
	if exist lsvendor.obj del lsvendor.obj
	if exist lmhostid.exe del lmhostid.exe
	if exist lmver.exe del lmver.exe
	if exist lmcksum.exe del lmcksum.exe
	if exist lmdown.exe del lmdown.exe
	if exist lmhostid.exe del lmhostid.exe
	if exist lminstall.exe del lminstall.exe
	if exist lmremove.exe del lmremove.exe
	if exist lmreread.exe del lmreread.exe
	if exist  lmswitchr.exe del  lmswitchr.exe
	if exist lmstat.exe del lmstat.exe
	if exist lmdiag.exe del lmdiag.exe
	if exist lmver.exe  del lmver.exe 
	if exist lmflex.exe  del lmflex.exe 
	if exist lmgr11.dll del lmgr11.dll
	if exist lmgr11.exp del lmgr11.exp
	if exist lmgr11.lib del lmgr11.lib
	if exist *.bin del *.bin
	if exist $(EVENTLOGMESSAGEFILE).h del $(EVENTLOGMESSAGEFILE).h
	if exist $(EVENTLOGMESSAGEFILE).res del $(EVENTLOGMESSAGEFILE).res
	if exist $(EVENTLOGMESSAGEFILE).rc del $(EVENTLOGMESSAGEFILE).rc
	if exist $(EVENTLOGMESSAGEFILE2).h del $(EVENTLOGMESSAGEFILE2).h
	if exist $(EVENTLOGMESSAGEFILE2).res del $(EVENTLOGMESSAGEFILE2).res
	if exist $(EVENTLOGMESSAGEFILE2).rc del $(EVENTLOGMESSAGEFILE2).rc


lmcrypt.obj:	$(SRCDIR)\lmcrypt.c $(SRCDIR)\lm_code.h $(SRCDIR)\lmclient.h
	$(CC) $(CFLAGS) $(SRCDIR)\lmcrypt.c
	if exist lmcrypt.exe del lmcrypt.exe

utils:	lmhostid.exe lmver.exe lmdown.exe lmremove.exe lmreread.exe lmswitchr.exe lmstat.exe lmdiag.exe lminstall.exe lmborrow.exe lmpath.exe

lmhostid.exe:   lmutil.exe
    copy lmutil.exe lmhostid.exe

lmver.exe:   lmutil.exe
    copy lmutil.exe lmver.exe

lmdown.exe:   lmutil.exe
    copy lmutil.exe lmdown.exe

lmremove.exe:   lmutil.exe
    copy lmutil.exe lmremove.exe

lmswitchr.exe:   lmutil.exe
    copy lmutil.exe lmswitchr.exe

lmreread.exe:   lmutil.exe
    copy lmutil.exe lmreread.exe

lmstat.exe:   lmutil.exe
    copy lmutil.exe lmstat.exe

lmborrow.exe:   lmutil.exe
    copy lmutil.exe lmborrow.exe

lmpath.exe:   lmutil.exe
    copy lmutil.exe lmpath.exe

lmdiag.exe:   lmutil.exe
    copy lmutil.exe lmdiag.exe

lminstall.exe:   lmutil.exe
    copy lmutil.exe lminstall.exe

        copy lmutil.exe lmhostid.exe
        copy lmutil.exe lmver.exe
        copy lmutil.exe lmdown.exe
        copy lmutil.exe lmremove.exe
        copy lmutil.exe lmreread.exe
        copy lmutil.exe lmswitchr.exe
        copy lmutil.exe lmstat.exe
        copy lmutil.exe lmborrow.exe
        copy lmutil.exe lmdiag.exe
        copy lmutil.exe lminstall.exe
    	copy lmutil.exe lmpath.exe


