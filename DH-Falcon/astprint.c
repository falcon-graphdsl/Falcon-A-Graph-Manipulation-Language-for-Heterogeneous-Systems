
//#include "falctypes.h"
#include <symtabold.h>
#include<stdlib.h>
#include "externs.h"
extern int utflag;
void proc_id::print(){
printf (" %s (",this->name);
tree_decl_stmt *temp=this->params,*t2;

while(temp!=NULL){argstack[argtop]=temp;temp=temp->next;argtop++;}
while(argtop>1){
t2=argstack[argtop-1];
printf("%s ",dtypenames[t2->lhs->datatype]);
if(t2->rhs!=NULL)printf("%s ",t2->rhs->name);
printf(",");
argtop--;
}
if(argtop==1){ t2=argstack[argtop-1];
		printf("%s ",dtypenames[t2->lhs->datatype]);
if(t2->rhs!=NULL)               printf("%s ",t2->rhs->name);
              }
if(this->vararg==1)printf(",...");
printf(")");
argtop=0;
}
statement::print(){
static int n;
int xu;
//printf("statement print");
if(this!=NULL);
if(this->sttype==SBLOCK_STMT){
printf("{");
//if(this->next!=NULL)this->next->print();
}
if(this->sttype==EBLOCK_STMT){
printf("}");
//if(this->next!=NULL)this->next->print();
}
if(this->sttype==FOR_EBLOCK_STMT){
printf("}");
//if(this->next!=NULL)this->next->print();
}
if(this->sttype==RETURN_STMT){
printf("return ");
if(this->expr1!=NULL)this->expr1->print(this->expr1);
printf(";");
}
if(this->sttype==GOTO_STMT){
printf("goto %s ;",this->name);
}
if(this->sttype==CONTINUE_STMT){
printf("continue;");
}
if(this->sttype==EMPTY_STMT){
printf(";");
}
if(this->sttype==SWITCH_STMT){
printf("switch (");
this->expr1->print(this->expr1);
printf(")");
if(this->f1!=NULL)this->f1->print();
}
if(this->sttype==CASE_STMT){
printf("case ");
this->expr1->print(this->expr1);
printf(" :");
}
if(this->sttype==DEFAULT_STMT){
printf("default");
this->expr1->print(this->expr1);
printf(" :");
}
if(this->sttype==BREAK_STMT){
printf("break;");
}
if(this->sttype==MACRO_STMT){
printf("\n#define ");
this->expr1->print(this->expr1);
printf ( " ");

if(this->f1!=NULL)this->f1->print();
printf("\n");
}
if(this->sttype==SECTIONS_STMT){
printf(" sections statement\n");
if(this->f1!=NULL)this->f1->print();
}
if(this->sttype==SECTION_STMT){
printf(" section statement\n");
if(this->f1!=NULL)this->f1->print();
}
if(this->sttype==SINGLE_STMT){
tree_expr *t1=this->expr1;
while(t1->lhs!=NULL && t1->lhs->expr_type!=VAR)t1=t1->lhs;

printf("single( ");
this->expr1->print(this->expr1);
printf(")");
if(this->f1!=NULL) this->f1->print();
if(this->f2!=NULL){
printf("else ");
this->f2->print();
}
}
if(this->sttype==IF_STMT){
printf("if( ");
this->expr1->print(this->expr1);
printf(" )");
if(this->f1!=NULL)this->f1->print();
if(this->f2!=NULL){
printf("else ");
this->f2->print();
}
}
if(this->sttype==ASSIGN_STMT){
///following if code can be delete i guess.taken care by DOWHILEEXPR_STMT
if(this->stassign->rhs==NULL&&this->sttype==ASSIGN_STMT){if(this->stassign->lhs!=NULL/* && this->stassign->lhs->name!=NULL*/){ this->stassign->lhs->print(this->stassign->lhs);printf("=");}}
if(this->stassign->asstype==DOWHILE_EXPR){
printf("while ( "); 
this->stassign->rhs->print(this->stassign->rhs);
printf(");");
}

if(this->stassign->asstype!=WHILE_EXPR && this->stassign->asstype!=-1){
/*if(this->stassign->rhs==NULL || this->stassign->rhs->expr_type==EXPR){this->stassign->print1(1);}*/
}
if(this->stassign->rhs!=NULL && this->stassign->rhs->expr_type!=FUNCALL)this->stassign->print1(1);
if(this->stassign->rhs!=NULL && this->stassign->rhs->expr_type==FUNCALL){
tree_expr *f1=this->stassign->rhs;
this->stassign->print1(1);
}
}

if(this->sttype==WHILE_STMT){
printf("while(");
this->expr1->print(this->expr1);
printf(") ");
if(this->stassign!=NULL)this->stassign->print1(1);//this->body->print();
else printf(" {");
}
if(this->sttype==DOWHILE_STMT){
printf("do {\n");
}
if(this->sttype==DOWHILEEXPR_STMT){
printf("while ( ");
this->expr1->print(this->expr1);
printf(");");
}
if(this->sttype==FOREACH_STMT){
fprintf(stderr,"FOREACH STMT\n");
if(this->expr2==NULL)fprintf(stderr,"EXPR2 NULL\n");
if(this->expr4 && this->expr4->lhs->expr_type!=STRUCTREF){
fprintf(stderr,"condition on iterator should be on lhs\n");errflag=1;
}
dir_decl *d2=this->expr2->lhs;
int it=this->itr;
if(d2==NULL)fprintf(stderr,"NULL");
fprintf(stderr," %d ",this->itr);
if(it<6)printf("foreach ( %s in %s.%s)",this->expr1->name,this->expr2->name,iternames[this->itr]);
if(d2->libdtype==GRAPH_TYPE){
if(it==0)this->expr1->lhs->libdtype=POINT_TYPE;
else if(it==1)this->expr1->lhs->libdtype=EDGE_TYPE;
else if(it<6)fprintf(stderr,"invalid iterator for Graph type");
else {
int flag=0,xu;
if(this->expr3!=NULL)printf("foreach (%s in %s.%s)",this->expr1->name,this->expr2->name,this->expr3->name);
}
}
if(d2->libdtype==POINT_TYPE){

if(it==2)this->expr1->lhs->libdtype=POINT_TYPE;
else if(it==3)this->expr1->lhs->libdtype=POINT_TYPE;
else if(it==4)this->expr1->lhs->libdtype=POINT_TYPE;
else fprintf(stderr,"invalid iterator for Point datatype");
}
if(d2->libdtype==COLLECTION_TYPE){
//printf("here");
if(it==5){
if(d2->tp1==NULL)printf("NULL");
tree_typedecl *t1=d2->tp1;
if(d2->tp1->next==NULL){
this->expr1->lhs->libdtype=-1;;this->expr1->lhs->dtype=d2->tp1->datatype;

}
else if(d2->tp1->next->next==NULL){fprintf(FP1,"H1\n"); this->expr1->lhs->libdtype=d2->tp1->next->libdatatype; this->expr1->lhs->dtype=d2->tp1->next->datatype;}
else{ this->expr1->lhs->libdtype=COLLECTION_TYPE;((dir_decl *)this->expr1->lhs)->tp1=d2->tp1->next;}

}
else fprintf(stderr,"invalid iterator for Collection datatype");

}
if(d2->libdtype==SET_TYPE ){
fprintf(FP1,"<CAN BE BUG>");
if(it==5){
tree_typedecl *t1=d2->tp1;
if(d2->tp1->next==NULL){
this->expr1->lhs->libdtype=-1;;this->expr1->lhs->dtype=d2->tp1->datatype;

}
else if(d2->tp1->next->next==NULL){fprintf(FP1,"H1\n"); this->expr1->lhs->libdtype=d2->tp1->next->libdatatype; this->expr1->lhs->dtype=d2->tp1->next->datatype;}
else{ this->expr1->lhs->libdtype=SET_TYPE;((dir_decl *)this->expr1->lhs)->tp1=d2->tp1->next;fprintf(FP1,"H1\n");}
}
else fprintf(stderr,"invalid iterator for Collection datatype");
}
if(this->stassign!=NULL) {this->stassign->print1(1);
check_morph_foreach(this);
}
else printf("{");
}
if(this->sttype==FOR_STMT){
printf("for (");
if(this->f1->stdecl!=NULL){
printf("%s ", dtypenames[this->f1->stdecl->lhs->datatype]);
if(this->f1->stdecl->dirrhs!=NULL) this->f1->stdecl->dirrhs->print();
printf(";");
}
if(this->f1->stassign!=NULL)this->f1->stassign->print1(1);
if(this->f2->stassign!=NULL){this->f2->stassign->print1(0);
printf(";");
}
if(this->f3!=NULL&& this->f3->stassign!=NULL)this->f3->stassign->print1(0);
if(this->stassign!=NULL){
printf(")");
this->stassign->print1(1);
printf("\n");
}
else printf(") {");
}
if(this->sttype==DECL_STMT){
if( this->ker==1)printf("THIS FUN I KERNEL\n");
tree_typedecl *x1=this->stdecl->lhs;
if(x1->libdatatype!=-1){
//if(x1->libdatatype!=SET_TYPE && x1->libdatatype!=COLLECTION_TYPE)alloc_extra(this);a
alloc_extra(this);
realloc_graph(this);
lock_graph(this);
printf("%s ",libdtypenames[x1->libdatatype]);
if(x1->libdatatype==EDGE_TYPE|| x1->libdatatype==POINT_TYPE){
if(x1->d1!=NULL)printf("(%s)",x1->d1->name);
}
if(x1->libdatatype==SET_TYPE || x1->libdatatype==COLLECTION_TYPE){
tree_typedecl *t1=x1->next;
int cnt=0;
while(t1!=NULL && t1->next!=NULL &&t1->libdatatype==-1 ){ printf ("[ "); cnt++;t1=t1->next;}

if(t1!=NULL && t1->libdatatype!=-1){
if(t1->d1!=NULL)printf("[ %s ( %s ) ] ",libdtypenames[t1->libdatatype],t1->d1->name);
}
if(t1!=NULL && t1->datatype!=-1){
if(t1->datatype!=STRUCT_TYPE&& t1->datatype!=ENUM_TYPE && t1->datatype!=UNION_TYPE)printf("[ %s ] ",dtypenames[t1->datatype]);
else printf("[ %s ] ",t1->name);
}
while(cnt >0){printf (" ] ");cnt--;}

}
if(x1->libdatatype==G_P_TYPE||x1->libdatatype==E_P_TYPE||x1->libdatatype==P_P_TYPE){
if(x1->d1==NULL)printf("G_P");
else {
printf("[%s,%s,",x1->d1->name,x1->t1->name);
if(x1->libdatatype==E_P_TYPE||x1->libdatatype==P_P_TYPE){
if(x1->d2!=NULL)printf("%s]",x1->d2->name);
else printf("%d]",x1->libsize);
}
else {
if(x1->d2!=NULL)printf("%s,",x1->d2->name);
else printf("%d,",x1->libsize);
}
if(x1->d3!=NULL)printf("%s]",x1->d3->name);
else if ( x1->libdatatype==G_P_TYPE)printf("%d]",x1->libelement);
}
}
if(this->stdecl->dirrhs!=NULL) {
printf("********dirrhs*******");
this->stdecl->dirrhs->print();
printf("********dirrhs*******");
printf(";");
}
}
if(x1->libdatatype==-1){
while(x1  && (x1->datatype==-1 || x1->datatype==TYPEDEF_TYPE)){printf("%s",x1->name);x1=x1->next;}
if(x1->datatype==STRUCT_TYPE||x1->datatype==ENUM_TYPE||x1->datatype==UNION_TYPE){
this->stdecl->print();
}
else {
if(this->stdecl->dirrhs!=NULL &&this->stdecl->dirrhs->procd!=1){
tree_typedecl *t1=this->stdecl->lhs;
while(t1 && (t1->datatype==TYPEDEF_TYPE||t1->datatype==-1) && t1->libdatatype==-1){t1=t1->next;}
while(t1!=NULL ){if(t1->name!=NULL){printf("%s ", t1->name);}t1=t1->next;}
if(this->stdecl->dirrhs!=NULL) {

this->stdecl->dirrhs->print();
printf(";");
}
}
}
if(this->stdecl->dirrhs!=NULL &&this->stdecl->dirrhs->procd==1){
printf("\n");
if(this->stdecl->lhs!=NULL){printf("%s ", dtypenames[this->stdecl->lhs->datatype]);}
if(this->stdecl->dirrhs!=NULL) {
this->stdecl->dirrhs->print();
//if(this->next && this->next->sttype!=SBLOCK_STMT)printf(";");
}

}
}
if(this->stdecl->dirrhs!=NULL && this->stdecl->dirrhs->tp1==NULL && this->stdecl->lhs->libdatatype==SET_TYPE)fprintf(FP1,"NULL HERE print statement");
}
if(this->next&& this->next->lineno>this->lineno)printf("\n");
if(this->next &&this->next->comma==1)printf(" , ");
if(errflag==1)fprintf(stderr,"lineno=%d\n",this->lineno);
errflag=0;
//printf("printint next");
if(this->next!=NULL)this->next->print();

}
int tree_decl_stmt::print(){
//printf("%s  ",dtypenames[this->lhs->datatype]);
tree_typedecl *x1=this->lhs;
while(x1 && (x1->datatype==-1 || x1->datatype==TYPEDEF_TYPE)){x1=x1->next;}
if(x1!=NULL && x1->name!=NULL)printf(" %s ",x1->name);
if(x1->def==true){
printf("{\n");
if(x1->datatype==ENUM_TYPE){ 
dir_decl *head=this->lhs->list->dirrhs;
head->print();
printf("}");
if(this->dirrhs!=NULL)this->dirrhs->print();
printf(";\n");
//while(head!=NULL){ printf("%s,\n",head->name);if(head->idrhs!=NULL)head=head->next;}
return;


}
tree_decl_stmt *head= x1->list;
while(head!=NULL){if(head->lhs->compoundtype==1){if(head->lhs->datatype==STRUCTREC_TYPE&& ( (strcmp(this->lhs->vname,head->lhs->vname)!=0)||(strlen(this->lhs->vname)!=strlen(head->lhs->vname))))printf("improper struct rec use\n");  head->print();head=head->next;continue;}
tree_typedecl *t1=head->lhs;
if(t1 && t1->datatype==STRUCTREC_TYPE&& strcmp(x1->name,t1->name)!=0)printf("improper struct rec use\n");
while(t1!=NULL)if(t1->name!=NULL){printf("%s ", t1->name);t1=t1->next;}
if(head->dirrhs!=NULL)head->dirrhs->print();
printf(";\n");
 head=head->next;}
printf("}");
}

if(this->dirrhs!=NULL) this->dirrhs->print();
if(this->dirrhs!=NULL || this->lhs->compoundtype==1)printf(";\n");//modified on feb12
}
void tree_id::print(tree_id *id){
int n;
varname[vartop++]=id;
tree_id *temp=id;
while(temp->next!=NULL){varname[vartop++]=temp->next; temp=temp->next;}
tree_id *t1= varname[vartop-1];
if(t1->dirdecl!=NULL)t1->dirdecl->print();
if(t1->rhs!=NULL){
printf("=");
t1->rhs->print(t1->rhs);
}
vartop--;
while(vartop!=0){
printf(",");
t1=varname[vartop-1];
if(t1->dirdecl!=NULL)t1->dirdecl->print();
if(t1->rhs!=NULL){
printf("=");
t1->rhs->print(t1->rhs);
}

vartop--;
}

}
void dir_decl::print(){
int n;
dir_decl *t1= this;
if(t1->parent!=NULL&&t1->libdtype!=COLLECTION_TYPE)printf(" %s",t1->parent->name);
if(t1->open_brack==1)printf("( ");
if(t1->ptrcnt>0){if(t1->ptrstring!=NULL)printf("%s ",t1->ptrstring);};
if(t1->name!=NULL){
if(t1->gpu)printf("<GPU>");
printf("%s ",t1->name);
}
if(t1->tp1!=NULL &&  (t1->libdtype==SET_TYPE|| t1->libdtype==COLLECTION_TYPE)){
tree_typedecl *tt1=t1->tp1->next;
if(tt1!=NULL && t1->tp1->d1!=NULL && tt1->libdatatype!=-1){
printf("[%s ( %s ) ]",libdtypenames[tt1->libdatatype],t1->tp1->d1->name);
t1->dev_no=t1->tp1->d1->dev_no;
}
if(tt1!=NULL && tt1->datatype!=-1&& t1->tp1->d1!=NULL){
if(tt1->datatype!=STRUCT_TYPE&& tt1->datatype!=ENUM_TYPE && tt1->datatype!=UNION_TYPE)printf("[ %s ] ",dtypenames[tt1->datatype]);
else printf("[ %s (%s ) ] ",tt1->name,t1->tp1->d1->name);
}
}
if(t1->structexpr!=NULL){
printf(" : ");
t1->structexpr->print(t1->structexpr);
}
if(t1->close_brack==1)printf(") ");
if(t1->brack_flag==2)printf("( ");
if(t1->sbrack_flag==1)printf("[ ");
if(t1->stat==1)printf("static ");
if(t1->typelist!=NULL && t1->typelist->name!=NULL) printf("%s ",t1->typelist->name);
if(t1->assign!=NULL){
t1->assign->print(t1->assign);

}
if(t1->ptrflag==1)printf(" *");
if(t1->parnames!=NULL){
dir_decl *t2=t1->parnames->dirrhs;
while(t2!=NULL && t2->nextv!=NULL){
if(t2->name!=NULL)printf("%s,",t2->name);
t2=t2->nextv;
}

if(t2!=NULL &&t2->name!=NULL)printf("%s",t2->name);
}
if(t1->params!=NULL){
tree_decl_stmt *t2=t1->params;
while(t2!=NULL && t2->next!=NULL){
//if(t2->lhs!=NULL && t2->lhs->datatype!=-1)printf("%s",dtypenames[t2->lhs->datatype]);
if(t2->lhs!=NULL&&t2->lhs->name!=NULL)printf("%s",t2->lhs->name);
//if(t2->lhs->libdatatype!=-1)printf("xxxx%s",libdtypenames[t2->lhs->datatype]);
if(t2->dirrhs!=NULL)t2->dirrhs->print();
printf(",");
t2=t2->next;
}
if(t2!=NULL &&t2->lhs!=NULL&&t2->lhs->name!=NULL)printf("%s",t2->lhs->name);
if(t2!=NULL && t2->dirrhs!=NULL)t2->dirrhs->print();
}
if(t1->brack_flag==2)printf(") ");
if(t1->sbrack_flag==1)printf("] ");
if(t1->next!=NULL)t1->next->print();
if(t1->rhs!=NULL){
printf("="); t1->rhs->print(t1->rhs);
}
if(t1->nextv!=NULL){
printf(",");
t1->nextv->print();
}
return;
}
void tree_expr::print(tree_expr *expr){
int n;
if(expr==NULL)return;
if(expr->pflag==100){expr->pflag=-100;return;}
int type=expr->expr_type;
fprintf(stderr," TYPE=%d ",type);
if(type==GET_TYPE){
printf("%s.getType() ", expr->lhs->name);
dir_decl *d1= expr->rhs->nextv;
while(d1->nextv!=NULL){
if(d1->gpu)printf("<GPU> ");printf("%s,",d1->name);d1=d1->nextv;
if(expr->lhs!=NULL && expr->lhs->lhs!=NULL && expr->lhs->lhs->extra_name!=NULL){
d1->extra_name=malloc(sizeof(char)*100);
strcpy(d1->extra_name,expr->lhs->lhs->extra_name);
}
}
if(d1!=NULL && d1->gpu)printf("<GPU> ");
if(d1!=NULL)printf("%s",d1->name);
if(d1!=NULL && expr->lhs!=NULL && expr->lhs->lhs!=NULL &&expr->lhs->lhs->extra_name!=NULL){
d1->extra_name=malloc(sizeof(char)*100);
strcpy(d1->extra_name,expr->lhs->lhs->extra_name);
}
}
if(type==CAST_EXPR){
if(expr->tpname!=NULL){
printf("(");
expr->tpname->print();
printf(")");
//expr->rhs->print(expr->rhs);
}
}
if(type==TYPE_SIZEOF){
printf("sizeof");
if(expr->tpname!=NULL){
printf("(");
expr->tpname->print();
printf(")");
}
}

if(type==TYPE_INIT){
if(expr->tpname!=NULL){
printf("(");
expr->tpname->print();
printf(")");
}
}
//printf("UNNI\n");
if(type==COND_OP){
printf("(");
expr->lhs->print(expr->lhs);
printf(") ?");
expr->rhs->lhs->print(expr->rhs->lhs);
printf(" : ");
expr->rhs->rhs->print(expr->rhs->rhs);

}
if(type==STRUCTREF){
fprintf(stderr,"LHS print\n");
int xu;
//if(expr->lhs==NULL)printf("lhs NULL");
if(expr->lhs!=NULL&& expr->lhs->expr_type==STRUCTREF){
fprintf(stderr,"LHS print\n");
expr->lhs->print(expr->lhs);
//if(expr->lhs->parent!=NULL)printf(" sref parent=%s\n",expr->lhs->parent->name);
if(expr->lhs->libdtype>=0){
fprintf(stderr,"LHS print\n");
//printf(" LIBDTYPe=%d\n",expr->lhs->libdtype);
}
if(expr->lhs->libdtype==COLLECTION_TYPE && !strcmp(expr->rhs->name,"size")){
expr->dtype=INT_TYPE;
expr->dtype=expr->lhs->dtype;
expr->rhs->print(expr->rhs);
return;
}
}
else {
fprintf(stderr,"LHS print\n");
dir_decl *d1= (dir_decl *)(expr->lhs->lhs);
if(d1==NULL)printf("d1 null return");
fprintf(stderr,"LHS print\n");
if( !strcmp("reset",expr->rhs->name) ){
//fprintf(FP1,"x %d  %d  x ",expr->lhs->libdtype,expr->rhs->dtype);
//fprintf(FP1," RESET POINT\n");
return;
}
if( !strcmp("src",expr->rhs->name) ){

utflag=0;
}
sref=sref+1;
if( d1->parent!=NULL){expr->parent=d1->parent;}//FEB14
//printf("XXXX");
if(d1==NULL)fprintf(stderr,"D1 NULL ");
if(d1!=NULL)fprintf(stderr,"D1 NOT NULL %s",d1->name);
if(d1->libstable!=NULL){
char *name=expr->rhs->name;
fprintf(stderr,"LIBDTYPE NOT NULL %s \n",expr->rhs->name);
int flag=0,i=0;
enum SIZES size;
if(d1->libdtype==GRAPH_TYPE ){
fprintf(stderr,"LIBDTYPE NOT NULL\n");
if(strstr(expr->rhs->name,"read")!=NULL)d1->read=1;
}
if(d1->libdtype==COLLECTION_TYPE ){
printf("COLLECTION TYPE %s",expr->rhs->name);
if(expr->rhs && expr->rhs->expr_type==FUNCALL){
for(int i=0;i<6;i++){
if(!strcmp(expr->rhs->name,libdtypefunctions[4][i])&&(strlen(expr->rhs->name)==strlen(libdtypefunctions[4][i]))){
printf("%s.",d1->name);
expr->libdtype=-1; expr->rhs->print(expr->rhs);return;}
}
}
}
if(expr->rhs && d1->libdtype==SET_TYPE ){
if(expr->rhs->expr_type==FUNCALL){
for(int i=0;i<4;i++){
if(!strcmp(expr->rhs->name,libdtypefunctions[3][i])&&(strlen(expr->rhs->name)==strlen(libdtypefunctions[3][i]))){
//printf("%s.",d1->name);
expr->libdtype=-1; expr->rhs->print(expr->rhs);return;}
}
}
}
if(d1->libdtype==G_P_TYPE){fprintf(stderr,"*****G_P_TYPE*****\n");expr->lhs->print(expr->lhs); expr->libdtype=G_P_TYPE; expr->rhs->print(expr->rhs);return;}
//printf("here");
size=0;
if(d1->libdtype==GRAPH_TYPE)size=fieldgraph;
if(d1->libdtype==EDGE_TYPE)size=fieldedge;
if(d1->libdtype==POINT_TYPE)size=fieldpoint;
if(d1->libdtype==SET_TYPE)size=fieldset;
if(d1->libdtype==COLLECTION_TYPE)size=fieldcollection;
for(i=0;i<size;i++){
if(!strcmp(name,libdtypefields[expr->lhs->lhs->libdtype][i])&&strlen(name)==strlen(libdtypefields[expr->lhs->lhs->libdtype][i])){
flag=1;
break;
}
}
if(d1->libdtype==GRAPH_TYPE)fprintf(stderr,"GRAPH %d",flag);
if(d1->libdtype==GRAPH_TYPE && flag==0){
for (std::map<char *,int>::iterator it1=newitr.begin(); it1!=newitr.end(); ++it1){
if(!strcmp(it1->first,name)&& (strlen(name)==(1+strlen(it1->first)))){
flag=1;
}
}
}
if(flag!=1){
//printf("enterring cuntiion\n");
size=0;
if(expr->lhs->lhs->libdtype==GRAPH_TYPE)size=fungraph;
if(expr->lhs->lhs->libdtype==EDGE_TYPE)size=funedge;
if(expr->lhs->lhs->libdtype==POINT_TYPE)size=funpoint;
for(i=0;i<size;i++){
if(!strcmp(name,libdtypefunctions[expr->lhs->lhs->libdtype][i])&&strlen(name)==strlen(libdtypefunctions[expr->lhs->lhs->libdtype][i])){
if(strstr(name,"add")!=NULL&& strstr(name,"Property")==NULL)MORPH_FLAG=1;
flag=2;
break;
}
}
}
if(flag==1 /*&& ariflag==1*/){
//printf("MATCHED\n");
if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
if(((i==1)||(i==3)) && expr->rhs->expr_type!=ARRREF &&ariflag==1){
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
else if(expr->rhs->expr_type==ARRREF&&((i==0)||(i==2)|| (i==4)|| (i==5))&&ariflag==1){
fprintf(stderr,"ivalid use of field %s\n ",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
else if((i==1) /*&& expr->rhs->expr_type==ARRREF*/){expr->libdtype=POINT_TYPE; /*printf("point type\n");*/expr->parent=d1;
}
else if((i==3) /*&& expr->rhs->expr_type==ARRREF*/){
expr->libdtype=EDGE_TYPE;
expr->parent=d1;
}
}
if(expr->lhs->lhs->libdtype==POINT_TYPE){
fprintf(stderr,"MATCHED\n");
if(((i==4)||(i==5)||(i==6)) && expr->rhs->expr_type!=ARRREF){
fprintf(stderr,"ivalid use of field %s\n ",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
else if(expr->rhs->expr_type==ARRREF&&((i==0)|| (i==1)||(i==2)||(i==3) )){
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
if((i==4) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
if((i==5) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
if((i==6) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
}
if(expr->lhs->lhs->libdtype==EDGE_TYPE){
//printf("edGE TYPE \n");
if(((i==0)||(i==1)) && expr->rhs->expr_type!=VAR){
fprintf(stderr,"ixxvalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
if(expr->rhs->expr_type==ARRREF&& i==2){
fprintf(stderr,"iyyvalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
if((i==0) && expr->rhs->expr_type==VAR){ /*printf("edGE TYPE %d\n",i);*/expr->libdtype=POINT_TYPE;  }
if((i==1) && expr->rhs->expr_type==VAR){ /*printf("edGE TYPE %d\n",i); */ expr->libdtype=POINT_TYPE; }
}
if(expr->lhs->lhs->libdtype==COLLECTION_TYPE){
printf("collection");
expr->libdtype=-1; expr->dtype=INT_TYPE;
}
}

if(flag==2 ){

if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
//fprintf(FP1,"// %shere %d\n",expr->lhs->lhs->name,i);
if(i==5) {expr->libdtype=EDGE_TYPE;expr->parent=expr->lhs->lhs;}
else expr->libdtype=-2;

}
}
if(flag==0){
fprintf(stderr,"CHECKING EXTRA PPTS\n");
extra_ppts *ex=NULL;
if(d1->libdtype==GRAPH_TYPE)ex=d1->ppts;
else if(d1->parent!=NULL){ex=d1->parent->ppts;/*fprintf(stderr,"parent=%d %s %s ",d1->parent->arg,d1->name,d1->parent->name);*/}
else if(d1->arg==1)ex=d1->ppts;
//if(ex!=NULL)fprintf(FP1,"NO EXTRA\n");
while(ex!=NULL&& flag==0){
fprintf(stderr,"%s %s",expr->rhs->name,ex->name);
if(!strcmp(ex->name,expr->rhs->name)&& strlen(ex->name)==strlen(expr->rhs->name)){
if(ex->libdtype!=d1->libdtype){
//********************************NEED TO ADD CHECK FOR ACCESS OF POINT/EDGE AND EXTRA PROPERTY********************************
//fprintf(FP1,"flag=1");
if(d1->libdtype==GRAPH_TYPE)expr->parent=d1;
else expr->parent=d1->parent;
if(d1->libdtype==GRAPH_TYPE && ex->libdtype!=G_P_TYPE &&ariflag==1){
fprintf(stderr,"cannot directly access %s property %s from graph  %s\n",libdtypenames[ex->libdtype] , ex->name,d1->name);
errflag=1;
}else {expr->libdtype=ex->t1->libdatatype;expr->dtype=ex->t1->datatype;flag=1;
//fprintf(FP1,"FLAg=1 %d %d ",ex->t1->libdatatype,ex->t1->datatype);
if(ex->dt1!=NULL)ex->dt1->printcode(0);
break;}//mdofied on fe14AN
}
if(ex->t1==NULL)printf("T! NULL\n");
//fprintf(FP1,"here %d %d",expr->dtype,ex->t1->datatype);
if(ex->t1->datatype!=-1&& expr->rhs->expr_type==VAR){expr->libdtype=ex->t1->libdatatype;
expr->dtype=ex->t1->datatype;
//printf("here %d",expr->dtype);
}
if(ex->t1->libdatatype==-1&& expr->rhs->expr_type==VAR){expr->libdtype=ex->t1->libdatatype;
expr->dtype=ex->t1->datatype;
}
flag=1;
break;
}
//printf("%s",expr->rhs->name);
ex=ex->next;
}
}
fprintf(stderr,"xxxxx\n");
//fprintf(stderr,"\n***%d\n",flag);
if(flag==0 && d1->arg==0 && !(d1->parent!=NULL && d1->parent->arg==1)&&d1->libdtype>=0){fprintf(stderr,"ivalid field access %s for %s datatype\n",name,libdtypenames[expr->lhs->lhs->libdtype]);errflag=1;}
//fprintf(stderr,"\n***\n");
if(flag==0 &&d1->libdtype!=-1){
extra_ppts *ex=NULL;
//printf("HERE\n");
if(d1->libdtype==GRAPH_TYPE)ex=d1->ppts;
else if(d1->parent!=NULL){ex=d1->parent->ppts;/*fprintf(stderr,"Parent=%d %s ",d1->parent->arg,d1->parent->name);*/}
tree_typedecl *t1=new tree_typedecl();
//t1->libdatatype=10;
t1->libdatatype=-2;
ex=new extra_ppts();
ex->name=malloc(sizeof(char)*100);
strcpy(ex->name,expr->rhs->name);
ex->t1=t1;
if(d1->libdtype==GRAPH_TYPE &&d1->arg==1){ ex->next=d1->ppts;d1->ppts=ex;}
else if(d1->parent!=NULL){ex->next=d1->parent->ppts;d1->parent->ppts=ex;}
else if(d1->arg==1){ ex->next=d1->ppts; d1->ppts=ex;}
}

}
else {expr->lhs->print(expr->lhs); expr->libdtype=expr->lhs->libdtype;/*printf("printing lhs first %d \n",expr->lhs->libdtype);*/if(expr->lhs->libdtype==-2){fprintf(stderr,"invalid structure access\n");errflag=1;}}
if(expr->libdtype!=-1 &&expr->libdtype!=-2  && d1->libstable==NULL){

char *name=expr->rhs->name;
//dir_decl *d2=expr->rhs->lhs;
//fprintf(stderr,"here %s\n",name);
int flag=0,i=0;
enum SIZES size;
if(expr->libdtype==GRAPH_TYPE)size=fieldgraph;
if(expr->libdtype==EDGE_TYPE)size=fieldedge;
if(expr->libdtype==POINT_TYPE)size=fieldpoint;
if(expr->libdtype==SET_TYPE)size=fieldset;
for(i=0;i<size;i++){
if((!strcmp(name,libdtypefields[expr->libdtype][i]))&&(strlen(name)==strlen(libdtypefields[expr->libdtype][i]))){
flag=1;
break;
}
}


if(flag!=1){
if(expr->libdtype==GRAPH_TYPE)size=fungraph;
if(expr->libdtype==EDGE_TYPE)size=funedge;
if(expr->libdtype==POINT_TYPE)size=funpoint;
for(i=0;i<size;i++){
if((!strcmp(name,libdtypefunctions[expr->libdtype][i]))&&(strlen(name)==strlen(libdtypefunctions[expr->libdtype][i]))){
flag=2;
break;
}
}
}
//fprintf(stderr,"here %s\n",name);
if(flag==1 /*&&ariflag==1*/){

if(expr->libdtype==GRAPH_TYPE){
if(((i==1)||(i==3)) && expr->rhs->expr_type!=ARRREF&&ariflag==1){
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);errflag=1;}
else if(expr->rhs->expr_type==ARRREF&&((i==0)||(i==2)|| (i==4)|| (i==5))&&ariflag==1){
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);errflag=1;}
else if((i==1) /*&& expr->rhs->expr_type==ARRREF*/){expr->libdtype=POINT_TYPE;}
else if((i==3)/* && expr->rhs->expr_type==ARRREF*/)expr->libdtype=EDGE_TYPE;
else if((i==0)||(i==2)||(i==4)||(i==5)||(i==6))expr->libdtype=-2;
}
else if(expr->libdtype==POINT_TYPE){
expr->libdtype=-1;
//printf("here %d\n",i);
if(((i==4)||(i==5)||(i==6)) && expr->rhs->expr_type!=ARRREF&&ariflag==1){
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);errflag=1;}
else if(expr->rhs->expr_type==ARRREF&&((i==0)|| (i==1)||(i==2)||(i==3) )){fprintf(stderr,"invalide used of filed\n");errflag=1;}
else if((i==4) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
else if((i==5) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
else if((i==6) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
 else if ((i==2)|| (i==3)||(i==7))expr->libdtype=-2;
//printf("ivalid use of field %s ",libdtypefields[expr->libdtype][i]);
}
else if(expr->libdtype==EDGE_TYPE){
//printf("EDGE TYPE\n");
if(((i==0)||(i==1)) && expr->rhs->expr_type!=ARRREF&&ariflag==1){
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);errflag=1;}
else if(expr->rhs->expr_type==ARRREF&& i==2&&ariflag==1){
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);errflag=1;}
else if((i==0) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
else if((i==1) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
else if((i==2)||(i==3))expr->libdtype=-2;
}

}

//fprintf(stderr,"here %s %di %d\n",name,expr->lhs->libdtype,d1->arg);
//if(expr->lhs && expr->lhs->parent==NULL)fprintf(stderr,"PARENT NULL");
if(expr->lhs && expr->lhs->parent!=NULL){
extra_ppts *ex=expr->lhs->parent->ppts;
//fprintf(stderr,"here %s %d %s",expr->rhs->name,flag,expr->lhs->parent->name);
while(ex!=NULL&& flag==0){
//fprintf(stderr,"here %s %s",expr->rhs->name,ex->name);
if(!strcmp(ex->name,expr->rhs->name)&& (strlen(ex->name)==strlen(expr->rhs->name))){
flag=1;
//fprintf(stderr,"matched\n");
//if(ex->var2!=NULL)fprintf(stderr,"name=%s dtyp=%d %d\n",ex->name, ex->var2->dtype,ex->t1->datatype);
if(ex->t1->datatype!=-1){expr->libdtype=-1;//THIS MAY NOT BE SUFFICIENT FEB 14 FOUND OUT
expr->dtype=ex->t1->datatype;
}
break;
}
ex=ex->next;
}
//printf("ADDING ");
if(flag==0&& expr->lhs && expr->lhs->libdtype>2)expr->libdtype=-1;
if(flag==0 && expr->lhs&& expr->lhs->parent && expr->lhs->parent->arg==1){
extra_ppts *ex2;
tree_typedecl *t12=new tree_typedecl();
//t12->libdatatype=10;
t12->libdatatype=-2;
ex2=new extra_ppts();
ex2->name=malloc(sizeof(char)*100);
strcpy(ex2->name,expr->rhs->name);
ex2->t1=t12;
ex2->next=expr->lhs->parent->ppts;
expr->lhs->parent->ppts=ex2;
}
}
//fprintf(stderr,"here %s %di %d %d\n",name,expr->lhs->libdtype,d1->arg,flag);
if(flag==0 && d1 && expr->libdtype<=2&& expr->libdtype>=0&&d1->arg==0 &&!(expr->lhs->parent!=NULL&& expr->lhs->parent->arg==1)){fprintf(stderr,"invalid field access %s for %s datatype\n",name,libdtypenames[expr->libdtype]);errflag=1;}
}
if(d1->libstable!=NULL){
expr->lhs->print(expr->lhs);
}
if(expr->lhs->dtype!=-1 && expr->lhs->dtype<=9){fprintf(stderr,"struct ref on noncompound datatype\n");errflag=1;}
printf(".");
//printf("RHS");
expr->rhs->print(expr->rhs);
expr->dtype=expr->rhs->dtype;
//printf("RHS");
sref=sref-1;
return;
}
}
if(type==PTRREF){
expr->lhs->print(expr->lhs);
printf("->");
printf("%s ",expr->rhs->name);
if(expr->lhs->parent!=NULL)expr->parent=expr->lhs->parent;//FEB14
return;
}
//printf("UNNI\n");
if(type==UNARYAND||  type==UNARYMUL || type==UNARYADD || type==UNARYOR || type==UNARYSUB || type==NOT||type==PREINC || type==PREDEC )printf("%s",exptypename[type]);
if(expr->expr_type==EXPR){
printf("(");

if(expr->exprlist!=NULL){
statement *t1=expr->exprlist;
t1->print();
}
}
if(expr->expr_type==VAR ){
if(expr->name!=NULL) printf("%s",expr->name);
if(expr->lhs!=NULL && expr->lhs->libdtype>=0&&expr->libdtype<0&&sref==0&&/*ariflag==0&&*/expr->rhs==NULL){expr->libdtype=expr->lhs->libdtype;
expr->parent=expr->lhs->parent;//FEb14
}

if(expr->expr_type==VAR)return;}
 if (expr->nodetype==TREE_INT){
printf( "%d", expr->ival);
return;
}
 if (expr->nodetype==TREE_BOOL){
if(expr->ival)printf( "true");
else printf("false");
return;
}
 if (expr->nodetype==TREE_ENUMCONST){
printf( "%s", expr->name);
return;
}
 if (expr->nodetype==TREE_FLOAT){
printf( "%f", expr->fval);
return;
}
if(expr->expr_type==NULL_ARR)return;
 if (expr->nodetype==TREE_STRING){
printf( "%s", expr->name);
return;
}
if(expr->expr_type==FUNCALL){
int unni;
if(expr->name!=NULL)printf("%s",expr->name);
if(expr->name!=NULL){
for(std::map<char *,statement *>::iterator it=fnames.begin();it!=fnames.end();it++){
if(!strcmp(it->first,expr->name)){
//printf("\n*******a call to function defined in this file*****\n");
fprintf(stderr,"unnikrishnan %s",expr->name);

statement *t1=it->second;
if(!t1){

fprintf(stderr,"unnikrishnan %s t1 null",expr->name);
continue;
}

if(expr->kernel==1)t1->ker=expr->kernel;
dir_decl *fd1=t1->stdecl->dirrhs;
if(fd1->params!=NULL){ 
tree_decl_stmt *tt=fd1->params;
assign_stmt *ta=expr->arglist;
int cnt=0;
while(tt!=NULL&&ta!=NULL){
cnt++;
if(cnt==1 && ta->next!=NULL && tt->next!=NULL){
dir_decl *tad1=ta->next->rhs->lhs;
dir_decl *ttd1= tt->next->dirrhs;
if(tad1->extra_name!=NULL)printf(" yyy%s \n",tad1->extra_name); 
if(tad1->extra_name!=NULL){
ttd1->extra_name=malloc(sizeof(char)*100);
//printf("%d SETTIG PARENT of  %s as %s in %s as %s \n",cnt, tt->lhs->name,tad1->name,it->first,ttd1->name );
strcpy(ttd1->extra_name,tad1->extra_name);
tt->lhs->parent=ttd1;
ttd1->ppts= tad1->ppts;
}
}
if(cnt==1 && ta->next==NULL && tt->next==NULL){

tt->dirrhs->extra_name=malloc(sizeof(char)*100);
if(ta->rhs!=NULL && ta->rhs->lhs!=NULL && ta->rhs->lhs->extra_name!=NULL )strcpy(tt->dirrhs->extra_name,ta->rhs->lhs->extra_name);
}
if((tt->dirrhs && tt->dirrhs->libdtype!=-1) ||(ta->rhs  && ta->rhs->lhs&& ta->rhs->lhs->forit==1)){
if(ta->rhs && ta->rhs->lhs && ta->rhs->lhs->libdtype==-1 && ta->rhs->lhs->forit!=1){fprintf(stderr,"mismatch in argument types");errflag=1;}
else {
extra_ppts *et=tt->dirrhs->ppts;
extra_ppts *et1;
if(cnt==1&& t1->ker==1){
t1->ker=1;
tt->dirrhs->parent=tt->next->dirrhs;
tt->dirrhs->forit=1;
}
if(tt->lhs->libdatatype==SET_TYPE){
tree_typedecl *de2=ta->rhs->lhs->decltype;
if(de2==NULL)fprintf(FP1,"NULL");
while(de2!=NULL && de2->next!=NULL){
de2=de2->next;
}
tree_typedecl *de1=tt->dirrhs->tp1;
if(de1==NULL)fprintf(FP1,"NULL");
while(de1!=NULL && de1->next!=NULL){
de1=de1->next;
}
}
if(cnt==1 && ta->rhs->lhs->forit==1){
tt->dirrhs->parent=tt->next->dirrhs;
tt->dirrhs->extra_name=tt->next->dirrhs->extra_name;
tt->dirrhs->forit=1;
tt->dirrhs->it=ta->rhs->lhs->it;
}
while(et!=NULL){
fprintf(stderr,"checking1 %s ",tt->dirrhs->name);
et1=ta->rhs->lhs->ppts;
if(ta->rhs->lhs->parent!=NULL)et1=ta->rhs->lhs->parent->ppts;
while(et1!=NULL){if(!strcmp(et1->name,et->name)&&(strlen(et1->name)==strlen(et->name)))break;et1=et1->next;}
if(et1==NULL){fprintf(stderr,"invalid property %s used in function %s for argument%d\n",et->name,expr->name,cnt);errflag=1;}
et=et->next;

}
}

}
if(tt->dirrhs!=NULL && ta->rhs!=NULL && ta->rhs->lhs!=NULL && tt->dirrhs->libdtype!=ta->rhs->lhs->libdtype){
if(tt->dirrhs->libdtype==-1)fprintf(stderr,"parameter%d not lib type but arg%d is of %s libdtype\n",cnt,cnt,libdtypenames[ta->rhs->lhs->libdtype]);
else if (ta->rhs->lhs->libdtype==-1)fprintf(stderr,"parameter%d is of %s lib type but arg%d is not lib type \n",cnt,libdtypenames[tt->dirrhs->libdtype],cnt);
else fprintf(stderr,"parameter%d %s lib type but arg%d is of %s lib type \n",cnt,libdtypenames[tt->dirrhs->libdtype],cnt,libdtypenames[ta->rhs->lhs->libdtype]);
errflag=1;
}
tt=tt->next;ta=ta->next;
}
}

}
}
}
//else printf("NULL\n");
 //printf("NULL\n");
printf("(");
assign_stmt *t1=expr->arglist;
while(t1!=NULL&&t1->next){
t1->print1(0);
printf(",");
t1=t1->next;
}
if(t1!=NULL)t1->print1(0);
if(expr->params!=NULL){
printf(",");
tree_decl_stmt *td=expr->params;
while(td->next!=NULL){if(td->lhs)printf("%s ",td->lhs->name);if(td->dirrhs)td->dirrhs->print();printf(",");td=td->next;}
if(td&& td->lhs)printf("%s ",td->lhs->name);
if(td&& td->dirrhs)td->dirrhs->print();
}
printf(")");
}
if (expr->lhs!=NULL &&expr->expr_type!=ARRREF&&expr->expr_type!=COND_OP&&expr->expr_type!=GET_TYPE){if(expr->expr_type!=FUNCALL){
if(expr->lhs->expr_type==VAR && expr->lhs->lhs && expr->lhs->lhs->libstable!=NULL &&ariflag==1){fprintf(stderr,"\n***binary operation not permitted directly on libdatatype***\n");errflag=1;}
if(expr->rhs && expr->rhs->expr_type==VAR && expr->rhs->lhs && expr->rhs->lhs->libstable!=NULL&& ariflag==1){fprintf(stderr,"\n***binary operation not permitted directly on libdatatype***\n");errflag=1;}
this->lhs->print(expr->lhs);
}
}
//printf("OK\n");
if(type==AND||type==ADD || type==MUL || type==SUB || type==DIV || type==MOD || type==AND || type==OR || type==XOR|| type==BAND || type==BOR)ariflag=1;
if(type==AND||type==ADD || type==MUL || type==SUB || type==DIV || type==MOD || type==AND || type==OR || type==XOR|| type==BAND || type==BOR|| type==PTRREF)printf("%s",exptypename[type]);
if(type==POSTINC|| type==POSTDEC || type==STRUCTREF || type==LT || type==GT|| type==LE || type==GE|| type==LE || type==EQ || type==NEQ)printf("%s",exptypename[type]);

if(type==ARRREF){
aref=1;
dir_decl *d1= (dir_decl *)(expr->lhs);
if(d1!=NULL && d1->libstable!=NULL){
char *name=expr->name;
int size,flag=0,i=0;
if( ariflag==1){
if(expr->lhs->libdtype==ITERATOR_TYPE&& d1->it==1){
expr->libdtype=EDGE_TYPE;
expr->parent=d1->parent;
}
if(expr->lhs->libdtype==ITERATOR_TYPE&& d1->it!=1){
expr->libdtype=POINT_TYPE;
expr->parent=d1->parent;
}
if(expr->lhs->libdtype==EDGE_TYPE)size=4;
if(expr->lhs->libdtype==POINT_TYPE)size=8;
}
}else if(expr->libdtype!=-1&&expr->libdtype!=-2){
}
printf("%s",expr->name);
assign_stmt *y=expr->earr_list;
while(y!=NULL){
printf("[");
if(y->lhs!=NULL)y->lhs->print(y->lhs);
y->rhs->print(y->rhs);

printf("]");
y=y->next;
}
aref=0;
}
if(type==INITBLOCK){
printf("{");
tree_expr *t1=expr->rhs;
while(t1->prev!=NULL)t1=t1->prev;
if(t1!=NULL) {
class designator *t2= t1->desi;
if(t2!=NULL){

while(t2!=NULL){
if(t2->expr!=NULL){
printf("[");
t2->expr->print(t2->expr);
printf("]");
}
if(t2->name!=NULL)printf(".%s",t2->name);
t2=t2->next;
}
printf("=");
}
if(t1->expr_type==VAR)printf("%s",t1->name);
if(t1->nodetype==TREE_INT)printf("%d",t1->ival);
t1=t1->next;
while(t1!=NULL){
printf(",");
class designator *t2= t1->desi;
if(t2!=NULL){

while(t2!=NULL){
if(t2->expr!=NULL){
printf("[");
t2->expr->print(t2->expr);
printf("]");
}
if(t2->name!=NULL)printf(".%s",t2->name);
t2=t2->next;
}
printf("=");
}
if(t1->expr_type==INITBLOCK)t1->print(t1);
if(t1->expr_type==VAR)printf("%s",t1->name);
if(t1->nodetype==TREE_INT)printf("%d",t1->ival);
t1=t1->next;
}
printf("}");
}
}
if(expr->rhs!=NULL&& type!=INITBLOCK && type!=COND_OP && type!=GET_TYPE){
expr->rhs->print(expr->rhs);
}

if(expr->expr_type==EXPR)printf(")");
}
	iter_stmt::print(){
	printf("while (");
	if(this->expr1!=NULL)this->expr1->print(this->expr1);
	printf( " ) {\n");
	if(this->body!=NULL){
	//printf("\nbody\n");
	this->body->print();
	}
	printf("}\n");
	}
	void assign_stmt::print1(bool semi){
	int flag=0;
	int xu;
	//printf("print1");
	if(this->asstype==EMPTY_ASSIGN ){
	if(semi)printf(";");
	return;
	}
	int type=-1;
	if(this->rhs!=NULL)type=this->rhs->expr_type;
	//ariflag=1;modif7
	ariflag=0;//modif7
	if(type==AND||type==ADD || type==MUL || type==SUB || type==DIV || type==MOD || type==AND || type==OR || type==XOR|| type==BAND || type==BOR)ariflag=1;
	if(this->lhs!=NULL){
	//printf("printing LHS\n");
	this->lhs->print(this->lhs);
	}
	else flag=1;
	//if(this->lhs->libdtype!=-1&& this->lhs->expr_type==STRUCTREF)
	ariflag=0;
	if(type==AND||type==ADD || type==MUL || type==SUB || type==DIV || type==MOD || type==AND || type==OR || type==XOR|| type==BAND || type==BOR/*|| type==STRUCTREF*/)ariflag=1;//modified on feb12
	if(this->rhs->expr_type==WHILE_EXPR)return;
	if(this->asstype==AASSIGN &&flag==0)printf("=");
	if(this->asstype==AMUL_ASSIGN&& flag==0)printf("*=");
	if(this->asstype==ADIV_ASSIGN&& flag==0)printf("/=");
	if(this->asstype==AMOD_ASSIGN&& flag==0)printf("%=");
	if(this->asstype==AADD_ASSIGN&&flag==0)printf("+=");
	if(this->asstype==ASUB_ASSIGN&&flag==0)printf("-=");
	if(this->asstype==ALEFT_ASSIGN&&flag==0)printf("<=");
	if(this->asstype==ARIGHT_ASSIGN&&flag==0)printf(">=");
	if(this->asstype==AAND_ASSIGN&&flag==0)printf("&=");
	if(this->asstype==AXOR_ASSIGN&&flag==0)printf("^=");
	if(this->asstype==AOR_ASSIGN&&flag==0)printf("|=");
	if(this->asstype==RRADD_ASSIGN&&flag==0)printf("R+=");
	if(this->rhs->pflag==100){
	this->rhs->pflag=-100;return;
	}
	//printf("RHS");
	//printf("%d xxx",type);
	if(this->rhs!=NULL){
	//printf("printing RHS non gpu\n");
this->rhs->print(this->rhs);//SEP13
}
//printf("l=%d  r=%d \n",this->lhs->libdtype,this->rhs->libdtype);
if(ariflag==0){
if(this->lhs && this->rhs &&this->lhs->libdtype>=0 && this->rhs->libdtype<0){fprintf(stderr,"mismath with left lib and right nonlib\n");errflag=1;}
if(this->lhs && this->rhs &&this->lhs->libdtype<0 && this->rhs->libdtype>=0){fprintf(stderr,"mismath with left nonlib and right lib\n");errflag=1;}
if(this->lhs && this->rhs &&this->lhs->libdtype>=0 && this->rhs->libdtype>=0  && ((this->rhs->libdtype) != (this->lhs->libdtype))){fprintf(stderr,"mismath with left lib=%sand right lib=%s\n",libdtypenames[this->lhs->libdtype],libdtypenames[this->rhs->libdtype]);errflag=1;}
//FEB14
if(this->lhs && this->lhs->libdtype>=0 ){
dir_decl *d1,*d2;
if(this->lhs){
d1=this->lhs->parent; while(d1&&d1->parent!=NULL) d1=d1->parent;
}
if(this->rhs){d2=this->rhs->lhs; while(d2 &&d2->parent!=NULL) d2=d2->parent;}
if(d1 && d1!=this->lhs->parent &&d1!=d2 )fprintf(stderr,"assigning to object with parent graph %s another object with no parent or different parent\n ",d1->name);
}
}



if(ariflag && this->lhs && this->lhs->libdtype>=0 &&this->lhs->libdtype!=this->rhs->libdtype){fprintf(stderr,"MISMATCH between left libtype=%s and right type\n",libdtypenames[this->lhs->libdtype]);errflag=1;
//ariflag=0;
if(this->rhs->libdtype>0)fprintf(stderr,"=%s\n",libdtypenames[this->lhs->libdtype]);
else fprintf(stderr,"\n");
}


if(this->rhs->pflag==-100)return;
if(this->semi)printf(";");
}
