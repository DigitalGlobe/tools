# ------------------------------------------------------------------------------
#
# build_libjpeg.py
#
# Summary : Builds the LibJPEG library.
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
    DESCRIPTION = "Builds the jpeg library."
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "libjpeg"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\libjpeg"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "libjpeg"
    _LIBNAME_STATIC = "libjpeg-static"
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    _PATH_NAME_INCLUDE_2 = "include"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\libjpeg"
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

        # copy APR source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        cmd = f'echo set_target_properties(jpeg PROPERTIES RUNTIME_OUTPUT_NAME libjpeg) >> sharedlib\\CMakeLists.txt'
        cmdResult = systemManager.execute(cmd)
        if cmdResult != 0:
            sys.exit(-1)

        cmd = f"echo set_target_properties(jpeg PROPERTIES OUTPUT_NAME libjpeg) >> sharedlib\\CMakeLists.txt"
        cmdResult = systemManager.execute(cmd)
        if cmdResult != 0:
            sys.exit(-1)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        # run CMake
        # -DCMAKE_POLICY_VERSION_MINIMUM is to avoid min compatability errors in CMake
        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DENABLE_STATIC=OFF "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DCMAKE_BUILD_TYPE={conf} " + f"{buildSourceName}"
            )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

        cmakeCommandLine = (
            f"{pathFinder.getCMakeFileName()} "
            + f"--build "
            + f". "
        # + f"-j 1 "
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

        srcIncludePath = pathFinder.path(cmakeInstallPath, "include")

        systemManager.distributeFiles(
            srcIncludePath,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
            )
        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "lib"),
            sdkOutDir,
            "*.lib",
            )
        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "bin"),
            sdkOutDir,
            "*.dll",
            )

        if buildSettings.ReleaseSpecified():
            systemManager.distributeFiles(
                pathFinder.path(cmakeInstallPath, "bin"),
                sdkOutDir,
                "*.pdb",
                )

        # --------------------------------------------------------------------------

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
