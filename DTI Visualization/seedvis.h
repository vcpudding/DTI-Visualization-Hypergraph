#ifndef SEEDVIS_H
#define SEEDVIS_H

#include <QWidget>
#include <QSignalMapper>
#include "ui_seedvis.h"
#include "TensorData.h"
#include "seedglwidget.h"
#include "ColorTable.h"

class SeedVis : public QWidget
{
	Q_OBJECT

public:
	SeedVis(QWidget *parent = 0);
	~SeedVis();

	friend class		DTIVisualization;

private:
	Ui::SeedVisClass ui;
	TensorData *		_tensorData;
	SeedData *			_seedData;
	SeedGLWidget *		_glWidget;
	ColorTable *		_colorTable;

	int					_currentSeedIndex;
	int					_seedCount;
	QString				_currentFolder;
	QSignalMapper *		_enableSeedMapper;
	QSignalMapper *		_seedTypeMapper;

	void				connectSignalSlots ();

private slots:
	void				addSeedRegion ();
	void				handleSeedSelectionChanged ();
	void				handleRemoveRegion ();
	void				handleSaveRegion ();
	void				handleLoadRegion ();
	void				handleEnableRegion (int seedIndex);
	void				handleSetRegionType (int seedIndex);

signals:
	void				fiberTrackingStarted ();

protected:
	//void				focusInEvent(QFocusEvent *e);
	void				showEvent (QShowEvent *e);

public:
	void				setTensorData (TensorData * tensorData);
	void				setCurrentFolder (const QString &folderName) {_currentFolder = folderName;}
};

#endif // SEEDVIS_H
