# ------------------------------------------------------------------------------
#
# build_curl.py
#
# Summary : Builds the CURL library.
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
    DESCRIPTION = "Builds the LibSSH2 library."

    _LIBNAME = 'libssh2'
    _DEBUG_SUFFIX = '_d'

    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "libssh2"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\LibSSH2"
    # ----------------------------------------------------------------------
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "libssh2"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\libssh2"
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

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        includeBase = pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, "..")
        libSuffix = f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib'
        externalLibs = {
            "ZLIB_INCLUDE_DIR": pathFinder.path(includeBase, "zlib"),
            "ZLIB_LIBRARY": pathFinder.path(sdkOutDir, f"zlib{libSuffix}"),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DBUILD_SHARED_LIBS=ON "
            + f"-DCRYPTO_BACKEND=WinCNG "
            + f"-DENABLE_ZLIB_COMPRESSION=ON "
            + f"-DENABLE_ECDSA_WINCNG=ON "
            + f"-DBUILD_EXAMPLES=OFF "
            + f"-DBUILD_TESTING=OFF "
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
            + f"-j 1 "
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

        systemManager.distributeFiles(
            pathFinder.path( cmakeInstallPath,"include"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )

        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "lib"),
            pathFinder.path(sdkOutDir),
            "*.lib",
            suffix=None if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX,
        )

        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "bin"),
            pathFinder.path(sdkOutDir),
            "*.dll",
            suffix=None if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX,
        )

        if not buildSettings.ReleaseSpecified():
            systemManager.distributeFiles(
                pathFinder.path(cmakeBuildPath, "src", conf),
                pathFinder.path(sdkOutDir),
                "*.pdb",
                suffix=(
                    None if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX
                ),
            )


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
