
 #include "singlemst.h"
cudaDeviceProp prop0;


 __device__ int   changed ;
;
 int   hchanged ;


 __global__ void   reset ( GGraph  graph ,/*SET*/GSet set ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int p=id;
if( id < graph.npoints){

 ((struct struct_hgraph  *)(graph.extra))->minppty[id].weight=99999999; 

 ((struct struct_hgraph  *)(graph.extra))->minedge[id]=99999999; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[id].set=99999999; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[id].lock=0; 

 }

}
__global__ void   minset ( GGraph  graph ,/*SET*/GSet set ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int p=id;
if( id < graph.npoints){

  int   ch ;


 int t1;

 int t2;

 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
int t=ut1;
t1=set.parent[id]; 

 t2=set.parent[ut1]; 

 if( t1!=t2 )
{

 GMIN(&(((struct struct_hgraph  *)(graph.extra))->minppty[t1].weight),ut2,ch);


 GMIN(&(((struct struct_hgraph  *)(graph.extra))->minppty[t2].weight),ut2,ch);


 }

 }

 }

}
__global__ void   Minedge ( GGraph  graph ,/*SET*/GSet set ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int p=id;
if( id < graph.npoints){

 int t1;

 int t2;

  int   t3 ;


 int e;

 int falcft3=graph.index[id+1]-graph.index[id];
int falcft4=graph.index[id];
for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
int t=ut4;
t1=set.parent[id]; 

 t2=set.parent[ut4]; 

 t3=ut5; 

 if( t1!=t2 )
{

 if( t3==((struct struct_hgraph  *)(graph.extra))->minppty[t1].weight )
{

 if(atomicCAS( &(((struct struct_hgraph  *)(graph.extra))->minppty[t1].lock),0,1)==0){

 e=ut3/2; 

 ((struct struct_hgraph  *)(graph.extra))->mark[e]=1; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t1].src=p; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t1].dst=t; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t1].weight=t3; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t1].set=t2; 

 }

 }

 if( t3==((struct struct_hgraph  *)(graph.extra))->minppty[t2].weight )
{

 if(atomicCAS( &(((struct struct_hgraph  *)(graph.extra))->minppty[t2].lock),0,1)==0){

 e=ut3/2; 

 ((struct struct_hgraph  *)(graph.extra))->mark[e]=1; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t2].src=p; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t2].dst=t; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t2].weight=t3; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t2].set=t1; 

 }

 }

 }

 }

 }

}
__global__ void   mstunion ( GGraph  graph ,/*SET*/GSet set ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int p=id;
if( id < graph.npoints){

 int t1;

 int t2;

  int   t3 , t4 ;


 t1=set.parent[id]; 

 t2=((struct struct_hgraph  *)(graph.extra))->minppty[t1].set; 

 t3=((struct struct_hgraph  *)(graph.extra))->minppty[t1].lock; 

 t4=((struct struct_hgraph  *)(graph.extra))->minppty[t2].lock; 

 if( t1!=t2&&t3==1 )
{

 //Currn_fun_name =mstunion
set.Union(t1,t2); 

 changed=1; 

 }

 }

}
__global__ void   initmark ( GGraph  graph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int e=id;
if( id < graph.nedges){

 ((struct struct_hgraph  *)(graph.extra))->mark[id]=999999999; 

 }

}
int   main ( int   argc ,char    *  argv [ ] ) 
 {
cudaGetDeviceProperties(&prop0,0); 
alloc_sync_array();


 if( argc!=2  )
{

 printf("error:- exec filename");


 return 1;

 }

 HGraph  hgraph ;

 

 

 

 GGraph graph;


 hgraph.read2(argv[1]);

int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 HSet hset;
hset.allocate(hgraph.npoints);

 GSet set;
set.allocate(graph.npoints);

 hgraph.cloneGPU(graph,0 );
int graphflag=0;

alloc_extra_graph(graph,graphflag,graph.npoints);

int TPB0=findthreadsperblock(prop0);

int graphpointkernelblocks=findblocksize(graph,graph.npoints,TPB0);

int graphedgekernelblocks=findblocksize(graph,graph.nedges,TPB0);
copygraphcurrentsize(graph);
cudaSetDevice(0);


 hset.cloneGPU(set,0 );
int setflag=0;
cudaSetDevice(0);


 hchanged=0; 

  double   rt1 =rtclock();


 cudaSetDevice(0);
for(int kk=0;kk<graph.nedges;kk+=graphedgekernelblocks*TPB0){ 
initmark<<<graphedgekernelblocks,TPB0>>>(graph,kk);}
cudaDeviceSynchronize();
cudaSetDevice(0);


 
 while(1)  { 

 int   falcvt1;
falcvt1=0;
if(cudaMemcpyToSymbol(changed,&(falcvt1),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1");

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
reset<<<graphpointkernelblocks,TPB0>>>(graph,set,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
minset<<<graphpointkernelblocks,TPB0>>>(graph,set,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
Minedge<<<graphpointkernelblocks,TPB0>>>(graph,set,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
mstunion<<<graphpointkernelblocks,TPB0>>>(graph,set,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);
cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){
findset<<<graphpointkernelblocks,TPB0>>>( set,kk);
}
cudaDeviceSynchronize();cudaSetDevice(0);


 int   falcvt2;
if(cudaMemcpyFromSymbol(&(falcvt2),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 2");
if(falcvt2==0)break;

 }

  double   rt2 =rtclock();


 printf("TIME =%f \n",(rt2-rt1)*1000);


 struct struct_hgraph  temp2;/*err*/
cudaMemcpy(&temp2,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy((((struct struct_hgraph  *)(hgraph.extra))->mark),(temp2.mark),sizeof(int)*hgraph.nedges,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 3");

  unsigned  long  int   mst =0;


 unsigned int hreduxsum0=0;
cudaMemcpyToSymbol(reduxsum0,&hreduxsum0,sizeof(unsigned int ),0,cudaMemcpyHostToDevice);
cudaSetDevice(0);
if(graph.nedges/1024> (32*1024)){
for(int kk=0;kk<graph.nedges;kk+=FALCK32*1024)RSUM0<<<FALCK32,1024>>>(graph,kk);
cudaDeviceSynchronize();
cudaSetDevice(0);}

if(graph.nedges/1024< (32*1024)){
RSUM0<<<graph.nedges/1024+1,1024>>>(graph,0);
cudaDeviceSynchronize();
cudaSetDevice(0);}
cudaMemcpyFromSymbol(&hreduxsum0,reduxsum0,sizeof(unsigned int ),0,cudaMemcpyDeviceToHost);
mst=hreduxsum0;
printf("\nmst cost=%lu\n",mst);


 }
