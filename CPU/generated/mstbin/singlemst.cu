
 #include "singlemst.h"


  int   hchanged , changed ;


 void   reset ( int & p ,HGraph & graph ,/*SET*/HSet &set ) 
 {

 ((struct struct_graph  *)(graph.extra))->minppty[p].weight=99999999; 

 ((struct struct_graph  *)(graph.extra))->minedge[p]=99999999; 

 ((struct struct_graph  *)(graph.extra))->minppty[p].set=99999999; 
 ((struct struct_graph  *)(graph.extra))->minppty[p].src=99999999; 
 ((struct struct_graph  *)(graph.extra))->minppty[p].dst=99999999; 

 ((struct struct_graph  *)(graph.extra))->minppty[p].lock=0; 

 }
void   minset ( int & p ,HGraph & graph ,/*SET*/HSet &set ) 
 {

  int   ch ;


 int t1;

 int t2;

 int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
t1=set.parent[p]; 

 t2=set.parent[ut1]; 

 if( t1!=t2  )//END
{

 HMIN(&(((struct struct_graph  *)(graph.extra))->minppty[t1].weight),ut2,t1,ch);


 HMIN(&(((struct struct_graph  *)(graph.extra))->minppty[t2].weight),ut2,t2,ch);


 }

 }

 }
void   Minedge ( int & p ,HGraph & graph ,/*SET*/HSet &set ) 
 {

 int t1;

 int t2;

  int   t3 ;


 int e;

 int falcft3=graph.index[p+1]-graph.index[p];
int falcft4=graph.index[p];
for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
t1=set.parent[p]; 

 t2=set.parent[ut4]; 

 t3=ut5; 

 if( t1!=t2  )//END
{

 if( t3==((struct struct_graph  *)(graph.extra))->minppty[t1].weight  )//END
{

  int   unnikri =0;


 if(__sync_val_compare_and_swap( &(((struct struct_graph  *)(graph.extra))->minppty[t1].lock),0,1)==0){

 e=ut3/2; 

 ((struct struct_graph  *)(graph.extra))->mark[e]=1; 

 ((struct struct_graph  *)(graph.extra))->minppty[t1].src=p; 

 ((struct struct_graph  *)(graph.extra))->minppty[t1].dst=ut4; 

 ((struct struct_graph  *)(graph.extra))->minppty[t1].weight=t3; 

 ((struct struct_graph  *)(graph.extra))->minppty[t1].set=t2; 

 }

 }

 if( t3==((struct struct_graph  *)(graph.extra))->minppty[t2].weight  )//END
{

 if(__sync_val_compare_and_swap( &(((struct struct_graph  *)(graph.extra))->minppty[t2].lock),0,1)==0){

 e=ut3/2; 

 ((struct struct_graph  *)(graph.extra))->mark[e]=1; 

 ((struct struct_graph  *)(graph.extra))->minppty[t2].src=p; 

 ((struct struct_graph  *)(graph.extra))->minppty[t2].dst=ut4; 

 ((struct struct_graph  *)(graph.extra))->minppty[t2].weight=t3; 

 ((struct struct_graph  *)(graph.extra))->minppty[t2].set=t1; 

 }

 }

 }

 }

 }
void   mstunion ( int & p ,HGraph & graph ,/*SET*/HSet &set ) 
 {

 int t1;

 int t2;

  int   t3 , t4 ;


 t1=set.parent[p]; 

 t2=((struct struct_graph  *)(graph.extra))->minppty[t1].set; 

 t3=((struct struct_graph  *)(graph.extra))->minppty[t1].lock; 

 t4=((struct struct_graph  *)(graph.extra))->minppty[t2].lock; 

 if( t1!=t2&&t2!=99999999  )//END
{

 set.Union(t1,t2); 

 changed=1; 

 }

 }
void   initmark ( int & e ,HGraph & graph ) 
 {

 ((struct struct_graph  *)(graph.extra))->mark[e]=999999999; 

 }
int   main ( int   argc ,char    *  argv [ ] ) 
 { 


 HGraph  graph ;

 graph.read2(argv[1]);

int hostgraph=0;
alloc_extra_graph(graph,hostgraph,graph.npoints);
; 

int cnt;
 HSet set;
set.allocate(graph.npoints);


 hchanged=0; 

 #pragma omp parallel for num_threads(12)
 for(int i=0;i<graph.nedges;i++)initmark(i,graph);

 
 while(1)  { 
++cnt;
 changed=0; 

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)reset(i,graph,set);//xx

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)minset(i,graph,set);//xx

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)Minedge(i,graph,set);//xx

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)mstunion(i,graph,set);//xx

 if( changed==0  )//END
break;
if(cnt==200){
printf("CNT=%d\n",cnt);
break;
}
 }
  unsigned  long  int   mst =0;
for(int i=0;i<graph.nedges;i++){

if( ((struct struct_graph  *)(graph.extra))->mark[i]==1){
mst+=1;
}

}



printf("mst cost=%lu",mst);


 }
