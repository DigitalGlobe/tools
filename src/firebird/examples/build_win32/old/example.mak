# The contents of this file are subject to the Interbase Public
# License Version 1.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy
# of the License at http://www.Inprise.com/IPL.html
#
# Software distributed under the License is distributed on an
# "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
# or implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code was created by Inprise Corporation
# and its predecessors. Portions created by Inprise Corporation are
# Copyright (C) Inprise Corporation.
#
# All Rights Reserved.
# Contributor(s): ______________________________________.
#------------------------------------------------------------------------
# EXAMPLES make file
#
#    To use Borland C 3.x or 4.x type the following on the command line:
#              "make -fexample.mak <example program>"
#                 e.g. make -fexample.mak api1.exe
#
#    To use MicroSoft C 7.0 type the following on the command line:
#                  "nmake MSFT=  /f example.mak <example program>" 
#                    e.g. nmake MSFT=  /f example.mak api1.exe           
#
#------------------------------------------------------------------------

INC_PATH=..\include

!ifdef MSFT

CC=cl
LINK=link
RC=rc

#------------------------------------------------------------------------
# MicroSoft C compiler and link flags
# The QCFLAGS and QWINLIB macros exist because a console app in MicroSoft
# C requires a special compile flag, /Mq (Quick Win app), and a special
# link library, llibcewq.
#------------------------------------------------------------------------

QCFLAGS=/c /AL /Ge /Zi /Mq /Od /G2 /Zp1 /W3 /I$(INC_PATH)
CFLAGS=/c /AL /Ge /Zi /Od /GA /G2 /Zp1 /W3 /I$(INC_PATH)
LFLAGS=/M /NOD /CO
QWINLIB=..\lib\gds libw llibcewq
WINLIB=..\lib\gds libw llibcew

!else

CC=bcc
LINK=tlink
RC=brc 

#------------------------------------------------------------------------
# Borland C compiler and link flags
# The QCFLAGS and QWINLIB macros exist because a console app in MicroSoft 
# C requires a special compile flag, /Mq (Quick Win app), and a special 
# link library, llibcewq.
#------------------------------------------------------------------------

CFLAGS=-c -ml -N -v -Od -WE -2  -I$(INC_PATH) 
QCFLAGS=$(CFLAGS)
LFLAGS=/n /m /Twe /v c0wl  
WINLIB=..\lib\gds import mathwl cwl 
QWINLIB=$(WINLIB)

!endif
       
.c.obj:
   $(CC) $(QCFLAGS) $<

TARGETS = winevent.exe api1.exe api2.exe api3.exe api4.exe api5.exe \
	  api6.exe api7.exe api8.exe api9.exe api10.exe api11.exe api12.exe \
	  api13.exe api15.exe api16t.exe apifull.exe

all: $(TARGETS)
     
winevent.obj : winevent.c $(INC_PATH)\ibase.h 
   $(CC) $(CFLAGS) winevent.c

winevent.exe : winevent.obj winevent.def winevent.res
     $(LINK) $(LFLAGS) winevent.obj, winevent.exe,, $(WINLIB), winevent.def
     $(RC) winevent.res

winevent.res : winevent.rc
     $(RC) -r winevent.rc

api1.exe: api1.obj example.def example.h $(INC_PATH)\ibase.h
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api2.exe: api2.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api3.exe: api3.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api4.exe: api4.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api5.exe: api5.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api6.exe: api6.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api7.exe: api7.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api8.exe: api8.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api9.exe: api9.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api10.exe: api10.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api11.exe: api11.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api12.exe: api12.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api13.exe: api13.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api15.exe: api15.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

api16t.exe: api16t.obj example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) api16t.obj, api16t.exe,, $(QWINLIB), example.def

apifull.exe: apifull.obj align.h example.def example.h  $(INC_PATH)\ibase.h 
     $(LINK) $(LFLAGS) $*.obj, $*.exe,, $(QWINLIB), example.def

clean:
     del *.obj                             
     del *.exe
     del *.res
     del *.map
