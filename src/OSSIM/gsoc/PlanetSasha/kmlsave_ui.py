# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/Users/Shared/ossim/trunk/gsoc/PlanetSasha/gui/kmlsave_ui.ui'
#
# Created: Mon Feb 14 21:57:48 2011
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
        Form.resize(355, 516)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/element-vector.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Form.setWindowIcon(icon)
        self.verticalLayout_3 = QtGui.QVBoxLayout(Form)
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.scrollArea = QtGui.QScrollArea(Form)
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName(_fromUtf8("scrollArea"))
        self.scrollAreaWidgetContents = QtGui.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 314, 705))
        self.scrollAreaWidgetContents.setObjectName(_fromUtf8("scrollAreaWidgetContents"))
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.scrollAreaWidgetContents)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.KmlGeneralgroupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.KmlGeneralgroupBox.setObjectName(_fromUtf8("KmlGeneralgroupBox"))
        self.verticalLayout_4 = QtGui.QVBoxLayout(self.KmlGeneralgroupBox)
        self.verticalLayout_4.setObjectName(_fromUtf8("verticalLayout_4"))
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
" }\n"
""))
        self.kmlname.setObjectName(_fromUtf8("kmlname"))
        self.gridLayout.addWidget(self.kmlname, 0, 1, 1, 1)
        self.label_2 = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)
        self.kmllabel = QtGui.QLineEdit(self.KmlGeneralgroupBox)
        self.kmllabel.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.kmllabel.setObjectName(_fromUtf8("kmllabel"))
        self.gridLayout.addWidget(self.kmllabel, 1, 1, 1, 1)
        self.Iconlabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.Iconlabel.setObjectName(_fromUtf8("Iconlabel"))
        self.gridLayout.addWidget(self.Iconlabel, 2, 0, 1, 1)
        self.SelectIcon = QtGui.QComboBox(self.KmlGeneralgroupBox)
        self.SelectIcon.setObjectName(_fromUtf8("SelectIcon"))
        self.SelectIcon.addItem(_fromUtf8(""))
        self.SelectIcon.setItemText(0, _fromUtf8(""))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/blue_circle.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.SelectIcon.addItem(icon1, _fromUtf8(""))
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/green_circle.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.SelectIcon.addItem(icon2, _fromUtf8(""))
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/red_circle.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.SelectIcon.addItem(icon3, _fromUtf8(""))
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/yellow_circle.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.SelectIcon.addItem(icon4, _fromUtf8(""))
        self.gridLayout.addWidget(self.SelectIcon, 2, 1, 1, 1)
        self.LineWidthlabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.LineWidthlabel.setObjectName(_fromUtf8("LineWidthlabel"))
        self.gridLayout.addWidget(self.LineWidthlabel, 3, 0, 1, 1)
        self.horizontalLayout_9 = QtGui.QHBoxLayout()
        self.horizontalLayout_9.setObjectName(_fromUtf8("horizontalLayout_9"))
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_9.addItem(spacerItem)
        self.LineWidth = QtGui.QSpinBox(self.KmlGeneralgroupBox)
        self.LineWidth.setStyleSheet(_fromUtf8("background-color: rgb(231, 231, 231);"))
        self.LineWidth.setObjectName(_fromUtf8("LineWidth"))
        self.horizontalLayout_9.addWidget(self.LineWidth)
        self.gridLayout.addLayout(self.horizontalLayout_9, 3, 1, 1, 1)
        self.Extrudelabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.Extrudelabel.setObjectName(_fromUtf8("Extrudelabel"))
        self.gridLayout.addWidget(self.Extrudelabel, 4, 0, 1, 1)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.Extrude = QtGui.QCheckBox(self.KmlGeneralgroupBox)
        self.Extrude.setText(_fromUtf8(""))
        self.Extrude.setObjectName(_fromUtf8("Extrude"))
        self.horizontalLayout_4.addWidget(self.Extrude)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem1)
        self.Offset = QtGui.QDoubleSpinBox(self.KmlGeneralgroupBox)
        self.Offset.setStyleSheet(_fromUtf8("background-color: rgb(231, 231, 231);"))
        self.Offset.setObjectName(_fromUtf8("Offset"))
        self.horizontalLayout_4.addWidget(self.Offset)
        self.gridLayout.addLayout(self.horizontalLayout_4, 4, 1, 1, 1)
        self.Tessellatelabel = QtGui.QLabel(self.KmlGeneralgroupBox)
        self.Tessellatelabel.setObjectName(_fromUtf8("Tessellatelabel"))
        self.gridLayout.addWidget(self.Tessellatelabel, 5, 0, 1, 1)
        self.horizontalLayout_6 = QtGui.QHBoxLayout()
        self.horizontalLayout_6.setObjectName(_fromUtf8("horizontalLayout_6"))
        self.Tessellate = QtGui.QCheckBox(self.KmlGeneralgroupBox)
        self.Tessellate.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Tessellate.setText(_fromUtf8(""))
        self.Tessellate.setObjectName(_fromUtf8("Tessellate"))
        self.horizontalLayout_6.addWidget(self.Tessellate)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_6.addItem(spacerItem2)
        self.gridLayout.addLayout(self.horizontalLayout_6, 5, 1, 1, 1)
        self.verticalLayout_4.addLayout(self.gridLayout)
        self.verticalLayout_2.addWidget(self.KmlGeneralgroupBox)
        self.KmlColorgroupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.KmlColorgroupBox.setObjectName(_fromUtf8("KmlColorgroupBox"))
        self.verticalLayout_5 = QtGui.QVBoxLayout(self.KmlColorgroupBox)
        self.verticalLayout_5.setObjectName(_fromUtf8("verticalLayout_5"))
        self.gridLayout_3 = QtGui.QGridLayout()
        self.gridLayout_3.setObjectName(_fromUtf8("gridLayout_3"))
        self.labelcolor = QtGui.QToolButton(self.KmlColorgroupBox)
        self.labelcolor.setMaximumSize(QtCore.QSize(100, 16777215))
        self.labelcolor.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.labelcolor.setObjectName(_fromUtf8("labelcolor"))
        self.gridLayout_3.addWidget(self.labelcolor, 0, 0, 1, 1)
        self.linecolor = QtGui.QToolButton(self.KmlColorgroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.linecolor.sizePolicy().hasHeightForWidth())
        self.linecolor.setSizePolicy(sizePolicy)
        self.linecolor.setAcceptDrops(False)
        self.linecolor.setAutoFillBackground(False)
        self.linecolor.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.linecolor.setAutoRaise(False)
        self.linecolor.setObjectName(_fromUtf8("linecolor"))
        self.gridLayout_3.addWidget(self.linecolor, 1, 0, 1, 1)
        self.polygoncolor = QtGui.QToolButton(self.KmlColorgroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.polygoncolor.sizePolicy().hasHeightForWidth())
        self.polygoncolor.setSizePolicy(sizePolicy)
        self.polygoncolor.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.polygoncolor.setObjectName(_fromUtf8("polygoncolor"))
        self.gridLayout_3.addWidget(self.polygoncolor, 2, 0, 1, 1)
        self.LabelColorhorizontalLayout = QtGui.QHBoxLayout()
        self.LabelColorhorizontalLayout.setObjectName(_fromUtf8("LabelColorhorizontalLayout"))
        self.labelcolorlabel = QtGui.QLineEdit(self.KmlColorgroupBox)
        self.labelcolorlabel.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.labelcolorlabel.setReadOnly(True)
        self.labelcolorlabel.setObjectName(_fromUtf8("labelcolorlabel"))
        self.LabelColorhorizontalLayout.addWidget(self.labelcolorlabel)
        self.LabelAlpha = QtGui.QSpinBox(self.KmlColorgroupBox)
        self.LabelAlpha.setStyleSheet(_fromUtf8("background-color: rgb(231, 231, 231);"))
        self.LabelAlpha.setMaximum(255)
        self.LabelAlpha.setProperty(_fromUtf8("value"), 255)
        self.LabelAlpha.setObjectName(_fromUtf8("LabelAlpha"))
        self.LabelColorhorizontalLayout.addWidget(self.LabelAlpha)
        self.gridLayout_3.addLayout(self.LabelColorhorizontalLayout, 0, 1, 1, 1)
        self.LineColorhorizontalLayout = QtGui.QHBoxLayout()
        self.LineColorhorizontalLayout.setObjectName(_fromUtf8("LineColorhorizontalLayout"))
        self.linecolorlabel = QtGui.QLineEdit(self.KmlColorgroupBox)
        self.linecolorlabel.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.linecolorlabel.setReadOnly(True)
        self.linecolorlabel.setObjectName(_fromUtf8("linecolorlabel"))
        self.LineColorhorizontalLayout.addWidget(self.linecolorlabel)
        self.LineAlpha = QtGui.QSpinBox(self.KmlColorgroupBox)
        self.LineAlpha.setStyleSheet(_fromUtf8("background-color: rgb(231, 231, 231);"))
        self.LineAlpha.setMaximum(255)
        self.LineAlpha.setProperty(_fromUtf8("value"), 255)
        self.LineAlpha.setObjectName(_fromUtf8("LineAlpha"))
        self.LineColorhorizontalLayout.addWidget(self.LineAlpha)
        self.gridLayout_3.addLayout(self.LineColorhorizontalLayout, 1, 1, 1, 1)
        self.PolyColorhorizontalLayout = QtGui.QHBoxLayout()
        self.PolyColorhorizontalLayout.setObjectName(_fromUtf8("PolyColorhorizontalLayout"))
        self.polygoncolorlabel = QtGui.QLineEdit(self.KmlColorgroupBox)
        self.polygoncolorlabel.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.polygoncolorlabel.setReadOnly(True)
        self.polygoncolorlabel.setObjectName(_fromUtf8("polygoncolorlabel"))
        self.PolyColorhorizontalLayout.addWidget(self.polygoncolorlabel)
        self.PolygonAlpha = QtGui.QSpinBox(self.KmlColorgroupBox)
        self.PolygonAlpha.setStyleSheet(_fromUtf8("background-color: rgb(231, 231, 231);"))
        self.PolygonAlpha.setMaximum(255)
        self.PolygonAlpha.setProperty(_fromUtf8("value"), 255)
        self.PolygonAlpha.setObjectName(_fromUtf8("PolygonAlpha"))
        self.PolyColorhorizontalLayout.addWidget(self.PolygonAlpha)
        self.gridLayout_3.addLayout(self.PolyColorhorizontalLayout, 2, 1, 1, 1)
        self.verticalLayout_5.addLayout(self.gridLayout_3)
        self.verticalLayout_2.addWidget(self.KmlColorgroupBox)
        self.groupBox = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.gridLayout_2 = QtGui.QGridLayout(self.groupBox)
        self.gridLayout_2.setObjectName(_fromUtf8("gridLayout_2"))
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.longitude = QtGui.QLineEdit(self.groupBox)
        self.longitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.longitude.setObjectName(_fromUtf8("longitude"))
        self.horizontalLayout_3.addWidget(self.longitude)
        self.latitude = QtGui.QLineEdit(self.groupBox)
        self.latitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.latitude.setObjectName(_fromUtf8("latitude"))
        self.horizontalLayout_3.addWidget(self.latitude)
        self.gridLayout_2.addLayout(self.horizontalLayout_3, 0, 0, 1, 1)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.Zoom = QtGui.QLineEdit(self.groupBox)
        self.Zoom.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.Zoom.setObjectName(_fromUtf8("Zoom"))
        self.horizontalLayout_5.addWidget(self.Zoom)
        self.Range = QtGui.QLineEdit(self.groupBox)
        self.Range.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.Range.setObjectName(_fromUtf8("Range"))
        self.horizontalLayout_5.addWidget(self.Range)
        self.Roll = QtGui.QLineEdit(self.groupBox)
        self.Roll.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.Roll.setObjectName(_fromUtf8("Roll"))
        self.horizontalLayout_5.addWidget(self.Roll)
        self.Pitch = QtGui.QLineEdit(self.groupBox)
        self.Pitch.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.Pitch.setObjectName(_fromUtf8("Pitch"))
        self.horizontalLayout_5.addWidget(self.Pitch)
        self.Head = QtGui.QLineEdit(self.groupBox)
        self.Head.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.Head.setObjectName(_fromUtf8("Head"))
        self.horizontalLayout_5.addWidget(self.Head)
        self.gridLayout_2.addLayout(self.horizontalLayout_5, 1, 0, 1, 1)
        self.verticalLayout_2.addWidget(self.groupBox)
        self.groupBox_2 = QtGui.QGroupBox(self.scrollAreaWidgetContents)
        self.groupBox_2.setObjectName(_fromUtf8("groupBox_2"))
        self.verticalLayout = QtGui.QVBoxLayout(self.groupBox_2)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.frame = QtGui.QFrame(self.groupBox_2)
        self.frame.setFrameShape(QtGui.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtGui.QFrame.Raised)
        self.frame.setObjectName(_fromUtf8("frame"))
        self.gridLayout_4 = QtGui.QGridLayout(self.frame)
        self.gridLayout_4.setObjectName(_fromUtf8("gridLayout_4"))
        self.addlink = QtGui.QToolButton(self.frame)
        self.addlink.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.addlink.setText(_fromUtf8(""))
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/firefox_logo.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.addlink.setIcon(icon5)
        self.addlink.setObjectName(_fromUtf8("addlink"))
        self.gridLayout_4.addWidget(self.addlink, 0, 0, 1, 1)
        self.addimage = QtGui.QToolButton(self.frame)
        self.addimage.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.addimage.setText(_fromUtf8(""))
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/gimp.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.addimage.setIcon(icon6)
        self.addimage.setObjectName(_fromUtf8("addimage"))
        self.gridLayout_4.addWidget(self.addimage, 0, 1, 1, 1)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout_4.addItem(spacerItem3, 0, 3, 1, 1)
        self.clean = QtGui.QToolButton(self.frame)
        self.clean.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
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
        self.clean.setText(_fromUtf8(""))
        icon7 = QtGui.QIcon()
        icon7.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/mActionDraw.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.clean.setIcon(icon7)
        self.clean.setObjectName(_fromUtf8("clean"))
        self.gridLayout_4.addWidget(self.clean, 0, 2, 1, 1)
        self.verticalLayout.addWidget(self.frame)
        self.description = QtGui.QTextEdit(self.groupBox_2)
        self.description.setObjectName(_fromUtf8("description"))
        self.verticalLayout.addWidget(self.description)
        self.verticalLayout_2.addWidget(self.groupBox_2)
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)
        self.verticalLayout_3.addWidget(self.scrollArea)
        self.horizontalLayout_8 = QtGui.QHBoxLayout()
        self.horizontalLayout_8.setObjectName(_fromUtf8("horizontalLayout_8"))
        spacerItem4 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem4)
        self.update = QtGui.QPushButton(Form)
        self.update.setText(_fromUtf8(""))
        self.update.setIcon(icon7)
        self.update.setObjectName(_fromUtf8("update"))
        self.horizontalLayout_8.addWidget(self.update)
        spacerItem5 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem5)
        self.save = QtGui.QPushButton(Form)
        self.save.setText(_fromUtf8(""))
        icon8 = QtGui.QIcon()
        icon8.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/db.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.save.setIcon(icon8)
        self.save.setObjectName(_fromUtf8("save"))
        self.horizontalLayout_8.addWidget(self.save)
        spacerItem6 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem6)
        self.opendir = QtGui.QPushButton(Form)
        self.opendir.setObjectName(_fromUtf8("opendir"))
        self.horizontalLayout_8.addWidget(self.opendir)
        spacerItem7 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem7)
        self.verticalLayout_3.addLayout(self.horizontalLayout_8)

        self.retranslateUi(Form)
        self.SelectIcon.setCurrentIndex(1)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Kml view-save", None, QtGui.QApplication.UnicodeUTF8))
        self.KmlGeneralgroupBox.setTitle(QtGui.QApplication.translate("Form", "General", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Form", "Name", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("Form", "Label", None, QtGui.QApplication.UnicodeUTF8))
        self.Iconlabel.setText(QtGui.QApplication.translate("Form", "Icon", None, QtGui.QApplication.UnicodeUTF8))
        self.SelectIcon.setItemText(1, QtGui.QApplication.translate("Form", "blue_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.SelectIcon.setItemText(2, QtGui.QApplication.translate("Form", "green_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.SelectIcon.setItemText(3, QtGui.QApplication.translate("Form", "red_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.SelectIcon.setItemText(4, QtGui.QApplication.translate("Form", "yellow_circle", None, QtGui.QApplication.UnicodeUTF8))
        self.LineWidthlabel.setText(QtGui.QApplication.translate("Form", "Line Width", None, QtGui.QApplication.UnicodeUTF8))
        self.Extrudelabel.setText(QtGui.QApplication.translate("Form", "Extrude", None, QtGui.QApplication.UnicodeUTF8))
        self.Tessellatelabel.setText(QtGui.QApplication.translate("Form", "Tessellate", None, QtGui.QApplication.UnicodeUTF8))
        self.KmlColorgroupBox.setTitle(QtGui.QApplication.translate("Form", "Color", None, QtGui.QApplication.UnicodeUTF8))
        self.labelcolor.setText(QtGui.QApplication.translate("Form", "Label", None, QtGui.QApplication.UnicodeUTF8))
        self.linecolor.setText(QtGui.QApplication.translate("Form", "Line", None, QtGui.QApplication.UnicodeUTF8))
        self.polygoncolor.setText(QtGui.QApplication.translate("Form", "Polygon", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("Form", "Position", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_2.setTitle(QtGui.QApplication.translate("Form", "Description", None, QtGui.QApplication.UnicodeUTF8))
        self.opendir.setText(QtGui.QApplication.translate("Form", "Open Dir", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
