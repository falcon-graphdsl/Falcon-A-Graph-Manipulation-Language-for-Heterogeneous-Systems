#ifndef HGRAPH_H
#define HGRAPH_H
#include<stdio.h>
#include<stdlib.h>
 #include <sys/types.h>
 #include <sys/time.h>
       #include <sys/stat.h>
       #include <unistd.h>
#include <fcntl.h>
#include<iostream>
#include<fstream>
 #include <sys/mman.h>
#include <inttypes.h>
#include </usr/local/cuda/include/cuda.h>
        #include </usr/local/cuda/include/cuda_runtime_api.h>
#include "GGraph.cu"
#include<thrust/host_vector.h>
#include<thrust/device_vector.h>
#define DISTANCETHRESHOLD	150
#define THRESHOLDDEGREE		10
#define INVALID               1234567890
struct Point {
 float x;
 float y;
 float z;
};
struct GColl{
int *size,*ptr;
};
int writelock[8388608];
class HGraph: public Graph{
public:
int maxnpoints,pnpoints,maxnedges,pnedges;
HGraph *hostpart;
GGraph *devicepart;
 int init();
int  readNodes(char *filename,int dims);
int  printNodes(char *filename,int dims);
int readEdges(char *filename);
int read(char *filename);
int read1(char *filename);
int read2(char *filename);
int readMorph(char *filename);
int optimizeone();
int optimizetwo();
int optimize();
int readMesh(char *filename,int dims);
int cloneGPU(GGraph &graph,int num);
int cloneMorphGPU(GGraph &graph,int num);
int copytoGPU(GGraph &graph);
int copyfromGPU(GGraph &graph);
int getPoint(int pos);
int deallocate();
int Check();
int acyclic(HGraph &graph);
int addEdge(int pos,int src,int dst,int weight);
int addEdge(int src,int dst,int weight);
int sortPoints();
void (*extra_alloc)(HGraph &);//needed for read,copy,allocate,deallocate extra  							                      // properties
};
int HGraph::init(){
}
int HGraph::getPoint(int pos){
if(pos<npoints)return points[pos].ipe;
return -1;
}
int HGraph::addEdge(int src,int dst,int weight){
int pos=__sync_fetch_and_add(&(nbrs[src].total),1);
pos=pos+nbrs[src].index;
if(pos<nedges*3){
//edges[3*pos].ipe=src;
//edges[3*pos+1].ipe=dst;
//edges[3*pos+2].ipe=weight;
edges[2*pos].ipe=dst;
edges[2*pos+1].ipe=weight;
return 1;
}
return -1;
}
int HGraph::addEdge(int pos,int src,int dst,int weight){
if(pos<nedges){
edges[3*pos].ipe=src;
edges[3*pos+1].ipe=dst;
edges[3*pos+2].ipe=weight;
return 1;
}
return -1;
}
int HGraph::readNodes(char *filename,int dims){
char temp[100];
char *fnode=(char *)malloc(sizeof(char)*100);
sprintf(fnode,"%s.node",filename);
FILE *fp=fopen(fnode,"r");
int index,npoints1;
float x,y;
pdims=dims;
fscanf(fp,"%d",&npoints1);
printf("HERE %d \n",npoints1);
points=(union float_int *)(malloc(sizeof(union float_int )*npoints1*3*pdims));
npoints=npoints1;
maxnpoints=npoints1*3;
fgets(temp,100,fp);
if(dims==2){
for(int i=0;i<npoints1;i++){
fscanf(fp,"%d%f%f",&index,&x,&y);
fgets(temp,100,fp);
points[2*index].fpe=x;
points[2*index+1].fpe=y;
}
}
printf("HERE %d \n",npoints1);
}
int HGraph::printNodes(char *filename,int dims){
FILE *fp=fopen(filename,"w+");
for(int i=0;i<npoints;i++){
fprintf(fp,"%d %f %f \n",i,points[2*i].fpe,points[2*i+1].fpe);
}
}

int HGraph::acyclic(HGraph &graph){
static int cnt;
int gindex,gtotal;
graph.npoints=npoints;
graph.maxnpoints=npoints;
graph.nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
  gindex=0; gtotal=0;

graph.points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      graph.edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims);
for(int i=0;i<(npoints);i++){
graph.points[i].ipe=i;
}
for(int i=0;i<npoints;i++){
int t1=nbrs[i].index;
int t2=nbrs[i].total;
for(int t3=0;t3<t2;t3++){
int p1=edges[3*(t3+t1)+1].ipe;
int tt1=nbrs[p1].index;
int tt2=nbrs[p1].total;
int flag=0;
for(int tt3=0;tt3<tt2;tt3++){
if((edges[3*(tt3+tt1)+1].ipe==i )&& (edges[3*(tt3+tt1)+2].ipe==edges[3*(t3+t1)+2].ipe)){
if(i>p1)continue;
else {
gtotal++;
graph.edges[3*gtotal].ipe=i;
graph.edges[3*gtotal+1].ipe=p1;
graph.edges[3*gtotal+2].ipe=edges[3*(t3+t1)+2].ipe;
gtotal++;
}
}
}
}
graph.nbrs[i].index=gindex;
if(i==0)graph.nbrs[i].total=gtotal;
else graph.nbrs[i].total=gtotal-graph.nbrs[i-1].index;
gindex=gtotal;
}
graph.nedges=gtotal;
graph.maxnedges=gtotal;
fprintf(stderr,"DONE %d \n",gtotal);
}

int HGraph::Check(){
static int cnt;
for(int i=0;i<npoints;i++){
int t1=nbrs[i].index;
int t2=nbrs[i].total;
for(int t3=0;t3<t2;t3++){
int p1=edges[3*(t3+t1)+1].ipe;//dst node of edge
int tt1=nbrs[p1].index; //index of dst
int tt2=nbrs[p1].total;//total of dst
int flag=0;
for(int tt3=0;tt3<tt2;tt3++){
//printf(" i=%d edge1=%d %d %d  edge2=%d %d %d\n",i,edges[3*(t3+t1)].ipe,edges[3*(t3+t1)+1].ipe,edges[3*(t3+t1)+2].ipe, edges[3*(tt3+tt1)].ipe,edges[3*(tt3+tt1)+1].ipe, edges[3*(tt3+tt1)+2].ipe);
if((edges[3*(tt3+tt1)+1].ipe==i )&& (edges[3*(tt3+tt1)+2].ipe==edges[3*(t3+t1)+2].ipe)){//reverse edge with same weight
if(p1>i){ edges[3*(tt3+tt1)+2].ipe=368856;
cnt++;
}
}
}
}
}
printf("************* CNT = %d *********************\n",cnt);
}


int HGraph::readEdges(char *filename){
FILE *fp=fopen(filename,"r");
fscanf(fp,"%d%d",&npoints,&nedges);
//for(int i=0;i<(npoints);i++){ points[i].ipe=i;}
int src,dst,weight;
nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
edims=2;
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims); 
int currsrc=0,index=0,currindex=0;
for(int i=0;i<(npoints);i++){ points[i].ipe=i;}
nbrs[0].index=0;
for(int i=0;i<nedges;i++){
fscanf(fp,"%d%d%d",&src,&dst,&weight);
if(src!=currsrc){
nbrs[currsrc].total=index-currindex;
if(src<10)printf(" %d %d index=%d total=%d \n",nbrs[currsrc].index,nbrs[currsrc].total,index,currindex);
if(src<10)printf(" src=%d dst=%d weight=%d\n",src,dst,weight);
for(int kk=currsrc+1;kk<src;kk++){ //if some points/nodes does not have any edges
nbrs[kk].index=index;
nbrs[kk].total=0;
}
currsrc=src;
nbrs[src].index=index;
currindex=index;
//edges[2*i].ipe=src;
edges[2*i].ipe=dst;
edges[2*i+1].ipe=weight;
index++;
}
else {
//edges[3*i].ipe=src;
//edges[3*i+1].ipe=dst;
//edges[3*i+2].ipe=weight;
edges[2*i].ipe=dst;
edges[2*i+1].ipe=weight;
index++;
}
}
nbrs[currsrc].total=index-currindex;
fclose(fp);
printf("READ OVER");
}

int HGraph::read1(char *file){
	std::ifstream cfile;
	cfile.open(file);

	int masterFD = open(file, O_RDONLY);
  	if (masterFD == -1) {
	printf("FileGraph::structureFromFile: unable to open %s.\n", file);
	return 1;
  	}

  	struct stat buf;
	int f = fstat(masterFD, &buf);
  	if (f == -1) {
    		printf("FileGraph::structureFromFile: unable to stat %s.\n", file);
    		abort();
  	}
  	size_t masterLength = buf.st_size;

  	int _MAP_BASE = MAP_PRIVATE;

  	void* m = mmap(0, masterLength, PROT_READ, _MAP_BASE, masterFD, 0);
  	if (m == MAP_FAILED) {
    		m = 0;
    		printf("FileGraph::structureFromFile: mmap failed.\n");
    		abort();
  	}
  	uint64_t* fptr = (uint64_t*)m;
  	__attribute__((unused)) uint64_t version = le64toh(*fptr++);
  	//assert(version == 1);
  	uint64_t sizeEdgeTy = le64toh(*fptr++);
printf("SIZEOF EDGE TYPE=%d ",sizeEdgeTy);
  	uint64_t numNodes = le64toh(*fptr++);
  	uint64_t numEdges = le64toh(*fptr++);
  	uint64_t *outIdx = fptr;
  	fptr += numNodes;
  	uint32_t *fptr32 = (uint32_t*)fptr;
  	uint32_t *outs = fptr32; 
  	fptr32 += numEdges;
  	if (numEdges % 2) fptr32 += 1;
  	unsigned  *edgeData = (unsigned *)fptr32;
	
	// cuda.
	npoints = numNodes;
	nedges = numEdges+1;
int count[npoints],incountar[npoints];
for(int i=0;i<npoints;i++){count[i]=incountar[i]=0;}

	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims);
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
 nbrs[i].index=le64toh(outIdx[i - 1]) + 1;
 nbrs[i].total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);

}
else {
  nbrs[i].index=1;
  nbrs[i].total=le64toh(outIdx[0]);
}
for (unsigned jj = 0; jj <nbrs[i].total; ++jj) {
unsigned edgeindex = nbrs[i].index + jj;
unsigned dst =le32toh(outs[edgeindex - 1]);
if(i<=5)printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
if(dst==1)printf("dst=%d src=%d \n",dst,i);
if (dst >=(npoints)) printf("\tinvalid edge\n");
edges[edgeindex*3].ipe=i;
edges[edgeindex*3+1].ipe=dst;
edges[edgeindex*3+2].ipe=edgeData[edgeindex - 1];
if(dst<npoints)(count[dst])++;
//if(edgeData[edgeindex - 1]==0)
//printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
}
}
innbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
unsigned long int incount=0;
for(int i=0;i<npoints;i++){innbrs[i].index=incount; innbrs[i].total=count[i];incount+=count[i];}
inedges=(int *)malloc(sizeof(union float_int)*(nedges+1));
for(int i=0;i<npoints;i++)count[i]=0;
for(int i=1;i<nedges;i++){
int p= edges[3*i+1].ipe;
if(p==1)printf("total innbrs %d src=%d dst=%d \n",innbrs[p].total,edges[3*i].ipe,p);
if((count[p]<innbrs[p].total)&& p<npoints){
inedges[innbrs[p].index+count[p]]=i;
(count[p])++;
}
else printf("IN NBRS IS MORE %d  count=%d total=%d p=%d",i,count[p],innbrs[p].total,p);
//if(p<=5)printf( "in Edge from %d  to %d with weight %d \n ",edges[3*i].ipe,edges[3*i+1].ipe,edges[3*i+2].ipe);
}
cfile.close();
return 1;
}

int HGraph::readMorph(char *file){
	std::ifstream cfile;
	cfile.open(file);

int zerocount=0;
	// copied from GaloisCpp/trunk/src/FileGraph.h
	int masterFD = open(file, O_RDONLY);
  	if (masterFD == -1) {
	printf("FileGraph::structureFromFile: unable to open %s.\n", file);
	return 1;
  	}

  	struct stat buf;
	int f = fstat(masterFD, &buf);
  	if (f == -1) {
    		printf("FileGraph::structureFromFile: unable to stat %s.\n", file);
    		abort();
  	}
  	size_t masterLength = buf.st_size;

  	int _MAP_BASE = MAP_PRIVATE;

  	void* m = mmap(0, masterLength, PROT_READ, _MAP_BASE, masterFD, 0);
  	if (m == MAP_FAILED) {
    		m = 0;
    		printf("FileGraph::structureFromFile: mmap failed.\n");
    		abort();
  	}
  	uint64_t* fptr = (uint64_t*)m;
  	__attribute__((unused)) uint64_t version = le64toh(*fptr++);
  	//assert(version == 1);
  	uint64_t sizeEdgeTy = le64toh(*fptr++);
printf("SIZEOF EDGE TYPE=%d ",sizeEdgeTy);
  	uint64_t numNodes = le64toh(*fptr++);
  	uint64_t numEdges = le64toh(*fptr++);
  	uint64_t *outIdx = fptr;
  	fptr += numNodes;
  	uint32_t *fptr32 = (uint32_t*)fptr;
  	uint32_t *outs = fptr32; 
  	fptr32 += numEdges;
  	if (numEdges % 2) fptr32 += 1;
  	unsigned  *edgeData = (unsigned *)fptr32;
	
	// cuda.
	npoints = numNodes;
	nedges = numEdges+1;
int *count,*indexmorph;
int max=0,min=99999;
edims=2;
pdims=1;
count=(int *)malloc(sizeof(int)*npoints);
indexmorph=(int *)malloc(sizeof(int)*npoints);
for(int i=0;i<npoints;i++)count[i]=0;
	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      //edges=(union float_int *)malloc(sizeof(union float_int)*(nedges+1)*edims*3);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges+1)*edims*2);
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
 nbrs[i].index=le64toh(outIdx[i - 1]) + 1;
// indexmorph[i]=le64toh(outIdx[i - 1])*3 + 1;
 indexmorph[i]=le64toh(outIdx[i - 1])*2 + 1;
 nbrs[i].total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);

}
else {
  nbrs[0].index=1;
 indexmorph[i]=1;
  nbrs[0].total=le64toh(outIdx[0]);
}
if(max<nbrs[i].total)max=nbrs[i].total;
if(min>nbrs[i].total)min=nbrs[i].total;
if(nbrs[i].total==0)zerocount++;
for (unsigned jj = 0; jj <nbrs[i].total; ++jj) {
unsigned edgeindex = nbrs[i].index + jj;
unsigned morphedgeindex =indexmorph[i] + jj;
unsigned dst =le32toh(outs[edgeindex - 1]);
//if(i<=5 || i==1048576 || i==1048577 || i==1048578 )printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
if(i<10 && jj==0){
printf("i=%d   index[i]=%d  index[i-1]=%d total[i]=%d morph[i]=%d morph[i-1]=%d\n",i,nbrs[i].index,nbrs[i-1].index,nbrs[i].total,indexmorph[i],indexmorph[i-1]);
if((indexmorph[i]-indexmorph[i-1])==(nbrs[i-1].total*3))printf(" 3 times total\n");
}
if (dst >=(npoints)) printf("\tinvalid edge\n");
//edges[morphedgeindex*2].ipe=i;
edges[morphedgeindex*2].ipe=dst;
edges[morphedgeindex*2+1].ipe=edgeData[edgeindex - 1];
//if(edgeData[edgeindex - 1]==0)
//printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
if(dst<npoints)(count[dst])++;
}
}
for(int i=0;i<npoints;i++)nbrs[i].index=indexmorph[i];
printf("ZERO COUNT=%d\n ",zerocount);
cfile.close();
printf("READ OVER");
printf("MAX=%d MIN=%d\n ",max,min);
return 1;
}





//__device__ __constant__ union float_int *edges1;
//__device__ __constant__ int *total1,*index1;
int HGraph::read(char *file){
	std::ifstream cfile;
	cfile.open(file);

int zerocount=0;
	// copied from GaloisCpp/trunk/src/FileGraph.h
	int masterFD = open(file, O_RDONLY);
  	if (masterFD == -1) {
	printf("FileGraph::structureFromFile: unable to open %s.\n", file);
	return 1;
  	}

  	struct stat buf;
	int f = fstat(masterFD, &buf);
  	if (f == -1) {
    		printf("FileGraph::structureFromFile: unable to stat %s.\n", file);
    		abort();
  	}
  	size_t masterLength = buf.st_size;

  	int _MAP_BASE = MAP_PRIVATE;

  	void* m = mmap(0, masterLength, PROT_READ, _MAP_BASE, masterFD, 0);
  	if (m == MAP_FAILED) {
    		m = 0;
    		printf("FileGraph::structureFromFile: mmap failed.\n");
    		abort();
  	}
  	uint64_t* fptr = (uint64_t*)m;
  	__attribute__((unused)) uint64_t version = le64toh(*fptr++);
  	//assert(version == 1);
  	uint64_t sizeEdgeTy = le64toh(*fptr++);
printf("SIZEOF EDGE TYPE=%d ",sizeEdgeTy);
  	uint64_t numNodes = le64toh(*fptr++);
  	uint64_t numEdges = le64toh(*fptr++);
  	uint64_t *outIdx = fptr;
  	fptr += numNodes;
  	uint32_t *fptr32 = (uint32_t*)fptr;
  	uint32_t *outs = fptr32; 
  	fptr32 += numEdges;
  	if (numEdges % 2) fptr32 += 1;
  	unsigned  *edgeData = (unsigned *)fptr32;
	
	// cuda.
	npoints = numNodes;
	nedges = numEdges+1;
int *count;
int max=0,min=99999;
count=(int *)malloc(sizeof(int)*npoints);
for(int i=0;i<npoints;i++)count[i]=0;
	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims);
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
 nbrs[i].index=le64toh(outIdx[i - 1]) + 1;
 nbrs[i].total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);

}
else {
  nbrs[0].index=1;
  nbrs[0].total=le64toh(outIdx[0]);
}
if(max<nbrs[i].total)max=nbrs[i].total;
if(min>nbrs[i].total)min=nbrs[i].total;
if(nbrs[i].total==0)zerocount++;
for (unsigned jj = 0; jj <nbrs[i].total; ++jj) {
unsigned edgeindex = nbrs[i].index + jj;
unsigned dst =le32toh(outs[edgeindex - 1]);
//if(i<=5 || i==1048576 || i==1048577 || i==1048578 )printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
if (dst >=(npoints)) printf("\tinvalid edge\n");
edges[edgeindex*3].ipe=i;
edges[edgeindex*3+1].ipe=dst;
edges[edgeindex*3+2].ipe=edgeData[edgeindex - 1];
//if(edgeData[edgeindex - 1]==0)
//printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
if(dst<npoints)(count[dst])++;
}
}
printf("ZERO COUNT=%d\n ",zerocount);
cfile.close();
printf("READ OVER");
printf("MAX=%d MIN=%d\n ",max,min);
return 1;
}
int HGraph::read2(char *file){
	std::ifstream cfile;
	cfile.open(file);

int zerocount=0;
	// copied from GaloisCpp/trunk/src/FileGraph.h
	int masterFD = open(file, O_RDONLY);
  	if (masterFD == -1) {
	printf("FileGraph::structureFromFile: unable to open %s.\n", file);
	return 1;
  	}

  	struct stat buf;
	int f = fstat(masterFD, &buf);
  	if (f == -1) {
    		printf("FileGraph::structureFromFile: unable to stat %s.\n", file);
    		abort();
  	}
  	size_t masterLength = buf.st_size;

  	int _MAP_BASE = MAP_PRIVATE;

  	void* m = mmap(0, masterLength, PROT_READ, _MAP_BASE, masterFD, 0);
  	if (m == MAP_FAILED) {
    		m = 0;
    		printf("FileGraph::structureFromFile: mmap failed.\n");
    		abort();
  	}
  	uint64_t* fptr = (uint64_t*)m;
  	__attribute__((unused)) uint64_t version = le64toh(*fptr++);
  	//assert(version == 1);
  	uint64_t sizeEdgeTy = le64toh(*fptr++);
printf("SIZEOF EDGE TYPE=%d ",sizeEdgeTy);
  	uint64_t numNodes = le64toh(*fptr++);
  	uint64_t numEdges = le64toh(*fptr++);
  	uint64_t *outIdx = fptr;
  	fptr += numNodes;
  	uint32_t *fptr32 = (uint32_t*)fptr;
  	uint32_t *outs = fptr32; 
  	fptr32 += numEdges;
  	if (numEdges % 2) fptr32 += 1;
  	unsigned  *edgeData = (unsigned *)fptr32;
	
	// cuda.
	npoints = numNodes;
	nedges = numEdges+1;
int *count;
int max=0,min=99999;
edims=2;
count=(int *)malloc(sizeof(int)*npoints);
for(int i=0;i<npoints;i++)count[i]=0;
	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*(npoints+1));
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims);
     // edges1=(union float_int *)malloc(sizeof(union float_int)*(nedges));
      //edges2=(union float_int *)malloc(sizeof(union float_int)*(nedges));
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
 nbrs[i].index=le64toh(outIdx[i - 1]) + 1;
 nbrs[i].total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);

}
else {
  nbrs[0].index=1;
  nbrs[0].total=le64toh(outIdx[0]);
}
if(max<nbrs[i].total)max=nbrs[i].total;
if(min>nbrs[i].total)min=nbrs[i].total;
if(nbrs[i].total==0)zerocount++;
for (unsigned jj = 0; jj <nbrs[i].total; ++jj) {
unsigned edgeindex = nbrs[i].index + jj;
unsigned dst =le32toh(outs[edgeindex - 1]);
//if(i<=5 || i==1048576 || i==1048577 || i==1048578 )printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
if (dst >=(npoints)) printf("\tinvalid edge\n");
edges[2*edgeindex].ipe=dst;
edges[2*edgeindex+1].ipe=edgeData[edgeindex - 1];
if(edgeData[edgeindex - 1]==0)
//printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
if(dst<npoints)(count[dst])++;
}
}
nbrs[npoints].index=nbrs[npoints-1].index+nbrs[npoints-1].total;
printf("ZERO COUNT=%d\n ",zerocount);
cfile.close();
printf("READ OVER");
printf("MAX=%d MIN=%d\n ",max,min);
return 1;
}

int HGraph::optimizeone(){
unsigned int nvv=npoints;
unsigned int insertindex=1;
fprintf(stderr, "\n opt1 \n ");
for(unsigned ii=0;ii<nvv;++ii){

unsigned src= points[ii].ipe;
unsigned dstindex= nbrs[src].index;
unsigned degree=nbrs[src].total;
if(degree && points[edges[3*dstindex+1].ipe].ipe> src+DISTANCETHRESHOLD){
//fprintf(stderr, "%d \n ",src);
unsigned nee=degree;
for(unsigned ee=0;ee<nee;++ee){
unsigned dst= edges[3*(dstindex+ee)+1].ipe;
unsigned dstentry=points[dst].ipe;
//swap done by below 3 statements
unsigned temp= nbrs[insertindex].index;
nbrs[insertindex].index=nbrs[dstentry].index;
nbrs[dstentry].index=temp;
//swap again points value
temp=points[ii].ipe;
points[ii].ipe=points[dst].ipe;
points[dst].ipe=temp;

if(++insertindex>=npoints)break;
}
if(insertindex>=npoints)break;
}
}
return 0;
}
int HGraph::optimizetwo(){
unsigned int nvv=npoints/2;
bool firsthalfsmaller=true;
unsigned int temp;
fprintf(stderr, "\n opt2 \n ");
for(unsigned ii=0;ii<nvv;++ii){
unsigned one=ii;
unsigned two=nvv+ii;
unsigned degreeone=nbrs[one].total;
unsigned degreetwo=nbrs[two].total;
if (degreeone > degreetwo && degreeone - degreetwo > THRESHOLDDEGREE && !firsthalfsmaller || degreetwo > degreeone && degreetwo - degreeone > THRESHOLDDEGREE && firsthalfsmaller) {
//fprintf(stderr, "%d  %d ",one,two);

temp=points[one].ipe;
points[one].ipe=points[two].ipe;
points[two].ipe=temp;

temp=nbrs[one].index;
nbrs[one].index=nbrs[two].index;
nbrs[two].index=temp;
firsthalfsmaller = !firsthalfsmaller;
}
}
return 0;


}
int HGraph::optimize(){
optimizeone();
//optimizetwo();
return 0;
}
int HGraph::sortPoints(){




}

int HGraph::cloneMorphGPU(GGraph &graph,int num){
cudaSetDevice(num);
graph.npoints=npoints;
//cudaMalloc(&(graph.maxnpoints),sizeof(int));
//cudaMemcpy(graph.maxnpoints,&(maxnpoints),sizeof(int),cudaMemcpyHostToDevice);
graph.maxnpoints=maxnpoints;
printf("MAX NNODES=%d %d %d\n",maxnpoints,npoints,edims);
cudaMalloc(&(graph.pnpoints),sizeof(int));
cudaMemcpy(graph.pnpoints,&(npoints),sizeof(int),cudaMemcpyHostToDevice);
graph.nedges=nedges;
//cudaMalloc(&(graph.maxnedges),sizeof(int));
//cudaMemcpy(graph.maxnedges,&(maxnedges),sizeof(int),cudaMemcpyHostToDevice);
graph.maxnedges=maxnedges;
cudaMalloc(&(graph.pnedges),sizeof(int));
cudaMemcpy(graph.pnedges,&(pnedges),sizeof(int),cudaMemcpyHostToDevice);
graph.pdims=pdims;
graph.edims=edims;
printf("YY\n");
if(cudaMalloc((void **)&(graph.points),(npoints+1)*3*sizeof(union float_int)*(pdims))!=cudaSuccess)printf("ALLOC ERRRR\n");
printf("YY1\n");
//if(cudaMalloc((void **)&(graph.edges),(nedges+1)*3*sizeof(union float_int)*(edims))!=cudaSuccess)printf("ALLOC EDGE ERROR\n");
if(cudaMalloc((void **)&(graph.edges),(nedges+1)*2*sizeof(union float_int)*(edims))!=cudaSuccess)printf("ALLOC EDGE ERROR\n");
printf("YY2\n");
//cudaMalloc((void **)&(graph.minedge),(maxnpoints+1)*sizeof(union float_int));
printf("YY3\n");
//cudaMalloc((void **)&(graph.maxedge),(maxnpoints+1)*sizeof(union float_int));
printf("YY4\n");
if(nbrs!=NULL|| outnbrs!=NULL || innbrs!=NULL){
struct neighbors *t1=(struct neighbors *)malloc(sizeof(struct neighbors));
cudaMalloc((void **)&(graph.nbrs),sizeof(struct neighbors)*(graph.npoints+1));
if(nbrs!=NULL){
cudaMemcpy(graph.nbrs,nbrs,sizeof(struct neighbors)*(graph.npoints+1),cudaMemcpyHostToDevice);
}
if(innbrs!=NULL){
struct neighbors *t1=(struct neighbors *)malloc(sizeof(struct neighbors));
cudaMemcpy(graph.innbrs,t1,sizeof(struct neighbors),cudaMemcpyHostToDevice);

}
if(outnbrs!=NULL){
cudaMemcpy(graph.outnbrs,t1,sizeof(struct neighbors),cudaMemcpyHostToDevice);

}
}
printf("XX\n");
cudaMemcpy(graph.points,points,(npoints+1)*sizeof(union float_int)*(pdims),cudaMemcpyHostToDevice);
printf("XX1\n");
//cudaMemcpy(graph.edges,edges,(nedges+1)*3*sizeof(union float_int)*(edims),cudaMemcpyHostToDevice);
cudaMemcpy(graph.edges,edges,(nedges+1)*2*sizeof(union float_int)*(edims),cudaMemcpyHostToDevice);
printf("XX2\n");
cudaMemcpy(graph.minedge,minedge,(maxnpoints+1)*sizeof(union float_int),cudaMemcpyHostToDevice);
printf("XX3\n");
cudaMemcpy(graph.maxedge,maxedge,(maxnpoints+1)*sizeof(union float_int),cudaMemcpyHostToDevice);
printf("XX4\n");
cudaDeviceSynchronize();
cudaSetDevice(0);
return 1;
}









	//allocOnHost();
int HGraph::cloneGPU(GGraph &graph,int num){
cudaSetDevice(num);
graph.npoints=npoints;
//cudaMalloc(&(graph.maxnpoints),sizeof(int));
//cudaMemcpy(graph.maxnpoints,&(maxnpoints),sizeof(int),cudaMemcpyHostToDevice);
graph.maxnpoints=maxnpoints;
//printf("MAX NNODES=%d %d \n",maxnpoints,npoints);
cudaMalloc(&(graph.pnpoints),sizeof(int));
cudaMemcpy(graph.pnpoints,&(npoints),sizeof(int),cudaMemcpyHostToDevice);
graph.nedges=nedges;
//cudaMalloc(&(graph.maxnedges),sizeof(int));
//cudaMemcpy(graph.maxnedges,&(maxnedges),sizeof(int),cudaMemcpyHostToDevice);
graph.maxnedges=maxnedges;
cudaMalloc(&(graph.pnedges),sizeof(int));
if(cudaMemcpy(graph.pnedges,&(pnedges),sizeof(int),cudaMemcpyHostToDevice)!=cudaSuccess)printf("ERROR pnedges");
graph.pdims=pdims;
graph.edims=edims;
pdims=2;
//printf("YY\n");
if(cudaMalloc((void **)&(graph.points),(npoints+1)*sizeof(union float_int)*3*(pdims))!=cudaSuccess)printf("ALLOC ERRRR\n");
//printf("YY1\n");
if(cudaMalloc((void **)&(graph.edges),(nedges+1)*sizeof(union float_int)*(edims))!=cudaSuccess)printf("ALLOC EDGE ERROR\n");
//if(cudaMalloc((void **)&(graph.edges1),(nedges)*sizeof(union float_int))!=cudaSuccess)printf("ALLOC EDGE ERROR\n");
//if(cudaMalloc((void **)&(graph.edges2),(nedges)*sizeof(union float_int))!=cudaSuccess)printf("ALLOC EDGE ERROR\n");
//if(cudaMalloc((void **)&(edges1),(nedges+1)*3*sizeof(union float_int)*(edims))!=cudaSuccess)printf("ALLOC EDGE ERROR\n");
//printf("YY2\n");
//cudaMalloc((void **)&(graph.minedge),(maxnpoints+1)*sizeof(union float_int));
//printf("YY3\n");
//cudaMalloc((void **)&(graph.maxedge),(maxnpoints+1)*sizeof(union float_int));
//printf("YY4\n");
if(nbrs!=NULL|| outnbrs!=NULL || innbrs!=NULL){
cudaMalloc((void **)&(graph.nbrs),sizeof(struct neighbors)*(graph.npoints+1));
if(nbrs!=NULL){
cudaMemcpy(graph.nbrs,nbrs,sizeof(struct neighbors)*(graph.npoints+1),cudaMemcpyHostToDevice);
}
if(innbrs!=NULL){
cudaMalloc((void **)&(graph.innbrs),sizeof(struct neighbors)*graph.npoints);
cudaMemcpy(graph.innbrs,innbrs,sizeof(struct neighbors)*npoints,cudaMemcpyHostToDevice);

}
if(outnbrs!=NULL){
cudaMalloc((void **)&(graph.outnbrs),sizeof(struct neighbors)*graph.npoints);
cudaMemcpy(graph.outnbrs,outnbrs,sizeof(struct neighbors)*graph.npoints,cudaMemcpyHostToDevice);

}
}
printf("XX copy points\n");
if(points!=NULL)cudaMemcpy(graph.points,points,(npoints+1)*sizeof(union float_int)*3*(pdims),cudaMemcpyHostToDevice);
printf("XX1 copy edges\n");
if(edges!=NULL)cudaMemcpy(graph.edges,edges,(nedges+1)*sizeof(union float_int)*(edims),cudaMemcpyHostToDevice);
printf("XX1 copy min edges\n");
if(minedge!=NULL)cudaMemcpy(graph.minedge,minedge,(maxnpoints+1)*sizeof(union float_int),cudaMemcpyHostToDevice);
//printf("XX3\n");
printf("XX1 copy max edges\n");
if(maxedge!=NULL)cudaMemcpy(graph.maxedge,maxedge,(maxnpoints+1)*sizeof(union float_int),cudaMemcpyHostToDevice);
//printf("XX4\n");
cudaDeviceSynchronize();
cudaSetDevice(0);
return 1;
}
int HGraph::copytoGPU(GGraph &graph){
graph.npoints=npoints;
graph.nedges=nedges;
graph.pdims=pdims;
graph.edims=edims;
if(nbrs!=NULL|| outnbrs!=NULL || innbrs!=NULL){
struct neighbors *t1=(struct neighbors *)malloc(sizeof(struct neighbors));
cudaMalloc((void **)&(graph.nbrs),sizeof(struct neighbors));
if(nbrs!=NULL){
cudaMemcpy(graph.nbrs,t1,sizeof(struct neighbors),cudaMemcpyHostToDevice);
}
if(innbrs!=NULL){
cudaMemcpy(graph.innbrs,t1,sizeof(struct neighbors),cudaMemcpyHostToDevice);


}
if(outnbrs!=NULL){
cudaMemcpy(graph.outnbrs,t1,sizeof(struct neighbors),cudaMemcpyHostToDevice);

}
}
cudaMemcpy(graph.points,points,(npoints)*sizeof(union float_int)*(pdims),cudaMemcpyHostToDevice);
cudaMemcpy(graph.edges,edges,(nedges+1)*sizeof(union float_int)*(edims),cudaMemcpyHostToDevice);
cudaMemcpy(graph.minedge,minedge,(npoints)*sizeof(union float_int),cudaMemcpyHostToDevice);
cudaMemcpy(graph.maxedge,maxedge,(npoints)*sizeof(union float_int),cudaMemcpyHostToDevice);
}
int HGraph::copyfromGPU(GGraph &graph){
npoints=graph.npoints;
nedges=graph.nedges;
pdims=graph.pdims;
edims=graph.edims;

if(graph.outnbrs!=NULL){
outnbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
 struct neighbors temp;
cudaMemcpy(&temp,graph.outnbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}
if(graph.innbrs!=NULL){
innbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
struct neighbors temp;
cudaMemcpy(&temp,graph.innbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}
if(graph.nbrs!=NULL){
nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
 struct neighbors temp;
cudaMemcpy(&temp,graph.nbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}
cudaMemcpy(points,graph.points,(npoints)*sizeof(union float_int)*(pdims),cudaMemcpyDeviceToHost);
cudaMemcpy(edges,graph.edges,(nedges)*sizeof(union float_int)*(edims),cudaMemcpyDeviceToHost);
cudaMemcpy(minedge,graph.minedge,(npoints)*sizeof(union float_int),cudaMemcpyDeviceToHost);
cudaMemcpy(maxedge,maxedge,(npoints)*sizeof(union float_int),cudaMemcpyDeviceToHost);

}
int HGraph::deallocate(){
}
int GGraph::cloneCPU(HGraph &hgraph){
printf("IN CLONE CPU\n");
hgraph.npoints=npoints;
hgraph.nedges=nedges;
hgraph.pdims=pdims;
hgraph.edims=edims;
hgraph.minedge=(union float_int *)malloc(sizeof(union float_int)*(npoints));
hgraph.maxedge=(union float_int *)malloc(sizeof(union float_int)*(npoints));
      hgraph.points=(union float_int *)malloc(sizeof(union float_int )*(npoints)*(pdims));
      hgraph.edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*(edims));
cudaMemcpy(hgraph.points,points,(npoints)*sizeof(union float_int)*(pdims),cudaMemcpyDeviceToHost);
cudaMemcpy(hgraph.edges,edges,(nedges)*sizeof(union float_int)*(edims),cudaMemcpyDeviceToHost);
cudaMemcpy(hgraph.minedge,minedge,(npoints)*sizeof(union float_int),cudaMemcpyDeviceToHost);
cudaMemcpy(hgraph.maxedge,maxedge,(npoints)*sizeof(union float_int),cudaMemcpyDeviceToHost);
if(outnbrs!=NULL){
 struct neighbors temp;
cudaMemcpy(&temp,outnbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}
if(innbrs!=NULL){

 struct neighbors temp;
cudaMemcpy(&temp,innbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);

}
if(nbrs!=NULL){
 struct neighbors temp;
cudaMemcpy(&temp,nbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}

}
int GGraph::copytoCPU(HGraph &hgraph){
hgraph.npoints=npoints;
hgraph.nedges=nedges;
hgraph.pdims=pdims;
hgraph.edims=edims;
if(outnbrs!=NULL){
 struct neighbors temp;
cudaMemcpy(&temp,outnbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}
if(innbrs!=NULL){
 struct neighbors temp;
cudaMemcpy(&temp,innbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}
if(nbrs!=NULL){
 struct neighbors temp;
cudaMemcpy(&temp,nbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}
cudaMemcpy(hgraph.points,points,(npoints)*sizeof(union float_int)*(pdims),cudaMemcpyDeviceToHost);
cudaMemcpy(hgraph.edges,edges,(nedges)*sizeof(union float_int)*(edims),cudaMemcpyDeviceToHost);
cudaMemcpy(hgraph.minedge,minedge,(npoints)*sizeof(union float_int),cudaMemcpyDeviceToHost);
cudaMemcpy(hgraph.maxedge,maxedge,(npoints)*sizeof(union float_int),cudaMemcpyDeviceToHost);



}
int GGraph::copyfromCPU(HGraph &hgraph){

npoints=hgraph.npoints;
nedges=hgraph.nedges;
pdims=hgraph.pdims;
edims=hgraph.edims;

if(outnbrs!=NULL){
 struct neighbors temp;
cudaMemcpy( &temp,&outnbrs, sizeof(struct neighbors),cudaMemcpyDeviceToHost );
}
if(nbrs!=NULL){
struct neighbors temp;
cudaMemcpy(&temp,nbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);

}
if(innbrs!=NULL){
struct neighbors temp;
cudaMemcpy(&temp,innbrs,sizeof(struct neighbors),cudaMemcpyDeviceToHost);
}
cudaMemcpy(points,hgraph.points,(npoints)*sizeof(union float_int)*(pdims),cudaMemcpyHostToDevice);
cudaMemcpy(edges,hgraph.edges,(nedges)*sizeof(union float_int)*(edims),cudaMemcpyHostToDevice);
cudaMemcpy(minedge,hgraph.minedge,(npoints)*sizeof(union float_int),cudaMemcpyHostToDevice);
cudaMemcpy(maxedge,hgraph.maxedge,(npoints)*sizeof(union float_int),cudaMemcpyHostToDevice);

}

int GGraph::allocate(){


}
int GGraph::deallocate(){
}
__device__ int GGraph::getEdgeWeight(int t1,int t2){
return edges[(nbrs[t1].index+t2)*3+2].ipe;

}
__device__ __forceinline__ void MIN( int *t1,int t2,int &result){
//__device__   void  MIN(int *t1,int t2,int &result){
int ch= *t1;
if(ch > t2){
atomicMin(t1,t2);
//if(*t1<ch && result!=1)
result=1;
}
//return result;
}
__device__  int GMIN(int *t1,int t2,int &result){
int ch= *t1;
if(ch > t2){
atomicMin(t1,t2);
result=1;
}
return result;
}
__device__  int MAX(int *t1,int t2,int &result){
int ch= *t1;
if(ch < t2){
atomicMax(t1,t2);
if(*t1>ch && result!=1)result=1;
}
return result;
}
__device__ int GADD(int *t1,int t2){
int t4=atomicAdd(t1,t2);
return t4;
}
__device__ int INCR(int *t1,int t2){
int t4=atomicAdd(t1,t2);
return t4;
}
__device__ int DECR(int *t1,int t2){
int t4=atomicAdd(t1,t2);
return t4;
}
__device__ int SUB(int *t1,int t2){
int t4=atomicSub(t1,t2);
return t4;
}
__device__ int AND(int *t1,int t2){
int t4=atomicOr(t1,t2);
return t4;
}
__device__ int OR(int *t1,int t2){
int t4=atomicAnd(t1,t2);
return t4;
}
__device__ int XOR(int *t1,int t2){
int t4=atomicXor(t1,t2);
return t4;
}

  int HMIN(int *t1,int t2,int id,int &result){
if(*t1 > t2){
while(__sync_val_compare_and_swap(&writelock[id],0,1)==1);
  if (*t1 > t2){
*t1=t2;
result=1; 
}
__sync_val_compare_and_swap(&writelock[id],1,0);
}
return result;
}
 void copyaftermorph(GGraph graph){
cudaMemcpy(&(graph.npoints), graph.pnpoints,sizeof(int),cudaMemcpyDeviceToHost);
cudaMemcpy(&(graph.nedges), graph.pnedges,sizeof(int),cudaMemcpyDeviceToHost);
}
int findthreadandblocksize(cudaDeviceProp &prop, int &blocksize,int &threadsize){
//printf("Threads per block=%d\n",prop.maxThreadsPerBlock);
//printf("maxThreadsPerMultiProcessor=%d\n",prop.maxThreadsPerMultiProcessor);
//printf("multiProcessorCount=%d\n",prop.multiProcessorCount);
//printf("total threads for the grid with barrier=%d\n", prop.multiProcessorCount*prop.maxThreadsPerMultiProcessor);
 blocksize= prop.multiProcessorCount*prop.maxThreadsPerMultiProcessor/prop.maxThreadsPerBlock;
 threadsize=prop.maxThreadsPerBlock;

}
__device__ unsigned volatile int barrcount;
__device__ void InitSyncGrid(const int index)
{
    if (index == 0)       barrcount = 0;
__threadfence();
    if (threadIdx.x == 0)        while (barrcount != 0);
    __syncthreads();
}
__device__ void SyncGrid()
{
    unsigned int oldc;
    __threadfence();
    if (threadIdx.x == 0)
    {
        oldc = atomicInc((unsigned int *)&barrcount, gridDim.x);
        __threadfence();
            while (barrcount != gridDim.x);
}
    __syncthreads();
}
__device__ void Kernel_Synced()
{
    InitSyncGrid(threadIdx.x + blockIdx.x*blockDim.x);
    SyncGrid();
}
double rtclock()
{
    struct timezone Tzp;
    struct timeval Tp;
    int stat;
    stat = gettimeofday (&Tp, &Tzp);
    if (stat != 0) printf("Error return from gettimeofday: %d",stat);
    return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

 int HADD(int *t1,int t2){
int t4=__sync_fetch_and_add(t1,t2);
return t4;
}

int findthreadsperblock(cudaDeviceProp x){
int cores=0;
switch (x.major){
     case 2: // Fermi
      if (x.minor == 1) cores = 48;
      else cores =  32;
      break;
     case 3: // Kepler
      cores =  192;
      break;
     case 5: // Maxwell
      cores =  128;
      break;
     default:
      printf("Unknown device type\n"); 
      break;
      }
return (x.maxThreadsPerBlock- (x.maxThreadsPerBlock%cores));


}
int findblocksize(GGraph graph, int elem,int TPB){
printf("ELEM =%d \n",elem);
if ( (elem/TPB+1)>(32*1024) )return (32*1024);
else return (elem/TPB+1);
}






#endif
