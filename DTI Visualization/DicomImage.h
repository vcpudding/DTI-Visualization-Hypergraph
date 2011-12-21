#pragma once
//#include "dicomlib.h"

class DicomImage
{
public:
	DicomImage(void);
	//DicomImage(const char * fileName);
	~DicomImage(void);

private:
	static bool			_bInitialized;
	//mxArray			*	_header;
	int					_width, _height;
	unsigned			_startOfPixelData;
	unsigned			_sizeOfPixelData;
	char *				_vr;

public:
	static	int			init			();
	int					readHeader		(const char * fileName);
	int					readData		(const char * fileName, unsigned *data);
	double			*	getNumericTag	(const char * tagName, int & nElement);
	double			*	getNumericTag	(int group, int element, int & nElement);
	char			*	getCharTag		(const char * tagName, int & nElement);
	char			*	getCharTag		(int group, int element, int & nElement);
	void				dispTagValue	(void * data, int isNumeric, int nElement);

	int					getWidth		() {return _width;}
	int					getHeight		() {return _height;}
	void				getVolSize		(double volSize [3]);

public:
	static	const char * TAG_ACQUISITION_DATE;
	static	const char * TAG_PATIENTS_NAME;
	static	const char * TAG_PATIENT_ID;
	static	const char * TAG_BODY_PART;
	static	const char * TAG_INSTANCE_NUMBER;
	static	const char * TAG_ACQUISITION_NUMBER;
	static	const char * TAG_PROTOCOL_NAME;
	static	const char * TAG_DIFFUSION_B_VALUE;
	static	const char * TAG_DIFFUSION_GRADIENT;
};