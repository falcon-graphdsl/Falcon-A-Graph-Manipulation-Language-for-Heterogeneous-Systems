#ifndef GRAPH_H
#define GRAPH_H
#include<stdlib.h>
union float_int {
int ipe;
float fpe;
};
struct neighbors {
int total,index;
};
class Graph {
public:
int npoints,nedges;
union float_int *edges,*points,*edges1,*edges2;
int *inedges;
short pdims,edims;
int point_id,edge_id;
struct neighbors *innbrs,*outnbrs,*nbrs;
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
innbrs=outnbrs=nbrs=NULL;
minedge=maxedge=NULL;
extra=NULL;
}
};
#endif 
