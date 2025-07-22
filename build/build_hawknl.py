# ------------------------------------------------------------------------------
#
# build_hawknl.py
#
# Summary : Builds HawkNL
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
    DESCRIPTION = "Builds the HawkNL library."

    _LIBNAME = "hawknl"
    _DEBUG_SUFFIX = "_d"

    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "hawknl"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that zstd the source code
    _PATH_NAME_SOURCE = "..\\src\\hawknl"
    # ----------------------------------------------------------------------
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "hawknl"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\hawknl"
    # ----------------------------------------------------------------------
    # the name of the path that contains the cmake files
    _PATH_NAME_CMAKE_SOURCE = "."
    _PATH_NAME_CMAKE_BUILD = "build"
    _PATH_NAME_CMAKE_INSTALL = "install"

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

        # MSBuild is under "Program Files (x86)"
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getMSBuildFileName(buildSettings.X64Specified())
        )

        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getNmakePathName(buildSettings.X64Specified())
        )

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

        buildSourceName = os.path.join(buildPathName, Program._PATH_NAME_CMAKE_SOURCE)
        cmakeBuildPath = os.path.join(buildSourceName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = os.path.join(
            cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL
        )

        systemManager.removeDirectory(cmakeBuildPath)

        sdkOutDir = os.path.join(
            buildPathName,
            "..",
            (
                Program._PATH_NAME_DISTRIBUTION_X64
                if buildSettings.X64Specified()
                else Program._PATH_NAME_DISTRIBUTION_X86
            ),
        )

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        # systemManager.removeDirectory(buildPathName)

        # copy Boost source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        includepath = os.path.join(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, '..')
        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DBUILD_SHARED_LIBS=ON "
            + f"-DCMAKE_REQUIRED_INCLUDES={includepath} "
            + f"{buildSourceName}"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        cmakeCommandLine = (
            f"{pathFinder.getCMakeFileName()} "
            + f"--build "
            + f". "
            + f"--config {conf} "
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        cmakeCommandLine = (
            f"{pathFinder.getCMakeFileName()} "
            + f"--install "
            + f". "
            + f"--config {conf} "
            + f"--prefix "
            + f"{os.path.join(cmakeBuildPath, "install")}"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        dllName = f'{Program._LIBNAME}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.dll'
        libName = f'{Program._LIBNAME}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib'
        pdbName = f'{Program._LIBNAME}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.pdb'

        systemManager.distributeFiles(
            os.path.join(cmakeInstallPath, "include"),
            os.path.join(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )

        systemManager.copyFile(
            os.path.join(cmakeInstallPath, "lib", "NL.lib"),
            os.path.join(sdkOutDir, libName)
        )

        systemManager.copyFile(
            os.path.join(cmakeInstallPath, "lib", "NL.dll"),
            os.path.join(sdkOutDir, dllName),
        )

        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                os.path.join(cmakeBuildPath, "Debug", "NL.pdb"),
                os.path.join(sdkOutDir, pdbName),
            )


# --------------------------------------------------------------------------

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
