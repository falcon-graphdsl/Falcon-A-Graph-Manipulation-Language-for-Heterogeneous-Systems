
#include "newgpusssp1global.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include "HSetOPT.h"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
struct struct_hgraph { 
    int   *dist ;//has to given size of property type
    int   *olddist ;//has to given size of property type
    bool  *updated ;//has to given size of property type
};
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints) {
     if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
    ((struct struct_hgraph  *)hgraph.extra)->dist=(int  *)malloc(sizeof(int ) * npoints) ;
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
temppnpoints=hgraph.pnpoints;
int temppnedges;
temppnedges=hgraph.pnpoints;
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.dist;
temp.dist=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.dist,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}

}
//resetgraph hgraph
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
    cudaMalloc( (void **)&(temp.olddist),sizeof(int )* graph.npoints); 
    cudaMalloc( (void **)&(temp.updated),sizeof(bool)* graph.npoints); 
    cudaMalloc( (void **)&(temp.dist),sizeof(int )* graph.npoints); 
    if(cudaMemcpy(graph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)
        printf("memcpyerror 5");
    cudaSetDevice(0);
}
