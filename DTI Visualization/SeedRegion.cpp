#include "SeedRegion.h"

#include <gl/glut.h>

SeedRegion::SeedRegion(void)
{
	_color[0] = 1;
	_color[1] = 1;
	_color[2] = 0;
}

SeedRegion::~SeedRegion(void)
{
}

void SeedRegion::addSeedPoint(int x, int y, int z)
{
	Point3 pt = {x,y,z};
	_seedBuf.push_back(pt);
}

void SeedRegion::drawSeedRegion()
{
	if (_seedBuf.empty())
	{
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glColor3fv(_color);
	glPushMatrix();
	glScalef(_scaleX, _scaleY, _scaleZ);
	glTranslatef(-_sizeX/2, -_sizeY/2, -_sizeZ/2);
	for (int i=0; i<_seedBuf.size(); ++i)
	{
		glPushMatrix();
		glTranslatef(_seedBuf[i].x+0.5, _seedBuf[i].y+0.5, _seedBuf[i].z+0.5);
		glutSolidCube(1);
		glPopMatrix();
	}
	//glutSolidCube(100);
	glPopMatrix();
}