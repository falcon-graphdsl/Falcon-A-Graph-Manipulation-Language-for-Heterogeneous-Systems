#include "DMRmay15global.h"
 #include "HGraph.h"
 //#include "HSet.h"
#include"hvec.h"
 #include<sys/time.h>
//#include </usr/local/cuda/include/cuda.h>
// #include </usr/local/cuda/include/cuda_runtime_api.h>
#include<unistd.h>
//#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>
struct struct_hgraph { 
int ntriangle;
int maxntriangle;
 int pntriangle;
struct node  *triangle ;//has to given size of property type
int *owner;
};
void read_hgraph_pptysize(HGraph &hgraph){
printf("enter size of propery triangle of hgraph ");
scanf("%d",&(((struct struct_hgraph  *)(hgraph.extra))->ntriangle));
}
void alloc_extra_hgraph(HGraph &hgraph,int flag) {
 if(flag==0)hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph )) ;
((struct struct_hgraph  *)hgraph.extra)->triangle=(struct node *)malloc(sizeof(struct node) * ((struct struct_hgraph  *)(hgraph.extra))->ntriangle*15) ;
((struct struct_hgraph  *)hgraph.extra)->owner=(int *)malloc(sizeof(int) * ((struct struct_hgraph  *)(hgraph.extra))->ntriangle*15) ;
}
void hgraph_realloc(HGraph &hgraph){
 struct struct_hgraph  temp;
temp=*((struct struct_hgraph  *)(hgraph.extra));
if(temp.ntriangle*2>temp.maxntriangle){
void *t1=temp.triangle;
temp.triangle=(struct node *)malloc(sizeof(struct node)*temp.ntriangle*2);
memcpy(temp.triangle,t1,sizeof(struct node)*temp.ntriangle);
free(t1);temp.maxntriangle=temp.ntriangle*2;
}

}
