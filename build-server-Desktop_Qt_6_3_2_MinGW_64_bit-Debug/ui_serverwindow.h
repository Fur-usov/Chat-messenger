/********************************************************************************
** Form generated from reading UI file 'serverwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWINDOW_H
#define UI_SERVERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNetwork;
    QAction *actionKey;
    QAction *actionEnable;
    QAction *actionDisable;
    QAction *actionSave_as_XML;
    QAction *actionExit;
    QWidget *centralwidget;
    QTextBrowser *textBrowser;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuSettings;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(481, 587);
        actionNetwork = new QAction(MainWindow);
        actionNetwork->setObjectName(QString::fromUtf8("actionNetwork"));
        actionKey = new QAction(MainWindow);
        actionKey->setObjectName(QString::fromUtf8("actionKey"));
        actionEnable = new QAction(MainWindow);
        actionEnable->setObjectName(QString::fromUtf8("actionEnable"));
        actionDisable = new QAction(MainWindow);
        actionDisable->setObjectName(QString::fromUtf8("actionDisable"));
        actionSave_as_XML = new QAction(MainWindow);
        actionSave_as_XML->setObjectName(QString::fromUtf8("actionSave_as_XML"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 50, 461, 491));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 481, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 481, 24));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menuSettings->addAction(actionNetwork);
        menuSettings->addAction(actionKey);
        menuFile->addAction(actionEnable);
        menuFile->addAction(actionDisable);
        menuFile->addAction(actionSave_as_XML);
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNetwork->setText(QCoreApplication::translate("MainWindow", "Network", nullptr));
        actionKey->setText(QCoreApplication::translate("MainWindow", "Key", nullptr));
        actionEnable->setText(QCoreApplication::translate("MainWindow", "Enable", nullptr));
        actionDisable->setText(QCoreApplication::translate("MainWindow", "Disable", nullptr));
        actionSave_as_XML->setText(QCoreApplication::translate("MainWindow", "Save as XML", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Information about users", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWINDOW_H
