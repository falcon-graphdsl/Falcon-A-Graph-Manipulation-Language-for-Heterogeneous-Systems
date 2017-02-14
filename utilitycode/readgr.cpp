#include "HGraph.h"
main(int argc,char *argv[]){
HGraph hgraph;
hgraph.read2(argv[1]);
for(int i=hgraph.nedges-10;i<hgraph.nedges;i++)printf("%d %d \n",hgraph.edges[2*i].ipe,hgraph.edges[2*i+1].ipe);
printf("index %d \n",hgraph.index[hgraph.npoints-1]);
printf("index %d \n",hgraph.index[hgraph.npoints]);
}
