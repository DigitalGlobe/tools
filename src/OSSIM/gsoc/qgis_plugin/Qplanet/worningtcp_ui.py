# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/Users/Shared/Development/gsoc/OssimPlanetSasha/gui/worningtcp.ui'
#
# Created: Fri Jul 31 11:46:19 2009
#      by: PyQt4 UI code generator 4.5
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(480, 71)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/icons/icons/epi.gif"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Form.setWindowIcon(icon)
        self.verticalLayout = QtGui.QVBoxLayout(Form)
        self.verticalLayout.setObjectName("verticalLayout")
        self.worningmessage = QtGui.QGroupBox(Form)
        self.worningmessage.setObjectName("worningmessage")
        self.verticalLayout_21 = QtGui.QVBoxLayout(self.worningmessage)
        self.verticalLayout_21.setObjectName("verticalLayout_21")
        self.label = QtGui.QLabel(self.worningmessage)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName("label")
        self.verticalLayout_21.addWidget(self.label)
        self.verticalLayout.addWidget(self.worningmessage)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Worning", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Form", "Use the preference setting to set TCP preference", None, QtGui.QApplication.UnicodeUTF8))

import images_rc
