#include "fibervis.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "TemplateFiberData.h"


FiberVis::FiberVis(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	_glWidget = new FiberGLWidget(this);
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(_glWidget);
	layout->setMargin(0);
	ui.dispFrame->setLayout(layout);

	_fiberData = new FiberData();
	_glWidget->setFiberData(_fiberData);

	_tensorData = NULL;
	_currentPath = "";
	_currentClusterIdx = 0;

	connectSignalSlots();

	//ui.controlFrame->setHidden(true);
}

FiberVis::~FiberVis()
{

}

void FiberVis::connectSignalSlots()
{
	connect(ui.volumeGroup, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowVolumeData(bool)));
	connect(ui.sagittalCheck, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowSagittal(bool)));
	connect(ui.coronalCheck, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowCoronal(bool)));
	connect(ui.axialCheck, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowAxial(bool)));
	connect(ui.sagittalSlider, SIGNAL(valueChanged(int)), _glWidget, SLOT(setSagittalSlice(int)));
	connect(ui.coronalSlider, SIGNAL(valueChanged(int)), _glWidget, SLOT(setCoronalSlice(int)));
	connect(ui.axialSlider, SIGNAL(valueChanged(int)), _glWidget, SLOT(setAxialSlice(int)));
	connect(ui.dispCombo, SIGNAL(currentIndexChanged(int)), _glWidget, SLOT(setDisplayMode(int)));
	connect(ui.colorCombo, SIGNAL(currentIndexChanged(int)), _glWidget, SLOT(setColorMode(int)));
	connect(ui.thickSpin, SIGNAL(valueChanged(double)), _glWidget, SLOT(setFiberThickness(double)));
	connect(ui.btnClusterFile, SIGNAL(clicked()), this, SLOT(handleOpenClusterFile()));
	connect(ui.btnEmbedFile, SIGNAL(clicked()), this, SLOT(handleOpenEmbedFile()));
	connect(ui.btnNextCluster, SIGNAL(clicked()), this, SLOT(handleNextCluster()));
	connect(ui.btnPrevCluster, SIGNAL(clicked()), this, SLOT(handlePrevCluster()));
	connect(ui.btnAllClusters, SIGNAL(clicked()), this, SLOT(handleAllClusters()));
	connect(ui.checkShowEmbed, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowEmbeddings(bool)));
	connect(ui.checkShowLabel, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowClusterLabels(bool)));
	connect(ui.checkColorByTpl, SIGNAL(toggled(bool)), _glWidget, SLOT(setIsColorEmbByTpl(bool)));
	connect(ui.btnAmb, SIGNAL(toggled(bool)), _glWidget, SLOT(setDispEmbedType(bool)));
	connect(ui.spinMinDist, SIGNAL(valueChanged(double)), _glWidget, SLOT(setEmbMinDistThres(double)));
	connect(ui.comboViewpoint, SIGNAL(currentIndexChanged(int)), _glWidget, SLOT(setViewpoint(int)));
}

void FiberVis::startFiberTracking(SeedData * seedData, TensorData * tensorData, const TrackingConfig & config)
{
	_fiberData->fiberTracking(seedData, tensorData, config);
	_glWidget->setTensorData(tensorData);
	_glWidget->setSeedData(seedData);
	_seedData = seedData;
	_tensorData = tensorData;

	ui.axialSlider->setMaximum(_tensorData->sizeZ()-1);
	ui.coronalSlider->setMaximum(_tensorData->sizeY()-1);
	ui.sagittalSlider->setMaximum(_tensorData->sizeX()-1);

	ui.axialSlider->setValue(_tensorData->sizeZ()/2);
	ui.coronalSlider->setValue(_tensorData->sizeY()/2);
	ui.sagittalSlider->setValue(_tensorData->sizeX()/2);
}

void FiberVis::showEvent(QShowEvent *e)
{
	_glWidget->makeCurrent();
	if (_tensorData)
	{
		_tensorData->refreshSlices();
	}
}

FiberVis* FiberVis::openFiberFile(const QString &fileName, int sizeX, int sizeY, int sizeZ)
{
	FiberVis * fiberVis = new FiberVis();
	fiberVis->_fiberData->loadFibers(fileName, sizeX, sizeY, sizeZ);
	fiberVis->ui.volumeGroup->setHidden(true);
	//fiberVis->ui.lineFiberFile->setText(fileName);
	fiberVis->setWindowTitle(fileName);
	fiberVis->_currentPath = fileName.left(fileName.lastIndexOf("\\"));
	return fiberVis;
}

void FiberVis::handleOpenClusterFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Cluster File"), _currentPath, tr("Text files (*.txt);;Cluster files (*.cluster)"));

	if (!fileName.isEmpty())
	{
		loadClusterFile(fileName);
	}
}

void FiberVis::handleOpenEmbedFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Embedding File"), _currentPath, tr("Fiber Files (*.emb)"));

	if (!fileName.isEmpty())
	{
		_fiberData->loadEmbeddings(fileName);
		ui.lineEmbedFile->setText(fileName);
		_glWidget->repaint();
	}
}

void FiberVis::handleNextCluster()
{
	if (_clusterBuf.empty())
	{
		return;
	}
	_currentClusterIdx = (_currentClusterIdx+1)%_clusterBuf.size();
	_fiberData->setCurrentCluster(_clusterBuf[_currentClusterIdx]);
	ui.lineCluster->setText(QString::number(_currentClusterIdx));
	ui.labelNumClusters->setText(QString(" / %1 (%2)").arg(_clusterBuf.size()).arg(_clusterBuf[_currentClusterIdx]));
	_glWidget->repaint();
}

void FiberVis::handlePrevCluster()
{
	if (_clusterBuf.empty())
	{
		return;
	}
	if (_currentClusterIdx==-1)
	{
		_currentClusterIdx = 0;
	}
	_currentClusterIdx = (_currentClusterIdx-1)%_clusterBuf.size();
	_fiberData->setCurrentCluster(_clusterBuf[_currentClusterIdx]);
	ui.lineCluster->setText(QString::number(_currentClusterIdx));
	ui.labelNumClusters->setText(QString(" / %1 (%2)").arg(_clusterBuf.size()).arg(_clusterBuf[_currentClusterIdx]));
	_glWidget->repaint();
}

void FiberVis::handleAllClusters()
{
	_currentClusterIdx = -1;
	_fiberData->setCurrentCluster(-1111);
	ui.lineCluster->setText("-1");
	ui.labelNumClusters->setText(QString(" / %1 ").arg(_clusterBuf.size()));
	_glWidget->repaint();
}

void FiberVis::loadClusterFile(const QString &fileName)
{
	//_clusterBuf = _fiberData->loadClusters(fileName);
	ui.lineClusterFile->setText(fileName);
	handleAllClusters();
}

void FiberVis::handleEmbed()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Norm File"), _currentPath, tr("Norm files (*.link)"));
	if (!fileName.isEmpty())
	{
		_fiberData->embedGPU(fileName);
		//_fiberData->embed(fileName);
		int ret = QMessageBox::question(this, "Embedding Result", "Do you want to save embedding results?", QMessageBox::Yes|QMessageBox::No);
		if (ret == QMessageBox::Yes)
		{
			QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Embedding File"), _currentPath, tr("Embedding files (*.emb)"));
			if (!saveFileName.isEmpty())
			{
				_fiberData->saveEmbeddings(saveFileName);
			}
		}
	}
}

void FiberVis::handleLoadNormFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Norm File"), _currentPath, tr("Norm files (*.link)"));
	if (!fileName.isEmpty())
	{
		_fiberData->loadNormMap(fileName);
		//_fiberData->normalizeFibers();
		_glWidget->repaint();
	}
}

void FiberVis::handleAssignLabel()
{
	QStringList clusterLabels = TemplateFiberData::clusterLabels();
	bool ok;
	QString label = QInputDialog::getItem(this, "Select an atlas cluster", "Cluster:", clusterLabels, 1, false, &ok);
	if (ok)
	{
		int clusterIdx = clusterLabels.indexOf(label)+1;
		_fiberData->assignAtlasLabel(clusterIdx);
		_glWidget->repaint();
	}
}