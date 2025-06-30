# ------------------------------------------------------------------------------
#
# build_firebird.py
#
# Summary : Builds the firebird library.
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
    DESCRIPTION = "Builds the firebird library."

    _PATH_NAME_INSTALLATION_DIR_X86 = "..\\..\\firebird\\x86"
    _PATH_NAME_INSTALLATION_DIR_X64 = "..\\..\\firebird\\x64"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "firebird"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\firebird"
    # ----------------------------------------------------------------------

    # ----------------------------------------------------------------------

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

        # systemManager.setEnvironmentVariableValue("VS170COMNTOOLS",

        # MSBuild is under "Program Files (x86)"
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getMSBuildFileName(buildSettings.X64Specified())
        )

        systemManager.appendToPathEnvironmentVariable(
            pathFinder.PATH_SED_EXECUTABLE
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

        srcInstallDir = os.path.join(
            buildPathName,
            f'output_{"x64" if buildSettings.X64Specified() else "Win32"}{"" if buildSettings.ReleaseSpecified() else "_debug"}'
        )
        installDir = os.path.join(
            buildPathName,
            (
                Program._PATH_NAME_INSTALLATION_DIR_X64
                if buildSettings.X64Specified()
                else Program._PATH_NAME_INSTALLATION_DIR_X86
            ),
        )

        # need to remove the install dir first, it interferes with the build
        systemManager.removeDirectory(installDir)

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        systemManager.removeDirectory(buildPathName)

        # copy UriParser to the Build area
        systemManager.copyDirectory( sourcePathName, buildPathName)

        buildPathName = os.path.join(buildPathName, "builds\\win32")

        # start building
        systemManager.changeDirectory(buildPathName)

        os.environ["FB_PROCESSOR_ARCHITECTURE"] = (
            "AMD64" if buildSettings.X64Specified() else "Win32"
        )

        # cmd = 'run_all.bat clean ' + ( '' if ( buildSettings.ReleaseSpecified() )  else 'debug')
        # print('cmd: ' + cmd)
        #
        # result = systemManager.execute(cmd)
        # if (result != 0) :
        #    sys.exit(-1)

        result = 0
        cmd = f'"{vcVars}" && clean_all.bat clean {"release" if (buildSettings.ReleaseSpecified()) else "debug"}'
        print("cmd: " + cmd)

        result = systemManager.execute(cmd)
        if result != 0:
            sys.exit(-1)

        cmd = f'"{vcVars}" && make_icu.bat clean {"release" if (buildSettings.ReleaseSpecified()) else "debug"}'
        print("cmd: " + cmd)
        result = systemManager.execute(cmd)
        if result != 0:
            sys.exit(-1)

        cmd = f'"{vcVars}" && make_boot.bat clean {"release" if (buildSettings.ReleaseSpecified()) else "debug"}'
        print("cmd: " + cmd)
        result = systemManager.execute(cmd)
        if result != 0:
            sys.exit(-1)

        cmd = f'"{vcVars}" && make_all.bat clean {"release" if (buildSettings.ReleaseSpecified()) else "debug"}'
        print("cmd: " + cmd)
        result = systemManager.execute(cmd)
        if result != 0:
            sys.exit(-1)

        systemManager.distributeFiles(srcInstallDir, installDir, "*")

        # firebird install docs recommend changing the fbclient_ms.lib to gds32_ms.lib (the borland name?), so we'll make a copy
        systemManager.copyFile(
            os.path.join(installDir, "lib//fbclient_ms.lib"),
            os.path.join(installDir, "lib//gds32_ms.lib"),
        )

        # To use the embedded server, we need to remove fbclient.dll and rename the fbembed.dll to fbclient.dll
        # newer versions of firebird don't habe fbembed.dll anymore
        if os.path.exists(os.path.join(installDir, "bin//fbembed.dll")):
            systemManager.removeFile(os.path.join(installDir, "bin//fbclient.dll"))
            systemManager.copyFile(
                os.path.join(installDir, "bin//fbembed.dll"),
                os.path.join(installDir, "bin//fbclient.dll"),
            )
            systemManager.removeFile(os.path.join(installDir, "bin//fbembed.dll"))


# --------------------------------------------------------------------------

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
