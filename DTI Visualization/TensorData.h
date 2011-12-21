#pragma once
#include "volumedata.h"

#include "armadillo"
using namespace arma;

typedef struct
{
	float D [6];
	float fa;
	float d0 [3];
}Tensor;

class TensorData :
	public VolumeData
{
public:
	TensorData(void);
	~TensorData(void);

private:
	int					_nAcquisition;
	double *			_bTable;
	Tensor *			_tensorField;
	int					_noiseSignalThres;
	bool				_bInterpolate;

public:
	virtual int			readOrientedData (const QStringList & fileList, const QString & dtiFileName = "");

	virtual void		getSagittalSlice ();
	virtual void		getCoronalSlice ();
	virtual void		getAxialSlice ();
	virtual void		setInfoStr();
	void				setNoiseSignalThres (int t) {_noiseSignalThres = t;}
	void				setInterpolate (bool bSet);

	int					readBTable (const QStringList & fileList);
	int					getTensorField ();
	static void			getTensorField (const unsigned * data, int nPixel, int nAcquisition, const double * bTable, int nsThres, Tensor * tensorField);
	static void			getTensorField (const unsigned * data, int nPixel, int nAcquisition, const double * bTable, int nsThres, const fmat & G, Tensor * tensorField);
	void				saveTensorField (const QString & fileName);
	void				loadTensorField (const QString & fileName);
	Tensor				getTensorAt (int x, int y, int z) const;
	Tensor				getTensorAt (double x, double y, double z) const;
	static void			test ();

private:
	unsigned long		getIndex (int x, int y, int z, int n);
	unsigned			getData (int x, int y, int z, int n);
	unsigned			getData (double x, double y, double z, int n);

	void				removeRedundantData ();
};

fmat tensorLog (const Tensor &t);
fmat tensorLog (const fmat &tMat);
fmat tensorExp (const fmat &tMat);
fmat tensorLinearInterp (double w1, const Tensor & t1, double w2, const Tensor & t2);
fmat tensorLinearInterp (double w1, const fmat & tMat1, double w2, const fmat & tMat2);
Tensor getTensorFromMat (const fmat & tMat);