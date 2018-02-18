#include<mpi.h>
#include "cpussspglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
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
//communication buffer datatype
 struct FALCbuffer{
      int *vid;
     int  *dist;
};
int  *tempdist;
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
        cudaMalloc(&(temp.dist),sizeof(int )*size);
        cudaMemcpy( &buff[i],&temp,sizeof(struct FALCbuffer),cudaMemcpyHostToDevice);
    }
    cudaMalloc(&(tempdist),sizeof(int )*size);
}
__global__ void sendbuff(GGraph hgraph,int *sendsize,struct FALCbuffer *sendbuff,int *tempdist,int kk,int off,int totelems){
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

__global__ void update(GGraph hgraph,struct FALCbuffer *recvbuff,int FALCnamount,int kk){
    int id=blockIdx.x*blockDim.x+threadIdx.x;
    if(id <FALCnamount){
        int vertex= recvbuff[kk].vid[id];
        if( ( ( struct struct_hgraph  * )(hgraph.extra))->dist[vertex] > recvbuff[kk].dist[id])
            ( ( struct struct_hgraph  * )(hgraph.extra))->dist[vertex] = recvbuff[kk].dist[id];
    }
}
