
#include "singledmr1global.h"
#include "../include/HGraph.h"
#include "../include/HSet.h"
struct struct_hgraph { 
int ntriangle;
int maxntriangle;
 int pntriangle;
    struct node  *triangle ;//has to given size of property type
    int *owner;
};
void read_hgraph_pptysize(HGraph &hgraph){
printf("enter size of propery triangle of hgraph ");
scanf("%d",&(((struct struct_hgraph  *)(hgraph.extra))->ntriangle));
}
void alloc_extra_hgraph(HGraph &hgraph,int flag,int npoints) {
     if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
    ((struct struct_hgraph  *)hgraph.extra)->triangle=(struct node *)malloc(sizeof(struct node) * ((struct struct_hgraph  *)(hgraph.extra))->ntriangle*3) ;
}
void read_and_allocate_hgraph(HGraph  &hgraph ){
printf("enter number of points and edges");
 scanf("%d%d",&(hgraph.npoints),&(hgraph.nedges));
 hgraph.points=(union float_int *)malloc(sizeof(union float_int)*hgraph.npoints);
hgraph.edges=(union float_int *)malloc(sizeof(union float_int)*hgraph.nedges*2);
read_hgraph_pptysize(hgraph);
 alloc_extra_hgraph(hgraph,0,hgraph.npoints);
}
void hgraph_realloc(HGraph &hgraph){
 struct struct_hgraph  temp;
temp=*((struct struct_hgraph  *)(hgraph.extra));
int temppnpoints;
temppnpoints=hgraph.pnpoints;
int temppnedges;
temppnedges=hgraph.pnpoints;
if(temp.ntriangle*2>temp.maxntriangle){
void *t1=temp.triangle;
temp.triangle=(struct node *)malloc(sizeof(struct node)*temp.ntriangle*2);
memcpy(temp.triangle,t1,sizeof(struct node)*temp.ntriangle);
free(t1);temp.maxntriangle=temp.ntriangle*2;
}

}
//resetgraph hgraph
void alloc_extra_graph(HGraph &graph,int npoints) {
     graph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
    ((struct struct_hgraph  *)graph.extra)->triangle=(struct node *)malloc(sizeof(struct node) * ((struct struct_hgraph  *)(graph.extra))->ntriangle*3) ;
    cudaSetDevice(0);
}
