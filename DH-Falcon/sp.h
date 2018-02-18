
#include "cpussspglobal.h"
 #include "HGraph.h"
 #include "GGraph.cu"
#include "thrust.cu"
 #include<sys/time.h>
#include </usr/local/cuda/include/cuda.h>
 #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>
struct struct_hgraph { 
int   *dist ;//has to given size of property type
};
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints) {
 if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
((struct struct_hgraph  *)hgraph.extra)->dist=(int  *)malloc(sizeof(int ) * npoints) ;
}
void read_and_allocate_hgraph(HGraph  &hgraph ){
printf("enter number of points and edges");
 scanf("%d%d",&(hgraph.npoints),&(hgraph.nedges));
 hgraph.points=(union float_int *)malloc(sizeof(union float_int)*hgraph.npoints);
hgraph.edges=(union float_int *)malloc(sizeof(union float_int)*hgraph.nedges*3);
alloc_extra_hgraph(hgraph,0);
}
void hgraph_realloc(HGraph &hgraph){
 struct struct_hgraph  temp;
temp=*((struct struct_hgraph  *)(hgraph.extra));
int temppnpoints;
temppnpoints=hgraph.pnpoints;
int temppnedges;
temppnedges=hgraph.pnpoints;
if(hgraph.pnpoints*2> hgraph.maxnpoints){
void *t1=temp.dist;
temp.dist=(int  *)malloc(sizeof(int )*hgraph.npoints*2);
memcpy(temp.dist,t1,sizeof(int )*hgraph.npoints);
free(t1);
hgraph.maxnpoints=hgraph.npoints*2;
}

}
struct FALCbuffer{
 int *vid;
int  *dist;
};
int  *tempdist;
 char FALChostname[256];
FALCallocbuff(struct FALCbuffer *buff,int tol,int npoints){
  for(int i=0;i=0;i<tol;i++){
buff[i].vid=(int *)malloc(sizeof(int)*npoints);
buff[i].dist=(int  *)malloc(sizeof(int )*npoints);
tempdist=(int  *)malloc(sizeof(int )*npoints);

}
}int FALCrank;
int FALCsize;
 MPI_Status *FALCstatus;
MPI_Request *FALCrequest;
int *FALCsendsize,*FALCrecvsize;
struct FALCbuffer *FALCsendbuff,*FALCrevbff;
