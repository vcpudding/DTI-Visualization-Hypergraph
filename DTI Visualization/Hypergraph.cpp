#include "Hypergraph.h"

#include <fstream>
#include <string>
#include <sstream>
#include <GL/glut.h>
#include <time.h>
#include <Windows.h>
#include <QProgressDialog>

Hypergraph::Hypergraph(void)
{
	_layoutDim = 2;
	_nVertices = 0;
	_edges = vector<HyperEdge> (0);
	_layout = vector<fvec> (0);
	//_colorTable = new ColorTable();
}

Hypergraph::~Hypergraph(void)
{
}

void Hypergraph::setNumOfVertices(int nVertices)
{
	_nVertices = nVertices;
	_selectedBuf.resize(nVertices);
	_selectedBuf.assign(nVertices, false);
	_clusters.resize(_nVertices);
}

void Hypergraph::saveHypergraph(const QString &fileName)
{
	ofstream graphFile (fileName.toStdString().c_str());
	graphFile <<_nVertices<<"\n";
	for (int i=0; i<_edges.size(); ++i)
	{
		for (int j=0; j<_edges[i].size(); ++j)
		{
			graphFile <<_edges[i][j] <<"\t";
		}
		graphFile <<"\n";
	}
	graphFile.close();
}

void Hypergraph::loadHypergraph(const QString &fileName)
{
	ifstream graphFile (fileName.toStdString().c_str());
	graphFile >>_nVertices;
	setNumOfVertices(_nVertices);
	string line;
	_edges.clear();
	while(getline(graphFile,line))
	{
		stringstream strLine(line);
		HyperEdge e(0);
		int fibIdx;
		while (strLine >>fibIdx)
		{
			e.push_back(fibIdx);
		}
		if (!e.empty())
		{
			_edges.push_back(e);
		}
	}
	graphFile.close();
}

void Hypergraph::layoutGraph( int boundary [], int dim/*=2*/, float c/*=1.0*/ )
{
	_layoutDim = dim;
	int nIteration = 500;
	float temperature = 10;

	srand(time(NULL));

	int area = 1;
	for (int i=0; i<dim; ++i)
	{
		area *= boundary[2*i+1]-boundary[2*i];
	}
	float k = c*sqrt(area*1.0f/_nVertices);

	/*initialize vertex layout*/
	int nEdges = _edges.size();
	int nTotal = _nVertices+nEdges;
	_layout.resize(nTotal);
	vector<fvec> dispBuf (nTotal);
	for (int i=0; i<nTotal; ++i)
	{
		_layout[i] = fvec(dim);
		for (int j=0; j<dim; ++j)
		{
			_layout[i](j) = boundary[2*j]+rand()%(boundary[2*j+1]-boundary[2*j]);
		}

		dispBuf[i] = fvec(dim);
	}

	int i,j;

	QProgressDialog prog ("Performing graph layout ...", "", 0, nIteration-1);
	prog.setCancelButton(NULL);
	for (int it=0; it<nIteration; ++it)
	{
		DWORD t1 = GetTickCount();
		bool bChanged = false;
		for (i=0; i<nTotal; ++i)
		{
			dispBuf[i].fill(0);
		}

		DWORD t2 = GetTickCount();
		/*calculate repulsive force*/
		for (i=0; i<nTotal; ++i)
		{
			for (j=0; j<i; ++j)
			{
				if (j==i)
				{
					continue;
				}
				fvec disp = getRepulsiveForce(_layout[i], _layout[j], k);
				dispBuf[j] += disp;
				dispBuf[i] -= disp;
			}
		}

		DWORD tRepForce = GetTickCount()-t2;

		/*calculate attractive force*/
		for (i=0; i<_edges.size(); ++i)
		{
			int edgeIdx = i+_nVertices;
			for (j=0; j<_edges[i].size(); ++j)
			{
				float c2 = 300.0;
				fvec disp1 = c2*getAttractiveForce(_layout[_edges[i][j]], _layout[edgeIdx], k);
				dispBuf[edgeIdx] -= disp1;
				dispBuf[_edges[i][j]] += disp1;
			}
		}
		DWORD tRepAttrForces = GetTickCount()-t2;

		/*restrict the points from being pushed towards the boundary*/
		for (i=0; i<nTotal; ++i)
		{
			for (j=0; j<dim; ++j)
			{
				float c1=1.0e6;
				float x = _layout[i](j);
				if (x==boundary[j*2] || x==boundary[j*2+1])
				{
					int stop = 1;
				}
				float d = c1*(1/(x-boundary[j*2]+1.0e-3)-1/(boundary[j*2+1]-x+1.0e-3));
				dispBuf[i](j) += d;
			}
		}

		/*limit the maximum displacement, boundary check*/
		for (i=0; i<nTotal; ++i)
		{
			float n = norm(dispBuf[i],2);
			fvec newLayout = _layout[i]+dispBuf[i]*min(n, temperature)/n;
			for (j=0; j<dim; ++j)
			{
				int minBoundary = boundary[2*j];
				int maxBoundary = boundary[2*j+1];
				newLayout(j) = newLayout(j) < minBoundary?minBoundary:(newLayout(j)>maxBoundary?maxBoundary:newLayout(j));
			}
			if (norm(newLayout-_layout[i], 2)>1.0e-3)
			{
				_layout[i] = newLayout;
				bChanged = true;
			}
		}

		if (!bChanged)
		{
			break;
		}

		DWORD perItTime = GetTickCount() - t1;
		t1 = GetTickCount();

		prog.setValue(it);
	}
}

void Hypergraph::saveLayout(const QString &fileName)
{
	ofstream layoutFile (fileName.toStdString().c_str(), ios::binary);
	layoutFile.write((char*)&_layoutDim, sizeof(int));
	layoutFile.write((char*)&_nVertices, sizeof(int));
	int nEdges = _edges.size();
	layoutFile.write((char*)&nEdges, sizeof(int));
	for (int i=0; i<nEdges; ++i)
	{
		int edgeSize = _edges[i].size();
		layoutFile.write((char*)&edgeSize, sizeof(int));
		for (int j=0; j<edgeSize; ++j)
		{
			layoutFile.write((char*)&(_edges[i][j]), sizeof(int));
		}
	}
	for(int i=0; i<_nVertices+nEdges; ++i)
	{
		layoutFile.write((char*)_layout[i].memptr(), _layoutDim*sizeof(float));
	}
	layoutFile.close();
}

void Hypergraph::loadLayout(const QString &fileName)
{
	ifstream layoutFile (fileName.toStdString().c_str(), ios::binary);
	layoutFile.read((char*)&_layoutDim, sizeof(int));
	layoutFile.read((char*)&_nVertices, sizeof(int));
	setNumOfVertices(_nVertices);
	int nEdges;
	layoutFile.read((char*)&nEdges, sizeof(int));
	_edges.clear();
	for (int i=0; i<nEdges; ++i)
	{
		int edgeSize;
		layoutFile.read((char*)&edgeSize, sizeof(int));
		HyperEdge e (edgeSize);
		for (int j=0; j<edgeSize; ++j)
		{
			layoutFile.read((char*)&(e[j]), sizeof(int));
		}
		_edges.push_back(e);
	}
	_layout.resize(_nVertices+nEdges);
	for(int i=0; i<_nVertices+nEdges; ++i)
	{
		_layout[i] = fvec(_layoutDim);
		layoutFile.read((char*)_layout[i].memptr(), _layoutDim*sizeof(float));
	}
	layoutFile.close();
}

void Hypergraph::drawHypergraph()
{
	if (_layout.empty())
	{
		return;
	}

	for (int i=0; i<_edges.size(); ++i)
	{
		int edgeIdx = i+_nVertices;
		for (int j=0; j<_edges[i].size(); ++j)
		{
			glBegin(GL_LINES);
			float alpha = _selectedBuf[_edges[i][j]]?1:0.3;
			glColor4f(1,1,1,alpha);
			glVertex2fv(_layout[_edges[i][j]].memptr());
			const uchar *col = _clusterColorer->getColor(i);
			glColor4ub(col[0], col[1], col[2], alpha*255);
			glVertex2fv(_layout[edgeIdx].memptr());
			glEnd();
		}
	}

	for (int i=0; i<_nVertices; ++i)
	{
		int clusterIdx = _clusters[i];
		float alpha = _selectedBuf[i]?1:0.3;
		const uchar *col = _clusterColorer->getColor(clusterIdx);
		glColor4ub(col[0], col[1], col[2], alpha*255);

		glPushMatrix();
		/*label the vertex*/
		glPushName(i);
		glTranslatef(_layout[i](0), _layout[i](1), 0);
		glutSolidSphere(5, 10, 10);
		glPopName();
		glPopMatrix();

		if (_layout[i](0)<0 || _layout[i](1)<0 || _layout[i](0)>1000 || _layout[i](1)>700)
		{
			int stop = 1;
		}
	}

	GLfloat oldLineWidth;
	glGetFloatv(GL_LINE_WIDTH, &oldLineWidth);
	glLineWidth(1);
	for (int i=0; i<_edges.size(); ++i)
	{
		if (_edges[i].empty())
		{
			continue;
		}
		//glColor3ubv(_colorTable->getColor(i*1.0/(_edges.size()-1)));
		glPushMatrix();
		/*label the edge*/
		glPushName(i+_nVertices);
		glTranslatef(_layout[_nVertices+i](0), _layout[_nVertices+i](1), 0);
		glColor3ubv(_clusterColorer->getColor(i));
		glutSolidCube(15);
		glColor4f(1,1,1, 0.3);
		glutWireCube(16);
		glPopName();
		glPopMatrix();
	}

	for (int i=0; i<_linkageBuf.size(); ++i)
	{
		int edgeIdx = _linkageBuf[i];
		glPushMatrix();
		glTranslatef(_layout[edgeIdx](0), _layout[edgeIdx](1), 0);
		/*glLineWidth(7);
		glColor3f(1,1,1);
		glutWireCube(22);*/
		glLineWidth(3);
		glColor3ubv(_clusterColorer->getColor(edgeIdx-_nVertices));
		glutWireCube(22);
		glPopMatrix();
	}

	glLineWidth(oldLineWidth);
}

bool * Hypergraph::selectVertices( const vector<int> &selectedIdx, int mode )
{
	bool *selectionBuf = new bool [_nVertices];
	memset(selectionBuf, false, sizeof(bool)*_nVertices);
	for (int i=0; i<selectedIdx.size(); ++i)
	{
		int idx = selectedIdx[i];
		if (idx<_nVertices)
		{
			selectionBuf[idx] = true;
		} else if (mode == 0) /*click mode*/
		{
			int edgeIdx = idx-_nVertices;
			for (int j=0; j<_edges[edgeIdx].size(); ++j)
			{
				selectionBuf[_edges[edgeIdx][j]] = true;
			}
		}
	}
	return selectionBuf;
}
void Hypergraph::setSelectionBuf(const vector<bool> selectionBuf)
{
	//memcpy(_selectedBuf, selectionBuf, _nVertices*sizeof(bool));
	copy(selectionBuf.begin(), selectionBuf.end(), _selectedBuf.begin());
}

void Hypergraph::addLinkageEdge( const vector<int> idxBuf )
{
	for (int i=0; i<idxBuf.size(); ++i)
	{
		if (idxBuf[i]>=_nVertices)
		{
			_linkageBuf.push_back(idxBuf[i]);
		}
	}
}

void Hypergraph::setLinkageEdge( const vector<int> idxBuf )
{
	_linkageBuf.clear();
	for (int i=0; i<idxBuf.size(); ++i)
	{
		if (idxBuf[i]>=_nVertices)
		{
			_linkageBuf.push_back(idxBuf[i]);
		}
	}
}

bool * Hypergraph::selectLinkage()
{
	vector<int> result (_nVertices);
	vector<int>::iterator resultPos = result.begin();
	for (int i=0; i<_linkageBuf.size(); ++i)
	{
		const HyperEdge & e = _edges[_linkageBuf[i]-_nVertices];
		if (i==0)
		{
			copy(e.begin(), e.end(), result.begin());
			advance(resultPos, e.size());
		} else
		{
			resultPos = set_intersection(e.begin(), e.end(), result.begin(), resultPos, result.begin());
		}
	}
	bool *selectionBuf = new bool [_nVertices];
	memset(selectionBuf, false, _nVertices*sizeof(bool));
	for (vector<int>::iterator it=result.begin(); it!=resultPos; ++it)
	{
		selectionBuf[*it] = true;
	}
	_linkageBuf.clear();

	return selectionBuf;
}

/*
void Hypergraph::resetLinkageBuf()
{
	_linkageBuf.clear();
}*/


void Hypergraph::renewSelection(const bool *selectionBuf, SelectionMode mode)
{
	for (int i=0; i<_nVertices; ++i)
	{
		switch (mode)
		{
		case SEL_NEW:
			_selectedBuf[i] = selectionBuf[i];
			break;
		case SEL_INTERSECT:
			_selectedBuf[i] = selectionBuf[i] & _selectedBuf[i];
			break;
		case SEL_UNION:
			_selectedBuf[i] = selectionBuf[i] | _selectedBuf[i];
			break;
		case SEL_DIFF:
			_selectedBuf[i] = selectionBuf[i] ^ _selectedBuf[i];
			break;
		case SEL_REMOVE: 
			_selectedBuf[i] = (~selectionBuf[i]) & _selectedBuf[i];
			break;
		}
	}
}

bool Hypergraph::assignSelectedToCluster()
{
	if (_linkageBuf.empty())
	{
		return false;
	}
	bool bChanged = false;
	int edgeIdx = _linkageBuf[0];
	int clusterIdx = edgeIdx-_nVertices;
	for (int i=0; i<_nVertices; ++i)
	{
		if (_selectedBuf[i] && _clusters[i]!=clusterIdx)
		{
			_clusters[i] = clusterIdx;
			bChanged = true;
		}
	}
	_linkageBuf.clear();
	return bChanged;
}

void Hypergraph::setClusters(const vector<int> &clusters)
{
	_clusters.resize(clusters.size());
	copy(clusters.begin(), clusters.end(), _clusters.begin());
}

void Hypergraph::findSampleSets(int vertexIdx, int nTotal, float &maxDist, const vector<float> &distMat, vector<int> &VBuf, vector<int> &SBuf, int Vmax, int Smax)
{
	srand(time(NULL));
	SBuf.clear();

	if (vertexIdx == 10)
	{
		int stop = 1;
	}

	/*vector<bool> picked (nTotal);
	picked.assign(nTotal, false);
	for (int i=0; i<VBuf.size(); i+=2)
	{
		picked[VBuf[i]] = true;
	}*/

	for (int i=0; i<nTotal; ++i)
	{
		if (i==vertexIdx /*|| picked[i]*/)
		{
			continue;
		}
		float p = (Smax-SBuf.size()/2)*1.0f/(nTotal-i);
		if ((time(NULL)%1000)/1000.0f <= p)
		{
			/*point picked*/
			float d = i<vertexIdx?distMat[vertexIdx*(vertexIdx-1)/2+i]:distMat[i*(i-1)/2+vertexIdx];
			if (d<maxDist)
			{
				int j=0;
				while(j<VBuf.size() && (VBuf[j+1] <d || (VBuf[j+1]==d && VBuf[j]<i)))
				{
					j+=2;
				}

				if (j==VBuf.size() || VBuf[j+1]>d || VBuf[j] != i)
				{
					vector<int>::iterator it = VBuf.insert(VBuf.begin()+j, d);
					VBuf.insert(it, i);
				}

				if (VBuf.size()>=Vmax*2)
				{
					VBuf.resize(Vmax*2);
					maxDist = VBuf[Vmax*2-1];
				}
			} else
			{
				SBuf.push_back(i);
				SBuf.push_back(d);

				if (SBuf.size()>=Smax*2)
				{
					break;
				}
			}
		}
	}
	if (VBuf.size()>Vmax*2)
	{
		VBuf.resize(Vmax*2);
	}
}

void Hypergraph::findSampleSets(int vertexIdx, float &maxDist, const vector<float> &distMat, list<int> &sampleBuf, int &Vsize, int Vmax, int Smax)
{
	//srand(time(NULL));
	int nTotal = _nVertices + _edges.size();

	int bufMax = Vmax+Smax;
	list<int>::iterator Vpos = sampleBuf.begin();
	advance(Vpos, Vsize*2);
	sampleBuf.erase(Vpos, sampleBuf.end());

	for (int i=0; i<vertexIdx; ++i)
	{
		float p = (bufMax-sampleBuf.size()/2)*1.0f/(vertexIdx-i);
		if ((time(NULL)%1000)/1000.0f <= p)
		{
			float d = distMat[vertexIdx*(vertexIdx-1)/2+i];
			list<int>::iterator insertPos = sampleBuf.begin();
			list<int>::iterator insertPosNext = insertPos;
			if (insertPos!=sampleBuf.end())
			{
				advance(insertPosNext, 1);
			}
			while(insertPos!=sampleBuf.end() && (*insertPosNext<d || *insertPosNext==d && *insertPos<i))
			{
				advance(insertPos, 2);
				if (insertPos!=sampleBuf.end())
				{
					advance(insertPosNext, 2);
				}
			}


			if (insertPos==sampleBuf.end() || *insertPosNext>d || *insertPos != i)
			{
				list<int>::iterator it = sampleBuf.insert(insertPos, d);
				sampleBuf.insert(it, i);

				if (d<maxDist)
				{
					++Vsize;
					if (Vsize>=Vmax)
					{
						Vsize = Vmax;
						maxDist = *(sampleBuf.rbegin());
					}
				}
			}

			if (sampleBuf.size()>=2*bufMax)
			{
				break;
			}
		}
	}
}

void Hypergraph::layoutFast(int boundary [], int dim, float c, int Vmax, int Smax, int nIteration)
{
	_layoutDim = dim;
	float temperature = 10;

	srand(time(NULL));

	int area = 1;
	for (int i=0; i<dim; ++i)
	{
		area *= boundary[2*i+1]-boundary[2*i];
	}
	float k = c*sqrt(area*1.0f/_nVertices);

	/*initialize vertex layout*/
	int nEdges = _edges.size();
	int nTotal = _nVertices+nEdges;
	_layout.resize(nTotal);
	vector<fvec> dispBuf (nTotal);
	for (int i=0; i<nTotal; ++i)
	{
		_layout[i] = fvec(dim);
		for (int j=0; j<dim; ++j)
		{
			_layout[i](j) = boundary[2*j]+rand()%(boundary[2*j+1]-boundary[2*j]);
		}

		dispBuf[i] = fvec(dim);
	}

	int i,j;

	vector<list<int> > sampleBufs (nTotal);
	vector<int> Vsizes (nTotal);

	vector<float> maxDists (nTotal);
	maxDists.assign(nTotal, 40);

	/*initialize distance matrix*/
	vector<float> distMat (nTotal*(nTotal-1)/2);
	distMat.assign(nTotal*(nTotal-1)/2, 50);
	for (int i=0; i<nEdges; ++i)
	{
		int edgeIdx = i+_nVertices;
		for (int j=0; j<_edges[i].size(); ++j)
		{
			int n1 = _edges[i][j];
			for (int k=0; k<j; ++k)
			{
				int n2 = _edges[i][k];
				distMat[n1*(n1-1)/2+n2] = 20;
			}

			distMat[edgeIdx*(edgeIdx-1)/2+n1] = 10;
		}
	}

	QProgressDialog prog ("Performing graph layout ...", "", 0, nIteration-1);
	prog.setCancelButton(NULL);

	for (int it=0; it<nIteration; ++it)
	{
		DWORD t1 = GetTickCount();
		bool bChanged = false;
		for (i=0; i<nTotal; ++i)
		{
			dispBuf[i].fill(0);
		}

		DWORD t2 = GetTickCount();
		DWORD tFindSample = 0;
		for (i=0; i<nTotal; ++i)
		{
			DWORD t3 = GetTickCount();
			findSampleSets(i, maxDists[i], distMat, sampleBufs[i], Vsizes[i], Vmax, Smax);
			list<int> &sampleBuf = sampleBufs[i];
			tFindSample += GetTickCount()-t3;

			//for (j=0; j<sampleBuf.size(); ++j)
			for (list<int>::iterator sampleIt = sampleBuf.begin(); sampleIt!=sampleBuf.end(); ++sampleIt)
			{
				int sampleIdx = *sampleIt;
				++sampleIt;
				float dist = *sampleIt;
				if (sampleIdx==i)
				{
					continue;
				}
				fvec d = _layout[sampleIdx]-_layout[i];
				float normD = norm(d,2);

				/*calculate repulsive force*/
				fvec dispRepulsive;
				if (normD<1.0e-5)
				{
					dispRepulsive = fvec(dim);
					for (int iDim=0; iDim<dim; ++iDim)
					{
						dispRepulsive(iDim) = rand()%100/100.0f;
					}
					dispRepulsive *= temperature;
				} else
				{
					dispRepulsive = d*k*k/pow(normD,2);
				}

				dispBuf[i] -= dispRepulsive;
				dispBuf[sampleIdx] += dispRepulsive;

				/*calculate attractive force*/
				//float dist = i>sampleIdx?distMat[i*(i-1)/2+sampleIdx]:distMat[sampleIdx*(sampleIdx-1)/2+i];
				/*if (dist<50)
				{
					float c3 = dist<15?3.0:1.0;
					fvec dispAttractive = c3*d*normD/k;
					dispBuf[i] += dispAttractive;
					dispBuf[sampleIdx] -= dispAttractive;
				}*/
			}
		}
		DWORD tRepAttrForces = GetTickCount()-t2;

		/*restrict the points from being pushed towards the boundary*/
		t2 = GetTickCount();
		for (i=0; i<nTotal; ++i)
		{
			for (j=0; j<dim; ++j)
			{
				float c1=1.0e6;
				float x = _layout[i](j);
				if (x==boundary[j*2] || x==boundary[j*2+1])
				{
					int stop = 1;
				}
				float d = c1*(1/(x-boundary[j*2]+1.0e-3)-1/(boundary[j*2+1]-x+1.0e-3));
				dispBuf[i](j) += d;
			}
		}
		DWORD tBoundaries = GetTickCount()-t2;

		/*limit the maximum displacement, boundary check*/
		t2 = GetTickCount();
		for (i=0; i<nTotal; ++i)
		{
			float n = norm(dispBuf[i],2);
			fvec newLayout = _layout[i]+dispBuf[i]*min(n, temperature)/n;
			for (j=0; j<dim; ++j)
			{
				int minBoundary = boundary[2*j];
				int maxBoundary = boundary[2*j+1];
				newLayout(j) = newLayout(j) < minBoundary?minBoundary:(newLayout(j)>maxBoundary?maxBoundary:newLayout(j));
			}
		}
		DWORD tLimits = GetTickCount()-t2;

		DWORD perItTime = GetTickCount() - t1;
		t1 = GetTickCount();

		prog.setValue(it);
	}
}

vector<unsigned> Hypergraph::computeGraphDistMat()
{
	int nTotal = _nVertices+_edges.size();
	vector<unsigned> pathBuf (nTotal*(nTotal-1)/2);
	pathBuf.assign(nTotal*(nTotal-1)/2, 100);

	/*initialization*/
	for (int i=0; i<_edges.size(); ++i)
	{
		int edgeIdx = _nVertices+i;
		for (int j=0; j<_edges[i].size(); ++j)
		{
			for (int k=0; k<j; ++k)
			{
				int idx = _edges[i][j]*(_edges[i][j]-1)/2+_edges[i][k];
				pathBuf[idx] = 2;
			}
			pathBuf[edgeIdx*(edgeIdx-1)/2+_edges[i][j]] = 1;
		}
	}

	for (int k=0; k<nTotal; ++k)
	{
		for (int i=0; i<nTotal; ++i)
		{
			if (i==k)
			{
				continue;
			}
			for (int j=0; j<i; ++j)
			{
				if (j==k)
				{
					continue;
				}
				int idxIJ = i*(i-1)/2+j;
				int idxIK = i>k?i*(i-1)/2+k:k*(k-1)/2+i;
				int idxJK = j>k?j*(j-1)/2+k:k*(k-1)/2+j;

				pathBuf[idxIJ] = min(pathBuf[idxIJ], pathBuf[idxIK]+pathBuf[idxJK]);
			}
		}
	}

	return pathBuf;
}


vector<unsigned> Hypergraph::computeEdgeDistMat()
{
	int nEdges = _edges.size();
	vector<unsigned> pathBuf (nEdges*(nEdges-1)/2);
	pathBuf.assign(nEdges*(nEdges-1)/2, UINT_MAX);

	/*initialization*/
	for (int i=0; i<nEdges; ++i)
	{
		for (int j=0; j<i; ++j)
		{
			HyperEdge v(_edges[i].size()+_edges[j].size());
			HyperEdge::iterator vIt = set_intersection(_edges[i].begin(), _edges[i].begin(), _edges[j].begin(), _edges[j].end(), v.begin());
			if (vIt!=v.begin())
			{
				pathBuf[i*(i-1)/2+j] = 2;
			}
		}
	}

	for (int k=0; k<nEdges; ++k)
	{
		for (int i=0; i<nEdges; ++i)
		{
			if (i==k)
			{
				continue;
			}
			for (int j=0; j<i; ++j)
			{
				if (j==k)
				{
					continue;
				}
				int idxIJ = i*(i-1)/2+j;
				int idxIK = i>k?i*(i-1)/2+k:k*(k-1)/2+i;
				int idxJK = j>k?j*(j-1)/2+k:k*(k-1)/2+j;

				pathBuf[idxIJ] = min(pathBuf[idxIJ], pathBuf[idxIK]+pathBuf[idxJK]);
			}
		}
	}

	return pathBuf;
}

vector<int> Hypergraph::getGraphDifference(const Hypergraph *newGraph)
{
	const vector<HyperEdge> &newEdges = newGraph->_edges;
	vector<int> diffSet (0);
	int nEdges = _edges.size();
	int nNewEdges = newEdges.size();
	int i=0;
	vector<int>::iterator it = diffSet.begin();
	for (; i<min(nEdges, nNewEdges); ++i)
	{
		vector<int> v(_edges[i].size()+newEdges[i].size());
		vector<int>::iterator vIt = set_symmetric_difference(_edges[i].begin(), _edges[i].end(), newEdges[i].begin(), newEdges[i].end(), v.begin());
		int originalSize = diffSet.size();
		diffSet.resize(vIt-v.begin()+diffSet.size());
		copy(v.begin(), vIt, diffSet.begin()+originalSize);
	}

	for (;i<nEdges; ++i)
	{
		int originalSize = diffSet.size();
		diffSet.resize(diffSet.size()+_edges[i].size());
		copy(_edges[i].begin(), _edges[i].end(), diffSet.begin()+originalSize);
	}

	for (;i<nNewEdges; ++i)
	{
		int originalSize = diffSet.size();
		diffSet.resize(diffSet.size()+newEdges[i].size());
		copy(_edges[i].begin(), _edges[i].end(), diffSet.begin()+originalSize);
	}

	sort(diffSet.begin(), diffSet.end());
	it = unique(diffSet.begin(), diffSet.end());
	diffSet.resize(it-diffSet.begin());
	return diffSet;
}

void Hypergraph::updateLayout(const vector<int> &changedSet, int boundary [], int dim, float c)
{
	_layoutDim = dim;
	int nIteration = 500;
	float temperature = 10;

	srand(time(NULL));

	int area = 1;
	for (int i=0; i<dim; ++i)
	{
		area *= boundary[2*i+1]-boundary[2*i];
	}
	float k = c*sqrt(area*1.0f/_nVertices);

	int nEdges = _edges.size();
	int nTotal = _nVertices+nEdges;
	vector<fvec> dispBuf (nTotal);
	for (int i=0; i<nTotal; ++i)
	{
		dispBuf[i] = fvec(dim);
	}
	int i,j;

	vector<bool> bMovable (nTotal);
	for (i=0; i<changedSet.size(); ++i)
	{
		bMovable[changedSet[i]] = true;
	}

	QProgressDialog prog ("Performing graph layout ...", "", 0, nIteration-1);
	prog.setCancelButton(NULL);
	for (int it=0; it<nIteration; ++it)
	{
		DWORD t1 = GetTickCount();
		bool bChanged = false;
		for (i=0; i<nTotal; ++i)
		{
			dispBuf[i].fill(0);
		}

		DWORD t2 = GetTickCount();
		/*calculate repulsive force*/
		for (i=0; i<changedSet.size(); ++i)
		{
			int idx = changedSet[i];
			for (j=0; j<nTotal; ++j)
			{
				if (j==idx)
				{
					continue;
				}
				fvec d = _layout[j]-_layout[idx];
				fvec disp;
				float normD = norm(d,2);
				if (normD<1.0e-5)
				{
					disp = fvec(dim);
					for (int iDim=0; iDim<dim; ++iDim)
					{
						disp(iDim) = rand()%100/100.0f;
					}
					disp *= temperature;
				} else
				{
					disp = d*k*k/pow(normD,2);
				}
				dispBuf[idx] -= disp;
			}
		}

		DWORD tRepForce = GetTickCount()-t2;

		/*calculate attractive force*/
		for (i=0; i<_edges.size(); ++i)
		{
			int edgeIdx = i+_nVertices;
			for (j=0; j<_edges[i].size(); ++j)
			{
				if (!bMovable[_edges[i][j]])
				{
					continue;
				}
				float c2 = 3.0;
				fvec d1 = _layout[edgeIdx] - _layout[_edges[i][j]];
				fvec disp1 = c2*d1*norm(d1,2)/k;
				dispBuf[edgeIdx] -= disp1;
				dispBuf[_edges[i][j]] += disp1;

				for (int m=0; m<_edges[i].size(); ++m)
				{
					if (m==j)
					{
						continue;
					}
					fvec d = _layout[_edges[i][m]] - _layout[_edges[i][j]];
					fvec disp = d*norm(d,2)/k;
					dispBuf[_edges[i][j]] += disp;
				}
			}
		}
		DWORD tRepAttrForces = GetTickCount()-t2;

		/*restrict the points from being pushed towards the boundary*/
		for (i=0; i<nTotal; ++i)
		{
			if (!bMovable[i])
			{
				continue;
			}
			for (j=0; j<dim; ++j)
			{
				float c1=1.0e6;
				float x = _layout[i](j);
				if (x==boundary[j*2] || x==boundary[j*2+1])
				{
					int stop = 1;
				}
				float d = c1*(1/(x-boundary[j*2]+1.0e-3)-1/(boundary[j*2+1]-x+1.0e-3));
				dispBuf[i](j) += d;
			}
		}

		/*limit the maximum displacement, boundary check*/
		for (i=0; i<nTotal; ++i)
		{
			if (!bMovable[i])
			{
				continue;
			}
			float n = norm(dispBuf[i],2);
			fvec newLayout = _layout[i]+dispBuf[i]*min(n, temperature)/n;
			for (j=0; j<dim; ++j)
			{
				int minBoundary = boundary[2*j];
				int maxBoundary = boundary[2*j+1];
				newLayout(j) = newLayout(j) < minBoundary?minBoundary:(newLayout(j)>maxBoundary?maxBoundary:newLayout(j));
			}
			if (norm(newLayout-_layout[i], 2)>1.0e-3)
			{
				_layout[i] = newLayout;
				bChanged = true;
			}
		}

		if (!bChanged)
		{
			break;
		}

		DWORD perItTime = GetTickCount() - t1;
		t1 = GetTickCount();

		prog.setValue(it);
	}
}

void Hypergraph::setEdges(const Hypergraph *newGraph)
{
	_edges.clear();
	_edges = newGraph->_edges;
}

void Hypergraph::layoutClustered(int boundary [], int nIteration, int dim/* =2 */, float c/* =1.0 */)
{
	_layoutDim = dim;
	float temperature = 10;
	int nEdges = _edges.size();
	int nTotal = _nVertices+nEdges;

	srand(time(NULL));

	int area = 1;
	for (int i=0; i<dim; ++i)
	{
		area *= boundary[2*i+1]-boundary[2*i];
	}
	float k = c*sqrt(area*1.0f/nTotal);

	/*initialize vertex layout*/
	_layout.resize(nTotal);
	vector<fvec> dispBuf (nTotal);
	for (int i=0; i<nTotal; ++i)
	{
		_layout[i] = fvec(dim);
		for (int j=0; j<dim; ++j)
		{
			_layout[i](j) = boundary[2*j]+rand()%(boundary[2*j+1]-boundary[2*j]);
		}

		dispBuf[i] = fvec(dim);
	}

	int i,j;

	QProgressDialog prog ("Performing graph layout ...", "", 0, nIteration-1);
	prog.setCancelButton(NULL);
	for (int it=0; it<nIteration; ++it)
	{
		DWORD t1 = GetTickCount();
		bool bChanged = false;
		for (i=0; i<nTotal; ++i)
		{
			dispBuf[i].fill(0);
		}

		DWORD t2 = GetTickCount();
		/*calculate repulsive force*/
		for (i=0; i<nTotal; ++i)
		{
			int clusterI = i<_nVertices?_clusters[i]:i-_nVertices;
			for (j=0; j<i; ++j)
			{
				int clusterJ = j<_nVertices?_clusters[j]:j-_nVertices;
				float edgeLengthSqr = 1;
				if (i<_nVertices)
				{
					edgeLengthSqr = clusterI==clusterJ?1.5:4;
				} else
				{
					if (j<_nVertices)
					{
						edgeLengthSqr = clusterI==clusterJ?1:4;
					} else
					{
						edgeLengthSqr = 9;
					}
				}
				edgeLengthSqr*=k*k;

				fvec d = _layout[j]-_layout[i];
				fvec disp;
				float normD = norm(d,2);
				if (normD<1.0e-5)
				{
					disp = fvec(dim);
					for (int iDim=0; iDim<dim; ++iDim)
					{
						disp(iDim) = rand()%100/100.0f;
					}
					disp *= temperature;
				} else
				{
					//disp = d*k*k/pow(normD,2);
					disp = edgeLengthSqr/pow(normD, 2)*d;
				}
				dispBuf[j] += disp;
				dispBuf[i] -= disp;
			}
		}

		DWORD tRepForce = GetTickCount()-t2;

		/*calculate attractive force*/
		for (i=0; i<_edges.size(); ++i)
		{
			int edgeIdx = i+_nVertices;
			for (j=0; j<_edges[i].size(); ++j)
			{
				float c2 = 10;
				float edgeLengthSqr = _clusters[_edges[i][j]]==i?1:9.0;
				edgeLengthSqr *= k;
				fvec d = _layout[edgeIdx] - _layout[_edges[i][j]];
				fvec disp = c2*d*norm(d,2)/edgeLengthSqr;
				dispBuf[edgeIdx] -= disp;
				dispBuf[_edges[i][j]] += disp;

				/*for (int m=0; m<j; ++m)
				{
					if (m==j)
					{
					continue;
					}
					float edgeLengthSqr = _clusters[_edges[i][j]]==_clusters[_edges[i][m]]?1:16.0;
					edgeLengthSqr *= k;
					fvec d = _layout[_edges[i][m]] - _layout[_edges[i][j]];
					fvec disp = c2*d*norm(d,2)/edgeLengthSqr;
					dispBuf[_edges[i][m]] -= disp;
					dispBuf[_edges[i][j]] += disp;
				}*/
			}
		}
		DWORD tRepAttrForces = GetTickCount()-t2;

		/*restrict the points from being pushed towards the boundary*/
		for (i=0; i<nTotal; ++i)
		{
			for (j=0; j<dim; ++j)
			{
				float c1=1.0e6;
				float x = _layout[i](j);
				if (x==boundary[j*2] || x==boundary[j*2+1])
				{
					int stop = 1;
				}
				float d = c1*(1/(x-boundary[j*2]+1.0e-3)-1/(boundary[j*2+1]-x+1.0e-3));
				dispBuf[i](j) += d;
			}
		}

		/*limit the maximum displacement, boundary check*/
		for (i=0; i<nTotal; ++i)
		{
			float n = norm(dispBuf[i],2);
			fvec newLayout = _layout[i]+dispBuf[i]*min(n, temperature)/n;
			for (j=0; j<dim; ++j)
			{
				int minBoundary = boundary[2*j];
				int maxBoundary = boundary[2*j+1];
				newLayout(j) = newLayout(j) < minBoundary?minBoundary:(newLayout(j)>maxBoundary?maxBoundary:newLayout(j));
			}
			if (norm(newLayout-_layout[i], 2)>1.0e-3)
			{
				_layout[i] = newLayout;
				bChanged = true;
			}
		}

		if (!bChanged)
		{
			break;
		}

		DWORD perItTime = GetTickCount() - t1;
		t1 = GetTickCount();

		prog.setValue(it);
	}
}

void Hypergraph::updateLayoutUnpinned(int boundary [], int dim/* =2 */, float c/* =1.0 */)
{
	_layoutDim = dim;
	int nIteration = 50;
	float temperature = 10;

	srand(time(NULL));

	int area = 1;
	for (int i=0; i<dim; ++i)
	{
		area *= boundary[2*i+1]-boundary[2*i];
	}
	float k = c*sqrt(area*1.0f/_nVertices);

	/*initialize vertex layout*/
	int nEdges = _edges.size();
	int nTotal = _nVertices+nEdges;
	//_layout.resize(nTotal);
	vector<fvec> dispBuf (nTotal);
	for (int i=0; i<nTotal; ++i)
	{
		dispBuf[i] = fvec(dim);
	}

	int i,j;

	QProgressDialog prog ("Performing graph layout ...", "", 0, nIteration-1);
	prog.setCancelButton(NULL);
	for (int it=0; it<nIteration; ++it)
	{
		DWORD t1 = GetTickCount();
		bool bChanged = false;
		for (i=0; i<nTotal; ++i)
		{
			dispBuf[i].fill(0);
		}

		DWORD t2 = GetTickCount();
		/*calculate repulsive force*/
		for (i=0; i<nTotal; ++i)
		{
			for (j=0; j<i; ++j)
			{
				if (j==i)
				{
					continue;
				}
				fvec d = _layout[j]-_layout[i];
				fvec disp;
				float normD = norm(d,2);
				if (normD<1.0e-5)
				{
					disp = fvec(dim);
					for (int iDim=0; iDim<dim; ++iDim)
					{
						disp(iDim) = rand()%100/100.0f;
					}
					disp *= temperature;
				} else
				{
					disp = d*k*k/pow(normD,2);
				}
				dispBuf[j] += disp;
				dispBuf[i] -= disp;
			}
		}

		DWORD tRepForce = GetTickCount()-t2;

		/*calculate attractive force*/
		for (i=0; i<_edges.size(); ++i)
		{
			int edgeIdx = i+_nVertices;
			for (j=0; j<_edges[i].size(); ++j)
			{
				float c2 = 3.0;
				fvec d1 = _layout[edgeIdx] - _layout[_edges[i][j]];
				fvec disp1 = c2*d1*norm(d1,2)/k/k;
				dispBuf[edgeIdx] -= disp1;
				dispBuf[_edges[i][j]] += disp1;

				for (int m=0; m<j; ++m)
				{
				if (m==j)
				{
				continue;
				}
				fvec d = _layout[_edges[i][m]] - _layout[_edges[i][j]];
				fvec disp = d*norm(d,2)/k/k;
				dispBuf[_edges[i][m]] -= disp;
				dispBuf[_edges[i][j]] += disp;
				}
			}
		}
		DWORD tRepAttrForces = GetTickCount()-t2;

		/*restrict the points from being pushed towards the boundary*/
		for (i=0; i<nTotal; ++i)
		{
			for (j=0; j<dim; ++j)
			{
				float c1=2.0e5;
				float x = _layout[i](j);
				float d = c1*(1/(x-boundary[j*2]+1.0e-3)-1/(boundary[j*2+1]-x+1.0e-3));
				dispBuf[i](j) += d;
			}
		}

		/*limit the maximum displacement, boundary check*/
		for (i=0; i<nTotal; ++i)
		{
			float n = norm(dispBuf[i],2);
			fvec newLayout = _layout[i]+dispBuf[i]*min(n, temperature)/n;
			for (j=0; j<dim; ++j)
			{
				int minBoundary = boundary[2*j];
				int maxBoundary = boundary[2*j+1];
				newLayout(j) = newLayout(j) < minBoundary?minBoundary:(newLayout(j)>maxBoundary?maxBoundary:newLayout(j));
			}
			if (norm(newLayout-_layout[i], 2)>1.0e-3)
			{
				_layout[i] = newLayout;
				bChanged = true;
			}
		}

		if (!bChanged)
		{
			break;
		}

		DWORD perItTime = GetTickCount() - t1;
		t1 = GetTickCount();

		prog.setValue(it);
	}

}

vector<unsigned> Hypergraph::getDistToModification(const vector<int> &modifySet)
{
	int nTotal = _nVertices+_edges.size();
	vector<unsigned> distVec (nTotal);
	distVec.assign(nTotal, 100);

	for (int i=0; i<modifySet.size(); ++i)
	{
		distVec[modifySet[i]] = 0;
	}

	int nEdges = _edges.size();
	vector<unsigned> pathBuf (nEdges*(nEdges+1)/2);
	pathBuf.assign(nEdges*(nEdges+1)/2, 100);

	/*initialization*/
	for (int i=0; i<nEdges; ++i)
	{
		for (int j=0; j<i; ++j)
		{
			HyperEdge v(_edges[i].size()+_edges[j].size());
			HyperEdge::iterator vIt = set_intersection(_edges[i].begin(), _edges[i].begin(), _edges[j].begin(), _edges[j].end(), v.begin());
			if (vIt!=v.begin())
			{
				pathBuf[i*(i-1)/2+j] = 2;
			}
		}

		for (int j=0; j<modifySet.size(); ++j)
		{
			int modifiedIdx = modifySet[j];
			HyperEdge::iterator it = find(_edges[i].begin(), _edges[i].end(), modifiedIdx);
			if (it!=_edges[i].end())
			{
				pathBuf[nEdges*(nEdges-1)/2+i] = 1;
				break;
			}
		}
	}

	for (int k=0; k<nEdges+1; ++k)
	{
		for (int i=0; i<nEdges+1; ++i)
		{
			if (i==k)
			{
				continue;
			}
			for (int j=0; j<i; ++j)
			{
				if (j==k)
				{
					continue;
				}
				int idxIJ = i*(i-1)/2+j;
				int idxIK = i>k?i*(i-1)/2+k:k*(k-1)/2+i;
				int idxJK = j>k?j*(j-1)/2+k:k*(k-1)/2+j;

				pathBuf[idxIJ] = min(pathBuf[idxIJ], pathBuf[idxIK]+pathBuf[idxJK]);
			}
		}
	}

	for (int i=0; i<nEdges; ++i)
	{
		distVec[i+_nVertices] = pathBuf[nEdges*(nEdges-1)/2+i];
		for (int j=0; j<_edges[i].size(); ++j)
		{
			distVec[_edges[i][j]] = min(distVec[i+_nVertices]+1, distVec[_edges[i][j]]);
		}
	}

	return distVec;
}

void Hypergraph::getPinnedWeights(const vector<unsigned> &distToModification)
{
	_pinWeights.resize(distToModification.size());
	float k = 0.5;
	for (int i=0; i<distToModification.size(); ++i)
	{
		_pinWeights[i] = exp(-k*distToModification[i]);
	}
}

void Hypergraph::updateLayoutPinned(int boundary [], int dim, float c)
{
	_layoutDim = dim;
	int nIteration = 50;
	float temperature = 10;

	srand(time(NULL));

	int area = 1;
	for (int i=0; i<dim; ++i)
	{
		area *= boundary[2*i+1]-boundary[2*i];
	}
	float k = c*sqrt(area*1.0f/_nVertices);

	/*initialize vertex layout*/
	int nEdges = _edges.size();
	int nTotal = _nVertices+nEdges;
	//_layout.resize(nTotal);
	vector<fvec> dispBuf (nTotal);
	for (int i=0; i<nTotal; ++i)
	{
		dispBuf[i] = fvec(dim);
	}

	int i,j;
	const float PIN_THRES = 1.0e-3;

	QProgressDialog prog ("Performing graph layout ...", "", 0, nIteration-1);
	prog.setCancelButton(NULL);
	for (int it=0; it<nIteration; ++it)
	{
		DWORD t1 = GetTickCount();
		bool bChanged = false;
		for (i=0; i<nTotal; ++i)
		{
			dispBuf[i].fill(0);
		}

		DWORD t2 = GetTickCount();
		/*calculate repulsive force*/
		for (i=0; i<nTotal; ++i)
		{
			if (_pinWeights[i]<PIN_THRES)
			{
				continue;
			}
			for (j=0; j<nTotal; ++j)
			{
				if (j==i)
				{
					continue;
				}
				fvec d = _layout[j]-_layout[i];
				fvec disp;
				float normD = norm(d,2);
				if (normD<1.0e-5)
				{
					disp = fvec(dim);
					for (int iDim=0; iDim<dim; ++iDim)
					{
						disp(iDim) = rand()%100/100.0f;
					}
					disp *= temperature;
				} else
				{
					disp = d*k*k/pow(normD,2);
				}
				//dispBuf[j] += disp;
				dispBuf[i] -= disp;
			}
		}

		DWORD tRepForce = GetTickCount()-t2;

		/*calculate attractive force*/
		for (i=0; i<_edges.size(); ++i)
		{
			int edgeIdx = i+_nVertices;
			for (j=0; j<_edges[i].size(); ++j)
			{
				if (_pinWeights[_edges[i][j]]<PIN_THRES)
				{
					continue;
				}
				float c2 = 3.0;
				fvec d1 = _layout[edgeIdx] - _layout[_edges[i][j]];
				fvec disp1 = c2*d1*norm(d1,2)/k/k;
				dispBuf[edgeIdx] -= disp1;
				dispBuf[_edges[i][j]] += disp1;

				for (int m=0; m<_edges[i].size(); ++m)
				{
					if (m==j)
					{
						continue;
					}
					fvec d = _layout[_edges[i][m]] - _layout[_edges[i][j]];
					fvec disp = d*norm(d,2)/k/k;
					//dispBuf[_edges[i][m]] -= disp;
					dispBuf[_edges[i][j]] += disp;
				}
			}
		}
		DWORD tRepAttrForces = GetTickCount()-t2;

		/*restrict the points from being pushed towards the boundary*/
		for (i=0; i<nTotal; ++i)
		{
			if (_pinWeights[i]<PIN_THRES)
			{
				continue;
			}
			for (j=0; j<dim; ++j)
			{
				float c1=2.0e5;
				float x = _layout[i](j);
				float d = c1*(1/(x-boundary[j*2]+1.0e-3)-1/(boundary[j*2+1]-x+1.0e-3));
				dispBuf[i](j) += d;
			}
		}

		/*limit the maximum displacement, boundary check*/
		for (i=0; i<nTotal; ++i)
		{
			if (_pinWeights[i]<PIN_THRES)
			{
				continue;
			}

			float n = norm(dispBuf[i],2);
			fvec newLayout = _layout[i]+dispBuf[i]*min(n, temperature*_pinWeights[i])/n;
			for (j=0; j<dim; ++j)
			{
				int minBoundary = boundary[2*j];
				int maxBoundary = boundary[2*j+1];
				newLayout(j) = newLayout(j) < minBoundary?minBoundary:(newLayout(j)>maxBoundary?maxBoundary:newLayout(j));
			}
			if (norm(newLayout-_layout[i], 2)>1.0e-3)
			{
				_layout[i] = newLayout;
				bChanged = true;
			}
		}

		if (!bChanged)
		{
			break;
		}

		DWORD perItTime = GetTickCount() - t1;
		t1 = GetTickCount();

		prog.setValue(it);
	}

}

void Hypergraph::drawPinWeights()
{
	for (int i=0; i<_nVertices+_edges.size(); ++i)
	{
		float w = _pinWeights[i];

		glPushMatrix();
		/*label the vertex*/
		glPushName(i);
		glTranslatef(_layout[i](0), _layout[i](1), 0);
		glColor3f(1,1,1);
		glutSolidSphere(5, 10, 16);
		glColor3f(w, w, w);
		glutSolidSphere(5, 10, 10);
		glPopName();
		glPopMatrix();

	}
}

void Hypergraph::layoutPartitioned(int boundary [], bool bUpdate)
{
	int dim = 2;
	int nIteration = bUpdate?50:500;
	float temperature = 10;
	typedef fvec2 LayoutPoint;

	srand(time(NULL));

	int area = 1;
	for (int i=0; i<dim; ++i)
	{
		area *= boundary[2*i+1]-boundary[2*i];
	}
	float k = 1.0*sqrt(area*1.0f/_nVertices);

	/*initialize vertex layout*/
	int nEdges = _edges.size();
	int nTotal = _nVertices+nEdges;
	_layout.resize(nTotal);
	vector<LayoutPoint> dispBuf (nTotal);

	if (!bUpdate)
	{
		for (int i=0; i<nTotal; ++i)
		{
			_layout[i] = fvec(dim);
			for (int j=0; j<dim; ++j)
			{
				_layout[i](j) = boundary[2*j]+rand()%(boundary[2*j+1]-boundary[2*j]);
			}
			dispBuf[i] = fvec(dim);
		}
		_pinWeights.resize(nTotal);
		_pinWeights.assign(nTotal, 1);
	} 

	int nClusters = *max_element(_clusters.begin(), _clusters.end())+1;
	int nPartitions;
	vector<int> partitionSizes;
	getPartitionsByEdge(nPartitions, partitionSizes, _partitions);
	//getPartitionsByCluster(nPartitions, partitionSizes, _partitions);
	//getPartitionsByKDTree(nPartitions, partitionSizes, _partitions);
	vector<LayoutPoint> partCenters (nPartitions);

	vector<vector<int> > edgeBuf (_nVertices);
	for (int i=0; i<_edges.size(); ++i)
	{
		for (int j=0; j<_edges[i].size(); ++j)
		{
			edgeBuf[_edges[i][j]].push_back(i+_nVertices);
		}
	}

	int i,j;
	DWORD t0 = GetTickCount();
	DWORD perItTime = 0;
	DWORD tReplInt = 0;
	DWORD tReplExt = 0;
	DWORD tAttr = 0;
	QProgressDialog prog ("Performing graph layout ...", "", 0, nIteration-1);
	prog.setCancelButton(NULL);
	for (int it=0; it<nIteration; ++it)
	{
		DWORD t1 = GetTickCount();
		bool bChanged = false;
		for (int i=0; i<nTotal; ++i)
		{
			dispBuf[i].fill(0);
		}

		for (int i=0; i<nPartitions; ++i)
		{
			partCenters[i].fill(0);
			for (int j=0; j<nTotal; ++j)
			{
				if (i!=_partitions[j])
				{
					continue;
				}
				partCenters[i] += _layout[j];
			}
			partCenters[i] /= partitionSizes[i];
		}

		for (int i=0; i<nTotal; ++i)
		{
			if (_pinWeights[i]<1.0e-3)
			{
				continue;
			}

			float edgeLength = (i<_nVertices&&_clusters[i]==-1)?1.5*k:k;

			/*interior repulsive forces*/
			DWORD t2 = GetTickCount();
			for (int j=0; j<i; ++j)
			{
				if (_partitions[i]!=_partitions[j])
				{
					continue;
				}
				int vertexIdx = j;
				fvec disp = getRepulsiveForce(_layout[i], _layout[j], k);
				dispBuf[i] -= disp;
				dispBuf[j] += disp;
			}
			tReplInt += GetTickCount() - t2;

			/*exterior repulsive forces*/
			t2 = GetTickCount();
			for (int j=0; j<nPartitions; ++j)
			{
				if (j==_partitions[i])
				{
					continue;
				}
				if (partitionSizes[j]!=0)
				{
					fvec disp = partitionSizes[j]*getRepulsiveForce(_layout[i], partCenters[j], k);
					dispBuf[i] -= disp;
				}
			}
			tReplExt += GetTickCount() - t2;

			/*attractive forces*/
			t2 = GetTickCount();
			if (i<_nVertices)
			{
				for (int j=0; j<edgeBuf[i].size(); ++j)
				{
					int edgeIdx = edgeBuf[i][j];
					fvec disp = getAttractiveForce(_layout[i], _layout[edgeIdx], edgeLength);
					dispBuf[i] += 3.0*disp;
					dispBuf[edgeIdx] -= 3.0*disp;
				}
			}
			tAttr += GetTickCount() - t2;

		}

		/*cooling*/
		for (int i=0; i<nTotal; ++i)
		{
			/*adjust forces*/
			for (int j=0; j<dim; ++j)
			{
				float c1=1.0e5;
				float x = _layout[i](j);
				float d = c1*(1.0/(x-boundary[j*2]+1.0e-3)-1.0/(boundary[j*2+1]-x+1.0e-3));
				dispBuf[i](j) += d;
			}
			float n = norm(dispBuf[i],2);
			fvec newLayout = _layout[i]+dispBuf[i]*min(n, temperature*_pinWeights[i])/n;
			for (int j=0; j<dim; ++j)
			{
				int minBoundary = boundary[2*j];
				int maxBoundary = boundary[2*j+1];
				newLayout(j) = newLayout(j) < minBoundary?minBoundary:(newLayout(j)>maxBoundary?maxBoundary:newLayout(j));
			}
			_layout[i] = newLayout;
		}

		perItTime += GetTickCount() - t1;
		prog.setValue(it);
	}
	DWORD totalTime = GetTickCount()-t0;
	tReplInt /= nIteration;
	tReplExt /= nIteration;
	tAttr /= nIteration;
	perItTime /= nIteration;
	return;
}

fvec Hypergraph::getRepulsiveForce(fvec &pt1, fvec &pt2, float edgeLength)
{
	int dim = pt1.n_elem;
	fvec d = pt2 - pt1;
	fvec disp;
	float normDSqr = dot(d,d);
	if (normDSqr<1.0e-6)
	{
		disp = fvec(dim);
		for (int iDim=0; iDim<dim; ++iDim)
		{
			disp(iDim) = rand()%100/100.0f;
		}
		disp *= edgeLength*edgeLength;
	} else
	{
		disp = d*edgeLength*edgeLength/normDSqr;
	}
	return disp;
}

fvec Hypergraph::getAttractiveForce(fvec &pt1, fvec &pt2, float edgeLength)
{
	fvec d = pt2 - pt1;
	fvec disp = d*dot(d,d)/edgeLength/edgeLength;
	return disp;
}

void Hypergraph::getPartitionsByCluster(int &nPartitions, vector<int> &partitionSizes, vector<int> &partitions)
{
	int minPartition = 5;
	int maxPartition = 15;
	
	int nClusters = *max_element(_clusters.begin(), _clusters.end())+1;
	int partitionIdx = 0;
	int partitionCnt = 0;

	_clusters.resize(_nVertices+_edges.size());
	for (int i=_nVertices; i<_nVertices+_edges.size(); ++i)
	{
		_clusters[i] = i-_nVertices;
	}
	_partitions.resize(_clusters.size());

	for (int i=0; i<nClusters; ++i)
	{
		int clusterSize = count(_clusters.begin(), _clusters.end(), i);
		int partitionSize = 0;
		if (clusterSize<=maxPartition)
		{
			partitionSize = clusterSize;
		} else
		{
			float n = (ceil(clusterSize*1.0f/maxPartition)+floor(clusterSize*1.0f/minPartition))/2.0;
			partitionSize = ceil(clusterSize/n);
		}

		for (int j=0; j<_clusters.size(); ++j)
		{
			if (_clusters[j]!=i)
			{
				continue;
			}
			/*if (partitionCnt>=partitionSize)
			{
				partitionSizes.push_back(partitionCnt);
				++partitionIdx;
				partitionCnt = 0;
			}*/
			_partitions[j] = partitionIdx;
			++partitionCnt;
		}
		partitionSizes.push_back(partitionCnt);
		++partitionIdx;
		partitionCnt = 0;
	}
	nPartitions = partitionIdx;
	_clusters.resize(_nVertices);
}

void Hypergraph::drawPartitions()
{
	srand(NULL);
	int nPartitions = *max_element(_partitions.begin(), _partitions.end())+1;
	ColorTable * colTable = new ColorTable();
	vector<float> colBuf (nPartitions);
	for (int i=0; i<nPartitions; ++i)
	{
		colBuf[i] = i*1.0/(nPartitions-1);
	}
	for (int i=0; i<_nVertices+_edges.size(); ++i)
	{
		glPushMatrix();
		/*label the vertex*/
		glPushName(i);
		glTranslatef(_layout[i](0), _layout[i](1), 0);
		glColor3f(1,1,1);
		glutSolidSphere(7, 10, 10);
		glColor3ubv(colTable->getColor(colBuf[_partitions[i]]));
		glutSolidSphere(5, 10, 10);
		glPopName();
		glPopMatrix();

	}
}

void Hypergraph::getPartitionsByKDTree(int &nPartitions, vector<int> &partitionSizes, vector<int> &partitions)
{
	int minPartition = 5;
	int maxPartition = 15;

	int nClusters = *max_element(_clusters.begin(), _clusters.end())+1;
	int partitionIdx = 0;
	int partitionCnt = 0;
	_clusters.resize(_nVertices+_edges.size());
	for (int i=_nVertices; i<_nVertices+_edges.size(); ++i)
	{
		_clusters[i] = i-_nVertices;
	}
	_partitions.resize(_clusters.size());

	for (int i=0; i<nClusters; ++i)
	{
		int clusterSize = count(_clusters.begin(), _clusters.end(), i);
		int partitionSize = 0;
		int nSubPartitions = (clusterSize*1.0f/maxPartition+clusterSize*1.0f/minPartition)/2;
		int depth = max(floor(log(1.0f*nSubPartitions)/log(2.0f)),0);
		nSubPartitions = pow(2.0f, depth);
		vector<fvec> ptBuf (clusterSize);
		vector<float> divisions (pow(2.0f, depth+1)-1);
		int bufIdx = 0;
		for (int j=0; j<_clusters.size(); ++j)
		{
			if (_clusters[j] == i)
			{
				ptBuf[bufIdx++] = _layout[j];
			}
		}
		kdPartition(0, depth, ptBuf.begin(), ptBuf.end(), divisions, 0);

		for (int j=0; j<_clusters.size(); ++j)
		{
			if (_clusters[j] == i)
			{
				int subPartitionIdx = kdCoord(_layout[j], depth, divisions);
				_partitions[j] = partitionIdx+subPartitionIdx;
			}
		}

		partitionIdx += nSubPartitions;
	}
	nPartitions = partitionIdx;
	partitionSizes.resize(nPartitions);
	for (int i=0; i<nPartitions; ++i)
	{
		partitionSizes[i] = count(_partitions.begin(), _partitions.end(), i);
	}

	_clusters.resize(_nVertices);
}


bool compareX (fvec pt1, fvec pt2) {return pt1(0)<=pt2(0);}
bool compareY (fvec pt1, fvec pt2) {return pt1(1)<=pt2(1);}

void Hypergraph::kdPartition (int depth, int targetDepth, vector<fvec>::iterator &firstPt, vector<fvec>::iterator &lastPt, 
							  vector<float> &divisions, int divisionIdx)
{
	if (depth>targetDepth)
	{
		return;
	}

	int nPoints = lastPt-firstPt;
	int nMidPoint = nPoints/2;
	int coord = depth%2;
	switch (coord)
	{
	case 0:
		nth_element(firstPt, firstPt+nMidPoint, lastPt, compareX);
		break;
	case 1:
		nth_element(firstPt, firstPt+nMidPoint, lastPt, compareY);
		break;
	}

	fvec &midPoint = *(firstPt+nMidPoint);
	divisions[divisionIdx] = midPoint(coord%2);
	kdPartition(depth+1, targetDepth, firstPt, firstPt+nMidPoint, divisions, 2*divisionIdx+1);
	kdPartition(depth+1, targetDepth, firstPt+nMidPoint, lastPt, divisions, 2*divisionIdx+2);
}

int Hypergraph::kdCoord(const fvec &pt, int depth, vector<float> &divisions)
{
	int coord = 0;
	int divisionIdx = 0;
	for (int d=0; d<=depth; ++d)
	{
		int c = 0;
		switch (d%2)
		{
		case 0:
			c = pt(0)>divisions[divisionIdx];
			break;
		case 1:
			c = pt(1)>divisions[divisionIdx];
			break;
		}

		coord |= c<<(depth-d);
		divisionIdx += pow(2.0f, d)+c;
	}
	return coord;
}

void Hypergraph::getPartitionsByEdge(int &nPartitions, vector<int> &partitionSizes, vector<int> &partitions)
{
	int nEdges = _edges.size();
	nPartitions = nEdges;
	partitions.resize(_nVertices+nEdges);
	partitions.assign(partitions.size(), -1);

	for (int i=0; i<nEdges; ++i)
	{
		for (int j=0; j<_edges[i].size(); ++j)
		{
			int idx = _edges[i][j];
			if (partitions[idx]==-1)
			{
				partitions[idx] = i;
			} else if (_clusters[idx]==i)
			{
				partitions[idx] = i;
			}
		}
		partitions[i+_nVertices] = i;
	}

	partitionSizes.resize(nPartitions);
	for (int i=0; i<nPartitions; ++i)
	{
		partitionSizes[i] = count(partitions.begin(), partitions.end(), i);
	}
}

vector<int> Hypergraph::getSelectedIdx()
{
	vector<int> selectedIdx;
	for (int i=0; i<_nVertices; ++i)
	{
		if (_selectedBuf[i])
		{
			selectedIdx.push_back(i);
		}
	}
	return selectedIdx;
}

int Hypergraph::getLinkage()
{
	if (_linkageBuf.empty())
	{
		return -1;
	} else
	{
		return _linkageBuf[0]-_nVertices;
	}
}