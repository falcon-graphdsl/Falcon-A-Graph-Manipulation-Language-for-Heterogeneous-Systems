
 #include "singcollbfs.h"
cudaDeviceProp prop0;
__device__ int   changed =0, coll1cnt =0, coll2cnt =0;
;
 int   hchanged ;


 __global__ void   relaxgraph ( GGraph  graph ,GColl coll1,GColl coll2,int   val ,int FALCX) 
 {
int id;
 if(threadIdx.x+blockDim.x*blockIdx.x>=(*(coll1.size)))return; 
id=coll1.ptr[threadIdx.x+blockDim.x*blockIdx.x];
int p=id;
if( id < graph.npoints){

 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
int t=ut1;
if( ((struct struct_hgraph  *)(graph.extra))->dist[ut1]>(val+1) )
{

 ((struct struct_hgraph  *)(graph.extra))->dist[ut1]=val+1; 

 int falcvt1;

falcvt1=GADD(&(coll2.size[0]),1);
coll2.ptr[falcvt1]=t; 

 changed=1; 

 }

 }

 }

}
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int t=id;
if( id < graph.npoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=1234567890; 

 }

}
void   SSSP ( char    *  name ) 
 {

 HGraph  hgraph ;

 

 GGraph graph;


  int   xx =0, temp =0;


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


 thrust::device_vector<int> coll1temp(graph.npoints);
GColl coll1;
  coll1.ptr=thrust::raw_pointer_cast(&coll1temp[0]);
alloccoll1size(coll1);
thrust::device_vector<int> coll2temp(graph.npoints);
GColl coll2;
  coll2.ptr=thrust::raw_pointer_cast(&coll2temp[0]);
alloccoll2size(coll2);
thrust::device_vector<int> coll3temp(graph.npoints);
GColl coll3;
  coll3.ptr=thrust::raw_pointer_cast(&coll3temp[0]);
alloccoll3size(coll3);


 int   falcvt2;
falcvt2=0;
struct struct_hgraph  temp1;
 cudaMemcpy(&temp1,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp1.dist[0]),&(falcvt2),sizeof(int ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1");

 ((struct struct_hgraph  *)(hgraph.extra))->dist[0]=0; 

  double   rt1 =rtclock();


 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
reset<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 int falcvt3;
cudaMemcpy(&falcvt3,&(coll1.size[0]),sizeof(int),cudaMemcpyDeviceToHost);
coll1temp[ falcvt3++]=0;
cudaMemcpy(&(coll1.size[0]),&falcvt3,sizeof(int),cudaMemcpyHostToDevice); 

 int falcvt4;
cudaMemcpy(&falcvt4,&(coll1.size[0]),sizeof(int),cudaMemcpyDeviceToHost);
relaxgraph<<<(falcvt4)/prop0.maxThreadsPerBlock+1,prop0.maxThreadsPerBlock>>>(graph,coll1,coll2,xx,0);
cudaDeviceSynchronize();


 
 while(1)  { 

 int   falcvt5;
falcvt5=0;
if(cudaMemcpyToSymbol(changed,&(falcvt5),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 2");

 coll3.ptr=coll1.ptr;


 coll1.ptr=coll2.ptr;


 coll2.ptr=coll3.ptr;


 cudaMemcpy(&temp,coll2.size,sizeof(int ),cudaMemcpyDeviceToHost);


 cudaMemcpy(coll1.size,&temp,sizeof(int),cudaMemcpyHostToDevice);


 temp=0; 

 cudaMemcpy(coll2.size,&temp,sizeof(int),cudaMemcpyHostToDevice);


 xx++; 

 int falcvt6;
cudaMemcpy(&falcvt6,&(coll1.size[0]),sizeof(int),cudaMemcpyDeviceToHost);
relaxgraph<<<(falcvt6)/prop0.maxThreadsPerBlock+1,prop0.maxThreadsPerBlock>>>(graph,coll1,coll2,xx,0);
cudaDeviceSynchronize();


 int   falcvt7;
if(cudaMemcpyFromSymbol(&(falcvt7),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 3");
if(falcvt7==0)break;

 }

  double   rt2 =rtclock();


 printf(" TIME %f \n",(rt2-rt1)*1000);


  int   maxdist =0;


 struct struct_hgraph  temp2;/*err*/
cudaMemcpy(&temp2,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy((((struct struct_hgraph  *)(hgraph.extra))->dist),(temp2.dist),sizeof(int)*hgraph.npoints,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 4");

 for (int   i =0;i<hgraph.npoints;i++) {

 if( maxdist<((struct struct_hgraph  *)(hgraph.extra))->dist[i]  )
maxdist=((struct struct_hgraph  *)(hgraph.extra))->dist[i]; 

 }

 printf("\nMAXDIST=%d\n",maxdist);


 }
int   main ( int   argc ,char    *  argv [ ] ) 
 {
cudaGetDeviceProperties(&prop0,0); 
alloc_sync_array();


 if( argc!=2  )
{

 printf("exec inputgraph\n");


 return 1;

 }

 SSSP(argv[1]);


 }
