# ------------------------------------------------------------------------------
#
# build_libpng.py
#
# Summary : Builds the LibPNG library.
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
    DESCRIPTION = "Builds the libpng library."

    _LIBNAME = 'libpng'
    _DEBUG_SUFFIX = '_d'

    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "libpng"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\libpng"
    # ----------------------------------------------------------------------
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "libpng"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\libpng"
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

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath(buildPathName, platform, conf)

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        systemManager.removeDirectory(buildPathName)

        # copy Boost source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        cmd = f"echo set_target_properties(png_shared PROPERTIES OUTPUT_NAME libpng) >> CMakeLists.txt"
        cmdResult = systemManager.execute(cmd)
        if cmdResult != 0:
            sys.exit(-1)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        includeBase = pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, "..")
        externalLibs = {
            "ZLIB_INCLUDE_DIR": pathFinder.path(includeBase, "zlib"),
            "ZLIB_LIBRARY": pathFinder.path(sdkOutDir, "zlib.lib"),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DPNG_SHARED=ON "
            + f"-DPNG_STATIC=OFF "
            + f"-DPNG_TESTS=OFF "
            + f"-DPNG_TOOLS=OFF "
            + f"-DPNG_DEBUG_POSTFIX={"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX} "
            + f"-DPNG_SHARED_OUTPUT_NAME=libpng "
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

        incdir = pathFinder.path(cmakeInstallPath, "include")
        for f in glob.glob(pathFinder.path(incdir, "*.h")):
            fname = f[len(incdir) + 1 :]
        systemManager.copyFile(f, pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, fname))

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

        systemManager.copyFile(
            pathFinder.path(cmakeInstallPath, "bin", dllName),
            pathFinder.path(sdkOutDir, dllName),
            )

        systemManager.copyFile(
            pathFinder.path(cmakeInstallPath, "lib", libName),
            pathFinder.path(sdkOutDir, libName),
            )

        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(
                cmakeBuildPath,
                conf,
                pdbName,
                ),
                pathFinder.path(sdkOutDir, pdbName),
                )

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
