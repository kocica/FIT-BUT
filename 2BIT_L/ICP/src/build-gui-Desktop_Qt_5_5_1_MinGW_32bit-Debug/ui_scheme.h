/********************************************************************************
** Form generated from reading UI file 'scheme.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEME_H
#define UI_SCHEME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Scheme
{
public:

    void setupUi(QDialog *Scheme)
    {
        if (Scheme->objectName().isEmpty())
            Scheme->setObjectName(QStringLiteral("Scheme"));
        Scheme->resize(920, 443);

        retranslateUi(Scheme);

        QMetaObject::connectSlotsByName(Scheme);
    } // setupUi

    void retranslateUi(QDialog *Scheme)
    {
        Scheme->setWindowTitle(QApplication::translate("Scheme", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class Scheme: public Ui_Scheme {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEME_H
