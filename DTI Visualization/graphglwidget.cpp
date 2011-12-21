#include "graphglwidget.h"

#include <gl/glut.h>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>

GraphGLWidget::GraphGLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	_graph = NULL;
	_pressX = _pressY = 0;
	_moveX = _moveY = 0;
	_bPicking = false;
	_mouseMove = MOUSE_CLICK;
	_gesture = GEST_CLICKDRAG;
	_selMode = SEL_NEW;
	_dispMode = DISP_GRAPH;
	_ctrlWidget = new GraphWidgetCtrl(this);
	_ctrlWidget->move(0,0);
	connectCtrlWidget();
}

GraphGLWidget::~GraphGLWidget()
{

}

void GraphGLWidget::connectCtrlWidget()
{
	//connect(_ctrlWidget->gestureMapper, SIGNAL(mapped(int)), this, SLOT(handleSetGesture(int)));
	//connect(_ctrlWidget->btnLinkage, SIGNAL(toggled(bool)), this, SLOT(handleInSelectLinkage(bool)));
	connect(_ctrlWidget->selModeMapper, SIGNAL(mapped(int)), this, SLOT(handleSetSelMode(int)));
	connect(_ctrlWidget->dispModeMapper, SIGNAL(mapped(int)), this, SLOT(handleSetDispMode(int)));
	connect(_ctrlWidget->btnAssign, SIGNAL(toggled(bool)), this, SLOT(handleAssignCluster(bool)));
}

void GraphGLWidget::initializeGL()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GraphGLWidget::setProjMatrix()
{
	int graphWidth = 1000;
	int graphHeight = 1000;
	float ratio1 = _widgetWidth*1.0/_widgetHeight;
	float ratio2 = graphWidth*1.0/graphHeight;
	if (ratio1>ratio2)
	{
		glOrtho((graphWidth-graphHeight*ratio1)/2.0, (graphWidth+graphHeight*ratio1)/2.0, 0, graphHeight, -100, 100);
	} else
	{
		glOrtho(0, graphWidth, (graphHeight-graphWidth/ratio1)/2.0, (graphHeight+graphWidth/ratio1)/2.0, -100, 100);
	}
}

void GraphGLWidget::resizeGL(int width, int height)
{
	_widgetWidth = width;
	_widgetHeight = height;

	glViewport(0, 0, _widgetWidth, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjMatrix();
}

void GraphGLWidget::paintGL()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!_graph)
	{
		return;
	}

	switch (_dispMode)
	{
	case DISP_GRAPH:
		_graph->drawHypergraph();
		break;
	case DISP_PINWEIGHTS:
		_graph->drawPinWeights();
		break;
	case DISP_PARTITIONS:
		_graph->drawPartitions();
		break;
	}

	if (_bPicking)
	{
		drawSelectionArea();
	}
}

void GraphGLWidget::mousePressEvent(QMouseEvent *e)
{
	_pressX = e->x();
	_pressY = e->y();
	_moveX = e->x();
	_moveY = e->y();
	_mouseMove = MOUSE_CLICK;
	switch (_gesture)
	{
	case GEST_CLICKDRAG:
		_bPicking = true;
		break;
	}
}

void GraphGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	_moveX = e->x();
	_moveY = e->y();
	_mouseMove = MOUSE_DRAG;
	repaint();
}

void GraphGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	bool *tmpSelectionBuf = NULL;

	if (_graph)
	{
		switch (_gesture)
		{
		case GEST_CLICKDRAG:
			if (_mouseMove==MOUSE_CLICK)
			{
				_selectedIdx = pick(_pressX, _pressY);
			} else
			{
				_selectedIdx = pick(_pressX, _pressY, _moveX, _moveY);
			}
			tmpSelectionBuf = _graph->selectVertices(_selectedIdx, _mouseMove);
			_graph->renewSelection(tmpSelectionBuf, _selMode);
			delete tmpSelectionBuf;

			emit selectionChanged(_graph->getSelectionBuf());
			_bPicking = false;
			break;
		case GEST_LINKAGE:
			if (_bPicking && _mouseMove==MOUSE_CLICK)
			{
				_selectedIdx = pick(_pressX, _pressY);
				_graph->setLinkageEdge(_selectedIdx);
			}
			break;
		}
	}
	repaint();
}

vector<int> GraphGLWidget::pick(int x, int y)
{
	GLdouble pickArea [4] = {x, y, 1, 1};
	const int BUFSIZE = 256;
	GLuint selectBuf [BUFSIZE];
	memset(selectBuf, 0, BUFSIZE*sizeof(GLuint));

	startPicking(BUFSIZE, selectBuf, pickArea);
	_graph->drawHypergraph();
	int hits = stopPicking();
	return processHits(hits, selectBuf);
}

vector<int> GraphGLWidget::pick(int pressX, int pressY, int moveX, int moveY)
{
	GLdouble pickArea[4];
	pickArea[0] = (pressX+moveX)/2;
	pickArea[1] = (pressY+moveY)/2;
	pickArea[2] = max(abs(moveX-pressX),1);
	pickArea[3] = max(abs(moveY-pressY),1);

	const int BUFSIZE = 10240;
	GLuint selectBuf [BUFSIZE];
	memset(selectBuf, 0, BUFSIZE*sizeof(GLuint));

	startPicking(BUFSIZE, selectBuf, pickArea);
	_graph->drawHypergraph();
	int hits = stopPicking();
	return processHits(hits, selectBuf);
}

vector<int> GraphGLWidget::pick(const vector<int> &clickBuf)
{
	vector<int> selectedIdx (0);
	for (int i=0; i<clickBuf.size()/2; ++i)
	{
		vector<int> clicked = pick(clickBuf[i*2], clickBuf[i*2+1]);
		selectedIdx.insert(selectedIdx.end(), clicked.begin(), clicked.end());
	}
	return selectedIdx;
}

void GraphGLWidget::startPicking(const int bufSize, GLuint selectBuf[], GLdouble pickArea [4])
{
	GLint viewport[4];
	float ratio;

	glSelectBuffer(bufSize,selectBuf);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix(pickArea[0],viewport[3]-pickArea[1],pickArea[2],pickArea[3],viewport);
	setProjMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void GraphGLWidget::startPicking(const int bufSize, GLuint selectBuf[]) 
{
	GLint viewport[4];
	float ratio;

	glSelectBuffer(bufSize,selectBuf);

	glGetIntegerv(GL_VIEWPORT,viewport);

	glRenderMode(GL_SELECT);

	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	if (_mouseMove==MOUSE_DRAG)
	{
		int cursorX = (_pressX+_moveX)/2;
		int cursorY = (_pressY+_moveY)/2;
		int width = max(abs(_moveX-_pressX),1);
		int height = max(abs(_moveY-_pressY),1);
		gluPickMatrix(cursorX,viewport[3]-cursorY,width,height,viewport);
	} else
	{
		gluPickMatrix(_pressX,viewport[3]-_pressY,1,1,viewport);
	}
	setProjMatrix();
	glMatrixMode(GL_MODELVIEW);
}

int GraphGLWidget::stopPicking() 
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	int hits = glRenderMode(GL_RENDER);
	return hits;
	/*if (hits != 0){
		processHits2(hits,selectBuf,0);
	}
	mode = RENDER;*/
}

vector<int> GraphGLWidget::processHits( int hits, GLuint selectBuf[] )
{
	vector<int> nameBuf(0);
	GLuint *ptr = selectBuf;
	for (int i=0; i<hits; ++i)
	{
		int nNames = *ptr;
		ptr += 3;
		for (int j=0; j<nNames; ++j)
		{
			nameBuf.push_back(*ptr);
			++ptr;
		}
	}
	return nameBuf;
}

void GraphGLWidget::handleSelectionChanged(const vector<bool> &selectionBuf)
{
	if (_graph)
	{
		_graph->setSelectionBuf(selectionBuf);
		repaint();
	}
}


void GraphGLWidget::handleSetGesture(int g)
{
	 _gesture = (Gesture)g;
}

void GraphGLWidget::drawSelectionArea()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, _widgetWidth, 0, _widgetHeight, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	switch (_gesture)
	{
	case GEST_CLICKDRAG:
		glColor4ub(0, 174, 239, 255);
		glBegin(GL_LINE_LOOP);
		glVertex2f(_pressX, _widgetHeight-_pressY);
		glVertex2f(_moveX, _widgetHeight-_pressY);
		glVertex2f(_moveX, _widgetHeight-_moveY);
		glVertex2f(_pressX, _widgetHeight-_moveY);
		glEnd();
		break;
	case GEST_LINKAGE:		
		break;
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void GraphGLWidget::handleInSelectLinkage(bool f)
{
	if (!f)
	{
		/*select linkages among selected edges*/
		bool *tmpSelectionBuf = _graph->selectLinkage();
		_graph->renewSelection(tmpSelectionBuf, _selMode);
		delete tmpSelectionBuf;

		emit selectionChanged(_graph->getSelectionBuf());
		repaint();
	}
	_bPicking = f;
}

void GraphGLWidget::handleSetSelMode(int m)
{
	_selMode = (SelectionMode)m;
}

void GraphGLWidget::handleAssignCluster(bool f)
{
	static Gesture oldGesture;
	if (f)
	{
		oldGesture = _gesture;
		handleSetGesture((int)GEST_LINKAGE);
		_bPicking = true;
	} else
	{
		int clusterIdx = _graph->getLinkage();
		if (clusterIdx!=-1)
		{
			emit clustersAssigned(clusterIdx);
		}
		_graph->resetLinkageBuf();

		handleSetGesture((int)oldGesture);
		_bPicking = false;
		repaint();
	}
}

void GraphGLWidget::handleGraphUpdated(Hypergraph *g)
{
	vector<int> diffSet = _graph->getGraphDifference(g);
	_graph->setEdges(g);
	vector<unsigned> distVec = _graph->getDistToModification(diffSet);
	_graph->getPinnedWeights(distVec);
	//_dispMode = DISP_PINWEIGHTS;
	int boundary [] = {0,1000,0,1000};
	//_graph->updateLayout(diffSet, boundary);
	//_graph->updateLayoutUnpinned(boundary);
	//_graph->updateLayoutPinned(boundary);
	_graph->layoutPartitioned(boundary, true);
}

void GraphGLWidget::handleSetDispMode(int d)
{
	_dispMode = (GraphDispMode)d;
	repaint();
}

void GraphGLWidget::handleClustersChanged(const vector<int> &clusters)
{
	_graph->setClusters(clusters);
}