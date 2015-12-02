/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glframework.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionLoad_Model;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    YGL::GLFramework *glFramework;
    QVBoxLayout *verticalLayout_2;
    QPushButton *SaveVertex;
    QPushButton *drawBorder_button;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBoxTrackball;
    QCheckBox *checkBoxLocator;
    QCheckBox *checkBoxGrid;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 720);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionLoad_Model = new QAction(MainWindow);
        actionLoad_Model->setObjectName(QStringLiteral("actionLoad_Model"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        glFramework = new YGL::GLFramework(centralWidget);
        glFramework->setObjectName(QStringLiteral("glFramework"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(glFramework->sizePolicy().hasHeightForWidth());
        glFramework->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(glFramework);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        SaveVertex = new QPushButton(centralWidget);
        SaveVertex->setObjectName(QStringLiteral("SaveVertex"));

        verticalLayout_2->addWidget(SaveVertex);

        drawBorder_button = new QPushButton(centralWidget);
        drawBorder_button->setObjectName(QStringLiteral("drawBorder_button"));

        verticalLayout_2->addWidget(drawBorder_button);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(20);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(200, 0));
        QFont font;
        font.setFamily(QStringLiteral("Monaco"));
        groupBox->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        checkBoxTrackball = new QCheckBox(groupBox);
        checkBoxTrackball->setObjectName(QStringLiteral("checkBoxTrackball"));
        checkBoxTrackball->setChecked(true);

        verticalLayout->addWidget(checkBoxTrackball);

        checkBoxLocator = new QCheckBox(groupBox);
        checkBoxLocator->setObjectName(QStringLiteral("checkBoxLocator"));
        checkBoxLocator->setChecked(false);

        verticalLayout->addWidget(checkBoxLocator);

        checkBoxGrid = new QCheckBox(groupBox);
        checkBoxGrid->setObjectName(QStringLiteral("checkBoxGrid"));
        checkBoxGrid->setChecked(false);

        verticalLayout->addWidget(checkBoxGrid);


        verticalLayout_2->addWidget(groupBox);


        horizontalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 30));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QT OpenGL Template", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionLoad_Model->setText(QApplication::translate("MainWindow", "Load Model...", 0));
        SaveVertex->setText(QApplication::translate("MainWindow", "SaveVertex", 0));
        drawBorder_button->setText(QApplication::translate("MainWindow", "DrawBorder", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Display", 0));
        checkBoxTrackball->setText(QApplication::translate("MainWindow", "Enable Trackball", 0));
        checkBoxLocator->setText(QApplication::translate("MainWindow", "Draw Locator", 0));
        checkBoxGrid->setText(QApplication::translate("MainWindow", "Draw Grid", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
