//#include <GL/glew.h>
//#if defined (__APPLE__) || defined(MACOSX)
//#include <GLUT/glut.h>
//#else
//#include <GL/glut.h>
//#endif
// includes
//#include <cuda_runtime.h>
//#include <cutil_inline.h>

//#include <vector_types.h>

//#include "cutil.h"
#include <QtGui/QApplication>
//#include "dtivisualization.h"
#include "tractvisualization.h"
//#include "appentrance.h"
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	/*try
	{*/
		QApplication a(argc, argv);
		/*DTIVisualization w;
		w.showMaximized();*/
		TractVisualization w;
		w.showMaximized();
		/*QDesktopWidget *desktop = a.desktop();
		int desktopWidth = desktop->width();
		int desktopHeight = desktop->height();
		AppEntrance w;
		w.setGeometry((desktopWidth-w.width())/2, (desktopHeight-w.height())/2, w.width(), w.height());
		w.show();*/
		//cudaSetDevice(cutGetMaxGflopsDeviceId());
		int r = a.exec();
		//cudaThreadExit();
		return r;
	/*} catch (...)
	{
		return 0;
	}*/
}
