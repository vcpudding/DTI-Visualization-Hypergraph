/********************************************************************************
** Form generated from reading ui file 'FiberWidgetCtrl.ui'
**
** Created: Wed Dec 21 21:30:40 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FIBERWIDGETCTRL_H
#define UI_FIBERWIDGETCTRL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FiberWidgetCtrlClass
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *btnOperation;
    QPushButton *btnSelMode;
    QPushButton *btnClrMode;
    QPushButton *btnDispMode;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *FiberWidgetCtrlClass)
    {
    if (FiberWidgetCtrlClass->objectName().isEmpty())
        FiberWidgetCtrlClass->setObjectName(QString::fromUtf8("FiberWidgetCtrlClass"));
    FiberWidgetCtrlClass->resize(2000, 29);
    horizontalLayout = new QHBoxLayout(FiberWidgetCtrlClass);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setMargin(3);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    btnOperation = new QPushButton(FiberWidgetCtrlClass);
    btnOperation->setObjectName(QString::fromUtf8("btnOperation"));

    horizontalLayout->addWidget(btnOperation);

    btnSelMode = new QPushButton(FiberWidgetCtrlClass);
    btnSelMode->setObjectName(QString::fromUtf8("btnSelMode"));
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(btnSelMode->sizePolicy().hasHeightForWidth());
    btnSelMode->setSizePolicy(sizePolicy);
    btnSelMode->setMinimumSize(QSize(100, 0));

    horizontalLayout->addWidget(btnSelMode);

    btnClrMode = new QPushButton(FiberWidgetCtrlClass);
    btnClrMode->setObjectName(QString::fromUtf8("btnClrMode"));

    horizontalLayout->addWidget(btnClrMode);

    btnDispMode = new QPushButton(FiberWidgetCtrlClass);
    btnDispMode->setObjectName(QString::fromUtf8("btnDispMode"));

    horizontalLayout->addWidget(btnDispMode);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);


    retranslateUi(FiberWidgetCtrlClass);

    QMetaObject::connectSlotsByName(FiberWidgetCtrlClass);
    } // setupUi

    void retranslateUi(QWidget *FiberWidgetCtrlClass)
    {
    FiberWidgetCtrlClass->setWindowTitle(QApplication::translate("FiberWidgetCtrlClass", "FiberWidgetCtrl", 0, QApplication::UnicodeUTF8));
    btnOperation->setText(QApplication::translate("FiberWidgetCtrlClass", "Operation", 0, QApplication::UnicodeUTF8));
    btnSelMode->setText(QApplication::translate("FiberWidgetCtrlClass", "Selection Mode", 0, QApplication::UnicodeUTF8));
    btnClrMode->setText(QApplication::translate("FiberWidgetCtrlClass", "Color Mode", 0, QApplication::UnicodeUTF8));
    btnDispMode->setText(QApplication::translate("FiberWidgetCtrlClass", "Display Mode", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FiberWidgetCtrlClass);
    } // retranslateUi

};

namespace Ui {
    class FiberWidgetCtrlClass: public Ui_FiberWidgetCtrlClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIBERWIDGETCTRL_H
