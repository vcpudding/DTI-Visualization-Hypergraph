#pragma once
#include "volumedata.h"

typedef struct
{
	float D [6];
	float eVal [3];
	float eVec [9];
}Tensor;

class DTIData :
	public VolumeData
{
public:
	DTIData(void);
	~DTIData(void);

private:
	float *			_tensorField;
	int				_nAcquisition;
	double *		_bTable;

public:
	int				readBTable (const QStringList & fileList);
	int				reconstructTensorField (const QStringList & fileList);
	
};
