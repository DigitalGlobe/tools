# ------------------------------------------------------------------------------
#
# build_liblas.py
#
# Summary : Builds the libLAS library.
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
    DESCRIPTION = "Builds the libLAS library."

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "liblas"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\liblas"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "liblas"
    _BOOST_DEBUG_SUFFIX = "-gd-"
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "include\\liblas"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\liblas"
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

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath(buildPathName, platform, conf)

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        systemManager.removeDirectory(buildPathName)

        # copy source to the Build area
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
            "BOOST_INCLUDEDIR": pathFinder.path(includeBase),
            "BOOST_LIBRARYDIR": sdkOutDir,
            "Boost_FILESYSTEM_LIBRARY": self._findBoostLibrary("boost_filesystem", pathFinder, sdkOutDir, buildSettings.ReleaseSpecified()),
            "Boost_IOSTREAMS_LIBRARY": self._findBoostLibrary("boost_iostreams", pathFinder, sdkOutDir, buildSettings.ReleaseSpecified()),
            "Boost_PROGRAM_OPTIONS_LIBRARY": self._findBoostLibrary("boost_program_options", pathFinder, sdkOutDir, buildSettings.ReleaseSpecified()),
            "Boost_THREAD_LIBRARY": self._findBoostLibrary("boost_thread", pathFinder, sdkOutDir, buildSettings.ReleaseSpecified()),
            # "GDAL_INCLUDE_DIR": pathFinder.path(includeBase, "gdal"),
            "GDAL_LIBRARY": pathFinder.path(sdkOutDir, "gdal.lib"),
            "GEOTIFF_INCLUDE_DIR": pathFinder.path(includeBase, "libgeotiff"),
            "GEOTIFF_LIBRARY": pathFinder.path(sdkOutDir, "geotiff.lib"),
            "JPEG_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libjpeg")),
            # "JPEG_LIBRRY": pathFinder.slasher(pathFinder.path(sdkOutDir, "libjpeg.lib")),
            "TIFF_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "libtiff")),
            "TIFF_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, "libtiff.lib")),
            "ZLIB_INCLUDE_DIR": pathFinder.slasher(pathFinder.path(includeBase, "zlib")),
            "ZLIB_LIBRARY": pathFinder.slasher(pathFinder.path(sdkOutDir, "zlib.lib")),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DWITH_UTILITIES=OFF "
            + f"-DWITH_TESTS=OFF "
            + f"-DCMAKE_BUILD_TYPE={conf} " + f"-DCMAKE_INSTALL_PREFIX={cmakeInstallPath} "
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

        incdir = pathFinder.path(cmakeInstallPath, "include", "liblas")
        libdir = pathFinder.path(cmakeInstallPath, "lib")
        bindir = pathFinder.path(cmakeInstallPath, "bin")

        systemManager.distributeFiles(
            incdir,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h*",
            )

        for f in glob.glob(pathFinder.path(libdir, "*.lib")):
            fname = f[len(libdir) + 1 :]
        fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.lib")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.lib"

        systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        for f in glob.glob(pathFinder.path(bindir, "*.dll")):
            fname = f[len(libdir) + 1 :]
            fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.dll")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.dll"
            systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

        if not buildSettings.ReleaseSpecified():
            pdfdir = pathFinder.path(cmakeBuildPath, conf)
            for f in glob.glob(pathFinder.path(pdfdir, "*.pdb")):
                fname = f[len(pdfdir) + 1 :]
                fname = f"{fname[:fname.find(f"{"" if (buildSettings.ReleaseSpecified()) else "d"}.pdb")]}{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}.pdb"
                systemManager.copyFile(f, pathFinder.path(sdkOutDir, fname))

    def _findBoostLibrary(self, libName, pathFinder, sdkOutDir, release):
        for f in glob.glob(pathFinder.path(sdkOutDir, f"{libName}*.lib")):
            if release:
                if f.find(Program._BOOST_DEBUG_SUFFIX) == -1:
                    return pathFinder.slasher(f)
                elif f.find(Program._BOOST_DEBUG_SUFFIX) != -1:
                    return pathFinder.slasher(f)
                return ""
        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
