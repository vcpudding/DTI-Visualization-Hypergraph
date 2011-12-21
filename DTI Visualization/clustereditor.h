#ifndef CLUSTEREDITOR_H
#define CLUSTEREDITOR_H

#include <QWidget>
#include "ui_clustereditor.h"
#include "fiberglwidget.h"

class ClusterEditor : public QWidget
{
	Q_OBJECT

public:
	ClusterEditor(QWidget *parent = 0);
	~ClusterEditor();

private:
	Ui::ClusterEditorClass ui;

	FiberGLWidget *			_glWidget;
	FiberData *				_fiberData;
	vector<int>				_originalClusters;
	bool					_bFiberLoaded;
	bool					_bClusterLoaded;

private:
	void					connectSignalSlots();

public slots:
	void					handleOpenFiberFile();
	void					handleOpenClusterFile();
	void					handleAssign();
	void					handleSave();
	//void					handleSelectOriginalCluster();
	void					handleSelectFibers(int row, int col);
};

#endif // CLUSTEREDITOR_H
