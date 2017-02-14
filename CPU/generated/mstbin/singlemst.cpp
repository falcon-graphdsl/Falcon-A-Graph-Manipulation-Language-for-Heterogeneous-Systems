
 #include "singlemst.h"


  int   hchanged , changed ;


 void   resetlock ( int & p ,HGraph & graph ,/*SET*/HSet set ) 
 {

 ((struct struct_graph  *)(graph.extra))->minppty[p].lock=0; 

 }
void   reset ( int & p ,HGraph & graph ,/*SET*/HSet set ) 
 {

 ((struct struct_graph  *)(graph.extra))->minppty[p].weight=99999999; 

 ((struct struct_graph  *)(graph.extra))->minedge[p]=99999999; 

 ((struct struct_graph  *)(graph.extra))->minppty[p].set=99999999; 

 ((struct struct_graph  *)(graph.extra))->minppty[p].lock=0; 

 }
void   minset ( int & p ,HGraph & graph ,/*SET*/HSet set ) 
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
int t=ut1;
t1=set.parent[p]; 

 t2=set.parent[ut1]; 

 if( t1!=t2  )
{

 HMIN(&(((struct struct_graph  *)(graph.extra))->minppty[t1].weight),ut2,t1,ch);


 HMIN(&(((struct struct_graph  *)(graph.extra))->minppty[t2].weight),ut2,t2,ch);


 }

 }

 }
void   Minedge ( int & p ,HGraph & graph ,/*SET*/HSet set ) 
 {

 int t1;

 int t2;

 int tt2;

  int   t3 ;


 int e;

 int falcft3=graph.index[p+1]-graph.index[p];
int falcft4=graph.index[p];
for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
int t=ut4;
t1=set.parent[p]; 

 t2=set.parent[ut4]; 

 if( graph.points[t1].ipe>graph.points[t2].ipe  )
{

 tt2=t2; 

 t2=t1; 

 t1=tt2; 

 }

 t3=ut5; 

 if( t1!=t2  )
{

 if( t3==((struct struct_graph  *)(graph.extra))->minppty[t1].weight  )
{

 if(__sync_val_compare_and_swap( &(((struct struct_graph  *)(graph.extra))->minppty[t1].lock),0,1)==0){

 e=ut3/2; 

 ((struct struct_graph  *)(graph.extra))->mark[e]=t3; 

 ((struct struct_graph  *)(graph.extra))->minppty[t1].src=p; 

 ((struct struct_graph  *)(graph.extra))->minppty[t1].dst=t; 

 ((struct struct_graph  *)(graph.extra))->minppty[t1].weight=t3; 

 ((struct struct_graph  *)(graph.extra))->minppty[t1].set=t2; 

 }

 }

 }

 }

 }
void   mstunion ( int & p ,HGraph & graph ,/*SET*/HSet set ) 
 {

 int t1;

 int t2;

  int   t3 , t4 ;


 t1=set.parent[p]; 

 t2=((struct struct_graph  *)(graph.extra))->minppty[t1].set; 

 t3=((struct struct_graph  *)(graph.extra))->minppty[t1].lock; 

 t4=((struct struct_graph  *)(graph.extra))->minppty[t2].lock; 

 if( t1!=t2&&t2!=99999999&&t3==1  )
{

 //Currn_fun_name =mstunion
set.Union(t1,t2); 

 changed=1; 

 }

 }
void   initmark ( int & e ,HGraph & graph ) 
 {

 ((struct struct_graph  *)(graph.extra))->mark[e]=0; 

 }
int   main ( int   argc ,char    *  argv [ ] ) 
 { 
alloc_sync_array();
if(argc>2)FALC_THREADS=atoi(argv[2]);

 if( argc!=4  )
{

 printf("error:- exec filename");


 return 1;

 }

 HGraph  graph ;

 

 

 

 graph.read2(argv[3]);

int hostgraph=0;
alloc_extra_graph(graph,hostgraph,graph.npoints);
; 

 HSet set;
set.allocate(graph.npoints);

 hchanged=0; 

  double   rt1 =rtclock();


 #pragma omp parallel for num_threads(FALC_THREADS)
 for(int i=0;i<graph.nedges;i++)initmark(i,graph);

 
 while(1)  { 

 changed=0; 

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)reset(i,graph,set);

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)minset(i,graph,set);

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)Minedge(i,graph,set);

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)mstunion(i,graph,set);
#pragma omp parallel for num_threads(FALC_THREADS) 
 for(int i=0;i<set.tot_size;i++)hfindset(i, set);


 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)resetlock(i,graph,set);

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)Minedge(i,graph,set);

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)mstunion(i,graph,set);
#pragma omp parallel for num_threads(FALC_THREADS) 
 for(int i=0;i<set.tot_size;i++)hfindset(i, set);


 if( changed==0  )
break;

 }

  double   rt2 =rtclock();


 printf("TIME=%f \n",(rt2-rt1)*1000);


  long  int   mst =0;


 for (int   i =0;i<graph.nedges;i++) {

 if( ((struct struct_graph  *)(graph.extra))->mark[i]  )
mst=mst+((struct struct_graph  *)(graph.extra))->mark[i]; 

 }

 printf("mst cost=%lu\n",mst);


 }
