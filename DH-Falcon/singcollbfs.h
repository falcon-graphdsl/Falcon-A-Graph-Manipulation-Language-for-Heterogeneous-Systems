
#include "singcollbfsglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>
struct struct_hgraph { 
int   *dist ;//has to given size of property type
};
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints) {
 if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
((struct struct_hgraph  *)hgraph.extra)->dist=(int  *)malloc(sizeof(int ) * npoints) ;
}
void read_and_allocate_hgraph(HGraph  &hgraph ){
printf("enter number of points and edges");
 scanf("%d%d",&(hgraph.npoints),&(hgraph.nedges));
 hgraph.points=(union float_int *)malloc(sizeof(union float_int)*hgraph.npoints);
hgraph.edges=(union float_int *)malloc(sizeof(union float_int)*hgraph.nedges*3);
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
//communication buffer datatype
 struct FALCbuffer{
      int *vid;
};
int FALCrank;//rank of process
char partitionfile[100];//second input
 char FALChostname[256];//name of host on which process running
int FALCsize;//total processes launched
 MPI_Status *FALCstatus,*FALCgpustatus;//used for MPI_Recv
MPI_Request *FALCrequest,*FALCcpurequest;//Used for MPI_Isend
int *FALCsendsize,*FALCgpusendsize;//send buffer size(for sending to remote machines)
 int *FALCrecvsize,*FALCgpurecvsize;
int FALCmsgno,FALCgpumsgno;//message number for messages used in code
 int FALCnamount,FALCgpunamount;
struct FALCbuffer *FALCsendbuff,*FALCgpusendbuff,*FALCrecvbuff,*FALCgpurecvbuff;//send and receive buffer for synchronizing global state
//allocate buffer for communication 
void FALCgpuallocbuff(struct FALCbuffer *buff,int tot,int size){
    struct FALCbuffer temp;
    for(int i=0;i<tot;i++){
        cudaMemcpy( &temp,&buff[i],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        cudaMalloc(&(temp.vid),sizeof(int)*size);
        cudaMemcpy(&buff[i],&temp,sizeof(struct FALCbuffer),cudaMemcpyHostToDevice);
        }
}
//allocate buffer for communication 
void FALCallocbuff(struct FALCbuffer *buff,int tot,int size){
        for(int i=0;i<tot;i++){
            buff[i].vid=(int *)malloc(sizeof(int)*size);
        }
}
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
if(flag==0)cudaMalloc((void **)&(graph.extra),sizeof(struct struct_hgraph ));
 cudaMemcpy(&temp,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
cudaMalloc( (void **)&(temp.dist),sizeof(int )* graph.npoints); 
if(cudaMemcpy(graph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 0");
cudaSetDevice(0);
}

void alloccoll1size(GColl &coll1){ cudaMalloc((void **)&(coll1.size),sizeof(int));
int temp=0;
cudaMemcpy(&(coll1.size[0]),&temp,sizeof(int),cudaMemcpyHostToDevice);
}

void alloccoll2size(GColl &coll2){ cudaMalloc((void **)&(coll2.size),sizeof(int));
int temp=0;
cudaMemcpy(&(coll2.size[0]),&temp,sizeof(int),cudaMemcpyHostToDevice);
}

void alloccoll3size(GColl &coll3){ cudaMalloc((void **)&(coll3.size),sizeof(int));
int temp=0;
cudaMemcpy(&(coll3.size[0]),&temp,sizeof(int),cudaMemcpyHostToDevice);
}
__device__ int   changed =0,GPU_changed, coll1cnt =0,GPU_coll1cnt, coll2cnt =0,GPU_coll2cnt,GPU_hchanged;
//BEGIN
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints+graph.remotepoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=1234567890; 

 }//end fun 0 

}
__global__ void   relaxgraph ( GGraph  graph ,GColl coll1,GColl coll2,int   val ,int FALCX) 
 {
int id;
 if(threadIdx.x+blockDim.x*blockIdx.x>=coll1.size)return; 
id=coll1.ptr[threadIdx.x+blockDim.x*blockIdx.x];
if( id < graph.localpoints){

 int falcft3=graph.index[id+1]-graph.index[id];
int falcft4=graph.index[id];
/*XX*/for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
if( ((struct struct_hgraph  *)(graph.extra))->dist/*here1*//*xx*/[ut4]>(val/*here1*/+1) ){

 ((struct struct_hgraph  *)(graph.extra))->dist/*here1*//*xx*/[ut4]=val/*here1*/+1; 

 int falcvt8;

falcvt8=GADD(&(coll2.size[0]),1);
coll2.ptr[falcvt8]=t/*here1*/; 

 GPU_changed=1; 

 }//end

 }//foreach

 }//end fun 0 

}
__global__ void   relaxgraph ( GGraph  graph ,GColl coll1,GColl coll2,int   val ,int FALCX) 
 {
int id;
 if(threadIdx.x+blockDim.x*blockIdx.x>=coll1.size)return; 
id=coll1.ptr[threadIdx.x+blockDim.x*blockIdx.x];
if( id < graph.localpoints){

 int falcft6=graph.index[id+1]-graph.index[id];
int falcft7=graph.index[id];
/*XX*/for(int falcft8=0;falcft8<falcft6;falcft8++){
int ut6=2*(falcft7+falcft8);
 int ut7=graph.edges[ut6].ipe;
int ut8=graph.edges[ut6+1].ipe;
if( ((struct struct_hgraph  *)(graph.extra))->dist/*here1*//*xx*/[ut7]>(val/*here1*/+1) ){

 ((struct struct_hgraph  *)(graph.extra))->dist/*here1*//*xx*/[ut7]=val/*here1*/+1; 

 int falcvt9;

falcvt9=GADD(&(coll2.size[0]),1);
coll2.ptr[falcvt9]=t/*here1*/; 

 GPU_changed=1; 

 }//end

 }//foreach

 }//end fun 0 

}
//END
