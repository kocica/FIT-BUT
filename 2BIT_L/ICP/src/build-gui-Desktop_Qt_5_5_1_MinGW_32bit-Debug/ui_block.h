/********************************************************************************
** Form generated from reading UI file 'block.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOCK_H
#define UI_BLOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Block
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *Block)
    {
        if (Block->objectName().isEmpty())
            Block->setObjectName(QStringLiteral("Block"));
        Block->resize(74, 71);
        pushButton = new QPushButton(Block);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 0, 75, 71));

        retranslateUi(Block);

        QMetaObject::connectSlotsByName(Block);
    } // setupUi

    void retranslateUi(QDialog *Block)
    {
        Block->setWindowTitle(QApplication::translate("Block", "Dialog", 0));
        pushButton->setText(QApplication::translate("Block", "WTF", 0));
    } // retranslateUi

};

namespace Ui {
    class Block: public Ui_Block {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOCK_H
