/********************************************************************************
** Form generated from reading ui file 'fibervis.ui'
**
** Created: Wed Dec 21 21:30:40 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FIBERVIS_H
#define UI_FIBERVIS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FiberVisClass
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *controlFrame;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QGroupBox *volumeGroup;
    QGridLayout *gridLayout_2;
    QSlider *axialSlider;
    QCheckBox *axialCheck;
    QCheckBox *coronalCheck;
    QCheckBox *sagittalCheck;
    QSlider *coronalSlider;
    QSlider *sagittalSlider;
    QFrame *line;
    QLabel *label_2;
    QComboBox *dispCombo;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QComboBox *colorCombo;
    QLabel *label_3;
    QDoubleSpinBox *thickSpin;
    QLabel *label_4;
    QComboBox *comboViewpoint;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QLabel *label_5;
    QLineEdit *lineClusterFile;
    QPushButton *btnClusterFile;
    QLabel *label_7;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnPrevCluster;
    QLineEdit *lineCluster;
    QLabel *labelNumClusters;
    QPushButton *btnNextCluster;
    QPushButton *btnAllClusters;
    QPushButton *btnTest;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_6;
    QCheckBox *checkShowEmbed;
    QCheckBox *checkShowLabel;
    QCheckBox *checkColorByTpl;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QLineEdit *lineEmbedFile;
    QPushButton *btnEmbedFile;
    QPushButton *btnAmb;
    QPushButton *btnAddAtlas;
    QDoubleSpinBox *spinMinDist;
    QFrame *dispFrame;

    void setupUi(QWidget *FiberVisClass)
    {
    if (FiberVisClass->objectName().isEmpty())
        FiberVisClass->setObjectName(QString::fromUtf8("FiberVisClass"));
    FiberVisClass->resize(1055, 786);
    verticalLayout = new QVBoxLayout(FiberVisClass);
    verticalLayout->setSpacing(2);
    verticalLayout->setMargin(11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 3, 0, 0);
    controlFrame = new QFrame(FiberVisClass);
    controlFrame->setObjectName(QString::fromUtf8("controlFrame"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(controlFrame->sizePolicy().hasHeightForWidth());
    controlFrame->setSizePolicy(sizePolicy);
    controlFrame->setFrameShape(QFrame::StyledPanel);
    controlFrame->setFrameShadow(QFrame::Raised);
    gridLayout = new QGridLayout(controlFrame);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(3);
    gridLayout->setVerticalSpacing(0);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);

    volumeGroup = new QGroupBox(controlFrame);
    volumeGroup->setObjectName(QString::fromUtf8("volumeGroup"));
    QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(volumeGroup->sizePolicy().hasHeightForWidth());
    volumeGroup->setSizePolicy(sizePolicy1);
    volumeGroup->setCheckable(true);
    gridLayout_2 = new QGridLayout(volumeGroup);
    gridLayout_2->setSpacing(6);
    gridLayout_2->setMargin(5);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout_2->setHorizontalSpacing(5);
    gridLayout_2->setVerticalSpacing(4);
    axialSlider = new QSlider(volumeGroup);
    axialSlider->setObjectName(QString::fromUtf8("axialSlider"));
    axialSlider->setOrientation(Qt::Horizontal);
    axialSlider->setTickPosition(QSlider::NoTicks);
    axialSlider->setTickInterval(1);

    gridLayout_2->addWidget(axialSlider, 0, 2, 1, 1);

    axialCheck = new QCheckBox(volumeGroup);
    axialCheck->setObjectName(QString::fromUtf8("axialCheck"));
    axialCheck->setChecked(true);

    gridLayout_2->addWidget(axialCheck, 0, 0, 1, 1);

    coronalCheck = new QCheckBox(volumeGroup);
    coronalCheck->setObjectName(QString::fromUtf8("coronalCheck"));
    coronalCheck->setChecked(true);

    gridLayout_2->addWidget(coronalCheck, 1, 0, 1, 1);

    sagittalCheck = new QCheckBox(volumeGroup);
    sagittalCheck->setObjectName(QString::fromUtf8("sagittalCheck"));
    sagittalCheck->setChecked(true);

    gridLayout_2->addWidget(sagittalCheck, 2, 0, 1, 1);

    coronalSlider = new QSlider(volumeGroup);
    coronalSlider->setObjectName(QString::fromUtf8("coronalSlider"));
    coronalSlider->setOrientation(Qt::Horizontal);

    gridLayout_2->addWidget(coronalSlider, 1, 2, 1, 1);

    sagittalSlider = new QSlider(volumeGroup);
    sagittalSlider->setObjectName(QString::fromUtf8("sagittalSlider"));
    sagittalSlider->setOrientation(Qt::Horizontal);

    gridLayout_2->addWidget(sagittalSlider, 2, 2, 1, 1);

    line = new QFrame(volumeGroup);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    gridLayout_2->addWidget(line, 0, 3, 3, 1);

    label_2 = new QLabel(volumeGroup);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout_2->addWidget(label_2, 0, 4, 1, 1);

    dispCombo = new QComboBox(volumeGroup);
    dispCombo->setObjectName(QString::fromUtf8("dispCombo"));

    gridLayout_2->addWidget(dispCombo, 0, 5, 1, 1);


    gridLayout->addWidget(volumeGroup, 0, 0, 1, 1);

    groupBox_2 = new QGroupBox(controlFrame);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
    groupBox_2->setSizePolicy(sizePolicy2);
    gridLayout_3 = new QGridLayout(groupBox_2);
    gridLayout_3->setSpacing(6);
    gridLayout_3->setMargin(11);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    label = new QLabel(groupBox_2);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout_3->addWidget(label, 0, 0, 1, 1);

    colorCombo = new QComboBox(groupBox_2);
    colorCombo->setObjectName(QString::fromUtf8("colorCombo"));

    gridLayout_3->addWidget(colorCombo, 0, 1, 1, 1);

    label_3 = new QLabel(groupBox_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout_3->addWidget(label_3, 1, 0, 1, 1);

    thickSpin = new QDoubleSpinBox(groupBox_2);
    thickSpin->setObjectName(QString::fromUtf8("thickSpin"));
    thickSpin->setMinimum(0.1);
    thickSpin->setMaximum(5);
    thickSpin->setSingleStep(0.1);
    thickSpin->setValue(1);

    gridLayout_3->addWidget(thickSpin, 1, 1, 1, 1);

    label_4 = new QLabel(groupBox_2);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout_3->addWidget(label_4, 2, 0, 1, 1);

    comboViewpoint = new QComboBox(groupBox_2);
    comboViewpoint->setObjectName(QString::fromUtf8("comboViewpoint"));

    gridLayout_3->addWidget(comboViewpoint, 2, 1, 1, 1);


    gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

    groupBox_3 = new QGroupBox(controlFrame);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
    groupBox_3->setSizePolicy(sizePolicy3);
    gridLayout_5 = new QGridLayout(groupBox_3);
    gridLayout_5->setSpacing(6);
    gridLayout_5->setMargin(11);
    gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
    label_5 = new QLabel(groupBox_3);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout_5->addWidget(label_5, 0, 0, 1, 1);

    lineClusterFile = new QLineEdit(groupBox_3);
    lineClusterFile->setObjectName(QString::fromUtf8("lineClusterFile"));
    QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(lineClusterFile->sizePolicy().hasHeightForWidth());
    lineClusterFile->setSizePolicy(sizePolicy4);

    gridLayout_5->addWidget(lineClusterFile, 0, 1, 1, 1);

    btnClusterFile = new QPushButton(groupBox_3);
    btnClusterFile->setObjectName(QString::fromUtf8("btnClusterFile"));
    sizePolicy4.setHeightForWidth(btnClusterFile->sizePolicy().hasHeightForWidth());
    btnClusterFile->setSizePolicy(sizePolicy4);
    btnClusterFile->setMinimumSize(QSize(30, 0));
    btnClusterFile->setMaximumSize(QSize(30, 16777215));

    gridLayout_5->addWidget(btnClusterFile, 0, 2, 1, 1);

    label_7 = new QLabel(groupBox_3);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout_5->addWidget(label_7, 1, 0, 1, 1);

    frame = new QFrame(groupBox_3);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(1);
    horizontalLayout->setMargin(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    btnPrevCluster = new QPushButton(frame);
    btnPrevCluster->setObjectName(QString::fromUtf8("btnPrevCluster"));
    QSizePolicy sizePolicy5(QSizePolicy::Maximum, QSizePolicy::Fixed);
    sizePolicy5.setHorizontalStretch(0);
    sizePolicy5.setVerticalStretch(0);
    sizePolicy5.setHeightForWidth(btnPrevCluster->sizePolicy().hasHeightForWidth());
    btnPrevCluster->setSizePolicy(sizePolicy5);
    btnPrevCluster->setMaximumSize(QSize(30, 16777215));

    horizontalLayout->addWidget(btnPrevCluster);

    lineCluster = new QLineEdit(frame);
    lineCluster->setObjectName(QString::fromUtf8("lineCluster"));
    sizePolicy5.setHeightForWidth(lineCluster->sizePolicy().hasHeightForWidth());
    lineCluster->setSizePolicy(sizePolicy5);
    lineCluster->setMaximumSize(QSize(30, 16777215));
    lineCluster->setReadOnly(true);

    horizontalLayout->addWidget(lineCluster);

    labelNumClusters = new QLabel(frame);
    labelNumClusters->setObjectName(QString::fromUtf8("labelNumClusters"));
    sizePolicy3.setHeightForWidth(labelNumClusters->sizePolicy().hasHeightForWidth());
    labelNumClusters->setSizePolicy(sizePolicy3);
    labelNumClusters->setMinimumSize(QSize(50, 0));
    labelNumClusters->setMaximumSize(QSize(50, 16777215));

    horizontalLayout->addWidget(labelNumClusters);

    btnNextCluster = new QPushButton(frame);
    btnNextCluster->setObjectName(QString::fromUtf8("btnNextCluster"));
    btnNextCluster->setMaximumSize(QSize(30, 16777215));

    horizontalLayout->addWidget(btnNextCluster);

    btnAllClusters = new QPushButton(frame);
    btnAllClusters->setObjectName(QString::fromUtf8("btnAllClusters"));
    btnAllClusters->setMaximumSize(QSize(30, 16777215));

    horizontalLayout->addWidget(btnAllClusters);


    gridLayout_5->addWidget(frame, 1, 1, 1, 2);

    btnTest = new QPushButton(groupBox_3);
    btnTest->setObjectName(QString::fromUtf8("btnTest"));

    gridLayout_5->addWidget(btnTest, 2, 0, 1, 3);


    gridLayout->addWidget(groupBox_3, 0, 2, 1, 1);

    groupBox_4 = new QGroupBox(controlFrame);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    sizePolicy3.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
    groupBox_4->setSizePolicy(sizePolicy3);
    gridLayout_6 = new QGridLayout(groupBox_4);
    gridLayout_6->setSpacing(6);
    gridLayout_6->setMargin(3);
    gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
    gridLayout_6->setHorizontalSpacing(3);
    gridLayout_6->setVerticalSpacing(5);
    checkShowEmbed = new QCheckBox(groupBox_4);
    checkShowEmbed->setObjectName(QString::fromUtf8("checkShowEmbed"));

    gridLayout_6->addWidget(checkShowEmbed, 2, 0, 1, 1);

    checkShowLabel = new QCheckBox(groupBox_4);
    checkShowLabel->setObjectName(QString::fromUtf8("checkShowLabel"));
    checkShowLabel->setChecked(false);

    gridLayout_6->addWidget(checkShowLabel, 3, 0, 1, 1);

    checkColorByTpl = new QCheckBox(groupBox_4);
    checkColorByTpl->setObjectName(QString::fromUtf8("checkColorByTpl"));

    gridLayout_6->addWidget(checkColorByTpl, 5, 0, 1, 1);

    frame_3 = new QFrame(groupBox_4);
    frame_3->setObjectName(QString::fromUtf8("frame_3"));
    frame_3->setFrameShape(QFrame::StyledPanel);
    frame_3->setFrameShadow(QFrame::Raised);
    horizontalLayout_2 = new QHBoxLayout(frame_3);
    horizontalLayout_2->setSpacing(3);
    horizontalLayout_2->setMargin(0);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_6 = new QLabel(frame_3);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    horizontalLayout_2->addWidget(label_6);

    lineEmbedFile = new QLineEdit(frame_3);
    lineEmbedFile->setObjectName(QString::fromUtf8("lineEmbedFile"));
    sizePolicy4.setHeightForWidth(lineEmbedFile->sizePolicy().hasHeightForWidth());
    lineEmbedFile->setSizePolicy(sizePolicy4);

    horizontalLayout_2->addWidget(lineEmbedFile);

    btnEmbedFile = new QPushButton(frame_3);
    btnEmbedFile->setObjectName(QString::fromUtf8("btnEmbedFile"));
    btnEmbedFile->setMaximumSize(QSize(30, 16777215));

    horizontalLayout_2->addWidget(btnEmbedFile);


    gridLayout_6->addWidget(frame_3, 1, 0, 1, 2);

    btnAmb = new QPushButton(groupBox_4);
    btnAmb->setObjectName(QString::fromUtf8("btnAmb"));
    btnAmb->setCheckable(true);

    gridLayout_6->addWidget(btnAmb, 2, 1, 1, 1);

    btnAddAtlas = new QPushButton(groupBox_4);
    btnAddAtlas->setObjectName(QString::fromUtf8("btnAddAtlas"));
    btnAddAtlas->setEnabled(false);

    gridLayout_6->addWidget(btnAddAtlas, 3, 1, 1, 1);

    spinMinDist = new QDoubleSpinBox(groupBox_4);
    spinMinDist->setObjectName(QString::fromUtf8("spinMinDist"));
    spinMinDist->setValue(30);

    gridLayout_6->addWidget(spinMinDist, 5, 1, 1, 1);


    gridLayout->addWidget(groupBox_4, 0, 3, 1, 1);


    verticalLayout->addWidget(controlFrame);

    dispFrame = new QFrame(FiberVisClass);
    dispFrame->setObjectName(QString::fromUtf8("dispFrame"));
    dispFrame->setFrameShape(QFrame::StyledPanel);
    dispFrame->setFrameShadow(QFrame::Raised);

    verticalLayout->addWidget(dispFrame);


    retranslateUi(FiberVisClass);
    QObject::connect(axialCheck, SIGNAL(toggled(bool)), axialSlider, SLOT(setEnabled(bool)));
    QObject::connect(coronalCheck, SIGNAL(toggled(bool)), coronalSlider, SLOT(setEnabled(bool)));
    QObject::connect(sagittalCheck, SIGNAL(toggled(bool)), sagittalSlider, SLOT(setEnabled(bool)));
    QObject::connect(btnAmb, SIGNAL(toggled(bool)), btnAddAtlas, SLOT(setEnabled(bool)));

    QMetaObject::connectSlotsByName(FiberVisClass);
    } // setupUi

    void retranslateUi(QWidget *FiberVisClass)
    {
    FiberVisClass->setWindowTitle(QApplication::translate("FiberVisClass", "FiberVis", 0, QApplication::UnicodeUTF8));
    volumeGroup->setTitle(QApplication::translate("FiberVisClass", "Show tensor field", 0, QApplication::UnicodeUTF8));
    axialCheck->setText(QApplication::translate("FiberVisClass", "Axial", 0, QApplication::UnicodeUTF8));
    coronalCheck->setText(QApplication::translate("FiberVisClass", "Coronal", 0, QApplication::UnicodeUTF8));
    sagittalCheck->setText(QApplication::translate("FiberVisClass", "Sagittal", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("FiberVisClass", "Display", 0, QApplication::UnicodeUTF8));
    dispCombo->clear();
    dispCombo->insertItems(0, QStringList()
     << QApplication::translate("FiberVisClass", "B0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("FiberVisClass", "Color", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("FiberVisClass", "FA", 0, QApplication::UnicodeUTF8)
    );
    groupBox_2->setTitle(QApplication::translate("FiberVisClass", "Tractography", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("FiberVisClass", "Color Mode", 0, QApplication::UnicodeUTF8));
    colorCombo->clear();
    colorCombo->insertItems(0, QStringList()
     << QApplication::translate("FiberVisClass", "Local Orientation", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("FiberVisClass", "Cluster", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("FiberVisClass", "Seed Region", 0, QApplication::UnicodeUTF8)
    );
    label_3->setText(QApplication::translate("FiberVisClass", "Thickness", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("FiberVisClass", "Viewpoint", 0, QApplication::UnicodeUTF8));
    comboViewpoint->clear();
    comboViewpoint->insertItems(0, QStringList()
     << QApplication::translate("FiberVisClass", "Free", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("FiberVisClass", "Top", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("FiberVisClass", "Frontal", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("FiberVisClass", "Left", 0, QApplication::UnicodeUTF8)
    );
    groupBox_3->setTitle(QApplication::translate("FiberVisClass", "Clusters", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("FiberVisClass", "Cluster file:", 0, QApplication::UnicodeUTF8));
    btnClusterFile->setText(QApplication::translate("FiberVisClass", "...", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("FiberVisClass", "Display cluster:", 0, QApplication::UnicodeUTF8));
    btnPrevCluster->setText(QApplication::translate("FiberVisClass", "<", 0, QApplication::UnicodeUTF8));
    lineCluster->setText(QApplication::translate("FiberVisClass", "-1", 0, QApplication::UnicodeUTF8));
    labelNumClusters->setText(QApplication::translate("FiberVisClass", " / 0 ", 0, QApplication::UnicodeUTF8));
    btnNextCluster->setText(QApplication::translate("FiberVisClass", ">", 0, QApplication::UnicodeUTF8));
    btnAllClusters->setText(QApplication::translate("FiberVisClass", "All", 0, QApplication::UnicodeUTF8));
    btnTest->setText(QApplication::translate("FiberVisClass", "Save selected index", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QApplication::translate("FiberVisClass", "Embedding", 0, QApplication::UnicodeUTF8));
    checkShowEmbed->setText(QApplication::translate("FiberVisClass", "Show embeddings", 0, QApplication::UnicodeUTF8));
    checkShowLabel->setText(QApplication::translate("FiberVisClass", "Show cluster labels", 0, QApplication::UnicodeUTF8));
    checkColorByTpl->setText(QApplication::translate("FiberVisClass", "Color by template cluster", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("FiberVisClass", "Embedding file:", 0, QApplication::UnicodeUTF8));
    btnEmbedFile->setText(QApplication::translate("FiberVisClass", "...", 0, QApplication::UnicodeUTF8));
    btnAmb->setText(QApplication::translate("FiberVisClass", "Show ambigous embs", 0, QApplication::UnicodeUTF8));
    btnAddAtlas->setText(QApplication::translate("FiberVisClass", "Add to atlas", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FiberVisClass);
    } // retranslateUi

};

namespace Ui {
    class FiberVisClass: public Ui_FiberVisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIBERVIS_H
