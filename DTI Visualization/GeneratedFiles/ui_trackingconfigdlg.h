/********************************************************************************
** Form generated from reading ui file 'trackingconfigdlg.ui'
**
** Created: Wed Dec 21 21:30:40 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRACKINGCONFIGDLG_H
#define UI_TRACKINGCONFIGDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_TrackingConfigDlgClass
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *faSpin;
    QLabel *label_2;
    QSpinBox *angleSpin;
    QLabel *label_3;
    QDoubleSpinBox *stepSpin;
    QLabel *label_4;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpinBox *minLengthSpin;
    QLabel *label_5;
    QSpinBox *maxLengthSpin;
    QLabel *label_6;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *isFiberCount;
    QLabel *label_7;
    QSpinBox *nFiberSpin;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TrackingConfigDlgClass)
    {
    if (TrackingConfigDlgClass->objectName().isEmpty())
        TrackingConfigDlgClass->setObjectName(QString::fromUtf8("TrackingConfigDlgClass"));
    TrackingConfigDlgClass->resize(374, 177);
    gridLayout = new QGridLayout(TrackingConfigDlgClass);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(TrackingConfigDlgClass);
    label->setObjectName(QString::fromUtf8("label"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy);
    label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label, 0, 0, 1, 1);

    faSpin = new QDoubleSpinBox(TrackingConfigDlgClass);
    faSpin->setObjectName(QString::fromUtf8("faSpin"));
    faSpin->setMaximum(1);
    faSpin->setSingleStep(0.05);
    faSpin->setValue(0.2);

    gridLayout->addWidget(faSpin, 0, 1, 1, 1);

    label_2 = new QLabel(TrackingConfigDlgClass);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    angleSpin = new QSpinBox(TrackingConfigDlgClass);
    angleSpin->setObjectName(QString::fromUtf8("angleSpin"));
    angleSpin->setMaximum(90);
    angleSpin->setValue(30);

    gridLayout->addWidget(angleSpin, 1, 1, 1, 1);

    label_3 = new QLabel(TrackingConfigDlgClass);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_3, 2, 0, 1, 1);

    stepSpin = new QDoubleSpinBox(TrackingConfigDlgClass);
    stepSpin->setObjectName(QString::fromUtf8("stepSpin"));
    stepSpin->setMinimum(0.1);
    stepSpin->setMaximum(100);
    stepSpin->setSingleStep(1);
    stepSpin->setValue(0.2);

    gridLayout->addWidget(stepSpin, 2, 1, 1, 1);

    label_4 = new QLabel(TrackingConfigDlgClass);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_4, 3, 0, 1, 1);

    frame = new QFrame(TrackingConfigDlgClass);
    frame->setObjectName(QString::fromUtf8("frame"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy(sizePolicy1);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(3);
    horizontalLayout->setMargin(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    minLengthSpin = new QSpinBox(frame);
    minLengthSpin->setObjectName(QString::fromUtf8("minLengthSpin"));
    minLengthSpin->setValue(20);

    horizontalLayout->addWidget(minLengthSpin);

    label_5 = new QLabel(frame);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
    label_5->setSizePolicy(sizePolicy2);

    horizontalLayout->addWidget(label_5);

    maxLengthSpin = new QSpinBox(frame);
    maxLengthSpin->setObjectName(QString::fromUtf8("maxLengthSpin"));
    maxLengthSpin->setMaximum(20000);
    maxLengthSpin->setValue(200);

    horizontalLayout->addWidget(maxLengthSpin);


    gridLayout->addWidget(frame, 3, 1, 1, 1);

    label_6 = new QLabel(TrackingConfigDlgClass);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_6, 4, 0, 1, 1);

    frame_2 = new QFrame(TrackingConfigDlgClass);
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
    frame_2->setSizePolicy(sizePolicy3);
    frame_2->setFrameShape(QFrame::StyledPanel);
    frame_2->setFrameShadow(QFrame::Raised);
    horizontalLayout_2 = new QHBoxLayout(frame_2);
    horizontalLayout_2->setSpacing(3);
    horizontalLayout_2->setMargin(0);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    isFiberCount = new QComboBox(frame_2);
    isFiberCount->setObjectName(QString::fromUtf8("isFiberCount"));

    horizontalLayout_2->addWidget(isFiberCount);

    label_7 = new QLabel(frame_2);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    sizePolicy2.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
    label_7->setSizePolicy(sizePolicy2);

    horizontalLayout_2->addWidget(label_7);

    nFiberSpin = new QSpinBox(frame_2);
    nFiberSpin->setObjectName(QString::fromUtf8("nFiberSpin"));
    nFiberSpin->setMinimum(1);
    nFiberSpin->setMaximum(99999);
    nFiberSpin->setSingleStep(100);
    nFiberSpin->setValue(200);

    horizontalLayout_2->addWidget(nFiberSpin);


    gridLayout->addWidget(frame_2, 4, 1, 1, 1);

    buttonBox = new QDialogButtonBox(TrackingConfigDlgClass);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    buttonBox->setCenterButtons(true);

    gridLayout->addWidget(buttonBox, 5, 0, 1, 2);


    retranslateUi(TrackingConfigDlgClass);
    QObject::connect(buttonBox, SIGNAL(accepted()), TrackingConfigDlgClass, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), TrackingConfigDlgClass, SLOT(reject()));

    isFiberCount->setCurrentIndex(1);


    QMetaObject::connectSlotsByName(TrackingConfigDlgClass);
    } // setupUi

    void retranslateUi(QDialog *TrackingConfigDlgClass)
    {
    TrackingConfigDlgClass->setWindowTitle(QApplication::translate("TrackingConfigDlgClass", "TrackingConfigDlg", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("TrackingConfigDlgClass", "FA threshold", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("TrackingConfigDlgClass", "Turning angle threshold", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("TrackingConfigDlgClass", "Step size (mm)", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("TrackingConfigDlgClass", "Length constraint (mm)", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("TrackingConfigDlgClass", "to", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("TrackingConfigDlgClass", "Stopping criteria", 0, QApplication::UnicodeUTF8));
    isFiberCount->clear();
    isFiberCount->insertItems(0, QStringList()
     << QApplication::translate("TrackingConfigDlgClass", "No. of seeds", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("TrackingConfigDlgClass", "No. of tracts", 0, QApplication::UnicodeUTF8)
    );
    label_7->setText(QApplication::translate("TrackingConfigDlgClass", "reaches", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(TrackingConfigDlgClass);
    } // retranslateUi

};

namespace Ui {
    class TrackingConfigDlgClass: public Ui_TrackingConfigDlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACKINGCONFIGDLG_H
