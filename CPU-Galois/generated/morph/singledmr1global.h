int FALC_THREADS;
 struct node {
int  nodes [ 3] ;
 /***/int  neighbours [3];
int  neighedgestart [ 3] ;
/*-1*/int   isbad ;
/*-1*/int   isdel ;
/*-1*/int   obtuse ;
/*-1*/int   owner ;
/*-1*/int   dims ;
/*-1*/int   id ;
/*-1*/int   index ;
};
 struct cent {
/*-1*/float   centerx ;
/*-1*/float   centery ;
};
 int falcgraphtriangle;
  int pngraphtriangle;
  int maxgraphtriangle;
