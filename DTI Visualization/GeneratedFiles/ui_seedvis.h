/********************************************************************************
** Form generated from reading ui file 'seedvis.ui'
**
** Created: Wed Dec 21 21:30:40 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SEEDVIS_H
#define UI_SEEDVIS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeedVisClass
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *vpComboBox;
    QLabel *label_2;
    QComboBox *dispComboBox;
    QFrame *line;
    QPushButton *btnZoomIn;
    QPushButton *btnZoomOut;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnTrack;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *seedTable;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnLoad;
    QPushButton *btnSave;
    QFrame *dispFrame;

    void setupUi(QWidget *SeedVisClass)
    {
    if (SeedVisClass->objectName().isEmpty())
        SeedVisClass->setObjectName(QString::fromUtf8("SeedVisClass"));
    SeedVisClass->resize(796, 672);
    verticalLayout = new QVBoxLayout(SeedVisClass);
    verticalLayout->setSpacing(3);
    verticalLayout->setMargin(0);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    frame_4 = new QFrame(SeedVisClass);
    frame_4->setObjectName(QString::fromUtf8("frame_4"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
    frame_4->setSizePolicy(sizePolicy);
    frame_4->setFrameShape(QFrame::StyledPanel);
    frame_4->setFrameShadow(QFrame::Raised);
    horizontalLayout_2 = new QHBoxLayout(frame_4);
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setMargin(3);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label = new QLabel(frame_4);
    label->setObjectName(QString::fromUtf8("label"));
    QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy1);
    label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    horizontalLayout_2->addWidget(label);

    vpComboBox = new QComboBox(frame_4);
    vpComboBox->setObjectName(QString::fromUtf8("vpComboBox"));
    QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(vpComboBox->sizePolicy().hasHeightForWidth());
    vpComboBox->setSizePolicy(sizePolicy2);
    vpComboBox->setMinimumSize(QSize(0, 0));

    horizontalLayout_2->addWidget(vpComboBox);

    label_2 = new QLabel(frame_4);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy1);
    label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    horizontalLayout_2->addWidget(label_2);

    dispComboBox = new QComboBox(frame_4);
    dispComboBox->setObjectName(QString::fromUtf8("dispComboBox"));
    sizePolicy2.setHeightForWidth(dispComboBox->sizePolicy().hasHeightForWidth());
    dispComboBox->setSizePolicy(sizePolicy2);

    horizontalLayout_2->addWidget(dispComboBox);

    line = new QFrame(frame_4);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    horizontalLayout_2->addWidget(line);

    btnZoomIn = new QPushButton(frame_4);
    btnZoomIn->setObjectName(QString::fromUtf8("btnZoomIn"));

    horizontalLayout_2->addWidget(btnZoomIn);

    btnZoomOut = new QPushButton(frame_4);
    btnZoomOut->setObjectName(QString::fromUtf8("btnZoomOut"));

    horizontalLayout_2->addWidget(btnZoomOut);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer_2);

    btnTrack = new QPushButton(frame_4);
    btnTrack->setObjectName(QString::fromUtf8("btnTrack"));
    sizePolicy2.setHeightForWidth(btnTrack->sizePolicy().hasHeightForWidth());
    btnTrack->setSizePolicy(sizePolicy2);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Play.png")), QIcon::Normal, QIcon::Off);
    btnTrack->setIcon(icon);

    horizontalLayout_2->addWidget(btnTrack);


    verticalLayout->addWidget(frame_4);

    frame_3 = new QFrame(SeedVisClass);
    frame_3->setObjectName(QString::fromUtf8("frame_3"));
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
    frame_3->setSizePolicy(sizePolicy3);
    frame_3->setFrameShape(QFrame::StyledPanel);
    frame_3->setFrameShadow(QFrame::Raised);
    verticalLayout_2 = new QVBoxLayout(frame_3);
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setMargin(3);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    seedTable = new QTableWidget(frame_3);
    if (seedTable->columnCount() < 4)
        seedTable->setColumnCount(4);
    QTableWidgetItem *__colItem = new QTableWidgetItem();
    seedTable->setHorizontalHeaderItem(0, __colItem);
    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    seedTable->setHorizontalHeaderItem(1, __colItem1);
    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    seedTable->setHorizontalHeaderItem(2, __colItem2);
    QTableWidgetItem *__colItem3 = new QTableWidgetItem();
    seedTable->setHorizontalHeaderItem(3, __colItem3);
    if (seedTable->rowCount() < 1)
        seedTable->setRowCount(1);
    seedTable->setObjectName(QString::fromUtf8("seedTable"));
    seedTable->setMaximumSize(QSize(16777215, 86));
    seedTable->setRowCount(1);

    verticalLayout_2->addWidget(seedTable);

    frame = new QFrame(frame_3);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setMargin(11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(-1, 0, -1, 0);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    btnAdd = new QPushButton(frame);
    btnAdd->setObjectName(QString::fromUtf8("btnAdd"));

    horizontalLayout->addWidget(btnAdd);

    btnRemove = new QPushButton(frame);
    btnRemove->setObjectName(QString::fromUtf8("btnRemove"));

    horizontalLayout->addWidget(btnRemove);

    btnLoad = new QPushButton(frame);
    btnLoad->setObjectName(QString::fromUtf8("btnLoad"));

    horizontalLayout->addWidget(btnLoad);

    btnSave = new QPushButton(frame);
    btnSave->setObjectName(QString::fromUtf8("btnSave"));

    horizontalLayout->addWidget(btnSave);


    verticalLayout_2->addWidget(frame);


    verticalLayout->addWidget(frame_3);

    dispFrame = new QFrame(SeedVisClass);
    dispFrame->setObjectName(QString::fromUtf8("dispFrame"));
    dispFrame->setFrameShape(QFrame::StyledPanel);
    dispFrame->setFrameShadow(QFrame::Raised);

    verticalLayout->addWidget(dispFrame);


    retranslateUi(SeedVisClass);
    QObject::connect(seedTable, SIGNAL(activated(QModelIndex)), seedTable, SLOT(resizeColumnsToContents()));

    QMetaObject::connectSlotsByName(SeedVisClass);
    } // setupUi

    void retranslateUi(QWidget *SeedVisClass)
    {
    SeedVisClass->setWindowTitle(QApplication::translate("SeedVisClass", "SeedVis", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("SeedVisClass", "View:", 0, QApplication::UnicodeUTF8));
    vpComboBox->clear();
    vpComboBox->insertItems(0, QStringList()
     << QApplication::translate("SeedVisClass", "Axial", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("SeedVisClass", "Coronal", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("SeedVisClass", "Sagittal", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("SeedVisClass", "Display:", 0, QApplication::UnicodeUTF8));
    dispComboBox->clear();
    dispComboBox->insertItems(0, QStringList()
     << QApplication::translate("SeedVisClass", "B0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("SeedVisClass", "Color", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("SeedVisClass", "FA", 0, QApplication::UnicodeUTF8)
    );
    btnZoomIn->setText(QApplication::translate("SeedVisClass", "Zoom In", 0, QApplication::UnicodeUTF8));
    btnZoomOut->setText(QApplication::translate("SeedVisClass", "Zoom Out", 0, QApplication::UnicodeUTF8));
    btnTrack->setText(QApplication::translate("SeedVisClass", "Start fiber tracking", 0, QApplication::UnicodeUTF8));
    seedTable->horizontalHeaderItem(1)->setText(QApplication::translate("SeedVisClass", "Name", 0, QApplication::UnicodeUTF8));
    seedTable->horizontalHeaderItem(2)->setText(QApplication::translate("SeedVisClass", "Type", 0, QApplication::UnicodeUTF8));
    seedTable->horizontalHeaderItem(3)->setText(QApplication::translate("SeedVisClass", "Color", 0, QApplication::UnicodeUTF8));
    btnAdd->setText(QApplication::translate("SeedVisClass", "Add", 0, QApplication::UnicodeUTF8));
    btnRemove->setText(QApplication::translate("SeedVisClass", "Remove", 0, QApplication::UnicodeUTF8));
    btnLoad->setText(QApplication::translate("SeedVisClass", "Load", 0, QApplication::UnicodeUTF8));
    btnSave->setText(QApplication::translate("SeedVisClass", "Save", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(SeedVisClass);
    } // retranslateUi

};

namespace Ui {
    class SeedVisClass: public Ui_SeedVisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEEDVIS_H
