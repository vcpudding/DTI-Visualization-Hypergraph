#pragma once

#include <list>
#include <vector>
#include "SeedData.h"
#include "TensorData.h"
#include "Hypergraph.h"
#include "common.h"

using namespace std;

typedef list<FloatPoint> ListFiber;
typedef FloatPoint* ArrFiber;
typedef vector<int> Link;

typedef struct  
{
	float			faThres;
	float			angleThres;
	float			stepSize;
	int				minLength, maxLength;
	int				nFibers;
	int				isFiberCount;
} TrackingConfig;

typedef fvec2 CylindPoint;

typedef struct  
{
	CylindPoint		posLLE;
	CylindPoint		posProj;
	float			minDist;
} EmbedPoint;

class FiberData
{
public:
	FiberData(void);
	~FiberData(void);

	static enum			DistMetric {DIST_HAUSDORFF, DIST_LCSS, DIST_DTW, DIST_MADAH};

	friend				CylindPoint		clusterCenter (FiberData *fiberData, int clusterIndex);
	friend				void			embedTemplateClusters ();
	friend				class			ClusterEditor;
	friend				class			expResultVis;

public:
	unsigned			_nPoints;
	int					_nFibers;
	int					_nSelectedFibers;
	vector<ArrFiber>	_fibers;
	vector<ArrFiber>	_clusterCenters;
	ColorMode			_colorMode;
	float				_fiberThickness;
	TensorData *		_tensorData;
	short				_sizeX, _sizeY, _sizeZ;
	float				_volSizeX, _volSizeY, _volSizeZ;
	vector<int>			_clusters;
	int					_currentCluster;
	EmbedPoint *		_embedPoints;
	bool				_bClustersLoaded;
	vector<bool>		_selectedBuf;
	float *				_normMap;
	int					_normSizeX, _normSizeY, _normSizeZ;
	bool				_bNormalized;
	ColorTable *		_clusterColorer;
	GLuint				_fiberDrawList;
	vector<bool>		_filterBuf;
	int					_selectionIndex;
	float				_graphThres;
	vector<vector<fmat33> > _clusterCovs;
	vector<double>		_paramAlpha;
	vector<double>		_paramBeta;
	vector<double>		_paramW;
	vector<double>		_fuzzyClusters;
	int					_nClusters;
	vector<vector<Link> >	_matchings;

public:
	void				fiberTracking (SeedData * seedData, TensorData * tensorData, const TrackingConfig & config);
	ListFiber			trackFiber (const FloatPoint &seedPt, TensorData *tensorData, double stepSize, double faThres, double angleThres);
	void				trackSingleDir (ListFiber &fib, FloatPoint & currDir, TensorData *tensorData, double stepSize, double faThres, double angleThres);

	void				setNumOfFibers(int n);
	void				drawFibers();
	void				drawClusterCenters ();
	void				drawMatchings ();
	void				genFiberDispList();
	void				setFiberColor (int fibIndex, int ptIndex, float alpha=1);
	void				drawEmbeddings (int type = 0, float minDistThres = 30);
	void				setFiberThickness (float thk) {_fiberThickness = thk;}
	void				setColorMode (int clrMode);
	void				setCurrentCluster (int cluster) {_currentCluster = cluster;}
	unsigned			loadFibers (const QString &fileName, int sizeX, int sizeY, int sizeZ);
	void				loadFibersFromTrk (const QString &fileName);
	void				loadClusters (const QString &fileName);
	void				loadFuzzyClusters (const QString &fileName);
	void				saveClusters (const QString &fileName);
	void				saveFuzzyClusters (const QString &fileName);
	void				loadEmbeddings (const QString &fileName);
	void				saveEmbeddings (const QString &fileName);
	void				loadNormMap (const QString &fileName);
	void				normalizeFibers ();
	void				resampleEqualSample (int n=15);
	void				resampleEqualInc (float incLength);
	void				saveFibers (const QString &fileName);
	void				initClusterColor ();
	void				saveKMeansClusterCenters (const QString &fileName);
	void				loadKMeansClusterCenters (const QString &fileName);
	void				filterByLength (float minLength, float maxLength);
	void				saveFibersAsTrk (const QString &fileName);

	int					nFibers () {return _nFibers;}
	int					nSelected () {return _nSelectedFibers;}
	unsigned			nPoints () {return _nPoints;}
	const ArrFiber		fiber (int index) const {return _fibers[index];}
	int					fiber (int index, float *ptBuf) const;
	void				embedGPU (const QString &normFileName, int nNeighbors = 80);
	void				saveAtlasWeights (const QString &normFileName, const QString &saveFileName);

	int					selectFibers (int left, int top, int right, int bottom, GLdouble *embModelMatrix, GLdouble *embProjMatrix, int *embViewport, int type = 0, float minDistThres = 30);
	int					selectFibers (vector<int> &idxBuf, SelectionMode mode);
	int					selectFiber (int fibIdx);
	int					selectFibers (int clusterIdx);
	void				assignLabel (int label);
	void				assignAtlasLabel (int label);
	void				addToTemplate ();

	void				saveDistMatrix (const QString &fileName);
	void				saveDistToRegion (const QString &fileName, const QString &regionFileName);

	void				clusterKMeans (int k);
	void				clusterKMeansWithPrune (int k);
	void				clusterFuzzyCMeans (bool bUpdate, const vector<int> &seedBuf = vector<int>(), int nSteps = 10000);
	void				clusterFuzzyCMeansDTW (bool bUpdate, const vector<int> &seedBuf = vector<int>(), int nSteps = 10000);
	void				clusterAFCC (int maxNumOfClusters);

	Hypergraph *		getGraphKMeans (DistMetric metric, vector<bool> &bClusterChanged=vector<bool>());
	Hypergraph *		getGraphFuzzyCMeans ();
	const vector<bool>&	getSelectionBuf () {return _selectedBuf;}
	void				setSelectionBuf (const vector<bool>selectionBuf);
	const vector<int>&	getClusters () {return _clusters;}
	void				setClusters (const vector<int> &clusters, vector<bool> &bClusterChanged);
	void				updateClusterCenters ();
	ColorTable *		getClusterColorer () {return _clusterColorer;}
	vector<int>			getSelectedIdx ();

	void				downSampleRandom (int nSamples);
	void				setGraphThres (float t) {_graphThres = t;}
	void				getClusterCovs (const vector<vector<Link> > &matchings, vector<vector<fmat> > &clusterCovs);
	void				getMatchings (vector<vector<Link> > &matchings);
	void				initClusterMedoids ();
	void				updateClusterCenters (const vector<vector<Link> > &matchings, vector<bool> &bClusterChanged=vector<bool>());
	void				updateClusterCentersFuzzy (const vector<vector<Link> > &matchings, vector<bool> &bClusterChanged=vector<bool>());
	void				updateClusterCentersFuzzyDTW ();
	void				updateFuzzyClustering (const vector<float> &distBuf, bool bEStep);
	void				getClustersFromFuzzyClusters ();
	void				assignCluster (int fibIdx, int clusterIdx);
	void				assignSelectedToCluster (int clusterIdx);
	float				getLikelihoodFunc (const vector<float> &distBuf);
	double				getAlpha (double x);
	double				getGammaFunc (double d, double alpha, double beta);
	double				getCostAlpha (double alpha, double sumPik, double sumDist, double sumLogDist);
};

float * getDistMatrixGPU (FiberData *fibData1, FiberData *fibData2);
float fibDistHausdorff (const ArrFiber fib1, const ArrFiber fib2);
float fibDistLCSS (const ArrFiber fib1, const ArrFiber fib2, float e, int d);
float fibDistDTW (const ArrFiber fib1, const ArrFiber fib2, int d, vector<int>& matching);
float fibDistDTWCov (const ArrFiber fib1, const ArrFiber fib2, int d, vector<int>& matching, vector<fmat33> covs = vector<fmat33>());
float fibDistMadah (const ArrFiber fib, const ArrFiber center, const Link &matching, const vector<fmat33> &covs = vector<fmat33>());
void matchFiber (const ArrFiber fib1, const ArrFiber fib2, Link &matching);