
 #include "newgpusssp1.h"
cudaDeviceProp prop0;
__device__ int   changed =0;
;
 int   hchanged =0;


 __global__ void   relaxgraph ( GGraph  graph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int p=id;
if( id < graph.npoints&& ((struct struct_hgraph  *)(graph.extra))->/*4*/updated[id]==true ){

 ((struct struct_hgraph  *)(graph.extra))->updated[id]=false; 

 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
int t=ut1;
GMIN(&(((struct struct_hgraph  *)(graph.extra))->dist[ut1]),((struct struct_hgraph  *)(graph.extra))->dist[id]+ut2,changed);
}
}

}
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int t=id;
if( id < graph.npoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=1234567890; 

 ((struct struct_hgraph  *)(graph.extra))->olddist[id]=1234567890; 

 ((struct struct_hgraph  *)(graph.extra))->updated[id]=false; 

 }

}
__global__ void   reset1 ( GGraph  graph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int t=id;
if( id < graph.npoints){

 if( ((struct struct_hgraph  *)(graph.extra))->dist[id]<((struct struct_hgraph  *)(graph.extra))->olddist[id] )
((struct struct_hgraph  *)(graph.extra))->updated[id]=true; 

 ((struct struct_hgraph  *)(graph.extra))->olddist[id]=((struct struct_hgraph  *)(graph.extra))->dist[id]; 

 }

}
void   SSSP ( char    *  name ) 
 {

 HGraph  hgraph ;

 

 GGraph graph;


 

 

 hgraph.read2(name);

int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 hgraph.cloneGPU(graph,0 );
int graphflag=0;

alloc_extra_graph(graph,graphflag,graph.npoints);

int TPB0=findthreadsperblock(prop0);

int graphpointkernelblocks=findblocksize(graph,graph.npoints,TPB0);

int graphedgekernelblocks=findblocksize(graph,graph.nedges,TPB0);
copygraphcurrentsize(graph);
cudaSetDevice(0);


  double   t1 , t2 ;


 t1=rtclock();


 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
reset<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 bool  falcvt1;
falcvt1=true;
struct struct_hgraph  temp1;
 cudaMemcpy(&temp1,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp1.updated[0]),&(falcvt1),sizeof(bool),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1");

 int   falcvt2;
falcvt2=0;
struct struct_hgraph  temp2;
 cudaMemcpy(&temp2,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp2.dist[0]),&(falcvt2),sizeof(int ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 2");

 
 while(1)  { 

 int   falcvt3;
falcvt3=0;
if(cudaMemcpyToSymbol(changed,&(falcvt3),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 3");

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
relaxgraph<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 int   falcvt4;
if(cudaMemcpyFromSymbol(&(falcvt4),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 4");
if(falcvt4==0)break;

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
reset1<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 }

 t2=rtclock();


 printf("\nTIME =%f",(t2-t1)*1000);


 struct struct_hgraph  temp3;/*err*/
cudaMemcpy(&temp3,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy((((struct struct_hgraph  *)(hgraph.extra))->dist),(temp3.dist),sizeof(int)*hgraph.npoints,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 5");

  int   maxdist =0;


 for (int   i =0;i<hgraph.npoints;i++) {

 if( maxdist<((struct struct_hgraph  *)(hgraph.extra))->dist[i]  )
maxdist=((struct struct_hgraph  *)(hgraph.extra))->dist[i]; 

 }

 printf("\nMAXDIST=%d\n",maxdist);


 return ;

 }
int   main ( int   argc ,char    *  argv [ ] ) 
 {
cudaGetDeviceProperties(&prop0,0); 
alloc_sync_array();


 if( argc!=2  )
{

 printf("error: exec filename");


 return 1;

 }

 SSSP(argv[1]);


 }
