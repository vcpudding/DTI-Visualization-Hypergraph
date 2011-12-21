#pragma once

#include <QString>
#include "nifti/nifti1_io.h"

class DTImage
{
public:
	DTImage(void);
	DTImage (QString rawFileName);
	~DTImage(void);

private:
	nifti_image	*	_rawImage;
	float		*	_bTable;
	int				_sizeX, _sizeY, _sizeZ;
	bool			_bInitialized;

private:
	int				readRawImage (QString fileName);
	int				readBTable (QString fileName);
};
