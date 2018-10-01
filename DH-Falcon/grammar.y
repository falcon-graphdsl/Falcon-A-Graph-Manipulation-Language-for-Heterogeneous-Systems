%{
#include <stdio.h>
    #include <string.h>
    #include <assert.h>
//#include "Graph.h"
//include "Graph.h"
//#include "HGraph.h"
//#include "GGraph.h"
#include "symtabold.h"
extern int morph_size,TOT_GPU_GRAPH,Gkernel;//yes
extern int union_type,TARGET,DUPVAR;
extern void setcomm();
extern void mpiinit(FILE *,FILE *);
extern void mpiinitgpu(FILE *,FILE *);
extern void mpiinitmultigpu(FILE *,FILE *);
extern void mpiinitcpugpu(FILE *,FILE *);
extern FILE *FP,*FP1,*FPG,*fp1;
 extern std::map<char *,int>newitr;
extern  std::map<char *,statement *>fnames;//function definition
 extern std::map<char *,statement *>fnamescond;//foreach call of function in char *with conditional
 extern std::map<char *,int>fnamessingle;//foreach call of function in char *with conditional
class statement *G1=NULL,*head=NULL,*temp=NULL,*temp1=NULL,*temp3=NULL,*de1,*de2;
int nitr=11,STMTNO=0,macroflag=-1;
int KERNEL=0,PEER_ENABLE,SINGLEFLAG;
int FOREACH_FUN_FLAG;
int ariflag=0,LINENO=1,arglistflag=0;
globsymtable GLOBAL_TABLE;
int GALOIS_FLAG,NUM_GPUS;
volatile int vartop;
extern void setparent();
extern char *union_string[2];
extern char *morphalloc_string[3];
extern FILE *yyin;
globsymtable *currsymtab=&GLOBAL_TABLE, *stemp;
char  *utflagarr[2][3]={{"3","+1","+2"},{"2","","+1"}};
char partitionfile[100];
int utflag=1;
int FUNCALL_FLAG=0,INNBRFLAG,OUTNBRFLAG,MPIFLAG=0;
char header[100],source[100],gheader[100],*tempitr=NULL;
struct funcall_list *fhead=NULL;//declared in tree.h
char *dtypenames[]={"int ","float ","short ","double ","char ","long ","void ","signed ","unsigned ","bool","...","struct ","struct_rec","enum","union ","typedef "};
char *MPIdtypenames[]={"MPI_INT","MPI_FLOAT","MPI_SHORT","MPI_DOUBLE","MPI_CHAR","MPI_LONG","MPI_VOID ","MPI_SIGNED ","MPI_UNSIGNED","MPI_BOOL","...","struct_invalid","struct_rec_invalid","enum_invalid","union_invalid ","typedef_invalid "};
char *libdtypenames[10]={"graph ","edge ","point ","set ","collection","g_p ","e_p ","g_p ","ITR","LINT"};
char *libdtypefields[5][8]={ { "npoints" ,"points","nedges","edges","directed","twod","isdel","unni"},{"src","dst","weight","isdel","unni","unni","unni","unni"},{"minEdge","maxEdge","x","y","nbrs","inNbrs","outNbrs","isdel"},{"parent","size","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"},{"size","PORST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"}};
char *libdtypefunctions[5][19]={ { "addPoint" ,"read","addEdge","deledge","delpoint","getweight","addProperty","addPointProperty","addEdgeProperty","init","reset","getedge","readNodes","read2","read1","redmorph","makePartition","updateFunction","UpdatePartition"},
{"del","setweight","changesrc","changedst","reset","init","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"},
{"getNeighbours","getInNeighbours","getOutNeighbours","del","addNeighbour","delNeighbour","reset","init","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"},
{"makeset","merge","find","getSize","clear","add","del","reset","init","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"},
{"add","del","merge","getSize","clear","reset","init","OrderByIntValue","empty","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"}};
char *exptypename[ ]={"+","-","*","/","%","&&","||","^","&","|","~","+","*","-","&","|","&","*","!","++","--","++","--","sizeof","align","[","<<",">>","<",">","<=",">=","==","!=","","","","","->","","","\(","",""};
char *libfunctions[]={"MIN","MAX","INCR","DECR","CAS","ADD","SUB","OR","AND","XOR","genericbarrier"};
char *libpartitionfunctions[]={">","<","+=1","-=1","CAS","+=","-=","||=","&&=","~=","genericbarrier"};
char *iternames[]={"points ", "edges ", "nbrs ", "innbrs ", "outnbrs ", "items "};
bool barrier=0;
struct globvar_in_fun *globvars_fun;
#include "grammar.h"
//#include "parse_cmd_line.h"
tree_id *varname[100];
tree_expr *exprstack[25];
tree_decl_stmt *argstack[25];
int exprtop=0,argtop=0;
volatile int declared=0;
statement *foreach;
//#include "../include/tree.h"
    extern int yylex();
    extern void skipline();
void yyerror(const char *s);
int flag=0,fflag=0;
statement *sblock_begin[100],*s1;
int sbtop=0,arrtop=0;
proc_id *FDEF=NULL;
tree_typedecl *FTYPE,*DECLTYPE=NULL,*DECLTYPE1=NULL;
dir_decl *FNAME;
int param_count=0,vararg=0,argcount=0;


bool assflag;

%}
%union {
    int  info;
    long ival;
    double fval;
    char* text;
    bool bval;
class  tree_node* ptr;
   // expr_list* e_list;  // list of expressions
    //lhs_list* l_list;   // list of lhs
    //struct parse_temp_t pair;
}

%token	IDENTIFIER I_CONSTANT B_CONSTANT F_CONSTANT STRING_LITERAL FUNC_NAME SIZEOF GETTYPE WHITE_SPACE SECTION SECTIONS PARALLEL
%token	PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN RADD_ASSIGN RMUL_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME ENUMERATION_CONSTANT

%token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE 
%token	CONST RESTRICT VOLATILE
%token	BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID SINGLE 
%token	COMPLEX IMAGINARY 
%token	STRUCT UNION ENUM ELLIPSIS STRUCT_REC

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN HASH_DEFINE

%token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL
%token  GRAPH EDGE SET COLLECTION POINT E_P G_P P_P GPU
%token  POINTS  EDGES  NBRS  INNBRS OUTNBRS ITEMS FOREACH  INOLD

%type<text> IDENTIFIER  STRING_LITERAL  TYPEDEF_NAME FOR FOREACH GETTYPE 
%type<ival> I_CONSTANT iterator B_CONSTANT
%type<fval> F_CONSTANT 
%type<ptr>  primary_expression  generic_selection constant postfix_expression unary_expression cast_expression multiplicative_expression additive_expression shift_expression paraargument_expression_list conditional_for
%type<ptr> relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression IN IN1 devno
%type<ptr> assignment_expression   enumeration_constant  expression argument_expression_list  unary_operator string init_declarator init_declarator_list declarator macro_statement compound_begin compound_end selection_statement
%type<ptr> direct_declarator declaration_specifiers storage_class_specifier type_specifier declaration block_item
%type<ptr> struct_or_union_specifier struct_or_union struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator type_qualifier struct_declarator_list  sections parallel_statement
%type<ptr>  assignment_operator enum_specifier enumerator_list enumerator  constant_expression compound_statement statement expression_statement block_item_list ENUMERATION_CONSTANT
%type<ptr> translation_unit external_declaration  parameter_list parameter_type_list parameter_declaration iteration_statement initializer_list initializer pointer jump_statement type_qualifier_list function_specifier direct_abstract_declarator abstract_declarator type_name designator_list designator designation lib_type_specifier alltype list1 declaration_list identifier_list
%start translation_unit 

%nonassoc ATOMIC1 EDGE1 POINT1 DIR_DECL MACRO1 GETTYPE1 PR_EXPR1
%nonassoc ';'
%nonassoc ','
%left '(' ')' PTR_OP INC_OP DEC_OP 
%left '&'
%left '+' '-'
%left '*'
%nonassoc THEN
%nonassoc ELSE
%%

primary_expression
	: IDENTIFIER {
	printf("XX\n");dir_decl *x=currsymtab->findsymbol($1);
	$$=new tree_expr(x);
	((tree_expr *)$$)->name=$1;
	if(x==NULL){fprintf(stderr,"\nCANNOT FIND NULL %s \n",$1);}
	((tree_expr *)$$)->nodetype=-1;	}/*$$=new tree_expr();*/ /* need to add code to copy datatype information from dir_decl to tree_expr */
	| constant {$$=$1;}
	| string {$$=$1;printf("STRING");}
	| '(' expression ')' {
        $$=binaryopnode(NULL,NULL,EXPR,-1);
	 statement *t1=temp3; 
	while(t1->comma==1)t1=t1->prev;
	((tree_expr *)$$)->exprlist=t1;temp1=temp3=t1->prev;printf("DDDDDDDDDDDDDDDDDDDD\n");}
	| generic_selection{ $$=$1;}
	;

constant
        : I_CONSTANT{
        $$=binaryopnode(NULL,NULL,-1,TREE_INT);
	((tree_expr *)$$)->ival=$1;
	((tree_expr *)$$)->dtype=0;fprintf(stderr,"INT %d",$1);}		/* includes character_constant */
	| F_CONSTANT{
        $$=binaryopnode(NULL,NULL,-1,TREE_FLOAT);
	((tree_expr *)$$)->fval=$1;
	((tree_expr *)$$)->dtype=1;}
        | B_CONSTANT{
        $$=binaryopnode(NULL,NULL,-1,TREE_BOOL);
	((tree_expr *)$$)->ival=$1;
	((tree_expr *)$$)->dtype=9;}
	| ENUMERATION_CONSTANT{$$=$1;}	/* after it has been defined as such */
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER{
        $$=binaryopnode(NULL,NULL,-1,TREE_ENUMCONST);
	((tree_expr *)$$)->name=$1; }
	;

string
	: STRING_LITERAL {
	 printf("STRING LITERAL %s ", $1);
        $$=binaryopnode(NULL,NULL,-1,TREE_STRING);
	 ((tree_expr *)$$)->name=malloc(sizeof(char)*100);
	 strcpy(((tree_expr *)$$)->name,$1);
	if(strlen($1)<=3)((tree_expr *)$$)->dtype=4;printf("STRING LITERAL\n");}
	| FUNC_NAME      {$$=new tree_expr();}
	;

generic_selection
	: GENERIC '(' assignment_expression ',' generic_assoc_list ')'{$$=new tree_node();}
	;

generic_assoc_list
	: generic_association
	| generic_assoc_list ',' generic_association
	;

generic_association
	: type_name ':' assignment_expression
	| DEFAULT ':' assignment_expression
	;

postfix_expression
	: primary_expression %prec PR_EXPR1  {	printf("xx\n");$$=$1; }
	| postfix_expression '[' expression ']'{
	 if(((tree_expr *)$1)->expr_type!=ARRREF){
	tree_expr *t1;t1=$1; 
	if(t1->expr_type!=STRUCTREF){
		t1->expr_type=ARRREF;
		$$=t1;
	} 
	else {
		 t1=((tree_expr *)$1)->rhs; 
		 t1->expr_type=ARRREF;
	}
	assign_stmt *y=t1->earr_list;
	if(y==NULL){
		t1->earr_list=((assign_stmt *)$3);
		t1->earr_list->next=NULL;
	}
	else{
		while(y->next!=NULL){
			y=y->next; printf("ONE\n");
		}
		 y->next=((assign_stmt *)$3);
		 y->next->next=NULL;
	}
	$$=$1;}
       else {
      	 ((tree_expr *)$1)->expr_type=ARRREF;
	 assign_stmt *y=((tree_expr *)$1)->earr_list;
	if(y==NULL){
		printf("UNNIKRISHNAN\n");
		((tree_expr *)$1)->earr_list=((assign_stmt *)$3);
		((tree_expr *)$1)->earr_list->next=NULL;
	}
	else{
		while(y->next!=NULL){
			y=y->next; printf("ONE\n");
		 }
		 y->next=((assign_stmt *)$3);
		y->next->next=NULL;
	 }
	 $$=$1; printf("ARRASSIGN");
	}
	temp1=temp3->prev; 
	temp3->prev=NULL; 
	temp1->next=NULL;}
	| postfix_expression '(' ')'{
	 tree_expr *t1=$1;
	 if(t1->expr_type!=STRUCTREF){
t1=funcallpostfix(t1,FUNCALL,KERNEL,NULL);
	} 
	else {
                t1=((tree_expr *)$1)->rhs;
t1=funcallpostfix(t1,FUNCALL,KERNEL,NULL);
	}
	$$=$1;}
	| postfix_expression '('assignment_expression ')' {
		tree_expr *t1=$1;
		if(t1->expr_type!=STRUCTREF){
t1=funcallpostfix(t1,FUNCALL,KERNEL,$3);
			FUNCALL_FLAG=1;
		       struct funcall_list *funtemp=malloc(sizeof(struct funcall_list));
		       funtemp->name=malloc(sizeof(char)*100);
                       if(t1->name!=NULL)strcpy(funtemp->name,t1->name);
                       funtemp->call=t1;
                       if(fhead!=NULL)funtemp->next=fhead;else funtemp->next=NULL;
                        fhead=funtemp;
		}
	      else {
                t1=((tree_expr *)$1)->rhs;
t1=funcallpostfix(t1,FUNCALL,KERNEL,$3);
             }
     t1=$1;
if(t1->expr_type==STRUCTREF&& t1->rhs && t1->rhs->name &&!(strcmp(t1->rhs->name,"updateFunction"))){
tree_expr *tt1=t1->rhs;
tree_expr *funname=tt1->arglist->rhs;
for (std::map<char *,statement *>::iterator it1=fnames.begin(); it1!=fnames.end(); ++it1){
if (!strcmp(funname->name, it1->first)&&(strlen(funname->name)==strlen(it1->first))){
statement *update_decl= it1->second;
update_decl->noprint=true;
break;
}
}
}
}
	| postfix_expression '('assignment_expression',' paraargument_expression_list ')' {
            assign_stmt *pt1=createassignlhsrhs(-1,NULL,$3); 
            tree_expr *t1;
            t1=$1;
           if(arglistflag==1){
                arglistflag=0;
                pt1->next=$5;
           }else {
                if(t1->expr_type!=STRUCTREF)t1->params=$5;
                else t1->rhs->params=$5;}
            if(t1->expr_type!=STRUCTREF){
                t1->expr_type=FUNCALL;
                t1->kernel=KERNEL;
                t1->arglist=pt1;
                struct funcall_list *funtemp=malloc(sizeof(struct funcall_list));
                funtemp->name=malloc(sizeof(char)*100);
                if(t1->name!=NULL)strcpy(funtemp->name,t1->name); 
                funtemp->call=t1;
                if(fhead!=NULL)funtemp->next=fhead;else funtemp->next=NULL;
                fhead=funtemp;
                FUNCALL_FLAG=1;
	 } //t1->rhs is datatype of property
	 else   {
		 ((tree_expr *)$1)->rhs->expr_type=FUNCALL;
		 ((tree_expr *)$1)->rhs->kernel=KERNEL;
		  t1->rhs->arglist=pt1;
         }
	 $$=$1;
         fprintf(stderr,"funcall \n");
         if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"makePartition"))){
             assign_stmt *ass=t1->rhs->arglist;
             int cnt=0;
             int flag=0;
             dir_decl *dd=currsymtab->findsymbol(t1->lhs->name);
             while(ass!=NULL){
                  if(ass->rhs->nodetype==TREE_INT&&ass->rhs->ival>1&&cnt==0){
                        flag=1;
                        dd->gpugpu=ass->rhs->ival;
                        if (dd->gpugpu>1)PEER_ENABLE=1;
                  }
                  if(ass->rhs->nodetype==TREE_INT&&ass->rhs->ival>0&&cnt==1)flag=2;
                  ass=ass->next;
                  cnt++;
            }
            if(flag==2)dd->cpugpu=true;

        }
        if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"addPointProperty"))){   
dir_decl *d1= currsymtab->findsymbol(t1->lhs->name);
if((TARGET==1 || TARGET==3 ||TARGET==4) &&d1->gpu==false){
d1->gpu=true;
TOT_GPU_GRAPH++;
}
if(TARGET==5 &&d1->cpugpu==0){
d1->cpugpu=1;
TOT_GPU_GRAPH++;
}
adddynamicproperty(t1,P_P_TYPE,pt1);      }
      if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"addEdgeProperty"))){   adddynamicproperty(t1,E_P_TYPE,pt1);      }
      if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"addProperty"))){addgraphproperty(t1,G_P_TYPE,pt1);     }
     if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"OrderByIntValue"))){
          dir_decl *d1=currsymtab->findsymbol(t1->lhs->name);
          d1->ordered=true;
     }
 /*    if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"updateFunction"))){//currently UpdateFunction has one arg 
tree_expr *t1=t1->rhs;tree_expr *funname=t1->arglist->rhs;for (std::map<char *,statement *>::iterator it1=fnames.begin(); it1!=fnames.end(); ++it1){if (!strcmp(funname->name, it1->first)&&(strlen(funname->name)==strlen(it1->first))){//fprintf(FP1,"//found funname %s\n",funname->name);statement *update_decl= it1->second;update_decl->noprint=true;break;}}}*/
}
	| postfix_expression PTR_OP IDENTIFIER{
                $$= binaryopnode($1,NULL,PTRREF,-1);
                ((tree_expr *)$$)->rhs=new tree_expr();
                ((tree_expr *)((tree_expr *)$$)->rhs)->name=$3;
                ((tree_expr *)((tree_expr *)$$)->rhs)->expr_type=VAR;
         }
	| postfix_expression '.' IDENTIFIER{
              printf("\nCLASS FUNCTION CALL\n");
                $$= binaryopnode($1,NULL,STRUCTREF,-1);
              ((tree_expr *)$$)->rhs=new tree_expr();
              ((tree_expr *)((tree_expr *)$$)->rhs)->name=$3;
              ((tree_expr *)((tree_expr *)$$)->rhs)->expr_type=VAR;
              ((tree_expr *)$$)->kernel=KERNEL;
              ((tree_expr *)($1))->nodetype=-10;
}
	| postfix_expression INC_OP{$$= binaryopnode($1,NULL,POSTINC,-1);}
        | postfix_expression'.'GETTYPE'('')'  identifier_list %prec GETTYPE1 { 
                $$= binaryopnode($1,NULL,GET_TYPE,-1);
              ((tree_expr *)$$)->rhs=new tree_expr();
              ((tree_expr *)((tree_expr *)$$)->rhs)->name=$3;
              ((tree_expr *)((tree_expr *)$$)->rhs)->expr_type=VAR;
              ((tree_expr *)((tree_expr *)$$)->rhs)->nextv=$6;
              tree_expr *u1=$1;
              fprintf(stderr,"GETYPE %d %s %d ",u1->expr_type,u1->lhs->name,((dir_decl *)(u1->lhs))->libdtype);
              if(u1->expr_type==VAR && ((dir_decl *)(u1->lhs))->libdtype==GRAPH_TYPE){
                   dir_decl *dg=u1->lhs;
                   tree_typedecl *tpold= dg->tp1;
                   tree_typedecl *tp1;
                   tp1=new tree_typedecl();
                   tp1->libdatatype=GRAPH_TYPE; 
                   tp1->name=malloc(sizeof(char)*100);
                   strcpy(tp1->name,libdtypenames[tp1->libdatatype]);
                   if(dg->ppts!=NULL){
                       fprintf(stderr,"EXTRA PROPERTIES NOT NULL\n");
                       extra_ppts *newppts,*oldppts=dg->ppts,*head;
                       newppts=malloc(sizeof(struct extra_ppts));
                       newppts->parent=NULL;
                       newppts->name=malloc(sizeof(char)*100);
                       fprintf(stderr," %s ",oldppts->name);
                       strncpy(newppts->name,oldppts->name,strlen(oldppts->name));
                       newppts->libdtype=oldppts->libdtype;
                       newppts->t1=oldppts->t1;//mutliple entries point to same type
                       newppts->var2=oldppts->var2;
                       newppts->var1=oldppts->var1;
                       newppts->var3=oldppts->var3;
                       //newppts->var2->name=malloc(sizeof(char)*100);
                       newppts->val2=oldppts->val2;
                      if(oldppts->var2!=NULL){
		     }
		    fprintf(stderr,"%d %d ",strlen(newppts->name),strlen(oldppts->name));
		    newppts->next=NULL;
		    newppts->parent=dg;
                    head=newppts;
                    oldppts=oldppts->next;
                    while(oldppts){
                        newppts->next=malloc(sizeof(struct extra_ppts));
                        newppts=newppts->next;
                        newppts->parent=NULL;
                       fprintf(stderr," %s ",oldppts->name);
                       newppts->name=malloc(sizeof(char)*100);
                       strcpy(newppts->name,oldppts->name);
                       fprintf(stderr," %s ",newppts->name);
                       newppts->libdtype=oldppts->libdtype;
                       newppts->t1=oldppts->t1;
                       newppts->var2=new dir_decl();
                       newppts->val2=oldppts->val2;
                       newppts->var2->name=malloc(sizeof(char)*100);
                       if(oldppts->var2!=NULL)strncpy(newppts->var2->name,oldppts->var2->name,strlen(oldppts->var2->name));
                        //strcpy(newppts->var2->name,oldppts->var2->name);commented and added the above on march 23
                       newppts->next=NULL;
                       oldppts=oldppts->next;

                }

                tp1->ppts=head;
                fprintf(stderr," %s ",tp1->ppts->name);
        }
       fprintf(stderr,"ADDED USING GETTYPE\n");
        symtableentry *old=NULL;currsymtab->addsymbol((dir_decl *)$6,(tree_typedecl *)tp1);
            }

	}
	| postfix_expression DEC_OP{$$= binaryopnode($1,NULL,POSTDEC,-1);}
	| '(' type_name ')' '{' initializer_list '}'{
             $$=new tree_expr();
             ((tree_expr *)$$)->expr_type=TYPE_INIT;
             ((tree_expr *)$$)->tpname=$2;
             ((tree_expr *)$$)->rhs=new tree_expr();
             ((tree_expr*)$$)->rhs->expr_type=INITBLOCK;
             ((tree_expr*)$$)->rhs->rhs=$5;}
	| '(' type_name ')' '{' initializer_list ',' '}'{
               $$=new tree_expr();
                ((tree_expr *)$$)->expr_type=TYPE_INIT;
                 ((tree_expr *)$$)->tpname=$2;
                 ((tree_expr *)$$)->rhs=new tree_expr();
                 ((tree_expr*)$$)->rhs->expr_type=INITBLOCK;
                  ((tree_expr*)$$)->rhs->rhs=$5;}
	;
paraargument_expression_list: argument_expression_list{fprintf(stderr,"argument list %d",LINENO);$$=$1;arglistflag=1;}
                            | parameter_type_list {fprintf(stderr, " parameter list %d",LINENO);$$=$1;}
argument_expression_list
	: assignment_expression {
                    assign_stmt *t1=createassignlhsrhs(-1,NULL,$1); 
                    $$=t1;
                     argcount++;}
	| argument_expression_list ',' assignment_expression{
                 assign_stmt *t1=$1;
                 while(t1->next!=NULL)t1=t1->next; 
                 t1->next=createassignlhsrhs(-1,NULL,$3);
                 t1=t1->next;
                 $$=$1;
             argcount++;}
	;

unary_expression
	: postfix_expression {printf("xxXXX\n");$$=$1;} 
	| INC_OP unary_expression{$$=binaryopnode($2,NULL,PREINC,-1);}
	| DEC_OP unary_expression{$$=binaryopnode($2,NULL,PREDEC,-1);}
	| unary_operator cast_expression{$$=binaryopnode($2,NULL,((tree_expr *)($1))->expr_type,-1);}
	| SIZEOF unary_expression{$$=binaryopnode(NULL,$2,TYPE_SIZEOF,-1);  }
	| SIZEOF '(' type_name ')'{
                  $$=new tree_expr();
                  ((tree_expr *)$$)->tpname=$3;
                  ((tree_expr *)$$)->expr_type=TYPE_SIZEOF;
         }
	| ALIGNOF '(' type_name ')'{            $$=new tree_id();}
	;

unary_operator
	: '&'{$$=new tree_expr(); ((tree_expr *)$$)->expr_type=UNARYAND;}
	| '*'{$$=new tree_expr(); ((tree_expr *)$$)->expr_type=UNARYMUL;}
	| '+'{$$=new tree_expr(); ((tree_expr *)$$)->expr_type=UNARYADD;}
	| '-'{$$=new tree_expr(); ((tree_expr *)$$)->expr_type=UNARYSUB;}
	| '~'{$$=new tree_expr(); ((tree_expr *)$$)->expr_type=COMP;}
	| '!'{$$=new tree_expr(); ((tree_expr *)$$)->expr_type=NOT;}
	;

cast_expression
	: unary_expression {$$=$1;}
	| '(' type_name ')' cast_expression{
		$$=binaryopnode(NULL,$4,CAST_EXPR,0);
		((tree_expr *)$$)->tpname=$2;}
	;

multiplicative_expression
	: cast_expression{printf("castxx\n"); $$=$1;}
	| multiplicative_expression '*' cast_expression {$$= binaryopnode((tree_expr *)$1,(tree_expr *)$3,MUL,-1);}
	| multiplicative_expression '/' cast_expression{$$= binaryopnode((tree_expr *)$1,(tree_expr *)$3,DIV,-1);}
	| multiplicative_expression '%' cast_expression{$$=  binaryopnode((tree_expr *)$1,(tree_expr *)$3,MOD,-1);}
	;

additive_expression
	: multiplicative_expression{$$=$1;}
	| additive_expression '+' multiplicative_expression {$$=  binaryopnode($1,$3,ADD,-1);}
	| additive_expression '-' multiplicative_expression{$$= binaryopnode($1,$3,SUB,-1);}
	;

shift_expression
	: additive_expression{$$=$1;}
	| shift_expression LEFT_OP additive_expression{$$= binaryopnode($1,$3,LSHIFT,-1);}
	| shift_expression RIGHT_OP additive_expression{$$=  binaryopnode($1,$3,RSHIFT,-1);}
	;

relational_expression
	: shift_expression{$$=$1;}
	| relational_expression '<' shift_expression{$$= binaryopnode($1,$3,LT,-1);}
	| relational_expression '>' shift_expression{$$=  binaryopnode($1,$3,GT,-1);}
	| relational_expression LE_OP shift_expression{$$=  binaryopnode($1,$3,LE,-1);}
	| relational_expression GE_OP shift_expression{$$=  binaryopnode($1,$3,GE,-1);}
	;

equality_expression
	: relational_expression{$$=$1;}
	| equality_expression EQ_OP relational_expression{$$=  binaryopnode($1,$3,EQ,-1);}
	| equality_expression NE_OP relational_expression{$$=  binaryopnode($1,$3,NEQ,-1);}
	;

and_expression
	: equality_expression{$$=$1;}
	| and_expression '&' equality_expression{$$=  binaryopnode($1,$3,BAND,-1);}
	;

exclusive_or_expression
	: and_expression{$$=$1;}
	| exclusive_or_expression '^' and_expression{$$=  binaryopnode($1,$3,XOR,-1);}
	;

inclusive_or_expression
	: exclusive_or_expression{$$=$1;}
	| inclusive_or_expression '|' exclusive_or_expression{$$=  binaryopnode($1,$3,BOR,-1);}
	;

logical_and_expression
	: inclusive_or_expression{$$=$1;}
	| logical_and_expression AND_OP inclusive_or_expression{  $$=  binaryopnode($1,$3,AND,-1);}
	;

logical_or_expression
	: logical_and_expression{$$=$1;}
	| logical_or_expression OR_OP logical_and_expression{$$=  binaryopnode($1,$3,OR,-1);}
	;

conditional_expression
	: logical_or_expression{$$=$1;}
	| logical_or_expression '?' expression ':' conditional_expression{
             $$=binaryopnode($1,NULL,COND_OP,0);
             ((tree_expr *)$$)->rhs=binaryopnode(temp3->stassign->rhs,$5,-1,0);
             temp1=temp1->prev;              temp1->next=NULL;                temp3=temp1;
             }
	;

assignment_expression
	: conditional_expression {
		$$=$1;
		assflag=0;
		printf("xx");/*((tree_expr *)$$)->print((tree_expr *)$$);*/
		printf("yy");}
	|  unary_expression assignment_operator assignment_expression{
		fprintf(stderr,"ASSIGN STMT %s \n",((tree_expr *)$1)->name);
		((assign_stmt *)$2)->lhs=(tree_expr *)$1;
		((tree_expr *)$1)->kernel=5;
		((tree_expr *)$3)->kernel=5;
		((assign_stmt *)$2)->rhs=(tree_expr *)$3;
		$$=$2;/*((assign_stmt *)$2)->print1(1);*/$$=$2;
		temp3=createstmt(ASSIGN_STMT,NULL,NULL,LINENO); 
		temp3->stmtno=STMTNO;
		temp3->stassign=$$;
		if(temp1==NULL){temp1=temp3;temp=G1=temp1; }
		 else {printf("ELSE ");
			if(temp1->sttype==ASSIGN_STMT  &&temp1->stmtno==STMTNO && temp1->stassign->lhs!=NULL){ 
				fprintf(stderr,"MULTIASSIGN");/*temp1->print();*/ 
				printf("THEN");/*temp3->print();*/
				temp1->prev->next=temp3; 
				temp3->prev=temp1->prev;
				temp1->prev=temp3;
				temp3->next=temp1;
				temp1=temp3;
				((assign_stmt *)$2)->rhs=NULL;
				printf("END");/*temp3->print();*/
				printf("END1");
			}
			else{             linkstmt(&temp1,temp3);			}
			assflag=1;/*temp3->print();*/
		}
		if(FUNCALL_FLAG==1 && fhead!=NULL)fhead->ass=$2;
		FUNCALL_FLAG=0;}
	;

assignment_operator
	: '='{		$$= createassign(AASSIGN);		arrtop=0;}
	| MUL_ASSIGN{		$$= createassign(AMUL_ASSIGN);}
	| DIV_ASSIGN{		$$= createassign(ADIV_ASSIGN);}
	| MOD_ASSIGN{		$$= createassign(AMOD_ASSIGN);}
	| ADD_ASSIGN{		$$= createassign(AADD_ASSIGN);}
	| RADD_ASSIGN{		$$= createassign(RRADD_ASSIGN);}
	| RMUL_ASSIGN{		$$= createassign(RRMUL_ASSIGN);}
	| SUB_ASSIGN{		$$= createassign(ASUB_ASSIGN);}
	| LEFT_ASSIGN{		$$= createassign(ALEFT_ASSIGN);}
	| RIGHT_ASSIGN{		$$= createassign(ARIGHT_ASSIGN);}
	| AND_ASSIGN{		$$= createassign(AAND_ASSIGN);}
	| XOR_ASSIGN{		$$= createassign(AXOR_ASSIGN);}
	| OR_ASSIGN{		$$= createassign(AOR_ASSIGN);}
	;

expression
	: assignment_expression{
		if(assflag==0  ){
			temp3=createstmt(ASSIGN_STMT,NULL,NULL,LINENO);
			temp3->stassign=createassignlhsrhs(-1,NULL,$1);
			if(temp1)temp1->next=temp3;
			temp3->prev=temp1;
			temp1=temp3;
			$$=new assign_stmt();
			((assign_stmt *)$$)->rhs=$1;
		}else {$$=$1;}}
	| expression ',' assignment_expression{
		if(assflag==0){
			temp3=createstmt(ASSIGN_STMT,NULL,NULL,LINENO);
			temp3->stassign=createassignlhsrhs(-1,NULL,$3); 
linkstmt(&temp1,temp3);
		}temp3->comma=1;}
	;

constant_expression
	: conditional_expression{$$=$1;}	/* with constraints */
	;

declaration
	: declaration_specifiers ';'{
		printf("here\n");
		$$=createdeclstmt($1,NULL,NULL);
		symtableentry *old=NULL;
		if(((tree_typedecl *)$1)->datatype>9)currsymtab->addsymbol((dir_decl *)NULL,(tree_typedecl *)$1);
		temp3=createstmt(DECL_STMT,NULL,NULL,LINENO);
		temp3->stdecl=$$;
		if(temp1==NULL){
			temp1=temp3;
		temp=G1=temp1;
		} else{ linkstmt(&temp1,temp3);}
		}
	| declaration_specifiers init_declarator_list ';'{
		symtableentry *old=NULL;
		currsymtab->addsymbol((dir_decl *)$2,(tree_typedecl *)$1);
        if(((dir_decl *)$2)!=NULL && currsymtab==(&GLOBAL_TABLE)){
dir_decl *dupvar= (dir_decl *)$2;
while(dupvar){
dupvar->dup_name=(char *)malloc(sizeof(char)*100);
for(int i=0;i<100;i++)dupvar->dup_name[i]='\0';
sprintf(dupvar->dup_name,"GPU_%s",dupvar->name);
dupvar=dupvar->nextv;
}}
		$$=createdeclstmt($1,NULL,$2);
		temp3=createstmt(DECL_STMT,NULL,NULL,LINENO);
		temp3->stdecl=$$;
		if(temp1==NULL){
			temp1=temp3;
			G1=temp=temp1;
		} else{ linkstmt(&temp1,temp3);
			printf("TEMP1 NOT NULL\n");
		}
	}
	| static_assert_declaration{$$=new symtableentry();}
	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers{
		tree_typedecl *t1=$1; 
		 t1->next=$2;
		$$=$1;}
	| storage_class_specifier{$$=$1;}
	| type_specifier declaration_specifiers{ 
		tree_typedecl *t1=$1;
		 t1->next=$2;
		$$=$1;}
	| type_specifier{
		$$=$1;
		printf("typespecifier\n");
		if(((tree_typedecl *)$$)->datatype==STRUCTREC_TYPE)printf("STRUCT REC");}
	| type_qualifier declaration_specifiers{
		$$=new tree_typedecl();
		((tree_typedecl *)$$)->name=malloc(sizeof(char)*100); 
		strcpy(((tree_typedecl *)$$)->name,((dir_decl *)$1)->name);
		((tree_typedecl *)$$)->next=$2;
		$$=$1;}
	| type_qualifier{
		$$=new tree_typedecl();
		((tree_typedecl *)$$)->datatype=-1;
		((tree_typedecl *)$$)->name=malloc(sizeof(char)*100);
		 strcpy(((tree_typedecl *)$$)->name,((dir_decl *)$1)->name);}
	| function_specifier declaration_specifiers{
		strcat(((tree_typedecl *)$2)->name,((dir_decl *)$1)->name);
		$$=$2;}
	| function_specifier{
		$$=new tree_typedecl();
		((tree_typedecl *)$$)->name=malloc(sizeof(char)*100);
		 strcpy(((tree_typedecl *)$$)->name,((dir_decl *)$1)->name);}
	| alignment_specifier declaration_specifiers{$$=new tree_typedecl();}
	| alignment_specifier{$$=new tree_typedecl();}
	;

init_declarator_list
	: init_declarator {$$=$1;}
	| init_declarator_list ',' init_declarator{
		dir_decl *t1=$1; while(t1->nextv!=NULL)t1=t1->nextv;
		t1->nextv=$3;
		$$=$1;} 
	;

init_declarator
	: declarator '=' initializer{((dir_decl *)$1)->rhs=$3;$$=$1;}
	| declarator {
		$$=$1;
		((dir_decl *)$1)->rhs=NULL;
		if(!strcmp(((dir_decl *)$1)->name,"common"))fprintf(stderr,"common %d ",LINENO);
	}
	| '<' GPU devno '>'declarator'=' initializer {
		((dir_decl *)$5)->gpu=1;
		((dir_decl *)$5)->rhs=$7;
		$$=$5;}
	| '<' GPU  devno '>'declarator{
		$$=new tree_id();
		((dir_decl *)$5)->gpu=1;
		$$=$5;}
	| '(' IDENTIFIER ')' declarator{
		$$=$4;
		((dir_decl *)$4)->parent=currsymtab->findsymbol($2);
	}
	| '(' IDENTIFIER ')'declarator'=' initializer {
		((dir_decl *)$4)->parent=currsymtab->findsymbol($2);
		((dir_decl *)$4)->rhs=$6;
		$$=$4;}
	;
storage_class_specifier
	: TYPEDEF{	$$= createstoragedef(TYPEDEF_TYPE,TYPEDEF_STR);	}
	| EXTERN{$$= createstoragedef(-1,EXTERN_STR);	}
	| STATIC {$$= createstoragedef(-1,STATIC_STR);		}
	| THREAD_LOCAL{$$=new tree_typedecl();}
	| AUTO{$$= createstoragedef(-1,AUTO_STR);		}
	| REGISTER{$$= createstoragedef(-1,REGISTER_STR);	
		DECLTYPE=$$;	}
	;

type_specifier
	: VOID{		$$= createtypedef(VOID_TYPE); 	}
	| CHAR{ 		$$= createtypedef(CHAR_TYPE); 	}
	| SHORT{ 		$$= createtypedef(SHORT_TYPE); 		}
	| INT { 		$$= createtypedef(INT_TYPE); 	}
	| LONG{ 		$$= createtypedef(LONG_TYPE); 	}
	| FLOAT{ 		$$= createtypedef(FLOAT_TYPE); 		}
	| DOUBLE{ 		$$= createtypedef(DOUBLE_TYPE); 		}
	| SIGNED{ 		$$= createtypedef(SIGNED_TYPE); 		}
	| UNSIGNED{ 		$$= createtypedef(UNSIGNED_TYPE); 		}
	| struct_or_union_specifier{
		$$=$1;
		if(((tree_typedecl *)$$)->datatype==STRUCTREC_TYPE)strcat(((tree_typedecl *)$$)->name," *");}
	| enum_specifier{$$=$1;}
	| BOOL{		$$= createtypedef(BOOL_TYPE);	  }
	| COMPLEX{$$=new tree_typedecl();}
	| IMAGINARY{$$=new tree_typedecl();}	  	/* non-mandated extension */
	| atomic_type_specifier{$$=new tree_typedecl();}
	| TYPEDEF_NAME{
		$$=new tree_typedecl();
		 ((tree_typedecl *)$$)->datatype=TYPEDEF_TNAME;
		((tree_typedecl *)$$)->name=malloc(sizeof(char)*100);
		strcpy(((tree_typedecl *)$$)->name,$1);
	}		/* after it has been defined as such */
        |lib_type_specifier{$$=$1;}
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}'{
		((tree_typedecl *)$1)->list=$3;
		$$=$1;
		((tree_typedecl *)$$)->name=NULL;
		((tree_typedecl *)$$)->def=1;
	}
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'{
		strcat(((tree_typedecl *)$1)->name,$2);
		((tree_typedecl *)$1)->list=$4;
		((tree_typedecl *)$1)->def=1;
		$$=$1;
		printf("STRUCT DECL NAME %s",$2);
		((tree_typedecl *)$$)->vname=malloc(sizeof(char)*100);
		strcpy(((tree_typedecl *)$$)->vname,$2);
	}
	| struct_or_union IDENTIFIER{
		printf("\nSREC NAME\n");
		strcat(((tree_typedecl *)$1)->name,$2);
		((tree_typedecl *)$1)->def=0;
		$$=$1;
		((tree_typedecl *)$$)->vname=malloc(sizeof(char)*100);
		strcpy(((tree_typedecl *)$$)->vname,$2);
	}
	;

struct_or_union
	: STRUCT{$$= createcompounddef(STRUCT_TYPE,"struct ");	}
	| STRUCT_REC{$$= createcompounddef(STRUCTREC_TYPE,"struct");	}
	| UNION{$$= createcompounddef(UNION_TYPE,"union");	}
	;

struct_declaration_list
	: struct_declaration{$$=$1;}
	| struct_declaration_list struct_declaration{
		tree_decl_stmt *t1=$1;
		 while(t1->next!=NULL)t1=t1->next;
		 t1->next=$2;
		$$=$1;
	}
	;

struct_declaration
	: specifier_qualifier_list ';'{
		$$=createdeclstmt($1,NULL,NULL);
		symtableentry *old=NULL;
		if(((tree_typedecl *)$1)->datatype!=STRUCTREC_TYPE)currsymtab->addsymbol((dir_decl *)NULL,(tree_typedecl *)$1);}	/* for anonymous struct/union */
	| specifier_qualifier_list struct_declarator_list ';'{
		$$=createdeclstmt($1,NULL,$2);
		symtableentry *old=NULL;
		if(((tree_typedecl *)$1)->datatype==STRUCT_TYPE||((tree_typedecl *)$1)->datatype==STRUCTREC_TYPE ||  ((tree_typedecl *)$1)->datatype==ENUM_TYPE||((tree_typedecl *)$1)->datatype==UNION_TYPE)
			currsymtab->addsymbol((dir_decl *)$2,(tree_typedecl *)$1);
	}
	| static_assert_declaration{$$=new tree_id();}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list{
		tree_typedecl *t1=$1; 
		t1->next=$2;
		$$=$1;
	}
	| type_specifier {
		$$=$1;
		printf("XXXXXX");
		if(((tree_typedecl *)$$)->datatype==STRUCT_TYPE)
			printf("\n******************NESTED STRUCTURE DECLARATION***************\n");
	}
	| type_qualifier specifier_qualifier_list{
		tree_typedecl *t1=new tree_typedecl();
		t1->name=malloc(sizeof(char)*100);
		 strcpy(t1->name,((dir_decl *)$1)->name);
		t1->next=$2;
		$$=t1;
	}
	| type_qualifier{
		$$=new tree_typedecl();
		((tree_typedecl *)$$)->name=malloc(sizeof(char)*100);
		 strcpy(((tree_typedecl *)$$)->name,((dir_decl *)$1)->name);
		printf("OK\n");
		printf("name=%s ",((tree_typedecl *)$$)->name);
	}
	;

struct_declarator_list
	: struct_declarator{$$=$1;}
	| struct_declarator_list ',' struct_declarator{
		dir_decl *t1=$1; 
		while(t1->nextv!=NULL)t1=t1->nextv;
		 t1->nextv=$3;
		$$=$1;
	}
	;

struct_declarator
	: ':' constant_expression{$$=new tree_id();}
	| declarator ':' constant_expression{
		$$=$1;
		((dir_decl *)$$)->structexpr=$3;
	}
	| declarator{$$=$1;}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'{ 
		printf ("here\n");
tree_typedecl *S= createcompounddef(ENUM_TYPE,NULL);
		 S->def=1;
		S->list=new tree_decl_stmt();
		S->list->dirrhs=$3;
		$$=S;
	}
	| ENUM '{' enumerator_list ',' '}'{ 
tree_typedecl *S= createcompounddef(ENUM_TYPE,NULL);
		S->def=1;
		S->list=new tree_decl_stmt();
		S->list->dirrhs=$3;
		$$=S;
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'{ 
tree_typedecl *S= createcompounddef(ENUM_TYPE,"enum");
		strcat(S->name, $2);
		S->vname=malloc(sizeof(char)*100);
		strcpy(S->vname,$2);
		 S->def=1;
		S->list=new tree_decl_stmt();
		S->list->dirrhs=$4;
		$$=S;
	}
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'{ 
tree_typedecl *S= createcompounddef(ENUM_TYPE,"eunm");
		strcat(S->name, $2);
		 S->name=malloc(sizeof(char)*100);
		S->vname=malloc(sizeof(char)*100);
		strcpy(S->vname,$2);
		S->def=1;
		S->list=new tree_decl_stmt();
		S->list->dirrhs=$4;
		$$=S;
	}
	| ENUM IDENTIFIER { 
tree_typedecl *S= createcompounddef(ENUM_TYPE,"enum");
		strcat(S->name, $2);
		S->vname=malloc(sizeof(char)*100);
		strcpy(S->vname,$2);
		S->def=0;
		S->list=new tree_decl_stmt();
	}
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator{
		dir_decl *t1=$1;
		 while(t1->nextv!=NULL)t1=t1->nextv;
		 t1->nextv=$3;
		 $$=$1;
	}
	;

enumerator	/* identifiers must be flagged as ENUMERATION_CONSTANT */
	: enumeration_constant '=' constant_expression{
		$$=new dir_decl();
		 ((dir_decl *)$$)->name=((tree_expr *)$1)->name;
		 ((dir_decl *)$$)->idrhs=$3;
		((dir_decl *)$$)->ctype=CENUM_TYPE;
	}
	| enumeration_constant{
		$$=new dir_decl();
		((dir_decl *)$$)->name=((tree_expr *)$1)->name;
		((dir_decl *)$$)->idrhs=NULL;
		((dir_decl *)$$)->ctype=CENUM_TYPE;
	}
	;

atomic_type_specifier
	: ATOMIC '(' type_name ')' 
	;

type_qualifier  /* CODE BELOW IS WRONG, use typedecl instead of dir_decl, i guess*/
	: CONST {		$$=createdirdecl("const",0,NULL,0,0,0,0,NULL);
/*	((dir_decl *)($$))->dtype=-1;((dir_decl *)($$))->libdtype=-1;*/
}
	| RESTRICT{		$$=createdirdecl("restrict",0,NULL,0,0,0,0,NULL);	}
	| VOLATILE{		$$=createdirdecl("volatile",0,NULL,0,0,0,0,NULL);	}
	| ATOMIC %prec ATOMIC1 {		$$=createdirdecl("atomic",0,NULL,0,0,0,0,NULL);	}
	;
lib_type_specifier
        : GRAPH {        $$=createlibtypedef(GRAPH_TYPE,NULL);	}
        | EDGE %prec EDGE1 {        $$=createlibtypedef(EDGE_TYPE,NULL);	}
        | POINT %prec POINT1 {        $$=createlibtypedef(POINT_TYPE,NULL);	}
        | EDGE '(' IDENTIFIER ')'  {     $$=createlibtypedef(EDGE_TYPE,$3);	}
        | POINT '(' IDENTIFIER ')' {         $$=createlibtypedef(POINT_TYPE,$3);	}
        | SET {        $$=createlibtypedef(SET_TYPE,NULL);	}
        | COLLECTION {        $$=createlibtypedef(COLLECTION_TYPE,NULL);	}
        ;
list1 
	: alltype{$$=$1;}
	| '['list1']'{
		tree_typedecl *t1=new tree_typedecl();
		t1->next=$2;
		$$=t1;
		t1->d1=((tree_typedecl*)$2)->d1;
	}//FEB 14 last statement
	| EDGE'('IDENTIFIER')'{ $$=createlibtypedef(EDGE_TYPE,$3);	((tree_typedecl *)$$)->ppts=NULL;}
	| GRAPH'('IDENTIFIER')'{ $$=createlibtypedef(GRAPH_TYPE,$3);	((tree_typedecl *)$$)->ppts=NULL;	}
	| POINT'(' IDENTIFIER ')'{ $$=createlibtypedef(POINT_TYPE,$3);	((tree_typedecl *)$$)->ppts=NULL;	}
        ;
alltype
	: VOID{		$$= createtypedef(VOID_TYPE);	 }
	| CHAR{		$$= createtypedef(CHAR_TYPE);	}
	| SHORT{		$$= createtypedef(SHORT_TYPE);	}
	| INT {		$$= createtypedef(INT_TYPE);	}
	| LONG{		$$= createtypedef(LONG_TYPE);		}
	| FLOAT{		$$= createtypedef(FLOAT_TYPE);	}
	| DOUBLE{		$$= createtypedef(DOUBLE_TYPE);	}
	| SIGNED{		$$= createtypedef(SIGNED_TYPE);	}
	| UNSIGNED{		$$= createtypedef(UNSIGNED_TYPE);	}
	| BOOL{		$$= createtypedef(BOOL_TYPE);	}
	| struct_or_union IDENTIFIER {
		strcat(((tree_typedecl *)$1)->name,$2);
		((tree_typedecl *)$1)->def=0;
		$$=$1;
		((tree_typedecl *)$$)->vname=malloc(sizeof(char)*100);
		strcpy(((tree_typedecl *)$$)->vname,$2);
	}
	| struct_or_union IDENTIFIER '(' IDENTIFIER ')'{
		strcat(((tree_typedecl *)$1)->name,$2);
		((tree_typedecl *)$1)->def=0;
		$$=$1;
		((tree_typedecl *)$$)->vname=malloc(sizeof(char)*100);
		strcpy(((tree_typedecl *)$$)->vname,$2);
		((tree_typedecl *)$1)->d1=currsymtab->findsymbol($4);
	}
	| ENUM IDENTIFIER { 
tree_typedecl *S= createcompounddef(ENUM_TYPE,"enum");
		strcat(S->name, $2);
		S->vname=malloc(sizeof(char)*100);
		strcpy(S->vname,$2);
		 S->def=0;
		S->list=new tree_decl_stmt();
	}
	;
function_specifier
	: INLINE{
		$$=new dir_decl();
		((dir_decl *)$$)->name=malloc(sizeof(char)*100); 
		strcpy(((dir_decl *)$$)->name," inline "); 
	}
	| NORETURN{
		 $$=new dir_decl();
		((dir_decl *)$$)->name=malloc(sizeof(char)*100);
		 strcpy(((dir_decl *)$$)->name," noreturn ");
	}
	;

alignment_specifier
	: ALIGNAS '(' type_name ')'
	| ALIGNAS '(' constant_expression ')'
	;

declarator
	: pointer direct_declarator {
		$$= $2;
		 ((dir_decl *)$$)->ptrcnt=$1->ptrflag;
		((dir_decl *)$$)->ptrstring=((dir_decl *)$1)->ptrstring;
		printf("stop\n");
	}
	| direct_declarator {$$=$1;}
	;

direct_declarator
	: IDENTIFIER %prec DIR_DECL {
		$$=new dir_decl();
		((dir_decl *)$$)->name=/*malloc(sizeof(char)*100); strcpy(((dir_decl *)$$)->name,*/$1;
		if(!strcmp($1,"common"))fprintf(stderr,"\nCOMMON %d  %s\n",LINENO,((dir_decl *)$$)->name);
	}
	|  direct_declarator '['list1 ']' {
		((dir_decl *)$1)->tp1=(tree_typedecl *)$3;
	}//FEB 14 last statement
	| '(' declarator ')'{
		dir_decl *t1=$2; 
		while(t1->next!=NULL)t1=t1->next;
		 t1->next=new dir_decl();
		 t1=t1->next;
		((dir_decl *)$2)->open_brack=1;
		t1->close_brack=1;
		$$=$2;
	}
	| direct_declarator '[' ']'{
		dir_decl *t1=$1;
		 while(t1->next!=NULL)t1=t1->next;
		 t1->next=new dir_decl();
		 t1=t1->next; 
		t1->sbrack_flag=1;
		$$=$1;
		t1->ctype=CARR_TYPE;
	}
	| direct_declarator '[' '*' ']'{
		dir_decl *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		 t1->next=new dir_decl();
		t1=t1->next; 
		t1->sbrack_flag=1;
		t1->ptrflag=1;
		$$=$1;
		t1->ctype=CARR_TYPE;
	}
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'{
		dir_decl *t1=$1;	
		 while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,$5,0,1,0,1,$4);
		t1=t1->next; 
		$$=$1;
		t1->ctype=CARR_TYPE; 
	}
	| direct_declarator '[' STATIC assignment_expression ']'{
		dir_decl *t1=$1;
		 while(t1->next!=NULL)t1=t1->next; 
		t1->next=createdirdecl(NULL,0,$4,0,1,0,1,NULL);
		t1=t1->next;
		$$=$1; 
		t1->ctype=CARR_TYPE;
	}
	| direct_declarator '[' type_qualifier_list '*' ']'{
		dir_decl *t1=$1;
		 while(t1->next!=NULL)t1=t1->next; 
		t1->next=createdirdecl(NULL,0,NULL,0,1,0,0,$3);
		t1=t1->next;
		t1->ptrflag=1;
		$$=$1;
		 t1->ctype=CARR_TYPE;
	}
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'{ 
		dir_decl *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,$5,0,1,0,1,$3);
		t1=t1->next;
		$$=$1;
		t1->ctype=CARR_TYPE;
	}
	| direct_declarator '[' type_qualifier_list assignment_expression ']'{ 
		dir_decl *t1=$1;
		 while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,$4,0,1,0,0,$3);
		 t1=t1->next; 
		 $$=$1;
		t1->ctype=CARR_TYPE;
	}
	| direct_declarator '[' type_qualifier_list ']'{
		dir_decl *t1=$1;
		while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,NULL,0,1,0,0,$3);
		t1=t1->next;
		 $$=$1;
		t1->ctype=CARR_TYPE;
	}
	| direct_declarator '[' assignment_expression ']'{
		dir_decl *t1=$1;
		 t1->ctype=CARR_TYPE;
		 while(t1->next!=NULL)t1=t1->next; 
		t1->next=createdirdecl(NULL,0,$3,0,1,0,0,NULL);
		t1=t1->next;
		 $$=$1;
		t1->ctype=CARR_TYPE;
	}
	| direct_declarator '(' parameter_type_list ')'{
		dir_decl *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		 t1->next=new dir_decl();
		 t1=t1->next; 
		 ((dir_decl *)$1)->params=$3;
		 proc_id *t2= new proc_id();
		t2->name=((dir_decl *)$1)->name;
		FDEF=t2;
		t2->params=$3;
		FNAME=new dir_decl();
	FNAME->procd=1; 
//		FNAME->singleflag=SINGLEFLAG;
		FNAME->name=((dir_decl *)$1)->name;
		printf("FAULT");
		FNAME->ctype=CPROC_TYPE;
		((dir_decl *)$1)->brack_flag=2;
		printf("dirdecl funname=%s ",((dir_decl *)$1)->name);
		t2->param_count=param_count;
		t2->vararg=vararg; 
		param_count=vararg=0;
		((dir_decl *)$1)->procd=1;
	//	fnamessingle[((dir_decl *)$1)->name]=SINGLEFLAG;
//                SINGLEFLAG=0;
		$$=$1;
	}
	| direct_declarator'.'IDENTIFIER '(' IDENTIFIER',' parameter_type_list ')' {
		fprintf(stderr,"DIRECT LIBDECLARATOR %s %s %s \n",((dir_decl *)$1)->name,$3,$5);
	}
	| direct_declarator '(' ')'{
		dir_decl *t1=$1;
		t1->brack_flag=2;
		$$=$1;
		((dir_decl *)$$)->procd=1;
		//((dir_decl *)$$)->singleflag=SINGLEFLAG;
               // SINGLEFLAG=0;
	}
	| direct_declarator '(' identifier_list ')'{
		dir_decl *t1=$1;
		 t1->brack_flag=2;
		//t1->procd=1;
	//	t1->singleflag=SINGLEFLAG;
          //      SINGLEFLAG=0;
		t1->parnames=new tree_decl_stmt();
		t1->parnames->dirrhs=$3;
		$$=t1;
	}
	;

pointer
	: '*' type_qualifier_list pointer{
		printf("ONE\n");
		if($3->ptrstring!=NULL){
			strcat(((dir_decl *)$2)->name, $3->ptrstring);
			printf("%s ",((dir_decl *)$2)->ptrstring);
		 }
		char *t1=((dir_decl *)$2)->name;
		 int i=strlen(t1);
		 for(int j=i;j>=1;j--)t1[j]=t1[j-1];
		 t1[0]='*';
		 $$=$3;
		 $$->ptrstring=t1;}
	| '*' type_qualifier_list{
		printf("TWO\n");
		char *t1=((dir_decl *)$2)->name; 
		int i=strlen(t1);
		for(int j=i;j>=1;j--)t1[j]=t1[j-1];
		t1[0]='*';
		$$=new tree_node(); 
		$$->ptrstring=t1;
	}
	| '*' pointer{
		printf("THREE\n");
		$$=$2;
		$2->ptrflag++;
		if($2->ptrstring!=NULL){
			int i=strlen($2->ptrstring);
			int j;
			printf("i=%d",i);
			 i++;
			for(j=i;j>=1;j--){
				printf("for %c",$2->ptrstring[j]);
				$2->ptrstring[j]=$2->ptrstring[j-1];
			}
			 $2->ptrstring[0]='*';
			printf("XX %s",$2->ptrstring);
		   }}
	| '*'{
		printf("FOUR\n");
		$$=new tree_node(); 
		$$->ptrflag++;
		$$->ptrstring=(char *)malloc(sizeof(char)*25); 
		strcpy($$->ptrstring," *");
	}
	;

type_qualifier_list
	: type_qualifier{$$=$1;}
	| type_qualifier_list type_qualifier{		strcat(((dir_decl *)$1)->name,((dir_decl *)$2)->name); 		$$=$1;	}
	;
parameter_type_list
	: parameter_list ',' ELLIPSIS{
		tree_decl_stmt *t1=$1;
		while(t1->next!=NULL)t1=t1->next;
		t1->next=new tree_decl_stmt();
		t1=t1->next;
		t1->lhs=new tree_typedecl();
		t1->lhs->datatype=ELLIPSIS_TYPE;
		$$=$1;
		 t1->lhs->name=malloc(sizeof(char)*100);
		strcpy(t1->lhs->name,dtypenames[t1->lhs->datatype]);vararg=1;
	}
	| parameter_list{$$=$1;}
	;

parameter_list
	: parameter_declaration{
		$$=$1;
		((tree_decl_stmt *)$$)->next=NULL;
		printf("FIRST\n");
		param_count++;
	}
	| parameter_list ',' parameter_declaration{
		tree_decl_stmt *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		t1->next=(tree_decl_stmt *)$3;
		$$=$1;
		printf("MORE THAN ONE\n");
		param_count++;
		$$=$1;
	}
	;

parameter_declaration
	: declaration_specifiers declarator{ 		$$=createdeclstmt($1,NULL,$2);	}
	| declaration_specifiers '<' GPU devno '>'declarator{		 $$=createdeclstmt($1,NULL,$6);
((dir_decl *)($6))->gpu=1;
((dir_decl *)($6))->arg=1;
	}
	| declaration_specifiers abstract_declarator{		$$=createdeclstmt($1,NULL,$2);
((dir_decl *)($2))->arg=1;
	}
	| declaration_specifiers{		$$=createdeclstmt($1,NULL,NULL);	}
	;

identifier_list
	: '<'GPU devno '>'IDENTIFIER{
		$$=createdirdecl($5,1,NULL,0,0,0,0,NULL);
	}
	| IDENTIFIER{
		$$=createdirdecl($1,0,NULL,0,0,0,0,NULL);
	}
	| identifier_list ',' IDENTIFIER{
		dir_decl *t1=$1; 
		while(t1->nextv!=NULL)t1=t1->nextv; 
		t1->nextv=new dir_decl();
		t1=t1->nextv;
		t1->name=malloc(sizeof(char)*100);
		 strcpy(t1->name,$3);
		$$=$1;
		t1->nextv=NULL;
	}
	;
devno : {$$=new tree_expr();}
      | ',' I_CONSTANT{$$=new tree_expr();}
      ;
type_name
	: specifier_qualifier_list abstract_declarator{
		printf("TYPE_NAME= SQL=%s  ADCT=",((tree_typedecl *)$1)->name);
		((dir_decl *)$2)->print();
		$$=new type_name();
		 ((type_name *)$$)->typedecl=$1;
		 ((type_name *)$$)->decl=$2;
	}
	| specifier_qualifier_list {
		/*printf("TYPE_NAME= SQL=%s  ADCT=NULL",((tree_typedecl *)$1)->name);*/
		$$=new type_name();
		 ((type_name *)$$)->typedecl=$1;
	}
	;

abstract_declarator
	: pointer direct_abstract_declarator{
		$$= $2; 
		((dir_decl *)$$)->ptrcnt=$1->ptrflag;
		((dir_decl *)$$)->ptrstring=((dir_decl *)$1)->ptrstring;
		printf("stop\n");
	}
	| pointer{
		$$=new dir_decl(); 
		((dir_decl *)$$)->ptrstring=$1->ptrstring;
		((dir_decl *)$$)->ptrcnt=1;
	}
	| direct_abstract_declarator{$$=$1;}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'{
		dir_decl *t1=new dir_decl();
		t1->brack_flag=2;
		 t1->next=$2;
		$$=t1; 
	}
	| '[' ']'{
		dir_decl *t1=new dir_decl();
		t1->sbrack_flag=1;
		$$=t1;
	}
	| '[' '*' ']'{
		dir_decl *t1=new dir_decl();
		t1->sbrack_flag=1;
		t1->ptrflag=1;
		$$=t1;
	}
	| '[' STATIC type_qualifier_list assignment_expression ']'{
		dir_decl *t1=createdirdecl(NULL,0,$4,0,1,0,1,$3);
		$$=t1;
	}
	| '[' STATIC assignment_expression ']'{ 
		dir_decl *t1=createdirdecl(NULL,0,$3,0,1,0,1,NULL);
		$$=t1;  
	}
	| '[' type_qualifier_list STATIC assignment_expression ']'{ 
		dir_decl *t1=createdirdecl(NULL,0,$4,0,1,0,1,$2);
		$$=t1;  
	}
	| '[' type_qualifier_list assignment_expression ']'{
		dir_decl *t1=createdirdecl(NULL,0,$3,0,1,0,0,$2);
		$$=t1; 
	}
	| '[' type_qualifier_list ']'{ 
		dir_decl *t1=createdirdecl(NULL,0,NULL,0,1,0,0,$2);
		$$=t1;
	}
	| '[' assignment_expression ']'{ 
		dir_decl *t1=createdirdecl(NULL,0,$2,0,1,0,0,NULL);
		$$=t1;
	  }
	| direct_abstract_declarator '[' ']'{
		dir_decl *t1=$1;
		 while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,NULL,0,1,0,0,NULL);
		 t1=t1->next;
		$$=$1;
	}
	| direct_abstract_declarator '[' '*' ']'{
		dir_decl *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		t1->next=new dir_decl();
		t1->next=createdirdecl(NULL,0,NULL,0,1,0,0,NULL);
		t1=t1->next; 
		t1->ptrflag=1;
		$$=$1;
	}
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'{ 
		    dir_decl *t1=$1; 
		    while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,$5,0,1,0,1,$4);
                    t1=t1->next;
                    $$=$1;
	 }
	| direct_abstract_declarator '[' STATIC assignment_expression ']'{ 
		dir_decl *t1=$1;
		 while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,$4,0,1,0,1,NULL);
		t1=t1->next;
		$$=$1;
	}
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']' { 
		dir_decl *t1=$1;
		 while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,$4,0,1,0,0,$3);
		t1=t1->next; 
		$$=$1;
	}
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'{
		dir_decl *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,$5,0,1,0,1,$3);
		t1=t1->next;
		$$=$1;
	}
	| direct_abstract_declarator '[' type_qualifier_list ']'{ 
		dir_decl *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,NULL,0,1,0,0,$3);
		t1=t1->next; 
		$$=$1;
	}
	| direct_abstract_declarator '[' assignment_expression ']'{
		dir_decl *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		t1->next=createdirdecl(NULL,0,$3,0,1,0,0,$3);
		t1=t1->next;
		$$=$1;
	}
	| '(' ')'{
		dir_decl *t1=new dir_decl();
		t1->brack_flag=2;
		$$=t1;
	}
	| '(' parameter_type_list ')'{ 
		dir_decl *t1=new dir_decl();
		t1->params=$2;
		t1->brack_flag=2;
		$$=t1;
	}
	| direct_abstract_declarator '(' ')'{
		dir_decl *t1=$1;
		 while(t1->next!=NULL)t1=t1->next;
		t1->next=new dir_decl();
		t1=t1->next;
		t1->brack_flag==2;
		$$=$1;
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
		dir_decl *t1=$1;
		 while(t1->next!=NULL)
		t1=t1->next;
		t1->next=new dir_decl();
		t1=t1->next;
		t1->params=$3;
		t1->brack_flag=2;
		$$=$1;
	}
	;

initializer
	: '{' initializer_list '}' {
		$$=new tree_expr();
		 ((tree_expr*)$$)->expr_type=INITBLOCK;
		 ((tree_expr*)$$)->rhs=$2;
	}
	| '{' initializer_list ',' '}'{
		$$=new tree_expr();
		 ((tree_expr*)$$)->expr_type=INITBLOCK;
		 ((tree_expr*)$$)->rhs=$2;
	}
	| assignment_expression {$$=$1;}
	;

initializer_list
	: designation initializer{
		((tree_expr *)$2)->desi=$1;
		$$=$2;
	}
	| initializer {$$=$1;}
	| initializer_list ',' designation initializer{
		 ((tree_expr *)$1)->next=$4; 
		((tree_expr *)$4)->prev=$1;
		$$=$4;
		  ((tree_expr *)$4)->desi=$3;
	  }
	| initializer_list ',' initializer{ 
		((tree_expr *)$1)->next=$3; 
		((tree_expr *)$3)->prev=$1;
		$$=$3;
	}
	;

designation
	: designator_list '='
	;

designator_list
	: designator{$$=$1;}
	| designator_list designator{
		designator *t1=$1;
		while(t1->next!=NULL)t1=t1->next;
		 t1->next=$2;
		 $$=$1;
	}
	;

designator
	: '[' constant_expression ']'{
		$$=new designator(); 
		((designator *)$$)->expr=$2;
	}
	| '.' IDENTIFIER{
		$$=new designator();
		 ((designator *)$$)->name=$2;
	}
	;

static_assert_declaration
	: STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';'
	;

statement
	: labeled_statement{$$=new tree_node();}
	| compound_statement{
		printf("NEW COMOUND STATEMENT\n");
		$$=sblock_begin[sbtop];
	}
	| expression_statement{
		$$=$1;
		printf("HERE");
	}
	| selection_statement{$$=new tree_node();((tree_node *)$$)->nodetype=TREE_IF;}
	| single_statement{$$=new tree_node();}
	| iteration_statement{
		$$=new tree_node() ;
		printf("end of while\n");
	}
	| jump_statement{$$=new tree_node();}
        | parallel_statement { }
        | macro_statement{$$=$1;  }
        | error {$$=new tree_node();}
	;
parallel_statement
        : PARALLEL SECTIONS compound_begin sections compound_end { 
		$3=sblock_begin[sbtop];
	       ((statement *)$3)->sttype=SECTIONS_STMT;
       }
	;
sections: SECTION compound_statement  { 
		$2=sblock_begin[sbtop];
		((statement *)$2)->sttype=SECTION_STMT;
}
       | sections SECTION compound_statement { 
		$3=sblock_begin[sbtop];
		((statement *)$3)->sttype=SECTION_STMT;
}
       ;
macro_statement
	: HASH_DEFINE WHITE_SPACE   postfix_expression  WHITE_SPACE postfix_expression %prec MACRO1{ 
		temp3=createstmt(MACRO_STMT,$3,NULL,LINENO); 
		temp3->expr2=$5;
		if(temp1)temp1->next=temp3;
		 temp1=temp3;
		$$=temp3;
	}
	;
labeled_statement
	: IDENTIFIER ':' statement{
		statement *t4=createstmt(LABELED_STMT,NULL,$1,LINENO);
		if(temp3->sttype==EBLOCK_STMT){
			 statement *t2=$3;
			 t2->prev->next=t4;
			t4->next=t2;
		}else {
		          t4->next=temp3;
			  temp3->prev->next=t4;
		 }
		 temp3=t4;
	}
	| CASE constant_expression ':' statement {
		statement *t4=createstmt(CASE_STMT,$2,NULL,LINENO);
		  if(temp3->sttype==EBLOCK_STMT){
			 statement *t2=$4;
			 t2->prev->next=t4;
			t4->next=t2;
		}else {
		          t4->next=temp3;; 
			  temp3->prev->next=t4;
		 }
		 temp3=t4;
	}
	| DEFAULT ':' statement{
		statement *t4=createstmt(DEFAULT_STMT,NULL,NULL,LINENO);
		if(temp3->sttype==EBLOCK_STMT){
			 statement *t2=$3; 
			t4->next=t2;
			t2->prev->next=t4;
		}else {
		          t4->next=temp3;
			 temp3->prev->next=t4;
		 }
		 temp3=t4;
	}
	;

compound_statement
	: compound_begin compound_end{$$=new tree_node(); }
	| compound_begin   block_item_list compound_end{
		printf("HERE");
		$$=new tree_node();
	}
	;
compound_begin
        : '{'{
		printf("NEW COMPOUND BEGIN");
		temp3=createstmt(SBLOCK_STMT,NULL,NULL,LINENO);
		if(temp1!=NULL){linkstmt(&temp1,temp3);
		}else {
			temp1=temp=temp3;
			printf("TEMP1 is NULL\n");
		}
		stemp=new globsymtable();
		stemp->parent=currsymtab;
		stemp->sym->parent=currsymtab->sym;
		currsymtab=stemp;
		sblock_begin[sbtop++]=temp3;
		if(FDEF!=NULL){
			if(FDEF->params!=NULL){
				printf("ADDING\n");
				FDEF->addsymbols(currsymtab);
			}
			FDEF=NULL;
			symtableentry *old=NULL;
			if(FTYPE==NULL){
				FTYPE=new tree_typedecl();
				 FTYPE->datatype=VOID_TYPE;
			}
			printf("FNAME=%s ",FNAME->name);
			currsymtab->parent->addsymbol(FNAME,FTYPE);
                        
			printf("added function name\n");
		}}
        ;
compound_end
        : '}'{
		printf("NEW COMPOUND END\n");
		temp3=createstmt(EBLOCK_STMT,NULL,NULL,LINENO);
		if(temp1!=NULL) temp1->next=temp3;
		temp3->prev=temp1;temp3->next=NULL;
		temp1=temp3;
		currsymtab=currsymtab->parent;sbtop--;
		}
        ;
b_compound_begin
        : '(' {
		stemp=new globsymtable();
		stemp->parent=currsymtab;
		stemp->sym->parent=currsymtab->sym;
		currsymtab=stemp; 
	}
block_item_list
	: block_item{
		if(temp==NULL){
			G1=temp1;
			flag=1;
			temp=temp1;
			temp->sttype=SBLOCK_STMT;
			$$=$1;
		}else
		 $$=$1;
	}
	| block_item_list block_item {/*((statement *)$1)->next=$2;*/ $$=$2;}
	;

block_item
	: declaration{$$=$1;}
	| statement { $$=$1;}
	;

expression_statement
	: ';' {		$$=createassign(EMPTY_ASSIGN);		}
	| expression ';'{
		$$=$1;
		while(temp1->next!=NULL&&temp1->sttype==ASSIGN_STMT&& temp1->stmtno==temp1->next->stmtno){printf("ASS \n");
		temp1=temp1->next;
		}
		temp1->stassign->semi=1;
		STMTNO++;
		FUNCALL_FLAG=0;
	}
	;
single_statement 
	: SINGLE '(' expression ')' statement %prec THEN {
		statement *t4=createstmt(SINGLE_STMT,((assign_stmt *)$3)->rhs,NULL,LINENO);
                SINGLEFLAG=1;
		t4->stassign=(assign_stmt *)$3;
	       tree_expr *bar=t4->expr1;
		 if(bar->expr_type==VAR && bar->libdtype==COLLECTION_TYPE)barrier=1;
		statement *t2=(statement *)$5; 
        createsinglestmt1(&t4,&t2,&temp3);
}
     
        | SINGLE '(' expression ')' statement ELSE statement{
		statement *t4=createstmt(SINGLE_STMT,((assign_stmt *)$3)->rhs,NULL,LINENO);
                SINGLEFLAG=1;
		t4->stassign=(assign_stmt *)$3;
	       tree_expr *bar=t4->expr1;
		 if(bar->expr_type==VAR && bar->libdtype==COLLECTION_TYPE)barrier=1;
		statement *t2=(statement *)$5; 
		statement *t1=(statement *)$7; 
        createsinglestmt(&t4,&t1,&t2,&temp3);
}
        ;
selection_statement
        : IF '(' expression ')' statement ELSE statement {  
		statement *t4=createstmt(IF_STMT,((assign_stmt *)($3))->rhs,NULL,LINENO);
statement *t1=$7;
statement *t2=$5;
fprintf(FP1,"/*IFSTMT*/");
if(t1->nodetype==TREE_IF)
fprintf(FP1,"//if else if \n");
createifstmt(&t4,&t1,&t2,&temp3,1);
if(temp1->sttype==IF_STMT)fprintf(FP1,"//IF ELSE IF \n");
char arr[100];
((assign_stmt *)($3))->printcode1(arr,1);
fprintf(FP1,"%s \n",arr);
	}
| IF '(' expression ')' statement %prec THEN{
		statement *t4=createstmt(IF_STMT,((assign_stmt *)($3))->rhs,NULL,LINENO);
		statement *t2=(statement *)$5;
createifstmt(&t4,NULL,&t2,&temp3,0);
	}
        | SWITCH '(' expression ')' statement{
		statement *t4=createstmt(SWITCH_STMT,((assign_stmt *)($3))->rhs,NULL,LINENO);
		t4->f1=((statement *)$5);
		((statement *)$5)->prev->prev->next=t4;
		temp3=t4;/*statement *t2=(statement *)$5; t2->prev->next=temp3;((assign_stmt *)$3)->rhs->pflag=100;temp1=temp3;*/		 FUNCALL_FLAG=0;
	}



iteration_statement
	: WHILE '(' expression ')' statement {
		 statement *t1;
		if(temp3->sttype==EBLOCK_STMT){
			t1=sblock_begin[sbtop];
			t1->sttype=WHILE_STMT;
			t1->expr1=((assign_stmt *)$3)->rhs;
		}else{
			 temp3->sttype=WHILE_STMT;
			 temp3->expr1=((assign_stmt *)$3)->rhs;
		}
		((assign_stmt *)$3)->rhs->pflag=100;
	}
	| DO statement WHILE '(' expression ')' ';'{
		$$=new tree_node();
		 statement *t1=sblock_begin[sbtop];
		t1->sttype=DOWHILE_STMT;
		statement *t4=createstmt(DOWHILEEXPR_STMT,((assign_stmt *)($5))->rhs,NULL,LINENO);
linkstmt(&temp1,temp3);
		((assign_stmt *)$5)->rhs->pflag=100;
		FUNCALL_FLAG=0;
	}
	| FOR '(' expression_statement expression_statement ')' statement{
		if(temp3->sttype==EBLOCK_STMT){
			 temp3->sttype=FOR_EBLOCK_STMT;
			statement *t1=sblock_begin[sbtop];
initforstmt1(&t1,(assign_stmt *)$3,(assign_stmt *)$4,(assign_stmt *)NULL);
			/*t1->print();*/statement *t2=t1;
			int i=0;int cnt=0; 
			if(((assign_stmt *)$3)->asstype!=EMPTY_ASSIGN)cnt++;
			if(((assign_stmt *)$4)->asstype!=EMPTY_ASSIGN)cnt++;
			 while(i<=cnt){
				i++;
				t2=t2->prev;
			}
			t2->next=t1; 
			t1->prev=t2;
              }else {
initforstmt1(&temp3,(assign_stmt *)$3,(assign_stmt *)$4,(assign_stmt *)NULL);
			statement *t2=temp3;
			int i=0; int cnt=0;
			 if(((assign_stmt *)$3)->asstype!=EMPTY_ASSIGN)cnt++;
			 if(((assign_stmt *)$4)->asstype!=EMPTY_ASSIGN)cnt++;
			 while(i<=cnt){i++;t2=t2->prev;}
			 t2->next=temp3; 
			temp3->prev=t2;
		}
		FUNCALL_FLAG=0;
}
	| FOR '(' expression_statement expression_statement expression ')' statement{
		if(temp3->sttype==EBLOCK_STMT){
			 temp3->sttype=FOR_EBLOCK_STMT;
			statement *t1=sblock_begin[sbtop];
initforstmt1(&t1,(assign_stmt *)$3,(assign_stmt *)$4,(assign_stmt *)$5);
			/*t1->print();*/
			statement *t2=t1; 
			int i=0; int cnt=0; 
			if(((assign_stmt *)$3)->asstype!=EMPTY_ASSIGN)cnt++;
			if(((assign_stmt *)$4)->asstype!=EMPTY_ASSIGN)cnt++;
			while(i<=cnt+1){
				i++;
				t2=t2->prev;
			}
			t2->next=t1;
			 t1->prev=t2;
        	}else {
initforstmt1(&temp3,(assign_stmt *)$3,(assign_stmt *)$4,(assign_stmt *)$5);
			  statement *t2=temp3;
			  int i=0; int cnt=0; 
			  if(((assign_stmt *)$3)->asstype!=EMPTY_ASSIGN)cnt++;
			  if(((assign_stmt *)$4)->asstype!=EMPTY_ASSIGN)cnt++;
			  while(i<=cnt+1){
					i++;
					t2=t2->prev;
			}
			t2->next=temp3;
			 temp3->prev=t2;
		}
		FUNCALL_FLAG=0;
	}
	| FOR b_compound_begin	  declaration expression_statement ')' statement{
		if(temp3->sttype==EBLOCK_STMT){ 
			temp3->sttype=FOR_EBLOCK_STMT;
			statement *t1=sblock_begin[sbtop];
initforstmt(&t1,(tree_decl_stmt *)$3,(assign_stmt *)$4,(assign_stmt *)NULL);
			statement *t2=t1;
			 int i=0;
			 int cnt=0; 
			if(((assign_stmt *)$4)->asstype!=EMPTY_ASSIGN)cnt++;
			 while(i<=cnt+1){
				i++;
				t2=t2->prev;
			}
			t2->next=t1;
			 t1->prev=t2;
		}else {
initforstmt(&temp3,(tree_decl_stmt *)$3,(assign_stmt *)$4,(assign_stmt *)NULL);
			statement *t2=temp3;
			int i=0; 
			int cnt=0;
			 if(((assign_stmt *)$4)->asstype!=EMPTY_ASSIGN)cnt++;
			while(i<=cnt+1){
				i++;
				t2=t2->prev;
			}
			t2->next=temp3;
			 temp3->prev=t2;
		}
		currsymtab=currsymtab->parent;
		FUNCALL_FLAG=0;
}
	| FOR b_compound_begin		  declaration expression_statement expression ')' statement{
		if(temp3->sttype==EBLOCK_STMT){
			 temp3->sttype=FOR_EBLOCK_STMT;
			 statement *t1=sblock_begin[sbtop];
initforstmt(&t1,(tree_decl_stmt *)$3,(assign_stmt *)$4,(assign_stmt *)$5);
			 statement *t2=t1;
			 int i=0; int cnt=0;
			 if(((assign_stmt *)$4)->asstype!=EMPTY_ASSIGN)cnt++;
			 while(i<=cnt+2){
				i++;
				t2=t2->prev;
			}
			t2->next=t1;
			 t1->prev=t2;
	     }else { 
initforstmt(&temp3,(tree_decl_stmt *)$3,(assign_stmt *)$4,(assign_stmt *)$5);
			statement *t2=temp3;
			int i=0;
			int cnt=0; 
			if(((assign_stmt *)$4)->asstype!=EMPTY_ASSIGN)cnt++;
			while(i<=cnt+2){
				i++;
				t2=t2->prev;
			}
			t2->next=temp3;
			 temp3->prev=t2;
	}
	currsymtab=currsymtab->parent;
	FUNCALL_FLAG=0;
}
        | FOREACH b_compound_begin IDENTIFIER IN IDENTIFIER'.'iterator ')' 
{
		dir_decl *d1=createdirdeclforeach((char *)$3,(char *)$5,ITERATOR_TYPE,-1,-1,(int)$7);
		} 

/*statement */ conditional_for
  {		if(temp3->sttype==EBLOCK_STMT){
			statement *t1=sblock_begin[sbtop];
                        if(temp3->expr4!=NULL){
				  temp3->expr4->print(temp3->expr4);      
				  fnamescond[t1->next->stassign->rhs->name]=temp1;
			}
			 temp3->sttype=FOREACH_EBLOCK_STMT;
                         t1->expr4=temp3->expr4; 
			 t1->expr5=$4;  
		         t1->sttype=FOREACH_STMT;
                FOREACH_FUN_FLAG=1;
			 t1->feb=1;
  initforeach1(&t1,(char *)$3,(char *)$5,(int)$7);
		}else {
if(temp1->expr4!=NULL){
				fnamescond[temp1->stassign->rhs->name]=temp1;
			 }
            temp3=createstmt(FOREACH_STMT,NULL,NULL,LINENO);
                FOREACH_FUN_FLAG=1;
			temp3->expr5=$4;
  initforeach1(&temp3,(char *)$3,(char *)$5,(int)$7);
			  temp3->prev=temp1->prev;
			 temp3->expr4=temp1->expr4;
			temp3->stassign=temp1->stassign;
			fnamescond[temp1->stassign->rhs->name]=temp3;
	    	//		fprintf(FP1," newiterator %s %d %d \n", temp3->stassign->rhs->name,temp3->itr,LINENO);
			temp3->prev->next=temp3;temp1=temp3;
	  }
	currsymtab=currsymtab->parent;KERNEL=0;
	FUNCALL_FLAG=0;}
        | FOREACH b_compound_begin IDENTIFIER IN IDENTIFIER ')' 
{
		dir_decl *d1=createdirdeclforeach((char *)$3,(char *)$5,ITERATOR_TYPE,-1,-1,(int)5);
                
		}conditional_for
  {             if(temp3->sttype==EBLOCK_STMT){
                	 temp3->sttype=FOREACH_EBLOCK_STMT;
                        statement *t1=sblock_begin[sbtop];
                        t1->expr5=$4;
			t1->feb=1;
                FOREACH_FUN_FLAG=1;
                        t1->sttype=FOREACH_STMT;
  initforeach(&t1,&temp3,(char *)$3,(char *)$5,5);
		}else {
            temp3=createstmt(FOREACH_STMT,NULL,NULL,LINENO);
                FOREACH_FUN_FLAG=1;
			 temp3->expr5=$4; 
  initforeach(&temp3,&temp3,(char *)$3,(char *)$5,5);
		temp3->prev=temp1->prev; 
			temp3->stassign=temp1->stassign;
			 temp3->prev->next=temp3;
			temp1=temp3; 
	     }
	    currsymtab=currsymtab->parent;
	    KERNEL=0;
            FUNCALL_FLAG=0;
} { }

	;
    IN  :  '(' '+' IDENTIFIER ')'    IN1   { 
		$$=new tree_expr(); 
		dir_decl *x1=currsymtab->findsymbol($3);
		((tree_expr *)$$)->rhs=new tree_expr(x1);
		((tree_expr *)($$))->expr_type=VAR;
	}
        |  '(' '-' IDENTIFIER ')'    IN1   { $$=$5;}
        | IN1 {
		$$=$1;
		fprintf(stderr,"\n normal foreach\n");
	}
        ;
IN1     : INOLD {
		fprintf(stderr,"INOLD");
		$$=NULL;
//new tree_expr(); ((tree_expr *)($$))->name=malloc(sizeof(char)*10);strcpy(((tree_expr *)$$)->name,"In");
}
	|  IN1  '*' { 
		$1=new tree_expr(); 
		((tree_expr *)($1))->name=malloc(sizeof(char)*10);
		strcpy(((tree_expr *)$1)->name,"In");
		((tree_expr *)($1))->ptrflag++;$$=$1;
	}
        ;
iterator: IDENTIFIER{ 
		fprintf(stderr,"ITERATOR FOUND");
        	if((!strcmp($1,"points"))&&(strlen($1)==strlen("points")))$$=0;
    		else if((!strcmp($1,"edges"))&&(strlen($1)==strlen("edges")))$$=1;
		else if((!strcmp($1,"nbrs"))&&(strlen($1)==strlen("nbrs"))){
				$$=2;
				OUTNBRFLAG=INNBRFLAG=1;
		}
                 else if((!strcmp($1,"innbrs"))&&(strlen($1)==strlen("innbrs"))){
				$$=3;
				INNBRFLAG=1;
				//utflag=a;
		}
               else if((!strcmp($1,"outnbrs"))&&(strlen($1)==strlen("outnbrs"))){
			$$=4;
			OUTNBRFLAG=1;
		}
		 else if((!strcmp($1,"items"))&&(strlen($1)==strlen("items"))){
			$$=5;
		//	OUTNBRFLAG=1;
		}
 		 else {
		       int flag=0;
			for (std::map<char *,int>::iterator it=newitr.begin(); it!=newitr.end(); ++it){
			          fprintf(stderr,"loop\n");
	    			fprintf(FP1," newiterator %s %d %d \n", it->first,it->second,LINENO);
				if(!strcmp(it->first,$1)){$$=it->second;flag=1;}
			}
                       tempitr=malloc(sizeof(char)*100);
			 strcpy(tempitr,$1);
		}
	}
	;

jump_statement
	: GOTO IDENTIFIER ';' {		 temp3=createstmt(GOTO_STMT,NULL,$2,LINENO);
             linkstmt(&temp1,temp3);
			$$=new tree_node();
	}
	| CONTINUE ';'{ 		 temp3=createstmt(CONTINUE_STMT,NULL,NULL,LINENO);
             linkstmt(&temp1,temp3);
		$$=new tree_node();
	}
	| BREAK ';' { 		 temp3=createstmt(BREAK_STMT,NULL,NULL,LINENO);
             linkstmt(&temp1,temp3);
		$$=new tree_node();
	}
	| RETURN ';'{		 temp3=createstmt(RETURN_STMT,NULL,NULL,LINENO);
             linkstmt(&temp1,temp3);
		$$=new tree_node();
	}
	| RETURN expression ';'{ 		 temp3=createstmt(RETURN_STMT,((assign_stmt *)$2)->rhs,NULL,LINENO);
		 temp1->prev->next=temp3;
		 temp3->prev=temp1->prev;
		 temp1=temp3;
		$$=new tree_node();
	}
	;

conditional_for: '('expression ')'  statement{  } 
  {		if(temp3->sttype==EBLOCK_STMT){ 
                                   //                       temp3->sttype=FOREACH_EBLOCK_STMT;
                        temp3->expr4=((assign_stmt *)$2)->rhs;
			statement *t1=sblock_begin[sbtop]; //top statement of compound block
                        statement * t4=t1->prev;//its prev
                        t4->prev->next=t1;//make t4 deleted
                        t1->prev=t4->prev;
                        t4->next=NULL;
                }else {
                         temp3->expr4=((assign_stmt *)$2)->rhs;
                         temp3->prev=temp3->prev->prev;
                         temp3->expr4=((assign_stmt *)$2)->rhs;
                         temp3->prev->next->next=NULL;
                         temp3->prev->next=temp3; 
                         temp1=temp3;  
                                                   
		}
} 
               |  statement  
               ;
translation_unit
	: external_declaration{$$=$1;}
	| translation_unit external_declaration{$$=$2;}
	;

external_declaration
	: function_definition {
		currsymtab=&GLOBAL_TABLE;
		$$=new tree_node();  
		/*getchar();printf("\n**********\n");temp->print();printf("\n**********\n");*/ 
		statement *t1=sblock_begin[sbtop]; 
		fnames[t1->prev->stdecl->dirrhs->name]=t1->prev;
        t1->prev->foreachflag=FOREACH_FUN_FLAG;
        FOREACH_FUN_FLAG=0;
		printf("\n *********fname=%s\n*******",t1->prev->stdecl->dirrhs->name);
	}
	| declaration{}
        | macro_statement{ }
	;

function_definition
	: declaration_specifiers declarator declaration_list{de2=temp3;} compound_statement{
		 if(temp3->sttype==EBLOCK_STMT)
			//fprintf(FP1," //EBLOCK STMT\n");
		//fprintf(FP1,"//%d \n",temp3->sttype);
		temp3->sttype=FUNCTION_EBLOCK_STMT;
		printf("FUNCTION\n");
		statement *t1=new statement();
		t1->stdecl=createdeclstmt($1,NULL,$2);
		((dir_decl *)$2)->procd=1;
		((dir_decl *)$2)->singleflag=SINGLEFLAG;
fnamessingle[((dir_decl *)$2)->name]=SINGLEFLAG;
                SINGLEFLAG=0;
		t1->sttype=DECL_STMT;
		t1->flist=fhead; fhead=NULL;
		statement *t2=sblock_begin[sbtop]; 
		t1->next=de1; 
		if(de1->prev!=NULL)de1->prev->next=t1;
		 else temp=t1;
		 de1->prev=t1; 
		de2->next=t2; 
		t2->prev=de2;
		t1->lineno=LINENO;
}
	| declaration_specifiers declarator compound_statement{
		printf("FUNCTIONXX\n");
		statement *t1=new statement();
//		if(temp3->sttype==EBLOCK_STMT)fprintf(FP1," //EBLOCK STMT\n");
		//fprintf(FP1,"//%d \n",temp3->sttype);
		temp3->sttype=FUNCTION_EBLOCK_STMT;
		if(barrier==1){
			t1->barrier=1;
			barrier=0;
			dir_decl *dd=$2;
			if(dd->params!=NULL ) {
				tree_decl_stmt *dd1=dd->params;
				 while(dd1->next!=NULL)dd1=dd1->next;
				 dd1->next=new tree_decl_stmt();
				 dd1=dd1->next;
				 dd1->lhs=new tree_typedecl(); 
				dd1->lhs->datatype=INT_TYPE;
				dd1->lhs->name=malloc(sizeof(char)*100);
				strcpy(dd1->lhs->name,dtypenames[dd1->lhs->datatype]);
				dd1->dirrhs=new dir_decl();
				dd1->dirrhs->name=malloc(sizeof(char)*100); 
				strcpy(dd1->dirrhs->name,"nblocks");
				dd1->next=new tree_decl_stmt();
				dd1=dd1->next;
				dd1->lhs=new tree_typedecl();
				 dd1->lhs->datatype=INT_TYPE;
				dd1->lhs->name=malloc(sizeof(char)*100);
				strcpy(dd1->lhs->name,dtypenames[dd1->lhs->datatype]);
				dd1->dirrhs=new dir_decl();
				dd1->dirrhs->name=malloc(sizeof(char)*100);
			       strcpy(dd1->dirrhs->name,"size");
			       dd1->next=NULL;
			       dd1->next=new tree_decl_stmt();
				dd1=dd1->next;
				dd1->lhs=new tree_typedecl();
				 dd1->lhs->datatype=INT_TYPE;
				dd1->lhs->name=malloc(sizeof(char)*100);
				strcpy(dd1->lhs->name,"int ");
				dd1->dirrhs=new dir_decl();
				dd1->dirrhs->name=malloc(sizeof(char)*100);
				 strcpy(dd1->dirrhs->name,"goal");
				dd1->next=new tree_decl_stmt();
				dd1=dd1->next;
				dd1->lhs=new tree_typedecl();
				 dd1->lhs->datatype=INT_TYPE;
				dd1->lhs->name=malloc(sizeof(char)*100);
				strcpy(dd1->lhs->name,"unsigned volatile int * ");
				dd1->dirrhs=new dir_decl();
				dd1->dirrhs->name=malloc(sizeof(char)*100); 
				strcpy(dd1->dirrhs->name,"arrayin");
				dd1->next=new tree_decl_stmt();
				dd1=dd1->next;
				dd1->lhs=new tree_typedecl();
				 dd1->lhs->datatype=INT_TYPE;
				dd1->lhs->name=malloc(sizeof(char)*100);
				strcpy(dd1->lhs->name,"unsigned volatile int * ");
				dd1->dirrhs=new dir_decl();
				dd1->dirrhs->name=malloc(sizeof(char)*100);
				 strcpy(dd1->dirrhs->name,"arrayout");
				dd1->next=NULL;		
		}
	}
	t1->stdecl=new tree_decl_stmt();
	t1->stdecl->lhs=$1;
	t1->sttype=DECL_STMT;
	t1->flist=fhead;
	 fhead=NULL;
	t1->stdecl->dirrhs=$2;
	t1->stdecl->dirrhs->singleflag=SINGLEFLAG;
fnamessingle[t1->stdecl->dirrhs->name]=SINGLEFLAG;
SINGLEFLAG=0;
	statement *t2=sblock_begin[sbtop]; 
	if(t2->prev==NULL){
		temp=t1;
		printf("T2 PREV IS NULL\n");
	}
	if(t2!=NULL){
		t1->prev=t2->prev;
		t2->prev=t1;
		t1->next=t2;
		if(t1->prev!=NULL)t1->prev->next=t1;
		printf("T2 IS NULL AS EXPECTED\n");
	}
	t1->lineno=LINENO;
}
	|  declarator declaration_list{de2=temp3;} compound_statement{
//		if(temp3->sttype==EBLOCK_STMT)fprintf(FP1," //EBLOCK STMT\n");
		//fprintf(FP1,"//%d \n",temp3->sttype);
		temp3->sttype=FUNCTION_EBLOCK_STMT;
		((dir_decl *)$1)->procd=1;
		((dir_decl *)$1)->singleflag=SINGLEFLAG;
fnamessingle[((dir_decl *)$1)->name]=SINGLEFLAG;
                SINGLEFLAG=0;
		printf("FUNCTION\n");
		statement *t1=new statement();
		t1->stdecl=new tree_decl_stmt();
		t1->stdecl->dirrhs=((dir_decl *)$1);
		t1->stdecl->lhs=new tree_typedecl();
		t1->stdecl->lhs->datatype=VOID_TYPE;
		t1->sttype=DECL_STMT;
		t1->flist=fhead;
		 fhead=NULL;
		statement *t2=sblock_begin[sbtop];
		 t1->next=de1;
		 if(de1->prev!=NULL)de1->prev->next=t1; 
		 else temp=t1; 
		de1->prev=t1;
		 de2->next=t2; 
		t2->prev=de2;
		t1->lineno=LINENO;
	}
	|  declarator compound_statement{
		printf("FUNCTIONNOONE\n");printf("FUNCTION\n");
		statement *t1=new statement();
//		if(temp3->sttype==EBLOCK_STMT)fprintf(FP1," //eblocK STMT\n");
		//fprintf(FP1,"//%d \n",temp3->sttype);
		temp3->sttype=FUNCTION_EBLOCK_STMT;
		if(barrier==1){
		t1->barrier=1;barrier=0;
	}
	t1->stdecl=new tree_decl_stmt();
	t1->stdecl->dirrhs=((dir_decl *)$1);
	t1->stdecl->lhs=new tree_typedecl();
	t1->stdecl->lhs->datatype=VOID_TYPE;
	t1->sttype=DECL_STMT;
	t1->flist=fhead;
	 fhead=NULL;
	statement *t2=sblock_begin[sbtop]; 
	if(t2->prev==NULL){
		temp=t1;
		printf("T2 PREV IS NULL\n");
	}
	if(t2!=NULL){
		t1->prev=t2->prev;
		t2->prev=t1;
		t1->next=t2;
		if(t1->prev!=NULL)t1->prev->next=t1;
		printf("T2 IS NULL\n");
	}/*((tree_id *)$1)->procdef->print();*//*((tree_id *)$1)->procdef->print();*/
	t1->lineno=LINENO;
	}

declaration_list
	: declaration{de1=temp3;$$=$1;}
	| declaration_list declaration{$$=$1;}
	;

%%
#include <stdio.h>

void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "%d*** %s\n",LINENO, s);
}
printlibdtypes(){
for(int k=0;k<5;k++){
for(int i=0;i<7;i++){
printf ("%d %d %s\n",k,i,libdtypefunctions[k][i]);
}
}
for(int k=0;k<3;k++){
for(int i=0;i<8;i++){
printf ("%d %d %s\n",k,i,libdtypefields[k][i]);
}
}
}

main(int argc, char *argv[]){
if(argc<2){
printf("LEss number of arguments\n");
return;
}
yyin=fopen(argv[1],"r");
int t1=0;
while(argv[1][t1]!='.')t1++;
strncpy(header,argv[1],t1+1);
strncpy(gheader,argv[1],t1);
strncpy(source,argv[1],t1+1);
int ffff=0;
if(argc >2 ){//for all extra command line argument. option for partition size on cpu gpu has to be added.
int temp=2;
while(argc>temp){
if(!strcmp(argv[temp],"-ptype")){
	if(argc <temp+1){fprintf(stderr,"point type missing give 0 or 1\n");exit(0);}
	union_type=atoi(argv[temp+1]);temp=temp+2;
}//if point is float give command line -ptype 1
if(!strcmp(argv[temp],"-mtype")){
	if(argc <temp+1){fprintf(stderr,"morph size missing  give integer value\n");exit(0);}
	morph_size=atoi(argv[temp+1]);temp=temp+2;
}//if algo is morph give allocation size -mtype size
if(!strcmp(argv[temp],"-target")){
if(argc <temp+1){ fprintf(stderr,"target platform \n");exit(0);}
TARGET=atoi(argv[temp+1]);
if(TARGET==5)DUPVAR=5;
if(TARGET>1)MPIFLAG=1;
temp++;
temp++;
ffff=TARGET;
}
}
}
//ADD A GLOBAL VARIABLE MAX_INT. This should be declared in HGraph.h as int MAX_INT=1234567890, for no compiling error.
dir_decl *d1=new dir_decl();
d1->name=malloc(sizeof(char)*30);
tree_typedecl *tt1=new tree_typedecl();
	tt1->datatype=INT_TYPE;tt1->name=malloc(sizeof(char)*100);strcpy(tt1->name,dtypenames[tt1->datatype]);
strcpy(d1->name,"MAX_INT");
GLOBAL_TABLE.addsymbol(d1,tt1);
strcat(source,"cu");
FP1=fopen(source,"w+");
yyparse();
strcat(header,"h");
strcat(gheader,"global.h");
FP=fopen(header,"w+");
FPG=fopen(gheader,"w+");
printf(" GHEADER %s ",gheader);
if(FPG==NULL){printf("FOPEN FAILED\n");return;}
fprintf(FP1,"\n #include \"%s\"\n",header);
fprintf(FP,"\n#include \"%s\"\n",gheader);
system("rm -f global.h");
if(GALOIS_FLAG==0){
//for(int ii=0;ii<TOT_GPU_GRAPH;ii++)fprintf(FP1,"cudaDeviceProp prop%d;\n",ii);
if(!(TARGET==0 || TARGET==2)){
fprintf(FP," #include \"HGraph.h\"\n #include \"GGraph.cu\"\n#include \"thrust.cu\"\n #include<sys/time.h>\n#include </usr/local/cuda/include/cuda.h>\n #include </usr/local/cuda/include/cuda_runtime_api.h>\n#include<unistd.h>\n#include</usr/local/cuda-5.0/samples/0_Simple/simplePrintf/cuPrintf.cu>\n");
}else {
//if(TARGET==2|| TARGET==3 || TARGET==4){
fprintf(FP,"#include<mpi.h>\n#include \"HGraph.h\"\n #include<sys/time.h>\n#include<unistd.h>\n");
}
}
else 
fprintf(FP," #include \"HGraph.h\"\n #include \"GGraph.cu\"\n #include \"HostColl.h\"\n #include<sys/time.h>\n#include<unistd.h>\n#include \"GaloisHeaders.h\"\n");
printf("\n*******xx***\n");temp->print();
printf("\n*XX********* going to set parent\n");
setparent();//set parents for arguments in function calls.
setcomm();
if(TARGET==2)mpiinit(FP1,FP);
if(TARGET==3)mpiinitgpu(FP1,FP);
if(TARGET==4)mpiinitmultigpu(FP1,FP);
if(TARGET==5)mpiinitcpugpu(FP1,FP);
//return 1;
printf("\n**********\n");
//temp->print();printf("\n**********\n");
printf("PARENT SET next codegen");
temp->codeGen1();//generate code.
for(int ii=0;ii<TOT_GPU_GRAPH;ii++)fprintf(FP,"cudaDeviceProp prop%d;\n",ii);
printf("code done");
if(TARGET==5){
TARGET=3;
DUPVAR=2;
FP1=FP;
statement *dups=temp;
while(dups->stdecl && dups->stdecl->dirrhs && dups->stdecl->dirrhs->procd!=1 ){
dups->codeGen(FP1);
dups=dups->next;
}
DUPVAR=1;
//Gkernel=0;
fprintf(FP1,"//BEGIN\n");
for(std::map< char *,statement *>::iterator it=fnamescond.begin(); it!=fnamescond.end();it++){
for(std::map< char *,statement *>::iterator it1=fnames.begin(); it1!=fnames.end();it1++){
fnamessingle[it->first]=fnamessingle[it1->first];
if(!strcmp(it1->first, it->first)){
it1->second->ker=1;
it1->second->codeGenfun();
}
fprintf(stderr, "%s %s \n", it1->first,it->first);
}
}
fprintf(FP1,"//END\n");
//printlibdtypes();
}
DUPVAR=1;
FP1=FP;
for(std::map< char *,statement *>::iterator it1=fnames.begin(); it1!=fnames.end();it1++){
statement *tt=it1->second;
if (tt->stdecl->dirrhs->singleflag){
fprintf(FP1, "//single flag %s %d %d \n",it1->first,tt->stdecl->dirrhs->singleflag,fnamessingle[it1->first]);
//fnamessingle[it1->first]=1;
it1->second->codeGenfun();
}
}
}
