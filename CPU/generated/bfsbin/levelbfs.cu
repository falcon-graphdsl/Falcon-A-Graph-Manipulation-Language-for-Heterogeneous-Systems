
 #include "levelbfs.h"
 int   changed =0, hchanged ;


  int   lev =0;


 void   reset ( int & t ,HGraph & graph ) 
 {

 ((struct struct_graph  *)(graph.extra))->dist[t]=1234567890; 

 }
void   BFS ( int & p ,HGraph & graph ,int   lev ) 
 {
 if( ((struct struct_graph  *)(graph.extra))->dist[p]==lev ){

  int   ch ;


 int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
if( ((struct struct_graph  *)(graph.extra))->dist[ut1]>(((struct struct_graph  *)(graph.extra))->dist[p]+1)  )//END
{

 ((struct struct_graph  *)(graph.extra))->dist[ut1]=((struct struct_graph  *)(graph.extra))->dist[p]+1; 

 changed=1; 

 }

 }

 }

}
int   main ( int   argc ,char    *  name [ ] ) 
 { 
alloc_sync_array();//0


 HGraph  graph ;

 

 graph.read2(name[1]);

int hostgraph=0;
alloc_extra_graph(graph,hostgraph,graph.npoints);
; 

  double   t1 , t2 ;


 t1=rtclock();


 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)reset(i,graph);//xx

 //ARRREF
((struct struct_graph  *)(graph.extra))->dist[0]=0; 

 
 while(1)  { 

 changed=0; 

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)BFS(i,graph,lev);//xx

 if( changed==0  )//END
break;

 lev++; 

 }

 t2=rtclock();


 printf("\nTIME =%f\n",(t2-t1)*1000);


  int   maxdist =0;


 for (int   i =0;i<graph.npoints;i++) {

 if( //ARRREF
maxdist<((struct struct_graph  *)(graph.extra))->dist[i]  )//END
maxdist=//ARRREF
((struct struct_graph  *)(graph.extra))->dist[i]; 

 }

 printf("\nMAXDIST=%d \n",maxdist);


 return ;

 }
