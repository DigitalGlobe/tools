# -*- coding: utf-8 -*-

"""
***************************************************************************
    MeanCoords.py
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
from qgis.PyQt.QtCore import QVariant

from qgis.core import QGis, QgsField, QgsFeature, QgsGeometry, QgsPoint

from processing.core.GeoAlgorithm import GeoAlgorithm
from processing.core.parameters import ParameterTableField
from processing.core.parameters import ParameterVector
from processing.core.outputs import OutputVector
from processing.tools import dataobjects, vector

pluginPath = os.path.split(os.path.split(os.path.dirname(__file__))[0])[0]


class MeanCoords(GeoAlgorithm):

    POINTS = 'POINTS'
    WEIGHT = 'WEIGHT'
    OUTPUT = 'OUTPUT'
    UID = 'UID'
    WEIGHT = 'WEIGHT'

    def getIcon(self):
        return QIcon(os.path.join(pluginPath, 'images', 'ftools', 'mean.png'))

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Mean coordinate(s)')
        self.group, self.i18n_group = self.trAlgorithm('Vector analysis tools')

        self.addParameter(ParameterVector(self.POINTS,
                                          self.tr('Input layer'), [ParameterVector.VECTOR_TYPE_ANY]))
        self.addParameter(ParameterTableField(self.WEIGHT,
                                              self.tr('Weight field'), MeanCoords.POINTS,
                                              ParameterTableField.DATA_TYPE_NUMBER, optional=True))
        self.addParameter(ParameterTableField(self.UID,
                                              self.tr('Unique ID field'), MeanCoords.POINTS,
                                              ParameterTableField.DATA_TYPE_NUMBER, optional=True))

        self.addOutput(OutputVector(MeanCoords.OUTPUT, self.tr('Mean coordinates')))

    def processAlgorithm(self, progress):
        layer = dataobjects.getObjectFromUri(self.getParameterValue(self.POINTS))
        weightField = self.getParameterValue(self.WEIGHT)
        uniqueField = self.getParameterValue(self.UID)

        if weightField is None:
            weightIndex = -1
        else:
            weightIndex = layer.fieldNameIndex(weightField)

        if uniqueField is None:
            uniqueIndex = -1
        else:
            uniqueIndex = layer.fieldNameIndex(uniqueField)

        fieldList = [QgsField('MEAN_X', QVariant.Double, '', 24, 15),
                     QgsField('MEAN_Y', QVariant.Double, '', 24, 15),
                     QgsField('UID', QVariant.String, '', 255)]

        writer = self.getOutputFromName(self.OUTPUT).getVectorWriter(
            fieldList, QGis.WKBPoint, layer.crs()
        )

        features = vector.features(layer)
        total = 100.0 / len(features)
        means = {}
        for current, feat in enumerate(features):
            progress.setPercentage(int(current * total))
            if uniqueIndex == -1:
                clazz = "Single class"
            else:
                clazz = unicode(feat.attributes()[uniqueIndex]).strip()
            if weightIndex == -1:
                weight = 1.00
            else:
                try:
                    weight = float(feat.attributes()[weightIndex])
                except:
                    weight = 1.00
            if clazz not in means:
                means[clazz] = (0, 0, 0)

            (cx, cy, totalweight) = means[clazz]
            geom = QgsGeometry(feat.geometry())
            geom = vector.extractPoints(geom)
            for i in geom:
                cx += i.x() * weight
                cy += i.y() * weight
                totalweight += weight
            means[clazz] = (cx, cy, totalweight)

        current = 0
        total = 100.0 / len(means)
        for (clazz, values) in means.iteritems():
            outFeat = QgsFeature()
            cx = values[0] / values[2]
            cy = values[1] / values[2]
            meanPoint = QgsPoint(cx, cy)

            outFeat.setGeometry(QgsGeometry.fromPoint(meanPoint))
            outFeat.setAttributes([cx, cy, clazz])
            writer.addFeature(outFeat)
            current += 1
            progress.setPercentage(int(current * total))

        del writer
