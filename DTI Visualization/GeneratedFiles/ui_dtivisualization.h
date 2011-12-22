/********************************************************************************
** Form generated from reading ui file 'dtivisualization.ui'
**
** Created: Wed Dec 21 21:30:41 2011
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DTIVISUALIZATION_H
#define UI_DTIVISUALIZATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DTIVisualizationClass
{
public:
    QAction *actionOpen_folder;
    QAction *actionLoad_selected_series;
    QAction *actionLoad_series_as_DTI;
    QAction *actionSave_tensor_field;
    QAction *actionFolder_explorer;
    QAction *actionTest;
    QAction *actionFuse_with_current_series;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *volumeTab;
    QMenuBar *menuBar;
    QMenu *menuHelp;
    QMenu *menuFile;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTreeWidget *fileTreeWidget;

    void setupUi(QMainWindow *DTIVisualizationClass)
    {
    if (DTIVisualizationClass->objectName().isEmpty())
        DTIVisualizationClass->setObjectName(QString::fromUtf8("DTIVisualizationClass"));
    DTIVisualizationClass->resize(846, 596);
    DTIVisualizationClass->setMinimumSize(QSize(600, 0));
    DTIVisualizationClass->setCursor(QCursor(Qt::ArrowCursor));
    actionOpen_folder = new QAction(DTIVisualizationClass);
    actionOpen_folder->setObjectName(QString::fromUtf8("actionOpen_folder"));
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/bag.png")), QIcon::Normal, QIcon::Off);
    actionOpen_folder->setIcon(icon);
    actionLoad_selected_series = new QAction(DTIVisualizationClass);
    actionLoad_selected_series->setObjectName(QString::fromUtf8("actionLoad_selected_series"));
    actionLoad_selected_series->setEnabled(false);
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/monitor.png")), QIcon::Normal, QIcon::Off);
    actionLoad_selected_series->setIcon(icon1);
    actionLoad_series_as_DTI = new QAction(DTIVisualizationClass);
    actionLoad_series_as_DTI->setObjectName(QString::fromUtf8("actionLoad_series_as_DTI"));
    actionLoad_series_as_DTI->setEnabled(false);
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/notepad.png")), QIcon::Normal, QIcon::Off);
    actionLoad_series_as_DTI->setIcon(icon2);
    actionSave_tensor_field = new QAction(DTIVisualizationClass);
    actionSave_tensor_field->setObjectName(QString::fromUtf8("actionSave_tensor_field"));
    actionSave_tensor_field->setEnabled(false);
    QIcon icon3;
    icon3.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/disk.png")), QIcon::Normal, QIcon::Off);
    actionSave_tensor_field->setIcon(icon3);
    actionFolder_explorer = new QAction(DTIVisualizationClass);
    actionFolder_explorer->setObjectName(QString::fromUtf8("actionFolder_explorer"));
    actionFolder_explorer->setCheckable(true);
    actionFolder_explorer->setChecked(true);
    actionTest = new QAction(DTIVisualizationClass);
    actionTest->setObjectName(QString::fromUtf8("actionTest"));
    QIcon icon4;
    icon4.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/clock.png")), QIcon::Normal, QIcon::Off);
    actionTest->setIcon(icon4);
    actionFuse_with_current_series = new QAction(DTIVisualizationClass);
    actionFuse_with_current_series->setObjectName(QString::fromUtf8("actionFuse_with_current_series"));
    actionFuse_with_current_series->setEnabled(false);
    QIcon icon5;
    icon5.addPixmap(QPixmap(QString::fromUtf8("Resources/Icons/Picture Document.png")), QIcon::Normal, QIcon::Off);
    actionFuse_with_current_series->setIcon(icon5);
    centralWidget = new QWidget(DTIVisualizationClass);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    horizontalLayout = new QHBoxLayout(centralWidget);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setMargin(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    tabWidget = new QTabWidget(centralWidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    volumeTab = new QWidget();
    volumeTab->setObjectName(QString::fromUtf8("volumeTab"));
    tabWidget->addTab(volumeTab, QString());

    horizontalLayout->addWidget(tabWidget);

    DTIVisualizationClass->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(DTIVisualizationClass);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 846, 22));
    menuHelp = new QMenu(menuBar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    menuFile = new QMenu(menuBar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuView = new QMenu(menuBar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    DTIVisualizationClass->setMenuBar(menuBar);
    mainToolBar = new QToolBar(DTIVisualizationClass);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    DTIVisualizationClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(DTIVisualizationClass);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    DTIVisualizationClass->setStatusBar(statusBar);
    dockWidget = new QDockWidget(DTIVisualizationClass);
    dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(dockWidget->sizePolicy().hasHeightForWidth());
    dockWidget->setSizePolicy(sizePolicy);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    verticalLayout = new QVBoxLayout(dockWidgetContents);
    verticalLayout->setSpacing(6);
    verticalLayout->setMargin(0);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    fileTreeWidget = new QTreeWidget(dockWidgetContents);
    fileTreeWidget->setObjectName(QString::fromUtf8("fileTreeWidget"));
    fileTreeWidget->setAutoExpandDelay(-1);
    fileTreeWidget->setItemsExpandable(false);
    fileTreeWidget->setHeaderHidden(true);

    verticalLayout->addWidget(fileTreeWidget);

    dockWidget->setWidget(dockWidgetContents);
    DTIVisualizationClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuView->menuAction());
    menuBar->addAction(menuHelp->menuAction());
    menuHelp->addAction(actionTest);
    menuFile->addAction(actionOpen_folder);
    menuFile->addSeparator();
    menuFile->addAction(actionLoad_selected_series);
    menuFile->addAction(actionFuse_with_current_series);
    menuFile->addSeparator();
    menuFile->addAction(actionLoad_series_as_DTI);
    menuFile->addAction(actionSave_tensor_field);
    menuView->addAction(actionFolder_explorer);
    mainToolBar->addAction(actionOpen_folder);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionLoad_selected_series);
    mainToolBar->addAction(actionFuse_with_current_series);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionLoad_series_as_DTI);
    mainToolBar->addAction(actionSave_tensor_field);

    retranslateUi(DTIVisualizationClass);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(DTIVisualizationClass);
    } // setupUi

    void retranslateUi(QMainWindow *DTIVisualizationClass)
    {
    DTIVisualizationClass->setWindowTitle(QApplication::translate("DTIVisualizationClass", "DTIVisualization", 0, QApplication::UnicodeUTF8));
    actionOpen_folder->setText(QApplication::translate("DTIVisualizationClass", "Open folder", 0, QApplication::UnicodeUTF8));
    actionLoad_selected_series->setText(QApplication::translate("DTIVisualizationClass", "Load selected series", 0, QApplication::UnicodeUTF8));
    actionLoad_series_as_DTI->setText(QApplication::translate("DTIVisualizationClass", "Load series as DTI", 0, QApplication::UnicodeUTF8));
    actionSave_tensor_field->setText(QApplication::translate("DTIVisualizationClass", "Save tensor field", 0, QApplication::UnicodeUTF8));
    actionFolder_explorer->setText(QApplication::translate("DTIVisualizationClass", "Folder explorer", 0, QApplication::UnicodeUTF8));
    actionTest->setText(QApplication::translate("DTIVisualizationClass", "Test", 0, QApplication::UnicodeUTF8));
    actionFuse_with_current_series->setText(QApplication::translate("DTIVisualizationClass", "Fuse with current series", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(volumeTab), QApplication::translate("DTIVisualizationClass", "Volume Viewer", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("DTIVisualizationClass", "Help", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("DTIVisualizationClass", "File", 0, QApplication::UnicodeUTF8));
    menuView->setTitle(QApplication::translate("DTIVisualizationClass", "View", 0, QApplication::UnicodeUTF8));
    fileTreeWidget->headerItem()->setText(0, QApplication::translate("DTIVisualizationClass", "1", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DTIVisualizationClass: public Ui_DTIVisualizationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DTIVISUALIZATION_H
