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
    DESCRIPTION = "Builds the FileGDB library."
    # ----------------------------------------------------------------------

    _LIBNAME = "FileGDBAPI"
    _DEBUG_SUFFIX = "_d"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "FileGDB"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\FileGDB"
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # the pattern for binary files
    _FILE_PATTERN_BINARY = "*.exe"

    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\FileGDB"
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

        if not buildSettings.X64Specified():
            print("32-bit build not supported")
            sys.exit(-1)

        sourcePathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_SOURCE
        )

        sdkOutDir = (
            sourcePathName
            + "\\..\\"
            + (
                Program._PATH_NAME_DISTRIBUTION_X64
                if buildSettings.X64Specified()
                else Program._PATH_NAME_DISTRIBUTION_X86
            )
        )
        incdir = pathFinder.path(sourcePathName, "include")
        bindir = pathFinder.path(sourcePathName, "bin64")
        libdir = pathFinder.path(sourcePathName, "lib64")

        systemManager.removeDirectory(
            pathFinder.path(sourcePathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE)
        )

        systemManager.distributeFiles(
            incdir,
            pathFinder.path(sourcePathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )

        for f in glob.glob(pathFinder.path(libdir, f"*{"" if (buildSettings.ReleaseSpecified()) else "d"}.lib")):
            fname = f[len(libdir) + 1 :]
            fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME) :]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib"

            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        for f in glob.glob(pathFinder.path(bindir, f"*{"" if (buildSettings.ReleaseSpecified()) else "d"}.dll")):
            fname = f[len(bindir) + 1 :]
            fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME)]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.dll"
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        if not buildSettings.ReleaseSpecified():
            for f in glob.glob(pathFinder.path(bindir, f"*.pdb")):
                fname = f[len(bindir) + 1 :]
                fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME)]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.pdb"
                systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))


# --------------------------------------------------------------------------

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
