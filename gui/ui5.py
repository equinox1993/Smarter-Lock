# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui.ui'
#
# Created by: PyQt5 UI code generator 5.4.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Form(QtWidgets.QWidget):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)
        self.setupUi(self)
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(240, 320)
        Form.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.verticalLayout = QtWidgets.QVBoxLayout(Form)
        self.verticalLayout.setSizeConstraint(QtWidgets.QLayout.SetDefaultConstraint)
        self.verticalLayout.setObjectName("verticalLayout")
        self.label = QtWidgets.QLabel(Form)
        font = QtGui.QFont()
        font.setPointSize(36)
        font.setBold(False)
        font.setWeight(50)
        self.label.setFont(font)
        self.label.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.label.setFrameShadow(QtWidgets.QFrame.Plain)
        self.label.setLineWidth(1)
        self.label.setText("")
        self.label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label.setObjectName("label")
        self.verticalLayout.addWidget(self.label)
        self.gridLayout = QtWidgets.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.numButton_9 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_9.sizePolicy().hasHeightForWidth())
        self.numButton_9.setSizePolicy(sizePolicy)
        self.numButton_9.setObjectName("numButton_9")
        self.gridLayout.addWidget(self.numButton_9, 2, 2, 1, 1)
        self.numButton_4 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_4.sizePolicy().hasHeightForWidth())
        self.numButton_4.setSizePolicy(sizePolicy)
        self.numButton_4.setObjectName("numButton_4")
        self.gridLayout.addWidget(self.numButton_4, 1, 0, 1, 1)
        self.numButton_1 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_1.sizePolicy().hasHeightForWidth())
        self.numButton_1.setSizePolicy(sizePolicy)
        self.numButton_1.setObjectName("numButton_1")
        self.gridLayout.addWidget(self.numButton_1, 0, 0, 1, 1)
        self.unlockButton = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.unlockButton.sizePolicy().hasHeightForWidth())
        self.unlockButton.setSizePolicy(sizePolicy)
        self.unlockButton.setObjectName("unlockButton")
        self.gridLayout.addWidget(self.unlockButton, 3, 2, 1, 1)
        self.numButton_2 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_2.sizePolicy().hasHeightForWidth())
        self.numButton_2.setSizePolicy(sizePolicy)
        self.numButton_2.setObjectName("numButton_2")
        self.gridLayout.addWidget(self.numButton_2, 0, 1, 1, 1)
        self.numButton_5 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_5.sizePolicy().hasHeightForWidth())
        self.numButton_5.setSizePolicy(sizePolicy)
        self.numButton_5.setObjectName("numButton_5")
        self.gridLayout.addWidget(self.numButton_5, 1, 1, 1, 1)
        self.numButton_3 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_3.sizePolicy().hasHeightForWidth())
        self.numButton_3.setSizePolicy(sizePolicy)
        self.numButton_3.setObjectName("numButton_3")
        self.gridLayout.addWidget(self.numButton_3, 0, 2, 1, 1)
        self.numButton_0 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_0.sizePolicy().hasHeightForWidth())
        self.numButton_0.setSizePolicy(sizePolicy)
        self.numButton_0.setObjectName("numButton_0")
        self.gridLayout.addWidget(self.numButton_0, 3, 1, 1, 1)
        self.numButton_6 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_6.sizePolicy().hasHeightForWidth())
        self.numButton_6.setSizePolicy(sizePolicy)
        self.numButton_6.setObjectName("numButton_6")
        self.gridLayout.addWidget(self.numButton_6, 1, 2, 1, 1)
        self.numButton_8 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_8.sizePolicy().hasHeightForWidth())
        self.numButton_8.setSizePolicy(sizePolicy)
        self.numButton_8.setObjectName("numButton_8")
        self.gridLayout.addWidget(self.numButton_8, 2, 1, 1, 1)
        self.numButton_7 = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.numButton_7.sizePolicy().hasHeightForWidth())
        self.numButton_7.setSizePolicy(sizePolicy)
        self.numButton_7.setObjectName("numButton_7")
        self.gridLayout.addWidget(self.numButton_7, 2, 0, 1, 1)
        self.clearButton = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.clearButton.sizePolicy().hasHeightForWidth())
        self.clearButton.setSizePolicy(sizePolicy)
        self.clearButton.setObjectName("clearButton")
        self.gridLayout.addWidget(self.clearButton, 3, 0, 1, 1)
        self.scanButton = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.scanButton.sizePolicy().hasHeightForWidth())
        self.scanButton.setSizePolicy(sizePolicy)
        self.scanButton.setObjectName("scanButton")
        self.gridLayout.addWidget(self.scanButton, 4, 0, 1, 1)
        self.ringButton = QtWidgets.QPushButton(Form)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ringButton.sizePolicy().hasHeightForWidth())
        self.ringButton.setSizePolicy(sizePolicy)
        self.ringButton.setObjectName("ringButton")
        self.gridLayout.addWidget(self.ringButton, 4, 2, 1, 1)
        self.gridLayout.setColumnStretch(0, 1)
        self.gridLayout.setColumnStretch(1, 1)
        self.gridLayout.setColumnStretch(2, 1)
        self.gridLayout.setRowStretch(0, 1)
        self.gridLayout.setRowStretch(1, 1)
        self.gridLayout.setRowStretch(2, 1)
        self.gridLayout.setRowStretch(3, 1)
        self.gridLayout.setRowStretch(4, 1)
        self.verticalLayout.addLayout(self.gridLayout)
        self.verticalLayout.setStretch(1, 5)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Form"))
        self.numButton_9.setText(_translate("Form", "9"))
        self.numButton_4.setText(_translate("Form", "4"))
        self.numButton_1.setText(_translate("Form", "1"))
        self.unlockButton.setText(_translate("Form", "Open"))
        self.numButton_2.setText(_translate("Form", "2"))
        self.numButton_5.setText(_translate("Form", "5"))
        self.numButton_3.setText(_translate("Form", "3"))
        self.numButton_0.setText(_translate("Form", "0"))
        self.numButton_6.setText(_translate("Form", "6"))
        self.numButton_8.setText(_translate("Form", "8"))
        self.numButton_7.setText(_translate("Form", "7"))
        self.clearButton.setText(_translate("Form", "Clear"))
        self.scanButton.setText(_translate("Form", " QR"))
        self.ringButton.setText(_translate("Form", "Ring"))

