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
int MORPH_FACT=1;
struct GColl{
int *size,*ptr;
};
int writelock[8388608];
class HGraph: public Graph{
public:
int maxnpoints,pnpoints,maxnedges,pnedges;
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
int acyclic();
int addEdge(int pos,int src,int dst,int weight);
int addEdge(int src,int dst,int weight);
int sortPoints();
void (*extra_alloc)(HGraph &);//needed for read,copy,allocate,deallocate extra  							                      // properties
};
int HGraph::init(){
return 0;
}
int HGraph::getPoint(int pos){
if(pos<npoints)return points[pos].ipe;
return -1;
}
int HGraph::addEdge(int src,int dst,int weight){
int pos=index[src]+outtotal[src];
edges[2*pos].ipe=dst;
edges[2*pos+1].ipe=weight;
outtotal[src]++;
return -1;
}
int HGraph::addEdge(int pos,int src,int dst,int weight){
if(pos<nedges){
pos=pos+index[src];
if(pos<index[src]+outtotal[src]*3){
edges[2*pos].ipe=dst;
edges[2*pos+1].ipe=weight;
edges[3*pos].ipe=src;
return 1;
}
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
return 0;
}
int HGraph::printNodes(char *filename,int dims){
FILE *fp=fopen(filename,"w+");
for(int i=0;i<npoints;i++){
fprintf(fp,"%d %f %f \n",i,points[2*i].fpe,points[2*i+1].fpe);
}
return 0;
}

//int HGraph::acyclic(HGraph &graph){
//}
int HGraph::acyclic( ){
HGraph graph;
//static int cnt;
int gindex=0,gtotal=0;
graph.npoints=npoints;
graph.maxnpoints=npoints;
graph.index=(int  *)malloc(sizeof(int)*(npoints+1));
  gindex=0; gtotal=0;

graph.points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*1);
      graph.edges=(union float_int *)malloc(sizeof(union float_int)*(nedges+1)*2);
for(int i=0;i<(npoints);i++){
graph.points[i].ipe=i;
}
for(int i=0;i<npoints;i++){
int t1=index[i];
int t2=index[i+1]-index[i];
for(int t3=0;t3<t2;t3++){
int p1=edges[2*(t3+t1)].ipe;//dest point
int tt1=index[p1];//dest index
int tt2=index[p1+1]-index[p1];//dest total outedgesa
int flag;
flag=0;
int min;
min=edges[2*(t3+t1)+1].ipe;
for(int tt3=0;tt3<tt2;tt3++){
//if(((edges[2*(tt3+tt1)].ipe==i )&& (edges[2*(tt3+tt1)+1].ipe!=edges[2*(t3+t1)+1].ipe)))printf(" Error %d %d %d \n %d %d %d \n", i,edges[2*(t3+t1)].ipe,edges[2*(t3+t1)+1].ipe,p1,edges[2*(tt3+tt1)].ipe,edges[2*(tt3+tt1)+1].ipe);
if(((edges[2*(tt3+tt1)].ipe==i )/*&& (edges[2*(tt3+tt1)+1].ipe==edges[2*(t3+t1)+1].ipe)*/)){
if(min >edges[2*(tt3+tt1)+1].ipe){
min=edges[2*(tt3+tt1)+1].ipe;
}
if(i>p1){
flag=1;
}
//printf("FLAG=%d \n",flag);
break;
}
}

if(flag==0){
graph.edges[2*gtotal].ipe=p1;
graph.edges[2*gtotal+1].ipe=min;
gtotal++;
}
}
graph.index[i]=gindex;
//if(i==0)graph.nbrs[i].total=gtotal;
//else graph.nbrs[i].total=gtotal-graph.nbrs[i].index+1;
gindex=gtotal;
}
graph.index[npoints]=gtotal;
graph.nedges=gtotal;
graph.maxnedges=gtotal;
index=graph.index;
edges=graph.edges;
nedges=gtotal;
points=graph.points;
printf("ACYCLIC OVER np=%d nedeges=%d index[np]=%d ind[np-1]=%d ind[0]=%d\n",npoints,nedges,index[npoints],index[npoints-1],index[1]);
return 0;
}

int HGraph::Check(){
return 0;
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

int HGraph::readMorph(char *file){
    if(!strcmp(file+strlen(file)-3,"txt")){
readEdges(file);
return 1;
}
	std::ifstream cfile;
	cfile.open(file);
MORPH_FACT=3;
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
//int *count;
int max=0,min=99999;
unsigned total=0,prevtotal=0;
edims=2;
	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
index=(int *)malloc(sizeof(int)*(npoints+1));
outtotal=(int *)malloc(sizeof(int)*(npoints+1));
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges+4)*edims*3);
 //index[i]=le64toh(outIdx[i - 1]) + 1;
 //total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);
for(int i=0;i<(npoints);i++){
points[i].ipe=i;
if(i>0){
if(i >1) index[i]=index[i-1]+prevtotal*3;
//if(i<10)fprintf(stderr,"i=%d index=%d prevtotal=%d outidx[i-1]=%d outidx[i]=%d\n",i,index[i],total,le64toh(outIdx[i - 1]),le64toh(outIdx[i]) - le64toh(outIdx[i - 1])); 
 total=le64toh(outIdx[i]) - le64toh(outIdx[i - 1]);
prevtotal=total;
outtotal[i]=total;
}
else {
index[0]=1;
 total=le64toh(outIdx[0]);
prevtotal=total;
outtotal[0]=total;
}
for (unsigned jj = 0; jj <total; ++jj) {
unsigned edgeindex;
if(i>0)edgeindex= le64toh(outIdx[i - 1])+1 + jj;
else edgeindex=1+jj;
unsigned eindex = index[i] + jj;

/*if(edgeindex > nedges*3){
printf("%d %d %d\n",nedges,edgeindex,i);
exit(0);

}*/
unsigned dst =le32toh(outs[edgeindex - 1]);
edges[2*eindex].ipe=dst;
unsigned weight=edgeData[edgeindex - 1];
edges[2*eindex+1].ipe=weight;

//if(edgeindex<40)printf("%d %d %d %d\n",eindex,index[i]+jj,dst,weight);
}
}
index[npoints]=index[npoints-1]+total*3;
printf("ZERO COUNT=%d %d\n ",zerocount,index[npoints]);
//for(int i=0;i<40;i++)printf("%d %d \n",edges[2*i].ipe,edges[2*i+1].ipe);
cfile.close();
printf("READ OVER");
printf("MAX=%d MIN=%d\n ",max,min);
return 1;
}





//__device__ __constant__ union float_int *edges1;
//__device__ __constant__ int *total1,*index1;
int HGraph::read(char *file){
return 1;
}

int HGraph::read2(char *file){
    if(!strcmp(file+strlen(file)-3,"txt")){
readEdges(file);
return 1;
}
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
//int *count;
int max=0,min=99999,total=0;
edims=2;
	printf("npoints=%d, nedges=%d.\n", (npoints), (nedges));
index=(int *)malloc(sizeof(int)*(npoints+1));
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*pdims);
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims);
for(int i=0;i<npoints;i++){
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
//edges[2*edgeindex+1].ipe=1;
}
}
index[npoints]=index[npoints-1]+total;
printf("ZERO COUNT=%d\n ",zerocount);
cfile.close();
printf("READ OVER");
printf("MAX=%d MIN=%d\n ",max,min);
return 0;
}


/*
The readEdges() function accepts input graph in text format.
syntax
npoints and nedges in line1
then edges in the order src dest weight, sorted by src id.
one edge per line.*/

int HGraph::readEdges(char *filename){                                                                                                                                                         
FILE *fp=fopen(filename,"r");
fscanf(fp,"%d%d",&npoints,&nedges);
nedges++;
int src,dst,weight;
edims=2;
points=(union float_int *)malloc(sizeof(union float_int )*(npoints+1)*1);
index=(int *)malloc(sizeof(int )*(npoints+1));
      edges=(union float_int *)malloc(sizeof(union float_int)*(nedges)*edims); 
int currsrc=-1,ttindex=1;
//int currindex=0;
for(int i=0;i<(npoints);i++){ points[i].ipe=i;}
for(int i=0;i<nedges;i++){
fscanf(fp,"%d%d%d",&src,&dst,&weight);
if(src!=currsrc){
for(int kk=currsrc+1;kk<src;kk++){ //if some points/nodes does not have any edges
index[kk]=ttindex;
}
currsrc=src;
index[src]=ttindex;
//currindex=ttindex;
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
for(int i=currsrc+1;i<=npoints;i++)index[i]=ttindex;
fclose(fp);
return 0;
}


int HGraph::optimizeone(){
return 0;
}
int HGraph::optimizetwo(){
return 0;


}
int HGraph::optimize(){
return 0;
}
int HGraph::sortPoints(){




return 0;
}

int HGraph::cloneMorphGPU(GGraph &graph,int num){
return 1;
}








int HGraph::cloneGPU(GGraph &graph,int num){
cudaSetDevice(num);
graph.npoints=npoints;
graph.maxnpoints=maxnpoints;
cudaMalloc(&(graph.pnpoints),sizeof(int));
cudaMemcpy(graph.pnpoints,&(npoints),sizeof(int),cudaMemcpyHostToDevice);
graph.nedges=nedges;
graph.maxnedges=maxnedges;
cudaMalloc(&(graph.pnedges),sizeof(int));
if(cudaMemcpy(graph.pnedges,&(pnedges),sizeof(int),cudaMemcpyHostToDevice)!=cudaSuccess)printf("error line 533 HGraph\n");
graph.pdims=pdims;
graph.edims=edims;
if(cudaMalloc((void **)&(graph.points),(npoints+1)*sizeof(union float_int)*(pdims))!=cudaSuccess)printf("ALLOC ERRRR\n");
if(cudaMalloc((void **)&(graph.index),(npoints+1)*sizeof(int))!=cudaSuccess)printf("ALLOC ERRRR\n");
cudaMemcpy(graph.index,index,sizeof(int)*(npoints+1),cudaMemcpyHostToDevice);
//printf("YY1\n");
if(cudaMalloc((void **)&(graph.edges),(nedges+1)*MORPH_FACT*sizeof(union float_int)*(edims))!=cudaSuccess)printf("ALLOC EDGE ERROR\n");
if(cudaMemcpy(graph.points,points,(npoints+1)*sizeof(union float_int)*(pdims),cudaMemcpyHostToDevice)!=cudaSuccess)printf("error line 541 HGraph");
if(cudaMemcpy(graph.edges,edges,(nedges+1)*MORPH_FACT*sizeof(union float_int)*(edims),cudaMemcpyHostToDevice)!=cudaSuccess)printf("error line 542  HGraph");
if(total!=NULL){
if(cudaMalloc((void **)&(graph.total),(npoints+1)*sizeof(int))!=cudaSuccess)printf("ALLOC ERRRR\n");
if(cudaMemcpy(graph.total,total,(npoints+1)*sizeof(int)*1,cudaMemcpyHostToDevice)!=cudaSuccess)printf("error line 545 HGraph");
}
if(outtotal!=NULL){
if(cudaMalloc((void **)&(graph.outtotal),(npoints+1)*sizeof(int))!=cudaSuccess)printf("ALLOC ERRRR\n");
if(cudaMemcpy(graph.outtotal,outtotal,sizeof(int)*(npoints+1),cudaMemcpyHostToDevice)!=cudaSuccess)printf("OUTTOTAL COPY ERROR\n");
}

cudaDeviceSynchronize();
cudaSetDevice(0);
return 1;
}
int HGraph::copytoGPU(GGraph &graph){

cudaMemcpy(graph.index,index,sizeof(int)*(npoints+1),cudaMemcpyHostToDevice);
if(cudaMemcpy(graph.edges,edges,(nedges+1)*MORPH_FACT*sizeof(union float_int)*(edims),cudaMemcpyHostToDevice)!=cudaSuccess)printf("error line 542  HGraph");
if(outtotal!=NULL){
cudaMemcpy(graph.outtotal,outtotal,sizeof(int)*(npoints+1),cudaMemcpyHostToDevice);
}
return 0;
}
int HGraph::copyfromGPU(GGraph &graph){


return 0;
}
int HGraph::deallocate(){
return 0;
}
int GGraph::cloneCPU(HGraph &hgraph){
return 0;

}
int GGraph::copytoCPU(HGraph &hgraph){
return 0;
}
int GGraph::copyfromCPU(HGraph &hgraph){
return 0;

}

int GGraph::allocate(){

return 0;

}
int GGraph::deallocate(){
return 0;
}
__device__ int GGraph::getEdgeWeight(int t1,int t2){
return edges[(index[t1]+t2)*2+1].ipe;

}
__device__ __forceinline__ void MIN( int *t1,int t2,int &result){
//__device__   void  MIN(int *t1,int t2,int &result){
int ch= *t1;
if(ch > t2){
atomicMin(t1,t2);
result=1;
}
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
int ch;
if(*t1 > t2){
ch=*t1;
__sync_val_compare_and_swap(t1,ch,t2);
result=1; 
}
return result;
}
 void copyaftermorph(GGraph graph){

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
 //  unsigned int oldc;
    __threadfence();
    if (threadIdx.x == 0)
    {
//        oldc =
 atomicInc((unsigned int *)&barrcount, gridDim.x);
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
