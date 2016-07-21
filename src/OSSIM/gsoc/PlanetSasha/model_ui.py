# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/Users/Shared/ossim/trunk/gsoc/PlanetSasha/gui/model_ui.ui'
#
# Created: Mon Feb 14 21:57:24 2011
#      by: PyQt4 UI code generator 4.8.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(384, 254)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/cubo.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Form.setWindowIcon(icon)
        self.verticalLayout_9 = QtGui.QVBoxLayout(Form)
        self.verticalLayout_9.setObjectName(_fromUtf8("verticalLayout_9"))
        self.scrollArea = QtGui.QScrollArea(Form)
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName(_fromUtf8("scrollArea"))
        self.scrollAreaWidgetContents = QtGui.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 343, 728))
        self.scrollAreaWidgetContents.setObjectName(_fromUtf8("scrollAreaWidgetContents"))
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.scrollAreaWidgetContents)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.KmlGeneralgroupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.KmlGeneralgroupBox.setObjectName(_fromUtf8("KmlGeneralgroupBox"))
        self.verticalLayout_8 = QtGui.QVBoxLayout(self.KmlGeneralgroupBox)
        self.verticalLayout_8.setObjectName(_fromUtf8("verticalLayout_8"))
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.label = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.label.setObjectName(_fromUtf8("label"))
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.kmlname = QtGui.QLineEdit(self.KmlGeneralgroupBox)
        self.kmlname.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.kmlname.setObjectName(_fromUtf8("kmlname"))
        self.gridLayout.addWidget(self.kmlname, 0, 2, 1, 1)
        self.label_2 = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)
        self.kmllabel = QtGui.QLineEdit(self.KmlGeneralgroupBox)
        self.kmllabel.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.kmllabel.setObjectName(_fromUtf8("kmllabel"))
        self.gridLayout.addWidget(self.kmllabel, 1, 2, 1, 1)
        self.Iconlabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.Iconlabel.setObjectName(_fromUtf8("Iconlabel"))
        self.gridLayout.addWidget(self.Iconlabel, 2, 0, 1, 1)
        self.LineWidthlabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.LineWidthlabel.setObjectName(_fromUtf8("LineWidthlabel"))
        self.gridLayout.addWidget(self.LineWidthlabel, 3, 0, 1, 1)
        self.altitudemode = QtGui.QComboBox(self.KmlGeneralgroupBox)
        self.altitudemode.setObjectName(_fromUtf8("altitudemode"))
        self.altitudemode.addItem(_fromUtf8(""))
        self.altitudemode.addItem(_fromUtf8(""))
        self.altitudemode.addItem(_fromUtf8(""))
        self.gridLayout.addWidget(self.altitudemode, 3, 2, 1, 1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.selectmodel = QtGui.QToolButton(self.KmlGeneralgroupBox)
        self.selectmodel.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.selectmodel.setText(_fromUtf8(""))
        self.selectmodel.setIcon(icon)
        self.selectmodel.setObjectName(_fromUtf8("selectmodel"))
        self.horizontalLayout.addWidget(self.selectmodel)
        self.modelpath = QtGui.QLineEdit(self.KmlGeneralgroupBox)
        self.modelpath.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.modelpath.setObjectName(_fromUtf8("modelpath"))
        self.horizontalLayout.addWidget(self.modelpath)
        self.gridLayout.addLayout(self.horizontalLayout, 2, 2, 1, 1)
        self.verticalLayout_8.addLayout(self.gridLayout)
        self.verticalLayout_2.addWidget(self.KmlGeneralgroupBox)
        self.KmlColorgroupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.KmlColorgroupBox.setObjectName(_fromUtf8("KmlColorgroupBox"))
        self.gridLayout_29 = QtGui.QGridLayout(self.KmlColorgroupBox)
        self.gridLayout_29.setObjectName(_fromUtf8("gridLayout_29"))
        self.tabWidget = QtGui.QTabWidget(self.KmlColorgroupBox)
        self.tabWidget.setObjectName(_fromUtf8("tabWidget"))
        self.tab = QtGui.QWidget()
        self.tab.setObjectName(_fromUtf8("tab"))
        self.verticalLayout_3 = QtGui.QVBoxLayout(self.tab)
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.gridLayout_3 = QtGui.QGridLayout()
        self.gridLayout_3.setObjectName(_fromUtf8("gridLayout_3"))
        self.label_3 = QtGui.QLabel(self.tab)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.gridLayout_3.addWidget(self.label_3, 1, 0, 1, 1)
        self.CameraHeading = QtGui.QLineEdit(self.tab)
        self.CameraHeading.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.CameraHeading.setAlignment(QtCore.Qt.AlignCenter)
        self.CameraHeading.setObjectName(_fromUtf8("CameraHeading"))
        self.gridLayout_3.addWidget(self.CameraHeading, 1, 1, 1, 1)
        self.CameraTilt = QtGui.QLineEdit(self.tab)
        self.CameraTilt.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.CameraTilt.setAlignment(QtCore.Qt.AlignCenter)
        self.CameraTilt.setObjectName(_fromUtf8("CameraTilt"))
        self.gridLayout_3.addWidget(self.CameraTilt, 2, 1, 1, 1)
        self.CameraLatitude = QtGui.QLineEdit(self.tab)
        self.CameraLatitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.CameraLatitude.setAlignment(QtCore.Qt.AlignCenter)
        self.CameraLatitude.setObjectName(_fromUtf8("CameraLatitude"))
        self.gridLayout_3.addWidget(self.CameraLatitude, 3, 1, 1, 1)
        self.CameraLongitude = QtGui.QLineEdit(self.tab)
        self.CameraLongitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.CameraLongitude.setAlignment(QtCore.Qt.AlignCenter)
        self.CameraLongitude.setObjectName(_fromUtf8("CameraLongitude"))
        self.gridLayout_3.addWidget(self.CameraLongitude, 4, 1, 1, 1)
        self.CameraRange = QtGui.QLineEdit(self.tab)
        self.CameraRange.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.CameraRange.setAlignment(QtCore.Qt.AlignCenter)
        self.CameraRange.setObjectName(_fromUtf8("CameraRange"))
        self.gridLayout_3.addWidget(self.CameraRange, 5, 1, 1, 1)
        self.CameraAltitude = QtGui.QLineEdit(self.tab)
        self.CameraAltitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.CameraAltitude.setAlignment(QtCore.Qt.AlignCenter)
        self.CameraAltitude.setObjectName(_fromUtf8("CameraAltitude"))
        self.gridLayout_3.addWidget(self.CameraAltitude, 6, 1, 1, 1)
        self.label_4 = QtGui.QLabel(self.tab)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.gridLayout_3.addWidget(self.label_4, 2, 0, 1, 1)
        self.label_5 = QtGui.QLabel(self.tab)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.gridLayout_3.addWidget(self.label_5, 3, 0, 1, 1)
        self.label_6 = QtGui.QLabel(self.tab)
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.gridLayout_3.addWidget(self.label_6, 4, 0, 1, 1)
        self.label_7 = QtGui.QLabel(self.tab)
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.gridLayout_3.addWidget(self.label_7, 5, 0, 1, 1)
        self.label_8 = QtGui.QLabel(self.tab)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.gridLayout_3.addWidget(self.label_8, 6, 0, 1, 1)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem)
        self.setcamera = QtGui.QPushButton(self.tab)
        self.setcamera.setStyleSheet(_fromUtf8("QPushButton {\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"     min-width: 44px;\n"
" }\n"
"\n"
" QPushButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }"))
        self.setcamera.setObjectName(_fromUtf8("setcamera"))
        self.horizontalLayout_2.addWidget(self.setcamera)
        self.gridLayout_3.addLayout(self.horizontalLayout_2, 7, 1, 1, 1)
        self.verticalLayout_3.addLayout(self.gridLayout_3)
        self.tabWidget.addTab(self.tab, _fromUtf8(""))
        self.tab_2 = QtGui.QWidget()
        self.tab_2.setObjectName(_fromUtf8("tab_2"))
        self.verticalLayout_4 = QtGui.QVBoxLayout(self.tab_2)
        self.verticalLayout_4.setObjectName(_fromUtf8("verticalLayout_4"))
        self.gridLayout_4 = QtGui.QGridLayout()
        self.gridLayout_4.setObjectName(_fromUtf8("gridLayout_4"))
        self.label_9 = QtGui.QLabel(self.tab_2)
        self.label_9.setObjectName(_fromUtf8("label_9"))
        self.gridLayout_4.addWidget(self.label_9, 1, 0, 1, 1)
        self.label_10 = QtGui.QLabel(self.tab_2)
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.gridLayout_4.addWidget(self.label_10, 2, 0, 1, 1)
        self.ModelTilt = QtGui.QLineEdit(self.tab_2)
        self.ModelTilt.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.ModelTilt.setObjectName(_fromUtf8("ModelTilt"))
        self.gridLayout_4.addWidget(self.ModelTilt, 2, 1, 1, 1)
        self.label_11 = QtGui.QLabel(self.tab_2)
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.gridLayout_4.addWidget(self.label_11, 3, 0, 1, 1)
        self.ModelLatitude = QtGui.QLineEdit(self.tab_2)
        self.ModelLatitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.ModelLatitude.setObjectName(_fromUtf8("ModelLatitude"))
        self.gridLayout_4.addWidget(self.ModelLatitude, 3, 1, 1, 1)
        self.label_12 = QtGui.QLabel(self.tab_2)
        self.label_12.setObjectName(_fromUtf8("label_12"))
        self.gridLayout_4.addWidget(self.label_12, 4, 0, 1, 1)
        self.ModelLongitude = QtGui.QLineEdit(self.tab_2)
        self.ModelLongitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.ModelLongitude.setObjectName(_fromUtf8("ModelLongitude"))
        self.gridLayout_4.addWidget(self.ModelLongitude, 4, 1, 1, 1)
        self.label_13 = QtGui.QLabel(self.tab_2)
        self.label_13.setObjectName(_fromUtf8("label_13"))
        self.gridLayout_4.addWidget(self.label_13, 5, 0, 1, 1)
        self.ModelRange = QtGui.QLineEdit(self.tab_2)
        self.ModelRange.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.ModelRange.setObjectName(_fromUtf8("ModelRange"))
        self.gridLayout_4.addWidget(self.ModelRange, 5, 1, 1, 1)
        self.label_14 = QtGui.QLabel(self.tab_2)
        self.label_14.setObjectName(_fromUtf8("label_14"))
        self.gridLayout_4.addWidget(self.label_14, 6, 0, 1, 1)
        self.ModelAltitude = QtGui.QLineEdit(self.tab_2)
        self.ModelAltitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.ModelAltitude.setObjectName(_fromUtf8("ModelAltitude"))
        self.gridLayout_4.addWidget(self.ModelAltitude, 6, 1, 1, 1)
        self.ModelHeading = QtGui.QLineEdit(self.tab_2)
        self.ModelHeading.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.ModelHeading.setObjectName(_fromUtf8("ModelHeading"))
        self.gridLayout_4.addWidget(self.ModelHeading, 1, 1, 1, 1)
        self.verticalLayout_4.addLayout(self.gridLayout_4)
        self.setmodel = QtGui.QPushButton(self.tab_2)
        self.setmodel.setObjectName(_fromUtf8("setmodel"))
        self.verticalLayout_4.addWidget(self.setmodel)
        self.tabWidget.addTab(self.tab_2, _fromUtf8(""))
        self.gridLayout_29.addWidget(self.tabWidget, 0, 0, 1, 1)
        self.verticalLayout_2.addWidget(self.KmlColorgroupBox)
        self.groupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.verticalLayout_5 = QtGui.QVBoxLayout(self.groupBox)
        self.verticalLayout_5.setObjectName(_fromUtf8("verticalLayout_5"))
        self.tabWidget_2 = QtGui.QTabWidget(self.groupBox)
        self.tabWidget_2.setObjectName(_fromUtf8("tabWidget_2"))
        self.tab_3 = QtGui.QWidget()
        self.tab_3.setObjectName(_fromUtf8("tab_3"))
        self.verticalLayout = QtGui.QVBoxLayout(self.tab_3)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.gridLayout_5 = QtGui.QGridLayout()
        self.gridLayout_5.setObjectName(_fromUtf8("gridLayout_5"))
        self.label_15 = QtGui.QLabel(self.tab_3)
        self.label_15.setObjectName(_fromUtf8("label_15"))
        self.gridLayout_5.addWidget(self.label_15, 0, 0, 1, 1)
        self.MLongitude = QtGui.QLineEdit(self.tab_3)
        self.MLongitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.MLongitude.setAlignment(QtCore.Qt.AlignCenter)
        self.MLongitude.setObjectName(_fromUtf8("MLongitude"))
        self.gridLayout_5.addWidget(self.MLongitude, 0, 1, 1, 1)
        self.label_16 = QtGui.QLabel(self.tab_3)
        self.label_16.setObjectName(_fromUtf8("label_16"))
        self.gridLayout_5.addWidget(self.label_16, 1, 0, 1, 1)
        self.label_18 = QtGui.QLabel(self.tab_3)
        self.label_18.setObjectName(_fromUtf8("label_18"))
        self.gridLayout_5.addWidget(self.label_18, 2, 0, 1, 1)
        self.MLatitude = QtGui.QLineEdit(self.tab_3)
        self.MLatitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.MLatitude.setAlignment(QtCore.Qt.AlignCenter)
        self.MLatitude.setObjectName(_fromUtf8("MLatitude"))
        self.gridLayout_5.addWidget(self.MLatitude, 1, 1, 1, 1)
        self.MAltitude = QtGui.QLineEdit(self.tab_3)
        self.MAltitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.MAltitude.setAlignment(QtCore.Qt.AlignCenter)
        self.MAltitude.setObjectName(_fromUtf8("MAltitude"))
        self.gridLayout_5.addWidget(self.MAltitude, 2, 1, 1, 1)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem1)
        self.setlonlat = QtGui.QPushButton(self.tab_3)
        self.setlonlat.setStyleSheet(_fromUtf8("QPushButton {\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"     min-width: 44px;\n"
" }\n"
"\n"
" QPushButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }"))
        self.setlonlat.setObjectName(_fromUtf8("setlonlat"))
        self.horizontalLayout_3.addWidget(self.setlonlat)
        self.gridLayout_5.addLayout(self.horizontalLayout_3, 3, 1, 1, 1)
        self.verticalLayout.addLayout(self.gridLayout_5)
        spacerItem2 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem2)
        self.tabWidget_2.addTab(self.tab_3, _fromUtf8(""))
        self.tab_4 = QtGui.QWidget()
        self.tab_4.setObjectName(_fromUtf8("tab_4"))
        self.verticalLayout_6 = QtGui.QVBoxLayout(self.tab_4)
        self.verticalLayout_6.setObjectName(_fromUtf8("verticalLayout_6"))
        self.gridLayout_6 = QtGui.QGridLayout()
        self.gridLayout_6.setObjectName(_fromUtf8("gridLayout_6"))
        self.label_28 = QtGui.QLabel(self.tab_4)
        self.label_28.setObjectName(_fromUtf8("label_28"))
        self.gridLayout_6.addWidget(self.label_28, 0, 0, 1, 1)
        self.label_29 = QtGui.QLabel(self.tab_4)
        self.label_29.setObjectName(_fromUtf8("label_29"))
        self.gridLayout_6.addWidget(self.label_29, 1, 0, 1, 1)
        self.label_30 = QtGui.QLabel(self.tab_4)
        self.label_30.setObjectName(_fromUtf8("label_30"))
        self.gridLayout_6.addWidget(self.label_30, 2, 0, 1, 1)
        self.MHeading = QtGui.QLineEdit(self.tab_4)
        self.MHeading.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.MHeading.setAlignment(QtCore.Qt.AlignCenter)
        self.MHeading.setObjectName(_fromUtf8("MHeading"))
        self.gridLayout_6.addWidget(self.MHeading, 0, 1, 1, 1)
        self.MTilt = QtGui.QLineEdit(self.tab_4)
        self.MTilt.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.MTilt.setAlignment(QtCore.Qt.AlignCenter)
        self.MTilt.setObjectName(_fromUtf8("MTilt"))
        self.gridLayout_6.addWidget(self.MTilt, 1, 1, 1, 1)
        self.MRoll = QtGui.QLineEdit(self.tab_4)
        self.MRoll.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.MRoll.setAlignment(QtCore.Qt.AlignCenter)
        self.MRoll.setObjectName(_fromUtf8("MRoll"))
        self.gridLayout_6.addWidget(self.MRoll, 2, 1, 1, 1)
        self.verticalLayout_6.addLayout(self.gridLayout_6)
        spacerItem3 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_6.addItem(spacerItem3)
        self.tabWidget_2.addTab(self.tab_4, _fromUtf8(""))
        self.tab_5 = QtGui.QWidget()
        self.tab_5.setObjectName(_fromUtf8("tab_5"))
        self.verticalLayout_7 = QtGui.QVBoxLayout(self.tab_5)
        self.verticalLayout_7.setObjectName(_fromUtf8("verticalLayout_7"))
        self.gridLayout_7 = QtGui.QGridLayout()
        self.gridLayout_7.setObjectName(_fromUtf8("gridLayout_7"))
        self.label_36 = QtGui.QLabel(self.tab_5)
        self.label_36.setObjectName(_fromUtf8("label_36"))
        self.gridLayout_7.addWidget(self.label_36, 0, 0, 1, 1)
        self.label_37 = QtGui.QLabel(self.tab_5)
        self.label_37.setObjectName(_fromUtf8("label_37"))
        self.gridLayout_7.addWidget(self.label_37, 1, 0, 1, 1)
        self.label_38 = QtGui.QLabel(self.tab_5)
        self.label_38.setObjectName(_fromUtf8("label_38"))
        self.gridLayout_7.addWidget(self.label_38, 2, 0, 1, 1)
        self.Xscale = QtGui.QLineEdit(self.tab_5)
        self.Xscale.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Xscale.setAlignment(QtCore.Qt.AlignCenter)
        self.Xscale.setObjectName(_fromUtf8("Xscale"))
        self.gridLayout_7.addWidget(self.Xscale, 0, 1, 1, 1)
        self.Yscale = QtGui.QLineEdit(self.tab_5)
        self.Yscale.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Yscale.setAlignment(QtCore.Qt.AlignCenter)
        self.Yscale.setObjectName(_fromUtf8("Yscale"))
        self.gridLayout_7.addWidget(self.Yscale, 1, 1, 1, 1)
        self.Zscale = QtGui.QLineEdit(self.tab_5)
        self.Zscale.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Zscale.setAlignment(QtCore.Qt.AlignCenter)
        self.Zscale.setObjectName(_fromUtf8("Zscale"))
        self.gridLayout_7.addWidget(self.Zscale, 2, 1, 1, 1)
        self.verticalLayout_7.addLayout(self.gridLayout_7)
        spacerItem4 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_7.addItem(spacerItem4)
        self.tabWidget_2.addTab(self.tab_5, _fromUtf8(""))
        self.verticalLayout_5.addWidget(self.tabWidget_2)
        self.verticalLayout_2.addWidget(self.groupBox)
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)
        self.verticalLayout_9.addWidget(self.scrollArea)
        self.horizontalLayout_8 = QtGui.QHBoxLayout()
        self.horizontalLayout_8.setObjectName(_fromUtf8("horizontalLayout_8"))
        self.reset = QtGui.QPushButton(Form)
        self.reset.setStyleSheet(_fromUtf8("QPushButton {\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"     min-width: 44px;\n"
" }\n"
"\n"
" QPushButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }"))
        self.reset.setText(_fromUtf8(""))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/mActionDraw.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.reset.setIcon(icon1)
        self.reset.setObjectName(_fromUtf8("reset"))
        self.horizontalLayout_8.addWidget(self.reset)
        spacerItem5 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem5)
        self.save = QtGui.QPushButton(Form)
        self.save.setStyleSheet(_fromUtf8("QPushButton {\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"     min-width: 44px;\n"
" }\n"
"\n"
" QPushButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }"))
        self.save.setObjectName(_fromUtf8("save"))
        self.horizontalLayout_8.addWidget(self.save)
        spacerItem6 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem6)
        self.opendir = QtGui.QPushButton(Form)
        self.opendir.setStyleSheet(_fromUtf8("QPushButton {\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"     min-width: 44px;\n"
" }\n"
"\n"
" QPushButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }"))
        self.opendir.setObjectName(_fromUtf8("opendir"))
        self.horizontalLayout_8.addWidget(self.opendir)
        self.verticalLayout_9.addLayout(self.horizontalLayout_8)

        self.retranslateUi(Form)
        self.tabWidget.setCurrentIndex(0)
        self.tabWidget_2.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Model", None, QtGui.QApplication.UnicodeUTF8))
        self.KmlGeneralgroupBox.setTitle(QtGui.QApplication.translate("Form", "General", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Form", "Kml - Name", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("Form", "Model - Name", None, QtGui.QApplication.UnicodeUTF8))
        self.Iconlabel.setText(QtGui.QApplication.translate("Form", "Model - Path", None, QtGui.QApplication.UnicodeUTF8))
        self.LineWidthlabel.setText(QtGui.QApplication.translate("Form", "Altitude - Mode", None, QtGui.QApplication.UnicodeUTF8))
        self.altitudemode.setItemText(0, QtGui.QApplication.translate("Form", "absolute", None, QtGui.QApplication.UnicodeUTF8))
        self.altitudemode.setItemText(1, QtGui.QApplication.translate("Form", "clampToGround", None, QtGui.QApplication.UnicodeUTF8))
        self.altitudemode.setItemText(2, QtGui.QApplication.translate("Form", "relativeToGround", None, QtGui.QApplication.UnicodeUTF8))
        self.KmlColorgroupBox.setTitle(QtGui.QApplication.translate("Form", "Look-At", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("Form", "Heading", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("Form", "Tilt", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("Form", "Latitude", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("Form", "Longitude", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setText(QtGui.QApplication.translate("Form", "Range", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setText(QtGui.QApplication.translate("Form", "Altitude", None, QtGui.QApplication.UnicodeUTF8))
        self.setcamera.setText(QtGui.QApplication.translate("Form", "Refresh", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("Form", "Camera", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("Form", "Heading", None, QtGui.QApplication.UnicodeUTF8))
        self.label_10.setText(QtGui.QApplication.translate("Form", "Tilt", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setText(QtGui.QApplication.translate("Form", "Latitude", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setText(QtGui.QApplication.translate("Form", "Longitude", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setText(QtGui.QApplication.translate("Form", "Range", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setText(QtGui.QApplication.translate("Form", "Altitude", None, QtGui.QApplication.UnicodeUTF8))
        self.setmodel.setText(QtGui.QApplication.translate("Form", "set", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QtGui.QApplication.translate("Form", "Model", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("Form", "Model-Position", None, QtGui.QApplication.UnicodeUTF8))
        self.label_15.setText(QtGui.QApplication.translate("Form", "Longitude", None, QtGui.QApplication.UnicodeUTF8))
        self.label_16.setText(QtGui.QApplication.translate("Form", "Latitude", None, QtGui.QApplication.UnicodeUTF8))
        self.label_18.setText(QtGui.QApplication.translate("Form", "Altitude", None, QtGui.QApplication.UnicodeUTF8))
        self.setlonlat.setText(QtGui.QApplication.translate("Form", "Refresh", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_3), QtGui.QApplication.translate("Form", "Location", None, QtGui.QApplication.UnicodeUTF8))
        self.label_28.setText(QtGui.QApplication.translate("Form", "Heading", None, QtGui.QApplication.UnicodeUTF8))
        self.label_29.setText(QtGui.QApplication.translate("Form", "Tilt", None, QtGui.QApplication.UnicodeUTF8))
        self.label_30.setText(QtGui.QApplication.translate("Form", "Roll", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_4), QtGui.QApplication.translate("Form", "Orientation", None, QtGui.QApplication.UnicodeUTF8))
        self.label_36.setText(QtGui.QApplication.translate("Form", "X", None, QtGui.QApplication.UnicodeUTF8))
        self.label_37.setText(QtGui.QApplication.translate("Form", "Y", None, QtGui.QApplication.UnicodeUTF8))
        self.label_38.setText(QtGui.QApplication.translate("Form", "Z", None, QtGui.QApplication.UnicodeUTF8))
        self.Xscale.setText(QtGui.QApplication.translate("Form", "1.0", None, QtGui.QApplication.UnicodeUTF8))
        self.Yscale.setText(QtGui.QApplication.translate("Form", "1.0", None, QtGui.QApplication.UnicodeUTF8))
        self.Zscale.setText(QtGui.QApplication.translate("Form", "1.0", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_5), QtGui.QApplication.translate("Form", "Scale", None, QtGui.QApplication.UnicodeUTF8))
        self.save.setText(QtGui.QApplication.translate("Form", "Save", None, QtGui.QApplication.UnicodeUTF8))
        self.opendir.setText(QtGui.QApplication.translate("Form", "Open Dir", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
