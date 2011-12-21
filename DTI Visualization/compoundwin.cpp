#include "compoundwin.h"

CompoundWin::CompoundWin(FiberData * fibData, Hypergraph * graph, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//_clusterColorer = new ColorTable();

	_fibWidget = new FiberGLWidget(this);
	_fibWidget->setFiberData(fibData);
	_graphWidget = new GraphGLWidget(this);
	_graphWidget->setGraph (graph);
	QHBoxLayout * fibLayout = new QHBoxLayout();
	fibLayout->setMargin(0);
	fibLayout->addWidget(_fibWidget);
	ui.fibFrame->setLayout(fibLayout);
	QHBoxLayout * graphLayout = new QHBoxLayout();
	graphLayout->setMargin(0);
	graphLayout->addWidget(_graphWidget);
	ui.graphFrame->setLayout(graphLayout);

	connect(_graphWidget, SIGNAL(selectionChanged(const vector<bool>&)), _fibWidget, SLOT(handleSelectionChanged(const vector<bool>&)));
	//connect(_graphWidget, SIGNAL(clustersChanged(const vector<int> &)), _fibWidget, SLOT(handleClustersChanged (const vector<int> &)));
	connect(_graphWidget, SIGNAL(clustersAssigned(int)), _fibWidget, SLOT(handleAssignClusters(int)));
	connect(_fibWidget, SIGNAL(graphUpdated(Hypergraph*)), _graphWidget, SLOT(handleGraphUpdated(Hypergraph*)));
	connect(_fibWidget, SIGNAL(clustersChanged(const vector<int> &)), _graphWidget, SLOT(handleClustersChanged (const vector<int> &)));
}

CompoundWin::~CompoundWin()
{
	//int nothing = 0;
}
