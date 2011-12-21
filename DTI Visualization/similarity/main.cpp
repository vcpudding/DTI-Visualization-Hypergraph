#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// includes
#include <cuda_runtime.h>
#include <cutil_inline.h>
#include <cutil_gl_inline.h>
#include <cutil_gl_error.h>
#include <cuda_gl_interop.h>
#include <vector_types.h>

#include "cutil.h"
#include <vector>

#include "cuda_main.h"
int main(int argc, char* argv[])
{
	cudaSetDevice(cutGetMaxGflopsDeviceId());
	//cudaGLSetGLDevice( cutGetMaxGflopsDeviceId() );
	std::vector<float3> f1,f2;
	f1.push_back(make_float3(1,0,0));
	f1.push_back(make_float3(2,0,0));
	f1.push_back(make_float3(3,0,0));
	f1.push_back(make_float3(4,0,0));
	f2.push_back(make_float3(1,1,0));
	f2.push_back(make_float3(2,1,0));
	f2.push_back(make_float3(3,1,0));
	f2.push_back(make_float3(4,1,0));
	float* min_array=new float[f1.size()];
	//float result=fiber_similarity((float*)&f1[0],(float*)&f2[0],f1.size(),f2.size());
	bind_fiber((float*)&f1[0],f1.size());
	similarity_with_binded_fiber((float*)&f2[0],f2.size(),min_array);
	unbind_fiber();
	float result=0;
	for (int i=0;i<f1.size();++i)
	{
		if(result<min_array[i])result=min_array[i];
	}
	printf("similarity:%f",result);
	delete [] min_array;
	cudaThreadExit();
	return 0;
}