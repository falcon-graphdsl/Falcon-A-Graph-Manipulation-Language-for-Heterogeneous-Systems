#include<stdlib.h>
class globsymtable;
extern class globsymtable *currsymtab;
 extern std::map<char *,int>newitr;
extern  std::map<char *,statement *>fnames;
 extern std::map<char *,statement *>fnamescond;
extern char *libdtypenames[];
tree_expr *binaryopnode(tree_expr *lhs,tree_expr *rhs,enum EXPR_TYPE etype,int ntype){
tree_expr *expr=new tree_expr();
 expr->lhs=lhs;
expr->rhs=rhs;
expr->expr_type=etype;
expr->nodetype=ntype;
return expr;
}
inline void  adddynamicproperty(tree_expr *t1, enum LIBDATATYPE x,assign_stmt *pt1){

            struct extra_ppts *ep=malloc(sizeof(struct extra_ppts));
            ep->parent=NULL;
            tree_typedecl *tt1=new tree_typedecl();
            tt1->libdatatype=x;
            tt1->d1=currsymtab->findsymbol(t1->lhs->name);
            struct extra_ppts *ep1=NULL;
            dir_decl *dpar=NULL;
            if(tt1->d1!=NULL)ep1=tt1->d1->ppts;
            if(ep1!=NULL)dpar=ep1->parent;
            if(dpar!=NULL)ep->parent=dpar;
            while(ep1!=NULL){
                  if(!strcmp(ep1->name,pt1->rhs->name))fprintf(stderr,"**********THIS PROPERTY ALREADY BELONG TO %s VARIABLE********************\n",tt1->d1->name);
                  ep1=ep1->next;
           }
           tree_decl_stmt *dst=(tree_decl_stmt *)t1->rhs->params;
           ep->dt1=dst;
           ep->t1=tt1->t1=(tree_typedecl *)(dst->lhs);
           if(currsymtab!=NULL && currsymtab->sym!=NULL && dst->lhs!=NULL){
                   tree_typedecl *xu=currsymtab->sym->findtypedefsymbol(dst->lhs->name);
                   if(xu!=NULL && xu->list!=NULL){
                          ep->t1=xu;
                          ep->t1->libdatatype=x;
                          tt1->t1->libdatatype=x;
                   }
           }
           ep->t1->libdatatype=x;
           tt1->name=malloc(sizeof(char)*100); 
           strcpy(tt1->name,libdtypenames[tt1->libdatatype]);
           dir_decl *var=new dir_decl();
           var->name=malloc(sizeof(char)*100);
           strcpy(var->name,pt1->rhs->name);
           tt1->ppts=ep;
           symtableentry *old=NULL;
           currsymtab->addsymbol((dir_decl *)var,(tree_typedecl *)tt1);
           if(dpar!=NULL){
		struct extra_ppts *ep=malloc(sizeof(struct extra_ppts));
                ep->parent=NULL;
                ep->child=NULL;
                tree_typedecl *tt1=new tree_typedecl(); 
                 tt1->libdatatype=x; 
                 tt1->d1=dpar;
                 tree_decl_stmt *dst=(tree_decl_stmt *)t1->rhs->params;
                 ep->dt1=dst;
                 ep->t1=tt1->t1=(tree_typedecl *)(dst->lhs);
                 ep->t1->libdatatype=x;
                 tt1->name=malloc(sizeof(char)*100); strcpy(tt1->name,libdtypenames[tt1->libdatatype]);
                 ep->name=malloc(sizeof(char)*100); strcpy(ep->name,pt1->rhs->name);
                 ep->next=dpar->ppts->child;
                 dpar->ppts->child=ep;
         }
      }
void addgraphproperty(tree_expr *t1, enum LIBDATATYPE x, assign_stmt *pt1){


            struct extra_ppts *ep=malloc(sizeof(struct extra_ppts));
            ep->parent=NULL;
            tree_typedecl *tt1=new tree_typedecl();
            tt1->libdatatype=x;
            tt1->d1=currsymtab->findsymbol(t1->lhs->name);
            ep->var2=NULL;
            ep->val2=-1;
           tree_decl_stmt *dst=(tree_decl_stmt *)t1->rhs->params;
           ep->t1=tt1->t1=(tree_typedecl *)(dst->lhs);
           ep->t1->libdatatype=x;
           tt1->name=malloc(sizeof(char)*100);
           strcpy(tt1->name,libdtypenames[tt1->libdatatype]);
           dir_decl *pptsize=dst->dirrhs;
           dir_decl *var=new dir_decl(); 
           var->name=malloc(sizeof(char)*100); 
           strcpy(var->name,pt1->rhs->name);
           ep->var1=var;
           ep->var1=new dir_decl();
           ep->var1->name=malloc(sizeof(char)*100);
           ep->var3=new dir_decl();
           ep->var3->name=malloc(sizeof(char)*100);
           strcpy(ep->var1->name,dst->lhs->name);
           strcpy(ep->var3->name,pt1->rhs->name);

           tt1->ppts=ep;
          symtableentry *old=NULL;
          tt1->d2=new dir_decl();
          tt1->d2->name=malloc(sizeof(char)*100);
          strcpy(tt1->d2->name,pt1->rhs->name);
          currsymtab->addsymbol((dir_decl *)var,(tree_typedecl *)tt1);

}
assign_stmt *createassign(enum ASSIGN_TYPE x){
 assign_stmt *ptr=new assign_stmt();
ptr->asstype=x;
return ptr;
}
assign_stmt *createassignlhsrhs(enum ASSIGN_TYPE x,tree_expr *lhs,tree_expr *rhs){
 assign_stmt *ptr=new assign_stmt();
ptr->asstype=x;
ptr->lhs=lhs;
ptr->rhs=rhs;
return ptr;
}
void linkstmt(statement **t1,statement *t2){
(*t1)->next=t2;
t2->prev=(*t1);
*t1=t2;
}
char *storagetypenames[]={"typedef","auto","register","static","typedef"};
tree_typedecl * createstoragedef( enum DATATYPE x,enum STORAGETYPE y){
tree_typedecl *ptr=new tree_typedecl();
ptr->datatype=x;
ptr->storagetype=TYPEDEF_STR;
ptr->name=malloc(sizeof(char )*100);
strcpy(ptr->name,storagetypenames[y]);
return ptr;
}
tree_typedecl * createtypedef( enum DATATYPE x){
tree_typedecl *ptr=new tree_typedecl();
ptr->datatype=x;
ptr->name=malloc(sizeof(char )*100);
strcpy(ptr->name,dtypenames[x]);
return ptr;
}
tree_typedecl * createlibtypedef( enum LIBDATATYPE x,char *name){
tree_typedecl *ptr=new tree_typedecl();
ptr->libdatatype=x;
ptr->name=malloc(sizeof(char )*100);
strcpy(ptr->name,libdtypenames[x]);
if(name!=NULL){
ptr->d1=currsymtab->findsymbol(name);
if(ptr->d1!=NULL)ptr->ppts=ptr->d1->ppts;
}
return ptr;
}
statement *createstmt(STMT_TYPE sttype,tree_expr *expr,char *name,int lineno){
statement *ptr=new statement();
ptr->sttype=sttype;
ptr->expr1=expr;
ptr->name=name;
ptr->lineno=lineno;
return ptr;
}
tree_typedecl *createcompounddef (enum DATATYPE x,char *name){
tree_typedecl *ptr=new tree_typedecl();
ptr->datatype=x;
ptr->compoundtype=1;
ptr->name=malloc(sizeof(char)*100);
strcpy(ptr->name,name);
return ptr;

}
tree_expr *funcallpostfix(tree_expr *t1,enum EXPR_TYPE type,int kernel, tree_expr  *arglist){
t1->expr_type=type;
t1->kernel=kernel;
if(arglist!=NULL){
t1->arglist=new assign_stmt();
t1->arglist->lhs=NULL;
t1->arglist->rhs=arglist;
assign_stmt *ass=t1->arglist;
while(ass!=NULL){
dir_decl *d1=ass->rhs->lhs;
if(d1!=NULL)d1->isparam=true;
ass=ass->next;
}
}
return t1;
}
tree_decl_stmt *createdeclstmt(class tree_typedecl *lhs,class tree_id *rhs,class dir_decl *dirrhs){
tree_decl_stmt *ptr=new tree_decl_stmt();
ptr->rhs=rhs;
ptr->lhs=lhs;
ptr->dirrhs=dirrhs;
return ptr;
}
dir_decl *createdirdecl(char *name,bool gpu,tree_expr *assign,int ptrcnt,bool sbrack_flag,int brack_flag,bool stat,dir_decl *typelist){
dir_decl *ptr=new dir_decl();
if(name!=NULL){
ptr->name=malloc(sizeof(char)*100);
strcpy(ptr->name,name);
}
if(gpu!=0)ptr->gpu=gpu;
if(assign!=NULL)ptr->assign=assign;
if(ptrcnt >0)ptr->ptrcnt=ptrcnt;
if(sbrack_flag!=0)ptr->sbrack_flag=sbrack_flag;
if(brack_flag>0)ptr->brack_flag=brack_flag;
if(stat>0)ptr->stat=stat;
if(typelist!=NULL)ptr->typelist=typelist;
return ptr;
}
linksinglef1(statement **t4ref,statement *t2,statement *temp3){
statement *t4=*t4ref;
if(t2->sttype=SBLOCK_STMT){
				t4->f1=t2;
				 statement *tt=t2->prev;
				while(tt->comma)tt=tt->prev;
					 tt->prev->next=t4;
					t2->prev->next=NULL;
}
		else { 
			statement *tt=temp3->prev;
			 while(tt->comma)tt=tt->prev; 
	  		tt->prev->next=t4;
			 t4->f1=temp3;
			temp3->prev->next=NULL;
		}
}
createifstmt(statement **t4ref,statement **t1ref,statement **t2ref,statement **temp3ref,int flg){
statement *t4=(*t4ref);
statement *t2=(*t2ref);
if(flg==1){
statement *t1=(*t1ref);
if(t1->nodetype==TREE_IF){
t4->f1=t2;
t4->f2=t1;
t4->sttype=IF_STMT;
statement *tt=temp;
while(tt->next!=NULL){
if(tt->stassign && tt->stassign->rhs==t4->expr1){
tt->prev->next=t4;
if(tt->next->sttype!=SBLOCK_STMT){
t4->f1=tt->next;
t4->f2=tt->next->next;
tt->next=NULL;
t4->f1->next=NULL;
break;
}else{
int cnt=1;
t4->f1=tt->next;
tt=tt->next;
while(tt->next){
if(tt->next->sttype==EBLOCK_STMT)cnt--;
if(tt->next->sttype==SBLOCK_STMT)cnt++;
if(cnt==0){
break;
}
tt=tt->next;
}
t4->f2=tt->next->next;
tt->next->next=NULL;
}
break;
}
tt=tt->next;
}
char arr[100];
t4->expr1->printcode1(t4->expr1,arr);
return;
}

statement *temp3=(*temp3ref);
		if(temp3==NULL)printf("HELLO\n");
		if(temp3->sttype==EBLOCK_STMT){
			 t4->f2=t1;
			 if(t2->sttype==SBLOCK_STMT){
					t4->f1=t2;
					 t2->prev->prev->next=t4;
					t1->prev->next=NULL;
			}else {
				t4->f1=t1->prev;
				 t4->f1->prev->prev->next=t4;
				t1->prev->next=NULL;
			}
		}
		else {
    			  t4->f2=temp3; 
			 if(t2->sttype==SBLOCK_STMT){
				 t4->f1=t2;
				  t2->prev->prev->next=t4;
				  temp3->prev->next=NULL;
			}else {
				t4->f1=temp3->prev;
				 if(t1->nodetype!=TREE_IF)t4->f1->prev->prev->next=t4;
                else {}
               temp3->prev->next=NULL;
			}
		}
		temp3->next=NULL;
		 temp3=t4;
		FUNCALL_FLAG=0;
return;
}

		 if(t2->sttype==SBLOCK_STMT){
			t4->f1=t2;
			t2->prev->prev->next=t4;
			 t2->prev->next=NULL;
		}else{
			 temp3->prev->prev->next=t4;
			 t4->f1=temp3;
			 temp3->prev->next=NULL;
		} 
		temp3->next=NULL;
		temp3=t4;
		FUNCALL_FLAG=0;
return;

}
createsinglestmt(statement **t4ref,statement **t1ref,statement **t2ref,statement **temp3ref){
statement *t4=(*t4ref);
statement *t2=(*t2ref);
statement *t1=(*t1ref);
statement *temp3=(*temp3ref);
		if(temp3==NULL)printf("HELLO\n");
	       tree_expr *bar=t4->expr1;
		 if(bar->expr_type==VAR && bar->libdtype==COLLECTION_TYPE)barrier=1;
		if(temp3->sttype==EBLOCK_STMT){
			  t4->f2=t1;
			 if(t2->sttype==SBLOCK_STMT){ 
				t4->f1=t2;
				 statement *tt=t2->prev;
				while(tt->comma)tt=tt->prev;
					 tt->prev->next=t4;
					t1->prev->next=NULL;
				}else {
					t4->f1=t1->prev; 
					statement *tt=t4->f1->prev;
					 while(tt->comma)tt=tt->prev;
					 tt->prev->next=t4;
					 t1->prev->next=NULL;
				}
	   }
	   else {
		    t4->f2=temp3; 
            linksinglef1(&t4,t2,temp3);
		    if(t2->sttype==SBLOCK_STMT){ 
			t4->f1=t2; 
			statement *tt=t2->prev;
			 while(tt->comma)tt=tt->prev;
			 tt->prev->next=t4;
			 tt->prev->next=t4;
			 temp3->prev->next=NULL;
		  }else {
			t4->f1=temp3->prev;
			statement *tt= t4->f1->prev;
			 while(tt->comma)tt=tt->prev;
			 tt->prev->next=t4;
			temp3->prev->next=NULL;
                  }
	}
	temp3->next=NULL;
        temp3=t4;
statement *t22=t4->f1;
       if(t22->prev && t22->prev->comma){
		statement *tt=t22->prev;
		tt->next=NULL;
		while(tt->comma==1)tt=tt->prev;
		tt->prev->next=t4;
		t4->f3=tt;
		 t4->comma=1;
	}
	FUNCALL_FLAG=0;
}
createsinglestmt1(statement **t4ref,statement **t2ref,statement **temp3ref){
statement *t4=(*t4ref);
statement *t2=(*t2ref);
statement *temp3=(*temp3ref);
		if(t2->sttype==SBLOCK_STMT){
			 t4->f1=t2; 
			statement *tt=t2->prev; 
			while(tt->comma)tt=tt->prev;
			 tt->prev->next=t4;
			t2->prev->next=NULL;}
		else { 
			statement *tt=temp3->prev;
			 while(tt->comma)tt=tt->prev; 
	  		tt->prev->next=t4;
			 t4->f1=temp3;
			temp3->prev->next=NULL;
		}
		temp3->next=NULL;
		temp3=t4;
		statement *tx;
		tx=t4->f1;	
		if(tx->prev && tx->prev->comma){
			statement *tt=tx->prev;
			tt->next=NULL;
			while(tt->comma==1)tt=tt->prev;
			tt->prev->next=t4;
			t4->f3=tt;
			 t4->comma=1;		
		}
		FUNCALL_FLAG=0;
}
initforeach(statement **t1ref,statement **temp3ref,char *name1,char *name2,int itr){
statement *t1=*t1ref;
statement *temp3=*temp3ref;
			 dir_decl *d1=currsymtab->findsymbol(name1);
			 dir_decl *d2=currsymtab->findsymbol(name2);
			 temp3->expr1=new tree_expr(d1);
			 temp3->expr2=new tree_expr(d2); 
			 temp3->itr=itr;
			if(tempitr!=NULL){
				temp3->expr3=new tree_expr();
				 temp3->expr3->name=malloc(sizeof(char)*100);
				strcpy(temp3->expr3->name,tempitr);
				tempitr=NULL;
				temp3->expr3->expr_type=VAR;
			}
}
initforeach1(statement **temp3ref,char *name1,char *name2,int itr){
statement *temp3=*temp3ref;
			 dir_decl *d1=currsymtab->findsymbol(name1);
			 dir_decl *d2=currsymtab->findsymbol(name2);
			 temp3->expr1=new tree_expr(d1);
			 temp3->expr2=new tree_expr(d2); 
			 temp3->itr=itr;
			if(tempitr!=NULL){
				temp3->expr3=new tree_expr();
				 temp3->expr3->name=malloc(sizeof(char)*100);
				strcpy(temp3->expr3->name,tempitr);
				tempitr=NULL;
				temp3->expr3->expr_type=VAR;
			}
}
dir_decl *createdirdeclforeach(char *name1,char *name2, int itrtype,int dtype,int forit,int it){
		dir_decl *d1=new dir_decl();
		d1->name=malloc(sizeof(char)*100); 
		strcpy(d1->name,name1);
		d1->libdtype=itrtype;
		d1->dtype=dtype;
		d1->forit=forit;
		if(tempitr!=NULL){
			d1->extra_name2=malloc(sizeof(char)*100);
			strcpy(d1->extra_name2,tempitr);
		}
		tree_typedecl *tp1=new tree_typedecl();
		tp1->libdatatype=itrtype;
		tp1->datatype=dtype;
		d1->it=it; 
		tp1->d1=currsymtab->findsymbol(name2);
		if(currsymtab->findsymbol(name1)==NULL){
			currsymtab->addsymbol(d1,tp1);
			d1->parent=tp1->d1;
			if(tp1->d1&& tp1->d1->gpu==1)KERNEL=1;
		}
return d1;
}
initforstmt(statement **t1ref,tree_decl_stmt *ass1,assign_stmt *ass2,assign_stmt *ass3){
statement *t1=*t1ref;
            t1->sttype=FOR_STMT;
	if(ass1!=NULL){		t1->f1=new statement();
			t1->f1->stdecl=ass1;}
if(ass2!=NULL){			t1->f2=new statement();
			 t1->f2->stassign=ass2;}
if(ass3!=NULL){			 t1->f3=new statement();
			 t1->f3->stassign=ass3;}
}
initforstmt1(statement **t1ref,assign_stmt *ass1,assign_stmt *ass2,assign_stmt *ass3){
statement *t1=*t1ref;
            t1->sttype=FOR_STMT;
	if(ass1!=NULL){		t1->f1=new statement();
			t1->f1->stassign=ass1;}
if(ass2!=NULL){			t1->f2=new statement();
			 t1->f2->stassign=ass2;}
if(ass3!=NULL){			 t1->f3=new statement();
			 t1->f3->stassign=ass3;}
}
