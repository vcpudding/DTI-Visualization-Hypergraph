#pragma once

#include <vector>
#include <QString>
#include <armadillo>
#include "ColorTable.h"
#include "common.h"

using namespace std;
using namespace arma;

typedef vector<int> HyperEdge;

class Hypergraph
{
public:
	Hypergraph(void);
	~Hypergraph(void);

private:
	int						_nVertices;
	vector<HyperEdge>		_edges;
	vector<fvec>			_layout;
	int						_layoutDim;
	//ColorTable *			_colorTable;
	vector<bool>			_selectedBuf;
	vector<int>				_linkageBuf;
	vector<int>				_clusters;
	ColorTable *			_clusterColorer;
	vector<float>			_pinWeights;
	vector<int>				_partitions;

public:
	void					setNumOfVertices (int nVertices);
	void					clearEdges () {_edges.clear();}
	void					addEdge (const HyperEdge & e) {_edges.push_back(e);}

	void					saveHypergraph (const QString &fileName);           
	void					loadHypergraph (const QString &fileName);
	void					layoutGraph (int boundary [], int dim=2, float c=1.0);
	void					layoutFast (int boundary [], int dim=2, float c=1.0, int Vmax=5, int Smax=10, int nIteration=1000);
	void					layoutClustered (int boundary [], int nIteration, int dim=2, float c=1.0);
	void					findSampleSets (int vertexIdx, int nTotal, float &maxDist, const vector<float> &distMat, vector<int> &VBuf, vector<int> &SBuf, int Vmax, int Smax);
	void					findSampleSets (int vertexIdx, float &maxDist, const vector<float> &distMat, list<int> &sampleBuf, int &Vsize, int Vmax, int Smax);

	void					saveLayout (const QString &fileName);
	void					loadLayout (const QString &fileName);
	void					drawHypergraph ();
	void					drawPinWeights ();
	void					drawPartitions ();
	const vector<bool>&		getSelectionBuf () {return _selectedBuf;}
	void					setSelectionBuf (const vector<bool> selectionBuf);
	bool *					selectVertices (const vector<int> &selectedIdx, int mode);
	bool *					selectLinkage ();
	void					renewSelection (const bool *selectionBuf, SelectionMode mode);
	void					addLinkageEdge (const vector<int> idxBuf);
	void					setLinkageEdge (const vector<int> idxBuf);
	void					resetLinkageBuf () {_linkageBuf.clear();}
	bool					assignSelectedToCluster ();
	void					setClusters (const vector<int> &clusters);
	const vector<int>&		getClusters () {return _clusters;}
	void					setClusterColorer (ColorTable *colorer) {_clusterColorer = colorer;}

	vector<unsigned>		computeGraphDistMat ();
	vector<unsigned>		computeEdgeDistMat ();
	vector<int>				getGraphDifference (const Hypergraph *newGraph);
	void					updateLayout (const vector<int> &changedSet, int boundary [], int dim=2, float c=1.0);
	void					setEdges (const Hypergraph *newGraph);

	void					updateLayoutUnpinned (int boundary [], int dim=2, float c=1.0);
	void					updateLayoutPinned (int boundary [], int dim=2, float c=1.0);
	void					layoutPartitioned (int boundary [], bool bUpdate = false);
	vector<unsigned>		getDistToModification (const vector<int> &modifySet);
	void					getPinnedWeights (const vector<unsigned> &distToModification);

	fvec					getRepulsiveForce (fvec &pt1, fvec &pt2, float edgeLength);
	fvec					getAttractiveForce (fvec &pt1, fvec &pt2, float edgeLength);
	void					getPartitionsByCluster (int &nPartitions, vector<int> &partitionSizes, vector<int> &partitions);
	void					getPartitionsByEdge (int &nPartitions, vector<int> &partitionSizes, vector<int> &partitions);
	void					getPartitionsByKDTree (int &nPartitions, vector<int> &partitionSizes, vector<int> &partitions);
	void					kdPartition (int depth, int targetDepth, vector<fvec>::iterator &firstPt, vector<fvec>::iterator &lastPt, 
										vector<float> &divisions, int divisionIdx);
	int						kdCoord (const fvec &pt, int depth, vector<float> &divisions);

	vector<int>				getSelectedIdx ();
	int						getLinkage ();
};
