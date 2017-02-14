
#include "falcondmrcpuglobal.h"
 #include "../include/HGraph.h"
  #include "../include/HSetOPT.h"
 #include<sys/time.h>
#include<unistd.h>
struct struct_graph { 
int ntriangle;
int maxntriangle;
 int pntriangle;
    struct node  *triangle ;//has to given size of property type
    int *owner;
};
void read_graph_pptysize(HGraph &graph){
printf("enter size of propery triangle of graph ");
scanf("%d",&(((struct struct_graph  *)(graph.extra))->ntriangle));
}
void alloc_extra_graph(HGraph &graph,int flag,int npoints)/*symtab37*/ {
     if(flag==0)graph.extra=(struct struct_graph  *)malloc(sizeof(struct struct_graph )) ;
    ((struct struct_graph  *)graph.extra)->triangle=(struct node *)malloc(sizeof(struct node) * ((struct struct_graph  *)(graph.extra))->ntriangle*8) ;
    ((struct struct_graph  *)graph.extra)->owner=(int *)malloc(sizeof(int) * ((struct struct_graph  *)(graph.extra))->ntriangle*8) ;
    ((struct struct_graph  *)graph.extra)->maxntriangle=( ( struct struct_graph  *) (graph.extra))->ntriangle*8;
}
void read_and_allocate_graph(HGraph  &graph ){
printf("enter number of points and edges");
 scanf("%d%d",&(graph.npoints),&(graph.nedges));
 graph.points=(union float_int *)malloc(sizeof(union float_int)*graph.npoints);
graph.edges=(union float_int *)malloc(sizeof(union float_int)*graph.nedges*2);
read_graph_pptysize(graph);
 alloc_extra_graph(graph,0,graph.npoints);
}
void graph_realloc(HGraph &graph){
 struct struct_graph  temp;
temp=*((struct struct_graph  *)(graph.extra));
int temppnpoints;
int temppnedges;
if(temp.ntriangle*2>temp.maxntriangle){
((struct struct_graph  *)(graph.extra))->maxntriangle=((struct struct_graph  *)(graph.extra))->maxntriangle*2;
((struct struct_graph  *)(graph.extra))->owner=(int *)realloc(((struct struct_graph  *)(graph.extra))->owner,sizeof(int)*((struct struct_graph  *)(graph.extra))->maxntriangle);
((struct struct_graph  *)(graph.extra))->triangle=(struct node *)realloc(((struct struct_graph  *)(graph.extra))->triangle,sizeof(struct node)*((struct struct_graph  *)(graph.extra))->maxntriangle);
}

}
//resetgraph graph
 int devngraphtriangle;
 void resetgraphtriangle(int id,HGraph &graph){
( ( struct struct_graph  *)(graph.extra))->owner[id]=1234567890;
 }
