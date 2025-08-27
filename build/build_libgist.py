# ------------------------------------------------------------------------------
#
# build_libgist.py
#
# Summary : Builds libGIST
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
    DESCRIPTION = "Builds the libGIST library."
    # ----------------------------------------------------------------------
    # the name of the dynamic solution file
    _FILE_NAME_SOLUTION = "libgist.vcxproj"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "libGIST"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\libGIST"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "libgist"
    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "include"
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\libgist"
    # ----------------------------------------------------------------------
    _PATH_NAME_NMAKE_INSTALL = "install"

    def __init__(self):

        pass

        # ----------------------------------------------------------------------

    def fixnmake(self, pathFinder, systemManager, libName):

        cc32 = f'"s/cc32[[:space:]]*=.*/cc32 = cl/"'
        link32 = f'"s/link32[[:space:]]*=.*/link32 = link/"'
        lib32 = f'"s/LIB32[[:space:]]*=.*/LIB32 = link -lib/"'
        cflags = f'"s/CFLAGS[[:space:]]*=.*/CFLAGS = \/ML \/nologo \/W3 \/DLIBGIST \/DWIN32 \/O2 \/c \/I..\/..\/include \/I..\/libgist/"'
        libname = f'"s/LIBRARY[[:space:]]*=[[:space:]]*\$\(DEST\)\/{libName}/LIBRARY = $(DEST)\/{libName}/"'

        # fix up the Makefile.nt to have the correct paths
        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f"-i.bak -E {cc32} "
            + f"Makefile.NT"
            )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f"-i.bak -E {link32} "
            + f"Makefile.NT"
            )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f"-i.bak -E {lib32} "
            + f"Makefile.NT"
            )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f"-i.bak -E {cflags} "
            + f"Makefile.NT"
            )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

        sedCommandLine = (
            f"{pathFinder.path(pathFinder.PATH_GNU_TOOLS, "sed.exe")} "
            + f"-i.bak -E {libname} "
            + f"Makefile.NT"
            )

        print("cmd: " + sedCommandLine)
        sedResult = systemManager.execute(sedCommandLine)
        if sedResult != 0:
            sys.exit(-1)

    def main(self):
        systemManager = SystemManager()
        pathFinder = PathFinder()

        # process command-line arguments
        buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)

        # initialize environment variables
        systemManager.initializeIncludeEnvironmentVariable(buildSettings.X64Specified())
        systemManager.initializeLibraryEnvironmentVariable(buildSettings.X64Specified())

        # systemManager.setEnvironmentVariableValue("VS170COMNTOOLS",

        # MSBuild is under "Program Files (x86)"
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getMSBuildFileName(buildSettings.X64Specified())
        )

        vcVars = pathFinder.getVCVARSFileName(buildSettings.X64Specified())

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

        nmakeInstallPath = pathFinder.path(buildPathName, Program._PATH_NAME_NMAKE_INSTALL)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"

        platform = "x64" if buildSettings.X64Specified() else "Win32"

        sdkOutDir = pathFinder.getSDKLibPath(buildPathName, platform, conf)

        # remove build dir
        systemManager.removeDirectory(buildPathName)

        # copy UriParser to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        srcdir = pathFinder.path(buildPathName, "src")

        # run Nmake
        nmakeCommandLine = f"nmake -f Makefile.NT"
        cmd = f'"{vcVars}" && {nmakeCommandLine}'

        systemManager.changeDirectory(pathFinder.path(srcdir, "libgist"))
        self.fixnmake(
            systemManager=systemManager,
            pathFinder=pathFinder,
            libName = f"libgist",
            )

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.changeDirectory(pathFinder.path(srcdir, "librtree"))
        self.fixnmake(
            systemManager=systemManager,
            pathFinder=pathFinder,
            libName = f"librtree",
            )

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.changeDirectory(pathFinder.path(srcdir, "libbtree"))
        self.fixnmake(
            systemManager=systemManager,
            pathFinder=pathFinder,
            libName = f"libbtree",
            )

        print("cmd: " + cmd)
        nmakeResult = systemManager.execute(cmd)
        if nmakeResult != 0:
            sys.exit(-1)

        systemManager.removeDirectory(
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE)
        )

        systemManager.distributeFiles(
            pathFinder.path(buildPathName, Program._PATH_NAME_INCLUDE),
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h",
            )

        systemManager.distributeFiles(
            pathFinder.path(buildPathName, "lib"),
            sdkOutDir,
            "*.lib"
            )

        # ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
