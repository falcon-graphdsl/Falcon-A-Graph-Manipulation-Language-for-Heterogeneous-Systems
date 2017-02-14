#ifndef FALCTYPES_H
#define FALCTYPES_H
#include<iostream>
#include<map>
#include<set>
#include<stdio.h>
#include<string.h>
struct UNION_KER {
	char *name;
	struct UNION_KER *next;
};
//node type of tree_node.THIS DATATYPE IS NOT USED. field nodetype of class treenode
enum TREE_NODE_TYPE  
{
	TREE_NODE,
	TREE_ID,       // 
	TREE_FIELD,    // A.B
	TREE_MAPACCESS,    // A[B]
	TREE_IDLIST,   // A, B, C, 
	TREE_TYPEDECL, // INT
	TREE_ARGDECL,  // a,b : B
	TREE_PROCDEF,  // proc A() {}
TREE_EXPR,     // c + 3
TREE_EXPR_RDC,       // c + 3
TREE_EXPR_BUILTIN,   // c + 3
TREE_EXPR_FOREIGN,  // Foreign Expression
TREE_EXPR_MAPACCESS,
TREE_SENT,     // 
TREE_SENTBLOCK, // { ... }
TREE_ASSIGN,   // C =D
TREE_VARDECL,  // INT x;
TREE_FOREACH,  
TREE_FOR,
TREE_ARRREF,
TREE_DOWHILE,
TREE_SWITCH,
TREE_IF,       // IF (x) THEN s; ELSE z ;
TREE_WHILE,    // While (x) {...} or Do {...} While (x)
TREE_RETURN,   // Return y;
TREE_CALL,     // Call to (built-in) function
TREE_FOREIGN,  // Foreign syntax
TREE_NOP,
TREE_INT,
TREE_FLOAT,
TREE_ENUMCONST,    
TREE_STRING,
TREE_BOOL
};
//stores inbuilt size of fileds/functions on each library  datatype.
//array of fields and functions available in grammar.y
enum SIZES { 
	fieldgraph=7,
	fieldedge=4,
	fieldpoint=8,
	fieldset=1,
	fieldcollection=1,
	fungraph=19,
funedge=6,
funpoint=8,
funset=9,
funcoll=9,
libfun=5,
iter=6
};
  
#define LIBTYPESIZE 8
//stores library datatype of variable(dir_decl)
enum LIBDATATYPE {
GRAPH_TYPE,//0
EDGE_TYPE,//1
POINT_TYPE,//2
SET_TYPE,//3
COLLECTION_TYPE,//4
G_P_TYPE,//5
E_P_TYPE,//6
P_P_TYPE,//7
ITERATOR_TYPE,//8
LINT_TYPE//9
};
#define TYPESIZE 16 
enum DATATYPE {//stores datatype of variable / constant
INT_TYPE,//0
FLOAT_TYPE,//1
SHORT_TYPE,//2
DOUBLE_TYPE,//3
CHAR_TYPE,//4
LONG_TYPE,//5
VOID_TYPE,//6
SIGNED_TYPE,//7
UNSIGNED_TYPE,//8
BOOL_TYPE,//9
ELLIPSIS_TYPE,//10
STRUCT_TYPE,//11
STRUCTREC_TYPE,//12.recursive declaration of struct for lib dataype, as lib does not have pointers
ENUM_TYPE,//13
UNION_TYPE,//14
TYPEDEF_TYPE,//15
TYPEDEF_TNAME,//16
};
enum ACCESSTYPE{
PTR_ACCESS,
ARR_ACESSS
};
//stores the storage type of variable
enum STORAGETYPE{
EXTERN_STR,
AUTO_STR,
REGISTER_STR,
STATIC_STR,
TYPEDEF_STR
};
//stores the compound  type 
enum COMPOUNDTYPE{
CSTRUCT_TYPE,
CENUM_TYPE,
CARR_TYPE,
CPROC_TYPE,
CUNION_TYPE
};

class tree_node {
public:
TREE_NODE_TYPE   nodetype;//tree node type
tree_node   *parent;//parent of tree node.
int ival;//if node is of type TREE_INT
double fval;//if node is of type TREE_FLOAT
int  ptrflag;//valid if node has ptr flag set
char *ptrstring;// not sure
    int line;//strores line number in high level code
int getline(); 
tree_node* getparent(); 
TREE_NODE_TYPE getnodetype();
  void setparent(tree_node* ); 
void setnodetype(TREE_NODE_TYPE nt) ;
tree_node();
//    symtab* sym_vars;
  //  symtab* sym_fields;
    //symtab* sym_procs;
};
//foreach iterator
enum ITERATOR{//not used in this version ??
POINT_ITYPE,//0. points for graph
EDGES_ITYPE,//1. edges for graph
NBRS_ITYPE,//2. nbrs   for point,edges
INNBRS_ITTYPE,//3.for point
OUTNBRS_ITYPE,//4. outnbrs for point
ITEMS_ITYPE,//5. items for colection,set
ADDED_ITYPE,//6.for newly added property for graph
NBRS1_ITTYPE,//7.for edge
NBRS2_ITTYPE//8.for edge
};
class tree_typedecl; //type declaration
class tree_expr;//tree expression
class proc_id;
class dir_decl;//variable 
struct funcall_list {
char *name;
struct funcall_list *next;
class tree_expr *call;
class assign_stmt *ass;
};
class tree_id: public tree_node {

public:
char *name;
tree_id *next;
tree_expr *rhs;
dir_decl *dirdecl;
class symtable *stable;
class tree_typedecl *decltype;
DATATYPE  dtype;
STORAGETYPE stype;
COMPOUNDTYPE ctype;
tree_expr *idrhs;
tree_expr *arr_list;
proc_id *procdef;
bool ptrfun;
tree_id();
char *getname();
void setname(char *);
void print();
void print(tree_id *);
int  find(tree_id *,tree_id *);
};
class tree_decl_stmt;
struct extra_ppts{
tree_typedecl *t1;
int val1, val2;
 tree_decl_stmt *dt1;
dir_decl *var1,*var2,*parent,*var3;
char *name;
struct extra_ppts *next,*child;
LIBDATATYPE  libdtype;
int partfun;
};
class dir_decl: public tree_node{//node for variable
public:
 dir_decl *next;// next variable in list, used in direct_abstract_list prodcution
dir_decl *prevv;//used when identifer declared using identifer_list production,ie variable declaration
dir_decl *nextv;
int dev_no;
char *name;//name of variable
char *extra_name;//used for properties/iteratos of graph for code generation
char *extra_name1;//used for properties/iteratos of graph for code generation
char *extra_name2;//used for properties/iteratos of graph for code generation
char *extra_fun;
char *update_fun;//partition update function 
char *extra_readfun,*extra_readfun1;
struct extra_ppts *ppts;//extra properties of graph
dir_decl *typelist;//type qualifier list in grammar type qualifier can have name CONST,VOLATILE etc
dir_decl *parent;//parent will point to parent graph/point/edge
bool sbrack_flag,ordered;//if in direct_abstract_list element has []
bool clone,iflag,read,isparam;//
int it;//iterator type
int forit;//foreach iterator
dir_decl();
int brack_flag;// ( ) brack in  function call
bool open_brack; //brack open in ( declarator )
bool gpu;//if gpu variable
bool arg;
bool local;
bool close_brack;// brack close in (declarator )
bool arr_flag;
bool stat;//if STATIC keyword is there 
 tree_id *type_qual;
tree_typedecl *tp1;
tree_expr *assign;//assignment expression in direct_declarator
tree_expr *rhs;//initializer for variable
tree_expr *structexpr;//  struct filed with : structexpr
tree_expr *idrhs;//enum constant value expression
bool procd;//if it is a procedure declaration
tree_decl_stmt *params;//parameter declarations for functions
tree_decl_stmt *parnames;//parameter names ,used for old type of function definition
int ptrflag;//valid if ptr
int ptrcnt;//how many pointers
class symtable *stable;//pointer to symtable
class libsymtable *libstable;//pointer to libsymtable, for library datatype
class tree_typedecl *decltype;
DATATYPE  dtype;//datatype of variable
LIBDATATYPE  libdtype;//lib dataype of variable
STORAGETYPE stype;//storage class extren ,auto etc
COMPOUNDTYPE ctype;//compound type enum,union etc
void print();//print variable
void printcode(int);//genereate code for variable to file
void printcode(int,char *);//genereate code for variable to file
void printcodearray(int,char *);//genereate code for variable to file
int  find(dir_decl *,dir_decl *);
};

enum EXPR_TYPE  {//expression type
ADD,//0
SUB,//1
MUL,//2
DIV,//3
MOD,//4
AND,//5
OR,//6
XOR,//7
BAND,//8
BOR,//9
COMP,//10
UNARYADD,//11
UNARYMUL,//12
UNARYSUB,//13
UNARYAND,//14
UNARYOR,//15
REF,//16
DREF,//17
NOT,//18
PREINC,//19
PREDEC,//20
POSTINC,//21
POSTDEC,//22
SIZE,//23
ALIGN,//24
ARRREF,//25
LSHIFT,//26
RSHIFT,//27
LT,//28
GT,//29
LE,//30
GE,//31
EQ,//32
NEQ,//33
VAR,//34
EINT,//35
EFLOAT,//36
ESTRING,//37
PTRREF,//38
FUNCALL,//39
TREE_STR,//40
STRUCTREF,//41
EXPR,//42
DOWHILE_EXPR,//43
WHILE_EXPR,//44
ENUMASSIGN,//45
NULL_ARR,//46
TREE_CAST,//47
INITBLOCK,//48
COND_OP,//49
TYPE_INIT,//50
TYPE_SIZEOF,//51
CAST_EXPR,//52
GET_TYPE//53
};

enum ASSIGN_TYPE{//assignment type
AASSIGN,// =
AMUL_ASSIGN,// *=
ADIV_ASSIGN,// /=
AMOD_ASSIGN,// %=
AADD_ASSIGN,// +=
ASUB_ASSIGN,// -=
ALEFT_ASSIGN,// <<=
ARIGHT_ASSIGN,// >>=
AAND_ASSIGN,// &&=
AXOR_ASSIGN,// ^=
AOR_ASSIGN,// ||=
EMPTY_ASSIGN,// ;
RRADD_ASSIGN,// REDUXSUM= . Falcon only
RRMUL_ASSIGN// REDUXMUL=  .Falcon only
};
class assign_stmt :public tree_node{
public:
class  tree_expr *lhs;//lhs expression of assignment
class tree_expr *rhs;//rhs expression of assignment
class tree_expr *middle;//middle part of assignment.****Not used in this version***
bool semi;//if semi colomn to be put after assignment
ASSIGN_TYPE asstype;//type of assignment
class assign_stmt *next;//next in list
void print1(bool);//print 
void printcode(bool);//print fo file
void printcode1(char *,bool);//print to file
};
class statement;
class type_name;
class  designator: public tree_node{//initialization of arrays etc
public:
class tree_expr *expr;
char *name;
class designator *next;
designator();
};
class tree_expr: public dir_decl {
public:
EXPR_TYPE expr_type;//expression type
LIBDATATYPE  exprlibtype;//libdatatype of expression
int pflag;//parameter
int argflag;//argument
int ptrflag;//pointe flag
int kernel;//kernel. >0 means inside GPU kernel
tree_expr *lhs;//lhs in expr syntax tree
tree_expr *rhs;//rhs in expr syntax tree
assign_stmt *earr_list;//for mutli dimensional array expression
tree_expr *next,*prev;//next prev in expression list
class designator *desi;//used in initialization of array enum  struct etc
class type_name *tpname;//type casted type name
class tree_typedecl *tp1;
assign_stmt *arglist;//arguments
class statement *exprlist;
void print(tree_expr *);//astprint to stdout
void printcode(tree_expr *);//print to file
printcode1(tree_expr *,char *);//print to file
void findgpuvars(tree_expr *);//finds gpu variables when in CPU code (kernel==0)
void findextraproperties(tree_expr *);
void FOR_findgpuvars(tree_expr *);//gpu variables in for statement.
tree_expr(dir_decl *);//constructor
tree_expr();//constructor
tree_expr(tree_expr *);//constructor
char *cpu_name;//new name for gpu variable after generatring cudaMemcpy
};

enum STMT_TYPE{//stmt type
COMP_STMT,//0
WHILE_STMT,//1
DOWHILE_STMT,//2
DOWHILEEXPR_STMT,//3
FOR_STMT,//4
EXPR_STMT,//5
IF_STMT,//6
SWITCH_STMT,//7
JUMP_STMT,//8
SBLOCK_STMT,//9
EBLOCK_STMT,//10
ASSIGN_STMT,//11
DECL_STMT,//12
ARRREF_STMT,//13
RETURN_STMT,//14
GOTO_STMT,//15
LABELED_STMT,//16
DEFAULT_STMT,//17
CASE_STMT,//18
CONTINUE_STMT,//19
BREAK_STMT,//20
EMPTY_STMT,//21
FOREACH_STMT,//22
MACRO_STMT,//23
FOREACH_EBLOCK_STMT,//24
SINGLE_STMT,//25
SECTION_STMT,//26
SECTIONS_STMT,//27
FOR_EBLOCK_STMT,//28
FUNCTION_EBLOCK_STMT,//29
};
class tree_decl_stmt;
//in tree_typedecl it is datatype and libdatatype for fields of type DATATYPE AND LIBDATATYPE. all other places it is dtype and libdtype.
class tree_typedecl:public tree_node {

public:
DATATYPE datatype;//datatype
LIBDATATYPE libdatatype;//library datatype
int libsize,libelement;
dir_decl *d1,*d2,*d3;
tree_typedecl *t1;//type declaration
struct extra_ppts *ppts;//extra properties of graph
bool def;//is definition true
bool exgpu;
bool tpdef;//is type def
char *name;//name examples struct node,int etc
char *vname;//variable name of struct
int storagetype;//storage type auto extern
int compoundtype;//is compound like struct union
int accesstype;
int qualifiertype;
tree_decl_stmt *list;//declaration statement list of struct/union
class tree_typedecl *next;//next type decl
class dir_decl *type_qual;//type qualifier const volatile
tree_typedecl();
};
class dir_decl;
class tree_decl_stmt:public tree_node{
public:
class tree_typedecl *lhs;// type of variable list
class tree_id *rhs;//rhs of declaration,variable list rhs->dirdecl will have variable list
class dir_decl *dirrhs;//variable list for paremeters
class tree_decl_stmt *next;//next delcaration statement
tree_decl_stmt():lhs(NULL),rhs(NULL),next(NULL),dirrhs(NULL){}
int print();
int printcode(int);
};
class type_name: public tree_node{
public:
class dir_decl *decl;
class tree_typedecl *typedecl;
type_name():decl(NULL),typedecl(NULL){}
void print();
void printcode(char *);
};
class iter_stmt;
class statement:public assign_stmt{

public:
STMT_TYPE sttype;//statement tyoe
bool stcomp;//cpugpu for partitioned execution
bool noprint;//for update function of partitioned execution
int barrier;
int itr,ker;
int foreachflag;
int lineno,stmtno;//line no high level code and stament no
class tree_expr *expr1,*expr2,*expr3,*expr4,*expr5;//used in statement like for,if etc
class statement *f1,*f2,*f3;//used in statements like if,for ,if-then-else
class statement *body;/**NOT USES?? **/
class tree_decl_stmt *stdecl;
class assign_stmt *stassign;
class iter_stmt   *stiter;
class statement *next,*prev;
struct funcall_list *flist;
char  *name;
bool comma;
bool feb;
print();
void codeGen(FILE *FP);
void codeGen1();

statement();
};
class iter_stmt {
public:
class tree_expr *expr1,*expr2,*expr3;
class statement *body;
print();
iter_stmt();
};

#endif
