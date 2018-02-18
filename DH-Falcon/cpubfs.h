
#include "cpubfsglobal.h"
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

__global__ void update(GGraph hgraph,struct FALCbuffer *recvbuff,int FALCnamount,int kk){
    int id=blockIdx.x*blockDim.x+threadIdx.x;
    if(id <FALCnamount){
        int vertex= recvbuff[kk].vid[id];
    }
}
__device__ int  GPU_changed;
__device__ int  GPU_lev;
//BEGIN
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints+graph.remotepoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=1234567890; 

 }//end fun 0 

}
__global__ void   BFS ( GGraph  graph ,int   lev ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints&& ((struct struct_hgraph  *)(graph.extra))->dist[id]==GPU_lev ){

 

 int falcft3=graph.index[id+1]-graph.index[id];
int falcft4=graph.index[id];
/*XX*/for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
if( ((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut4]>(((struct struct_hgraph  *)(graph.extra))->dist[id]+1) ){

 ((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut4]=((struct struct_hgraph  *)(graph.extra))->dist[id]+1; 

 GPU_changed=1; 

 }//end

 }//foreach

 }//end fun 0 

}
//END
