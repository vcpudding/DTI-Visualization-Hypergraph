#ifndef MDS_H
#define MDS_H

float MDS (float *distMat, int nInput, int outDim, float *outPoints, float thres=1.0e-6);

float weightedMDS(const float *distMat, const float *weightMatrix, int nInput, int outDim, float *outPoints, float thres = 1.0e-6);

void testMDS ();

#endif