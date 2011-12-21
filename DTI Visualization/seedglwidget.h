#ifndef SEEDGLWIDGET_H
#define SEEDGLWIDGET_H

#include <Windows.h>
#include "glwidget.h"
#include <QMouseEvent>
#include <vector>

#include "TensorData.h"
#include "SeedData.h"

using namespace std;

typedef double * Point;

class SeedGLWidget : public GLWidget
{
	Q_OBJECT

public:
	SeedGLWidget(QWidget *parent);
	~SeedGLWidget();

private:
	static const int	_MaxSeedCount;
	TensorData *		_tensorData;
	SeedData *			_seedData;

	int					_viewportHeight;
	vector<Point>		_penTrack;
	bool				_bDrawing;
	bool				_bAddRegion;
	int					_minDrawX, _maxDrawX;
	int					_minDrawY, _maxDrawY;

	GLuint				_fboId;
	GLuint				_texId;

	int					_currentSeedIndex;
	uchar				_currentSeedColor [3];

	void				drawPenTrack ();
	void				drawSeedRegion ();
	void				selectRegion ();
	bool				initFBO ();

protected:
	void				initializeGL();
	void				resizeGL(int width, int height);
	void				paintGL();
	void				mousePressEvent (QMouseEvent *e);
	void				mouseMoveEvent (QMouseEvent* e);
	void				mouseReleaseEvent(QMouseEvent *e);
	void				wheelEvent( QWheelEvent * e);

	virtual void		setRotationMatrix();

public:
	void				setVolumeData (VolumeData * volData);
	void				setSeedData (SeedData * seedData);

public slots:
	void				setViewpoint(int v);
	void				setCurrentSeed (int index, unsigned color);
	void				zoomIn ();
	void				zoomOut ();
};

#endif // SEEDGLWIDGET_H
