# ------------------------------------------------------------------------------
#
# build_tbb.py
#
# Summary : Builds the ThreadingBuildingBlocks library
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
    DESCRIPTION = "Builds ThreadingBuildingBlocks."

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "tbb"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\tbb"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "tbb"
    _DEBUG_SUFFIX = "_d"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    _PATH_NAME_INCLUDE_2 = "include"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\tbb"
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

        buildSourceName = os.path.join(buildPathName, Program._PATH_NAME_CMAKE_SOURCE)
        cmakeBuildPath = os.path.join(buildPathName, Program._PATH_NAME_CMAKE_BUILD)
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

        # copy APR source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        # run CMake
        # -DCMAKE_POLICY_VERSION_MINIMUM is to avoid min compatability errors in CMake
        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DBUILD_SHARED_LIBS=ON "
            + f"-DTBB_TEST=OFF "
            + f"-DCMAKE_DEBUG_POSTFIX=_d "
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
            + f"{cmakeInstallPath}"
        )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        srcIncludePath = os.path.join(cmakeInstallPath, "include", "tbb")
        srcLibPath = os.path.join(cmakeInstallPath, "lib")
        srcBinPath = os.path.join(cmakeInstallPath, "bin")

        systemManager.distributeFiles(
            srcIncludePath,
            os.path.join(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )

        systemManager.distributeFiles(
            srcLibPath,
            sdkOutDir,
            "*.lib"
        )

        systemManager.distributeFiles(
            srcBinPath,
            sdkOutDir,
            "*.dll"
        )

        if not buildSettings.ReleaseSpecified():
            libName = (
                f"{Program._LIBNAME}"
                + f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}'
                + f".lib"
            )

            systemManager.removeFile(os.path.join(sdkOutDir, f"{Program._LIBNAME}_debug.lib"))
            systemManager.copyFile(
                os.path.join(srcLibPath, f"{Program._LIBNAME}_debug.lib"),
                os.path.join(sdkOutDir, libName),
            )

            systemManager.distributeFiles(
                srcBinPath,
                sdkOutDir,
                "*.pdb"
            )


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
