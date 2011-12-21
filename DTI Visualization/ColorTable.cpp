#include "ColorTable.h"

#include <math.h>
#include <QColor>
#include <QMessageBox>

#include <GL/glut.h>

ColorTable::ColorTable(void)
{
	loadSpectrum("Resources\\spectrum.bmp");
}

ColorTable::ColorTable(const QString & spectrumFileName)
{
	loadSpectrum(spectrumFileName);
}

ColorTable::~ColorTable(void)
{
}

void ColorTable::loadSpectrum(const QString &fileName)
{

	QImage img = (QImage(fileName, "BMP")).rgbSwapped();
	//_spectrumImg = new uchar [img.bytesPerLine()*img.height()];
	//memcpy(_spectrumImg, img.bits(), img.bytesPerLine()*img.height());
	_spectrumImg.resize(img.bytesPerLine()*img.height());
	_spectrumImg.assign(img.bits(), img.bits()+_spectrumImg.size());
	_sWidth = img.width();
	_sHeight = img.height();
	_nColors = _sWidth;
}

void ColorTable::drawSpectrum(int orientation)
{
	//int width =1, height =1;
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0, width, 0, height);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//float rot = orientation==1?90:0;
	//glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	//GLuint texture;
	//// allocate a texture name
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glGenTextures( 1, &texture );// select our current texture
	//glBindTexture( GL_TEXTURE_2D, texture );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	//// build our texture mipmaps
	//gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB4, _sWidth, _sHeight,
	//	GL_RGBA, GL_UNSIGNED_BYTE, _spectrumImg.begin());

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture( GL_TEXTURE_2D, texture );
	//glPushMatrix();
	//glTranslatef(height, 0, 0);
	//glRotatef(rot, 0.0, 0.0, 1.0);
	//glBegin(GL_QUADS);
	//glTexCoord2d(0.0,0.0); glVertex2f(0.0, 0.0);
	//glTexCoord2d(1.0,0.0); glVertex2f(width, 0.0);
	//glTexCoord2d(1.0,1.0); glVertex2f(width, height);
	//glTexCoord2d(0.0,1.0); glVertex2f(0.0, height);
	//glEnd();
	//glPopMatrix();
	//glDisable(GL_TEXTURE_2D);
}

uchar * ColorTable::getColor(double a)
{
	double pos = a*(_sWidth-1);
	int p1 = floor(pos);
	int p2 = ceil(pos);
	double t = pos-p1;
	for (int i=0; i<4; ++i)
	{
		_colBuf[i] = (1-t)*_spectrumImg[4*p1+i] + t*_spectrumImg[4*p2+i];
	}
	_colBuf[3] = 255;
	return _colBuf;
}

void ColorTable::setNumOfColors(int n)
{
	_nColors = n;
	_spectrumImg.resize(n*4);
}

void ColorTable::setColor(int i, uchar r, uchar g, uchar b)
{
	_spectrumImg[4*i] = r;
	_spectrumImg[4*i+1] = g;
	_spectrumImg[4*i+2] = b;
	_spectrumImg[4*i+3] = 255;
}

uchar * ColorTable::getColor(int i)
{
	for (int j=0; j<3; ++j)
	{
		_colBuf[j] = i<0?125:_spectrumImg[4*i+j];
	}
	_colBuf[3] = 255;
	return _colBuf;
}