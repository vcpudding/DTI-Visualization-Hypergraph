#include "DicomImage.h"

#include <stdio.h>
#include <fstream>
#include <QProgressDialog>

const char * DicomImage::TAG_ACQUISITION_DATE = "Tag_0008_0022";
const char * DicomImage::TAG_PATIENTS_NAME = "Tag_0010_0010";
const char * DicomImage::TAG_PATIENT_ID = "Tag_0010_0020";
const char * DicomImage::TAG_BODY_PART = "Tag_0018_0015";
const char * DicomImage::TAG_INSTANCE_NUMBER = "Tag_0020_0013";
const char * DicomImage::TAG_ACQUISITION_NUMBER = "Tag_0020_0011";
const char * DicomImage::TAG_PROTOCOL_NAME = "Tag_0018_1030";
const char * DicomImage::TAG_DIFFUSION_B_VALUE = "Tag_0018_9087";
const char * DicomImage::TAG_DIFFUSION_GRADIENT = "Tag_0018_9089";

bool DicomImage::_bInitialized = false;

using namespace std;

DicomImage::DicomImage(void)
{
	_width = 0;
	_height = 0;
	_startOfPixelData = 0;
	_sizeOfPixelData = 0;
	//_header = NULL;
}

DicomImage::~DicomImage(void)
{
}

int DicomImage::readData(const char * fileName, unsigned *data)
{
	//if (!_header)
	//{
	//	readHeader(fileName);
	//	if (!_header)
	//	{
	//		cout <<"error";
	//		return 0;
	//	}
	//}

	//ifstream fp;
	//fp.open(fileName, ios::binary);
	//unsigned char * tmpData = new unsigned char[_sizeOfPixelData];
	//fp.seekg(_startOfPixelData, ios::beg);
	//fp.read((char*)tmpData, _sizeOfPixelData);
	//switch(_vr[1])
	//{
	//case 'B':
	//	//data = new unsigned [_width*_height];
	//	for (int i=0; i<_width*_height; ++i)
	//	{
	//		data[i] = (unsigned int)tmpData[i];
	//	}
	//	break;
	//case 'W':
	//	//data = new unsigned [_width*_height];
	//	for (int i=0; i<_width*_height; ++i)
	//	{
	//		data[i] = (reinterpret_cast<unsigned short*>(tmpData))[i];
	//	}
	//	break;
	//default:
	//	memset (data, 0, _width*_height*sizeof(unsigned));
	//}
	//delete tmpData;
	return 1;
}

int DicomImage::init()
{
	//QProgressDialog prog ("Initializing DICOM library", "loading...", 0, 1);
	//prog.setCancelButton(NULL);
	//prog.setBar(NULL);
	//prog.show();
	//prog.setValue(0);
	///* Call the MCR and library initialization functions */
	//if( !mclInitializeApplication(NULL,0) )
	//{
	//	fprintf(stderr, "Could not initialize the application.\n");
	//	return 0;
	//}

	//if (!dicomlibInitialize())
	//{
	//	fprintf(stderr,"Could not initialize the library.\n");
	//	return 0;
	//}
	//_bInitialized = true;
	//prog.setValue(1);
	return 1;
}

int DicomImage::readHeader(const char *fileName)
{
	if (!_bInitialized && !init())
	{
		return 0;
	}

	/*mxArray * input_ptr;
	mxArray * output_ptr = NULL;

	input_ptr = mxCreateString(fileName);
	mlfRead_dicom_image(1, &output_ptr, input_ptr);

	if (mxIsEmpty(output_ptr))
	{
		_header = NULL;
		return 0;
	}
	else
	{
		_header = output_ptr;
		try
		{
			int nElement;
			_height = (int)getNumericTag(0x0028, 0x0010, nElement)[0];
			_width = (int)getNumericTag(0x0028, 0x0011, nElement)[0];
			_startOfPixelData = (unsigned)getNumericTag("StartOfPixelData", nElement)[0];
			_sizeOfPixelData = (unsigned)getNumericTag("SizeOfPixelData", nElement)[0];
			_vr = getCharTag("VROfPixelData", nElement);
		}
		catch (...)
		{
			return 0;
		}
		return 1;
	}*/
	return 1;
}

double * DicomImage::getNumericTag(const char *tagName, int & nElement)
{
	/*
	if (!_header)
		{
			return NULL;
		}*/
	
	/*mxArray * tmp = mxGetField(_header, 0, tagName);
	if (!tmp)
	{
		cout <<"Tag doesn't exist"<<endl;
		return NULL;
	}
	if (!mxIsNumeric(tmp))
	{
		cout <<"Tag is not numeric"<<endl;
		return NULL;
	}
	mwSize nDim = mxGetNumberOfDimensions(tmp);
	const mwSize * dims = mxGetDimensions(tmp);
	nElement = 1;
	for (unsigned i=0; i<nDim; ++i)
	{
		nElement *= dims[i];
	}
	double * ret = new double [nElement];
	memcpy(ret, mxGetPr(tmp), nElement * sizeof(double));
	return ret;*/
	return NULL;
}

double * DicomImage::getNumericTag(int group, int element, int & nElement)
{
	char s [256];
	sprintf(s, "Tag_%04x_%04x", group, element);
	return getNumericTag(s, nElement);
}

char * DicomImage::getCharTag(const char *tagName, int &nElement)
{
	/*
	if (!_header)
		{
			return NULL;
		}*/
	
	/*
	mxArray * tmp = mxGetField(_header, 0, tagName);
		if (!tmp)
		{
			cout <<"Tag doesn't exist"<<endl;
			return NULL;
		}
		if (!mxIsChar(tmp))
		{
			cout <<"Tag is not character string"<<endl;
			return NULL;
		}
		const mwSize * dims = mxGetDimensions(tmp);
		nElement = dims[1];
		mxChar * srcData = (mxChar *)mxGetData(tmp);
		char * ret = new char [nElement+1];
		for (int i=0; i<nElement; ++i)
		{
			ret[i] = (char)srcData[i];
		}
		ret[nElement] = '\0';
		++nElement;
		return ret;*/
	return NULL;
	
}

char * DicomImage::getCharTag(int group, int element, int & nElement)
{
	char s [256];
	sprintf(s, "Tag_%04x_%04x", group, element);
	return getCharTag(s, nElement);
}

void DicomImage::dispTagValue(void *data, int isNumeric, int nElement)
{
	if (!data || nElement <= 0)
	{
		//cout <<"Empty data value"<<endl;
		return;
	}
	if (isNumeric)
	{
		double * pData = (double*) data;
		for (int i=0; i<nElement-1; ++i)
		{
			//cout <<pData[i] <<",";
		}
		//cout <<pData[nElement-1]<<endl;
	}
	else
	{
		char * pData = (char*) data;
		for (int i=0; i<nElement; ++i)
		{
			//cout <<pData[i];
		}
		//cout<<endl;
	}
}

void DicomImage::getVolSize(double volSize [3])
{
	int nElement;
	double * volSizeXY = getNumericTag(0x0028, 0x0030, nElement);
	memcpy(volSize, volSizeXY, nElement*sizeof(double));
	double * volSizeZ = getNumericTag(0x0018, 0x0088, nElement);
	volSize[2] = volSizeZ[0];
}