
 #include "kcore.h"
// checklibfub ADD 
// LIBFUN ADD 
//libfunction call ADD
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


 void   relaxgraph1 ( int & p , /**u1**/HGraph & graph ,int   level ) 
 {

 ((struct struct_hgraph  *)(graph.extra))->dist1[p]=false; 

 int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
((struct struct_hgraph  *)(graph.extra))->dist1/*xx*/[ut1]=false; 

 }//foreach

 }//end fun 0 
void   relaxgraph ( int & p , /**u1**/HGraph & graph ,int   level ) 
 {
 if( ((struct struct_hgraph  *)(graph.extra))->updated[p]==1 ){

 int falcft3=graph.index[p+1]-graph.index[p];
int falcft4=graph.index[p];
/*XX*/for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
HADD(&(((struct struct_hgraph  *)(graph.extra))->dist1/*xx*/[ut4]),1,/*xx*/changed);//rhs not null


 }//foreach

 }//end fun 0 

}
void   reset ( int & t , /**u1**/HGraph & graph ,int   level ) 
 {

 ((struct struct_hgraph  *)(graph.extra))->dist[t]=graph.index[t+1].ipe - graph.index[t].ipe; 

 ((struct struct_hgraph  *)(graph.extra))->updated[t]=0; 

 if( ((struct struct_hgraph  *)(graph.extra))->dist[t]<level )((struct struct_hgraph  *)(graph.extra))->updated[t]=1; 

 }//end fun 0 
void   reset1 ( int & t , /**u1**/HGraph & graph ,int   level ) 
 {

 ((struct struct_hgraph  *)(graph.extra))->dist[t]=((struct struct_hgraph  *)(graph.extra))->dist[t]-((struct struct_hgraph  *)(graph.extra))->dist1[t]; 

 if( ((struct struct_hgraph  *)(graph.extra))->dist[t]<level&&((struct struct_hgraph  *)(graph.extra))->updated[t]==0 )((struct struct_hgraph  *)(graph.extra))->updated[t]=1; 

 if( ((struct struct_hgraph  *)(graph.extra))->updated[t]==1 )((struct struct_hgraph  *)(graph.extra))->updated[t]==2; 

 }//end fun 0 
void   SSSP ( char    *  name ) 
 {

 /*s1 0 0*/HGraph  hgraph ;

 

 

 

 hgraph.readPointsN(partitionfile,FALCsize);
hgraph.makeNPartitionsMPI(name,2*FALCrank,FALCsize);
hgraph.gpupart.readPointsN(partitionfile,FALCsize);
hgraph.gpupart.makeNPartitionsMPI(name,2*FALCrank+1,FALCsize);
 FALCallocbuff(FALCsendbuff,FALCsize,hgraph.remotepoints);
 FALCallocbuff(FALCrecvbuff,FALCsize,hgraph.npoints);
 FALCgpuallocbuff(FALCgpusendbuff,FALCsize,hgraph.gpupart.remotepoints);
 FALCgpuallocbuff(FALCgpurecvbuff,FALCsize,hgraph.gpupart.npoints);
int TPB0=1024;
alloc_extra_hgraph(hgraph.gpupart,0);
int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 /*SINGLE FLAG reset 0 */
cudaSetDevice(0);
 
reset<<<hgraph.gpupart.npoints/TPB0+1,TPB0>>>(hgraph,0);
/*SINGLE FLAG reset 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints+hgraph.remotepoints;i++)reset(i,hgraph);

}
}


 
 while(lev<100)  { 

 /*TE=1*///GPU ASS 
int   falcvt1;
falcvt1=0;
if(cudaMemcpyToSymbol(GPU_changed,&(falcvt1),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 0");//val=1 
changed=0; 


 sendprefix<<<(hgraph.localpoints+hgraph.remotepoints)/1024+1,1024>>>(hgraph,tempgpudist1);
cudaDeviceSynchronize();
#pragma omp parallel for num_threads(32)
for(int i=0;i<hgraph.localpoints+hgraph.remotepoints;i++){
    tempdist1[i]= (( struct struct_hgraph  *)(hgraph.extra))->dist1[i];
}
/*SINGLE FLAG relaxgraph 0 */
cudaSetDevice(0);
 
relaxgraph<<<hgraph.gpupart.localpoints/TPB0+1,TPB0>>>(hgraph,0);
/*SINGLE FLAG relaxgraph 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints;i++)relaxgraph(i,hgraph);

}
}
for(int kk=1;kk<FALCsize;kk++){
    int offstart,offend;
    offstart=hgraph.gpupart.offset[kk-1];
    offend=hgraph.gpupart.offset[kk];
sendbuff<<<(offend-offstart)/1024+1,1024>>>(hgraph.gpupart,FALCgpusendsize,FALCgpusendbuff,tempgpudist1,kk-1,offstart,(offend-offstart));
cudaDeviceSynchronize();
    #pragma omp parallel for   num_threads(32)
    for(int i=hgraph.offset[kk]-1;i<hgraph.offset[kk];i++){
        sendbuff(i,hgraph,FALCsendsize,FALCsendbuff,tempdist1,kk-1);
}
    }
FALcmsgno=0;
for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i==2*FALCrank|| i==2*FALCrank+1){
    int temp1=0;
    if(i==2*FALCrank+1){
    cudaMemcpy( &temp1,&(FALCgpusendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);
    cudaMemcpy( &temp,&(FALCgpusendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
    MPI_Isend((temp.vid), temp1, MPI_INT, i/2 ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);//msgno send GPU value same node.
    FALCmsgno++;
}
else{//send from CPU to GPU of same node.rank==0, i==0. rank==1,i==2
     MPI_Isend(FALCsendbuff[i].vid, FALCsendsize[i], MPI_INT, i/2 ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    FALCmsgno++;
}
}
    if(i<FALCrank){
MPI_Isend(FALCsendbuff[i].vid, FALCsendsize[i], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i+1]);
    FALCmsgno++;
        cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.vid), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);
    FALCmsgno++;
    }
    if(i>FALCrank){
MPI_Isend(FALCsendbuff[i-1].vid, FALCsendsize[i-1], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    FaLCmsgno++;
        cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.vid), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);
    FALCmsgno++;
    }
}
int cnt=0,gpucnt=0;
for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i==2*FALCrank|| i==2*FALCrank+1){
    int temp1=0;
    if(i==2*FALCrank+1){
    for(int kk=0;kk<FALCsize/2;kk++){
    if(kk<FALCrank){
    int kk1=4*(FALCrank-kk);//4
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.vid,hgraph.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.vid,hgraph.npoints, MPI_INT,kk,kk1+1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
    }
    if(i>FALCrank){
    int kk1=2*(kk-FALCrank);//2
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.vid,hgraph.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.vid,hgraph.npoints, MPI_INT,kk,kk1+1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
    }
    if(kk==FALCrank){
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.vid,hgraph.npoints, MPI_INT,kk, 4*FALCrank, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
    }}
}
else {
    for(int kk=0;kk<FALCsize/2;kk++){
    if(kk<FALCrank){
    int kk1=2*(FALCrank-kk);
    MPI_Recv(FALCrecvbuff[cnt].vid,hgraph.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
    MPI_Recv(FALCrecvbuff[cnt].vid,hgraph.npoints, MPI_INT,kk, kk1+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
}
if(kk >FALCrank){
    int kk1=2*(kk-FALCrank)-2;
    MPI_Recv(FALCrecvbuff[cnt].vid,hgraph.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
    MPI_Recv(FALCrecvbuff[cnt].vid,hgraph.npoints, MPI_INT,kk, kk1+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
}
if(kk==FALCrank){
    MPI_Recv(FALCrecvbuff[cnt].vid,hgraph.npoints, MPI_INT,kk, 4*FALCrank+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
}
}
}
}
}
FALCmsgno=0;
for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i==2*FALCrank|| i==2*FALCrank+1){
    int temp1=0;
    if(i==2*FALCrank+1){
    cudaMemcpy( &temp1,&(FALCgpusendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);
    cudaMemcpy( &temp,&(FALCgpusendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
    MPI_Isend((temp.dist1), temp1, MPI_INT, i/2 ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);//msgno send GPU value same node.
    FALCmsgno++;
}
else{//send from CPU to GPU of same node.rank==0, i==0. rank==1,i==2
     MPI_Isend(FALCsendbuff[i].dist1, FALCsendsize[i], MPI_INT, i/2 ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    FALCmsgno++;
}
}
    if(i<FALCrank){
MPI_Isend(FALCsendbuff[i].dist1, FALCsendsize[i], MPI_INT, i ,FALCmsgn, MPI_COMM_WORLD,&FALCrequest[i+1]);
    FALCmsgno++;
        cudaMemcpy( &temp,&(FALCsendbuff[i]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.dist1), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);
    FALCmsgno++;
    }
    if(i>FALCrank){
MPI_Isend(FALCsendbuff[i-1].dist1, FALCsendsize[i-1], MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    FaLCmsgno++;
        cudaMemcpy( &temp,&(FALCsendbuff[i-1]),sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        int temp1;
        cudaMemcpy( &temp1,&(FALCsendsize[i-1]),sizeof(int),cudaMemcpyDeviceToHost);
        MPI_Isend((temp.dist1), temp1, MPI_INT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCgpurequest[i]);

    FALCmsgno++;
    }
}
 cnt=0,gpucnt=0;
for(int i=0;i<FALCsize;i++){
    struct FALCbuffer temp;
    if(i==2*FALCrank|| i==2*FALCrank+1){
    int temp1=0;
    if(i==2*FALCrank+1){
    for(int kk=0;kk<FALCsize/2;kk++){
    if(kk<FALCrank){
    int kk1=4*(FALCrank-kk);//4
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.dist1,hgraph.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.dist1,hgraph.npoints, MPI_INT,kk,kk1+1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
    }
    if(i>FALCrank){
    int kk1=2*(kk-FALCrank);//2
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.dist1,hgraph.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.dist1,hgraph.npoints, MPI_INT,kk,kk1+1, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
    }
    if(kk==FALCrank){
        cudaMemcpy( &temp,&FALCgpurecvbuff[gpucnt],sizeof(struct FALCbuffer),cudaMemcpyDeviceToHost);
        MPI_Recv(temp.dist1,hgraph.npoints, MPI_INT,kk, 4*FALCrank, MPI_COMM_WORLD,&FALCgpustatus[gpucnt]);
    gpucnt++;
    }}
}
else {
    for(int kk=0;kk<FALCsize/2;kk++){
    if(kk<FALCrank){
    int kk1=2*(FALCrank-kk);
    MPI_Recv(FALCrecvbuff[cnt].dist1,hgraph.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
    MPI_Recv(FALCrecvbuff[cnt].dist1,hgraph.npoints, MPI_INT,kk, kk1+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
}
if(kk >FALCrank){
    int kk1=2*(kk-FALCrank)-2;
    MPI_Recv(FALCrecvbuff[cnt].dist1,hgraph.npoints, MPI_INT,kk, kk1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
    MPI_Recv(FALCrecvbuff[cnt].dist1,hgraph.npoints, MPI_INT,kk, kk1+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
}
if(kk==FALCrank){
    MPI_Recv(FALCrecvbuff[cnt].dist1,hgraph.npoints, MPI_INT,kk, 4*FALCrank+1, MPI_COMM_WORLD,&FALCstatus[cnt]);//msgno recv GPU Value
    cnt++;
}
}
}
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


 
 //GPU IF STMT 
//GPU_GPU_changed
int   falcvt2;
if(cudaMemcpyFromSymbol(&(falcvt2),GPU_changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 1");
if(falcvt2==0&& changed==0)break;

 /*SINGLE FLAG reset1 0 */
cudaSetDevice(0);
 
reset1<<<hgraph.gpupart.localpoints/TPB0+1,TPB0>>>(hgraph,0);
/*SINGLE FLAG reset1 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints;i++)reset1(i,hgraph);

}
}


 /*SINGLE FLAG relaxgraph1 0 */
cudaSetDevice(0);
 
relaxgraph1<<<hgraph.gpupart.localpoints/TPB0+1,TPB0>>>(hgraph,0);
/*SINGLE FLAG relaxgraph1 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints;i++)relaxgraph1(i,hgraph);

}
}


 /*TE=2*/level++; 

 }//end

 for (int   i =0;i<hgraph.npoints;i++) {

 printf("%d\n",/*xx*//*XX3*/((struct struct_hgraph  *)(hgraph.hostparts[0].extra))->dist[i]);//rhs not null


 }//endfor

 return ;

 }//end fun 0 
int   main ( int   argc ,char    *  argv [ ] ) 
 {FALCmpiinit(argc,argv);
sprintf(partitionfile,"%s",argv[2]);


 /*TE=1*///GPU ASS 
//val=1 
graph.index[t+1].ipe - graph.index[t].ipelevel=atoi(argv[2]); 


 SSSP(argv[1]);//rhs not null


 MPI_Finalize();
}//end fun 0 
