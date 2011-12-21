#include "PixImage.h"

PixImage::PixImage(void)
{
}

PixImage::~PixImage(void)
{
}

void PixImage::init()
{
	_rows = 0;
	_cols = 0;
	_planes = 0;

	_data = NULL;
	_bTable = NULL;
	_nBTableItem = 0;
}

int PixImage::readRawData(QStringList orderedFileList, bool bCheckDiffusion)
{
	DicomImage * tmpImages = new DicomImage[orderedFileList.count()];
	double tmpBTable [4*1000];
	_nBTableItem = 0;
	for (int i=0; i<orderedFileList.count(); ++i)
	{
		if (!tmpImages[i].readFile(orderedFileList[i].toStdString().c_str()))
		{
			return 0;
		}
		if (i==0)
		{
			_rows = tmpImages[i].getHeight();
			_cols = tmpImages[i].getWidth();
			_planes = orderedFileList.count();
			_data = new unsigned[_rows*_cols*_planes];
		}
		memcpy(_data+i*_rows*_cols, tmpImages[i].getData(), _rows*_cols*sizeof(unsigned));
		if (bCheckDiffusion)
		{
			int nElement;
			double * pBVal = tmpImages[i].getNumericTag(DicomImage::TAG_DIFFUSION_B_VALUE, nElement);
			double * pBVec = tmpImages[i].getNumericTag(DicomImage::TAG_DIFFUSION_GRADIENT, nElement);
			if (pBVal && pBVec)
			{
				bool bItemExisted = false;
				for (int i=0; i<_nBTableItem; ++i)
				{
					if (tmpBTable[4*i] == pBVal[0] && tmpBTable[4*i+1] == pBVec[0] && tmpBTable[4*i+2] == pBVec[1] && tmpBTable[4*i+3] == pBVec[2])
					{
						bItemExisted = true;
						break;
					}
				}
				if (!bItemExisted)
				{
					tmpBTable[4*_nBTableItem] = pBVal[0];
					for (int i=0; i<3; ++i)
					{
						tmpBTable[4*_nBTableItem+i+1] = pBVec[i];
					}
					++_nBTableItem;
				}
			}
		}
	}
	if (_nBTableItem>0)
	{
		_bTable = new double[_nBTableItem];
		memcpy(_bTable, tmpBTable, _nBTableItem*sizeof(double));
		getADCData();
	}
	delete tmpImages;
	return 1;
}

void PixImage::getADCData()
{
	if (_nBTableItem != 2 || !_bTable)
	{
		return;
	}
	if (_bTable[0] > 0)
	{
		for (int j=0; j<4; ++j)
		{
			_bTable[4+j] = _bTable[j];
			_bTable[j] = 0;
		}
	}
	int planes = _planes/_nBTableItem;
	unsigned * tmpData = new unsigned[_rows*_cols*planes];
	for (int j=0; j<planes; ++j)
	{
		for (int k=0; k<_rows; ++k)
		{
			for (int l=0; l<_cols; ++l)
			{
				double bVal = _bTable[4];
				double s = getDataAt(k, l, j*_nBTableItem+1);
				double s0 = getDataAt(k, l, j*_nBTableItem);
				double adc = -1.0/bVal*log(s/s0);
				tmpData[_rows*_cols*j+_cols*k+l] = (int)adc;
			}
		}
	}
	delete _data;
	_planes = planes;
	_data = new unsigned [_rows*_cols*_planes];
	memcpy(_data, tmpData, _rows*_cols*_planes*sizeof(unsigned));
	delete tmpData;
}