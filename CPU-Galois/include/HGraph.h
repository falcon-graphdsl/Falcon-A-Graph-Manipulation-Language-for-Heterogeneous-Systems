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
int HEAVIEST;
struct Point {
 float x;
 float y;
 float z;
};
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
int getPoint(int pos);
int deallocate();
int addEdge(int pos,int src,int dst,int weight);
int addEdge(int src,int dst,int weight);
int sortEdges(int point);
void (*extra_alloc)(HGraph &);//needed for read,copy,allocate,deallocate extra  							                      // properties
};









 int HGraph::addPoint(float x,float y){
int x1=1;
int t1;
t1=__sync_fetch_and_add(pnpoints,x1);
points[2*t1].fpe=x;
points[2*t1+1].fpe=y;
return t1;
}

 int HGraph::addPoint(int x,int y){
int x1=1;
int t1;
t1=__sync_fetch_and_add(pnpoints,x1);
points[2*t1].ipe=x;
points[2*t1+1].ipe=y;
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
//fprintf(stderr,"%d %d %d %d\n ",point,edges[2*i].ipe,edges[2*i+1],1000);
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
int pos;
if(pos<nedges*3){
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
std::map<int,int>vertexmap1;
std::map<int,int>vertexmap;
int HGraph::optimizeone(){
return 0;
}
int HGraph::optimizetwo(){
return 0;
}
int HGraph::optimize(){
return 0;
}

int HGraph::deallocate(){
}
 void inline  HMIN(int *t1,int t2,int id,int &result){
if(*t1 > t2){
volatile int ch=*t1;
__sync_val_compare_and_swap(t1,ch,t2);
if(*t1<ch)result=1; 
}
//return result;
}
  int  HMAX(int *t1,int t2,int id,int &result){
if(*t1 <  t2){
  if (*t1 < t2){
volatile int ch=*t1;
__sync_val_compare_and_swap(t1,ch,t2);
result=1; 
}
}
return result;
}
 int HADD(int *t1,int t2){
int t4=__sync_fetch_and_add(t1,t2);
return t4;
}
 int HINCR(int *t1,int t2){
int t4=__sync_fetch_and_add(t1,t2);
return t4;
}
 int HDECR(int *t1,int t2){
int t4=__sync_fetch_and_sub(t1,t2);
return t4;
}
 int HSUB(int *t1,int t2){
int t4=__sync_fetch_and_sub(t1,t2);
return t4;
}
 int HAND(int *t1,int t2){
int t4=__sync_fetch_and_and(t1,t2);
return t4;
}

 int HOR(int *t1,int t2){
int t4=__sync_fetch_and_or(t1,t2);
return t4;
}
 int HXOR(int *t1,int t2){
int t4=__sync_fetch_and_xor(t1,t2);
return t4;
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
















#endif
