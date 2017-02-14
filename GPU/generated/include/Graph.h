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
union float_int *edges,*points,*inedges;
int *inindex,*index,*outtotal,*total;
short pdims,edims;
int point_id,edge_id;
union float_int  *minedge,*maxedge;
void *extra;//void * can be typecasted to any structure
Graph(){
pdims=1;
edims=3;
npoints=nedges=0;
edges=points=NULL;
//innbrs=outnbrs=nbrs=NULL;
minedge=maxedge=NULL;
inindex=index=total=NULL;
extra=NULL;
outtotal=NULL;
total=NULL;
}
};
#endif 
