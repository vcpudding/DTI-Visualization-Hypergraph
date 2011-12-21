#include "seedvis.h"

#include <QCheckBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>

SeedVis::SeedVis(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.seedTable->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	ui.seedTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	ui.seedTable->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	ui.seedTable->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);

	ui.seedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.seedTable->setSelectionMode(QAbstractItemView::SingleSelection);

	_colorTable = new ColorTable("Resources/seed_spectrum.bmp");

	_tensorData = NULL;
	_seedData = new SeedData();
	_enableSeedMapper = new QSignalMapper();
	_seedTypeMapper = new QSignalMapper();

	_glWidget = new SeedGLWidget(this);
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(_glWidget);
	layout->setMargin(0);
	ui.dispFrame->setLayout(layout);

	_glWidget->setSeedData(_seedData);

	_seedCount = 0;
	_currentSeedIndex = -1;

	connectSignalSlots();
}

SeedVis::~SeedVis()
{

}


void SeedVis::setTensorData(TensorData * tensorData)
{
	_tensorData = tensorData;
	//_seedData->setVolumeData(tensorData);
	_glWidget->setVolumeData(tensorData);
	ui.seedTable->clearContents();
	addSeedRegion();
}

void SeedVis::connectSignalSlots()
{
	connect(ui.vpComboBox, SIGNAL(currentIndexChanged(int)), _glWidget, SLOT(setViewpoint(int)));
	connect(ui.dispComboBox, SIGNAL(currentIndexChanged(int)), _glWidget, SLOT(setDisplayMode(int)));
	connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addSeedRegion()));
	connect(ui.btnRemove, SIGNAL(clicked()), this, SLOT(handleRemoveRegion()));
	connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(handleSaveRegion()));
	connect(ui.btnLoad, SIGNAL(clicked()), this, SLOT(handleLoadRegion()));
	connect(ui.seedTable, SIGNAL(itemSelectionChanged()), this, SLOT(handleSeedSelectionChanged()));
	connect(_enableSeedMapper, SIGNAL(mapped(int)), this, SLOT(handleEnableRegion(int)));
	connect(_seedTypeMapper, SIGNAL(mapped(int)), this, SLOT(handleSetRegionType(int)));
	connect(ui.btnZoomIn, SIGNAL(clicked()), _glWidget, SLOT(zoomIn()));
	connect(ui.btnZoomOut, SIGNAL(clicked()), _glWidget, SLOT(zoomOut()));
	connect(ui.btnTrack, SIGNAL(clicked()), this, SIGNAL(fiberTrackingStarted()));
}

void SeedVis::showEvent(QShowEvent *e)
{
	if (!_tensorData)
	{
		return;
	}

	//ui.dispComboBox->setCurrentIndex(_tensorData->getDisplayMode());
	_glWidget->makeCurrent();
	_glWidget->setViewpoint(ui.vpComboBox->currentIndex());
	_glWidget->setDisplayMode(ui.dispComboBox->currentIndex());
	this->repaint();
}

void SeedVis::addSeedRegion ()
{
	if (_seedCount>=SeedData::MAX_SEED_COUNT)
	{
		return;
	}

	_currentSeedIndex = _seedCount;
	++_seedCount;

	ui.seedTable->setRowCount(_seedCount);

	QCheckBox * checkBox = new QCheckBox();
	ui.seedTable->setCellWidget(_currentSeedIndex, 0, checkBox);
	checkBox->setCheckState(Qt::Checked);
	connect(checkBox, SIGNAL(stateChanged(int)), _enableSeedMapper, SLOT(map()));
	_enableSeedMapper->setMapping(checkBox, _currentSeedIndex);

	QTableWidgetItem * item = new QTableWidgetItem(QString("Region %1").arg(_currentSeedIndex));
	ui.seedTable->setItem(_currentSeedIndex, 1, item);

	QComboBox * typeCombo = new QComboBox();
	QStringList typeStr;
	typeStr <<"Seed Region" <<"ROI";
	typeCombo->insertItems(0, typeStr);
	ui.seedTable->setCellWidget(_currentSeedIndex, 2, typeCombo);
	connect(typeCombo, SIGNAL(currentIndexChanged(int)), _seedTypeMapper, SLOT(map()));
	_seedTypeMapper->setMapping(typeCombo, _currentSeedIndex);

	uchar * color = _colorTable->getColor(_currentSeedIndex*1.0/(SeedData::MAX_SEED_COUNT-1));

	QTableWidgetItem * colorItem = new QTableWidgetItem(QString("(%1, %2, %3)").arg(color[0]).arg(color[1]).arg(color[2]));
	QPixmap colorIcon (20, 20);
	colorIcon.fill(QColor::fromRgb(color[0], color[1], color[2]));
	colorItem->setIcon(colorIcon);
	unsigned colorData = (color[0]<<16)|(color[1]<<8)|color[2];
	colorItem->setData(Qt::UserRole, colorData);
	ui.seedTable->setItem(_currentSeedIndex, 3, colorItem);

	_seedData->activateSeedBuf(_currentSeedIndex);
	_seedData->setColor(_currentSeedIndex, colorData);
	_seedData->setType(_currentSeedIndex, 0);

	ui.seedTable->selectRow(_currentSeedIndex);
}

void SeedVis::handleSeedSelectionChanged()
{

	int row = ui.seedTable->currentRow();
	if (row<0)
	{
		return;
	}

	/*QCheckBox * checkBox = (QCheckBox*)ui.seedTable->cellWidget(row, 0);
	if (!checkBox->isChecked())
	{
		ui.seedTable->selectRow(-1);
	} */

	_currentSeedIndex = row;
	QTableWidgetItem * item = ui.seedTable->item(row, 3);
	_glWidget->setCurrentSeed(row, item->data(Qt::UserRole).toUInt());
	ui.seedTable->scrollToItem(item);
}

void SeedVis::handleRemoveRegion()
{
	int row = ui.seedTable->currentRow();
	if (row<0)
	{
		return;
	}
	ui.seedTable->removeRow(row);
	_seedData->removeSeedBuf(row);
	_seedData->refreshSlices();
	--_seedCount;
	_currentSeedIndex = row>=_seedCount?_seedCount-1:row;
	ui.seedTable->setRowCount(_seedCount);
	ui.seedTable->selectRow(_currentSeedIndex);
	_glWidget->repaint();
}

void SeedVis::handleSaveRegion()
{
	int row = ui.seedTable->currentRow();
	QTableWidgetItem * item = ui.seedTable->item(row, 1);
	QString fileName = QFileDialog::getSaveFileName(this, "Save selected region", _currentFolder, "Text file (*.txt)");
	if (!fileName.isEmpty())
	{
		_seedData->saveSeedBuf(row, fileName);
	}
}

void SeedVis::handleLoadRegion()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Load region from file", _currentFolder, "Text file (*.txt)");
	if (!fileName.isEmpty())
	{
		_seedData->loadSeedBuf(_currentSeedIndex, fileName);
		_seedData->refreshSlices();
		_glWidget->repaint();
	}
}

void SeedVis::handleEnableRegion(int seedIndex)
{
	QCheckBox * checkBox = (QCheckBox*)ui.seedTable->cellWidget(seedIndex, 0);
	if (checkBox->isChecked())
	{
		QTableWidgetItem * item = ui.seedTable->item(seedIndex, 3);
		_seedData->activateSeedBuf(seedIndex);
		ui.seedTable->selectRow(seedIndex);
	} else
	{
		_seedData->deactivateSeedBuf(seedIndex);
		if (_currentSeedIndex==seedIndex)
		{
			int row = seedIndex+1>=_seedCount?0:seedIndex+1;
			ui.seedTable->selectRow(row);
		}
	}
	_seedData->refreshSlices();
	_glWidget->repaint();
}

void SeedVis::handleSetRegionType(int seedIndex)
{
	QComboBox * typeCombo = (QComboBox *)ui.seedTable->cellWidget(seedIndex, 2);
	int type = typeCombo->currentIndex();
	_seedData->setType(seedIndex, type);
}