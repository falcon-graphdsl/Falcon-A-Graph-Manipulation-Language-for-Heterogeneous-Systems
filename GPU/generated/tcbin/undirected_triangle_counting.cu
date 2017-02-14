
 #include "undirected_triangle_counting.h"
cudaDeviceProp prop0;
__device__ int   changed =0;
;
 int   hchanged ;


 __global__ void   TC ( GGraph  graph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int p=id;
if( id < graph.npoints){

 struct dev_vec coll;
coll= create_coll_device(100);


  int   ch ;


 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
int t=ut1;
int t1;

 t1=t; 

 if( graph.points[t1].ipe<=graph.points[id].ipe )
continue;

 add_coll_device(coll,t1); 

 }

 for (int   i =0;i<coll.size;i++) {

 int p2;

 p2=coll.D_Vec[i]; 

 int falcft3=graph.index[p2+1]-graph.index[p2];
int falcft4=graph.index[p2];
for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
int t2=ut4;
int t1;

 t1=t2; 

 if( graph.points[t1].ipe<=graph.points[p2].ipe )
continue;

 for (int   i =0;i<coll.size;i++) {

 int p3;

 p3=coll.D_Vec[i]; 

 if( graph.points[p3].ipe==graph.points[t1].ipe )
GADD(&(changed),1);


 }

 }

 }

 }

}
int   main ( int   argc ,char    *  name [ ] ) 
 {
cudaGetDeviceProperties(&prop0,0); 
alloc_sync_array();


 if( argc!=2  )
{

 printf("error: exec filename");


 return 1;

 }

 HGraph  hgraph ;

 

 hgraph.read2(name[1]);

int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 GGraph graph;


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


 int   falcvt1;
falcvt1=0;
struct struct_hgraph  temp1;
 cudaMemcpy(&temp1,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp1.dist[0]),&(falcvt1),sizeof(int ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1");

 int   falcvt2;
falcvt2=0;
if(cudaMemcpyToSymbol(changed,&(falcvt2),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 2");

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
TC<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 t2=rtclock();


 printf("\nTIME =%f\n",(t2-t1)*1000);


  int   count ;


 if(cudaMemcpyFromSymbol(&(count),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 3");

 printf("\ntriangles=%d \n",count);


 return ;

 }
