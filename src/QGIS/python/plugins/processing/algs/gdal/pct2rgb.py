# -*- coding: utf-8 -*-

"""
***************************************************************************
    pct2rgb.py
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

from qgis.PyQt.QtGui import QIcon

from processing.algs.gdal.GdalAlgorithm import GdalAlgorithm
from processing.tools.system import isWindows
from processing.core.parameters import ParameterRaster
from processing.core.parameters import ParameterSelection
from processing.core.outputs import OutputRaster
from processing.algs.gdal.GdalUtils import GdalUtils

pluginPath = os.path.split(os.path.split(os.path.dirname(__file__))[0])[0]


class pct2rgb(GdalAlgorithm):

    INPUT = 'INPUT'
    OUTPUT = 'OUTPUT'
    NBAND = 'NBAND'

    def getIcon(self):
        return QIcon(os.path.join(pluginPath, 'images', 'gdaltools', '8-to-24-bits.png'))

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('PCT to RGB')
        self.group, self.i18n_group = self.trAlgorithm('[GDAL] Conversion')
        self.addParameter(ParameterRaster(pct2rgb.INPUT,
                                          self.tr('Input layer'), False))
        options = []
        for i in range(25):
            options.append(unicode(i + 1))
        self.addParameter(ParameterSelection(pct2rgb.NBAND,
                                             self.tr('Band to convert'), options))
        self.addOutput(OutputRaster(pct2rgb.OUTPUT, self.tr('PCT to RGB')))

    def getConsoleCommands(self):
        arguments = []
        arguments.append('-b')
        arguments.append(unicode(self.getParameterValue(pct2rgb.NBAND) + 1))
        arguments.append('-of')
        out = self.getOutputValue(pct2rgb.OUTPUT)
        arguments.append(GdalUtils.getFormatShortNameFromFilename(out))
        arguments.append(self.getParameterValue(pct2rgb.INPUT))
        arguments.append(out)

        commands = []
        if isWindows():
            commands = ['cmd.exe', '/C ', 'pct2rgb.bat',
                        GdalUtils.escapeAndJoin(arguments)]
        else:
            commands = ['pct2rgb.py', GdalUtils.escapeAndJoin(arguments)]

        return commands
