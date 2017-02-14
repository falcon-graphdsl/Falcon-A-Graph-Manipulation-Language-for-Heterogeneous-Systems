#ifndef GGRAPH_H
#define GGRAPH_H
#include "Graph.h"
#include <stdio.h>
#include </usr/local/cuda/include/cuda.h>
    #include </usr/local/cuda/include/cuda_runtime_api.h>
class HGraph;
class GGraph: public Graph {
public:
//int *maxnpoints,*pnpoints,*maxnedges,*pnedges;
int maxnpoints,*pnpoints,maxnedges,*pnedges;//made this as only pnpoints and pnedges are used in atomic.
int allocate();
 int copytoCPU(HGraph &hgraph);
int  copyfromCPU(HGraph &hgraph);
 int cloneCPU(HGraph &hgraph);
 int deallocate();
//__device__  int addPoint(int x,int y);
//__constant__ union float_int   *edges;
__device__ int getEdgeWeight(int t1,int t2);
__device__ int addPoint(float x,float y);
__device__ int addPoint(int x,int y);
__device__ int addEdge(int p1,int p2);
__device__ int addEdge(int p1,int p2,int weight);
 void (*extra_alloc)(GGraph &);
};
__device__ int GGraph::addPoint(float x,float y){
int x1=1;
int t1;
t1=atomicAdd(pnpoints,x1);
//if(t1<*(maxnpoints)){
if(t1<(maxnpoints)){
points[2*t1].fpe=x;
points[2*t1+1].fpe=y;
}
return t1;
}
__device__ int GGraph::addPoint(int x,int y){
int x1=1;
int t1;
t1=atomicAdd(pnpoints,x1);
//if(t1<*(maxnpoints)){
if(t1<(maxnpoints)){
points[2*t1].ipe=x;
points[2*t1+1].ipe=y;
}
return t1;
}
__device__ int GGraph::addEdge(int p1,int p2){
int x=1;
int t1;
t1=atomicAdd(pnedges,x);
//if(t1<*(maxnpoints)){
if(t1<(maxnpoints)){
edges[3*t1].ipe=p1;
edges[3*t1+1].ipe=p2;
edges[3*t1+2].ipe=0;

}
return t1;
}
__device__ int GGraph::addEdge(int p1,int p2,int weight){
int x=1;
int t1;
t1=atomicAdd(pnedges,x);
//if(t1<*(maxnpoints)){
if(t1<(maxnpoints)){
edges[3*t1].ipe=p1;
edges[3*t1+1].ipe=p2;
edges[3*t1+2].ipe=weight;
}
return t1;
}
//__device__
 volatile unsigned int *arrayin,*arrayout;
__device__ volatile unsigned int ARRU[1024],ARRO[1024];
unsigned int goal;
void alloc_sync_array(){
	if (cudaMalloc((void **)&arrayin, 1024*sizeof(volatile unsigned int )) != cudaSuccess) printf("allocating arrayin failed");
	if (cudaMalloc((void **)&arrayout, 1024*sizeof(volatile unsigned int )) != cudaSuccess) printf("allocating arrayout failed");
}
__device__ void __gpu_sync(unsigned goalVal, volatile unsigned *Arrayin, volatile unsigned *Arrayout) {
	// thread ID in a block
	unsigned tid_in_blk = threadIdx.x * blockDim.y + threadIdx.y;
	unsigned nBlockNum = gridDim.x * gridDim.y;
	unsigned bid = blockIdx.x * gridDim.y + blockIdx.y;
	// only thread 0 is used for synchronization
	if (tid_in_blk == 0) {
		Arrayin[bid] = goalVal;
		__threadfence();
	}
	if (bid == 0) {
		if (tid_in_blk < nBlockNum) {
			while (Arrayin[tid_in_blk] != goalVal){
				//Do nothing here
			}
		}
		__syncthreads();
		if (tid_in_blk < nBlockNum) {
			Arrayout[tid_in_blk] = goalVal;
			__threadfence();
		}
	}
	if (tid_in_blk == 0) {
		while (Arrayout[bid] != goalVal) {
			//Do nothing here
		}
	}
	__syncthreads();
}
/*

__device__ void __gpu_sync(unsigned goalVal, volatile unsigned *Arrayin, volatile unsigned *Arrayout) {
	// thread ID in a block
	unsigned tid_in_blk = threadIdx.x ;//
	unsigned nBlock = gridDim.x ;//0
	unsigned bid = blockIdx.x  ;//0

   if(bid==0){
	if (tid_in_blk == 0) {
		Arrayin[bid] = 1;
		__threadfence();
	}
		__threadfence();
__syncthreads();
for(int pb=tid_in_blk;pb<nBlock;pb+=1){
 while(Arrayin[pb]==0){
__threadfence_block();
}
}
__syncthreads();

		__threadfence();
 
for(int pb=tid_in_blk;pb<nBlock;pb+=1){

		Arrayin[pb] =0;
		__threadfence();
}

}
else {

if(tid_in_blk==0){
Arrayin[bid]=1;

		__threadfence();
 while(Arrayin[bid]==1){
__threadfence_block();
}
}
__syncthreads();
}
}*/
#endif
