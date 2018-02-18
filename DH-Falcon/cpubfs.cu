
 #include "cpubfs.h"
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


 

 void   reset ( int & t , /**u1**/HGraph & graph ) 
 {

 ((struct struct_hgraph  *)(graph.extra))->dist[t]=1234567890; 

 }//end fun 0 
void   BFS ( int & p , /**u1**/HGraph & graph ,int   lev ) 
 {
 if( ((struct struct_hgraph  *)(graph.extra))->dist[p]==lev ){

 

 int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
if( ((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut1]>(((struct struct_hgraph  *)(graph.extra))->dist[p]+1) ){

 ((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut1]=((struct struct_hgraph  *)(graph.extra))->dist[p]+1; 

 /*TE=1*///GPU ASS 
int   falcvt1;
falcvt1=1;
if(cudaMemcpyToSymbol(GPU_changed,&(falcvt1),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 0");//val=1 
changed=1; 


 }//end

 }//foreach

 }//end fun 0 

}
int   main ( int   argc ,char    *  name [ ] ) 
 {FALCmpiinit(argc,argv);
sprintf(partitionfile,"%s",argv[2]);


 /*s1 0 0*/HGraph  hgraph ;

 

 hgraph.readPointsN(partitionfile,FALCsize);
hgraph.makeNPartitionsMPI(name[1],2*FALCrank,FALCsize);
hgraph.gpupart.readPointsN(partitionfile,FALCsize);
hgraph.gpupart.makeNPartitionsMPI(name[1],2*FALCrank+1,FALCsize);
 FALCallocbuff(FALCsendbuff,FALCsize,hgraph.remotepoints);
 FALCallocbuff(FALCrecvbuff,FALCsize,hgraph.npoints);
 FALCgpuallocbuff(FALCgpusendbuff,FALCsize,hgraph.gpupart.remotepoints);
 FALCgpuallocbuff(FALCgpurecvbuff,FALCsize,hgraph.gpupart.npoints);
int TPB0=1024;
alloc_extra_hgraph(hgraph.gpupart,0);
int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 cudaSetDevice(0);
 
reset<<<hgraph.gpupart.npoints/TPB0+1,TPB0>>>(hgraph,0);
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints+hgraph.remotepoints;i++)reset(i,hgraph);

}
}


 /*XX3*/((struct struct_hgraph  *)(hgraph.hostparts[0].extra))->dist[0]=0; 

 
 while(1)  { 

 /*TE=1*///GPU ASS 
int   falcvt2;
falcvt2=0;
if(cudaMemcpyToSymbol(GPU_changed,&(falcvt2),sizeof(int ),0,cudaMemcpyHostToDevice)!=cudaSuccess)printf("memcpyerror 1");//val=1 
changed=0; 


 cudaSetDevice(0);
 
BFS<<<hgraph.gpupart.localpoints/TPB0+1,TPB0>>>(hgraph,lev,0);
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints;i++)BFS(i,,lev);

}
}
//changed should be synchronized as it is a global var

FALCmsgno++;
if(FALCrank!=0){
for(int i=1;i< FALCsize;i++){
int temp;
cudaMemcpyFromSymbol(&temp,GPU_changed,sizeof(int),0,cudaMemcpyDeviceToHost);
temp+=changed;
MPI_Isend(&temp,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);
}
}
if(FALCrank==0){
    int tempchanged=0;
    int temp0;
    cudaMemcpyFromSymbol(&temp0,GPU_changed,sizeof(int),0,cudaMemcpyDeviceToHost);
temp0+=changed;
    for(int i=1;i<FALCsize;i++){
        MPI_Recv(&tempchanged,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        temp0+=tempchanged;
    }
    cudaMemcpyToSymbol(GPU_changed,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);
    changed=temp0;

FALCmsgno++;
    for(int i=1;i< FALCsize;i++)MPI_Isend(&temp0,1,MPI_INT,i,FALCmsgno,MPI_COMM_WORLD,&FALCrequest[i-1]);
}
else {
    
FALCmsgno++;
    int temp0;
    MPI_Recv(&temp0,1,MPI_INT,0,FALCmsgno,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    cudaMemcpyToSymbol(changed,&temp0,sizeof(int),0,cudaMemcpyHostToDevice);
    changed=temp0;
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
int   falcvt3;
if(cudaMemcpyFromSymbol(&(falcvt3),GPU_changed,sizeof(int ),0,cudaMemcpyDeviceToHost)!=cudaSuccess)printf("memcpyerror 2");
if(falcvt3==0&& changed==0)break;

 /*TE=2*/lev++; 

 }//end

 for (int   i =0;i<hgraph.npoints;i++)printf("%d\n",/*xx*//*XX3*/((struct struct_hgraph  *)(hgraph.hostparts[0].extra))->dist[i]);

 return ;

 MPI_Finalize();
}//end fun 0 
