/********************************************************************************
** Form generated from reading UI file 'crowdengine.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CROWDENGINE_H
#define UI_CROWDENGINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CrowdEngine
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CrowdEngine)
    {
        if (CrowdEngine->objectName().isEmpty())
            CrowdEngine->setObjectName(QStringLiteral("CrowdEngine"));
        CrowdEngine->resize(400, 300);
        menuBar = new QMenuBar(CrowdEngine);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        CrowdEngine->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CrowdEngine);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CrowdEngine->addToolBar(mainToolBar);
        centralWidget = new QWidget(CrowdEngine);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        CrowdEngine->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CrowdEngine);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CrowdEngine->setStatusBar(statusBar);

        retranslateUi(CrowdEngine);

        QMetaObject::connectSlotsByName(CrowdEngine);
    } // setupUi

    void retranslateUi(QMainWindow *CrowdEngine)
    {
        CrowdEngine->setWindowTitle(QApplication::translate("CrowdEngine", "CrowdEngine", 0));
    } // retranslateUi

};

namespace Ui {
    class CrowdEngine: public Ui_CrowdEngine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CROWDENGINE_H
