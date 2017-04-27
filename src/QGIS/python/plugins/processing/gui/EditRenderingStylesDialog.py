# -*- coding: utf-8 -*-

"""
***************************************************************************
    EditRenderingStylesDialog.py
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

from qgis.PyQt import uic
from qgis.PyQt.QtCore import Qt
from qgis.PyQt.QtWidgets import QDialog, QHeaderView, QTableWidgetItem

from processing.gui.RenderingStyles import RenderingStyles
from processing.gui.RenderingStyleFilePanel import RenderingStyleFilePanel
from processing.core.outputs import OutputRaster
from processing.core.outputs import OutputVector

pluginPath = os.path.split(os.path.dirname(__file__))[0]
WIDGET, BASE = uic.loadUiType(
    os.path.join(pluginPath, 'ui', 'DlgRenderingStyles.ui'))


class EditRenderingStylesDialog(BASE, WIDGET):

    def __init__(self, alg):
        super(EditRenderingStylesDialog, self).__init__(None)
        self.setupUi(self)

        self.alg = alg.getCopy()

        self.tblStyles.horizontalHeader().setResizeMode(QHeaderView.Stretch)
        self.setWindowTitle(self.alg.name)

        self.valueItems = {}
        self.dependentItems = {}
        self.setTableContent()

    def setTableContent(self):
        numOutputs = 0
        for output in self.alg.outputs:
            if isinstance(output, (OutputVector, OutputRaster)):
                if not output.hidden:
                    numOutputs += 1
        self.tblStyles.setRowCount(numOutputs)

        i = 0
        for output in self.alg.outputs:
            if isinstance(output, (OutputVector, OutputRaster)):
                if not output.hidden:
                    item = QTableWidgetItem(output.description + '<'
                                            + output.__class__.__name__ + '>')
                    item.setFlags(Qt.ItemIsEnabled)
                    self.tblStyles.setItem(i, 0, item)
                    item = RenderingStyleFilePanel()
                    style = \
                        RenderingStyles.getStyle(self.alg.commandLineName(),
                                                 output.name)
                    if style:
                        item.setText(unicode(style))
                    self.valueItems[output.name] = item
                    self.tblStyles.setCellWidget(i, 1, item)
                    self.tblStyles.setRowHeight(i, 22)
            i += 1

    def accept(self):
        styles = {}
        for key in self.valueItems.keys():
            styles[key] = unicode(self.valueItems[key].getValue())
        RenderingStyles.addAlgStylesAndSave(self.alg.commandLineName(), styles)

        QDialog.accept(self)

    def reject(self):
        QDialog.reject(self)
