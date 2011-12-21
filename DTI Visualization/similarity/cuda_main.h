#ifndef CUDA_MAIN_H
#define CUDA_MAIN_H
#define CUDA_BlockSize_X 16
#define CUDA_BlockSize_Y 16
#define CUDA_BlockSize 256
extern "C"{
	float fiber_similarity(float* f1, float* f2, int n1, int n2);
	//float fiber_similarity_(float3* f1, float3* f2, int n1, int n2);
	void bind_fiber(float* fiber_data,int vn);
	void unbind_fiber();
	void similarity_with_binded_fiber(float* fiber, int vn,float * min_array);
	//void getTransformaitonMap(float* subjectMat,int sizeX,int sizeY,int sizeZ,float* atlasMat, int atlasSizeX, int atlasSizeY, int atlasSizeZ,float* flow_field,float* _normMap);
	void transformBindedPoints(float* fiberPoints, int vn, float* subjectMat,float* atlasMat, int atlasSizeX, int atlasSizeY, int atlasSizeZ,float* flow_field);
};
#endif