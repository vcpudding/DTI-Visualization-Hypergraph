#pragma once

#include <Windows.h>
#include <QStringList>
#include <GL/gl.h>

class VolumeData
{
public:
	VolumeData(void);
	~VolumeData(void);

	friend class		GLWidget;
	friend class		SeedData;
	friend class		FiberData;

	static enum			DisplayMode {TENSOR_B0, TENSOR_DIR, TENSOR_FA};

protected:
	unsigned *			_data;
	int					_sizeX, _sizeY, _sizeZ;
	int					_nVoxels;
	double				_volSizeX, _volSizeY, _volSizeZ;
	double				_scaleX, _scaleY, _scaleZ;
	int					_pixSizeX, _pixSizeY, _pixSizeZ;
	double				_pSliceIdx;
	double				_cSliceIdx;
	double				_aSliceIdx;
	GLuint				_pTexId, _cTexId, _aTexId;
	unsigned char *		_pSlice;
	unsigned char *		_cSlice;
	unsigned char *		_aSlice;
	DisplayMode			_displayMode;

	double				_winPos;
	double				_winLength;

	QStringList			_infoStr;

	unsigned			getIndex (int x, int y, int z) const;
	unsigned			getIndex (const int * indices, const int * dims, int colDir, int rowDir, int sliceDir);
	int					getDir (const double * cosines);
	unsigned char		getGrayscale (unsigned d);
	unsigned char		getGrayscale (int x, int y, int z);
	unsigned char		getGrayscale (double x, double y, double z);
	void				initBuffers ();

public:
	/*virtual functions to be overridden by sub-class*/
	virtual int 		readOrientedData (const QStringList & fileList);

	virtual void		getSagittalSlice ();
	virtual void		getCoronalSlice ();
	virtual void		getAxialSlice ();
	virtual void		setInfoStr();

	void				setDisplayMode (int m);
	int					getDisplayMode () {return _displayMode;}

	void				drawSagittalSlice ();
	void				drawCoronalSlice();
	void				drawAxialSlice();

	void				setTransformMatrix ();
	void				drawSagittalPlane (int width, int height, bool showSliceIdx = true);
	void				drawCoronalPlane (int width, int height, bool showSliceIdx = true);
	void				drawAxialPlane (int width, int height, bool showSliceIdx = true);

	const QStringList &	getInfoStr () {return _infoStr;}

	void				setPSlice (int pos);
	void				setCSlice (int pos);
	void				setASlice (int pos);

	double				setPSlicePrime (double pos);
	double				setCSlicePrime (double pos);
	double				setASlicePrime (double pos);

	void				refreshSlices ();

	void				setWindow (int wlStep, int wwStep);

	int					sizeX() {return _sizeX;}
	int					sizeY() {return _sizeY;}
	int					sizeZ() {return _sizeZ;}
	double				pixSizeX () {return _scaleX;}
	double				pixSizeY () {return _scaleY;}
	double				pixSizeZ () {return _scaleZ;}
};
