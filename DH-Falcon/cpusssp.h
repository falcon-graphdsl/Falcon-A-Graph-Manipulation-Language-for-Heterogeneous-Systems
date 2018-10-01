
#include "cpussspglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>
struct struct_hgraph { 
int   *olddist ;//has to given size of property type
bool  *updated ;//has to given size of property type
int   *dist ;//has to given size of property type
};
void alloc_extra_hgraph(GGraph &hgraph, int flag,int npoints) {
 cudaSetDevice(0);
struct struct_hgraph  temp;
if(flag==0)cudaMalloc((void **)&(hgraph.extra),sizeof(struct struct_hgraph ));
 cudaMemcpy(&temp,((struct struct_hgraph  *)(hgraph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
cudaMalloc( (void **)&(temp.olddist),sizeof(int )* hgraph.npoints); 
cudaMalloc( (void **)&(temp.updated),sizeof(bool)* hgraph.npoints); 
cudaMalloc( (void **)&(temp.dist),sizeof(int )* hgraph.npoints); 
if(cudaMemcpy(hgraph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 0");cudaSetDevice(0);
}
void hgraph_realloc(GGraph &hgraph){
 struct struct_hgraph  temp;
cudaSetDevice(0);
if(cudaMemcpy(&temp,hgraph.extra,sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 1");
if(((hgraph.npoints-hgraph.temppnpoints)*100/hgraph.npoints>50 )||( hgraph.npoints*2>hgraph.maxnpoints)){//
int  *t1=temp.olddist; cudaMalloc((void **)&(temp.olddist),sizeof(int )*hgraph.npoints*2);
if(cudaMemcpy(temp.olddist,t1,sizeof(int )*hgraph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 2");
hgraph.maxnpoints=hgraph.npoints*2;
cudaFree(t1);
}
if(((hgraph.npoints-hgraph.temppnpoints)*100/hgraph.npoints>50 )||( hgraph.npoints*2>hgraph.maxnpoints)){//
bool *t1=temp.updated; cudaMalloc((void **)&(temp.updated),sizeof(bool)*hgraph.npoints*2);
if(cudaMemcpy(temp.updated,t1,sizeof(bool)*hgraph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 3");
hgraph.maxnpoints=hgraph.npoints*2;
cudaFree(t1);
}
if(((hgraph.npoints-hgraph.temppnpoints)*100/hgraph.npoints>50 )||( hgraph.npoints*2>hgraph.maxnpoints)){//
int  *t1=temp.dist; cudaMalloc((void **)&(temp.dist),sizeof(int )*hgraph.npoints*2);
if(cudaMemcpy(temp.dist,t1,sizeof(int )*hgraph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 4");
hgraph.maxnpoints=hgraph.npoints*2;
cudaFree(t1);
}
if(cudaMemcpy(hgraph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 5");
cudaSetDevice(0);

}
cudaDeviceProp prop0;
