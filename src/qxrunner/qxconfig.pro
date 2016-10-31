#----------------------------------------------------------------------
# File:    qxconfig.pro
# Purpose: Configuration parameters and functions for Qx projects.        
# PreCond: TEMPLATE variable must be set.
#----------------------------------------------------------------------
QT += core gui widgets


LANGUAGE = C++

#----------------------------------------------------------------------
# If QX_CONFIG_MODIFIER is 'true' the following values can be provided
# in the CONFIG variable for simplified compiler and linker options
# specification. If set to 'false' the standard qmake handling of the
# CONFIG variable is applied.
#
# Value          Description
# -----          -----------
# debug_static   Build target in debug mode. If target is a library
#                create it as static. If target is an app link it with
#                debug static libraries (if possible). This is the
#                default.
# debug_dll      Build target in debug mode. If target is a library
#                create it as a DLL/shared image. If target is an app
#                link it with debug DLL's/shared images. 
# release_static Build target in release mode. If target is a library
#                create it as static. If target is an app link it with
#                release static libraries (if possible).
# release_dll    Build target in release mode. If target is a library
#                create it as a DLL/shared image. If target is an app
#                link it with release DLL's/shared images.
#----------------------------------------------------------------------

QX_CONFIG_MODIFIER = true

#----------------------------------------------------------------------
# CONFIG set up. Assigns unique options to the CONFIG variable.  
#----------------------------------------------------------------------

CONFIG -= debug_and_release build_all   # Not convincing

CONFIG += qt warn_on

contains(QX_CONFIG_MODIFIER, true) {

    config_done = false

    # Do some cleaning first
    CONFIG = $$unique(CONFIG)
    CONFIG -= debug release dll staticlib use_staticlib use_dll

    debug_dll {
        CONFIG -= debug_dll
        CONFIG += debug
        contains(TEMPLATE, lib) {
            CONFIG += dll
        }
        contains(TEMPLATE, app) {
            CONFIG += use_dll
        }
        config_done = true
    }

    release_static {
        CONFIG -= release_static
        CONFIG += release
        contains(TEMPLATE, lib) {
            CONFIG += staticlib
        }
        contains(TEMPLATE, app) {
            CONFIG += use_staticlib
        }
        config_done = true
    }

    release_dll {
        CONFIG -= release_dll
        CONFIG += release
        contains(TEMPLATE, lib) {
            CONFIG += dll
        }
        contains(TEMPLATE, app) {
            CONFIG += use_dll
        }
        config_done = true
    }

    contains(config_done, false) {  # debug_static / default
        CONFIG -= debug_static
        CONFIG += debug
        contains(TEMPLATE, lib) {
            CONFIG += staticlib
        }
        contains(TEMPLATE, app) {
            CONFIG += use_staticlib
        }
    }

                           message(******************************)
    debug:staticlib:       message(Mode: debug staticlib)
    debug:dll:             message(Mode: debug dll)
    debug:use_staticlib:   message(Mode: debug use_staticlib)
    debug:use_dll:         message(Mode: debug use_dll)
    release:staticlib:     message(Mode: release staticlib)
    release:dll:           message(Mode: release dll)
    release:use_staticlib: message(Mode: release use_staticlib)
    release:use_dll:       message(Mode: release use_dll)
                           message(******************************)
}

#----------------------------------------------------------------------
# Definition of configuration specific library base filenames.
#----------------------------------------------------------------------

QX_RUNNERFILENAME  = qxrunner
QX_CPPUNITFILENAME = qxcppunit
CPPUNITFILENAME    = cppunit

debug {
    QX_RUNNERFILENAME  = $${QX_RUNNERFILENAME}_d
    QX_CPPUNITFILENAME = $${QX_CPPUNITFILENAME}_d
}

release {
    QX_RUNNERFILENAME  = $${QX_RUNNERFILENAME}
    QX_CPPUNITFILENAME = $${QX_CPPUNITFILENAME}
}

win32:debug {
    CPPUNITFILENAME = $${CPPUNITFILENAME}_d
    # No debug version of CppUnit library on Linux?
}

win32:dll {
    QX_RUNNERFILENAME  = $${QX_RUNNERFILENAME}_dll
    QX_CPPUNITFILENAME = $${QX_CPPUNITFILENAME}_dll
    CPPUNITFILENAME    = $${CPPUNITFILENAME}_dll
}

win32:use_dll {
    QX_RUNNERFILENAME  = $${QX_RUNNERFILENAME}_dll
    QX_CPPUNITFILENAME = $${QX_CPPUNITFILENAME}_dll
    CPPUNITFILENAME    = $${CPPUNITFILENAME}_dll
}

unix:dll {
    QX_RUNNERFILENAME  = $${QX_RUNNERFILENAME}_shared
    QX_CPPUNITFILENAME = $${QX_CPPUNITFILENAME}_shared
}

unix:use_dll {
    QX_RUNNERFILENAME  = $${QX_RUNNERFILENAME}_shared
    QX_CPPUNITFILENAME = $${QX_CPPUNITFILENAME}_shared
}

#----------------------------------------------------------------------
# Definition of configuration specific intermediate directories.
#----------------------------------------------------------------------

win32 {
    debug {
        OBJECTS_DIR = debug
    }
    release {
        OBJECTS_DIR = release
    }
    dll {
        OBJECTS_DIR = $${OBJECTS_DIR}_dll
    }
    use_dll {
        OBJECTS_DIR = $${OBJECTS_DIR}_dll
    }
    RCC_DIR = generatedfiles
}

unix {
    debug {
        OBJECTS_DIR = .debug
    }
    release {
        OBJECTS_DIR = .release
    }
    dll {
        OBJECTS_DIR = $${OBJECTS_DIR}_shared
    }
    use_dll {
        OBJECTS_DIR = $${OBJECTS_DIR}_shared
    }
    RCC_DIR = .generatedfiles
}
DESTDIR = $$OBJECTS_DIR
UI_DIR  = $$RCC_DIR
MOC_DIR = $${RCC_DIR}/$${OBJECTS_DIR}

#======================================================================
# Helper Functions. See the code for referenced variables.
#======================================================================

#----------------------------------------------------------------------
# Replace / with \ in given path.
#----------------------------------------------------------------------

defineReplace(winPath) {

    variable = $$1
#    variable ~= s///
    variable ~= s,/,\\,g

    return($$variable)
}

#----------------------------------------------------------------------
# Prepare command for library copy to lib folder after build.
#----------------------------------------------------------------------

defineReplace(winCopyLib) {

    excludefile = $$winPath($${DESTDIR}/xcopy_exclude.txt)
    libdir = $$winPath($$QX_LIBDIR)
    files = $$winPath($${DESTDIR}/$${TARGET}*)
    
    copy_cmd  =   @echo .exp >  $$excludefile
    copy_cmd += | @echo .ilk >> $$excludefile
    copy_cmd += | xcopy/C/R/Y/EXCLUDE:$$excludefile $$files $$libdir
    
    return($$copy_cmd)
}

#----------------------------------------------------------------------
# Returns compiler options. Right now for MSVC debug mode only. 
#----------------------------------------------------------------------

defineReplace(compilerOptions) {

    compiler_options = ""

    win32:debug {
        # MSVC creates a program database file (.pdb) in debug mode
        tmp = $$find(MAKEFILE_GENERATOR, MSVC)
        !isEmpty(tmp) {
            pdbfile = $$winPath($${DESTDIR}/$${TARGET}.pdb)
            compiler_options = /Fd"$$pdbfile"
        }
    }
    
    return($$compiler_options)
}

#----------------------------------------------------------------------
# Returns filename of QxRunner library suitable for input to linker. 
#----------------------------------------------------------------------

defineReplace(qxRunnerLibForLinker) {

    win32 {
        libname = $$winPath($${QX_LIBDIR}/$${QX_RUNNERFILENAME}.lib)
    }
    unix {
        libname = -L$$QX_LIBDIR -l$$QX_RUNNERFILENAME
    }
    
    return($$libname)
}

#----------------------------------------------------------------------
# Returns filename of QxCppUnit library suitable for input to linker. 
#----------------------------------------------------------------------

defineReplace(qxCppUnitLibForLinker) {

    win32 {
        libname = $$winPath($${QX_LIBDIR}/$${QX_CPPUNITFILENAME}.lib)
    }
    unix {
        libname = -L$$QX_LIBDIR -l$$QX_CPPUNITFILENAME
    }
    
    return($$libname)
}

#----------------------------------------------------------------------
# Returns filename of CppUnit library suitable for input to linker. 
#----------------------------------------------------------------------

defineReplace(cppUnitLibForLinker) {

    win32 {
        libname = $$winPath($$(CPPUNIT)/lib/$${CPPUNITFILENAME}.lib)
    }
    unix {
        libname = -L$$(CPPUNIT)/lib -l$$CPPUNITFILENAME
    }
    
    return($$libname)
}
