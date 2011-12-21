#ifndef VOLUMEVIS_H
#define VOLUMEVIS_H

#include <QWidget>
#include "ui_volumevis.h"
#include "glwidget.h"
#include "TensorData.h"

class VolumeVis : public QWidget
{
	Q_OBJECT

public:
	VolumeVis(QWidget *parent = 0);
	~VolumeVis();

	friend class		DTIVisualization;

private:
	Ui::VolumeVisClass ui;

	GLWidget *			_glWidget;
	VolumeData *		_volData;
	VolumeData *		_fuseData;
	TensorData *		_tensorData;
	bool				_isTensorDataLoded;

	void				connectSignalSlots();

protected:
	void				showEvent(QShowEvent *e);

public:
	bool				isTensorDataLoaded () { return _isTensorDataLoded; }
	TensorData *		getTensorData () {return _tensorData;}

public slots:
	void				loadVolumeData (const QStringList & fileList);
	void				fuseVolumeData (const QStringList & fileList);
	void				loadDTIData (const QStringList & fileList, const QString & dtiFileName = "");
	int					saveTensorField (const QString & fileName);
	void				handleViewpointChanged ();
	void				handleSetInterpolate (bool bSet);
};

#endif // VOLUMEVIS_H
