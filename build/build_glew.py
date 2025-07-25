# ------------------------------------------------------------------------------
#
# build_glew.py
#
# Summary : Builds the GlEW libraries and exes.
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

    DESCRIPTION = "Builds the GLEW library."
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "glew"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\glew"

    # ----------------------------------------------------------------------
    # the base name of the library
    _LIBNAME = "glew32"
    _DEBUG_SUFFIX = "_d"

    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\glew"
    # ----------------------------------------------------------------------
    # the name of the path that contains the cmake files
    _PATH_NAME_CMAKE_SOURCE = "."
    _PATH_NAME_CMAKE_BUILD = "build\\build-glew"
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

        buildSourceName = pathFinder.path(buildPathName, Program._PATH_NAME_CMAKE_SOURCE)
        cmakeBuildPath = pathFinder.path(buildPathName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = pathFinder.path(
            cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL
        )
        systemManager.removeDirectory(cmakeBuildPath)

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
        systemManager.changeDirectory(sourcePathName)
        # systemManager.removeDirectory(buildPathName)

        # copy Boost source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        systemManager.removeDirectory(cmakeBuildPath)
        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DBUILD_UTILS=OFF "
            + f"-DBUILD_SHARED_LIBS=ON "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.16 "
            + f"{buildSourceName}/build/cmake"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        cmakeCommandLine = (
            f"{pathFinder.getCMakeFileName()} "
            + f"--build "
            + f". "
            + f"-j 1 "
            + f"--target glew "
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
            + f"{pathFinder.path(cmakeBuildPath, "install")}"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        srcIncludePath = pathFinder.path(cmakeInstallPath, "include")

        systemManager.distributeFiles(
            srcIncludePath,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h",
        )

        dllName = (
            f"{Program._LIBNAME}"
            + f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}'
            + f".dll"
        )
        libName = (
            f"{Program._LIBNAME}"
            + f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}'
            + f".lib"
        )
        pdbName = (
            f"{Program._LIBNAME}"
            + f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}'
            + f".pdb"
        )

        systemManager.copyFile(
            pathFinder.path(
                cmakeInstallPath,
                "lib",
                f"{Program._LIBNAME}{'' if (buildSettings.ReleaseSpecified()) else 'd'}.lib",
            ),
            pathFinder.path(sdkOutDir, libName),
        )

        systemManager.copyFile(
            pathFinder.path(
                cmakeInstallPath,
                "bin",
                f"{Program._LIBNAME}{'' if (buildSettings.ReleaseSpecified()) else 'd'}.dll",
            ),
            pathFinder.path(sdkOutDir, dllName),
        )

        # for some reason, the pdb doesn't get installed during "cmake --install"
        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(
                    cmakeInstallPath,
                    "lib",
                    f"{Program._LIBNAME}{'' if (buildSettings.ReleaseSpecified()) else 'd'}.pdb",
                ),
                pathFinder.path(sdkOutDir, pdbName),
            )


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
