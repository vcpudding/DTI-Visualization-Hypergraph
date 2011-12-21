/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and 
 * proprietary rights in and to this software and related documentation. 
 * Any use, reproduction, disclosure, or distribution of this software 
 * and related documentation without an express license agreement from
 * NVIDIA Corporation is strictly prohibited.
 *
 * Please refer to the applicable NVIDIA end user license agreement (EULA) 
 * associated with this source code for terms and conditions that govern 
 * your use of this NVIDIA software.
 * 
 */

 /* This example demonstrates how to use the Cuda OpenGL bindings with the
  * runtime API.
  * Device code.
  */

#ifndef _MAIN_KERNEL_H_
#define _MAIN_KERNEL_H_
#include "cuda_main.h"
#include <cutil_inline.h>
float3* d_binded_fiber;
int binded_fiber_vn=0;
__constant__ float d_subjectMat[16];
__constant__ float d_atlasMat[16];
///////////////////////////////////////////////////////////////////////////////
//! Simple kernel to modify vertex positions in sine wave pattern
//! @param data  data in global memory
///////////////////////////////////////////////////////////////////////////////
__device__ float3 matrixMultVector(float *m,float3 v)//rv=m*v
{
	float3 rv;
 	//rv.x=m[0]*v.x+m[1]*v.y+m[2]*v.z+m[3];
 	//rv.y=m[4]*v.x+m[5]*v.y+m[6]*v.z+m[7];
 	//rv.z=m[8]*v.x+m[9]*v.y+m[10]*v.z+m[11];
	rv.x=m[0]*v.x+m[4]*v.y+m[8]*v.z+m[12];
	rv.y=m[1]*v.x+m[5]*v.y+m[9]*v.z+m[13];
	rv.z=m[2]*v.x+m[6]*v.y+m[10]*v.z+m[14];
	return rv;
	//rv.w=m[12]*v.x+m[13]*v.y+m[14]*v.z+m[15]*v.w;
}
__global__ void _transformBindedPoints(float3* bindedPoints,int vn, int atlasSizeX, int atlasSizeY,int atlasSizeZ,float3* d_flow_field, int step)
{
    unsigned int index = blockIdx.x*blockDim.x + threadIdx.x;
	int nPerAtlasSlice=atlasSizeX*atlasSizeY;
	if(index<vn)
	{
		//the coordinate begin with 1
		//////////////////////////////////////////////////////////////////////////
 	//	float3 current_x1=bindedPoints[index];
		//current_x1=make_float3(xIndex+1,yIndex+1,zIndex+1);
 	//	float3 current_global_coord1=matrixMultVector(d_subjectMat,current_x1);
 	//	float3 current_atlas_coord1=matrixMultVector(d_atlasMat,current_global_coord1);
 	//	d_normMap[index]=current_atlas_coord1;//make_float3(1,1,1);
 	//	return;
		//////////////////////////////////////////////////////////////////////////
		float3 current_x=bindedPoints[index];
		current_x=make_float3(current_x.x+1,current_x.y+1,current_x.z+1);
		float3 current_global_coord=matrixMultVector(d_subjectMat,current_x);
		float3 current_atlas_coord;//=matrixMultVector(d_atlasMat,current_global_coord);
		int3 current_atlas_index;//=make_int3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
		float3 current_field;//=d_flow_field[current_atlas_index.z*nPerAtlasSlice+current_atlas_index.y*atlasSizeX+current_atlas_index.x];
		
		for(int i=0;i<step;++i)
		{
			current_atlas_coord=matrixMultVector(d_atlasMat,current_global_coord);
			current_atlas_index=make_int3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
			if(current_atlas_index.x>=atlasSizeX||current_atlas_index.y>=atlasSizeY||current_atlas_index.z>=atlasSizeZ)
			{
				bindedPoints[index]=make_float3(-1,-1,-1);
				return;
			}
			current_field=d_flow_field[current_atlas_index.z*nPerAtlasSlice+current_atlas_index.y*atlasSizeX+current_atlas_index.x];
			current_global_coord=make_float3(current_global_coord.x+current_field.x/step,current_global_coord.y+current_field.y/step,current_global_coord.z+current_field.z/step);			
		}
		current_atlas_coord=matrixMultVector(d_atlasMat,current_global_coord);
		current_atlas_coord=make_float3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
		bindedPoints[index]=current_atlas_coord;
	}
}
__global__ void get_Transformaiton_Map(int sizeX,int sizeY,int sizeZ,int atlasSizeX, int atlasSizeY,int atlasSizeZ,float3* d_flow_field,float3* d_normMap, int step)
{
    unsigned int index = blockIdx.x*blockDim.x + threadIdx.x;
	unsigned int idx=index;
	int nPerSlice=sizeX*sizeY;
	int zIndex=idx/nPerSlice;
	idx=idx-zIndex*nPerSlice;
	int yIndex=idx/sizeX;
	int xIndex=idx-yIndex*sizeX;
	int nPerAtlasSlice=atlasSizeX*atlasSizeY;
	if(zIndex<sizeZ)
	{
		//the coordinate begin with 1
		//////////////////////////////////////////////////////////////////////////
 		float3 current_x1=make_float3(xIndex+1,yIndex+1,zIndex+1);
 		float3 current_global_coord1=matrixMultVector(d_subjectMat,current_x1);
 		float3 current_atlas_coord1=matrixMultVector(d_atlasMat,current_global_coord1);
 		d_normMap[index]=current_atlas_coord1;//make_float3(1,1,1);

		//////////////////////////////////////////////////////////////////////////
		//float3 current_x=make_float3(xIndex+1,yIndex+1,zIndex+1);
		//float3 current_global_coord=matrixMultVector(d_subjectMat,current_x);
		//float3 current_atlas_coord;//=matrixMultVector(d_atlasMat,current_global_coord);
		//int3 current_atlas_index;//=make_int3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
		//float3 current_field;//=d_flow_field[current_atlas_index.z*nPerAtlasSlice+current_atlas_index.y*atlasSizeX+current_atlas_index.x];
		//
		//for(int i=0;i<step;++i)
		//{
		//	current_atlas_coord=matrixMultVector(d_atlasMat,current_global_coord);
		//	current_atlas_index=make_int3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
		//	if(current_atlas_index.x>=atlasSizeX||current_atlas_index.y>=atlasSizeY||current_atlas_index.z>=atlasSizeZ)
		//	{
		//		d_normMap[index]=make_float3(-1,-1,-1);
		//		return;
		//	}
		//	current_field=d_flow_field[current_atlas_index.z*nPerAtlasSlice+current_atlas_index.y*atlasSizeX+current_atlas_index.x];
		//	current_global_coord=make_float3(current_global_coord.x+current_field.x/step,current_global_coord.y+current_field.y/step,current_global_coord.z+current_field.z/step);			
		//}
		//current_atlas_coord=matrixMultVector(d_atlasMat,current_global_coord);
		//current_atlas_coord=make_float3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
		//d_normMap[index]=current_atlas_coord;
		float3 current_x=make_float3(xIndex+1,yIndex+1,zIndex+1);
		float3 current_global_coord=matrixMultVector(d_subjectMat,current_x);
		float3 current_atlas_coord;//=matrixMultVector(d_atlasMat,current_global_coord);
		int3 current_atlas_index;//=make_int3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
		float3 current_field;//=d_flow_field[current_atlas_index.z*nPerAtlasSlice+current_atlas_index.y*atlasSizeX+current_atlas_index.x];
		
		for(int i=0;i<step;++i)
		{
			current_atlas_coord=matrixMultVector(d_atlasMat,current_global_coord);
			current_atlas_index=make_int3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
			if(current_atlas_index.x>=atlasSizeX||current_atlas_index.y>=atlasSizeY||current_atlas_index.z>=atlasSizeZ)
			{
				d_normMap[index]=make_float3(-1,-1,-1);
				return;
			}
			current_field=d_flow_field[current_atlas_index.z*nPerAtlasSlice+current_atlas_index.y*atlasSizeX+current_atlas_index.x];
			current_global_coord=make_float3(current_global_coord.x+current_field.x/step,current_global_coord.y+current_field.y/step,current_global_coord.z+current_field.z/step);			
		}
		current_atlas_coord=matrixMultVector(d_atlasMat,current_global_coord);
		current_atlas_coord=make_float3(current_atlas_coord.x-1,current_atlas_coord.y-1,current_atlas_coord.z-1);
		d_normMap[index].y=current_atlas_coord.x;
	}
}
__global__ void get_matrix(float3* f1, float3* f2, int n1, int n2,float* resultMatrix,int pitch)
{
    unsigned int idx = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int idy = blockIdx.y*blockDim.y + threadIdx.y;
	if(idx<n1&&idy<n2)
	{
		float3 distXYZ=make_float3(f1[idx].x-f2[idy].x,f1[idx].y-f2[idy].y,f1[idx].z-f2[idy].z);
		float dist=distXYZ.x*distXYZ.x+distXYZ.y*distXYZ.y+distXYZ.z*distXYZ.z;
		float* row = (float*)((char*)resultMatrix + idy * pitch);
		row[idx] = dist; 
	}
}
__global__ void get_max(float* input_Matrix,int input_pitch,int input_w, int input_h,float* result_Matrix,int result_pitch,int result_w, int result_h)
{
    unsigned int idx = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int idy = blockIdx.y*blockDim.y + threadIdx.y;	
	if(idx<result_w&&idy<result_h)
	{
		float* input_row = (float*)((char*)input_Matrix + idy * input_pitch);
		float* result_row = (float*)((char*)result_Matrix + idy * result_pitch);
		int src1=idx<<1;
		int src2=src1+1;
		float result=input_row[src1];
		if(src2<input_w)
		{
			float result2=input_row[src2];
			if(result2>result)result=result2;
		}
		result_row[idx] = result; 
	}
}
//shrink column
__global__ void get_min_array(float* input_Matrix,int input_pitch,int input_w, int input_h,float* result_Matrix,int result_pitch, int result_h)
{
    unsigned int idx = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int idy = blockIdx.y*blockDim.y + threadIdx.y;	
	if(idx<input_w&&idy<result_h)
	{
		int src1=idy<<1;
		int src2=src1+1;
		float* input_row1 = (float*)((char*)input_Matrix + src1 * input_pitch);
		float* input_row2 = (float*)((char*)input_Matrix + src2 * input_pitch);
		
		float* result_row = (float*)((char*)result_Matrix + idy * result_pitch);
		float result=input_row1[idx];
		if(src2<input_h)
		{
			float result2=input_row2[idx];
			if(result2<result)result=result2;
		}
		result_row[idx] = result; 
	}
}
__global__ void get_min(float* input_Matrix,int input_pitch, int input_h,float* result_Matrix,int result_pitch, int result_h)
{
    unsigned int idx = blockIdx.x*blockDim.x + threadIdx.x;	
	if(idx<result_h)
	{
		int src1=idx<<1;
		int src2=src1+1;
		float* input_row1 = (float*)((char*)input_Matrix + src1 * input_pitch);
		float* input_row2 = (float*)((char*)input_Matrix + src2 * input_pitch);
		float* result_row = (float*)((char*)result_Matrix + idx * result_pitch);
		float result=input_row1[0];
		if(src2<input_h)
		{
			float result2=input_row2[0];
			if(result2<result)result=result2;
		}
		result_row[0] = result; 
	}
}
void get_min_array(float3* fiber, int vn,float * min_array)
{
    // execute the kernel
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//test
//     float* resultMatrix[2];
// 	resultMatrix[0]=new float[vn*binded_fiber_vn];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	dim3 dimBlock(CUDA_BlockSize_X,CUDA_BlockSize_Y);
	dim3 dimGrid((binded_fiber_vn + dimBlock.x - 1)/dimBlock.x,(vn + dimBlock.y - 1)/dimBlock.y);
	float* d_resultMatrix[2];
	int pitch[2];

	float3* d_fiber;
	CUDA_SAFE_CALL( cudaMalloc((void **) &d_fiber, vn * sizeof(float3)));
	CUDA_SAFE_CALL( cudaMemcpy(d_fiber,fiber, vn * sizeof(float3), cudaMemcpyHostToDevice) ); 	
	int input_h=vn;	
	int result_h=(vn+1)/2;
	CUDA_SAFE_CALL(cudaMallocPitch((void**)&d_resultMatrix[0],(size_t  *) &pitch[0], (size_t)binded_fiber_vn * sizeof(float),(size_t) vn));
	
	get_matrix<<< dimGrid, dimBlock >>>(d_binded_fiber,d_fiber,binded_fiber_vn,vn,d_resultMatrix[0],pitch[0]);
	CUDA_SAFE_CALL(cudaFree(d_fiber));

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//test
// 	cutilSafeCall( cudaMemcpy2D( resultMatrix[0], binded_fiber_vn*sizeof(float), d_resultMatrix[0],pitch[0],sizeof(float)*binded_fiber_vn,vn, cudaMemcpyDeviceToHost) ); 
// 	for(int j=0;j<vn;++j)
// 	{
// 		for(int i=0;i<binded_fiber_vn;++i)
// 		{
// 			printf("%f\t",resultMatrix[0][j*binded_fiber_vn+i]);
// 		}
// 		printf("\n");
// 	}
// 	resultMatrix[1]=new float[result_h*binded_fiber_vn];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	CUDA_SAFE_CALL(cudaMallocPitch((void**)&d_resultMatrix[1],(size_t  *) &pitch[1], (size_t)binded_fiber_vn * sizeof(float),(size_t) result_h));
	int current_result_buffer=1;
	int current_input_buffer=0;
	while(input_h>1)
	{
		dimGrid.y=(result_h + dimBlock.y - 1)/dimBlock.y;
		get_min_array<<< dimGrid, dimBlock >>>( d_resultMatrix[current_input_buffer],pitch[current_input_buffer],binded_fiber_vn, input_h,d_resultMatrix[current_result_buffer],pitch[current_result_buffer], result_h);
		input_h=result_h;
		result_h=(result_h+1)/2;
		current_result_buffer=current_input_buffer;
		current_input_buffer=1-current_input_buffer;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//test
// 		cutilSafeCall( cudaMemcpy2D( resultMatrix[current_input_buffer], binded_fiber_vn*sizeof(float), d_resultMatrix[current_input_buffer],pitch[current_input_buffer],sizeof(float)*binded_fiber_vn,input_h, cudaMemcpyDeviceToHost) ); 
// 		//cutilSafeCall( cudaMemcpy2D( resultMatrix[0], n1*sizeof(float), d_resultMatrix[0],pitch[0],sizeof(float)*n1,n2, cudaMemcpyDeviceToHost) ); 
// 		//cutilSafeCall( cudaMemcpy2D( resultMatrix[1], n12*sizeof(float), d_resultMatrix[1],pitch[1],n12*sizeof(float),n2, cudaMemcpyDeviceToHost) ); 
// 		printf("\n");
// 		for(int j=0;j<input_h;++j)
// 		{
// 			for(int i=0;i<binded_fiber_vn;++i)
// 			{
// 				printf("%f\t",resultMatrix[current_input_buffer][j*binded_fiber_vn+i]);
// // 				if(current_input_buffer==0)
// // 					printf("%f\t",resultMatrix[0][j*n1+i]);
// // 				else
// // 					printf("%f\t",resultMatrix[1][j*n12+i]);
// 			}
// 			printf("\n");
// 		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	cutilSafeCall( cudaMemcpy2D( min_array, binded_fiber_vn*sizeof(float), d_resultMatrix[current_input_buffer],pitch[current_input_buffer],sizeof(float)*binded_fiber_vn,1, cudaMemcpyDeviceToHost) ); 
	CUDA_SAFE_CALL(cudaFree(d_resultMatrix[0]));
	CUDA_SAFE_CALL(cudaFree(d_resultMatrix[1]));
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//test
// 	delete [] resultMatrix[0];
// 	delete [] resultMatrix[1];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
}
float fiber_similarity_(float3* f1, float3* f2, int n1, int n2)
{
    // execute the kernel
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//test
//     float* resultMatrix[2];
// 	resultMatrix[0]=new float[n1*n2];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	dim3 dimBlock(CUDA_BlockSize_X,CUDA_BlockSize_Y);
	dim3 dimGrid((n1 + dimBlock.x - 1)/dimBlock.x,(n2 + dimBlock.y - 1)/dimBlock.y);
	float* d_resultMatrix[2];
	int pitch[2];
	float3* d_f1;
	float3* d_f2;
	CUDA_SAFE_CALL( cudaMalloc((void **) &d_f1, n1 * sizeof(float3))); 
	CUDA_SAFE_CALL( cudaMalloc((void **) &d_f2, n2 * sizeof(float3)));
	CUDA_SAFE_CALL( cudaMemcpy(d_f1,f1, n1 * sizeof(float3), cudaMemcpyHostToDevice) ); 	
	CUDA_SAFE_CALL( cudaMemcpy(d_f2,f2, n2 * sizeof(float3), cudaMemcpyHostToDevice) ); 	
	int input_w=n1;
	
	int result_w=(n1+1)/2;
	CUDA_SAFE_CALL(cudaMallocPitch((void**)&d_resultMatrix[0],(size_t  *) &pitch[0], (size_t)n1 * sizeof(float),(size_t) n2));
	
	get_matrix<<< dimGrid, dimBlock >>>(d_f1,d_f2,n1,n2,d_resultMatrix[0],pitch[0]);
	CUDA_SAFE_CALL(cudaFree(d_f1));
	CUDA_SAFE_CALL(cudaFree(d_f2));
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//test
// 	cutilSafeCall( cudaMemcpy2D( resultMatrix[0], n1*sizeof(float), d_resultMatrix[0],pitch[0],sizeof(float)*n1,n2, cudaMemcpyDeviceToHost) ); 
// 	for(int j=0;j<n2;++j)
// 	{
// 		for(int i=0;i<n1;++i)
// 		{
// 			printf("%f\t",resultMatrix[0][j*n1+i]);
// 		}
// 		printf("\n");
// 	}
// 	resultMatrix[1]=new float[result_w*n2];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	CUDA_SAFE_CALL(cudaMallocPitch((void**)&d_resultMatrix[1],(size_t  *) &pitch[1], result_w * sizeof(float),(size_t) n2));
	int current_result_buffer=1;
	int current_input_buffer=0;
	while(input_w>1)
	{
		dimGrid.x=(result_w + dimBlock.x - 1)/dimBlock.x;
		get_max<<< dimGrid, dimBlock >>>( d_resultMatrix[current_input_buffer],pitch[current_input_buffer],input_w, n2,d_resultMatrix[current_result_buffer],pitch[current_result_buffer],result_w, n2);
		input_w=result_w;
		result_w=(result_w+1)/2;
		current_result_buffer=current_input_buffer;
		current_input_buffer=1-current_input_buffer;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//test
// 		cutilSafeCall( cudaMemcpy2D( resultMatrix[current_input_buffer], input_w*sizeof(float), d_resultMatrix[current_input_buffer],pitch[current_input_buffer],sizeof(float)*input_w,n2, cudaMemcpyDeviceToHost) ); 
// 		//cutilSafeCall( cudaMemcpy2D( resultMatrix[0], n1*sizeof(float), d_resultMatrix[0],pitch[0],sizeof(float)*n1,n2, cudaMemcpyDeviceToHost) ); 
// 		//cutilSafeCall( cudaMemcpy2D( resultMatrix[1], n12*sizeof(float), d_resultMatrix[1],pitch[1],n12*sizeof(float),n2, cudaMemcpyDeviceToHost) ); 
// 		printf("\n");
// 		for(int j=0;j<n2;++j)
// 		{
// 			for(int i=0;i<input_w;++i)
// 			{
// 				printf("%f\t",resultMatrix[current_input_buffer][j*input_w+i]);
// // 				if(current_input_buffer==0)
// // 					printf("%f\t",resultMatrix[0][j*n1+i]);
// // 				else
// // 					printf("%f\t",resultMatrix[1][j*n12+i]);
// 			}
// 			printf("\n");
// 		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	int threadsPerBlock = CUDA_BlockSize;
	int blocksPerGrid;// = (n2 + threadsPerBlock - 1) / threadsPerBlock;
	int input_h=n2;
	int result_h=(n2+1)/2;
	while(input_h>1)
	{		
		blocksPerGrid = (result_h + threadsPerBlock - 1) / threadsPerBlock;
		get_min<<<blocksPerGrid, threadsPerBlock>>>(d_resultMatrix[current_input_buffer],pitch[current_input_buffer], input_h,d_resultMatrix[current_result_buffer],pitch[current_result_buffer], result_h);
		input_h=result_h;
		result_h=(result_h+1)/2;
		current_result_buffer=current_input_buffer;
		current_input_buffer=1-current_input_buffer;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//test
// 		cutilSafeCall( cudaMemcpy2D( resultMatrix[current_input_buffer], sizeof(float), d_resultMatrix[current_input_buffer],pitch[current_input_buffer],sizeof(float),input_h, cudaMemcpyDeviceToHost) ); 
// 		//cutilSafeCall( cudaMemcpy2D( resultMatrix[0], n1*sizeof(float), d_resultMatrix[0],pitch[0],sizeof(float)*n1,n2, cudaMemcpyDeviceToHost) ); 
// 		//cutilSafeCall( cudaMemcpy2D( resultMatrix[1], n12*sizeof(float), d_resultMatrix[1],pitch[1],n12*sizeof(float),n2, cudaMemcpyDeviceToHost) ); 
// 		printf("\n");
// 		for(int j=0;j<input_h;++j)
// 		{
// 			printf("%f\t",resultMatrix[current_input_buffer][j]);
// // 			if(current_input_buffer==0)
// // 				printf("%f\t",resultMatrix[0][j*n1]);
// // 			else
// // 				printf("%f\t",resultMatrix[1][j*n12]);
// 			printf("\n");
// 		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	float result_;
	cutilSafeCall( cudaMemcpy2D( &result_, sizeof(float), d_resultMatrix[current_input_buffer],pitch[current_input_buffer],sizeof(float),1, cudaMemcpyDeviceToHost) ); 
	CUDA_SAFE_CALL(cudaFree(d_resultMatrix[0]));
	CUDA_SAFE_CALL(cudaFree(d_resultMatrix[1]));
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//test
// 	delete [] resultMatrix[0];
// 	delete [] resultMatrix[1];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	return result_;
	
}
// Wrapper for the __global__ call that sets up the kernel call
float fiber_similarity(float* f1, float* f2, int n1, int n2)
{
	float3* f1_3=(float3*)f1;
	float3* f2_3=(float3*)f2;
	return fiber_similarity_(f1_3,f2_3,  n1,  n2);
}
void bind_fiber(float* fiber_data,int vn)
{
	if(binded_fiber_vn!=0)
		unbind_fiber();
	float3* binded_fiber=(float3*)fiber_data;
	binded_fiber_vn=vn;
	CUDA_SAFE_CALL( cudaMalloc((void **) &d_binded_fiber, vn * sizeof(float3))); 
	CUDA_SAFE_CALL( cudaMemcpy(d_binded_fiber,binded_fiber, vn * sizeof(float3), cudaMemcpyHostToDevice) ); 		
}
void unbind_fiber()
{
	CUDA_SAFE_CALL(cudaFree(d_binded_fiber));
	binded_fiber_vn=0;
}
void similarity_with_binded_fiber(float* fiber, int vn,float * min_array)
{
	float3* fiber_3=(float3*)fiber;
	get_min_array(fiber_3, vn,min_array);
}
void getTransformaitonMap(float* subjectMat,int sizeX,int sizeY,int sizeZ,float* atlasMat, int atlasSizeX, int atlasSizeY, int atlasSizeZ,float* flow_field,float* _normMap)
{
	unsigned int subject_size=sizeX*sizeY*sizeZ;
	cutilSafeCall(cudaMemcpyToSymbol("d_subjectMat", (const void *)subjectMat, sizeof(float)*16, 0, cudaMemcpyHostToDevice));
	cutilSafeCall(cudaMemcpyToSymbol("d_atlasMat", (const void *)atlasMat, sizeof(float)*16, 0, cudaMemcpyHostToDevice));
	float3* d_normMap;
	CUDA_SAFE_CALL( cudaMalloc((void **) &d_normMap, subject_size * sizeof(float3))); 
	float3* d_flow_field;
	CUDA_SAFE_CALL( cudaMalloc((void **) &d_flow_field, atlasSizeX*atlasSizeY*atlasSizeZ * sizeof(float3))); 
	CUDA_SAFE_CALL( cudaMemcpy(d_flow_field,(float3*)flow_field, atlasSizeX*atlasSizeY*atlasSizeZ * sizeof(float3), cudaMemcpyHostToDevice) ); 
	dim3 dimBlock(CUDA_BlockSize);
	dim3 dimGrid(( subject_size+ dimBlock.x - 1)/dimBlock.x);
	get_Transformaiton_Map<<< dimGrid, dimBlock >>>(sizeX,sizeY,sizeZ,atlasSizeX, atlasSizeY,atlasSizeZ,d_flow_field, d_normMap,  64);
	CUDA_SAFE_CALL( cudaMemcpy((float3*)_normMap,d_normMap, subject_size * sizeof(float3), cudaMemcpyDeviceToHost) ); 
	////////////////////////////////////////////////////////////////////////////
// 	float3* debugmap=(float3*)_normMap;
// 	int index=0;
// 	for(int k=0;k<sizeZ;k++)
// 	{
// 		for(int j=0;j<sizeY;j++)
// 		{
// 			for(int i=0;i<sizeX;i++)
// 			{
// 				if(debugmap[index].x>atlasSizeX||debugmap[index].y>atlasSizeY||debugmap[index].z>atlasSizeZ)
// 				{
// 					printf("error");
// 				}
// 				++index;
// 			}
// 		}
// 	}
	////////////////////////////////////////////////////////////////////////////
	CUDA_SAFE_CALL(cudaFree(d_normMap));
	CUDA_SAFE_CALL(cudaFree(d_flow_field));
		
}
 void transformBindedPoints(float* fiberPoints, int vn, float* subjectMat,float* atlasMat, int atlasSizeX, int atlasSizeY, int atlasSizeZ,float* flow_field)
 {
	//if(binded_fiber_vn!=vn)
	{
		bind_fiber(fiberPoints,vn);
	}
 	cutilSafeCall(cudaMemcpyToSymbol("d_subjectMat", (const void *)subjectMat, sizeof(float)*16, 0, cudaMemcpyHostToDevice));
 	cutilSafeCall(cudaMemcpyToSymbol("d_atlasMat", (const void *)atlasMat, sizeof(float)*16, 0, cudaMemcpyHostToDevice));
 	float3* d_flow_field;
 	CUDA_SAFE_CALL( cudaMalloc((void **) &d_flow_field, atlasSizeX*atlasSizeY*atlasSizeZ * sizeof(float3))); 
 	CUDA_SAFE_CALL( cudaMemcpy(d_flow_field,(float3*)flow_field, atlasSizeX*atlasSizeY*atlasSizeZ * sizeof(float3), cudaMemcpyHostToDevice) ); 
 	dim3 dimBlock(CUDA_BlockSize);
 	dim3 dimGrid(( binded_fiber_vn+ dimBlock.x - 1)/dimBlock.x);
	_transformBindedPoints<<< dimGrid, dimBlock >>>(d_binded_fiber,binded_fiber_vn,atlasSizeX, atlasSizeY,atlasSizeZ,d_flow_field,  64);
 	CUDA_SAFE_CALL( cudaMemcpy((float3*)fiberPoints,d_binded_fiber, vn * sizeof(float3), cudaMemcpyDeviceToHost) ); 
 	////////////////////////////////////////////////////////////////////////////
 // 	float3* debugmap=(float3*)_normMap;
 // 	int index=0;
 // 	for(int k=0;k<sizeZ;k++)
 // 	{
 // 		for(int j=0;j<sizeY;j++)
 // 		{
 // 			for(int i=0;i<sizeX;i++)
 // 			{
 // 				if(debugmap[index].x>atlasSizeX||debugmap[index].y>atlasSizeY||debugmap[index].z>atlasSizeZ)
 // 				{
 // 					printf("error");
 // 				}
 // 				++index;
 // 			}
 // 		}
 // 	}
 	////////////////////////////////////////////////////////////////////////////
 	CUDA_SAFE_CALL(cudaFree(d_flow_field));
 		
 }
#endif // #ifndef _SIMPLEGL_KERNEL_H_
