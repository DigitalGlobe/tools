# -*- coding: utf-8 -*-

"""
***************************************************************************
    fillnodata.py
    ---------------------
    Date                 : August 2012
    Copyright            : (C) 2012 by Victor Olaya
    Email                : volayaf at gmail dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Victor Olaya'
__date__ = 'August 2012'
__copyright__ = '(C) 2012, Victor Olaya'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

import os

from processing.algs.gdal.GdalAlgorithm import GdalAlgorithm

from processing.core.parameters import ParameterRaster
from processing.core.parameters import ParameterNumber
from processing.core.parameters import ParameterBoolean
from processing.core.outputs import OutputRaster

from processing.tools.system import isWindows

from processing.algs.gdal.GdalUtils import GdalUtils

pluginPath = os.path.split(os.path.split(os.path.dirname(__file__))[0])[0]


class fillnodata(GdalAlgorithm):

    INPUT = 'INPUT'
    DISTANCE = 'DISTANCE'
    ITERATIONS = 'ITERATIONS'
    BAND = 'BAND'
    MASK = 'MASK'
    NO_DEFAULT_MASK = 'NO_DEFAULT_MASK'
    OUTPUT = 'OUTPUT'

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Fill nodata')
        self.group, self.i18n_group = self.trAlgorithm('[GDAL] Analysis')
        self.addParameter(ParameterRaster(
            self.INPUT, self.tr('Input layer'), False))
        self.addParameter(ParameterNumber(self.DISTANCE,
                                          self.tr('Search distance'), 0, 9999, 100))
        self.addParameter(ParameterNumber(self.ITERATIONS,
                                          self.tr('Smooth iterations'), 0, 9999, 0))
        self.addParameter(ParameterNumber(self.BAND,
                                          self.tr('Band to operate on'), 1, 9999, 1))
        self.addParameter(ParameterRaster(self.MASK,
                                          self.tr('Validity mask'), True))
        self.addParameter(ParameterBoolean(self.NO_DEFAULT_MASK,
                                           self.tr('Do not use default validity mask'), False))

        self.addOutput(OutputRaster(self.OUTPUT, self.tr('Filled')))

    def getConsoleCommands(self):
        output = self.getOutputValue(self.OUTPUT)

        arguments = []
        arguments.append('-md')
        arguments.append(unicode(self.getParameterValue(self.DISTANCE)))

        if self.getParameterValue(self.ITERATIONS) != 0:
            arguments.append('-si')
            arguments.append(unicode(self.getParameterValue(self.ITERATIONS)))

        arguments.append('-b')
        arguments.append(unicode(self.getParameterValue(self.BAND)))

        mask = self.getParameterValue(self.MASK)
        if mask is not None:
            arguments.append('-mask')
            arguments.append(mask)

        if self.getParameterValue(self.NO_DEFAULT_MASK):
            arguments.append('-nomask')

        arguments.append('-of')
        arguments.append(GdalUtils.getFormatShortNameFromFilename(output))

        arguments.append(self.getParameterValue(self.INPUT))
        arguments.append(output)

        commands = []
        if isWindows():
            commands = ['cmd.exe', '/C ', 'gdal_fillnodata.bat',
                        GdalUtils.escapeAndJoin(arguments)]
        else:
            commands = ['gdal_fillnodata.py',
                        GdalUtils.escapeAndJoin(arguments)]

        return commands
