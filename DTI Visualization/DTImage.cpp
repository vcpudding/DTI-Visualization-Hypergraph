#include "DTImage.h"
#include <ifstream>

DTImage::DTImage(void)
{
	_rawImage = NULL;
	_bTable = NULL;
	_sizeX = _sizeY = _sizeZ = 0;
}

DTImage::~DTImage(void)
{
}

DTImage::DTImage(QString rawFileName)
{
	_rawImage = NULL;
	_bTable = NULL;
	_sizeX = _sizeY = _sizeZ = 0;
	if (!readRawImage(rawFileName) || !readBTable(rawFileName))
	{
		delete _rawImage;
		delete _bTable;
		_rawImage = NULL;
		_bTable = NULL;
		_sizeX = _sizeY = _sizeZ = 0;
		_bInitialized = false;
	}
	_bInitialized = true;
}

int DTImage::readRawImage(QString fileName)
{
	_rawImage = nifti_image_read(fileName.toStdString().c_str(), 1);
	if (!_rawImage)
	{
		QMessageBox msg;
		msg.setText("Cannot open selected file!");
		msg.exec();
		return 0;
	}

	_sizeX = _rawImage->dim[1];
	_sizeY = _rawImage->dim[2];
	_sizeZ = _rawImage->dim[3];

	return 1;
}

int DTImage::readBTable(QString fileName)
{
	if (!_rawImage)
	{
		return 0;
	}

	int acqCount = _rawImage->dim[3];
	_bTable = new float [4*acqCount];
	float val;

	fileName.replace(QRegExp("\.nii$"), ".bvec");
	ifstream bvecFile;
	bvecFile.open(fileName.toStdString().c_str());
	if (!bvecFile)
	{
		QMessageBox msg;
		msg.setText("Cannot open b-vec file!");
		msg.exec();
		return 0;
	}
	for (int i=0; i<3*acqCount;++i)
	{
		bvecFile >>val;
		_bTable[(i/3)*4+i%3] = val;
	}
	bvecFile.close();

	fileName.replace(QRegExp("\.bvec$"), ".bval");
	ifstream bvalFile;
	bvalFile.open(fileName.toStdString().c_str());
	if (!bvalFile)
	{
		QMessageBox msg;
		msg.setText("Cannot open b-val file!");
		msg.exec();
		return 0;
	}
	for (int i=0; i<acqCount;++i)
	{
		bvalFile >>val;
		_bTable[i*4] = val;
	}
	bvalFile.close();

	return 1;
}