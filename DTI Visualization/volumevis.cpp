#include "volumevis.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

VolumeVis::VolumeVis(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_glWidget = new GLWidget ();
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(_glWidget);
	layout->setMargin(0);
	ui.dispFrame->setLayout(layout);

	ui.dtiFrame->setVisible(false);

	_volData = NULL;
	_fuseData = NULL;
	_tensorData = NULL;
	_isTensorDataLoded = false;

	connectSignalSlots();
}

VolumeVis::~VolumeVis()
{

}

void VolumeVis::connectSignalSlots()
{
	connect(ui.btnParasagittal, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowSagittal(bool)));
	connect(ui.btnCoronal, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowCoronal(bool)));
	connect(ui.btnAxial, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowAxial(bool)));
	connect(ui.dispComboBox, SIGNAL(currentIndexChanged(int)), _glWidget, SLOT(setDisplayMode(int)));
	connect(ui.sliceCheckBox, SIGNAL(toggled(bool)), _glWidget, SLOT(setShowSliceIdx(bool)));
	connect(ui.viewCombo, SIGNAL(currentIndexChanged(int)), _glWidget, SLOT(setViewpoint(int)));
	connect(_glWidget, SIGNAL(viewpointChanged()), this, SLOT(handleViewpointChanged()));
	connect(ui.checkInterpolate, SIGNAL(toggled(bool)), this, SLOT(handleSetInterpolate(bool)));
}

void VolumeVis::loadVolumeData(const QStringList &fileList)
{
	if (!_volData)
	{
		_volData = new VolumeData();
	}
	_volData->readOrientedData(fileList);
	_glWidget->setVolumeData(_volData);
	ui.dtiFrame->setVisible(false);
	_isTensorDataLoded = false;
}

void VolumeVis::loadDTIData(const QStringList &fileList, const QString & dtiFileName)
{
	if (!_tensorData)
	{
		_tensorData = new TensorData();
	}

	if (dtiFileName.isEmpty())
	{
		bool ok;
		int nsThres =  QInputDialog::getInteger(NULL, "Input noise signal threshold",
			"Signal level:", 20, 0, 10000, 1, &ok);
		if (ok)
		{
			_tensorData->setNoiseSignalThres(nsThres);

			if (_tensorData->readOrientedData(fileList))
			{
				_glWidget->setVolumeData(_tensorData);
				ui.dtiFrame->setVisible(true);
				_isTensorDataLoded = true;
			} else
			{
				loadVolumeData(fileList);
			}
		}
	} else
	{
		if (QMessageBox::question(this, "Load DTI series", "Load saved .tsf file?", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
		{
			_tensorData->readOrientedData(fileList, dtiFileName);
			_glWidget->setVolumeData(_tensorData);
			ui.dtiFrame->setVisible(true);
			_isTensorDataLoded = true;
		} else
		{
			loadDTIData(fileList);
		}
	}
}

int VolumeVis::saveTensorField(const QString & fileName)
{
	if (!_tensorData)
	{
		return 0;
	}	
	_tensorData->saveTensorField(fileName);
	return 1;
}

void VolumeVis::handleViewpointChanged()
{
	ui.viewCombo->setCurrentIndex(0);
}

void VolumeVis::fuseVolumeData(const QStringList &fileList)
{
	/*if (!_fuseData)
	{
		_fuseData = new VolumeData();
	}
	_fuseData->readOrientedData(fileList);
	_glWidget->addVolumeData(_fuseData);*/
}

void VolumeVis::handleSetInterpolate(bool bSet)
{
	if (_tensorData)
	{
		_tensorData->setInterpolate(bSet);
	}
}

void VolumeVis::showEvent(QShowEvent *e)
{
	VolumeData * volData = _tensorData?_tensorData:_volData;
	if (!volData)
	{
		return;
	}
	//ui.dispComboBox->setCurrentIndex(volData->getDisplayMode());
	_glWidget->makeCurrent();
	_glWidget->setDisplayMode(ui.dispComboBox->currentIndex());
	_glWidget->setViewpoint(ui.viewCombo->currentIndex());
}