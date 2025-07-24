# ------------------------------------------------------------------------------
#
# build_aprutil.py
#
# Summary : Builds the APR Util library.
#
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
#
# build_apr.py
#
# Summary : Builds the APR library.
#
# ------------------------------------------------------------------------------


import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *
from FileDistributor import *
from XmlUtils import *


# ------------------------------------------------------------------------------
# The Program class represents the main class of the script.
class Program:

    # --------------------------------------------------------------------------
    # constants

    # ----------------------------------------------------------------------
    # a description of what the script does
    DESCRIPTION = "Builds the APR Util library."
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "aprutil"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\aprutil"
    # ----------------------------------------------------------------------
    # the name of the release makefile
    _FILE_NAME_MAKEFILE = "Makefile.win"
    # ----------------------------------------------------------------------

    _DEBUG_SUFFIX = "_d"
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
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\aprutil"
    # ----------------------------------------------------------------------

    _PATH_NAME_NMAKE_INSTALL = "install"

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

        vcVars = pathFinder.getVCVARSFileName(buildSettings.X64Specified())

        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getWindowsSdkBinPathName(buildSettings.X64Specified())
        )

        # determine path names
        buildPathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_BUILD
        )
        sourcePathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_SOURCE
        )

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

        nmakeCommandLine = (
            f'nmake /f "{Program._FILE_NAME_MAKEFILE}" '
            + f"CFG='{"Release" if (buildSettings.ReleaseSpecified()) else "Debug"}' "
            + f"PREFIX={os.path.join(buildPathName, Program._PATH_NAME_NMAKE_INSTALL) } "
            + f'ARCH="{"x64" if (buildSettings.X64Specified()) else "Win32"} {"Release" if (buildSettings.ReleaseSpecified()) else "Debug"}" '
            + f'USEMAK=1 '
            + f" buildall install "
        )

        cmd = f'"{vcVars}" && {nmakeCommandLine}'

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.removeDirectory(
            os.path.join(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE)
        )
        systemManager.distributeFiles(
            os.path.join(buildPathName, Program._PATH_NAME_NMAKE_INSTALL, "include"),
            os.path.join(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h",
        )

        libdir = os.path.join(buildPathName,Program._PATH_NAME_NMAKE_INSTALL, "lib")
        bindir = os.path.join(buildPathName, Program._PATH_NAME_NMAKE_INSTALL, "bin")

        # we need to rename the debug libs to have a _d suffix (they have a 'd' suffix now)
        for f in glob.glob(os.path.join(libdir, "*.lib")):
            systemManager.copyFile(f, os.path.join(sdkOutDir, f.replace("-1.lib", f"{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib")))

        for f in glob.glob(os.path.join(bindir, "*.dll")):
            systemManager.copyFile(f, os.path.join(sdkOutDir, f.replace("-1.dll", f"{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.dll")))

        if not buildSettings.ReleaseSpecified():
            for f in glob.glob(os.path.join(libdir, "*.pdb")):
                systemManager.copyFile(f, os.path.join(sdkOutDir, f.replace("-1.pdb", f"{Program._DEBUG_SUFFIX}.pdb")))


# --------------------------------------------------------------------------

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
