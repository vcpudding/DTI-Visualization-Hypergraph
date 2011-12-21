#include <time.h>
#include <armadillo>

using namespace arma;

float MDS (float *distMat, int nInput, int outDim, float *outPoints, float thres=1.0e-6)
{
 	srand(time(NULL));
 
 	fmat X = randu<fmat>(nInput, outDim);
 	/////////////////
 	//test
 	/*X <<-0.266 <<-0.539 <<endr
 	  <<0.451  <<0.252  <<endr
 	  <<0.016  <<-0.238 <<endr
 	  <<-0.2   <<0.524  <<endr;*/
 	////////////////
 
 	double err = 10.0e4;
 
 	double constTerm = 0.0;
 	for (int i=0; i<nInput; ++i)
 	{
 		for (int j=0; j<i; ++j)
 		{
 			//constTerm += distMat(i,j)*distMat(i,j);
			constTerm += distMat[i*nInput+j]*distMat[i*nInput+j];
 		}
 	}
 
 	fmat V(nInput, nInput);
 	for (int i=0; i<nInput; ++i)
 	{
 		for (int j=0; j<=i; ++j)
 		{
 			V(i,j) = (i==j?nInput-1:-1);
 			V(j,i) = V(i,j);
 		}
 	}
 
 	fmat Z;
 	int nIt = 0;
 	float lastErr = 10e4;
 
 	fmat BZ (nInput, nInput);
 	do
 	{
 		Z = X;
 		//compute error
 		BZ.fill(0);
 		for (int i=0; i<nInput; ++i)
 		{
 			for (int j=0; j<i; ++j)
 			{
 				float d = 0.0;
 				for (int k=0; k<outDim; ++k)
 				{
 					float x1 = Z(i,k), x2 = Z(j,k);
 					d += (Z(i,k)-Z(j,k))*(Z(i,k)-Z(j,k));
 				}
 				if (d==0)
 				{
 					continue;
 				}
 				d = sqrt(d);
 				BZ(i,j) = -1.0*distMat[i*nInput+j]/d;
 				BZ(j,i) = BZ(i,j);
 			}
 		}
 		for (int i=0; i<nInput; ++i)
 		{
 			for (int j=0; j<nInput; ++j)
 			{
 				if (i==j)
 				{
 					continue;
 				}
 				BZ(i,i) -= BZ(i,j);
 			}
 		}
 		if (nIt>0)
 		{
 			X = (1.0/nInput) * BZ*Z;
 			lastErr = err;
 		}
 		float XVX = trace(trans(X)*V*X);
 		float XBZ = trace(trans(X)*BZ*Z);
 		err = constTerm + XVX - 2*XBZ;
 		++nIt;
 		if (nIt > 10000)
 		{
 			break;
 		}
 	} while (nIt<=1 || lastErr-err >= thres);
 
 	for (int i=0;i <nInput; ++i)
 	{
 		for (int j=0; j<outDim; ++j)
 		{
 			outPoints[i*outDim+j] = X(i,j);
 		}
 	}
 
 	return err;
}

//float MDS_metric (fmat &distMat, int outDim, float *outPoints)
//{
//	int nInput = distMat.n_rows;
//	fmat H(nInput, nInput);
//	fmat A(nInput, nInput);
//	/*SymmetricMatrix H (nInput);
//	SymmetricMatrix A (nInput);*/
//
//	for (int i=0; i<nInput; ++i)
//	{
//		for (int j=0; j<=i; ++j)
//		{
//			H(i,j) = (i==j?1:0)-1.0/nInput;
//			H(j,i) = H(i,j);
//			double d = distMat(i,j);
//			A(i,j) = -d*d/2;
//			A(j,i) = A(i,j);
//		}
//	}
//	fmat B1 =H*A*H;
//	fmat B = B1;
//
//	fvec eigVal;
//	fmat eigVec;
//	eig_sym(eigVal, eigVec, B);
//
//	for (int i=0; i<nInput; ++i)
//	{
//		if (eigVal(i)<0)
//		{
//			double e = eigVal(i);
//			e = e;
//		}
//		eigVal(i) = eigVal(i)<=0?0:sqrt(eigVal(i));
//	}
//	fmat X1 = eigVec*diagmat(eigVal);
//
//	for (int i=0; i<nInput; ++i)
//	{
//		for (int j=0; j<outDim; ++j)
//		{
//			//X(i,j) = X1.element(i, nInput-1-j);
//			outPoints[i*outDim+j] = X1(i, nInput-1-j);
//		}
//	}
//
//	double err = 0;
//	for (int i=0; i<nInput-outDim; ++i)
//	{
//		err += eigVal(i);
//	}
//	err /= sum(eigVal);
//
//	return err;
//}

float weightedMDS(const float *distMat, const float *weightMatrix, int nInput, int outDim, float *outPoints, float thres = 1.0e-8)
{
	srand(time(NULL));

	//float testVal [] = {-0.266, 0.451, 0.016, -0.2, -0.539, 0.252, -0.238, 0.524};
	fmat X = randu<fmat>(nInput, outDim);
	//fmat X (testVal, 4, 2);

	double err = 10.0e4;

	double constTerm = 0.0;
	for (int i=1; i<nInput; ++i)
	{
		for (int j=0; j<i; ++j)
		{
			constTerm += distMat[i*(i-1)/2+j]*distMat[i*(i-1)/2+j];
		}
	}

	fmat V(nInput, nInput);
	V.fill(0);
	for (int i=1; i<nInput; ++i)
	{
		for (int j=0; j<i; ++j)
		{
				V(i,j) = -weightMatrix[i*(i-1)/2+j];
				V(j,i) = V(i,j);
				V(i,i) -= V(i,j);
				V(j,j) -= V(i,j);
		}
	}
	fmat V1 = inv(V+1)-1.0/nInput/nInput;

	fmat Z;
	int nIt = 0;
	float lastErr = 10e4;

	fmat BZ (nInput, nInput);
	do
	{
		lastErr = err;
		Z = X;
		//compute error
		BZ.fill(0);
		for (int i=1; i<nInput; ++i)
		{
			for (int j=0; j<i; ++j)
			{
				float d = 0.0;
				for (int k=0; k<outDim; ++k)
				{
					float x1 = Z(i,k), x2 = Z(j,k);
					d += (Z(i,k)-Z(j,k))*(Z(i,k)-Z(j,k));
				}
				d = sqrt(d);
				BZ(i,j) = d==0?0:-weightMatrix[i*(i-1)/2+j]*distMat[i*(i-1)/2+j]/d;
				BZ(j,i) = BZ(i,j);
				BZ(i,i) -= BZ(i,j);
				BZ(j,j) -= BZ(i,j);
			}
		}
		X = V1*BZ*Z;

		float XVX = trace(trans(X)*V*X);
		float XBZ = trace(trans(X)*BZ*Z);
		err = constTerm + XVX - 2*XBZ;
		++nIt;
		if (nIt > 10000)
		{
			break;
		}
	} while (nIt<=1 || lastErr-err >= thres);

	for (int i=0;i <nInput; ++i)
	{
		for (int j=0; j<outDim; ++j)
		{
			outPoints[i*outDim+j] = X(i,j);
		}
	}

	return err;
}

void testMDS ()
{
	float distMat [] = {0,5,3,4,
						5,0,2,2,
						3,2,0,1,
						4,2,1,0};
	float mdsPoints [8];
	MDS(distMat, 4, 2, mdsPoints);
}