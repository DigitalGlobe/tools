# ------------------------------------------------------------------------------
#
# build_crypto.py
#
# Summary : Builds the Crypto++ library.
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
    DESCRIPTION = "Builds the crypto library."
    # ----------------------------------------------------------------------
    # the name of the dynamic solution file
    _FILE_NAME_SOLUTION = "cryptlib.vcxproj"

    # ----------------------------------------------------------------------
    # the name of the path that will contain intermediary build files
    _PATH_NAME_BUILD = "crypto"
    # ----------------------------------------------------------------------
    # the name of the path that contains the source code
    _PATH_NAME_SOURCE = "..\\src\\crypto"
    # ----------------------------------------------------------------------

    _PATH_NAME_DISTRIBUTION_X86 = "..\\sdk\\x86\\lib"
    _PATH_NAME_DISTRIBUTION_X64 = "..\\sdk\\x64\\lib"

    _LIBNAME = "cryptlib"
    _DEBUG_SUFFIX = "_d"

    _QT_DIR_X86 = "..\\..\\QT\\5.7\\x86\\lib\cmake\\qt5"
    _QT_DIR_X64 = "..\\..\\QT\\5.7\\x64\\lib\cmake\\qt5"

    # the name of the path for all include files
    _PATH_NAME_INCLUDE = "."
    # ----------------------------------------------------------------------
    # the name of the distribution path for all include files
    _PATH_NAME_DISTRIBUTION_INCLUDE = "..\\..\\include\\crypto"
    # ----------------------------------------------------------------------
    _PATH_NAME_BUILD_PATH = "build"

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
        xmlUtils = XmlUtils()

        # process command-line arguments
        buildSettings = BuildSettingSet.fromCommandLine(Program.DESCRIPTION)

        # initialize environment variables
        systemManager.initializeIncludeEnvironmentVariable(buildSettings.X64Specified())
        systemManager.initializeLibraryEnvironmentVariable(buildSettings.X64Specified())

        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getVisualStudioBinPathName(buildSettings.X64Specified())
        )
        systemManager.appendToPathEnvironmentVariable(
            Program._QT_DIR_X64
            if buildSettings.X64Specified()
            else Program._QT_DIR_X86 + "\\bin"
        )

        # MSBuild is under "Program Files (x86)"
        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getMSBuildFileName(buildSettings.X64Specified())
        )

        systemManager.appendToPathEnvironmentVariable(
            pathFinder.getWindowsSdkBinPathName(buildSettings.X64Specified())
        )

        # get the paths
        buildPathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_BUILD)
        sourcePathName = systemManager.getCurrentRelativePathName(Program._PATH_NAME_SOURCE)

        nmakeBuildPath = pathFinder.path(buildPathName, Program._PATH_NAME_BUILD_PATH)

        systemManager.removeDirectory(nmakeBuildPath)

        sdkOutDir = pathFinder.path(
            buildPathName,
            "..",
            (
                Program._PATH_NAME_DISTRIBUTION_X64
                if buildSettings.X64Specified()
                else Program._PATH_NAME_DISTRIBUTION_X86
            )
        )

        # remove build dir
        systemManager.changeDirectory(sourcePathName)
        # systemManager.removeDirectory(buildPathName)

        # copy UriParser to the Build area
        systemManager.copyDirectory(sourcePathName, buildPathName)

        # start building
        systemManager.changeDirectory(buildPathName)

        systemManager.makeDirectory(nmakeBuildPath)
        systemManager.changeDirectory(nmakeBuildPath)

        conf = "Release" if (buildSettings.ReleaseSpecified()) else "Debug"
        platform = "x64" if buildSettings.X64Specified() else "Win32"

        # build the solution
        solutionFileName = pathFinder.path(buildPathName, Program._FILE_NAME_SOLUTION)

        msBuildCommandLine = ( f'"{pathFinder.getMSBuildFileName(buildSettings.X64Specified())}" '
                              + f'/p:platform={platform} '
                              + f'"{solutionFileName}"'
        )

        libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
        pdbName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"

        buildOutDir   = pathFinder.path( buildPathName, 'build' )
        propfile   = pathFinder.path( buildPathName, 'linker.props' )

        msBuildCommandLine += f' /p:OutDir="{buildOutDir}"'
        msBuildCommandLine += f' /p:TargetExtension=dll'
        msBuildCommandLine += f' /p:SolutionDir="{buildPathName}"'
        msBuildCommandLine += f' /p:TargetName={Program._LIBNAME}{"" if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX}'
        msBuildCommandLine += f' /p:Configuration={conf}'

        linkerprops = {'OutputFile':pathFinder.path( buildOutDir, libName )}
        if buildSettings.ReleaseSpecified():
            linkerprops['DebugSymbols'] = 'false'
        else:
            linkerprops['DebugSymbols'] = 'true'
            linkerprops['DebugType'] = 'full'
            linkerprops['ProgramDatabaseFile'] = '$(OutDir)\\' + pdbName

        if buildSettings.ReleaseSpecified():
            compprops = {'DebugInformationFormat':'None'}
        else:
            compprops = {'DebugInformationFormat':'ProgramDatabase', 'ProgramDataBaseFileName':pathFinder.path( buildOutDir, pdbName )}

        xmlUtils.buildLib(conf, platform, compprops, linkerprops, propfile)

        msBuildCommandLine +=  f' /p:ForceImportBeforeCppTargets="{propfile}"'

        print('cmd: ' + msBuildCommandLine)

        msbuildResult = systemManager.execute(msBuildCommandLine)
        if (msbuildResult != 0) :
            sys.exit(-1)

        systemManager.distributeFiles(
            buildPathName,
            pathFinder.path(buildPathName, Program._PATH_NAME_DISTRIBUTION_INCLUDE),
            "*.h",
        )

        libName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".lib"
        pdbName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".pdb"
        dllName = Program._LIBNAME + ( '' if ( buildSettings.ReleaseSpecified() )  else Program._DEBUG_SUFFIX) + ".dll"

        systemManager.copyFile(
            pathFinder.path(buildOutDir, libName),
            pathFinder.path(sdkOutDir, libName),
        )

        # systemManager.copyFile(
        #     pathFinder.path(buildOutDir, dllName),
        #     pathFinder.path(sdkOutDir, dllName),
        # )

        if not buildSettings.ReleaseSpecified():
            systemManager.copyFile(
                pathFinder.path(buildOutDir, pdbName),
                pathFinder.path(sdkOutDir, pdbName),
            )

        # ----------------------------------------------------------------------


# --------------------------------------------------------------------------

# ------------------------------------------------------------------------------
Program().main()
# ------------------------------------------------------------------------------
