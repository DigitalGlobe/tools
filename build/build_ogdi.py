# ------------------------------------------------------------------------------
#
# build_ogdi.py
#
# Summary : Builds the Open Geographic Datastore Interface library
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
    DESCRIPTION = "Builds the OGDI library."
    # the name of the release makefile
    _FILE_NAME_MAKEFILE = "makefile"
    # ----------------------------------------------------------------------

    _LIBNAME = "ogdi"
    _DEBUG_SUFFIX = "_d"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "ogdi"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\ogdi"
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
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\ogdi"
    # ----------------------------------------------------------------------

    # --------------------------------------------------------------------------
    # constructors

    # ----------------------------------------------------------------------
    # Constructs this program.
    #
    # Parameters :
    #     self : this program
    def __init__(self):

        pass

    # ----------------------------------------------------------------------

    # --------------------------------------------------------------------------
    # public methods

    # ----------------------------------------------------------------------
    # The main method of the program.
    #
    # Parameters :
    #     self : this program
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

        systemManager.appendToPathEnvironmentVariable(pathFinder.PATH_GNU_TOOLS)

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

        os.environ["TOPDIR"] = buildPathName

        sdkOutDir = (
            buildPathName
            + "\\..\\"
            + (
                Program._PATH_NAME_DISTRIBUTION_X64
                if buildSettings.X64Specified()
                else Program._PATH_NAME_DISTRIBUTION_X86
            )
        )

        # remove build dir
        systemManager.removeDirectory(buildPathName)
        systemManager.copyDirectory(sourcePathName, buildPathName)
        systemManager.changeDirectory(buildPathName)

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
        pdbName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
            + ".pdb"
        )

        nmakeCommandLine = (
            f'make '
            + f"_MSC_VER=1900 "
            + f"TARGET=win32 "
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
        systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_INCLUDE),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h",
        )

        systemManager.copyFile(
            pathFinder.path(buildPathName, libName), pathFinder.path(sdkOutDir, libName)
        )


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
