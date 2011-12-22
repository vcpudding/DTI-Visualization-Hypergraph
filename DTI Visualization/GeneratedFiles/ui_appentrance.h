/********************************************************************************
** Form generated from reading ui file 'appentrance.ui'
**
** Created: Wed Dec 21 21:30:41 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_APPENTRANCE_H
#define UI_APPENTRANCE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AppEntranceClass
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *btnDicom;
    QPushButton *btnTract;

    void setupUi(QWidget *AppEntranceClass)
    {
    if (AppEntranceClass->objectName().isEmpty())
        AppEntranceClass->setObjectName(QString::fromUtf8("AppEntranceClass"));
    AppEntranceClass->resize(165, 87);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(AppEntranceClass->sizePolicy().hasHeightForWidth());
    AppEntranceClass->setSizePolicy(sizePolicy);
    AppEntranceClass->setContextMenuPolicy(Qt::NoContextMenu);
    verticalLayout = new QVBoxLayout(AppEntranceClass);
    verticalLayout->setSpacing(9);
    verticalLayout->setMargin(15);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
    btnDicom = new QPushButton(AppEntranceClass);
    btnDicom->setObjectName(QString::fromUtf8("btnDicom"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(btnDicom->sizePolicy().hasHeightForWidth());
    btnDicom->setSizePolicy(sizePolicy1);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Spotlight Blue Button.png")), QIcon::Normal, QIcon::Off);
    btnDicom->setIcon(icon);

    verticalLayout->addWidget(btnDicom);

    btnTract = new QPushButton(AppEntranceClass);
    btnTract->setObjectName(QString::fromUtf8("btnTract"));
    sizePolicy1.setHeightForWidth(btnTract->sizePolicy().hasHeightForWidth());
    btnTract->setSizePolicy(sizePolicy1);
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Web Browser.png")), QIcon::Normal, QIcon::Off);
    btnTract->setIcon(icon1);

    verticalLayout->addWidget(btnTract);


    retranslateUi(AppEntranceClass);

    QMetaObject::connectSlotsByName(AppEntranceClass);
    } // setupUi

    void retranslateUi(QWidget *AppEntranceClass)
    {
    AppEntranceClass->setWindowTitle(QApplication::translate("AppEntranceClass", "DTI Visualization Platform", 0, QApplication::UnicodeUTF8));
    btnDicom->setText(QApplication::translate("AppEntranceClass", "Dicom Explorer", 0, QApplication::UnicodeUTF8));
    btnTract->setText(QApplication::translate("AppEntranceClass", "Tractography Explorer", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(AppEntranceClass);
    } // retranslateUi

};

namespace Ui {
    class AppEntranceClass: public Ui_AppEntranceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPENTRANCE_H
