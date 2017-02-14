#ifndef EXTERNS_H
#define EXTERNS_H
extern char gp_string[200],extra_array[200],gpcopy_string[300],*mainfunarr;
extern  int extemp;
extern int morph_size,TOT_GPU_GRAPH;//yes
extern  int GLOBAL_ID_FLAG;
extern int union_type;
extern int FALCLINENO;
extern int Gkernel,kerdec15;
extern int ftempcnt;
extern int errcnt;
extern int replicnt;
extern int errflag,Temp;
extern int pdim_size;
extern int sref;
extern int MORPH_FLAG;
extern bool aref;
extern int EXTRAFLAG,CPUGPU,FUN_EBLOCK_RET;
extern fun(FILE *FP1,char *, int ,char *,char *[],int ,char *,int ,char *);
 extern std::map<char *,int>newitr;
extern  std::map<char *,statement *>fnames;//function definition
extern FILE *FP,*FP1,*FPG,*fp1;
 extern std::map<char *,statement *>fnamescond;//foreach call of function in char *with conditional
extern int genpartitioncodegpu(tree_expr *,dir_decl *);
extern int genpartitioncodecpu(tree_expr *,dir_decl *);
extern int codegenupdatefunction(dir_decl *,tree_expr *);
extern class statement *G1,*head,*temp,*temp1,*temp3;
extern volatile int vartop;
extern int INNBRFLAG,errcnt;
extern int  LIBFUNCSIZE;
extern char *dtypenames[],curr_fun_name[100],curr_fun_name1[100];
extern char *libdtypenames[10],*cpy[];
extern char *exptypename[ ],threadstmt[];
extern tree_id *varname[100];
extern char *libdtypefields[5][8],*libfunctions[],*libpartitionfunctions[];
extern char *libdtypefunctions[5][19];
extern int ariflag,FOREACH_FLAG,foreachhostflag;
extern struct UNION_KER *head_position;
extern int print_gpu_if_stmt(statement *,int );
extern check_gpu_expr(tree_expr *);
extern lock_graph(statement *);
extern check_morph_foreach(statement *);
extern realloc_graph(statement *);
extern alloc_extra(statement *);
#endif
