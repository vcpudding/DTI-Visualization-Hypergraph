#include <gl/glew.h>
#include "VolumeData.h"
#include "DicomImage.h"

#include <QMessageBox>
#include <QProgressDialog>
#include <gl/glut.h>
#include <math.h>
#include <string>

using namespace std;

VolumeData::VolumeData(void)
{
	_data = NULL;
	_sizeX = _sizeY = _sizeZ = 0;
	_nVoxels = 0;
	_volSizeX = _volSizeY = _volSizeZ = 0;
	_scaleX = _scaleY = _scaleZ = 1;
	_pixSizeX = _pixSizeY = _pixSizeZ = 0;
	_pSliceIdx = 0;
	_cSliceIdx = 0;
	_aSliceIdx = 0;
	_pTexId = _cTexId = _aTexId = -1;
	_pSlice = NULL;
	_cSlice = NULL;
	_aSlice = NULL;
	_winPos = 1500;
	_winLength = 1000;
	_infoStr = QStringList();
}

VolumeData::~VolumeData(void)
{
}

void VolumeData::getSagittalSlice()
{
	if (!_data)
	{
		return;
	}

	if (!_pSlice)
	{
		_pSlice = new unsigned char [_pixSizeY*_pixSizeZ];
		glGenTextures(1, &_pTexId);
	}

	if (_pSliceIdx<0 || _pSliceIdx>_sizeX-1)
	{
		memset(_pSlice, 0, _pixSizeY*_pixSizeZ*sizeof(uchar));
	} else
	{
		double ky = (_sizeY-1)*1.0/(_pixSizeY-1);
		double kz = (_sizeZ-1)*1.0/(_pixSizeZ-1);
		for (int i=0; i<_pixSizeZ; ++i)
		{
			for (int j=0; j<_pixSizeY; ++j)
			{
				double y = j*ky;
				double z = i*kz;
				_pSlice[i*_pixSizeY+j] = getGrayscale((double)_pSliceIdx, y, z);
			}
		}
	}

	// allocate a texture name
	glBindTexture( GL_TEXTURE_2D, _pTexId );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	// build our texture mipmaps
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_LUMINANCE, _pixSizeY, _pixSizeZ,
		GL_LUMINANCE, GL_UNSIGNED_BYTE, _pSlice);
}

void VolumeData::getCoronalSlice()
{
	if (!_data)
	{
		return;
	}

	if (!_cSlice)
	{
		_cSlice = new unsigned char [_pixSizeX*_pixSizeZ];
		glGenTextures(1, &_cTexId);
	}

	if (_cSliceIdx<0 || _cSliceIdx>_sizeY-1)
	{
		memset(_cSlice, 0, _pixSizeX*_pixSizeZ*sizeof(uchar));
	} else
	{
		double kx = (_sizeX-1)*1.0/(_pixSizeX-1);
		double kz = (_sizeZ-1)*1.0/(_pixSizeZ-1);
		for (int i=0; i<_pixSizeZ; ++i)
		{
			for (int j=0; j<_pixSizeX; ++j)
			{
				double x = j*kx;
				double z = i*kz;
				_cSlice[i*_pixSizeX+j] = getGrayscale(x, (double)_cSliceIdx, z);
			}
		}
	}

	// allocate a texture name
	glBindTexture( GL_TEXTURE_2D, _cTexId );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	// build our texture mipmaps
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_LUMINANCE, _pixSizeX, _pixSizeZ,
		GL_LUMINANCE, GL_UNSIGNED_BYTE, _cSlice);
}

void VolumeData::getAxialSlice()
{
	if (!_data)
	{
		return;
	}

	if (!_aSlice)
	{
		_aSlice = new unsigned char [_pixSizeX*_pixSizeY];
		glGenTextures(1, &_aTexId);
	}

	if (_aSliceIdx<0 || _aSliceIdx>_sizeZ-1)
	{
		memset(_aSlice, 0, _pixSizeX*_pixSizeY*sizeof(uchar));
	} else
	{
		double kx = (_sizeX-1)*1.0/(_pixSizeX-1);
		double ky = (_sizeY-1)*1.0/(_pixSizeY-1);
		for (int i=0; i<_pixSizeY; ++i)
		{
			for (int j=0; j<_pixSizeX; ++j)
			{
				double x = j*kx;
				double y = i*ky;
				_aSlice[i*_pixSizeX+j] = getGrayscale(x, y, (double)_aSliceIdx);
			}
		}
	}

	glBindTexture( GL_TEXTURE_2D, _aTexId );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_LUMINANCE, _pixSizeX, _pixSizeY,
		GL_LUMINANCE, GL_UNSIGNED_BYTE, _aSlice);
}

unsigned VolumeData::getIndex(int x, int y, int z) const
{
	return z*_sizeX*_sizeY + y*_sizeX + x;
}

unsigned VolumeData::getIndex(const int * indices, const int * dims, int colDir, int rowDir, int sliceDir)
{
	int coords [3];
	int colIdx = abs(colDir)-1;
	int rowIdx = abs(rowDir)-1;
	int sliceIdx = abs(sliceDir)-1;
	coords[colIdx] = colDir>0?indices[0]:dims[colIdx]-indices[0];
	coords[rowIdx] = rowDir>0?indices[1]:dims[rowIdx]-indices[1];
	coords[sliceIdx] = sliceDir>0?indices[1]:dims[sliceIdx]-indices[1];
	return getIndex(coords[0], coords[1], coords[2]);
}

unsigned char VolumeData::getGrayscale(unsigned d)
{
	double g = 128+(d-_winPos)*255.0/_winLength;
	g = g<0?0:(g>255?255:g);
	return (unsigned char)g;
}

void VolumeData::drawSagittalSlice()
{
	if (!_pSlice)
	{
		return;
	}
	glPushMatrix();
	setTransformMatrix();

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, _pTexId);
	glBegin( GL_QUADS );
	//glColor3f(1.0, 1.0, 1.0);
	glTexEnvi(GL_TEXTURE_2D, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_2D, GL_COMBINE_ALPHA, GL_MODULATE);
	glTexCoord2d(0.0,0.0); glVertex3f(_pSliceIdx, 0.0, 0.0);
	glTexCoord2d(1.0,0.0); glVertex3f(_pSliceIdx, _sizeY,0.0);
	glTexCoord2d(1.0,1.0); glVertex3f(_pSliceIdx, _sizeY,_sizeZ);
	glTexCoord2d(0.0,1.0); glVertex3f(_pSliceIdx, 0.0,_sizeZ);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(_pSliceIdx, 0.0, 0.0);
	glVertex3f(_pSliceIdx, _sizeY,0.0);
	glVertex3f(_pSliceIdx, _sizeY,_sizeZ);
	glVertex3f(_pSliceIdx, 0.0,_sizeZ);
	glEnd();
	glPopMatrix();
}

void VolumeData::drawCoronalSlice()
{
	if (!_cSlice)
	{
		return;
	}

	glPushMatrix();
	setTransformMatrix();

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, _cTexId);
	glBegin( GL_QUADS );
	//glColor3f(1.0, 1.0, 1.0);
	glTexEnvi(GL_TEXTURE_2D, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_2D, GL_COMBINE_ALPHA, GL_MODULATE);
	glTexCoord2d(0.0,0.0); glVertex3f(0.0, _cSliceIdx, 0.0);
	glTexCoord2d(1.0,0.0); glVertex3f(_sizeX, _cSliceIdx,0.0);
	glTexCoord2d(1.0,1.0); glVertex3f(_sizeX, _cSliceIdx,_sizeZ);
	glTexCoord2d(0.0,1.0); glVertex3f(0.0, _cSliceIdx,_sizeZ);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, _cSliceIdx, 0.0);
	glVertex3f(_sizeX, _cSliceIdx,0.0);
	glVertex3f(_sizeX, _cSliceIdx,_sizeZ);
	glVertex3f(0.0, _cSliceIdx,_sizeZ);
	glEnd();
	glPopMatrix();
}

void VolumeData::drawAxialSlice()
{
	if (!_aSlice)
	{
		return;
	}

	glPushMatrix();
	setTransformMatrix();

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, _aTexId );
	glBegin( GL_QUADS );
	//glColor3f(1.0, 1.0, 1.0);
	glTexEnvi(GL_TEXTURE_2D, GL_COMBINE_RGB, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_2D, GL_COMBINE_ALPHA, GL_MODULATE);
	glTexCoord2d(0.0,0.0); glVertex3f(0.0, 0.0, _aSliceIdx);
	glTexCoord2d(1.0,0.0); glVertex3f(_sizeX,0.0, _aSliceIdx);
	glTexCoord2d(1.0,1.0); glVertex3f(_sizeX,_sizeY, _aSliceIdx);
	glTexCoord2d(0.0,1.0); glVertex3f(0.0,_sizeY, _aSliceIdx);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 0.0, _aSliceIdx);
	glVertex3f(_sizeX,0.0, _aSliceIdx);
	glVertex3f(_sizeX,_sizeY, _aSliceIdx);
	glVertex3f(0.0,_sizeY, _aSliceIdx);
	glEnd();
	glPopMatrix();
}

//void VolumeData::setPSlice(int pos)
//{
//	_pSliceIdx = pos;
//	getSagittalSlice();
//}
//
//void VolumeData::setASlice(int pos)
//{
//	_aSliceIdx = pos;
//	getAxialSlice();
//}
//
//void VolumeData::setCSlice(int pos)
//{
//	_cSliceIdx = pos;
//	getCoronalSlice();
//}

void VolumeData::drawSagittalPlane(int width, int height, bool showSliceIdx)
{
	if (!_pSlice)
	{
		return;
	}

	double imgWidth = _sizeY*_scaleY;
	double imgHeight = _sizeZ*_scaleZ;
	double ratio = width*1.0/height;
	double imgRatio = imgWidth/imgHeight;

	if (imgRatio>ratio)
	{
		width = imgWidth;
		height = imgWidth/ratio;
	} else
	{
		width = imgHeight*ratio;
		height = imgHeight;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef((width-imgWidth)/2, (height-imgHeight)/2, 0);

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, _pTexId);
	glBegin( GL_QUADS );
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2d(0.0,1.0); glVertex2f(0.0, imgHeight);
	glTexCoord2d(1.0,1.0); glVertex2f(imgWidth,imgHeight);
	glTexCoord2d(1.0,0.0); glVertex2f(imgWidth,0.0);
	glTexCoord2d(0.0,0.0); glVertex2f(0.0,0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (showSliceIdx)
	{
		float y = _cSliceIdx*1.0f/_sizeY*imgWidth;
		float z = _aSliceIdx*1.0f/_sizeZ*imgHeight;
		glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		glVertex2f(y, 0);
		glVertex2f(y, imgHeight);

		glColor3f(0, 0, 1);
		glVertex2f(0, z);
		glVertex2f(imgWidth, z);
		glEnd();
	}

	glPopMatrix();
}

void VolumeData::drawCoronalPlane(int width, int height, bool showSliceIdx)
{
	if (!_cSlice)
	{
		return;
	}
	double imgWidth = _sizeX*_scaleX;
	double imgHeight = _sizeZ*_scaleZ;
	double ratio = width*1.0/height;
	double imgRatio = imgWidth/imgHeight;

	if (imgRatio>ratio)
	{
		width = imgWidth;
		height = imgWidth/ratio;
	} else
	{
		width = imgHeight*ratio;
		height = imgHeight;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef((width-imgWidth)/2, (height-imgHeight)/2, 0);

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, _cTexId);
	glBegin( GL_QUADS );
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2d(0.0,1.0); glVertex2f(0.0, imgHeight);
	glTexCoord2d(1.0,1.0); glVertex2f(imgWidth,imgHeight);
	glTexCoord2d(1.0,0.0); glVertex2f(imgWidth,0.0);
	glTexCoord2d(0.0,0.0); glVertex2f(0.0,0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (showSliceIdx)
	{
		float x = _pSliceIdx*1.0f/_sizeX*imgWidth;
		float z = _aSliceIdx*1.0f/_sizeZ*imgHeight;
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex2f(x, 0);
		glVertex2f(x, imgHeight);

		glColor3f(0, 0, 1);
		glVertex2f(0, z);
		glVertex2f(imgWidth, z);
		glEnd();
	}

	glPopMatrix();
}

void VolumeData::drawAxialPlane(int width, int height, bool showSliceIdx)
{
	if (!_aSlice)
	{
		return;
	}
	double imgWidth = _sizeX*_scaleX;
	double imgHeight = _sizeY*_scaleY;
	double ratio = width*1.0/height;
	double imgRatio = imgWidth/imgHeight;

	if (imgRatio>ratio)
	{
		width = imgWidth;
		height = imgWidth/ratio;
	} else
	{
		width = imgHeight*ratio;
		height = imgHeight;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef((width-imgWidth)/2, (height-imgHeight)/2, 0);

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, _aTexId);
	glBegin( GL_QUADS );
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2d(0.0,0.0); glVertex2f(0.0, imgHeight);
	glTexCoord2d(1.0,0.0); glVertex2f(imgWidth,imgHeight);
	glTexCoord2d(1.0,1.0); glVertex2f(imgWidth,0.0);
	glTexCoord2d(0.0,1.0); glVertex2f(0.0,0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (showSliceIdx)
	{
		float x = _pSliceIdx*1.0f/_sizeX*imgWidth;
		float y = _cSliceIdx*1.0f/_sizeY*imgHeight;
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex2f(x, 0);
		glVertex2f(x, imgHeight);

		glColor3f(0, 1, 0);
		glVertex2f(0, y);
		glVertex2f(imgWidth, y);
		glEnd();
	}

	glPopMatrix();
}

unsigned char VolumeData::getGrayscale(int x, int y, int z)
{
	return getGrayscale(_data[getIndex(x,y,z)]);
}

unsigned char VolumeData::getGrayscale(double x, double y, double z)
{
	double tx = x-floor(x);
	double ty = y-floor(y);
	double tz = z-floor(z);
	int x1 = floor(x), x2 = ceil(x);
	int y1 = floor(y), y2 = ceil(y);
	int z1 = floor(z), z2 = ceil(z);
	unsigned char d = (1-tz)*((1-ty)*((1-tx)*getGrayscale(x1, y1, z1)+tx*getGrayscale(x2, y1, z1))
							     +ty*((1-tx)*getGrayscale(x1, y2, z1)+tx*getGrayscale(x2, y2, z1)))
					     +tz*((1-ty)*((1-tx)*getGrayscale(x1, y1, z2)+tx*getGrayscale(x2, y1, z2))
							     +ty*((1-tx)*getGrayscale(x1, y2, z2)+tx*getGrayscale(x2, y2, z2)));
	return d;
}

void VolumeData::setWindow(int wlStep, int wwStep)
{
	_winPos += wlStep;
	_winLength += wwStep;

	getSagittalSlice();
	getCoronalSlice();
	getAxialSlice();
	setInfoStr();
}

void VolumeData::setInfoStr()
{
	_infoStr.clear();
	_infoStr <<QString("Image Size: %1 x %2 x %3").arg(_sizeX).arg(_sizeY).arg(_sizeZ);
	_infoStr <<QString("Voxel Size (mm): %1 x %2 x %3").arg(_volSizeX).arg(_volSizeY).arg(_volSizeZ);
	_infoStr <<QString("WL/WW: %1 / %2").arg(_winPos).arg(_winLength);
}

void VolumeData::initBuffers()
{
	if (_data)
	{
		delete _data;
		_data = NULL;
	}
	if (_pSlice)
	{
		delete _pSlice;
		_pSlice = NULL;
	}
	if (_cSlice)
	{
		delete _cSlice;
		_cSlice = NULL;
	}
	if (_aSlice)
	{
		delete _aSlice;
		_aSlice = NULL;
	}
}

int VolumeData::readOrientedData(const QStringList & fileList)
{
	initBuffers();

	DicomImage * dcmImage = new DicomImage();
	if (!dcmImage->readHeader(fileList[0].toStdString().c_str()))
	{
		QMessageBox::warning(NULL, "File Error", "Cannot open selected series!");
		return 0;
	}
	int nElement;
	_winPos = dcmImage->getNumericTag(0x0028, 0x1050, nElement)[0];
	_winLength = dcmImage->getNumericTag(0x0028, 0x1051, nElement)[0];

	double * iop = dcmImage->getNumericTag(0x0020, 0x0037, nElement);
	int colDir = getDir(iop);
	int rowDir = getDir(iop+3);

	double normal [3];
	normal[0] = iop[1]*iop[5] - iop[2]*iop[4];
	normal[1] = iop[2]*iop[3] - iop[0]*iop[5];
	normal[2] = iop[0]*iop[4] - iop[1]*iop[3];
	int sliceDir = getDir(normal);
	int colIdx = abs(colDir)-1;
	int rowIdx = abs(rowDir)-1;
	int sliceIdx = abs(sliceDir)-1;

	int dims [3];
	dims[colIdx] = dcmImage->getWidth();
	dims[rowIdx] = dcmImage->getHeight();
	dims[sliceIdx] = fileList.count();
	_sizeX = dims[0];
	_sizeY = dims[1];
	_sizeZ = dims[2];

	double tmpSize [3], volSize[3];
	dcmImage->getVolSize(tmpSize);
	volSize[colIdx] = tmpSize[0];
	volSize[rowIdx] = tmpSize[1];
	volSize[sliceIdx] = tmpSize[2];
	_volSizeX = volSize[0];
	_volSizeY = volSize[1];
	_volSizeZ = volSize[2];

	int nVoxelPerSlice = dcmImage->getWidth()*dcmImage->getHeight();
	_nVoxels = nVoxelPerSlice*fileList.count();
	_data = new unsigned[_nVoxels];
	memset(_data, 0, _nVoxels*sizeof(unsigned));

	unsigned * tempData = new unsigned[nVoxelPerSlice];
	int indices [3];
	int coords [3];
	QProgressDialog prog ("Loading volume data", "loading...", 0, fileList.count());
	prog.setCancelButton(NULL);
	prog.show();
	for (int k=0; k<fileList.count(); ++k)
	{
		prog.setValue(k);
		dcmImage->readData(fileList[k].toStdString().c_str(), tempData);
		for (int j=0; j<dcmImage->getHeight(); ++j)
		{
			for (int i=0; i<dcmImage->getWidth(); ++i)
			{
				coords[colIdx] = colDir>0?i:dims[colIdx]-1-i;
				coords[rowIdx] = rowDir>0?j:dims[rowIdx]-1-j;
				coords[sliceIdx] = sliceDir>0?k:dims[sliceIdx]-1-k;

				unsigned index = getIndex(coords[0], coords[1], coords[2]);
				_data[index] = tempData[j*dcmImage->getWidth()+i];
			}
		}
	}
	delete tempData;

	double minSize = min(min(_volSizeX, _volSizeY), _volSizeZ);
	_scaleX = _volSizeX;
	_scaleY = _volSizeY;
	_scaleZ = _volSizeZ;

	_pixSizeX = ceil(_sizeX*_volSizeX/minSize);
	_pixSizeY = ceil(_sizeY*_volSizeY/minSize);
	_pixSizeZ = ceil(_sizeZ*_volSizeZ/minSize);

	_pSliceIdx = _sizeX/2;
	_cSliceIdx = _sizeY/2;
	_aSliceIdx = _sizeZ/2;

	setPSlice(_sizeX/2);
	setCSlice(_sizeY/2);
	setASlice(_sizeZ/2);

	setInfoStr();

	return 1;
}

int VolumeData::getDir(const double *cosines)
{
	int maxIdx = 0;
	double maxCos = 0;
	for (int i=0; i<3; ++i)
	{
		if (fabs(cosines[i])>maxCos)
		{
			maxCos = fabs(cosines[i]);
			maxIdx = cosines[i]>=0?i+1:-(i+1);
		}
	}
	return maxIdx;
}

void VolumeData::setTransformMatrix()
{
	glScalef(_scaleX, _scaleY, _scaleZ);
	glTranslatef(-_sizeX/2, -_sizeY/2, -_sizeZ/2);
}

void VolumeData::setPSlice(int pos)
{
	_pSliceIdx = pos<0?0:(pos>=_sizeX?_sizeX-1:pos);
	_pSliceIdx += 0.5;
	getSagittalSlice();
}

void VolumeData::setCSlice(int pos)
{
	_cSliceIdx = pos<0?0:(pos>=_sizeY?_sizeY-1:pos);
	_cSliceIdx += 0.5;
	getCoronalSlice();
}

void VolumeData::setASlice(int pos)
{
	_aSliceIdx = pos<0?0:(pos>=_sizeZ?_sizeZ-1:pos);
	_aSliceIdx += 0.5;
	getAxialSlice();
}

double VolumeData::setPSlicePrime(double pos)
{
	int index = floor(pos/_scaleX) + _sizeX/2;
	index = index<0?0:(index>_sizeX-1?_sizeX-1:index);
	_pSliceIdx = index+0.5;
	getSagittalSlice();
	return (_pSliceIdx-_sizeX/2)*_scaleX;
}

double VolumeData::setCSlicePrime(double pos)
{
	int index = floor(pos/_scaleY) + _sizeY/2;
	index = index<0?0:(index>=_sizeY-1?_sizeY-1:index);
	_cSliceIdx = index+0.5;
	getCoronalSlice();
	return (_cSliceIdx-_sizeY/2)*_scaleY;
}

double VolumeData::setASlicePrime(double pos)
{
	int index = floor(pos/_scaleZ) + _sizeZ/2;
	index = index<0?0:(index>=_sizeZ-1?_sizeZ-1:index);
	_aSliceIdx = index+0.5;
	getAxialSlice();
	return (_aSliceIdx-_sizeZ/2)*_scaleZ;
}

void VolumeData::setDisplayMode(int m)
{
	DisplayMode dm = (DisplayMode)m;
	if (_displayMode!=dm)
	{
		_displayMode = dm;
		getSagittalSlice();
		getCoronalSlice();
		getAxialSlice();
	}
}

void VolumeData::refreshSlices()
{
	setASlicePrime(0);
	setCSlicePrime(0);
	setPSlicePrime(0);
}