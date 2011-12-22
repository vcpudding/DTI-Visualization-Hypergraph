/********************************************************************************
** Form generated from reading ui file 'clustereditor.ui'
**
** Created: Wed Dec 21 21:30:41 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CLUSTEREDITOR_H
#define UI_CLUSTEREDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClusterEditorClass
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineFiberFile;
    QPushButton *btnFiberFile;
    QLabel *label_2;
    QLineEdit *lineClusterFile;
    QPushButton *btnClusterFile;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QListWidget *listMajorBundles;
    QPushButton *btnAssign;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QTableWidget *tableResult;
    QPushButton *btnSave;
    QSpacerItem *horizontalSpacer_2;
    QFrame *dispFrame;

    void setupUi(QWidget *ClusterEditorClass)
    {
    if (ClusterEditorClass->objectName().isEmpty())
        ClusterEditorClass->setObjectName(QString::fromUtf8("ClusterEditorClass"));
    ClusterEditorClass->resize(796, 674);
    horizontalLayout = new QHBoxLayout(ClusterEditorClass);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setMargin(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    frame = new QFrame(ClusterEditorClass);
    frame->setObjectName(QString::fromUtf8("frame"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy(sizePolicy);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    verticalLayout = new QVBoxLayout(frame);
    verticalLayout->setSpacing(6);
    verticalLayout->setMargin(11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    groupBox = new QGroupBox(frame);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy1);
    gridLayout = new QGridLayout(groupBox);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    lineFiberFile = new QLineEdit(groupBox);
    lineFiberFile->setObjectName(QString::fromUtf8("lineFiberFile"));

    gridLayout->addWidget(lineFiberFile, 0, 1, 1, 1);

    btnFiberFile = new QPushButton(groupBox);
    btnFiberFile->setObjectName(QString::fromUtf8("btnFiberFile"));
    btnFiberFile->setMaximumSize(QSize(40, 16777215));

    gridLayout->addWidget(btnFiberFile, 0, 2, 1, 1);

    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    lineClusterFile = new QLineEdit(groupBox);
    lineClusterFile->setObjectName(QString::fromUtf8("lineClusterFile"));

    gridLayout->addWidget(lineClusterFile, 1, 1, 1, 1);

    btnClusterFile = new QPushButton(groupBox);
    btnClusterFile->setObjectName(QString::fromUtf8("btnClusterFile"));
    btnClusterFile->setMaximumSize(QSize(40, 16777215));

    gridLayout->addWidget(btnClusterFile, 1, 2, 1, 1);


    verticalLayout->addWidget(groupBox);

    groupBox_2 = new QGroupBox(frame);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
    groupBox_2->setSizePolicy(sizePolicy2);
    gridLayout_2 = new QGridLayout(groupBox_2);
    gridLayout_2->setSpacing(6);
    gridLayout_2->setMargin(11);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    listMajorBundles = new QListWidget(groupBox_2);
    listMajorBundles->setObjectName(QString::fromUtf8("listMajorBundles"));
    QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(listMajorBundles->sizePolicy().hasHeightForWidth());
    listMajorBundles->setSizePolicy(sizePolicy3);
    listMajorBundles->setMaximumSize(QSize(16777215, 300));

    gridLayout_2->addWidget(listMajorBundles, 0, 0, 1, 2);

    btnAssign = new QPushButton(groupBox_2);
    btnAssign->setObjectName(QString::fromUtf8("btnAssign"));

    gridLayout_2->addWidget(btnAssign, 1, 1, 1, 1);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);


    verticalLayout->addWidget(groupBox_2);

    groupBox_3 = new QGroupBox(frame);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    gridLayout_3 = new QGridLayout(groupBox_3);
    gridLayout_3->setSpacing(6);
    gridLayout_3->setMargin(11);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    tableResult = new QTableWidget(groupBox_3);
    tableResult->setObjectName(QString::fromUtf8("tableResult"));

    gridLayout_3->addWidget(tableResult, 0, 0, 1, 2);

    btnSave = new QPushButton(groupBox_3);
    btnSave->setObjectName(QString::fromUtf8("btnSave"));

    gridLayout_3->addWidget(btnSave, 1, 1, 1, 1);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout_3->addItem(horizontalSpacer_2, 1, 0, 1, 1);


    verticalLayout->addWidget(groupBox_3);


    horizontalLayout->addWidget(frame);

    dispFrame = new QFrame(ClusterEditorClass);
    dispFrame->setObjectName(QString::fromUtf8("dispFrame"));
    dispFrame->setFrameShape(QFrame::StyledPanel);
    dispFrame->setFrameShadow(QFrame::Raised);

    horizontalLayout->addWidget(dispFrame);


    retranslateUi(ClusterEditorClass);

    QMetaObject::connectSlotsByName(ClusterEditorClass);
    } // setupUi

    void retranslateUi(QWidget *ClusterEditorClass)
    {
    ClusterEditorClass->setWindowTitle(QApplication::translate("ClusterEditorClass", "ClusterEditor", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("ClusterEditorClass", "Files", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("ClusterEditorClass", "Fiber file:", 0, QApplication::UnicodeUTF8));
    btnFiberFile->setText(QApplication::translate("ClusterEditorClass", "Open", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("ClusterEditorClass", "Cluster file:", 0, QApplication::UnicodeUTF8));
    btnClusterFile->setText(QApplication::translate("ClusterEditorClass", "Open", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("ClusterEditorClass", "Major Bundles", 0, QApplication::UnicodeUTF8));
    btnAssign->setText(QApplication::translate("ClusterEditorClass", "Assign", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("ClusterEditorClass", "Results", 0, QApplication::UnicodeUTF8));
    btnSave->setText(QApplication::translate("ClusterEditorClass", "Save", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ClusterEditorClass);
    } // retranslateUi

};

namespace Ui {
    class ClusterEditorClass: public Ui_ClusterEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLUSTEREDITOR_H
