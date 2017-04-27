# -*- coding: utf-8 -*-

"""
***************************************************************************
    RasterLayerStatistics.py
    ---------------------
    Date                 : January 2013
    Copyright            : (C) 2013 by Victor Olaya
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
__date__ = 'January 2013'
__copyright__ = '(C) 2013, Victor Olaya'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

import math
import codecs

from processing.core.GeoAlgorithm import GeoAlgorithm
from processing.core.parameters import ParameterRaster
from processing.core.outputs import OutputNumber
from processing.core.outputs import OutputHTML
from processing.tools import dataobjects
from processing.tools import raster


class RasterLayerStatistics(GeoAlgorithm):

    INPUT = 'INPUT'

    MIN = 'MIN'
    MAX = 'MAX'
    SUM = 'SUM'
    MEAN = 'MEAN'
    COUNT = 'COUNT'
    NO_DATA_COUNT = 'NO_DATA_COUNT'
    STD_DEV = 'STD_DEV'
    OUTPUT_HTML_FILE = 'OUTPUT_HTML_FILE'

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Raster layer statistics')
        self.group, self.i18n_group = self.trAlgorithm('Raster tools')

        self.addParameter(ParameterRaster(self.INPUT, self.tr('Input layer')))

        self.addOutput(OutputHTML(self.OUTPUT_HTML_FILE, self.tr('Statistics')))
        self.addOutput(OutputNumber(self.MIN, self.tr('Minimum value')))
        self.addOutput(OutputNumber(self.MAX, self.tr('Maximum value')))
        self.addOutput(OutputNumber(self.SUM, self.tr('Sum')))
        self.addOutput(OutputNumber(self.MEAN, self.tr('Mean value')))
        self.addOutput(OutputNumber(self.COUNT, self.tr('valid cells count')))
        self.addOutput(OutputNumber(self.COUNT, self.tr('No-data cells count')))
        self.addOutput(OutputNumber(self.STD_DEV, self.tr('Standard deviation')))

    def processAlgorithm(self, progress):
        outputFile = self.getOutputValue(self.OUTPUT_HTML_FILE)
        uri = self.getParameterValue(self.INPUT)
        layer = dataobjects.getObjectFromUri(uri)
        values = raster.scanraster(layer, progress)

        n = 0
        nodata = 0
        mean = 0
        M2 = 0
        sum = 0
        minvalue = None
        maxvalue = None

        for v in values:
            if v is not None:
                sum += v
                n = n + 1
                delta = v - mean
                mean = mean + delta / n
                M2 = M2 + delta * (v - mean)
                if minvalue is None:
                    minvalue = v
                    maxvalue = v
                else:
                    minvalue = min(v, minvalue)
                    maxvalue = max(v, maxvalue)
            else:
                nodata += 1

        variance = M2 / (n - 1)
        stddev = math.sqrt(variance)

        data = []
        data.append('Valid cells: ' + unicode(n))
        data.append('No-data cells: ' + unicode(nodata))
        data.append('Minimum value: ' + unicode(minvalue))
        data.append('Maximum value: ' + unicode(maxvalue))
        data.append('Sum: ' + unicode(sum))
        data.append('Mean value: ' + unicode(mean))
        data.append('Standard deviation: ' + unicode(stddev))

        self.createHTML(outputFile, data)

        self.setOutputValue(self.COUNT, n)
        self.setOutputValue(self.NO_DATA_COUNT, nodata)
        self.setOutputValue(self.MIN, minvalue)
        self.setOutputValue(self.MAX, maxvalue)
        self.setOutputValue(self.SUM, sum)
        self.setOutputValue(self.MEAN, mean)
        self.setOutputValue(self.STD_DEV, stddev)

    def createHTML(self, outputFile, algData):
        f = codecs.open(outputFile, 'w', encoding='utf-8')
        f.write('<html><head>')
        f.write('<meta http-equiv="Content-Type" content="text/html; \
                charset=utf-8" /></head><body>')
        for s in algData:
            f.write('<p>' + unicode(s) + '</p>')
        f.write('</body></html>')
        f.close()
