
 #include "singcollbfs.h"
cudaDeviceProp prop0;
// cluster comm changed 
// cluster comm changed 
void FALCmpiinit(int argc,char **argv){
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);
MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);
FALCsize*=2;
  gethostname(FALChostname,255);
FALCsendbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);
FALCrecvbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);
cudaMalloc(&FALCgpusendbuff,sizeof(struct FALCbuffer )*FALCsize);
cudaMalloc(&FALCgpurecvbuff,sizeof(struct FALCbuffer )*FALCsize);
FALCsendsize=(int *)malloc(sizeof(int)*FALCsize);
FALCrecvsize=(int *)malloc(sizeof(int)*FALCsize);
cudaMalloc(&FALCgpusendsize,sizeof(int)*FALCsize);
cudaMalloc(&FALCgpurecvsize,sizeof(int)*FALCsize);
for(int i=0;i<FALCsize;i++){
int temp=0;
cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);
FALCsendsize[i]=FALCrecvsize[i]=0;}
 FALCcpustatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);
 FALCcpurequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);
 FALCgpustatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);
 FALCgpurequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);
}
__device__ int   changed =0,GPU_changed, coll1cnt =0,GPU_coll1cnt, coll2cnt =0,GPU_coll2cnt,GPU_hchanged;
;


 __global__ void   relaxgraph ( GGraph  graph ,GColl coll1,GColl coll2,int   val ,int FALCX) 
 {
int id;
 if(threadIdx.x+blockDim.x*blockIdx.x>=coll1.size)return; 
id=coll1.ptr[threadIdx.x+blockDim.x*blockIdx.x];
if( id < graph.localpoints){

 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
if( ((struct struct_hgraph  *)(graph.extra))->dist/*here1*//*xx*/[ut1]>(val/*here1*/+1) ){

 ((struct struct_hgraph  *)(graph.extra))->dist/*here1*//*xx*/[ut1]=val/*here1*/+1; 

 int falcvt1;

falcvt1=GADD(&(coll2.size[0]),1);
coll2.ptr[falcvt1]=t/*here1*/; 

 changed/*here1*/=1; 

 }//end

 }//foreach

 }//end fun 0 

}
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints+graph.remotepoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=1234567890; 

 }//end fun 0 

}
void   SSSP ( char    *  name ) 
 {

 /*s1 0 0*/HGraph  hgraph ;

 

 GGraph graph;


 

 hgraph.readPointsN(partitionfile,FALCsize);
hgraph.makeNPartitionsMPI(name/*here1*/,2*FALCrank,FALCsize);
hgraph.gpupart.readPointsN(partitionfile,FALCsize);
hgraph.gpupart.makeNPartitionsMPI(name/*here1*/,2*FALCrank+1,FALCsize);
 FALCallocbuff(FALCsendbuff,FALCsize,hgraph.remotepoints);
 FALCallocbuff(FALCrecvbuff,FALCsize,hgraph.npoints);
 FALCgpuallocbuff(FALCgpusendbuff,FALCsize,hgraph.gpupart.remotepoints);
 FALCgpuallocbuff(FALCgpurecvbuff,FALCsize,hgraph.gpupart.npoints);
int TPB0=1024;
alloc_extra_hgraph(hgraph.gpupart,0);
int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 /*TE=1*///GPU ASS 
hgraph/*here1*/.cloneGPU(graph/*here1*/,0 );
int graph/*here1*/flag=0;

alloc_extra_graph(graph,graphflag,graph.npoints);

int TPB0=findthreadsperblock(&prop0);

int graphpointkernelblocks=findblocksize(graph,graph.npoints,TPB0);

int graphedgekernelblocks=findblocksize(graph,graph.nedges,TPB0);
copygraph/*here1*/currentsize(graph/*here1*/);
cudaSetDevice(0);
//val=1 
coll2.ptr[falcvt1]=t/*here1*/graph/*here1*/=hgraph/*here1*/; 


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


 /*TE=1*///GPU ASS 
int   falcvt2;
falcvt2=0;
struct struct_hgraph  temp1;
 cudaMemcpy(&temp1,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp1.dist/*here1*/[0]),&(falcvt2),sizeof(int ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1");/***/((struct struct_hgraph  *)(graph.extra))->dist/*here1*/[0]=0; 


 /*XX3*/((struct struct_hgraph  *)(hgraph.hostparts[0].extra))->dist/*here1*/[0]=0; 

 cudaSetDevice(0);
 
reset<<<graph.gpupart.npoints/TPB0+1,TPB0>>>(graph,0);


 /*TE=2*/int falcvt3;
cudaMemcpy(&falcvt3,&(coll1.size[0]),sizeof(int),cudaMemcpyDeviceToHost);
coll1temp[ falcvt3++]=0;
cudaMemcpy(&(coll1.size[0]),&falcvt3,sizeof(int),cudaMemcpyHostToDevice); 

 int falcvt4;
cudaMemcpy(&falcvt4,&(coll1.size[0]),sizeof(int),cudaMemcpyDeviceToHost);
relaxgraph<<<(falcvt4)/prop0.maxThreadsPerBlock+1,prop0.maxThreadsPerBlock>>>(graph,coll1,coll2,xx,0);
cudaDeviceSynchronize();


 
 while(1)  { 

 /*TE=1*///GPU ASS 
int   falcvt5;
falcvt5=0;
if(cudaMemcpyToSymbol(GPU_changed/*here1*/,&(falcvt5),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 2");//val=1 
changed/*here1*/=0; 


 /*TE=3*///GPU ASS 
//val=3 
coll3.ptr=coll1.ptr;
coll3/*here1*/=coll1/*here1*/; 


 /*TE=3*///GPU ASS 
//val=3 
coll1.ptr=coll2.ptr;
coll1/*here1*/=coll2/*here1*/; 


 /*TE=3*///GPU ASS 
//val=3 
coll2.ptr=coll3.ptr;
coll2/*here1*/=coll3/*here1*/; 


 /*TE=2*///GPU ASS 
//val=2 
//Dtype -1 4=
cudaMemcpy(&temp/*here1*/,coll2.size/**/,sizeof(int),cudaMemcpyDeviceToHost);
temp/*here1*/=coll2.size/**/; 


 /*TE=1*///GPU ASS 
cudaMemcpy(coll1.size/**/,&temp/*here1*/,sizeof(int),cudaMemcpyHostToDevice);
coll1.size/**/=temp/*here1*/; 


 temp/*here1*/=0; 

 /*TE=1*///GPU ASS 
cudaMemcpy(coll2.size/**/,&temp/*here1*/,sizeof(int),cudaMemcpyHostToDevice);
coll2.size/**/=temp/*here1*/; 


 xx/*here1*/++; 

 int falcvt6;
cudaMemcpy(&falcvt6,&(coll1.size[0]),sizeof(int),cudaMemcpyDeviceToHost);
relaxgraph<<<(falcvt6)/prop0.maxThreadsPerBlock+1,prop0.maxThreadsPerBlock>>>(graph,coll1,coll2,xx,0);
cudaDeviceSynchronize();


 
 //GPU IF STMT 
int   falcvt7;
if(cudaMemcpyFromSymbol(&(falcvt7),changed/*here1*/,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 3");
if(falcvt7==0)break;

 }//end

 }//end fun 0 
int   main ( int   argc ,char    *  argv [ ] ) 
 {FALCmpiinit(argc,argv);
sprintf(partitionfile,"%s",argv[2]);


 SSSP(argv[1]);//rhs not null


 MPI_Finalize();
}//end fun 0 
TARGET= 5 
