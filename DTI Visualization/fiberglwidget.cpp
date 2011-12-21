#include <Windows.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "fiberglwidget.h"
#include "track ball/trackball.h"
#include "TemplateFiberData.h"
#include "FiberWidgetCtrl.h"
#include <QMessageBox>

FiberGLWidget::FiberGLWidget(QWidget *parent)
	: GLWidget(parent)
{
	_fiberData = new FiberData();
	_tensorData = NULL;
	_bShowVolumeData = false;
	_bShowEmbeddings = false;
	_bSelecting = false;
	_bColorTplEmb = false;
	_bShowClusterLabels = false;
	_bColorTplEmb = false;
	_embTranslateX = _embTranslateY = 0;
	_embScale = 1;
	_embOp = EMB_NULL;
	_fibOp = OP_NAV;
	_showEmbType = 0;
	_embMinDistThres = 50;
	_selMode = SEL_NEW;
	_dispMode = DISP_FIBERS;

	_ctrlWidget = new FiberWidgetCtrl(this);
	_ctrlWidget->move(0,0);
	connectCtrlWidget();
}

void FiberGLWidget::connectCtrlWidget()
{
	connect(_ctrlWidget->fibOpMapper, SIGNAL(mapped(int)), this, SLOT(handleSetOperation(int)));
	connect(_ctrlWidget->selModeMapper, SIGNAL(mapped(int)), this, SLOT(handleSetSelMode(int)));
	connect(_ctrlWidget->clrModeMapper, SIGNAL(mapped(int)), this, SLOT(handleSetClrMode(int)));
	connect(_ctrlWidget->dispModeMapper, SIGNAL(mapped(int)), this, SLOT(handleSetDispMode(int)));
}

FiberGLWidget::~FiberGLWidget()
{
	
}

void FiberGLWidget::setProjMatrix()
{
	gluPerspective(60.0, _widgetWidth*1.0/_widgetHeight, 0.1, 10000);
}

void FiberGLWidget::initializeGL()
{
	GLWidget::initializeGL();

	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	//Enable Lighting
	//glEnable(GL_LIGHTING);

	////Specify a single directional light
	//float color_intensity=0.8;
	//GLfloat light_color[] = { color_intensity,color_intensity,color_intensity,1.0};
	////GLfloat light_color[] = { 1.0f,1.0f,1.0f,1.0f};
	//GLfloat light_pos[] = { 1.0f,1.0f,1.0f,0.0f};

	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);	

	//glEnable(GL_LIGHT0);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	//glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);
	//glDisable(GL_LIGHTING);
}

void FiberGLWidget::resizeGL(int w, int h)
{
	GLWidget::resizeGL(w, h);
	_widgetWidth = w;
	_widgetHeight = h;
	glViewport(0,0,_widgetWidth, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void FiberGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int anchorWidth = 120, anchorHeight = 120;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(_translateX, _translateY, _zoom);
	setRotationMatrix();

	/*glEnable(GL_DEPTH_TEST);
	glColor3f(1,1,1);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (_tensorData && _bShowVolumeData)
	{
		if (_bShowSagittal)
		{
			_tensorData->drawSagittalSlice();
		}
		if (_bShowCoronal)
		{
			_tensorData->drawCoronalSlice();
		}
		if (_bShowAxial)
		{
			_tensorData->drawAxialSlice();
		}
	}*/
	if (_fiberData)
	{
		switch (_dispMode)
		{
		case DISP_FIBERS:
			_fiberData->drawFibers();
			break;
		case DISP_CLUSTER_CENTERS:
			_fiberData->drawClusterCenters();
			break;
		case DISP_MATCHINGS:
			_fiberData->drawMatchings();
			break;
		}
	}
	glPopMatrix();

	//glViewport(_widgetWidth-anchorWidth, 0, anchorWidth, anchorHeight);
	drawAnchor();
	/*drawImageInfo();*/
	if (_fibOp==OP_SEL)
	{
		drawPenTrack();
	}
}

void FiberGLWidget::setTensorData(TensorData *tensorData)
{
	GLWidget::setVolumeData(tensorData);
	_tensorData = tensorData;
}

void FiberGLWidget::mousePressEvent(QMouseEvent *e)
{
	_pressX = e->x();
	_pressY = e->y();
	_posX = e->x();
	_posY = e->y();
	_bClick = true;

	if (_fibOp==OP_NAV)
	{
		gltbMouse(GLUT_LEFT_BUTTON, GLUT_DOWN, e->x(), e->y());
	}
}

void FiberGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	_bClick = false;
	switch (_fibOp)
	{
	case OP_NAV:
		if (e->buttons()==Qt::LeftButton)
		{
			gltbMotion(e->x(), e->y());
		} else if (e->buttons()==Qt::RightButton)
		{
			_zoom -= e->y() - _posY;
			_zoom = _zoom>0?0:(_zoom<-2000?2000:_zoom);
			gltbMotion(_pressX, _pressY);
		}
		break;
	case OP_SEL:
		if (e->buttons()==Qt::LeftButton)
		{
			_penTrack.push_back(e->x());
			_penTrack.push_back(e->y());
		}
		break;
	}
	_posX = e->x();
	_posY = e->y();
	repaint();
}

void FiberGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch (_fibOp)
	{
	case OP_NAV:
		gltbMouse(GLUT_LEFT_BUTTON, GLUT_UP, e->x(), e->y());
		break;
	case OP_SEL:
		vector<int> selectedIdx;
		if (_bClick)
		{
			selectedIdx = pick(e->x(), e->y());
		} else
		{
			selectedIdx = pick(_penTrack);
		}
		if (!selectedIdx.empty())
		{
			QMessageBox::information(this, "Selected fiber", QString::number(selectedIdx[0]));
		}
		_fiberData->selectFibers(selectedIdx, _selMode);
		emit selectionChanged(_fiberData->getSelectionBuf());
		_penTrack.clear();
		break;
	}
	repaint();
}

void FiberGLWidget::drawSelectionRegion()
{
	if (_embOp != EMB_SELECT)
	{
		return;
	}
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(5, 505, _widgetHeight-5, _widgetHeight-505);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4f(1.0, 1.0, 0.0, 0.5);
	glBegin(GL_QUADS);
	glVertex2f(_pressX, _pressY);
	glVertex2f(_pressX, _posY);
	glVertex2f(_posX, _posY);
	glVertex2f(_posX, _pressY);
	glEnd();
}

void FiberGLWidget::drawPenTrack()
{
	if (_penTrack.empty())
	{
		return;
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	GLint oldViewport [4];
	glGetIntegerv(GL_VIEWPORT,oldViewport);
	glViewport(0, 0, _widgetWidth, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, _widgetWidth, _widgetHeight, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0, 1.0, 0.0);
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	for (int i=0; i<_penTrack.size()/2; ++i)
	{
		glVertex2f(_penTrack[i*2], _penTrack[i*2+1]);
	}
	glEnd();
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
}

void FiberGLWidget::drawEmbedPanel()
{
	if (!_fiberData)
	{
		return;
	}
	int width = 500;
	int height = 500;

	glViewport(5, 5, width, height);
	glDisable(GL_DEPTH_TEST);

	const float PI = 3.1415926;
	float w = 2.0*PI;
	float h = 1.1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w/h>width*1.0f/height)
	{
		h = w*height/width;
	} else
	{
		w = h*width/height;
	}
	glOrtho(w/2, -w/2, -h/2, h/2, -50, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glScalef(_embScale, _embScale, _embScale);
	glTranslatef(_embTranslateX, _embTranslateY, 0);

	glColor3f(1,1,1);
	glPushMatrix();
	glScalef(w, h, 1);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xF0F0); 
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(0, -h/2);
	glVertex2f(0, h/2);
	glVertex2f(-w/2, 0);
	glVertex2f(w/2, 0);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	TemplateFiberData::drawAtlasEmbeds(1, _bShowClusterLabels, _bColorTplEmb);
	_fiberData->drawEmbeddings(_showEmbType, _embMinDistThres);

	if (_bSelecting)
	{
		glGetIntegerv(GL_VIEWPORT, _embViewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, _embModelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, _embProjMatrix);
	}
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.5, 0.5, -0.5, 0.5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1,1,1);
	glutWireCube(1);
}

void FiberGLWidget::drawImageInfo()
{

	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, _widgetWidth, _widgetHeight);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, _widgetWidth, 0, _widgetHeight);
	glMatrixMode(GL_MODELVIEW);

	glColor3f(1.0, 1.0, 1.0);
	char str [1024];
	sprintf_s(str, 1024, "Tract count: %d / %d\nAmbiguity threshold: %f", _fiberData->nSelected(), _fiberData->nFibers(), _embMinDistThres);
	GLWidget::drawString(str, 10, _widgetHeight-20, 12);
}

void FiberGLWidget::selectFibers(vector<int> &penTrack)
{
	if (penTrack.empty())
	{
		return;
	}

	////for (vector<int>::iterator it=penTrack.begin(); it!=penTrack.end(); ++it)
	//vector<int>::iterator it = penTrack.begin()+2;
	//int i=1;
	//while(it!=penTrack.end())
	//{
	//	int dist = max(abs(penTrack[i*2]-penTrack[(i-1)*2]), abs(penTrack[i*2+1]-penTrack[(i-1)*2+1]));
	//	if (dist>4)
	//	{
	//		float step = 2.0 / dist;
	//		vector<int> interp;
	//		for (float t=step; t<=1.0-step; t+=step)
	//		{
	//			int x = t*penTrack[i*2]+(1-t)*penTrack[(i-1)*2];
	//			int y = t*penTrack[i*2+1]+(1-t)*penTrack[(i-1)*2+1];
	//			penTrack.insert(it, x);
	//			penTrack.insert(it, y);
	//		}
	//	}
	//	it += 2;
	//	i = (it-penTrack.begin())/2;
	//}

	const int SELECT_BUFSIZE = 1024;
	GLuint select_buf[SELECT_BUFSIZE]; //selection buffer
	GLuint hits; //number of hits
	GLint viewport[4];

	glSelectBuffer(SELECT_BUFSIZE, select_buf);

	glGetIntegerv(GL_VIEWPORT, viewport);

	vector<int> selectedFibs(0);
	for (int i=0; i<penTrack.size()/2; ++i)
	{
		int x = penTrack[i*2];
		int y = _widgetHeight-penTrack[i*2+1];
		float pickRegionSize = 10;
		glRenderMode(GL_SELECT);

		glInitNames();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix(x, y, pickRegionSize, pickRegionSize, viewport);
		gluPerspective(60.0, _widgetWidth*1.0/_widgetHeight, 0.1, 10000);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(_translateX, _translateY, _zoom);
		setRotationMatrix();
		_fiberData->drawFibers();
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		hits = glRenderMode(GL_RENDER); //switch back to RENDER mode

		GLuint *ptr = select_buf;
		for (int j=0; j<hits; ++j)
		{
			int nNames = *ptr;
			ptr += 3;
			for (int j=0; j<nNames; ++j)
			{
				selectedFibs.push_back(*ptr);
				++ptr;
			}
		}
	}
	//_fiberData->selectFibers(selectedFibs);
	//_penTrack.clear();
}

void FiberGLWidget::startPicking(GLdouble pickArea [4], int bufSize, GLuint selectBuf[])
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

int FiberGLWidget::stopPicking()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	int hits = glRenderMode(GL_RENDER);
	return hits;
}

vector<int> FiberGLWidget::processHits(int hits, GLuint selectBuf [])
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

vector<int> FiberGLWidget::processHitsClosest(int hits, GLuint selectBuf [])
{
	GLuint *ptr = selectBuf;
	GLuint minZ = 0xffffffff;
	int nClosetsNames;
	GLuint *ptrClosestNames=NULL;

	for (int i = 0; i < hits; i++) {	
		int nNames = *ptr;
		ptr++;
		if (*ptr < minZ) {
			nClosetsNames = nNames;
			minZ = *ptr;
			ptrClosestNames = ptr+2;
		}

		ptr += nNames+2;
	}
	
	vector<int> nameBuf (0);
	if (ptrClosestNames)
	{
		for (int i=0; i<nClosetsNames; ++i)
		{
			nameBuf.push_back(*(ptrClosestNames+i));
		}
	}
	return nameBuf;
}

vector<int> FiberGLWidget::pick(int x, int y)
{
	GLdouble pickArea [4] = {x, y, 5, 5};
	const int BUFSIZE = 256;
	GLuint selectBuf [BUFSIZE];
	memset(selectBuf, 0, BUFSIZE*sizeof(GLuint));
	startPicking(pickArea, BUFSIZE, selectBuf);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(_translateX, _translateY, _zoom);
	setRotationMatrix();
	_fiberData->drawFibers();
	glPopMatrix();

	int hits = stopPicking();
	return processHitsClosest(hits, selectBuf);
}

vector<int> FiberGLWidget::pick(int pressX, int pressY, int moveX, int moveY)
{
	GLdouble pickArea[4];
	pickArea[0] = (pressX+moveX)/2;
	pickArea[1] = (pressY+moveY)/2;
	pickArea[2] = max(abs(moveX-pressX),1);
	pickArea[3] = max(abs(moveY-pressY),1);

	const int BUFSIZE = 10240;
	GLuint selectBuf [BUFSIZE];
	memset(selectBuf, 0, BUFSIZE*sizeof(GLuint));

	startPicking(pickArea, BUFSIZE, selectBuf);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(_translateX, _translateY, _zoom);
	setRotationMatrix();
	_fiberData->drawFibers();
	glPopMatrix();
	int hits = stopPicking();
	return processHitsClosest(hits, selectBuf);
}

vector<int> FiberGLWidget::pick(const vector<int> &penTrack)
{
	GLdouble pickArea [4];
	vector<int> selectedFibs(0);
	for (int i=1; i<penTrack.size()/2; ++i)
	{
		vector<int> tmpBuf = pick(penTrack[(i-1)*2], penTrack[(i-1)*2+1], penTrack[i*2], penTrack[i*2+1]);
		selectedFibs.insert(selectedFibs.end(), tmpBuf.begin(), tmpBuf.end());
	}
	return selectedFibs;
}

void FiberGLWidget::handleSelectionChanged(const vector<bool> &selectionBuf)
{
	if (_fiberData)
	{
		_fiberData->setSelectionBuf(selectionBuf);
		repaint();
	}
}

void FiberGLWidget::handleClustersChanged(const vector<int> &clusters)
{
	vector<bool> bClusterChanged;
	_fiberData->setClusters(clusters, bClusterChanged);
	//_fiberData->updateClusterCenters();
	//_fiberData->initClusterMedoids();
	//_fiberData->initClusterColor();
	Hypergraph *g = _fiberData->getGraphKMeans(FiberData::DIST_MADAH, bClusterChanged);
	emit graphUpdated(g);
	repaint();
}

void FiberGLWidget::handleAssignClusters(int clusterIdx)
{
	_fiberData->assignSelectedToCluster(clusterIdx);
	_fiberData->clusterFuzzyCMeans(true, vector<int>(), 1);
	_fiberData->getClustersFromFuzzyClusters();
	emit clustersChanged(_fiberData->getClusters());

	Hypergraph *g = _fiberData->getGraphFuzzyCMeans();
	emit graphUpdated(g);
	repaint();
}