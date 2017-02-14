
 #include ".h"
 int   ntria ;


  int   changed , hchanged , pnnodes , pntriangles , success ;


  int   aborted ;


 

 

 Galois::InsertBag<struct node>  WL1 ;
Galois::InsertBag<struct node>  WL2 ;
Galois::InsertBag<struct node>   *  current ;
Galois::InsertBag<struct node>   *  collnext ;
Galois::InsertBag<struct node>   *  colltemp ;
;


 void   readTriangle ( HGraph & hgraph ,char    *  name ) 
 {

 FILE*fp; 

 FILE*fp1; 

  char   temp [ 100] ;


  int   ntriaone , ntriatwo , n1 , n2 , n3 , n4 ;


  char    *  name1 ,  *  name2 ;


 name1=(char *)malloc(sizeof(char )*100); 

 name2=(char *)malloc(sizeof(char )*100); 

 sprintf(name1,"%s.ele",name);


 sprintf(name2,"%s.poly",name);


 fp=fopen(name1,"r");


 fscanf(fp,"%d",&ntriaone);


 fgets(temp,100,fp);


 for (int   i =0;i<ntriaone;i++) {

 fscanf(fp,"%d%d%d%d",&n1,&n2,&n3,&n4);


 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].nodes[0]=n2; 

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

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].nodes[1]=n3; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].nodes[2]=INVALID; 

 fgets(temp,100,fp1);


 }

 fclose(fp1);


 }
struct  node  addTriangle ( HGraph & graph ,int & p1 ,int & p2 ,int & p3 ) 
 {

  int   t1 ;


 int  tr1 ;


 t1=HADD(&(devngraphtriangle),1);


 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[0]=p1; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[1]=p2; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]=p3; 

 // this is a access where rhs needs to b converted to int 
tr1=t1;


 return tr1;

 }
struct  node  addTriangle1 ( HGraph & graph ,int & p1 ,int & p2 ) 
 {

  int   t1 ;


 int  tr1 ;


 t1=HADD(&(devngraphtriangle),1);


 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[0]=p1; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[1]=p2; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]=INVALID; 

 // this is a access where rhs needs to b converted to int 
tr1=t1;


 return tr1;

 }
void   verify ( int &  T ,HGraph & graph ) 
 {

  int   x ;


 if( ((struct struct_hgraph  *)(graph.extra))->triangle[T].isdel==0&&((struct struct_hgraph  *)(graph.extra))->triangle[T].isbad==1  )
{
 changed=1; 
 }

 }
float   distanceSquare ( struct Point & p1 ,struct Point & p2 ,HGraph & graph ) 
 {

  float   x1 =p1.x-p2.x;


  float   y1 =p1.y-p2.y;


  float   dsqr =x1*x1+y1*y1;


 return dsqr;

 }
float   distance ( struct Point & one ,struct Point & two ,HGraph & graph ) 
 {

 return sqrtf(distanceSquare(one,two,graph));

 }
struct  node  getOpposite (  t1 ,HGraph & graph ,int   obtuse ) 
 {

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].nodes[2]==INVALID) ?2 : 3; 

  int   commonedgepoint1 =(obtuse+1)%dims;


 for (unsigned   ii =0;ii<dims;++ii) {

  int   nnedgestart =((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].neighedgestart[ii];


 if( nnedgestart==commonedgepoint1  )
{

 return t1;

 }

 }

 return INVALID;

 }
bool  copyNeighbours (  t1 , t2 ,HGraph & graph ) 
 {

  int   dims ;


 for (int   i =0;i<3;i++) {

 ((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].neighbours[i]=((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t2].neighbours[i]; 

 ((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].neighedgestart[i]=((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t2].neighedgestart[i]; 

 }

 return false;

 }
bool  updateNeighbour (  t1 , t2 , t3 ,HGraph & graph ) 
 {

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].nodes[2]==INVALID) ?2 : 3; 

 for (int   i =0;i