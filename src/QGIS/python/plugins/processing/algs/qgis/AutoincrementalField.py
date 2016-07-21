# -*- coding: utf-8 -*-

"""
***************************************************************************
    AutoincrementalField.py
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

from qgis.PyQt.QtCore import QVariant
from qgis.core import QgsField, QgsFeature
from processing.core.GeoAlgorithm import GeoAlgorithm
from processing.core.parameters import ParameterVector
from processing.core.outputs import OutputVector
from processing.tools import dataobjects, vector


class AutoincrementalField(GeoAlgorithm):

    INPUT = 'INPUT'
    OUTPUT = 'OUTPUT'

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Add autoincremental field')
        self.group, self.i18n_group = self.trAlgorithm('Vector table tools')
        self.addParameter(ParameterVector(self.INPUT,
                                          self.tr('Input layer'), [ParameterVector.VECTOR_TYPE_ANY]))
        self.addOutput(OutputVector(self.OUTPUT, self.tr('Incremented')))

    def processAlgorithm(self, progress):
        output = self.getOutputFromName(self.OUTPUT)
        vlayer = \
            dataobjects.getObjectFromUri(self.getParameterValue(self.INPUT))
        vprovider = vlayer.dataProvider()
        fields = vprovider.fields()
        fields.append(QgsField('AUTO', QVariant.Int))
        writer = output.getVectorWriter(fields, vprovider.geometryType(),
                                        vlayer.crs())
        outFeat = QgsFeature()
        features = vector.features(vlayer)
        total = 100.0 / len(features)
        for current, feat in enumerate(features):
            progress.setPercentage(int(current * total))
            geom = feat.geometry()
            outFeat.setGeometry(geom)
            attrs = feat.attributes()
            attrs.append(current)
            outFeat.setAttributes(attrs)
            writer.addFeature(outFeat)
        del writer
