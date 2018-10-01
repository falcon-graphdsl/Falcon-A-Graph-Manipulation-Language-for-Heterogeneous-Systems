
#include "singlemstglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>
#include<mpi.h>
#include "HGraph.h"
 #include<sys/time.h>
#include<unistd.h>
struct struct_hgraph { 
int   *minedge ;//has to given size of property type
int   *mark ;//has to given size of property type
int   *Weight ;//has to given size of property type
struct node  *minppty ;//has to given size of property type
};
void alloc_extra_hgraph(GGraph &hgraph, int flag,int npoints) {
 cudaSetDevice(0);
struct struct_hgraph  temp;
if(flag==0)cudaMalloc((void **)&(hgraph.extra),sizeof(struct struct_hgraph ));
 cudaMemcpy(&temp,((struct struct_hgraph  *)(hgraph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
cudaMalloc( (void **)&(temp.minedge),sizeof(int )* hgraph.npoints); 
cudaMalloc( (void **)&(temp.mark),sizeof(int )* hgraph.nedges); 
cudaMalloc( (void **)&(temp.Weight),sizeof(int )* hgraph.npoints); 
cudaMalloc( (void **)&(temp.minppty),sizeof(struct node)* hgraph.npoints); 
if(cudaMemcpy(hgraph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 0");cudaSetDevice(0);
}
void hgraph_realloc(GGraph &hgraph){
 struct struct_hgraph  temp;
cudaSetDevice(0);
if(cudaMemcpy(&temp,hgraph.extra,sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 1");
void *t1=temp.minedge; cudaMalloc((void **)&(temp.minedge),sizeof(minedge)*int .npoints*2);
if(((hgraph.npoints-temppnpoints)*100/hgraph.npoints>50 )||( hgraph.npoints*2>hgraph.maxnpoints)){//
if(cudaMemcpy(temp.minedge,t1,sizeof(int )*hgraph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 2");
hgraph.maxnpoints=hgraph.npoints*2;
cudaFree(t1);
}
if(((hgraph.nedges-temppnedges)*100/hgraph.nedges>50)|| (hgraph.npoints*2>hgraph.maxnpoints)){//
void *t1=temp.mark; cudaMalloc((void **)&(temp.mark),sizeof(mark)*int .nedges*2);
if(cudaMemcpy(temp.mark,t1,sizeof(int )*hgraph.nedges,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 3");
hgraph.maxnedges=hgraph.nedges*2;
cudaFree(t1);}
void *t1=temp.Weight; cudaMalloc((void **)&(temp.Weight),sizeof(Weight)*int .npoints*2);
if(((hgraph.npoints-temppnpoints)*100/hgraph.npoints>50 )||( hgraph.npoints*2>hgraph.maxnpoints)){//
if(cudaMemcpy(temp.Weight,t1,sizeof(int )*hgraph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 4");
hgraph.maxnpoints=hgraph.npoints*2;
cudaFree(t1);
}
void *t1=temp.minppty; cudaMalloc((void **)&(temp.minppty),sizeof(minppty)*struct node.npoints*2);
if(((hgraph.npoints-temppnpoints)*100/hgraph.npoints>50 )||( hgraph.npoints*2>hgraph.maxnpoints)){//
if(cudaMemcpy(temp.minppty,t1,sizeof(struct node)*hgraph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 5");
hgraph.maxnpoints=hgraph.npoints*2;
cudaFree(t1);
}
if(cudaMemcpy(hgraph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 6");
cudaSetDevice(0);

}
//communication buffer datatype
 struct FALCbuffer{
      int *vid;
     int  *Weight;
};
int  *tempWeight;
int FALCrank;//rank of process
char partitionfile[100];//second input
 char FALChostname[256];//name of host on which process running
int FALCsize;//total processes launched
 MPI_Status *FALCstatus;//used for MPI_Recv
MPI_Request *FALCrequest;//Used for MPI_Isend
int *FALCsendsize;//send buffer size(for sending to remote machines)
 int *FALCrecvsize;
int FALCmsgno;//message number for messages used in code
 int FALCnamount;
struct FALCbuffer *FALCsendbuff,*FALCrecvbuff;//send and receive buffer for synchronizing global state
//allocate buffer for communication 
void FALCallocbuff(struct FALCbuffer *buff,int tot,int size){
    struct FALCbuffer temp;
    for(int i=0;i<tot;i++){
        cudaMemcpy( &temp,&buff[i],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        cudaMalloc(&(temp.vid),sizeof(int)*size);
        cudaMalloc(&(temp.Weight),sizeof(int )*size);
        cudaMemcpy(&buff[i],&temp,sizeof(struct FALCbuffer),cudaMemcpyHostToDevice);
    }
    cudaMalloc(&(tempWeight),sizeof(int )*size);
}
__global__ void sendprefix(GGraph hgraph,int  *tempWeight,int  *tempWeight){
int id=threadIdx.x+blockDim.x*blockIdx.x;
if(id<(hgraph.localpoints+hgraph.remotepoints)){
tempWeight[id]=((struct struct_hgraph  *)(hgraph.extra))->Weight[id];
tempWeight[id]=((struct struct_hgraph  *)(hgraph.extra))->Weight[id];
}
}
__global__ void sendbuff(GGraph hgraph,int *sendsize,struct FALCbuffer *sendbuff,int  *tempWeight,int  *tempWeight,int kk,int off,int totelems){
    int id=threadIdx.x+blockDim.x*blockIdx.x;
    if(id <totelems){
    int loc=0;
    int flag=0;
    if( (( struct struct_hgraph  *)(hgraph.extra))->Weight[id+off]!=tempWeight[id+off])flag=1;
    if( (( struct struct_hgraph  *)(hgraph.extra))->Weight[id+off]!=tempWeight[id+off])flag=1;
    if(flag==1){
        loc=atomicAdd(&sendsize[kk],1);
        sendbuff[kk].vid[loc]=hgraph.remotevertexid[id+off];
        sendbuff[kk].Weight[loc]=(( struct struct_hgraph  *)(hgraph.extra))->Weight[id+off];
         tempWeight[id+off]=(( struct struct_hgraph  *)(hgraph.extra))->Weight[id+off];
        sendbuff[kk].Weight[loc]=(( struct struct_hgraph  *)(hgraph.extra))->Weight[id+off];
         tempWeight[id+off]=(( struct struct_hgraph  *)(hgraph.extra))->Weight[id+off];
    }
}
}

__global__ void update(GGraph hgraph,struct FALCbuffer *recvbuff,int FALCnamount,int kk){
    int id=blockIdx.x*blockDim.x+threadIdx.x;
    if(id <FALCnamount){
        int vertex= recvbuff[kk].vid[id];
        if( ( ( struct struct_hgraph  * )(hgraph.extra))->Weight[vertex] > recvbuff[kk].Weight[id])
            ( ( struct struct_hgraph  * )(hgraph.extra))->Weight[vertex] = recvbuff[kk].Weight[id];
        if( ( ( struct struct_hgraph  * )(hgraph.extra))->Weight[vertex] > recvbuff[kk].Weight[id])
            ( ( struct struct_hgraph  * )(hgraph.extra))->Weight[vertex] = recvbuff[kk].Weight[id];
    }
}
__global__ void sendbuffsingle(GGraph hgraph,int *sendsize,struct FALCbuffer *sendbuff,int *templock,int kk,int off,int totelems){
    int id=threadIdx.x+blockDim.x*blockIdx.x;
    if(id <totelems){
    int loc=0;
    int flag=0;
    if( (( struct struct_hgraph  *)(hgraph.extra))->lock[id+off]!=templock[id+off])flag=1;
    if(flag==1){
        loc=atomicAdd(&sendsize[kk],1);
        sendbuff[kk].vid[loc]=hgraph.local_to_orig[id+off];
        sendbuff[kk].lock[loc]=(( struct struct_hgraph  *)(hgraph.extra))->lock[id+off];
         templock[id+off]=(( struct struct_hgraph  *)(hgraph.extra))->lock[id+off];
    }
}
}

__global__ void updatesingle(GGraph hgraph,struct FALCbuffer *recvbuff,int FALCnamount,int kk){
    int id=blockIdx.x*blockDim.x+threadIdx.x;
    if(id <FALCnamount){
        int vertex= recvbuff[kk].vid[id];
        if( ( ( struct struct_hgraph  * )(hgraph.extra))->lock[vertex] > kk)
            ( ( struct struct_hgraph  * )(hgraph.extra))->lock[vertex] =kk;
    }
}
//single flag Minedge 1 1 
__global__ void   Minedge ( GGraph  graph ,GSet  set ,int FALCX,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints){

 int t1;

 int t2;

 

 int e;

 int falcft6=graph.index[id+1]-graph.index[id];
int falcft7=graph.index[id];
/*XX*/for(int falcft8=0;falcft8<falcft6;falcft8++){
int ut6=2*(falcft7+falcft8);
 int ut7=graph.edges[ut6].ipe;
int ut8=graph.edges[ut6+1].ipe;
t1=set./**/parent[id]; 

 t2=set./**/parent[ut7]; 

 t3=struct struct_hgraph  temp0;/*xx*/
cudaSetDevice(0);
((struct struct_hgraph  *)(graph.extra))->getWeight(id,/*xx*/ut7); 

 if( t1!=t2 ){

 if( t3==((struct struct_hgraph  *)(graph.extra))->Weight[t1] ){

 

 if(atomicCAS( &(((struct struct_hgraph  *)(graph.extra))->minppty[t1].lock),0,FALCRANK)==0)}//end

 if( t3==((struct struct_hgraph  *)(graph.extra))->Weight[t2] ){

 if(atomicCAS( &(((struct struct_hgraph  *)(graph.extra))->minppty[t2].lock),0,FALCRANK)==0)}//end

 }//end

 }//foreach

 }//end fun 0 

}
