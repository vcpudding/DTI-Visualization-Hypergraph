/********************************************************************************
** Form generated from reading ui file 'volumevis.ui'
**
** Created: Wed Dec 21 21:30:40 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VOLUMEVIS_H
#define UI_VOLUMEVIS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VolumeVisClass
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnParasagittal;
    QToolButton *btnCoronal;
    QToolButton *btnAxial;
    QLabel *label_2;
    QComboBox *viewCombo;
    QFrame *line;
    QCheckBox *sliceCheckBox;
    QFrame *line_2;
    QFrame *dtiFrame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *dispComboBox;
    QCheckBox *checkInterpolate;
    QSpacerItem *horizontalSpacer;
    QFrame *dispFrame;

    void setupUi(QWidget *VolumeVisClass)
    {
    if (VolumeVisClass->objectName().isEmpty())
        VolumeVisClass->setObjectName(QString::fromUtf8("VolumeVisClass"));
    VolumeVisClass->resize(1030, 758);
    verticalLayout = new QVBoxLayout(VolumeVisClass);
    verticalLayout->setSpacing(0);
    verticalLayout->setMargin(0);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    frame = new QFrame(VolumeVisClass);
    frame->setObjectName(QString::fromUtf8("frame"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy(sizePolicy);
    frame->setFrameShape(QFrame::Panel);
    frame->setFrameShadow(QFrame::Raised);
    horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(4);
    horizontalLayout->setMargin(3);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    btnParasagittal = new QToolButton(frame);
    btnParasagittal->setObjectName(QString::fromUtf8("btnParasagittal"));
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/P.ico")), QIcon::Normal, QIcon::Off);
    btnParasagittal->setIcon(icon);
    btnParasagittal->setCheckable(true);
    btnParasagittal->setChecked(true);

    horizontalLayout->addWidget(btnParasagittal);

    btnCoronal = new QToolButton(frame);
    btnCoronal->setObjectName(QString::fromUtf8("btnCoronal"));
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/C.ico")), QIcon::Normal, QIcon::Off);
    btnCoronal->setIcon(icon1);
    btnCoronal->setCheckable(true);
    btnCoronal->setChecked(true);

    horizontalLayout->addWidget(btnCoronal);

    btnAxial = new QToolButton(frame);
    btnAxial->setObjectName(QString::fromUtf8("btnAxial"));
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/A.ico")), QIcon::Normal, QIcon::Off);
    btnAxial->setIcon(icon2);
    btnAxial->setCheckable(true);
    btnAxial->setChecked(true);

    horizontalLayout->addWidget(btnAxial);

    label_2 = new QLabel(frame);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout->addWidget(label_2);

    viewCombo = new QComboBox(frame);
    viewCombo->setObjectName(QString::fromUtf8("viewCombo"));

    horizontalLayout->addWidget(viewCombo);

    line = new QFrame(frame);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    horizontalLayout->addWidget(line);

    sliceCheckBox = new QCheckBox(frame);
    sliceCheckBox->setObjectName(QString::fromUtf8("sliceCheckBox"));
    sliceCheckBox->setChecked(true);

    horizontalLayout->addWidget(sliceCheckBox);

    line_2 = new QFrame(frame);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::VLine);
    line_2->setFrameShadow(QFrame::Sunken);

    horizontalLayout->addWidget(line_2);

    dtiFrame = new QFrame(frame);
    dtiFrame->setObjectName(QString::fromUtf8("dtiFrame"));
    dtiFrame->setEnabled(true);
    dtiFrame->setFrameShape(QFrame::StyledPanel);
    dtiFrame->setFrameShadow(QFrame::Raised);
    horizontalLayout_2 = new QHBoxLayout(dtiFrame);
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setMargin(0);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label = new QLabel(dtiFrame);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout_2->addWidget(label);

    dispComboBox = new QComboBox(dtiFrame);
    dispComboBox->setObjectName(QString::fromUtf8("dispComboBox"));
    dispComboBox->setEnabled(true);
    QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(dispComboBox->sizePolicy().hasHeightForWidth());
    dispComboBox->setSizePolicy(sizePolicy1);

    horizontalLayout_2->addWidget(dispComboBox);

    checkInterpolate = new QCheckBox(dtiFrame);
    checkInterpolate->setObjectName(QString::fromUtf8("checkInterpolate"));

    horizontalLayout_2->addWidget(checkInterpolate);


    horizontalLayout->addWidget(dtiFrame);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);


    verticalLayout->addWidget(frame);

    dispFrame = new QFrame(VolumeVisClass);
    dispFrame->setObjectName(QString::fromUtf8("dispFrame"));
    dispFrame->setFrameShape(QFrame::StyledPanel);
    dispFrame->setFrameShadow(QFrame::Raised);

    verticalLayout->addWidget(dispFrame);


    retranslateUi(VolumeVisClass);

    QMetaObject::connectSlotsByName(VolumeVisClass);
    } // setupUi

    void retranslateUi(QWidget *VolumeVisClass)
    {
    VolumeVisClass->setWindowTitle(QApplication::translate("VolumeVisClass", "VolumeVis", 0, QApplication::UnicodeUTF8));
    btnParasagittal->setText(QApplication::translate("VolumeVisClass", "P", 0, QApplication::UnicodeUTF8));
    btnCoronal->setText(QApplication::translate("VolumeVisClass", "C", 0, QApplication::UnicodeUTF8));
    btnAxial->setText(QApplication::translate("VolumeVisClass", "A", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("VolumeVisClass", "Viewpoint:", 0, QApplication::UnicodeUTF8));
    viewCombo->clear();
    viewCombo->insertItems(0, QStringList()
     << QApplication::translate("VolumeVisClass", "Free", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("VolumeVisClass", "Axial", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("VolumeVisClass", "Coronal", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("VolumeVisClass", "Sagittal", 0, QApplication::UnicodeUTF8)
    );
    sliceCheckBox->setText(QApplication::translate("VolumeVisClass", "Show slice positions", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("VolumeVisClass", "Display:", 0, QApplication::UnicodeUTF8));
    dispComboBox->clear();
    dispComboBox->insertItems(0, QStringList()
     << QApplication::translate("VolumeVisClass", "Signal level", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("VolumeVisClass", "Color", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("VolumeVisClass", "FA", 0, QApplication::UnicodeUTF8)
    );
    checkInterpolate->setText(QApplication::translate("VolumeVisClass", "Show Interpolation", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(VolumeVisClass);
    } // retranslateUi

};

namespace Ui {
    class VolumeVisClass: public Ui_VolumeVisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMEVIS_H
