#include "tractvisualization.h"

#include "CylindProj.h"
#include "TensorData.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "clustereditor.h"
#include "expresultvis.h"
#include "graphglwidget.h"

//#include "mds.h"
#include "TemplateFiberData.h"
#include "compoundwin.h"


TractVisualization::TractVisualization(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//TEST_DATA_PATH = "D:\\Stella\\Research\\Sample data\\test data\\hypergraph\\";
	TEST_DATA_PATH = "D:\\Study\\Hypergraph\\hypergraph test data\\";

	_fiberActionGroup = new QActionGroup(this);
	_fiberActionGroup->addAction(ui.actionRotation);
	_fiberActionGroup->addAction(ui.actionSelect_fibers);
	_fiberActionGroup->addAction(ui.actionPan);
	_fiberActionGroup->setExclusive(true);
	ui.actionRotation->setChecked(true);

	_fiberActionMapper = new QSignalMapper(this);
	_fiberActionMapper->setMapping(ui.actionRotation, 0);
	_fiberActionMapper->setMapping(ui.actionSelect_fibers, 1);
	_fiberActionMapper->setMapping(ui.actionPan, 2);
	connectSignalSlots();
	_graph = new Hypergraph();
	_fibData = new FiberData();
	_graph->setClusterColorer(_fibData->getClusterColorer());
}

TractVisualization::~TractVisualization()
{

}

void TractVisualization::connectSignalSlots()
{
	connect(ui.actionOpen_tract_file, SIGNAL(triggered()), this, SLOT(handleOpenFiberFile()));
	connect(ui.actionCluster_Editor, SIGNAL(triggered()), this, SLOT(handleClusterEditor()));
	connect(ui.actionTemplate_Maker, SIGNAL(triggered()), this, SLOT(handleTemplateMaker()));
	connect(ui.actionEmbed, SIGNAL(triggered()), this, SLOT(handleEmbed()));
	connect(ui.actionTest, SIGNAL(triggered()), this, SLOT(handleTest()));
	connect(ui.actionLoad_norm_files, SIGNAL(triggered()), this, SLOT(handleLoadNormFile()));
	connect(ui.mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(handleActivateWindow(QMdiSubWindow*)));
	connect(ui.actionAssign_label, SIGNAL(triggered()), this, SLOT(handleAssignLabel()));
	connect(ui.actionTract_MDS, SIGNAL(triggered()), this, SLOT(handleTractMDS()));
	connect(ui.actionCluster_MDS, SIGNAL(triggered()), this, SLOT(handleClusterMDS()));
	connect(ui.actionExperiment_result, SIGNAL(triggered()), this, SLOT(handleExpResult()));
	//connect(_fiberActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(handleFiberOp(QAction*)));

	connect(ui.actionRotation, SIGNAL(triggered()), _fiberActionMapper, SLOT(map()));
	connect(ui.actionSelect_fibers, SIGNAL(triggered()), _fiberActionMapper, SLOT(map()));
	connect(ui.actionPan, SIGNAL(triggered()), _fiberActionMapper, SLOT(map()));
	connect(_fiberActionMapper, SIGNAL(mapped(int)), this, SLOT(handleFiberOp(int)));

	connect(ui.actionAtlas_tractography, SIGNAL(triggered()), this, SLOT(demoAtlasTract()));
	connect(ui.actionTarget_tract_embedding, SIGNAL(triggered()), this, SLOT(demoTargetTracts()));
	connect(ui.actionLabeled_tract_embedding, SIGNAL(triggered()), this, SLOT(demoLabeledTract()));
	connect(ui.actionGroup_comparison, SIGNAL(triggered()), this, SLOT(handleExpResult()));
	connect(ui.actionWhole_brain_tractography, SIGNAL(triggered()), this, SLOT(demoWholeBrain()));
	connect(ui.actionSave_tracts, SIGNAL(triggered()), this, SLOT(handleSaveFiber()));
	connect(ui.actionResample, SIGNAL(triggered()), this, SLOT(handleResampleFib()));
	connect(ui.actionK_means, SIGNAL(triggered()), this, SLOT(handleClusterKMeans()));
	connect(ui.actionSave_clusters, SIGNAL(triggered()), this, SLOT(handleSaveClusters()));
	connect(ui.actionSave_cluster_centers, SIGNAL(triggered()), this, SLOT(handleSaveClusterCenters()));
	connect(ui.actionLoad_clusters, SIGNAL(triggered()), this, SLOT(handleLoadClusters()));
	connect(ui.actionLoad_cluster_centers, SIGNAL(triggered()), this, SLOT(handleLoadClusterCenters()));
	connect(ui.actionK_means_graph, SIGNAL(triggered()), this, SLOT(handleGetGraphKMeans()));
	connect(ui.actionSave_hypergraph, SIGNAL(triggered()), this, SLOT(handleSaveGraph()));
	connect(ui.actionLoad_hypergraph, SIGNAL(triggered()), this, SLOT(handleLoadGraph()));
	connect(ui.actionSave_hypergraph_layout, SIGNAL(triggered()), this, SLOT(handleSaveGraphLayout()));
	connect(ui.actionLoad_hypergraph_layout, SIGNAL(triggered()), this, SLOT(handleLoadGraphLayout()));
	connect(ui.actionLayout_graph, SIGNAL(triggered()), this, SLOT(handleLayoutGraph()));
	connect(ui.actionPartitioned_layout, SIGNAL(triggered()), this, SLOT(handleLayoutPartitioned()));
	connect(ui.actionShow_graph, SIGNAL(triggered()), this, SLOT(handleShowGraph()));
	connect(ui.actionOpen_compound_window, SIGNAL(triggered()), this, SLOT(handleCompoundWin()));
	connect(ui.actionFast_layout, SIGNAL(triggered()), this, SLOT(handleFastLayout()));
	connect(ui.actionClustered_layout, SIGNAL(triggered()), this, SLOT(handleClusteredLayout()));
	connect(ui.actionLength_filter, SIGNAL(triggered()), this, SLOT(handleLengthFilter()));
	connect(ui.actionK_means_with_prune, SIGNAL(triggered()), this, SLOT(handleClusterKMeansWithPrune()));
	connect(ui.actionRandom, SIGNAL(triggered()), this, SLOT(handleDownsampleRandom()));
	connect(ui.actionFuzzy_c_means, SIGNAL(triggered()), this, SLOT(handleClusterFuzzyCMeans()));
	connect(ui.actionSave_fuzzy_clusters, SIGNAL(triggered()), this, SLOT(handleSaveFuzzyClusters()));
	connect(ui.actionLoad_fuzzy_clusters, SIGNAL(triggered()), this, SLOT(handleLoadFuzzyClusters()));
	connect(ui.actionGenerate_fuzzy_graph, SIGNAL(triggered()), this, SLOT(handleGetGraphFuzzyCMeans()));
	connect(ui.actionOpen_fiber_window, SIGNAL(triggered()), this, SLOT(handleFiberWin()));
}

void TractVisualization::handleOpenFiberFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Fiber File"), TEST_DATA_PATH, tr("Track Files (*.trk);;Text Files (*.txt);;Fiber Files (*.fib)"));

	if (!fileName.isEmpty())
	{
		if (fileName.contains(QRegExp(".trk$")))
		{
			_fibData->loadFibersFromTrk(fileName);
		} else
		{
			_fibData->loadFibers(fileName, 144, 124, 60);
		}
	}
}

void TractVisualization::handleClusterEditor()
{
	/*FiberVis* child = editTemplateClusters();
	ui.mdiArea->addSubWindow(child);
	child->showMaximized();*/
	/*ClusterEditor * child = new ClusterEditor();
	ui.mdiArea->addSubWindow(child);
	child->showMaximized();*/
}

void TractVisualization::handleTemplateMaker()
{
	embedTemplateClusters();
}

void TractVisualization::handleActivateWindow(QMdiSubWindow *subWin)
{
	if (!subWin)
	{
		//ui.menuTractography->setEnabled(false);
		return;
	}

	FiberVis *fibVis = qobject_cast<FiberVis *>(subWin->widget());
	
	bool isFibVisActive = false;
	if(fibVis)
	{
		isFibVisActive = true;
	}
	//ui.menuTractography->setEnabled(isFibVisActive);
}

void TractVisualization::handleEmbed()
{
	FiberVis *fibVis = getActiveFibWin();
	if (!fibVis)
	{
		return;
	}

	fibVis->handleEmbed();
}

void makeTestTracts ()
{
	ofstream outFile ("D:\\Stella\\Research\\Sample data\\test data\\hypergraph\\test tracts.txt");
	int i, j;

	for (i=0; i<10; ++i)
	{
		for (j=0; j<10; ++j)
		{
			outFile<< 5.0f+i*0.5f <<"\t";
			outFile<< 5.0f+j <<"\t";
			outFile<< 5.0f <<"\t";
		}
		outFile <<endl;
	}

	for (i=10; i<20; ++i)
	{
		for (j=0; j<10; ++j)
		{
			outFile<<  15.0f+i*0.5f <<"\t";
			outFile<<  5.0f+j <<"\t";
			outFile<<  5.0f <<"\t";
		}
		outFile <<endl;
	}

	for (i=20; i<30; ++i)
	{
		for (j=0; j<10; ++j)
		{
			outFile<< 7.5f+j <<"\t";
			outFile<< 7.5f+i*0.5f <<"\t";
			outFile<< 15.0f <<"\t";
		}
		outFile <<endl;
	}
	outFile.close();
}

void TractVisualization::handleTest()
{
	//_fibData->loadFibersFromTrk(QString("%1labeled.trk").arg(TEST_DATA_PATH));
	_fibData->loadFibersFromTrk(QString("%1whole brain tracks test.trk").arg(TEST_DATA_PATH));
	//_fibData->loadFibersFromTrk("D:\\Stella\\Research\\Sample data\\test data\\hypergraph\\labeled.trk");

	//_fibData->loadFuzzyClusters("D:\\Stella\\Research\\Sample data\\test data\\hypergraph\\labeled fuzzy cluster.fuz");
	//int seeds [5] = {45, 277, 556, 666, 928};
	//int seeds [20] = {3400, 2080, 2824, 1108, 4573, 410, 2692, 1455, 2168, 3008, 2841,2545,588,4366,112,4649,225,4993,2961,1236};
	int seeds [14] = {135,192,221,271,280,283,291,950,1008,1059,1238,1577,1688,1897};
	vector<int> seedBuf (14);
	seedBuf.assign(seeds, seeds+14);
	handleFiberWin();
	//_fibData->clusterFuzzyCMeans(false, seedBuf);
	_fibData->clusterAFCC(14, seedBuf);
	//_fibData->saveFuzzyClusters("D:\\Stella\\Research\\Sample data\\test data\\hypergraph\\whole brain cluster.fuz");
	//_fibData->saveFuzzyClusters("D:\\Stella\\Research\\Sample data\\test data\\hypergraph\\labeled fuzzy cluster.fuz");
	//_fibData->saveFuzzyClusters("D:\\Stella\\Research\\Sample data\\test data\\hypergraph\\whole brain test cluster.fuz");
	handleGetGraphFuzzyCMeans();
	handleLayoutPartitioned();
	//_graph->loadLayout(QString("%1whole brain test hypergraph.dat").arg(TEST_DATA_PATH));
	_graph->setClusters(_fibData->getClusters());
	_graph->setClusterColorer(_fibData->getClusterColorer());
	handleCompoundWin();
	//_fibData->loadFibers("D:\\Stella\\Research\\Sample data\\test data\\hypergraph\\test tracts.txt", 30, 30, 30);
	//handleFiberWin();
	//int seeds [] = {5,15,25};
	//vector<int> seedBuf (3);
	//seedBuf.assign(seeds, seeds+3);
	//_fibData->clusterFuzzyCMeansDTW(false, seedBuf);
	//vector<int> matching3;
	//float dist3 = fibDistDTWCov(_fibData->fiber(4), _fibData->fiber(277), 10, matching3);
	int stop = 1;
}

void TractVisualization::handleLoadNormFile()
{
	FiberVis* vis = getActiveFibWin();
	if (vis)
	{
		vis->handleLoadNormFile();
	}
}

FiberVis* TractVisualization::getActiveFibWin()
{
	QMdiSubWindow *activeSubWindow = ui.mdiArea->activeSubWindow();
	if (!activeSubWindow)
	{
		return NULL;
	}

	FiberVis *fibVis = qobject_cast<FiberVis *>(activeSubWindow->widget());
	return fibVis;
}

void TractVisualization::handleAssignLabel()
{
	FiberVis* vis = getActiveFibWin();
	if (vis)
	{
		vis->handleAssignLabel();
	}
}

void TractVisualization::handleTractMDS()
{
	TemplateFiberData::templateData->tractMDS();
}

void TractVisualization::handleClusterMDS()
{
	TemplateFiberData::templateData->clusterMDS();
}

void TractVisualization::handleExpResult()
{
	expResultVis *vis = new expResultVis(this);
	ui.mdiArea->addSubWindow(vis);
	vis->showMaximized();
}

void TractVisualization::demoAtlasTract()
{
	FiberVis * fibVis = FiberVis::openFiberFile("test data/tract/atlas/o_template_tract.fib", 181, 217, 181);
	fibVis->_fiberData->loadClusters("test data/tract/atlas/o_template_tract.cluster");
	fibVis->_fiberData->loadEmbeddings("test data/tract/atlas/o_template_tract.emb");
	fibVis->_fiberData->setColorMode(COLOR_ATLAS_CLUSTER);
	ui.mdiArea->addSubWindow(fibVis);
	fibVis->showMaximized();
	fibVis->_glWidget->setAxisDirs(1,-1,1);
	fibVis->_glWidget->repaint();
}

void TractVisualization::demoTargetTracts()
{
	FiberVis * fibVis = FiberVis::openFiberFile("test data/tract/test data/subject 01/cc_ic.fib", 144, 144, 60);
	//fibVis->_fiberData->loadClusters("test data/tract/atlas/o_template_tract.cluster");
	fibVis->_fiberData->loadEmbeddings("test data/tract/test data/subject 01/cc_ic.emb");
	fibVis->_fiberData->setColorMode(COLOR_DIR);
	ui.mdiArea->addSubWindow(fibVis);
	fibVis->showMaximized();
	fibVis->_glWidget->setEmbMinDistThres(20);
	fibVis->_glWidget->setAxisDirs(-1,-1,1);
	fibVis->_glWidget->repaint();
}

void TractVisualization::demoLabeledTract()
{
	FiberVis * fibVis = FiberVis::openFiberFile("test data/tract/test data/labeled/04/labeled.fib", 144, 144, 64);
	fibVis->_fiberData->loadClusters("test data/tract/test data/labeled/04/labeled.cluster");
	fibVis->_fiberData->loadEmbeddings("test data/tract/test data/labeled/04/labeled.emb");
	fibVis->_fiberData->setColorMode(COLOR_CLUSTER);
	ui.mdiArea->addSubWindow(fibVis);
	fibVis->showMaximized();
	fibVis->_glWidget->setEmbMinDistThres(50);
	fibVis->_glWidget->setAxisDirs(-1,-1,1);
	fibVis->_glWidget->repaint();
}

void TractVisualization::demoWholeBrain()
{
	FiberVis * fibVis = FiberVis::openFiberFile("test data/tract/test data/whole brain/whole brain.fib", 144, 144, 60);
	fibVis->_fiberData->loadClusters("test data/tract/test data/whole brain/whole brain.cluster");
	fibVis->_fiberData->loadEmbeddings("test data/tract/test data/whole brain/whole brain.emb");
	fibVis->_fiberData->setColorMode(COLOR_DIR);
	ui.mdiArea->addSubWindow(fibVis);
	fibVis->showMaximized();
	fibVis->_glWidget->setEmbMinDistThres(15);
	fibVis->_glWidget->setAxisDirs(-1,-1,1);
	fibVis->_glWidget->repaint();
}

void TractVisualization::demoNumOfNeighbors()
{

}

void TractVisualization::handleFiberOp(int op)
{
}

void TractVisualization::handleResampleFib()
{
	_fibData->resampleEqualSample(15);
}

void TractVisualization::handleSaveFiber()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, "Select file to save", TEST_DATA_PATH, "Track file (*.trk);;Text file (*.txt);;Fiber file (*.fib)");


	if (!fileName.isEmpty())
	{
		if (fileName.contains(QRegExp(".trk$")))
		{
			_fibData->saveFibersAsTrk(fileName);
		} else
		{
			_fibData->saveFibers(fileName);
		}
	}
}

void TractVisualization::handleClusterKMeans()
{
	bool ok;
	int k = QInputDialog::getInteger(this, "Select number of clusters", "k=", 10, 3, 500, 1, &ok);
	if (ok)
	{
		_fibData->clusterKMeans(k);
		_fibData->_bClustersLoaded = true;
		_fibData->initClusterColor();
		_fibData->genFiberDispList();
	}
}

void TractVisualization::handleClusterKMeansWithPrune()
{
	bool ok;
	int k = QInputDialog::getInteger(this, "Select number of clusters", "k=", 10, 3, 500, 1, &ok);
	if (ok)
	{
		_fibData->clusterKMeansWithPrune(k);
		_fibData->_bClustersLoaded = true;
		_fibData->initClusterColor();
		_fibData->genFiberDispList();
	}
}

void TractVisualization::handleSaveClusters()
{

	QString fileName = QFileDialog::getSaveFileName(NULL, "Select file to save", TEST_DATA_PATH, "Text file (*.txt)");
	if (!fileName.isEmpty())
	{
		_fibData->saveClusters(fileName);
	}
}

void TractVisualization::handleSaveClusterCenters()
{
	FiberVis* vis = getActiveFibWin();
	if (!vis)
	{
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(NULL, "Select file to save", TEST_DATA_PATH, "Text file (*.txt)");
	if (!fileName.isEmpty())
	{
		vis->_fiberData->saveKMeansClusterCenters(fileName);
	}
}

void TractVisualization::handleLoadClusters()
{
	/*FiberVis* vis = getActiveFibWin();
	if (!vis)
	{
		return;
	}*/

	QString fileName = QFileDialog::getOpenFileName(NULL, "Select file to open", TEST_DATA_PATH, "Text file (*.txt)");
	if (!fileName.isEmpty())
	{
		//vis->_fiberData->loadClusters(fileName);
		_fibData->loadClusters(fileName);
		_graph->setClusters(_fibData->getClusters());
	}

}

void TractVisualization::handleLoadClusterCenters()
{
	QString fileName = QFileDialog::getOpenFileName(NULL, "Select file to open", TEST_DATA_PATH, "Text file (*.txt)");
	if (!fileName.isEmpty())
	{
		_fibData->loadKMeansClusterCenters(fileName);
	}
}

void TractVisualization::handleGetGraphKMeans()
{
	/*FiberVis* vis = getActiveFibWin();
	if (!vis)
	{
		return;
	}*/
	bool ok;
	double tolerance = QInputDialog::getDouble(this, "Edge tolerance", "Tolerance:", 1000, 0, 10000, 10, &ok);
	if (ok)
	{
		//_fibData->resample(15);
		if (_graph)
		{
			delete _graph;
		}
		_fibData->setGraphThres(tolerance/10000);
		_graph = _fibData->getGraphKMeans(FiberData::DIST_MADAH);
		_graph->setClusterColorer(_fibData->getClusterColorer());
	}
}

void TractVisualization::handleSaveGraph()
{
	if (!_graph)
	{
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(NULL, "Select file to save", TEST_DATA_PATH, "Text file (*.txt)");
	if (!fileName.isEmpty())
	{
		_graph->saveHypergraph(fileName);
	}
}

void TractVisualization::handleLoadGraph()
{
	if (!_graph)
	{
		return;
	}

	QString fileName = QFileDialog::getOpenFileName(NULL, "Select file to open", TEST_DATA_PATH, "Text file (*.txt)");
	if (!fileName.isEmpty())
	{
		_graph->loadHypergraph(fileName);
	}

}

void TractVisualization::handleSaveGraphLayout()
{
	if (!_graph)
	{
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(NULL, "Select file to save", TEST_DATA_PATH, "Binary file (*.dat)");
	if (!fileName.isEmpty())
	{
		_graph->saveLayout(fileName);
	}
}

void TractVisualization::handleLoadGraphLayout()
{
	if (!_graph)
	{
		return;
	}

	QString fileName = QFileDialog::getOpenFileName(NULL, "Select file to open", TEST_DATA_PATH, "Binary file (*.dat)");
	if (!fileName.isEmpty())
	{
		_graph->loadLayout(fileName);
	}
}

void TractVisualization::handleLayoutGraph()
{
	if (!_graph)
	{
		return;
	}

	int boundary [] = {0,1000,0,1000};
	
	bool ok;
	float c = (float)QInputDialog::getDouble(this, "Set constant", "value:", 1.0, 0, 100.0, 0.1, &ok);
	if (ok)
	{
		_graph->layoutGraph(boundary, 2, c);
		handleShowGraph();
	}
}

void TractVisualization::handleShowGraph()
{
	GraphGLWidget * w = new GraphGLWidget(this);
	w->setGraph(_graph);
	ui.mdiArea->addSubWindow(w);
	w->showMaximized();
}

void TractVisualization::handleCompoundWin()
{
	CompoundWin *w = new CompoundWin(_fibData, _graph, this);
	//this->setCentralWidget(w);
	ui.mdiArea->addSubWindow(w);
	w->showMaximized();
}

void TractVisualization::handleFastLayout()
{
	if (!_graph)
	{
		return;
	}

	int boundary [] = {0,1000,0,700};
	_graph->layoutFast(boundary, 2, 3.0, 10, 20, 5000);
	handleShowGraph();
}

void TractVisualization::handleClusteredLayout()
{
	int boundary [] = {0,1000,0,700};
	_graph->layoutClustered(boundary, 500);
	handleShowGraph();
}

void TractVisualization::handleLengthFilter()
{
	bool ok;
	double minLength = QInputDialog::getDouble(this, "Length filter range", "Minimum length (mm):", 50, 0, 150, 1, &ok);
	if (ok)
	{
		double maxLength = QInputDialog::getDouble(this, "Length filter range", "Maximum length (mm):", 150, minLength, 500, 1, &ok);
		if (ok)
		{
			_fibData->filterByLength(minLength, maxLength);
		}
	}
}

void TractVisualization::handleDownsampleRandom()
{
	bool ok;
	int k = QInputDialog::getInteger(this, "Select number of samples", "k=", 500, 10, 8000, 1, &ok);
	if (ok)
	{
		_fibData->downSampleRandom(k);
	}
}

void TractVisualization::handleLayoutPartitioned()
{
	int boundary [] = {0,1000,0,1000};
	_graph->setClusters(_fibData->getClusters());
	_graph->setClusterColorer(_fibData->getClusterColorer());
	_graph->layoutPartitioned(boundary, false);
}

void TractVisualization::handleClusterFuzzyCMeans()
{
	int bCluster = QMessageBox::question(this, "Fuzzy c-means cluster", "Start fuzzy c-means clustering with selected seed fibers?", QMessageBox::Yes|QMessageBox::No);
	if (bCluster==QMessageBox::Yes)
	{
		vector<int> seedBuf = _fibData->getSelectedIdx();
		_fibData->clusterFuzzyCMeans(false, seedBuf);
	}
}

void TractVisualization::handleLoadFuzzyClusters()
{
	QString fileName = QFileDialog::getOpenFileName(NULL, "Select file to open", TEST_DATA_PATH, "Fuzzy cluster file (*.fuz)");
	if (!fileName.isEmpty())
	{
		_fibData->loadFuzzyClusters(fileName);
	}
}

void TractVisualization::handleSaveFuzzyClusters()
{

	QString fileName = QFileDialog::getSaveFileName(NULL, "Select file to save",TEST_DATA_PATH, "Fuzzy cluster file (*.fuz)");
	if (!fileName.isEmpty())
	{
		_fibData->saveFuzzyClusters(fileName);
	}
}

void TractVisualization::handleGetGraphFuzzyCMeans()
{
	if (_graph)
	{
		delete _graph;
	}
	_graph = _fibData->getGraphFuzzyCMeans();
}

void TractVisualization::handleFiberWin()
{
	FiberGLWidget * w = new FiberGLWidget(this);
	w->setFiberData(_fibData);
	ui.mdiArea->addSubWindow(w);
	w->showMaximized();
}