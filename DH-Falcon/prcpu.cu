
 #include "prcpu.h"
// checklibfub ADD 
// LIBFUN ADD 
//libfunction call ADD
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
 int   changed =0, hchanged =0;


  int   fchanged =0, fhchanged =0;


 void   relaxgraph ( int & p , /**u1**/HGraph & graph ) 
 {

  int   outs =graph.index[p+1].ipe - graph.index[p].ipe;


  float   sum =((struct struct_hgraph  *)(graph.extra))->PR[p]/outs;


 int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
HADD(&(((struct struct_hgraph  *)(graph.extra))->val/*xx*/[ut1]),sum,/*xx*/changed);//rhs not null


 }//foreach

 }//end fun 0 
void   reset ( int & t , /**u1**/HGraph & graph ) 
 {

 ((struct struct_hgraph  *)(graph.extra))->PR[t]=1/graph.npoints; 

 ((struct struct_hgraph  *)(graph.extra))->val[t]=0.000000; 

 }//end fun 0 
void   reset1 ( int & t , /**u1**/HGraph & graph ,float   d ) 
 {

  float   sum =d*((struct struct_hgraph  *)(graph.extra))->val[t]+(1-d)/graph.npoints;


 ((struct struct_hgraph  *)(graph.extra))->PR[t]=sum; 

 ((struct struct_hgraph  *)(graph.extra))->val[t]=0.000000; 

 }//end fun 0 
void   PageRank ( char    *  name ) 
 {

 /*s1 0 0*/HGraph  hgraph ;

  int   cnt =0;


 

 

 hgraph.readPointsN(partitionfile,FALCsize);
hgraph.makeNPartitionsMPI(name,FALCrank,FALCsize);
int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
 FALCallocbuff(FALCsendbuff,FALCsize,hgraph.remotepoints);
 FALCallocbuff(FALCrecvbuff,FALCsize,hgraph.npoints);
; 

  float   d =0.850000;


 /*SINGLE FLAG reset 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints+hgraph.remotepoints;i++)reset(i,hgraph);


 
 while(cnt<100)  { 

 changed=0; 

 #pragma omp parallel for num_threads(32)
for(int i=0;i<hgraph.localpoints+hgraph.remotepoints;i++){
    tempval[i]= (( struct struct_hgraph  *)(hgraph.extra))->val[i];
}
/*SINGLE FLAG relaxgraph 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints;i++)relaxgraph(i,hgraph);
#pragma omp parallel for num_threads(32)
for(int i=0;i<hgraph.localpoints+hgraph.remotepoints;i++){
    tempval[i]= (( struct struct_hgraph  *)(hgraph.extra))->val[i];
}
for(int kk=1;kk<FALCsize;kk++){
    #pragma omp parallel for   num_threads(32)
    for(int i=hgraph.offset[kk-1];i<hgraph.offset[kk];i++){
        sendbuff(i,hgraph,FALCsendsize,FALCsendbuff,kk-1);
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
        MPI_Recv(FALCrecvbuff[i].vid,hgraph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    if(i>FALCrank)
    MPI_Recv(FALCrecvbuff[i-1].vid,hgraph.npoints, MPI_INT,i, FALCmsgno, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}
//EXTRA DATATYPE val
//1 7 
// SEND_DATATYPE=1

FALCmsgno++;
for(int i=0;i<FALCsize;i++){
    if(i<FALCrank)
        MPI_Isend((FALCsendbuff[i].val), FALCsendsize[i], MPI_FLOAT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i]);
    if(i>FALCrank)
        MPI_Isend((FALCsendbuff[i-1].val), FALCsendsize[i-1], MPI_FLOAT, i ,FALCmsgno, MPI_COMM_WORLD,&FALCrequest[i-1]);
}
for(int i=0;i<FALCsize;i++){
    if(i<FALCrank)
        MPI_Recv(FALCrecvbuff[i].val,hgraph.npoints, MPI_FLOAT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i]);
    if(i>FALCrank)
        MPI_Recv(FALCrecvbuff[i-1].val,hgraph.npoints, MPI_FLOAT,i, FALCmsgno, MPI_COMM_WORLD,&FALCstatus[i-1]);
}

for(int kk=0;kk<(FALCsize-1);kk++){
    MPI_Get_count(&FALCstatus[kk], MPI_INT, &FALCnamount);
    #pragma omp parallel for   num_threads(32)
    for(int i=0;i<FALCnamount;i++){ 
        int vertex= FALCrecvbuff[kk].vid[i];
        if( ( ( struct struct_hgraph  * )(hgraph.extra))->val[vertex] > FALCrecvbuff[kk].val[i])
            ( ( struct struct_hgraph  * )(hgraph.extra))->val[vertex] = FALCrecvbuff[kk].val[i];
    }
}
//here only master node of a point has updated value, sync it over all nodes needed. future work
for(int i=0;i<FALCsize;i++)FALCsendsize[i]=0;

 if( cnt<100 )break;

 /*SINGLE FLAG reset1 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints;i++)reset1(i,hgraph,d);


 cnt++; 

 }//end

 for (int   i =0;i<hgraph.npoints;i++) {

 printf("%f\n",/*xx*//***/((struct struct_hgraph  *)(hgraph.extra))->PR[i]);//rhs not null


 }//endfor

 return ;

 }//end fun 0 
int   main ( int   argc ,char    *  argv [ ] ) 
 {FALCmpiinit(argc,argv);
sprintf(partitionfile,"%s",argv[2]);


 PageRank(argv[1]);//rhs not null


 MPI_Finalize();
}//end fun 0 
