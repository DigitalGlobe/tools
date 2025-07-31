# ------------------------------------------------------------------------------
#
# build_libiconv.py
#
# Summary : Builds the LibICONV library.
#
# ------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *
from XmlUtils import *


class Program:
    # ----------------------------------------------------------------------
    # a description of what the script does
    DESCRIPTION = "Builds the libICONV library."
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "libICONV"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\libICONV"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "libiconv"
    _LIBNAME_CHARSET = "charset"
    _DEBUG_SUFFIX = "_d"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "include"
    _PATH_NAME_BIN = "bin"
    _PATH_NAME_LIB = "lib"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\libiconv"
    # ----------------------------------------------------------------------
    _PATH_NAME_NMAKE_INSTALL = "install"

    def __init__(self):

        pass

    # ----------------------------------------------------------------------

    def main(self):
        systemManager = SystemManager()
        pathFinder = PathFinder()

        # process command-line arguments
        buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)

        # initialize environment variables
        systemManager.initializeIncludeEnvironmentVariable(buildSettings.X64Specified())
        systemManager.initializeLibraryEnvironmentVariable(buildSettings.X64Specified())

        # systemManager.setEnvironmentVariableValue("VS170COMNTOOLS",

        # MSBuild is under "Program Files (x86)"
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getMSBuildFileName(buildSettings.X64Specified())
        )

        vcVars = pathFinder.getVCVARSFileName(buildSettings.X64Specified())

        compileOutDir = ""
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getWindowsSdkBinPathName(buildSettings.X64Specified())
        )

        # get the paths
        buildPathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_BUILD
        )
        sourcePathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_SOURCE
        )

        nmakeInstallPath = pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL)

        sdkOutDir = pathFinder.path(
            buildPathName,
            "..",
            (
                Program._PATH_NAME_DISTRIBUTION_X64
                if buildSettings.X64Specified()
                else Program._PATH_NAME_DISTRIBUTION_X86
            ),
        )

        # remove build dir
        systemManager.removeDirectory(buildPathName)

        # copy source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)
        systemManager.changeDirectory(buildPathName)

        systemManager.copyFile(
            pathFinder.path(buildPathName, "include", "iconv.h.msvc-shared"),
            pathFinder.path(buildPathName, "include", "iconv.h"),
        )
        # start building
        srcdir = pathFinder.path(buildPathName, "lib")

        installpath = pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL)

        # run Nmake
        nmakeCommandLine = (
            f"nmake -f Makefile.msvc "
            + f"DLL=1 "
            + f"NO_NLS=1 "
            + f"DEBUG={'0' if buildSettings.ReleaseSpecified() else "1"} "
            + f"PREFIX={installpath} IIPREFIX={installpath} "
        )

        # cmd = f'"{vcVars}" && {nmakeCommandLine} config.h'
        # print("cmd: " + cmd)
        # nmakeResult = systemManager.execute(cmd)
        # if nmakeResult != 0:
        #     sys.exit(-1)

        # systemManager.changeDirectory(srcdir)
        cmd = f'"{vcVars}" && {nmakeCommandLine} all install'
        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.removeDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE)
        )

        systemManager.copyFile(
            pathFinder.path(
                buildPathName,
                Program._PATH_NAME_NMAKE_INSTALL,
                Program._PATH_NAME_INCLUDE,
                "iconv.h",
            ),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, "iconv.h")
        )
        systemManager.copyFile(
            pathFinder.path(
                buildPathName,
                Program._PATH_NAME_NMAKE_INSTALL,
                Program._PATH_NAME_BIN,
                "iconv.dll",
            ),
            pathFinder.path(
                sdkOutDir,
                f'{Program._LIBNAME}{"" if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX}.dll',
            ),
        )
        systemManager.copyFile(
            pathFinder.path(
                buildPathName,
                Program._PATH_NAME_NMAKE_INSTALL,
                Program._PATH_NAME_LIB,
                "iconv.lib",
            ),
            pathFinder.path(
                sdkOutDir,
                f'{Program._LIBNAME}{"" if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX}.lib',
            ),
        )
        systemManager.copyFile(
            pathFinder.path(
                buildPathName,
                Program._PATH_NAME_NMAKE_INSTALL,
                Program._PATH_NAME_LIB,
                "charset.lib",
            ),
            pathFinder.path(
                sdkOutDir,
                f'{Program._LIBNAME_CHARSET}{"" if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX}.lib',
            ),
        )

        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(
                    buildPathName,
                    Program._PATH_NAME_LIB,
                    "iconv.pdb",
                ),
                pathFinder.path(
                    buildPathName,
                    sdkOutDir,
                    f'{Program._LIBNAME}{"" if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX}.pdb',
                ),
            )


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
