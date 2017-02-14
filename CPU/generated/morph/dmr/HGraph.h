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
//#include "GGraph.cu"
#include "Graph.h"
#include "Galois/Endian.h"
#include "Galois/MethodFlags.h"
#include "Galois/LargeArray.h"
#include "Galois/Graph/Details.h"
#include "Galois/Runtime/Context.h"
#include "Galois/Runtime/ll/CacheLineStorage.h"
#include "Galois/Runtime/ll/CompilerSpecific.h"

#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/utility.hpp>
#include <parallel/algorithm>


#define DISTANCETHRESHOLD	150
#define THRESHOLDDEGREE		10
#define INVALID               1234567890
struct Point {
 float x;
 float y;
 float z;
};
int writelock[8388608*8];
//SIZE=4847571
class HGraph: public Graph{
public:
int maxnpoints,*pnpoints,maxnedges,pnedges;
 int init();
int  readNodes(char *filename,int dims);
int  printNodes(char *filename,int dims);
int readEdges(char *filename);
int readEdges1(char *filename);
void printEdges(char *filename);
int read(char *filename);
int read2(char *filename);
int readMorph(char *filename);
int read1(char *filename);
int optimizeone();
int optimizetwo();
int optimize();
int readMesh(char *filename,int dims);
int addPoint(int,int);
int addPoint(float,float);
int sortPoints();
int MakePartition();
int MakePartition1();
  typedef boost::counting_iterator<uint64_t> iterator;
typedef iterator local_iterator;
  iterator edge_begin(int  N) const {
printf(" EDGE BEGIN\n");
return index[N];//index[N]
}
  iterator edge_end(int  N) const{
printf(" EDGE END\n");
return index[N]+index[N+1]-index[N];
}
  iterator local_begin() const {
    unsigned int id = Galois::Runtime::LL::getTID();
    unsigned int num = Galois::getActiveThreads();
int begin= (npoints + num - 1) / num * id;
printf("LOCAL begin %d %d \n", id,begin);

return begin; 
//return 0;
}
//definiton for local_begin and local_end copied from localBegin and localEnd from Galois/Graph/Details.h
  iterator local_end() const{
    unsigned int id = Galois::Runtime::LL::getTID();
    unsigned int num = Galois::getActiveThreads();
    uint64_t end = (npoints + num - 1) / num * (id + 1);
//uint64_t retval= std::min(end,npoints);
//return retval;
printf("LOCAL END %d %d %d \n",num, id,end); 
    if( id < num-1)return end;
    else return npoints;
//return npoints;
}
  iterator begin() const {
printf(" BEGIN\n");
return index[0];
}
  iterator end() const{
printf(" END\n");
return index[npoints];
}
//int cloneGPU(GGraph &graph);
//int copytoGPU(GGraph &graph);
//int copyfromGPU(GGraph &graph);
int getPoint(int pos);
int deallocate();
int addEdge(int pos,int src,int dst,int weight);
int addEdge(int src,int dst,int weight);
int sortEdges(int point);
void (*extra_alloc)(HGraph &);//needed for read,copy,allocate,deallocate extra  							                      // properties
};
/*
int makesymmetric(HGraph &input,HGraph &output){
   output.nedges=input.nedges*2;
   output.npoints=input.npoints;
output.index=(int *)malloc(sizeof(int)*(input.npoints+1));
output.points=(union float_int *)malloc(sizeof(union float_int )*(output.npoints+1)*output.pdims);
      output.edges=(union float_int *)malloc(sizeof(union float_int)*(output.nedges)*output.edims);
int *currindex=(int *)malloc(sizeof(int)*input.npoints);
int *incomings=(int *)malloc(sizeof(int)*input.npoints);
for(int i=0;i<input.npoints;i++){incomings[i]=currindex[i]=0;}
for(int i=0;i<input.npoints;i++){
int t1=index[i+1]-index[i];//input total
int t2=input.index[i];//input index
for (unsigned jj = 0; jj <t1; ++jj) {
 int ind= jj+t2;//input index,for this point
int dst=input.edges[3*ind+1].ipe;//dst point
if(dst <3 ) printf("%d %d  %d \n",input.edges[3*ind].ipe,dst,input.edges[3*ind+2].ipe);
incomings[dst]++;//increment incomings for dst point
}
}
int currpos=0;
for(int i=0;i<input.npoints;i++){
int t1=index[i+1]-index[i];//input total
int t2=index[i];//input index
output.index[i]=currpos;
currindex[i]=currpos;
currpos=currpos+1;
}
for(int i=0;i<input.npoints;i++){
int t1=input.nbrs[i].total;//input total outgoing
int t2=input.nbrs[i].index;//input index
int t3=output.nbrs[i].total;//output total
int t4=output.nbrs[i].index;//output index
for (unsigned jj = 0; jj <t1; ++jj) {
 int ind= jj+t2;//input index,for this point
 int ind1= t4+jj;//output index,for this point
output.edges[3*ind1].ipe=input.edges[3*ind].ipe;
output.edges[3*ind1+1].ipe=input.edges[3*ind+1].ipe;
output.edges[3*ind1+2].ipe=input.edges[3*ind+2].ipe;
}
currindex[i]+=t1;
}
for(int i=0;i<3;i++){
printf(" input total=%d incomin=%d output total=%d \n",input.nbrs[i].total,incomings[i],output.nbrs[i].total);
}

for(int i=0;i<input.npoints;i++){
int t1=input.nbrs[i].total;
int t2=input.nbrs[i].index;//input index
int t4=currindex[i];//output index
int t3=output.nbrs[i].index;
for (unsigned jj = 0; jj <t1; ++jj) {
 int ind= jj+t2;//input index,for this point
int dst=input.edges[3*ind+1].ipe;//dst point
int ind1=currindex[dst];
output.edges[3*ind1].ipe=input.edges[3*ind+1].ipe;
output.edges[3*ind1+1].ipe=input.edges[3*ind].ipe;
output.edges[3*ind1+2].ipe=input.edges[3*ind+2].ipe;
currindex[dst]++;
}
}
}*/









 int HGraph::addPoint(float x,float y){
int x1=1;
int t1;
t1=__sync_fetch_and_add(pnpoints,x1);
//if(t1<*(maxnpoints)){
//if(t1<(maxnpoints)){
points[2*t1].fpe=x;
points[2*t1+1].fpe=y;
//}
return t1;
}

 int HGraph::addPoint(int x,int y){
int x1=1;
int t1;
t1=__sync_fetch_and_add(pnpoints,x1);
//if(t1<*(maxnpoints)){
//if(t1<(maxnpoints)){
points[2*t1].ipe=x;
points[2*t1+1].ipe=y;
//}
return t1;
}
int HGraph::init(){
}
int HGraph::getPoint(int pos){
if(pos<npoints)return points[pos].ipe;
return -1;
}

int HGraph::sortEdges(int point){
int minpos;
int tt=index[point+1]-index[point];
int start=index[point];
int end=start+tt;
for(int i=start;i<end;i++){
minpos=i;
for(int j=i+1;j<end;j++){
if(edges[2*minpos+1].ipe> edges[2*j+1].ipe)minpos=j;
}
int dst=edges[2*minpos].ipe;
int weight=edges[2*minpos+1].ipe;
edges[2*minpos].ipe=edges[2*i].ipe;
edges[2*minpos+1].ipe=edges[2*i+1].ipe;
edges[2*i].ipe=dst;
edges[2*i+1].ipe=weight;
}
if(point<3){
for(int i=start;i<end;i++){
fprintf(stderr,"%d %d %d %d\n ",point,edges[2*i].ipe,edges[2*i+1],1000);
}
}
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
int HGraph::addEdge(int src,int dst,int weight){
//int pos=__sync_fetch_and_add(&(nbrs[src].total),1);
int pos;
//pos=pos+nbrs[src].index;
if(pos<nedges*3){
//edges[3*pos].ipe=src;
//edges[3*pos+1].ipe=dst;
//edges[3*pos+2].ipe=weight;
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
pnpoints=(int *)malloc(sizeof(int));
fscanf(fp,"%d",&npoints1);
printf("HERE %d \n",npoints1);
points=(union float_int *)(malloc(sizeof(union float_int )*npoints1*15*pdims));
npoints=npoints1;
*pnpoints=npoints1;
maxnpoints=npoints1*3;
fgets(temp,100,fp);
if(dims==2){
for(int i=0;i<npoints1;i++){
fscanf(fp,"%d%f%f",&index,&x,&y);
if(i<10)
fprintf(stderr,"%d %f %f\n",index,x,y);
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
/*
void HGraph::printEdges(char *filename){

FILE *fp=fopen(filename,"w+");

fprintf(fp,"#Nodes: %d\n",npoints);
fprintf(fp,"#Edges: %d\n",nedges);
fprintf(fp,"#Directed\n");
for(int i=0;i<nedges;i++)
if(edges[3*i+2].ipe!=0)
fprintf(fp,"%d %d %d \n",edges[3*i],edges[3*i+1],edges[3*i+2]);
else
fprintf(fp,"%d %d %d \n",edges[3*i],edges[3*i+1],rand()%100);

fclose(fp);
}*/
int HGraph::readEdges(char *filename){                                                                                                                       
FILE *fp=fopen(filename,"r");
fscanf(fp,"%d%d",&npoints,&nedges);
nedges++;
int src,dst,weight;
edims=2;
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*1);
index=(int *)malloc(sizeof(int )*(npoints+1));
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims); 
int currsrc=-1,ttindex=1,currindex=0;
for(int i=0;i<(npoints);i++){ points[i].ipe=i;}
for(int i=0;i<nedges;i++){
fscanf(fp,"%d%d%d",&src,&dst,&weight);
if(src!=currsrc){
for(int kk=currsrc+1;kk<src;kk++){ //if some points/nodes does not have any edges
index[kk]=ttindex;
}
currsrc=src;
index[src]=ttindex;
currindex=ttindex;
edges[2*ttindex].ipe=dst;
edges[2*ttindex+1].ipe=weight;
ttindex++;
}
else {
edges[2*ttindex].ipe=dst;
edges[2*ttindex+1].ipe=weight;
ttindex++;
}
}
for(int i=currsrc+1;i<=npoints;i++)index[i]=ttindex-1;
fclose(fp);
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
	npoints = numNodes;
	nedges = numEdges+1;
int *count,*incountar;
count=(int *)malloc(sizeof(int)*npoints);
incountar=(int *)malloc(sizeof(int)*npoints);
for(int i=0;i<npoints;i++){count[i]=incountar[i]=0;}

	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
index=(int *)malloc(sizeof(int)*npoints);
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims);
int total;
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
index[i]=le64toh(outIdx[i - 1]) + 1;
total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);

}
else {
  index[i]=1;
 total=le64toh(outIdx[0]);
}
index[npoints]=index[npoints-1]+total;
for (unsigned jj = 0; jj <total; ++jj) {
unsigned edgeindex = index[i] + jj;
unsigned dst =le32toh(outs[edgeindex - 1]);
if(i<=5)printf("Edge from %d to %d with cost %d \n",i,dst,edgeData[edgeindex - 1]);
if(dst==1)printf("dst=%d src=%d \n",dst,i);
if (dst >=(npoints)) printf("\tinvalid edge\n");
edges[edgeindex*2].ipe=dst;
edges[edgeindex*2+1].ipe=edgeData[edgeindex - 1];
if(dst<npoints)(count[dst])++;
}
}
inindex=(int *)malloc(sizeof(int )*(npoints+1));
unsigned long int incount=0;
for(int i=0;i<npoints;i++){inindex[i]=incount; incount+=count[i];}
inindex[npoints]=index[npoints];
inedges=(union float_int *)malloc(sizeof(union float_int)*(nedges+1));
for(int i=0;i<npoints;i++)count[i]=0;
for(int i=1;i<nedges;i++){
int p= edges[2*i+1].ipe;
inedges[inindex[i]+count[p]].ipe=i;
inedges[inindex[i]+count[p]+1].ipe=edges[2*i+1].ipe;
(count[p])++;

}
cfile.close();
return 1;
}


/*
int HGraph::readEdges1(char *filename){
FILE *fp=fopen(filename,"r");

fscanf(fp,"%d%d",&npoints,&nedges);
int src,dst,weight;
nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims); 
int currsrc=0,index=0,currindex=0;
for(int i=0;i<(npoints);i++){ points[i].ipe=i;}
nbrs[0].index=0;
int nbrmin=99999,nbrmax=0;
int cnt=0;
for(int i=0;i<nedges;i++){

fscanf(fp,"%d%d ",&src,&dst);
if(src!=currsrc){
int xxx=index-currindex;
if(nbrmax<xxx)nbrmax=xxx;
if(nbrmin>xxx)nbrmin=xxx;
currsrc=src;
//nbrs->index[src]=index;
currindex=index;
//edges[3*i].ipe=src;
//edges[3*i+1].ipe=dst;
//edges[3*i+2].ipe=1;
}
else {
//edges[3*i].ipe=src;
//edges[3*i+1].ipe=dst;
//edges[3*i+2].ipe=1;
index++;
}
}
fclose(fp);
printf("NBRMAX=%d NBRMIN=%d \n",nbrmax,nbrmin);
printf("READ OVER");
}*/


/*
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
count=(int *)malloc(sizeof(int)*npoints);
indexmorph=(int *)malloc(sizeof(int)*npoints);
for(int i=0;i<npoints;i++)count[i]=0;
	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
nbrs=(struct neighbors *)malloc(sizeof(struct neighbors)*npoints);
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges+1)*edims*3);
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
 nbrs[i].index=le64toh(outIdx[i - 1]) + 1;
 indexmorph[i]=le64toh(outIdx[i - 1])*3 + 1;
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
edges[morphedgeindex*3].ipe=i;
edges[morphedgeindex*3+1].ipe=dst;
edges[morphedgeindex*3+2].ipe=edgeData[edgeindex - 1];
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
}*/


int HEAVIEST;
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
int max=0,min=99999,total=0;
edims=2;
	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
index=(int *)malloc(sizeof(int)*(npoints+1));
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims);
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
 index[i]=le64toh(outIdx[i - 1]) + 1;
 total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);
}
else {
index[0]=1;
 total=le64toh(outIdx[0]);
}
for (unsigned jj = 0; jj <total; ++jj) {
unsigned edgeindex = index[i] + jj;
if(edgeindex > nedges){
printf("%d %d %d\n",nedges,edgeindex,i);
exit(0);

}
unsigned dst =le32toh(outs[edgeindex - 1]);
edges[2*edgeindex].ipe=dst;
edges[2*edgeindex+1].ipe=edgeData[edgeindex - 1];
if(edgeData[edgeindex - 1]>HEAVIEST)HEAVIEST=edgeData[edgeindex - 1];
}
}
index[npoints]=index[npoints-1]+total;
printf("ZERO COUNT=%d\n ",zerocount);
cfile.close();
printf("READ OVER");
printf("MAX=%d MIN=%d\n ",max,min);
}





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
int max=0,min=99999,maxedge=0,pendcnt=0,total=0;
int nbrmax=0,nbrmin=9999;
count=(int *)malloc(sizeof(int)*npoints);
for(int i=0;i<npoints;i++)count[i]=0;
	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
index=(int  *)malloc(sizeof(int)*(npoints+1));
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims);
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
 index[i]=le64toh(outIdx[i - 1]) + 1;
 total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);
}
else {
  index[0]=1;
 total=le64toh(outIdx[0]);
}
for (unsigned jj = 0; jj <total; ++jj) {
unsigned edgeindex = index[i] + jj;
unsigned dst =le32toh(outs[edgeindex - 1]);
if (dst >=(npoints)) printf("\tinvalid edge\n");
edges[edgeindex*2].ipe=dst;
edges[edgeindex*2+1].ipe=edgeData[edgeindex - 1];
}
}
index[npoints]=index[npoints-1]+total;
cfile.close();
printf("READ OVER");
return 1;
}
struct tempforsort{
int point;
int total;
}*pointlist;
int comparefunction( int p1, int p2){
//if(p1.total==p2.total) return p1.index <p2.index;
 return p1 > p2;
}
/*
int comparefunction1(struct neighbors p1, struct neighbors p2){
if(p1.total==p2.total) return p1.index <p2.index;
 return p1.total > p2.total;
}*/
std::map<int,int>vertexmap1;
std::map<int,int>vertexmap;
/*
int HGraph::sortPoints(){
for(int i=0;i<npoints;i++)vertexmap[index[i]]=i;
//std::sort(nbrs,nbrs+20,comparefunction1);
__gnu_parallel::sort(nbrs,nbrs+npoints,comparefunction1);
for(int i=0;i<npoints;i++)vertexmap1[vertexmap[index[i]]]=i;
//__gnu_parallel::sort(pointlist,pointlist+npoints,comparefunction1);
//for(int i=0;i<20;i++)printf("%d  %d  %d  %d \n",i, vertexmap[nbrs[i].index],nbrs[i].total,nbrs[i].index);
for(int i=0;i<nedges;i++){
edges[2*i].ipe=vertexmap1[edges[2*i].ipe];
}
*/
/*
int HGraph:: MakePartition1(){ 

int remote=0,edgepart1=0,edgepart2=0;
int maxnbrs=16;
int part1nodes=0;
for(int i=0;i<npoints;i++){
if(nbrs[i].total>=(maxnbrs-10)){
int t1= nbrs[i].index;
int t2=nbrs[i].total;
for(int t3=t1;t3<t1+t2;t3++){
if(!(points[edges[3*t3].ipe].ipe&0x08000000)){
points[edges[3*t3].ipe].ipe=points[edges[3*t3].ipe].ipe|0x08000000;
part1nodes++;
}
if(!(points[edges[3*t3+1].ipe].ipe&0x08000000)){
points[edges[3*t3+1].ipe].ipe=points[edges[3*t3+1].ipe].ipe|0x08000000;
part1nodes++;
}
}

}
}
bool *remotenodes=(bool *)malloc(sizeof(bool)*npoints);
for(int i=0;i<npoints;i++)remotenodes[i]=false;
printf(" part1nodes=%d\n",part1nodes);
for(int i=0;i<nedges;i++){
if((points[edges[3*i].ipe].ipe&0x08000000)!=(points[edges[3*i+1].ipe].ipe&0x08000000)){//end points in two partition
remote++;
if(points[edges[3*i].ipe].ipe&0x08000000)edgepart1++;
else edgepart2++;
remotenodes[edges[3*i].ipe]=remotenodes[edges[3*i+1].ipe]=true;
}
else if(points[edges[3*i].ipe].ipe&0x08000000)edgepart1++;//both points in partition1
else edgepart2++;//both points in partition2
}
printf("REMOTE EDGES %d  %d  %d\n",remote,edgepart1,edgepart2);
remote=0;
for(int i=0;i<npoints;i++)if(points[i].ipe&0x08000000)remote++;
printf("POINTS PART1  %d  PART2 %d  \n",remote,npoints-remote);
remote=0;
int remotenodepart1=0,remotenodepart2=0,nodepart1=0,nodepart2=0;
for(int i=0;i<npoints;i++){
if(remotenodes[i]==true){//node is remote
if(points[i].ipe&0x08000000)remotenodepart1++; //node is in partition1
else remotenodepart2++;//node in parition2 
remote++;
}
else {

if(points[i].ipe&0x08000000)vertexmap1[points[i].ipe]=nodepart1++;//add node1 partition1 map
else vertexmap[points[i].ipe]=nodepart2++;//add node to partition2 map

}
//for(int i=0;i<npoints;i++)if(remotenodes[i]==true){
//if(points[i].ipe&0x08000000)vertexmap[points[i].ipe]=nodepart1;
//else vertexmap[points[i].ipe]=nodepart2++;
//}
//printf("REMOTE NODES %d part1=%d part2=%d\n",remote,remotenodepart1,remotenodepart2);
}
}

*/
/*

int HGraph:: MakePartition(){
int remote=0,edgepart1=0,edgepart2=0;

srand(3);
bool *remotenodes=(bool *)malloc(sizeof(bool)*npoints);
for(int i=0;i<npoints;i++)remotenodes[i]=false;
for(int i=0;i<npoints;i++){
if(rand()%2==0)points[i].ipe=points[i].ipe|0x08000000;
}
for(int i=0;i<nedges;i++){
if((points[edges[3*i].ipe].ipe&0x08000000)!=(points[edges[3*i+1].ipe].ipe&0x08000000)){
remote++;
//edgepart1++,edgepart2++;
if(points[edges[3*i].ipe].ipe&0x08000000)edgepart1++;
else edgepart2++;
remotenodes[edges[3*i].ipe]=remotenodes[edges[3*i+1].ipe]=true;
}
else if(points[edges[3*i].ipe].ipe&0x08000000)edgepart1++;
else edgepart2++;
}
printf("REMOTE EDGES %d \n",remote);
remote=0;
for(int i=0;i<npoints;i++)if(points[i].ipe&0x08000000)remote++;
printf("POINTS PART1  %d  PART2 %d  \n",remote,npoints-remote);
remote=0;
for(int i=0;i<npoints;i++)if(remotenodes[i]==true)remote++;
printf("REMOTE NODES %d\n",remote);
}*/
int HGraph::optimizeone(){
return 0;
}
int HGraph::optimizetwo(){
return 0;
}
int HGraph::optimize(){
//optimizeone();
//optimizetwo();
return 0;
}









int HGraph::deallocate(){
}
 void inline  HMIN(int *t1,int t2,int id,int &result){
if(*t1 > t2){
//while(__sync_val_compare_and_swap(&writelock[id],0,1)==1);
  if (*t1 > t2){
volatile int ch=*t1;
__sync_val_compare_and_swap(t1,ch,t2);
if(*t1<ch)result=1; 
}
//__sync_val_compare_and_swap(&writelock[id],1,0);
}
//return result;
}
//__device__  int MAX(int *t1,int t2,int &result){
//int ch= *t1;
//if(ch < t2){
//atomicMax(t1,t2);
//if(*t1>ch && result!=1)result=1;
//}
//return result;
//}
  int  HMAX(int *t1,int t2,int id,int &result){
if(*t1 <  t2){
while(__sync_val_compare_and_swap(&writelock[id],0,1)==1);
  if (*t1 < t2){
volatile int ch=*t1;
__sync_val_compare_and_swap(t1,ch,t2);
result=1; 
}
__sync_val_compare_and_swap(&writelock[id],1,0);
}
return result;
}
 int HADD(int *t1,int t2){
int t4=__sync_fetch_and_add(t1,t2);
return t4;
}
//__device__ int ADD(int *t1,int t2){
//int t4=atomicAdd(t1,t2);
//return t4;
//}
 int HINCR(int *t1,int t2){
int t4=__sync_fetch_and_add(t1,t2);
return t4;
}
//__device__ int INCR(int *t1,int t2){
//int t4=atomicAdd(t1,t2);
//return t4;
//}
 int HDECR(int *t1,int t2){
int t4=__sync_fetch_and_sub(t1,t2);
return t4;
}
//__device__ int DECR(int *t1,int t2){
//int t4=atomicAdd(t1,t2);
//return t4;
//}
 int HSUB(int *t1,int t2){
int t4=__sync_fetch_and_sub(t1,t2);
return t4;
}
//__device__ int SUB(int *t1,int t2){
//int t4=atomicSub(t1,t2);
//return t4;
//}
 int HAND(int *t1,int t2){
int t4=__sync_fetch_and_and(t1,t2);
return t4;
}

//__device__ int AND(int *t1,int t2){
//int t4=atomicAnd(t1,t2);
//return t4;
//}
 int HOR(int *t1,int t2){
int t4=__sync_fetch_and_or(t1,t2);
return t4;
}
//__device__ int OR(int *t1,int t2){
//int t4=atomicOr(t1,t2);
//return t4;
//}
 int HXOR(int *t1,int t2){
int t4=__sync_fetch_and_xor(t1,t2);
return t4;
}
//__device__ int XOR(int *t1,int t2){
//int t4=atomicXor(t1,t2);
//return t4;
//}
// int HNAND(int *t1,int t2){
//int t4=__sync_fetch_and_nand(t1,t2);
//return t4;
//}
double rtclock()
{
    struct timezone Tzp;
    struct timeval Tp;
    int stat;
    stat = gettimeofday (&Tp, &Tzp);
    if (stat != 0) printf("Error return from gettimeofday: %d",stat);
    return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}
















#endif
