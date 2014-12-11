/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *m_centralwidget;
    QGridLayout *s_mainGridLayout;
    QFrame *frame_2;
    QFrame *frame;
    QPushButton *m_colour;
    QPushButton *m_reset;
    QComboBox *m_vboSelection;
    QSlider *m_normalSize;
    QCheckBox *m_normals;
    QCheckBox *m_wireframe;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1321, 675);
        MainWindow->setBaseSize(QSize(0, 0));
        m_centralwidget = new QWidget(MainWindow);
        m_centralwidget->setObjectName(QStringLiteral("m_centralwidget"));
        s_mainGridLayout = new QGridLayout(m_centralwidget);
        s_mainGridLayout->setObjectName(QStringLiteral("s_mainGridLayout"));
        frame_2 = new QFrame(m_centralwidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);

        s_mainGridLayout->addWidget(frame_2, 1, 8, 1, 1);

        frame = new QFrame(m_centralwidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        m_colour = new QPushButton(frame);
        m_colour->setObjectName(QStringLiteral("m_colour"));
        m_colour->setGeometry(QRect(291, 0, 85, 32));

        s_mainGridLayout->addWidget(frame, 4, 6, 1, 2);

        m_reset = new QPushButton(m_centralwidget);
        m_reset->setObjectName(QStringLiteral("m_reset"));

        s_mainGridLayout->addWidget(m_reset, 4, 1, 1, 1);

        m_vboSelection = new QComboBox(m_centralwidget);
        m_vboSelection->setObjectName(QStringLiteral("m_vboSelection"));

        s_mainGridLayout->addWidget(m_vboSelection, 4, 0, 1, 1);

        m_normalSize = new QSlider(m_centralwidget);
        m_normalSize->setObjectName(QStringLiteral("m_normalSize"));
        m_normalSize->setLayoutDirection(Qt::RightToLeft);
        m_normalSize->setMinimum(1);
        m_normalSize->setMaximum(20);
        m_normalSize->setPageStep(1);
        m_normalSize->setValue(6);
        m_normalSize->setSliderPosition(6);
        m_normalSize->setOrientation(Qt::Horizontal);
        m_normalSize->setInvertedAppearance(true);
        m_normalSize->setTickPosition(QSlider::TicksBothSides);

        s_mainGridLayout->addWidget(m_normalSize, 4, 4, 1, 1);

        m_normals = new QCheckBox(m_centralwidget);
        m_normals->setObjectName(QStringLiteral("m_normals"));

        s_mainGridLayout->addWidget(m_normals, 4, 3, 1, 1);

        m_wireframe = new QCheckBox(m_centralwidget);
        m_wireframe->setObjectName(QStringLiteral("m_wireframe"));

        s_mainGridLayout->addWidget(m_wireframe, 0, 6, 1, 1);

        MainWindow->setCentralWidget(m_centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1321, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(m_vboSelection, m_reset);
        QWidget::setTabOrder(m_reset, m_normals);

        retranslateUi(MainWindow);

        m_vboSelection->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Colossal WIN!", 0));
        m_colour->setText(QApplication::translate("MainWindow", "Colour", 0));
        m_reset->setText(QApplication::translate("MainWindow", "reset", 0));
        m_vboSelection->clear();
        m_vboSelection->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "sphere", 0)
         << QApplication::translate("MainWindow", "cylinder", 0)
         << QApplication::translate("MainWindow", "cone", 0)
         << QApplication::translate("MainWindow", "disk", 0)
         << QApplication::translate("MainWindow", "plane", 0)
         << QApplication::translate("MainWindow", "torus", 0)
         << QApplication::translate("MainWindow", "teapot", 0)
         << QApplication::translate("MainWindow", "octahedron", 0)
         << QApplication::translate("MainWindow", "dodecahedron", 0)
         << QApplication::translate("MainWindow", "icosahedron", 0)
         << QApplication::translate("MainWindow", "tetrahedron", 0)
         << QApplication::translate("MainWindow", "football", 0)
         << QApplication::translate("MainWindow", "cube", 0)
         << QApplication::translate("MainWindow", "troll", 0)
         << QApplication::translate("MainWindow", "buddah", 0)
         << QApplication::translate("MainWindow", "dragon", 0)
         << QApplication::translate("MainWindow", "bunny", 0)
        );
        m_vboSelection->setCurrentText(QApplication::translate("MainWindow", "plane", 0));
        m_normals->setText(QApplication::translate("MainWindow", "normals   size", 0));
        m_wireframe->setText(QApplication::translate("MainWindow", "wireframe", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
