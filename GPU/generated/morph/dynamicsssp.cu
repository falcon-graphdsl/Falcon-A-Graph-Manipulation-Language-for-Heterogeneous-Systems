
 #include "dynamicsssp.h"
cudaDeviceProp prop0;
__device__ int   changed =0;
;
 int   hchanged =0;


 void   processincrementation ( char    *  name ,HGraph & hgraph ) 
 {

 FILE*fp1; 

 fp1=fopen(name,"r");


  int   src , dst , weight ;


  int   i =0;


 
 while(fscanf(fp1,"%d%d%d",&src,&dst,&weight)!=EOF)  { 

 hgraph.addEdge( src,dst,weight); 

 ((struct struct_hgraph  *)(hgraph.extra))->updated[src]=true; 

 i++; 

 }

 }
__global__ void   relaxgraph ( GGraph  graph ,int FALCX) 
 {
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
 
int p=id;
if( id < graph.npoints&& ((struct struct_hgraph  *)(graph.extra))->/*4*/updated[id]==true ){

 ((struct struct_hgraph  *)(graph.extra))->updated[id]=false; 

 int falcft0=graph.outtotal[id];
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
void   SSSP ( char    *  name ,char    *  name1 ) 
 {

 HGraph  hgraph ;

 

 

 GGraph graph;


 

 hgraph.readMorph(name);

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
if(cudaMemcpy(&(temp1.updated[0]),&(falcvt1),sizeof(bool),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 6");

 int   falcvt2;
falcvt2=0;
struct struct_hgraph  temp2;
 cudaMemcpy(&temp2,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp2.dist[0]),&(falcvt2),sizeof(int ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 7");

 
 while(1)  { 

 int   falcvt3;
falcvt3=0;
if(cudaMemcpyToSymbol(changed,&(falcvt3),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 8");

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
relaxgraph<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 int   falcvt4;
if(cudaMemcpyFromSymbol(&(falcvt4),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 9");
if(falcvt4==0)break;

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
reset1<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 }

 struct struct_hgraph  temp3;/*err*/
cudaMemcpy(&temp3,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy((((struct struct_hgraph  *)(hgraph.extra))->dist),(temp3.dist),sizeof(int)*hgraph.npoints,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 10");

 t2=rtclock();


 printf("\nTIME =%f",(t2-t1)*1000);


  int   maxdist =0;


 for (int   i =0;i<hgraph.npoints;i++) {

 if( maxdist<((struct struct_hgraph  *)(hgraph.extra))->dist[i]  )
maxdist=((struct struct_hgraph  *)(hgraph.extra))->dist[i]; 

 }

 printf("\nMAXDIST=%d\n",maxdist);


 processincrementation(name1,hgraph);


 struct struct_hgraph  temp4;/*err*/
cudaMemcpy(&temp4,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);

//updated updated
//updated dist
if(cudaMemcpy((temp4.updated),(((struct struct_hgraph  *)(hgraph.extra))->updated),sizeof(bool)*graph.npoints,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 11");

 hgraph.copytoGPU(graph);cudaSetDevice(0);


 
 while(1)  { 

 int   falcvt5;
falcvt5=0;
if(cudaMemcpyToSymbol(changed,&(falcvt5),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 12");

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
relaxgraph<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 int   falcvt6;
if(cudaMemcpyFromSymbol(&(falcvt6),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 13");
if(falcvt6==0)break;

 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
reset1<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);


 }

 struct struct_hgraph  temp5;/*err*/
cudaMemcpy(&temp5,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy((((struct struct_hgraph  *)(hgraph.extra))->dist),(temp5.dist),sizeof(int)*hgraph.npoints,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 14");

 maxdist=0; 

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


 if( argc!=3  )
{

 printf("error:-exec   file1 file2");


 return 1;

 }

 SSSP(argv[1],argv[2]);


 }
