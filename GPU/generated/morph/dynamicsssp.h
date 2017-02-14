
#include "dynamicssspglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include "HSetOPT.h"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
struct struct_hgraph { 
    bool  *updated ;//has to given size of property type
    int   *dist ;//has to given size of property type
    int   *olddist ;//has to given size of property type
};
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints)/*symtab37*/ {
     if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
    ((struct struct_hgraph  *)hgraph.extra)->updated=(bool *)malloc(sizeof(bool) * npoints*3) ;
    ((struct struct_hgraph  *)hgraph.extra)->dist=(int  *)malloc(sizeof(int ) * npoints*3) ;
}
void read_and_allocate_hgraph(HGraph  &hgraph ){
printf("enter number of points and edges");
 scanf("%d%d",&(hgraph.npoints),&(hgraph.nedges));
 hgraph.points=(union float_int *)malloc(sizeof(union float_int)*hgraph.npoints);
hgraph.edges=(union float_int *)malloc(sizeof(union float_int)*hgraph.nedges*2);
alloc_extra_hgraph(hgraph,0,hgraph.npoints);
}
void hgraph_realloc(HGraph &hgraph){
 struct struct_hgraph  temp;
temp=*((struct struct_hgraph  *)(hgraph.extra));
int temppnpoints;
int temppnedges;
if(hgraph.npoints*2> hgraph.maxnpoints){
void *t1=temp.updated;
temp.updated=(bool *)malloc(sizeof(bool)*hgraph.npoints*2);
}
if(hgraph.npoints*2> hgraph.maxnpoints){
void *t1=temp.dist;
temp.dist=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
}

}
//resetgraph hgraph
void graph_realloc(GGraph &graph){
 struct struct_hgraph  temp;
cudaSetDevice(0);
if(cudaMemcpy(&temp,graph.extra,sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 0");
if(((graph.npoints-graph.maxnpoints)*100/graph.npoints>50 )||( graph.npoints*2>graph.maxnpoints)){//
void *t1=temp.olddist; cudaMalloc((void **)&(temp.olddist),sizeof(int )*graph.npoints*2);
if(cudaMemcpy(temp.olddist,t1,sizeof(int )*graph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 1");
graph.maxnpoints=graph.npoints*2;
cudaFree(t1);
}
if(((graph.npoints-graph.maxnpoints)*100/graph.npoints>50 )||( graph.npoints*2>graph.maxnpoints)){//
void *t1=temp.updated; cudaMalloc((void **)&(temp.updated),sizeof(bool)*graph.npoints*2);
if(cudaMemcpy(temp.updated,t1,sizeof(bool)*graph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 2");
graph.maxnpoints=graph.npoints*2;
cudaFree(t1);
}
if(((graph.npoints-graph.maxnpoints)*100/graph.npoints>50 )||( graph.npoints*2>graph.maxnpoints)){//
void *t1=temp.dist; cudaMalloc((void **)&(temp.dist),sizeof(int )*graph.npoints*2);
if(cudaMemcpy(temp.dist,t1,sizeof(int )*graph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 3");
graph.maxnpoints=graph.npoints*2;
cudaFree(t1);
}
if(cudaMemcpy(graph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 4");
cudaSetDevice(0);
}
__device__ int devngrapholddist;
__device__ int devngraphupdated;
__device__ int devngraphdist;
void copygraphcurrentsize(GGraph &graph){
 cudaMemcpyToSymbol(falcgraphpoint,&(graph.npoints),0,sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpyToSymbol(falcgraphedge,&(graph.nedges),0,sizeof(int),cudaMemcpyHostToDevice);
}
__device__ int falcgraphaddpointgraph(){
 int t1=atomicAdd(&falcgraphpoint,1);
 return t1;
}
__device__ int falcgraphaddedgegraph(){
 int t1=atomicAdd(&falcgraphedge,1);
 return t1;
}
void alloc_extra_graph(GGraph &graph,int flag,int npoints) {/*
cudaSetDevice(0);*/
     struct struct_hgraph  temp;
     if(flag==0)
        cudaMalloc((void **)&(graph.extra),sizeof(struct struct_hgraph ));
     cudaMemcpy(&temp,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
    cudaMalloc( (void **)&(temp.olddist),sizeof(int )* graph.npoints*3); 
    cudaMalloc( (void **)&(temp.updated),sizeof(bool)* graph.npoints*3); 
    cudaMalloc( (void **)&(temp.dist),sizeof(int )* graph.npoints*3); 
    if(cudaMemcpy(graph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)
        printf("memcpyerror 5");
    cudaSetDevice(0);
}
