/********************************************************************************
** Form generated from reading ui file 'GraphWidgetCtrl.ui'
**
** Created: Wed Dec 21 21:30:40 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GRAPHWIDGETCTRL_H
#define UI_GRAPHWIDGETCTRL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphWidgetCtrlClass
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *btnMode;
    QPushButton *btnAssignCluster;
    QPushButton *btnShowEdge;
    QPushButton *btnDispMode;
    QPushButton *btnZoomIn;
    QPushButton *btnFit;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *GraphWidgetCtrlClass)
    {
    if (GraphWidgetCtrlClass->objectName().isEmpty())
        GraphWidgetCtrlClass->setObjectName(QString::fromUtf8("GraphWidgetCtrlClass"));
    GraphWidgetCtrlClass->resize(4000, 30);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(GraphWidgetCtrlClass->sizePolicy().hasHeightForWidth());
    GraphWidgetCtrlClass->setSizePolicy(sizePolicy);
    horizontalLayout = new QHBoxLayout(GraphWidgetCtrlClass);
    horizontalLayout->setSpacing(2);
    horizontalLayout->setMargin(2);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    btnMode = new QPushButton(GraphWidgetCtrlClass);
    btnMode->setObjectName(QString::fromUtf8("btnMode"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(btnMode->sizePolicy().hasHeightForWidth());
    btnMode->setSizePolicy(sizePolicy1);
    btnMode->setMinimumSize(QSize(100, 0));

    horizontalLayout->addWidget(btnMode);

    btnAssignCluster = new QPushButton(GraphWidgetCtrlClass);
    btnAssignCluster->setObjectName(QString::fromUtf8("btnAssignCluster"));
    btnAssignCluster->setCheckable(true);
    btnAssignCluster->setFlat(false);

    horizontalLayout->addWidget(btnAssignCluster);

    btnShowEdge = new QPushButton(GraphWidgetCtrlClass);
    btnShowEdge->setObjectName(QString::fromUtf8("btnShowEdge"));

    horizontalLayout->addWidget(btnShowEdge);

    btnDispMode = new QPushButton(GraphWidgetCtrlClass);
    btnDispMode->setObjectName(QString::fromUtf8("btnDispMode"));

    horizontalLayout->addWidget(btnDispMode);

    btnZoomIn = new QPushButton(GraphWidgetCtrlClass);
    btnZoomIn->setObjectName(QString::fromUtf8("btnZoomIn"));

    horizontalLayout->addWidget(btnZoomIn);

    btnFit = new QPushButton(GraphWidgetCtrlClass);
    btnFit->setObjectName(QString::fromUtf8("btnFit"));

    horizontalLayout->addWidget(btnFit);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);


    retranslateUi(GraphWidgetCtrlClass);

    QMetaObject::connectSlotsByName(GraphWidgetCtrlClass);
    } // setupUi

    void retranslateUi(QWidget *GraphWidgetCtrlClass)
    {
    GraphWidgetCtrlClass->setWindowTitle(QApplication::translate("GraphWidgetCtrlClass", "GraphWidgetCtrl", 0, QApplication::UnicodeUTF8));
    btnMode->setText(QApplication::translate("GraphWidgetCtrlClass", "Selection Mode", 0, QApplication::UnicodeUTF8));
    btnAssignCluster->setText(QApplication::translate("GraphWidgetCtrlClass", "Assign To...", 0, QApplication::UnicodeUTF8));
    btnShowEdge->setText(QApplication::translate("GraphWidgetCtrlClass", "Show Edges", 0, QApplication::UnicodeUTF8));
    btnDispMode->setText(QApplication::translate("GraphWidgetCtrlClass", "Display Mode", 0, QApplication::UnicodeUTF8));
    btnZoomIn->setText(QApplication::translate("GraphWidgetCtrlClass", "Zoom In", 0, QApplication::UnicodeUTF8));
    btnFit->setText(QApplication::translate("GraphWidgetCtrlClass", "Fit Window", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(GraphWidgetCtrlClass);
    } // retranslateUi

};

namespace Ui {
    class GraphWidgetCtrlClass: public Ui_GraphWidgetCtrlClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHWIDGETCTRL_H
