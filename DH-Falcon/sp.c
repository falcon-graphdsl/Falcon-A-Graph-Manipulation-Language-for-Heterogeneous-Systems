//else sttype 0 //else sttype 0 
 #include "cpusssp.h"
FALCmpiinit(){
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &FALCrank);
MPI_Comm_size(MPI_COMM_WORLD, &FALCsize);
  gethostname(hostname,255);
FALCsendbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);
FALCrecvdbuff=(struct FALCbuffer *)malloc(sizeof(struct FALCbuffer )*FALCsize);
FALCsendsize=(int *)malloc(sizeof(int)*FALCsize);
FALCrecvsize=(int *)malloc(sizeof(int)*FALCsize);
for(int i=0;i<FALCsize;i++)sendsize[i]=recvsize=0;
 FALCstatus=(MPI_Status *)malloc(sizeof(MPI_Status)*FALCsize);
 FALCrequest=(MPI_Request *)malloc(sizeof(MPI_Request)*FALCsize);
}
 int   changed =0, hchanged =0;


 /*X 0X*/// &&      [id] 
 //41 
// && /*J74*/((struct struct_hgraph  *)(graph.extra))->updated     [id] 
void   relaxgraph ( int & p ,HGraph & graph ) /*XX*/
 {
 if( /*J74*/((struct struct_hgraph  *)(graph.extra))->updated[p]==true ){

 ((struct struct_hgraph  *)(graph.extra))->updated[p]=false; 

 int falcft0=graph.inindex[id+1]-graph.inindex[id];
int falcft1=graph.index[id];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.inedges[ut0].ipe;
int ut2=graph.inedges[ut0+1].ipe;
int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
HMIN(&(((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut1]),((struct struct_hgraph  *)(graph.extra))->dist[p]+ut2,/**/ut1,/**/changed);//rhs not null


 }//foreach

 }//end fun 0 

}
/*X 0X*/void   reset ( int & t ,HGraph & graph ) /*XX*/
 {

 ((struct struct_hgraph  *)(graph.extra))->dist[t]=1234567890; 

 ((struct struct_hgraph  *)(graph.extra))->olddist[t]=1234567890; 

 ((struct struct_hgraph  *)(graph.extra))->updated[t]=false; 

 }//end fun 0 
/*X 0X*/void   reset1 ( int & t ,HGraph & graph ) /*XX*/
 {

 if( ((struct struct_hgraph  *)(graph.extra))->dist[t]<((struct struct_hgraph  *)(graph.extra))->olddist[t] )((struct struct_hgraph  *)(graph.extra))->updated[t]=true; 

 ((struct struct_hgraph  *)(graph.extra))->olddist[t]=((struct struct_hgraph  *)(graph.extra))->dist[t]; 

 }//end fun 0 
/*X 0X*/void   SSSP ( char    *  name ) /*XX*/
 {

 /*s1 0 0*/HGraph  hgraph ;

 

 hgraph.read1(name);
int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.npoints;i++)reset(i,hgraph);/xx

 /***/((struct struct_hgraph  *)(hgraph.extra))->updated[0]=true; 

 /***/((struct struct_hgraph  *)(hgraph.extra))->dist[0]=0; 

 
 while(1)  { 

 changed=0; 

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.npoints;i++)relaxgraph(i,hgraph);/xx
#pragma omp parallel for   num_threads(32)
for(int i=hgraph.hostparts[0].npoints;i<hgraph.offset[1];i++){
int loc;
int flag=0;
if( ((struct struct struct_hgraph  *)(hgraph.extra))->dist[i]!=((struct struct struct_hgraph  *)(hgraph.extra))->tempdist[i])flag=1;
if(flag==1){
loc=__sync_fetch_and_add(&sendsize[0],1);
sendbuff[0].vid[loc]=hgraphremotevertexid[i];
sendbuff[0].dist[i]=((struct struct struct_hgraph  *)(hgraph.extra))->dist[i];
 //((struct struct struct_hgraph  *)(hgraph.extra))->tempdist[i]=((struct struct struct_hgraph  *)(hgraph.extra))->dist[i];
}
}
for(int kk=2;kk<NPARTS;kk++){
 #pragma omp parallel for   num_threads(32)
for(int i=hgraph.offset[kk-1];i<hgraph.offset[kk];i++){
int loc;
int flag;
if( ((struct struct struct_hgraph  *)(hgraph.extra))->dist[i]!=((struct struct struct_hgraph  *)(hgraph.extra))->tempdist[i])flag=1;
if(flag==1){
loc=__sync_fetch_and_add(&sendsize[kk-1],1);
sendbuff[kk-1].vid[loc]=hgraphremotevertexid[i];
sendbuff[kk-1].dist[i]=((struct struct struct_hgraph  *)(hgraph.extra))->dist[i];
 //((struct struct struct_hgraph  *)(hgraph.extra)->tempdist[i]=((struct struct struct_hgraph  *)(hgraph.extra))->dist[i];
}
}

}for(int i=0;i<FALCrank;i++){
MPI_Isend((FALCsendbuff[i].vid), FALCsendsize[i], MPI_INT, i ,messageno, MPI_COMM_WORLD,&request[i]);
}
for(int i=FALCrank+1;i<NPARTS;i++){
MPI_Isend((FALCsendbuff[i-1].vid), FALCsendsize[i-1], MPI_INT, i ,messageno, MPI_COMM_WORLD,&request[i-1]);
}
for(int i=0;i<FALCrank;i++){
MPI_Recv(FALCrecvbuff[i].vid,1024*1024, MPI_INT,i, messageno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}
for(int i=FALCrank+1;i<NPARTS;i++){
MPI_Recv(FALCrecvbuff[i-1].vid,1024*1024, MPI_INT,i, messageno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}
for(int i=0;i<FALCrank;i++){
MPI_Isend((FALCsendbuff[i-1].dist), FALCsendsize[i-1], MPI_INT, i ,messageno, MPI_COMM_WORLD,&request[i-1]);
}
for(int i=FALCrank+1;i<NPARTS;i++){
MPI_Isend((FALCsendbuff[i-1].dist), FALCsendsize[i-1], MPI_INT, i ,messageno, MPI_COMM_WORLD,&request[i-1]);
}
for(int i=0;i<FALCrank;i++){
MPI_Recv(FALCrecvbuff[i].dist,1024*1024, MPI_INT,i, messageno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}
for(int i=FALCrank+1;i<NPARTS;i++){
MPI_Recv(FALCrecvbuff[i-1].dist,1024*1024, MPI_INT,i, messageno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}


 if( changed==0 )break;

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.npoints;i++)reset1(i,hgraph);/xx

 }//end

 for (int   i =0;i<hgraph.points;i++) {

 printf("%d\n",/**//***/((struct struct_hgraph  *)(hgraph.extra))->dist[i]);//rhs not null


 }//endfor

 return ;

 }//end fun 0 
/*X 0X*/int   main ( int   argc ,char    *  argv [ ] ) /*XX*/
 { 
alloc_sync_array();//0


 SSSP(argv[1]);//rhs not null


 }//end fun 0 
