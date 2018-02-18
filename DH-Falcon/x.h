
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
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints) {
 if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
((struct struct_hgraph  *)hgraph.extra)->olddist=(int  *)malloc(sizeof(int ) * npoints) ;
((struct struct_hgraph  *)hgraph.extra)->updated=(bool *)malloc(sizeof(bool) * npoints) ;
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
void *t1=temp.olddist;
temp.olddist=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.olddist,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.updated;
temp.updated=(bool *)malloc(sizeof(bool)*hgraph.npoints*2);
memcpy(temp.updated,t1,sizeof(bool)*hgraph.npoints);
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
     int  *dist;
};
int  *tempdist, *tempgpudist;
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
        cudaMalloc(&(temp.dist),sizeof(int )*size);
        cudaMemcpy(&buff[i],&temp,sizeof(struct FALCbuffer),cudaMemcpyHostToDevice);
        }
        cudaMalloc(&(tempgpudist),sizeof(int )*size);
}
//allocate buffer for communication 
void FALCallocbuff(struct FALCbuffer *buff,int tot,int size){
        for(int i=0;i<tot;i++){
            buff[i].vid=(int *)malloc(sizeof(int)*size);
            buff[i].dist=(int  *)malloc(sizeof(int )*size);
        }
        tempdist=(int  *)malloc(sizeof(int )*size);
}
__global__ void sendprefix(GGraph hgraph,int  *tempdist){
int id=threadIdx.x+blockDim.x*blockIdx.x;
if(id<(hgraph.localpoints+hgraph.remotepoints)){
tempdist[id]=((struct struct_hgraph  *)(hgraph.extra))->dist[id];
}
}
__global__ void sendbuff(GGraph hgraph,int *sendsize,struct FALCbuffer *sendbuff,int  *tempdist,int kk,int off,int totelems){
    int id=threadIdx.x+blockDim.x*blockIdx.x;
    if(id <totelems){
    int loc=0;
    int flag=0;
    if( (( struct struct_hgraph  *)(hgraph.extra))->dist[id+off]!=tempdist[id+off])flag=1;
    if(flag==1){
        loc=atomicAdd(&sendsize[kk],1);
        sendbuff[kk].vid[loc]=hgraph.remotevertexid[id+off];
        sendbuff[kk].dist[loc]=(( struct struct_hgraph  *)(hgraph.extra))->dist[id+off];
         tempdist[id+off]=(( struct struct_hgraph  *)(hgraph.extra))->dist[id+off];
    }
}
}
void sendbuff(int i,HGraph &hgraph,int *sendsize,struct FALCbuffer *sendbuff,tempdist,int kk);
    int loc=0;
    int flag=0;
    if( (( struct struct_hgraph  *)(hgraph.extra))->dist[i]!=tempdist[i])flag=1;
    if(flag==1){
        loc=__sync_fetch_and_add(&sendsize[kk],1);
        sendbuff[kk].vid[loc]=hgraph.remotevertexid[i];
        sendbuff[kk].dist[loc]=(( struct struct_hgraph  *)(hgraph.extra))->dist[i];
         tempdist[i]=(( struct struct_hgraph  *)(hgraph.extra))->dist[i];
    }
}

__global__ void update(GGraph hgraph,struct FALCbuffer *recvbuff,int FALCnamount,int kk){
    int id=blockIdx.x*blockDim.x+threadIdx.x;
    if(id <FALCnamount){
        int vertex= recvbuff[kk].vid[id];
        if( ( ( struct struct_hgraph  * )(hgraph.extra))->dist[vertex] > recvbuff[kk].dist[id])
            ( ( struct struct_hgraph  * )(hgraph.extra))->dist[vertex] = recvbuff[kk].dist[id];
    }
}
 int   changed =0, hchanged =0;


 void   relaxgraph ( int & p ,HGraph & graph ) 
 {
 if( ((struct struct_hgraph  *)(graph.extra))->updated/*here1*/[p]==true ){

 ((struct struct_hgraph  *)(graph.extra))->updated[p]=false; 

 int falcft3=graph.index[p+1]-graph.index[p];
int falcft4=graph.index[p];
/*XX*/for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
HMIN(&(((struct struct_hgraph  *)(graph.extra))->dist/*here1*//*xx*/[ut4]),((struct struct_hgraph  *)(graph.extra))->dist[p]+ut5,/*xx*/ut4,/*xx*/ut1,/*xx*/GPU_changed);//rhs not null


 }//foreach

 }//end fun 0 

}
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints+graph.remotepoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=1234567890; 

 ((struct struct_hgraph  *)(graph.extra))->olddist[id]=1234567890; 

 ((struct struct_hgraph  *)(graph.extra))->updated[id]=false; 

 }//end fun 0 

}
__global__ void   relaxgraph ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints&& ((struct struct_hgraph  *)(graph.extra))->updated/*here1*/[id]==true ){

 ((struct struct_hgraph  *)(graph.extra))->updated[id]=false; 

 int falcft6=graph.index[id+1]-graph.index[id];
int falcft7=graph.index[id];
/*XX*/for(int falcft8=0;falcft8<falcft6;falcft8++){
int ut6=2*(falcft7+falcft8);
 int ut7=graph.edges[ut6].ipe;
int ut8=graph.edges[ut6+1].ipe;
HGMIN(&(((struct struct_hgraph  *)(graph.extra))->dist/*here1*//*xx*/[ut7]),((struct struct_hgraph  *)(graph.extra))->dist[id]+ut8,/*xx*/ut7,/*xx*/ut4,/*xx*/ut1,/*xx*/GPU_changed);//rhs not null


 }//foreach

 }//end fun 0 

}
__global__ void   reset1 ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints){

 if( ((struct struct_hgraph  *)(graph.extra))->dist[id]<((struct struct_hgraph  *)(graph.extra))->olddist[id] )((struct struct_hgraph  *)(graph.extra))->updated[id]=true; 

 ((struct struct_hgraph  *)(graph.extra))->olddist[id]=((struct struct_hgraph  *)(graph.extra))->dist[id]; 

 }//end fun 0 

}
