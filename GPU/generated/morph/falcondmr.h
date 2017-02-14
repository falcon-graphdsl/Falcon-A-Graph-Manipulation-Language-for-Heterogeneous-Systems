
#include "falcondmrglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include "HSetOPT.h"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
struct struct_hgraph { 
int ntriangle;
int maxntriangle;
 int pntriangle;
    struct node  *triangle ;//has to given size of property type
    int *owner;
};
void read_hgraph_pptysize(HGraph &hgraph){
printf("enter size of propery triangle of hgraph ");
scanf("%d",&(((struct struct_hgraph  *)(hgraph.extra))->ntriangle));
}
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints)/*symtab37*/ {
     if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
    ((struct struct_hgraph  *)hgraph.extra)->triangle=(struct node *)malloc(sizeof(struct node) * ((struct struct_hgraph  *)(hgraph.extra))->ntriangle*3) ;
    ((struct struct_hgraph  *)hgraph.extra)->owner=(int *)malloc(sizeof(int) * ((struct struct_hgraph  *)(hgraph.extra))->ntriangle*3) ;
    ((struct struct_hgraph  *)hgraph.extra)->maxntriangle=( ( struct struct_hgraph  *) (hgraph.extra))->ntriangle*3;
}
void read_and_allocate_hgraph(HGraph  &hgraph ){
printf("enter number of points and edges");
 scanf("%d%d",&(hgraph.npoints),&(hgraph.nedges));
 hgraph.points=(union float_int *)malloc(sizeof(union float_int)*hgraph.npoints);
hgraph.edges=(union float_int *)malloc(sizeof(union float_int)*hgraph.nedges*2);
read_hgraph_pptysize(hgraph);
 alloc_extra_hgraph(hgraph,0,hgraph.npoints);
}
void hgraph_realloc(HGraph &hgraph){
 struct struct_hgraph  temp;
temp=*((struct struct_hgraph  *)(hgraph.extra));
int temppnpoints;
int temppnedges;
if(temp.ntriangle*2>temp.maxntriangle){
((struct struct_hgraph  *)(hgraph.extra))->maxntriangle=((struct struct_hgraph  *)(hgraph.extra))->maxntriangle*2;
((struct struct_hgraph  *)(hgraph.extra))->owner=(int *)realloc(((struct struct_hgraph  *)(hgraph.extra))->owner,sizeof(int)*((struct struct_hgraph  *)(hgraph.extra))->maxntriangle);
((struct struct_hgraph  *)(hgraph.extra))->triangle=(struct node *)realloc(((struct struct_hgraph  *)(hgraph.extra))->triangle,sizeof(struct node)*((struct struct_hgraph  *)(hgraph.extra))->maxntriangle);
}

}
//resetgraph hgraph
 int devnhgraphtriangle;
 void resethgraphtriangle(int id,HGraph &hgraph){
( ( struct struct_hgraph  *)(hgraph.extra))->owner[id]=1234567890;
 }
void graph_realloc(GGraph &graph){
 struct struct_hgraph  temp;
cudaSetDevice(0);
if(cudaMemcpy(&temp,graph.extra,sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 0");
if(((temp.ntriangle-temp.pntriangle)*100/temp.ntriangle>50)|| (temp.ntriangle*2>temp.maxntriangle)){
void *t1=temp.triangle;cudaMalloc((void **)&(temp.triangle),sizeof(struct node)*temp.ntriangle*2);
if(cudaMemcpy(temp.triangle,t1,sizeof(struct node)*temp.ntriangle,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 1");
temp.maxntriangle=temp.ntriangle*2;
cudaFree(t1);
}
if(cudaMemcpy(graph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 2");
cudaSetDevice(0);
}
__global__ void resetgraphtriangle(GGraph graph,int FALCX){
int idx=blockDim.x*blockIdx.x+threadIdx.x+FALCX;
 if(idx < ( ( struct struct_hgraph  *)(graph.extra))->ntriangle)
( ( struct struct_hgraph  *)(graph.extra))->owner[idx]=99999999;
 }
__device__ int devngraphtriangle;
void copygraphcurrentsize(GGraph &graph){
 cudaMemcpyToSymbol(falcgraphpoint,&(graph.npoints),0,sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpyToSymbol(falcgraphedge,&(graph.nedges),0,sizeof(int),cudaMemcpyHostToDevice);
    struct struct_hgraph  temp;
    cudaMemcpy(&temp,graph.extra,sizeof(struct struct_hgraph  ),cudaMemcpyDeviceToHost);
    cudaMemcpyToSymbol(&falcgraphtriangle,&(temp.ntriangle),0,sizeof(int),cudaMemcpyHostToDevice);
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
    cudaMalloc((void **)&( temp.triangle),sizeof(struct node)* temp.ntriangle*3); 
cudaMalloc((void **)&( temp.owner),sizeof(int)* temp.ntriangle*3); 
    if(cudaMemcpy(graph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)
        printf("memcpyerror 3");
    cudaSetDevice(0);
}
