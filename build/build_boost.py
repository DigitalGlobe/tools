# ------------------------------------------------------------------------------
#
# build_boost.py
#
# Summary : Builds the Boost libraries.
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
class Program :

    # --------------------------------------------------------------------------
    # constants

    # ----------------------------------------------------------------------
    # a description of what the script does
    DESCRIPTION = "Builds the Boost libraries."
    # ----------------------------------------------------------------------

    _BOOST_VERSION = "1.88"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "Boost"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\Boost"
    # ----------------------------------------------------------------------
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = 'boost'
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = '..\\..\\include\\boost'
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
    def __init__(self) :

        pass
    # ----------------------------------------------------------------------

    # --------------------------------------------------------------------------
    # public methods

    # ----------------------------------------------------------------------
    # The main method of the program.
    #
    # Parameters :
    #     self : this program
    def main(self) :
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
        buildPathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
        sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)

        buildSourceName = os.path.join(buildPathName, Program._PATH_NAME_CMAKE_SOURCE)
        cmakeBuildPath = os.path.join(buildPathName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = os.path.join(cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL)

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

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f'-DBUILD_SHARED_LIBS=ON '
            + f'-A {platform} '
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

        srcIncludePath = os.path.join(
            cmakeInstallPath,
            "include",
            f'boost-{Program._BOOST_VERSION.replace(".", "_")}',
            "boost"
            )

        systemManager.distributeFiles(
            srcIncludePath,
            os.path.join(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*"
        )

        systemManager.distributeFiles(
            os.path.join(cmakeInstallPath, "lib"),
            sdkOutDir,
            "*.lib",
        )
        systemManager.distributeFiles(
            os.path.join(cmakeInstallPath, "bin"),
            sdkOutDir,
            "*.dll",
        )
        if not buildSettings.ReleaseSpecified():
            systemManager.distributeFiles(
                os.path.join(cmakeInstallPath, "bin"),
                sdkOutDir, "lib",
                "*.pdb",
            )

    # ----------------------------------------------------------------------

# --------------------------------------------------------------------------

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
