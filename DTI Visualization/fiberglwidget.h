#ifndef FIBERGLWIDGET_H
#define FIBERGLWIDGET_H

#include "glwidget.h"
#include "FiberData.h"
#include <QMouseEvent>
#include "common.h"
#include "FiberWidgetCtrl.h"

class FiberGLWidget : public GLWidget
{
	Q_OBJECT

public:
	FiberGLWidget(QWidget *parent);
	~FiberGLWidget();

	static enum			EmbOp {EMB_SELECT, EMB_MOVE, EMB_ZOOM, EMB_THRES, EMB_NULL};

private:
	FiberData *			_fiberData;
	TensorData *		_tensorData;
	SeedData *			_seedData;
	bool				_bShowVolumeData;
	bool				_bShowEmbeddings;
	bool				_bShowClusterLabels;
	bool				_bSelecting;
	bool				_bColorTplEmb;
	bool				_bClick;

	GLdouble			_embModelMatrix[16];
	GLdouble			_embProjMatrix[16];
	int					_embViewport[4];

	float				_embTranslateX, _embTranslateY;
	float				_embScale;
	EmbOp				_embOp;
	FiberOp				_fibOp;
	int					_showEmbType;
	float				_embMinDistThres;
	SelectionMode		_selMode;

	vector<int>			_penTrack;
	FiberWidgetCtrl *	_ctrlWidget;
	FibDispMode			_dispMode;
	
protected:
	void				initializeGL();
	void				paintGL();
	void				resizeGL(int w, int h);
	void				mousePressEvent(QMouseEvent *e);
	void				mouseMoveEvent(QMouseEvent *e);
	void				mouseReleaseEvent(QMouseEvent *e);

public:
	void				setFiberData (FiberData *fiberData) {_fiberData = fiberData;}
	void				setSeedData (SeedData *seedData) {_seedData = seedData;}
	void				setTensorData (TensorData *tensorData);
	void				setProjMatrix ();

	void				drawSelectionRegion ();
	void				drawEmbedPanel ();
	void				drawImageInfo ();
	void				drawPenTrack ();

	void				selectFibers (vector<int> &penTrack);
	void				connectCtrlWidget ();
	void				startPicking (GLdouble pickArea [4], int bufSize, GLuint selectBuf[]);
	int					stopPicking ();
	vector<int>			processHits (int hits, GLuint selectBuf []);
	vector<int>			processHitsClosest (int hits, GLuint selectBuf []);
	vector<int>			pick (int x, int y);
	vector<int>			pick (int pressX, int pressY, int moveX, int moveY);
	vector<int>			pick (const vector<int> &penTrack);

public slots:
	void				setShowVolumeData (bool bShow) {_bShowVolumeData = bShow; repaint();}
	void				setColorMode (int colorMode) {_fiberData->setColorMode(colorMode); repaint();}
	void				setFiberThickness (double thk) {_fiberData->setFiberThickness(thk); repaint();}
	void				setShowEmbeddings (bool bShow) {_bShowEmbeddings = bShow; repaint();}
	void				setShowClusterLabels (bool bShow) {_bShowClusterLabels = bShow; repaint();}
	void				setIsColorEmbByTpl (bool bSet) {_bColorTplEmb = bSet; repaint();}
	void				setDispEmbedType (bool bAmb) {_showEmbType = bAmb; repaint();}
	void				setEmbMinDistThres (double thres) {_embMinDistThres = thres; repaint();}
	void				handleSelectionChanged (const vector<bool> &selectionBuf);
	void				handleClustersChanged (const vector<int> &clusters);
	void				handleSetOperation (int op) {_fibOp = (FiberOp)op;}
	void				handleSetSelMode (int mode) {_selMode = (SelectionMode)mode;}
	void				handleAssignClusters (int clusterIdx);
	void				handleSetClrMode (int mode) {_fiberData->setColorMode(mode); repaint();}
	void				handleSetDispMode (int mode) {_dispMode = (FibDispMode)mode; repaint ();}

signals:
	void				selectionChanged (const vector<bool> &selectionBuf);
	void				clustersChanged (const vector<int> &clusters);
	void				graphUpdated (Hypergraph *g);
};

#endif // FIBERGLWIDGET_H
