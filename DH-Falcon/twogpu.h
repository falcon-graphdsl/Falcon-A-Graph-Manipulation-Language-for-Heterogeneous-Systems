
#include "twogpuglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>
struct struct_hgraph { 
int   *dist1 ;//has to given size of property type
int nchanged;
int maxnchanged;
 int pnchanged;
int   *changed ;//has to given size of property type
int   *dist ;//has to given size of property type
int *owner;
};
void read_hgraph_pptysize(HGraph &hgraph){
printf("enter size of propery changed of hgraph ");
scanf("%d",&(((struct struct_hgraph  *)(hgraph.extra))->nchanged));
}
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints) {
 if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
((struct struct_hgraph  *)hgraph.extra)->dist1=(int  *)malloc(sizeof(int ) * npoints) ;
((struct struct_hgraph  *)hgraph.extra)->changed=(int  *)malloc(sizeof(int ) * ((struct struct_hgraph  *)(hgraph.extra))->nchanged) ;
((struct struct_hgraph  *)hgraph.extra)->dist=(int  *)malloc(sizeof(int ) * npoints) ;
}
void read_and_allocate_hgraph(HGraph  &hgraph ){
printf("enter number of points and edges");
 scanf("%d%d",&(hgraph.npoints),&(hgraph.nedges));
 hgraph.points=(union float_int *)malloc(sizeof(union float_int)*hgraph.npoints);
hgraph.edges=(union float_int *)malloc(sizeof(union float_int)*hgraph.nedges*3);
read_hgraph_pptysize(hgraph);
 alloc_extra_hgraph(hgraph,0);
}
void hgraph_realloc(HGraph &hgraph){
 struct struct_hgraph  temp;
temp=*((struct struct_hgraph  *)(hgraph.extra));
int temppnpoints;
temppnpoints=hgraph.pnpoints;
int temppnedges;
temppnedges=hgraph.pnpoints;
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.dist1;
temp.dist1=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.dist1,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}
if(temp.nchanged*2>temp.maxnchanged){
void *t1=temp.changed;
temp.changed=(int  *)malloc(sizeof(int )*temp.nchanged*2);
memcpy(temp.changed,t1,sizeof(int )*temp.nchanged);
free(t1);temp.maxnchanged=temp.nchanged*2;
}
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.dist;
temp.dist=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.dist,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}

}
void copygraphcurrentsize(GGraph &graph){
 cudaMemcpyToSymbol(falcgraphpoint,&(graph.npoints),0,sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpyToSymbol(falcgraphedge,&(graph.nedges),0,sizeof(int),cudaMemcpyHostToDevice);
struct struct_hgraph  temp;
cudaMemcpy(&temp,graph.extra,sizeof(struct struct_hgraph  ),cudaMemcpyDeviceToHost);
cudaMemcpyToSymbol(&falcgraphchanged,&(temp.nchanged),0,sizeof(int),cudaMemcpyHostToDevice);
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
if(flag==0)cudaMalloc((void **)&(graph.extra),sizeof(struct struct_hgraph ));
 cudaMemcpy(&temp,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
cudaMalloc((void **)&( temp.changed),sizeof(int )* temp.nchanged); 
cudaMalloc((void **)&( temp.owner),sizeof(int)* temp.nchanged); 
cudaMalloc( (void **)&(temp.dist),sizeof(int )* graph.npoints); 
if(cudaMemcpy(graph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 0");
cudaSetDevice(0);
}
void copygraph1currentsize(GGraph &graph){
 cudaMemcpyToSymbol(falcgraph1point,&(graph.npoints),0,sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpyToSymbol(falcgraph1edge,&(graph.nedges),0,sizeof(int),cudaMemcpyHostToDevice);
struct struct_hgraph  temp;
cudaMemcpy(&temp,graph.extra,sizeof(struct struct_hgraph  ),cudaMemcpyDeviceToHost);
cudaMemcpyToSymbol(&falcgraph1changed,&(temp.nchanged),0,sizeof(int),cudaMemcpyHostToDevice);
}
__device__ int falcgraph1addpointgraph(){
 int t1=atomicAdd(&falcgraph1point,1);
 return t1;
}
__device__ int falcgraph1addedgegraph(){
 int t1=atomicAdd(&falcgraph1edge,1);
 return t1;
}
void alloc_extra_graph1(GGraph &graph1,int flag,int npoints) {/*
cudaSetDevice(1);*/
 struct struct_hgraph  temp;
if(flag==0)cudaMalloc((void **)&(graph1.extra),sizeof(struct struct_hgraph ));
 cudaMemcpy(&temp,((struct struct_hgraph  *)(graph1.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
cudaMalloc((void **)&( temp.changed),sizeof(int )* temp.nchanged); 
cudaMalloc((void **)&( temp.owner),sizeof(int)* temp.nchanged); 
cudaMalloc( (void **)&(temp.dist),sizeof(int )* graph1.npoints); 
if(cudaMemcpy(graph1.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1");
cudaSetDevice(0);
}
cudaDeviceProp prop1;
