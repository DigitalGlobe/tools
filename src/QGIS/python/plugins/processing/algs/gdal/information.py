# -*- coding: utf-8 -*-

"""
***************************************************************************
    information.py
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
from processing.core.parameters import ParameterRaster
from processing.core.parameters import ParameterBoolean
from processing.core.outputs import OutputHTML
from processing.algs.gdal.GdalUtils import GdalUtils

pluginPath = os.path.split(os.path.split(os.path.dirname(__file__))[0])[0]


class information(GdalAlgorithm):

    INPUT = 'INPUT'
    OUTPUT = 'OUTPUT'
    NOGCP = 'NOGCP'
    NOMETADATA = 'NOMETADATA'

    def getIcon(self):
        return QIcon(os.path.join(pluginPath, 'images', 'gdaltools', 'raster-info.png'))

    def commandLineName(self):
        return "gdalorg:rasterinfo"

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Information')
        self.group, self.i18n_group = self.trAlgorithm('[GDAL] Miscellaneous')
        self.addParameter(ParameterRaster(information.INPUT,
                                          self.tr('Input layer'), False))
        self.addParameter(ParameterBoolean(information.NOGCP,
                                           self.tr('Suppress GCP info'), False))
        self.addParameter(ParameterBoolean(information.NOMETADATA,
                                           self.tr('Suppress metadata info'), False))
        self.addOutput(OutputHTML(information.OUTPUT,
                                  self.tr('Layer information')))

    def getConsoleCommands(self):
        arguments = []
        if self.getParameterValue(information.NOGCP):
            arguments.append('-nogcp')
        if self.getParameterValue(information.NOMETADATA):
            arguments.append('-nomd')
        arguments.append(self.getParameterValue(information.INPUT))
        return ['gdalinfo', GdalUtils.escapeAndJoin(arguments)]

    def processAlgorithm(self, progress):
        GdalUtils.runGdal(self.getConsoleCommands(), progress)
        output = self.getOutputValue(information.OUTPUT)
        f = open(output, 'w')
        f.write('<pre>')
        for s in GdalUtils.getConsoleOutput()[1:]:
            f.write(unicode(s))
        f.write('</pre>')
        f.close()
