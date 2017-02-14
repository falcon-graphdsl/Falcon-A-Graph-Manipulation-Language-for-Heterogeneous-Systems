#ifndef GRAPH_H
#define GRAPH_H
#include<stdlib.h>
union float_int {
unsigned int ipe;
float fpe;
};
class Graph {
public:
int npoints,nedges;
union float_int *edges,*points,*inedges;
int *index,*inindex;
short pdims,edims;
int point_id,edge_id;
union float_int  *minedge,*maxedge;
void *extra;//void * can be typecasted to any structure
Graph(){
//npoints=( int *)malloc(sizeof(int));
//nedges=(int *)malloc(sizeof(int));
//pdims=(short *)malloc(sizeof(short));
//edims=(short *)malloc(sizeof(short));
pdims=1;
edims=3;
npoints=nedges=0;
edges=points=NULL;
minedge=maxedge=NULL;
index=NULL;
extra=NULL;
}
};
#endif 
