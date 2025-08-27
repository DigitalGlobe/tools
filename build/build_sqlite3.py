# ------------------------------------------------------------------------------
#
# build_newmat.py
#
# Summary : Builds the Newmat library.
#
# ------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *

# ------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program:

    # --------------------------------------------------------------------------
    # constants

    # ----------------------------------------------------------------------
    # a description of what the script does
    DESCRIPTION = "Builds the SQLite3 library."
    # ----------------------------------------------------------------------

    _FILE_NAME_MAKEFILE = "Makefile.msc"
    # ----------------------------------------------------------------------

    _LIBNAME = "sqlite3"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "sqlite3"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\sqlite3"
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # the pattern for binary files
    _FILE_PATTERN_BINARY = "*.exe"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"

    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\sqlite3"
    # ----------------------------------------------------------------------

    _PATH_NAME_NMAKE_INSTALL = "install"
    # --------------------------------------------------------------------------
    # constructors

    # ----------------------------------------------------------------------
    # Constructs this program.
    #
    # Parameters :
    # self : this program
    def __init__(self):

        pass

        # ----------------------------------------------------------------------

        # --------------------------------------------------------------------------
        # public methods

        # ----------------------------------------------------------------------
        # The main method of the program.
        #
        # Parameters :
        # self : this program
    def main(self):

        systemManager = SystemManager()
        pathFinder = PathFinder()

        # process command-line arguments
        buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)

        # initialize environment variables
        systemManager.initializeIncludeEnvironmentVariable(buildSettings.X64Specified())
        systemManager.initializeLibraryEnvironmentVariable(buildSettings.X64Specified())

        # MSBuild is under "Program Files (x86)"
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getMSBuildFileName(buildSettings.X64Specified())
        )

        vcVars = pathFinder.getVCVARSFileName(buildSettings.X64Specified())

        compileOutDir = ""
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getWindowsSdkBinPathName(buildSettings.X64Specified())
        )

        # determine path names
        binaryPathName = (
            systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X64)
        if (buildSettings.X64Specified())
        else systemManager.getCurrentRelativePathName(Program._PATH_NAME_BINARY_X86)
        )
        buildPathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_BUILD
            )
        sourcePathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_SOURCE
            )

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath(buildPathName, platform, conf)

        # remove build dir
        systemManager.removeDirectory(buildPathName)
        systemManager.copyDirectory(sourcePathName, buildPathName)
        systemManager.changeDirectory(buildPathName)

        nmakeBuildPath = pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL)

        opts = (
            f"-DSQLITE_ENABLE_FTS3=1 "
            + f"-DSQLITE_ENABLE_FTS4=1 "
            + f"-DSQLITE_ENABLE_FTS5=1 "
            + f"-DSQLITE_ENABLE_RTREE=1 "
            + f"-DSQLITE_ENABLE_JSON1=1 "
            + f"-DSQLITE_ENABLE_GEOPOLY=1 "
            + f"-DSQLITE_ENABLE_SESSION=1 "
            + f"-DSQLITE_ENABLE_PREUPDATE_HOOK=1 "
            + f"-DSQLITE_ENABLE_SERIALIZE=1 "
            + f"-DSQLITE_ENABLE_MATH_FUNCTIONS=1"

        )

        nmakeCommandLine = (
            f'nmake /f "{pathFinder.path(buildPathName, Program._FILE_NAME_MAKEFILE)}" '
            + f"core "
            + f"_MSC_VER=1900 "
            + f"{"" if (buildSettings.X64Specified()) else "DEBUG=1 "}"
            + f"SYMBOLS={"0" if (buildSettings.ReleaseSpecified()) else "1"} "
            + f"NO_TCL=1 "
            + f'OPTS="{opts}" '
            + f'OPTFLAGS="{"/MD /Op" if (buildSettings.ReleaseSpecified()) else "/MDd /Z7"}" '
            + f'INSTALLDIR={nmakeBuildPath} '
            + f'{"nodebug=1" if (buildSettings.ReleaseSpecified()) else ""} '
            )

        cmd = f'"{vcVars}" && {nmakeCommandLine}'

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.removeDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE)
        )

        hName = (
            Program._LIBNAME
            + ".h"
            )
        dllName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
        + ".dll"
        )
        libName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
        + ".lib"
        )
        exeName = (
            Program._LIBNAME
            + ".exe"
            )
        pdbName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
        + ".pdb"
        )

        systemManager.copyFile(
            pathFinder.path(buildPathName, Program._LIBNAME + ".h"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, hName),
            )
        systemManager.copyFile(
            pathFinder.path(buildPathName, Program._LIBNAME + ".dll"),
            pathFinder.path(sdkOutDir, dllName),
            )
        systemManager.copyFile(
            pathFinder.path(buildPathName, Program._LIBNAME + ".lib"),
            pathFinder.path(sdkOutDir, libName),
            )
        systemManager.copyFile(
            pathFinder.path(buildPathName, exeName),
            pathFinder.path(sdkOutDir, exeName),
            )

        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(buildPathName, Program._LIBNAME + ".pdb"),
                pathFinder.path(sdkOutDir, pdbName),
                )

        # --------------------------------------------------------------------------

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
