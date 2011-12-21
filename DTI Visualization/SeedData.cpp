#include "SeedData.h"

#include <gl/glu.h>
#include <math.h>
#include <fstream>
#include <time.h>

MTRand * SeedData::_randGenerator = NULL;

SeedData::SeedData(void)
{
	_colorBuf = new uchar [3*MAX_SEED_COUNT];
	_bActiveBuf = new bool [MAX_SEED_COUNT];
	_typeBuf = new RegionType [MAX_SEED_COUNT];
	memset(_colorBuf, 0, 3*MAX_SEED_COUNT*sizeof(uchar));
	memset(_bActiveBuf, 0, MAX_SEED_COUNT*sizeof(bool));
	memset(_typeBuf, 0, MAX_SEED_COUNT*sizeof(RegionType));

	_seedBuf = vector<vector<int> >(0);
	_totalSeedCount = 0;
}

SeedData::~SeedData(void)
{
}

void SeedData::setVolumeData(VolumeData * volData)
{
	_sizeX = volData->_sizeX;
	_sizeY = volData->_sizeY;
	_sizeZ = volData->_sizeZ;

	_scaleX = volData->_scaleX;
	_scaleY = volData->_scaleY;
	_scaleZ = volData->_scaleZ;

	_pixSizeX = volData->_pixSizeX;
	_pixSizeY = volData->_pixSizeY;
	_pixSizeZ = volData->_pixSizeZ;

	_pSliceIdx = volData->_pSliceIdx;
	_cSliceIdx = volData->_cSliceIdx;
	_aSliceIdx = volData->_aSliceIdx;

	_nVoxels = _sizeX*_sizeY*_sizeZ;
	_data = new unsigned [_nVoxels];
	memset(_data, 0, sizeof(unsigned)*_nVoxels);
	

	memset(_colorBuf, 0, 3*MAX_SEED_COUNT*sizeof(uchar));
	memset(_bActiveBuf, false, MAX_SEED_COUNT*sizeof(bool));
	memset(_typeBuf, 0, MAX_SEED_COUNT*sizeof(RegionType));

	/*_colorBuf[0] = 255;
	_colorBuf[1] = 240;
	_colorBuf[2] = 0;*/
	_bActiveBuf[0] = true;
}

void SeedData::getSagittalSlice()
{
	if (!_data)
	{
		return;
	}

	if (!_pSlice)
	{
		_pSlice = new unsigned char [_sizeY*_sizeZ*3];
		glGenTextures(1, &_pTexId);
	}

	memset(_pSlice, 0, _sizeY*_sizeZ*3*sizeof(unsigned char));
	if (_pSliceIdx<0 || _pSliceIdx>_sizeX-1)
	{
		goto BindTexture;
	}

	for (int i=0; i<_sizeZ; ++i)
	{
		for (int j=0; j<_sizeY; ++j)
		{
			for (int k=0; k<MAX_SEED_COUNT; ++k)
			{
				if (_bActiveBuf[k] && isSeedPoint(_pSliceIdx, j, i, k))
				{
					for (int l=0; l<3; ++l)
					{
						_pSlice[3*(i*_sizeY+j)+l] = _colorBuf[3*k+l];
					}
					break;
				}
			}
		}
	}

BindTexture:
	// allocate a texture name
	glBindTexture( GL_TEXTURE_2D, _pTexId );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	// build our texture mipmaps
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, _sizeY, _sizeZ,
		GL_RGB, GL_UNSIGNED_BYTE, _pSlice);
}

void SeedData::getCoronalSlice()
{
	if (!_data)
	{
		return;
	}

	if (!_cSlice)
	{
		_cSlice = new unsigned char [_sizeX*_sizeZ*3];
		glGenTextures(1, &_cTexId);
	}

	memset(_cSlice, 0, _sizeX*_sizeZ*3*sizeof(unsigned char));
	if (_aSliceIdx<0 || _aSliceIdx>_sizeY-1)
	{
		goto BindTexture;
	}

	for (int i=0; i<_sizeZ; ++i)
	{
		for (int j=0; j<_sizeX; ++j)
		{
			for (int k=0; k<MAX_SEED_COUNT; ++k)
			{
				if (_bActiveBuf[k] && isSeedPoint(j, _cSliceIdx, i, k))
				{
					for (int l=0; l<3; ++l)
					{
						_cSlice[3*(i*_sizeX+j)+l] = _colorBuf[3*k+l];
					}
					break;
				}
			}
		}
	}

BindTexture:
	glBindTexture( GL_TEXTURE_2D, _cTexId );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	// build our texture mipmaps
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, _sizeX, _sizeZ,
		GL_RGB, GL_UNSIGNED_BYTE, _cSlice);
}

void SeedData::getAxialSlice()
{
	if (!_data)
	{
		return;
	}

	if (!_aSlice)
	{
		_aSlice = new unsigned char [_sizeX*_sizeY*3];
		glGenTextures(1, &_aTexId);
	}

	memset(_aSlice, 0, _sizeX*_sizeY*3*sizeof(unsigned char));
	if (_aSliceIdx<0 || _aSliceIdx>_sizeZ-1)
	{
		goto BindTexture;
	}

	for (int i=0; i<_sizeY; ++i)
	{
		for (int j=0; j<_sizeX; ++j)
		{
			for (int k=0; k<MAX_SEED_COUNT; ++k)
			{
				if (_bActiveBuf[k] && isSeedPoint(j, i, _aSliceIdx, k))
				{
					for (int l=0; l<3; ++l)
					{
						_aSlice[3*(i*_sizeX+j)+l] = _colorBuf[3*k+l];
					}
					break;
				}
			}
		}
	}

BindTexture:
	glBindTexture( GL_TEXTURE_2D, _aTexId );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, _sizeX, _sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, _aSlice);
}

int SeedData::isSeedPoint(int x, int y, int z, int k)
{
	unsigned data = _data[getIndex(x, y, z)];
	unsigned tmp = data & (1<<k);
	return tmp>0?1:0;
}

int SeedData::isSeedPoint(int pos, int k)
{
	unsigned data = _data[pos];
	unsigned tmp = data & (1<<k);
	return tmp>0?1:0;
}

bool SeedData::addSeedPoint(double x, double y, double z, int k)
{
	x = floor(x);
	y = floor(y);
	z = floor(z);
	if (x<0||x>=_sizeX-1 || y<0||y>=_sizeY-1 || z<0||z>_sizeZ-1)
	{
		return false;
	}
	unsigned pos = getIndex(x, y, z);
	unsigned currentMask = 1<<k;
	
	_data[pos] |= currentMask;
	return true;
}

bool SeedData::addSeedPoint(int pos, int k)
{
	if (pos<0 || pos>_nVoxels)
	{
		return false;
	}
	unsigned currentMask = 1<<k;
	
	_data[pos] |= currentMask;
	return true;
}

void SeedData::activateSeedBuf(int index)
{
	_bActiveBuf[index] = true;
}

void SeedData::setColor(int index, unsigned color)
{
	_colorBuf[3*index] = (0xFF0000&color)>>16;
	_colorBuf[3*index+1] = (0x00FF00&color)>>8;
	_colorBuf[3*index+2] = 0xFF&color;
}

void SeedData::setType(int index, int type)
{
	_typeBuf[index] = (RegionType)type;
}

void SeedData::removeSeedPoint(double x, double y, double z, int k)
{
	x = floor(x);
	y = floor(y);
	z = floor(z);
	if (x<0||x>=_sizeX-1 || y<0||y>=_sizeY-1 || z<0||z>_sizeZ-1)
	{
		return;
	}
	unsigned pos = getIndex(x, y, z);
	unsigned mask = ~(1<<k);
	_data[pos] &= mask;
}

void SeedData::removeSeedBuf(int index)
{
	_bActiveBuf[index] = false;
	unsigned mask = ~(1<<index);
	for (int i=0; i<_nVoxels; ++i)
	{
		_data[i] &= mask;
	}
	//_seedBuf[index].clear();
}

void SeedData::loadSeedBuf(int index, const QString &fileName)
{
	_bActiveBuf[index] = true;
	unsigned mask = 1<<index;
	ifstream inFile (fileName.toStdString().c_str());
	int pos;
	while (inFile >> pos)
	{
		addSeedPoint(pos, index);
	}
	inFile.close();
}

void SeedData::saveSeedBuf(int index, const QString &fileName)
{
	unsigned mask = 1<<index;
	ofstream outFile (fileName.toStdString().c_str());
	for (int i=0; i<_nVoxels; ++i)
	{
		if (isSeedPoint(i, index))
		{
			outFile <<i <<"\n";
		}
	}
	outFile.close();
}

void SeedData::deactivateSeedBuf(int index)
{
	_bActiveBuf[index] = false;
}

bool SeedData::refreshSeedBuf()
{
	_totalSeedCount = 0;
	_seedBuf.resize(MAX_SEED_COUNT);
	for (int i=0; i<MAX_SEED_COUNT; ++i)
	{
		_seedBuf[i].clear();
		if (!_bActiveBuf[i] || _typeBuf[i] != REGION_SEED)
		{
			continue;
		}
		for (int j=0; j<_nVoxels; ++j)
		{
			if (isSeedPoint(j, i))
			{
				_seedBuf[i].push_back(j);
				++_totalSeedCount;
			}
		}
	}
	return _totalSeedCount>0?true:false;
}

FloatPoint SeedData::getRandomSeed(int & bufIndex)
{
	FloatPoint pt;
	if (_totalSeedCount<=0)
	{
		pt.fill(0);
		return pt;
	}

	if (!_randGenerator)
	{
		_randGenerator = new MTRand(time(NULL));
	}
	int bufRand = floor((_totalSeedCount)*(*_randGenerator)());
	bufIndex = 0;
	int seedCount = _seedBuf[0].size();
	while (bufRand>=seedCount)
	{
		++bufIndex;
		seedCount += _seedBuf[bufIndex].size();
	}
	int seedRand = floor((_seedBuf[bufIndex].size())*(*_randGenerator)());
	int seedIndex = _seedBuf[bufIndex][seedRand];
	pt(0) = seedIndex % _sizeX;
	pt(1) = (seedIndex/_sizeX) % _sizeY;
	pt(2) = seedIndex/_sizeX/_sizeY;
	float * ptMem = pt.memptr();
	return pt;
}