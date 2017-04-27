## Makefile for testdata (testdata) created by pkgdata
## from ICU Version 3.0

NAME=testdata
CNAME=testdata
LIBNAME=testdata
SRCDIR=C:\tools\src\firebird\extern\icu\source\test\testdata\out\build
TARGETDIR=C:\tools\src\firebird\extern\icu\source\test\testdata\out
TEMP_DIR=C:\tools\src\firebird\extern\icu\source\test\testdata\out\build
MODE=common
MAKEFILE=C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_common.mak
ENTRYPOINT=testdata
TARGET_VERSION=(null)
MKINSTALLDIRS=mkdir



## List files [1] containing data files to process (note: - means stdin)
LISTFILES= C:\Users\TIMTIS~1\AppData\Local\Temp\nmCCF8.tmp


## Data Files [27]
DATAFILES= testdata_casing.res testdata_conversion.res testdata_mc.res testdata_root.res \
 testdata_testtable32.res testdata_te.res testdata_te_IN.res testdata_testtypes.res \
 testdata_icu26_testtypes.res testdata_icu26e_testtypes.res testdata_testempty.res \
 testdata_testaliases.res testdata_icuio.res testdata_iscii.res testdata_DataDrivenCollationTest.res \
 testdata_test.icu testdata_test1.cnv testdata_test3.cnv testdata_test4.cnv \
 testdata_test4x.cnv testdata_ibm9027.cnv testdata_idna_rules.res \
 testdata_nfscsi.spp testdata_nfscss.spp testdata_nfscis.spp testdata_nfsmxs.spp \
 testdata_nfsmxp.spp


## Data File Paths [27]
DATAFILEPATHS= "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_casing.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_conversion.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_mc.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_root.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_testtable32.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_te.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_te_IN.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_testtypes.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_icu26_testtypes.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_icu26e_testtypes.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_testempty.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_testaliases.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_icuio.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_iscii.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_DataDrivenCollationTest.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test.icu" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test1.cnv" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test3.cnv" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test4.cnv" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test4x.cnv" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_ibm9027.cnv" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_idna_rules.res" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfscsi.spp" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfscss.spp" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfscis.spp" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfsmxs.spp" \
 "C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfsmxp.spp"


ICUROOT=C:\tools\src\firebird\extern\icu\Win32\Release\bin

GENCMN = $(ICUROOT)\gencmn.exe
# common file to make:
CMNTARGET=testdata.dat

all: "$(TARGETDIR)\$(CMNTARGET)"

clean:
	-@erase "$(TARGETDIR)\$(CMNTARGET)"

install: "$(TARGETDIR)\$(CMNTARGET)"
	copy "$(TARGETDIR)\$(CMNTARGET)" "$(INSTALLTO)\$(CMNTARGET)"

rebuild: clean all

"$(TARGETDIR)\$(CMNTARGET)" : $(DATAFILEPATHS)
	"$(GENCMN)" -C " Copyright (C) 2004, International Business Machines Corporation and others. All Rights Reserved. " -d "$(TARGETDIR)" -E  -n "$(NAME)" 0 <<
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_casing.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_conversion.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_mc.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_root.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_testtable32.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_te.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_te_IN.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_testtypes.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_icu26_testtypes.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_icu26e_testtypes.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_testempty.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_testaliases.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_icuio.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_iscii.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_DataDrivenCollationTest.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test.icu
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test1.cnv
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test3.cnv
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test4.cnv
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_test4x.cnv
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_ibm9027.cnv
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_idna_rules.res
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfscsi.spp
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfscss.spp
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfscis.spp
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfsmxs.spp
C:\tools\src\firebird\extern\icu\source\test\testdata\out\build\testdata_nfsmxp.spp
<<


# End of makefile for testdata [common mode]

