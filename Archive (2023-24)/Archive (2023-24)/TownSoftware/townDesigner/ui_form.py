# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'form.ui'
##
## Created by: Qt User Interface Compiler version 6.7.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QFrame, QGridLayout, QHBoxLayout,
    QLabel, QMainWindow, QPushButton, QScrollArea,
    QSizePolicy, QSpacerItem, QTextEdit, QVBoxLayout,
    QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(858, 627)
        MainWindow.setWindowOpacity(1.000000000000000)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.gridLayout = QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName(u"gridLayout")
        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.frame_2 = QFrame(self.centralwidget)
        self.frame_2.setObjectName(u"frame_2")
        self.frame_2.setMouseTracking(False)
        self.frame_2.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame_2.setFrameShadow(QFrame.Shadow.Raised)

        self.horizontalLayout.addWidget(self.frame_2)

        self.frame = QFrame(self.centralwidget)
        self.frame.setObjectName(u"frame")
        self.frame.setMinimumSize(QSize(27, 0))
        self.frame.setMaximumSize(QSize(283, 16777215))
        self.frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame.setFrameShadow(QFrame.Shadow.Raised)
        self.widget = QWidget(self.frame)
        self.widget.setObjectName(u"widget")
        self.widget.setGeometry(QRect(10, 8, 265, 591))
        self.verticalLayout_2 = QVBoxLayout(self.widget)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setSpacing(9)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.columnLabel = QLabel(self.widget)
        self.columnLabel.setObjectName(u"columnLabel")

        self.horizontalLayout_2.addWidget(self.columnLabel)

        self.rowLabel = QLabel(self.widget)
        self.rowLabel.setObjectName(u"rowLabel")

        self.horizontalLayout_2.addWidget(self.rowLabel)


        self.verticalLayout.addLayout(self.horizontalLayout_2)

        self.horizontalLayout_3 = QHBoxLayout()
        self.horizontalLayout_3.setSpacing(0)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.columnTextEdit = QTextEdit(self.widget)
        self.columnTextEdit.setObjectName(u"columnTextEdit")
        self.columnTextEdit.setMinimumSize(QSize(81, 31))
        self.columnTextEdit.setMaximumSize(QSize(81, 31))
        self.columnTextEdit.viewport().setProperty("cursor", QCursor(Qt.CursorShape.IBeamCursor))
        self.columnTextEdit.setMouseTracking(False)
        self.columnTextEdit.setLayoutDirection(Qt.LayoutDirection.RightToLeft)
        self.columnTextEdit.setTabChangesFocus(True)
        self.columnTextEdit.setCursorWidth(3)

        self.horizontalLayout_3.addWidget(self.columnTextEdit)

        self.columnUpButton = QPushButton(self.widget)
        self.columnUpButton.setObjectName(u"columnUpButton")
        self.columnUpButton.setMinimumSize(QSize(22, 31))
        self.columnUpButton.setMaximumSize(QSize(21, 31))
        self.columnUpButton.setCursor(QCursor(Qt.CursorShape.PointingHandCursor))

        self.horizontalLayout_3.addWidget(self.columnUpButton)

        self.columnDownButton = QPushButton(self.widget)
        self.columnDownButton.setObjectName(u"columnDownButton")
        self.columnDownButton.setMinimumSize(QSize(22, 31))
        self.columnDownButton.setMaximumSize(QSize(21, 31))
        self.columnDownButton.setCursor(QCursor(Qt.CursorShape.PointingHandCursor))

        self.horizontalLayout_3.addWidget(self.columnDownButton)

        self.horizontalSpacer = QSpacerItem(9, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.horizontalSpacer)

        self.rowTextEdit = QTextEdit(self.widget)
        self.rowTextEdit.setObjectName(u"rowTextEdit")
        self.rowTextEdit.setMinimumSize(QSize(81, 31))
        self.rowTextEdit.setMaximumSize(QSize(81, 31))
        self.rowTextEdit.viewport().setProperty("cursor", QCursor(Qt.CursorShape.IBeamCursor))
        self.rowTextEdit.setMouseTracking(False)
        self.rowTextEdit.setLayoutDirection(Qt.LayoutDirection.RightToLeft)
        self.rowTextEdit.setTabChangesFocus(True)
        self.rowTextEdit.setCursorWidth(3)

        self.horizontalLayout_3.addWidget(self.rowTextEdit)

        self.rowUpButton = QPushButton(self.widget)
        self.rowUpButton.setObjectName(u"rowUpButton")
        self.rowUpButton.setMinimumSize(QSize(22, 31))
        self.rowUpButton.setMaximumSize(QSize(21, 31))
        self.rowUpButton.setCursor(QCursor(Qt.CursorShape.PointingHandCursor))

        self.horizontalLayout_3.addWidget(self.rowUpButton)

        self.rowDownButton = QPushButton(self.widget)
        self.rowDownButton.setObjectName(u"rowDownButton")
        self.rowDownButton.setMinimumSize(QSize(22, 31))
        self.rowDownButton.setMaximumSize(QSize(21, 31))
        self.rowDownButton.setCursor(QCursor(Qt.CursorShape.PointingHandCursor))

        self.horizontalLayout_3.addWidget(self.rowDownButton)


        self.verticalLayout.addLayout(self.horizontalLayout_3)

        self.submitButton = QPushButton(self.widget)
        self.submitButton.setObjectName(u"submitButton")
        self.submitButton.setCursor(QCursor(Qt.CursorShape.PointingHandCursor))
        self.submitButton.setMouseTracking(False)

        self.verticalLayout.addWidget(self.submitButton)


        self.verticalLayout_2.addLayout(self.verticalLayout)

        self.rightPanelSpacer = QSpacerItem(20, 208, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_2.addItem(self.rightPanelSpacer)

        self.scrollArea = QScrollArea(self.widget)
        self.scrollArea.setObjectName(u"scrollArea")
        self.scrollArea.setMinimumSize(QSize(251, 291))
        self.scrollArea.setMaximumSize(QSize(264, 291))
        self.scrollArea.setWidgetResizable(True)
        self.scrollAreaWidgetContents_2 = QWidget()
        self.scrollAreaWidgetContents_2.setObjectName(u"scrollAreaWidgetContents_2")
        self.scrollAreaWidgetContents_2.setGeometry(QRect(0, 0, 261, 289))
        self.scrollArea.setWidget(self.scrollAreaWidgetContents_2)

        self.verticalLayout_2.addWidget(self.scrollArea, 0, Qt.AlignmentFlag.AlignBottom)


        self.horizontalLayout.addWidget(self.frame)


        self.gridLayout.addLayout(self.horizontalLayout, 0, 0, 1, 1)

        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
#if QT_CONFIG(tooltip)
        self.frame.setToolTip("")
#endif // QT_CONFIG(tooltip)
        self.columnLabel.setText(QCoreApplication.translate("MainWindow", u"Columns:", None))
        self.rowLabel.setText(QCoreApplication.translate("MainWindow", u"Rows:", None))
#if QT_CONFIG(tooltip)
        self.columnTextEdit.setToolTip(QCoreApplication.translate("MainWindow", u"Map length", None))
#endif // QT_CONFIG(tooltip)
        self.columnTextEdit.setPlaceholderText(QCoreApplication.translate("MainWindow", u"0", None))
        self.columnUpButton.setText(QCoreApplication.translate("MainWindow", u"\u2227", None))
        self.columnDownButton.setText(QCoreApplication.translate("MainWindow", u"\u2228", None))
#if QT_CONFIG(tooltip)
        self.rowTextEdit.setToolTip(QCoreApplication.translate("MainWindow", u"Map height", None))
#endif // QT_CONFIG(tooltip)
        self.rowTextEdit.setPlaceholderText(QCoreApplication.translate("MainWindow", u"0", None))
        self.rowUpButton.setText(QCoreApplication.translate("MainWindow", u"\u2227", None))
        self.rowDownButton.setText(QCoreApplication.translate("MainWindow", u"\u2228", None))
#if QT_CONFIG(tooltip)
        self.submitButton.setToolTip(QCoreApplication.translate("MainWindow", u"<html><head/><body><p>Submit map dimentions</p></body></html>", None))
#endif // QT_CONFIG(tooltip)
        self.submitButton.setText(QCoreApplication.translate("MainWindow", u"Submit Dimentions", None))
    # retranslateUi

