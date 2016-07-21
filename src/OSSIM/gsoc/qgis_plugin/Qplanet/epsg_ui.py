# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/Users/Shared/Development/gsoc/OssimPlanetSasha/gui/epsg_ui.ui'
#
# Created: Sun Aug 16 02:52:33 2009
#      by: PyQt4 UI code generator 4.5.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(300, 296)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/icons/icons/600px-Brosen_windrose.svg.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Form.setWindowIcon(icon)
        self.verticalLayout_3 = QtGui.QVBoxLayout(Form)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.groupBox = QtGui.QGroupBox(Form)
        self.groupBox.setObjectName("groupBox")
        self.verticalLayout = QtGui.QVBoxLayout(self.groupBox)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.Code_rb = QtGui.QRadioButton(self.groupBox)
        self.Code_rb.setObjectName("Code_rb")
        self.horizontalLayout.addWidget(self.Code_rb)
        self.Params_rb = QtGui.QRadioButton(self.groupBox)
        self.Params_rb.setObjectName("Params_rb")
        self.horizontalLayout.addWidget(self.Params_rb)
        self.Title_rb = QtGui.QRadioButton(self.groupBox)
        self.Title_rb.setObjectName("Title_rb")
        self.horizontalLayout.addWidget(self.Title_rb)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.Code_cb = QtGui.QCheckBox(self.groupBox)
        self.Code_cb.setAutoExclusive(False)
        self.Code_cb.setObjectName("Code_cb")
        self.horizontalLayout_2.addWidget(self.Code_cb)
        self.Params_cb = QtGui.QCheckBox(self.groupBox)
        self.Params_cb.setAutoExclusive(False)
        self.Params_cb.setObjectName("Params_cb")
        self.horizontalLayout_2.addWidget(self.Params_cb)
        self.Title_cb = QtGui.QCheckBox(self.groupBox)
        self.Title_cb.setAutoExclusive(False)
        self.Title_cb.setObjectName("Title_cb")
        self.horizontalLayout_2.addWidget(self.Title_cb)
        self.All_cb = QtGui.QCheckBox(self.groupBox)
        self.All_cb.setAutoExclusive(False)
        self.All_cb.setObjectName("All_cb")
        self.horizontalLayout_2.addWidget(self.All_cb)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem1)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.Search = QtGui.QToolButton(self.groupBox)
        self.Search.setObjectName("Search")
        self.horizontalLayout_4.addWidget(self.Search)
        self.OutList = QtGui.QComboBox(self.groupBox)
        self.OutList.setObjectName("OutList")
        self.horizontalLayout_4.addWidget(self.OutList)
        self.verticalLayout.addLayout(self.horizontalLayout_4)
        spacerItem2 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem2)
        self.verticalLayout_3.addWidget(self.groupBox)
        self.printout = QtGui.QTextEdit(Form)
        self.printout.setTextInteractionFlags(QtCore.Qt.TextSelectableByKeyboard|QtCore.Qt.TextSelectableByMouse)
        self.printout.setObjectName("printout")
        self.verticalLayout_3.addWidget(self.printout)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Epsg", None, QtGui.QApplication.UnicodeUTF8))
        self.Code_rb.setText(QtGui.QApplication.translate("Form", "Code", None, QtGui.QApplication.UnicodeUTF8))
        self.Params_rb.setText(QtGui.QApplication.translate("Form", "Params", None, QtGui.QApplication.UnicodeUTF8))
        self.Title_rb.setText(QtGui.QApplication.translate("Form", "Title", None, QtGui.QApplication.UnicodeUTF8))
        self.Code_cb.setText(QtGui.QApplication.translate("Form", "Code", None, QtGui.QApplication.UnicodeUTF8))
        self.Params_cb.setText(QtGui.QApplication.translate("Form", "Param", None, QtGui.QApplication.UnicodeUTF8))
        self.Title_cb.setText(QtGui.QApplication.translate("Form", "Title", None, QtGui.QApplication.UnicodeUTF8))
        self.All_cb.setText(QtGui.QApplication.translate("Form", "All", None, QtGui.QApplication.UnicodeUTF8))
        self.Search.setText(QtGui.QApplication.translate("Form", "->", None, QtGui.QApplication.UnicodeUTF8))

import images_rc
