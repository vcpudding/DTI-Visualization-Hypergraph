#include "TemplateFiberData.h"
#include "glwidget.h"
#include <gl/glut.h>
#include <gl/gle.h>
#include "similarity/cuda_main.h"

#include <QDir>
#include <QMessageBox>
#include <QProgressDialog>

TemplateFiberData * TemplateFiberData::templateData = new TemplateFiberData();

TemplateFiberData::TemplateFiberData(void)
{
	_bShowLabels = false;
	_totalPtCount = 0;
	_clusterColorTable = new ColorTable("Resources/seed_spectrum.bmp");
	_colorMode = COLOR_SEED;
}

TemplateFiberData::~TemplateFiberData(void)
{
}

void TemplateFiberData::loadClusterEmbeddings(const QString &fileName)
{
	requireData(false, true, false, false);
	vector<CylindPoint> cylindBuf (_clusterBuf.size());
	ifstream embedFile (fileName.toStdString().c_str());
	int cluster;
	float angle, z;
	while(embedFile >>cluster >>angle >>z)
	{
		int i=0;
		for (;i<_clusterBuf.size(); ++i)
		{
			if (cluster==_clusterBuf[i])
			{
				cylindBuf[i] <<angle <<z;
				break;
			}
		}
	}

	_clusterEmbeds = cylindBuf;
}

void TemplateFiberData::loadClusterLabels(const QString &fileName)
{
	ifstream labelFile(fileName.toStdString().c_str());
	QStringList labelList;
	if (!labelFile)
	{
		return;
	}
	char line [256];
	while(labelFile.getline(line, 256))
	{
		labelList <<QString(line);
	}
	labelFile.close();


	_clusterLabels = labelList;
}

void TemplateFiberData::drawAtlasEmbeds(float cylindRadius, bool bShowLabels, bool bColorTpl)
{
	if (!templateData)
	{
		templateData = new TemplateFiberData();
	}

	templateData->requireData(false, false, true, true);

	glPointSize(5);
	glColor3f(1,1,1);
	for (int i=0; i<templateData->_clusterEmbeds.size(); ++i)
	{
		float x = cylindRadius*templateData->_clusterEmbeds[i](0);
		float y = cylindRadius*templateData->_clusterEmbeds[i](1);
		glPushMatrix();
		glTranslatef(x,y,0);
		if (bColorTpl)
		{
			glColor3ubv(templateData->getTplColor(templateData->_clusterBuf[i]));
		} else
		{
			glColor3f(0.8, 0.8, 0.8);
		}
		//glColor3f(1,1,1);
		glutSolidSphere(0.1, 20, 20);
		glPopMatrix();

		//glColor3f(0.5,0.5,0.5);
		if (bShowLabels)
		{
			int labelIndex = templateData->_clusterBuf[i]%100;
			QString label = templateData->_clusterLabels[labelIndex];
			GLWidget::drawString(label.toStdString().c_str(), x+0.03*templateData->_clusterLabels[labelIndex].length(), y+0.15, 12);
		}
	}
}

float TemplateFiberData::dist(const FloatPoint *f1, const FloatPoint *f2)
{
	int ptCount1 = f1[0](0), ptCount2 = f2[0](0);
	/*float *tmpFib1 = new float[ptCount1*3];
	float *tmpFib2 = new float[ptCount2*3];
	for (int i=0; i<ptCount1; ++i)
	{
		memcpy(tmpFib1+3*i, f1[i+1].memptr(), 3*sizeof(float));
	}
	for (int i=0; i<ptCount2; ++i)
	{
		memcpy(tmpFib2+3*i, f2[i+1].memptr(), 3*sizeof(float));
	}
	float result1=fiber_similarity(tmpFib1,tmpFib2,ptCount1,ptCount2);
	delete []tmpFib1;
	delete []tmpFib2;
	return result1;*/
 	vector<float> minPt2FibDist (ptCount1);
 	vector<float> pt2FibDist (ptCount2);
 	for (int i=1; i<=ptCount1; ++i)
 	{
 		for (int j=1; j<=ptCount2; ++j)
 		{
 			pt2FibDist[j-1] = dot(f1[i]-f2[j], f1[i]-f2[j]);
 			/*pt2FibDist[j-1] = 0;
 			for (int k=0; k<3; ++k)
 			{
 				pt2FibDist[j-1] += (floor(f1[i](k))-floor(f2[j](k)))*(floor(f1[i](k))-floor(f2[j](k)));
 			}*/
 		}
 		minPt2FibDist[i-1] = *min_element(pt2FibDist.begin(), pt2FibDist.end());
 	}
 	float maxOfMinDist = *max_element(minPt2FibDist.begin(), minPt2FibDist.end());
 	float result2=sqrt(maxOfMinDist);
 	/*if (result1==result2)
 	{
 		printf("C");
 	}*/
 	return result2;
}

float TemplateFiberData::symDist(const FloatPoint *f1, const FloatPoint *f2)
{
	int ptCount1 = f1[0](0), ptCount2 = f2[0](0);
	fmat distBuf(ptCount1, ptCount2);
	distBuf.fill(1000);
	for (int i=1; i<=ptCount1; ++i)
	{
		for (int j=1; j<=ptCount2; ++j)
		{
			FloatPoint v = f1[i]-f2[j];
			float d = v(0)*v(0)+v(1)*v(1);
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
	return (sqrt(d1)+sqrt(d2))/2;
}

vector<float> TemplateFiberData::getAtlasNeighbors(const FloatPoint *targetFib, int numOfNeighbors, float &minBundleDist)
{
	templateData->requireData(true, true, true, false);

	int nFibers = templateData->_fibers.size();
	int nClusters = templateData->_clusterBuf.size();
	fvec distBuf (nFibers);
	fvec bundleDistBuf (nClusters);
	bundleDistBuf.fill(1.0e5);
	for (int i=0; i<nFibers; ++i)
	{
		//distBuf(i) = symDist(templateData->fiber(i), targetFib);
		float d = dist(targetFib, templateData->_fibers[i]); //non-symmetric distance instead
		distBuf(i) = d;
		int clusterIdx = templateData->_clusters[i];
		for (int j=0; j<nClusters; ++j)
		{
			if (templateData->_clusterBuf[j]==clusterIdx)
			{
				bundleDistBuf(j) = d<bundleDistBuf(j)?d:bundleDistBuf(j);
				break;
			}
		}
	}
	uvec sortedIndices = sort_index(distBuf);
	minBundleDist = distBuf(sortedIndices(0));
	distBuf.set_size(0);

	vector<float> weights (nClusters);
	float sumWeights = 0;
	for (int i=0; i<nClusters; ++i)
	{
		weights[i] = 0;
		for (int j=0; j<numOfNeighbors && j<nFibers; ++j)
		{
			int index = sortedIndices(j);
			int cluster = templateData->_clusters[index];
			if (cluster==templateData->_clusterBuf[i])
			{
				++weights[i];
			}
		}
		weights[i] /= bundleDistBuf[i]+1.0e-5;
		sumWeights += weights[i];
	}

	for (int i=0; i<nClusters; ++i)
	{
		weights[i] /= sumWeights;
	}

	return weights;
}

EmbedPoint TemplateFiberData::getEmbedding(const FloatPoint *targetFib)
{
	EmbedPoint embedPt;
	float minDist = 0;
	CylindPoint posLLE = lleEmbedding(targetFib, minDist);
	CylindPoint posProj = cylindEmbedding(targetFib);
	embedPt.posLLE = posLLE;
	embedPt.minDist = minDist;
	embedPt.posProj = posProj;
	return embedPt;
}

CylindPoint TemplateFiberData::lleEmbedding(const FloatPoint *targetFib, float &minBundleDist)
{
	if (!templateData)
	{
		templateData = new TemplateFiberData();
	}

	vector<float> weights = getAtlasNeighbors(targetFib, 20, minBundleDist);
	CylindPoint embedPt;
	embedPt.fill(0);
	for (int i=0; i<weights.size(); ++i)
	{
		embedPt += weights[i]*templateData->_clusterEmbeds[i];
	}

	return embedPt;
}

CylindPoint TemplateFiberData::cylindEmbedding(const FloatPoint *targetFib)
{
	int nPoint = targetFib[0](0);

	CylindPoint retPt;
	retPt.fill(0);
	float avrAngle = 0;

	for (int j=1; j<=nPoint; ++j)
	{
		CylindPoint projPt = cylindProjection(targetFib[j]);
		float angle = projPt(0);
		avrAngle += projPt(0);
	}
	avrAngle /= nPoint;

	unsigned avrPtCount = 0;
	float avrZ = 0;
	for (int j=1; j<=targetFib[0](0); ++j)
	{
		CylindPoint projPt = cylindProjection(targetFib[j]);
		if (fabs(projPt(0)-avrAngle)<=0.5)
		{
			avrZ += projPt(1);
			++avrPtCount;
		}
	}
	if (avrPtCount==0)
	{
		return retPt;
	}
	avrZ /= avrPtCount;

	retPt(0) = avrAngle;
	retPt(1) = avrZ;
	return retPt;
}

float TemplateFiberData::getTplColorIndex(int clusterIndex)
{
	requireData(false, true, false, false);

	int i=0;
	for (;i<_clusterBuf.size(); ++i)
	{
		if (_clusterBuf[i]==clusterIndex%100)
		{
			return i*1.0/13;
		}
	}

	return -1;
}

uchar * TemplateFiberData::getTplColor(int clusterIndex)
{
	float index = templateData->getTplColorIndex(clusterIndex);
	return templateData->_clusterColorTable->getColor(index);
}

CylindPoint TemplateFiberData::clusterEmb(int clusterIdx, int left)
{
	templateData->requireData(false, true, true, false);

	if (left)
	{
		clusterIdx+=100;
	}

	for (int i=0; i<templateData->_clusterBuf.size(); ++i)
	{
		if (templateData->_clusterBuf[i] == clusterIdx)
		{
			return templateData->_clusterEmbeds[i];
		}
	}
	CylindPoint tmpEmb;
	tmpEmb.fill(0);
	return tmpEmb;
}

QStringList TemplateFiberData::clusterLabels()
{
	templateData->requireData(false, false, false, true);
	return templateData->_clusterLabels.mid(1);
}

void TemplateFiberData::requireData(bool bFibers, bool bClusters, bool bEmbs, bool bLabels)
{
	if (bFibers && _fibers.empty())
	{
		_totalPtCount = loadFibers("test data/tract/atlas/o_template_tract.fib", 181, 217, 181);
	}

	if (bClusters && _clusterBuf.empty())
	{
		//_clusterBuf = loadClusters("test data/tract/atlas/o_template_tract.cluster");
	}

	if (bEmbs && _clusterEmbeds.empty())
	{
		loadClusterEmbeddings("test data/tract/atlas/o_template_tract.cemb");
	}

	if (bLabels && _clusterLabels.isEmpty())
	{
		loadClusterLabels("major_bundles.txt");
	}
}

//vector<float> TemplateFiberData::gpuFiberDist(const FloatPoint *targetFib)
//{
//}

float *TemplateFiberData::getAtlasDistMatrixGPU(FiberData *fibData)
{ 
	templateData->requireData(true, false, false, false);
	int nTargetFibs = fibData->nFibers();
	int nAtlasFibers = templateData->nFibers();

	float * targetToAtlasDist = getDistMatrixGPU(fibData, templateData);
	/*float * atlasToTagetDist = getDistMatrixGPU(templateData, fibData);
	
	for (int i=0; i<nTargetFibs; ++i)
	{
		for (int j=0; j<nAtlasFibers; ++j)
		{
			float d1 = targetToAtlasDist[i*nAtlasFibers+j];
			float d2 = atlasToTagetDist[j*nTargetFibs+i];
			targetToAtlasDist[i*nAtlasFibers+j] = min(d1, d2);
		}
	}

	delete [] atlasToTagetDist;*/
	return targetToAtlasDist;
}

CylindPoint TemplateFiberData::lleEmbedding(float *distVec, int numOfNeighbors, float &minBundleDist)
{
	vector<float> weights = getAtlasWeights(distVec, numOfNeighbors, minBundleDist);

	CylindPoint embedPt;
	embedPt.fill(0);
	for (int i=0; i<weights.size(); ++i)
	{
		embedPt += weights[i]*templateData->_clusterEmbeds[i];
	}

	return embedPt;
}

vector<float> TemplateFiberData::getAtlasWeights(float *distVec, int numOfNeighbors, float &minBundleDist)
{
	templateData->requireData(true, true, true, false);

	int nAtlasFibers = templateData->_fibers.size();
	int nAtlasClusters = templateData->_clusterBuf.size();
	fvec distBuf (nAtlasFibers);
	fvec bundleDistBuf (nAtlasClusters);
	bundleDistBuf.fill(1.0e5);
	for (int i=0; i<nAtlasFibers; ++i)
	{
		float d = distVec[i];
		distBuf(i) = d;
		int clusterIdx = templateData->_clusters[i];
		for (int j=0; j<nAtlasClusters; ++j)
		{
			if (templateData->_clusterBuf[j]==clusterIdx)
			{
				bundleDistBuf(j) = d<bundleDistBuf(j)?d:bundleDistBuf(j);
				break;
			}
		}
	}
	uvec sortedIndices = sort_index(distBuf);
	minBundleDist = distBuf(sortedIndices(0));
	distBuf.set_size(0);

	vector<float> weights (nAtlasClusters);
	float sumWeights = 0;
	for (int i=0; i<nAtlasClusters; ++i)
	{
		weights[i] = 0;
		for (int j=0; j<numOfNeighbors && j<nAtlasFibers; ++j)
		{
			int index = sortedIndices(j);
			int cluster = templateData->_clusters[index];
			if (cluster==templateData->_clusterBuf[i])
			{
				++weights[i];
			}
		}
		//weights[i] /= bundleDistBuf[i]+1.0e-5;
		//sumWeights += weights[i];
	}
	for (int i=0; i<nAtlasClusters; ++i)
	{
		weights[i] /= numOfNeighbors;
	}

	return weights;
}

EmbedPoint* TemplateFiberData::getEmbeddingsGPU(FiberData *fibData, int numOfNeighbors)
{
	templateData->requireData(true, true, true, false);

	DWORD tStart = GetTickCount();
	int nTargetFibs = fibData->nFibers();
	int nAtlasFibs = templateData->nFibers();
	float *distMatrix = getAtlasDistMatrixGPU(fibData);
	EmbedPoint *embedPts = new EmbedPoint[nTargetFibs];

	DWORD tDist = (GetTickCount()-tStart)/nTargetFibs;

	tStart = GetTickCount();

	QProgressDialog progress ("Computing embedding points...", "", 0, nTargetFibs-1);
	progress.setCancelButton(NULL);
	progress.show();

	for (int i=0; i<nTargetFibs; ++i)
	{
		progress.setValue(i);
		embedPts[i].posLLE = lleEmbedding(distMatrix+nAtlasFibs*i, numOfNeighbors, embedPts[i].minDist);
		embedPts[i].posProj = cylindEmbedding(fibData->fiber(i));
	}
	delete []distMatrix;
	DWORD tEnd = (GetTickCount()-tStart)/nTargetFibs;
	QMessageBox::information(NULL, "Exec time", QString("Distance matrix / tract: %1; Embedding / tract: %2").arg(tDist).arg(tEnd));
	return embedPts;
}



void TemplateFiberData::tractMDS()
{
	requireData(true, false, false, false);
	float *distMatrix = getAtlasDistMatrixGPU(this);
	int nFibers = _nFibers;
	for (int i=0; i<nFibers; ++i)
	{
		for (int j=0; j<i; ++j)
		{
			distMatrix[i*nFibers+j] = (distMatrix[i*nFibers+j]+distMatrix[j*nFibers+i])/2.0;
			distMatrix[j*nFibers+i] = distMatrix[i*nFibers+j];
		}
	}
	float *tractEmbs = new float[nFibers*2];
	MDS(distMatrix, nFibers, 2, tractEmbs, 1.0e-6);

	ofstream outFile ("test data/tract/atlas/tractMDS.txt");
	for (int i=0; i<nFibers; ++i)
	{
		outFile <<tractEmbs[2*i] <<"\t" <<tractEmbs[2*i+1] <<endl;
	}
	outFile.close();

	QMessageBox::information(NULL, "MDS", "Done!");
}

void TemplateFiberData::clusterMDS()
{
	requireData(true, true, false, false);
	int nFibers = _nFibers;
	float *distMatrix = getAtlasDistMatrixGPU(this);
	for (int i=0; i<nFibers; ++i)
	{
		for (int j=0; j<i; ++j)
		{
			distMatrix[i*nFibers+j] = (distMatrix[i*nFibers+j]+distMatrix[j*nFibers+i])/2.0;
			distMatrix[j*nFibers+i] = distMatrix[i*nFibers+j];
		}
	}
	int nClusters = _clusterBuf.size();
	float *clusterDistMatrix = new float[nClusters*nClusters];
	for (int i=0; i<nClusters; ++i)
	{
		for (int j=0; j<i; ++j)
		{
			int count = 0;
			float d = 0;
			for (int k=0; k<nFibers; ++k)
			{
				if (_clusters[k] != _clusterBuf[i])
				{
					continue;
				}
				for (int l=0; l<nFibers; ++l)
				{
					if (_clusters[l] != _clusterBuf[j])
					{
						continue;
					}
					d += distMatrix[k*nFibers+l];
					++count;
				}
			}
			if (count<=0)
			{
				printf("error");
			}
			d/=count;
			clusterDistMatrix[i*nClusters+j] = d;
			clusterDistMatrix[j*nClusters+i] = d;
		}
		clusterDistMatrix[i*nClusters+i] = 0;
	}
	delete [] distMatrix;
	distMatrix = NULL;
	float *clusterEmbs = new float[nClusters*2];
	MDS(clusterDistMatrix, nClusters, 2, clusterEmbs, 1.0e-6);

	ofstream outFile ("test data/tract/atlas/clusterMDS.txt");
	for (int i=0; i<nClusters; ++i)
	{
		outFile <<_clusterBuf[i] <<"\t" <<clusterEmbs[2*i] <<"\t" <<clusterEmbs[2*i+1] <<endl;
	}
	outFile.close();

	QMessageBox::information(NULL, "Cluster MDS", "Done!");
}

void TemplateFiberData::drawAtlasTracts()
{
	templateData->requireData(true, true, false, false);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glTranslatef(-90.5, -90.5, -90.5);
	glLineWidth(2);
	float tmpFibF [6000];
	gleDouble tmpFibD [2000][3];
	for (int i=0; i<templateData->nFibers(); ++i)
	{
		glColor3ubv(getTplColor(templateData->_clusters[i]));
		glBegin(GL_LINE_STRIP);
		for (int j=1; j<=templateData->_fibers[i][0](0); ++j)
		{
			glVertex3fv(templateData->_fibers[i][j].memptr());
		}
		glEnd();
	}
	glPopMatrix();
}

void TemplateFiberData::drawAtlasTracts(vector<int> &highlightClusters)
{
	templateData->requireData(true, true, false, false);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glTranslatef(-90.5, -90.5, -90.5);
	glLineWidth(2);
	float tmpFibF [6000];
	gleDouble tmpFibD [2000][3];
	for (int i=0; i<templateData->nFibers(); ++i)
	{
		int clusterIdx = templateData->_clusters[i];
		bool bHighlight = false;
		for (int j=0; j<highlightClusters.size(); ++j)
		{
			if (clusterIdx%100 == highlightClusters[j]%100)
			{
				bHighlight = true;
				break;
			}
		}
		if (!bHighlight)
		{
			glDisable(GL_LIGHTING);
			glColor3f(0.5,0.5,0.5);
			glBegin(GL_LINE_STRIP);
			for (int j=1; j<=templateData->_fibers[i][0](0); ++j)
			{
				glVertex3fv(templateData->_fibers[i][j].memptr());
			}
			glEnd();
		} else
		{
			glEnable(GL_LIGHTING);
			glColor3ubv(getTplColor(templateData->_clusters[i]));
			int nPoint = templateData->fiber(i, tmpFibF);
			for (int i=0; i<nPoint; ++i)
			{
				tmpFibD[i][0] = tmpFibF[i*3];
				tmpFibD[i][1] = tmpFibF[i*3+1];
				tmpFibD[i][2] = tmpFibF[i*3+2];
			}
			//glePolyCylinder (nPoint,		/* num points in polyline */
			//	tmpFibD,	/* polyline vertces */
			//	NULL,	/* colors at polyline verts */
			//	0.3);
		}
	}
	glPopMatrix();

}