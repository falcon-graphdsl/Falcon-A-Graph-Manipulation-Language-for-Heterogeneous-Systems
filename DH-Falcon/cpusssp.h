
#include "cpussspglobal.h"
#include<mpi.h>
#include "HGraph.h"
 #include<sys/time.h>
#include<unistd.h>
struct struct_hgraph { 
int   *olddist ;//has to given size of property type
bool  *updated ;//has to given size of property type
int   *dist ;//has to given size of property type
};
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints) {
 if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
((struct struct_hgraph  *)hgraph.extra)->olddist=(int  *)malloc(sizeof(int ) * npoints) ;
((struct struct_hgraph  *)hgraph.extra)->updated=(bool *)malloc(sizeof(bool) * npoints) ;
((struct struct_hgraph  *)hgraph.extra)->dist=(int  *)malloc(sizeof(int ) * npoints) ;
}
void read_and_allocate_hgraph(HGraph  &hgraph ){
printf("enter number of points and edges");
 scanf("%d%d",&(hgraph.npoints),&(hgraph.nedges));
 hgraph.points=(union float_int *)malloc(sizeof(union float_int)*hgraph.npoints);
hgraph.edges=(union float_int *)malloc(sizeof(union float_int)*hgraph.nedges*3);
alloc_extra_hgraph(hgraph,0,hgraph.npoints);
}
void hgraph_realloc(HGraph &hgraph){
 struct struct_hgraph  temp;
temp=*((struct struct_hgraph  *)(hgraph.extra));
int temppnpoints;
temppnpoints=hgraph.pnpoints;
int temppnedges;
temppnedges=hgraph.pnpoints;
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.olddist;
temp.olddist=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.olddist,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.updated;
temp.updated=(bool *)malloc(sizeof(bool)*hgraph.npoints*2);
memcpy(temp.updated,t1,sizeof(bool)*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.dist;
temp.dist=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.dist,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}

}
//communication buffer datatype
 struct FALCbuffer{
      int *vid;
     int  *dist;
};
int  *tempdist;
int FALCrank;//rank of process
char partitionfile[100];//second input
 char FALChostname[256];//name of host on which process running
int FALCsize;//total processes launched
 MPI_Status *FALCstatus;//used for MPI_Recv
MPI_Request *FALCrequest;//Used for MPI_Isend
int *FALCsendsize;//send buffer size(for sending to remote machines)
 int *FALCrecvsize;
int FALCmsgno;//message number for messages used in code
 int FALCnamount;
struct FALCbuffer *FALCsendbuff,*FALCrecvbuff;//send and receive buffer for synchronizing global state
//allocate buffer for communication 
void FALCallocbuff(struct FALCbuffer *buff,int tot,int size){
    for(int i=0;i<tot;i++){
        buff[i].vid=(int *)malloc(sizeof(int)*size);
        buff[i].dist=(int  *)malloc(sizeof(int )*size);
    }
    tempdist=(int  *)malloc(sizeof(int )*size);
}
void sendbuff(int i,HGraph &hgraph,int *sendsize,struct FALCbuffer *sendbuff,int kk){
    int loc=0;
    int flag=0;
    if( (( struct struct_hgraph  *)(hgraph.extra))->dist[i]!=tempdist[i])flag=1;
    if(flag==1){
        loc=__sync_fetch_and_add(&sendsize[kk],1);
        sendbuff[kk].vid[loc]=hgraph.remotevertexid[i];
        sendbuff[kk].dist[loc]=(( struct struct_hgraph  *)(hgraph.extra))->dist[i];
         tempdist[i]=(( struct struct_hgraph  *)(hgraph.extra))->dist[i];
    }
}
