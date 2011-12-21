#include <QtGui>
#include <QtOpenGL>
#include <QFileDialog>

#include <math.h>
#include <sstream>

#include <GL/glut.h>

#include "glwidget.h"
#include "track ball/trackball.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
	_zoom = -100.0;
	_translateX = 0;
	_translateY = 0;
	_volumeData = NULL;
	//_volumes = vector<VolumeData*>(0);
	_viewpoint = VP_FREE;

	_aSlicePos = _pSlicePos = _cSlicePos = 0;
	_bShowSagittal = _bShowCoronal = _bShowAxial = true;
	_planeWidth = 0;
	_bShowSliceIdx = true;
	
	_pressX = _pressY = 0;
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);

	gltbInit(GLUT_LEFT_BUTTON);
	gltbAnimate(false);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int anchorWidth = 200, anchorHeight = 200;

	glViewport(0,0,_widgetWidth-_planeWidth, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (_widgetWidth-_planeWidth)*1.0/_widgetHeight, 0.1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(_translateX, _translateY, _zoom);
	setRotationMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (_volumeData)
	{
		if (_bShowSagittal)
		{
			_volumeData->drawSagittalSlice();
		}
		if (_bShowCoronal)
		{
			_volumeData->drawCoronalSlice();
		}
		if (_bShowAxial)
		{
			_volumeData->drawAxialSlice();
		}
	}
	glPopMatrix();

	//glViewport(_widgetWidth-_planeWidth-anchorWidth, 0, anchorWidth, anchorHeight);
	drawAnchor();

	drawImageInfo();
	drawPlanes(_widgetWidth-_planeWidth, _planeWidth, _widgetHeight);
}

void GLWidget::resizeGL(int width, int height)
{
	if ( height == 0 )
	{
		height = 1;
	}

	_widgetWidth = width;
	_widgetHeight = height;
	gltbReshape(width, height);
}

void GLWidget::drawImageInfo()
{
	if (!_volumeData)
	{
		return;
	}
	glDisable(GL_DEPTH_TEST);
	GLint oldViewport [4];
	glGetIntegerv(GL_VIEWPORT,oldViewport);

	glViewport(0, 0, _widgetWidth, _widgetHeight);	
    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, _widgetWidth, 0, _widgetHeight);
    glMatrixMode(GL_MODELVIEW);

	glColor3f(1.0, 1.0, 1.0);
	const QStringList & infoList = _volumeData->getInfoStr();
	for (int i=0; i<infoList.count(); ++i)
	{
		drawString(infoList[i].toStdString().c_str(), 10, _widgetHeight-7*i, 12);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
}

void GLWidget::drawString(const char * s, float x, float y, float lineHeight)
{
	glRasterPos2f(x, y);
	const char *p = s;
	int lines = 1;
	for(; *p; ++p) {
		if (*p == '\n') 
		{
			lines++;
			glRasterPos2f(x, y-(lines*lineHeight));
		}
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *p);
	}
}

void GLWidget::wheelEvent ( QWheelEvent * event )
{
	if (!_volumeData)
	{
		return;
	}
	int hoverWin = 0;
	if (event->x()<_widgetWidth-_planeWidth)
	{
		return;
	} else
	{
		hoverWin = floor(event->y()*1.0 / (_widgetHeight/3.0));
	}

	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	if (event->orientation() == Qt::Vertical) {
		switch (hoverWin)
		{
		case 2:
			//Sagittal
			_pSlicePos += numSteps;
			_pSlicePos = _volumeData->setPSlicePrime(_pSlicePos);
			break;
		case 1:
			//Coronal
			_cSlicePos += numSteps;
			_aSlicePos = _volumeData->setCSlicePrime(_cSlicePos);	
			break;
		case 0:
			//Axial
			_aSlicePos += numSteps;
			_aSlicePos = _volumeData->setASlicePrime(_aSlicePos);			
			break;
		}
	}
	_pressX = event->x();
	_pressY = event->y();
	gltbMouse(GLUT_LEFT_BUTTON, GLUT_DOWN, _pressX, _pressY);
	gltbMotion(_pressX, _pressY);
	repaint();
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
	_posX = e->x();
	_posY = e->y();

	gltbMouse(GLUT_LEFT_BUTTON, GLUT_DOWN, e->x(), e->y());
	_pressX = e->x();
	_pressY = e->y();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() == Qt::LeftButton)
	{
		if (_pressX<_widgetWidth-_planeWidth)
		{
			if (_viewpoint!=VP_FREE)
			{
				emit viewpointChanged();
			}
			gltbMotion(e->x(), e->y());
		} else
		{
			//int delta = (e->y()-_posY)*8;
			/*QWheelEvent * e = new QWheelEvent(QPoint(e->x(), e->y()), e->y()-_posY, Qt::LeftButton, Qt::NoModifier);
			wheelEvent(e);*/
		}
	} else if (e->buttons()==Qt::RightButton)
	{
		_zoom -= e->y() - _posY;
		if (_zoom > 0)
			_zoom = 0;
		if (_zoom < -2000)
			_zoom = -2000;
		gltbMotion(_pressX, _pressY);
	} else if (e->buttons()==Qt::LeftButton|Qt::RightButton && _volumeData)
	{
		_volumeData->setWindow((e->x()-_posX)/10, (e->y()-_posY)/10);
		gltbMotion(_pressX, _pressY);
	}

	_posX = e->x();
	_posY = e->y();

	repaint();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	gltbMouse(GLUT_LEFT_BUTTON, GLUT_UP, e->x(), e->y());
}

void GLWidget::drawAnchor()
{
	int anchorWidth = 100;
	int anchorHeight = 100;

	GLint oldViewport [4];
	glGetIntegerv(GL_VIEWPORT,oldViewport);
	glViewport(_widgetWidth-anchorWidth, 0, anchorWidth, anchorHeight);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(30, anchorWidth*1.0/anchorHeight, 0.5, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0, 0, -10);
	setRotationMatrix();
	glScalef(2, 2, 2);

	glLineWidth(3.0);
	glBegin(GL_LINES);

	//left
	glColor3f(1, 0, 0);
	glVertex3f(_xDir, 0, 0);
	glVertex3f(0, 0, 0); 

	//front
	glColor3f(0, 1, 0);
	glVertex3f(0, _yDir, 0);
	glVertex3f(0, 0, 0);

	//top
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, _zDir);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
}

void GLWidget::drawPlanes(int left, int width, int height)
{
	int heightPerPlane = height/3;
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);

	char str [256];
	int slicePos;

	for (int i=0; i<3; ++i)
	{
		glViewport(left, i*heightPerPlane, width, heightPerPlane);

		if (_volumeData)
		{
			switch (i)
			{
			case 0:
				//Sagittal
				_volumeData->drawSagittalPlane(width, heightPerPlane, _bShowSliceIdx);
				break;
			case 1:
				//Coronal
				_volumeData->drawCoronalPlane(width, heightPerPlane, _bShowSliceIdx);
				break;
			case 2:
				//Axial
				_volumeData->drawAxialPlane(width, heightPerPlane, _bShowSliceIdx);
				break;
			}
		}

		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, heightPerPlane);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1,1,1);
		glBegin(GL_LINE_STRIP);
		glVertex2f(1,1);
		glVertex2f(width-1,1);
		glVertex2f(width-1,heightPerPlane-1);
		glVertex2f(1,heightPerPlane-1);
		glVertex2f(1,1);
		glEnd();
		switch (i)
		{
		case 0:
			slicePos = _volumeData?_volumeData->_pSliceIdx:0;
			drawString("Front", 5, heightPerPlane-5, 12);
			sprintf(str, "Slice #: %i", slicePos);
			break;
		case 1:
			slicePos = _volumeData?_volumeData->_cSliceIdx:0;
			drawString("Left", 5, heightPerPlane-5, 12);
			sprintf(str, "Slice #: %i", slicePos);
			break;
		case 2:
			slicePos = _volumeData?_volumeData->_aSliceIdx:0;
			drawString("Left", 5, heightPerPlane-5, 12);
			sprintf(str, "Slice #: %i", slicePos);
			break;
		}
		drawString(str, width-50, heightPerPlane-5, 12);
		glEnable(GL_DEPTH_TEST);
	}
}

void GLWidget::setVolumeData(VolumeData * volData)
{
	_volumeData = volData;
	_aSlicePos = volData->setASlicePrime(0);
	_pSlicePos = volData->setPSlicePrime(0);
	_cSlicePos = volData->setCSlicePrime(0);
}

void GLWidget::setDisplayMode(int m)
{
	if (_volumeData)
	{
		_volumeData->setDisplayMode(m);
		repaint();
	}
}

void GLWidget::setRotationMatrix ()
{
	gltbMatrix();
}

void GLWidget::setViewpoint(int v)
{
	_viewpoint = (Viewpoint)v;
	if (v!=VP_FREE)
	{
		gltbInitRotation (v);
	}
	repaint();
}

void GLWidget::setAxialSlice(int slice)
{
	_volumeData->setASlice(slice);
	//_aSlicePos = slice;
	repaint();
}

void GLWidget::setCoronalSlice(int slice)
{
	_volumeData->setCSlice(slice);
	//_cSlicePos = slice;
	repaint();
}

void GLWidget::setSagittalSlice(int slice)
{
	_volumeData->setPSlice(slice);
	//_pSlicePos = slice;
	repaint();
}