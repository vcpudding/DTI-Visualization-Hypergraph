#include "CylindProj.h"
#include <QMessageBox>

float brainCenter[] = {89.5, 100, 86};

CylindPoint cylindProjection(const FloatPoint &pt)
{
	CylindPoint retPt;
	FloatPoint brainCenterPt;
	brainCenterPt <<brainCenter[0] <<brainCenter[1] <<brainCenter[2];
	FloatPoint ptVec = pt-brainCenterPt;
	//ptVec /= sqrt(ptVec(0)*ptVec(0)+ptVec(1)*ptVec(1));
	const float PI = 3.1415926;
	float angle = atan2(ptVec(1), ptVec(0)) + PI/2;
	//angle = angle<0?angle+2*PI:angle;
	angle = angle>PI?angle-2*PI:angle;
	retPt(0) = angle;
	retPt(1) = ptVec(2) / 109*PI;
	return retPt;
}

CylindPoint clusterCenter(FiberData *fiberData, int clusterIndex)
{
	CylindPoint retPt;
	retPt.fill(0);

	FloatPoint brainCenterPt;
	brainCenterPt <<brainCenter[0] <<brainCenter[1] <<brainCenter[2];

	unsigned clusterPtCount = 0;
	float avrAngle = 0;

	char outFileName[512];
	sprintf_s(outFileName, 512, "test data/tract/atlas/tract_embs_%02d.txt", clusterIndex);
	vector<float> angleBuf;
	ofstream outFile (outFileName);
	for (int i=0; i<fiberData->_fibers.size(); ++i)
	{
		if (fiberData->_clusters[i]!=clusterIndex)
		{
			continue;
		}
		for (int j=1; j<=fiberData->_fibers[i][0](0); ++j)
		{
			CylindPoint projPt = cylindProjection(fiberData->_fibers[i][j]);
			outFile <<projPt(0) <<"\t" <<projPt(1) <<endl;
			float * ptMem = fiberData->_fibers[i][j].memptr();
			float angle = projPt(0);
			avrAngle += projPt(0);
			//angleBuf.push_back(angle);
			++clusterPtCount;
		}
	}
	if (clusterPtCount==0)
	{
		return retPt;
	}

	avrAngle /= clusterPtCount;
	/*sort(angleBuf.begin(), angleBuf.end());
	avrAngle = angleBuf[clusterPtCount/2];*/

	unsigned avrPtCount = 0;
	float avrZ = 0;
	for (int i=0; i<fiberData->_fibers.size(); ++i)
	{
		if (fiberData->_clusters[i]!=clusterIndex)
		{
			continue;
		}
		for (int j=1; j<=fiberData->_fibers[i][0](0); ++j)
		{
			CylindPoint projPt = cylindProjection(fiberData->_fibers[i][j]);
			if (fabs(projPt(0)-avrAngle)<=0.5)
			{
				avrZ += projPt(1);
				++avrPtCount;
			}
		}
	}
	if (avrPtCount==0)
	{
		return retPt;
	}
	avrZ /= avrPtCount;

	retPt(0) = avrAngle;
	retPt(1) = avrZ;
	outFile <<retPt(0) <<"\t" <<retPt(1) <<endl;
	outFile.close();
	return retPt;
}

void embedTemplateClusters()
{
	/*FiberData * fiberData = new FiberData();
	fiberData->loadFibers("test data/tract/atlas/o_template_tract.fib", 144,144,60);
	vector<int> clusterBuf = fiberData->loadClusters("test data/tract/atlas/o_template_tract.cluster");
	QString clusterEmbName = "test data/tract/atlas/o_template_tract.cemb";
	QString embName = "test data/tract/atlas/o_template_tract.emb";

	ofstream clusterEmbFile (clusterEmbName.toStdString().c_str());
	ofstream embFile (embName.toStdString().c_str(), ios::binary);
	vector<CylindPoint> clusterEmbBuf (0);

	for (int i=0; i<clusterBuf.size(); ++i)
	{
		CylindPoint projPt = clusterCenter(fiberData, clusterBuf[i]);
		clusterEmbBuf.push_back(projPt);
	}

	zAxisNonUniformScaling(clusterEmbBuf, clusterBuf);

	for (int i=0; i<clusterEmbBuf.size(); ++i)
	{
		clusterEmbFile <<clusterBuf[i] <<"\t" <<clusterEmbBuf[i](0) <<"\t" <<clusterEmbBuf[i](1) <<"\n";
	}

	for (int i=0; i<fiberData->nFibers(); ++i)
	{
		int clusterIdx = fiberData->_clusters[i];
		int embIdx = 0;
		for (; embIdx<clusterEmbBuf.size(); ++embIdx)
		{
			if (clusterIdx == clusterBuf[embIdx])
			{
				break;
			}
		}

		float minDist = 0;
		CylindPoint projPt = clusterEmbBuf[embIdx];
		embFile.write((char*)(projPt.memptr()), 2*sizeof(float));
		embFile.write((char*)(projPt.memptr()), 2*sizeof(float));
		embFile.write((char*)(&minDist), sizeof(float));
	}


	clusterEmbFile.close();
	embFile.close();
	delete fiberData;
	QMessageBox::information(NULL, "Cylindrical Embedding", "Done!");*/
}

void zAxisNonUniformScaling (vector<CylindPoint> &embBuf)
{
	const float PI = 3.1415926;
	float delta = 0.5;
	for (int i=0; i<embBuf.size(); ++i)
	{
		for (int j=0; j<i; ++j)
		{
			float d = (embBuf[i](0)-embBuf[j](0))*(embBuf[i](0)-embBuf[j](0))*PI*PI+
				(embBuf[i](1)-embBuf[j](1))*(embBuf[i](1)-embBuf[j](1));
			if (d<0.1)
			{
				float z;
				int idx1, idx2;
				if (embBuf[i](1)>0 && embBuf[j](1)>0)
				{
					z = min(embBuf[i](1), embBuf[j](1));
					idx1 = embBuf[i](1)<embBuf[j](1)?i:j;
					idx2 = embBuf[i](1)<embBuf[j](1)?j:i;
				} else if (embBuf[i](1)<0 && embBuf[j](1)<0)
				{
					z = max(embBuf[i](1), embBuf[j](1));
					idx1 = embBuf[i](1)<embBuf[j](1)?j:i;
					idx2 = embBuf[i](1)<embBuf[j](1)?i:j;
				} else
				{
					z = (embBuf[i](1)+embBuf[j](1))/2;
					idx1 = embBuf[i](1)<embBuf[j](1)?i:j;
					idx2 = embBuf[i](1)<embBuf[j](1)?j:i;
				}
				for (int k=0; k<embBuf.size(); ++k)
				{
					if (fabs(embBuf[k](1)-z)<1.0e-3)
					{
						continue;
					}

					if (z>0)
					{
						embBuf[k](1) += embBuf[k](1)>z?delta:0;
					} else if (z<0)
					{
						embBuf[k](1) -= embBuf[k](1)<z?delta:0;
					} else
					{
						embBuf[k](1) += embBuf[k](1)>0?delta/2:(embBuf[k](1)<z?-delta/2:0);
					}
				}
			}
		}
	}
}


void zAxisNonUniformScaling (vector<CylindPoint> &embBuf, vector<int>&clusterBuf)
{
	const float PI = 3.1415926;
	float delta = 0.2;
	for (int i=0; i<embBuf.size(); ++i)
	{
		for (int j=0; j<i; ++j)
		{
			float d = (embBuf[i](0)-embBuf[j](0))*(embBuf[i](0)-embBuf[j](0))*PI*PI+
				(embBuf[i](1)-embBuf[j](1))*(embBuf[i](1)-embBuf[j](1));
			if (d<0.5)
			{
				//expand the area
				float z;
				float deltaI = 0, deltaJ = 0;

				if (embBuf[i](1)>0 && embBuf[j](1)>0)
				{
					z = min(embBuf[i](1), embBuf[j](1));
					if (embBuf[i](1)<=embBuf[j](1))
					{
						//embBuf[j](1) += delta;
						deltaJ = delta;
					}
					else 
					{
						//embBuf[i](1) += delta;
						deltaI = delta;
					}
				} else if (embBuf[i](1)<0 && embBuf[j](1)<0)
				{
					z = max(embBuf[i](1), embBuf[j](1));
					if (embBuf[i](1)>=embBuf[j](1))
					{
						//embBuf[j](1) -= delta;
						deltaJ = -delta;
					}
					else 
					{
						//embBuf[i](1) -= delta;
						deltaI = -delta;
					}
				} else
				{
					z = 0;//(embBuf[i](1)+embBuf[j](1))/2;
					if (embBuf[i](1)>=0)
					{
						/*embBuf[i](1) += delta/2;
						embBuf[j](1) -= delta/2;*/
						deltaI = delta/2;
						deltaJ = -delta/2;
					}
					else 
					{
						/*embBuf[i](1) -= delta/2;
						embBuf[j](1) += delta/2;*/
						deltaI = -delta/2;
						deltaJ = delta/2;
					}
				}

				for (int k=0; k<embBuf.size(); ++k)
				{
					if (clusterBuf[k]%100 == clusterBuf[i]%100)
					{
						embBuf[k](1) += deltaI;
						continue;
					} else if (clusterBuf[k]%100 == clusterBuf[j]%100)
					{
						embBuf[k](1) += deltaJ;
						continue;
					}

					if (z>0)
					{
						embBuf[k](1) += embBuf[k](1)>z?delta:0;
					} else if (z<0)
					{
						embBuf[k](1) -= embBuf[k](1)<z?delta:0;
					} else
					{
						embBuf[k](1) += embBuf[k](1)>0?delta:(embBuf[k](1)<z?-delta:0);
					}
				}
			}
		}
	}
}

float zAxisDensity(vector<CylindPoint> &embBuf, float z)
{
	float tolerance = 0.5;
	vector<CylindPoint> ptBuf;
	const float PI = 3.1415926;
	for (int i=0; i<embBuf.size(); ++i)
	{
		if (fabs(embBuf[i](1)-z)<=tolerance)
		{
			ptBuf.push_back(embBuf[i]);
		}
	}

	float minDist = 1.0e5;
	for (int i=0; i<ptBuf.size(); ++i)
	{
		for (int j=0; j<i; ++j)
		{
			float d = (ptBuf[i](0)-ptBuf[j](0))*(ptBuf[i](0)-ptBuf[j](0))*PI*PI+
				(ptBuf[i](1)-ptBuf[j](1))*(ptBuf[i](1)-ptBuf[j](1));
			minDist = d<minDist?d:minDist;
		}
	}
	return 1.0/(sqrt(minDist)+1.0e-3);
}