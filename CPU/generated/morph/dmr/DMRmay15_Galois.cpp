#include "DMRmay15.h"
#include<math.h>
#include<iostream>
#include<ostream>
#include<cmath>
#include "colldeclmay6.h"
#ifndef APPS_SSSP_SSSP_H
#define APPS_SSSP_SSSP_H

#include "llvm/Support/CommandLine.h"

#include <limits>
#include <string>
#include <sstream>
#include <stdint.h>
extern llvm::cl::opt<unsigned int> memoryLimit;
#endif
#define PI		3.14159265358979323846	// from C99 standard.
   int devngraphtriangle,badcount;
int   ntria ;


   int changed;
//int *changed;
  int    pnnodes , pntriangles , success ;
 int   hchanged;


   int   aborted ;

struct data {
int id;
}; 

 
 HGraph  hgraph ;
Galois::InsertBag<struct node> initialBad,nextBad,*current,*next,*temp;

 void   readTriangle ( HGraph & hgraph ,char    *  name ) 
 {

 FILE*fp; 

 FILE*fp1; 

  char   temp [ 100] ;


  int   ntriaone , ntriatwo , n1 , n2 , n3 , n4 ;


  char    *  name1 =(char *)malloc(sizeof(char )*100);


  char    *  name2 =(char *)malloc(sizeof(char )*100);


 sprintf(name1,"%s.ele",name);


 sprintf(name2,"%s.poly",name);

printf("XX");
 fp=fopen(name1,"r");
printf("XX");


 fscanf(fp,"%d",&ntriaone);


 fgets(temp,100,fp);


 for (int   i =0;i<ntriaone;i++) {

 fscanf(fp,"%d%d%d%d",&n1,&n2,&n3,&n4);

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].nodes[0]=n2; 
 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].id=n1; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].nodes[1]=n3; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].nodes[2]=n4; 

 fgets(temp,100,fp);


 }

 fclose(fp);


 fp1=fopen(name2,"r");


 fgets(temp,100,fp1);


 fscanf(fp1,"%d",&ntriatwo);


 fgets(temp,100,fp1);


 for (int   i =0;i<ntriatwo;i++) {

 fscanf(fp1,"%d%d%d%d",&n1,&n2,&n3,&n4);


 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].nodes[0]=n2; 
 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].id=n1+ntriaone; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].nodes[1]=n3; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].nodes[2]=INVALID; 

 fgets(temp,100,fp1);


 }

 fclose(fp1);


 }
  int addTriangle(HGraph  graph,int t1,int t2,int t3){
int t4;
int tt=1;
t4=__sync_fetch_and_add(&devngraphtriangle,tt);
((struct struct_hgraph *)(graph.extra))->triangle[t4].nodes[0]=t1;
((struct struct_hgraph *)(graph.extra))->triangle[t4].nodes[1]=t2;
((struct struct_hgraph *)(graph.extra))->triangle[t4].nodes[2]=t3;
return t4;
}

  float   distanceSquare ( struct Point  p1 ,struct Point  p2 ,HGraph   graph ) 
 {

  float   x1 =p1.x-p2.x;


  float   y1 =p1.y-p2.y;


  float   dsqr =x1*x1+y1*y1;


 return dsqr;

 }

   float   distance ( struct Point  one ,struct Point  two ,HGraph   graph ) 
 {

 return sqrtf(distanceSquare(one,two,graph));

 }

   int   getOpposite ( int  t1 ,HGraph   graph ,int   obtuse ) 
 {

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]==INVALID) ?2 : 3; 

  int   commonedgepoint1 =(obtuse+1)%dims;


 for (unsigned   ii =0;ii<dims;++ii) {

  int   nnedgestart =
((struct struct_hgraph  *)(graph.extra))->triangle[t1].neighedgestart[ii];


 if( nnedgestart==commonedgepoint1 ){

 return t1;

 }

 }

 return INVALID;

 }

   bool  copyNeighbours ( int  t1 ,int  t2 ,HGraph   graph ) 
 {

  int   dims ;


 for (int   i =0;i<3;i++) {

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].neighbours[i]=((struct struct_hgraph  *)(graph.extra))->triangle[t2].neighbours[i]; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].neighedgestart[i]=((struct struct_hgraph  *)(graph.extra))->triangle[t2].neighedgestart[i]; 

 }

 return false;

 }

   bool  updateNeighbour ( int  t1 ,int  t2 ,int  t3 ,HGraph   graph ) 
 {

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]==INVALID) ?2 : 3; 

 for (int   i =0;i<dims;i++) {

 if( ((struct struct_hgraph  *)(graph.extra))->triangle[t1].neighbours[i]==t2 ){

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].neighbours[i]=t3; 

 }

 }

 return false;

 }
   bool  checkbad1 ( int  t ,HGraph   graph ) 
 {

 for (int   i =0;i<3;i++) {

 struct Point p1,p2,p3;

  int   te =(i+1)%3;


  int   te2 =(i+2)%3;


 if( ((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[i]!=INVALID&&((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te]!=INVALID&&((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te2]!=INVALID ){

 int repli0=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[i];
p1.x=graph.points[2*repli0].fpe;
p1.y=graph.points[2*repli0+1].fpe;


 int repli1=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te];
p2.x=graph.points[2*repli1].fpe;
p2.y=graph.points[2*repli1+1].fpe;


 int repli2=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te2];
p3.x=graph.points[2*repli2].fpe;
p3.y=graph.points[2*repli2+1].fpe;


  float   vax =p2.x-p1.x;


  float   vay =p2.y-p1.y;


  float   vbx =p3.x-p1.x;


  float   vby =p3.y-p1.y;


  float   dp =vax*vbx+vay*vby;


 if( dp<0 )((struct struct_hgraph  *)(graph.extra))->triangle[t].obtuse=i; else {

  float   dstsqrap1 =distanceSquare(p2,p1,graph);


  float   dstsqrap2 =distanceSquare(p3,p1,graph);


  float   c =dp/sqrtf(dstsqrap1*dstsqrap2);

 if( c>cos(30.000000*PI/180) ){__sync_fetch_and_add(&badcount,1);return 1;}

 }

 }

 }
return 0;

 }

   bool  checkbad ( int  t ,HGraph   graph ) 
 {

 for (int   i =0;i<3;i++) {

 struct Point p1,p2,p3;

  int   te =(i+1)%3;


  int   te2 =(i+2)%3;


 if( ((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[i]!=INVALID&&((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te]!=INVALID&&((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te2]!=INVALID ){

 int repli0=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[i];
p1.x=graph.points[2*repli0].fpe;
p1.y=graph.points[2*repli0+1].fpe;


 int repli1=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te];
p2.x=graph.points[2*repli1].fpe;
p2.y=graph.points[2*repli1+1].fpe;


 int repli2=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te2];
p3.x=graph.points[2*repli2].fpe;
p3.y=graph.points[2*repli2+1].fpe;


  float   vax =p2.x-p1.x;


  float   vay =p2.y-p1.y;


  float   vbx =p3.x-p1.x;


  float   vby =p3.y-p1.y;


  float   dp =vax*vbx+vay*vby;


 if( dp<0 )((struct struct_hgraph  *)(graph.extra))->triangle[t].obtuse=i; else {

  float   dstsqrap1 =distanceSquare(p2,p1,graph);


  float   dstsqrap2 =distanceSquare(p3,p1,graph);


  float   c =dp/sqrtf(dstsqrap1*dstsqrap2);

 if( c>cos(30.000000*PI/180) ){__sync_fetch_and_add(&badcount,1);return 1;}

 }

 }

 }
return 0;

 }
   void   verify (int id, HGraph   graph ) 
//   void   verify ( HGraph   graph ,int *changed) 
 {
//XX 9 0
if(id <(( struct struct_hgraph  *)(graph.extra))->ntriangle){

  int   x ;


 if( ((struct struct_hgraph  *)(graph.extra))->triangle[id].isdel==0&&((struct struct_hgraph  *)(graph.extra))->triangle[id].isbad==1 ){
 __sync_fetch_and_add(&changed,1);
// __sync_fetch_and_add(changed,1);
 }

 }

 }
  int   hadjacent ( int  t1 ,int  t2 ,HGraph  graph ) 
 {

  int   common , firstmatch ;


 common=0; 

 firstmatch=3; 

  int   dimone , dimtwo ;


 dimone=(((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]==INVALID) ?2 : 3; 

 dimtwo=(((struct struct_hgraph  *)(graph.extra))->triangle[t2].nodes[2]==INVALID) ?2 : 3; 

 for (int   i =0;i<dimone;i++) {

 for (int   j =0;j<dimtwo;j++) {

 if( ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[i]==((struct struct_hgraph  *)(graph.extra))->triangle[t2].nodes[j] ){

 if( ++common==2 )return firstmatch;else firstmatch=i; 

 }

 }

 }

 return 3;

 }

   int   adjacent ( int  t1 ,int  t2 ,HGraph   graph ) 
 {

  int   common , firstmatch ;


 common=0; 

 firstmatch=3; 

  int   dimone , dimtwo ;


 dimone=(((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]==INVALID) ?2 : 3; 

 dimtwo=(((struct struct_hgraph  *)(graph.extra))->triangle[t2].nodes[2]==INVALID) ?2 : 3; 

 for (int   i =0;i<dimone;i++) {

 for (int   j =0;j<dimtwo;j++) {

 if( ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[i]==((struct struct_hgraph  *)(graph.extra))->triangle[t2].nodes[j] ){

 if( ++common==2 )return firstmatch;else firstmatch=i; 

 }

 }

 }

 return 3;

 }
  void   hfindneighbours1( void *ptr ) 
 {
//XX 9 0
struct data *dd=(struct data *)(ptr);
int index= dd->id;
printf("INDEX=%d ",index);
int init;
 init=(( struct struct_hgraph  *)(hgraph.extra))->ntriangle/6*index;
int end=init+(( struct struct_hgraph  *)(hgraph.extra))->ntriangle/6;
 for(int id=init;id<end;id++){
  int   iirow =0;
//if(id%10000==0  )printf("%d index=%d xx\n",id,index );
if(id%1000==0)printf("%d  %d xx\n",id,(( struct struct_hgraph  *)(hgraph.extra))->ntriangle );

 for(int unni0=0;unni0<(( struct struct_hgraph  *)(hgraph.extra))->ntriangle;unni0++){


 if( id!=unni0 ){

  int   commonedge =hadjacent(id,unni0,hgraph);


 if( commonedge<3&&iirow<3 ){

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighbours[iirow]=unni0; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighedgestart[iirow]=commonedge; 

 ++iirow; 
if(iirow==3){
if(id<10)printf("%d %d %d %d \n",id, ((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighbours[0],((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighbours[1], ((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighbours[2]); 
break;
}
 }

 }

 }//foreach

 for (;iirow<3;++iirow) {

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighbours[iirow]=INVALID; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighedgestart[iirow]=3; 

 }

 }
}

  void   hfindneighbours ( HGraph  graph ) 
 {
//XX 9 0

 for(int id=0;id<(( struct struct_hgraph  *)(graph.extra))->ntriangle;id++){
  int   iirow =0;
if(id%1000==0)printf("%d  %d xx\n",id,(( struct struct_hgraph  *)(graph.extra))->ntriangle );

 for(int unni0=0;unni0<(( struct struct_hgraph  *)(graph.extra))->ntriangle;unni0++){


 if( id!=unni0 ){

  int   commonedge =hadjacent(id,unni0,graph);


 if( commonedge<3&&iirow<3 ){

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].neighbours[iirow]=unni0; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].neighedgestart[iirow]=commonedge; 

 ++iirow; 
if(iirow==3){
if(id<10)printf("%d %d %d %d \n",id, ((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighbours[0],((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighbours[1], ((struct struct_hgraph  *)(hgraph.extra))->triangle[id].neighbours[2]); 
break;
}
 }

 }

 }//foreach

 for (;iirow<3;++iirow) {

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].neighbours[iirow]=INVALID; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].neighedgestart[iirow]=3; 

 }

 }
}

 

   void   findneighbours ( int id,HGraph   graph ) 
 {
//XX 9 0
if(id <(( struct struct_hgraph  *)(graph.extra))->ntriangle){
//if(id <100){

  int   iirow =0;


 for(int unni0=0;unni0<(( struct struct_hgraph  *)(graph.extra))->ntriangle;unni0++){


 if( id!=unni0 ){

  int   commonedge =adjacent(id,unni0,graph);


 if( commonedge<3&&iirow<3 ){

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].neighbours[iirow]=unni0; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].neighedgestart[iirow]=commonedge; 

 ++iirow; 
if(iirow==3)break;
 }

 }

 }//foreach

 for (;iirow<3;++iirow) {

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].neighbours[iirow]=INVALID; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].neighedgestart[iirow]=3; 

 }


 }

 }
  void   dinit (int id, HGraph   graph ) 
 {
//XX 9 0
if(id <(( struct struct_hgraph  *)(graph.extra))->ntriangle){

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].obtuse=3; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[id].isbad=checkbad1(id,graph);
 if(((struct struct_hgraph  *)(graph.extra))->triangle[id].isbad==1){
if(id<100)printf(" this %d tria with id %d  is bad \n",id,((struct struct_hgraph  *)(graph.extra))->triangle[id].id);
initialBad.push(((struct struct_hgraph  *)(graph.extra))->triangle[id]);
} 


 ((struct struct_hgraph  *)(graph.extra))->triangle[id].isdel=0; 

 }

 }
  struct cent  getCenter ( int  t ,HGraph   graph ) 
 {

   struct cent   ce1 ;


 struct Point p1,p2,p3;

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[2]==INVALID) ?2 : 3; 

 int repli3=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[0];
p1.x=graph.points[2*repli3].fpe;
p1.y=graph.points[2*repli3+1].fpe;


 int repli4=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[1];
p2.x=graph.points[2*repli4].fpe;
p2.y=graph.points[2*repli4+1].fpe;

if(dims==3){
 int repli5=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[2];
p3.x=graph.points[2*repli5].fpe;
p3.y=graph.points[2*repli5+1].fpe;
}
 if( !(((struct struct_hgraph *)(graph.extra))->triangle[t].nodes[0]<((struct struct_hgraph  *)(graph.extra))->ntriangle&&((struct struct_hgraph *)(graph.extra))->triangle[t].nodes[1]<((struct struct_hgraph  *)(graph.extra))->ntriangle&&((struct struct_hgraph *)(graph.extra))->triangle[t].nodes[2]<((struct struct_hgraph  *)(graph.extra))->ntriangle) ){


 ce1.centerx=0.000000; 

 ce1.centery=0.000000; 

 return ce1;

 }

 if( dims==2 ){

 ce1.centerx=(p1.x+p2.x)*0.500000; 

 ce1.centery=p1.y+p1.y*0.500000; 

 return ce1;

 }

  float   vax =p2.x-p1.x;
  float   vay =p2.y-p1.y;
  float   vbx =p3.x-p1.x;
  float   vby =p3.y-p1.y;
  float   xxlen =distance(p1,p2,graph);
  float   yylen =distance(p1,p3,graph);
  float   cosine =(vax*vbx+vay*vby)/(xxlen*yylen);
  float   sinesq =1.0-cosine*cosine;
  float   plen =yylen/xxlen;
  float   ss =plen*cosine;
  float   tt =plen*sinesq;
  float   wp =(plen-cosine)/(2.0*tt);
  float   wb =0.500000-(wp*ss);
 ce1.centerx=p1.x*(1.0-wb-wp)+p2.x*wb+p3.x*wp; 
ce1.centery=p1.y*(1.0-wb-wp)+p2.y*wb+p3.y*wp; 

 return ce1;

 }

   float   radiusSqaure ( struct Point  p1 ,int  t ,HGraph   graph ) 
 {

 struct Point p2;

 int repli6=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[0];
p2.x=graph.points[2*repli6].fpe;
p2.y=graph.points[2*repli6+1].fpe;


 return distanceSquare(p1,p2,graph);

 }

   bool  inCircumCircle ( struct Point  P0 ,int  t1 ,HGraph   graph ) 
 {

   struct cent   ce1 ;


 ce1=getCenter(t1,graph);


 struct Point P1,P2;

 P1.x=ce1.centerx; 

 P1.y=ce1.centery; 

  float   dd =distanceSquare(P1,P0,graph);


 return dd<=radiusSqaure(P1,t1,graph);

 }

int dimcount=0;
template <typename Pusher,typename Pusher1>
  void   drefine (int id, HGraph   &graph ,int   nblocks ,int   size , unsigned int   goal ,int kk, Pusher &current,Pusher1 &nextlist) 
//void   drefine (int id, HGraph   &graph ,int   nblocks ,int   size , unsigned int   goal ,int kk) 
//template <typename Pusher>
 // void   drefine (int id, HGraph   &graph ,int   nblocks ,int   size , unsigned int   goal ,int kk, Pusher &//current) 
 {
   struct cent   cent_new ;


 struct dev_vec pred,frontier,post,conntria1,conntria2;
pred= create_coll_device(100);
frontier= create_coll_device(100);
post= create_coll_device(100);
conntria1= create_coll_device(100);
conntria2= create_coll_device(100);
 struct dev_vec conn1,conn2,conn3,conn4;
conn1= create_coll_device(100);
conn2= create_coll_device(100);
conn3= create_coll_device(100);
conn4= create_coll_device(100);
int uxu=0;
 int  ceelem ;

  int   lchanged=0 ;


  int   successfull ;
 //unsigned wpt=1;
if(id < (( struct struct_hgraph  *)(graph.extra))->ntriangle){
if((((struct struct_hgraph  *)(graph.extra))->triangle[id].isdel)){
//if(id<20)printf( "deleted %d triangle\n",id);
return;
}
if(!(((struct struct_hgraph  *)(graph.extra))->triangle[id].isbad))return;
if( (!(((struct struct_hgraph  *)(graph.extra))->triangle[id].isdel))&&(((struct struct_hgraph  *)(graph.extra))->triangle[id].isbad) ){
 ceelem=id; 

  int   cebot ;

//size of frontier,pred not set
 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[id].obtuse; 

  int   itr =0;


 
 while(cebot<3&&++itr<100&&ceelem<((struct struct_hgraph  *)(graph.extra))->ntriangle)  { 

 ceelem=getOpposite(ceelem,graph,cebot);


 if( ceelem<((struct struct_hgraph  *)(graph.extra))->ntriangle ){

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 }

 }

 if( ceelem>=((struct struct_hgraph  *)(graph.extra))->ntriangle||((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].isdel ){

 ceelem=id; 

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 }

 cent_new=getCenter(ceelem,graph);


 struct Point P1;

 int P0;

 P1.x=cent_new.centerx; //float to float assign

 P1.y=cent_new.centery; 

//if(ceelem==1 || ceelem==25)printf(" added1 I AM HERE %d\n",id);
 add_coll_device(pred,ceelem); 
 add_coll_device(frontier,ceelem); 

while(frontier.size>0 )  {
 int  curr ;
curr=get_elem_coll_device(frontier,curr);
if(curr>=((struct struct_hgraph *)(graph.extra))->ntriangle)break;
volatile  int   dims ;

 if((((struct struct_hgraph  *)(graph.extra))->triangle[curr].nodes[2]==INVALID))dims=2;else dims=3; 
int iunni=0;
 for (int   iunni =0;iunni<dims;iunni++) {

 int  next ;

 next=((struct struct_hgraph  *)(graph.extra))->triangle[curr].neighbours[iunni]; 
if(next>=((struct struct_hgraph *)(graph.extra))->ntriangle )break;
 if( ((struct struct_hgraph  *)(graph.extra))->triangle[next].isdel )continue;
volatile   int   nextdims ;


if((((struct struct_hgraph  *)(graph.extra))->triangle[next].nodes[2]==INVALID))nextdims=2; else  nextdims=3; 

 if( !(next!=ceelem&&dims==2&&nextdims==2)&&inCircumCircle(P1,next,graph) ){
 if( nextdims==2&&dims==2 ){

 for (int jj=0;jj<pred.size;++jj) {

 int  temp1 ;


temp1=get_elem_coll_device(pred,temp1); 
if( (!(((struct struct_hgraph  *)(graph.extra))->triangle[temp1].isdel))&&(((struct struct_hgraph  *)(graph.extra))->triangle[temp1].isbad) )
  nextlist.push(((struct struct_hgraph *)(graph.extra))->triangle[temp1]);
}

frontier.size=0;
pred.size=0; conn1.size=0;conn2.size=0; conntria1.size=0; conntria2.size=0;
 ceelem=next; 

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 itr=0; 

 
while(cebot<3&&++itr<100&&ceelem<((struct struct_hgraph  *)(graph.extra))->ntriangle)  { //if replace with while

 ceelem=getOpposite(ceelem,graph,cebot);


 if( ceelem<((struct struct_hgraph  *)(graph.extra))->ntriangle ){

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 }

 }

 if( ceelem>=((struct struct_hgraph  *)(graph.extra))->ntriangle||((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].isdel ){

 ceelem=next; 

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 }

cent_new=getCenter(ceelem,graph);

 P1.x=cent_new.centerx; 

 P1.y=cent_new.centery; 
//if(ceelem==1 || ceelem==25)printf("added2  I AM HERE %d\n",id);
 add_coll_device(pred,ceelem); 
add_coll_device(frontier,ceelem); 

 }else {
  unsigned   jj ;


 for (jj=0;jj<pred.size;jj++) {

 if( pred.D_Vec[jj]==next )break;

 }

 if( jj==pred.size ){

//if(next==1 || next==25)printf(" added3 I AM HERE %d\n",id);
add_coll_device(pred,next); 
add_coll_device(frontier,next); 
 }

 }

 }else {

  int   pi1 =((struct struct_hgraph  *)(graph.extra))->triangle[curr].neighedgestart[iunni];


 if( pi1>=3 )continue;

 int p1=((struct struct_hgraph  *)(graph.extra))->triangle[curr].nodes[pi1];

int p2=((struct struct_hgraph  *)(graph.extra))->triangle[curr].nodes[(pi1+1)%3];

  unsigned   jj ;


 for (jj=0;jj<conn1.size;jj+=1) {

 if( conn1.D_Vec[jj]==p1&&conn2.D_Vec[jj]==p2 )break;

 }

 if( jj==conn1.size ){

 add_coll_device(conn1,p1); 

 add_coll_device(conn2,p2); 

 add_coll_device(conntria1,curr); 

 add_coll_device(conntria2,next); 

 }

 }
 }

 }

 }

 }
for(int i=0;i<pred.size;i++)((struct struct_hgraph  *)(graph.extra))->owner[pred.D_Vec[i]]=id;
//#pragma omp barrier
for(int i=0;i<pred.size;i++){
int cav1=pred.D_Vec[i];
if(((struct struct_hgraph  *)(graph.extra))->owner[cav1]<id){break;}
else if(((struct struct_hgraph *)(graph.extra))->owner[cav1]>id) ((struct struct_hgraph *)(graph.extra))->owner[cav1]=id;
}
//#pragma omp barrier
 int barrflag=0;for(int i=0;i<pred.size;i++){if(((struct struct_hgraph *)(graph.extra))->owner[pred.D_Vec[i]]!=id){barrflag=1;break;}}
if(id < (( struct struct_hgraph  *)(graph.extra))->ntriangle){
if(( !(((struct struct_hgraph  *)(graph.extra))->triangle[id].isdel))&&((((struct struct_hgraph  *)(graph.extra))->triangle[id].isbad) )){
if(barrflag==0 )//else missing
{

//if(id>500004)printf("OKAY\n");
 ++successfull; 

 int p0;

  int   dims ;

 dims=(((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].nodes[2]==INVALID) ?2 : 3; 

 p0=graph.addPoint(cent_new.centerx,cent_new.centery); //has to check
 if( dims==2 ){
 int  tr1 , tr2 ;

 tr1=addTriangle(graph,p0,((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].nodes[0],INVALID);
((struct struct_hgraph  *)(graph.extra))->triangle[tr1].id=tr1;

 tr2=addTriangle(graph,p0,((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].nodes[1],INVALID);
((struct struct_hgraph  *)(graph.extra))->triangle[tr2].id=tr2;
 copyNeighbours(tr1,ceelem,graph);

 copyNeighbours(tr2,ceelem,graph);

 add_coll_device(post,tr1); 

 add_coll_device(post,tr2); 



 }

 for (unsigned   ii =0;ii<conn1.size;ii+=1) {

 int p1=conn1.D_Vec[ii];

 int p2=conn2.D_Vec[ii];

 int  connsrc ;


 connsrc=conntria1.D_Vec[ii]; 

 int  conndst ;


 conndst=conntria2.D_Vec[ii]; 

 int  newtri ;


 newtri=addTriangle(graph,p0,p1,p2); 
((struct struct_hgraph  *)(graph.extra))->triangle[newtri].id=newtri;

  int   jj1 ;


 for (jj1=0;jj1<pred.size;jj1+=1) {

 if( pred.D_Vec[jj1]==conndst )break;

 jj1++; 

 }

 int  newconn ;


 if( jj1==pred.size )newconn=conndst; else newconn=connsrc; 

  int   iinbr =0;


 ((struct struct_hgraph  *)(graph.extra))->triangle[newtri].neighbours[0]=newconn; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[newtri].neighedgestart[0]=1; 

 iinbr++; 

 for (unsigned   jj =0;jj<post.size;++jj) {

  int   commonedgestart =adjacent(post.D_Vec[jj],newtri,graph);


 if( commonedgestart<3&&iinbr<3 ){

 ((struct struct_hgraph  *)(graph.extra))->triangle[newtri].neighbours[iinbr]=post.D_Vec[jj]; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[newtri].neighedgestart[iinbr]=commonedgestart; 

 ++iinbr; 

 int  temp1 ;


 temp1=post.D_Vec[jj]; 

 int  unx ;


 unx=post.D_Vec[jj]; 

 updateNeighbour(unx,newconn,newtri,graph);


 }

 }

 add_coll_device(post,newtri); 

 }



  unsigned   jj ;


 for (jj=0;jj<pred.size;++jj) {

 int  temp1 ;


temp1=get_elem_coll_device(pred,temp1); 

 ((struct struct_hgraph  *)(graph.extra))->triangle[temp1].isdel=1; 
//if(temp1<100)printf(" %d deleted\n",temp1);

 }

 for (unsigned   ii =0;ii<post.size;++ii) {

 int  xyx ;


 xyx=get_elem_coll_device(post,xyx); 

 ((struct struct_hgraph  *)(graph.extra))->triangle[xyx].obtuse=3; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[xyx].isbad=checkbad(xyx,graph);
 //if(((struct struct_hgraph  *)(graph.extra))->triangle[xyx].isbad==1)initialBad.push(((struct struct_hgraph  *)(graph.extra))->triangle[xyx]);
 if(((struct struct_hgraph  *)(graph.extra))->triangle[xyx].isbad==1)nextlist.push(((struct struct_hgraph  *)(graph.extra))->triangle[xyx]);
 ((struct struct_hgraph  *)(graph.extra))->triangle[xyx].isdel=0; 

 lchanged|=((struct struct_hgraph  *)(graph.extra))->triangle[xyx].isbad; 

 }

 if( lchanged ){

 changed=1; 

 }

 }

else {

 if( !(((struct struct_hgraph  *)(graph.extra))->triangle[id].isdel)&&(((struct struct_hgraph  *)(graph.extra))->triangle[id].isbad) ){

 lchanged=1; 
 changed=1; 
//if(id<100)printf("pushing %d back to worklist",id);
nextlist.push(((struct struct_hgraph  *)(graph.extra))->triangle[id]);
 ++aborted; 

 }
 }
}//IF DONE

}
pred.size=frontier.size=conn1.size=post.size=conn2.size=conntria1.size=conntria2.size=0;
}
  void printnbrdev(int i,HGraph  graph){
printf("%d %d %d %d\n",i,(((struct struct_hgraph *)(graph.extra))->triangle[i].neighbours[0]),(((struct struct_hgraph *)(graph.extra))->triangle[i].neighbours[1]),(((struct struct_hgraph *)(graph.extra))->triangle[i].neighbours[2]));
}
  void reset(int i,HGraph  &graph){
if(i<((struct struct_hgraph *)(graph.extra))->ntriangle){
((struct struct_hgraph *)(graph.extra))->owner[i]=1234567890;
}
}
void readnbrsfromfile(char *name,HGraph &hgraph){
char *filename=(char *)malloc(sizeof(char)*100);
sprintf(filename,"%s.nbr",name);
FILE *fp2=fopen(filename,"r");
if(fp2==NULL)printf("********OPEN FALIED\n");
int ttt;
for(int i=0;i<((struct struct_hgraph *)(hgraph.extra))->ntriangle;i++){
fscanf(fp2,"%d%d%d%d%d%d%d",&ttt,&(((struct struct_hgraph *)(hgraph.extra))->triangle[i].neighbours[0]),&(((struct struct_hgraph *)(hgraph.extra))->triangle[i].neighbours[1]),&(((struct struct_hgraph *)(hgraph.extra))->triangle[i].neighbours[2]),
&(((struct struct_hgraph  *)(hgraph.extra))->triangle[i].neighedgestart[0]),&(((struct struct_hgraph  *)(hgraph.extra))->triangle[i].neighedgestart[1]),&(((struct struct_hgraph  *)(hgraph.extra))->triangle[i].neighedgestart[2]));
if(i<10 )fprintf(stderr,"%d %d %d %d %d %d %d\n",ttt,(((struct struct_hgraph *)(hgraph.extra))->triangle[i].neighbours[0]),(((struct struct_hgraph *)(hgraph.extra))->triangle[i].neighbours[1]),(((struct struct_hgraph *)(hgraph.extra))->triangle[i].neighbours[2]),
(((struct struct_hgraph  *)(hgraph.extra))->triangle[i].neighedgestart[0]),(((struct struct_hgraph  *)(hgraph.extra))->triangle[i].neighedgestart[1]),(((struct struct_hgraph  *)(hgraph.extra))->triangle[i].neighedgestart[2]));

}
fclose(fp2);
}
struct Preprocess {
  Preprocess()  { }
  void operator()(int item) const {

   dinit(item,hgraph);
  // dinit(item+5000004,hgraph);
  // dinit(item+1000004,hgraph);
   dinit(item+250004,hgraph);
  }
};
 //template <typename Pusher>
struct Process {
   Process() {}
  void operator()(struct node  &item) {
  //void operator()(struct node  &item,Galois::UserContext<struct node> &ctx) {
int id= item.id;
//if(id<100)printf(" %d in worklist\n",id);
int goal=100;
drefine(id,hgraph,13,1024,goal,10,*current,*next);//access gpu value in kernel blocksize
}
   };

main(int argc,char *argv[]){
Galois::StatManager statManger;
LonestarStart(argc,argv,name,desc,url);
  int   ntria ;
int *arrayin,*arrayout;
//drefine, cudaFuncCachePreferL1);
printf("read");
 /*NULL*/hgraph.readNodes(argv[3],2); 
printf("read");
int hosthgraph=1;
hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph ));
 read_hgraph_pptysize(hgraph);
alloc_extra_hgraph(hgraph,hosthgraph);


printf("read");
 readTriangle(hgraph,argv[3]);
printf("read");
 readnbrsfromfile(argv[3],hgraph);
//for(int i=0;i<((struct struct_hgraph *)(hgraph.extra))->ntriangle;i++)
//dinit(i,hgraph);//access gpu value in kernel blocksize
Galois::do_all_local(hgraph,Preprocess(),Galois::loopname("findbad"));
//return;
using namespace Galois::WorkList;

  typedef LocalQueue<dChunkedLIFO<256>, ChunkedLIFO<256> > BQ;
  typedef AltChunkedLIFO<32> Chunked;
fprintf(stderr," TOTAL BAD TRIANGLES =%d\n",badcount);
devngraphtriangle=((struct struct_hgraph *)(hgraph.extra))->ntriangle;
int goal=0;
current=&initialBad;
next=&nextBad;
double tt1=rtclock();
do{
changed=0;
      Galois::do_all_local(*current, Process());
std::swap(current,next);
if((*current).empty())break;
((struct struct_hgraph *)(hgraph.extra))->ntriangle=devngraphtriangle;
printf("  %d \n",devngraphtriangle);
}while(changed);
double tt2=rtclock();
printf("time=%f\n",(tt2-tt1)*1000);
  //    Galois::for_each_local(initialBad, Process());
for(int i=0;i<devngraphtriangle;i++){
if(((struct struct_hgraph *)(hgraph.extra))->triangle[i].isbad&&( !((struct struct_hgraph *)(hgraph.extra))->triangle[i].isdel)){
printf("bad %d \n",i);
}
}
return;
int kk;
double t1=rtclock();
 do {
++goal;
changed=0;
 dimcount=0;
//#pragma omp for
for(int i=0;i<devngraphtriangle;i++)reset(i,hgraph);
badcount=0;
//#pragma omp for
for(int i=0;i<devngraphtriangle;i++)
//drefine(i,hgraph,13,1024,goal,kk);//access gpu value in kernel blocksize
((struct struct_hgraph *)(hgraph.extra))->ntriangle=devngraphtriangle;
//printf("****** ITR  OVER*********8\n");
 } while ( changed);
printf("  %d \n",devngraphtriangle);
double t2=rtclock();
printf("time=%f\n",(t2-t1)*1000);
for(int i=0;i<devngraphtriangle;i++){
if(((struct struct_hgraph *)(hgraph.extra))->triangle[i].isbad&&( !((struct struct_hgraph *)(hgraph.extra))->triangle[i].isdel)){
printf("bad %d \n",i);
}
}
//the code below is verify

//verify<<<temp15/prop.maxThreadsPerBlock,prop.maxThreadsPerBlock>>>(graph);//access gpu value in kernel blocksize

 }
