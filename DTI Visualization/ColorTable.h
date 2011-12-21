#pragma once

#include <QImage>
#include <QString>

#include <vector>

using namespace std;

class ColorTable
{
public:
	ColorTable(void);
	ColorTable(const QString & spectrumFileName);
	~ColorTable(void);

private:
	vector<uchar>	_spectrumImg;
	int				_sWidth, _sHeight;
	uchar			_colBuf [4];
	int				_nColors;

public:
	void			loadSpectrum (const QString & fileName);
	void			drawSpectrum (int orientation = 1);
	uchar *			getColor (double a);
	void			setNumOfColors (int n);
	void			setColor (int i, uchar r, uchar g, uchar b);
	uchar *			getColor (int i);
};
