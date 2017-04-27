# Makefile (use with nmake) for Fulfill API example on Windows.
# This makefile is shipped to users as part of the Flexlm kit.

# edit the path to your flexlm version.
FLEXLMDIR     = ..\..

PLATFORM      = i86_n3

FLEXLMINCDIR  = $(FLEXLMDIR)\machind
FLEXLMBINDIR  = $(FLEXLMDIR)\$(PLATFORM)

SYSLIBS	      = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib \
		advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib \
		odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib \
		winspool.lib comdlg32.lib advapi32.lib shell32.lib \
		ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib \
		wsock32.lib libcmt.lib

SRC_DIR       = $(FLEXLMDIR)\examples\fulfill
INCLUDES      = -I$(FLEXLMINCDIR) -I$(SRC_DIR)

CC	      = cl
CDEBUG        = 
CFLAGS        = $(INCLUDES) $(CDEBUG) /nologo /MT /GX /O2 \
		/D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD

FULFILL_LIB   = fulfill.lib
PROGRAM       	= fulfill.exe

LDFLAGS       = /nologo /subsystem:console /incremental:no /machine:I386
LD	      = LINK $(LDFLAGS)
LDLIBS        = $(FULFILL_LIB) SSLeay32.lib libeay32.lib $(SYSLIBS)

SRCS          = $(SRC_DIR)\fulfillExample.c
OBJS          = fulfillExample.obj

default:	$(PROGRAM)

run:	$(PROGRAM)
	$(PROGRAM)

$(PROGRAM):	$(OBJS)
	$(LD) $(LDFLAGS) /out:$(PROGRAM) $(OBJS) $(LDLIBS)
	copy $(SRC_DIR)\*.properties .
                
$(OBJS):        $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	if exist $(PROGRAM) del $(PROGRAM)
	if exist *.obj del *.obj
