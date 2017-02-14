#ifndef HSET_H
#define HSET_H

#define STACK_SIZE 8
#include<stdio.h>
#include "GSet.h"
extern void CudaTest(char *msg);
class HSet : public Set {
public:
//int Findcompress1(int);
void allocate(int n){
tot_size=n;
printf("**********************SIZE=%d\n",n);
parent=(int *)malloc(sizeof(int)*(n));
for(int i=0;i<n;i++){
(parent[i])=i; 
if(i<10)printf("IN ALLOC i=%d parent=%d \n",i,(parent[i]));
}
}
void cloneGPU(GSet &gset,int num){
cudaSetDevice(num);
gset.tot_size=tot_size;
gset.errflag=0;
fprintf(stderr,"SIZE OF STACK=%lu",sizeof(int)*tot_size*STACK_SIZE/(1000*1000));
if(cudaMalloc((void **)&(gset.parent),sizeof(int )*tot_size)!= cudaSuccess)printf ("allocating graph parent failed");
if(cudaMemcpy(gset.parent,parent,sizeof(int)*tot_size,cudaMemcpyHostToDevice)!=cudaSuccess)printf("SET COPY ERROR\n");
gset.lock=0;
cudaSetDevice(0);
}
int FindSet(int n1){

if(parent[n1]!=n1) parent[n1]=FindSet(parent[n1]);
else return parent[n1];
}
void  LinkSet(int t1,int t2){
if(t1>t2)parent[t2]=t1;
else {
      parent[t1]=t2;
 
}
}
 int Findcompress1(int n1){
if(parent[n1]==n1)return n1;
 int rep=n1;
int next;
int prev=-1;
while(parent[rep]!=rep){
 next=parent[rep];
if(prev>=0 && parent[prev]==rep)parent[prev]=rep;
prev=rep;
rep=next;
}
return rep;
}

/*void Union (int n1,int n2){
int t1,t2;
t1=FindSet(n1);
t2=FindSet(n2);
LinkSet(t1,t2);
}*/
int Union(int t1,int t2){
while(true){
      t1=Findcompress1(t1);
      t2=Findcompress1(t2);
      if (t1==t2)
        return 0;

      if ( t1 >t2)
        std::swap(t1, t2);
      if (__sync_bool_compare_and_swap(&(parent[t1]), t1, t2)) {
        return t2;
      }
}
   }
};
void GSet::allocate(int n){
}
void GSet::copytoCPU(HSet gset){
}
__device__ void GSet::Union (int n1,int n2){
//int idx = blockIdx.x * blockDim.x + threadIdx.x;
int t1,t2;
t1=n1;t2=n2;
do {
   do {
 //    t1=FindSet(t1);
//t2=FindSet(t2);
t1=Findcompress(t1);
t2=Findcompress(t2);
       if(t1==t2) return ;
  }while(t1!=parent[t1]|| t2!=parent[t2]);
}while(!(LinkSet(t1,t2)));
}
__device__ int GSet::Findcompress(int n1){
if(parent[n1]==n1)return n1;
int rep=parent[n1];
int prev=-1;
while(parent[rep]!=rep){
int next=parent[rep];
//if(prev>=0 && parent[prev]==rep)parent[prev]=rep;
if(prev>=0 && parent[prev]==rep)parent[prev]=next;
prev=rep;
rep=next;
}
return rep;
}
__device__ int GSet::LinkSet(int t1,int t2){
int x1;
if(t1<t2){
x1=atomicCAS(&(parent[t1]),t1,t2);
if(x1==t1&& parent[t1]==t2)return t2;
 return 0;
}
else {
x1=atomicCAS(&(parent[t2]),t2,t1);
if(x1==t2&&parent[t2]==t1)return t1;
 return 0;
}
}
__device__ int   GSet::LinkMerge(int t1,int t2){
//int n1=parent[t1];
while(true){
parent[t1]=Findcompress(t1);
parent[t2]=Findcompress(t2);
if(parent[t1]==parent[t2])return 0;
if(parent[t1]>parent[t2]){
if(atomicCAS(&parent[t2],t2,t1))return t1;

}
else {
if(atomicCAS(&parent[t1],t1,t2))return t2;

}

}
}
__global__ void findset(GSet set){
int id= blockIdx.x * blockDim.x + threadIdx.x;
if( id < set.tot_size){
set.parent[id]=set.Findcompress(id);
}
}
__global__ void findset(GSet set,int FALCX){
int id= blockIdx.x * blockDim.x + threadIdx.x+FALCX;
if( id < set.tot_size){
set.parent[id]=set.Findcompress(id);
}
}
void hfindset (int i,HSet &set){
set.parent[i]=set.Findcompress1(i);
//set.parent[i]=set.Findcompress(i);
}


#endif
