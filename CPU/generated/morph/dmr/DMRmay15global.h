  int pngraphtriangle;
  int maxgraphtriangle;
#define INVALID 1234567890
 struct cent {
float   centerx ;
float   centery ;
};
 struct node {
int  nodes [ 3] ;
 int  neighbours [3];
int  neighedgestart [ 3] ;
int   obtuse ;
int   dims ;
int id;
bool   isbad ;
bool   isdel ;
};
