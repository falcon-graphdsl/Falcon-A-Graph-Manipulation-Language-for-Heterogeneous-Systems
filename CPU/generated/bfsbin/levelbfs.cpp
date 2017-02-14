
 #include "levelbfs.h"
 int   changed =0, hchanged ;


  int   lev =0;


 void   reset ( int & t ,HGraph & graph ) 
 {

 ((struct struct_graph  *)(graph.extra))->dist[t]=1234567890; 

 }
void   BFS ( int & p ,HGraph & graph ,int   lev ) 
 {
 if( ((struct struct_graph  *)(graph.extra))->/*4*/dist[p]==lev ){

  int   ch ;


 int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
int t=ut1;
if( ((struct struct_graph  *)(graph.extra))->dist[ut1]>(((struct struct_graph  *)(graph.extra))->dist[p]+1)  )
{

 ((struct struct_graph  *)(graph.extra))->dist[ut1]=((struct struct_graph  *)(graph.extra))->dist[p]+1; 

 changed=1; 

 }

 }

 }

}
int   main ( int   argc ,char    *  argv [ ] ) 
 { 
alloc_sync_array();
if(argc>2)FALC_THREADS=atoi(argv[2]);

 if( argc!=4  )
{

 printf("exec -t threads file");


 return 1;

 }

 HGraph  graph ;

 

 graph.read2(argv[3]);

int hostgraph=0;
alloc_extra_graph(graph,hostgraph,graph.npoints);
; 

  double   t1 , t2 ;


 t1=rtclock();


 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)reset(i,graph);

 ((struct struct_graph  *)(graph.extra))->dist[0]=0; 

 
 while(1)  { 

 changed=0; 

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<graph.npoints;i++)BFS(i,graph,lev);

 if( changed==0  )
break;

 lev++; 

 }

 t2=rtclock();


 printf("\nTIME =%f\n",(t2-t1)*1000);


 ((struct struct_graph  *)(graph.extra))->dist=((struct struct_graph  *)(graph.extra))->dist; 

  int   maxdist =0;


 for (int   i =0;i<graph.npoints;i++) {

 if( maxdist<((struct struct_graph  *)(graph.extra))->dist[i]  )
maxdist=((struct struct_graph  *)(graph.extra))->dist[i]; 

 }

 printf("\nMAXDIST=%d \n",maxdist);


 return ;

 }
