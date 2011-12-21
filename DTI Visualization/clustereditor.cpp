#include "clustereditor.h"

ClusterEditor::ClusterEditor(QWidget *parent)
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

	_bFiberLoaded = false;
	_bClusterLoaded = false;
}

ClusterEditor::~ClusterEditor()
{

}

void ClusterEditor::connectSignalSlots()
{
	connect(ui.btnFiberFile, SIGNAL(clicked()), this, SLOT(handleOpenFiberFile()));
	connect(ui.btnClusterFile, SIGNAL(clicked()), this, SLOT(handleOpenClusterFile()));
	connect(ui.btnAssign, SIGNAL(clicked()), this, SLOT(handleAssign()));
	connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(handleSave()));
}

void ClusterEditor::handleOpenFiberFile()
{
	/*QString fileName = QFileDialog::getOpenFileName(this, tr("Open Fiber File"), "test data/tract", tr("Fiber Files (*.fib)"));

	if (!fileName.isEmpty())
	{
		_fiberData->loadFibers(fileName);
		ui.lineFiberFile->setText(fileName);
		_bFiberLoaded = true;
	}*/
}

void ClusterEditor::handleOpenClusterFile()
{
	/*QString fileName = QFileDialog::getOpenFileName(this, tr("Open Cluster File"), "test data/tract", tr("Cluster files (*.cluster)"));

	if (!fileName.isEmpty())
	{
		_fiberData->loadClusters(fileName);
		ui.lineClusterFile->setText(fileName);
		_bClusterLoaded = true;

		_originalClusters.resize(_fiberData->_fibers.size());
		_originalClusters.assign(_fiberData->_clusters, _fiberData->_clusters+_originalClusters.size());
	}*/
}

void ClusterEditor::handleAssign()
{
	/*if (!_bClusterLoaded || !_bFiberLoaded)
	{
		return;
	}

	int selectedLabel = ui.listMajorBundles->selectedIndexes()[0];
	_fiberData->assignLabel(selectedLabel);*/
}

void ClusterEditor::handleSave()
{
	/*QString fileName = QFileDialog::getSaveFileName(this, tr("Save Cluster File"), "test data/tract", tr("Cluster files (*.cluster)"));

	if (!fileName.isEmpty())
	{
		_fiberData->saveClusters(fileName);
		this->hide();
	}*/
}

void ClusterEditor::handleSelectFibers(int row, int col)
{
	//if (col == 0)
	//{
	//	//select by original cluster
	//	int clusterIdx = ui.tableResult->item(row, col)->data(Qt::UserRole);
	//	memset(_fiberData->_selectedBuf, 0, _fiberData->_fibers.size()*sizeof(bool));
	//	for (int i=0; i<_originalClusters.size(); ++i)
	//	{
	//		if (_originalClusters[i] == clusterIdx)
	//		{
	//			_fiberData->_selectedBuf[i] = true;
	//		}
	//	}
	//} else
	//{
	//	//select by new label
	//	int clusterIdx = ui.tableResult->item(row, col)->data(Qt::UserRole);
	//	_fiberData->selectFibers(clusterIdx);
	//}
	//repaint();
}