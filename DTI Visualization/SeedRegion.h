#pragma once

#include <vector>

using namespace std;

typedef struct  
{
	int x, y, z;
} Point3;

//typedef struct  
//{
//	float x,y,z;
//} FloatPoint3;

class SeedRegion
{
public:
	SeedRegion(void);
	~SeedRegion(void);

	friend class		TensorData;

private:
	int					_sizeX, _sizeY, _sizeZ;
	double				_scaleX, _scaleY, _scaleZ;
	vector<Point3>		_seedBuf;
	float				_color [3];

public:
	void				addSeedPoint (int x, int y, int z);
	void				setColor (float r, float g, float b) {_color[0] = r; _color[1] = g; _color[2] = b;}
	void				drawSeedRegion ();
	//FloatPoint3			getRandomSeed ();
};
