#pragma once

#include "FiberData.h"
#include "ColorTable.h"
#include "CylindProj.h"
#include "mds.h"

class TemplateFiberData :
	public FiberData
{
public:
	TemplateFiberData(void);
	~TemplateFiberData(void);

	friend					CylindPoint		getProjection (const TemplateFiberData *templateData, const FloatPoint* fib);

private:
	vector<CylindPoint>		_clusterEmbeds;
	QStringList				_clusterLabels;
	vector<int>				_clusterBuf;
	bool					_bShowLabels;
	unsigned				_totalPtCount;
	ColorTable *			_clusterColorTable;

public:
	static 
	TemplateFiberData*		templateData;

public:
	void					loadClusterEmbeddings (const QString &fileName);
	void					loadClusterLabels (const QString &fileName);

	void					requireData (bool bFibers, bool bClusters, bool bEmbs, bool bLabels);

	void					tractMDS ();
	void					clusterMDS ();

	float					getTplColorIndex (int clusterIndex);
	static uchar *			getTplColor (int clusterIndex);

	static void				drawAtlasEmbeds(float cylindRadius, bool bShowLabels = true, bool bColorTpl = false);
	static void				drawAtlasTracts();
	static void				drawAtlasTracts(vector<int> &highlightClusters);
	static vector<float>	getAtlasNeighbors (const FloatPoint *targetFib, int numOfNeighbors, float &minBundleDist);
	static EmbedPoint		getEmbedding (const FloatPoint *targetFib);
	static CylindPoint		lleEmbedding (const FloatPoint *targetFib, float &minBundleDist);
	static float*			getAtlasDistMatrixGPU (FiberData *fibData);
	static CylindPoint		lleEmbedding (float *distVec, int numOfNeighbors, float &minBundleDist);
	static vector<float>	getAtlasWeights (float *distVec, int numOfNeighbors, float &minBundleDist);
	static EmbedPoint *		getEmbeddingsGPU (FiberData *fibData, int numOfNeighbors = 20);

	static CylindPoint		cylindEmbedding (const FloatPoint *targetFib);
	static float			dist (const FloatPoint *f1, const FloatPoint *f2);
	static float			symDist (const FloatPoint *f1, const FloatPoint *f2);

	static QStringList		clusterLabels();
	static CylindPoint		clusterEmb (int clusterIdx, int left);
};