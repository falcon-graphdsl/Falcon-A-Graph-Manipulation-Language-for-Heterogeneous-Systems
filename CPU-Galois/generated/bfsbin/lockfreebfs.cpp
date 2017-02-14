
 #include "lockfreebfs.h"
HGraph  hgraph ;

 

 Galois::InsertBag<struct node>  pred ;
;


 template<typename Pusher0 >
void   relaxEdge ( int & p ,HGraph & hgraph ,int & p1 ,int   weight ,Pusher0 &pred ) 
 {

  int   changed =0;


  int   ddata =((struct struct_hgraph  *)(hgraph.extra))->dist[p];


  int   newdist =((struct struct_hgraph  *)(hgraph.extra))->dist[p1]+weight;


 if( newdist<(((struct struct_hgraph  *)(hgraph.extra))->dist[p])  )
{

 ((struct struct_hgraph  *)(hgraph.extra))->dist[p]=newdist; 

 changed=1; 

   struct node   tt1 ;


 tt1.w=newdist; 

 tt1.n1=p; 

 pred.push(tt1); 

 }

 }
template<typename Pusher0 >
void   relaxNode1 ( struct node  req ,HGraph & hgraph ,Pusher0 &pred ) 
 {

 int p1;

   struct node   temp ;


 temp=req; 

 p1=temp.n1; 

 int falcft0=hgraph.index[p1+1]-hgraph.index[p1];
int falcft1=hgraph.index[p1];
for(int falcft2=0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=hgraph.edges[ut0].ipe;
int ut2=hgraph.edges[ut0+1].ipe;
int t=ut1;
 int   weight =1;


 relaxEdge(ut1,hgraph,p1,weight,pred);

 }

 }
struct Process1 {
 Process1(){}
 void operator()(struct node &req){
  relaxNode1(req,hgraph,pred);
}
};

void falctfun0(int &id,HGraph &hgraph){
  if (id < hgraph.npoints) {
 ((struct struct_hgraph  *)(hgraph.extra))->dist[id]=1234567890;
}
}
int   main ( int   argc ,char    *  argv [ ] ) 
 { LonestarStart(argc,argv,name,desc,url);
if(argc>2)FALC_THREADS=atoi(argv[2]);

 if( argc!=4  )
{

 printf("exec -t theads file");


 return 1;

 }

 

 int p;

 hgraph.read2(argv[3]);

int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 #pragma omp parallel for   num_threads(FALC_THREADS)
for(int id=0;id<hgraph.npoints;id++)falctfun0( id,hgraph);

 p=0; 
 ((struct struct_hgraph  *)(hgraph.extra))->dist[p]=0; 

 int falcft3=hgraph.index[p+1]-hgraph.index[p];
int falcft4=hgraph.index[p];
for(int falcft5=0;falcft5<falcft3;falcft5++){
int ut3=2*(falcft4+falcft5);
 int ut4=hgraph.edges[ut3].ipe;
int ut5=hgraph.edges[ut3+1].ipe;
int t=ut4;
 int   weight =1;


 relaxEdge(ut4,hgraph,p,weight,pred);


 }

 
double rt1=rtclock();
 Galois::do_all_local( pred,Process1());


double rt2=rtclock();
printf("TIME %f \n", (rt2-rt1)*1000);
  int   maxdist =0;


 for (int   i =0;i<hgraph.npoints;i++) {

 if( ((struct struct_hgraph  *)(hgraph.extra))->dist[i]>maxdist  )
maxdist=((struct struct_hgraph  *)(hgraph.extra))->dist[i]; 

 }

 printf("MAX DIST=%d \n",maxdist);


 }
