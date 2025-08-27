# ------------------------------------------------------------------------------
#
# build_galib.py
#
# Summary : Builds the GALib / Genetic Algorithm Components
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
    DESCRIPTION = "Builds the libpsl library."
    # ----------------------------------------------------------------------

    _LIBNAME = "psl"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "libpsl"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\libpsl"
    # ----------------------------------------------------------------------

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
    _PATH_NAME_INCLUDE = "libpsl"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\libpsl"
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

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath(buildPathName, platform, conf)

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        systemManager.removeDirectory(buildPathName)

        # copy UriParser to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # modify the project name
        nmake = pathFinder.path(buildPathName, "msvc", "install-msvc.mak")
        sedResult = systemManager.replaceInFile(
            nmake,
            f"{Program._LIBNAME}",
            f"\\*",
            )
        if sedResult != 0:
            sys.exit(-1)

        nmake = pathFinder.path(buildPathName, "msvc", "install-msvc.mak")
        sedResult = systemManager.replaceInFile(
            nmake,
            f"lib\\*",
            f"lib{Program._LIBNAME}",
            )
        if sedResult != 0:
            sys.exit(-1)

        nmakeInstallPath = pathFinder.path(buildPathName, nmakeInstallPath)
        systemManager.makeDirectory(nmakeInstallPath)

        # start building
        systemManager.changeDirectory(pathFinder.path(buildPathName, "msvc"))

        libName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
        + ".lib"
        )
        dllName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
        + ".dll"
        )
        pdbName = (
            Program._LIBNAME
            + ("" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX)
        + ".pdb"
        )

        # run Nmake
        nmakeCommandLine = (
            f"nmake -f Makefile.vc "
            + f"CFG={"release" if (buildSettings.ReleaseSpecified()) else "debug"} "
            + f"PSL_DLL_SUFFIX={"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX} "
            + f"PSL_LIB=vs$(VSVER)\$(CFG)\$(PLAT)\psl{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib "
            + f"DISABLE_RUNTIME=1 "
            + f"DISABLE_BUILTIN=1 "
            + f'PREFIX="{pathFinder.path(buildPathName, nmakeInstallPath)}" '
            + f" install "
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
            pathFinder.path(nmakeInstallPath, "include"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h",
            )

        systemManager.copyFile(
            pathFinder.path(nmakeInstallPath, "lib", libName), pathFinder.path(sdkOutDir, libName)
        )
        systemManager.copyFile(
            pathFinder.path(nmakeInstallPath, "bin", dllName),
            pathFinder.path(sdkOutDir, dllName),
            )
        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(nmakeInstallPath, "bin", pdbName),
                pathFinder.path(sdkOutDir, pdbName),
                )

        # --------------------------------------------------------------------------

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
