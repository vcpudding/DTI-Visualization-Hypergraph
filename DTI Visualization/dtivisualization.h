#ifndef DTIVISUALIZATION_H
#define DTIVISUALIZATION_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include "ui_dtivisualization.h"

//#include "glwidget.h"
#include "DicomImageSet.h"
#include "volumevis.h"
#include "seedvis.h"
#include "fibervis.h"

using namespace DicomImageSet;

class DTIVisualization : public QMainWindow
{
	Q_OBJECT

public:
	DTIVisualization(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DTIVisualization();

private:
	Ui::DTIVisualizationClass ui;
	VolumeVis *		_volumeVis;
	SeedVis *		_seedVis;
	FiberVis *		_fiberVis;

private:
	void			connectSignalSlots();

private:
	QString			_currentFolder;
	QStringList		_selectedSeries;

protected slots:
	void			handleOpenFolder		();
	void			handleTreeItemClicked	(QTreeWidgetItem * item);
	void			handleLoadSeries		();
	void			handleLoadDTI			();
	void			handleSaveDTI			();
	void			handleFuseSeries		();
	void			handleFiberTracking		();
	void			handleTest				();
};

#endif // DTIVISUALIZATION_H
