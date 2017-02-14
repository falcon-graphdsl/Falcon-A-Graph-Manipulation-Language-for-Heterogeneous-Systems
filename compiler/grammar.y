%{
#include <stdio.h>
    #include <string.h>
    #include <assert.h>
#include "symtabold.h"
int GPUCODEFLAG=0;
int ERRPRINT;
extern int morph_size,TOT_GPU_GRAPH,MORPH_FLAG;//yes
extern int union_type,errflag,pdim_size;
extern FILE *FP,*FP1,*FPG,*fp1;
 extern std::map<char *,int>newitr;
 extern std::map<char *,int>newitrdtype;
extern  std::map<char *,statement *>fnames;//function definition
 extern std::map<char *,statement *>fnamescond;//foreach call of function in char *with conditional
class statement *G1=NULL,*head=NULL,*temp=NULL,*temp1=NULL,*temp3=NULL,*de1,*de2;
int nitr=11,STMTNO=0,macroflag=-1;
int KERNEL=0,PEER_ENABLE;
int FOREACH_FUN_FLAG;
int ariflag=0,LINENO=1,arglistflag=0;
globsymtable GLOBAL_TABLE;
int GALOIS_FLAG;
char *mainfunarr;
volatile int vartop;
extern void setparent();
extern char *union_string[2];
extern char *morphalloc_string[3];
extern FILE *yyin;
globsymtable *currsymtab=&GLOBAL_TABLE, *stemp;
char  *utflagarr[2][3]={{"3","+1","+2"},{"2","","+1"}};
int utflag=1;
int FUNCALL_FLAG=0,INNBRFLAG,OUTNBRFLAG;
char header[100],source[100],gheader[100],*tempitr=NULL;
struct funcall_list *fhead=NULL;//declared in tree.h
char *dtypenames[]={"int ","float ","short ","double ","char ","long ","void ","signed ","unsigned ","bool","...","struct ","struct_rec","enum","union ","typedef "};
char *libdtypenames[10]={"graph ","edge ","point ","set ","collection","g_p ","e_p ","g_p ","ITR","LINT"};
char *libdtypefields[5][8]={ { "npoints" ,"points","nedges","edges","directed","twod","isdel","XXXXX"},{"src","dst","weight","isdel","XXXXX","XXXXX","XXXXX","XXXXX"},{"minEdge","maxEdge","x","y","nbrs","inNbrs","outNbrs","isdel"},{"parent","size","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"},{"size","PORST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"}};
char *libdtypefunctions[5][19]={ { "addPoint" ,"read","addEdge","deledge","delpoint","getweight","addProperty","addPointProperty","addEdgeProperty","init","reset","getedge","readNodes","read2","read1","redmorph","OutDegree","InDegree","UpdatePartition"},
{"del","setweight","changesrc","changedst","reset","init","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"},
{"getNeighbours","getInNeighbours","getOutNeighbours","del","addNeighbour","delNeighbour","reset","init","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"},
{"makeset","merge","find","getSize","clear","Union","Merge","Findcompress","init","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"},
{"add","del","merge","getSize","clear","reset","init","OrderByIntValue","empty","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST","PQRST"}};
char *exptypename[ ]={"+","-","*","/","%","&&","||","^","&","|","~","+","*","-","&","|","&","*","!","++","--","++","--","sizeof","align","[","<<",">>","<",">","<=",">=","==","!=","","","","","->","","","\(","",""};
char *libfunctions[]={"MIN","MAX","INCR","DECR","CAS","ADD","SUB","OR","AND","XOR","genericbarrier"};
char *libpartitionfunctions[]={">","<","+=1","-=1","CAS","+=","-=","||=","&&=","~=","genericbarrier"};
char *iternames[]={"points ", "edges ", "nbrs ", "innbrs ", "outnbrs ", "items "};
bool barrier=0;
#include "grammar.h"
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
//	printf("XX\n");
dir_decl *x=currsymtab->findsymbol($1);
	$$=new tree_expr(x);
	((tree_expr *)$$)->name=$1;
	((tree_expr *)$$)->nodetype=-1;	}/*$$=new tree_expr();*/ /* need to add code to copy datatype information from dir_decl to tree_expr */
	| constant {$$=$1;}
	| string {$$=$1;
	}
	| '(' expression ')' {
        $$=binaryopnode(NULL,NULL,EXPR,-1);
	 statement *t1=temp3; 
	while(t1->comma==1)t1=t1->prev;
	((tree_expr *)$$)->exprlist=t1;temp1=temp3=t1->prev;//printf("DDDDDDDDDDDDDDDDDDDD\n");
	}
	| generic_selection{ $$=$1;}
	;

constant
        : I_CONSTANT{
        $$=binaryopnode(NULL,NULL,-1,TREE_INT);
	((tree_expr *)$$)->ival=$1;
	((tree_expr *)$$)->dtype=0;
	}
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
char *tempstr=(char *)calloc(100,sizeof(char));
strcpy(tempstr,$1);
        $$=binaryopnode(NULL,NULL,-1,TREE_STRING);
	 ((tree_expr *)$$)->name=malloc(sizeof(char)*100);
     for(int i=0;i<100;i++) ((tree_expr *)$$)->name[i]='\0';
	 strcpy(((tree_expr *)$$)->name,tempstr);
	if(strlen($1)<=3)((tree_expr *)$$)->dtype=4;
	}
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
	: primary_expression %prec PR_EXPR1  {
	$$=$1; }
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
			y=y->next;
		}
		 y->next=((assign_stmt *)$3);
		 y->next->next=NULL;
	}
	$$=$1;}
       else {
      	 ((tree_expr *)$1)->expr_type=ARRREF;
	 assign_stmt *y=((tree_expr *)$1)->earr_list;
	if(y==NULL){
		((tree_expr *)$1)->earr_list=((assign_stmt *)$3);
		((tree_expr *)$1)->earr_list->next=NULL;
	}
	else{
		while(y->next!=NULL){
			y=y->next;
		 }
		 y->next=((assign_stmt *)$3);
		y->next->next=NULL;
	 }
	 $$=$1;
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
	 } 
	 else   {
		 ((tree_expr *)$1)->rhs->expr_type=FUNCALL;
		 ((tree_expr *)$1)->rhs->kernel=KERNEL;
		  t1->rhs->arglist=pt1;
         }
	 $$=$1;
        if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"addPointProperty"))){   adddynamicproperty(t1,P_P_TYPE,pt1);      }
      if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"addEdgeProperty"))){   adddynamicproperty(t1,E_P_TYPE,pt1);      }
      if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"addProperty"))){addgraphproperty(t1,G_P_TYPE,pt1);     }
     if(t1->expr_type==STRUCTREF&&!(strcmp(t1->rhs->name,"OrderByIntValue"))){
          dir_decl *d1=currsymtab->findsymbol(t1->lhs->name);
          d1->ordered=true;
     }
}
	| postfix_expression PTR_OP IDENTIFIER{
                $$= binaryopnode($1,NULL,PTRREF,-1);
                ((tree_expr *)$$)->rhs=new tree_expr();
                ((tree_expr *)((tree_expr *)$$)->rhs)->name=$3;
                ((tree_expr *)((tree_expr *)$$)->rhs)->expr_type=VAR;
         }
	| postfix_expression '.' IDENTIFIER{
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
              if(u1->expr_type==VAR && ((dir_decl *)(u1->lhs))->libdtype==GRAPH_TYPE){
                   dir_decl *dg=u1->lhs;
                   tree_typedecl *tpold= dg->tp1;
                   tree_typedecl *tp1;
                   tp1=new tree_typedecl();
                   tp1->libdatatype=GRAPH_TYPE; 
                   tp1->name=malloc(sizeof(char)*100);
                   strcpy(tp1->name,libdtypenames[tp1->libdatatype]);
                   if(dg->ppts!=NULL){
                     extra_ppts *newppts,*oldppts=dg->ppts,*head;
                       newppts=malloc(sizeof(struct extra_ppts));
                       newppts->parent=NULL;
                       newppts->name=malloc(sizeof(char)*100);
                     strncpy(newppts->name,oldppts->name,strlen(oldppts->name));
                       newppts->libdtype=oldppts->libdtype;
                       newppts->t1=oldppts->t1;//mutliple entries point to same type
                       newppts->var2=oldppts->var2;
                       newppts->var1=oldppts->var1;
                       newppts->var3=oldppts->var3;
                       newppts->val2=oldppts->val2;
                      if(oldppts->var2!=NULL){
		     }
		    newppts->next=NULL;
		    newppts->parent=dg;
                    head=newppts;
                    oldppts=oldppts->next;
                    while(oldppts){
                        newppts->next=malloc(sizeof(struct extra_ppts));
                        newppts=newppts->next;
                        newppts->parent=NULL;
                       newppts->name=malloc(sizeof(char)*100);
                       strcpy(newppts->name,oldppts->name);
                       newppts->libdtype=oldppts->libdtype;
                       newppts->t1=oldppts->t1;
                       newppts->var2=new dir_decl();
                       newppts->val2=oldppts->val2;
                       newppts->var2->name=malloc(sizeof(char)*100);
                       if(oldppts->var2!=NULL)strncpy(newppts->var2->name,oldppts->var2->name,strlen(oldppts->var2->name));
                       newppts->next=NULL;
                       oldppts=oldppts->next;

                }

                tp1->ppts=head;
        }
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
paraargument_expression_list: argument_expression_list{
	$$=$1;arglistflag=1;}
                            | parameter_type_list {
	$$=$1;}
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
	: postfix_expression {
	//printf("xxXXX\n");
	$$=$1;} 
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
	: cast_expression{
	 $$=$1;}
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
}
	|  unary_expression assignment_operator assignment_expression{
		((assign_stmt *)$2)->lhs=(tree_expr *)$1;
		((tree_expr *)$1)->kernel=5;
		((tree_expr *)$3)->kernel=5;
		((assign_stmt *)$2)->rhs=(tree_expr *)$3;
		$$=$2;/*((assign_stmt *)$2)->print1(1);*/$$=$2;
		temp3=createstmt(ASSIGN_STMT,NULL,NULL,LINENO); 
		temp3->stmtno=STMTNO;
		temp3->stassign=$$;
		if(temp1==NULL){temp1=temp3;temp=G1=temp1; }
		 else {
			if(temp1->sttype==ASSIGN_STMT  &&temp1->stmtno==STMTNO && temp1->stassign->lhs!=NULL){ 
				temp1->prev->next=temp3; 
				temp3->prev=temp1->prev;
				temp1->prev=temp3;
				temp3->next=temp1;
				temp1=temp3;
				((assign_stmt *)$2)->rhs=NULL;
			}
			else{             linkstmt(&temp1,temp3);			}
			assflag=1;
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
		$$=createdeclstmt($1,NULL,$2);
		temp3=createstmt(DECL_STMT,NULL,NULL,LINENO);
		temp3->stdecl=$$;
		if(temp1==NULL){
			temp1=temp3;
			G1=temp=temp1;
		} else{ linkstmt(&temp1,temp3);
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
}
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
	}
	| '<' GPU devno '>'declarator'=' initializer {
		((dir_decl *)$5)->gpu=1;
		((dir_decl *)$5)->rhs=$7;
GPUCODEFLAG=1;
		$$=$5;}
	| '<' GPU  devno '>'declarator{
		$$=new tree_id();
		((dir_decl *)$5)->gpu=1;
GPUCODEFLAG=1;
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
		((tree_typedecl *)$$)->vname=malloc(sizeof(char)*100);
		strcpy(((tree_typedecl *)$$)->vname,$2);
	}
	| struct_or_union IDENTIFIER{
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
	}
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
	}
	| direct_declarator {$$=$1;}
	;

direct_declarator
	: IDENTIFIER %prec DIR_DECL {
		$$=new dir_decl();
		((dir_decl *)$$)->name=/*malloc(sizeof(char)*100); strcpy(((dir_decl *)$$)->name,*/$1;
	}
	|  direct_declarator '['list1 ']' {
		((dir_decl *)$1)->tp1=(tree_typedecl *)$3;
	}
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
		FNAME->name=((dir_decl *)$1)->name;
		FNAME->ctype=CPROC_TYPE;
		((dir_decl *)$1)->brack_flag=2;
		t2->param_count=param_count;
		t2->vararg=vararg; 
		param_count=vararg=0;
		((dir_decl *)$1)->procd=1;
		$$=$1;
	}
	| direct_declarator'.'IDENTIFIER '(' IDENTIFIER',' parameter_type_list ')' {
	}
	| direct_declarator '(' ')'{
		dir_decl *t1=$1;
		t1->brack_flag=2;
		$$=$1;
		((dir_decl *)$$)->procd=1;
	}
	| direct_declarator '(' identifier_list ')'{
		dir_decl *t1=$1;
		 t1->brack_flag=2;
		t1->procd=1;
		t1->parnames=new tree_decl_stmt();
		t1->parnames->dirrhs=$3;
		$$=t1;
	}
	;

pointer
	: '*' type_qualifier_list pointer{
		if($3->ptrstring!=NULL){
			strcat(((dir_decl *)$2)->name, $3->ptrstring);
		 }
		char *t1=((dir_decl *)$2)->name;
		 int i=strlen(t1);
		 for(int j=i;j>=1;j--)t1[j]=t1[j-1];
		 t1[0]='*';
		 $$=$3;
		 $$->ptrstring=t1;}
	| '*' type_qualifier_list{
		char *t1=((dir_decl *)$2)->name; 
		int i=strlen(t1);
		for(int j=i;j>=1;j--)t1[j]=t1[j-1];
		t1[0]='*';
		$$=new tree_node(); 
		$$->ptrstring=t1;
	}
	| '*' pointer{
		$$=$2;
		$2->ptrflag++;
		if($2->ptrstring!=NULL){
			int i=strlen($2->ptrstring);
			int j;
			 i++;
			for(j=i;j>=1;j--){
				$2->ptrstring[j]=$2->ptrstring[j-1];
			}
			 $2->ptrstring[0]='*';
		   }}
	| '*'{
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
		param_count++;
	}
	| parameter_list ',' parameter_declaration{
		tree_decl_stmt *t1=$1; 
		while(t1->next!=NULL)t1=t1->next;
		t1->next=(tree_decl_stmt *)$3;
		$$=$1;
		param_count++;
		$$=$1;
	}
	;

parameter_declaration
	: declaration_specifiers declarator{ 		$$=createdeclstmt($1,NULL,$2);	}
	| declaration_specifiers '<' GPU devno '>'declarator{		 $$=createdeclstmt($1,NULL,$6);
((dir_decl *)($6))->gpu=1;
GPUCODEFLAG=1;
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
GPUCODEFLAG=1;
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
		$$=sblock_begin[sbtop];
	}
	| expression_statement{
		$$=$1;
	}
	| selection_statement{$$=new tree_node();((tree_node *)$$)->nodetype=TREE_IF;}
	| single_statement{$$=new tree_node();}
	| iteration_statement{
		$$=new tree_node() ;
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
		$$=new tree_node();
	}
	;
compound_begin
        : '{'{
		temp3=createstmt(SBLOCK_STMT,NULL,NULL,LINENO);
		if(temp1!=NULL){linkstmt(&temp1,temp3);
		}else {
			temp1=temp=temp3;
		}
		stemp=new globsymtable();
		stemp->parent=currsymtab;
		stemp->sym->parent=currsymtab->sym;
		currsymtab=stemp;
		sblock_begin[sbtop++]=temp3;
		if(FDEF!=NULL){
			if(FDEF->params!=NULL){
				FDEF->addsymbols(currsymtab);
			}
			FDEF=NULL;
			symtableentry *old=NULL;
			if(FTYPE==NULL){
				FTYPE=new tree_typedecl();
				 FTYPE->datatype=VOID_TYPE;
			}
			currsymtab->parent->addsymbol(FNAME,FTYPE);
		}}
        ;
compound_end
        : '}'{
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
		while(temp1->next!=NULL&&temp1->sttype==ASSIGN_STMT&& temp1->stmtno==temp1->next->stmtno){
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
		t4->stassign=(assign_stmt *)$3;
	       tree_expr *bar=t4->expr1;
		 if(bar->expr_type==VAR && bar->libdtype==COLLECTION_TYPE)barrier=1;
		statement *t2=(statement *)$5; 
        createsinglestmt1(&t4,&t2,&temp3);
}
     
        | SINGLE '(' expression ')' statement ELSE statement{
		statement *t4=createstmt(SINGLE_STMT,((assign_stmt *)$3)->rhs,NULL,LINENO);
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
//fprintf(FP1,"/*IFSTMT*/");
//if(t1->nodetype==TREE_IF)
//fprintf(FP1,"//if else if \n");
createifstmt(&t4,&t1,&t2,&temp3,1);
//if(temp1->sttype==IF_STMT)fprintf(FP1,"//IF ELSE IF \n");
char arr[100];
((assign_stmt *)($3))->printcode1(arr,1);
//fprintf(FP1,"%s \n",arr);
	}
| IF '(' expression ')' statement %prec THEN{
		statement *t4=createstmt(IF_STMT,((assign_stmt *)($3))->rhs,NULL,LINENO);
		statement *t2=(statement *)$5;
//fprintf(FP1,"//else sttype %d ",t2->sttype);
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
			temp3->prev->next=temp3;temp1=temp3;
	  }
	currsymtab=currsymtab->parent;KERNEL=0;
	FUNCALL_FLAG=0;}
        | FOREACH b_compound_begin IDENTIFIER IN IDENTIFIER ')' 
{
		dir_decl *d1=createdirdeclforeach((char *)$3,(char *)$5,ITERATOR_TYPE,-1,-1,(int)5);
                
		}conditional_for
  {             if(temp3->sttype==EBLOCK_STMT){
                        statement *t1=sblock_begin[sbtop];
                        if(temp3->expr4!=NULL){
				  temp3->expr4->print(temp3->expr4);      
				  fnamescond[t1->next->stassign->rhs->name]=temp1;
}
                	 temp3->sttype=FOREACH_EBLOCK_STMT;
                        t1->expr5=$4;
			t1->feb=1;
                FOREACH_FUN_FLAG=1;
                        t1->sttype=FOREACH_STMT;
  initforeach(&t1,&temp3,(char *)$3,(char *)$5,5);
		}else {
if(temp3->expr4!=NULL){
				fnamescond[temp1->stassign->rhs->name]=temp1;
			 }
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
dir_decl *d1=new dir_decl();
d1->name=malloc(sizeof(char)*30);
strcpy(d1->name,$3);
tree_typedecl *tt1=new tree_typedecl();
	tt1->datatype=INT_TYPE;tt1->name=malloc(sizeof(char)*100);strcpy(tt1->name,dtypenames[tt1->datatype]);
        currsymtab->addsymbol(d1,tt1);
		dir_decl *x1=currsymtab->findsymbol($3);
		((tree_expr *)$$)->rhs=new tree_expr(x1);
		((tree_expr *)($$))->expr_type=VAR;
	}
        |  '(' '-' IDENTIFIER ')'    IN1   { $$=$5;}
        | IN1 {
		$$=$1;
	}
        ;
IN1     : INOLD {
		$$=NULL;
}
	|  IN1  '*' { 
		$1=new tree_expr(); 
		((tree_expr *)($1))->name=malloc(sizeof(char)*10);
		strcpy(((tree_expr *)$1)->name,"In");
		((tree_expr *)($1))->ptrflag++;$$=$1;
	}
        ;
iterator: IDENTIFIER{ 
        	if((!strcmp($1,"points"))&&(strlen($1)==strlen("points")))$$=0;
    		else if((!strcmp($1,"edges"))&&(strlen($1)==strlen("edges")))$$=1;
		else if((!strcmp($1,"nbrs"))&&(strlen($1)==strlen("nbrs"))){
				$$=2;
				OUTNBRFLAG=INNBRFLAG=1;
		}
                 else if((!strcmp($1,"innbrs"))&&(strlen($1)==strlen("innbrs"))){
				$$=3;
				INNBRFLAG=1;
		}
               else if((!strcmp($1,"outnbrs"))&&(strlen($1)==strlen("outnbrs"))){
			$$=4;
			OUTNBRFLAG=1;
		}
		 else if((!strcmp($1,"items"))&&(strlen($1)==strlen("items"))){
			$$=5;
		}
 		 else {
		       int flag=0;
			for (std::map<char *,int>::iterator it=newitr.begin(); it!=newitr.end(); ++it){
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
                        temp3->expr4=((assign_stmt *)$2)->rhs;
			statement *t1=sblock_begin[sbtop]; 
                        statement * t4=t1->prev;
                        t4->prev->next=t1;
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
		statement *t1=sblock_begin[sbtop]; 
		fnames[t1->prev->stdecl->dirrhs->name]=t1->prev;
        t1->prev->foreachflag=FOREACH_FUN_FLAG;
        FOREACH_FUN_FLAG=0;
	}
	| declaration{}
        | macro_statement{ }
	;

function_definition
	: declaration_specifiers declarator declaration_list{de2=temp3;} compound_statement{
		 if(temp3->sttype==EBLOCK_STMT)
		temp3->sttype=FUNCTION_EBLOCK_STMT;
		statement *t1=new statement();
		t1->stdecl=createdeclstmt($1,NULL,$2);
		((dir_decl *)$2)->procd=1;
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
		statement *t1=new statement();

		temp3->sttype=FUNCTION_EBLOCK_STMT;
		if(barrier==1){
			t1->barrier=1;
			barrier=0;
			dir_decl *dd=$2;
			if(dd->params!=NULL &&  dd->params->dirrhs && dd->params->dirrhs->gpu) {
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
	statement *t2=sblock_begin[sbtop]; 
	if(t2->prev==NULL){
		temp=t1;
	}
	if(t2!=NULL){
		t1->prev=t2->prev;
		t2->prev=t1;
		t1->next=t2;
		if(t1->prev!=NULL)t1->prev->next=t1;
	}
	t1->lineno=LINENO;
}
	|  declarator declaration_list{de2=temp3;} compound_statement{
		temp3->sttype=FUNCTION_EBLOCK_STMT;
		((dir_decl *)$1)->procd=1;
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
		statement *t1=new statement();
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
	}
	if(t2!=NULL){
		t1->prev=t2->prev;
		t2->prev=t1;
		t1->next=t2;
		if(t1->prev!=NULL)t1->prev->next=t1;
	}
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
}
}
for(int k=0;k<3;k++){
for(int i=0;i<8;i++){
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
mainfunarr=(char *)malloc(sizeof(char)*500);
for(int i=0;i<500;i++)mainfunarr[i]='\0';
while(argv[1][t1]!='.')t1++;
strncpy(header,argv[1],t1+1);
strncpy(gheader,argv[1],t1);
strncpy(source,argv[1],t1+1);
if(argc >2 ){
//for all extra command line argument. option for partition size on cpu gpu has to be added.
int temp=2;
while(argc>temp){
if(!strcmp(argv[temp],"-ptype"))union_type=atoi(argv[temp+1]);//if point is float give command line -ptype 1
if(!strcmp(argv[temp],"-pdim"))pdim_size=atoi(argv[temp+1]);//if points have dim==2 or dim==3 give that value. default is 1.
if(!strcmp(argv[temp],"-mtype")){
morph_size=atoi(argv[temp+1]);//if algo is morph give allocation size -mtype size
MORPH_FLAG=1;
}
temp=temp+2;
}
}
dir_decl *d1=new dir_decl();
d1->name=malloc(sizeof(char)*30);
tree_typedecl *tt1=new tree_typedecl();
	tt1->datatype=INT_TYPE;tt1->name=malloc(sizeof(char)*100);strcpy(tt1->name,dtypenames[tt1->datatype]);
strcpy(d1->name,"MAX_INT");
GLOBAL_TABLE.addsymbol(d1,tt1);
if(GPUCODEFLAG==1)strcat(source,"cu");
if(GPUCODEFLAG==0)strcat(source,"cpp");
FP1=fopen(source,"w+");
int parserr;

parserr=yyparse();
if(parserr!=0)exit(0);
strcat(header,"h");
strcat(gheader,"global.h");
FP=fopen(header,"w+");
FPG=fopen(gheader,"w+");
if(FPG==NULL){printf("FOPEN FAILED\n");return;}
fprintf(FPG,"int FALC_THREADS;\n");
fprintf(FP1,"\n #include \"%s\"\n",header);
fprintf(FP,"\n#include \"%s\"\n",gheader);
system("rm -f global.h");
if(GALOIS_FLAG==0){
for(int ii=0;ii<TOT_GPU_GRAPH;ii++)fprintf(FP1,"cudaDeviceProp prop%d;\n",ii);
if(GPUCODEFLAG){
fprintf(FP," #include \"HGraph.h\"\n #include \"GGraph.cu\"\n#include \"thrust.cu\"\n #include \"HSetOPT.h\"\n #include<sys/time.h>\n#include </usr/local/cuda/include/cuda.h>\n #include </usr/local/cuda/include/cuda_runtime_api.h>\n#include<unistd.h>\n");
}else {
fprintf(FP," #include \"../include/HGraph.h\"\n  #include \"../include/HSetOPT.h\"\n #include<sys/time.h>\n#include<unistd.h>\n");

}
}
else{

fprintf(FP,"#include \"../include/HGraph.h\"\n");
fprintf(FP,"#include \"../include/HSet.h\"\n");
}
temp->print();
setparent();
ERRPRINT=1;
temp->print();
if(errflag!=0){
printf("skipping codegeneration\n");
exit(0);
}
temp->codeGen1();
for(int ii=1;ii<TOT_GPU_GRAPH;ii++)fprintf(FP,"cudaDeviceProp prop%d;\n",ii);
printf("codegeneration done\n output files \n 1)%s\n %s\n %s \n",source,header,gheader);
}

