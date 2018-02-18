
#include "prcpuglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>
struct struct_hgraph { 
float   *val ;//has to given size of property type
float   *PR ;//has to given size of property type
};
void alloc_extra_hgraph(GGraph &hgraph, int flag,int npoints) {
 cudaSetDevice(0);
struct struct_hgraph  temp;
if(flag==0)cudaMalloc((void **)&(hgraph.extra),sizeof(struct struct_hgraph ));
 cudaMemcpy(&temp,((struct struct_hgraph  *)(hgraph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
cudaMalloc( (void **)&(temp.val),sizeof(float )* hgraph.npoints); 
cudaMalloc( (void **)&(temp.PR),sizeof(float )* hgraph.npoints); 
if(cudaMemcpy(hgraph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 0");cudaSetDevice(0);
}
void hgraph_realloc(GGraph &hgraph){
 struct struct_hgraph  temp;
cudaSetDevice(0);
if(cudaMemcpy(&temp,hgraph.extra,sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 1");
void *t1=temp.val; cudaMalloc((void **)&(temp.val),sizeof(val)*float .npoints*2);
if(((hgraph.npoints-temppnpoints)*100/hgraph.npoints>50 )||( hgraph.npoints*2>hgraph.maxnpoints)){//
if(cudaMemcpy(temp.val,t1,sizeof(float )*hgraph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 2");
hgraph.maxnpoints=hgraph.npoints*2;
cudaFree(t1);
}
void *t1=temp.PR; cudaMalloc((void **)&(temp.PR),sizeof(PR)*float .npoints*2);
if(((hgraph.npoints-temppnpoints)*100/hgraph.npoints>50 )||( hgraph.npoints*2>hgraph.maxnpoints)){//
if(cudaMemcpy(temp.PR,t1,sizeof(float )*hgraph.npoints,cudaMemcpyDeviceToDevice)!=cudaSuccess)printf("memcpyerror 3");
hgraph.maxnpoints=hgraph.npoints*2;
cudaFree(t1);
}
if(cudaMemcpy(hgraph.extra,&temp,sizeof(struct struct_hgraph ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 4");
cudaSetDevice(0);

}
__global__ void sendprefix(GGraph hgraph,float  *tempval){
int id=threadIdx.x+blockDim.x*blockIdx.x;
if(id<(hgraph.localpoints+hgraph.remotepoints)){
tempval[id]=((struct struct_hgraph  *)(hgraph.extra))->val[id];
}
}
__global__ void sendbuff(GGraph hgraph,int *sendsize,struct FALCbuffer *sendbuff,float  *tempval,int kk,int off,int totelems){
    int id=threadIdx.x+blockDim.x*blockIdx.x;
    if(id <totelems){
    int loc=0;
    int flag=0;
    if( (( struct struct_hgraph  *)(hgraph.extra))->val[id+off]!=tempval[id+off])flag=1;
    if(flag==1){
        loc=atomicAdd(&sendsize[kk],1);
        sendbuff[kk].vid[loc]=hgraph.remotevertexid[id+off];
        sendbuff[kk].val[loc]=(( struct struct_hgraph  *)(hgraph.extra))->val[id+off];
         tempval[id+off]=(( struct struct_hgraph  *)(hgraph.extra))->val[id+off];
    }
}
}

__global__ void update(GGraph hgraph,struct FALCbuffer *recvbuff,int FALCnamount,int kk){
    int id=blockIdx.x*blockDim.x+threadIdx.x;
    if(id <FALCnamount){
        int vertex= recvbuff[kk].vid[id];
        if( ( ( struct struct_hgraph  * )(hgraph.extra))->val[vertex] > recvbuff[kk].val[id])
            ( ( struct struct_hgraph  * )(hgraph.extra))->val[vertex] = recvbuff[kk].val[id];
    }
}
