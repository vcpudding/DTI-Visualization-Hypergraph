#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "DicomImage.h"
#include "VolumeData.h"

#include <vector>

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

	enum			Viewpoint	{VP_FREE, VP_AXIAL, VP_CORONAL, VP_SAGITTAL};

protected:
    void			initializeGL();
    void			paintGL();
	void			resizeGL(int width, int height);
	void			mousePressEvent(QMouseEvent *e);
	void			mouseMoveEvent(QMouseEvent *e);
	void			mouseReleaseEvent(QMouseEvent *e);
	void			wheelEvent ( QWheelEvent * event );

protected:
	VolumeData*		_volumeData;
	//vector<VolumeData*>	_volumes;
	int				_posX, _posY;
	int				_pressX, _pressY;
	float			_zoom, _translateX, _translateY;
	Viewpoint		_viewpoint;
	int				_xDir, _yDir, _zDir;

	int				_widgetWidth;
	int				_widgetHeight;
	int				_planeWidth;

	/*tunable parameters*/
	int				_aSlicePos, _cSlicePos, _pSlicePos;
	bool			_bShowSagittal;
	bool			_bShowCoronal;
	bool			_bShowAxial;
	bool			_bShowSliceIdx;

	void			drawImageInfo ();
	void			drawAnchor ();
	void			drawPlanes (int left, int width, int height);

	virtual void	setRotationMatrix ();

public:
	void			setVolumeData (VolumeData * volData);
	//void			addVolumeData (VolumeData * volData);
	static	void	drawString(const char * s, float x, float y, float lineHeight);

public slots:
	void			setShowSagittal (bool bShow) {_bShowSagittal = bShow; repaint();}
	void			setShowCoronal (bool bShow) {_bShowCoronal = bShow; repaint();}
	void			setShowAxial (bool bShow) {_bShowAxial = bShow; repaint();}
	void			setDisplayMode (int m);
	void			setShowSliceIdx (bool bShow) {_bShowSliceIdx = bShow; repaint();}
	void			setViewpoint (int v);
	void			setAxialSlice (int slice);
	void			setCoronalSlice (int slice);
	void			setSagittalSlice (int slice);
	void			setAxisDirs (int x, int y, int z) {_xDir = x; _yDir = y; _zDir = z; repaint();}

signals:
	void			viewpointChanged ();
};

#endif
