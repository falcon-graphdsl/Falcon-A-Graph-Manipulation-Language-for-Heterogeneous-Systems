#ifndef GSET_H
#define GSET_H
#include "Set.h"
class HSet;
class GSet: public Set {
public:
int lock;
int errflag;
int *lock1,*stack;
void allocate(int n);

void copytoCPU(HSet gset);

__device__ int  FindSet(int n1);
//__device__ int  FindSet(GSet set,int n1);
__device__ int  Findcompress(int n1);
__device__ int LinkMerge(int t1,int t2);

__device__ int LinkSet(int t1,int t2);
__device__ void Union (int n1,int n2);

};
#endif
