# ------------------------------------------------------------------------------
#
# build_minizip.py
#
# Summary : Builds the MiniZip library.
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
    DESCRIPTION = "Builds the Minizip library."
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "minizip"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\minizip"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "minizip"
    _DEBUG_SUFFIX = "_d"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "include\\minizip"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\minizip"
    # ----------------------------------------------------------------------
    # ----------------------------------------------------------------------
    # the name of the path that contains the cmake files
    _PATH_NAME_CMAKE_SOURCE = "."
    _PATH_NAME_CMAKE_BUILD = "build"
    _PATH_NAME_CMAKE_INSTALL = "install"

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

        buildSourceName = pathFinder.path(
            buildPathName, Program._PATH_NAME_CMAKE_SOURCE
        )
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

        # copy source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        includeBase = pathFinder.path(
            buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, ".."
        )
        libSuffix = (
            f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib'
        )
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

        pdbName = (
            f"{Program._LIBNAME}"
            + f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}'
            + f".pdb"
        )

        srcIncludePath = pathFinder.path(cmakeInstallPath, "include", f"minizip")
        srcLibPath = pathFinder.path(cmakeInstallPath, "lib")

        systemManager.distributeFiles(
            srcIncludePath,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )

        systemManager.distributeFiles(
            srcLibPath,
            sdkOutDir,
            "*.lib",
            suffix=None if buildSettings.ReleaseSpecified() else Program._DEBUG_SUFFIX,
        )

        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(
                    cmakeBuildPath,
                    conf,
                    f"{Program._LIBNAME}.pdb",
                ),
                pathFinder.path(sdkOutDir, pdbName),
            )


# --------------------------------------------------------------------------

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
