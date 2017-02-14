#include "HGraph.h"
main(int argc,char *argv[]){
HGraph hgraph;
hgraph.readEdges(argv[1]);
hgraph.read2(argv[2]);
FILE *fp=fopen(argv[3],"wb");
long int size=0;
size=sizeof(uint64_t)*4;
size=size+sizeof(uint64_t)*hgraph.npoints;
size=size+sizeof(uint32_t)*hgraph.nedges;
if(hgraph.nedges%2)size=size+sizeof(uint32_t);
size=size+sizeof(uint32_t)*hgraph.nedges;
if(hgraph.nedges%2)size=size+sizeof(uint32_t);
uint64_t version=1;
fwrite(&version,sizeof(uint64_t),1,fp); 
uint64_t sizeedgetype=4;
fwrite(&sizeedgetype,sizeof(uint64_t),1,fp); 
uint64_t numnodes=hgraph.npoints;
uint64_t numedges=hgraph.nedges-1;

fwrite(&numnodes,sizeof(uint64_t),1,fp); 
fwrite(&numedges,sizeof(uint64_t),1,fp); 
uint64_t *index;
index=malloc(sizeof(uint64_t)*(numnodes+1));
for(int i=0;i<=numnodes;i++)index[i]=hgraph.index[i]-1;
for(int i=0;i<10;i++)printf(" index=%d %d\n",index[i],numnodes);
for(int i=1;i<=numnodes;i++){
fwrite(&(index[i]),sizeof(uint64_t),1,fp);
}
unsigned zero=0;
unsigned *edgedst=malloc(sizeof(int)*(numedges+1));
for(int i=0;i<=numedges;i++)edgedst[i]=hgraph.edges[2*i].ipe;
for(int i=0;i<10;i++)printf(" edgedst=%d\n",edgedst[i]);
unsigned *edgedata=malloc(sizeof(int)*(numedges+1));
for(int i=0;i<=numedges;i++)edgedata[i]=hgraph.edges[2*i+1].ipe;
for(int i=0;i<10;i++)printf(" edgeweight=%d\n",edgedata[i]);
for(int i=1;i<=numedges;i++){
fwrite(&(edgedst[i]),sizeof(unsigned),1,fp);
}
for(int i=1;i<=numedges;i++){
fwrite(&(edgedata[i]),sizeof(unsigned),1,fp);
}
fclose(fp);
printf("SIZE=%ld",size);
}
