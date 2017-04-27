## Makefile for icudt30l (icudt30) created by pkgdata
## from ICU Version 3.0

NAME=icudt30l
CNAME=icudt30l
LIBNAME=icudt30
SRCDIR=.
TARGETDIR=C:\tools\src\firebird\extern\icu\source\data\out\build\icudt30l
TEMP_DIR=C:\tools\src\firebird\extern\icu\source\data\out\tmp
MODE=dll
MAKEFILE=C:\tools\src\firebird\extern\icu\source\data\out\tmp\icudt30l_dll.mak
ENTRYPOINT=icudt30
TARGET_VERSION=(null)
MKINSTALLDIRS=mkdir



## List files [1] containing data files to process (note: - means stdin)
LISTFILES= pkgdatain.txt


## Data Files [67]
DATAFILES= unorm.icu uprops.icu pnames.icu unames.icu ucadata.icu invuca.icu uidna.spp \
 cnvalias.icu ibm-37_P100-1995.cnv ibm-1047_P100-1995.cnv gb18030.cnv \
 ibm-1386_P100-2002.cnv ibm-943_P15A-2003.cnv icu-internal-25546.cnv \
 ibm-942_P12A-1999.cnv ibm-943_P130-1999.cnv windows-874-2000.cnv \
 windows-936-2000.cnv res_index.res root.res pt_BR.res t_Any_Accents.res \
 t_Any_Publishing.res t_Arab_Latn.res t_Beng_InterIndic.res t_Cyrl_Latn.res \
 t_Deva_InterIndic.res t_FWidth_HWidth.res t_Grek_Latn.res t_Grek_Latn_UNGEGN.res \
 t_Gujr_InterIndic.res t_Guru_InterIndic.res t_Hani_Latn.res t_Hebr_Latn.res \
 t_Hira_Kana.res t_Hira_Latn.res t_InterIndic_Beng.res t_InterIndic_Deva.res \
 t_InterIndic_Gujr.res t_InterIndic_Guru.res t_InterIndic_Knda.res \
 t_InterIndic_Latn.res t_InterIndic_Mlym.res t_InterIndic_Orya.res \
 t_InterIndic_Taml.res t_InterIndic_Telu.res t_Knda_InterIndic.res \
 t_Latn_InterIndic.res t_Latn_Jamo.res t_Latn_Kana.res t_Mlym_InterIndic.res \
 t_Orya_InterIndic.res t_Taml_InterIndic.res t_Telu_InterIndic.res \
 t_Latn_NPinyn.res t_Tone_Digit.res t_Hani_SpHan.res translit_index.res \
 coll/root.res coll/res_index.res sent.brk char.brk line.brk word.brk \
 title.brk line_th.brk word_th.brk


## Data File Paths [67]
DATAFILEPATHS= ".\unorm.icu" ".\uprops.icu" ".\pnames.icu" ".\unames.icu" ".\ucadata.icu" ".\invuca.icu" \
 ".\uidna.spp" ".\cnvalias.icu" ".\ibm-37_P100-1995.cnv" ".\ibm-1047_P100-1995.cnv" \
 ".\gb18030.cnv" ".\ibm-1386_P100-2002.cnv" ".\ibm-943_P15A-2003.cnv" ".\icu-internal-25546.cnv" \
 ".\ibm-942_P12A-1999.cnv" ".\ibm-943_P130-1999.cnv" ".\windows-874-2000.cnv" \
 ".\windows-936-2000.cnv" ".\res_index.res" ".\root.res" ".\pt_BR.res" ".\t_Any_Accents.res" \
 ".\t_Any_Publishing.res" ".\t_Arab_Latn.res" ".\t_Beng_InterIndic.res" \
 ".\t_Cyrl_Latn.res" ".\t_Deva_InterIndic.res" ".\t_FWidth_HWidth.res" \
 ".\t_Grek_Latn.res" ".\t_Grek_Latn_UNGEGN.res" ".\t_Gujr_InterIndic.res" \
 ".\t_Guru_InterIndic.res" ".\t_Hani_Latn.res" ".\t_Hebr_Latn.res" ".\t_Hira_Kana.res" \
 ".\t_Hira_Latn.res" ".\t_InterIndic_Beng.res" ".\t_InterIndic_Deva.res" \
 ".\t_InterIndic_Gujr.res" ".\t_InterIndic_Guru.res" ".\t_InterIndic_Knda.res" \
 ".\t_InterIndic_Latn.res" ".\t_InterIndic_Mlym.res" ".\t_InterIndic_Orya.res" \
 ".\t_InterIndic_Taml.res" ".\t_InterIndic_Telu.res" ".\t_Knda_InterIndic.res" \
 ".\t_Latn_InterIndic.res" ".\t_Latn_Jamo.res" ".\t_Latn_Kana.res" ".\t_Mlym_InterIndic.res" \
 ".\t_Orya_InterIndic.res" ".\t_Taml_InterIndic.res" ".\t_Telu_InterIndic.res" \
 ".\t_Latn_NPinyn.res" ".\t_Tone_Digit.res" ".\t_Hani_SpHan.res" ".\translit_index.res" \
 ".\coll\root.res" ".\coll\res_index.res" ".\sent.brk" ".\char.brk" ".\line.brk" \
 ".\word.brk" ".\title.brk" ".\line_th.brk" ".\word_th.brk"


ICUROOT=C:\tools\src\firebird\extern\icu\Win32\Release\bin

GENCMN = $(ICUROOT)\gencmn.exe
# DLL file to make:
DLLTARGET=icudt30.dll

LINK32 = link.exe
LINK32_FLAGS = /nologo /out:"$(TARGETDIR)\$(DLLTARGET)" /DLL /NOENTRY /base:"0x4ad00000" /implib:"$(TARGETDIR)\$(LIBNAME).lib" /comment:" Copyright (C) 2004, International Business Machines Corporation and others. All Rights Reserved. "
GENCCODE = $(ICUROOT)\genccode.exe

# Windows specific DLL version information.
!IF EXISTS("$(TEMP_DIR)\icudata.res")
DATA_VER_INFO="$(TEMP_DIR)\icudata.res"
!ELSE
DATA_VER_INFO=
!ENDIF

# intermediate obj file:
CMNOBJTARGET=$(NAME)_dat.obj

# common file to make:
CMNTARGET=icudt30l.dat

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
unorm.icu
uprops.icu
pnames.icu
unames.icu
ucadata.icu
invuca.icu
uidna.spp
cnvalias.icu
ibm-37_P100-1995.cnv
ibm-1047_P100-1995.cnv
gb18030.cnv
ibm-1386_P100-2002.cnv
ibm-943_P15A-2003.cnv
icu-internal-25546.cnv
ibm-942_P12A-1999.cnv
ibm-943_P130-1999.cnv
windows-874-2000.cnv
windows-936-2000.cnv
res_index.res
root.res
pt_BR.res
t_Any_Accents.res
t_Any_Publishing.res
t_Arab_Latn.res
t_Beng_InterIndic.res
t_Cyrl_Latn.res
t_Deva_InterIndic.res
t_FWidth_HWidth.res
t_Grek_Latn.res
t_Grek_Latn_UNGEGN.res
t_Gujr_InterIndic.res
t_Guru_InterIndic.res
t_Hani_Latn.res
t_Hebr_Latn.res
t_Hira_Kana.res
t_Hira_Latn.res
t_InterIndic_Beng.res
t_InterIndic_Deva.res
t_InterIndic_Gujr.res
t_InterIndic_Guru.res
t_InterIndic_Knda.res
t_InterIndic_Latn.res
t_InterIndic_Mlym.res
t_InterIndic_Orya.res
t_InterIndic_Taml.res
t_InterIndic_Telu.res
t_Knda_InterIndic.res
t_Latn_InterIndic.res
t_Latn_Jamo.res
t_Latn_Kana.res
t_Mlym_InterIndic.res
t_Orya_InterIndic.res
t_Taml_InterIndic.res
t_Telu_InterIndic.res
t_Latn_NPinyn.res
t_Tone_Digit.res
t_Hani_SpHan.res
translit_index.res
coll/root.res
coll/res_index.res
sent.brk
char.brk
line.brk
word.brk
title.brk
line_th.brk
word_th.brk
<<


# End of makefile for icudt30l [dll mode]

