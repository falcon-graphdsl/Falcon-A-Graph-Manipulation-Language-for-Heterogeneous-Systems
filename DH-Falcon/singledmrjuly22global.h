 struct node {
int  nodes [ 3] ;
 int  neighbours [3];
int  neighedgestart [ 3] ;
int   isbad ;
int   isdel ;
int   obstuse ;
int   owner ;
int   dims ;
int   index ;
};
 struct cent {
double   centerx ;
double   centery ;
};
__device__ int falcgraphpoint,falcgraphedge;
__device__ int falcgraphtriangle;
 __device__ int pngraphtriangle;
 __device__ int maxgraphtriangle;
