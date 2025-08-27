# ------------------------------------------------------------------------------
#
# build_quazip.py
#
# Summary : Builds the QuaZip library.
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
class Program :

    # --------------------------------------------------------------------------
    # constants

    # ----------------------------------------------------------------------
    # a description of what the script does
    DESCRIPTION = "Builds the Quazip library."

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "quazip"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\quazip"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "quazip"
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\quazip"
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

        buildSourceName = pathFinder.path(
            buildPathName, Program._PATH_NAME_CMAKE_SOURCE
            )
        cmakeBuildPath = pathFinder.path(buildPathName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = pathFinder.path(
            cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL
            )

        systemManager.removeDirectory(cmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"

        platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath(buildPathName, platform, conf)

        os.environ["QTDIR"] = pathFinder.getQtPathName(buildSettings.X64Specified())

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        # systemManager.removeDirectory(buildPathName)

        # copy APR source to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        includeBase = pathFinder.path(
            buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE, ".."
            )
        libSuffix = (
            f'{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib'
            )

        externalLibs = {
            "ZLIB_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "zlib")),
            "ZLIB_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, "zlib.lib")),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        # run CMake
        # -DCMAKE_POLICY_VERSION_MINIMUM is to avoid min compatability errors in CMake
        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f'-A {platform} '
            + f'-DCMAKE_POLICY_VERSION_MINIMUM=3.10 '
            + f'-DBUILD_SHARED_LIBS=ON '
            + f'-DQUAZIP_FETCH_LIBS=OFF '
            + f"-DQUAZIP_INSTALL=ON "
            + f'-DQt5_DIR="{pathFinder.slasher(pathFinder.path(pathFinder.getQtPathName(buildSettings.X64Specified()), "lib", "cmake", "Qt5"))}" '
            + f'-DCMAKE_INSTALL_PREFIX={cmakeInstallPath} '
            + f'{externalLibStr} '
            + f'{buildSourceName}'
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
            + f"{cmakeInstallPath}"
            )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
            sys.exit(-1)

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

        incdir = pathFinder.path(cmakeInstallPath, "include", "QuaZip-Qt6-1.5", Program._LIBNAME)
        libdir = pathFinder.path(cmakeInstallPath, "lib")
        bindir = pathFinder.path(cmakeInstallPath, "bin")

        systemManager.distributeFiles(
            incdir,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
            )

        for f in glob.glob(pathFinder.path(libdir, "*.lib")):
            fname = f[len(libdir) + 1 :]
        fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME) :]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib"

        systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        for f in glob.glob(pathFinder.path(bindir, "*.dll")):
            fname = f[len(bindir) + 1 :]
            fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME)]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.dll"
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        if not buildSettings.ReleaseSpecified():
            pdfdir = pathFinder.path(cmakeBuildPath, Program._LIBNAME, conf)
            for f in glob.glob(pathFinder.path(pdfdir, "*.pdb")):
                fname = f[len(pdfdir) + 1 :]
                fname = f"{fname[:fname.find(Program._LIBNAME) + len(Program._LIBNAME)]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.pdb"
                systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
