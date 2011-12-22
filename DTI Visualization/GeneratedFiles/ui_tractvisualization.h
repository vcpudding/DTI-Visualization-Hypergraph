/********************************************************************************
** Form generated from reading ui file 'tractvisualization.ui'
**
** Created: Wed Dec 21 21:30:40 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TRACTVISUALIZATION_H
#define UI_TRACTVISUALIZATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TractVisualizationClass
{
public:
    QAction *actionOpen_tract_file;
    QAction *actionCluster_Editor;
    QAction *actionTemplate_Maker;
    QAction *actionEmbed;
    QAction *actionLoad_norm_files;
    QAction *actionAssign_label;
    QAction *actionTest;
    QAction *actionTract_MDS;
    QAction *actionCluster_MDS;
    QAction *actionExperiment_result;
    QAction *actionAtlas_tractography;
    QAction *actionTarget_tract_embedding;
    QAction *actionLabeled_tract_embedding;
    QAction *actionGroup_comparison;
    QAction *actionWhole_brain_tractography;
    QAction *actionRotation;
    QAction *actionSelect_fibers;
    QAction *actionPan;
    QAction *actionResample;
    QAction *actionSave_tracts;
    QAction *actionSave_clusters;
    QAction *actionK_means;
    QAction *actionSave_cluster_centers;
    QAction *actionLoad_clusters;
    QAction *actionLoad_cluster_centers;
    QAction *actionK_means_graph;
    QAction *actionSave_hypergraph;
    QAction *actionLayout_graph;
    QAction *actionLoad_hypergraph;
    QAction *actionSave_hypergraph_layout;
    QAction *actionLoad_hypergraph_layout;
    QAction *actionShow_graph;
    QAction *actionOpen_compound_window;
    QAction *actionFast_layout;
    QAction *actionClustered_layout;
    QAction *actionPartitioned_layout;
    QAction *actionLength_filter;
    QAction *actionK_means_with_prune;
    QAction *actionRandom;
    QAction *actionFuzzy_c_means;
    QAction *actionLoad_fuzzy_clusters;
    QAction *actionSave_fuzzy_clusters;
    QAction *actionGenerate_fuzzy_graph;
    QAction *actionOpen_fiber_window;
    QAction *actionFuzzy_c_means_one_step;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTemplate_Tractography;
    QMenu *menuTractography;
    QMenu *menuCluster;
    QMenu *menuGenerate_hypergraph;
    QMenu *menuFilters;
    QMenu *menuDown_sample;
    QMenu *menuHelp;
    QMenu *menuDemo;
    QMenu *menuOperation;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TractVisualizationClass)
    {
    if (TractVisualizationClass->objectName().isEmpty())
        TractVisualizationClass->setObjectName(QString::fromUtf8("TractVisualizationClass"));
    TractVisualizationClass->resize(651, 547);
    actionOpen_tract_file = new QAction(TractVisualizationClass);
    actionOpen_tract_file->setObjectName(QString::fromUtf8("actionOpen_tract_file"));
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Rename Document.png")), QIcon::Normal, QIcon::Off);
    actionOpen_tract_file->setIcon(icon);
    actionCluster_Editor = new QAction(TractVisualizationClass);
    actionCluster_Editor->setObjectName(QString::fromUtf8("actionCluster_Editor"));
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Write Document.png")), QIcon::Normal, QIcon::Off);
    actionCluster_Editor->setIcon(icon1);
    actionTemplate_Maker = new QAction(TractVisualizationClass);
    actionTemplate_Maker->setObjectName(QString::fromUtf8("actionTemplate_Maker"));
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Play All.png")), QIcon::Normal, QIcon::Off);
    actionTemplate_Maker->setIcon(icon2);
    actionEmbed = new QAction(TractVisualizationClass);
    actionEmbed->setObjectName(QString::fromUtf8("actionEmbed"));
    actionEmbed->setEnabled(true);
    QIcon icon3;
    icon3.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Discussion.png")), QIcon::Normal, QIcon::Off);
    actionEmbed->setIcon(icon3);
    actionLoad_norm_files = new QAction(TractVisualizationClass);
    actionLoad_norm_files->setObjectName(QString::fromUtf8("actionLoad_norm_files"));
    QIcon icon4;
    icon4.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Backup Green Button.png")), QIcon::Normal, QIcon::Off);
    actionLoad_norm_files->setIcon(icon4);
    actionAssign_label = new QAction(TractVisualizationClass);
    actionAssign_label->setObjectName(QString::fromUtf8("actionAssign_label"));
    QIcon icon5;
    icon5.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Appointment Urgent.png")), QIcon::Normal, QIcon::Off);
    actionAssign_label->setIcon(icon5);
    actionTest = new QAction(TractVisualizationClass);
    actionTest->setObjectName(QString::fromUtf8("actionTest"));
    actionTract_MDS = new QAction(TractVisualizationClass);
    actionTract_MDS->setObjectName(QString::fromUtf8("actionTract_MDS"));
    QIcon icon6;
    icon6.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Smiley Sad Blue.png")), QIcon::Normal, QIcon::Off);
    actionTract_MDS->setIcon(icon6);
    actionCluster_MDS = new QAction(TractVisualizationClass);
    actionCluster_MDS->setObjectName(QString::fromUtf8("actionCluster_MDS"));
    QIcon icon7;
    icon7.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Smiley Sad.png")), QIcon::Normal, QIcon::Off);
    actionCluster_MDS->setIcon(icon7);
    actionExperiment_result = new QAction(TractVisualizationClass);
    actionExperiment_result->setObjectName(QString::fromUtf8("actionExperiment_result"));
    QIcon icon8;
    icon8.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/paint.png")), QIcon::Normal, QIcon::Off);
    actionExperiment_result->setIcon(icon8);
    actionAtlas_tractography = new QAction(TractVisualizationClass);
    actionAtlas_tractography->setObjectName(QString::fromUtf8("actionAtlas_tractography"));
    actionTarget_tract_embedding = new QAction(TractVisualizationClass);
    actionTarget_tract_embedding->setObjectName(QString::fromUtf8("actionTarget_tract_embedding"));
    actionLabeled_tract_embedding = new QAction(TractVisualizationClass);
    actionLabeled_tract_embedding->setObjectName(QString::fromUtf8("actionLabeled_tract_embedding"));
    actionGroup_comparison = new QAction(TractVisualizationClass);
    actionGroup_comparison->setObjectName(QString::fromUtf8("actionGroup_comparison"));
    actionWhole_brain_tractography = new QAction(TractVisualizationClass);
    actionWhole_brain_tractography->setObjectName(QString::fromUtf8("actionWhole_brain_tractography"));
    actionRotation = new QAction(TractVisualizationClass);
    actionRotation->setObjectName(QString::fromUtf8("actionRotation"));
    actionRotation->setCheckable(true);
    actionRotation->setChecked(true);
    actionSelect_fibers = new QAction(TractVisualizationClass);
    actionSelect_fibers->setObjectName(QString::fromUtf8("actionSelect_fibers"));
    actionSelect_fibers->setCheckable(true);
    actionPan = new QAction(TractVisualizationClass);
    actionPan->setObjectName(QString::fromUtf8("actionPan"));
    actionPan->setCheckable(true);
    actionResample = new QAction(TractVisualizationClass);
    actionResample->setObjectName(QString::fromUtf8("actionResample"));
    actionSave_tracts = new QAction(TractVisualizationClass);
    actionSave_tracts->setObjectName(QString::fromUtf8("actionSave_tracts"));
    actionSave_clusters = new QAction(TractVisualizationClass);
    actionSave_clusters->setObjectName(QString::fromUtf8("actionSave_clusters"));
    actionK_means = new QAction(TractVisualizationClass);
    actionK_means->setObjectName(QString::fromUtf8("actionK_means"));
    actionSave_cluster_centers = new QAction(TractVisualizationClass);
    actionSave_cluster_centers->setObjectName(QString::fromUtf8("actionSave_cluster_centers"));
    actionLoad_clusters = new QAction(TractVisualizationClass);
    actionLoad_clusters->setObjectName(QString::fromUtf8("actionLoad_clusters"));
    actionLoad_cluster_centers = new QAction(TractVisualizationClass);
    actionLoad_cluster_centers->setObjectName(QString::fromUtf8("actionLoad_cluster_centers"));
    actionK_means_graph = new QAction(TractVisualizationClass);
    actionK_means_graph->setObjectName(QString::fromUtf8("actionK_means_graph"));
    actionSave_hypergraph = new QAction(TractVisualizationClass);
    actionSave_hypergraph->setObjectName(QString::fromUtf8("actionSave_hypergraph"));
    actionLayout_graph = new QAction(TractVisualizationClass);
    actionLayout_graph->setObjectName(QString::fromUtf8("actionLayout_graph"));
    actionLoad_hypergraph = new QAction(TractVisualizationClass);
    actionLoad_hypergraph->setObjectName(QString::fromUtf8("actionLoad_hypergraph"));
    actionSave_hypergraph_layout = new QAction(TractVisualizationClass);
    actionSave_hypergraph_layout->setObjectName(QString::fromUtf8("actionSave_hypergraph_layout"));
    actionLoad_hypergraph_layout = new QAction(TractVisualizationClass);
    actionLoad_hypergraph_layout->setObjectName(QString::fromUtf8("actionLoad_hypergraph_layout"));
    actionShow_graph = new QAction(TractVisualizationClass);
    actionShow_graph->setObjectName(QString::fromUtf8("actionShow_graph"));
    actionOpen_compound_window = new QAction(TractVisualizationClass);
    actionOpen_compound_window->setObjectName(QString::fromUtf8("actionOpen_compound_window"));
    actionFast_layout = new QAction(TractVisualizationClass);
    actionFast_layout->setObjectName(QString::fromUtf8("actionFast_layout"));
    actionClustered_layout = new QAction(TractVisualizationClass);
    actionClustered_layout->setObjectName(QString::fromUtf8("actionClustered_layout"));
    actionPartitioned_layout = new QAction(TractVisualizationClass);
    actionPartitioned_layout->setObjectName(QString::fromUtf8("actionPartitioned_layout"));
    actionLength_filter = new QAction(TractVisualizationClass);
    actionLength_filter->setObjectName(QString::fromUtf8("actionLength_filter"));
    actionK_means_with_prune = new QAction(TractVisualizationClass);
    actionK_means_with_prune->setObjectName(QString::fromUtf8("actionK_means_with_prune"));
    actionRandom = new QAction(TractVisualizationClass);
    actionRandom->setObjectName(QString::fromUtf8("actionRandom"));
    actionFuzzy_c_means = new QAction(TractVisualizationClass);
    actionFuzzy_c_means->setObjectName(QString::fromUtf8("actionFuzzy_c_means"));
    actionLoad_fuzzy_clusters = new QAction(TractVisualizationClass);
    actionLoad_fuzzy_clusters->setObjectName(QString::fromUtf8("actionLoad_fuzzy_clusters"));
    actionSave_fuzzy_clusters = new QAction(TractVisualizationClass);
    actionSave_fuzzy_clusters->setObjectName(QString::fromUtf8("actionSave_fuzzy_clusters"));
    actionGenerate_fuzzy_graph = new QAction(TractVisualizationClass);
    actionGenerate_fuzzy_graph->setObjectName(QString::fromUtf8("actionGenerate_fuzzy_graph"));
    actionOpen_fiber_window = new QAction(TractVisualizationClass);
    actionOpen_fiber_window->setObjectName(QString::fromUtf8("actionOpen_fiber_window"));
    actionFuzzy_c_means_one_step = new QAction(TractVisualizationClass);
    actionFuzzy_c_means_one_step->setObjectName(QString::fromUtf8("actionFuzzy_c_means_one_step"));
    centralWidget = new QWidget(TractVisualizationClass);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    horizontalLayout = new QHBoxLayout(centralWidget);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setMargin(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    mdiArea = new QMdiArea(centralWidget);
    mdiArea->setObjectName(QString::fromUtf8("mdiArea"));

    horizontalLayout->addWidget(mdiArea);

    TractVisualizationClass->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(TractVisualizationClass);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 651, 22));
    menuFile = new QMenu(menuBar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuTemplate_Tractography = new QMenu(menuBar);
    menuTemplate_Tractography->setObjectName(QString::fromUtf8("menuTemplate_Tractography"));
    menuTractography = new QMenu(menuBar);
    menuTractography->setObjectName(QString::fromUtf8("menuTractography"));
    menuTractography->setEnabled(true);
    menuCluster = new QMenu(menuTractography);
    menuCluster->setObjectName(QString::fromUtf8("menuCluster"));
    menuGenerate_hypergraph = new QMenu(menuTractography);
    menuGenerate_hypergraph->setObjectName(QString::fromUtf8("menuGenerate_hypergraph"));
    menuFilters = new QMenu(menuTractography);
    menuFilters->setObjectName(QString::fromUtf8("menuFilters"));
    menuDown_sample = new QMenu(menuTractography);
    menuDown_sample->setObjectName(QString::fromUtf8("menuDown_sample"));
    menuHelp = new QMenu(menuBar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    menuDemo = new QMenu(menuBar);
    menuDemo->setObjectName(QString::fromUtf8("menuDemo"));
    menuOperation = new QMenu(menuBar);
    menuOperation->setObjectName(QString::fromUtf8("menuOperation"));
    TractVisualizationClass->setMenuBar(menuBar);
    mainToolBar = new QToolBar(TractVisualizationClass);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    mainToolBar->setEnabled(true);
    TractVisualizationClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(TractVisualizationClass);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    TractVisualizationClass->setStatusBar(statusBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuTractography->menuAction());
    menuBar->addAction(menuTemplate_Tractography->menuAction());
    menuBar->addAction(menuHelp->menuAction());
    menuBar->addAction(menuDemo->menuAction());
    menuBar->addAction(menuOperation->menuAction());
    menuFile->addAction(actionOpen_tract_file);
    menuFile->addAction(actionSave_tracts);
    menuFile->addSeparator();
    menuFile->addAction(actionLoad_clusters);
    menuFile->addAction(actionSave_clusters);
    menuFile->addAction(actionLoad_cluster_centers);
    menuFile->addAction(actionSave_cluster_centers);
    menuFile->addAction(actionLoad_fuzzy_clusters);
    menuFile->addAction(actionSave_fuzzy_clusters);
    menuFile->addSeparator();
    menuFile->addAction(actionSave_hypergraph);
    menuFile->addAction(actionLoad_hypergraph);
    menuFile->addAction(actionSave_hypergraph_layout);
    menuFile->addAction(actionLoad_hypergraph_layout);
    menuFile->addSeparator();
    menuFile->addAction(actionOpen_compound_window);
    menuFile->addAction(actionOpen_fiber_window);
    menuTemplate_Tractography->addAction(actionCluster_Editor);
    menuTemplate_Tractography->addAction(actionTemplate_Maker);
    menuTemplate_Tractography->addAction(actionTract_MDS);
    menuTemplate_Tractography->addAction(actionCluster_MDS);
    menuTractography->addAction(actionEmbed);
    menuTractography->addAction(actionLoad_norm_files);
    menuTractography->addAction(actionAssign_label);
    menuTractography->addAction(actionResample);
    menuTractography->addAction(menuCluster->menuAction());
    menuTractography->addAction(menuGenerate_hypergraph->menuAction());
    menuTractography->addAction(menuFilters->menuAction());
    menuTractography->addAction(menuDown_sample->menuAction());
    menuCluster->addAction(actionK_means);
    menuCluster->addAction(actionK_means_with_prune);
    menuCluster->addAction(actionFuzzy_c_means);
    menuCluster->addAction(actionFuzzy_c_means_one_step);
    menuGenerate_hypergraph->addAction(actionK_means_graph);
    menuGenerate_hypergraph->addAction(actionGenerate_fuzzy_graph);
    menuGenerate_hypergraph->addSeparator();
    menuGenerate_hypergraph->addAction(actionLayout_graph);
    menuGenerate_hypergraph->addAction(actionFast_layout);
    menuGenerate_hypergraph->addAction(actionClustered_layout);
    menuGenerate_hypergraph->addAction(actionPartitioned_layout);
    menuGenerate_hypergraph->addSeparator();
    menuGenerate_hypergraph->addAction(actionShow_graph);
    menuFilters->addAction(actionLength_filter);
    menuDown_sample->addAction(actionRandom);
    menuHelp->addAction(actionTest);
    menuHelp->addAction(actionExperiment_result);
    menuDemo->addAction(actionAtlas_tractography);
    menuDemo->addAction(actionTarget_tract_embedding);
    menuDemo->addAction(actionLabeled_tract_embedding);
    menuDemo->addAction(actionGroup_comparison);
    menuDemo->addAction(actionWhole_brain_tractography);
    menuOperation->addAction(actionRotation);
    menuOperation->addAction(actionSelect_fibers);
    menuOperation->addAction(actionPan);
    mainToolBar->addAction(actionOpen_tract_file);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionTemplate_Maker);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionEmbed);
    mainToolBar->addAction(actionLoad_norm_files);
    mainToolBar->addAction(actionAssign_label);
    mainToolBar->addSeparator();

    retranslateUi(TractVisualizationClass);

    QMetaObject::connectSlotsByName(TractVisualizationClass);
    } // setupUi

    void retranslateUi(QMainWindow *TractVisualizationClass)
    {
    TractVisualizationClass->setWindowTitle(QApplication::translate("TractVisualizationClass", "TractVisualization", 0, QApplication::UnicodeUTF8));
    actionOpen_tract_file->setText(QApplication::translate("TractVisualizationClass", "Open tract file", 0, QApplication::UnicodeUTF8));
    actionCluster_Editor->setText(QApplication::translate("TractVisualizationClass", "Cluster Editor", 0, QApplication::UnicodeUTF8));
    actionTemplate_Maker->setText(QApplication::translate("TractVisualizationClass", "Template Maker", 0, QApplication::UnicodeUTF8));
    actionEmbed->setText(QApplication::translate("TractVisualizationClass", "Embed", 0, QApplication::UnicodeUTF8));
    actionLoad_norm_files->setText(QApplication::translate("TractVisualizationClass", "Load norm files", 0, QApplication::UnicodeUTF8));
    actionAssign_label->setText(QApplication::translate("TractVisualizationClass", "Assign label", 0, QApplication::UnicodeUTF8));
    actionTest->setText(QApplication::translate("TractVisualizationClass", "Test", 0, QApplication::UnicodeUTF8));
    actionTract_MDS->setText(QApplication::translate("TractVisualizationClass", "Tract MDS", 0, QApplication::UnicodeUTF8));
    actionCluster_MDS->setText(QApplication::translate("TractVisualizationClass", "Cluster MDS", 0, QApplication::UnicodeUTF8));
    actionExperiment_result->setText(QApplication::translate("TractVisualizationClass", "Experiment result", 0, QApplication::UnicodeUTF8));
    actionAtlas_tractography->setText(QApplication::translate("TractVisualizationClass", "Atlas tractography", 0, QApplication::UnicodeUTF8));
    actionTarget_tract_embedding->setText(QApplication::translate("TractVisualizationClass", "Target tract embedding", 0, QApplication::UnicodeUTF8));
    actionLabeled_tract_embedding->setText(QApplication::translate("TractVisualizationClass", "Labeled tract embedding", 0, QApplication::UnicodeUTF8));
    actionGroup_comparison->setText(QApplication::translate("TractVisualizationClass", "Group comparison", 0, QApplication::UnicodeUTF8));
    actionWhole_brain_tractography->setText(QApplication::translate("TractVisualizationClass", "Whole brain tractography", 0, QApplication::UnicodeUTF8));
    actionRotation->setText(QApplication::translate("TractVisualizationClass", "Rotation", 0, QApplication::UnicodeUTF8));
    actionSelect_fibers->setText(QApplication::translate("TractVisualizationClass", "Select fibers", 0, QApplication::UnicodeUTF8));
    actionPan->setText(QApplication::translate("TractVisualizationClass", "Pan", 0, QApplication::UnicodeUTF8));
    actionResample->setText(QApplication::translate("TractVisualizationClass", "Resample", 0, QApplication::UnicodeUTF8));
    actionSave_tracts->setText(QApplication::translate("TractVisualizationClass", "Save tracts as", 0, QApplication::UnicodeUTF8));
    actionSave_clusters->setText(QApplication::translate("TractVisualizationClass", "Save clusters", 0, QApplication::UnicodeUTF8));
    actionK_means->setText(QApplication::translate("TractVisualizationClass", "K-means", 0, QApplication::UnicodeUTF8));
    actionSave_cluster_centers->setText(QApplication::translate("TractVisualizationClass", "Save cluster centers", 0, QApplication::UnicodeUTF8));
    actionLoad_clusters->setText(QApplication::translate("TractVisualizationClass", "Load clusters", 0, QApplication::UnicodeUTF8));
    actionLoad_cluster_centers->setText(QApplication::translate("TractVisualizationClass", "Load cluster centers", 0, QApplication::UnicodeUTF8));
    actionK_means_graph->setText(QApplication::translate("TractVisualizationClass", "Generate graph", 0, QApplication::UnicodeUTF8));
    actionSave_hypergraph->setText(QApplication::translate("TractVisualizationClass", "Save hypergraph", 0, QApplication::UnicodeUTF8));
    actionLayout_graph->setText(QApplication::translate("TractVisualizationClass", "Layout graph", 0, QApplication::UnicodeUTF8));
    actionLoad_hypergraph->setText(QApplication::translate("TractVisualizationClass", "Load hypergraph", 0, QApplication::UnicodeUTF8));
    actionSave_hypergraph_layout->setText(QApplication::translate("TractVisualizationClass", "Save hypergraph layout", 0, QApplication::UnicodeUTF8));
    actionLoad_hypergraph_layout->setText(QApplication::translate("TractVisualizationClass", "Load hypergraph layout", 0, QApplication::UnicodeUTF8));
    actionShow_graph->setText(QApplication::translate("TractVisualizationClass", "Show graph", 0, QApplication::UnicodeUTF8));
    actionOpen_compound_window->setText(QApplication::translate("TractVisualizationClass", "Open compound window", 0, QApplication::UnicodeUTF8));
    actionFast_layout->setText(QApplication::translate("TractVisualizationClass", "Fast layout", 0, QApplication::UnicodeUTF8));
    actionClustered_layout->setText(QApplication::translate("TractVisualizationClass", "Clustered layout", 0, QApplication::UnicodeUTF8));
    actionPartitioned_layout->setText(QApplication::translate("TractVisualizationClass", "Partitioned layout", 0, QApplication::UnicodeUTF8));
    actionLength_filter->setText(QApplication::translate("TractVisualizationClass", "Length filter", 0, QApplication::UnicodeUTF8));
    actionK_means_with_prune->setText(QApplication::translate("TractVisualizationClass", "K-means w/ prune", 0, QApplication::UnicodeUTF8));
    actionRandom->setText(QApplication::translate("TractVisualizationClass", "Random", 0, QApplication::UnicodeUTF8));
    actionFuzzy_c_means->setText(QApplication::translate("TractVisualizationClass", "Fuzzy c-means", 0, QApplication::UnicodeUTF8));
    actionLoad_fuzzy_clusters->setText(QApplication::translate("TractVisualizationClass", "Load fuzzy clusters", 0, QApplication::UnicodeUTF8));
    actionSave_fuzzy_clusters->setText(QApplication::translate("TractVisualizationClass", "Save fuzzy clusters", 0, QApplication::UnicodeUTF8));
    actionGenerate_fuzzy_graph->setText(QApplication::translate("TractVisualizationClass", "Generate fuzzy graph", 0, QApplication::UnicodeUTF8));
    actionOpen_fiber_window->setText(QApplication::translate("TractVisualizationClass", "Open fiber window", 0, QApplication::UnicodeUTF8));
    actionFuzzy_c_means_one_step->setText(QApplication::translate("TractVisualizationClass", "Fuzzy c-means one step", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("TractVisualizationClass", "File", 0, QApplication::UnicodeUTF8));
    menuTemplate_Tractography->setTitle(QApplication::translate("TractVisualizationClass", "Template", 0, QApplication::UnicodeUTF8));
    menuTractography->setTitle(QApplication::translate("TractVisualizationClass", "Tractography", 0, QApplication::UnicodeUTF8));
    menuCluster->setTitle(QApplication::translate("TractVisualizationClass", "Cluster", 0, QApplication::UnicodeUTF8));
    menuGenerate_hypergraph->setTitle(QApplication::translate("TractVisualizationClass", "Hypergraph", 0, QApplication::UnicodeUTF8));
    menuFilters->setTitle(QApplication::translate("TractVisualizationClass", "Filters", 0, QApplication::UnicodeUTF8));
    menuDown_sample->setTitle(QApplication::translate("TractVisualizationClass", "Down sample", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("TractVisualizationClass", "Help", 0, QApplication::UnicodeUTF8));
    menuDemo->setTitle(QApplication::translate("TractVisualizationClass", "Demo", 0, QApplication::UnicodeUTF8));
    menuOperation->setTitle(QApplication::translate("TractVisualizationClass", "Operation", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TractVisualizationClass: public Ui_TractVisualizationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACTVISUALIZATION_H
