
#include "kcoreglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>
struct struct_hgraph { 
int   *updated ;//has to given size of property type
int   *dist1 ;//has to given size of property type
int   *dist ;//has to given size of property type
};
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints) {
 if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
((struct struct_hgraph  *)hgraph.extra)->updated=(int  *)malloc(sizeof(int ) * npoints) ;
((struct struct_hgraph  *)hgraph.extra)->dist1=(int  *)malloc(sizeof(int ) * npoints) ;
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
void *t1=temp.updated;
temp.updated=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.updated,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.dist1;
temp.dist1=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.dist1,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}
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
     int  *dist1;
};
int  *tempdist1, *tempgpudist1;
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
        cudaMalloc(&(temp.dist1),sizeof(int )*size);
        cudaMemcpy(&buff[i],&temp,sizeof(struct FALCbuffer),cudaMemcpyHostToDevice);
        }
        cudaMalloc(&(tempgpudist1),sizeof(int )*size);
}
//allocate buffer for communication 
void FALCallocbuff(struct FALCbuffer *buff,int tot,int size){
        for(int i=0;i<tot;i++){
            buff[i].vid=(int *)malloc(sizeof(int)*size);
            buff[i].dist1=(int  *)malloc(sizeof(int )*size);
        }
        tempdist1=(int  *)malloc(sizeof(int )*size);
}
__global__ void sendprefix(GGraph hgraph,int  *tempdist1){
int id=threadIdx.x+blockDim.x*blockIdx.x;
if(id<(hgraph.localpoints+hgraph.remotepoints)){
tempdist1[id]=((struct struct_hgraph  *)(hgraph.extra))->dist1[id];
}
}
__global__ void sendbuff(GGraph hgraph,int *sendsize,struct FALCbuffer *sendbuff,int  *tempdist1,int kk,int off,int totelems){
    int id=threadIdx.x+blockDim.x*blockIdx.x;
    if(id <totelems){
    int loc=0;
    int flag=0;
    if( (( struct struct_hgraph  *)(hgraph.extra))->dist1[id+off]!=tempdist1[id+off])flag=1;
    if(flag==1){
        loc=atomicAdd(&sendsize[kk],1);
        sendbuff[kk].vid[loc]=hgraph.remotevertexid[id+off];
        sendbuff[kk].dist1[loc]=(( struct struct_hgraph  *)(hgraph.extra))->dist1[id+off];
         tempdist1[id+off]=(( struct struct_hgraph  *)(hgraph.extra))->dist1[id+off];
    }
}
}
void sendbuff(int i,HGraph &hgraph,int *sendsize,struct FALCbuffer *sendbuff,tempdist1,int kk);
    int loc=0;
    int flag=0;
    if( (( struct struct_hgraph  *)(hgraph.extra))->dist1[i]!=tempdist1[i])flag=1;
    if(flag==1){
        loc=__sync_fetch_and_add(&sendsize[kk],1);
        sendbuff[kk].vid[loc]=hgraph.remotevertexid[i];
        sendbuff[kk].dist1[loc]=(( struct struct_hgraph  *)(hgraph.extra))->dist1[i];
         tempdist1[i]=(( struct struct_hgraph  *)(hgraph.extra))->dist1[i];
    }
}

__global__ void update(GGraph hgraph,struct FALCbuffer *recvbuff,int FALCnamount,int kk){
    int id=blockIdx.x*blockDim.x+threadIdx.x;
    if(id <FALCnamount){
        int vertex= recvbuff[kk].vid[id];
        if( ( ( struct struct_hgraph  * )(hgraph.extra))->dist1[vertex] > recvbuff[kk].dist1[id])
            ( ( struct struct_hgraph  * )(hgraph.extra))->dist1[vertex] = recvbuff[kk].dist1[id];
    }
}
__device__ int  GPU_changed,GPU_hchanged,GPU_level;
//BEGIN
__global__ void   reset ( GGraph  graph ,int   level ,int FALCX,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints+graph.remotepoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=graph.index[t+1].ipe - graph.index[t].ipe; 

 ((struct struct_hgraph  *)(graph.extra))->updated[id]=0; 

 if( ((struct struct_hgraph  *)(graph.extra))->dist[id]<level )((struct struct_hgraph  *)(graph.extra))->updated[id]=1; 

 }//end fun 0 

}
__global__ void   relaxgraph ( GGraph  graph ,int   level ,int FALCX,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints&& ((struct struct_hgraph  *)(graph.extra))->updated[id]==1 ){

 int falcft6=graph.index[id+1]-graph.index[id];
int falcft7=graph.index[id];
/*XX*/for(int falcft8=0;falcft8<falcft6;falcft8++){
int ut6=2*(falcft7+falcft8);
 int ut7=graph.edges[ut6].ipe;
int ut8=graph.edges[ut6+1].ipe;
HGADD(&(((struct struct_hgraph  *)(graph.extra))->dist1/*xx*/[ut7]),1,/*xx*/GPU_changed);//rhs not null


 }//foreach

 }//end fun 0 

}
__global__ void   reset1 ( GGraph  graph ,int   level ,int FALCX,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=((struct struct_hgraph  *)(graph.extra))->dist[id]-((struct struct_hgraph  *)(graph.extra))->dist1[id]; 

 if( ((struct struct_hgraph  *)(graph.extra))->dist[id]<level&&((struct struct_hgraph  *)(graph.extra))->updated[id]==0 )((struct struct_hgraph  *)(graph.extra))->updated[id]=1; 

 if( ((struct struct_hgraph  *)(graph.extra))->updated[id]==1 )((struct struct_hgraph  *)(graph.extra))->updated[id]==2; 

 }//end fun 0 

}
__global__ void   relaxgraph1 ( GGraph  graph ,int   level ,int FALCX,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints){

 ((struct struct_hgraph  *)(graph.extra))->dist1[id]=false; 

 int falcft9=graph.index[id+1]-graph.index[id];
int falcft10=graph.index[id];
/*XX*/for(int falcft11=0;falcft11<falcft9;falcft11++){
int ut9=2*(falcft10+falcft11);
 int ut10=graph.edges[ut9].ipe;
int ut11=graph.edges[ut9+1].ipe;
((struct struct_hgraph  *)(graph.extra))->dist1/*xx*/[ut10]=false; 

 }//foreach

 }//end fun 0 

}
//END
