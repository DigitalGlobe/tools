# -*- coding: utf-8 -*-
"""QGIS Unit tests for QgsRelation.

.. note:: This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
"""
__author__ = 'Matthias Kuhn'
__date__ = '07/10/2013'
__copyright__ = 'Copyright 2013, The QGIS Project'
# This will get replaced with a git SHA1 when you do a git archive
__revision__ = '$Format:%H$'

import qgis  # NOQA

from qgis.core import (QgsVectorLayer,
                       QgsFeature,
                       QgsRelation,
                       QgsGeometry,
                       QgsPoint,
                       QgsMapLayerRegistry
                       )
from qgis.testing import start_app, unittest

start_app()


def createReferencingLayer():
    layer = QgsVectorLayer("Point?field=fldtxt:string&field=foreignkey:integer",
                           "referencinglayer", "memory")
    pr = layer.dataProvider()
    f1 = QgsFeature()
    f1.setFields(layer.pendingFields())
    f1.setAttributes(["test1", 123])
    f1.setGeometry(QgsGeometry.fromPoint(QgsPoint(100, 200)))
    f2 = QgsFeature()
    f2.setFields(layer.pendingFields())
    f2.setAttributes(["test2", 123])
    f2.setGeometry(QgsGeometry.fromPoint(QgsPoint(101, 201)))
    assert pr.addFeatures([f1, f2])
    return layer


def createReferencedLayer():
    layer = QgsVectorLayer(
        "Point?field=x:string&field=y:integer&field=z:integer",
        "referencedlayer", "memory")
    pr = layer.dataProvider()
    f1 = QgsFeature()
    f1.setFields(layer.pendingFields())
    f1.setAttributes(["foo", 123, 321])
    f1.setGeometry(QgsGeometry.fromPoint(QgsPoint(1, 1)))
    f2 = QgsFeature()
    f2.setFields(layer.pendingFields())
    f2.setAttributes(["bar", 456, 654])
    f2.setGeometry(QgsGeometry.fromPoint(QgsPoint(2, 2)))
    f3 = QgsFeature()
    f3.setFields(layer.pendingFields())
    f3.setAttributes(["foobar", 789, 554])
    f3.setGeometry(QgsGeometry.fromPoint(QgsPoint(2, 3)))
    assert pr.addFeatures([f1, f2, f3])
    return layer


def formatAttributes(attrs):
    return repr([unicode(a) for a in attrs])


class TestQgsRelation(unittest.TestCase):

    def setUp(self):
        self.referencedLayer = createReferencedLayer()
        self.referencingLayer = createReferencingLayer()
        QgsMapLayerRegistry.instance().addMapLayers([self.referencedLayer, self.referencingLayer])

    def tearDown(self):
        QgsMapLayerRegistry.instance().removeAllMapLayers()

    def test_isValid(self):
        rel = QgsRelation()
        assert not rel.isValid()

        rel.setRelationId('rel1')
        assert not rel.isValid()

        rel.setRelationName('Relation Number One')
        assert not rel.isValid()

        rel.setReferencingLayer(self.referencingLayer.id())
        assert not rel.isValid()

        rel.setReferencedLayer(self.referencedLayer.id())
        assert not rel.isValid()

        rel.addFieldPair('foreignkey', 'y')
        assert rel.isValid()

    def test_getRelatedFeatures(self):
        rel = QgsRelation()

        rel.setRelationId('rel1')
        rel.setRelationName('Relation Number One')
        rel.setReferencingLayer(self.referencingLayer.id())
        rel.setReferencedLayer(self.referencedLayer.id())
        rel.addFieldPair('foreignkey', 'y')

        feat = next(self.referencedLayer.getFeatures())

        self.assertEqual(rel.getRelatedFeaturesFilter(feat), '"foreignkey" = 123')

        it = rel.getRelatedFeatures(feat)
        assert [a.attributes() for a in it] == [[u'test1', 123], [u'test2', 123]]

    def test_getReferencedFeature(self):
        rel = QgsRelation()
        rel.setRelationId('rel1')
        rel.setRelationName('Relation Number One')
        rel.setReferencingLayer(self.referencingLayer.id())
        rel.setReferencedLayer(self.referencedLayer.id())
        rel.addFieldPair('foreignkey', 'y')

        feat = next(self.referencingLayer.getFeatures())

        f = rel.getReferencedFeature(feat)

        assert f.isValid()
        assert f[0] == 'foo'

    def test_fieldPairs(self):
        rel = QgsRelation()

        rel.setRelationId('rel1')
        rel.setRelationName('Relation Number One')
        rel.setReferencingLayer(self.referencingLayer.id())
        rel.setReferencedLayer(self.referencedLayer.id())
        rel.addFieldPair('foreignkey', 'y')

        assert (rel.fieldPairs() == {'foreignkey': 'y'})


if __name__ == '__main__':
    unittest.main()
