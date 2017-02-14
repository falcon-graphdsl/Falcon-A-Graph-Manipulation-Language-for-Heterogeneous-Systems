
 #include "cpusssp.h"
 int   changed =0, hchanged =0;


 void   relaxgraph ( int & p ,HGraph & unnigraph ) 
 {
 if( ((struct struct_graph  *)(unnigraph.extra))->updated[p]==true ){

 ((struct struct_graph  *)(unnigraph.extra))->updated[p]=false; 

 int falcft0=unnigraph.index[p+1]-unnigraph.index[p];
int falcft1=unnigraph.index[p];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=unnigraph.edges[ut0].ipe;
int ut2=unnigraph.edges[ut0+1].ipe;
HMIN(&(((struct struct_graph  *)(unnigraph.extra))->dist[ut1]),((struct struct_graph  *)(unnigraph.extra))->dist[p]+ut2,ut1,changed);
}
}

}
void   reset ( int & t ,HGraph & graph ) 
 {

 ((struct struct_graph  *)(graph.extra))->dist[t]=1234567890; 

 ((struct struct_graph  *)(graph.extra))->olddist[t]=1234567890; 

 ((struct struct_graph  *)(graph.extra))->updated[t]=false; 

 }
void   reset1 ( int & t ,HGraph & graph ) 
 {

 if( ((struct struct_graph  *)(graph.extra))->dist[t]<((struct struct_graph  *)(graph.extra))->olddist[t]  )//END
((struct struct_graph  *)(graph.extra))->updated[t]=true; 

 ((struct struct_graph  *)(graph.extra))->olddist[t]=((struct struct_graph  *)(graph.extra))->dist[t]; 

 }
void   SSSP ( char    *  name ) 
 {

 HGraph  graph ;

 

 

 

 graph.read2(name);

int hostgraph=0;
alloc_extra_graph(graph,hostgraph,graph.npoints);
; 

  double   t1 , t2 ;


 t1=rtclock();


 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)reset(i,graph);//xx

 //ARRREF
((struct struct_graph  *)(graph.extra))->updated[0]=true; 

 //ARRREF
((struct struct_graph  *)(graph.extra))->dist[0]=0; 

 
 while(1)  { 

 changed=0; 

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)relaxgraph(i,graph);//xx

 if( changed==0  )//END
break;

 #pragma omp parallel for   num_threads(12)
for(int i=0;i<graph.npoints;i++)reset1(i,graph);//xx

 }

 t2=rtclock();


 printf("\nTIME =%f",(t2-t1)*1000);


  int   maxdist =0;


 for (int   i =0;i<graph.npoints;i++) {

 if( //ARRREF
maxdist<((struct struct_graph  *)(graph.extra))->dist[i]  )//END
maxdist=//ARRREF
((struct struct_graph  *)(graph.extra))->dist[i]; 

 }

 printf("\nMAXDIST=%d\n",maxdist);


 return ;

 }
int   main ( int   argc ,char    *  argv [ ] ) 
 { 
alloc_sync_array();//0


 SSSP(argv[1]);


 }
