# ------------------------------------------------------------------------------
#
# build_apr.py
#
# Summary : Builds the APR library.
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
    DESCRIPTION = "Builds the APR library."

    # the name of the release makefile
    _FILE_NAME_MAKEFILE = "Makefile.win"
    _FILE_NAME_ICONV_MAKEFILE = "apriconv.mak"
    # ----------------------------------------------------------------------

    _LIBNAME = "libapr"
    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "apr"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\apr"
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------
    # the pattern for binary files
    _FILE_PATTERN_BINARY = "*.exe"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 32-bit binary files
    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit binary files
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"

    # the name of the path that will contain built 32-bit library files
    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    # ----------------------------------------------------------------------
    # the name of the path that will contain built 64-bit library files
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_APR_INCLUDE = "..\\..\\include\\apr"
    _PATH_NAME_DISTRIBUTION_APR_UTIL_INCLUDE = "..\\..\\include\\apr-util"
    # ----------------------------------------------------------------------

    _PATH_NAME_NMAKE_INSTALL = "install"

    _PATH_NAME_APR_SOURCE = "apr"
    _PATH_NAME_APR_ICONV_SOURCE = "apr-iconv"
    _PATH_NAME_APR_UTIL_SOURCE = "apr-util"

    # the name of the path that contains the cmake files
    _PATH_NAME_CMAKE_BUILD = "cmake-build"
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

        vcVars = pathFinder.getVCVARSFileName(buildSettings.X64Specified())

        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getWindowsSdkBinPathName(buildSettings.X64Specified())
        )

        buildPathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_BUILD
            )
        sourcePathName = systemManager.getCurrentRelativePathName(
            Program._PATH_NAME_SOURCE
            )

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
            platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath( buildPathName, platform, conf)

        # remove build dir
        systemManager.removeDirectory(buildPathName)
        systemManager.copyDirectory(sourcePathName, buildPathName)

        buildSourceName = pathFinder.path(buildPathName, Program._PATH_NAME_APR_SOURCE)
        cmakeBuildPath = pathFinder.path(buildSourceName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = pathFinder.path(
            cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL
            )

        systemManager.removeDirectory(cmakeBuildPath)
        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmake

        cmake = pathFinder.path(cmakeBuildPath, "..", "CMakeLists.txt")
        # modify the project name
        sedResult = systemManager.replaceInFile(
            cmake,
            "libaprapp-1",
            f"libaprapp{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}",
            )
        if sedResult != 0:
        sys.exit(-1)

        # modify the library name
        sedResult = systemManager.replaceInFile(
            cmake,
            "apr_libname\\s*libapr-1",
            f"apr_libname libapr{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}",
            )

        if sedResult != 0:
        sys.exit(-1)

        incdir = pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_APR_INCLUDE, "..")

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_BUILD_TYPE={conf} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DAPR_BUILD_SHARED=ON "
            + f"-DAPR_BUILD_STATIC=OFF "
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

        systemManager.removeDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_APR_INCLUDE)
        )
        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "include"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_APR_INCLUDE),
            "*.h",
            )

        libdir = pathFinder.path(cmakeInstallPath, "lib")
        bindir = pathFinder.path(cmakeInstallPath, "bin")

        # we need to rename the debug libs to have a _d suffix (they have a 'd' suffix now)
        for f in glob.glob(pathFinder.path(libdir, "*.lib")):
            fname = f[len(bindir) + 1 :]
        systemManager.copyFile(f, sdkOutDir)

        for f in glob.glob(pathFinder.path(bindir, "*.dll")):
            fname = f[len(bindir) + 1 :]
        systemManager.copyFile(f, sdkOutDir)

        if not buildSettings.ReleaseSpecified():
        for f in glob.glob(pathFinder.path(libdir, "*.pdb")):
            fname = f[len(bindir) + 1 :]
        systemManager.copyFile(f, sdkOutDir)

        # systemManager.changeDirectory(pathFinder.path(buildPathName, "apr-iconv"))

        # nmakeCommandLine = (
        # f'nmake /f "{Program._FILE_NAME_ICONV_MAKEFILE}" '
        # + f"PREFIX={pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL) } "
        # + f'CFG="apriconv - {"x64" if (buildSettings.X64Specified()) else "Win32"} {"Release" if (buildSettings.ReleaseSpecified()) else "Debug"}" '
        # + f"USEMAK=1 "
        # + f" "
        # )

        # cmd = f'"{vcVars}" && {nmakeCommandLine}'

        # print("cmd: " + cmd)
        # nmakeResult = systemManager.execute(cmd)
        # if nmakeResult != 0:
        # sys.exit(-1)

        buildSourceName = pathFinder.path(buildPathName, Program._PATH_NAME_APR_UTIL_SOURCE)
        cmakeBuildPath = pathFinder.path(buildSourceName, Program._PATH_NAME_CMAKE_BUILD)
        cmakeInstallPath = pathFinder.path(
            cmakeBuildPath, Program._PATH_NAME_CMAKE_INSTALL
            )

        systemManager.removeDirectory(cmakeBuildPath)
        systemManager.makeDirectory(cmakeBuildPath)
        systemManager.changeDirectory(cmakeBuildPath)

        cmake = pathFinder.path(cmakeBuildPath, "..", "CMakeLists.txt")

        # modify the project name
        sedResult = systemManager.replaceInFile(
            cmake,
            "aprutil-1",
            f"aprutil{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}",
            )
        if sedResult != 0:
        sys.exit(-1)

        # modify the library name
        sedResult = systemManager.replaceInFile(
            cmake,
            "libaprutil-1",
            f"libaprutil{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}",
            )

        # modify the library name
        sedResult = systemManager.replaceInFile(
            cmake,
            "apr_crypto_openssl-1",
            f"apr_crypto_openssl{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}",
            )

        # modify the library name
        sedResult = systemManager.replaceInFile(
            cmake,
            "apr_dbd_odbc-1",
            f"apr_dbd_odbc{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}",
            )

        # modify the library name
        sedResult = self._replaceInFile(
            pathFinder,
            systemManager,
            cmake,
            "apr_ldap-1",
            f"apr_ldap{"" if (buildSettings.ReleaseSpecified()) else Program._DEBUG_SUFFIX}",
            )

        includeBase = pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_APR_INCLUDE, "..")
        externalLibs = {
            "APR_INCLUDE_DIR": pathFinder.path(incdir, "apr"),
            "APR_LIBRARIES": pathFinder.path(sdkOutDir, f"libapr.lib"),
            "EXPAT_LIBRARY": pathFinder.path(sdkOutDir, f"libexpat.lib"),
            "EXPAT_INCLUDE_DIR": pathFinder.path(incdir, "expat"),
            "OPENSSL_ROOT_DIR": pathFinder.path(buildPathName, "..", "openssl"),
        }

        externalLibStr = ""
        for [key, val] in externalLibs.items():
            externalLibStr += f'-D{key}="{val}" '

        cmakeCommandLine = (
            f'{pathFinder.getCMakeFileName()} -G "{pathFinder.VISUAL_STUDIO_VERSION}" '
            + f"-A {platform} "
            + f"-DCMAKE_POLICY_VERSION_MINIMUM=3.10 "
            + f"-DBUILD_SHARED_LIBS=ON "
            + f"-DAPU_HAVE_CRYPTO=ON "
            + f"-DCMAKE_BUILD_TYPE={conf} "
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
            + f"--prefix {cmakeInstallPath} "
            )

        print("cmake: " + cmakeCommandLine)
        cmakeResult = systemManager.execute(cmakeCommandLine)
        if cmakeResult != 0:
        sys.exit(-1)

        systemManager.removeDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_APR_UTIL_INCLUDE)
        )
        systemManager.distributeFiles(
            pathFinder.path(cmakeInstallPath, "include"),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_APR_UTIL_INCLUDE),
            "*.h",
            )

        libdir = pathFinder.path(cmakeInstallPath, "lib")
        bindir = pathFinder.path(cmakeInstallPath, "bin")

        # we need to rename the debug libs to have a _d suffix (they have a 'd' suffix now)
        for f in glob.glob(pathFinder.path(libdir, "*.lib")):
            fname = f[len(bindir) + 1 :]
        systemManager.copyFile(f, sdkOutDir)

        for f in glob.glob(pathFinder.path(bindir, "*.dll")):
            fname = f[len(bindir) + 1 :]
        systemManager.copyFile(f, sdkOutDir)

        if not buildSettings.ReleaseSpecified():
        for f in glob.glob(pathFinder.path(bindir, "*.pdb")):
            fname = f[len(bindir) + 1 :]
        systemManager.copyFile(f, sdkOutDir)

        # --------------------------------------------------------------------------

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
