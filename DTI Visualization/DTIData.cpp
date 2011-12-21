#include "DTIData.h"

#include <QMessageBox>
#include "DicomImage.h"
#include "armadillo"

DTIData::DTIData(void)
{
	_tensorField = NULL;
	_nAcquisition = 0;
	_bTable = NULL;
}

DTIData::~DTIData(void)
{
}

int DTIData::readBTable(const QStringList & fileList)
{
	if (_bTable)
	{
		delete _bTable;
		_bTable = NULL;
	}
	double tmpBTable [400];
	_nAcquisition = 0;
	DicomImage * dcmImage = new DicomImage();
	while (true)
	{
		dcmImage->readHeader(fileList[_nAcquisition].toStdString().c_str());
		try
		{
			int nElement;
			double bVal = dcmImage->getNumericTag(0x0000, 0x0000, nElement)[0];
			if (_nAcquisition>0 && bVal<=0)
			{
				break;
			}
			double * bVecs = dcmImage->getNumericTag(0x0000, 0x0000, nElement);
			tmpBTable[4*_nAcquisition] = bVal;
			memcpy(tmpBTable+4*_nAcquisition+1, bVecs, 3*sizeof(double));
			++_nAcquisition;
		}
		catch (...)
		{
			QMessageBox::warning(NULL, "File Error", "Cannot read diffusion information from the selected series!");
			return 0;
		}
	}

	_bTable = new double[4*_nAcquisition];
	memcpy(_bTable, tmpBTable, 4*_nAcquisition);
	return 1;
}

int DTIData::reconstructTensorField(const QStringList &fileList)
{
	readBTable(fileList);
	readData(fileList);
	if (!_data || !_bTable)
	{
		return 0;
	}

	int nVoxels = _sizeX*_sizeY*_sizeZ;
	
	/*Reconstruct DTI*/
	memset(_tensorField, 0, nVoxels*sizeof(Tensor));
	mat G (_nAcquisition-1, 6);
	for (int i=1; i<_nAcquisition+1; ++i)
	{
		G(i-1, 0) = _bTable[i*4+1]*_bTable[i*4+1]; //g1*g1
		G(i-1, 1) = _bTable[i*4+2]*_bTable[i*4+2]; //g2*g2
		G(i-1, 2) = _bTable[i*4+3]*_bTable[i*4+3]; //g3*g3
		G(i-1, 3) = 2*_bTable[i*4+1]*_bTable[i*4+2]; //2*g1*g2
		G(i-1, 4) = 2*_bTable[i*4+1]*_bTable[i*4+3]; //2*g1*g3
		G(i-1, 5) = 2*_bTable[i*4+2]*_bTable[i*4+3]; //2*g2*g3
	}
	vec B (_nAcquisition-1);

	mat D (3,3);
	QProgressDialog prog("Reconstructing diffusion tensor field", "", 0, nVoxels);
	prog.setCancelButton(NULL);
	prog.show();
	for (i=0; i<nVoxels; ++i)
	{
		bool bHasInvalidData = false;
		for (j=0; j<_nAcquisition-1; ++j)
		{
			B(j) = data[j*nVoxels+i];
			if (B(j)>=1.0e10)
			{
				bHasInvalidData = true;
				break;
			}
		}
		if (bHasInvalidData)
		{
			continue;
		}
		vec d = solve(G, B);
		for (j=0; j<6; ++j)
		{
			_tensorField[i].D[j] = d(j);
		}
		D <<d(0) <<d(3) <<d(4) <<endr <<d(3) <<d(1) <<d(5) <<endr <<d(4) <<d(5) <<d(2) <<endr;
		vec eigVal;
		mat eigVec;
		eig_sym(eigVal, eigVec, D);
		ucolvec sorts = sort_index(eigVal, 1);
		for (j = 0; j<3; ++j)
		{
			if (eigVal(sorts(0))<=0)
			{
				continue;
			}
			_tensorField[i].eVal[j] = eigVal(sorts(j));
			for (int k=0; k<3; ++k)
			{
				_tensorField[i].eVec[3*k+j] = eigVec(k, sorts(j));
			}
		}
		prog.setValue(i);
	}

	return 1;
}