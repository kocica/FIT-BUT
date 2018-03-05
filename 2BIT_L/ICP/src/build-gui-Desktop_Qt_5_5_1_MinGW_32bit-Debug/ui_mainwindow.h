/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QGroupBox *groupBox;
    QPushButton *AddBlock;
    QPushButton *SubBlock;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_4;
    QPushButton *Scheme;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1209, 680);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 542, 281, 81));
        pushButton->setMinimumSize(QSize(91, 81));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(0, 460, 141, 81));
        pushButton_2->setMinimumSize(QSize(91, 81));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(140, 460, 141, 81));
        pushButton_3->setMinimumSize(QSize(91, 81));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 281, 181));
        AddBlock = new QPushButton(groupBox);
        AddBlock->setObjectName(QStringLiteral("AddBlock"));
        AddBlock->setGeometry(QRect(0, 20, 71, 71));
        QFont font;
        font.setPointSize(21);
        AddBlock->setFont(font);
        AddBlock->setAutoFillBackground(false);
        SubBlock = new QPushButton(groupBox);
        SubBlock->setObjectName(QStringLiteral("SubBlock"));
        SubBlock->setGeometry(QRect(70, 20, 71, 71));
        SubBlock->setFont(font);
        pushButton_6 = new QPushButton(groupBox);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(140, 20, 71, 71));
        pushButton_6->setFont(font);
        pushButton_7 = new QPushButton(groupBox);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(210, 20, 71, 71));
        pushButton_7->setFont(font);
        pushButton_8 = new QPushButton(groupBox);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(0, 90, 71, 71));
        pushButton_8->setFont(font);
        pushButton_9 = new QPushButton(groupBox);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(140, 90, 71, 71));
        pushButton_9->setFont(font);
        pushButton_10 = new QPushButton(groupBox);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(210, 90, 71, 71));
        pushButton_10->setFont(font);
        pushButton_11 = new QPushButton(groupBox);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(70, 90, 71, 71));
        pushButton_11->setFont(font);
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(0, 380, 281, 81));
        pushButton_4->setMinimumSize(QSize(91, 81));
        Scheme = new QPushButton(centralWidget);
        Scheme->setObjectName(QStringLiteral("Scheme"));
        Scheme->setGeometry(QRect(-10, -10, 1221, 631));
        Scheme->setStyleSheet(QStringLiteral("background-color: rgb(240, 240, 240)"));
        Scheme->setAutoDefault(false);
        Scheme->setFlat(true);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 180, 281, 201));
        MainWindow->setCentralWidget(centralWidget);
        Scheme->raise();
        pushButton->raise();
        pushButton_2->raise();
        pushButton_3->raise();
        groupBox->raise();
        pushButton_4->raise();
        label->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1209, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        Scheme->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Run", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Step back", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Step forward", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Blocks", 0));
        AddBlock->setText(QApplication::translate("MainWindow", "+", 0));
        SubBlock->setText(QApplication::translate("MainWindow", "-", 0));
        pushButton_6->setText(QApplication::translate("MainWindow", "*", 0));
        pushButton_7->setText(QApplication::translate("MainWindow", "/", 0));
        pushButton_8->setText(QApplication::translate("MainWindow", "&&", 0));
        pushButton_9->setText(QApplication::translate("MainWindow", "^", 0));
        pushButton_10->setText(QApplication::translate("MainWindow", "!", 0));
        pushButton_11->setText(QApplication::translate("MainWindow", "|", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "?", 0));
        Scheme->setText(QString());
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
