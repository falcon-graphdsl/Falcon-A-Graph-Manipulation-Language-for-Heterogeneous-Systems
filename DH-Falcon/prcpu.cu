
 #include "prcpu.h"
// checklibfub ADD 
// LIBFUN ADD 
//libfunction call ADD
void FALCmpiinit(int argc,char **argv){
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);
MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);
  gethostname(FALChostname,255);
cudaSetDevice(FALCrank);
cudaMalloc(&FALCsendbuff,sizeof(struct FALCbuffer )*FALCsize);
cudaMalloc(&FALCrecvbuff,sizeof(struct FALCbuffer )*FALCsize);
cudaMalloc(&FALCsendsize,sizeof(int)*FALCsize);
cudaMalloc(&FALCrecvsize,sizeof(int)*FALCsize);
for(int i=0;i<FALCsize;i++){int temp=0;
cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}
 FALCstatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);
 FALCrequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);
}
 int   changed =0, hchanged =0;


  int   fchanged =0, fhchanged =0;


 __global__ void   relaxgraph ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.npoints){

  int   outs =graph.index[p+1].ipe - graph.index[p].ipe;


  float   sum =((struct struct_hgraph  *)(graph.extra))->PR[id]/outs;


 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
GADD(&(((struct struct_hgraph  *)(graph.extra))->val/*xx*/[ut1]),sum,/*xx*/changed);//rhs not null


 }//foreach

 }//end fun 0 

}
__global__ void   reset ( GGraph  graph ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.npoints){

 ((struct struct_hgraph  *)(graph.extra))->PR[id]=1/graph.npoints; 

 ((struct struct_hgraph  *)(graph.extra))->val[id]=0.000000; 

 }//end fun 0 

}
__global__ void   reset1 ( GGraph  graph ,float   d ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.npoints){

  float   sum =d*((struct struct_hgraph  *)(graph.extra))->val[id]+(1-d)/graph.npoints;


 ((struct struct_hgraph  *)(graph.extra))->PR[id]=sum; 

 ((struct struct_hgraph  *)(graph.extra))->val[id]=0.000000; 

 }//end fun 0 

}
void   PageRank ( char    *  name ) 
 {

 GGraph  hgraph ;


  int   cnt =0;


 /*TE=2*/

 /*TE=2*/

 /*TE=2*/hgraph.readPointsN(partitionfile,FALCsize);
hgraph.makeNPartitionsMPI(name,FALCrank,FALCsize);
; 

  float   d =0.850000;


 /*SINGLE FLAG reset 0 */
cudaSetDevice(FALCrank);
 
reset<<<hgraph.npoints/TPB0+1,TPB0>>>(hgraph,kk);

cudaDeviceSynchronize();
cudaSetDevice(FALCrank);


 
 while(cnt<100)  { 

 changed=0; 

 sendprefix<<<(hgraph.localpoints+hgraph.remotepoints)/1024+1,1024>>>(hgraph,tempval);
cudaDeviceSynchronize();
/*SINGLE FLAG relaxgraph 0 */
cudaSetDevice(FALCrank);
 
relaxgraph<<<hgraph.localpoints/TPB0+1,TPB0>>>(hgraph,kk);

cudaDeviceSynchronize();
cudaSetDevice(FALCrank);
for(int kk=1;kk<FALCsize;kk++){
    int offstart,offend;
    offstart=hgraph.offset[kk-1];
    offend=hgraph.offset[kk];
sendbuff<<<(offend-offstart)/1024+1,1024>>>(hgraph,FALCsendsize,FALCsendbuff,tempval,kk-1,offstart,(offend-offstart));
}
cudaDeviceSynchronize();
for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i<FALCrank){
        cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.vid), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    }    if(i>FALCrank){
        cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.vid), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);

    }}for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i<FALCrank){
        cudaMemcpy( &temp,&FALCrecvbuff[i],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.vid,hgraph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }    if(i>FALCrank){
        cudaMemcpy( &temp,&FALCrecvbuff[i-1],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.vid,hgraph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }}//EXTRA DATATYPE val
//1 7 

FALCmsgno++;
for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i<FALCrank){
        cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.val), temp1, MPI_FLOAT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    }    if(i>FALCrank){
        cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&FALCsendsize[i-1],sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.val), temp1, MPI_FLOAT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);
    }}for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i<FALCrank){
        cudaMemcpy( &temp,&(FALCrecvbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.val,hgraph.npoints, MPI_FLOAT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i]);
    }    if(i>FALCrank){
        cudaMemcpy( &temp,&(FALCrecvbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.val,hgraph.npoints, MPI_FLOAT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i-1]);
    }
}
for(int kk=0;kk<(FALCsize-1);kk++){
    MPI_Get_count(&FALCstatus[kk], MPI_INT, &FALCnamount);
    update<<< FALCnamount/1024+1,1024>>>(hgraph,FALCrecvbuff,FALCnamount,kk);
}
cudaDeviceSynchronize();
//here only master node of a point has updated value, sync it over all nodes needed. future work
for(int i=0;i<FALCsize;i++){int temp=0;
cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}


 if( cnt<100 )break;

 /*SINGLE FLAG reset1 0 */
cudaSetDevice(FALCrank);
 
reset1<<<hgraph.localpoints/TPB0+1,TPB0>>>(hgraph,d,kk);

cudaDeviceSynchronize();
cudaSetDevice(FALCrank);


 cnt++; 

 }//end

 struct struct_hgraph   temp0;
 cudaMemcpy((void *)&temp0,hgraph.extra,sizeof( struct struct_hgraph ),cudaMemcpyDeviceToHost);
struct struct_hgraph   temp1;
 cudaMemcpy((void *)&temp1,hgraph.extra,sizeof( struct struct_hgraph ),cudaMemcpyDeviceToHost);
float  *temp3=(float  *) malloc(sizeof(float )*hgraph.npoints);
struct struct_hgraph   temp4;
 cudaMemcpy((void *)&temp4,hgraph.extra,sizeof(struct struct_hgraph  ),cudaMemcpyDeviceToHost);
cudaMemcpy(temp3, temp4.PR,sizeof(float )*hgraph.npoints,cudaMemcpyDeviceToHost);
for (int   i =0;i<hgraph.npoints;i++) {

 printf("%f\n",/*xx*/temp3[i]);//rhs not null


 }//endfor

 return ;

 }//end fun 0 
int   main ( int   argc ,char    *  argv [ ] ) 
 {FALCmpiinit(argc,argv);
sprintf(partitionfile,"%s",argv[2]);


 PageRank(argv[1]);//rhs not null


 MPI_Finalize();
}//end fun 0 
