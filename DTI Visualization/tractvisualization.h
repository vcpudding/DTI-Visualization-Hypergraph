#ifndef TRACTVISUALIZATION_H
#define TRACTVISUALIZATION_H

#include <QMainWindow>
#include "ui_tractvisualization.h"
#include <QMdiSubWindow>
#include <QSignalMapper>
#include "fibervis.h"

class TractVisualization : public QMainWindow
{
	Q_OBJECT

public:
	TractVisualization(QWidget *parent = 0);
	~TractVisualization();

private:
	Ui::TractVisualizationClass ui;
	QString			TEST_DATA_PATH;
	QActionGroup*	_fiberActionGroup;
	QSignalMapper*	_fiberActionMapper;

	Hypergraph *	_graph;
	FiberData *		_fibData;
	//ColorTable *	_clusterColorer;

	void			connectSignalSlots ();
	FiberVis*		getActiveFibWin ();

private slots:
	void			handleOpenFiberFile ();
	void			handleSaveFiber ();
	void			handleSaveClusters();
	void			handleClusterEditor ();
	void			handleTemplateMaker ();
	void			handleActivateWindow (QMdiSubWindow *subWin);
	void			handleLoadNormFile ();
	void			handleEmbed ();
	void			handleTest ();
	void			handleAssignLabel();
	void			handleTractMDS();
	void			handleClusterMDS();
	void			handleExpResult();
	void			handleFiberOp (int op);
	void			handleResampleFib ();
	void			handleClusterKMeans();
	void			handleClusterKMeansWithPrune();
	void			handleSaveClusterCenters ();
	void			handleLoadClusters();
	void			handleLoadClusterCenters ();
	void			handleGetGraphKMeans ();
	void			handleGetGraphFuzzyCMeans ();
	void			handleSaveGraph();
	void			handleLoadGraph();
	void			handleLayoutGraph();
	void			handleLayoutPartitioned();
	void			handleSaveGraphLayout();
	void			handleLoadGraphLayout();
	void			handleShowGraph ();
	void			handleFastLayout ();
	void			handleClusteredLayout ();
	void			handleLengthFilter ();
	void			handleDownsampleRandom ();

	void			handleCompoundWin();
	void			handleClusterFuzzyCMeans();
	void			handleLoadFuzzyClusters();
	void			handleSaveFuzzyClusters();
	void			handleFiberWin();

	void			demoAtlasTract();
	void			demoTargetTracts();
	void			demoLabeledTract();
	void			demoWholeBrain();
	void			demoNumOfNeighbors();
};

#endif // TRACTVISUALIZATION_H
