/********************************************************************************
** Form generated from reading ui file 'expresultvis.ui'
**
** Created: Wed Dec 21 21:30:40 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EXPRESULTVIS_H
#define UI_EXPRESULTVIS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtOpenGL/QGLWidget>

QT_BEGIN_NAMESPACE

class Ui_expResultVisClass
{
public:

    void setupUi(QGLWidget *expResultVisClass)
    {
    if (expResultVisClass->objectName().isEmpty())
        expResultVisClass->setObjectName(QString::fromUtf8("expResultVisClass"));
    expResultVisClass->resize(400, 300);

    retranslateUi(expResultVisClass);

    QMetaObject::connectSlotsByName(expResultVisClass);
    } // setupUi

    void retranslateUi(QGLWidget *expResultVisClass)
    {
    expResultVisClass->setWindowTitle(QApplication::translate("expResultVisClass", "expResultVis", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(expResultVisClass);
    } // retranslateUi

};

namespace Ui {
    class expResultVisClass: public Ui_expResultVisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPRESULTVIS_H
