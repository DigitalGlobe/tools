# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/Users/Shared/ossim/trunk/gsoc/PlanetSasha/gui/data_ui.ui'
#
# Created: Mon Feb 14 21:56:50 2011
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
        Form.resize(429, 283)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/db.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Form.setWindowIcon(icon)
        self.verticalLayout = QtGui.QVBoxLayout(Form)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.tabWidget = QtGui.QTabWidget(Form)
        self.tabWidget.setObjectName(_fromUtf8("tabWidget"))
        self.tab_2 = QtGui.QWidget()
        self.tab_2.setObjectName(_fromUtf8("tab_2"))
        self.verticalLayout_16 = QtGui.QVBoxLayout(self.tab_2)
        self.verticalLayout_16.setObjectName(_fromUtf8("verticalLayout_16"))
        self.ElevationGroupBox = QtGui.QGroupBox(self.tab_2)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Maximum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ElevationGroupBox.sizePolicy().hasHeightForWidth())
        self.ElevationGroupBox.setSizePolicy(sizePolicy)
        self.ElevationGroupBox.setTitle(_fromUtf8(""))
        self.ElevationGroupBox.setObjectName(_fromUtf8("ElevationGroupBox"))
        self.gridLayout_30 = QtGui.QGridLayout(self.ElevationGroupBox)
        self.gridLayout_30.setObjectName(_fromUtf8("gridLayout_30"))
        self.InputElevlabel = QtGui.QLabel(self.ElevationGroupBox)
        self.InputElevlabel.setObjectName(_fromUtf8("InputElevlabel"))
        self.gridLayout_30.addWidget(self.InputElevlabel, 1, 2, 1, 1)
        self.Tilinglabel = QtGui.QLabel(self.ElevationGroupBox)
        self.Tilinglabel.setObjectName(_fromUtf8("Tilinglabel"))
        self.gridLayout_30.addWidget(self.Tilinglabel, 2, 2, 1, 1)
        self.horizontalLayout_12 = QtGui.QHBoxLayout()
        self.horizontalLayout_12.setObjectName(_fromUtf8("horizontalLayout_12"))
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_12.addItem(spacerItem)
        self.InputRaster = QtGui.QComboBox(self.ElevationGroupBox)
        self.InputRaster.setObjectName(_fromUtf8("InputRaster"))
        self.InputRaster.addItem(_fromUtf8(""))
        self.InputRaster.setItemText(0, _fromUtf8(""))
        self.horizontalLayout_12.addWidget(self.InputRaster)
        self.gridLayout_30.addLayout(self.horizontalLayout_12, 1, 3, 1, 1)
        self.horizontalLayout_13 = QtGui.QHBoxLayout()
        self.horizontalLayout_13.setObjectName(_fromUtf8("horizontalLayout_13"))
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_13.addItem(spacerItem1)
        self.Tile = QtGui.QLineEdit(self.ElevationGroupBox)
        self.Tile.setObjectName(_fromUtf8("Tile"))
        self.horizontalLayout_13.addWidget(self.Tile)
        self.gridLayout_30.addLayout(self.horizontalLayout_13, 2, 3, 1, 1)
        self.horizontalLayout_7 = QtGui.QHBoxLayout()
        self.horizontalLayout_7.setObjectName(_fromUtf8("horizontalLayout_7"))
        self.Autotiling = QtGui.QRadioButton(self.ElevationGroupBox)
        self.Autotiling.setEnabled(False)
        self.Autotiling.setObjectName(_fromUtf8("Autotiling"))
        self.horizontalLayout_7.addWidget(self.Autotiling)
        self.ManualTiling = QtGui.QRadioButton(self.ElevationGroupBox)
        self.ManualTiling.setChecked(True)
        self.ManualTiling.setObjectName(_fromUtf8("ManualTiling"))
        self.horizontalLayout_7.addWidget(self.ManualTiling)
        self.gridLayout_30.addLayout(self.horizontalLayout_7, 0, 3, 1, 1)
        self.CopyElev = QtGui.QCheckBox(self.ElevationGroupBox)
        self.CopyElev.setEnabled(False)
        self.CopyElev.setObjectName(_fromUtf8("CopyElev"))
        self.gridLayout_30.addWidget(self.CopyElev, 0, 2, 1, 1)
        self.verticalLayout_16.addWidget(self.ElevationGroupBox)
        spacerItem2 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_16.addItem(spacerItem2)
        self.groupBox_5 = QtGui.QGroupBox(self.tab_2)
        self.groupBox_5.setTitle(_fromUtf8(""))
        self.groupBox_5.setObjectName(_fromUtf8("groupBox_5"))
        self.verticalLayout_5 = QtGui.QVBoxLayout(self.groupBox_5)
        self.verticalLayout_5.setObjectName(_fromUtf8("verticalLayout_5"))
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.ExportElev = QtGui.QPushButton(self.groupBox_5)
        self.ExportElev.setObjectName(_fromUtf8("ExportElev"))
        self.horizontalLayout_5.addWidget(self.ExportElev)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_5.addItem(spacerItem3)
        self.elevdir = QtGui.QPushButton(self.groupBox_5)
        self.elevdir.setEnabled(True)
        self.elevdir.setObjectName(_fromUtf8("elevdir"))
        self.horizontalLayout_5.addWidget(self.elevdir)
        self.verticalLayout_5.addLayout(self.horizontalLayout_5)
        self.verticalLayout_16.addWidget(self.groupBox_5)
        self.tabWidget.addTab(self.tab_2, _fromUtf8(""))
        self.tab_7 = QtGui.QWidget()
        self.tab_7.setObjectName(_fromUtf8("tab_7"))
        self.verticalLayout_4 = QtGui.QVBoxLayout(self.tab_7)
        self.verticalLayout_4.setObjectName(_fromUtf8("verticalLayout_4"))
        self.scrollArea_2 = QtGui.QScrollArea(self.tab_7)
        self.scrollArea_2.setWidgetResizable(True)
        self.scrollArea_2.setObjectName(_fromUtf8("scrollArea_2"))
        self.scrollAreaWidgetContents_2 = QtGui.QWidget()
        self.scrollAreaWidgetContents_2.setGeometry(QtCore.QRect(0, 0, 342, 626))
        self.scrollAreaWidgetContents_2.setObjectName(_fromUtf8("scrollAreaWidgetContents_2"))
        self.verticalLayout_3 = QtGui.QVBoxLayout(self.scrollAreaWidgetContents_2)
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.tabWidget_3 = QtGui.QTabWidget(self.scrollAreaWidgetContents_2)
        self.tabWidget_3.setObjectName(_fromUtf8("tabWidget_3"))
        self.tab_6 = QtGui.QWidget()
        self.tab_6.setObjectName(_fromUtf8("tab_6"))
        self.verticalLayout_8 = QtGui.QVBoxLayout(self.tab_6)
        self.verticalLayout_8.setObjectName(_fromUtf8("verticalLayout_8"))
        self.horizontalLayout_6 = QtGui.QHBoxLayout()
        self.horizontalLayout_6.setObjectName(_fromUtf8("horizontalLayout_6"))
        self.data = QtGui.QTableWidget(self.tab_6)
        self.data.setObjectName(_fromUtf8("data"))
        self.data.setColumnCount(1)
        self.data.setRowCount(0)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/element-cell.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        item = QtGui.QTableWidgetItem()
        self.data.setHorizontalHeaderItem(0, item)
        item.setIcon(icon1)
        self.horizontalLayout_6.addWidget(self.data)
        self.verticalLayout_8.addLayout(self.horizontalLayout_6)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.label_3 = QtGui.QLabel(self.tab_6)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.horizontalLayout_2.addWidget(self.label_3)
        spacerItem4 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem4)
        self.legendname = QtGui.QLineEdit(self.tab_6)
        self.legendname.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.legendname.setObjectName(_fromUtf8("legendname"))
        self.horizontalLayout_2.addWidget(self.legendname)
        self.verticalLayout_8.addLayout(self.horizontalLayout_2)
        self.verticalLayout_12 = QtGui.QVBoxLayout()
        self.verticalLayout_12.setObjectName(_fromUtf8("verticalLayout_12"))
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.label = QtGui.QLabel(self.tab_6)
        self.label.setObjectName(_fromUtf8("label"))
        self.horizontalLayout_4.addWidget(self.label)
        spacerItem5 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem5)
        self.textlegendcolor = QtGui.QComboBox(self.tab_6)
        self.textlegendcolor.setObjectName(_fromUtf8("textlegendcolor"))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.setItemText(0, _fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.textlegendcolor.addItem(_fromUtf8(""))
        self.horizontalLayout_4.addWidget(self.textlegendcolor)
        self.verticalLayout_12.addLayout(self.horizontalLayout_4)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.label_4 = QtGui.QLabel(self.tab_6)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.horizontalLayout_3.addWidget(self.label_4)
        spacerItem6 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem6)
        self.TrueColor = QtGui.QComboBox(self.tab_6)
        self.TrueColor.setObjectName(_fromUtf8("TrueColor"))
        self.TrueColor.addItem(_fromUtf8(""))
        self.TrueColor.setItemText(0, _fromUtf8(""))
        self.TrueColor.addItem(_fromUtf8(""))
        self.TrueColor.addItem(_fromUtf8(""))
        self.horizontalLayout_3.addWidget(self.TrueColor)
        self.verticalLayout_12.addLayout(self.horizontalLayout_3)
        self.horizontalLayout_14 = QtGui.QHBoxLayout()
        self.horizontalLayout_14.setObjectName(_fromUtf8("horizontalLayout_14"))
        self.label_5 = QtGui.QLabel(self.tab_6)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.horizontalLayout_14.addWidget(self.label_5)
        spacerItem7 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_14.addItem(spacerItem7)
        self.AlphaBkg = QtGui.QComboBox(self.tab_6)
        self.AlphaBkg.setObjectName(_fromUtf8("AlphaBkg"))
        self.AlphaBkg.addItem(_fromUtf8(""))
        self.AlphaBkg.setItemText(0, _fromUtf8(""))
        self.AlphaBkg.addItem(_fromUtf8(""))
        self.AlphaBkg.addItem(_fromUtf8(""))
        self.horizontalLayout_14.addWidget(self.AlphaBkg)
        self.verticalLayout_12.addLayout(self.horizontalLayout_14)
        self.horizontalLayout_15 = QtGui.QHBoxLayout()
        self.horizontalLayout_15.setObjectName(_fromUtf8("horizontalLayout_15"))
        self.label_6 = QtGui.QLabel(self.tab_6)
        self.label_6.setEnabled(True)
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.horizontalLayout_15.addWidget(self.label_6)
        spacerItem8 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_15.addItem(spacerItem8)
        self.PngRead = QtGui.QComboBox(self.tab_6)
        self.PngRead.setEnabled(True)
        self.PngRead.setObjectName(_fromUtf8("PngRead"))
        self.PngRead.addItem(_fromUtf8(""))
        self.PngRead.setItemText(0, _fromUtf8(""))
        self.PngRead.addItem(_fromUtf8(""))
        self.PngRead.addItem(_fromUtf8(""))
        self.horizontalLayout_15.addWidget(self.PngRead)
        self.verticalLayout_12.addLayout(self.horizontalLayout_15)
        self.verticalLayout_8.addLayout(self.verticalLayout_12)
        self.horizontalLayout_16 = QtGui.QHBoxLayout()
        self.horizontalLayout_16.setObjectName(_fromUtf8("horizontalLayout_16"))
        self.label_7 = QtGui.QLabel(self.tab_6)
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.horizontalLayout_16.addWidget(self.label_7)
        self.kmlbkgcolor = QtGui.QToolButton(self.tab_6)
        self.kmlbkgcolor.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.kmlbkgcolor.setText(_fromUtf8(""))
        self.kmlbkgcolor.setObjectName(_fromUtf8("kmlbkgcolor"))
        self.horizontalLayout_16.addWidget(self.kmlbkgcolor)
        self.bkgcolor = QtGui.QLineEdit(self.tab_6)
        self.bkgcolor.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.bkgcolor.setReadOnly(True)
        self.bkgcolor.setObjectName(_fromUtf8("bkgcolor"))
        self.horizontalLayout_16.addWidget(self.bkgcolor)
        self.verticalLayout_8.addLayout(self.horizontalLayout_16)
        self.horizontalLayout_17 = QtGui.QHBoxLayout()
        self.horizontalLayout_17.setObjectName(_fromUtf8("horizontalLayout_17"))
        self.label_8 = QtGui.QLabel(self.tab_6)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.horizontalLayout_17.addWidget(self.label_8)
        self.PngKomp = QtGui.QComboBox(self.tab_6)
        self.PngKomp.setObjectName(_fromUtf8("PngKomp"))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.setItemText(0, _fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.PngKomp.addItem(_fromUtf8(""))
        self.horizontalLayout_17.addWidget(self.PngKomp)
        self.verticalLayout_8.addLayout(self.horizontalLayout_17)
        self.horizontalLayout_18 = QtGui.QHBoxLayout()
        self.horizontalLayout_18.setObjectName(_fromUtf8("horizontalLayout_18"))
        self.label_10 = QtGui.QLabel(self.tab_6)
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.horizontalLayout_18.addWidget(self.label_10)
        self.height = QtGui.QLineEdit(self.tab_6)
        self.height.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.height.setObjectName(_fromUtf8("height"))
        self.horizontalLayout_18.addWidget(self.height)
        self.label_9 = QtGui.QLabel(self.tab_6)
        self.label_9.setObjectName(_fromUtf8("label_9"))
        self.horizontalLayout_18.addWidget(self.label_9)
        self.width = QtGui.QLineEdit(self.tab_6)
        self.width.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.width.setObjectName(_fromUtf8("width"))
        self.horizontalLayout_18.addWidget(self.width)
        self.verticalLayout_8.addLayout(self.horizontalLayout_18)
        self.horizontalLayout_22 = QtGui.QHBoxLayout()
        self.horizontalLayout_22.setObjectName(_fromUtf8("horizontalLayout_22"))
        self.label_12 = QtGui.QLabel(self.tab_6)
        self.label_12.setObjectName(_fromUtf8("label_12"))
        self.horizontalLayout_22.addWidget(self.label_12)
        spacerItem9 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_22.addItem(spacerItem9)
        self.lineEdit_5 = QtGui.QLineEdit(self.tab_6)
        self.lineEdit_5.setEnabled(False)
        self.lineEdit_5.setMinimumSize(QtCore.QSize(125, 0))
        self.lineEdit_5.setMaximumSize(QtCore.QSize(125, 16777215))
        self.lineEdit_5.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.lineEdit_5.setObjectName(_fromUtf8("lineEdit_5"))
        self.horizontalLayout_22.addWidget(self.lineEdit_5)
        self.verticalLayout_8.addLayout(self.horizontalLayout_22)
        self.tabWidget_3.addTab(self.tab_6, _fromUtf8(""))
        self.tab_8 = QtGui.QWidget()
        self.tab_8.setObjectName(_fromUtf8("tab_8"))
        self.verticalLayout_11 = QtGui.QVBoxLayout(self.tab_8)
        self.verticalLayout_11.setObjectName(_fromUtf8("verticalLayout_11"))
        self.groupBox = QtGui.QGroupBox(self.tab_8)
        self.groupBox.setCheckable(True)
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.verticalLayout_13 = QtGui.QVBoxLayout(self.groupBox)
        self.verticalLayout_13.setObjectName(_fromUtf8("verticalLayout_13"))
        self.horizontalLayout_20 = QtGui.QHBoxLayout()
        self.horizontalLayout_20.setObjectName(_fromUtf8("horizontalLayout_20"))
        self.label_2 = QtGui.QLabel(self.groupBox)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.horizontalLayout_20.addWidget(self.label_2)
        spacerItem10 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_20.addItem(spacerItem10)
        self.AdvLines = QtGui.QLineEdit(self.groupBox)
        self.AdvLines.setMinimumSize(QtCore.QSize(125, 0))
        self.AdvLines.setMaximumSize(QtCore.QSize(125, 16777215))
        self.AdvLines.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.AdvLines.setAlignment(QtCore.Qt.AlignCenter)
        self.AdvLines.setObjectName(_fromUtf8("AdvLines"))
        self.horizontalLayout_20.addWidget(self.AdvLines)
        self.verticalLayout_13.addLayout(self.horizontalLayout_20)
        self.horizontalLayout_23 = QtGui.QHBoxLayout()
        self.horizontalLayout_23.setObjectName(_fromUtf8("horizontalLayout_23"))
        self.label_13 = QtGui.QLabel(self.groupBox)
        self.label_13.setObjectName(_fromUtf8("label_13"))
        self.horizontalLayout_23.addWidget(self.label_13)
        spacerItem11 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_23.addItem(spacerItem11)
        self.AdvThin = QtGui.QLineEdit(self.groupBox)
        self.AdvThin.setMinimumSize(QtCore.QSize(125, 0))
        self.AdvThin.setMaximumSize(QtCore.QSize(125, 16777215))
        self.AdvThin.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.AdvThin.setAlignment(QtCore.Qt.AlignCenter)
        self.AdvThin.setObjectName(_fromUtf8("AdvThin"))
        self.horizontalLayout_23.addWidget(self.AdvThin)
        self.verticalLayout_13.addLayout(self.horizontalLayout_23)
        self.horizontalLayout_21 = QtGui.QHBoxLayout()
        self.horizontalLayout_21.setObjectName(_fromUtf8("horizontalLayout_21"))
        self.label_11 = QtGui.QLabel(self.groupBox)
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.horizontalLayout_21.addWidget(self.label_11)
        spacerItem12 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_21.addItem(spacerItem12)
        self.AdvLabelNum = QtGui.QLineEdit(self.groupBox)
        self.AdvLabelNum.setMinimumSize(QtCore.QSize(125, 0))
        self.AdvLabelNum.setMaximumSize(QtCore.QSize(125, 16777215))
        self.AdvLabelNum.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.AdvLabelNum.setAlignment(QtCore.Qt.AlignCenter)
        self.AdvLabelNum.setObjectName(_fromUtf8("AdvLabelNum"))
        self.horizontalLayout_21.addWidget(self.AdvLabelNum)
        self.verticalLayout_13.addLayout(self.horizontalLayout_21)
        self.horizontalLayout_24 = QtGui.QHBoxLayout()
        self.horizontalLayout_24.setObjectName(_fromUtf8("horizontalLayout_24"))
        self.label_14 = QtGui.QLabel(self.groupBox)
        self.label_14.setObjectName(_fromUtf8("label_14"))
        self.horizontalLayout_24.addWidget(self.label_14)
        spacerItem13 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_24.addItem(spacerItem13)
        self.AdvUse = QtGui.QLineEdit(self.groupBox)
        self.AdvUse.setEnabled(False)
        self.AdvUse.setMinimumSize(QtCore.QSize(125, 0))
        self.AdvUse.setMaximumSize(QtCore.QSize(125, 16777215))
        self.AdvUse.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.AdvUse.setAlignment(QtCore.Qt.AlignCenter)
        self.AdvUse.setObjectName(_fromUtf8("AdvUse"))
        self.horizontalLayout_24.addWidget(self.AdvUse)
        self.verticalLayout_13.addLayout(self.horizontalLayout_24)
        self.horizontalLayout_25 = QtGui.QHBoxLayout()
        self.horizontalLayout_25.setObjectName(_fromUtf8("horizontalLayout_25"))
        self.label_15 = QtGui.QLabel(self.groupBox)
        self.label_15.setObjectName(_fromUtf8("label_15"))
        self.horizontalLayout_25.addWidget(self.label_15)
        spacerItem14 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_25.addItem(spacerItem14)
        self.AdvRange = QtGui.QLineEdit(self.groupBox)
        self.AdvRange.setEnabled(False)
        self.AdvRange.setMinimumSize(QtCore.QSize(125, 0))
        self.AdvRange.setMaximumSize(QtCore.QSize(125, 16777215))
        self.AdvRange.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.AdvRange.setAlignment(QtCore.Qt.AlignCenter)
        self.AdvRange.setObjectName(_fromUtf8("AdvRange"))
        self.horizontalLayout_25.addWidget(self.AdvRange)
        self.verticalLayout_13.addLayout(self.horizontalLayout_25)
        self.verticalLayout_10 = QtGui.QVBoxLayout()
        self.verticalLayout_10.setObjectName(_fromUtf8("verticalLayout_10"))
        self.flag_v = QtGui.QCheckBox(self.groupBox)
        self.flag_v.setObjectName(_fromUtf8("flag_v"))
        self.verticalLayout_10.addWidget(self.flag_v)
        self.flag_c = QtGui.QCheckBox(self.groupBox)
        self.flag_c.setObjectName(_fromUtf8("flag_c"))
        self.verticalLayout_10.addWidget(self.flag_c)
        self.flag_n = QtGui.QCheckBox(self.groupBox)
        self.flag_n.setObjectName(_fromUtf8("flag_n"))
        self.verticalLayout_10.addWidget(self.flag_n)
        self.flag_s = QtGui.QCheckBox(self.groupBox)
        self.flag_s.setObjectName(_fromUtf8("flag_s"))
        self.verticalLayout_10.addWidget(self.flag_s)
        self.flag_f = QtGui.QCheckBox(self.groupBox)
        self.flag_f.setObjectName(_fromUtf8("flag_f"))
        self.verticalLayout_10.addWidget(self.flag_f)
        self.verticalLayout_13.addLayout(self.verticalLayout_10)
        self.verticalLayout_11.addWidget(self.groupBox)
        self.groupBox_2 = QtGui.QGroupBox(self.tab_8)
        self.groupBox_2.setEnabled(True)
        self.groupBox_2.setTitle(_fromUtf8(""))
        self.groupBox_2.setCheckable(False)
        self.groupBox_2.setObjectName(_fromUtf8("groupBox_2"))
        self.verticalLayout_14 = QtGui.QVBoxLayout(self.groupBox_2)
        self.verticalLayout_14.setObjectName(_fromUtf8("verticalLayout_14"))
        self.horizontalLayout_26 = QtGui.QHBoxLayout()
        self.horizontalLayout_26.setObjectName(_fromUtf8("horizontalLayout_26"))
        self.placemark = QtGui.QCheckBox(self.groupBox_2)
        self.placemark.setObjectName(_fromUtf8("placemark"))
        self.horizontalLayout_26.addWidget(self.placemark)
        self.placemarks = QtGui.QComboBox(self.groupBox_2)
        self.placemarks.setObjectName(_fromUtf8("placemarks"))
        self.placemarks.addItem(_fromUtf8(""))
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/yellow_circle.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.placemarks.addItem(icon2, _fromUtf8(""))
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/red_circle.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.placemarks.addItem(icon3, _fromUtf8(""))
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/blue_circle.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.placemarks.addItem(icon4, _fromUtf8(""))
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/green_circle.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.placemarks.addItem(icon5, _fromUtf8(""))
        self.horizontalLayout_26.addWidget(self.placemarks)
        self.verticalLayout_14.addLayout(self.horizontalLayout_26)
        self.horizontalLayout_27 = QtGui.QHBoxLayout()
        self.horizontalLayout_27.setObjectName(_fromUtf8("horizontalLayout_27"))
        self.addlegend = QtGui.QCheckBox(self.groupBox_2)
        self.addlegend.setChecked(True)
        self.addlegend.setObjectName(_fromUtf8("addlegend"))
        self.horizontalLayout_27.addWidget(self.addlegend)
        self.adddescription = QtGui.QCheckBox(self.groupBox_2)
        self.adddescription.setChecked(True)
        self.adddescription.setObjectName(_fromUtf8("adddescription"))
        self.horizontalLayout_27.addWidget(self.adddescription)
        self.flag_z = QtGui.QCheckBox(self.groupBox_2)
        self.flag_z.setChecked(True)
        self.flag_z.setObjectName(_fromUtf8("flag_z"))
        self.horizontalLayout_27.addWidget(self.flag_z)
        self.verticalLayout_14.addLayout(self.horizontalLayout_27)
        self.horizontalLayout_28 = QtGui.QHBoxLayout()
        self.horizontalLayout_28.setObjectName(_fromUtf8("horizontalLayout_28"))
        self.longitude = QtGui.QLineEdit(self.groupBox_2)
        self.longitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.longitude.setAlignment(QtCore.Qt.AlignCenter)
        self.longitude.setObjectName(_fromUtf8("longitude"))
        self.horizontalLayout_28.addWidget(self.longitude)
        self.latitude = QtGui.QLineEdit(self.groupBox_2)
        self.latitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.latitude.setAlignment(QtCore.Qt.AlignCenter)
        self.latitude.setObjectName(_fromUtf8("latitude"))
        self.horizontalLayout_28.addWidget(self.latitude)
        self.refresh = QtGui.QToolButton(self.groupBox_2)
        self.refresh.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.refresh.setText(_fromUtf8(""))
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/mActionDraw.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.refresh.setIcon(icon6)
        self.refresh.setObjectName(_fromUtf8("refresh"))
        self.horizontalLayout_28.addWidget(self.refresh)
        self.verticalLayout_14.addLayout(self.horizontalLayout_28)
        self.placedescription = QtGui.QLineEdit(self.groupBox_2)
        self.placedescription.setEnabled(True)
        self.placedescription.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.placedescription.setObjectName(_fromUtf8("placedescription"))
        self.verticalLayout_14.addWidget(self.placedescription)
        self.verticalLayout_11.addWidget(self.groupBox_2)
        self.tabWidget_3.addTab(self.tab_8, _fromUtf8(""))
        self.verticalLayout_3.addWidget(self.tabWidget_3)
        self.groupBox_3 = QtGui.QGroupBox(self.scrollAreaWidgetContents_2)
        self.groupBox_3.setTitle(_fromUtf8(""))
        self.groupBox_3.setObjectName(_fromUtf8("groupBox_3"))
        self.verticalLayout_6 = QtGui.QVBoxLayout(self.groupBox_3)
        self.verticalLayout_6.setObjectName(_fromUtf8("verticalLayout_6"))
        self.horizontalLayout_19 = QtGui.QHBoxLayout()
        self.horizontalLayout_19.setObjectName(_fromUtf8("horizontalLayout_19"))
        self.exportoverlay = QtGui.QPushButton(self.groupBox_3)
        self.exportoverlay.setObjectName(_fromUtf8("exportoverlay"))
        self.horizontalLayout_19.addWidget(self.exportoverlay)
        spacerItem15 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_19.addItem(spacerItem15)
        self.overlaydir = QtGui.QPushButton(self.groupBox_3)
        self.overlaydir.setEnabled(True)
        self.overlaydir.setObjectName(_fromUtf8("overlaydir"))
        self.horizontalLayout_19.addWidget(self.overlaydir)
        self.verticalLayout_6.addLayout(self.horizontalLayout_19)
        self.verticalLayout_3.addWidget(self.groupBox_3)
        self.scrollArea_2.setWidget(self.scrollAreaWidgetContents_2)
        self.verticalLayout_4.addWidget(self.scrollArea_2)
        self.tabWidget.addTab(self.tab_7, _fromUtf8(""))
        self.tab = QtGui.QWidget()
        self.tab.setObjectName(_fromUtf8("tab"))
        self.verticalLayout_15 = QtGui.QVBoxLayout(self.tab)
        self.verticalLayout_15.setObjectName(_fromUtf8("verticalLayout_15"))
        self.VectorGroupBox = QtGui.QGroupBox(self.tab)
        self.VectorGroupBox.setTitle(_fromUtf8(""))
        self.VectorGroupBox.setObjectName(_fromUtf8("VectorGroupBox"))
        self.verticalLayout_9 = QtGui.QVBoxLayout(self.VectorGroupBox)
        self.verticalLayout_9.setObjectName(_fromUtf8("verticalLayout_9"))
        self.InputVectorhorizontalLayout = QtGui.QHBoxLayout()
        self.InputVectorhorizontalLayout.setObjectName(_fromUtf8("InputVectorhorizontalLayout"))
        self.Point = QtGui.QRadioButton(self.VectorGroupBox)
        self.Point.setObjectName(_fromUtf8("Point"))
        self.InputVectorhorizontalLayout.addWidget(self.Point)
        self.Line = QtGui.QRadioButton(self.VectorGroupBox)
        self.Line.setObjectName(_fromUtf8("Line"))
        self.InputVectorhorizontalLayout.addWidget(self.Line)
        self.Polygon = QtGui.QRadioButton(self.VectorGroupBox)
        self.Polygon.setObjectName(_fromUtf8("Polygon"))
        self.InputVectorhorizontalLayout.addWidget(self.Polygon)
        self.verticalLayout_9.addLayout(self.InputVectorhorizontalLayout)
        self.FeatureTypehorizontalLayout = QtGui.QHBoxLayout()
        self.FeatureTypehorizontalLayout.setObjectName(_fromUtf8("FeatureTypehorizontalLayout"))
        self.InputVectorlabel = QtGui.QLabel(self.VectorGroupBox)
        self.InputVectorlabel.setObjectName(_fromUtf8("InputVectorlabel"))
        self.FeatureTypehorizontalLayout.addWidget(self.InputVectorlabel)
        self.InputVector = QtGui.QComboBox(self.VectorGroupBox)
        self.InputVector.setObjectName(_fromUtf8("InputVector"))
        self.InputVector.addItem(_fromUtf8(""))
        self.InputVector.setItemText(0, _fromUtf8(""))
        self.FeatureTypehorizontalLayout.addWidget(self.InputVector)
        self.verticalLayout_9.addLayout(self.FeatureTypehorizontalLayout)
        self.verticalLayout_15.addWidget(self.VectorGroupBox)
        self.scrollArea = QtGui.QScrollArea(self.tab)
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName(_fromUtf8("scrollArea"))
        self.scrollAreaWidgetContents = QtGui.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 342, 643))
        self.scrollAreaWidgetContents.setObjectName(_fromUtf8("scrollAreaWidgetContents"))
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.scrollAreaWidgetContents)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.KmlGeneralgroupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.KmlGeneralgroupBox.setObjectName(_fromUtf8("KmlGeneralgroupBox"))
        self.gridLayout_27 = QtGui.QGridLayout(self.KmlGeneralgroupBox)
        self.gridLayout_27.setObjectName(_fromUtf8("gridLayout_27"))
        self.Iconlabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.Iconlabel.setObjectName(_fromUtf8("Iconlabel"))
        self.gridLayout_27.addWidget(self.Iconlabel, 1, 0, 1, 1)
        self.LabelFieldlabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.LabelFieldlabel.setObjectName(_fromUtf8("LabelFieldlabel"))
        self.gridLayout_27.addWidget(self.LabelFieldlabel, 2, 0, 1, 1)
        self.LineWidthlabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.LineWidthlabel.setObjectName(_fromUtf8("LineWidthlabel"))
        self.gridLayout_27.addWidget(self.LineWidthlabel, 3, 0, 1, 1)
        self.SelectIcon = QtGui.QComboBox(self.KmlGeneralgroupBox)
        self.SelectIcon.setObjectName(_fromUtf8("SelectIcon"))
        self.SelectIcon.addItem(_fromUtf8(""))
        self.SelectIcon.setItemText(0, _fromUtf8(""))
        self.SelectIcon.addItem(icon4, _fromUtf8(""))
        self.SelectIcon.addItem(icon5, _fromUtf8(""))
        self.SelectIcon.addItem(icon3, _fromUtf8(""))
        self.SelectIcon.addItem(icon2, _fromUtf8(""))
        self.gridLayout_27.addWidget(self.SelectIcon, 1, 1, 1, 1)
        self.AttributeList = QtGui.QComboBox(self.KmlGeneralgroupBox)
        self.AttributeList.setObjectName(_fromUtf8("AttributeList"))
        self.gridLayout_27.addWidget(self.AttributeList, 2, 1, 1, 1)
        self.LineWidth = QtGui.QSpinBox(self.KmlGeneralgroupBox)
        self.LineWidth.setProperty(_fromUtf8("value"), 1)
        self.LineWidth.setObjectName(_fromUtf8("LineWidth"))
        self.gridLayout_27.addWidget(self.LineWidth, 3, 1, 1, 1)
        self.Tessellate = QtGui.QCheckBox(self.KmlGeneralgroupBox)
        self.Tessellate.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Tessellate.setText(_fromUtf8(""))
        self.Tessellate.setObjectName(_fromUtf8("Tessellate"))
        self.gridLayout_27.addWidget(self.Tessellate, 4, 1, 1, 1)
        self.Tessellatelabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.Tessellatelabel.setObjectName(_fromUtf8("Tessellatelabel"))
        self.gridLayout_27.addWidget(self.Tessellatelabel, 4, 0, 1, 1)
        self.Extrude = QtGui.QCheckBox(self.KmlGeneralgroupBox)
        self.Extrude.setText(_fromUtf8(""))
        self.Extrude.setChecked(True)
        self.Extrude.setObjectName(_fromUtf8("Extrude"))
        self.gridLayout_27.addWidget(self.Extrude, 5, 1, 1, 1)
        self.Extrudelabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.Extrudelabel.setObjectName(_fromUtf8("Extrudelabel"))
        self.gridLayout_27.addWidget(self.Extrudelabel, 5, 0, 1, 1)
        self.verticalLayout_2.addWidget(self.KmlGeneralgroupBox)
        self.KmlElevationgroupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.KmlElevationgroupBox.setObjectName(_fromUtf8("KmlElevationgroupBox"))
        self.gridLayout_28 = QtGui.QGridLayout(self.KmlElevationgroupBox)
        self.gridLayout_28.setObjectName(_fromUtf8("gridLayout_28"))
        self.horizontalLayout_8 = QtGui.QHBoxLayout()
        self.horizontalLayout_8.setObjectName(_fromUtf8("horizontalLayout_8"))
        self.Offsetlabel = QtGui.QLabel(self.KmlElevationgroupBox)
        self.Offsetlabel.setObjectName(_fromUtf8("Offsetlabel"))
        self.horizontalLayout_8.addWidget(self.Offsetlabel)
        self.Offset = QtGui.QDoubleSpinBox(self.KmlElevationgroupBox)
        self.Offset.setMaximum(20000000.0)
        self.Offset.setProperty(_fromUtf8("value"), 1000.0)
        self.Offset.setObjectName(_fromUtf8("Offset"))
        self.horizontalLayout_8.addWidget(self.Offset)
        self.gridLayout_28.addLayout(self.horizontalLayout_8, 7, 0, 1, 1)
        self.horizontalLayout_9 = QtGui.QHBoxLayout()
        self.horizontalLayout_9.setObjectName(_fromUtf8("horizontalLayout_9"))
        self.HeightFieldlabel = QtGui.QLabel(self.KmlElevationgroupBox)
        self.HeightFieldlabel.setObjectName(_fromUtf8("HeightFieldlabel"))
        self.horizontalLayout_9.addWidget(self.HeightFieldlabel)
        self.HeightColumn = QtGui.QComboBox(self.KmlElevationgroupBox)
        self.HeightColumn.setObjectName(_fromUtf8("HeightColumn"))
        self.horizontalLayout_9.addWidget(self.HeightColumn)
        self.gridLayout_28.addLayout(self.horizontalLayout_9, 5, 0, 1, 1)
        self.horizontalLayout_10 = QtGui.QHBoxLayout()
        self.horizontalLayout_10.setObjectName(_fromUtf8("horizontalLayout_10"))
        self.Heightlabel = QtGui.QLabel(self.KmlElevationgroupBox)
        self.Heightlabel.setObjectName(_fromUtf8("Heightlabel"))
        self.horizontalLayout_10.addWidget(self.Heightlabel)
        self.ExtrudeType = QtGui.QComboBox(self.KmlElevationgroupBox)
        self.ExtrudeType.setEditable(False)
        self.ExtrudeType.setObjectName(_fromUtf8("ExtrudeType"))
        self.ExtrudeType.addItem(_fromUtf8(""))
        self.ExtrudeType.setItemText(0, _fromUtf8(""))
        self.ExtrudeType.addItem(_fromUtf8(""))
        self.ExtrudeType.addItem(_fromUtf8(""))
        self.ExtrudeType.addItem(_fromUtf8(""))
        self.ExtrudeType.addItem(_fromUtf8(""))
        self.horizontalLayout_10.addWidget(self.ExtrudeType)
        self.gridLayout_28.addLayout(self.horizontalLayout_10, 3, 0, 1, 1)
        self.horizontalLayout_11 = QtGui.QHBoxLayout()
        self.horizontalLayout_11.setObjectName(_fromUtf8("horizontalLayout_11"))
        self.AltitudeModelabel = QtGui.QLabel(self.KmlElevationgroupBox)
        self.AltitudeModelabel.setObjectName(_fromUtf8("AltitudeModelabel"))
        self.horizontalLayout_11.addWidget(self.AltitudeModelabel)
        self.AltitudeMode = QtGui.QComboBox(self.KmlElevationgroupBox)
        self.AltitudeMode.setObjectName(_fromUtf8("AltitudeMode"))
        self.AltitudeMode.addItem(_fromUtf8(""))
        self.AltitudeMode.setItemText(0, _fromUtf8(""))
        self.AltitudeMode.addItem(_fromUtf8(""))
        self.AltitudeMode.addItem(_fromUtf8(""))
        self.AltitudeMode.addItem(_fromUtf8(""))
        self.horizontalLayout_11.addWidget(self.AltitudeMode)
        self.gridLayout_28.addLayout(self.horizontalLayout_11, 1, 0, 1, 1)
        self.verticalLayout_2.addWidget(self.KmlElevationgroupBox)
        self.KmlColorgroupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.KmlColorgroupBox.setObjectName(_fromUtf8("KmlColorgroupBox"))
        self.gridLayout_29 = QtGui.QGridLayout(self.KmlColorgroupBox)
        self.gridLayout_29.setObjectName(_fromUtf8("gridLayout_29"))
        self.LabelColorhorizontalLayout = QtGui.QHBoxLayout()
        self.LabelColorhorizontalLayout.setObjectName(_fromUtf8("LabelColorhorizontalLayout"))
        self.LabelColor = QtGui.QLineEdit(self.KmlColorgroupBox)
        self.LabelColor.setReadOnly(True)
        self.LabelColor.setObjectName(_fromUtf8("LabelColor"))
        self.LabelColorhorizontalLayout.addWidget(self.LabelColor)
        self.LabelAlpha = QtGui.QSpinBox(self.KmlColorgroupBox)
        self.LabelAlpha.setMaximum(255)
        self.LabelAlpha.setProperty(_fromUtf8("value"), 255)
        self.LabelAlpha.setObjectName(_fromUtf8("LabelAlpha"))
        self.LabelColorhorizontalLayout.addWidget(self.LabelAlpha)
        self.gridLayout_29.addLayout(self.LabelColorhorizontalLayout, 1, 2, 1, 1)
        self.LineColorhorizontalLayout = QtGui.QHBoxLayout()
        self.LineColorhorizontalLayout.setObjectName(_fromUtf8("LineColorhorizontalLayout"))
        self.LineColor = QtGui.QLineEdit(self.KmlColorgroupBox)
        self.LineColor.setReadOnly(True)
        self.LineColor.setObjectName(_fromUtf8("LineColor"))
        self.LineColorhorizontalLayout.addWidget(self.LineColor)
        self.LineAlpha = QtGui.QSpinBox(self.KmlColorgroupBox)
        self.LineAlpha.setMaximum(255)
        self.LineAlpha.setProperty(_fromUtf8("value"), 255)
        self.LineAlpha.setObjectName(_fromUtf8("LineAlpha"))
        self.LineColorhorizontalLayout.addWidget(self.LineAlpha)
        self.gridLayout_29.addLayout(self.LineColorhorizontalLayout, 2, 2, 1, 1)
        self.PolyColorhorizontalLayout = QtGui.QHBoxLayout()
        self.PolyColorhorizontalLayout.setObjectName(_fromUtf8("PolyColorhorizontalLayout"))
        self.PolyColor = QtGui.QLineEdit(self.KmlColorgroupBox)
        self.PolyColor.setReadOnly(True)
        self.PolyColor.setObjectName(_fromUtf8("PolyColor"))
        self.PolyColorhorizontalLayout.addWidget(self.PolyColor)
        self.PolygonAlpha = QtGui.QSpinBox(self.KmlColorgroupBox)
        self.PolygonAlpha.setMaximum(255)
        self.PolygonAlpha.setProperty(_fromUtf8("value"), 255)
        self.PolygonAlpha.setObjectName(_fromUtf8("PolygonAlpha"))
        self.PolyColorhorizontalLayout.addWidget(self.PolygonAlpha)
        self.gridLayout_29.addLayout(self.PolyColorhorizontalLayout, 3, 2, 1, 1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        spacerItem16 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem16)
        self.colorModeEnum = QtGui.QComboBox(self.KmlColorgroupBox)
        self.colorModeEnum.setObjectName(_fromUtf8("colorModeEnum"))
        self.colorModeEnum.addItem(_fromUtf8(""))
        self.colorModeEnum.setItemText(0, _fromUtf8(""))
        self.colorModeEnum.addItem(_fromUtf8(""))
        self.colorModeEnum.addItem(_fromUtf8(""))
        self.horizontalLayout.addWidget(self.colorModeEnum)
        self.gridLayout_29.addLayout(self.horizontalLayout, 0, 2, 1, 1)
        self.VectorPolygonColor = QtGui.QToolButton(self.KmlColorgroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.VectorPolygonColor.sizePolicy().hasHeightForWidth())
        self.VectorPolygonColor.setSizePolicy(sizePolicy)
        self.VectorPolygonColor.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.VectorPolygonColor.setObjectName(_fromUtf8("VectorPolygonColor"))
        self.gridLayout_29.addWidget(self.VectorPolygonColor, 3, 0, 1, 1)
        self.VectorLineColor = QtGui.QToolButton(self.KmlColorgroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.VectorLineColor.sizePolicy().hasHeightForWidth())
        self.VectorLineColor.setSizePolicy(sizePolicy)
        self.VectorLineColor.setAcceptDrops(False)
        self.VectorLineColor.setAutoFillBackground(False)
        self.VectorLineColor.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.VectorLineColor.setAutoRaise(False)
        self.VectorLineColor.setObjectName(_fromUtf8("VectorLineColor"))
        self.gridLayout_29.addWidget(self.VectorLineColor, 2, 0, 1, 1)
        self.VectorLabelColor = QtGui.QToolButton(self.KmlColorgroupBox)
        self.VectorLabelColor.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.VectorLabelColor.setObjectName(_fromUtf8("VectorLabelColor"))
        self.gridLayout_29.addWidget(self.VectorLabelColor, 1, 0, 1, 1)
        self.ColorModelabel = QtGui.QLabel(self.KmlColorgroupBox)
        self.ColorModelabel.setObjectName(_fromUtf8("ColorModelabel"))
        self.gridLayout_29.addWidget(self.ColorModelabel, 0, 0, 1, 1)
        self.verticalLayout_2.addWidget(self.KmlColorgroupBox)
        self.groupBox_4 = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.groupBox_4.setTitle(_fromUtf8(""))
        self.groupBox_4.setObjectName(_fromUtf8("groupBox_4"))
        self.verticalLayout_7 = QtGui.QVBoxLayout(self.groupBox_4)
        self.verticalLayout_7.setObjectName(_fromUtf8("verticalLayout_7"))
        self.horizontalLayout_29 = QtGui.QHBoxLayout()
        self.horizontalLayout_29.setObjectName(_fromUtf8("horizontalLayout_29"))
        self.ExportVect = QtGui.QPushButton(self.groupBox_4)
        self.ExportVect.setObjectName(_fromUtf8("ExportVect"))
        self.horizontalLayout_29.addWidget(self.ExportVect)
        spacerItem17 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_29.addItem(spacerItem17)
        self.vectordir = QtGui.QPushButton(self.groupBox_4)
        self.vectordir.setEnabled(False)
        self.vectordir.setObjectName(_fromUtf8("vectordir"))
        self.horizontalLayout_29.addWidget(self.vectordir)
        self.verticalLayout_7.addLayout(self.horizontalLayout_29)
        self.verticalLayout_2.addWidget(self.groupBox_4)
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)
        self.verticalLayout_15.addWidget(self.scrollArea)
        self.tabWidget.addTab(self.tab, _fromUtf8(""))
        self.verticalLayout.addWidget(self.tabWidget)

        self.retranslateUi(Form)
        self.tabWidget.setCurrentIndex(0)
        self.tabWidget_3.setCurrentIndex(0)
        self.textlegendcolor.setCurrentIndex(1)
        self.TrueColor.setCurrentIndex(1)
        self.AlphaBkg.setCurrentIndex(1)
        self.PngRead.setCurrentIndex(2)
        self.PngKomp.setCurrentIndex(9)
        self.placemarks.setCurrentIndex(1)
        self.SelectIcon.setCurrentIndex(1)
        self.ExtrudeType.setCurrentIndex(3)
        self.AltitudeMode.setCurrentIndex(3)
        self.colorModeEnum.setCurrentIndex(1)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Data", None, QtGui.QApplication.UnicodeUTF8))
        self.InputElevlabel.setText(QtGui.QApplication.translate("Form", "Input", None, QtGui.QApplication.UnicodeUTF8))
        self.Tilinglabel.setText(QtGui.QApplication.translate("Form", "Tiling", None, QtGui.QApplication.UnicodeUTF8))
        self.Autotiling.setText(QtGui.QApplication.translate("Form", "AutoTiling", None, QtGui.QApplication.UnicodeUTF8))
        self.ManualTiling.setText(QtGui.QApplication.translate("Form", "ManualTiling", None, QtGui.QApplication.UnicodeUTF8))
        self.CopyElev.setText(QtGui.QApplication.translate("Form", "cp", None, QtGui.QApplication.UnicodeUTF8))
        self.ExportElev.setText(QtGui.QApplication.translate("Form", "Export", None, QtGui.QApplication.UnicodeUTF8))
        self.elevdir.setText(QtGui.QApplication.translate("Form", "Open Dir", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QtGui.QApplication.translate("Form", "Elevation", None, QtGui.QApplication.UnicodeUTF8))
        self.data.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("Form", "Raster", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("Form", "name", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Form", "legend text color", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(1, QtGui.QApplication.translate("Form", "red", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(2, QtGui.QApplication.translate("Form", "orange", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(3, QtGui.QApplication.translate("Form", "yellow", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(4, QtGui.QApplication.translate("Form", "green", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(5, QtGui.QApplication.translate("Form", "blue", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(6, QtGui.QApplication.translate("Form", "indigo", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(7, QtGui.QApplication.translate("Form", "violet", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(8, QtGui.QApplication.translate("Form", "white", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(9, QtGui.QApplication.translate("Form", "black", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(10, QtGui.QApplication.translate("Form", "gray", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(11, QtGui.QApplication.translate("Form", "brown", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(12, QtGui.QApplication.translate("Form", "magenta", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(13, QtGui.QApplication.translate("Form", "aqua", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(14, QtGui.QApplication.translate("Form", "grey", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(15, QtGui.QApplication.translate("Form", "cyan", None, QtGui.QApplication.UnicodeUTF8))
        self.textlegendcolor.setItemText(16, QtGui.QApplication.translate("Form", "purple", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("Form", "True-Color support", None, QtGui.QApplication.UnicodeUTF8))
        self.TrueColor.setItemText(1, QtGui.QApplication.translate("Form", "TRUE", None, QtGui.QApplication.UnicodeUTF8))
        self.TrueColor.setItemText(2, QtGui.QApplication.translate("Form", "FALSE", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("Form", "Transparent Background", None, QtGui.QApplication.UnicodeUTF8))
        self.AlphaBkg.setItemText(1, QtGui.QApplication.translate("Form", "TRUE", None, QtGui.QApplication.UnicodeUTF8))
        self.AlphaBkg.setItemText(2, QtGui.QApplication.translate("Form", "FALSE", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("Form", "PNG Read", None, QtGui.QApplication.UnicodeUTF8))
        self.PngRead.setItemText(1, QtGui.QApplication.translate("Form", "FALSE", None, QtGui.QApplication.UnicodeUTF8))
        self.PngRead.setItemText(2, QtGui.QApplication.translate("Form", "TRUE", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setText(QtGui.QApplication.translate("Form", "Background Color", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setText(QtGui.QApplication.translate("Form", "PNG Compression", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(1, QtGui.QApplication.translate("Form", "1", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(2, QtGui.QApplication.translate("Form", "2", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(3, QtGui.QApplication.translate("Form", "3", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(4, QtGui.QApplication.translate("Form", "4", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(5, QtGui.QApplication.translate("Form", "5", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(6, QtGui.QApplication.translate("Form", "6", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(7, QtGui.QApplication.translate("Form", "7", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(8, QtGui.QApplication.translate("Form", "8", None, QtGui.QApplication.UnicodeUTF8))
        self.PngKomp.setItemText(9, QtGui.QApplication.translate("Form", "9", None, QtGui.QApplication.UnicodeUTF8))
        self.label_10.setText(QtGui.QApplication.translate("Form", "Height", None, QtGui.QApplication.UnicodeUTF8))
        self.height.setText(QtGui.QApplication.translate("Form", "480", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("Form", "Width", None, QtGui.QApplication.UnicodeUTF8))
        self.width.setText(QtGui.QApplication.translate("Form", "640", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setText(QtGui.QApplication.translate("Form", "At", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget_3.setTabText(self.tabWidget_3.indexOf(self.tab_6), QtGui.QApplication.translate("Form", "I/O", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("Form", "Options", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("Form", "Lines", None, QtGui.QApplication.UnicodeUTF8))
        self.AdvLines.setText(QtGui.QApplication.translate("Form", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setText(QtGui.QApplication.translate("Form", "Thin", None, QtGui.QApplication.UnicodeUTF8))
        self.AdvThin.setText(QtGui.QApplication.translate("Form", "1", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setText(QtGui.QApplication.translate("Form", "LabelNum", None, QtGui.QApplication.UnicodeUTF8))
        self.AdvLabelNum.setText(QtGui.QApplication.translate("Form", "5", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setText(QtGui.QApplication.translate("Form", "Use", None, QtGui.QApplication.UnicodeUTF8))
        self.label_15.setText(QtGui.QApplication.translate("Form", "Range", None, QtGui.QApplication.UnicodeUTF8))
        self.flag_v.setText(QtGui.QApplication.translate("Form", "Do not show category labels", None, QtGui.QApplication.UnicodeUTF8))
        self.flag_c.setText(QtGui.QApplication.translate("Form", "Do not show category numbers", None, QtGui.QApplication.UnicodeUTF8))
        self.flag_n.setText(QtGui.QApplication.translate("Form", "Skip category with no labels", None, QtGui.QApplication.UnicodeUTF8))
        self.flag_s.setText(QtGui.QApplication.translate("Form", "Draw smooth gradient", None, QtGui.QApplication.UnicodeUTF8))
        self.flag_f.setText(QtGui.QApplication.translate("Form", "Flip legend", None, QtGui.QApplication.UnicodeUTF8))
        self.placemark.setText(QtGui.QApplication.translate("Form", "Placemark", None, QtGui.QApplication.UnicodeUTF8))
        self.placemarks.setItemText(0, QtGui.QApplication.translate("Form", "icon", None, QtGui.QApplication.UnicodeUTF8))
        self.placemarks.setItemText(1, QtGui.QApplication.translate("Form", "yellow_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.placemarks.setItemText(2, QtGui.QApplication.translate("Form", "red_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.placemarks.setItemText(3, QtGui.QApplication.translate("Form", "blue_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.placemarks.setItemText(4, QtGui.QApplication.translate("Form", "green_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.addlegend.setText(QtGui.QApplication.translate("Form", "l", None, QtGui.QApplication.UnicodeUTF8))
        self.adddescription.setText(QtGui.QApplication.translate("Form", "d", None, QtGui.QApplication.UnicodeUTF8))
        self.flag_z.setText(QtGui.QApplication.translate("Form", "i", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget_3.setTabText(self.tabWidget_3.indexOf(self.tab_8), QtGui.QApplication.translate("Form", "Opt", None, QtGui.QApplication.UnicodeUTF8))
        self.exportoverlay.setText(QtGui.QApplication.translate("Form", "Export", None, QtGui.QApplication.UnicodeUTF8))
        self.overlaydir.setText(QtGui.QApplication.translate("Form", "Open Dir", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_7), QtGui.QApplication.translate("Form", "Overlay", None, QtGui.QApplication.UnicodeUTF8))
        self.Point.setText(QtGui.QApplication.translate("Form", "Point", None, QtGui.QApplication.UnicodeUTF8))
        self.Line.setText(QtGui.QApplication.translate("Form", "Line", None, QtGui.QApplication.UnicodeUTF8))
        self.Polygon.setText(QtGui.QApplication.translate("Form", "Polygon", None, QtGui.QApplication.UnicodeUTF8))
        self.InputVectorlabel.setText(QtGui.QApplication.translate("Form", "Input", None, QtGui.QApplication.UnicodeUTF8))
        self.KmlGeneralgroupBox.setTitle(QtGui.QApplication.translate("Form", "General", None, QtGui.QApplication.UnicodeUTF8))
        self.Iconlabel.setText(QtGui.QApplication.translate("Form", "Icon", None, QtGui.QApplication.UnicodeUTF8))
        self.LabelFieldlabel.setText(QtGui.QApplication.translate("Form", "LabelField", None, QtGui.QApplication.UnicodeUTF8))
        self.LineWidthlabel.setText(QtGui.QApplication.translate("Form", "Line Width", None, QtGui.QApplication.UnicodeUTF8))
        self.SelectIcon.setItemText(1, QtGui.QApplication.translate("Form", "blue_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.SelectIcon.setItemText(2, QtGui.QApplication.translate("Form", "green_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.SelectIcon.setItemText(3, QtGui.QApplication.translate("Form", "red_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.SelectIcon.setItemText(4, QtGui.QApplication.translate("Form", "yellow_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.Tessellatelabel.setText(QtGui.QApplication.translate("Form", "Tessellate", None, QtGui.QApplication.UnicodeUTF8))
        self.Extrudelabel.setText(QtGui.QApplication.translate("Form", "Extrude", None, QtGui.QApplication.UnicodeUTF8))
        self.KmlElevationgroupBox.setTitle(QtGui.QApplication.translate("Form", "Elevation", None, QtGui.QApplication.UnicodeUTF8))
        self.Offsetlabel.setText(QtGui.QApplication.translate("Form", "OffSet", None, QtGui.QApplication.UnicodeUTF8))
        self.HeightFieldlabel.setText(QtGui.QApplication.translate("Form", "HeightField", None, QtGui.QApplication.UnicodeUTF8))
        self.Heightlabel.setText(QtGui.QApplication.translate("Form", "Heigth", None, QtGui.QApplication.UnicodeUTF8))
        self.ExtrudeType.setItemText(1, QtGui.QApplication.translate("Form", "2D", None, QtGui.QApplication.UnicodeUTF8))
        self.ExtrudeType.setItemText(2, QtGui.QApplication.translate("Form", "3D", None, QtGui.QApplication.UnicodeUTF8))
        self.ExtrudeType.setItemText(3, QtGui.QApplication.translate("Form", "Offset", None, QtGui.QApplication.UnicodeUTF8))
        self.ExtrudeType.setItemText(4, QtGui.QApplication.translate("Form", "Attribute", None, QtGui.QApplication.UnicodeUTF8))
        self.AltitudeModelabel.setText(QtGui.QApplication.translate("Form", "Altitude   ", None, QtGui.QApplication.UnicodeUTF8))
        self.AltitudeMode.setItemText(1, QtGui.QApplication.translate("Form", "clampToGround", None, QtGui.QApplication.UnicodeUTF8))
        self.AltitudeMode.setItemText(2, QtGui.QApplication.translate("Form", "relativeToGround", None, QtGui.QApplication.UnicodeUTF8))
        self.AltitudeMode.setItemText(3, QtGui.QApplication.translate("Form", "absolute", None, QtGui.QApplication.UnicodeUTF8))
        self.KmlColorgroupBox.setTitle(QtGui.QApplication.translate("Form", "Color", None, QtGui.QApplication.UnicodeUTF8))
        self.colorModeEnum.setItemText(1, QtGui.QApplication.translate("Form", "normal", None, QtGui.QApplication.UnicodeUTF8))
        self.colorModeEnum.setItemText(2, QtGui.QApplication.translate("Form", "random", None, QtGui.QApplication.UnicodeUTF8))
        self.VectorPolygonColor.setText(QtGui.QApplication.translate("Form", "Polygon", None, QtGui.QApplication.UnicodeUTF8))
        self.VectorLineColor.setText(QtGui.QApplication.translate("Form", "Line", None, QtGui.QApplication.UnicodeUTF8))
        self.VectorLabelColor.setText(QtGui.QApplication.translate("Form", "Label", None, QtGui.QApplication.UnicodeUTF8))
        self.ColorModelabel.setText(QtGui.QApplication.translate("Form", "ColorMode", None, QtGui.QApplication.UnicodeUTF8))
        self.ExportVect.setText(QtGui.QApplication.translate("Form", "Export", None, QtGui.QApplication.UnicodeUTF8))
        self.vectordir.setText(QtGui.QApplication.translate("Form", "Open Dir", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("Form", "KML", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
