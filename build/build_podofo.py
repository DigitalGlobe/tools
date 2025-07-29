# ------------------------------------------------------------------------------
#
# build_podofo.py
#
# Summary : Builds the PoDoFo library.
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
    DESCRIPTION = "Builds the Podofo library."
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "podofo"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\podofo"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "podofo"
    _DEBUG_SUFFIX = "_d"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\podofo"
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

        # copy APR source to the Build area
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
            "FREETYPE_INCLUDE_DIRS": pathFinder.slasher(pathFinder.path(includeBase, "freetype")),
            "FREETYPE_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"freetype{libSuffix}")),
            "JPEG_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libjpeg")),
            "JPEG_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libjpeg{libSuffix}")),
            "LIBXML2_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase)),
            "LIBXML2_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libxml2{libSuffix}")),
            "PNG_PNG_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libpng")),
            "PNG_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libpng{libSuffix}")),
            "OPENSSL_ROOT_DIR": pathFinder.path(buildPathName, "..", "openssl"),
            "TIFF_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libtiff")),
            "TIFF_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"libtiff{libSuffix}")),
            "ZLIB_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "zlib")),
            "ZLIB_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, f"zlib{libSuffix}")),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        # run CMake
        # -DCMAKE_POLICY_VERSION_MINIMUM is to avoid min compatability errors in CMake
        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DPODOFO_BUILD_LIB_ONLY=ON "
            + f"-DPODOFO_BUILD_STATIC=OFF "
            + f"-DCMAKE_INSTALL_PREFIX={cmakeInstallPath} "
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

        srcIncludePath = pathFinder.path(cmakeInstallPath, "include", "podofo")
        srcBinPath = pathFinder.path(cmakeInstallPath, "bin")
        srcLibPath = pathFinder.path(cmakeInstallPath, "lib")

        systemManager.distributeFiles(
            srcIncludePath,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
        )
        systemManager.copyFile(
            pathFinder.path(
                srcLibPath,
                f"{Program._LIBNAME}.lib",
            ),
            pathFinder.path(sdkOutDir, libName),
        )

        systemManager.copyFile(
            pathFinder.path(
                srcBinPath,
                f"{Program._LIBNAME}.dll",
            ),
            pathFinder.path(sdkOutDir, dllName),
        )

        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(
                    cmakeBuildPath,
                    "src",
                    "podofo",
                    conf,
                    f"{Program._LIBNAME}.pdb",
                ),
                pathFinder.path(sdkOutDir, pdbName),
            )


# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
