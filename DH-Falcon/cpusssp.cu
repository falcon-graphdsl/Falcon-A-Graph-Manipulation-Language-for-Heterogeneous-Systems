
 #include "cpusssp.h"
// checklibfub MIN 
// LIBFUN MIN 
//libfunction call MIN
// clustercomm.c LIB FUNCTION MIN 3rd ARG changed to be communicated
__device__ int   changed =0;
;
 int   hchanged =0;


  int   fchanged =0, fhchanged =0;


 __global__ void   relaxgraph ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.npoints&& ((struct struct_hgraph  *)(graph.extra))->updated[id]==true ){

 ((struct struct_hgraph  *)(graph.extra))->updated[id]=false; 

 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
GMIN(&(((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut1]),((struct struct_hgraph  *)(graph.extra))->dist[id]+ut2,/*xx*/changed);//rhs not null


 }//foreach

 }//end fun 0 

}
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.npoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=1234567890; 

 ((struct struct_hgraph  *)(graph.extra))->olddist[id]=1234567890; 

 ((struct struct_hgraph  *)(graph.extra))->updated[id]=false; 

 }//end fun 0 

}
__global__ void   reset1 ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.npoints){

 if( ((struct struct_hgraph  *)(graph.extra))->dist[id]<((struct struct_hgraph  *)(graph.extra))->olddist[id] )((struct struct_hgraph  *)(graph.extra))->updated[id]=true; 

 ((struct struct_hgraph  *)(graph.extra))->olddist[id]=((struct struct_hgraph  *)(graph.extra))->dist[id]; 

 }//end fun 0 

}
void   SSSP ( char    *  name ) 
 {

 GGraph  hgraph ;


 /*TE=2*/

 /*TE=2*/

 /*TE=2*/

 /*TE=2*/hgraph.read2(name);

int TPB0=findthreadsperblock(prop0);

int hgraphpointkernelblocks=findblocksize(hgraph,hgraph.npoints,TPB0);
; 

 /*SINGLE FLAG reset 0 */
cudaSetDevice(0);
for(int kk=0;kk<hgraph.npoints;kk+=hgraphpointkernelblocks*TPB0){ 
reset<<<hgraphpointkernelblocks,TPB0>>>(hgraph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);
cudaDeviceSynchronize();
cudaSetDevice(0);


 /*TE=1*///GPU ASS 
bool  falcvt1;
falcvt1=true;
struct struct_hgraph  temp0;
 cudaMemcpy(&temp0,((struct struct_hgraph  *)(hgraph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp0.updated[0]),&(falcvt1),sizeof(bool),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 6"); 


 /*TE=1*///GPU ASS 
int   falcvt2;
falcvt2=0;
struct struct_hgraph  temp1;
 cudaMemcpy(&temp1,((struct struct_hgraph  *)(hgraph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp1.dist[0]),&(falcvt2),sizeof(int ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 7"); 


 
 while(1)  { 

 /*TE=1*///GPU ASS 
int   falcvt3;
falcvt3=0;
if(cudaMemcpyToSymbol(changed,&(falcvt3),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 8");//val=1 
 


 /*SINGLE FLAG relaxgraph 0 */
cudaSetDevice(0);
for(int kk=0;kk<hgraph.npoints;kk+=hgraphpointkernelblocks*TPB0){ 
relaxgraph<<<hgraphpointkernelblocks,TPB0>>>(hgraph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);
cudaDeviceSynchronize();
cudaSetDevice(0);


 
 //GPU IF STMT 
int   falcvt4;
if(cudaMemcpyFromSymbol(&(falcvt4),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 9");
if(falcvt4==0)break;

 /*SINGLE FLAG reset1 0 */
cudaSetDevice(0);
for(int kk=0;kk<hgraph.npoints;kk+=hgraphpointkernelblocks*TPB0){ 
reset1<<<hgraphpointkernelblocks,TPB0>>>(hgraph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);
cudaDeviceSynchronize();
cudaSetDevice(0);


 }//end

 struct struct_hgraph   temp2;
 cudaMemcpy((void *)&temp2,hgraph.extra,sizeof( struct struct_hgraph ),cudaMemcpyDeviceToHost);
struct struct_hgraph   temp3;
 cudaMemcpy((void *)&temp3,hgraph.extra,sizeof( struct struct_hgraph ),cudaMemcpyDeviceToHost);
int  *temp5=(int  *) malloc(sizeof(int )*hgraph.npoints);
struct struct_hgraph   temp6;
 cudaMemcpy((void *)&temp6,hgraph.extra,sizeof(struct struct_hgraph  ),cudaMemcpyDeviceToHost);
cudaMemcpy(temp5, temp6.dist,sizeof(int )*hgraph.npoints,cudaMemcpyDeviceToHost);
for (int   i =0;i<hgraph.npoints;i++) {

 printf("%d\n",/*xx*/temp5[i]);//rhs not null


 }//endfor

 return ;

 }//end fun 0 
int   main ( int   argc ,char    *  argv [ ] ) 
 {
cudaGetDeviceProperties(&prop0,0); 
alloc_sync_array();//1


 SSSP(argv[1]);//rhs not null


 }//end fun 0 
