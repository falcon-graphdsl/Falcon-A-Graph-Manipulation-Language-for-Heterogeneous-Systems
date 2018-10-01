
 #include "singlemst.h"
// cluster comm t1 
// cluster comm t2 
// cluster comm t3 
// cluster comm t4 
// cluster comm changed 
// cluster comm t1 
// cluster comm t2 
// cluster comm t3 
// cluster comm t1 
// cluster comm t2 
// clustercomm.c LIB FUNCTION MIN 3rd ARG changed to be communicated
// clustercomm.c LIB FUNCTION MIN 3rd ARG changed to be communicated
void FALCmpiinit(int argc,char **argv){
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);
MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);
  gethostname(FALChostname,255);
cudaMalloc(&FALCsendbuff,sizeof(struct FALCbuffer )*FALCsize);
cudaMalloc(&FALCrecvbuff,sizeof(struct FALCbuffer )*FALCsize);
cudaMalloc(&FALCsendsize,sizeof(int)*FALCsize);
cudaMalloc(&FALCrecvsize,sizeof(int)*FALCsize);
for(int i=0;i<FALCsize;i++){int temp=0;
cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}
 FALCstatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);
 FALCrequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);
}


 __device__ int   changed ;
;
 int   hchanged ;


 __global__ void   reset ( GGraph  graph ,GSet  set ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints){

 ((struct struct_hgraph  *)(graph.extra))->Weight[id]=99999999; 

 ((struct struct_hgraph  *)(graph.extra))->minedge[id]=99999999; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[id].lock=0; 

 }//end fun 0 

}
__global__ void   minset ( GGraph  graph ,GSet  set ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints){

  int   ch ;


 int t1;

 int t2;

 int falcft0=graph.index[id+1]-graph.index[id];
int falcft1=graph.index[id];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
t1=set./**/parent[id]; 

 ((struct struct_hgraph  *)(graph.extra))->minedge[id]=99999999; 

 t2=set./**/parent[ut1]; 

 if( t1!=t2 ){

 GMIN(&(((struct struct_hgraph  *)(graph.extra))->Weight[t1]),ut2,/*xx*/changed);//rhs not null


 GMIN(&(((struct struct_hgraph  *)(graph.extra))->Weight[t2]),ut2,/*xx*/changed);//rhs not null


 }//end

 }//foreach

 }//end fun 0 

}
__global__ void   Minedge ( GGraph  graph ,GSet  set ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints){

 int t1;

 int t2;

  int   t3 ;


 int e;

 int falcft3=graph.index[id+1]-graph.index[id];
int falcft4=graph.index[id];
/*XX*/for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
t1=set./**/parent[id]; 

 t2=set./**/parent[ut4]; 

 t3=struct struct_hgraph  temp0;/*xx*/
cudaSetDevice(0);
((struct struct_hgraph  *)(graph.extra))->getWeight(id,/*xx*/ut4); 

 if( t1!=t2 ){

 if( t3==((struct struct_hgraph  *)(graph.extra))->Weight[t1] ){

  int   unnikri =0;


 if(atomicCAS( &(((struct struct_hgraph  *)(graph.extra))->minppty[t1].lock),FALCRANK,MAX_INT-1)==FALCRANK){

 e=ut3/2; 

 ((struct struct_hgraph  *)(graph.extra))->mark[e]=1; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t1].src=p; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t1].dst=t; 

 ((struct struct_hgraph  *)(graph.extra))->Weight[t1]=t3; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t1].set=t2; 

 }//end

 }//end

 if( t3==((struct struct_hgraph  *)(graph.extra))->Weight[t2] ){

 if(atomicCAS( &(((struct struct_hgraph  *)(graph.extra))->minppty[t2].lock),FALCRANK,MAX_INT-1)==FALCRANK){

 e=ut3/2; 

 ((struct struct_hgraph  *)(graph.extra))->mark[e]=1; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t2].src=p; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t2].dst=t; 

 ((struct struct_hgraph  *)(graph.extra))->Weight[t2]=t3; 

 ((struct struct_hgraph  *)(graph.extra))->minppty[t2].set=t1; 

 }//end

 }//end

 }//end

 }//foreach

 }//end fun 0 

}
__global__ void   mstunion ( GGraph  graph ,GSet  set ,int FALCX) 
 {
/* 0 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.localpoints){

 int t1;

 int t2;

  int   t3 , t4 ;


 t1=set./**/parent[id]; 

 t2=((struct struct_hgraph  *)(graph.extra))->minppty[t1].set; 

 t3=((struct struct_hgraph  *)(graph.extra))->minppty[t1].lock; 

 t4=((struct struct_hgraph  *)(graph.extra))->minppty[t2].lock; 

 if( t1!=t2&&t3==1 ){

 setUnion(t1,/*xx*/t2); 

 changed=1; 

 }//end

 }//end fun 0 

}
__global__ void   initmark ( GGraph  graph ,int FALCX) 
 {
/* 1 xx*/int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < graph.nedges){

 ((struct struct_hgraph  *)(graph.extra))->mark[id]=999999999; 

 }//end fun 0 

}
int   main ( int   argc ,char    *  argv [ ] ) 
 {FALCmpiinit(argc,argv);
sprintf(partitionfile,"%s",argv[2]);


 GGraph  hgraph ;


 /*TE=2*/

 /*TE=2*/

 /*TE=2*/

 /*TE=2*/

 /*TE=2*///better to read graph in a temporary HGraph object and the clone to GGraph.
//Temporary HGraph object can be inside the GGraph class itself.
//For GGraph do not allocate offset in GPU. It is not needed in any kernel.
hgraph.readPointsN(partitionfile,FALCsize);
hgraph.makeNPartitionsMPI(argv[1],FALCrank,FALCsize);
int hgraphflag=0;
alloc_extra_hgraph(hgraph,hgraphflag,hgraph.npoints);
 FALCallocbuff(FALCsendbuff,FALCsize,hgraph.remotepoints);
 FALCallocbuff(FALCrecvbuff,FALCsize,hgraph.npoints);
int TPB0=1024;
; 

 GSet hset;
hset.allocate(hgraph.npoints);

 /*TE=1*///GPU ASS 
int   falcvt1;
falcvt1=0;
if(cudaMemcpyToSymbol(changed,&(falcvt1),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 7");//val=1 
t1!=t2&&t3==1 


 /*SINGLE FLAG initmark 0 */
cudaSetDevice(0);
for(int kk=0;kk<hgraph.nedges;kk+=hgraphedgekernelblocks*TPB0){ 
initmark<<<hgraphedgekernelblocks,TPB0>>>(hgraph,kk);}
cudaDeviceSynchronize();
cudaSetDevice(0);


 
 while(1)  { 

 /*TE=1*///GPU ASS 
int   falcvt2;
falcvt2=0;
if(cudaMemcpyToSymbol(changed,&(falcvt2),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 8");//val=1 
 


 /*SINGLE FLAG reset 0 */
cudaSetDevice(0);
for(int kk=0;kk<hgraph.nedges;kk+=hgraphedgekernelblocks*TPB0){ 
reset<<<hgraphedgekernelblocks,TPB0>>>(cudaSetDevice(0);
 
reset<<<hgraph.localpoints/TPB0+1,TPB0>>>(hgraph,hset,0);
cudaDeviceSynchronize();
cudaSetDevice(0);


 sendprefix<<<(hgraph.localpoints+hgraph.remotepoints)/1024+1,1024>>>(hgraph,tempWeight,tempWeight);
cudaDeviceSynchronize();
/*SINGLE FLAG minset 0 */
cudaSetDevice(0);
for(int kk=0;kk<hgraph.nedges;kk+=hgraphedgekernelblocks*TPB0){ 
minset<<<hgraphedgekernelblocks,TPB0>>>(cudaSetDevice(0);
 
minset<<<hgraph.localpoints/TPB0+1,TPB0>>>(hgraph,hset,0);
cudaDeviceSynchronize();
cudaSetDevice(0);
for(int kk=1;kk<FALCsize;kk++){
    int offstart,offend;
    offstart=hgraph.offset[kk-1];
    offend=hgraph.offset[kk];
sendbuff<<<(offend-offstart)/1024+1,1024>>>(hgraph,FALCsendsize,FALCsendbuff,tempWeight,tempWeight,kk-1,offstart,(offend-offstart));
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
    }}
FALCmsgno++;
for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i<FALCrank){
        cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.Weight), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    }    if(i>FALCrank){
        cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&FALCsendsize[i-1],sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.Weight), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);
    }}for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i<FALCrank){
        cudaMemcpy( &temp,&(FALCrecvbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.Weight,hgraph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i]);
    }    if(i>FALCrank){
        cudaMemcpy( &temp,&(FALCrecvbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.Weight,hgraph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i-1]);
    }
}//changed should be synchronized as it is a global var

FALCmsgno++;
if(FALCrank!=0){
for(int i=1;i< FALCsize;i++){
int temp;
cudaMemcpyFromSymbol(&temp,changed,sizeof(int),0,cudaMemcpyDeviceToHost);
MPI_Isend(&temp,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);
}
}
if(FALCrank==0){
    int tempchanged=0;
    int temp0;
    cudaMemcpyFromSymbol(&temp0,changed,sizeof(int),0,cudaMemcpyDeviceToHost);
    for(int i=1;i<FALCsize;i++){
        MPI_Recv(&tempchanged,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        temp0+=tempchanged;
    cudaMemcpyToSymbol(changed,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);
    }

FALCmsgno++;
    for(int i=1;i< FALCsize;i++)MPI_Isend(&temp0,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);
}
else {
    
FALCmsgno++;
    int temp0;
    MPI_Recv(&temp0,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    cudaMemcpyToSymbol(changed,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);
}

FALCmsgno++;
for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i<FALCrank){
        cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.Weight), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    }    if(i>FALCrank){
        cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&FALCsendsize[i-1],sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.Weight), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);
    }}for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i<FALCrank){
        cudaMemcpy( &temp,&(FALCrecvbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.Weight,hgraph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i]);
    }    if(i>FALCrank){
        cudaMemcpy( &temp,&(FALCrecvbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.Weight,hgraph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i-1]);
    }
}//changed should be synchronized as it is a global var

FALCmsgno++;
if(FALCrank!=0){
for(int i=1;i< FALCsize;i++){
int temp;
cudaMemcpyFromSymbol(&temp,changed,sizeof(int),0,cudaMemcpyDeviceToHost);
MPI_Isend(&temp,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);
}
}
if(FALCrank==0){
    int tempchanged=0;
    int temp0;
    cudaMemcpyFromSymbol(&temp0,changed,sizeof(int),0,cudaMemcpyDeviceToHost);
    for(int i=1;i<FALCsize;i++){
        MPI_Recv(&tempchanged,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        temp0+=tempchanged;
    cudaMemcpyToSymbol(changed,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);
    }

FALCmsgno++;
    for(int i=1;i< FALCsize;i++)MPI_Isend(&temp0,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);
}
else {
    
FALCmsgno++;
    int temp0;
    MPI_Recv(&temp0,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    cudaMemcpyToSymbol(changed,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);
}

for(int kk=0;kk<(FALCsize-1);kk++){
    MPI_Get_count(&FALCstatus[kk], MPI_INT, &FALCnamount);
    update<<< FALCnamount/1024+1,1024>>>(hgraph,FALCrecvbuff,FALCnamount,kk);
}
cudaDeviceSynchronize();
//here only master node of a point has updated value, sync it over all nodes needed. future work
for(int i=0;i<FALCsize;i++){int temp=0;
cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}


 /*SINGLE FLAG Minedge 1 */
for(int kk=1;kk<FALCsize;kk++){
    int offstart,offend;
    offstart=hgraph.offset[kk-1];
    offend=hgraph.offset[kk];
sendbuffsingle<<<(offend-offstart)/1024+1,1024>>>(hgraph,FALCsendsize,FALCsendbuff,templock,kk-1,offstart,(offend-offstart));
}
cudaDeviceSynchronize();
if(FALCRANK!=0){
    struct FALCbuffer temp;
        cudaMemcpy( &temp,&(FALCsendbuff[0]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[0]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.vid), temp1, MPI_INT,0,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
        MPI_Isend((temp.lock), temp1, MPI_INT, i ,FALCmsgno+1, MPI_COMM_WORLD,&FALCrequest[i]);
    }
    if(FALCrank==0){
for(int i=1;i<FALCsize;i++){
        cudaMemcpy( &temp,&(FALCsendbuff[0]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        MPI_Recv((temp.vid), hgraph.npoints, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);

        MPI_Recv((temp.lock), hgraph.npoints, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);

//now update here
    MPI_Get_count(&FALCstatus[i], MPI_INT, &FALCnamount);
    updatesingle<<< FALCnamount/1024+1,1024>>>(hgraph,FALCrecvbuff,FALCnamount,i);
cudaDeviceSynchronize();
    }
struct struct_hgraph  ftt;
cudaMemcpy(&ftt,( (struct struct_hgraph  *)(hgraph.extra)),sizeof(struct struct struct_hgraph ),cudaMemcpyDeviceToHost);
for(int i=1;i<FALCsize;i++)MPI_Send(temp.lock,hgraph.npoints,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCRequest[i]);
}
if(FALCRANK!=0){
struct struct_hgraph  ftt;
cudaMemcpy(&ftt,( (struct struct_hgraph  *)(hgraph.extra)),sizeof(struct struct struct_hgraph ),cudaMemcpyDeviceToHost);
MPI_Recv(temp.lock,hgraph.npoints,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCstatus[0]);
}
cudaMemcpy(( (struct struct_hgraph  *)(hgraph.extra)),sizeof(struct struct struct_hgraph ),cudaMemcpyHostToDevice);
for(int i=0;i<FALCsize;i++){int temp=0;
cudaMemcpy(&FALCsendsize[i],&temp,sizeof(int),cudaMemcpyHostToDevice);}
cudaSetDevice(0);
for(int kk=0;kk<hgraph.nedges;kk+=hgraphedgekernelblocks*TPB0){ 
Minedge<<<hgraphedgekernelblocks,TPB0>>>(cudaSetDevice(0);
 
Minedge<<<hgraph.localpoints/TPB0+1,TPB0>>>(hgraph,hset,0);
cudaDeviceSynchronize();
cudaSetDevice(0);


 /*SINGLE FLAG mstunion 0 */
cudaSetDevice(0);
for(int kk=0;kk<hgraph.nedges;kk+=hgraphedgekernelblocks*TPB0){ 
mstunion<<<hgraphedgekernelblocks,TPB0>>>(cudaSetDevice(0);
 
mstunion<<<hgraph.localpoints/TPB0+1,TPB0>>>(hgraph,hset,0);
cudaDeviceSynchronize();
cudaSetDevice(0);
cudaSetDevice(0);
for(int kk=0;ii<hgraph.tot_size;kk+=hgraphpointkernelblocksize*TPB0){
findset<<<hgraphpointkernelblcoksize,TPB0>>>( hset,kk);
}
cudaDeviceSynchronize();cudaSetDevice(0);


 
 //GPU IF STMT 
int   falcvt3;
if(cudaMemcpyFromSymbol(&(falcvt3),changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 9");
if(falcvt3==0)break;

 }//end

  unsigned  int   mst =0;


 
 if( ((struct struct_hgraph  *)(hgraph.extra))->mark[(null)/2]==1 )mst=mst+hgraph.edges[(null)+1].ipe; 
 }//foreach

 printf("mst cost=%lu",/*xx*/mst);//rhs not null


 MPI_Finalize();
}//end fun 0 
