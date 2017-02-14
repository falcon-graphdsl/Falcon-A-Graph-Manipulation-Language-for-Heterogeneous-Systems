
 #include "mstcoll.h"
HGraph  hgraph ;

 HSet hset;


 

  int   glimit ;


  int   bcnt ;


 

 Galois::InsertBag<struct workitem>  WL1 ;
Galois::InsertBag<struct workitem>  WL2 ;
Galois::InsertBag<struct workitem>  WL3 ;
Galois::InsertBag<struct workitem>  WL4 ;
;


 Galois::InsertBag<struct node>  mst ;
Galois::InsertBag<struct workitem>   *  current ;
Galois::InsertBag<struct workitem>   *  next ;
Galois::InsertBag<struct workitem>   *  pending ;
;


 Galois::InsertBag<struct workitem>   *  temp ;
;


 Galois::InsertBag<struct workitem>  temp11 ;
;


 Galois::InsertBag<int>  coll1 ;
;


 template<typename Pusher0 ,typename Pusher1,typename Pusher2>
void   findLightest ( struct workitem  req ,HGraph & hgraph ,Pusher0 &pred, Pusher1 &next, Pusher2 &pending, int   useLimit ,/*SET*/HSet hset ) 
 {

   struct workitem   req1 ;


 req1=req; 

 int src=req1.src;

  int   cur =req1.cur;


 int falcftadv0=cur;
int falcft0=hgraph.index[src+1]-hgraph.index[src];
int falcft1=hgraph.index[src];
for(int falcft2=falcftadv0;falcft2<falcft0;falcft2++){
int ut0=2*(falcft1+falcft2);
 int ut1=hgraph.edges[ut0].ipe;
int ut2=hgraph.edges[ut0+1].ipe;
int t=ut1;
 int   weight =ut2;


 if( useLimit&&weight>glimit  )
{

   struct workitem   tt ;


 tt.src=src; 

 tt.dst=t; 

 tt.weight=weight; 

 tt.cur=cur; 

 pending.push(tt); 

 return ;

 }

 int rep=hset.parent[src];

 int dst;

  int   old , ch ;


 rep=hset.Findcompress(src); 

 if( rep!=hset.Findcompress(ut1)  )
{

   struct workitem   tt ;


 tt.src=src; 

 tt.dst=t; 

 tt.weight=weight; 

 tt.cur=cur; 

 next.push(tt); 

 
 while(weight<(((struct struct_hgraph  *)(hgraph.extra))->minedge[rep]))  { 

 HMIN(&(((struct struct_hgraph  *)(hgraph.extra))->minedge[rep]),weight,rep,ch);


 }

 return ;

 }

 cur=cur+1; 

 }

 }
template<typename Pusher0 >
void   findLightest1 ( struct workitem  req ,HGraph & hgraph ,Pusher0 &pred, /*SET*/HSet hset ) 
 {

   struct workitem   req1 ;


 req1=req; 

 int src=req1.src;

 int rep=hset.Findcompress(src);

  int   cur =req1.cur;


  int   weight =req1.weight;


 if( rep<hgraph.npoints&&((struct struct_hgraph  *)(hgraph.extra))->minedge[rep]==weight  )
{

 int dst=req1.dst;

 rep=hset.Merge(src,dst); 

 if( dst<hgraph.npoints&&rep  )
{

 ((struct struct_hgraph  *)(hgraph.extra))->minedge[rep]=1234567890; 

   struct node   tt ;


 tt.src=src; 

 tt.dst=dst; 

 tt.weight=req1.weight; 

 mst.push(tt); 

 }

 }

 }
template<typename Pusher0 ,typename Pusher1,typename Pusher2>
void   findLightest2 ( int & p ,HGraph & hgraph ,Pusher0 &pred, Pusher1 &next, Pusher2 &pending, int   useLimit ,/*SET*/HSet hset ) 
 {

   struct workitem   req ;


 req.src=p; 

 req.cur=0; 

 findLightest(req,hgraph,pred,next,pending,useLimit,hset);


 }
struct Process1 {
 Process1(){}
 void operator()( int req){
  findLightest2(req,hgraph,*current,*next,*pending,bcnt,hset);
}
};

struct Process2 {
 Process2(){}
 void operator()(struct workitem &req){
  findLightest1(req,hgraph,*current,hset);
}
};

struct Process3 {
 Process3(){}
 void operator()(struct workitem &req){
  findLightest(req,hgraph,*current,*next,*pending,bcnt,hset);
}
};

int   main ( int   argc ,char    *  argv [ ] ) 
 {int argct1= argc > 4 ?4:argc;
  LonestarStart(argct1,argv,name,desc,url);
if(argc>2)FALC_THREADS=atoi(argv[2]);

 if( argc!=4  )
{

 printf("exec -t theads file");


 return 1;

 }

 

  int   a ;


 hgraph.read2(argv[3]);
hset.allocate(hgraph.npoints);
int hosthgraph=0;
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);
; 

 for (int   i =0;i<hgraph.npoints;i++)hgraph.sortEdges(i);

 glimit=2000; 

 current=&WL1; 

 next=&WL2; 

 pending=&WL3; 

  int   bcnt =0;


  double   rt1 =rtclock();


 for (int   i =0;i<hgraph.npoints;i++) {

 ((struct struct_hgraph  *)(hgraph.extra))->minedge[i]=1234567890; 

 }

 
 Galois::do_all_local( hgraph,Process1());


 bcnt=1; 

 
 while(1)  { 

 
 while(1)  { 

 temp=current; 

 current=next; 

 next=temp; 

 
 Galois::do_all_local(*current,Process2());


 
 Galois::do_all_local(*current,Process3());


 current->clear(); 

 if( next->empty()  )
break;

 }

 glimit*=2; 

 if( pending->empty()  )
break;

 temp=next; 

 current=pending; 

 next=temp; 

 }

  double   rt2 =rtclock();


 printf("TIME=%f \n",(rt2-rt1)*1000);


 }
