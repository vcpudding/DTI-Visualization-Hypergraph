#include "expresultvis.h"

#include <gl/glut.h>
#include <gl/gle.h>
#include <fstream>
#include "TemplateFiberData.h"

#include <QDir>

#define M_PI 3.1415926

expResultVis::expResultVis(QWidget *parent)
	: QGLWidget(parent)
{
	ui.setupUi(this);

	_labeledFiberData = NULL;

	_ordinaryMDS = NULL;
	_constrainedMDS = NULL;
	_fibToRegDist = NULL;
	_clusters = NULL;
	_colorTable = new ColorTable();
	_fiberData = NULL;
}

expResultVis::~expResultVis()
{

}

void expResultVis::initializeGL()
{
	glShadeModel(GL_SMOOTH);
	//Enable Lighting
	glEnable(GL_LIGHTING);

	//Specify a single directional light
	float color_intensity=0.8;
	GLfloat light_color[] = { color_intensity,color_intensity,color_intensity,1.0};
	//GLfloat light_color[] = { 1.0f,1.0f,1.0f,1.0f};
	GLfloat light_pos[] = { 0.3f,0.3f,0.3f,1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);	

	glEnable(GL_LIGHT0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void expResultVis::resizeGL(int width, int height)
{
	_widgetWidth = width;
	_widgetHeight = height;
}

void expResultVis::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//drawAtlasClusterMDS();
	//drawAtlasTract();
	//drawAtlasTractEmbs();
	//drawLabeledEmbs();
	//drawGroupEmbs();

	drawOrdinaryAndConstrainedMDS();
}

void expResultVis::drawAtlasClusterMDS()
{
	if (_atlasMDS.empty())
	{
		ifstream inFile("test data/tract/atlas/clusterMDS.txt");
		int clusterIdx;
		float x,y;

		while (inFile >>clusterIdx >>x >>y)
		{
			FloatPoint pt;
			pt <<x <<y <<clusterIdx;
			_atlasMDS.push_back(pt);
		}

		inFile.close();
	}

	float minX = 1.0e5, minY = 1.0e5, maxX = -1.0e5, maxY = -1.0e5;
	for (int i=0; i<_atlasMDS.size(); ++i)
	{
		float x = _atlasMDS[i](0);
		float y = _atlasMDS[i](1);
		minX = x<minX?x:minX;
		maxX = x>maxX?x:maxX;
		minY = y<minY?y:minY;
		maxY = y>maxY?y:maxY;
	}

	glViewport(5,5,500,500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(minX-5, maxX+5, minY-5, maxY+5);
	glOrtho(minX-5, maxX+5, minY-5, maxY+5, -20, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glColor3f(1,1,1);
	glPushMatrix();
	glScalef((maxX-minX+10), (maxX-minX+10), 1);
	glutSolidCube(1);
	glPopMatrix();

	for (int i=0; i<_atlasMDS.size(); ++i)
	{
		glColor3ubv(TemplateFiberData::getTplColor(_atlasMDS[i](2)));
		glPushMatrix();
		glTranslatef(_atlasMDS[i](0), _atlasMDS[i](1), 0);
		glutSolidSphere(2, 20, 20);
		glPopMatrix();
		/*glColor3f(1,0,1);
		glVertex2f(_atlasMDS[i](0), _atlasMDS[i](1));*/
	}
}

void expResultVis::drawAtlasTract()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, _widgetWidth*1.0/_widgetHeight, 1, 10000);

	glViewport(0, _widgetHeight/2, _widgetWidth/2, _widgetHeight/2);
	//glViewport(0, 0, _widgetWidth, _widgetHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0,0,-180);
	glRotatef(180, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	TemplateFiberData::drawAtlasTracts();
	glPopMatrix();


	glViewport(_widgetWidth/2, _widgetHeight/2, _widgetWidth/2, _widgetHeight/2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0,0,-180);
	glRotatef(90, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	TemplateFiberData::drawAtlasTracts();
	glPopMatrix();

	glViewport(0, 0, _widgetWidth/2, _widgetHeight/2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0,0,-180);
	//glRotatef(-90, 0, 0, 1);
	glRotatef(180, 0, 0, 1);
	TemplateFiberData::drawAtlasTracts();
	glPopMatrix();

	glViewport(_widgetWidth/2, 0, _widgetWidth/2, _widgetHeight/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float w = M_PI * _widgetWidth/_widgetHeight;
	float h = M_PI;
	gluOrtho2D(-w, w, -M_PI, M_PI);

	glDisable(GL_DEPTH_TEST);
	glColor3f(1,1,1);
	glPushMatrix();
	glScalef(2*w, 2*h, 1);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xF0F0); 
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(0, -h);
	glVertex2f(0, h);
	glVertex2f(-w, 0);
	glVertex2f(w, 0);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	TemplateFiberData::drawAtlasEmbeds(1, false, true);
}

void expResultVis::drawAtlasTractEmbs()
{
	vector<int> highlightClusters;
	highlightClusters.push_back(9);
	highlightClusters.push_back(12);
	highlightClusters.push_back(112);
	highlightClusters.push_back(6);
	highlightClusters.push_back(106);
	highlightClusters.push_back(8);
	highlightClusters.push_back(108);


	glViewport(5, 5, _widgetWidth/2-10, _widgetHeight-10);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float w = M_PI * (_widgetWidth/2)/_widgetHeight;
	float h = M_PI * _widgetHeight/(_widgetWidth/2);
	if (w<h)
	{
		glOrtho(-w, w, -M_PI, M_PI, -20,20);
		h = M_PI;
	} else
	{
		glOrtho(-M_PI, M_PI, -h, h, -20,20);
		w = M_PI;
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.5, 0.5, 0.5);

	/*glDisable(GL_DEPTH_TEST);
	glColor3f(1,1,1);
	glPushMatrix();
	glScalef(2*w, 2*h, 1);
	glutSolidCube(1);
	glPopMatrix();*/

	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xF0F0); 
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(0, -h);
	glVertex2f(0, h);
	glVertex2f(-w, 0);
	glVertex2f(w, 0);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (int i=0; i<highlightClusters.size(); ++i)
	{
		char fileName[512];
		sprintf_s(fileName, 512, "test data/tract/atlas/tract_embs_%02d.txt", highlightClusters[i]);
		ifstream inFile (fileName);

		uchar * clusterCol = TemplateFiberData::getTplColor(highlightClusters[i]);
		glColor4ub(clusterCol[0]*0.2, clusterCol[1]*0.2, clusterCol[2]*0.2, 35);
		float phi, z;
		CylindPoint pt;
		while(inFile >>phi >>z)
		{
			glPushMatrix();
			glTranslatef(phi, z, 0);
			glutSolidSphere(0.01, 20, 20);
			glPopMatrix();
		}
		inFile.close();

		glColor3ubv(clusterCol);
		glPushMatrix();
		glTranslatef(phi, z, 10);
		glutSolidSphere(0.04, 20, 20);
		glPopMatrix();
	}
	glDisable(GL_BLEND);

	glEnable(GL_LIGHTING);

	glViewport(_widgetWidth/2, 0, _widgetWidth/2, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, _widgetWidth*0.5/_widgetHeight, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-150);
	glRotatef(180, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	TemplateFiberData::drawAtlasTracts(highlightClusters);

	glDisable(GL_LIGHTING);
}

void expResultVis::drawLabeledEmbs()
{
	if(!_labeledFiberData)
	{
		_labeledFiberData = new FiberData();
		_labeledFiberData->loadFibers("test data/tract/test data/labeled/labeled.fib", 144, 144, 64);
		_labeledFiberData->loadClusters("test data/tract/test data/labeled/labeled.cluster");
		_labeledFiberData->loadEmbeddings("test data/tract/test data/labeled/labeled.emb");
	}

	glViewport(0, 0, _widgetWidth/2, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, _widgetWidth*0.5/_widgetHeight, 1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-60);
	glRotatef(180, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glTranslatef(-72, -72, -32);
	float tmpFibF [6000];
	gleDouble tmpFibD [2000][3];
	for (int i=0; i<_labeledFiberData->nFibers(); ++i)
	{
		glColor3ubv(TemplateFiberData::getTplColor(_labeledFiberData->_clusters[i]+4));
		int nPoint = _labeledFiberData->fiber(i, tmpFibF);
		for (int i=0; i<nPoint; ++i)
		{
			tmpFibD[i][0] = tmpFibF[i*3];
			tmpFibD[i][1] = tmpFibF[i*3+1];
			tmpFibD[i][2] = tmpFibF[i*3+2];
		}
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glViewport(_widgetWidth/2+5, 5, _widgetWidth/2-10, _widgetHeight-10);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float w = M_PI * (_widgetWidth/2)/_widgetHeight;
	float h = M_PI * _widgetHeight/(_widgetWidth/2);
	if (w<h)
	{
		glOrtho(-w, w, -M_PI, M_PI, -20,20);
		h = M_PI;
	} else
	{
		glOrtho(-M_PI, M_PI, -h, h, -20,20);
		w = M_PI;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(0.2, 0.2, 0.2);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xF0F0); 
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(0, -h);
	glVertex2f(0, h);
	glVertex2f(-w, 0);
	glVertex2f(w, 0);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	TemplateFiberData::drawAtlasEmbeds(1, false, false);

	vector<int> clusterIdx;
	clusterIdx.push_back(8);
	clusterIdx.push_back(9);
	clusterIdx.push_back(12);

	for (int i=0; i<_labeledFiberData->nFibers(); ++i)
	{
		glColor3ubv(TemplateFiberData::getTplColor(_labeledFiberData->_clusters[i]+4));
		glPushMatrix();
		CylindPoint embedPt = _labeledFiberData->_embedPoints[i].posLLE;
		glTranslatef(embedPt(0), embedPt(1), 0);
		glutSolidSphere(0.08, 20, 20);
		glPopMatrix();
	}
}

void expResultVis::drawGroupEmbs()
{
	if (_healthyEmbeds.empty())
	{
		QDir groupDir ("test data/tract/test data/group comparison");
		QStringList healthyFolds = groupDir.entryList(QStringList("healthy*"), QDir::Dirs);
		QStringList subjectFolds = groupDir.entryList(QStringList("subject*"), QDir::Dirs);

		for (int i=0; i<healthyFolds.count(); ++i)
		{
			groupDir.cd(healthyFolds[i]);
			QString embFileName = groupDir.entryList(QStringList("*.emb"), QDir::Files)[0];
			loadEmbFile(groupDir.absoluteFilePath(embFileName), _healthyEmbeds);
			groupDir.cdUp();
		}

		for (int i=0; i<subjectFolds.count(); ++i)
		{
			groupDir.cd(subjectFolds[i]);
			QString embFileName = groupDir.entryList(QStringList("*.emb"), QDir::Files)[0];
			loadEmbFile(groupDir.absoluteFilePath(embFileName), _subjectEmbeds);
			groupDir.cdUp();
		}
	}

	glDisable(GL_LIGHTING);

	glViewport(0, 0, _widgetWidth, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float w = M_PI * _widgetWidth/_widgetHeight;
	float h = M_PI * _widgetHeight/_widgetWidth;
	if (w<h)
	{
		glOrtho(-M_PI, M_PI, -h, h, -20,20);
		w = M_PI;
	} else
	{
		glOrtho(-w, w, -M_PI, M_PI, -20,20);
		h = M_PI;
	}

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1,1,1);
	glPushMatrix();
	glScalef(2*w, 2*h, 1);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xF0F0); 
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(0, -h);
	glVertex2f(0, h);
	glVertex2f(-w, 0);
	glVertex2f(w, 0);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	TemplateFiberData::drawAtlasEmbeds(1, false, false);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4ub(0, 114, 188, 25);
	for (int i=0; i<_healthyEmbeds.size(); ++i)
	{
		glPushMatrix();
		CylindPoint embedPt = _healthyEmbeds[i];
		glTranslatef(embedPt(0), embedPt(1), 0);
		glutSolidSphere(0.06, 20, 20);
		glPopMatrix();
	}

	glColor4ub(198, 0, 51, 25);
	for (int i=0; i<_subjectEmbeds.size(); ++i)
	{
		glPushMatrix();
		CylindPoint embedPt = _subjectEmbeds[i];
		glTranslatef(embedPt(0), embedPt(1), 0);
		glutSolidSphere(0.06, 20, 20);
		glPopMatrix();
	}

	glDisable(GL_BLEND);
}

void expResultVis::loadEmbFile(const QString &fileName, vector<CylindPoint> &ptBuf)
{
	ifstream inFile (fileName.toStdString().c_str(), ios::binary);
	if (!inFile)
	{
		return;
	}

	float angle, z;
	float tmpPt [2];
	EmbedPoint embPt;
	while(!inFile.eof())
	{
		inFile.read((char*)tmpPt, 2*sizeof(float));
		embPt.posLLE <<tmpPt[0] <<tmpPt[1];
		inFile.read((char*)tmpPt, 2*sizeof(float));
		embPt.posProj <<tmpPt[0] <<tmpPt[1];
		float d=0;
		inFile.read((char*)&d, sizeof(float));
		embPt.minDist = d;
		ptBuf.push_back(embPt.posLLE);
	}
	inFile.close();
}

void expResultVis::drawOrdinaryAndConstrainedMDS()
{
	int nFibers = 5348;
	int nRegions = 3;
	if (!_ordinaryMDS)
	{
		_ordinaryMDS = new float[nFibers*2];
		ifstream ordinaryMDSFile ("D:/Stella/Research/Sample data/test data/landmark/ordinary mds.dat", ios::binary);
		ordinaryMDSFile.read((char*)_ordinaryMDS, nFibers*2*sizeof(float));
		ordinaryMDSFile.close();
	}
	if (!_constrainedMDS)
	{
		_constrainedMDS = new float[(nFibers+1)*2];
		ifstream constrainedMDSFile ("D:/Stella/Research/Sample data/test data/landmark/constrained mds w=N.dat", ios::binary);
		constrainedMDSFile.read((char*)_constrainedMDS, (nFibers+1)*2*sizeof(float));
		constrainedMDSFile.close();
	}
	if (!_fibToRegDist)
	{
		_fibToRegDist = new float[nFibers*nRegions];
		for (int i=0; i<nRegions; ++i)
		{
			QString distFileName = QString("D:/Stella/Research/Sample data/test data/landmark/whole brain dist to tr%1.dat").arg(i+1);
			ifstream distFile (distFileName.toStdString().c_str(), ios::binary);
			distFile.read((char*)(_fibToRegDist+i*nFibers), nFibers*sizeof(float));
			distFile.close();
		}
	}
	/*if (!_clusters)
	{
		_clusters = new int[nFibers];
		ifstream clusterFile ("D:/Stella/Research/Sample data/test data/landmark/whole brain clusters.txt");
		for (int i=0; i<nFibers; ++i)
		{
			clusterFile >>_clusters[i];
		}
		clusterFile.close();
	}*/

	if (!_fiberData)
	{
		_fiberData = new FiberData();
		_fiberData->loadFibers("D:/Stella/Research/Sample data/test data/landmark/whole brain tracts.txt", 114, 114, 60);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-80, 80, -80, 80, -80, 80);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float thres = 10;
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0,0,_widgetWidth/2, _widgetHeight);

	for (int i=0; i<nFibers; ++i)
	{
		for (int j=0; j<nRegions; ++j)
		{
			switch (j)
			{
			case 0:
				glColor4f(1,0,0,0.2);
				break;
			case 1:
				glColor4f(0,1,0,0.2);
				break;
			case 2:
				glColor4f(0,0,1,0.2);
				break;
			}
			if (_fibToRegDist[j*nFibers+i]<thres)
			{
				glPushMatrix();
				glTranslatef(_ordinaryMDS[i*2], _ordinaryMDS[i*2+1], 0);
				glutSolidSphere(1.0, 10, 10);
				glPopMatrix();
			}
		}
	}

	//glColor3f(1,1,1);
	glBegin(GL_POINTS);
	for (int i=0; i<nFibers; ++i)
	{
		//_fiberData->setFiberColor(i, 1);
		glVertex2f(_ordinaryMDS[i*2], _ordinaryMDS[i*2+1]);
	}
	glEnd();

	glViewport(_widgetWidth/2,0,_widgetWidth/2, _widgetHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-80, 80, -80, 80, -80, 80);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i=0; i<nFibers; ++i)
	{
		for (int j=0; j<nRegions; ++j)
		{
			switch (j)
			{
			case 0:
				glColor4f(1,0,0,0.2);
				break;
			case 1:
				glColor4f(0,1,0,0.2);
				break;
			case 2:
				glColor4f(0,0,1,0.2);
				break;
			}
			if (_fibToRegDist[j*nFibers+i]<thres)
			{
				glPushMatrix();
				glTranslatef(_constrainedMDS[i*2], _constrainedMDS[i*2+1], 0);
				glutSolidSphere(1.0, 10, 10);
				glPopMatrix();
			}
		}
	}

	//glColor3f(1,1,1);
	glBegin(GL_POINTS);
	for (int i=0; i<nFibers; ++i)
	{
		//_fiberData->setFiberColor(i, 1);
		glVertex2f(_constrainedMDS[i*2], _constrainedMDS[i*2+1]);
	}
	glEnd();
}