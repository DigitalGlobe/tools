# ------------------------------------------------------------------------------
#
# build_qt.py
#
# Summary : Builds Qt libs
#
# ------------------------------------------------------------------------------

import glob
import os
import sys

from BuildSettingSet import *
from PathFinder import *
from SystemManager import *


class Program:
    DESCRIPTION = "Builds Qt."

    _PATH_NAME_BINARY_X86 = "..\\sdk\\x86\\bin"
    _PATH_NAME_BINARY_X64 = "..\\sdk\\x64\\bin"

    _PATH_NAME_BUILD = "Qt"
    _QT_VERSION = "6.8"

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _PATH_NAME_SOURCE = "..\\src\\Qt"

    _FIREBIRD_BASE = "..\\..\\firebird"
    _FIREBIRD_INCLUDE = "include"
    _FIREBIRD_LIB_PATH = "lib"
    _FIREBIRD_LIB = "fbclient_ms"

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

        vcVars = pathFinder.getVCVARSFileName(buildSettings.X64Specified())

        # append gnuwin32 path for flex and bison
        # Qt needs specific versions, so they are placed within the Qt source tree
        gnuToolsPath = pathFinder.path(
            systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD),
            "gnuwin32",
            "bin"
        )
        systemManager.appendToPathEnvironmentVariable(gnuToolsPath)

        # Append  qtbase/bin to the path
        qtBaseBin = pathFinder.path(
            systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD),
            "qtbase",
            "bin"
        )
        systemManager.appendToPathEnvironmentVariable(qtBaseBin)

        # remove build dir
        systemManager.removeDirectory(buildPathName)
        systemManager.makeDirectory(buildPathName)
        systemManager.changeDirectory(buildPathName)

        # determine path names
        print("Getting Paths")
        buildPathName = systemManager.getCurrentRelativePathName( Program._PATH_NAME_BUILD)
        sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)
        binPathName = systemManager.getCurrentRelativePathName(pathFinder.path("..", "..","Qt",Program._QT_VERSION))

        # QT out-of-source builds need to be in a parallel directory
        buildPathQTSrcName = pathFinder.path(buildPathName, "Qt")
        buildPathQTBuildName = pathFinder.path(buildPathName, "build")

        print("build path: " + buildPathName)
        print("source path: " + sourcePathName)
        print("install path: " + binPathName)

        # link the source to the build directory w/o copying
        cmd = f'mklink /j {buildPathQTSrcName} {sourcePathName}'
        print("cmd: " + cmd)
        result = systemManager.execute(cmd)
        if result != 0:
            sys.exit(-1)

        systemManager.makeDirectory(buildPathQTBuildName)
        systemManager.changeDirectory(buildPathQTBuildName)

        binDir = binPathName
        if buildSettings.X64Specified():
            binDir = pathFinder.path(binPathName, "x64")
        else:
            binDir = pathFinder.path(binPathName, "x86")

        if buildSettings.ReleaseSpecified():
            buildType = "-release"
        else:
            buildType = "-debug"

        firebirdBase = pathFinder.path(
            pathFinder.path(buildPathName, Program._FIREBIRD_BASE),
            ("x64" if buildSettings.X64Specified() else "x86"),
        )

        # firebirdBase = pathFinder.path(pathFinder.path(buildPathName, Program._FIREBIRD_BASE), 'x86')
        # firebirdInclude = pathFinder.slasher(pathFinder.path(firebirdBase, Program._FIREBIRD_INCLUDE))
        # firebirdLib = pathFinder.slasher(pathFinder.path(firebirdBase, Program._FIREBIRD_LIB_PATH))

        os.environ["Interbase_ROOT"] = pathFinder.slasher(firebirdBase)

        cmdConfigure = (
            f'{pathFinder.path(sourcePathName, "configure")} '
            + f'-prefix "{binDir}" '
            + f'{buildType} '
            # + f"-mp "
            # + f"-developer-build "
            + f'-platform win32-msvc '
            + f'-opensource '
            + f'-confirm-license '
            + f'-shared '
            + f'-opengl dynamic '
            + f'-qt-libpng '
            + f'-qt-libjpeg '
            + f'-qt-zlib '
            # + f"-no-compile-examples "
            + f'-nomake examples '
            + f'-nomake tests '
            + f'-no-icu '
            + f'-skip qtbluetooth '
            + f'-skip qtcharts '
            + f'-skip qtconnectivity '
            + f'-skip qtdatavis3d '
            + f'-skip qtdoc '
            + f'-skip qtfeedback '
            + f'-skip qtgraphs '
            + f'-skip qtlocation '
            + f'-skip qtpim '
            + f'-skip qtpositioning '
            + f'-skip qt3d '
            + f'-skip qtquick3d '
            + f'-skip qtquick3dphysics '
            + f'-skip qtquickeffectmaker '
            + f'-skip qtsensors '
            + f'-skip qtserialbus '
            + f'-skip qtserialport '
            + f'-skip qtspeech '
            + f'-skip qtvirtualkeyboard '
            + f'-skip qtwayland '
            + f'-skip qtwebchannel '
            + f'-skip qtwebengine '
            + f'-skip qtwebview '
            + f'-sql-ibase '
        )

        print(f"cwd: {os.getcwd()}")
        cmd = f'"{vcVars}" && {cmdConfigure}'
        print("cmd: " + cmd)

        result = systemManager.execute(cmd)
        if result != 0:
            sys.exit(-1)

        # nmake --------------------------------------------------------------------------------
        cmd = f'"{vcVars}" && ninja'
        print("command: " + cmd)
        result = systemManager.execute(cmd)
        if result != 0:
            sys.exit(-1)

        # install -------------------------------------------------------------------------------
        cmdInstall = "ninja install"
        print("command: " + cmdInstall)
        result = systemManager.execute(cmdInstall)
        if result != 0:
            sys.exit(-1)

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
