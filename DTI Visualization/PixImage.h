#pragma once

#include <QStringList>
#include <math.h>

#include "DicomImage.h"

class PixImage
{
public:
	PixImage(void);
	~PixImage(void);

private:
	int				_rows, _cols, _planes;
	unsigned	*	_data;
	double		*	_bTable;
	int				_nBTableItem;

private:
	void			init		();

public:
	int				readRawData (QStringList orderedFileList, bool bCheckDiffusion = true);
	void			getADCData ();
	unsigned		getDataAt	(int row, int col, int plane) {return _data[_rows*_cols*plane+_cols*row+col];}
};
