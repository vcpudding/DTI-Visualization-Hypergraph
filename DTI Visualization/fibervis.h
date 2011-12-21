#ifndef FIBERVIS_H
#define FIBERVIS_H

#include <QWidget>
#include "ui_fibervis.h"
#include "fiberglwidget.h"

#include <vector>
using namespace std;

class FiberVis : public QWidget
{
	Q_OBJECT

public:
	FiberVis(QWidget *parent = 0);
	~FiberVis();

	friend					class TractVisualization;

private:
	Ui::FiberVisClass ui;
	FiberGLWidget *			_glWidget;
	FiberData *				_fiberData;
	TensorData *			_tensorData;
	SeedData *				_seedData;
	vector<int>				_clusterBuf;
	int						_currentClusterIdx;
	QString					_currentPath;

	void					connectSignalSlots();

protected:
	void					showEvent(QShowEvent *e);

public:
	void					startFiberTracking (SeedData * seedData, TensorData * tensorData, const TrackingConfig & config);
	static FiberVis *		openFiberFile (const QString &fileName, int sizeX = 144, int sizeY = 144, int sizeZ = 64);
	void					loadClusterFile (const QString &fileName);

public slots:
	void					handlePrevCluster ();
	void					handleNextCluster ();
	void					handleAllClusters ();

	void					handleOpenClusterFile ();
	void					handleOpenEmbedFile ();
	void					handleEmbed ();
	void					handleLoadNormFile ();
	void					handleAssignLabel();
};

#endif // FIBERVIS_H
