#ifndef EXPRESULTVIS_H
#define EXPRESULTVIS_H

#include <QGLWidget>
#include "ui_expresultvis.h"
#include "FiberData.h"
#include "ColorTable.h"

class expResultVis : public QGLWidget
{
	Q_OBJECT

public:
	expResultVis(QWidget *parent = 0);
	~expResultVis();

private:
	Ui::expResultVisClass ui;
	int					_widgetWidth, _widgetHeight;
	vector<FloatPoint>	_atlasMDS;
	vector<CylindPoint>	_atlasTractEmbs;
	vector<CylindPoint>	_atlasClusterEmbs;
	FiberData *			_labeledFiberData;
	vector<CylindPoint>	_healthyEmbeds;
	vector<CylindPoint>	_subjectEmbeds;

	float *				_ordinaryMDS;
	float *				_constrainedMDS;
	float *				_fibToRegDist;
	int *				_clusters;
	ColorTable *		_colorTable;
	FiberData *			_fiberData;

protected:
	void				initializeGL();
	void				resizeGL(int width, int height);
	void				paintGL();

public:
	void				drawAtlasClusterMDS ();
	void				drawAtlasTract ();
	void				drawAtlasTractEmbs();
	void				drawLabeledEmbs();
	void				drawGroupEmbs();

	void				loadEmbFile (const QString &fileName, vector<CylindPoint> &ptBuf);

	void				drawOrdinaryAndConstrainedMDS ();
};

#endif // EXPRESULTVIS_H
