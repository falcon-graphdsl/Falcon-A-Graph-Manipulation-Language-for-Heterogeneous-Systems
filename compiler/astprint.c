
#include <symtabold.h>
#include<stdlib.h>
#include "externs.h"
extern int utflag;
extern int ERRPRINT;

int  FALCERR;

void proc_id::print(){
tree_decl_stmt *temp=this->params,*t2;

while(temp!=NULL){argstack[argtop]=temp;temp=temp->next;argtop++;}
while(argtop>1){
t2=argstack[argtop-1];
if(t2->rhs!=NULL)printf("%s ",t2->rhs->name);
argtop--;
}
if(argtop==1){ t2=argstack[argtop-1];
		//printf("%s ",dtypenames[t2->lhs->datatype]);
//if(t2->rhs!=NULL)               printf("%s ",t2->rhs->name);
              }
argtop=0;
}
statement::print(){
static int n;
int xu;
FALCLINENO=this->lineno;
if(this!=NULL);
if(this->sttype==SBLOCK_STMT){
}
if(this->sttype==EBLOCK_STMT){
}
if(this->sttype==FOR_EBLOCK_STMT){
}
if(this->sttype==RETURN_STMT){
if(this->expr1!=NULL)this->expr1->print(this->expr1);
}
if(this->sttype==GOTO_STMT){
}
if(this->sttype==CONTINUE_STMT){
}
if(this->sttype==EMPTY_STMT){
}
if(this->sttype==SWITCH_STMT){
if(this->expr1)
this->expr1->print(this->expr1);
if(this->f1!=NULL)this->f1->print();
}
if(this->sttype==CASE_STMT){
if(this->expr1)
this->expr1->print(this->expr1);
}
if(this->sttype==DEFAULT_STMT){
if(this->expr1)
this->expr1->print(this->expr1);
}
if(this->sttype==BREAK_STMT){
}
if(this->sttype==MACRO_STMT){
if(this->expr1)
this->expr1->print(this->expr1);

if(this->f1!=NULL)this->f1->print();
}
if(this->sttype==SECTIONS_STMT){
if(this->f1!=NULL)this->f1->print();
}
if(this->sttype==SECTION_STMT){
if(this->f1!=NULL)this->f1->print();
}
if(this->sttype==SINGLE_STMT){
tree_expr *t1=this->expr1;
while(t1->lhs!=NULL && t1->lhs->expr_type!=VAR)t1=t1->lhs;

if(this->expr1)
this->expr1->print(this->expr1);
if(this->f1!=NULL) this->f1->print();
if(this->f2!=NULL){
this->f2->print();
}
}
if(this->sttype==IF_STMT){
if(this->expr1)
this->expr1->print(this->expr1);
if(this->f1!=NULL)this->f1->print();
if(this->f2!=NULL){
this->f2->print();
}
}
if(this->sttype==ASSIGN_STMT){
if(this->stassign->rhs==NULL&&this->sttype==ASSIGN_STMT){if(this->stassign->lhs!=NULL/* && this->stassign->lhs->name!=NULL*/){ this->stassign->lhs->print(this->stassign->lhs);
}}
if(this->stassign->asstype==DOWHILE_EXPR){
if(this->stassign->rhs)
this->stassign->rhs->print(this->stassign->rhs);
}

if(this->stassign->asstype!=WHILE_EXPR && this->stassign->asstype!=-1){
}
if(this->stassign->rhs!=NULL && this->stassign->rhs->expr_type!=FUNCALL)this->stassign->print1(1);
if(this->stassign->rhs!=NULL && this->stassign->rhs->expr_type==FUNCALL){
tree_expr *f1=this->stassign->rhs;
this->stassign->print1(1);
}
}

if(this->sttype==WHILE_STMT){
if(this->expr1)
this->expr1->print(this->expr1);
if(this->stassign!=NULL)this->stassign->print1(1);//this->body->print();
}
if(this->sttype==DOWHILE_STMT){
}
if(this->sttype==DOWHILEEXPR_STMT){
if(this->expr1)
this->expr1->print(this->expr1);
}
if(this->sttype==FOREACH_STMT){
if(this->expr4 && this->expr4->lhs->expr_type!=STRUCTREF){
if(ERRPRINT)
fprintf(stderr,"error line no:%d-condition on iterator should be on lhs\n",this->lineno);errflag=1;
}
dir_decl *d2=this->expr2->lhs;
int it=this->itr;
if(d2 && d2->libdtype==GRAPH_TYPE){
if(it==0)this->expr1->lhs->libdtype=POINT_TYPE;
else if(it==1)this->expr1->lhs->libdtype=EDGE_TYPE;
else if(it<6){
}
else {
int flag=0,xu;
}
}
if(d2 && d2->libdtype==POINT_TYPE){

if(it==2)this->expr1->lhs->libdtype=POINT_TYPE;
else if(it==3)this->expr1->lhs->libdtype=POINT_TYPE;
else if(it==4)this->expr1->lhs->libdtype=POINT_TYPE;
else{ 
if(ERRPRINT)
fprintf(stderr,"error line no:%d-invalid iterator for Point datatype\n",this->lineno);
errflag=1;
}
}
if(d2 && d2->libdtype==COLLECTION_TYPE){
if(it==5){
tree_typedecl *t1=d2->tp1;
if(d2->tp1 && d2->tp1->next==NULL){
if(this->expr1){
this->expr1->lhs->libdtype=-1;;this->expr1->lhs->dtype=d2->tp1->datatype;
}

}
else if( d2 && d2->tp1->next->next==NULL){
if(this->expr1){

 this->expr1->lhs->libdtype=d2->tp1->next->libdatatype; this->expr1->lhs->dtype=d2->tp1->next->datatype;
}
}
else{
if(this->expr1){
 this->expr1->lhs->libdtype=COLLECTION_TYPE;((dir_decl *)this->expr1->lhs)->tp1=d2->tp1->next;
}

}

}
else { 
if(ERRPRINT)
fprintf(stderr,"error line no:%d-invalid iterator for Collection datatype\n",this->lineno);

errflag=1;
}
}
if(d2 && d2->libdtype==SET_TYPE ){
if(it==5){
tree_typedecl *t1=d2->tp1;
if(d2->tp1 && d2->tp1->next==NULL){
this->expr1->lhs->libdtype=-1;;this->expr1->lhs->dtype=d2->tp1->datatype;

}
else if(d2->tp1 && d2->tp1->next && d2->tp1->next->next==NULL){
if(this->expr1){

 this->expr1->lhs->libdtype=d2->tp1->next->libdatatype; this->expr1->lhs->dtype=d2->tp1->next->datatype;
}
}
else{
if(this->expr1){
 this->expr1->lhs->libdtype=SET_TYPE;((dir_decl *)this->expr1->lhs)->tp1=d2->tp1->next;
}
}
}
else{ 
if(ERRPRINT)
fprintf(stderr,"error line no:%d-invalid iterator for Collection datatype\n",this->lineno);
errflag=1;
}
}
if(this->stassign!=NULL) {this->stassign->print1(1);
check_morph_foreach(this);
}
}
if(this->sttype==FOR_STMT){
if(this->f1 && this->f1->stdecl!=NULL){
if(this->f1->stdecl->dirrhs!=NULL) this->f1->stdecl->dirrhs->print();
}
if(this->f1 && this->f1->stassign!=NULL)this->f1->stassign->print1(1);
if(this->f2 && this->f2->stassign!=NULL){this->f2->stassign->print1(0);
}
if(this->f3!=NULL&& this->f3->stassign!=NULL)this->f3->stassign->print1(0);
if(this->stassign!=NULL){
this->stassign->print1(1);
}
}
if(this->sttype==DECL_STMT){
tree_typedecl *x1=this->stdecl->lhs;
if(x1->libdatatype!=-1){
if(ERRPRINT==0){
alloc_extra(this);
realloc_graph(this);
lock_graph(this);
}
if(x1->libdatatype==EDGE_TYPE|| x1->libdatatype==POINT_TYPE){
}
if(x1->libdatatype==SET_TYPE || x1->libdatatype==COLLECTION_TYPE){
tree_typedecl *t1=x1->next;
int cnt=0;
while(t1!=NULL && t1->next!=NULL &&t1->libdatatype==-1 ){ 
cnt++;t1=t1->next;}

if(t1!=NULL && t1->libdatatype!=-1){
}
if(t1!=NULL && t1->datatype!=-1){
}

}
if(x1->libdatatype==G_P_TYPE||x1->libdatatype==E_P_TYPE||x1->libdatatype==P_P_TYPE){
if(x1->d1==NULL){
}
else {
if(x1->libdatatype==E_P_TYPE||x1->libdatatype==P_P_TYPE){
}
else {
}
}
}
if(this->stdecl && this->stdecl->dirrhs!=NULL) {
this->stdecl->dirrhs->print();
}
}
if(x1->libdatatype==-1){
while(x1  && (x1->datatype==-1 || x1->datatype==TYPEDEF_TYPE)){
x1=x1->next;}
if(x1->datatype==STRUCT_TYPE||x1->datatype==ENUM_TYPE||x1->datatype==UNION_TYPE){
if(this->stdecl)this->stdecl->print();
}
else {
if(this->stdecl->dirrhs!=NULL &&this->stdecl->dirrhs->procd!=1){
tree_typedecl *t1=this->stdecl->lhs;
while(t1 && (t1->datatype==TYPEDEF_TYPE||t1->datatype==-1) && t1->libdatatype==-1){t1=t1->next;}
while(t1!=NULL ){if(t1->name!=NULL){
}t1=t1->next;
}
if(this->stdecl->dirrhs!=NULL) {

this->stdecl->dirrhs->print();
}
}
}
if(this->stdecl->dirrhs!=NULL &&this->stdecl->dirrhs->procd==1){
if(this->stdecl->lhs!=NULL){
}
if(this->stdecl->dirrhs!=NULL) {
this->stdecl->dirrhs->print();
}

}
}
}
if(this->next!=NULL)this->next->print();

}
int tree_decl_stmt::print(){
tree_typedecl *x1=this->lhs;
while(x1 && (x1->datatype==-1 || x1->datatype==TYPEDEF_TYPE)){x1=x1->next;}
if(x1->def==true){
;
if(x1->datatype==ENUM_TYPE){ 
dir_decl *head=this->lhs->list->dirrhs;
head->print();
if(this->dirrhs!=NULL)this->dirrhs->print();
return;


}
tree_decl_stmt *head= x1->list;
while(head!=NULL){
if(head->lhs->compoundtype==1){
head->print();head=head->next;continue;}
tree_typedecl *t1=head->lhs;
while(t1!=NULL)if(t1->name!=NULL){
t1=t1->next;
}
if(head->dirrhs!=NULL)head->dirrhs->print();
 head=head->next;}
}

if(this->dirrhs!=NULL) this->dirrhs->print();
}
void tree_id::print(tree_id *id){
int n;
varname[vartop++]=id;
tree_id *temp=id;
while(temp->next!=NULL){varname[vartop++]=temp->next; temp=temp->next;}
tree_id *t1= varname[vartop-1];
if(t1->dirdecl!=NULL)t1->dirdecl->print();
if(t1->rhs!=NULL){
t1->rhs->print(t1->rhs);
}
vartop--;
while(vartop!=0){
t1=varname[vartop-1];
if(t1->dirdecl!=NULL)t1->dirdecl->print();
if(t1->rhs!=NULL){
t1->rhs->print(t1->rhs);
}

vartop--;
}

}
void dir_decl::print(){
int n;
dir_decl *t1= this;
if(t1->ptrcnt>0){
};
if(t1->name!=NULL){
}
if(t1->tp1!=NULL &&  (t1->libdtype==SET_TYPE|| t1->libdtype==COLLECTION_TYPE)){
tree_typedecl *tt1=t1->tp1->next;
if(tt1!=NULL && t1->tp1->d1!=NULL && tt1->libdatatype!=-1){
t1->dev_no=t1->tp1->d1->dev_no;
}
if(tt1!=NULL && tt1->datatype!=-1&& t1->tp1->d1!=NULL){
}
}
if(t1->structexpr!=NULL){
t1->structexpr->print(t1->structexpr);
}
if(t1->assign!=NULL){
t1->assign->print(t1->assign);

}
if(t1->parnames!=NULL){
dir_decl *t2=t1->parnames->dirrhs;
while(t2!=NULL && t2->nextv!=NULL){
t2=t2->nextv;
}

}
if(t1->params!=NULL){
tree_decl_stmt *t2=t1->params;
while(t2!=NULL && t2->next!=NULL){
if(t2->dirrhs!=NULL)t2->dirrhs->print();
t2=t2->next;
}
if(t2!=NULL && t2->dirrhs!=NULL)t2->dirrhs->print();
}
if(t1->next!=NULL)t1->next->print();
if(t1->rhs!=NULL){
}
if(t1->nextv!=NULL){
t1->nextv->print();
}
return;
}
void tree_expr::print(tree_expr *expr){
int n;
if(expr==NULL)return;
if(expr->pflag==100){expr->pflag=-100;return;}
int type=expr->expr_type;
if(type==GET_TYPE){
dir_decl *d1= expr->rhs->nextv;
dir_decl *t1=d1;

if(MORPH_FLAG && t1 &&ERRPRINT==1){

if(t1->gpu==1)fprintf(FP,"void %s_realloc(GGraph &%s){\n %s temp;\n",t1->name,t1->name,t1->extra_name);
if (t1->ppts!=NULL && t1->gpu==1){//printf("%s has extra properties",t1->name);
fprintf(FP,"cudaSetDevice(%d);\n%scudaMemcpy(&temp,%s.extra,sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s\n",t1->dev_no,cpy[0],t1->name,t1->extra_name,cpy[1],errcnt++,cpy[2]);
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE){
fprintf(FP,"if(((temp.n%s-temp.pn%s)*100/temp.n%s>50)|| (temp.n%s*2>temp.maxn%s)){\n",ep->name,ep->name,ep->name,ep->name,ep->name);
fprintf(FP,"void *t1=temp.%s;cudaMalloc((void **)&(temp.%s),sizeof(%s)*temp.n%s*2);\n",ep->name,ep->name,ep->t1->name,ep->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*temp.n%s,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,ep->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"temp.maxn%s=temp.n%s*2;\n",ep->name,ep->name);
fprintf(FP,"cudaFree(t1);\n}\n");
}
if(ep->t1->libdatatype==P_P_TYPE ){
fprintf(FP,"if(((%s.npoints-%s.maxnpoints)*100/%s.npoints>50 )||( %s.npoints*2>%s.maxnpoints)){//\n",t1->name,t1->name,t1->name,t1->name,t1->name,t1->name);
fprintf(FP,"void *t1=temp.%s; cudaMalloc((void **)&(temp.%s),sizeof(%s)*%s.npoints*2);\n",ep->name,ep->name,ep->t1->name,t1->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*%s.npoints,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,t1->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"%s.maxnpoints=%s.npoints*2;\n",t1->name,t1->name);
fprintf(FP,"cudaFree(t1);\n}\n");
}
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"if(((%s.nedges-%s.maxnnedges)*100/%s.nedges>50)|| (%s.npoints*2>%s.maxnpoints)){//\n",t1->name,t1->name,t1->name,t1->name,t1->name,t1->name);
fprintf(FP,"void *t1=temp.%s; cudaMalloc((void **)&(temp.%s),sizeof(%s)*%s.nedges*2);\n",ep->name,ep->name,ep->t1->name,t1->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*%s.nedges,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,t1->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"%s.maxnedges=%s.nedges*2;\n",t1->name,t1->name);
fprintf(FP,"cudaFree(t1);}\n");
}
ep=ep->next;
}
fprintf(FP,"%scudaMemcpy(%s.extra,&temp,sizeof(%s),cudaMemcpyHostToDevice)%s%d%s\n",cpy[0],t1->name,t1->extra_name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"cudaSetDevice(0);\n}\n");

}

lock_graph1(t1);




}
while(d1->nextv!=NULL){
if(expr->lhs!=NULL && expr->lhs->lhs!=NULL && expr->lhs->lhs->extra_name!=NULL){
d1->extra_name=malloc(sizeof(char)*100);
strcpy(d1->extra_name,expr->lhs->lhs->extra_name);
}
}
if(d1!=NULL && expr->lhs!=NULL && expr->lhs->lhs!=NULL &&expr->lhs->lhs->extra_name!=NULL){
d1->extra_name=malloc(sizeof(char)*100);
strcpy(d1->extra_name,expr->lhs->lhs->extra_name);
}
}
if(type==CAST_EXPR){
if(expr->tpname!=NULL){
expr->tpname->print();
}
}
if(type==TYPE_SIZEOF){
if(expr->tpname!=NULL){
expr->tpname->print();
}
}

if(type==TYPE_INIT){
if(expr->tpname!=NULL){
expr->tpname->print();
}
}
if(type==COND_OP){
if(expr->lhs)
expr->lhs->print(expr->lhs);
if(expr->rhs&& expr->rhs->lhs)
expr->rhs->lhs->print(expr->rhs->lhs);
if(expr->rhs && expr->rhs->rhs)
expr->rhs->rhs->print(expr->rhs->rhs);

}
if(type==STRUCTREF){
int xu;
if(expr->lhs!=NULL&& expr->lhs->expr_type==STRUCTREF){
expr->lhs->print(expr->lhs);
if(expr->lhs->libdtype>=0){
}
if(expr->lhs->libdtype==POINT_TYPE ){
if(expr->lhs  && expr->lhs->parent){
struct extra_ppts *ep=expr->lhs->parent->ppts;
while(ep!=NULL){
if(!strcmp(expr->rhs->name,ep->name)&& ep->t1->libdatatype!=P_P_TYPE){
FALCERR=1;
if(ERRPRINT)
printf("error LINE:-%d invalid use of graph extra property %s \n",FALCLINENO,expr->rhs->name);
break;
}
if(!strcmp(expr->rhs->name,ep->name) )break;
ep=ep->next;
}
if(ep==NULL){
FALCERR=1;
//if(ERRPRINT)printf("error LINE:-%d  graph extra property %s does not exist \n",FALCLINENO,expr->rhs->name);

}
if(ep!=NULL && expr->rhs->expr_type!=VAR){
}
}
}
if(expr->lhs->libdtype==EDGE_TYPE ){
if(expr->lhs  && expr->lhs->parent){
struct extra_ppts *ep=expr->lhs->parent->ppts;
while(ep!=NULL){
if(!strcmp(expr->rhs->name,ep->name)&& ep->t1->libdatatype!=E_P_TYPE){
FALCERR=1;
if(ERRPRINT)printf("error LINE:-%d invalid use of graph extra property %s \n",FALCLINENO,expr->rhs->name);
break;
}
if(!strcmp(expr->rhs->name,ep->name) )break;
ep=ep->next;
}
if(ep==NULL){
FALCERR=1;
//if(ERRPRINT)printf("error LINE:-%d  graph extra property %s does not exist \n",FALCLINENO,expr->rhs->name);

}
if(ep!=NULL && expr->rhs->expr_type!=VAR){
}
}
}
if(expr->lhs->libdtype==COLLECTION_TYPE && !strcmp(expr->rhs->name,"size")){
expr->dtype=INT_TYPE;
expr->dtype=expr->lhs->dtype;
expr->rhs->print(expr->rhs);
return;
}
}
else {
dir_decl *d1= (dir_decl *)(expr->lhs->lhs);
if( !strcmp("reset",expr->rhs->name) ){
return;
}
if( !strcmp("src",expr->rhs->name) ){

}
sref=sref+1;
if( d1 && d1->parent!=NULL){expr->parent=d1->parent;}//FEB14
if(d1 && d1->libstable!=NULL){
char *name=expr->rhs->name;
int flag=0,i=0;
enum SIZES size;
if(d1->libdtype==GRAPH_TYPE ){
if(strstr(expr->rhs->name,"read")!=NULL){ d1->read=1;
if(expr->rhs->arglist==NULL || expr->rhs->arglist->rhs==NULL){
if(ERRPRINT)
fprintf(stderr,"error LINE:-%d FILE name not specified as first argument of read function\n",FALCLINENO);
FALCERR=1;
}
}
}
if(d1->libdtype==COLLECTION_TYPE ){
if(expr->rhs && expr->rhs->expr_type==FUNCALL){
for(int i=0;i<6;i++){
if(!strcmp(expr->rhs->name,libdtypefunctions[4][i])&&(strlen(expr->rhs->name)==strlen(libdtypefunctions[4][i]))){
expr->libdtype=-1; expr->rhs->print(expr->rhs);return;}
}
}
}
if(expr->rhs && d1->libdtype==SET_TYPE ){
if(expr->rhs->expr_type==FUNCALL){
for(int i=0;i<4;i++){
if(!strcmp(expr->rhs->name,libdtypefunctions[3][i])&&(strlen(expr->rhs->name)==strlen(libdtypefunctions[3][i]))){
expr->libdtype=-1; expr->rhs->print(expr->rhs);return;}
}
}
}
if(d1->libdtype==G_P_TYPE){
expr->libdtype=G_P_TYPE; expr->rhs->print(expr->rhs);return;}
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
if(d1->libdtype==GRAPH_TYPE && flag==0){
for (std::map<char *,int>::iterator it1=newitr.begin(); it1!=newitr.end(); ++it1){
if(!strcmp(it1->first,name)&& (strlen(name)==(1+strlen(it1->first)))){
flag=1;
}
}
}
if(flag!=1){
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
if(flag==0  && expr->lhs &&expr->lhs->lhs && expr->lhs->lhs->libdtype==GRAPH_TYPE){
struct extra_ppts *ep=expr->lhs->lhs->ppts;
while(ep!=NULL){
if(!strcmp(name,ep->name)&& ep->t1->libdatatype!=G_P_TYPE ){
break;
}
ep=ep->next;
}
}
if(flag==1 /*&& ariflag==1*/){
if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
if(((i==1)||(i==3)) && expr->rhs->expr_type!=ARRREF &&ariflag==1){//1=points,3=edges
errflag=1;
}
else if(expr->rhs->expr_type==ARRREF&&((i==0)||(i==2)|| (i==4)|| (i==5))/*&&ariflag==1*/){
errflag=1;
return;
}
else if((i==1) /*&& expr->rhs->expr_type==ARRREF*/){expr->libdtype=POINT_TYPE; /*printf("point type\n");*/expr->parent=d1;
}
else if((i==3) /*&& expr->rhs->expr_type==ARRREF*/){
expr->libdtype=EDGE_TYPE;
expr->parent=d1;
}
}
if(expr->lhs->lhs->libdtype==POINT_TYPE){
if(((i==4)||(i==5)||(i==6)) && expr->rhs->expr_type!=ARRREF){
if(ERRPRINT)
fprintf(stderr,"invalid use of field %s\n ",libdtypefields[expr->lhs->lhs->libdtype][i]);
errflag=1;}
else if(expr->rhs->expr_type==ARRREF&&((i==0)|| (i==1)||(i==2)||(i==3) )){
if(ERRPRINT)
fprintf(stderr,"invalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);
errflag=1;}
if((i==4) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
if((i==5) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
if((i==6) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
}
if(expr->lhs->lhs->libdtype==EDGE_TYPE){
if(((i==0)||(i==1)) && expr->rhs->expr_type!=VAR){
if(ERRPRINT)
fprintf(stderr,"ixxvalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);
errflag=1;}
if(expr->rhs->expr_type==ARRREF&& i==2){
if(ERRPRINT)
fprintf(stderr,"iyyvalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);
errflag=1;}
if((i==0) && expr->rhs->expr_type==VAR){ /*printf("edGE TYPE %d\n",i);*/expr->libdtype=POINT_TYPE;  }
if((i==1) && expr->rhs->expr_type==VAR){ /*printf("edGE TYPE %d\n",i); */ expr->libdtype=POINT_TYPE; }
}
if(expr->lhs->lhs->libdtype==COLLECTION_TYPE){
expr->libdtype=-1; expr->dtype=INT_TYPE;
}
}

if(flag==2 ){

if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
if(i==5) {expr->libdtype=EDGE_TYPE;expr->parent=expr->lhs->lhs;}
else expr->libdtype=-2;

}
}
if(flag==0){
extra_ppts *ex=NULL;
if(d1->libdtype==GRAPH_TYPE)ex=d1->ppts;
else if(d1->parent!=NULL){ex=d1->parent->ppts;/*fprintf(stderr,"parent=%d %s %s ",d1->parent->arg,d1->name,d1->parent->name);*/}
else if(d1->arg==1)ex=d1->ppts;
while(ex!=NULL&& flag==0){
if(!strcmp(ex->name,expr->rhs->name)&& strlen(ex->name)==strlen(expr->rhs->name)){
if(ex->libdtype!=d1->libdtype){
if(d1->libdtype==GRAPH_TYPE)expr->parent=d1;
else expr->parent=d1->parent;
if(d1->libdtype==GRAPH_TYPE && ex->libdtype!=G_P_TYPE &&ariflag==1){
if(ERRPRINT)
fprintf(stderr,"cannot directly access %s property %s from graph  %s\n",libdtypenames[ex->libdtype] , ex->name,d1->name);
errflag=1;
}else {expr->libdtype=ex->t1->libdatatype;expr->dtype=ex->t1->datatype;flag=1;
if(ex->dt1!=NULL &&ERRPRINT==0)ex->dt1->printcode(0);
break;}//mdofied on fe14AN
}
if(ex->t1->datatype!=-1&& expr->rhs->expr_type==VAR){expr->libdtype=ex->t1->libdatatype;
expr->dtype=ex->t1->datatype;
}
if(ex->t1->libdatatype==-1&& expr->rhs->expr_type==VAR){expr->libdtype=ex->t1->libdatatype;
expr->dtype=ex->t1->datatype;
}
flag=1;
break;
}
ex=ex->next;
}
}
if(flag==0 && d1->arg==0 && !(d1->parent!=NULL && d1->parent->arg==1)&&d1->libdtype>=0){
}
if(flag==0 &&d1->libdtype!=-1){
extra_ppts *ex=NULL;
if(d1->libdtype==GRAPH_TYPE)ex=d1->ppts;
else if(d1->parent!=NULL){ex=d1->parent->ppts;/*fprintf(stderr,"Parent=%d %s ",d1->parent->arg,d1->parent->name);*/}
tree_typedecl *t1=new tree_typedecl();
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
else {expr->lhs->print(expr->lhs); expr->libdtype=expr->lhs->libdtype;/*printf("printing lhs first %d \n",expr->lhs->libdtype);*/if(expr->lhs->libdtype==-2){
if(ERRPRINT)
fprintf(stderr,"invalid structure access\n");
errflag=1;
}}
if(expr->libdtype!=-1 &&expr->libdtype!=-2  && d1->libstable==NULL){

char *name=expr->rhs->name;
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
if(flag==1 /*&&ariflag==1*/){

if(expr->libdtype==GRAPH_TYPE){
if(((i==1)||(i==3)) && expr->rhs->expr_type!=ARRREF&&ariflag==1){
if(ERRPRINT)
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);
errflag=1;}
else if(expr->rhs->expr_type==ARRREF&&((i==0)||(i==2)|| (i==4)|| (i==5))&&ariflag==1){
if(ERRPRINT)
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);
errflag=1;}
else if((i==1) /*&& expr->rhs->expr_type==ARRREF*/){expr->libdtype=POINT_TYPE;}
else if((i==3)/* && expr->rhs->expr_type==ARRREF*/)expr->libdtype=EDGE_TYPE;
else if((i==0)||(i==2)||(i==4)||(i==5)||(i==6))expr->libdtype=-2;
}
else if(expr->libdtype==POINT_TYPE){
expr->libdtype=-1;
if(((i==4)||(i==5)||(i==6)) && expr->rhs->expr_type!=ARRREF&&ariflag==1){
if(ERRPRINT)
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);
errflag=1;}
else if(expr->rhs->expr_type==ARRREF&&((i==0)|| (i==1)||(i==2)||(i==3) )){
if(ERRPRINT)
fprintf(stderr,"invalide used of filed\n");
errflag=1;}
else if((i==4) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
else if((i==5) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
else if((i==6) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
 else if ((i==2)|| (i==3)||(i==7))expr->libdtype=-2;
else {
if(ERRPRINT)
printf("ivalid use of field %s ",libdtypefields[expr->libdtype][i]);
}
}
else if(expr->libdtype==EDGE_TYPE){
if(((i==0)||(i==1)) && expr->rhs->expr_type!=ARRREF&&ariflag==1){
errflag=1;}
else if(expr->rhs->expr_type==ARRREF&& i==2&&ariflag==1){
if(ERRPRINT)
fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);
errflag=1;}
else if((i==0) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
else if((i==1) /*&& expr->rhs->expr_type==ARRREF*/)expr->libdtype=POINT_TYPE;
else if((i==2)||(i==3))expr->libdtype=-2;
}

}

if(expr->lhs && expr->lhs->parent!=NULL){
extra_ppts *ex=expr->lhs->parent->ppts;
while(ex!=NULL&& flag==0){
if(!strcmp(ex->name,expr->rhs->name)&& (strlen(ex->name)==strlen(expr->rhs->name))){
flag=1;
if(ex->t1->datatype!=-1){expr->libdtype=-1;//THIS MAY NOT BE SUFFICIENT FEB 14 FOUND OUT
expr->dtype=ex->t1->datatype;
}
break;
}
ex=ex->next;
}
if(flag==0&& expr->lhs && expr->lhs->libdtype>2)expr->libdtype=-1;
if(flag==0 && expr->lhs&& expr->lhs->parent && expr->lhs->parent->arg==1){
extra_ppts *ex2;
tree_typedecl *t12=new tree_typedecl();
t12->libdatatype=-2;
ex2=new extra_ppts();
ex2->name=malloc(sizeof(char)*100);
strcpy(ex2->name,expr->rhs->name);
ex2->t1=t12;
ex2->next=expr->lhs->parent->ppts;
expr->lhs->parent->ppts=ex2;
}
}
if(flag==0 && d1 && expr->libdtype<=2&& expr->libdtype>=0&&d1->arg==0 &&!(expr->lhs->parent!=NULL&& expr->lhs->parent->arg==1)){
if(ERRPRINT)
fprintf(stderr,"invalid field access %s for %s datatype\n",name,libdtypenames[expr->libdtype]);
errflag=1;}
}
if(d1 && d1->libstable!=NULL){
expr->lhs->print(expr->lhs);
}
if(expr->lhs->dtype!=-1 && expr->lhs->dtype<=9){
if(ERRPRINT)
fprintf(stderr,"struct ref on noncompound datatype\n");
errflag=1;}
expr->rhs->print(expr->rhs);
expr->dtype=expr->rhs->dtype;
sref=sref-1;
return;
}
}
if(type==PTRREF){
expr->lhs->print(expr->lhs);
if(expr->lhs->parent!=NULL)expr->parent=expr->lhs->parent;//FEB14
return;
}
if(expr->expr_type==EXPR){

if(expr->exprlist!=NULL){
statement *t1=expr->exprlist;
t1->print();
}
}
if(expr->expr_type==VAR ){
if(expr->lhs!=NULL && expr->lhs->libdtype>=0&&expr->libdtype<0&&sref==0&&/*ariflag==0&&*/expr->rhs==NULL){expr->libdtype=expr->lhs->libdtype;
expr->parent=expr->lhs->parent;//FEb14
}

if(expr->expr_type==VAR)return;}
 if (expr->nodetype==TREE_INT){
return;
}
 if (expr->nodetype==TREE_BOOL){
return;
}
 if (expr->nodetype==TREE_ENUMCONST){
return;
}
 if (expr->nodetype==TREE_FLOAT){
return;
}
if(expr->expr_type==NULL_ARR)return;
 if (expr->nodetype==TREE_STRING){
return;
}
if(expr->expr_type==FUNCALL){
int unni;
if(expr->name!=NULL){

	int libf=0,ind=-1;
	if(expr->name!=NULL){
	char temp[20];
	for(int i=0;i<11;i++){if(!strcmp(libfunctions[i],expr->name) && strlen(libfunctions[i])==strlen(expr->name)){ind=i;libf=1;break;}
	}
if(ind==1 || ind==0){
assign_stmt *tass0= expr->arglist;
if(tass0==NULL || tass0->next==NULL || tass0->next->next==NULL){
printf("error LINE:-%d not enough number of argument for %s \n",FALCLINENO,libfunctions[ind]);
FALCERR=1;
}
int tt=tass0->rhs->expr_type;
if(tt!=VAR && tt!=STRUCTREF){
printf("error LINE:-%d first argument for %s should be var\n",FALCLINENO,libfunctions[ind]);
FALCERR=1;
}
if(tass0==NULL || tass0->next==NULL || tass0->next->next==NULL)tass0=tass0->next->next;
 tt=tass0->rhs->expr_type;
if(tt!=VAR && tt!=STRUCTREF){
printf("error LINE:-%d last argument for %s should be var\n",FALCLINENO,libfunctions[ind]);
FALCERR=1;
}

}
}

for(std::map<char *,statement *>::iterator it=fnames.begin();it!=fnames.end();it++){
if(!strcmp(it->first,expr->name)){

statement *t1=it->second;
if(!t1){

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
if( tad1 &&   tad1->extra_name!=NULL){
ttd1->extra_name=malloc(sizeof(char)*100);
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
if(ta->rhs && ta->rhs->lhs && ta->rhs->lhs->libdtype==-1 && ta->rhs->lhs->forit!=1){
}
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
while(de2!=NULL && de2->next!=NULL){
de2=de2->next;
}
tree_typedecl *de1=tt->dirrhs->tp1;
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
et1=ta->rhs->lhs->ppts;
if(ta->rhs->lhs->parent!=NULL)et1=ta->rhs->lhs->parent->ppts;
while(et1!=NULL){if(!strcmp(et1->name,et->name)&&(strlen(et1->name)==strlen(et->name)))break;et1=et1->next;}
if(et1==NULL){
if(ERRPRINT)fprintf(stderr,"LINE:-%d invalid property %s used in function %s for argument%d\n",FALCLINENO,et->name,expr->name,cnt);
}
et=et->next;

}
}

}
if(tt->dirrhs!=NULL && ta->rhs!=NULL && ta->rhs->lhs!=NULL && tt->dirrhs->libdtype!=ta->rhs->lhs->libdtype){
if(tt->dirrhs->libdtype==-1 ){
}
else if (ta->rhs->lhs->libdtype==-1 && ta->rhs->lhs->it<0){
}
else{
}
}
tt=tt->next;ta=ta->next;
}
}

}
}
}
 //printf("NULL\n");
assign_stmt *t1=expr->arglist;
while(t1!=NULL&&t1->next){
t1->print1(0);
t1=t1->next;
}
if(t1!=NULL)t1->print1(0);
if(expr->params!=NULL){
tree_decl_stmt *td=expr->params;
while(td->next!=NULL){
if(td->dirrhs)td->dirrhs->print();
td=td->next;}
if(td&& td->dirrhs)td->dirrhs->print();
}
}
if (expr->lhs!=NULL &&expr->expr_type!=ARRREF&&expr->expr_type!=COND_OP&&expr->expr_type!=GET_TYPE){if(expr->expr_type!=FUNCALL){
if(expr->lhs->expr_type==VAR && expr->lhs->lhs && expr->lhs->lhs->libstable!=NULL &&ariflag==1){
}
if(expr->rhs && expr->rhs->expr_type==VAR && expr->rhs->lhs && expr->rhs->lhs->libstable!=NULL&& ariflag==1){
}
this->lhs->print(expr->lhs);
}
}
if(type==AND||type==ADD || type==MUL || type==SUB || type==DIV || type==MOD || type==AND || type==OR || type==XOR|| type==BAND || type==BOR)ariflag=1;

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
assign_stmt *y=expr->earr_list;
while(y!=NULL){
if(y->lhs!=NULL)y->lhs->print(y->lhs);
y->rhs->print(y->rhs);

y=y->next;
}
aref=0;
}
if(type==INITBLOCK){
tree_expr *t1=expr->rhs;
while(t1->prev!=NULL)t1=t1->prev;
if(t1!=NULL) {
class designator *t2= t1->desi;
if(t2!=NULL){

while(t2!=NULL){
if(t2->expr!=NULL){
t2->expr->print(t2->expr);
}
t2=t2->next;
}
}
t1=t1->next;
while(t1!=NULL){
class designator *t2= t1->desi;
if(t2!=NULL){

while(t2!=NULL){
if(t2->expr!=NULL){
t2->expr->print(t2->expr);
}
t2=t2->next;
}
}
if(t1->expr_type==INITBLOCK)t1->print(t1);
t1=t1->next;
}
}
}
if(expr->rhs!=NULL&& type!=INITBLOCK && type!=COND_OP && type!=GET_TYPE){
expr->rhs->print(expr->rhs);
}

}
	iter_stmt::print(){
	if(this->expr1!=NULL)this->expr1->print(this->expr1);
	if(this->body!=NULL){
	//printf("\nbody\n");
	this->body->print();
	}
	}
	void assign_stmt::print1(bool semi){
	int flag=0;
	int xu;
	//printf("print1");
	if(this->asstype==EMPTY_ASSIGN ){
	//if(semi)printf(";");
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
	if(this->rhs->pflag==100){
	this->rhs->pflag=-100;return;
	}
	//printf("RHS");
	//printf("%d xxx",type);
	if(this->rhs!=NULL){
	//printf("printing RHS non gpu\n");
this->rhs->print(this->rhs);//SEP13
}
if(ariflag==0){
if(this->lhs && this->rhs &&this->lhs->libdtype>=0 && this->rhs->libdtype<0){
}
if(this->lhs && this->rhs &&this->lhs->libdtype<0 && this->rhs->libdtype>=0){
}
if(this->lhs && this->rhs &&this->lhs->libdtype>=0 && this->rhs->libdtype>=0  && ((this->rhs->libdtype) != (this->lhs->libdtype))){
}
if(this->lhs && this->lhs->libdtype>=0 ){
dir_decl *d1,*d2;
if(this->lhs){
d1=this->lhs->parent; while(d1&&d1->parent!=NULL) d1=d1->parent;
}
if(this->rhs){d2=this->rhs->lhs; while(d2 &&d2->parent!=NULL) d2=d2->parent;}
}
}



if(ariflag && this->lhs && this->lhs->libdtype>=0 &&this->lhs->libdtype!=this->rhs->libdtype){
if(ERRPRINT)
fprintf(stderr,"MISMATCH between left libtype=%s and right type\n",libdtypenames[this->lhs->libdtype]);
errflag=1;
}


if(this->rhs->pflag==-100)return;
}
