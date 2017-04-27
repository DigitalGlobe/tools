# -*- coding: utf-8 -*-

"""
***************************************************************************
    PointsLayerFromTable.py
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
__date__ = 'August 2013'
__copyright__ = '(C) 2013, Victor Olaya'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

from qgis.core import QGis
from qgis.core import QgsCoordinateReferenceSystem
from qgis.core import QgsFeature
from qgis.core import QgsGeometry
from qgis.core import QgsPoint
from processing.core.GeoAlgorithm import GeoAlgorithm
from processing.core.parameters import ParameterTable
from processing.core.parameters import ParameterTableField
from processing.core.parameters import ParameterCrs
from processing.core.outputs import OutputVector
from processing.tools import dataobjects, vector


class PointsLayerFromTable(GeoAlgorithm):

    INPUT = 'INPUT'
    XFIELD = 'XFIELD'
    YFIELD = 'YFIELD'
    OUTPUT = 'OUTPUT'
    TARGET_CRS = 'TARGET_CRS'

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Points layer from table')
        self.group, self.i18n_group = self.trAlgorithm('Vector creation tools')
        self.addParameter(ParameterTable(self.INPUT,
                                         self.tr('Input layer')))
        self.addParameter(ParameterTableField(self.XFIELD,
                                              self.tr('X field'), self.INPUT, ParameterTableField.DATA_TYPE_ANY))
        self.addParameter(ParameterTableField(self.YFIELD,
                                              self.tr('Y field'), self.INPUT, ParameterTableField.DATA_TYPE_ANY))
        self.addParameter(ParameterCrs(self.TARGET_CRS,
                                       self.tr('Target CRS'), 'EPSG:4326'))
        self.addOutput(OutputVector(self.OUTPUT, self.tr('Points from table')))

    def processAlgorithm(self, progress):
        source = self.getParameterValue(self.INPUT)
        vlayer = dataobjects.getObjectFromUri(source)
        output = self.getOutputFromName(self.OUTPUT)
        vprovider = vlayer.dataProvider()
        fields = vprovider.fields()
        writer = output.getVectorWriter(fields, QGis.WKBPoint, self.crs)
        xfieldindex = vlayer.fieldNameIndex(self.getParameterValue(self.XFIELD))
        yfieldindex = vlayer.fieldNameIndex(self.getParameterValue(self.YFIELD))

        crsId = self.getParameterValue(self.TARGET_CRS)
        targetCrs = QgsCoordinateReferenceSystem()
        targetCrs.createFromUserInput(crsId)
        self.crs = targetCrs

        outFeat = QgsFeature()
        features = vector.features(vlayer)
        total = 100.0 / len(features)
        for current, feature in enumerate(features):
            progress.setPercentage(int(current * total))
            attrs = feature.attributes()
            try:
                x = float(attrs[xfieldindex])
                y = float(attrs[yfieldindex])
            except:
                continue
            pt = QgsPoint(x, y)
            outFeat.setGeometry(QgsGeometry.fromPoint(pt))
            outFeat.setAttributes(attrs)
            writer.addFeature(outFeat)

        del writer
