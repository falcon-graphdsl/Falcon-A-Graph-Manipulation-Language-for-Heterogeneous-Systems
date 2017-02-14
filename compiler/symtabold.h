#ifndef OLDSYMTAB_H 
#define OLDSYMTAB_H     
#include "falctypes.h"
#include<iostream>
#include<set>
#include<map>
#include<string.h>
int check_gpu(statement *t1);
int print_gpu_assign_stmt(statement *t1,int val);
int realloc_graph1(dir_decl *t1 );
 int lock_graph1(dir_decl *t1 );
class tree_decl_stmt;
class tree_typedecl;
class tree_node;
class dir_decl;
extern volatile int declared;
class globsymtable;
class proc_id:public tree_node{
public:
char *name;
tree_decl_stmt *params;
int param_count,vararg;
void print();
proc_id();
void addsymbols(globsymtable *);
};
class tree_decl_stmt;
class symtableentry: public tree_node
{

public:
    symtableentry() ;
insert(tree_typedecl *, dir_decl *);
    bool getarg(); 

    tree_typedecl* type;
    bool isarg;
    std::set<dir_decl *> vars;
    std::set<DATATYPE> ftypes;
};

class symtable: public symtableentry
{
public:
symtable():parent(NULL),symtabtype(-1){}
    symtable(int symtabtype1) :
            parent(NULL), symtabtype(symtabtype1)  {
    }
int checkdup(dir_decl *);
    int getsymtabtype(); 
    void setparent(symtable* ); 
    symtable* getparent(); 
addparams(dir_decl *,tree_typedecl *);
    bool addsymbol1(dir_decl* , tree_typedecl* , symtableentry*& ) ;
    bool addsymbol(dir_decl * , tree_typedecl* , symtableentry*& ); 
print_decl(tree_decl_stmt *);
    symtableentry* findsymbol(dir_decl* );
dir_decl *findsymbol(char *); 
dir_decl *findsymbol1(char *); 
dir_decl *findsymbol(char *,int); 
    std::set<symtableentry*>& getentries() ;
tree_typedecl *findtypedefsymbol(char *name);

    int getsize() ;
    void merge(symtable* ) ;
    void removeentry(symtableentry *) ;


    void add_symbol(symtableentry* ) ;

    std::set<symtableentry *> entries;
    symtable* parent;
std::set<tree_decl_stmt *> stmts;
std::map<char *,tree_typedecl *>newtypes;
    int symtabtype;
};
class libsymtableentry:public tree_node{
public:
    tree_typedecl* type;
    bool isarg;
    std::set<dir_decl *> vars;
    std::set<LIBDATATYPE> ftypes;
//    symtableentry() ;
insert(tree_typedecl *, dir_decl *);
  //  bool getarg(); */
};

class libsymtable:public libsymtableentry
{
public:
libsymtable():parent(NULL),symtabtype(-1),gparent(NULL){}
    libsymtable(int symtabtype1) :
            parent(NULL), gparent(NULL),symtabtype(symtabtype1)  {
    }
    int getsymtabtype();
    void setparent(libsymtable* );
    libsymtable* getparent();
//addparams(dir_decl *,tree_typedecl *);
    bool addsymbol1(dir_decl* , tree_typedecl* , libsymtableentry*& ) ;
    bool addsymbol(dir_decl * , tree_typedecl* , libsymtableentry*& );
//print_decl(tree_decl_stmt *);
    libsymtableentry* findsymbol(dir_decl* );
dir_decl *findsymbol(char *);
    std::set<libsymtableentry*>& getentries() ;
tree_typedecl *findtypedefsymbol(char *name);

  //  int getsize() ;
    //void merge(libsymtable* ) ;
    //void removeentry(libsymtableentry *) ;


    //void add_symbol(libsymtableentry* ) ;

    std::set<libsymtableentry *> entries;
    libsymtable* parent;
    globsymtable *gparent;
std::set<tree_decl_stmt *> stmts;
std::map<char *,tree_typedecl *>newtypes;
    int symtabtype;
};
class globsymtable {
public:
symtable *sym;
libsymtable *libsym;
globsymtable *parent;
globsymtable(){
//printf("in constructor\n");
sym=new symtable();
libsym=NULL;
parent=NULL;}

addsymbol(dir_decl *,tree_typedecl *);
int checkdup(dir_decl *);
dir_decl *findsymbol(char *name);
dir_decl *findsymbol(dir_decl *);
};
extern print_partitioned_assign(statement *);
extern printexpr_type(tree_expr *,int d);
extern volatile int vartop;
extern int GALOIS_FLAG;
extern char *dtypenames[];
extern char *iternames[];
extern char *libdtypenames[10];
extern char *exptypename[ ];
extern tree_id *varname[100];
extern globsymtable GLOBAL_TABLE;
extern globsymtable *currsymtab, *stemp;
extern tree_expr *exprstack[25];
extern tree_decl_stmt *argstack[25];
extern int exprtop,argtop,PUSHER_CNT;
extern volatile int declared;
extern void dir_decl::print();
extern  statement::print();
extern void tree_expr::print(tree_expr *);
//int FOREACH_FLAG=0,EXTRA_CRBRK=0,MAINF=0,foreachhostflag;
//FILE *FP=fopen("extrasssp.h","w+");
//FILE *FP1=fopen("ssspcode.cu","w+");
//FILE *FP,*FP1,*FPG,*fp1;
#endif
