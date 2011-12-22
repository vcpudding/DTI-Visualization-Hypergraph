/********************************************************************************
** Form generated from reading ui file 'compoundwin.ui'
**
** Created: Wed Dec 21 21:30:41 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_COMPOUNDWIN_H
#define UI_COMPOUNDWIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CompoundWinClass
{
public:
    QGridLayout *gridLayout;
    QFrame *fibFrame;
    QFrame *graphFrame;

    void setupUi(QWidget *CompoundWinClass)
    {
    if (CompoundWinClass->objectName().isEmpty())
        CompoundWinClass->setObjectName(QString::fromUtf8("CompoundWinClass"));
    CompoundWinClass->resize(592, 448);
    gridLayout = new QGridLayout(CompoundWinClass);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    fibFrame = new QFrame(CompoundWinClass);
    fibFrame->setObjectName(QString::fromUtf8("fibFrame"));
    fibFrame->setFrameShape(QFrame::StyledPanel);
    fibFrame->setFrameShadow(QFrame::Raised);

    gridLayout->addWidget(fibFrame, 0, 0, 1, 1);

    graphFrame = new QFrame(CompoundWinClass);
    graphFrame->setObjectName(QString::fromUtf8("graphFrame"));
    graphFrame->setFrameShape(QFrame::StyledPanel);
    graphFrame->setFrameShadow(QFrame::Raised);

    gridLayout->addWidget(graphFrame, 0, 1, 1, 1);


    retranslateUi(CompoundWinClass);

    QMetaObject::connectSlotsByName(CompoundWinClass);
    } // setupUi

    void retranslateUi(QWidget *CompoundWinClass)
    {
    CompoundWinClass->setWindowTitle(QApplication::translate("CompoundWinClass", "CompoundWin", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CompoundWinClass);
    } // retranslateUi

};

namespace Ui {
    class CompoundWinClass: public Ui_CompoundWinClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPOUNDWIN_H
