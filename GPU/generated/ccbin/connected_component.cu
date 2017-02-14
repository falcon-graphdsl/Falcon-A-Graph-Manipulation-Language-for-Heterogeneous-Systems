
 #include "connected_component.h"
cudaDeviceProp prop0;
__device__ int   changed =0;
;
 int   hchanged =0;


 __global__ void   relaxgraph ( GGraph  unnigraph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int p=id;
if( id < unnigraph.npoints){

 int falcft0=unnigraph.index[id+1]-unnigraph.index[id];
int falcft1=unnigraph.index[id];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=unnigraph.edges[ut0].ipe;
int ut2=unnigraph.edges[ut0+1].ipe;
int t=ut1;
GMIN(&(((struct struct_hgraph  *)(unnigraph.extra))->dist[ut1]),((struct struct_hgraph  *)(unnigraph.extra))->dist[id],changed);


 GMIN(&(((struct struct_hgraph  *)(unnigraph.extra))->dist[id]),((struct struct_hgraph  *)(unnigraph.extra))->dist[ut1],changed);


 }

 }

}
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int t=id;
if( id < graph.npoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=graph.points[id].ipe; 

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


 int   falcvt1;
falcvt1=0;
struct struct_hgraph  temp1;
 cudaMemcpy(&temp1,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp1.dist[0]),&(falcvt1),sizeof(int ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1");

 
 while(1)  { 

 int   falcvt2;
falcvt2=0;
if(cudaMemcpyToSymbol(changed,&(falcvt2),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 2");

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
relaxgraph<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 int   falcvt3;
if(cudaMemcpyFromSymbol(&(falcvt3),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 3");
if(falcvt3==0)break;

 }

 t2=rtclock();


 printf("\nTIME =%f",(t2-t1)*1000);


 struct struct_hgraph  temp2;/*err*/
cudaMemcpy(&temp2,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy((((struct struct_hgraph  *)(hgraph.extra))->dist),(temp2.dist),sizeof(int)*hgraph.npoints,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 4");

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
