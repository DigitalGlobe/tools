# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/Users/sasha/gis/PlanetSasha/gui/GrassShell_ui.ui'
#
# Created: Sun Aug  1 11:21:40 2010
#      by: PyQt4 UI code generator 4.7.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(289, 222)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/icons/512 Terminal.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Form.setWindowIcon(icon)
        self.verticalLayout = QtGui.QVBoxLayout(Form)
        self.verticalLayout.setObjectName("verticalLayout")
        self.outimageinfo = QtGui.QTextEdit(Form)
        self.outimageinfo.setTextInteractionFlags(QtCore.Qt.TextSelectableByKeyboard|QtCore.Qt.TextSelectableByMouse)
        self.outimageinfo.setObjectName("outimageinfo")
        self.verticalLayout.addWidget(self.outimageinfo)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.command = QtGui.QLineEdit(Form)
        self.command.setObjectName("command")
        self.horizontalLayout.addWidget(self.command)
        self.hist = QtGui.QComboBox(Form)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.hist.sizePolicy().hasHeightForWidth())
        self.hist.setSizePolicy(sizePolicy)
        self.hist.setMinimumSize(QtCore.QSize(10, 10))
        self.hist.setMaximumSize(QtCore.QSize(25, 16777215))
        self.hist.setObjectName("hist")
        self.horizontalLayout.addWidget(self.hist)
        self.verticalLayout.addLayout(self.horizontalLayout)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Grass Shell", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
