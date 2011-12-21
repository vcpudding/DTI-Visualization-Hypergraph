#pragma once
#include "VolumeData.h"

#include <vector>
#include "mtrand/mtrand.h"

#include <armadillo>

using namespace std;
using namespace arma;

typedef fvec3 FloatPoint;

class SeedData :
	public VolumeData
{
public:
	SeedData(void);
	~SeedData(void);

	static const int	MAX_SEED_COUNT = 16;
	static enum			RegionType {REGION_SEED, REGION_ROI};

	friend class		SeedGLWidget;

private:
	uchar *				_colorBuf;
	bool *				_bActiveBuf;
	RegionType *		_typeBuf;
	vector<vector<int> >_seedBuf;
	int					_totalSeedCount;

	//static CRandomMersenne *	_randGenerator;
	static MTRand *		_randGenerator;

	int					isSeedPoint (int x, int y, int z, int k);
	int					isSeedPoint (int pos, int k);

public:
	virtual void		getSagittalSlice ();
	virtual void		getCoronalSlice ();
	virtual void		getAxialSlice ();

	void				setVolumeData (VolumeData * volData);
	bool				addSeedPoint (double x, double y, double z, int k);
	bool				addSeedPoint (int pos, int k);
	void				activateSeedBuf (int index);
	void				deactivateSeedBuf (int index);
	void				setColor (int index, unsigned color);
	void				setType (int index, int type);
	void				removeSeedPoint (double x, double y, double z, int k);
	void				removeSeedBuf (int index);
	void				saveSeedBuf (int index, const QString &fileName);
	void				loadSeedBuf (int index, const QString &fileName);

	bool				refreshSeedBuf ();
	FloatPoint			getRandomSeed (int & bufIndex);
};