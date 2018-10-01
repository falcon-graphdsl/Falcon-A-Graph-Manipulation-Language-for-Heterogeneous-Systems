
 #include "singlebfs.h"
cudaDeviceProp prop0;
// clustercomm.c LIB FUNCTION MIN 3rd ARG changed to be communicated
void FALCmpiinit(int argc,char **argv){
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);
MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);
  gethostname(FALChostname,255);
FALCsendbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);
FALCrecvbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);
FALCsendsize=(int *)malloc(sizeof(int)*FALCsize);
FALCrecvsize=(int *)malloc(sizeof(int)*FALCsize);
for(int i=0;i<FALCsize;i++){
FALCsendsize[i]=FALCrecvsize[i]=0;}
 FALCstatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);
 FALCrequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);
}
__device__ int   changed =0;
;
 int   hchanged ;


 __global__ void   reset ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.npoints){

 ((struct struct_hgraph  *)(graph.extra))->dist[id]=1234567890; 

 }//end fun 0 

}
__global__ void   BFS ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.npoints){

  int   ch ;


 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
GMIN(&(((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut1]),((struct struct_hgraph  *)(graph.extra))->dist[id]+1,/*xx*/changed);//rhs not null


 }//foreach

 }//end fun 0 

}
int   main ( int   argc ,char    *  name [ ] ) 
 {FALCmpiinit(argc,argv);
sprintf(partitionfile,"%s",argv[2]);


 /*s1 0 0*/HGraph  hgraph ;

 

 hgraph.readPointsN(partitionfile,FALCsize);
hgraph.makeNPartitionsMPI(name[1],FALCrank,FALCsize);
int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
 FALCallocbuff(FALCsendbuff,FALCsize,hgraph.remotepoints);
 FALCallocbuff(FALCrecvbuff,FALCsize,hgraph.npoints);
; 

 GGraph graph;


 /*TE=1*///GPU ASS 
hgraph.cloneGPU(graph,0 );
int graphflag=0;

alloc_extra_graph(graph,graphflag,graph.npoints);

int TPB0=findthreadsperblock(&prop0);

int graphpointkernelblocks=findblocksize(graph,graph.npoints,TPB0);

int graphedgekernelblocks=findblocksize(graph,graph.nedges,TPB0);
copygraphcurrentsize(graph);
cudaSetDevice(0);
//val=1 
 


 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
reset<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);
cudaDeviceSynchronize();
cudaSetDevice(0);


 /*TE=1*///GPU ASS 
int   falcvt1;
falcvt1=0;
struct struct_hgraph  temp1;
 cudaMemcpy(&temp1,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy(&(temp1.dist[0]),&(falcvt1),sizeof(int ),cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1"); 


 
 while(1)  { 

 /*TE=1*///GPU ASS 
int   falcvt2;
falcvt2=0;
if(cudaMemcpyToSymbol(changed,&(falcvt2),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 2");//val=1 
 


 cudaSetDevice(0);
for(int kk=0;kk<graph.npoints;kk+=graphpointkernelblocks*TPB0){ 
BFS<<<graphpointkernelblocks,TPB0>>>(graph,kk);
}
cudaDeviceSynchronize();
cudaSetDevice(0);
cudaDeviceSynchronize();
cudaSetDevice(0);
for(int kk=1;kk<FALCsize;kk++){
    #pragma omp parallel for   num_threads(32)
    for(int i=graph.offset[kk-1];i<graph.offset[kk];i++){
        sendbuff(i,graph,FALCsendsize,FALCsendbuff,kk-1);
    }
}
for(int i=0;i<FALCsize;i++){
    if(i<FALCrank)
        MPI_Isend((FALCsendbuff[i].vid), FALCsendsize[i], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    if(i>FALCrank)
        MPI_Isend((FALCsendbuff[i-1].vid), FALCsendsize[i-1], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);
}
for(int i=0;i<FALCsize;i++){
    if(i<FALCrank)
        MPI_Recv(FALCrecvbuff[i].vid,graph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    if(i>FALCrank)
    MPI_Recv(FALCrecvbuff[i-1].vid,graph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}

FALCmsgno++;
for(int i=0;i<FALCsize;i++){
    if(i<FALCrank)
        MPI_Isend((FALCsendbuff[i].dist), FALCsendsize[i], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    if(i>FALCrank)
        MPI_Isend((FALCsendbuff[i-1].dist), FALCsendsize[i-1], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);
}
for(int i=0;i<FALCsize;i++){
    if(i<FALCrank)
        MPI_Recv(FALCrecvbuff[i].dist,graph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i]);
    if(i>FALCrank)
        MPI_Recv(FALCrecvbuff[i-1].dist,graph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i-1]);
}
//changed should be synchronized as it is a global var

FALCmsgno++;
if(FALCrank!=0)for(int i=1;i< FALCsize;i++)MPI_Isend(&changed,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);
if(FALCrank==0){
    int tempchanged=0;
    for(int i=1;i<FALCsize;i++){
        MPI_Recv(&tempchanged,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        changed+=tempchanged;
    }

FALCmsgno++;
    for(int i=1;i< FALCsize;i++)MPI_Isend(&changed,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);
}
else {
    
FALCmsgno++;
    MPI_Recv(&changed,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}
for(int kk=0;kk<(FALCsize-1);kk++){
    MPI_Get_count(&FALCstatus[kk], MPI_INT, &FALCnamount);
    #pragma omp parallel for   num_threads(32)
    for(int i=0;i<FALCnamount;i++){ 
        int vertex= FALCrecvbuff[kk].vid[i];
        if( ( ( struct struct_hgraph  * )(graph.extra))->dist[vertex] > FALCrecvbuff[kk].dist[i])
            ( ( struct struct_hgraph  * )(graph.extra))->dist[vertex] = FALCrecvbuff[kk].dist[i];
    }
}
//here only master node of a point has updated value, sync it over all nodes needed. future work
for(int i=0;i<FALCsize;i++)FALCsendsize[i]=0;

 /*TE=2*///GPU ASS 
//val=2 
//Dtype -1 -1=
if(cudaMemcpyFromSymbol(&(hchanged),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 3"); 


 if( hchanged==0 )break;

 }//end

 /*TE=2*///GPU ASS 
//val=2 
struct struct_hgraph  temp2;/*xx*/
cudaSetDevice(0);
cudaMemcpy(&temp2,((struct struct_hgraph  *)(graph.extra)),sizeof(struct struct_hgraph ),cudaMemcpyDeviceToHost);
if(cudaMemcpy((((struct struct_hgraph  *)(hgraph.extra))->dist),(temp2.dist),sizeof(int)*hgraph.npoints,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 4"); 


 for (int   i =0;i<hgraph.npoints;i++)printf("%d\n",/*xx*//***/((struct struct_hgraph  *)(hgraph.extra))->dist[i]);

 return ;

 MPI_Finalize();
}//end fun 0 
