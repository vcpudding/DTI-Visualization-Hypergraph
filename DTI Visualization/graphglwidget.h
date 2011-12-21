#ifndef GRAPHGLWIDGET_H
#define GRAPHGLWIDGET_H

#include <QGLWidget>
#include "Hypergraph.h"
#include "GraphWidgetCtrl.h"
#include <vector>

using namespace std;

class GraphGLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GraphGLWidget(QWidget *parent);
	~GraphGLWidget();

	enum			MouseMove {MOUSE_CLICK, MOUSE_DRAG};

private:
	int				_widgetWidth;
	int				_widgetHeight;
	Hypergraph *	_graph;
	int				_pressX, _pressY;
	int				_moveX, _moveY;
	MouseMove		_mouseMove;
	bool			_bPicking;
	Gesture			_gesture;
	SelectionMode	_selMode;
	GraphWidgetCtrl*_ctrlWidget;
	vector<int>		_selectedIdx;
	GraphDispMode		_dispMode;

protected:
	void			initializeGL();
	void			resizeGL(int width, int height);
	void			paintGL ();
	void			mousePressEvent(QMouseEvent *e);
	void			mouseMoveEvent(QMouseEvent *e);
	void			mouseReleaseEvent(QMouseEvent *e);

private:
	void			connectCtrlWidget ();
	void			drawSelectionArea ();

public:
	void			setGraph (Hypergraph *graph) {_graph = graph;}

	vector<int>		pick (int x, int y);
	vector<int>		pick (int pressX, int pressY, int moveX, int moveY);
	vector<int>		pick (const vector<int> &clickBuf);

	void			startPicking (const int bufSize, GLuint selectBuf[], GLdouble pickArea [4]);
	void			startPicking (const int bufSize, GLuint selectBuf[]);
	int				stopPicking();
	void			setProjMatrix ();
	vector<int>		processHits(int hits, GLuint selectBuf[]);

signals:
	void			selectionChanged (const vector<bool> &selectionBuf);
	void			clustersChanged (const vector<int> &clusters);
	void			clustersAssigned (int clusterIdx);

public slots:
	void			handleSelectionChanged (const vector<bool> &selectionBuf);
	void			handleSetGesture (int g);
	void			handleInSelectLinkage (bool f);
	void			handleSetSelMode (int m);
	void			handleAssignCluster (bool f);
	void			handleGraphUpdated (Hypergraph *g);
	void			handleSetDispMode (int d);
	void			handleClustersChanged (const vector<int> &clusters);
};

#endif // GRAPHGLWIDGET_H
