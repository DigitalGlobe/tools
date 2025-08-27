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

        buildSourceName = pathFinder.path(buildPathName, Program._PATH_NAME_CMAKE_SOURCE)
        cmakeBuildPath = pathFinder.path(buildSourceName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = pathFinder.path(
            cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL
            )

        systemManager.removeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath( buildPathName, platform, conf)

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        # systemManager.removeDirectory(buildPathName)

        # copy Boost source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        if not buildSettings.ReleaseSpecified():
            cmd = f'echo set_target_properties(NL PROPERTIES OUTPUT_NAME "hawknl") >> CMakeLists.txt'
            cmdResult = systemManager.execute(cmd)
            if cmdResult != 0:
                sys.exit(-1)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        extIncludePath = pathFinder.getVCPKGIncludePath()
        externalLibs = {
            # "PROJ_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(extIncludePath, "proj")),
            # "PROJ_LIBRARY_RELEASE": pathFinder.slasher(pathFinder.path(pathFinder.getVCPKGLibPath(buildSettings.ReleaseSpecified()), f"proj.lib")),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        includepath = pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, '..')
        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DBUILD_SHARED_LIBS=ON "
            + f"-DCMAKE_REQUIRED_INCLUDES={includepath} "
            + f"{externalLibStr} "
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
            + f"{pathFinder.path(cmakeBuildPath, "install")}"
            )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        libName = f'{Program._LIBNAME}.lib'
        dllName = f'{Program._LIBNAME}.dll'
        pdbName = f'{Program._LIBNAME}.pdb'

        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "include"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
            )

        systemManager.copyFile(
            pathFinder.path(cmakeInstallPath, "lib", libName),
            pathFinder.path(sdkOutDir, libName)
        )

        systemManager.copyFile(
            pathFinder.path(cmakeInstallPath, "lib", dllName),
            pathFinder.path(sdkOutDir, dllName),
            )

        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(cmakeBuildPath, "Debug", pdbName),
                pathFinder.path(sdkOutDir, pdbName),
                )

        # --------------------------------------------------------------------------

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
