# -*- coding: utf-8 -*-

"""
***************************************************************************
    ExtractByAttribute.py
    ---------------------
    Date                 : May 2010
    Copyright            : (C) 2010 by Michael Minn
    Email                : pyqgis at michaelminn dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Michael Minn'
__date__ = 'May 2010'
__copyright__ = '(C) 2010, Michael Minn'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

from qgis.PyQt.QtCore import QVariant
from qgis.core import QgsExpression, QgsFeatureRequest
from processing.core.GeoAlgorithm import GeoAlgorithm
from processing.core.GeoAlgorithmExecutionException import GeoAlgorithmExecutionException
from processing.core.parameters import ParameterVector
from processing.core.parameters import ParameterTableField
from processing.core.parameters import ParameterSelection
from processing.core.parameters import ParameterString
from processing.core.outputs import OutputVector
from processing.tools import dataobjects


class ExtractByAttribute(GeoAlgorithm):
    INPUT = 'INPUT'
    FIELD = 'FIELD'
    OPERATOR = 'OPERATOR'
    VALUE = 'VALUE'
    OUTPUT = 'OUTPUT'

    OPERATORS = ['=',
                 '!=',
                 '>',
                 '>=',
                 '<',
                 '<=',
                 'begins with',
                 'contains'
                 ]

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Extract by attribute')
        self.group, self.i18n_group = self.trAlgorithm('Vector selection tools')

        self.i18n_operators = ['=',
                               '!=',
                               '>',
                               '>=',
                               '<',
                               '<=',
                               self.tr('begins with'),
                               self.tr('contains')]

        self.addParameter(ParameterVector(self.INPUT,
                                          self.tr('Input Layer'), [ParameterVector.VECTOR_TYPE_ANY]))
        self.addParameter(ParameterTableField(self.FIELD,
                                              self.tr('Selection attribute'), self.INPUT))
        self.addParameter(ParameterSelection(self.OPERATOR,
                                             self.tr('Operator'), self.i18n_operators))
        self.addParameter(ParameterString(self.VALUE, self.tr('Value')))

        self.addOutput(OutputVector(self.OUTPUT, self.tr('Extracted (attribute)')))

    def processAlgorithm(self, progress):
        layer = dataobjects.getObjectFromUri(self.getParameterValue(self.INPUT))
        fieldName = self.getParameterValue(self.FIELD)
        operator = self.OPERATORS[self.getParameterValue(self.OPERATOR)]
        value = self.getParameterValue(self.VALUE)

        fields = layer.pendingFields()
        writer = self.getOutputFromName(self.OUTPUT).getVectorWriter(fields,
                                                                     layer.wkbType(), layer.crs())

        idx = layer.fieldNameIndex(fieldName)
        fieldType = fields[idx].type()

        if fieldType != QVariant.String and operator in self.OPERATORS[-2:]:
            op = ''.join(['"%s", ' % o for o in self.OPERATORS[-2:]])
            raise GeoAlgorithmExecutionException(
                self.tr('Operators %s can be used only with string fields.' % op))

        if fieldType in [QVariant.Int, QVariant.Double, QVariant.UInt, QVariant.LongLong, QVariant.ULongLong]:
            expr = '"%s" %s %s' % (fieldName, operator, value)
        elif fieldType == QVariant.String:
            if operator not in self.OPERATORS[-2:]:
                expr = """"%s" %s '%s'""" % (fieldName, operator, value)
            elif operator == 'begins with':
                expr = """"%s" LIKE '%s%%'""" % (fieldName, value)
            elif operator == 'contains':
                expr = """"%s" LIKE '%%%s%%'""" % (fieldName, value)
        elif fieldType in [QVariant.Date, QVariant.DateTime]:
            expr = """"%s" %s '%s'""" % (fieldName, operator, value)
        else:
            raise GeoAlgorithmExecutionException(
                self.tr('Unsupported field type "%s"' % fields[idx].typeName()))

        expression = QgsExpression(expr)
        if not expression.hasParserError():
            req = QgsFeatureRequest(expression)
        else:
            raise GeoAlgorithmExecutionException(expression.parserErrorString())

        for f in layer.getFeatures(req):
            writer.addFeature(f)

        del writer
