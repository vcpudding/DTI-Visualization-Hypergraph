#include <gl/glew.h>
#include "seedglwidget.h"

#include <gl/glut.h>
#include "track ball/trackball.h"
#include "tess.h"
#include <math.h>

#include <QProgressDialog>
#include <QMessageBox>

SeedGLWidget::SeedGLWidget(QWidget *parent)
	: GLWidget(parent)
{
	_tensorData = NULL;
	_seedData = NULL;
	_viewpoint = VP_AXIAL;
	_viewportHeight = 200;

	_penTrack = vector<Point>(0);
	_bDrawing = false;

	_fboId = 0;
	_texId = 0;

	_currentSeedIndex = 0;
	_bAddRegion = true;
	//_seedCount = 0;
}

SeedGLWidget::~SeedGLWidget()
{

}

void SeedGLWidget::initializeGL()
{
	GLWidget::initializeGL();
	//glDepthFunc(GL_LEQUAL);
	initTess();
	if (!initFBO())
	{
		QMessageBox::warning(this, "FBO error", "Cannot initialize FBO");
	}
}

void SeedGLWidget::resizeGL(int width, int height)
{
	GLWidget::resizeGL(width, height);
}

void SeedGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewportWidth = _viewportHeight*(_widgetWidth-_planeWidth)*1.0/_widgetHeight;

	int anchorWidth = 200, anchorHeight = 200;

	glViewport(0,0,_widgetWidth-_planeWidth, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewportWidth, 0, _viewportHeight, -2000, 2000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glTranslatef(_translateX, _translateY, 0);
	glTranslatef(viewportWidth/2.0, _viewportHeight/2.0, 0);
	setRotationMatrix();

	glColor3f(1, 1, 1);
	if (_volumeData)
	{
		switch (_viewpoint)
		{
		case VP_AXIAL:
			_volumeData->drawAxialSlice();
			break;
		case VP_CORONAL:
			_volumeData->drawCoronalSlice();
			break;
		case VP_SAGITTAL:
			_volumeData->drawSagittalSlice();
			break;
		}
	}

	drawSeedRegion();

	glPopMatrix();

	if (_bDrawing)
	{
		drawPenTrack();
	}

	glViewport(_widgetWidth-_planeWidth-anchorWidth, 0, anchorWidth, anchorHeight);
	drawAnchor();

	drawImageInfo();
	drawPlanes(_widgetWidth-_planeWidth, _planeWidth, _widgetHeight);
}

void SeedGLWidget::setViewpoint(int v)
{
	//_viewpoint = (GLWidget::Viewpoint)(v+1);
	GLWidget::setViewpoint(v+1);
	repaint();
}

void SeedGLWidget::mousePressEvent(QMouseEvent* e)
{
	GLWidget::mousePressEvent(e);

	_bDrawing = true;
	_penTrack.clear();
	_minDrawX = _maxDrawX = e->x();
	_minDrawY = _maxDrawY = e->y();

	if (e->button() == Qt::LeftButton)
	{
		_bAddRegion = true;
	} else
	{
		_bAddRegion = false;
	}
}

void SeedGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() == Qt::LeftButton || e->buttons()==Qt::RightButton)
	{
		Point pt = new double [2];
		pt[0] = e->x();
		pt[1] = e->y();
		_penTrack.push_back(pt);
		_minDrawX = _minDrawX<pt[0]?_minDrawX:pt[0];
		_maxDrawX = _maxDrawX>pt[0]?_maxDrawX:pt[0];
		_minDrawY = _minDrawY<pt[1]?_minDrawY:pt[1];
		_maxDrawY = _maxDrawY>pt[1]?_maxDrawY:pt[1];
	} else if (e->buttons()==Qt::LeftButton|Qt::RightButton)
	{
		_translateX += (e->x()-_posX)/10;
		_translateY -= (e->y()-_posY)/10;
	}

	_posX = e->x();
	_posY = e->y();

	repaint();
}

void SeedGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	GLWidget::mouseReleaseEvent(e);

	Point pt = new double [2];
	pt[0] = e->x();
	pt[1] = e->y();
	_penTrack.push_back(pt);

	selectRegion();
	_bDrawing = false;
	repaint();
}

void SeedGLWidget::wheelEvent(QWheelEvent * e)
{
	GLWidget::wheelEvent(e);
	int hoverWin = 0;
	if (e->x()<_widgetWidth-_planeWidth)
	{
		return;
	} else
	{
		hoverWin = floor(e->y()*1.0 / (_widgetHeight/3.0));
	}

	if (e->orientation() == Qt::Vertical) {
		switch (hoverWin)
		{
		case 2:
			//Sagittal
			_seedData->setPSlicePrime(_pSlicePos);
			break;
		case 1:
			//Coronal
			_seedData->setCSlicePrime(_cSlicePos);
			break;
		case 0:
			//Axial
			_seedData->setASlicePrime(_aSlicePos);
			break;
		}
	}
	repaint();
}

void SeedGLWidget::setVolumeData(VolumeData * volData)
{
	makeCurrent();
	GLWidget::setVolumeData(volData);

	_tensorData = dynamic_cast<TensorData*>(volData);

	_seedData->setVolumeData(_tensorData);
	_seedData->setASlice(_aSlicePos);
	_seedData->setPSlice(_pSlicePos);
	_seedData->setCSlice(_cSlicePos);
	
	_currentSeedIndex = 0;
	//_seedCount = 0;

	repaint();
}

void SeedGLWidget::drawPenTrack()
{
	glViewport(0, 0, _widgetWidth-_planeWidth, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, _widgetWidth-_planeWidth, _widgetHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (_bAddRegion)
	{

		glColor4ub(_currentSeedColor[0], _currentSeedColor[1], _currentSeedColor[2], 128);

		gluTessBeginPolygon(tessObj, NULL);
		gluTessBeginContour(tessObj);
		for (int i=0; i<_penTrack.size(); ++i)
		{
			gluTessVertex(tessObj, _penTrack[i], _penTrack[i]);
		}
		gluTessEndContour(tessObj);
		gluTessEndPolygon(tessObj);

		glColor4ub(_currentSeedColor[0], _currentSeedColor[1], _currentSeedColor[2], 255);
	} else
	{
		glColor3f(1, 1, 1);
	}

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xF0F0); 
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	for (int i=0; i<_penTrack.size(); ++i)
	{
		glVertex2dv(_penTrack[i]);
	}
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void SeedGLWidget::selectRegion()
{
	/*if (_seedRegions.empty())
	{
		return;
	}*/

	glViewport (0, 0, 1024, 1024);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fboId);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, _widgetWidth-_planeWidth, _widgetHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1, 1, 1);

	gluTessBeginPolygon(tessObj, NULL);
	gluTessBeginContour(tessObj);
	for (int i=0; i<_penTrack.size(); ++i)
	{
		gluTessVertex(tessObj, _penTrack[i], _penTrack[i]);
	}
	gluTessEndContour(tessObj);
	gluTessEndPolygon(tessObj);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	GLubyte * tempBuffer = new GLubyte [1024*1024];
	glBindTexture(GL_TEXTURE_2D, _texId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tempBuffer);

	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	int viewport[4];

	int viewportWidth = _viewportHeight*(_widgetWidth-_planeWidth)*1.0/_widgetHeight;
	glViewport (0, 0, 1024, 1024);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewportWidth, 0, _viewportHeight, -2000, 2000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(_translateX, _translateY, 0);
	glTranslatef(viewportWidth/2.0, _viewportHeight/2.0, 0);
	setRotationMatrix();
	_volumeData->setTransformMatrix();

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	int minX = floor(_minDrawX*1023.0/(_widgetWidth-_planeWidth-1));
	int maxX = ceil(_maxDrawX*1023.0/(_widgetWidth-_planeWidth-1));
	int minY = floor((_widgetHeight-_maxDrawY)*1023.0/(_widgetHeight-1));
	int maxY = ceil((_widgetHeight-_minDrawY)*1023.0/(_widgetHeight-1));

	for (int i=minX; i<=maxX; ++i)
	{
		for (int j=minY; j<maxY; ++j)
		{
			if (tempBuffer[j*1024+i]>0)
			{
				double x, y, z;
				gluUnProject(i, j, 0, modelMatrix, projMatrix, viewport, &x, &y, &z);
				//add to seed region list

				switch (_viewpoint)
				{
				case 1: //AXIAL
					z = _seedData->_aSliceIdx;
					break;
				case 2: //CORONAL
					y = _seedData->_cSliceIdx;
					break;
				case 3: //SAGITTAL
					x = _seedData->_pSliceIdx;
					break;
				}
				if (_bAddRegion)
				{
					_seedData->addSeedPoint(x,y,z, _currentSeedIndex);
				} else
				{
					_seedData->removeSeedPoint(x,y,z,_currentSeedIndex);
				}
			}
		}
	}

	delete tempBuffer;

	_seedData->setASlice(_aSlicePos);
	_seedData->setPSlice(_pSlicePos);
	_seedData->setCSlice(_cSlicePos);

}

bool SeedGLWidget::initFBO()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		return false;
	}
	// Creating texture.
	glGenTextures(1, &_texId);    
	glBindTexture(GL_TEXTURE_2D, _texId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffersEXT(1, &_fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fboId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _texId, 0);
	int stat = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (stat != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		QMessageBox::warning(this, "Error occurred", "FBO could not be initialized!");
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		return false;
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

void SeedGLWidget::drawSeedRegion()
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1, 1, 1, 0.5);
	switch (_viewpoint)
	{
	case VP_AXIAL:
		_seedData->drawAxialSlice();
		break;
	case VP_CORONAL:
		_seedData->drawCoronalSlice();
		break;
	case VP_SAGITTAL:
		_seedData->drawSagittalSlice();
	}
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void SeedGLWidget::setCurrentSeed(int index, unsigned color)
{
	_currentSeedIndex = index;
	_currentSeedColor[0] = (0xFF0000&color)>>16;
	_currentSeedColor[1] = (0x00FF00&color)>>8;
	_currentSeedColor[2] = 0xFF&color;
}

void SeedGLWidget::setSeedData(SeedData *seedData)
{
	_seedData = seedData;
	int t= 0;
}

void SeedGLWidget::setRotationMatrix()
{
	switch (_viewpoint)
	{
	case 1:
		glRotatef(-180, 1, 0, 0);
		break;
	case 2:
		glRotatef(-90, 1, 0, 0);
		break;
	case 3:
		glRotatef(-90, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		break;
	}
}

void SeedGLWidget::zoomIn()
{
	_viewportHeight -= 10;
	if (_viewportHeight < -2000)
		_viewportHeight = -2000;
	repaint();
}

void SeedGLWidget::zoomOut()
{
	_viewportHeight += 10;
	if (_viewportHeight > 2000)
		_viewportHeight = 2000;
	repaint();
}