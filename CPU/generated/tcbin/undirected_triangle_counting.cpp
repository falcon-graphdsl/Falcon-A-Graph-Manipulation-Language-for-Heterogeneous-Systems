
 #include "undirected_triangle_counting.h"
 int   changed =0, hchanged ;


 void   TC ( int & p ,HGraph & graph ) 
 {

 struct dev_vec  coll ; 


 int falcft0=graph.index[p+1]-graph.index[p];
int falcft1=graph.index[p];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=graph.edges[ut0].ipe;
int ut2=graph.edges[ut0+1].ipe;
int t=ut1;
int t1;

 t1=t; 

 if( graph.points[t1].ipe<=graph.points[p].ipe  )
continue;

 add_coll_device(coll,t1); 

 }

 for (int   i =0;i<coll.size;i++) {

 int p2;

 p2=coll.D_Vec[i]; 

 int falcft3=graph.index[p2+1]-graph.index[p2];
int falcft4=graph.index[p2];
for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=graph.edges[ut3].ipe;
int ut5=graph.edges[ut3+1].ipe;
int t2=ut4;
int t1;

 t1=t2; 

 if( graph.points[t1].ipe<=graph.points[p2].ipe  )
continue;

 for (int   i =0;i<coll.size;i++) {

 int p3;

 p3=coll.D_Vec[i]; 

 if( graph.points[p3].ipe==graph.points[t1].ipe  )
HADD(&(changed),1);


 }

 }

 }

 coll.size=0; 

 }
int   main ( int   argc ,char    *  argv [ ] ) 
 { 
alloc_sync_array();
if(argc>2)FALC_THREADS=atoi(argv[2]);

 if( argc!=4  )
{

 printf("error: exec -t threads filename");


 return 1;

 }

 HGraph  hgraph ;

 

 hgraph.read2(argv[3]);

int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

  double   t1 , t2 ;


 t1=rtclock();


 changed=0; 

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int i=0;i<hgraph.npoints;i++)TC(i,hgraph);

 t2=rtclock();


 printf("\nTIME =%f\n",(t2-t1)*1000);


  int   count ;


 count=changed; 

 printf("\ntriangles=%d \n",count);


 return 1;

 }
