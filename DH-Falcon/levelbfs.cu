
 #include "levelbfs.h"
// cluster comm changed 
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
 int   changed ;


  int   lev =0;


 void   reset ( int & t , /**u1**/HGraph & graph ) 
 {

 ((struct struct_hgraph  *)(graph.extra))->dist[t]=1234567890; 

 }//end fun 0 
void   BFS ( int & p , /**u1**/HGraph & graph ,int   lev ) 
 {
 if( ((struct struct_hgraph  *)(graph.extra))->dist[p]==lev ){

  int   ch ;


 int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
/*XX*/for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
if( ((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut1]>(((struct struct_hgraph  *)(graph.extra))->dist[p]+1) ){

 ((struct struct_hgraph  *)(graph.extra))->dist/*xx*/[ut1]=((struct struct_hgraph  *)(graph.extra))->dist[p]+1; 

 changed=1; 

 }//end

 }//foreach

 }//end fun 0 

}
int   main ( int   argc ,char    *  argv [ ] ) 
 {FALCmpiinit(argc,argv);
sprintf(partitionfile,"%s",argv[2]);


 /*s1 0 0*/HGraph  hgraph ;

 

 hgraph.readPointsN(partitionfile,FALCsize);
hgraph.makeNPartitionsMPI(argv[1],FALCrank,FALCsize);
int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
 FALCallocbuff(FALCsendbuff,FALCsize,hgraph.remotepoints);
 FALCallocbuff(FALCrecvbuff,FALCsize,hgraph.npoints);
; 

 /*SINGLE FLAG reset 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints+hgraph.remotepoints;i++)reset(i,hgraph);


 if(FALCrank==0){
/***/((struct struct_hgraph  *)(hgraph.extra))->dist[0]=0;
}


 
 while(1)  { 

 changed=0; 

 /*SINGLE FLAG BFS 0 */
#pragma omp parallel for   num_threads(12)
for(int i=0;i<hgraph.localpoints;i++)BFS(i,hgraph,lev);
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
    }
}
//here only master node of a point has updated value, sync it over all nodes needed. future work
for(int i=0;i<FALCsize;i++)FALCsendsize[i]=0;

 if( changed==0 )break;

 lev++; 

 }//end

 for (int   i =0;i<hgraph.npoints;i++)printf("%d\n",/*xx*//***/((struct struct_hgraph  *)(hgraph.extra))->dist[i]);

 return 1;

 MPI_Finalize();
}//end fun 0 
