#include "TensorData.h"

#include "DicomImage.h"

#include <Windows.h>
#include <QMessageBox>
#include <QProgressDialog>
#include <string>

#include <Gl/glut.h>

using namespace std;

TensorData::TensorData(void)
{
	_tensorField = NULL;
	_bTable = NULL;
	_nAcquisition = 0;
	_displayMode = TENSOR_B0;
	_noiseSignalThres = 20;
	_bInterpolate = false;
}

TensorData::~TensorData(void)
{
}

int TensorData::readBTable(const QStringList &fileList)
{
	if (_bTable)
	{
		delete _bTable;
		_bTable = NULL;
	}
	_nAcquisition = 0;
	DicomImage * dcmImage = new DicomImage();
	double tmpBTable [400];
	QProgressDialog prog ("Reading b-table...", "", 0, 1);
	prog.setCancelButton(NULL);
	prog.show();
	prog.setValue(0);
	while (true)
	{
		try
		{
			dcmImage->readHeader(fileList[_nAcquisition].toStdString().c_str());
			int nElement;
			double val = dcmImage->getNumericTag(0x0018, 0x9087, nElement)[0];
			if (_nAcquisition>0 && val<=0)
			{
				break;
			}
			double * vecs = dcmImage->getNumericTag(0x0018, 0x9089, nElement);
			tmpBTable[4*_nAcquisition] = val;
			memcpy(tmpBTable+4*_nAcquisition+1, vecs, 3*sizeof(double));
			++_nAcquisition;
		} catch(...)
		{
			QMessageBox::warning(NULL, "File Error", "Cannot read diffusion information from the selected series!");
			return 0;
		}
	}
	prog.setValue(1);
	if (_nAcquisition<7)
	{
		QMessageBox::warning(NULL, "File Error", "The selected series is not DTI!");
		return 0;
		//return 1;
	}
	_bTable = new double [4*_nAcquisition];
	memcpy(_bTable, tmpBTable, 4*_nAcquisition*sizeof(double));
	return 1;
}

unsigned TensorData::getData (int x, int y, int z, int n)
{
	return _data[getIndex(x,y,z,n)];
}

unsigned TensorData::getData(double x, double y, double z, int n)
{
	double tx = x-floor(x);
	double ty = y-floor(y);
	double tz = z-floor(z);
	int x1 = floor(x), x2 = ceil(x);
	int y1 = floor(y), y2 = ceil(y);
	int z1 = floor(z), z2 = ceil(z);
	unsigned d = (1-tz)*((1-ty)*((1-tx)*getData(x1, y1, z1, n)+tx*getData(x2, y1, z1, n))
							+ty*((1-tx)*getData(x1, y2, z1, n)+tx*getData(x2, y2, z1, n)))
				    +tz*((1-ty)*((1-tx)*getData(x1, y1, z2, n)+tx*getData(x2, y1, z2, n))
							+ty*((1-tx)*getData(x1, y2, z2, n)+tx*getData(x2, y2, z2, n)));
	return d;
}

unsigned long TensorData::getIndex(int x, int y, int z, int n)
{
	return (unsigned long)n*_nVoxels + (unsigned long)(z*_sizeX*_sizeY + y*_sizeX + x);
}

int TensorData::getTensorField()
{
	if (!_data || !_bTable)
	{
		return 0;
	}

	if (_tensorField)
	{
		delete _tensorField;
		_tensorField = NULL;
	}

	_tensorField = new Tensor [_nVoxels];//[_pixSizeX*_pixSizeY*_pixSizeZ];
	memset (_tensorField, 0, _nVoxels*sizeof(Tensor));

	fmat G (_nAcquisition-1, 6);
	for (int i=1; i<_nAcquisition; ++i)
	{
		G(i-1, 0) = _bTable[i*4+1]*_bTable[i*4+1]; //g1*g1
		G(i-1, 1) = _bTable[i*4+2]*_bTable[i*4+2]; //g2*g2
		G(i-1, 2) = _bTable[i*4+3]*_bTable[i*4+3]; //g3*g3
		G(i-1, 3) = 2*_bTable[i*4+1]*_bTable[i*4+2]; //2*g1*g2
		G(i-1, 4) = 2*_bTable[i*4+1]*_bTable[i*4+3]; //2*g1*g3
		G(i-1, 5) = 2*_bTable[i*4+2]*_bTable[i*4+3]; //2*g2*g3
	}

	unsigned * dataBuf = new unsigned [_sizeX*_sizeY*_nAcquisition];
	/*double kx = (_sizeX-1)*1.0/(_pixSizeX-1);
	double ky = (_sizeY-1)*1.0/(_pixSizeY-1);
	double kz = (_sizeZ-1)*1.0/(_pixSizeZ-1);*/
	int nPixel = _sizeX*_sizeY;

	QProgressDialog prog ("Reconstructing tensor field...", "", 0, _pixSizeZ);
	prog.setCancelButton(NULL);
	prog.show();
	for (int i=0; i<_sizeZ; ++i)
	{
		prog.setValue(i);
		//double z = kz*i;
		for (int n=0; n<_nAcquisition; ++n)
		{
			for (int j=0; j<_sizeY; ++j)
			{
				//double y = ky*j;
				for (int k=0; k<_sizeX; ++k)
				{
					dataBuf[n*nPixel+j*_pixSizeX+k] = getData(k, j, i, n);
				}
			}
		}
		getTensorField(dataBuf, nPixel, _nAcquisition, _bTable, _noiseSignalThres, G, _tensorField+i*nPixel);
	}
	return 1;
}

void TensorData::getTensorField(const unsigned * data, int nPixel, int nAcquisition, const double * bTable, int nsThres, Tensor * tensorField)
{
	fmat G (nAcquisition-1, 6);
	for (int i=1; i<nAcquisition; ++i)
	{
		G(i-1, 0) = bTable[i*4+1]*bTable[i*4+1]; //g1*g1
		G(i-1, 1) = bTable[i*4+2]*bTable[i*4+2]; //g2*g2
		G(i-1, 2) = bTable[i*4+3]*bTable[i*4+3]; //g3*g3
		G(i-1, 3) = 2*bTable[i*4+1]*bTable[i*4+2]; //2*g1*g2
		G(i-1, 4) = 2*bTable[i*4+1]*bTable[i*4+3]; //2*g1*g3
		G(i-1, 5) = 2*bTable[i*4+2]*bTable[i*4+3]; //2*g2*g3
	}
	getTensorField(data, nPixel, nAcquisition,  bTable, nsThres, G, tensorField);
}

void TensorData::getTensorField(const unsigned * data, int nPixel, int nAcquisition, const double * bTable, int nsThres, const fmat & G, Tensor * tensorField)
{

	const unsigned DATA_THRES = nsThres;
	fmat dataVec (nAcquisition-1, nPixel);
	dataVec.fill(0);

	for (int i=0; i<nPixel; ++i)
	{
		float S0 = data[i];
		if (S0<DATA_THRES)
		{
			continue;
		}
		for (int j=1; j<nAcquisition; ++j)
		{
			float S = data[i+j*nPixel];
			float d = (S0<=0||S<=0)?0:1.0/bTable[j*4]*log(S0/S);
			dataVec(j-1, i) = d<0?0:d;
		}
	}

	fmat tensorVec = solve(G, dataVec);

	fmat tensorMat (3,3);
	fvec eigVal (3);
	fmat eigVec (3,3);

	for (int i=0; i<nPixel; ++i)
	{
		if (max(dataVec.col(i))<=0)
		{
			continue;
		}

		for (int j=0; j<6; ++j)
		{
			tensorField[i].D[j] = tensorVec(j, i);
		}

		tensorMat <<tensorVec(0, i) <<tensorVec(3, i) <<tensorVec(4, i) <<endr
			<<tensorVec(3, i) <<tensorVec(1, i) <<tensorVec(5, i) <<endr
			<<tensorVec(4, i) <<tensorVec(5, i) <<tensorVec(2, i);
		
		eig_sym(eigVal, eigVec, tensorMat);
		
		for (int j=0; j<3; ++j)
		{
			eigVal(j) = eigVal(j)<0?0:eigVal(j);
		}

		int index0 = eigVal(0)>eigVal(1)?(eigVal(0)>eigVal(2)?0:2):(eigVal(1)>eigVal(2)?1:2);
		
		if (eigVal(index0)<=0)
		{
			continue;
		}
		tensorField[i].fa = sqrt(0.5*((eigVal(0)-eigVal(1))*(eigVal(0)-eigVal(1))+(eigVal(0)-eigVal(2))*(eigVal(0)-eigVal(2))+(eigVal(2)-eigVal(1))*(eigVal(2)-eigVal(1)))/
			(eigVal(0)*eigVal(0)+eigVal(1)*eigVal(1)+eigVal(2)*eigVal(2)));

		for (int k=0; k<3; ++k)
		{
			tensorField[i].d0[k] = eigVec(k, index0);
		}
	}
}

void TensorData::getSagittalSlice()
{
	if (!_tensorField || !_data)
	{
		return;
	}

	if (!_pSlice)
	{
		_pSlice = new unsigned char [_pixSizeY*_pixSizeZ*3];
		glGenTextures(1, &_pTexId);
	}


	if (_pSliceIdx<0 || _pSliceIdx>_sizeX-1)
	{
		memset(_pSlice, 0, _pixSizeY*_pixSizeZ*3*sizeof(uchar));
	} else
	{
		int x = _pSliceIdx*(_pixSizeX-1)/(_sizeX-1);
		double ky = (_sizeY-1)*1.0/(_pixSizeY-1);
		double kz = (_sizeZ-1)*1.0/(_pixSizeZ-1);
		Tensor t;
		QProgressDialog prog ("Retrieving slice data", "wait...", 0, _pixSizeZ-1);
		prog.setCancelButton(NULL);
		if (_displayMode!=TENSOR_B0 && _bInterpolate)
		{
			prog.show();
		}
		for (int i=0; i<_pixSizeZ; ++i)
		{
			prog.setValue(i);
			for (int j=0; j<_pixSizeY; ++j)
			{
				double y = j*ky;
				double z = i*kz;
				if (_displayMode == TENSOR_B0)
				{
					unsigned char d = VolumeData::getGrayscale((double)_pSliceIdx, y, z);
					for (int k=0; k<3; ++k)
					{
						_pSlice[(i*_pixSizeY+j)*3+k] = d;
					}
				} else if (_displayMode == TENSOR_DIR)
				{
					if (_bInterpolate)
					{
						t = getTensorAt((double)_pSliceIdx, y, z);
					} else
					{
						t = getTensorAt((int)_pSliceIdx, (int)y, (int)z);
					}

					for (int k=0; k<3; ++k)
					{
						_pSlice[(i*_pixSizeY+j)*3+k] = t.fa*fabs(t.d0[k])*255;
					}
				} else if (_displayMode == TENSOR_FA)
				{
					if (_bInterpolate)
					{
						t = getTensorAt((double)_pSliceIdx, y, z);
					} else
					{
						t = getTensorAt((int)_pSliceIdx, (int)y, (int)z);
					}

					for (int k=0; k<3; ++k)
					{
						_pSlice[(i*_pixSizeY+j)*3+k] = t.fa*255;
					}
				}
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
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, _pixSizeY, _pixSizeZ,
		GL_RGB, GL_UNSIGNED_BYTE, _pSlice);
}

void TensorData::getCoronalSlice()
{
	if (!_tensorField || !_data)
	{
		return;
	}

	if (!_cSlice)
	{
		_cSlice = new unsigned char [_pixSizeX*_pixSizeZ*3];
		glGenTextures(1, &_cTexId);
	}

	if (_cSliceIdx<0 || _cSliceIdx>_sizeY-1)
	{
		memset(_cSlice, 0, _pixSizeX*_pixSizeZ*3*sizeof(uchar));
	} else
	{
		int y = _cSliceIdx*(_pixSizeY-1)/(_sizeY-1);
		double kx = (_sizeX-1)*1.0/(_pixSizeX-1);
		double kz = (_sizeZ-1)*1.0/(_pixSizeZ-1);
		Tensor t;
		QProgressDialog prog ("Retrieving slice data", "wait...", 0, _pixSizeZ-1);
		prog.setCancelButton(NULL);
		if (_displayMode!=TENSOR_B0 && _bInterpolate)
		{
			prog.show();
		}
		for (int i=0; i<_pixSizeZ; ++i)
		{
			prog.setValue(i);
			for (int j=0; j<_pixSizeX; ++j)
			{
				double x = j*kx;
				double z = i*kz;
				if (_displayMode == TENSOR_B0)
				{
					unsigned char d = VolumeData::getGrayscale(x, (double)_cSliceIdx, z);
					for (int k=0; k<3; ++k)
					{
						_cSlice[(i*_pixSizeX+j)*3+k]= d;
					}
				} else if (_displayMode == TENSOR_DIR)
				{
					if (_bInterpolate)
					{
						t = getTensorAt(x, _cSliceIdx, z);
					} else
					{
						t = getTensorAt((int)x, (int)_cSliceIdx, (int)z);
					}

					for (int k=0; k<3; ++k)
					{
						_cSlice[(i*_pixSizeX+j)*3+k] = t.fa*fabs(t.d0[k])*255;
					}
				} else if (_displayMode == TENSOR_FA)
				{
					if (_bInterpolate)
					{
						t = getTensorAt(x, _cSliceIdx, z);
					} else
					{
						t = getTensorAt((int)x, (int)_cSliceIdx, (int)z);
					}

					for (int k=0; k<3; ++k)
					{
						_cSlice[(i*_pixSizeX+j)*3+k] = t.fa*255;
					}
				}
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
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, _pixSizeX, _pixSizeZ,
		GL_RGB, GL_UNSIGNED_BYTE, _cSlice);
}

void TensorData::getAxialSlice()
{
	if (!_tensorField || !_data)
	{
		return;
	}

	if (!_aSlice)
	{
		_aSlice = new unsigned char [_pixSizeX*_pixSizeY*3];
		glGenTextures(1, &_aTexId);
	}

	if (_aSliceIdx<0 || _aSliceIdx>_sizeZ-1)
	{
		memset(_aSlice, 0, _pixSizeX*_pixSizeY*sizeof(uchar));
	} else
	{
		int z = _aSliceIdx*(_pixSizeZ-1)/(_sizeZ-1);
		double kx = (_sizeX-1)*1.0/(_pixSizeX-1);
		double ky = (_sizeY-1)*1.0/(_pixSizeY-1);
		Tensor t;
		QProgressDialog prog ("Retrieving slice data", "wait...", 0, _pixSizeY-1);
		prog.setCancelButton(NULL);
		if (_displayMode!=TENSOR_B0 && _bInterpolate)
		{
			prog.show();
		}
		for (int i=0; i<_pixSizeY; ++i)
		{
			prog.setValue(i);
			for (int j=0; j<_pixSizeX; ++j)
			{
				double x = j*kx;
				double y = i*ky;
				if (_displayMode == TENSOR_B0)
				{
					unsigned char d = VolumeData::getGrayscale(x, y, (double)_aSliceIdx);
					for (int k=0; k<3; ++k)
					{
						_aSlice[(i*_pixSizeX+j)*3+k]= d;
					}
				} else if (_displayMode == TENSOR_DIR)
				{
					if (_bInterpolate)
					{
						t = getTensorAt(x, y, (double)_aSliceIdx);
					} else
					{
						t = getTensorAt((int)x, (int)y, (int)_aSliceIdx);
					}

					for (int k=0; k<3; ++k)
					{
						_aSlice[(i*_pixSizeX+j)*3+k] = t.fa*fabs(t.d0[k])*255;
					}
				} else if (_displayMode == TENSOR_FA)
				{
					if (_bInterpolate)
					{
						t = getTensorAt(x, y, _aSliceIdx);
					} else
					{
						t = getTensorAt((int)x, (int)y, (int)_aSliceIdx);
					}

					for (int k=0; k<3; ++k)
					{
						_aSlice[(i*_pixSizeX+j)*3+k] = t.fa*255;
					}
				}
			}
		}
	}

	glBindTexture( GL_TEXTURE_2D, _aTexId );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, _pixSizeX, _pixSizeY,
		GL_RGB, GL_UNSIGNED_BYTE, _aSlice);
}

void TensorData::setInfoStr()
{
	_infoStr.clear();
	_infoStr <<QString("Image Size: %1 x %2 x %3").arg(_sizeX).arg(_sizeY).arg(_sizeZ);
	_infoStr <<QString("Voxel Size (mm): %1 x %2 x %3").arg(_volSizeX).arg(_volSizeY).arg(_volSizeZ);
	_infoStr <<QString("WL/WW: %1 / %2").arg(_winPos).arg(_winLength);
	_infoStr <<QString("No. of acquisition: %1").arg(_nAcquisition);
}

//void TensorData::setDisplayMode(int m)
//{
//	DisplayMode dm = (DisplayMode)m;
//	if (_displayMode!=dm)
//	{
//		_displayMode = dm;
//		getSagittalSlice();
//		getCoronalSlice();
//		getAxialSlice();
//	}
//}

int TensorData::readOrientedData(const QStringList & fileList, const QString & dtiFileName)
{
	if (!readBTable(fileList))
	{
		return 0;
	}

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
	dims[sliceIdx] = fileList.count()/_nAcquisition;
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

	double minSize = min(min(_volSizeX, _volSizeY), _volSizeZ);
	_scaleX = _volSizeX;
	_scaleY = _volSizeY;
	_scaleZ = _volSizeZ;

	_pixSizeX = ceil(_sizeX*_volSizeX/minSize);
	_pixSizeY = ceil(_sizeY*_volSizeY/minSize);
	_pixSizeZ = ceil(_sizeZ*_volSizeZ/minSize);

	int nVoxelPerSlice = dcmImage->getWidth()*dcmImage->getHeight();
	_nVoxels = nVoxelPerSlice*fileList.count()/_nAcquisition;

	unsigned * tempData = new unsigned[nVoxelPerSlice];
	int indices [3];
	int coords [3];
	if (dtiFileName.isEmpty())
	{
		_data = new unsigned[_nVoxels*_nAcquisition];
		memset(_data, 0, _nVoxels*_nAcquisition*sizeof(unsigned));

		QProgressDialog prog ("Loading volume data", "loading...", 0, fileList.count()-1);
		prog.setCancelButton(NULL);
		prog.show();
		for (int k=0; k<fileList.count(); ++k)
		{
			prog.setValue(k);
			int kSlice = k/_nAcquisition;
			int kAcquisition = k%_nAcquisition;
			if (k%_nAcquisition == 0 && kSlice <= 2)
			{
				dcmImage->readHeader(fileList[k].toStdString().c_str());
			}
			dcmImage->readData(fileList[k].toStdString().c_str(), tempData);
			for (int j=0; j<dcmImage->getHeight(); ++j)
			{
				for (int i=0; i<dcmImage->getWidth(); ++i)
				{
					coords[colIdx] = colDir>0?i:dims[colIdx]-1-i;
					coords[rowIdx] = rowDir>0?j:dims[rowIdx]-1-j;
					coords[sliceIdx] = sliceDir>0?kSlice:dims[sliceIdx]-1-kSlice;

					unsigned index = getIndex(coords[0], coords[1], coords[2], kAcquisition);
					_data[index] = tempData[j*dcmImage->getWidth()+i];
				}
			}
		}
		getTensorField();
		removeRedundantData();
	} else
	{
		_data = new unsigned[_nVoxels];
		memset(_data, 0, _nVoxels*sizeof(unsigned));

		QProgressDialog prog ("Loading DTI data", "loading...", 0, fileList.count()-1);
		prog.setCancelButton(NULL);
		prog.show();
		for (int k=0; k<fileList.count(); k+=_nAcquisition)
		{
			prog.setValue(k);
			int kSlice = k/_nAcquisition;
			if (kSlice<=2)
			{
				dcmImage->readHeader(fileList[k].toStdString().c_str());
			}
			dcmImage->readData(fileList[k].toStdString().c_str(), tempData);
			for (int j=0; j<dcmImage->getHeight(); ++j)
			{
				for (int i=0; i<dcmImage->getWidth(); ++i)
				{
					coords[colIdx] = colDir>0?i:dims[colIdx]-1-i;
					coords[rowIdx] = rowDir>0?j:dims[rowIdx]-1-j;
					coords[sliceIdx] = sliceDir>0?kSlice:dims[sliceIdx]-1-kSlice;

					unsigned index = getIndex(coords[0], coords[1], coords[2], 0);
					_data[index] = tempData[j*dcmImage->getWidth()+i];
				}
			}
		}
		loadTensorField(dtiFileName);
	}
	delete tempData;

	_pSliceIdx = _sizeX/2;
	_cSliceIdx = _sizeY/2;
	_aSliceIdx = _sizeZ/2;

	setPSlicePrime(0);
	setCSlicePrime(0);
	setASlicePrime(0);

	setInfoStr();

	return 1;
}

void TensorData::saveTensorField(const QString & fileName)
{
	ofstream outFile (fileName.toStdString().c_str(), ios::binary);
	outFile.write((char*)_tensorField, _nVoxels*sizeof(Tensor));
	outFile.close();
}

void TensorData::loadTensorField(const QString & fileName)
{
	ifstream inFile (fileName.toStdString().c_str(), ios::binary);
	if (_tensorField)
	{
		delete _tensorField;
	}
	unsigned nPixels = _nVoxels;
	_tensorField = new Tensor [nPixels];
	inFile.read((char*)_tensorField, nPixels*sizeof(Tensor));
	inFile.close();
}

//unsigned long TensorData::getTensorFieldIndex(int x, int y, int z)
//{
//	return z*_pixSizeX*_pixSizeY+y*_pixSizeX+x;
//}

void TensorData::test()
{
	fmat mat1(3,3), mat2(3,3);
	mat1 <<8 <<0 <<0 <<endr
		<<0 <<4 <<0 <<endr
		<<0 <<0 <<1 <<endr;
	mat2 <<0 <<4 <<0 <<endr
		<<8 <<0 <<0 <<endr
		<<0 <<0 <<1 <<endr;
	Tensor t1 = getTensorFromMat(mat1);
	Tensor t2 = getTensorFromMat(mat2);
	fmat interpMat = tensorLinearInterp(0.1, t1, 0.9, t2);
	float* interpMatMem = interpMat.memptr();
	Tensor interpTensor = getTensorFromMat(interpMat);
	float tensorDet1 = det(mat1), tensorDet2 = det(mat2), tensorDet = det(interpMat);
	int stop = 1;
	return;
}

void TensorData::removeRedundantData()
{
	unsigned * newData = new unsigned[_nVoxels];
	memcpy(newData, _data, _nVoxels*sizeof(unsigned));
	delete _data;
	_data = newData;
}

void TensorData::setInterpolate(bool bSet)
{
	_bInterpolate = bSet;
	getSagittalSlice();
	getCoronalSlice();
	getAxialSlice();
}

Tensor TensorData::getTensorAt(int x, int y, int z) const
{
	int index = VolumeData::getIndex(x,y,z);
	return _tensorField[index];
}

Tensor TensorData::getTensorAt (double x, double y, double z) const
{
	double tx = x-floor(x);
	double ty = y-floor(y);
	double tz = z-floor(z);
	int x1 = floor(x), x2 = ceil(x);
	int y1 = floor(y), y2 = ceil(y);
	int z1 = floor(z), z2 = ceil(z);
	fmat tMat11 = tensorLinearInterp(1-tx, getTensorAt(x1,y1,z1), tx, getTensorAt(x2,y1,z1));
	fmat tMat12 = tensorLinearInterp(1-tx, getTensorAt(x1,y2,z1), tx, getTensorAt(x2,y2,z1));
	fmat tMat21 = tensorLinearInterp(1-tx, getTensorAt(x1,y1,z2), tx, getTensorAt(x2,y1,z2));
	fmat tMat22 = tensorLinearInterp(1-tx, getTensorAt(x1,y2,z2), tx, getTensorAt(x2,y2,z2));

	fmat tMat1 = tensorLinearInterp(1-ty, tMat11, ty, tMat12);
	fmat tMat2 = tensorLinearInterp(1-ty, tMat21, ty, tMat22);

	fmat tMat = tensorLinearInterp(1-tz, tMat1, tz, tMat2);
	
	return getTensorFromMat(tMat);
}

Tensor getTensorFromMat(const fmat & tMat)
{
	fmat eigVec(3,3);
	fvec eigVal(3);
	eig_sym(eigVal, eigVec, tMat);
	float * eigVecMem = eigVec.memptr();

	Tensor t;
	memset(&t, 0, sizeof(Tensor));
	t.D[0] = tMat(0,0);
	t.D[1] = tMat(1,1);
	t.D[2] = tMat(2,2);
	t.D[3] = tMat(0,1);
	t.D[4] = tMat(0,2);
	t.D[5] = tMat(1,2);

	int index0 = eigVal(0)>eigVal(1)?(eigVal(0)>eigVal(2)?0:2):(eigVal(1)>eigVal(2)?1:2);

	if (eigVal(index0)<=0)
	{
		return t;
	}
	t.fa = sqrt(0.5*((eigVal(0)-eigVal(1))*(eigVal(0)-eigVal(1))+(eigVal(0)-eigVal(2))*(eigVal(0)-eigVal(2))+(eigVal(2)-eigVal(1))*(eigVal(2)-eigVal(1)))/
		(eigVal(0)*eigVal(0)+eigVal(1)*eigVal(1)+eigVal(2)*eigVal(2)));

	for (int k=0; k<3; ++k)
	{
		t.d0[k] = eigVec(k, index0);
	}
	return t;
}

fmat tensorLog (const Tensor &t)
{
	fmat tMat(3,3);
	tMat <<t.D[0] <<t.D[3] <<t.D[4] <<endr
		<<t.D[3] <<t.D[1] <<t.D[5] <<endr
		<<t.D[4] <<t.D[5] <<t.D[2];

	return tensorLog(tMat);
}

fmat tensorLog (const fmat &tMat)
{
	fvec eigVal(3);
	fmat eigVec(3,3);
	eig_sym(eigVal, eigVec, tMat);

	bool bValid = min(eigVal)>0;

	if (!bValid)
	{
		fmat retMat(3,3);
		retMat.fill(0);
		return retMat;
	}

	eigVal = log(eigVal);
	fmat logMat = eigVec*diagmat(eigVal)*inv(eigVec);
	return logMat;
}

fmat tensorExp (const fmat &tMat)
{
	fvec eigVal(3);
	fmat eigVec(3,3);
	eig_sym(eigVal, eigVec, tMat);

	eigVal = exp(eigVal);
	fmat expMat = eigVec*diagmat(eigVal)*inv(eigVec);
	return expMat;
}

fmat tensorLinearInterp(double w1, const Tensor & t1, double w2, const Tensor & t2)
{
	fmat tMat1(3,3), tMat2(3,3);
	tMat1 <<t1.D[0] <<t1.D[3] <<t1.D[4] <<endr
		<<t1.D[3] <<t1.D[1] <<t1.D[5] <<endr
		<<t1.D[4] <<t1.D[5] <<t1.D[2];
	tMat2 <<t2.D[0] <<t2.D[3] <<t2.D[4] <<endr
		<<t2.D[3] <<t2.D[1] <<t2.D[5] <<endr
		<<t2.D[4] <<t2.D[5] <<t2.D[2];

	return tensorLinearInterp(w1, tMat1, w2, tMat2);
}

fmat tensorLinearInterp(double w1, const fmat & tMat1, double w2, const fmat & tMat2)
{
	if (w1<1.0e-3)
	{
		return tMat2;
	}

	if (w2<1.0e-3)
	{
		return tMat1;
	}
	fmat logMat1 = tensorLog(tMat1);
	fmat logMat2 = tensorLog(tMat2);
	float * logMatMem1 = logMat1.memptr();
	float * logMatMem2 = logMat2.memptr();

	fmat retLogMat = w1*logMat1+w2*logMat2;
	float * retLogMatMem = retLogMat.memptr();
	fmat retExpMat = tensorExp(retLogMat);
	float * retExpMatMem = retExpMat.memptr();
	int stop = 1;
	return retExpMat;
}