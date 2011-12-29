#include "TemplateFiberData.h"
#include "glwidget.h"
#include <gl/glut.h>
#include <gl/gle.h>
//#include "similarity/cuda_main.h"

#include "FiberData.h"

#include <Windows.h>
#include <time.h>
#include <gl/glut.h>
#include <QMessageBox>
#include <QProgressDialog>
//#include "similarity/cuda_main.h"
#include "TemplateFiberData.h"
//#include "similarity/cuda_main.h"
#include "Gamma/Gamma.h"
#include <math.h>

FiberData::FiberData(void)
{
	_fibers = vector<ArrFiber>(0);
	//_colorMode = COLOR_DIR;
	_colorMode = COLOR_CLUSTER;
	_fiberThickness = 1;
	_tensorData = NULL;
	_embedPoints = NULL;
	_currentCluster = -1111;
	_bClustersLoaded = false;
	_normMap = NULL;
	_normSizeX = _normSizeY = _normSizeZ = 0;
	_nFibers = 0;
	_bNormalized = false;			
	_nSelectedFibers = 0;
	_clusterColorer = new ColorTable();

	_selectionIndex = 1;
	_volSizeX = _volSizeY = _volSizeZ = 1;
	_fiberDrawList = 0;
	_graphThres = 0.1;
	_nClusters = 0;
}

FiberData::~FiberData(void)
{
	for (int i=0; i<_fibers.size(); ++i)
	{
		delete [] _fibers[i];
	}
	_fibers.clear();
	if (_normMap)
	{
		delete [] _normMap;
	}

	if (_embedPoints)
	{
		delete [] _embedPoints;
	}
	if (_tensorData)
	{
		delete _tensorData;
	}
}

void FiberData::fiberTracking(SeedData * seedData, TensorData * tensorData, const TrackingConfig & config)
{
	_fibers.clear();
	_fibers.resize(config.nFibers);
	int nFibersFound = 0;
	int nSeeds = 0;
	//seedData->refreshSeedBuf();
	while(true)
	{
		int seedBufIndex;
		FloatPoint seedPt = seedData->getRandomSeed(seedBufIndex);
		ListFiber fib = trackFiber(seedPt, tensorData, config.stepSize, config.faThres, config.angleThres*3.1415926/180);
		if (fib.size()>=2 && fib.size()>config.minLength/config.stepSize && fib.size()<config.maxLength/config.stepSize)
		{
			_fibers[nFibersFound] = new FloatPoint[fib.size()+1];
			_fibers[nFibersFound][0](0) = fib.size();
			_fibers[nFibersFound][0](1) = 0;
			_fibers[nFibersFound][0](2) = 0;
			int i=1;
			for (ListFiber::iterator it = fib.begin(); it!=fib.end(); ++it)
			{
				_fibers[nFibersFound][i] = *it;
				++i;
			}
			++nFibersFound;
		}
		++nSeeds;

		if (config.isFiberCount && nFibersFound>=config.nFibers)
		{
			break;
		} else if (!config.isFiberCount && nSeeds>=config.nFibers)
		{
			break;
		}
	}
	_fibers.resize(nFibersFound);
	_nFibers = nFibersFound;
	
	_tensorData = tensorData;
}

ListFiber FiberData::trackFiber(const FloatPoint &seedPt, TensorData *tensorData, double stepSize, double faThres, double angleThres)
{
	ListFiber fib (0);
	fib.push_back(seedPt);
	Tensor currTensor = tensorData->getTensorAt((double)seedPt(0), (double)seedPt(1), (double)seedPt(2));
	if (currTensor.fa<faThres)
	{
		return fib;
	}
	FloatPoint nextDir;
	nextDir(0) = currTensor.d0[0]/*/tensorData->pixSizeX()*/;
	nextDir(1) = currTensor.d0[1]/*/tensorData->pixSizeY()*/;
	nextDir(2) = currTensor.d0[2]/*/tensorData->pixSizeZ()*/;
	nextDir /= norm(nextDir, 2);
	trackSingleDir(fib, nextDir, tensorData, stepSize, faThres, angleThres);
	nextDir *= -1;
	fib.reverse();
	trackSingleDir(fib, nextDir, tensorData, stepSize, faThres, angleThres);
	return fib;
}

void FiberData::trackSingleDir(ListFiber &fib, FloatPoint &currDir, TensorData *tensorData, double stepSize, double faThres, double angleThres)
{
	if (fib.empty())
	{
		return;
	}

	const FloatPoint &currPos = fib.back();
	//const float * currPosMem = currPos.memptr();
	Tensor currTensor = tensorData->getTensorAt((int)currPos(0), (int)currPos(1), (int)currPos(2));
	if (currTensor.fa < faThres)
	{
		return;
	}
	FloatPoint nextDir;
	nextDir <<currTensor.d0[0]/*/tensorData->pixSizeX()*/ <<currTensor.d0[1]/*/tensorData->pixSizeY()*/ <<currTensor.d0[2]/*/tensorData->pixSizeZ()*/;
	//nextDir /= norm(nextDir, 2);
	float cosine = dot(currDir, nextDir);
	if (cosine<0)
	{
		nextDir *= -1;
		cosine *= -1;
	}
	if (cosine<=cos(angleThres))
	{
		return;
	}
	
	FloatPoint nextDirTrans;
	nextDirTrans <<nextDir[0]/tensorData->pixSizeX() <<nextDir[1]/tensorData->pixSizeY() <<nextDir[2]/tensorData->pixSizeZ();
	FloatPoint nextPos = currPos + stepSize/norm(nextDirTrans,2)*nextDirTrans;
	fib.push_back(nextPos);
	trackSingleDir(fib, nextDir, tensorData, stepSize, faThres, angleThres);
}

void FiberData::drawFibers()
{
	glEnable(GL_DEPTH_TEST);
	/*if (_fiberDrawList==0)
	{
		genFiberDispList();
	}*/

	glLineWidth(_fiberThickness);
	glPushMatrix();
	glTranslatef(-_sizeX*_volSizeX/2, -_sizeY*_volSizeY/2, -_sizeZ*_volSizeZ/2);
	/*for (int i=0; i<_nFibers; ++i)
	{
		if (_filterBuf[i])
		{
			glCallList(_fiberDrawList+i);
		}
	}*/
	for(int i=0; i<_nFibers; ++i)
	{
		glPushName(i);
		//glLoadName(i);
		glBegin(GL_LINE_STRIP);
		for (int j=1; j<=_fibers[i][0](0); ++j)
		{
			const uchar* col = getFiberColor(i,j);
			glColor4ubv(col);
			glVertex3fv(_fibers[i][j].memptr());
		}
		glEnd();
		glPopName();
	}

	glPopMatrix();
}

void FiberData::drawFibersAsTubes( float radius )
{
	if (_fiberDrawList==0)
	{
		genFiberDispListAsTube(0.3);
	}
	glPushMatrix();
	glTranslatef(-_sizeX*_volSizeX/2, -_sizeY*_volSizeY/2, -_sizeZ*_volSizeZ/2);
	for (int i=0; i<_nFibers; ++i)
	{
		glCallList(_fiberDrawList+i);
	}
	glPopMatrix();
}

void FiberData::drawClusterCenters()
{
	glEnable(GL_DEPTH_TEST);

	glLineWidth(_fiberThickness);
	glPushMatrix();
	glTranslatef(-_sizeX*_volSizeX/2, -_sizeY*_volSizeY/2, -_sizeZ*_volSizeZ/2);

	for (int i=0; i<_clusterCenters.size(); ++i)
	{
		glBegin(GL_LINE_STRIP);
		for (int j=1; j<=_clusterCenters[i][0](0); ++j)
		{
			float c = j*0.5/_clusterCenters[i][0](0)+0.5;
			glColor3f(c,c,c);
			glVertex3fv(_clusterCenters[i][j].memptr());
		}
		glEnd();
	}

	glPopMatrix();

}

void FiberData::drawMatchings()
{
	drawClusterCenters();

	if (_matchings.empty())
	{
		return;
	}

	glEnable(GL_DEPTH_TEST);

	glLineWidth(_fiberThickness);
	glPushMatrix();
	glTranslatef(-_sizeX*_volSizeX/2, -_sizeY*_volSizeY/2, -_sizeZ*_volSizeZ/2);

	for (int i=0; i<_nFibers; ++i)
	{
		int clusterIdx = _clusters[i];
		if (clusterIdx!=-1)
		{
			Link &matching = _matchings[i][clusterIdx];
			glBegin(GL_LINES);
			for (int j=1; j<=_fibers[i][0](0); ++j)
			{
				//setFiberColor(i,j);
				glVertex3fv(_fibers[i][j].memptr());
				float c = matching[j]*0.5/_clusterCenters[clusterIdx][0](0)+0.5;
				glColor3f(c,c,c);
				glVertex3fv(_clusterCenters[clusterIdx][matching[j]].memptr());
			}
			glEnd();
		}
	}

	glPopMatrix();
}

void FiberData::genFiberDispList()
{
	glDeleteLists(_fiberDrawList, _nFibers);
	_fiberDrawList = glGenLists(_nFibers);
	//glListBase(_fiberDrawList);
	for(int i=0; i<_nFibers; ++i)
	{
		glNewList(_fiberDrawList+i, GL_COMPILE);
		glInitNames();
		glPushName(0);
		glLoadName(i);
		glBegin(GL_LINE_STRIP);
		for (int j=1; j<=_fibers[i][0](0); ++j)
		{
			//setFiberColor(i,j);
			glVertex3fv(_fibers[i][j].memptr());
		}
		glEnd();
		glEndList();
	}
}

void FiberData::genFiberDispListAsTube(float radius)
{
	glDeleteLists(_fiberDrawList, _nFibers);
	_fiberDrawList = glGenLists(_nFibers);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	gleDouble ptBuf [200][3];
	float colBuf [200][3];

	for (int i=0; i<_nFibers; ++i)
	{
		glNewList(_fiberDrawList+i, GL_COMPILE);
		int nPoints = _fibers[i][0](0);
		for (int j=0; j<nPoints; ++j)
		{
			ptBuf[j][0] = _fibers[i][j+1](0);
			ptBuf[j][1] = _fibers[i][j+1](1);
			ptBuf[j][2] = _fibers[i][j+1](2);

			const uchar* col = getFiberColor(i,j+1);
			colBuf[j][0] = col[0]/255.0f;
			colBuf[j][1] = col[1]/255.0f;
			colBuf[j][2] = col[2]/255.0f;
		}

		glePolyCylinder (nPoints, ptBuf, colBuf, radius);
		glEndList();
	}
}

const uchar* FiberData::getFiberColor(int fibIndex, int ptIndex, float alpha)
{
	uchar *col;
	FloatPoint dirVec;
	int clusterIdx;
	FloatPoint t1, t2;
	float lambda;

	switch (_colorMode)
	{
	case COLOR_DIR:
		alpha = _selectedBuf[fibIndex]?1.0:0.6;
		dirVec = ptIndex==1?_fibers[fibIndex][ptIndex+1]-_fibers[fibIndex][ptIndex]:_fibers[fibIndex][ptIndex]-_fibers[fibIndex][ptIndex-1];
		dirVec /= norm(dirVec,2);
		//glColor4f(fabs(dirVec[0])*alpha, fabs(dirVec[1])*alpha, fabs(dirVec[2])*alpha, alpha);
		_fibColBuf[0] = fabs(dirVec[0])*alpha*255;
		_fibColBuf[1] = fabs(dirVec[1])*alpha*255;
		_fibColBuf[2] = fabs(dirVec[2])*alpha*255;
		_fibColBuf[3] = alpha*255;
		break;
	case COLOR_CLUSTER:
		clusterIdx = _clusters[fibIndex];
		alpha = _selectedBuf[fibIndex]?1.0:0.3;
		if (clusterIdx<0)
		{
			_fibColBuf[0] = alpha*255;
			_fibColBuf[1] = alpha*255;
			_fibColBuf[2] = alpha*255;
			_fibColBuf[3] = alpha*255;
		} else
		{
			col = _clusterColorer->getColor(clusterIdx);
			_fibColBuf[0] = col[0]*alpha;
			_fibColBuf[1] = col[1]*alpha;
			_fibColBuf[2] = col[2]*alpha;
			_fibColBuf[3] = alpha*255;
		}
		break;
	case COLOR_ATLAS_CLUSTER:
		col = TemplateFiberData::getTplColor(_clusters[fibIndex]);
		_fibColBuf[0] = col[0];
		_fibColBuf[1] = col[1];
		_fibColBuf[2] = col[2];
		_fibColBuf[3] = alpha*255;

		break;
		glColor4f(0.8,0.5,0.5, alpha);
		_fibColBuf[0] = 255;
		_fibColBuf[1] = 180;
		_fibColBuf[2] = 180;
		_fibColBuf[3] = alpha*255;
		break;
	case COLOR_TC:
		if (ptIndex>1)
		{
			break;
		}
		t1 = _fibers[fibIndex][1];
		t2 = _fibers[fibIndex][(int)(_fibers[fibIndex][0](0))];
		t1(2) /= _sizeZ*1.0/_sizeX;
		t2(2) /= _sizeZ*1.0/_sizeX;
		lambda = 1.0/sqrt(dot(t1, t2));
		glColor4f(lambda*sqrt(t1(0)*t2(0)), lambda*sqrt(t1(1)*t2(1)), lambda*sqrt(t1(2)*t2(2)), alpha);
		_fibColBuf[0] = lambda*sqrt(t1(0)*t2(0))*255;
		_fibColBuf[1] = lambda*sqrt(t1(1)*t2(1))*255;
		_fibColBuf[2] = lambda*sqrt(t1(2)*t2(2))*255;
		_fibColBuf[3] = alpha*255;
		break;
	}
	return _fibColBuf;
}

unsigned FiberData::loadFibers(const QString &fileName, int sizeX, int sizeY, int sizeZ)
{
	_tensorData = NULL;
	_sizeX = sizeX;
	_sizeY = sizeY;
	_sizeZ = sizeZ;
	ifstream inFile;
	inFile.open(fileName.toStdString().c_str());
	if (!inFile)
	{
		QMessageBox::warning(NULL, "Open file error", QString("File %1 does not exist!").arg(fileName));
		return 0;
	}
	//count the number of fibers
	int numOfFibers = 0;
	unsigned totalPtCount = 0;
	string line;
	inFile.seekg(0, ios::end);
	int fileLength = inFile.tellg();
	inFile.seekg(0, ios::beg);
	inFile.clear();

	QProgressDialog progress ("Loading file...", "", 0, 1000);
	progress.setCancelButton(NULL);

	while (getline(inFile, line))
	{
		progress.setValue((inFile.tellg())*progress.maximum()*1.0/fileLength);
		++numOfFibers;
	}
	progress.setLabelText("Reading fiber data...");
	progress.setValue(0);
	progress.show();

	_fibers.resize(numOfFibers);
	//_nFibers = numOfFibers;
	setNumOfFibers(numOfFibers);

	int i = 0;

	if (!inFile)
		inFile.clear();

	inFile.seekg(0, ios::beg);

	while (getline(inFile, line))
	{
		progress.setValue(i*progress.maximum()*1.0/(numOfFibers-1));

		stringstream str(line);
		int numOfPoints = 0;

		float x, y, z;
		while (str >>x >>y >>z)
		{
			++numOfPoints;
		}

		str.clear();
		str.seekg(0, ios::beg);
		int pos = str.tellg();

		_fibers[i] = new FloatPoint[numOfPoints+1];
		_fibers[i][0].fill(0);
		_fibers[i][0](0) = numOfPoints;

		for (int j = 1; j<=numOfPoints; ++j)
		{
			str >>x >>y >>z;
			x = x<0.0?0.0:x;
			y = y<0.0?0.0:y;
			z = z<0.0?0.0:z;
			FloatPoint pt;
			pt <<x <<y <<z;
			_fibers[i][j] = pt;
		}
		++i;
		totalPtCount += numOfPoints;
	}
	inFile.close();
	_nPoints = totalPtCount;

	return totalPtCount;
}

void FiberData::loadClusters(const QString &fileName)
{
	ifstream inFile(fileName.toStdString().c_str());

	_clusters.resize(_nFibers);
	for (int i=0; i<_nFibers; ++i)
	{
		try
		{
			inFile>>_clusters[i];
		}
		catch (...)
		{
			QMessageBox::warning(NULL, "File Error", "Wrong cluster file!");
			return;
		}
	}
	_currentCluster = -1111;
	_bClustersLoaded = true;

	//updateClusterCenters();
	_nClusters = *max_element(_clusters.begin(), _clusters.end())+1;
	initClusterMedoids();
	vector<vector<Link> > matchings;
	getMatchings(matchings);
	vector<bool>bClusterChanged (_nClusters);
	bClusterChanged.assign(_nClusters, true);
	updateClusterCenters(matchings, bClusterChanged);

	initClusterColor();
}

void FiberData::initClusterColor()
{
	int nClusters = _clusterCenters.size();
	FloatPoint t1, t2;
	_clusterColorer->setNumOfColors(nClusters);
	ColorTable* tmpColorTable = new ColorTable();
	srand(time(NULL));
	for (int i=0; i<nClusters; ++i)
	{
		const uchar * col = tmpColorTable->getColor(i*1.0/(nClusters-1));
		_clusterColorer->setColor(i, col[0], col[1], col[2]);
	}
}

void FiberData::loadEmbeddings(const QString &fileName)
{
	ifstream inFile (fileName.toStdString().c_str(), ios::binary);
	if (!inFile)
	{
		return;
	}

	inFile.seekg(0, ios::end);
	int numOfFibers = inFile.tellg()/5/sizeof(float);
	inFile.seekg(0, ios::beg);
	inFile.clear();

	if (_fibers.empty())
	{
		_nFibers = numOfFibers;
	} else if (numOfFibers!=_nFibers)
	{
		QMessageBox::warning(NULL, "File error", "Embedding file doesn't match!");
		return;
	}

	if (_embedPoints)
	{
		delete [] _embedPoints;
	}

	_embedPoints = new EmbedPoint[_nFibers];

	float angle, z;
	float tmpPt [2];
	EmbedPoint embPt;
	for (int i=0; i<_nFibers; ++i)
	{
		inFile.read((char*)tmpPt, 2*sizeof(float));
		embPt.posLLE <<tmpPt[0] <<tmpPt[1];
		inFile.read((char*)tmpPt, 2*sizeof(float));
		embPt.posProj <<tmpPt[0] <<tmpPt[1];
		float d=0;
		inFile.read((char*)&d, sizeof(float));
		embPt.minDist = d;
		_embedPoints[i] = embPt;
	}

	inFile.close();
}


void FiberData::drawEmbeddings(int type, float minDistThres)
{
	if (!_embedPoints)
	{
		return;
	}

	glEnable(GL_BLEND);
	for (int i=0; i<_fibers.size(); ++i)
	{
		EmbedPoint emb = _embedPoints[i];
		bool bCurrentType = false;
		if (type == 1 && emb.minDist>=minDistThres)
		{
			bCurrentType = true;
		} else if (type==0 && emb.minDist<minDistThres)
		{
			bCurrentType = true;
		}

		if (!bCurrentType)
		{
			continue;
		}

		CylindPoint pt = type?_embedPoints[i].posProj:_embedPoints[i].posLLE;

		float x = pt(0);
		float y = pt(1);

		if (_colorMode == COLOR_DIR /*|| _colorMode == COLOR_CLUSTER*/)
		{
			if (_selectedBuf[i])
			{
				glColor4f(1,0,1, 0.1);
			} else 
			{
				glColor4ub(0,174,239, 25);
			}
		} else
		{
			//setFiberColor(i, 1, 0.1);
		}

		glPushMatrix();
		glTranslatef(x,y,0);
		glutSolidSphere(0.08, 20, 20);
		glPopMatrix();
	}
	//glEnd();
}

int FiberData::fiber(int index, float *ptBuf) const
{
	int nPoint = _fibers[index][0](0);
	for (int j=0; j<nPoint; ++j)
	{
		memcpy(ptBuf+3*j, _fibers[index][j+1].memptr(), 3*sizeof(float));
	}
	return nPoint;
}

void FiberData::embedGPU(const QString &normFileName, int nNeighbors)
{
	loadNormMap(normFileName);

	if (_embedPoints)
	{
		delete [] _embedPoints;
	}

	_embedPoints = TemplateFiberData::getEmbeddingsGPU(this, nNeighbors);
}

void FiberData::saveEmbeddings(const QString &fileName)
{
	ofstream embFile (fileName.toStdString().c_str(), ios::binary);
	for (int i=0; i<_fibers.size(); ++i)
	{
		embFile.write((char*)(_embedPoints[i].posLLE.memptr()), 2*sizeof(float));
		embFile.write((char*)(_embedPoints[i].posProj.memptr()), 2*sizeof(float));
		embFile.write((char*)(&_embedPoints[i].minDist), sizeof(float));
	}
	embFile.close();	
}

int FiberData::selectFibers(int left, int top, int right, int bottom, GLdouble *embModelMatrix, GLdouble *embProjMatrix, int *embViewport, int type, float minDistThres)
{
	if (_fibers.empty() || !_embedPoints)
	{
		return 0;
	}

	GLdouble minX, minY, maxX, maxY, z;
	gluUnProject(left, bottom, 0, embModelMatrix, embProjMatrix, embViewport, &minX, &minY, &z);
	gluUnProject(right, top, 0, embModelMatrix, embProjMatrix, embViewport, &maxX, &maxY, &z);

	if (minX>maxX)
	{
		GLdouble tmp = minX;
		minX = maxX;
		maxX = tmp;
	}

	if (minY>maxY)
	{
		GLdouble tmp = minY;
		minY = maxY;
		maxY = tmp;
	}

	int selectedCount = 0;
	/*if (!_clusters)
	{
		_clusters = new int[_nFibers];
		memset(_clusters, 0, _nFibers*sizeof(int));
	}*/
	
	for (int i=0; i<_fibers.size(); ++i)
	{
		EmbedPoint emb = _embedPoints[i];
		bool bCurrentType = false;
		if ( (type == 1 && emb.minDist>=minDistThres) ||(type==0 && emb.minDist<minDistThres))
		{
			bCurrentType = true;
		} 

		if (!bCurrentType)
		{
			_selectedBuf[i] = false;
			continue;
		}

		CylindPoint pt = type?emb.posProj:emb.posLLE;
		float x = pt(0);
		float y = pt(1);
		if (x>minX && x<maxX && y>minY && y<maxY)
		{
			_selectedBuf[i] = true;
			//_clusters[i] = _selectionIndex;
			++selectedCount;
		} else
		{
			_selectedBuf[i] = false;
		}
	}
	
	_nSelectedFibers = selectedCount;
	_selectionIndex = (_selectionIndex+1)%3;
	return selectedCount;
}

void FiberData::saveClusters(const QString &fileName)
{	
	ofstream outFile (fileName.toStdString().c_str());
	for (int i=0; i<_fibers.size(); ++i)
	{
		outFile <<_clusters[i] <<"\n";
	}
	outFile.close();
}

void FiberData::assignLabel(int label)
{
	for (int i=0; i<_fibers.size(); ++i)
	{
		if (_selectedBuf[i])
		{
			_clusters[i] = label;
		}
	}
}

void FiberData::assignAtlasLabel(int label)
{
	if (_fibers.empty())
	{
		return;
	}

	for (int i=0; i<_fibers.size(); ++i)
	{
		if (_selectedBuf[i])
		{
			//_clusters[i] = label;
			int left = _embedPoints[i].posProj(0)<=0?0:1;
			CylindPoint clusterEmb = TemplateFiberData::templateData->clusterEmb(label, left);
			_embedPoints[i].posLLE = clusterEmb;
			_embedPoints[i].minDist = 0;
		}
	}
}

int FiberData::selectFiber(int fibIdx)
{
	if (_fibers.empty() || fibIdx<0 || fibIdx>=_fibers.size())
	{
		return 0;
	}

	_selectedBuf[fibIdx] = true;
	return 1;
}

int FiberData::selectFibers(int clusterIdx)
{
	if (_fibers.empty())
	{
		return 0;
	}

	_selectedBuf.assign(_nFibers, true);
	int selectedFibCount = 0;
	for (int i=0; i<_fibers.size(); ++i)
	{
		if (_clusters[i]==clusterIdx)
		{
			_selectedBuf[i] = true;
			++selectedFibCount;
		}
	}

	return selectedFibCount;
}

void FiberData::loadNormMap(const QString &fileName)
{
	if (_bNormalized)
	{
		return;
	}
	ifstream normFile (fileName.toStdString().c_str(), ios::binary);
	float subjectMat [16];
	float atlasMat[16];

	normFile.read((char*)(subjectMat), 16*sizeof(float));
	normFile.read((char*)(&_sizeX), sizeof(int));
	normFile.read((char*)(&_sizeY), sizeof(int));
	normFile.read((char*)(&_sizeZ), sizeof(int));
	normFile.read((char*)(atlasMat), 16*sizeof(float));
	normFile.read((char*)(&_normSizeX), sizeof(int));
	normFile.read((char*)(&_normSizeY), sizeof(int));
	normFile.read((char*)(&_normSizeZ), sizeof(int));

	int nVoxels = _sizeX*_sizeY*_sizeZ*3;
	int nNormVoxels = _normSizeX*_normSizeY*_normSizeZ*3;
	if (_normMap)
	{
		delete []_normMap;
	}
	_normMap = new float[nVoxels];
	float* flow_field = new float[nNormVoxels];
	normFile.read((char*)(flow_field), nNormVoxels*sizeof(float));
	normFile.close();

	float *normPtBuf = new float[3*this->nPoints()];
	int nTargetFibs = this->nFibers();
	unsigned bufPos = 0;

	for (int i=0; i<nTargetFibs; ++i)
	{
		//int fibLength = normFiber(_fibers[i], _normMap, normPtBuf+bufPos);
		int fibLength = this->fiber(i, normPtBuf+bufPos);
		bufPos += fibLength*3;
	}
/*	for (int i=0; i<nTargetFibs; ++i)
	{
		for (int j=1; j<=this[i](0); ++j)
		{
			FloatPoint pt=this[i][j];
			normPtBuf[bufPos]=pt(0);
			normPtBuf[bufPos+1]=pt(1);
			normPtBuf[bufPos+2]=pt(2);
			bufPos += 3;
		}	
	}*/	
	
	//transformBindedPoints(normPtBuf, this->nPoints(), subjectMat, atlasMat,  _normSizeX,  _normSizeY,  _normSizeZ,  flow_field);
	bufPos = 0;
	for (int i=0; i<nTargetFibs; ++i)
	{
		for (int j=1; j<=_fibers[i][0](0); ++j)
		{
			_fibers[i][j](0)=/*_normSizeX-*/normPtBuf[bufPos];
			_fibers[i][j](1)=normPtBuf[bufPos+1];
			_fibers[i][j](2)=normPtBuf[bufPos+2];
			bufPos += 3;
		}	
	}
	_sizeX = _normSizeX;
	_sizeY = _normSizeY;
	_sizeZ = _normSizeZ;
	_bNormalized = true;
	delete [] normPtBuf;
	delete [] flow_field;
}

void FiberData::addToTemplate()
{
	if ( _fibers.empty())
	{
		return;
	}

	int newClusterIdx = 0;
	/*if (!_clusters)
	{
		_clusters = new int[_nFibers];
		memset(_clusters, 0, _nFibers*sizeof(int));
		newClusterIdx = 1;
	} else
	{*/
		for (int i=0; i<_nFibers; ++i)
		{
			newClusterIdx = _clusters[i]>newClusterIdx?_clusters[i]:newClusterIdx;
		}
		++newClusterIdx;
	//}

	for (int i=0; i<_nFibers; ++i)
	{
		if (_selectedBuf[i])
		{
			_clusters[i] = newClusterIdx;
		}
	}

	CylindPoint projPt = clusterCenter(this, newClusterIdx);
}

void FiberData::saveAtlasWeights(const QString &normFileName, const QString &saveFileName)
{
	if (_fibers.empty())
	{
		return;
	}

	loadNormMap(normFileName);

	float * distMat = TemplateFiberData::getAtlasDistMatrixGPU(this);
	int nAtlasFibs = TemplateFiberData::templateData->nFibers();

	ofstream outFile(saveFileName.toStdString().c_str());
	float minDist;
	for (int i=0; i<_nFibers; ++i)
	{
		vector<float> w = TemplateFiberData::getAtlasWeights(distMat+i*nAtlasFibs, 80, minDist);
		for (int j=0; j<w.size(); ++j)
		{
			outFile <<w[j] <<"\t";
		}
		outFile <<endl;
	}
	outFile.close();
}

int FiberData::selectFibers(vector<int> &idxBuf, SelectionMode mode)
{
	if (_fibers.empty())
	{
		return 0;
	}

	vector<bool> tmpSelectionBuf (_nFibers);
	for (int i=0; i<idxBuf.size(); ++i)
	{
		tmpSelectionBuf[idxBuf[i]] = true;
	}

	_nSelectedFibers = 0;
	for (int i=0; i<_nFibers; ++i)
	{
		switch (mode)
		{
		case SEL_NEW:
			_selectedBuf[i] = tmpSelectionBuf[i];
			break;
		case SEL_INTERSECT:
			_selectedBuf[i] = tmpSelectionBuf[i]&_selectedBuf[i];
			break;
		case SEL_UNION:
			_selectedBuf[i] = tmpSelectionBuf[i] | _selectedBuf[i];
			break;
		case SEL_DIFF:
			_selectedBuf[i] = tmpSelectionBuf[i] ^ _selectedBuf[i];
			break;
		case SEL_REMOVE: 
			_selectedBuf[i] = (~tmpSelectionBuf[i]) & _selectedBuf[i];
			break;
		}
		if (_selectedBuf[i])
		{
			++_nSelectedFibers;
		}
	}
	return _nSelectedFibers;
}

float *getDistMatrixGPU(FiberData *fibData1, FiberData *fibData2)
{
	//float *normPtBuf = new float[3*fibData1->nPoints()];
	//int nTargetFibs = fibData1->nFibers();
	//vector<unsigned> fibEnds (nTargetFibs);
	//unsigned bufPos = 0;

	//for (int i=0; i<nTargetFibs; ++i)
	//{
	//	//int fibLength = normFiber(_fibers[i], _normMap, normPtBuf+bufPos);
	//	int fibLength = fibData1->fiber(i, normPtBuf+bufPos);
	//	fibEnds[i] = i==0?fibLength:fibEnds[i-1]+fibLength;
	//	bufPos += fibLength*3;
	//}

	//int nNormPoints = fibEnds[nTargetFibs-1];
	//bind_fiber(normPtBuf,nNormPoints);
	//delete [] normPtBuf;

	//float tmpAtlasFib [5000];

	//int nAtlasFibers = fibData2->nFibers();
	//float *minArray = new float[nNormPoints];
	//float *distMatrix = new float[nTargetFibs*nAtlasFibers];
	//memset(distMatrix, 0, nTargetFibs*nAtlasFibers*sizeof(float)); 

	//QProgressDialog progress ("Computing distance matrix...", "", 0, nAtlasFibers-1);
	//progress.setCancelButton(NULL);
	//progress.show();

	//for (int i=0; i<nAtlasFibers; ++i)
	//{
	//	progress.setValue(i);
	//	int atlasFibLength = fibData2->fiber(i, tmpAtlasFib);
	//	similarity_with_binded_fiber(tmpAtlasFib,atlasFibLength,minArray);
	//	for (int j=0; j<nTargetFibs; ++j)
	//	{
	//		unsigned fibBegin = j==0?0:fibEnds[j-1];
	//		float d = *max_element(minArray+fibBegin, minArray+fibEnds[j]);
	//		d=sqrt(d);
	//		/*float d = 0;
	//		for (int k=fibBegin; k<fibEnds[j]; ++k)
	//		{
	//			d += sqrt(minArray[k]);
	//		}
	//		d /= (fibEnds[j]-fibBegin);*/
	//		distMatrix[j*nAtlasFibers+i] = d;
	//	}
	//}
	//delete [] minArray;
	//return distMatrix;
	return NULL;
}

void FiberData::saveDistMatrix(const QString &fileName)
{
	//_nFibers = 10000;
	float *distMatrix = getDistMatrixGPU(this, this);

	ofstream saveFile (fileName.toStdString().c_str(), ios::binary);
	saveFile.write((char*)distMatrix, sizeof(float)*_nFibers*_nFibers);
	saveFile.close();
	delete distMatrix;
}

void FiberData::saveDistToRegion(const QString &fileName, const QString &regionFileName)
{
	FiberData *regData = new FiberData();
	regData->loadFibers(regionFileName, 114,114,60);
	float *distMatrix = getDistMatrixGPU(this, regData);

	ofstream saveFile (fileName.toStdString().c_str(), ios::binary);
	for (int i=0; i<_nFibers; ++i)
	{
		float d = *min_element(distMatrix+i*regData->nFibers(), distMatrix+(i+1)*regData->nFibers());
		saveFile.write((char*)&d, sizeof(float));
	}
	delete distMatrix;
	saveFile.close();
}

void FiberData::resampleEqualSample(int n)
{
	for (int i=0;i<_nFibers; ++i)
	{
		int fibLength = _fibers[i][0](0);
		ArrFiber resampledFib = new FloatPoint [n+1];
		resampledFib[0].fill(0);
		resampledFib[0](0)= n;
		for (int j=0; j<n; ++j)
		{
			float t = j*1.0f/(n-1);
			float m = (fibLength-1)*t+1;
			int j1 = floor(m);
			int j2 = ceil(m);
			resampledFib[j+1] = (1-(m-j1))*_fibers[i][j1]+(m-j1)*_fibers[i][j2];
		}
		ArrFiber oldFib = _fibers[i];
		_fibers[i] = resampledFib;
	}
}

void FiberData::resampleEqualInc(float incLength)
{
	for (int i=0;i<_nFibers; ++i)
	{
		int nOriSample = _fibers[i][0](0);
		float fibLength = 0;
		vector<float> stepLengths (nOriSample);
		for (int j=2; j<=nOriSample; ++j)
		{
			float d = norm(_fibers[i][j]-_fibers[i][j-1], 2);
			fibLength += d;
			stepLengths[j-1] = d;
		}

		int nNewSample = ceil(fibLength/incLength);
		ArrFiber tmpFib = new FloatPoint [nNewSample+1];
		tmpFib[0].fill(0);
		tmpFib[0](0) = nNewSample;
		tmpFib[1] = _fibers[i][1];
		FloatPoint lastPt = _fibers[i][1];
		FloatPoint currPt = _fibers[i][2];
		int currIdx = 2;
		for (int j=2; j<=nNewSample; ++j)
		{
			float d = norm(currPt-lastPt, 2);
			while (d<incLength && currIdx<=nOriSample)
			{
				currPt = _fibers[i][currIdx];
				d += stepLengths[currIdx-1];
				++currIdx;
			}

			if (d>=incLength)
			{
				float t = incLength/d;
				tmpFib[j] = (1-t)*lastPt+t*currPt;
				lastPt = tmpFib[j];
			} else
			{
				tmpFib[j] = currPt;
			}
		}
	}
}

void FiberData::saveFibers(const QString &fileName)
{
	ofstream fibFile(fileName.toStdString().c_str());
	for (int i=0; i<_nFibers; ++i)
	{
		for(int j=1; j<=_fibers[i][0](0); ++j)
		{
			fibFile <<_fibers[i][j](0) <<"\t"<<_fibers[i][j](1) <<"\t" <<_fibers[i][j](2) <<"\t";
		}
		fibFile <<"\n";
	}
	fibFile.close();
}

void FiberData::saveKMeansClusterCenters(const QString &fileName)
{
	ofstream fibFile(fileName.toStdString().c_str());
	for (int i=0; i<_clusterCenters.size(); ++i)
	{
		for(int j=1; j<=_fibers[i][0](0); ++j)
		{
			fibFile <<_clusterCenters[i][j](0) <<"\t"<<_clusterCenters[i][j](1) <<"\t" <<_clusterCenters[i][j](2) <<"\t";
		}
		fibFile <<"\n";
	}
	fibFile.close();
}

void FiberData::clusterKMeans(int k)
{
	_clusters.assign(_nFibers, 0);

	/*make fibers of the same dimension*/
	int nDim = 15;
	this->resampleEqualSample(nDim);

	/*initiate cluster centers*/
	_clusterCenters.resize(k);
	int * clusterCount = new int[k];

	int nClusterSpecified = 0;
	srand(time(NULL));
	for (int i=0; i<_nFibers; ++i)
	{
		float p = (k-nClusterSpecified)*1.0f / (_nFibers-i);
		if ((rand()%1000)/1000.0f < p)
		{
			_clusterCenters[nClusterSpecified] = new FloatPoint[nDim+1];
			for (int j=0; j<=nDim; ++j)
			{
				_clusterCenters[nClusterSpecified][j] = _fibers[i][j];
			}
			++nClusterSpecified;
		}
	}

	bool bChanged = true;
	int nIt = 0;
	while(bChanged)
	{
		bChanged = false;
		memset(clusterCount, 0, k*sizeof(int));

		DWORD t1 = GetTickCount();
		//assign points to nearest clusters
		for (int i=0; i<_nFibers; ++i)
		{
			float minDist = 1.0e5;
			int clusterIdx = 0;
			for (int j=0; j<k; ++j)
			{
				float dist = fibDistHausdorff(_fibers[i], _clusterCenters[j]);
				if (dist<minDist)
				{
					clusterIdx = j;
					minDist = dist;
				}
			}
			if (_clusters[i] != clusterIdx)
			{
				bChanged = true;
			}
			_clusters[i] = clusterIdx;
			clusterCount[clusterIdx]++;
		}

		//recompute cluster centers
		for (int i=0; i<k; ++i)
		{
			for (int j=1; j<=nDim; ++j)
			{
				_clusterCenters[i][j].fill(0);
			}
		}

		for (int i=0; i<_nFibers; ++i)
		{
			int clusterIdx = _clusters[i];
			for (int j=1; j<=nDim; ++j)
			{
				_clusterCenters[clusterIdx][j] += _fibers[i][j]/clusterCount[clusterIdx];
			}
		}
	
		DWORD perItTime = GetTickCount()-t1;
		if (nIt==0)
		{
			QMessageBox::information(NULL, "Per-iteration time", QString::number(perItTime));
		}
		nIt ++;
	}
	
}

void FiberData::clusterKMeansWithPrune(int k)
{
	_clusters.assign(_nFibers, 0);

	/*make fibers of the same dimension*/
	int nDim = 15;
	this->resampleEqualSample(nDim);

	/*initiate cluster centers*/
	_clusterCenters.resize(k);
	int * clusterCount = new int[k];

	int nClusterSpecified = 0;
	srand(time(NULL));
	for (int i=0; i<_nFibers; ++i)
	{
		float p = (k-nClusterSpecified)*1.0f / (_nFibers-i);
		if ((rand()%1000)/1000.0f < p)
		{
			_clusterCenters[nClusterSpecified] = new FloatPoint[nDim+1];
			for (int j=0; j<=nDim; ++j)
			{
				_clusterCenters[nClusterSpecified][j] = _fibers[i][j];
			}
			++nClusterSpecified;
		}
	}

	vector<float> MBuf (_nFibers);
	vector<float> mBuf (k);
	float mMax;
	vector<float> distC1Buf (_nFibers);
	vector<float> distC2Buf (_nFibers);
	vector<int> secNearClusters (_nFibers);

	bool bChanged = true;
	vector<bool> bClusterChanged (k);
	int nIt = 0;

	vector<vector<Link> > matchings;
	while (bChanged)
	{
		bChanged = false;
		bClusterChanged.assign(k, false);
		memset(clusterCount, 0, k*sizeof(int));

		DWORD t1 = GetTickCount();
		//assign points to nearest clusters
		for (int i=0; i<_nFibers; ++i)
		{
			//test pruning conditions
			bool bPruned = false;
			if (nIt > 0)
			{
				/*if (distC1Buf[i] <
					fibDistHausdorff(_clusterCenters[_clusters[i]], _clusterCenters[secNearClusters[i]])/2)
				{
					bPruned = true;
				} else*/ if (MBuf[i]>0)
				{
					bPruned = true;
				}
			}

			if (!bPruned)
			{
				//float minDist = 1.0e5;
				distC1Buf[i] = 1.0e5;
				distC2Buf[i] = 1.0e5;
				int oldClusterIdx = _clusters[i];
				for (int j=0; j<k; ++j)
				{
					float dist = fibDistHausdorff(_fibers[i], _clusterCenters[j]);
					if (dist<distC1Buf[i])
					{
						secNearClusters[i] = _clusters[i];
						distC2Buf[i] = distC1Buf[i];

						_clusters[i] = j;
						distC1Buf[i] = dist;
					} else if (dist<distC2Buf[i])
					{
						secNearClusters[i] = j;
						distC2Buf[i] = dist;
					}
				}
				if (oldClusterIdx != _clusters[i])
				{
					bChanged = true;
					bClusterChanged[_clusters[i]] = true;
					bClusterChanged[oldClusterIdx] = true;
				}
			}

			clusterCount[_clusters[i]]++;
		}

		//ArrFiber tmpCenter = new FloatPoint [nDim];
		//recompute cluster centers
		mMax = 0;
		for (int i=0; i<k; ++i)
		{
			if (!bClusterChanged[i])
			{
				mBuf[i] = 0;
			} else
			{
				ArrFiber oldCenter = _clusterCenters[i];
				_clusterCenters[i] = new FloatPoint [nDim+1];
				for (int j=0; j<=nDim; ++j)
				{
					_clusterCenters[i][j].fill(0);
				}
				_clusterCenters[i][0](0) = nDim;
				for (int j=0; j<_nFibers; ++j)
				{
					if (_clusters[j]==i)
					{
						for (int k=1; k<=nDim; ++k)
						{
							_clusterCenters[i][k] += _fibers[j][k]/clusterCount[i];
						}
					}
				}
				mBuf[i] = fibDistHausdorff(_clusterCenters[i], oldCenter);
				//delete oldCenter;
			}
			mMax = mBuf[i]>mMax?mBuf[i]:mMax;
		}

		for (int i=0; i<_nFibers; ++i)
		{
			if (MBuf[i]<0)
			{
				MBuf[i] = distC2Buf[i]-distC1Buf[i];				
			} else
			{
				MBuf[i] -= (mBuf[_clusters[i]]+mMax);
			}
		}
		DWORD perItTime = GetTickCount()-t1;
		if (nIt==0)
		{
			QMessageBox::information(NULL, "Per-iteration time", QString::number(perItTime));
		}
		++nIt;
	}
}

float fibDistHausdorff(const ArrFiber f1, const ArrFiber f2)
{
	int ptCount1 = f1[0](0), ptCount2 = f2[0](0);
	fmat distBuf(ptCount1, ptCount2);
	distBuf.fill(1000);
	for (int i=1; i<=ptCount1; ++i)
	{
		for (int j=1; j<=ptCount2; ++j)
		{
			float d = norm(f1[i]-f2[j], 2);
			distBuf(i-1, j-1) = d;
		}
	}
	float d1=0, d2=0;
	for (int i=0; i<ptCount1; ++i)
	{
		float d = min(distBuf.row(i));
		d1 = d > d1? d: d1;
	}

	for (int i=0; i<ptCount2; ++i)
	{
		float d = min(distBuf.col(i));
		d2 = d > d2? d: d2;
	}
	float dMCP = max(d1,d2);	
	return sqrt(dMCP);
	//return 1-exp(-dMCP/900);
}

Hypergraph * FiberData::getGraphKMeans(DistMetric metric, vector<bool> &bClusterChanged)
{
	int nClusters = _clusterCenters.size();
	vector<vector<Link> > matchings;
	getMatchings(matchings);
	if (bClusterChanged.empty())
	{
		bClusterChanged.resize(nClusters);
		bClusterChanged.assign(nClusters, true);
	}
	updateClusterCenters(matchings, bClusterChanged);

	Hypergraph *g = new Hypergraph();
	g->setNumOfVertices(_nFibers);
	vector<float> distBuf (nClusters*_nFibers);

	for (int i=0; i<nClusters; ++i)
	{
		int clusterSize = count(_clusters.begin(), _clusters.end(), i);
		for (int j=0; j<_nFibers; ++j)
		{
			float dist;
			switch (metric)
			{
			case DIST_HAUSDORFF:
				dist = fibDistHausdorff(_fibers[j], _clusterCenters[i]);
				break;
			case DIST_LCSS:
				dist = fibDistLCSS(_fibers[j], _clusterCenters[i], 20, 10);
				break;
			case DIST_DTW:
				dist = fibDistDTW(_fibers[j], _clusterCenters[i], 10, matchings[j][i]);
				break;
			case DIST_MADAH:
				dist = fibDistMadah(_fibers[j], _clusterCenters[i], matchings[j][i], _clusterCovs[i]);
				break;
			}
			distBuf[j*nClusters+i] = dist;
		}
	}
	
	for (int i=0; i<nClusters; ++i)
	{
		HyperEdge e;
		int clusterSize = count(_clusters.begin(), _clusters.end(), i);
		if (clusterSize==0)
		{
			g->addEdge(e);
			continue;
		}
		for (int j=0; j<_nFibers; ++j)
		{
			float minDist = distBuf[j*nClusters+_clusters[j]];
			float dist = distBuf[j*nClusters+i];
			if (dist-minDist<=_graphThres*minDist)
			{
				e.push_back(j);
			} else if (i==_clusters[j])
			{
				int stop = 1;
			}
		}
		g->addEdge(e);
	}
 	return g;
}

void FiberData::loadKMeansClusterCenters(const QString &fileName)
{
	_clusterCenters.clear();
	ifstream inFile(fileName.toStdString().c_str());
	string line;
	while (getline(inFile, line))
	{
		stringstream str(line);
		int numOfPoints = 0;

		float x, y, z;
		while (str >>x >>y >>z)
		{
			++numOfPoints;
		}

		str.clear();
		str.seekg(0, ios::beg);
		int pos = str.tellg();

		ArrFiber fib = new FloatPoint[numOfPoints+1];
		fib[0].fill(0);
		fib[0](0) = numOfPoints;

		for (int j = 1; j<=numOfPoints; ++j)
		{
			str >>x >>y >>z;
			x = x<0.0?0.0:x;
			y = y<0.0?0.0:y;
			z = z<0.0?0.0:z;
			FloatPoint pt;
			pt <<x <<y <<z;
			fib[j] = pt;
		}
		_clusterCenters.push_back(fib);
	}
	inFile.close();
}

void FiberData::setSelectionBuf(const vector<bool>selectionBuf)
{
	copy(selectionBuf.begin(), selectionBuf.end(), _selectedBuf.begin());
}

void FiberData::setNumOfFibers(int n)
{
	_nFibers = n;
	_selectedBuf.resize(n);
	_selectedBuf.assign(n, true);
	_filterBuf.resize(n);
	_filterBuf.assign(n, true);

	if (_clusters.size()!=n)
	{
		_clusters.resize(n);
		_clusters.assign(n, 0);
	}

	_fuzzyClusters.resize(n);
	_fuzzyClusters.assign(n, 0);
}

void FiberData::setClusters(const vector<int> &clusters, vector<bool> &bClusterChanged)
{
	int nCluster1 = *max_element(clusters.begin(), clusters.end())+1;
	int nCluster2 = *max_element(_clusters.begin(), _clusters.end())+1;
	bClusterChanged.resize(max(nCluster1, nCluster2));
	bClusterChanged.assign(bClusterChanged.size(), false);
	for (int i=0; i<_nFibers; ++i)
	{
		if (_clusters[i]!=clusters[i])
		{
			bClusterChanged[_clusters[i]] = true;
		}
	}
	copy(clusters.begin(), clusters.begin()+_nFibers, _clusters.begin());
}

void FiberData::updateClusterCenters()
{
	int nClusters = *max_element(_clusters.begin(), _clusters.end())+1;
	_clusterCenters.resize(nClusters);
	int fibSize = 15;
	resampleEqualSample(fibSize);

	for (int i=0; i<nClusters; ++i)
	{
		//delete _clusterCenters[i];
		_clusterCenters[i] = new FloatPoint[fibSize+1];
		for (int j=0; j<=fibSize; ++j)
		{
			_clusterCenters[i][j].fill(0);
			if (j==0)
			{
				_clusterCenters[i][j](0) = fibSize;
			}
		}
		float clusterSize = (float) count(_clusters.begin(), _clusters.end(), i);
		for (int j=0; j<_nFibers; ++j)
		{
			if (_clusters[j] == i)
			{
				for (int k=1; k<=fibSize; ++k)
				{
					_clusterCenters[i][k] += _fibers[j][k]/clusterSize;
				}
			}
		}
	}
}

void FiberData::loadFibersFromTrk(const QString &fileName)
{
	ifstream inFile (fileName.toStdString().c_str(), ios::binary);
	//ifstream inFile ("D:\\Study\\Hypergraph\\hypergraph test data\\whole brain tracks5000.trk", ios::binary);
	if (!inFile)
	{
		QMessageBox::warning(NULL, "File error", "Cannot open file!");
		return;
	}

	char strID [6];
	short nScalars;
	short nProperties;
	int nFibers;

	inFile.read(strID, 6);
	if (strcmp(strID, "TRACK\0"))
	{
		QMessageBox::warning(NULL, "File error", "This is not a trk file!");
		return;
	}

	inFile.read((char*)&_sizeX, sizeof(short));
	inFile.read((char*)&_sizeY, sizeof(short));
	inFile.read((char*)&_sizeZ, sizeof(short));

	inFile.read((char*)&_volSizeX, sizeof(float));
	inFile.read((char*)&_volSizeY, sizeof(float));
	inFile.read((char*)&_volSizeZ, sizeof(float));

	inFile.seekg(12, ios::cur);
	inFile.read((char*)&nScalars, sizeof(short));
	inFile.seekg(200, ios::cur);
	inFile.read((char*)&nProperties, sizeof(short));
	inFile.seekg(988, ios::beg);
	int sizeOfInt = sizeof(int);
	inFile.read((char*)&nFibers, sizeof(int));
	_fibers.resize(nFibers);
	inFile.seekg(1000, ios::beg);
	QProgressDialog prog("Loading track file...", "", 0, nFibers);
	prog.setCancelButton(NULL);
	int fiberCnt = 0;
	for (int fiberCnt = 0; fiberCnt<nFibers; ++fiberCnt)
	{
		prog.setValue(fiberCnt);
		int pos = inFile.tellg();
		int numOfPoints;
		inFile.read((char*)&numOfPoints, sizeof(int));
		ArrFiber fib = new FloatPoint[numOfPoints+1];
		fib[0].fill(0);
		fib[0](0) = numOfPoints;
		float fibLength = 0;
		for (int i=1; i<=numOfPoints; ++i)
		{
			inFile.read((char*)fib[i].memptr(), 3*sizeof(float));
			inFile.seekg(nScalars*sizeof(float), ios::cur);
		}
		inFile.seekg(nProperties*sizeof(float), ios::cur);
		_fibers[fiberCnt] = fib;
	}
	setNumOfFibers(_fibers.size());
}

void FiberData::filterByLength(float minLength, float maxLength)
{
	QProgressDialog prog("Filtering...", "", 0, _nFibers);
	prog.setCancelButton(NULL);
	for (int i=0; i<_nFibers; ++i)
	{
		prog.setValue(i);
		float fibLength = 0;
		for (int j=2; j<=_fibers[i][0](0); ++j)
		{
			FloatPoint d = _fibers[i][j] - _fibers[i][j-1];
			fibLength += norm(d,2);
		}
		if (fibLength<minLength || fibLength>maxLength)
		{
			_filterBuf[i] = false;
		} else
		{
			_filterBuf[i] = true;
		}
	}
}

void FiberData::saveFibersAsTrk(const QString &fileName)
{
	ofstream outFile (fileName.toStdString().c_str(), ios::binary);
	short nScalars = 0;
	short nProperties = 0;

	outFile.write("TRACK\0", 6);

	outFile.write((char*)&_sizeX, sizeof(short));
	outFile.write((char*)&_sizeY, sizeof(short));
	outFile.write((char*)&_sizeZ, sizeof(short));

	outFile.write((char*)&_volSizeX, sizeof(float));
	outFile.write((char*)&_volSizeY, sizeof(float));
	outFile.write((char*)&_volSizeZ, sizeof(float));

	outFile.seekp(12, ios::cur);
	outFile.write((char*)&nScalars, sizeof(short));
	outFile.seekp(200, ios::cur);
	outFile.write((char*)&nProperties, sizeof(short));
	outFile.seekp(988, ios::beg);

	int nFilteredFibers = count(_filterBuf.begin(), _filterBuf.end(), true);
	outFile.write((char*)&nFilteredFibers, sizeof(int));

	outFile.seekp(1000, ios::beg);
	QProgressDialog prog("Saving track file...", "", 0, _nFibers);
	prog.setCancelButton(NULL);
	for (int i = 0; i<_nFibers; ++i)
	{
		prog.setValue(i);
		if (!_filterBuf[i])
		{
			continue;
		}
		int numOfPoints = _fibers[i][0](0);
		outFile.write((char*)&numOfPoints, sizeof(int));
		for (int j=1; j<=numOfPoints; ++j)
		{
			outFile.write((char*)_fibers[i][j].memptr(), 3*sizeof(float));
		}
	}
	outFile.close();
}

void FiberData::setColorMode(int clrMode)
{
	if (_colorMode!=clrMode)
	{
		_colorMode = (ColorMode)clrMode;
		genFiberDispList();
	}
}

void FiberData::downSampleRandom(int nSamples)
{
	vector<bool> selected (_nFibers);
	selected.assign(_nFibers, false);

	srand(time(NULL));
	int nSelected = 0;
	for (int i=0; i<_nFibers; ++i)
	{
		float p = (nSamples-nSelected)*1.0f/(_nFibers-i);
		if ((rand()%1000)/1000.0f<p)
		{
			selected[i] = true;
			++nSelected;
		}
	}

	int pos = 0;
	vector<ArrFiber>::iterator it = _fibers.begin();
	while(it!=_fibers.end())
	{
		if (!selected[pos])
		{
			it = _fibers.erase(it);
			/*int w = it-_fibers.begin();
			pos=pos;*/
		} else
		{
			it++;
		}
		pos++;
	}

	setNumOfFibers(_fibers.size());
}

float fibDistLCSS(const ArrFiber fib1, const ArrFiber fib2, float e, int d)
{
	int n1 = fib1[0](0);
	int n2 = fib2[0](0);

	vector<float> m((n1+1)*(n2+1));
	vector<float> simRev (2);
	for (int rev = 0; rev<=1; ++rev)
	{
		m.assign((n1+1)*(n2+1), 0);
		for (int i=1; i<=n1; ++i)
		{
			for (int j=max(i-d, 1); j<=min(i+d, n2); ++j)
			{
				float dist = rev?norm(fib1[n1-i+1]-fib2[j], "inf"):norm(fib1[i]-fib2[j], "inf");
				if (dist<e)
				{
					m[i*(n2+1)+j] = 1 + m[(i-1)*(n2+1)+j-1];
				} else
				{
					m[i*(n2+1)+j] = max(m[(i-1)*(n2+1)+j], m[i*(n2+1)+(j-1)]);
				}
			}
		}

		float shapeDist = 1-m[(n1+1)*(n2+1)-1]/min(n1, n2);
		float endDist = 0;
		if (rev)
		{
			endDist += norm(fib1[n1]-fib2[1], 2) + norm(fib1[1]-fib2[n2], 2);
		} else
		{
			endDist += norm(fib1[1]-fib2[1], 2) + norm(fib1[n1]-fib2[n2], 2);
		}
		endDist = 1-exp(-endDist*endDist/90);

		simRev[rev] = 0.5*shapeDist+0.5*endDist;
	}

	return min(simRev[0], simRev[1]);
}

float fibDistDTW(const ArrFiber fib1, const ArrFiber fib2, int d, vector<int>& matching)
{
	int n1 = fib1[0](0);
	int n2 = fib2[0](0);

	int matSize = (n1+1)*(n2+1);
	vector<float> m(matSize*2);
	m.assign(m.size(), 1.0e5);
	vector<float> simRev (2);
	float *mPtr = &m[0];
	
	for (int rev = 0; rev<=1; ++rev)
	{
		m[rev*matSize] = 0;

		for (int i=1; i<=n1; ++i)
		{
			//for (int j=max(i-d, 1); j<=min(i+d, n2); ++j)
			for (int j=1; j<=n2; ++j)
			{
				float dist = rev?norm(fib1[n1-i+1]-fib2[j], 2):norm(fib1[i]-fib2[j], 2);
				m[rev*matSize+i*(n2+1)+j] = dist + min(m[rev*matSize+(i-1)*(n2+1)+(j-1)], 
					min(m[rev*matSize+(i-1)*(n2+1)+j], m[rev*matSize+i*(n2+1)+j-1]));
			}
		}
		simRev[rev] = m[(rev+1)*matSize-1];
	}
	
	bool bRev = simRev[0]>simRev[1];
	float distDTW = bRev?simRev[1]:simRev[0];

	matching.resize(n1+1);
	matching.assign(n1+1, -1);
	if (bRev)
	{
		m.erase(m.begin(), m.begin()+matSize);
	} else
	{
		m.resize(matSize);
	}

	int i = n1;
	int j = n2;
	while (i>=1 && j>=1)
	{
		if (matching[i] == -1)
		{
			matching[i] = j;
		} else
		{
			if (norm(fib1[i]-fib2[matching[i]], 2)>norm(fib1[i]-fib2[j], 2))
			{
				matching[i] = j;
			}
		}

		if (m[(i-1)*(n2+1)+j]<m[i*(n2+1)+j-1])
		{
			if (m[(i-1)*(n2+1)+j]<m[(i-1)*(n2+1)+j-1])
			{
				i--;
			} else
			{
				i--;
				j--;
			}
		} else
		{
			if (m[i*(n2+1)+j-1]<m[(i-1)*(n2+1)+j-1])
			{
				j--;
			} else
			{
				i--;
				j--;
			}

		}
	}

	float length = 0;
	for (int i=2; i<=n1; ++i)
	{
		length += norm(fib1[i]-fib1[i-1], 2);
	}

	return distDTW/length;
}

void FiberData::getClusterCovs(const vector<vector<Link> > &matchings, vector<vector<fmat> > &clusterCovs)
{
	int nClusters = _clusterCenters.size();
	clusterCovs.resize(nClusters);

	for (int i=0; i<nClusters; ++i)
	{
		int centerSize = _clusterCenters[i][0](0);
		clusterCovs[i].resize(centerSize);
		for (int j=0; j<centerSize; ++j)
		{
			clusterCovs[i][j].fill(0);
		}

		int clusterSize = count(_clusters.begin(), _clusters.end(), i);
		if (clusterSize<=1)
		{
			for (int j=0; j<centerSize; ++j)
			{
				clusterCovs[i][j](0,0) = 1;
				clusterCovs[i][j](1,1) = 1;
				clusterCovs[i][j](2,2) = 1;
			}
			continue;
		}

		vector<int> matchedPts (centerSize);

		for (int j=0; j<_nFibers; ++j)
		{
			if (_clusters[j] == i)
			{
				for (int k=1; k<=_fibers[j][0](0); ++k)
				{
					int ik = matchings[j][i][k];
					fvec d = _fibers[j][k]-_clusterCenters[i][ik];
					clusterCovs[i][ik-1] += d*trans(d);
					matchedPts[ik-1]++;
				}
			}
		}

		for (int j=0; j<centerSize; ++j)
		{
			fmat tmpInv;
			tmpInv = inv(clusterCovs[i][j]/matchedPts[j]);
			if (tmpInv.n_elem==0)
			{
				clusterCovs[i][j] <<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
			} else
			{
				clusterCovs[i][j] = tmpInv;
			}
		}
	}
}

void matchFiber(const ArrFiber f1, const ArrFiber f2, Link &matching)
{
	int ptCount1 = f1[0](0), ptCount2 = f2[0](0);
	for (int i=1; i<=ptCount1; ++i)
	{
		float minDist = 1.0e5;
		for (int j=1; j<=ptCount2; ++j)
		{
			float d = norm(f1[i]-f2[j], 2);
			if (d<minDist)
			{
				minDist = d;
				matching[i] = j;
			}
		}
	}	
}

void FiberData::getMatchings(vector<vector<Link> > &matchings)
{
	int nClusters = _clusterCenters.size();
	matchings.resize(_nFibers);
	for (int i=0; i<_nFibers; ++i)
	{
		matchings[i].resize(nClusters);
		for (int k=0; k<nClusters; ++k)
		{
			matchings[i][k].resize(_fibers[i][0](0)+1);
			matchFiber(_fibers[i], _clusterCenters[k], matchings[i][k]);
		}
	}
}

void FiberData::getMatchingsDTW(vector<vector<Link> > &matchings)
{
	int nClusters = _clusterCenters.size();
	matchings.resize(_nFibers);
	for (int i=0; i<_nFibers; ++i)
	{
		matchings[i].resize(nClusters);
		for (int k=0; k<nClusters; ++k)
		{
			matchings[i][k].resize(_fibers[i][0](0)+1);
			//matchFiber(_fibers[i], _clusterCenters[k], matchings[i][k]);
			fibDistDTW(_fibers[i], _clusterCenters[k], 10, matchings[i][k]);
		}
	}
}

float fibDistMadah(const ArrFiber fib, const ArrFiber center, const Link &matching, const vector<fmat33> &covs)
{
	float dMadah;
	int n1 = fib[0](0);
	int n2 = center[0](0);

	if (n2==0)
	{
		return n1==0?0:1.0e6;
	}
	float dMahalanobis = 0;
	//int nRepMatching = 0;
	int i, ik;
	fvec d;
	float len1 = 0;
	float len2 = 0;
	float lenRep = 0;
	for (int i=2; i<=n2; ++i)
	{
		len2 += norm(center[i]-center[i-1], 2);
	}

	for (i=1; i<=n1; ++i)
	{
		ik = matching[i];
		d = fib[i]-center[ik];
		float dist = covs.empty()?dot(d, d):dot(d, covs[ik]*d);
		//float dist = dot(d,d);
		if (i>1 && matching[i-1] == ik)
		{
			//++nRepMatching;
			lenRep += dist;
		}
		dMahalanobis += dist;

		if (i>1)
		{
			len1 += norm(fib[i]-fib[i-1], 2);
		}
	}
	dMadah = sqrt(dMahalanobis)*(len2+lenRep)/len1/len1;
	if (dMadah!=dMadah)
	{
		int stop = 1;
	}
	return dMadah;
}

void FiberData::initClusterMedoids()
{
	int nClusters = *max_element(_clusters.begin(), _clusters.end())+1;
	_clusterCenters.resize(nClusters);

	resampleEqualSample(15);

	for (int i=0; i<nClusters; ++i)
	{
		float minCost = 1.0e10;
		int medoidIdx = -1;
		int clusterSize = count(_clusters.begin(), _clusters.end(), i);
		for (int j=0; j<_nFibers; ++j)
		{
			if (_clusters[j] != i)
			{
				continue;
			}
			float cost = 0;
			for (int k=0; k<_nFibers; ++k)
			{
				if (_clusters[k] == i && k!=j)
				{
					Link m (_fibers[k][0](0)+1);
					matchFiber(_fibers[k], _fibers[j], m);
					cost += fibDistMadah(_fibers[k], _fibers[j], m)/clusterSize;
				}
			}
			if (cost < minCost)
			{
				medoidIdx = j;
				cost = minCost;
			}
		}
		if (medoidIdx==-1)
		{
			_clusterCenters[i] = new FloatPoint[1];
			_clusterCenters[i][0].fill(0);
			continue;
		} else
		{
			ArrFiber medoidFib = _fibers[medoidIdx];
			_clusterCenters[i] = new FloatPoint[(int)medoidFib[0](0)+1];
			for (int j=0; j<=medoidFib[0](0); ++j)
			{
				_clusterCenters[i][j] = medoidFib[j];
			}
		}
	}
}

void FiberData::updateClusterCenters(const vector<vector<Link> > &matchings, vector<bool> &bClusterChanged)
{
	int nClusters = _clusterCenters.size();
	_clusterCovs.resize(nClusters);

	for (int i=0; i<nClusters; ++i)
	{
		/*if (!bClusterChanged[i])
		{
			continue;
		}*/

		int clusterSize = count(_clusters.begin(), _clusters.end(), i);
		if (clusterSize==0)
		{
			_clusterCenters[i] = new FloatPoint[1];
			_clusterCenters[i][0].fill(0); 
			_clusterCovs[i].resize(1);
			continue;
		}

		int nPoints = _clusterCenters[i][0](0);
		_clusterCovs[i].resize(nPoints+1);
		if (clusterSize<=3)
		{
			int fibIdx = find(_clusters.begin(), _clusters.end(), i)-_clusters.begin();
			for (int k=1; k<=nPoints; ++k)
			{
				_clusterCenters[i][k] = _fibers[fibIdx][k];
			}

			for(int k=0; k<=nPoints; ++k)
			{
				_clusterCovs[i][k] <<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
			}
			continue;
		}

		vector<vector<FloatPoint> > matchedBufs (nPoints+1);

		for (int j=0; j<_nFibers; ++j)
		{
			if (_clusters[j]==i)
			{
				const Link &matching = matchings[j][i];
				for (int k=1; k<=_fibers[j][0](0); ++k)
				{
					int ik = matching[k];
					matchedBufs[ik].push_back(_fibers[j][k]);
				}
			}
		}

		for (int j=1; j<=nPoints; ++j)
		{
			if (matchedBufs[j].size()>0)
			{
				_clusterCenters[i][j].fill(0);
				for (int k=0; k<matchedBufs[j].size(); ++k)
				{
					_clusterCenters[i][j] += matchedBufs[j][k];
				}
				_clusterCenters[i][j] /= matchedBufs[j].size();

				_clusterCovs[i][j].fill(0);
				for (int k=0; k<matchedBufs[j].size(); ++k)
				{
					FloatPoint d = matchedBufs[j][k]-_clusterCenters[i][j];
					_clusterCovs[i][j] += d*trans(d);
				}
				_clusterCovs[i][j] /= matchedBufs[j].size();

				fmat tmpInv = inv(_clusterCovs[i][j]);
				if (tmpInv.n_elem==0)
				{
					_clusterCovs[i][j] <<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
				} else
				{
					float n = trace(abs(tmpInv));
					if (n>1.0e5)
					{
						_clusterCovs[i][j] <<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
					} else
					{
						_clusterCovs[i][j] = tmpInv;
					}
				}
			}
		}

		for (int j=1; j<=nPoints; ++j)
		{
			if (matchedBufs[j].size() == 0)
			{
				if (j==1)
				{
					int nonZeroIdxNext = 1;
					while (matchedBufs[nonZeroIdxNext].size()==0)
					{
						++nonZeroIdxNext;
					}
					_clusterCenters[i][j] = _clusterCenters[i][nonZeroIdxNext];
				} else if (j==nPoints)
				{
					int nonZeroIdxPrev = j;
					while (matchedBufs[nonZeroIdxPrev].size()==0)
					{
						--nonZeroIdxPrev;
					}
					_clusterCenters[i][j] = _clusterCenters[i][nonZeroIdxPrev];
				} else
				{
					int nonZeroIdxPrev = j;
					int nonZeroIdxNext = j;
					while (matchedBufs[nonZeroIdxNext].size()==0)
					{
						++nonZeroIdxNext;
					}
					while (matchedBufs[nonZeroIdxPrev].size()==0)
					{
						--nonZeroIdxPrev;
					}
					_clusterCenters[i][j] = (_clusterCenters[i][nonZeroIdxNext]+_clusterCenters[i][nonZeroIdxPrev])*0.5;
				}

				_clusterCovs[i][j] <<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
			}
		}
	}
}

void FiberData::updateFuzzyClustering(const vector<float> &distBuf, bool bEStep)
{	
	/*E-Step*/
	if (bEStep)
	{
		double filterThres = 0.25*1.0e-3;
		_fuzzyClusters.assign(_nFibers*_nClusters, 0);
		_filterBuf.assign(_nFibers, false);
		for (int i=0; i<_nFibers; ++i)
		{
			/*if (_filterBuf[i])
			{
				continue;
			}*/
			double denom = 0;
			for (int j=0; j<_nClusters; ++j)
			{
				float dist = distBuf[i*_nClusters+j];
				double fp = _paramW[j]*getGammaFunc(dist, _paramAlpha[j], _paramBeta[j]);
				_fuzzyClusters[i*_nClusters+j] = fp;
				denom += fp;
			}

			if (denom>filterThres)
			{
				for (int j=0; j<_nClusters; ++j)
				{
					_fuzzyClusters[i*_nClusters+j] /= denom;
				}
			} else
			{
				for (int j=0; j<_nClusters; ++j)
				{
					_fuzzyClusters[i*_nClusters+j] =0;
				}
				_filterBuf[i] = true;
			}
		}
	}


	double lastCostFunc = getLikelihoodFunc(distBuf);
	/*M-Step*/
	int nNotFiltered = count(_filterBuf.begin(), _filterBuf.end(), false);
	vector<double> sumPikBuf (_nClusters);
	vector<double> sumDistBuf (_nClusters);
	vector<double> sumLogDistBuf (_nClusters);
	vector<double> xBuf (_nClusters);
	for (int i=0; i<_nClusters; ++i)
	{
		sumPikBuf[i] = 0;
		sumDistBuf[i] = 0;
		sumLogDistBuf[i] = 0;
		for (int j=0; j<_nFibers; ++j)
		{
			if (!_filterBuf[j])
			{
				sumPikBuf[i] += _fuzzyClusters[j*_nClusters+i];
				sumDistBuf[i] += _fuzzyClusters[j*_nClusters+i]*distBuf[j*_nClusters+i];
				sumLogDistBuf[i] += _fuzzyClusters[j*_nClusters+i]*log(std::max(distBuf[j*_nClusters+i],1.0e-6f));
			}
		}

		xBuf[i] = log(sumDistBuf[i]/sumPikBuf[i]) - sumLogDistBuf[i]/sumPikBuf[i];

		_paramBeta[i] = _paramAlpha[i]*sumPikBuf[i]/sumDistBuf[i];

		double lastCostAlpha = getCostAlpha(_paramAlpha[i], sumPikBuf[i], sumDistBuf[i], sumLogDistBuf[i]);
		double lastAlpha = _paramAlpha[i];
		_paramAlpha[i] = getAlpha(xBuf[i]);
		double costAlpha = getCostAlpha(_paramAlpha[i], sumPikBuf[i], sumDistBuf[i], sumLogDistBuf[i]);
		if (costAlpha<lastCostAlpha)
		{
			ofstream logFile ("log.txt", ios::app);
			logFile <<"=========alpha cost error==========="<<endl;
			logFile <<"\t*******cluster #" <<i <<"**********"<<endl;
			logFile <<"\t\tsumPik = " <<sumPikBuf[i] <<endl;
			logFile <<"\t\tsumDist = " <<sumDistBuf[i] <<endl;
			logFile <<"\t\tsumLogDist = " <<sumLogDistBuf[i] <<endl;
			//logFile <<"\t\tx = " <<xBuf[i] <<endl;
			logFile <<"\t\talpha = " <<lastAlpha <<"=>" <<_paramAlpha[i] <<endl;
			//logFile <<"\t\tbeta = " <<_paramBeta[i] <<endl;
			//logFile <<"\t\tw = " <<_paramW[i] <<endl;
			logFile <<"alpha cost: " <<lastCostAlpha <<"=>" <<costAlpha <<"(" <<costAlpha-lastCostAlpha <<")"<<endl;
			logFile.close();
		}

		_paramW[i] = sumPikBuf[i]/nNotFiltered;
	}

	double costFunc = getLikelihoodFunc(distBuf);
	if (costFunc<lastCostFunc)
	{
		ofstream logFile ("log.txt", ios::app);
		logFile <<"=========cost function error==========="<<endl;
		for (int i=0; i<_nClusters; ++i)
		{
			logFile <<"\t*******cluster #" <<i <<"**********"<<endl;
			logFile <<"\t\tsumPik = " <<sumPikBuf[i] <<endl;
			logFile <<"\t\tsumDist = " <<sumDistBuf[i] <<endl;
			logFile <<"\t\tsumLogDist = " <<sumLogDistBuf[i] <<endl;
			logFile <<"\t\tx = " <<xBuf[i] <<endl;
			logFile <<"\t\talpha = " <<_paramAlpha[i] <<endl;
			logFile <<"\t\tbeta = " <<_paramBeta[i] <<endl;
			logFile <<"\t\tw = " <<_paramW[i] <<endl;
		}
		logFile <<"costFunc: " <<lastCostFunc <<"=>" <<costFunc <<endl;
		logFile.close();
	}
}

void FiberData::clusterFuzzyCMeans(bool bUpdate, const vector<int> &seedBuf, int nSteps)
{
	if (!bUpdate)
	{
		initClusters(seedBuf.size(), seedBuf);

		_paramAlpha.resize(_nClusters);
		_paramBeta.resize(_nClusters);
		_paramW.resize(_nClusters);
		_paramAlpha.assign(_nClusters, 1.0);
		_paramBeta.assign(_nClusters, 10);
		_paramW.assign(_nClusters, 1.0f/_nClusters);
	} else
	{
		updateClusterCentersFuzzy(_matchings);
	}

	float costFunc = 1.0e6;
	float lastCostFunc = 0;
	vector<float> distBuf (_nFibers*_nClusters);
	int nIt = 0;

	getMatchings(_matchings);
	for (int i=0; i<_nFibers; ++i)
	{
		for (int j=0; j<_nClusters; ++j)
		{
			distBuf[i*_nClusters+j] = fibDistMadah(_fibers[i], _clusterCenters[j], _matchings[i][j], _clusterCovs[j]);
		}
	}

	const float *ptrDistBuf = &distBuf[0];

	while (nIt <nSteps && fabs((costFunc-lastCostFunc)/lastCostFunc)>1.0e-3)
	//while (true)
	{

		lastCostFunc = costFunc;

		if (bUpdate&&nIt==0)
		{
			updateFuzzyClustering(distBuf, false);
		} else
		{
			updateFuzzyClustering(distBuf, true);
		}

		costFunc = getLikelihoodFunc(distBuf);
		if (nIt>0&&costFunc<lastCostFunc)
		{
			float e = costFunc-lastCostFunc;
			int stop = 1;
		}

		++nIt;
	}

	updateClusterCentersFuzzy(_matchings);
	getClustersFromFuzzyClusters();
}

double FiberData::getGammaFunc(double d, double alpha, double beta)
{
	return pow(min(max(d, 1.0e-6), 1.0e2), alpha-1)*pow(beta, alpha)*exp(-beta*d)/Gamma(alpha);
}

float FiberData::getLikelihoodFunc(const vector<float> &distBuf)
{
	float costFunc = 0;
	for (int i=0; i<_nFibers; ++i)
	{
		//if (!_filterBuf[i])
		{
			double l = 0;
			for (int j=0; j<_nClusters; ++j)
			{
				/*float pIK, lg, wk, c;
				pIK = _fuzzyClusters[i*_nClusters+j];
				lg = (_paramAlpha[j]-1)*log(distBuf[i*_nClusters+j]+1.0e-6) + _paramAlpha[j]*log(_paramBeta[j]+1.0e-6) - _paramBeta[j]*distBuf[i*_nClusters+j]-LogGamma(_paramAlpha[j]);
				wk = log(_paramW[j]);
				c = pIK*(lg+wk);
				costFunc += c;*/
				l += _paramW[j] * getGammaFunc(distBuf[i*_nClusters+j], _paramAlpha[j], _paramBeta[j]);
			}
			costFunc += log(max(l, 1.0e-6));
		}
	}

	return costFunc;
}

void FiberData::updateClusterCentersFuzzy(const vector<vector<Link> > &matchings, vector<bool> &bClusterChanged)
{
	int nClusters = _clusterCenters.size();
	_clusterCovs.resize(nClusters);

	for (int i=0; i<nClusters; ++i)
	{
		//logFile <<"****************Cluster #"<<i<<"*****************" <<endl;

		int nPoints = _clusterCenters[i][0](0);
		vector<float> denomBuf(nPoints+1);
		int clusterSize = count(_clusters.begin(), _clusters.end(), i);

		for (int j=1; j<=nPoints; ++j)
		{

			FloatPoint meanPt;
			meanPt.fill(0);
			denomBuf[j] = 0;
			for (int k=0; k<_nFibers; ++k)
			{
				//float p = _fuzzyClusters[k*_nClusters+i];
				float p = _fuzzyClusters[k*_nClusters+i]*_fuzzyClusters[k*_nClusters+i];
				if (p==0)
				{
					continue;
				}
				const Link &matching = matchings[k][i];
				//vector<int> corresPts;
				for (int l=1; l<=nPoints; ++l)
				{
					if (matching[l]==j)
					{
						meanPt += _fibers[k][l]*p;
						denomBuf[j] += p;
						//corresPts.push_back(l);
					}
				}
			}

			if (denomBuf[j]>0)
			{
				meanPt = meanPt/denomBuf[j];
				_clusterCenters[i][j] = meanPt;
			}

		}

		for (int j=1; j<=nPoints; ++j)
		{
			if (denomBuf[j]==0)
			{
				if (j==1)
				{
					int nonZeroIdxNext = 1;
					while (nonZeroIdxNext<nPoints && denomBuf[nonZeroIdxNext]==0)
					{
						++nonZeroIdxNext;
					}
					_clusterCenters[i][j] = _clusterCenters[i][nonZeroIdxNext];
				} else if (j==nPoints)
				{
					int nonZeroIdxPrev = j;
					while (nonZeroIdxPrev>1 && denomBuf[nonZeroIdxPrev]==0)
					{
						--nonZeroIdxPrev;
					}
					_clusterCenters[i][j] = _clusterCenters[i][nonZeroIdxPrev];
				} else
				{
					int nonZeroIdxPrev = j;
					int nonZeroIdxNext = j;
					while (nonZeroIdxNext<nPoints && denomBuf[nonZeroIdxNext]==0)
					{
						++nonZeroIdxNext;
					}
					while (nonZeroIdxPrev>1 && denomBuf[nonZeroIdxPrev]==0)
					{
						--nonZeroIdxPrev;
					}
					_clusterCenters[i][j] = (_clusterCenters[i][nonZeroIdxNext]+_clusterCenters[i][nonZeroIdxPrev])*0.5;
				}
			}			
		}

		for (int j=2; j<nPoints; ++j)
		{
			_clusterCenters[i][j] = (_clusterCenters[i][j-1]+_clusterCenters[i][j]+_clusterCenters[i][j+1])/3.0;
		}

		_clusterCovs[i].resize(nPoints+1);
		for (int j=1; j<=nPoints; ++j)
		{
			if (denomBuf[j]==0)
			{
				_clusterCovs[i][j] <<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
			} else
			{
				_clusterCovs[i][j].fill(0);
				for (int k=0; k<_nFibers; ++k)
				{
					const Link &matching = matchings[k][i];
					vector<int> corresPts;
					for (int l=1; l<=nPoints; ++l)
					{
						if (matching[l]==j)
						{
							FloatPoint d = _fibers[k][l]-_clusterCenters[i][j];
							FloatPoint d1 = d*(1-dot(d, _clusterCenters[i][j])/(norm(d,2)*norm(_clusterCenters[i][j],2)));
							//_clusterCovs[i][j] += _fuzzyClusters[k*_nClusters+i]*(d1*trans(d1));
							//_clusterCovs[i][j] += _fuzzyClusters[k*_nClusters+i]*(d*trans(d));
							_clusterCovs[i][j] += (_fuzzyClusters[k*_nClusters+i]*_fuzzyClusters[k*_nClusters+i])*(d*trans(d));
							//corresPts.push_back(l);
						}
					}

					/*if (!corresPts.empty())
					{
						float minDist = 1.0e6;
						int minCorresPt = corresPts[0];
						for (int n=0; n<corresPts.size(); ++n)
						{
							float d = norm(_fibers[k][corresPts[n]]-_clusterCenters[i][j], 2);
							if (d<minDist)
							{
								minCorresPt = corresPts[n];
								minDist = d;
							}
						}
						FloatPoint dVec = _fibers[k][minCorresPt]-_clusterCenters[i][j];
						_clusterCovs[i][j] += _fuzzyClusters[k*_nClusters+i]*(dVec*trans(dVec));
					}*/
				}

				_clusterCovs[i][j] /= denomBuf[j];
				fmat tmpInv = inv(_clusterCovs[i][j]);
				if (tmpInv.n_elem==0 || trace(abs(tmpInv))>1.0e5)
				{
					_clusterCovs[i][j] <<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
				} else
				{
					_clusterCovs[i][j] = tmpInv;
				}
			}
		}
		
	}
}

void FiberData::getClustersFromFuzzyClusters()
{
	_clusters.resize(_nFibers);
	for (int i=0; i<_nFibers; ++i)
	{
		vector<double>::iterator maxIt = max_element(_fuzzyClusters.begin()+i*_nClusters, _fuzzyClusters.begin()+(i+1)*_nClusters);
		if (*maxIt<1.0e-6)
		{
			_clusters[i] = -1;
		} else
		{
			_clusters[i] = maxIt-(_fuzzyClusters.begin()+i*_nClusters);
		}
	}
}

Hypergraph * FiberData::getGraphFuzzyCMeans()
{
	Hypergraph *g = new Hypergraph();
	g->setNumOfVertices(_nFibers);
	float probThres = 0.1;
	float distThres = 10;

	/*vector<HyperEdge> edges (_nClusters);
	
	for (int i=0; i<_nFibers; ++i)
	{
		float maxProb = *max_element(_fuzzyClusters.begin()+i*_nClusters, _fuzzyClusters.begin()+(i+1)*_nClusters);
		if (maxProb<1.0e-6)
		{
			if (distThres)
			{
				Link matching (_fibers[j][0](0)+1);
				matchFiber(_fibers[j], _clusterCenters[i], matching);
				float dist = fibDistMadah(_fibers[j], _clusterCenters[i], matching, _clusterCovs[i]);
				if (dist<=distThres)
				{
					e.push_back(j);
				}
			}
		} else
		{
			float prob = _fuzzyClusters[j*_nClusters+i];
			if (prob>=probThres)
			{
				e.push_back(j);
			}
		}
	}*/

	for (int i=0; i<_nClusters; ++i)
	{
		HyperEdge e;

		for (int j=0; j<_nFibers; ++j)
		{
			float maxProb = *max_element(_fuzzyClusters.begin()+j*_nClusters, _fuzzyClusters.begin()+(j+1)*_nClusters);
			if (maxProb<1.0e-6)
			{
				/*Link matching (_fibers[j][0](0)+1);
				matchFiber(_fibers[j], _clusterCenters[i], matching);*/
				float dist = fibDistMadah(_fibers[j], _clusterCenters[i], _matchings[j][i], _clusterCovs[i]);
				if (dist<=distThres)
				{
					e.push_back(j);
				}
			} else
			{
				float prob = _fuzzyClusters[j*_nClusters+i];
				if (prob>=probThres)
				{
					e.push_back(j);
				}
			}
		}

		g->addEdge(e);
	}
	return g;
}

void FiberData::assignCluster(int fibIdx, int clusterIdx)
{
	for (int i=0; i<_nClusters; ++i)
	{
		_fuzzyClusters[fibIdx*_nClusters+i] = i==clusterIdx?1.0:0.0;
		_filterBuf[fibIdx] = false;
	}
}

void FiberData::saveFuzzyClusters(const QString &fileName)
{
	ofstream outFile (fileName.toStdString().c_str(), ios::binary);
	outFile.write((char*)&_nFibers, sizeof(int));
	outFile.write((char*)&_nClusters, sizeof(int));
	outFile.write((char*)&_fuzzyClusters[0], _nFibers*_nClusters*sizeof(float));
	outFile.write((char*)&_paramAlpha[0], _nClusters*sizeof(float));
	outFile.write((char*)&_paramBeta[0], _nClusters*sizeof(float));
	outFile.write((char*)&_paramW[0], _nClusters*sizeof(float));
	for (int i=0; i<_nClusters; ++i)
	{
		int nPoints = _clusterCenters[i][0](0);
		outFile.write((char*)&nPoints, sizeof(int));
		for (int j=1; j<=nPoints; ++j)
		{
			outFile.write((char*)_clusterCenters[i][j].memptr(), 3*sizeof(float));
			outFile.write((char*)_clusterCovs[i][j].memptr(), 9*sizeof(float));
		}
	}
	outFile.close();
}

void FiberData::loadFuzzyClusters(const QString &fileName)
{
	ifstream inFile(fileName.toStdString().c_str(), ios::binary);
	int nFibers;
	inFile.read((char*)&nFibers, sizeof(int));
	if (nFibers!=_nFibers)
	{
		return;
	}
	inFile.read((char*)&_nClusters, sizeof(int));
	_fuzzyClusters.resize(_nFibers*_nClusters);
	_clusterCenters.resize(_nClusters);
	_clusterCovs.resize(_nClusters);
	_paramAlpha.resize(_nClusters);
	_paramBeta.resize(_nClusters);
	_paramW.resize(_nClusters);
	inFile.read((char*)&_fuzzyClusters[0], _nFibers*_nClusters*sizeof(float));
	inFile.read((char*)&_paramAlpha[0], _nClusters*sizeof(float));
	inFile.read((char*)&_paramBeta[0], _nClusters*sizeof(float));
	inFile.read((char*)&_paramW[0], _nClusters*sizeof(float));

	for (int i=0; i<_nClusters; ++i)
	{
		int nPoints;
		inFile.read((char*)&nPoints, sizeof(int));
		_clusterCenters[i] = new FloatPoint [nPoints+1];
		_clusterCovs[i].resize(nPoints+1);
		_clusterCenters[i][0](0) = nPoints;

		for (int j=1; j<=nPoints; ++j)
		{
			inFile.read((char*)_clusterCenters[i][j].memptr(), 3*sizeof(float));
			inFile.read((char*)_clusterCovs[i][j].memptr(), 9*sizeof(float));
		}
	}

	getClustersFromFuzzyClusters();
	initClusterColor();

	_filterBuf.resize(_nFibers);
	_filterBuf.assign(_nFibers, false);
	for (int i=0; i<_nFibers; ++i)
	{
		_filterBuf[i] = _clusters[i]==-1;
	}
}

vector<int> FiberData::getSelectedIdx()
{
	vector<int> selectedIdx (0);
	for (int i=0; i<_nFibers; ++i)
	{
		if (_selectedBuf[i])
		{
			selectedIdx.push_back(i);
		}
	}
	return selectedIdx;
}

void FiberData::assignSelectedToCluster(int clusterIdx)
{
	for (int i=0; i<_nFibers; ++i)
	{
		if (_selectedBuf[i])
		{
			assignCluster(i, clusterIdx);
		}
	}
}

double FiberData::getAlpha(double x)
{
	double xd = (double)x;
	double alpha, lastAlpha;
	alpha = (3-x+sqrt((x-3)*(x-3)+24*x))/(12*x);
	/*lastAlpha = 0;
	while (fabs(alpha-lastAlpha)>1.0e-3)
	{
		lastAlpha = alpha;
		alpha -= (log(alpha)-DiGamma(alpha)-x)/(1/alpha-TriGamma(alpha));
	}*/
	return alpha;
}

double FiberData::getCostAlpha(double alpha, double sumPik, double sumDist, double sumLogDist)
{
	return (sumLogDist - sumPik*log(sumDist/sumPik) - sumPik)*alpha + sumPik*alpha*log(alpha) - sumPik*LogGamma(alpha);
}

void FiberData::clusterFuzzyCMeansDTW(bool bUpdate, const vector<int> &seedBuf, int nSteps)
{
	if (!bUpdate)
	{
		_nClusters = seedBuf.size();
		if (seedBuf.empty())
		{
			return;
		}

		_fuzzyClusters.resize(_nClusters*_nFibers);
		_paramAlpha.resize(_nClusters);
		_paramBeta.resize(_nClusters);
		_paramW.resize(_nClusters);
		_clusterCovs.resize(_nClusters);

		_paramAlpha.assign(_nClusters, 1.0);
		_paramBeta.assign(_nClusters, 10);
		_paramW.assign(_nClusters, 1.0f/_nClusters);

		int nDim = 15;
		resampleEqualSample(nDim);
		for (int i=0; i<_nClusters; ++i)
		{
			_clusterCovs[i].resize(nDim+1);
			for (int j=0; j<=nDim; ++j)
			{
				_clusterCovs[i][j]<<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
			}
		}

		/*initiate cluster centers*/
		_clusterCenters.resize(_nClusters);
		for (int i=0; i<_nClusters; ++i)
		{
			_clusterCenters[i] = new FloatPoint[nDim+1];
			for (int j=0; j<=nDim; ++j)
			{
				_clusterCenters[i][j] = _fibers[seedBuf[i]][j];
			}

		}
		initClusterColor();
	} else
	{
		updateClusterCentersFuzzy(_matchings);
	}

	float costFunc = 1.0e6;
	float lastCostFunc = 0;
	vector<float> distBuf (_nFibers*_nClusters);
	int nIt = 0;

	getMatchings(_matchings);
	for (int i=0; i<_nFibers; ++i)
	{
		for (int j=0; j<_nClusters; ++j)
		{
			distBuf[i*_nClusters+j] = fibDistMadah(_fibers[i], _clusterCenters[j], _matchings[i][j], _clusterCovs[j]);
			//distBuf[i*_nClusters+j] = fibDistDTWCov(_fibers[i], _clusterCenters[j], 10, _matchings[i][j], _clusterCovs[j]);
		}
	}

	const float *ptrDistBuf = &distBuf[0];

	while (nIt <nSteps && fabs((costFunc-lastCostFunc)/lastCostFunc)>1.0e-3)
		//while (true)
	{

		lastCostFunc = costFunc;

		if (bUpdate&&nIt==0)
		{
			updateFuzzyClustering(distBuf, false);
		} else
		{
			updateFuzzyClustering(distBuf, true);
		}

		costFunc = getLikelihoodFunc(distBuf);
		if (nIt>0&&costFunc<lastCostFunc)
		{
			float e = costFunc-lastCostFunc;
			int stop = 1;
		}

		++nIt;
	}

	/*for (int i=0; i<_nFibers; ++i)
	{
		for (int j=0; j<_nClusters; ++j)
		{
			fibDistDTWCov(_fibers[i], _clusterCenters[j], 10, _matchings[i][j]);
		}
	}*/
	updateClusterCentersFuzzyDTW();
	getClustersFromFuzzyClusters();
}

float fibDistDTWCov(const ArrFiber fib1, const ArrFiber fib2, int d, vector<int>& matching, vector<fmat33> covs)
{
	int n1 = fib1[0](0);
	int n2 = fib2[0](0);

	bool bRev = false;
	float dist1 = norm(fib1[1]-fib2[1],2)+norm(fib1[n1]-fib2[n2],2);
	float dist2 = norm(fib1[1]-fib2[n2],2)+norm(fib1[n1]-fib2[1],2);
	if (dist1>dist2)
	{
		bRev = true;
	}

	int matSize = (n1+1)*(n2+1);
	vector<float> m(matSize);
	m.assign(m.size(), 1.0e5);
	float *mPtr = &m[0];

	m[0] = 0;
	for (int i=1; i<=n1; ++i)
	{
		//for (int j=max(i-d, 1); j<=min(i+d, n2); ++j)
		for (int j=1; j<=n2; ++j)
		{
			float dist = bRev?norm(fib1[i]-fib2[n2-j+1], 2):norm(fib1[i]-fib2[j], 2);
			m[i*(n2+1)+j] = dist + min(m[(i-1)*(n2+1)+(j-1)], min(m[(i-1)*(n2+1)+j], m[i*(n2+1)+j-1]));
		}
	}
	float distDTW = m[matSize-1];

	matching.resize(n1+1);
	matching.assign(n1+1, -1);

	int i = n1;
	int j = n2;
	float distDTWCov = 0;
	while (i>=1 && j>=1)
	{
		int jIdx = bRev?n2+1-j:j;

		FloatPoint d = fib1[i]-fib2[jIdx];
		distDTWCov += covs.empty()?sqrt(dot(d,d)):sqrt(dot(d, covs[jIdx]*d));

		if (matching[i] == -1)
		{
			matching[i] = jIdx;
		} else
		{
			if (norm(fib1[i]-fib2[matching[i]], 2)>norm(fib1[i]-fib2[jIdx], 2))
			{
				matching[i] = jIdx;
			}
		}

		if (m[(i-1)*(n2+1)+j]<m[i*(n2+1)+j-1])
		{
			if (m[(i-1)*(n2+1)+j]<m[(i-1)*(n2+1)+j-1])
			{
				i--;
			} else
			{
				i--;
				j--;
			}
		} else
		{
			if (m[i*(n2+1)+j-1]<m[(i-1)*(n2+1)+j-1])
			{
				j--;
			} else
			{
				i--;
				j--;
			}

		}
	}

	float length = 0;
	for (int i=2; i<=n2; ++i)
	{
		length += norm(fib2[i]-fib2[i-1], 2);
	}

	return distDTWCov/length;
}

void FiberData::updateClusterCentersFuzzyDTW()
{
	vector<vector<Link> > matchings (_nFibers);
	for (int i=0; i<_nFibers; ++i)
	{
		matchings[i].resize(_nClusters+1);
		for (int j=0; j<_nClusters; ++j)
		{
			fibDistDTWCov(_fibers[i], _clusterCenters[j], 10, matchings[i][j]);
		}
	}

	updateClusterCentersFuzzy(matchings);
}

void FiberData::initClusters(int nClusters, const vector<int> &seedBuf)
{
	_clusters.assign(_nFibers, 0);
	_nClusters = nClusters;
	_fuzzyClusters.resize(_nFibers*_nClusters);
	_fuzzyClusters.assign(_fuzzyClusters.size(), 1.0/_nClusters);

	/*make fibers of the same dimension*/
	int nDim = 15;
	this->resampleEqualSample(nDim);

	/*initiate cluster centers*/
	_clusterCenters.resize(_nClusters);

	if (seedBuf.empty())
	{
		int nClusterSpecified = 0;
		srand(time(NULL));
		for (int i=0; i<_nFibers; ++i)
		{
			float p = (_nClusters-nClusterSpecified)*1.0f / (_nFibers-i);
			if ((rand()%1000)/1000.0f < p)
			{
				_clusterCenters[nClusterSpecified] = new FloatPoint[nDim+1];
				for (int j=0; j<=nDim; ++j)
				{
					_clusterCenters[nClusterSpecified][j] = _fibers[i][j];
				}
				++nClusterSpecified;
			}
		}
	} else
	{
		for (int i=0; i<_nClusters; ++i)
		{
			_clusterCenters[i] = new FloatPoint[nDim+1];
			for (int j=0; j<=nDim; ++j)
			{
				_clusterCenters[i][j] = _fibers[seedBuf[i]][j];
			}
		}
	}

	_clusterCovs.resize(_nClusters);
	for (int i=0; i<_nClusters; ++i)
	{
		_clusterCovs[i].resize(nDim+1);
		for (int j=0; j<=nDim; ++j)
		{
			_clusterCovs[i][j]<<1<<0<<0<<endr<<0<<1<<0<<endr<<0<<0<<1<<endr;
		}
	}
	initClusterColor();
}

void FiberData::clusterAFCC(int maxNumOfClusters, const vector<int> &seedBuf, const vector<int> &mustLink, const vector<int> &cannotLink, bool bUpdate)
{
	if (!bUpdate)
	{
		initClusters(maxNumOfClusters, seedBuf);
	}
	
	//vector<double> clusterCards (_nClusters);
	//clusterCards.assign(_nClusters, _nFibers*1.0/_nClusters);

	double cost = 10000;
	double lastCost = 0;
	//int it0 = 5;
	//double eta0 = 1.0;
	//double tau = 1.0;
	int nIt = 0;
	double maxDiffMembership = 1.0;
	vector<float> distBuf (_nFibers*_nClusters);
	vector<float> constBuf (_nFibers*_nClusters);
	vector<double> oldMembership (_nFibers*_nClusters);
	while (maxDiffMembership>1.0e-1/* && fabs((cost-lastCost)/lastCost)>1.0e-3*/ && nIt<100)
	{
		lastCost = cost;
		maxDiffMembership = 0;

		getMatchings(_matchings);
		for (int i=0; i<_nFibers; ++i)
		{
			for (int j=0; j<_nClusters; ++j)
			{
				float d = fibDistMadah(_fibers[i], _clusterCenters[j], _matchings[i][j], _clusterCovs[j]);
				distBuf[i*_nClusters+j] = d;
			}
		}

		int nConstraints = mustLink.size()/2+cannotLink.size()/2;
		float alpha;
		if (nConstraints==0)
		{
			alpha = 0;
		} else
		{
			float alphaNumer = 0, alphaDenom = 0;
			for (int i=0; i<_nFibers*_nClusters; ++i)
			{
				alphaNumer += pow(_fuzzyClusters[i]*distBuf[i], 2);
				alphaDenom += _fuzzyClusters[i]*_fuzzyClusters[i];
			}
			alpha = sqrt(alphaNumer/alphaDenom)*_nFibers/nConstraints;
		}

		oldMembership.assign(_fuzzyClusters.begin(), _fuzzyClusters.end());

		constBuf.assign(constBuf.size(), 0);
		for (int i=0; i<_nFibers; ++i)
		{
			for (int j=0; j<_nClusters; ++j)
			{
				for (int k=0; k<mustLink.size()/2; ++k)
				{
					if (mustLink[2*k]==i||mustLink[2*k+1]==i)
					{
						int idx = mustLink[2*k]==i?mustLink[2*k+1]:mustLink[2*k];
						for (int l=0; l<_nClusters; ++l)
						{
							if (l!=j)
							{
								constBuf[i*_nClusters+j] += _fuzzyClusters[idx*_nClusters+l];
							}
						}
					}
				}

				for (int k=0; k<cannotLink.size()/2; ++k)
				{
					if (cannotLink[2*k]==i||cannotLink[2*k+1]==i)
					{
						int idx = cannotLink[2*k]==i?cannotLink[2*k+1]:cannotLink[2*k];
						constBuf[i*_nClusters+j] += _fuzzyClusters[idx*_nClusters+j];
					}
				}
			}
		}

		for (int i=0; i<_nFibers; ++i)
		{
			float sumInvSqrDist = 0;
			float Cvr = 0;
			for (int j=0; j<_nClusters; ++j)
			{
				float d = distBuf[i*_nClusters+j];
				sumInvSqrDist += 1.0f/(d*d+1.0e-6);
				Cvr += constBuf[i*_nClusters+j]/(d*d+1.0e-6);
			}
			Cvr /= sumInvSqrDist;

			float sumProb = 0;
			for (int j=0; j<_nClusters; ++j)
			{
				float d = distBuf[i*_nClusters+j];
				float uFCM = 1/(d*d+1.0e-6)/sumInvSqrDist;
				float uConstraints = alpha/2.0f*(Cvr-constBuf[i*_nClusters+j])/(d*d+1.0e-6);
				_fuzzyClusters[i*_nClusters+j] = uFCM+uConstraints;

				if (_fuzzyClusters[i*_nClusters+j]<0)
				{
					float p = _fuzzyClusters[i*_nClusters+j];
					_fuzzyClusters[i*_nClusters+j]=0;
					int stop = 1;
				}
				sumProb += _fuzzyClusters[i*_nClusters+j];


			}

			for (int j=0; j<_nClusters; ++j)
			{
				_fuzzyClusters[i*_nClusters+j] /= sumProb;

				if (fabs(_fuzzyClusters[i*_nClusters+j]-oldMembership[i*_nClusters+j])>maxDiffMembership)
				{
					maxDiffMembership = fabs(_fuzzyClusters[i*_nClusters+j]-oldMembership[i*_nClusters+j]);
				}
			}

			if (sumProb!=1.0)
			{
				float sumProb1 = sumProb/alpha;
				int stop = 1;
			}
		}

		updateClusterCentersFuzzy(_matchings);

		for (int i=0; i<_nFibers; ++i)
		{
			for (int j=0; j<_nClusters; ++j)
			{
				double currCost = pow(_fuzzyClusters[i*_nClusters+j]*distBuf[i*_nClusters+j], 2);
				cost += currCost;
			}
		}

		++nIt;

		getClustersFromFuzzyClusters();

		/*int bContinue = QMessageBox::question(NULL, "AFCC", QString("Cost = %1. Continue?").arg(cost), QMessageBox::Yes|QMessageBox::No);
		if (bContinue == QMessageBox::No)
		{
			break;
		}*/
	}
}

void FiberData::getPairwiseConstraints(int clusterIdx, vector<int> &mustLink, vector<int> &cannotLink)
{
	vector<int> repFibers (_nClusters);
	
	getMatchings(_matchings);
	for (int i=0; i<_nClusters; ++i)
	{
		float minDist = 1.0e5;
		for (int j=0; j<_nFibers; ++j)
		{
			float d = fibDistMadah(_fibers[j], _clusterCenters[i], _matchings[j][i], _clusterCovs[i]);
			if (d<minDist)
			{
				minDist = d;
				repFibers[i] = j;
			}
		}
	}

	for (int i=0; i<_nFibers; ++i)
	{
		if (_selectedBuf[i])
		{
			for (int j=0; j<i; ++j)
			{
				if (_selectedBuf[j])
				{
					mustLink.push_back(i);
					mustLink.push_back(j);
				}
			}

			for (int j=0; j<_nClusters; ++j)
			{
				if (j==clusterIdx)
				{
					mustLink.push_back(i);
					mustLink.push_back(repFibers[j]);
				} else
				{
					cannotLink.push_back(i);
					cannotLink.push_back(repFibers[j]);
				}
			}
		}
	}
}