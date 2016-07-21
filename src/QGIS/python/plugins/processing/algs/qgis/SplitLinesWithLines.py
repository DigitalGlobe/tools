# -*- coding: utf-8 -*-

"""
***************************************************************************
    SplitLines.py
    ---------------------
    Date                 : November 2014
    Revised              : February 2016
    Copyright            : (C) 2014 by Bernhard Ströbl
    Email                : bernhard dot stroebl at jena dot de
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Bernhard Ströbl'
__date__ = 'November 2014'
__copyright__ = '(C) 2014, Bernhard Ströbl'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

from qgis.core import QGis, QgsFeatureRequest, QgsFeature, QgsGeometry
from processing.core.GeoAlgorithm import GeoAlgorithm
from processing.core.parameters import ParameterVector
from processing.core.outputs import OutputVector
from processing.core.ProcessingLog import ProcessingLog
from processing.tools import dataobjects
from processing.tools import vector


class SplitLinesWithLines(GeoAlgorithm):

    INPUT_A = 'INPUT_A'
    INPUT_B = 'INPUT_B'

    OUTPUT = 'OUTPUT'

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Split lines with lines')
        self.group, self.i18n_group = self.trAlgorithm('Vector overlay tools')
        self.addParameter(ParameterVector(self.INPUT_A,
                                          self.tr('Input layer'), [ParameterVector.VECTOR_TYPE_LINE]))
        self.addParameter(ParameterVector(self.INPUT_B,
                                          self.tr('Split layer'), [ParameterVector.VECTOR_TYPE_LINE]))

        self.addOutput(OutputVector(self.OUTPUT, self.tr('Splitted')))

    def processAlgorithm(self, progress):
        layerA = dataobjects.getObjectFromUri(self.getParameterValue(self.INPUT_A))
        layerB = dataobjects.getObjectFromUri(self.getParameterValue(self.INPUT_B))

        sameLayer = self.getParameterValue(self.INPUT_A) == self.getParameterValue(self.INPUT_B)
        fieldList = layerA.pendingFields()

        writer = self.getOutputFromName(self.OUTPUT).getVectorWriter(fieldList,
                                                                     QGis.WKBLineString, layerA.dataProvider().crs())

        spatialIndex = vector.spatialindex(layerB)

        outFeat = QgsFeature()
        features = vector.features(layerA)
        total = 100.0 / float(len(features))

        for current, inFeatA in enumerate(features):
            inGeom = QgsGeometry(inFeatA.geometry())
            attrsA = inFeatA.attributes()
            outFeat.setAttributes(attrsA)
            inLines = [inGeom]
            lines = spatialIndex.intersects(inGeom.boundingBox())

            if len(lines) > 0:  # hasIntersections
                splittingLines = []

                for i in lines:
                    request = QgsFeatureRequest().setFilterFid(i)
                    inFeatB = layerB.getFeatures(request).next()
                    # check if trying to self-intersect
                    if sameLayer:
                        if inFeatA.id() == inFeatB.id():
                            continue

                    splitGeom = QgsGeometry(inFeatB.geometry())

                    if inGeom.intersects(splitGeom):
                        splittingLines.append(splitGeom)

                if len(splittingLines) > 0:
                    for splitGeom in splittingLines:
                        splitterPList = vector.extractPoints(splitGeom)
                        outLines = []

                        while len(inLines) > 0:
                            inGeom = inLines.pop()
                            inPoints = vector.extractPoints(inGeom)

                            if inGeom.intersects(splitGeom):
                                try:
                                    result, newGeometries, topoTestPoints = inGeom.splitGeometry(splitterPList, False)
                                except:
                                    ProcessingLog.addToLog(ProcessingLog.LOG_WARNING,
                                                           self.tr('Geometry exception while splitting'))
                                    result = 1

                                # splitGeometry: If there are several intersections
                                # between geometry and splitLine, only the first one is considered.
                                if result == 0:  # split occurred

                                    if inPoints == vector.extractPoints(inGeom):
                                        # bug in splitGeometry: sometimes it returns 0 but
                                        # the geometry is unchanged
                                        outLines.append(inGeom)
                                    else:
                                        inLines.append(inGeom)

                                        for aNewGeom in newGeometries:
                                            inLines.append(aNewGeom)
                                else:
                                    outLines.append(inGeom)
                            else:
                                outLines.append(inGeom)

                        inLines = outLines

            for aLine in inLines:
                if len(aLine.asPolyline()) > 2 or \
                        (len(aLine.asPolyline()) == 2 and
                         aLine.asPolyline()[0] != aLine.asPolyline()[1]):
                    # sometimes splitting results in lines of zero length
                    outFeat.setGeometry(aLine)
                    writer.addFeature(outFeat)

            progress.setPercentage(int(current * total))

        del writer
