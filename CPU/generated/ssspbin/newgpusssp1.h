
#include "newgpusssp1global.h"
 #include "../include/HGraph.h"
  #include "../include/HSetOPT.h"
 #include<sys/time.h>
#include<unistd.h>
struct struct_graph { 
    int   *olddist ;//has to given size of property type
    bool  *updated ;//has to given size of property type
    int   *dist ;//has to given size of property type
};
void alloc_extra_graph(HGraph &graph,int flag,int npoints)/*symtab37*/ {
     if(flag==0)graph.extra=(struct struct_graph  *)malloc(sizeof(struct struct_graph )) ;
    ((struct struct_graph  *)graph.extra)->olddist=(int  *)malloc(sizeof(int ) * npoints) ;
    ((struct struct_graph  *)graph.extra)->updated=(bool *)malloc(sizeof(bool) * npoints) ;
    ((struct struct_graph  *)graph.extra)->dist=(int  *)malloc(sizeof(int ) * npoints) ;
}
void read_and_allocate_graph(HGraph  &graph ){
printf("enter number of points and edges");
 scanf("%d%d",&(graph.npoints),&(graph.nedges));
 graph.points=(union float_int *)malloc(sizeof(union float_int)*graph.npoints);
graph.edges=(union float_int *)malloc(sizeof(union float_int)*graph.nedges*2);
alloc_extra_graph(graph,0,graph.npoints);
}
//resetgraph graph
