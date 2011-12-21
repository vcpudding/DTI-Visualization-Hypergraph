#include "dtivisualization.h"
#include "trackingconfigdlg.h"

#include <QMessageBox>

DTIVisualization::DTIVisualization(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_volumeVis = new VolumeVis();
	QHBoxLayout * layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->addWidget(_volumeVis);
	ui.volumeTab->setLayout(layout);

	_seedVis = new SeedVis();
	_fiberVis = new FiberVis();

	connectSignalSlots();
}

DTIVisualization::~DTIVisualization()
{
}

void DTIVisualization::connectSignalSlots()
{
	connect(ui.actionOpen_folder, SIGNAL(triggered()), this, SLOT(handleOpenFolder()));
	connect(ui.fileTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(handleTreeItemClicked(QTreeWidgetItem *)));
	connect(ui.actionLoad_selected_series, SIGNAL(triggered()), this, SLOT(handleLoadSeries()));
	connect(ui.actionLoad_series_as_DTI, SIGNAL(triggered()), this, SLOT(handleLoadDTI()));
	connect(ui.actionSave_tensor_field, SIGNAL(triggered()), this, SLOT(handleSaveDTI()));
	connect(ui.actionFolder_explorer, SIGNAL(toggled(bool)), ui.dockWidget, SLOT(setVisible(bool)));
	connect(ui.actionTest, SIGNAL(triggered()), this, SLOT(handleTest()));
	connect(ui.actionFuse_with_current_series, SIGNAL(triggered()), this, SLOT(handleFuseSeries()));
	connect(_seedVis, SIGNAL(fiberTrackingStarted()), this, SLOT(handleFiberTracking()));
}

void DTIVisualization::handleOpenFolder()
{
	/*QFileDialog fileDlg;
	fileDlg.setFileMode(QFileDialog::DirectoryOnly);
	if (fileDlg.exec())
	{*/
		//QString folderName = fileDlg.selectedFiles()[0];
		//QString folderName = "D:\\+Study\\LEUNG KIN PING\\19-Jan-2009\\";
		QString folderName = "D:\\Stella\\Research\\Sample data\\From Henry Mak\\DICOM\\LEUNG KIN PING\\19-Jan-2009\\";
		QDir folder (folderName);
		if (!folder.exists(DATASET_FILE_NAME))
		{
			parseFolder(folderName);
		}
		readDatasetFile(folder.filePath(DATASET_FILE_NAME), ui.fileTreeWidget);
		_currentFolder = folderName;/**/
	//}
}

void DTIVisualization::handleTreeItemClicked(QTreeWidgetItem *item)
{
	QString itemData = item->data(0, Qt::UserRole).toString();
	if (!itemData.isEmpty())
	{
		_selectedSeries = getOrderedDcmFiles(QDir(_currentFolder).filePath(DATASET_FILE_NAME), itemData+"/File");
		if (!_selectedSeries.isEmpty())
		{
			ui.actionLoad_selected_series->setEnabled(true);
			ui.actionLoad_series_as_DTI->setEnabled(true);
			ui.actionSave_tensor_field->setEnabled(true);
		} else
		{
			ui.actionLoad_selected_series->setEnabled(false);
			ui.actionLoad_series_as_DTI->setEnabled(false);
			ui.actionSave_tensor_field->setEnabled(false);
		}
	}
}

void DTIVisualization::handleLoadSeries()
{
	_volumeVis->loadVolumeData(_selectedSeries);
	ui.tabWidget->removeTab(1);
	ui.actionFuse_with_current_series->setEnabled(true);
}

void DTIVisualization::handleLoadDTI()
{
	QTreeWidgetItem * loadedItem = ui.fileTreeWidget->currentItem();
	QString queryStr = loadedItem->data(0, Qt::UserRole).toString();
	QString dtiFileName = getDTIFileName(_currentFolder, queryStr);
	_volumeVis->loadDTIData(_selectedSeries, dtiFileName);
	if (_volumeVis->isTensorDataLoaded())
	{
		ui.tabWidget->addTab(_seedVis, "Seed Viewer");
		_seedVis->setTensorData(_volumeVis->getTensorData());
		_seedVis->setCurrentFolder(_currentFolder);
	}
	ui.actionFuse_with_current_series->setEnabled(true);
}

void DTIVisualization::handleSaveDTI()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, "Save tensor field", _currentFolder, "Tensor file (*.tsf)");
	if (!fileName.isEmpty())
	{
		QTreeWidgetItem * loadedItem = ui.fileTreeWidget->currentItem();
		QString queryStr = loadedItem->data(0, Qt::UserRole).toString();
		if(_volumeVis->saveTensorField(fileName))
		{
			insertDTIFileItem(_currentFolder, queryStr, fileName);
			if (loadedItem->text(0).right(1).compare("*")!=0)
			{
				loadedItem->setText(0, loadedItem->text(0)+"*");
			}
		}
	}
}

void DTIVisualization::handleTest ()
{
	/*QString queryStr = "root/Patient[@patients_name = 'LEUNG KIN PING']/Date[@acquisition_date = '733792']/Protocol[@protocol_name = 'T2W_FLAIR SENSE ']/Collection[@acquisition_number = '1001']";
	insertDTIFileItem(_currentFolder+"dicom_dataset.xml", queryStr, "D:/Stella/Research/Sample data/From Henry Mak/DICOM/LEUNG KIN PING/19-Jan-2009/test1.tsf");*/
	TensorData::test();
}

void DTIVisualization::handleFuseSeries()
{
	_volumeVis->fuseVolumeData(_selectedSeries);
	ui.actionFuse_with_current_series->setEnabled(false);
}

void DTIVisualization::handleFiberTracking()
{
	SeedData * seedData = _seedVis->_seedData;
	if (seedData->refreshSeedBuf())
	{
		TrackingConfig * pConfig = TrackingConfigDlg::getConfig(this, "Fiber Tracking Configuration");
		if (!pConfig)
		{
			return;
		}
		_fiberVis->startFiberTracking(seedData, _seedVis->_tensorData, *pConfig);
		ui.tabWidget->addTab(_fiberVis, "Tractography Viewer");
		ui.tabWidget->setCurrentWidget(_fiberVis);
	} else
	{
		QMessageBox::warning(NULL, "Empty seed buffer", "No seed region is specified yet!");
	}
}