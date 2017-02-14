#ifndef HSET_H
#define HSET_H

#define STACK_SIZE (1024)
#include<stdio.h>
#include "Set.h"
#include<omp.h>
//extern void CudaTest(char *msg);
class HSet : public Set {
int *stack,*mcomp;
public:
void allocate(int n){
tot_size=n;
printf("**********************SIZE=%d\n",n);
parent=(int *)malloc(sizeof(int)*(n));
//size=(int *)malloc(sizeof(int)*(n));
for(int i=0;i<n;i++){
(parent[i])=i; 
//(size[i])=1;
}
}
int Findcompress(int n1);
int Findcompress1(int n1);

int Merge(int t1,int t2){
while(true){
      t1=Findcompress(t1);
      t2=Findcompress(t2);
      if (t1==t2)
        return 0;

      if ( t1 >t2)
        std::swap(t1, t2);
      if (__sync_bool_compare_and_swap(&(parent[t1]), t1, t2)) {
        return t2;
      }
}
   }

/*int Union(int t1,int t2){
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
   }*/
int LinkSet(int t1,int t2){
int x1;
if(t1<t2){

x1=__sync_bool_compare_and_swap(&(parent[t1]), t1, t2);
if(x1==t1 &&parent[t1]==t2)return t2;
return 0;
}
else {
x1=__sync_bool_compare_and_swap(&(parent[t2]), t2, t1);

if(x1==t2 &&parent[t2]==t1)return t1;
return 0;

}

}
int Union(int n1,int n2){
int t1=n1,t2=n2;
do {
do {
      t1=Findcompress(t1);
      t2=Findcompress(t2);
      if (t1==t2)
        return t1;
}while(t1!=parent[t1] || t2!=parent[t2]);
}while(!LinkSet(t1,t2));
      }

};


int HSet::Findcompress(int n1){
if(parent[n1]==n1)return n1;
 int rep=parent[n1];
int next;
int prev=-1;
while(parent[rep]!=rep){
 next=parent[rep];
if(prev>=0 && parent[prev]==rep)parent[prev]=next;
prev=rep;
rep=next;
}
return rep;
}

 int HSet::Findcompress1(int n1){
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
void hfindset (int i,HSet &set){
//set.parent[i]=set.FindSet(i);
set.parent[i]=set.Findcompress(i);
}

#endif
