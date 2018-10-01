#include<stdio.h>
#include<stdlib.h>
#include "falctypes.h"
#include <symtabold.h>
#include "mpi.h"
#include "mpigpu.h"
#include "mpicpugpu.h"
//added dup_name!=NULL in line 2450
int morph_size,TOT_GPU_GRAPH;//
 int GLOBAL_ID_FLAG;
int union_type;//0 means ipe and 1 means fpe for point,edge
int Gkernel,kerdec15;//>0 means currently in GPU code
int ftempcnt;// counter for temporary
int errcnt;//counter for temporary
int replicnt;//counter for temporary
int errflag,Temp;//Temp counte for temporary
int pdim_size;//size of point dimennsion. can be <=3
int sref;
int MORPH_FLAG;
int TARGET,DUPVAR;
bool aref;
int EXTRAFLAG,CPUGPU,FUN_EBLOCK_RET;
char gp_string[200],extra_array[200],gpcopy_string[300];
 int extemp;
FILE *FP,*FP1,*FPG,*fp1;//FILE pointers
statement *CURR_CLUST_DEF;
int CURR_CLUST_CNT;
 std::map<char *,int>newitr;//iterator by Graph.addProperty()
 std::map<char *,statement *>fnames;//function definition
 std::map<char *,int>fnamessingle;//function with single statement.
 std::map<char *,statement *>fnamescond;//foreach call of function in char *with conditional
extern class statement *G1,*head,*temp,*temp1,*temp3;
extern int nitr;
char *union_string[2]={"ipe","fpe"};
char *pdim_string[4]={"","","2*","3*"};
char *morphalloc_string[3]={"","*3","*2"};
char *cpy[]={"if(","!=cudaSuccess)printf(\"memcpyerror ","\");"};


alloc_extra_fun(dir_decl *t1,int cpugpu,int gpugpu){
if(t1->gpu==0){
fprintf(FP,"void alloc_extra_%s(HGraph &%s,int flag,int npoints) {\n ",t1->name,t1->name);
t1->extra_fun=(char *)malloc(sizeof(char)*200);
fprintf(FP,"if(flag==0)%s.extra=(%s *)malloc(sizeof(%s)) ;\n",t1->name,t1->extra_name,t1->extra_name);
sprintf(t1->extra_fun,"alloc_extra_%s(%s,host%s,%s.npoints);",t1->name,t1->name,t1->name,t1->name);
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * %s.nedges%s) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,t1->name,morphalloc_string[morph_size]);
}
else if(ep->t1->libdatatype==P_P_TYPE){
fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * npoints%s) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,morphalloc_string[morph_size]);
}
else if(ep->t1->libdatatype==G_P_TYPE){
if(ep->val2!=-1){
fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * ((%s *)(%s.extra))->n%s%s) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,t1->extra_name,t1->name,ep->name,morphalloc_string[morph_size]);
}
else {
fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * ((%s *)(%s.extra))->n%s%s) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,t1->extra_name,t1->name,ep->name,morphalloc_string[morph_size]);

}
tree_typedecl *ept1=ep->t1;


}
else  {
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"cudaMalloc( (void **)&(temp.%s),sizeof(%s)* %s.nedges%s); \n",ep->name,ep->t1->name,t1->name,morphalloc_string[morph_size]);
}
else if(ep->t1->libdatatype==P_P_TYPE ){
fprintf(FP,"cudaMalloc( (void **)&(temp.%s),sizeof(%s)* %s.npoints%s); \n",ep->name,ep->t1->name,t1->name,morphalloc_string[morph_size]);
}
else if(ep->t1->libdatatype==G_P_TYPE){
if(ep->val2!=-1){
fprintf(FP,"cudaMalloc( (void **)&(temp.%s),sizeof(%s)* %d); \n",ep->name,ep->t1->name,ep->val2);
fprintf(fp1,"__device__ int falc%s%s;\n __device__ int pn%s%s;\n __device__ int max%s%s;\n",t1->name,ep->name,t1->name,ep->name,t1->name,ep->name);
fprintf(FP,"cudaMalloc( (void **)&(temp.owner),sizeof(int)* %d); \n",ep->val2);
}
else {

}

}

}

ep=ep->next;}
fprintf(FP,"}\n");
}

if(t1->gpu==1){
fprintf(FP,"void alloc_extra_%s(GGraph &%s, int flag,int npoints) {\n/* cudaSetDevice(%d);*/\n",t1->name,t1->name,t1->dev_no);
t1->extra_fun=(char *)malloc(sizeof(char)*100);
sprintf(t1->extra_fun,"alloc_extra_%s(%s,%sflag,%s.npoints);",t1->name,t1->name,t1->name,t1->name);
if((t1->gpu==1|| cpugpu==1) && t1->ppts!=NULL){
fprintf(FP,"%s temp;\n",t1->extra_name);
fprintf(FP,"if(flag==0)cudaMalloc((void **)&(%s.extra),sizeof(%s));\n cudaMemcpy(&temp,((%s *)(%s.extra)),sizeof(%s),cudaMemcpyDeviceToHost);\n",t1->name,t1->extra_name,t1->extra_name,t1->name,t1->extra_name);
}
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(t1->gpu==1|| cpugpu==1){

if(ep->t1->libdatatype==P_P_TYPE ){
fprintf(FP,"cudaMalloc( (void **)&(temp.%s),sizeof(%s)* npoints%s); \n",ep->name,ep->t1->name,morphalloc_string[morph_size]);
}
if(ep->t1->libdatatype==G_P_TYPE){
if(ep->val2!=-1){
fprintf(FP,"cudaMalloc( (void **)&(temp.%s),sizeof(%s)* %d); \n",ep->name,ep->t1->name,ep->val2);
fprintf(fp1,"__device__ int falc%s%s;\n __device__ int pn%s%s;\n __device__ int max%s%s;\n",t1->name,ep->name,t1->name,ep->name,t1->name,ep->name);
fprintf(FP,"cudaMalloc( (void **)&(temp.owner),sizeof(int)* %d); \n",ep->val2);
}
else {

fprintf(FP,"cudaMalloc(&(temp.%s),sizeof(%s)*temp.n%s);\n" ,ep->name,ep->t1->name,ep->name);
}

}

}

ep=ep->next;}
fprintf(FP,"%scudaMemcpy(%s.extra,&temp,sizeof(%s),cudaMemcpyHostToDevice)%s%d%s",cpy[0],t1->name,t1->extra_name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"/*cudaSetDevice(0);*/\n");
fprintf(FP,"}\n");
}
}
alloc_extra_fun(dir_decl *t1){

if (t1->ppts!=NULL){
if(t1->gpu==0){
fprintf(FP,"void alloc_extra_%s(HGraph &%s,int flag,int npoints) {\n ",t1->name,t1->name);
t1->extra_fun=(char *)malloc(sizeof(char)*200);
sprintf(t1->extra_fun,"alloc_extra_%s(%s,host%s,%s.npoints);",t1->name,t1->name,t1->name,t1->name);
}
if(t1->gpu==1){
fprintf(FP,"void alloc_extra_%s(GGraph &%s, int flag,int npoints) {\n cudaSetDevice(%d);\n",t1->name,t1->name,t1->dev_no);
t1->extra_fun=(char *)malloc(sizeof(char)*100);
sprintf(t1->extra_fun,"alloc_extra_%s(%s,%sflag,%s.npoints);",t1->name,t1->name,t1->name,t1->name);
}
if(t1->gpu==0 && t1->ppts!=NULL){
fprintf(FP,"if(flag==0)%s.extra=(%s *)malloc(sizeof(%s)) ;\n",t1->name,t1->extra_name,t1->extra_name);
}
if(t1->gpu==1 && t1->ppts!=NULL){
fprintf(FP,"%s temp;\n",t1->extra_name);
fprintf(FP,"if(flag==0)cudaMalloc((void **)&(%s.extra),sizeof(%s));\n cudaMemcpy(&temp,((%s *)(%s.extra)),sizeof(%s),cudaMemcpyDeviceToHost);\n",t1->name,t1->extra_name,t1->extra_name,t1->name,t1->extra_name);
}
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(t1->gpu==0){
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * %s.nedges%s) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,t1->name,morphalloc_string[morph_size]);
}
if(ep->t1->libdatatype==P_P_TYPE){
fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * npoints%s) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,morphalloc_string[morph_size]);
}
if(ep->t1->libdatatype==G_P_TYPE){
if(ep->val2!=-1){
fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * ((%s *)(%s.extra))->n%s%s) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,t1->extra_name,t1->name,ep->name,morphalloc_string[morph_size]);
//fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * %d) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,ep->val2);
}
else {
fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * ((%s *)(%s.extra))->n%s%s) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,t1->extra_name,t1->name,ep->name,morphalloc_string[morph_size]);

//fprintf(FP,"((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * %s ) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,ep->var2->name);
}
tree_typedecl *ept1=ep->t1;


}
}
else {
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"cudaMalloc( (void **)&(temp.%s),sizeof(%s)* %s.nedges%s); \n",ep->name,ep->t1->name,t1->name,morphalloc_string[morph_size]);
}
if(ep->t1->libdatatype==P_P_TYPE ){
fprintf(FP,"cudaMalloc( (void **)&(temp.%s),sizeof(%s)* %s.npoints%s); \n",ep->name,ep->t1->name,t1->name,morphalloc_string[morph_size]);
}
if(ep->t1->libdatatype==G_P_TYPE){
if(ep->val2!=-1){
fprintf(FP,"cudaMalloc( (void **)&(temp.%s),sizeof(%s)* %d); \n",ep->name,ep->t1->name,ep->val2);
fprintf(fp1,"__device__ int falc%s%s;\n __device__ int pn%s%s;\n __device__ int max%s%s;\n",t1->name,ep->name,t1->name,ep->name,t1->name,ep->name);
fprintf(FP,"cudaMalloc( (void **)&(temp.owner),sizeof(int)* %d); \n",ep->val2);
}
else {

}

}

}

ep=ep->next;}
if(t1->gpu==1){
fprintf(FP,"%scudaMemcpy(%s.extra,&temp,sizeof(%s),cudaMemcpyHostToDevice)%s%d%s",cpy[0],t1->name,t1->extra_name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"cudaSetDevice(0);\n");
}
fprintf(FP,"}\n");}
}
void check_morph_foreach(statement *t){

if(t->stassign!=NULL && t->stassign->rhs && t->stassign->rhs->expr_type==FUNCALL){
tree_expr *expr=t->stassign->rhs;
for(std::map<char *,statement *>::reverse_iterator it=fnames.rbegin();it!=fnames.rend();it++){
if(!strcmp(it->first,expr->name)){
statement *t1=it->second;//function declaration statement
if(t1->barrier==1)t->barrier=1;
}


}
}

}
char **copynames=NULL;
int copycnt=0;
dir_decl *D2;
int checkdup(char **names,char *name,int cnt){
for(int i=0;i<cnt;i++){
if(!strcmp(names[i],name))return 1;
}
return 0;
}
void setcomm(){
if(copynames==NULL){
copynames=(char **)malloc(sizeof(char *)*10);
for(int i=0;i<10;i++)copynames[i]=(char *)malloc(sizeof(char)*100);
copycnt=0;
}

for(std::map<char *,statement *>::reverse_iterator it2=fnamescond.rbegin();it2!=fnamescond.rend();it2++){ //is part of  foreachstatemet
for(std::map<char *,statement *>::reverse_iterator it1=fnames.rbegin();it1!=fnames.rend();it1++){// function declaration

if(!strcmp(it1->first,it2->first)){
dir_decl *dd=((dir_decl *)(it2->second->expr2->lhs));//GRAPH OBJECT
if(dd!=NULL && D2==NULL && dd->ppts!=NULL)D2=dd;
statement *st=it1->second;//function def
statement *st1=it2->second;//foreach call
fprintf(stderr," kernel %s \n", it1->first);
CURR_CLUST_DEF=st;
CURR_CLUST_DEF->updatecopies=(char **)malloc(sizeof(char *)*10);
CURR_CLUST_DEF->updaterhs=(char **)malloc(sizeof(char *)*10);
st1->updatecopies=(char **)malloc(sizeof(char *)*10);
st1->updaterhs=(char **)malloc(sizeof(char *)*10);
 CURR_CLUST_CNT=0;
st->commcodeGen(FP1);//defined in clustercomm.c- FIND PROPERTTIES AND VARS TO BE communicated
st->updatecopiescnt=CURR_CLUST_CNT;
st1->updatecopiescnt=CURR_CLUST_CNT;
st->sbtop=it2->second->sbtop;
fprintf(stderr,"\n************\n");
for(int i=0;i<st->updatecopiescnt;i++){
fprintf(stderr,"%s %s ",it1->first,CURR_CLUST_DEF->updatecopies[i]);
st1->updatecopies[i]=(char *)malloc(sizeof(char)*100);
strcpy(st1->updatecopies[i],CURR_CLUST_DEF->updatecopies[i]);
if(checkdup(copynames,CURR_CLUST_DEF->updatecopies[i],copycnt)==0){
strcpy(copynames[copycnt],CURR_CLUST_DEF->updatecopies[i]);
copycnt++;
}
if(CURR_CLUST_DEF->updaterhs[i]){
fprintf(stderr," value from property %s ",CURR_CLUST_DEF->updaterhs[i]);

st1->updaterhs[i]=(char *)malloc(sizeof(char)*100);
strcpy(st1->updaterhs[i],CURR_CLUST_DEF->updaterhs[i]);
}
} 
fprintf(stderr,"\n************\n");
}
}
}
if(TARGET==2)gencommbuff(FP1,FP,D2,copynames,copycnt);
if(TARGET==3 || TARGET==4)gencommbuffgpu(FP1,FP,D2,copynames,copycnt);
if(TARGET==5)gencommbuffcpugpu(FP1,FP,D2,copynames,copycnt);
statement *prev=NULL;
/*
for(std::map<char *,statement *>::iterator it1=fnamescond.begin();it1!=fnamescond.end();it1++){// function declaration
std::map<char *,statement *>::iterator it2=fnamescond.begin();
statement *st2;
if(it1!=fnamescond.begin()){
it2=--it1;
st2=it2->second;
it1++;fprintf(stderr," prev fun %s \n",it2->first);
}
statement *st=it1->second;
fprintf(stderr,"\n%s %d %d************\n",it1->first,st->updatecopiescnt,st->sbtop);
for(int i=0;i<st->updatecopiescnt;i++){
fprintf(stderr,"%s %s ",it1->first,st->updatecopies[i]);
if(st->updaterhs[i]){
fprintf(stderr," value from property %s ",st->updaterhs[i]);
if(st!=st2){

for(int k=0;k<st2->updatecopiescnt;k++){
if(!strcmp(st2->updatecopies[k],st->updaterhs[i]))fprintf(stderr,"%s already set by previous function %s\n",st->updatecopies[i],it2->first);
}

}
}
fprintf(stderr,"\n");
} 
}
*/
fprintf(stderr,"\n************\n");
//alloccommbuf1(FP1,FP,d2,COMMPPT,COMMPPT_SIZE);
}
//fnames function definition<char *,statement>
//fnames cond foreach call of function map <char *,statement>
void setparent(){
for(std::map<char *,statement *>::reverse_iterator it1=fnames.rbegin();it1!=fnames.rend();it1++){ //check it is part of  forea
//fprintf(FP1,"// function  %s \n",it1->first);
statement *t0=it1->second;
if(t0 && t0->stdecl && t0->stdecl->dirrhs && t0->stdecl->dirrhs->procd==1){
dir_decl *d1=t0->stdecl->dirrhs;
//d1->printcode(0);
}

//fprintf(FP1,"\n//----------------------\n called functions :- \n");
struct funcall_list *fhead= it1->second->flist;//fhead=name of function,call expression and assignment and next pointer. Declared in tree.h
while(fhead!=NULL){
//fprintf(FP1,"//%s ",fhead->name);
if(GLOBAL_TABLE.findsymbol(fhead->name)){
for(std::map<char *,statement *>::reverse_iterator it2=fnames.rbegin();it2!=fnames.rend();it2++){ //check it is part of  forea

if( it2->first!=NULL && !(strcmp(it2->first,fhead->name))&&it2->second!=NULL){//it->first is also a foreach call 

statement *t1=it2->second;//function declaration statement
if(t1 && t1->stdecl && t1->stdecl->dirrhs && t1->stdecl->dirrhs->procd==1){
//fprintf(FP1,"//procd %s",fhead->name);
fprintf(stderr,"//procd %s",fhead->name);
dir_decl *d1=t1->stdecl->dirrhs;
tree_decl_stmt *decl,*decl1;
decl=d1->params;
//d1->printcode(0);
char arr[200];
for(int i=0;i<200;i++)arr[i]='\0';
//fprintf(FP1,"\ncall ");
tree_expr *expr=fhead->call;
assign_stmt *ass,*ass1;
int cnt=0;
char extra_name[100];
char parent_name[100];
int it=-1;
for(int i=0;i<100;i++)extra_name[i]='\0';
for(int i=0;i<100;i++)parent_name[i]='\0';
if(expr->arglist!=NULL)ass=expr->arglist;
//fprintf(FP1,"(");
dir_decl *itr=NULL;

while(ass&& ass->next&&decl&&decl->next){
if(ass->rhs->lhs && ((dir_decl *)(ass->rhs->lhs))&& ((dir_decl *)(ass->rhs->lhs))->it>=0){
it=((dir_decl *)(ass->rhs->lhs))->it;
//fprintf(FP1,"/*%d */ ",((dir_decl *)(ass->rhs->lhs))->it);
//if(((dir_decl *)(ass->rhs->lhs))->parent)fprintf(FP1,"parent:=%s %s",((dir_decl *)(ass->rhs->lhs))->parent->extra_name,((dir_decl *)(ass->rhs->lhs))->parent->name);
fprintf(stderr, "%s\n",((dir_decl *)(ass->rhs->lhs))->parent->name);
if( ((dir_decl *)(ass->rhs->lhs))->parent->parent)fprintf(stderr, "%s\n",((dir_decl *)(ass->rhs->lhs))->parent->parent->name);
if( ((dir_decl *)(ass->rhs->lhs))->parent->tp1 && ((dir_decl *)(ass->rhs->lhs))->parent->tp1->d1){
fprintf(stderr, "%s\n",((dir_decl *)(ass->rhs->lhs))->parent->tp1->d1->name);
strcpy(extra_name,((dir_decl *)(ass->rhs->lhs))->parent->tp1->d1->extra_name);
strcpy(parent_name,((dir_decl *)(ass->rhs->lhs))->parent->tp1->d1->name);
t1->expr2=new tree_expr();
t1->expr2->name=malloc(sizeof(char)*100);
strcpy(t1->expr2->name,((dir_decl *)(ass->rhs->lhs))->parent->name);
}
else {
strcpy(extra_name,((dir_decl *)(ass->rhs->lhs))->parent->extra_name);
strcpy(parent_name,((dir_decl *)(ass->rhs->lhs))->parent->name);
}
itr=((dir_decl *)(decl->dirrhs));
itr->it=((dir_decl *)(ass->rhs->lhs))->it;
t1->itr=((dir_decl *)(ass->rhs->lhs))->it;
for(std::map<char *,statement *>::reverse_iterator it4=fnamescond.rbegin();it4!=fnamescond.rend();it4++) if((it4->first!=NULL)&&!strcmp(it4->first,fhead->name)){
t1->expr4=it4->second->expr4;
break;
}
//t1->expr4=it2->second->expr4;
//if(t1->expr4==NULL)fprintf(FP1,"EXPR 4 NULL %s ",fhead->name);
cnt++;
//break;

//fprintf(FP1,"%s %s,",ass->rhs->name,decl->dirrhs->name);
ass1=ass;
decl1=decl;
while(ass1 && decl1 && strcmp(ass1->rhs->name,parent_name)){
cnt++;
ass1=ass1->next;
decl1=decl1->next;
}
if(itr){
//fprintf(FP1,"%s %s)\n",ass1->rhs->name,decl1->dirrhs->name);
ass=expr->arglist;
for(int i=0;i<cnt;i++){
//decl=decl->next;
}
//fprintf(FP1,"//set parent of %s to %s \n",itr->name,decl1->dirrhs->name); 
fprintf(stderr,"set parent of %s to %s \n",itr->name,decl1->dirrhs->name); 
itr->parent=decl1->dirrhs;
itr->parent->ppts=((dir_decl *)(ass->rhs->lhs))->parent->ppts;
decl1->dirrhs->extra_name=malloc(sizeof(char)*100);
for(int i=0;i<100;i++)decl1->dirrhs->extra_name[i]='\0';
strcpy(decl1->dirrhs->extra_name,extra_name);
itr->it=it;
itr->forit=1;
itr=NULL;
cnt=0;
for(int i=0;i<100;i++)extra_name[i]='\0';
for(int i=0;i<100;i++)parent_name[i]='\0';
}
}
ass=ass->next;
decl=decl->next;
}
}
}
}
}
fhead=fhead->next;
//fprintf(FP1,"\n");
}

}
return;

}

lock_graph(statement *st){

tree_typedecl *x1=st->stdecl->lhs;
if(x1->libdatatype==GRAPH_TYPE){
dir_decl *t1=st->stdecl->dirrhs;
if(t1->gpu==1){

struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE){
fprintf(FP,"__global__ void reset%s%s(GGraph %s){\n",t1->name,ep->name,t1->name);
fprintf(FP,"int idx=blockDim.x*blockIdx.x+threadIdx.x;\n if(idx < ( ( %s *)(%s.extra))->n%s)\n",t1->extra_name,t1->name,ep->name);
fprintf(FP, "( ( %s *)(%s.extra))->owner[idx]=99999999;\n }\n",t1->extra_name,t1->name);
}
ep=ep->next;
}
}
}
}
lock_graph1(dir_decl *t1 ){
//printf("ALLOC EXTR CALLED \n");
//FILE *fp1=fopen("global.h","a+");
if(t1->gpu==1){

struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE){
fprintf(FP,"__global__ void reset%s%s(GGraph %s){\n",t1->name,ep->name,t1->name);
fprintf(FP,"int idx=blockDim.x*blockIdx.x+threadIdx.x;\n if(idx < ( ( %s *)(%s.extra))->n%s)\n",t1->extra_name,t1->name,ep->name);
fprintf(FP, "( ( %s *)(%s.extra))->owner[idx]=99999999;\n }\n",t1->extra_name,t1->name);
}
ep=ep->next;
}
}

}
realloc_graph1(dir_decl *t1 ){
//printf("ALLOC EXTR CALLED \n");
//FILE *fp1=fopen("global.h","a+");
fp1=FPG;
if(t1->gpu==0)fprintf(FP,"void %s_realloc(HGraph &%s){\n %s temp;\n",t1->name,t1->name,t1->extra_name);
if(t1->gpu==1){
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE){

 fprintf(stderr,"symtabFP==NULL\n");
fprintf(FP,"__device__ int falcadd%s%s( ){\n int t1=atomicAdd(&falc%s%s,1);\n return t1;\n}\n",t1->name,ep->name,t1->name,ep->name);
fprintf(fp1,"__device__ int falc%s%s;\n",t1->name,ep->name);

}
ep=ep->next;
}
fprintf(FP,"void %s_realloc(GGraph &%s){\n cudaSetDevice(%d);\n%s temp;\n",t1->name,t1->name,t1->dev_no,t1->extra_name);

}
if (t1->ppts!=NULL && t1->gpu==0){//printf("%s has extra properties",t1->name);
fprintf(FP,"temp=*((%s *)(%s.extra));\n",t1->extra_name,t1->name);
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE){
fprintf(FP,"if(((temp.n%s-temp.pn%s)*100/temp.n%s>50) || (temp->n%s*2>temp.maxn%s)){\n",ep->name,ep->name,ep->name,ep->name,ep->name);
fprintf(FP,"void *t1=temp.%s;\ntemp.%s=(%s *)malloc(sizeof(%s)*temp.n%s*2);\n",ep->name,ep->name,ep->t1->name,ep->t1->name,ep->name);
fprintf(FP,"memcpy(temp.%s,t1,sizeof(%s)*temp.n%s);\nfree(t1);\n",ep->name,t1->extra_name,ep->name);

}
if(ep->t1->libdatatype==P_P_TYPE ){
fprintf(FP,"if(((%s.npoints-%s.pnpoints)*100/%s.npoints>50 )||(%s.npoints*2>%s.maxnpoints)){//\n",t1->name,t1->name,t1->name,t1->name,t1->name);
fprintf(FP,"void *t1=temp.%s;\ntemp.%s=(%s *)malloc(sizeof(%s)*%s.npoints*2);\n",ep->name,t1->extra_name,t1->extra_name,t1->name);
fprintf(FP,"memcpy(temp.%s,t1,sizeof(%s)*%s.npoints);\nfree(t1);\n%s.maxnpoints=%s.npoints*2;\n}\n",ep->name,t1->extra_name,t1->name,t1->name,t1->name);
}
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"if(((%s.nedges-%s.pnedges)*100/%s.nedges>50)|| (%s.nedges*2>%s.maxnedges)){//\n",t1->name,t1->name,t1->name,t1->name,t1->name);
fprintf(FP,"void *t1=temp.%s;\ntemp.%s=(%s *)malloc(sizeof(%s)*%s.nedges*2);\n",ep->name,t1->extra_name,t1->extra_name,t1->name);
fprintf(FP,"memcpy(temp.%s,t1,sizeof(%s)*%s.nedges);\nfree(t1);%s.maxnedges=%s.nedges*2;\n}\n",ep->name,ep->t1->name,t1->name,t1->name);
}
ep=ep->next;


}
}
if (t1->ppts!=NULL && t1->gpu==1){//printf("%s has extra properties",t1->name);
fprintf(FP,"cudaSetDevice(%d);\n%scudaMemcpy(&temp,%s.extra,sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s\n",t1->dev_no,cpy[0],t1->name,t1->extra_name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"int temppnpoints;\ncudaMemcpy(&temppnpoints,%s.pnpoints,sizeof(int),cudaMemcpyDeviceToHost);\n",t1->name);
fprintf(FP,"int temppnedges;\ncudaMemcpy(&temppnedges,%s.pnpoints,sizeof(int),cudaMemcpyDeviceToHost);\n",t1->name);
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE){
fprintf(FP,"if(((temp.n%s-temp.pn%s)*100/temp.n%s>50 )|| (temp.n%s*2 >temp.maxn%s)){\n",ep->name,ep->name,ep->name,ep->name,ep->name);
fprintf(FP,"void *t1=temp.%s;cudaMalloc((void **)&(temp.%s),sizeof(%s)*temp.n%s*2);\n",ep->name,ep->name,ep->t1->name,ep->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*temp.n%s,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,ep->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"temp.maxn%s=temp.n%s*2;\n",ep->name,ep->name);
fprintf(FP,"cudaFree(t1);\n");
fprintf(FP,"t1=temp.owner;\ncudaMalloc((void **)&(temp.owner),sizeof(int)*temp.n%s*2);\ncudaFree(t1);\n}\n",ep->name);


}
if(ep->t1->libdatatype==P_P_TYPE ){
fprintf(FP,"if(((%s.npoints-temppnpoints)*100/%s.npoints>50)|| (%s.npoints*2>%s.maxnpoints)){//\n",t1->name,t1->name,t1->name,t1->name,t1->name);
fprintf(FP,"void *t1=temp.%s; cudaMalloc((void **)&(temp.%s),sizeof(%s)*%s.npoints*2);\n",ep->name,ep->name,ep->t1->name,t1->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*%s.npoints,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,t1->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"%s.maxnpoints=%s.npoints*2;\n",t1->name,t1->name);
fprintf(FP,"cudaFree(t1);\n}\n");
}
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"if(((%s.nedges-temppnedges)*100/%s.nedges>50)||( %s.nedges*2>%s.maxnedges)){//\n",t1->name,t1->name,t1->name,t1->name,t1->name);
fprintf(FP,"void *t1=temp.%s; cudaMalloc((void **)&(temp.%s),sizeof(%s)*%s.nedges*2);\n",ep->name,ep->name,ep->t1->name,t1->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*%s.nedges,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,t1->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"%s.maxnedges=%s.nedges*2;\n",t1->name,t1->name);
fprintf(FP,"cudaFree(t1);\n}\n");
}
ep=ep->next;
}
fprintf(FP,"\n %scudaMemcpy(%s.extra,&temp,sizeof(%s),cudaMemcpyHostToDevice)%s%d%s\n",cpy[0],t1->name,t1->extra_name,cpy[1],errcnt++,cpy[2]);
}

fprintf(FP,"\ncudaSetDevice(0);\n}\n");
//fclose(fp1);
}
realloc_graph(statement *st ){
//printf("ALLOC EXTR CALLED \n");
//FILE *fp1=fopen("global.h","a+");
fp1=FPG;
int cnt=0;
tree_typedecl *x1=st->stdecl->lhs;
if(x1->libdatatype==GRAPH_TYPE){
dir_decl *t1=st->stdecl->dirrhs;
if(t1->gpu==0)fprintf(FP,"void %s_realloc(HGraph &%s){\n %s temp;\n",t1->name,t1->name,t1->extra_name);
if(t1->gpu==1)fprintf(FP,"void %s_realloc(GGraph &%s){\n %s temp;\n",t1->name,t1->name,t1->extra_name);
while(t1!=NULL){
if (t1->ppts!=NULL && t1->gpu==0){//printf("%s has extra properties",t1->name);
fprintf(FP,"temp=*((%s *)(%s.extra));\n",t1->extra_name,t1->name);
struct extra_ppts *ep=t1->ppts;
fprintf(FP,"int temppnpoints;\ntemppnpoints=%s.pnpoints;\n",t1->name);
fprintf(FP,"int temppnedges;\ntemppnedges=%s.pnpoints;\n",t1->name);
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE){
fprintf(FP,"if(temp.n%s*2>temp.maxn%s){\n",ep->name,ep->name);
fprintf(FP,"void *t1=temp.%s;\ntemp.%s=(%s *)malloc(sizeof(%s)*temp.n%s*2);\n",ep->name,ep->name,ep->t1->name,ep->t1->name,ep->name);
fprintf(FP,"memcpy(temp.%s,t1,sizeof(%s)*temp.n%s);\nfree(t1);temp.maxn%s=temp.n%s*2;\n}\n",ep->name,ep->t1->name,ep->name,ep->name,ep->name);
}
if(ep->t1->libdatatype==P_P_TYPE ){
fprintf(FP,"if(%s.pnpoints*2> %s.maxnpoints){\n",t1->name,t1->name);
fprintf(FP,"void *t1=temp.%s;\ntemp.%s=(%s *)malloc(sizeof(%s)*%s.npoints*2);\n",ep->name,ep->name,ep->t1->name,ep->t1->name,t1->name);
fprintf(FP,"memcpy(temp.%s,t1,sizeof(%s)*%s.npoints);\nfree(t1);\n%s.maxnpoints=%s.npoints*2;\n}\n",ep->name,ep->t1->name,t1->name,t1->name,t1->name);
}
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"if(%s.pnedges*2> %s.maxnedges){\n",t1->name,t1->name);
fprintf(FP,"void *t1=temp.%s;\ntemp.%s=(%s *)malloc(sizeof(%s)*%s.nedges*2);\n",ep->name,ep->name,ep->t1->name,ep->t1->name,t1->name);
fprintf(FP,"memcpy(temp.%s,t1,sizeof(%s)*%s.nedges);\nfree(t1);\n%s.maxnedges=%s.nedges*2;\n}\n",ep->name,ep->t1->name,t1->name,t1->name,t1->name);
}
ep=ep->next;

}
}
if (t1->ppts!=NULL && t1->gpu==1){//printf("%s has extra properties",t1->name);
fprintf(FP,"cudaSetDevice(%d);\n%scudaMemcpy(&temp,%s.extra,sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s\n",t1->dev_no,cpy[0],t1->name,t1->extra_name,cpy[1],errcnt++,cpy[2]);
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE){
fprintf(FP,"if(((temp.n%s-temp.pn%s)*100/temp.n%s>50)|| (temp.n%s*2>temp.maxn%s)){\n",ep->name,ep->name,ep->name,ep->name,ep->name);
fprintf(FP,"%s *t1=temp.%s;cudaMalloc((void **)&(temp.%s),sizeof(%s)*temp.n%s*2);\n",ep->t1->name,ep->name,ep->name,ep->t1->name,ep->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*temp.n%s,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,ep->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"temp.maxn%s=temp.n%s*2;\n",ep->name,ep->name);
fprintf(FP,"cudaFree(t1);\n}\n");
}
if(ep->t1->libdatatype==P_P_TYPE ){
fprintf(FP,"if(((%s.npoints-%s.temppnpoints)*100/%s.npoints>50 )||( %s.npoints*2>%s.maxnpoints)){//\n",t1->name,t1->name,t1->name,t1->name,t1->name);
fprintf(FP,"%s *t1=temp.%s; cudaMalloc((void **)&(temp.%s),sizeof(%s)*%s.npoints*2);\n",ep->t1->name, ep->name,ep->name,ep->t1->name,t1->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*%s.npoints,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,t1->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"%s.maxnpoints=%s.npoints*2;\n",t1->name,t1->name);
fprintf(FP,"cudaFree(t1);\n}\n");
}
if(ep->t1->libdatatype==E_P_TYPE ){
fprintf(FP,"if(((%s.nedges-temppnedges)*100/%s.nedges>50)|| (%s.npoints*2>%s.maxnpoints)){//\n",t1->name,t1->name,t1->name,t1->name,t1->name);
fprintf(FP,"%s *t1=temp.%s; cudaMalloc((void **)&(temp.%s),sizeof(%s)*%s.nedges*2);\n",ep->t1->name,ep->name,ep->name,ep->name,ep->t1->name,t1->name);
fprintf(FP,"%scudaMemcpy(temp.%s,t1,sizeof(%s)*%s.nedges,cudaMemcpyDeviceToDevice)%s%d%s\n",cpy[0],ep->name,ep->t1->name,t1->name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"%s.maxnedges=%s.nedges*2;\n",t1->name,t1->name);
fprintf(FP,"cudaFree(t1);}\n");
//fprintf(FP,"t1=temp.owner;\ncudaMalloc((void **)&(temp.owner),sizeof(int)*temp.n%s*2);\ncudafree(t1);\n}\n",ep->name);
}
ep=ep->next;
}
fprintf(FP,"%scudaMemcpy(%s.extra,&temp,sizeof(%s),cudaMemcpyHostToDevice)%s%d%s\n",cpy[0],t1->name,t1->extra_name,cpy[1],errcnt++,cpy[2]);
fprintf(FP,"cudaSetDevice(0);\n");

}

fprintf(FP,"\n}\n");

t1=t1->next;
}
}

//fclose(fp1);
}


alloc_extra(statement *st ){
//printf("ALLOC EXTR CALLED \n");
int gpf=0;
//FILE *fp1=fopen("global.h","a+");
fp1=FPG;
tree_typedecl *x1=st->stdecl->lhs;

if(x1->libdatatype==GRAPH_TYPE){
dir_decl *t1=st->stdecl->dirrhs;
t1=st->stdecl->dirrhs;
while(t1!=NULL){if (t1->ppts!=NULL){//printf("%s has extra properties",t1->name);
printf("decicded to give structure property name struct_%s\n ",t1->name);
t1->extra_name=(char *)malloc(sizeof(char)*100);
 sprintf(t1->extra_name,"struct struct_%s ",t1->name);
fprintf(FP,"struct struct_%s { \n",t1->name);
struct extra_ppts *ep=t1->ppts;
while(ep!=NULL){
if(ep->t1->libdatatype==G_P_TYPE &&ep->t1->exgpu==0){
gpf=1;
fprintf(FP,"int n%s;\nint maxn%s;\n int pn%s;\n",ep->name,ep->name,ep->name);
if(t1->gpu==1)fprintf(fp1,"__device__ int falc%s%s;\n__device__ int devmaxn%s%s;\n__device__ int pn%s%s;\n",t1->name,ep->name,t1->name,ep->name,t1->name,ep->name);
ep->t1->exgpu=1;
}
if(ep->t1->libdatatype!=-1)
{fprintf(FP,"%s  *%s ;//has to given size of property type\n",ep->t1->name,ep->name);}
else fprintf(FP,"%s  %s ;\n",ep->t1->name,ep->name);
ep=ep->next;
}
ep=t1->ppts->child;
while(ep!=NULL){
if(ep->t1->libdatatype!=-1)
{fprintf(FP,"%s  *%s ;//has to given size of property type\n",ep->t1->name,ep->name);}
else fprintf(FP,"%s  %s ;\n",ep->t1->name,ep->name);
ep=ep->next;
}
if(gpf==1)fprintf(FP,"int *owner;\n");
fprintf(FP,"};\n");}
 t1=t1->nextv;}
t1=st->stdecl->dirrhs;
while(t1!=NULL){if (t1->ppts!=NULL){
if (t1->ppts!=NULL && t1->gpu==0){
struct extra_ppts *epp=t1->ppts;
int flag=0,exread=0;
while(epp!=NULL){
if(epp->t1->libdatatype==G_P_TYPE){
if(flag==0){fprintf(FP,"void read_%s_pptysize(HGraph &%s){\n",t1->name,t1->name);flag=1;exread=1;}
fprintf(FP,"printf(\"enter size of propery %s of %s \");\n",epp->name,t1->name);
fprintf(FP,"scanf(\"%%d\",&(((%s *)(%s.extra))->n%s));\n",t1->extra_name,t1->name,epp->name);
t1->extra_readfun=(char *)malloc(sizeof(char)*100);
sprintf(t1->extra_readfun,"read_%s_pptysize(%s);",t1->name,t1->name);
}
epp=epp->next;



}
if(flag==1)fprintf(FP,"}\n");
}
}
if (t1->ppts!=NULL){
//if(t1->cpugpu==true|| t1->gpugpu>1){
//t1->gpu=1;
//alloc_extra_fun(t1,1,1);
//t1->gpu=0;
//alloc_extra_fun(t1);
//}
//else
 alloc_extra_fun(t1);
}
if(t1->read==0 && t1->gpu==0){
fprintf(FP,"void read_and_allocate_%s(HGraph  &%s ){\n",t1->name,t1->name);
fprintf(FP,"printf(\"enter number of points and edges\");\n scanf(\"%%d%%d\",&(%s.npoints),&(%s.nedges));\n ",t1->name,t1->name);
fprintf(FP,"%s.points=(union float_int *)malloc(sizeof(union float_int)*%s.npoints);\n",t1->name,t1->name);
fprintf(FP,"%s.edges=(union float_int *)malloc(sizeof(union float_int)*%s.nedges*3);\n",t1->name,t1->name);
if(t1->extra_readfun!=NULL)fprintf(FP,"%s\n ",t1->extra_readfun);
if(!(t1->cpugpu || t1->gpugpu))fprintf(FP,"alloc_extra_%s(%s,0,%s.npoints);\n",t1->name,t1->name,t1->name);
t1->extra_readfun1=malloc(sizeof(char)*100);
sprintf(t1->extra_readfun1,"read_and_allocate_%s(%s);\n",t1->name,t1->name);
fprintf(FP,"}\n"); 
}

 t1=t1->nextv;}
}
//fclose(fp1);
}








void proc_id::addsymbols(globsymtable *symt){

tree_decl_stmt *temp=this->params;
if(temp==NULL || temp->dirrhs==NULL)return;
printf("NOT NULL\n");
tree_decl_stmt *temp1=temp->next;
if(temp->dirrhs!=NULL)printf("%s ",temp->dirrhs->name);
if(temp1!=NULL&& temp->dirrhs!=NULL){
while(temp1!=NULL){
while(temp1!=NULL){
printf("COMPARE\n");
if(temp1->dirrhs==NULL)break;
printf("%s ",temp->dirrhs->name);
//if(temp1->dirrhs->name==NULL)printf("rhs1NULL");
//if(temp->dirrhs->name==NULL)printf("rhsNULL");
printf("%s ",temp->dirrhs->name);
 if(!strcmp(temp1->dirrhs->name,temp->dirrhs->name)){
      printf("two parameters have same name\n");
     return ;
  }
  temp1=temp1->next;
 }
 temp=temp->next; temp1=temp->next;
}
}
temp=this->params;
while(temp!=NULL){
printf("looping params\n");
if(temp->dirrhs==NULL){temp=temp->next;continue;}
if(temp->lhs!=NULL && temp->lhs->libdatatype==SET_TYPE){
//tree_typedecl *t1=temp->lhs;
tree_typedecl *t1=temp->dirrhs->tp1;
while(t1!=NULL && t1->next!=NULL)t1=t1->next;
if(t1!=NULL)t1->d1=symt->findsymbol(t1->name);
if(t1!=NULL && t1->d1!=NULL)printf("ADDED FOR SET PARAM %s",t1->d1->name);
}
//if(symt->findsymbol(temp->dirrhs)!=NULL && declared==0)fprintf(stderr,"variable %s already used ",temp->dirrhs->name);
if(temp->lhs!=NULL && temp->dirrhs!=NULL)symt->addsymbol(temp->dirrhs,temp->lhs);
temp=temp->next;

}
 declared=0;
}
int tree_decl_stmt::printcode(int f){
FILE *temp;
//fprintf(FP1,"/*DECL*/");
tree_typedecl *x1=this->lhs;
//if(this->dirrhs &&this->dirrhs->stable==GLOBAL_TABLE.sym)fprintf(FP1,"GLOBAL VARIABLE\n");
if(this->lhs->list && this->lhs->list->dirrhs && this->lhs->list->dirrhs->stable ==GLOBAL_TABLE.sym){
temp=FP1;
//FP1=fopen("global.h","a+");
FP1=FPG;
}
else temp=FP1;
if(x1->def==true){
while(x1 && (x1->datatype==-1 || x1->datatype==TYPEDEF_TYPE)){x1=x1->next;}
if(x1!=NULL && x1->name!=NULL)fprintf(FP1," %s ",x1->name);
//if(x1!=NULL && x1->vname!=NULL)fprintf(FP1," %s ",x1->vname);
fprintf(FP1,"{\n");
if(x1->datatype==ENUM_TYPE){ 
dir_decl *head=this->lhs->list->dirrhs;
head->printcode(1);
fprintf(FP1,"};\n");
dir_decl *t1=this->dirrhs;

int gf=0,hf=0;
t1=this->dirrhs;
while(t1!=NULL){if (t1->gpu==1)gf=1;if(t1->gpu==0)hf=1; if(hf&&gf) break; t1=t1->nextv;}
if(gf==1){ if(Gkernel==0)fprintf(FP1,"__device__ ");else fprintf(FP1," ");
int cnt=0;
tree_typedecl *tt1=this->lhs;
while(tt1 && (tt1->datatype==TYPEDEF_TYPE||tt1->datatype==-1) && tt1->libdatatype==-1){tt1=tt1->next;}
while(tt1!=NULL ){if(tt1->name!=NULL){fprintf(FP1,"%s ", tt1->name);}tt1=tt1->next;}
t1=this->dirrhs;
while(t1!=NULL){if (t1->gpu==1){if(cnt>0)fprintf(FP1,","); t1->printcode(1);cnt++; }t1=t1->nextv;}
fprintf(FP1,";\n");
}
if(hf==1){ fprintf(FP1," ");
int cnt=0;
tree_typedecl *tt1=this->lhs;
while(tt1 && (tt1->datatype==TYPEDEF_TYPE||tt1->datatype==-1) && tt1->libdatatype==-1){tt1=tt1->next;}
while(tt1!=NULL ){if(tt1->name!=NULL){fprintf(FP1,"%s ", tt1->name);}tt1=tt1->next;}
t1=this->dirrhs;
while(t1!=NULL){if (t1->gpu==0){if(cnt>0)fprintf(FP1,","); t1->printcode(0);cnt++; }t1=t1->nextv;}
fprintf(FP1,";\n");
}
if(this->dirrhs!=NULL)this->dirrhs->printcode(1);
fprintf(FP1,";\n");
while(head!=NULL){ printf("%s,\n",head->name);if(head->idrhs!=NULL)head=head->next;}
return;

}
tree_decl_stmt *head= x1->list;

while(head!=NULL){if(head->lhs->compoundtype==1){if(head->lhs->datatype==STRUCTREC_TYPE&& ( (strcmp(this->lhs->vname,head->lhs->vname)!=0)||(strlen(this->lhs->vname)!=strlen(head->lhs->vname))))printf("improper struct rec use\n");  head->printcode(0);head=head->next;printf("%s\n ",head->dirrhs->name);continue;}
tree_typedecl *t1=head->lhs;
if(t1 && t1->datatype==STRUCTREC_TYPE&& strcmp(x1->name,t1->name)!=0)printf("improper struct rec use\n");
while(t1!=NULL)if(t1->name!=NULL){
if( t1->libdatatype==GRAPH_TYPE )fprintf(FP1,"GGraph ");
else if( (t1->libdatatype==POINT_TYPE||t1->libdatatype==EDGE_TYPE ) && (t1->parent!=NULL) )fprintf(FP1,"int ");
else if( (t1->libdatatype==POINT_TYPE )/* && (t1->parent==NULL)*/ )fprintf(FP1,"int ");
else if( (t1->libdatatype==EDGE_TYPE ) /*&& (t1->parent==NULL)*/ )fprintf(FP1,"int  ");
else if( (t1->datatype==STRUCTREC_TYPE ) /*&& (t1->parent==NULL)*/ )fprintf(FP1,"int  ");
else if( (t1->libdatatype==G_P_TYPE && t1->parent!=NULL ) /*&& (t1->parent==NULL)*/ )fprintf(FP1,"int  ");
else fprintf(FP1,"%s ", t1->name);t1=t1->next;}
if(head->dirrhs!=NULL){
head->dirrhs->printcode(1);
}


fprintf(FP1,";\n");
printf("%s\n ",head->dirrhs->name);
 head=head->next;}
fprintf(FP1,"}");
fprintf(FP1,";\n");
printf("RETURN");
FP1=temp;
//return;
}
int gf=0,hf=0;
printf("retur\n");
dir_decl *t1=this->dirrhs;
while(t1!=NULL){if (t1->gpu==1 && t1->parent==NULL)gf=1;if(t1->gpu==0&& t1->parent==NULL)hf=1; if(hf&&gf) break; t1=t1->nextv;}
if(gf==1){ if(Gkernel==0)fprintf(FP1,"__device__ ");else fprintf(FP1,"");//fprintf(FP1,"__device__ ");
int cnt=0;
tree_typedecl *tt1=this->lhs;
while(tt1 && (tt1->datatype==TYPEDEF_TYPE||tt1->datatype==-1) && tt1->libdatatype==-1){tt1=tt1->next;}
while(tt1!=NULL ){
if(tt1->datatype==STRUCTREC_TYPE){fprintf(FP1, "int ");
break;
}
if(tt1->name!=NULL){fprintf(FP1,"%s ", tt1->name);}tt1=tt1->next;}
t1=this->dirrhs;
//if(tt1->datatype==STRUCTREC_TYPE){ if(t1)t1->printcode(1);}
//else {
while(t1!=NULL){if (t1->gpu==1){if(cnt>0)fprintf(FP1,","); fprintf(FP1," %s ",t1->name);
if(t1->ctype==CARR_TYPE){fprintf(FP1,"[");
t1=t1->next;
if(t1 &&t1->assign!=NULL){
 t1->assign->printcode(t1->assign);
}fprintf(FP1,"]");}
cnt++; }
t1=t1->nextv;}
//}
fprintf(FP1,";\n");
}
if(hf==1){ fprintf(FP1," ");
int cnt=0;
tree_typedecl *tt1=this->lhs;
while(tt1 && (tt1->datatype==TYPEDEF_TYPE||tt1->datatype==-1) && tt1->libdatatype==-1){tt1=tt1->next;}
while(tt1!=NULL ){
if(tt1->datatype==STRUCTREC_TYPE){fprintf(FP1, "int ");
break;
}
if(tt1->name!=NULL){fprintf(FP1," %s  ", tt1->name);}tt1=tt1->next;
}
t1=this->dirrhs;
//if(tt1->datatype==STRUCTREC_TYPE){ if(t1)t1->printcode(1);}
//else {
while(t1!=NULL){if (t1->gpu==0){if(cnt>0)fprintf(FP1,","); fprintf(FP1," %s ",t1->name);
if(t1->ctype==CARR_TYPE){fprintf(FP1,"["); 
t1=t1->next;
if(t1 && t1->assign!=NULL){
t1->assign->printcode(t1->assign);}fprintf(FP1,"]");} 
cnt++; }
if(t1->parent!=NULL && f==1 && t1->libdtype==-1){
if(t1->parent && t1->parent->extra_name && t1->libdtype==-1){
t1->extra_name=malloc(sizeof(char)*100);
strcpy(t1->extra_name,t1->parent->extra_name);
}

if(t1->name){
t1->extra_name1=malloc(sizeof(char)*100);
strcpy(t1->extra_name1,t1->name);
}
extra_ppts *ep=t1->parent->ppts;
while(ep!=NULL){
 tree_typedecl  *dec=ep->t1;
if(dec && t1->tp1!=NULL){
if(dec->name && t1->tp1->name && !strcmp(dec->name, t1->tp1->name)) {
fprintf(stderr,"FOUND AN EXTRA PROPERTY\n");
t1->extra_name2=malloc(sizeof(char)*100);
if(ep->var3!=NULL)strcpy(t1->extra_name2,ep->var3->name);
else t1->extra_name2=NULL;
break;
}
}
ep=ep->next;
}
}
t1=t1->nextv;}
//}
fprintf(FP1,";\n");
}
gf=hf=0;
int cnt=0;
t1=this->dirrhs;
while(t1!=NULL){if (t1->gpu==1 && t1->parent!=NULL)gf=1;if(t1->gpu==0&& t1->parent!=NULL)hf=1; if(hf&&gf) break; t1=t1->nextv;}
if(gf==1){ if(Gkernel==0)fprintf(FP1,"__device__ int ");else fprintf(FP1,"int ");//fprintf(FP1,"__device__ ");
t1=this->dirrhs;
while(t1!=NULL){if (t1->gpu==1){if(cnt>0)fprintf(FP1,","); fprintf(FP1," %s ",t1->name);cnt++; }
t1=t1->nextv;}
fprintf(FP1,";\n");
}
cnt=0;
if(hf==1){  fprintf(FP1,"int ");//fprintf(FP1,"__device__ ");
t1=this->dirrhs;
while(t1!=NULL){if (t1->gpu==0){if(cnt>0)fprintf(FP1,","); fprintf(FP1," %s ",t1->name);t1->iflag=1;cnt++; }
if(t1->parent!=NULL && f==1 && t1->libdtype==-1){
if(t1->parent && t1->parent->extra_name && t1->libdtype==-1){
t1->extra_name=malloc(sizeof(char)*100);
strcpy(t1->extra_name,t1->parent->extra_name);
}

if(t1->name){
t1->extra_name1=malloc(sizeof(char)*100);
strcpy(t1->extra_name1,t1->name);
}
extra_ppts *ep=t1->parent->ppts;
while(ep!=NULL){
 tree_typedecl  *dec=ep->t1;
if(dec && t1->tp1!=NULL){
if(dec->name && t1->tp1->name && !strcmp(dec->name, t1->tp1->name)) {
fprintf(stderr,"FOUND AN EXTRA PROPERTY\n");
t1->extra_name2=malloc(sizeof(char)*100);
if(ep->var3!=NULL)strcpy(t1->extra_name2,ep->var3->name);
else t1->extra_name2=NULL;
break;
}
}
ep=ep->next;
}
}

t1=t1->nextv;}
fprintf(FP1,";\n");
}

}



void dir_decl::printcodearray(int ref,char *print_string){
int n;
dir_decl *t1= this;
if(t1->open_brack==1)strcat(print_string,"( ");
if(t1->ptrcnt>0){if(t1->ptrstring!=NULL)strcat(print_string,t1->ptrstring);};
if(t1->name!=NULL){
strcat(print_string,t1->name);
}
if(t1->structexpr!=NULL){
strcat(print_string," : ");
//t1->structexpr->print(t1->structexpr);
}
if(t1->close_brack==1)strcat(print_string,") ");
if(t1->brack_flag==2)strcat(print_string, ");");
if(t1->sbrack_flag==1)strcat(print_string,"[ ");
if(t1->stat==1)strcat(print_string,"static ");
if(t1->typelist!=NULL && t1->typelist->name!=NULL) strcat(print_string,t1->typelist->name);
if(t1->assign!=NULL){
t1->assign->printcode1(t1->assign,print_string);
}
if(t1->brack_flag==2)strcat(print_string,") ");
if(t1->sbrack_flag==1)strcat(print_string,"] ");
if(t1->next!=NULL)t1->next->printcode(ref,print_string);
if(t1->rhs!=NULL){
strcat(print_string,"="); t1->rhs->printcode1(t1->rhs,print_string);
}
return;
}

void dir_decl::printcode(int ref,char *print_string){
int n;
dir_decl *t1= this;
if(t1->open_brack==1)strcat(print_string,"( ");
if(t1->ptrcnt>0){if(t1->ptrstring!=NULL)strcat(print_string,t1->ptrstring);};
if(t1->name!=NULL){
if(t1->gpu){
if( t1->libdtype==SET_TYPE && ref==1 && t1->params==NULL)strcat(print_string,"GSet &");
if( t1->libdtype==SET_TYPE && ref==0 && t1->params==NULL)strcat(print_string,"GSet ");
if( t1->libdtype==GRAPH_TYPE && ref==1 && t1->params==NULL)strcat(print_string,"GGraph &");
if( t1->libdtype==GRAPH_TYPE && ref==0 && t1->params==NULL)strcat(print_string,"GGraph ");
if( ( t1->libdtype==POINT_TYPE||t1->libdtype==EDGE_TYPE) && ref==1 && t1->params==NULL && t1->parent!=NULL)strcat(print_string,"int &");
if( (t1->libdtype==POINT_TYPE||t1->libdtype==EDGE_TYPE) && ref==0 && t1->params==NULL && t1->parent!=NULL)strcat(print_string,"int/***/ ");
if( t1->libdtype==POINT_TYPE&& ref==0 && t1->params==NULL && t1->parent==NULL)strcat(print_string,"struct Point ");
if( t1->libdtype==POINT_TYPE&& ref==1 && t1->params==NULL && t1->parent==NULL)strcat(print_string,"struct Point &");
if( t1->libdtype==EDGE_TYPE&& ref==0 && t1->params==NULL && t1->parent==NULL)strcat(print_string,"struct Edge ");
if( t1->libdtype==EDGE_TYPE&& ref==1 && t1->params==NULL && t1->parent==NULL)strcat(print_string,"struct Edge ");
}else {
if( t1->libdtype==SET_TYPE && ref==1 && t1->params==NULL)strcat(print_string,"HSet &");
if( t1->libdtype==SET_TYPE && ref==0 && t1->params==NULL)strcat(print_string,"HSet ");
if( t1->libdtype==GRAPH_TYPE && ref==1 && t1->params==NULL)strcat(print_string,"/***/HGraph &");
if( t1->libdtype==GRAPH_TYPE && ref==0 && t1->params==NULL)strcat(print_string,"HGraph ");
if( ( t1->libdtype==POINT_TYPE||t1->libdtype==EDGE_TYPE) && ref==1 && t1->params==NULL /*&& t1->parent!=NULL*/)strcat(print_string,"int &");
if( (t1->libdtype==POINT_TYPE||t1->libdtype==EDGE_TYPE) && ref==0 && t1->params==NULL && t1->parent!=NULL)strcat(print_string,"int ");
if( t1->libdtype==POINT_TYPE&& ref==0 && t1->params==NULL && t1->parent==NULL)strcat(print_string,"struct Point ");
if( t1->libdtype==POINT_TYPE&& ref==1 && t1->params==NULL && t1->parent==NULL)strcat(print_string,"struct Point &");
if( t1->libdtype==EDGE_TYPE&& ref==0 && t1->params==NULL && t1->parent==NULL)strcat(print_string,"struct Edge ");
if( t1->libdtype==EDGE_TYPE&& ref==1 && t1->params==NULL && t1->parent==NULL)strcat(print_string,"struct Edge ");
}
strcat(print_string,t1->name);
}
if(t1->structexpr!=NULL){
strcat(print_string," : ");
//t1->structexpr->print(t1->structexpr);
}
if(t1->close_brack==1)strcat(print_string,") ");
if(t1->brack_flag==2)strcat(print_string, ");");
if(t1->sbrack_flag==1)strcat(print_string,"[ ");
if(t1->stat==1)strcat(print_string,"static ");
if(t1->typelist!=NULL && t1->typelist->name!=NULL) strcat(print_string,t1->typelist->name);
if(t1->assign!=NULL){
t1->assign->printcode1(t1->assign,print_string);
}
if(t1->ptrflag==1)strcat(print_string," *");
if(t1->parnames!=NULL){
dir_decl *t2=t1->parnames->dirrhs;
while(t2!=NULL && t2->nextv!=NULL){
if(t2->name!=NULL){
strcat(print_string,",");
strcat(print_string,t2->name);
}
t2=t2->nextv;
}

if(t2!=NULL &&t2->name!=NULL){
strcat(print_string,",");
strcat(print_string,t2->name);
}
}
if(t1->params!=NULL){
tree_decl_stmt *t2=t1->params;
while(t2!=NULL && t2->next!=NULL){
//fprintf(FP1,"/*PARAM*/");
if(t2->dirrhs->forit==1 ){
if(t2->dirrhs!=NULL){
t2->dirrhs->extra_name=malloc(sizeof(char)*100);
strcpy(t2->dirrhs->extra_name,t2->dirrhs->parent->extra_name);
if(t2->dirrhs->gpu==1){
t2->dirrhs->extra_name1=malloc(sizeof(char)*100);
strcpy(t2->dirrhs->extra_name1,"id");
if(GLOBAL_ID_FLAG==1){
//fprintf(FP1,"//global id sfour \n");
strcpy(t2->dirrhs->extra_name1,"id+graph.start_point");
}
t2=t2->next;
continue;
}else {

if(t2->dirrhs!=NULL)t2->dirrhs->printcode(ref,print_string);
strcat(print_string,",");
t2->dirrhs->extra_name1=malloc(sizeof(char)*100);
strcpy(t2->dirrhs->extra_name1,t2->dirrhs->name);
}
strcpy(t2->dirrhs->extra_name,t2->next->dirrhs->extra_name);
}
t2=t2->next;continue;}
if(t2->lhs!=NULL&&t2->lhs->name!=NULL && t2->lhs->libdatatype!=GRAPH_TYPE && t2->lhs->libdatatype!=POINT_TYPE && t2->lhs->libdatatype!=EDGE_TYPE && t2->lhs->libdatatype!=SET_TYPE)strcat(print_string,t2->lhs->name);


if(t2->lhs->libdatatype==SET_TYPE){
int gf=0,hf=0;
tree_typedecl *x2=t2->lhs;
while(x2->next!=NULL)x2=x2->next;
if(x2!=NULL && x2->d1!=NULL) gf=x2->d1->gpu;
if(gf==1){
if(ref==1) strcat(print_string,"GSet &");
if(ref==0) strcat(print_string,"GSet ");
}
if(gf==0){
if(ref==1) strcat(print_string,"HSet &");
if(ref==0) strcat(print_string,"HSet ");

}

}

if(t2->dirrhs!=NULL)t2->dirrhs->printcode(ref,print_string);
strcat(print_string,",/**U**/");
t2=t2->next;
}
fprintf(FP1,"/*%d*/",t2->lhs->libdatatype);
if(t2!=NULL &&t2->lhs!=NULL&&t2->lhs->name!=NULL&& t2->lhs->libdatatype!=GRAPH_TYPE && t2->lhs->libdatatype!=POINT_TYPE && t2->lhs->libdatatype!=EDGE_TYPE && t2->lhs->libdatatype!=SET_TYPE)strcat(print_string,t2->lhs->name);
if(t2->lhs->libdatatype==SET_TYPE){
int gf=0,hf=0;
tree_typedecl *x2=t2->lhs;
while(x2->next!=NULL)x2=x2->next;
if(x2!=NULL && x2->d1!=NULL) gf=x2->d1->gpu;
if(gf==1){
if(ref==1) strcat(print_string,"GSet &");
if(ref==0) strcat(print_string,"GSet ");
}
if(gf==0){
if(ref==1) strcat(print_string,"HSet &");
if(ref==0) strcat(print_string,"HSet ");

}
}
if(t2!=NULL && t2->dirrhs!=NULL)t2->dirrhs->printcode(ref,print_string);
}
if(t1->brack_flag==2)strcat(print_string,") ");
if(t1->sbrack_flag==1)strcat(print_string,"] ");
if(t1->next!=NULL)t1->next->printcode(ref,print_string);
if(t1->rhs!=NULL){
strcat(print_string,"="); t1->rhs->printcode1(t1->rhs,print_string);
}
return;
}








void dir_decl::printcode(int ref){
int n;
dir_decl *t1= this;
//fprintf(FP1,"/*%d*/",t1->libdtype);
//fprintf(FP1,"/*x %d %d*/",t1->gpugpu,t1->gpu);
if(t1->open_brack==1)fprintf(FP1,"( ");
if(t1->ptrcnt>0){if(t1->ptrstring!=NULL)fprintf(FP1," %s ",t1->ptrstring);};
if(t1->name!=NULL){
if( t1->gpu){
if( t1->libdtype==SET_TYPE && ref==1 && t1->params==NULL)fprintf(FP1,"GSet &");
if( t1->libdtype==SET_TYPE && ref==0 && t1->params==NULL)fprintf(FP1,"GSet ");
if( t1->libdtype==GRAPH_TYPE && ref==1 && t1->params==NULL)fprintf(FP1,"GGraph &");
if( t1->libdtype==GRAPH_TYPE && ref==0 && t1->params==NULL)fprintf(FP1,"GGraph ");
//it happens here
if( t1->libdtype==POINT_TYPE&& ref==0 && t1->params==NULL && t1->parent==NULL)fprintf(FP1,"struct Point ");
if( t1->libdtype==POINT_TYPE&& ref==1 && t1->params==NULL && t1->parent==NULL)fprintf(FP1,"struct Point &");
if( t1->libdtype==EDGE_TYPE&& ref==0 && t1->params==NULL && t1->parent==NULL)fprintf(FP1,"struct Edge ");
if( t1->libdtype==EDGE_TYPE&& ref==1 && t1->params==NULL && t1->parent==NULL)fprintf(FP1,"struct Edge ");
if(( t1->libdtype==POINT_TYPE||t1->libdtype==EDGE_TYPE) && ref==1 && t1->params==NULL && t1->parent!=NULL)fprintf(FP1,"int &");
if(( t1->libdtype==POINT_TYPE||t1->libdtype==EDGE_TYPE) && ref==0 && t1->params==NULL && t1->parent!=NULL)fprintf(FP1,"int /***/");
}else {
if( t1->libdtype==SET_TYPE && ref==1 && t1->params==NULL)fprintf(FP1,"HSet &");
if( t1->libdtype==SET_TYPE && ref==0 && t1->params==NULL)fprintf(FP1,"HSet ");
if( t1->libdtype==GRAPH_TYPE && ref==1 && t1->params==NULL)fprintf(FP1," /**u1**/HGraph &");
if( t1->libdtype==GRAPH_TYPE && ref==0 && t1->params==NULL)fprintf(FP1,"/*s1 %d %d*/HGraph ",t1->gpugpu,t1->gpu);
if( t1->libdtype==POINT_TYPE&& ref==0 && t1->params==NULL && t1->parent==NULL)fprintf(FP1,"struct Point  ");
if( t1->libdtype==POINT_TYPE&& ref==1 && t1->params==NULL && t1->parent==NULL)fprintf(FP1,"struct Point &");
if( t1->libdtype==EDGE_TYPE&& ref==0 && t1->params==NULL && t1->parent==NULL)fprintf(FP1,"struct Edge ");
if( t1->libdtype==EDGE_TYPE&& ref==1 && t1->params==NULL && t1->parent==NULL)fprintf(FP1,"struct Edge ");
if(( t1->libdtype==POINT_TYPE||t1->libdtype==EDGE_TYPE) && ref==1 && t1->params==NULL && t1->parent!=NULL)fprintf(FP1,"int &");
if(( t1->libdtype==POINT_TYPE||t1->libdtype==EDGE_TYPE) && ref==0 && t1->params==NULL && t1->parent!=NULL)fprintf(FP1,"int ");
}
if(t1->libdtype<0 && t1->tp1!=NULL && t1->dtype==STRUCT_TYPE)fprintf(FP1,"%s ", t1->tp1->name);
fprintf(FP1," %s ",t1->name);
}
if(t1->structexpr!=NULL){
fprintf(FP1," : ");
t1->structexpr->print(t1->structexpr);
}
if(t1->close_brack==1)fprintf(FP1,") ");
if(t1->brack_flag==2)fprintf(FP1,"( ");
if(t1->sbrack_flag==1)fprintf(FP1,"[ ");
if(t1->stat==1)fprintf(FP1,"static ");
if(t1->typelist!=NULL && t1->typelist->name!=NULL) fprintf(FP1,"%s ",t1->typelist->name);
if(t1->assign!=NULL){
t1->assign->printcode(t1->assign);
}
if(t1->ptrflag==1)fprintf(FP1," *");
if(t1->parnames!=NULL){
//fprintf(FP1,"//XX\n");
dir_decl *t2=t1->parnames->dirrhs;
while(t2!=NULL && t2->nextv!=NULL){
if(t2->name!=NULL)fprintf(FP1," %s,",t2->name);
t2=t2->nextv;
}

if(t2!=NULL &&t2->name!=NULL)fprintf(FP1,"%s",t2->name);
}
if(t1->params!=NULL){
tree_decl_stmt *t2=t1->params;
while(t2!=NULL && t2->next!=NULL){
//fprintf(FP1,"/*PARAM*/");
if(t2->dirrhs->forit==1 ){
if(t2->dirrhs!=NULL){
t2->dirrhs->extra_name=malloc(sizeof(char)*100);
if(t2->dirrhs->gpu==1){
t2->dirrhs->extra_name1=malloc(sizeof(char)*100);
strcpy(t2->dirrhs->extra_name1,"id");
if(GLOBAL_ID_FLAG==1){
//fprintf(FP1,"//global id sthree \n");
strcpy(t2->dirrhs->extra_name1,"id+graph.start_point");
}
t2=t2->next;continue;
}else {

if(t2->dirrhs!=NULL)t2->dirrhs->printcode(ref);
fprintf(FP1,",");
t2->dirrhs->extra_name1=malloc(sizeof(char)*100);
strcpy(t2->dirrhs->extra_name1,t2->dirrhs->name);
}
strcpy(t2->dirrhs->extra_name,t2->next->dirrhs->extra_name);
}
t2=t2->next;continue;}
//fprintf(FP1,"//XX\n");
if(t2->lhs!=NULL&&t2->lhs->name!=NULL && t2->lhs->libdatatype!=GRAPH_TYPE && t2->lhs->libdatatype!=POINT_TYPE && t2->lhs->libdatatype!=EDGE_TYPE && t2->lhs->libdatatype!=SET_TYPE && t2->lhs->libdatatype!=COLLECTION_TYPE && t2->dirrhs->parent==NULL)fprintf(FP1,"%s ",t2->lhs->name);

if(t2->lhs!=NULL&&t2->lhs->name!=NULL && t2->lhs->libdatatype!=GRAPH_TYPE && t2->lhs->libdatatype!=POINT_TYPE && t2->lhs->libdatatype!=EDGE_TYPE && t2->lhs->libdatatype!=SET_TYPE && t2->lhs->libdatatype!=COLLECTION_TYPE && t2->dirrhs->parent!=NULL)fprintf(FP1,"int ");

if(t2->lhs->libdatatype==SET_TYPE){
dir_decl *d1= t2->dirrhs;
d1->printcode(ref);
//if(d1&& d1->tp1 && d1->tp1->d1 && d1->tp1->d1->gpu==1)fprintf(FP1,"GSet %s ",d1->name);
//if(d1 && d1->tp1 && d1->tp1->d1 && d1->tp1->d1->gpu==0)fprintf(FP1,"HSet %s ",d1->name);
t2=t2->next;
continue;
}
if(t2->lhs->libdatatype==COLLECTION_TYPE){
dir_decl *d1= t2->dirrhs;
d1->printcode(ref);
if(d1 && d1->tp1 && d1->gpu==0 &&ref==1) fprintf(FP1,"Pusher%d &%s, ",PUSHER_CNT++,d1->name);
else fprintf(FP1,"GColl %s,",d1->name);

t2=t2->next;
continue;
}
if(t2->dirrhs!=NULL)t2->dirrhs->printcode(ref);
fprintf(FP1,",");
t2=t2->next;
}
if(t2!=NULL &&t2->lhs!=NULL&&t2->lhs->name!=NULL&& t2->lhs->libdatatype!=GRAPH_TYPE && t2->lhs->libdatatype!=POINT_TYPE && t2->lhs->libdatatype!=EDGE_TYPE && t2->lhs->libdatatype!=SET_TYPE && t2->lhs->libdatatype!=COLLECTION_TYPE)fprintf(FP1,"%s ",t2->lhs->name);
if(t2->lhs->libdatatype==SET_TYPE){
dir_decl *d1= t2->dirrhs;
d1->printcode(ref);
//tree_typedecl *x2=d1->tp1;
//while(x2->next!=NULL)x2=x2->next;
//if(x2->d1 && x2->d1->gpu==1)fprintf(FP1,"GSet %s ",d1->name);
//if( x2->d1 && x2->d1->gpu!=1)fprintf(FP1,"HSet %s ",d1->name);
}
if(t2->lhs->libdatatype==COLLECTION_TYPE){
dir_decl *d1= t2->dirrhs;
if(d1&& d1->tp1 && d1->tp1->d1 && d1->tp1->d1->gpu==1)fprintf(FP1,"GSet %s ",d1->name);
if(d1 && d1->tp1 && d1->gpu==0 &&ref==1)fprintf(FP1,"Pusher%d &%s ",PUSHER_CNT++,d1->name);
else fprintf(FP1,",GColl %s",d1->name);
t2=t2->next;
}
if(t2!=NULL && (t2->lhs->libdatatype!=SET_TYPE) && (t2->lhs->libdatatype!=COLLECTION_TYPE)  && t2->dirrhs!=NULL)t2->dirrhs->printcode(ref);
}
if(t1->brack_flag==2 && DUPVAR==1)fprintf(FP1,",int FALCX");
if(t1->brack_flag==2&&Gkernel==0)fprintf(FP1,") ");
if(t1->brack_flag==2&&Gkernel==1)fprintf(FP1,",int FALCX) ");
if(t1->brack_flag==2&&Gkernel>1)fprintf(FP1,") ");
if(t1->sbrack_flag==1)fprintf(FP1,"] ");
if(t1->rhs!=NULL){
tree_expr *te1= t1->rhs;
while(te1->lhs!=NULL)te1=te1->lhs;
fprintf(FP1,"="); 
fprintf(stderr,"printing rhs");
char temp[250];
for(int i=0;i<250;i++)temp[i]='\0';
t1->rhs->printcode1(t1->rhs,temp);
fprintf(FP1,"%s",temp);
}
if(t1->next!=NULL)t1->next->printcode(ref);
return;
}



void type_name::printcode(char *print_string){
if(this->typedecl!=NULL)strcat(print_string,this->typedecl->name);
if(this->decl!=NULL && this->decl->name!=NULL) strcat(print_string,this->decl->name);
if(this->decl!=NULL && this->decl->ptrcnt!=NULL) strcat(print_string,"*");
}

void type_name::print(){
if(this->typedecl!=NULL)printf("x%sx",this->typedecl->name);
if(this->decl!=NULL) this->decl->print();
}
tree_decl_stmt *currdecl=NULL,*decl_temp;

proc_id::proc_id(){
name=NULL;params=NULL;param_count=0;vararg=0;
}
    symtableentry::symtableentry() {
type=NULL;
isarg=0;
}

symtableentry::insert(tree_typedecl *type1, dir_decl *idlist){
printf("INSERT\n");
this->type=type1;
dir_decl *head=idlist;
while(head!=NULL){
this->vars.insert(head);
head->dtype=type1->datatype;
head->tp1=type1;
head->libdtype=type1->libdatatype;
head->stype=type->storagetype;
//head->ctype=type->compoundtype;
//head->stable=currsymtab;
/*if (type->datatype==STRUCT_TYPE ){
printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUU\n");
}*/
head->decltype=type1;
head=head->nextv;
}

printf("INSERT\n");
std::set<dir_decl *>::iterator I = this->vars.begin();
for(; I!=vars.end();I++){
dir_decl *temp=*I;
printf("inserted %s to symtbale \n",temp->name);
}
}
    bool symtableentry::getarg() {
        return isarg;
    }
    int symtable::getsymtabtype() {
        return symtabtype;
    }


    void symtable::setparent(symtable* p) {
        parent = p;
        //assert(this!=parent);
    }
    symtable* symtable::getparent() {
        return parent;
    }
int symtable::checkdup(dir_decl *id){
dir_decl *head=id;
if(head==NULL)return 1;
while(head->next!=NULL){
  dir_decl *head1=head->nextv;
if(head1==NULL)return 1;
while(head1!=NULL){
if(!strcmp(head->name,head1->name)){
printf("same variable name used twice in declaration \n");
return -1;
}
head1=head1->nextv;
}
head=head->nextv;
}
return 1;
}
symtable::addparams(dir_decl *id,tree_typedecl *type){
         symtableentry *newref=new symtableentry();
         newref->insert(type,id);
        add_symbol(newref);

}
    bool symtable::addsymbol1(dir_decl* id, tree_typedecl* type, symtableentry*& old_def) {
printf("ADD SYMBOL1");
//     if(checkdup(id)==-1)return false;
 //       old_def = findsymbol(id);
//if(old_def!=NULL)printf("VARIABLE IS THERE\n");
  //      if (old_def != NULL) {
//fprintf(stderr,"variable name already used before\n");
//	return false;
//}
         symtableentry *newref=new symtableentry();
         newref->insert(type,id);
dir_decl *head=id;
while(head!=NULL){
head->stable=this;
head=head->next;
}
        add_symbol(newref);
        return true;
    }
//this function is not called from anywhere
symtable::print_decl(tree_decl_stmt *head){
printf("**************** STRUCT DECLARATIONS********\n");
tree_decl_stmt *tmp=head;
while(tmp!=NULL){
printf ("%s ",dtypenames[tmp->lhs->datatype]);
dir_decl *id=tmp->dirrhs;
while(id!=NULL){
printf ("%s  ",id->name);
id=id->next;
};
tmp=tmp->next;
printf("\n");
}
printf("**************** STRUCT DECLARATIONS********\n");
}

    bool symtable::addsymbol(dir_decl * id, tree_typedecl* type1, symtableentry*& old_def) {
printf("addsymbol\n");
//if(id->name!=NULL)printf("%s",id->name);
//if(id==NULL)return;//added on feb11
//if(type1!=NULL && type1->datatype==STRUCT_TYPE)fprintf(FP1,"iadd symbol STRUCT ");
printf("%s ", dtypenames[type1->datatype]);
if(type1->datatype==TYPEDEF_TYPE){
int flag=0;
printf("HERE");
for (std::map<char *,tree_typedecl *>::iterator it=this->newtypes.begin(); it!=this->newtypes.end(); ++it){
printf("loop\n");
printf("%s ", it->first);
if(!strcmp(it->first,id->name)){flag=1;break;}
}
if(flag==0){
printf("new type %s", id->name);
this->newtypes[id->name]=type1;
if(type1->next!=NULL && type1->next->vname!=NULL){
printf("ADDING NEW TYPE STRUCT NODE\n");
this->newtypes[type1->next->name]=type1;

}
type1->tpdef=1;
printf("\nSUCCESSFULL DECLARATION1\n");
}else { printf(" datatype %s alreadly defined error \n",id->name);}
return;
}

if(type1==NULL)printf("NULL\n");
tree_typedecl *x,*type=type1;
while(type->next!=NULL&& ( type->datatype==TYPEDEF_TYPE || type->datatype==-1))type=type->next;
if (type->datatype==STRUCT_TYPE || type->datatype==UNION_TYPE ||type->datatype==ENUM_TYPE){
printf("addsymbol is def true\n");
if(type->datatype==ENUM_TYPE&& type->def==1){
printf("addsymbol it is true\n");
int f1=0;
if(type->name!=NULL)printf("%s",type->name);
tree_typedecl *t1=findtypedefsymbol(type->vname);
if(t1==NULL) this->newtypes[type->vname]=type;
else {printf("datatype %s alerady defined",type->name); return;}
if(id!=NULL)addsymbol1(id,type,old_def);
return;
}

 printf("addsymbol%d\n",type->datatype);
type->compoundtype=1;
printf ("\n****************STRUCT/ENUM/UNION DECLARATION****************\n\n");
printf("%s  %s \n",type->name,type->vname);
tree_decl_stmt *head= type->list;
int flag=0;
tree_decl_stmt  *ihead;
if(type->def==1){
while(head->next!=NULL){
if(checkdup(head->dirrhs)==-1)return false;

ihead=head->next;
head=head->next;
}
printf("UNNI\n");
if((type->name!=NULL) ){
printf("KRISHNAN\n");
x=findtypedefsymbol(type->name);
printf("HERE %s\n",type->name);
if(x==NULL){
this->newtypes[type->name]=type;
printf("\nSUCCESSFULL DECLARATION1\n");
}else { printf(" datatype %s alreadly defined error \n",type->name);}
}
if(type->name==NULL)
printf("\nSUCCESSFULL DECLARATION\n");
} 
if(id!=NULL){
int f1=0;
if(type->name!=NULL){

x=this->findtypedefsymbol(type->name);
if(x==NULL){printf("compound datatype %s not defined \n",type->name);
}

else if( type->datatype!=x->datatype&& x->datatype!=TYPEDEF_TYPE){printf("datatype of %s doesnot match with declaration",x->name);}
else {
printf("%s",x->name);
addsymbol1(id,x,old_def);
}
}
}
if(id==NULL){
//if(type1!=NULL && type1->list!=NULL)fprintf(FP1,"iadd symbol STRUCT ");
if(type1!=NULL && type1->list!=NULL && type1->list->dirrhs!=NULL){
//if(type1!=NULL && type1->datatype==STRUCT_TYPE)fprintf(FP1,"iadd symbol STRUCT ");
type1->list->dirrhs->stable=this;

}
}
return;
}
if(id->procd==1){
printf("PROC DECLARATION\n");
printf("%s ",id->name);
if(checkdup(id)==-1) return false;
else return addsymbol1(id,type,old_def);
}
printf("not enum/struct/union\n");
        return addsymbol1(id, type, old_def);
    }
tree_typedecl *symtable::findtypedefsymbol(char *name){
int flag=0;
tree_typedecl *t1;
printf("OUTSIDE LOOP\n");
for (std::map<char *,tree_typedecl *>::iterator it=this->newtypes.begin(); it!=this->newtypes.end(); ++it){
printf("LOOP\n");
printf("%s ",it->first);
if(!strcmp(it->first,name)&&(strlen(it->first)==strlen(name))){t1=it->second;flag=1;break;}
}
if(flag==1){
printf("typedef datatype found\n");
return t1;
}
else {
if(this->parent==NULL) return NULL;
else return this->parent->findtypedefsymbol(name);

}


}
symtable *symtable::findtable(char *name) {
        std::set<symtableentry*>::iterator I;
        std::set<dir_decl *>::iterator J;
        dir_decl *variable;
        for (I = entries.begin(); I != entries.end(); I++) {
            symtableentry* e = *I;
        for (J =e->vars.begin(); J != e->vars.end(); J++) {
            variable= *J;
             char* c = variable->name;
if(! strcmp(c,name)){return  this;}
       }
        }
        if (parent == NULL) return NULL;
    else return parent->findtable(name);
}
dir_decl *symtable::findsymbol(char *name) {
        std::set<symtableentry*>::iterator I;
        std::set<dir_decl *>::iterator J;
        dir_decl *variable;
        for (I = entries.begin(); I != entries.end(); I++) {
            symtableentry* e = *I;
        for (J =e->vars.begin(); J != e->vars.end(); J++) {
            variable= *J;
             char* c = variable->name;
if(! strcmp(c,name)){return  variable;}
       }
        }
        if (parent == NULL) return NULL;
    else return parent->findsymbol(name);
}

dir_decl *symtable::findsymbol(char *name,int x) {
        std::set<symtableentry*>::iterator I;
        std::set<dir_decl *>::iterator J;
        dir_decl *variable;
        for (I = entries.begin(); I != entries.end(); I++) {
            symtableentry* e = *I;
        for (J =e->vars.begin(); J != e->vars.end(); J++) {
            variable= *J;
             char* c = variable->name;
if(! strcmp(c,name)){return  variable;}
       }
        }
//        return parent->findsymbol(name);
         return NULL;
}

    symtableentry* symtable::findsymbol(dir_decl* id) {
printf("FIND SYMBOL  %d\n",declared);
        std::set<symtableentry*>::iterator I;
        std::set<dir_decl *>::iterator J;
        dir_decl *variable;
         dir_decl *head=id;
          while(head!=NULL){ 
        for (I = entries.begin(); I != entries.end(); I++) {
            symtableentry* e = *I;
        for (J =e->vars.begin(); J != e->vars.end(); J++) {
            variable= *J;
             char* c = variable->name;
   printf("%s %s\n" ,c,head->name);
if(! strcmp(c,head->name)){return  e;}
//printf("SIZE1 %s %s\n",c,head->name);
       }
        }
 head=head->nextv;
    }
printf("returning null\n");
         return NULL;
}
    int symtable::getsize() {
        return entries.size();
    }

    std::set<symtableentry*>& symtable::getentries() {
        return entries;
    }
    void symtable::removeentry(symtableentry *e) {
        entries.erase(e);
    }

    void symtable::merge(symtable* A) {
        //assert(A!=NULL);
        std::set<symtableentry*>::iterator i;
        for (i = A->entries.begin(); i != A->entries.end(); i++) {
            entries.insert(*i);
        }
        A->entries.clear();
}
    void symtable::add_symbol(symtableentry* e) {
        entries.insert(e);
       
    }
int globsymtable::checkdup(dir_decl *id){
dir_decl *head=id;
if(head==NULL)return 1;
printf("cd");
while(head->nextv!=NULL){
printf("CD");
  dir_decl *head1=head->nextv;
if(head1==NULL)return 1;
while(head1!=NULL){
printf("%s  %s \n",head->name,head1->name);
if(!strcmp(head->name,head1->name)&&strlen(head->name)==strlen(head1->name)){
printf("same variable name used twice in declaration \n");
return -1;
}
head1=head1->nextv;
}
head=head->nextv;
}
return 1;
}
dir_decl *libsymtable::findsymbol(char *name) {
        std::set<libsymtableentry*>::iterator I;
        std::set<dir_decl *>::iterator J;
        dir_decl *variable;
        for (I = entries.begin(); I != entries.end(); I++) {
            libsymtableentry* e = *I;
        for (J =e->vars.begin(); J != e->vars.end(); J++) {
            variable= *J;
             char* c = variable->name;
if(! strcmp(c,name)&&(strlen(c)==strlen(name))){return  variable;}
       }
        }
        if (parent == NULL) return NULL;
        return parent->findsymbol(name);
         return NULL;
}
libsymtableentry::insert(tree_typedecl *type1, dir_decl *idlist){
static int DEV_NO=0;
printf("INSERT\n");
this->type=type1;
dir_decl *head=idlist;
while(head!=NULL){
if((currsymtab)==(&GLOBAL_TABLE)){
if(type1->libdatatype==COLLECTION_TYPE)GALOIS_FLAG=1;
head->isparam=true;
}
if(type1->libdatatype==GRAPH_TYPE&&head->gpu==1 && head->arg!=1){
fprintf(stderr,"/* set device  %s %d */\n",head->name,DEV_NO);
head->dev_no=DEV_NO%4;
DEV_NO=(DEV_NO+1)%4;
TOT_GPU_GRAPH++;
}
printf("%s ", head->name);
this->vars.insert(head);
if(type1->libdatatype==SET_TYPE || type1->libdatatype==COLLECTION_TYPE){
printf("SET TYPE %s %d \n",type1->name,type1->libdatatype);
tree_typedecl *tt1= new tree_typedecl();
tt1->libdatatype=type1->libdatatype;
tt1->name=malloc(sizeof(char)*100);
if(head->tp1 && head->tp1->d1)tt1->d1=head->tp1->d1;
strcpy(tt1->name,type1->name);
tt1->next=head->tp1;
head->libdtype=tt1->libdatatype;
if(tt1->d1 && tt1->d1->gpu==1)head->gpu=1;
if(type1->libdatatype==SET_TYPE)head->tp1=tt1;
//if(type1->libdatatype==COLLECTION_TYPE)head->tp1=tt1;
//head->tp1=tt1;
printf( "%s %d  %d ",head->name,tt1->libdatatype,head->tp1->libdatatype);
}else{ head->libdtype=type1->libdatatype;
head->tp1=type1;
}
head->stype=type->storagetype;
head->libstable=currsymtab->libsym;
head->ppts=type1->ppts;
head->decltype=type1;
printf( "%s %d ",head->name,head->tp1->libdatatype);
head=head->nextv;
}
struct extra_ppts *ep=type1->ppts;
while(ep!=NULL){
fprintf(stderr,"ADDED  extrappty=%s",ep->name);
ep=ep->next;
}
}
bool libsymtable::addsymbol1(dir_decl *id,tree_typedecl *type,libsymtableentry *&old_def){         
printf("LIB TABLE INSERT\n");
libsymtableentry *newref=new libsymtableentry();
         newref->insert(type,id);
dir_decl *head=id;
while(head!=NULL){
        head->parent=type->d1;
if(type->d1!=NULL)fprintf(stderr,"COLL %S ",type->d1->name);//FEB14
//printf("XX");
head->libstable=this;
//printf("XX");
head=head->next;
}
entries.insert(newref);
}
    libsymtableentry* libsymtable::findsymbol(dir_decl* id) {
printf("FIND SYMBOL  %d\n",declared);
        std::set<libsymtableentry*>::iterator I;
        std::set<dir_decl *>::iterator J;
        dir_decl *variable;
         dir_decl *head=id;
          while(head!=NULL){ 
        for (I = entries.begin(); I != entries.end(); I++) {
            libsymtableentry* e = *I;
        for (J =e->vars.begin(); J != e->vars.end(); J++) {
            variable= *J;
             char* c = variable->name;
   printf("%s %s\n" ,c,head->name);
if(! strcmp(c,head->name)&&(strlen(c)==strlen(head->name))){return  e;}
//printf("SIZE1 %s %s\n",c,head->name);
       }

        }
 head=head->nextv;
    }
//        if (parent == NULL) return NULL;
  //      return parent->findsymbol(id);
printf("NULL RETURNED\n");
         return NULL;
}
int globsymtable::addsymbol(dir_decl *x,tree_typedecl *type1){

dir_decl *head=x;
printf("GLOBAL INSERT\n");
if(this->checkdup(head)==-1) {return;}
symtableentry *id=this->sym->findsymbol(x);
if(id!=NULL){
printf("variable already used in current block\n");
return;
}

libsymtableentry *lid=NULL;


if(type1->libdatatype!=-1){
if(type1->libdatatype==E_P_TYPE ||type1->libdatatype==P_P_TYPE || type1->libdatatype==G_P_TYPE){
//if(libdtypenames[type1->libdatatype][0]!=libdtypenames[type1->d1->libdtype][0]){printf("TYPE MISMATCH\n") ;}
dir_decl *d1=x,*d2=type1->d1;
if(d2!=NULL){
extra_ppts *ex= type1->ppts;
ex->libdtype=type1->t1->libdatatype;
//fprintf(FP1,"adding %s to %s libdatatype=%d datatype=%d",x->name,d2->name,ex->libdtype,type1->t1->datatype);
x->dtype=type1->t1->datatype;
ex->name=malloc(sizeof(char)*100);
strcpy(ex->name,x->name);
//ex->var2=x;commented on march 23rd
ex->next=d2->ppts; d2->ppts=ex;
ex=d2->ppts;

//while(ex!=NULL){
//fprintf(stderr, "pptname = %s %d  %d \n",ex->name,ex->var2->dtype,ex->t1->datatype);
//ex=ex->next;
//}
}else {
 if(this->libsym==NULL)  { this->libsym=new libsymtable(); this->libsym->gparent=this;}
    libsymtableentry *old=NULL;
type1->libdatatype=-1;
this->libsym->addsymbol1(x,type1,old);
} 
if(type1->libdatatype==G_P_TYPE){
//fprintf(stderr,"adding to graph a property");
newitr[x->name]=++nitr;
}
}
else {
if(this->libsym!=NULL)lid=this->libsym->findsymbol(x);
if(lid!=NULL){
printf("variable already used in current blocki\n");
return;
}
printf("GLOBAL INSERT\n");
 if(this->libsym==NULL) {this->libsym=new libsymtable();this->libsym->gparent=this;}
libsymtableentry *old=NULL;
this->libsym->addsymbol1(x,type1,old);
return;
}
}

else {
symtableentry *old=NULL;
if(this->sym==NULL)this->sym=new symtable();
this->sym->addsymbol(x,type1,old);
return;
}
}
dir_decl *globsymtable::findsymbol(char *name){
dir_decl *x;
if(this->sym!=NULL){
x=this->sym->findsymbol(name);
if(x!=NULL) return x;
}
if(this->libsym!=NULL){
x=this->libsym->findsymbol(name);
printf("found %s in libsymtable",name);
if(x!=NULL) return x;
}

if(this->parent!=NULL)return this->parent->findsymbol(name);
return NULL;
}
int cnt=0;
check_gpu_expr(tree_expr *expr){
if(expr==NULL)return 0;
fprintf(stderr, " %d ",expr->expr_type);
if(expr->expr_type==VAR || expr->expr_type==ARRREF){
if(expr->lhs!=NULL){
//if(((dir_decl *)(expr->lhs))->libdtype==GRAPH_TYPE)fprintf(FP1," %s %d \n",((dir_decl *)(expr->lhs))->name,((dir_decl *)(expr->lhs))->gpugpu);
if(((dir_decl *)(expr->lhs))->gpu==1 /*|| ((dir_decl *)(expr->lhs))->dup_name!=NULL*/)return 1;
tree_expr *dup=expr;
while(dup->lhs && dup->lhs->lhs )dup=dup->lhs;
if(dup->lhs &&((dir_decl *)(dup->lhs))->dup_name && DUPVAR==5){
//fprintf(FP1,"DUPVAR %s %d\n",((dir_decl *)(dup->lhs))->dup_name,DUPVAR);
return 1;
}
if(dup->lhs &&((dir_decl *)(dup->lhs))->dup_name ){
//fprintf(FP1,"DUPVAR %s %d\n",((dir_decl *)(dup->lhs))->dup_name,DUPVAR);
}
if((((dir_decl *)(expr->lhs))->gpugpu>1)|| (((dir_decl *)(expr->lhs))->cpugpu==1))return 5;
else return 0;
}
//else return 0;
}
fprintf(stderr, "lhs ");
if(expr->lhs!=NULL) {
fprintf(stderr,"%s\n" ,expr->lhs->name);
if(check_gpu_expr(expr->lhs)==1)return 1;
if(check_gpu_expr(expr->lhs)==5)return 5;
}
fprintf(stderr, "rhs ");
if(expr->rhs!=NULL) {
fprintf(stderr,"%s\n" ,expr->rhs->name);
if(check_gpu_expr(expr->rhs)==1)return 1;
if(check_gpu_expr(expr->rhs)==5)return 5;
}
return 0;
}
int check_gpu(statement *t1){
int ret=0;
tree_expr *ls,*rs;
ls=t1->stassign->lhs;
//fprintf(FP1, "LHS ");
if(ls!=NULL && check_gpu_expr(ls)==1)ret=1;
if(ls!=NULL && check_gpu_expr(ls)==5){
ret=5;
return ret;
}
rs=t1->stassign->rhs;
fprintf(stderr, "RHS ");
if(rs!=NULL&& check_gpu_expr(rs)==1){if(ret==1)ret=3;else ret=2;} 
fprintf(stderr, "done ");
return ret;
}
recu_expr_type(tree_expr *expr){
fprintf(FP1," type=%d \n",expr->expr_type);
if(expr->lhs!=NULL){
fprintf(FP1,"lhs ");
recu_expr_type(expr->lhs);
}else fprintf(FP1,"NULL");
if(expr->rhs!=NULL){
fprintf(FP1,"rhs ");
recu_expr_type(expr->rhs);
}else fprintf(FP1,"NULL");

}


int print_gpu_if_stmt(statement *t1,int val){
//fprintf(FP1,"/*GPU ASS*/");
char arr1[200],arr2[200],arr3[200];
for(int i=0;i<200;i++){arr3[i]=arr2[i]=arr1[i]='\0';}
if(val==1){
//fprintf(FP1,"//val==1\n");
t1->expr1->expr_type=-1;
//t1->expr1->rhs=NULL;
//recu_expr_type(t1->expr1->rhs);
//fprintf(FP1,"*/ ");
//fprintf(FP1,"/* ");
//recu_expr_type(t1->expr1->lhs);
//fprintf(FP1,"*/ ");
t1->expr1->lhs->printcode1(t1->expr1->lhs,arr1);
t1->expr1->rhs->printcode1(t1->expr1->rhs,arr2);
if(DUPVAR==5){
char *tt=strdup(arr1);
strcpy(arr1,"GPU_");
strcat(arr1,tt);
fprintf(FP1, "//GPU_%s\n",arr1);
}
//if(gpcopy_string[0]!='\0')fprintf(FP1,"/*GP COPY NOT NULL */\n");
//fprintf(FP1, "//val1=1lhs=%s   rhs=%s\n",arr1,arr2);
//fprintf(FP1,"/** lhs is libtype %d**/\n",t1->expr1->lhs->libdtype);

if(t1->expr1->lhs->libdtype>=0){
tree_expr *te1=t1->expr1->lhs;
while(te1->lhs!=NULL)te1=te1->lhs;
if(t1->expr1->lhs->libdtype>0 && t1->expr1->lhs->libdtype!=3 && t1->expr1->rhs->libdtype==t1->expr1->lhs->libdtype){
if(te1->clone==0){
printf("clone==0\n");
}
else {
printf(" CHECKING\n");
if(gpcopy_string[0]!='\0'){
fprintf(FP1,"%s\n",gpcopy_string);
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
char tarr[200],*p,temp[200];
printf(" CHECKING\n");
for(int i=-0;i<200;i++)tarr[i]='\0';
p=strstr(arr1,"extra))->");
sprintf(tarr,"temp%d.",extemp);
extemp++;
if(extemp<=10)sprintf(tarr+6,"%s",p+(strlen("extra))->")));
if(extemp>10)sprintf(tarr+7,"%s",p+(strlen("extra))->")));
for(int i=0;i<25;i++)temp[i]='\0';
if(t1->expr1->lhs->libdtype==7)strcpy(temp,"npoints");
if(t1->expr1->lhs->libdtype==6)strcpy(temp,"nedges");
sprintf(arr3,"%s.%s",te1->name,temp);

for(int i=-0;i<200;i++)arr1[i]=tarr[i];
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy((%s),(%s),sizeof(int)*%s,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,arr2,arr3,cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy((%s),(%s),%s,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
return;
}
char temp[25];
te1=t1->expr1->rhs;
while(te1->lhs!=NULL)te1=te1->lhs;
for(int i=0;i<25;i++)temp[i]='\0';
if(t1->expr1->lhs->libdtype==7)strcpy(temp,"npoints");
else if(t1->expr1->lhs->libdtype==6)strcpy(temp,"nedges");
else {
if(t1->expr1->rhs&& t1->expr1->rhs->rhs)sprintf(temp,"n%s",t1->expr1->rhs->rhs->name);
}
if(te1!=NULL)sprintf(arr3,"%s.%s",te1->name,temp);
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),sizeof(%s),cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,arr2,arr3,cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),%s,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
}
}
}
if(t1->expr1->rhs->dtype>=0 && t1->expr1->rhs->dtype==11){
//fprintf(FP1,"/********* here something has to be done code has to be written ******************/\n");
}
if(t1->expr1->rhs->dtype>=0 && t1->expr1->rhs->dtype!=11){

if(t1->expr1->lhs &&((dir_decl *)(t1->expr1->lhs->lhs))->ctype==CARR_TYPE){

fprintf(FP1,"//array assign\n");
fprintf(FP1,"cudaMemcpy((void *)&");
t1->expr1->lhs->printcode(t1->expr1->lhs);
fprintf(FP1,",&");
t1->expr1->rhs->printcode(t1->expr1->rhs);
fprintf(FP1,",sizeof(%s)*",dtypenames[t1->expr1->rhs->dtype]);
((dir_decl *)(t1->expr1->lhs->lhs))->next->assign->printcode(((dir_decl *)(t1->expr1->lhs->lhs))->next->assign);
fprintf(FP1,",cudaMemcpyHostToDevice);\n");







}
else {
if(((tree_expr *)(t1->expr1->lhs))->expr_type==STRUCTREF){
tree_expr *te1= t1->expr1->lhs;
while(te1->lhs!=NULL)te1=te1->lhs;
dir_decl *dd=te1;
//if(dd!=NULL)fprintf(FP1,"cudaSetDevice(%d);\n",dd->dev_no);
}
fprintf(FP1, "%s  falcvt%d;\n",dtypenames[t1->expr1->rhs->dtype],++Temp);
if(t1->expr1->lhs->expr_type==VAR){fprintf(FP1,"%scudaMemcpyFromSymbol(&(falcvt%d),%s,sizeof(%s),0,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],Temp,arr1,dtypenames[t1->expr1->rhs->dtype],cpy[1],errcnt++,cpy[2]);
t1->expr1->lhs->cpu_name=malloc((sizeof(char))*100);
sprintf(t1->expr1->lhs->cpu_name,"falcvt%d",Temp);
//fprintf(FP1,"cudaSetDevice(0);\n");
//if(DUPVAR!=5)
fprintf(FP1,"\nif(%s==",t1->expr1->lhs->cpu_name);
t1->expr1->rhs->printcode(t1->expr1->rhs);
if(DUPVAR==5){
fprintf(FP1, "&& %s==",arr1+4);
t1->expr1->rhs->printcode(t1->expr1->rhs);
}
fprintf(FP1,")");
if(t1->f1!=NULL){
t1->f1->codeGen(FP1);
return;
}
}
else{
fprintf(FP1,"falcvt%d=",Temp);
t1->expr1->rhs->printcode(t1->expr1->rhs);
fprintf(FP1,";\n");
if(gpcopy_string[0]!='\0'){
fprintf(FP1,"%s",gpcopy_string);
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
char tarr[200],*p;
printf(" CHECKING\n");
for(int i=-0;i<200;i++)tarr[i]='\0';
if(arr1[strlen(arr1)-1]==']'){
p=strstr(arr1,"extra))->");
sprintf(tarr,"temp%d.",extemp);//this changd made due to strstr in structref tree.c
extemp++;
if(p)sprintf(tarr+6,"%s",p+(strlen("extra))->")));//this changd made due to strstr in structref tree.c
}
else {
p=strrchr(arr1,'.');
sprintf(tarr,"temp%d",extemp);//this changd made due to strstr in structref tree.c
extemp++;
sprintf(tarr+5,"%s",p+1);//this changd made due to strstr in structref tree.c
}
for(int i=-0;i<200;i++)arr1[i]=tarr[i];
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy(&falcvt%d,&(%s),sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s",cpy[0],Temp,arr1,dtypenames[t1->expr1->rhs->dtype],cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy(&falcvt%d,&(%s),sizeof(%s),cudaMemcpyHDeviceToHost)%s%d%s",cpy[0],Temp,arr1,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
//fprintf(FP1,"cudaSetDevice(0);\n");
if(DUPVAR!=5)fprintf(FP1,"/*XX*/if(falcvt%d==%s)\n",Temp,arr2);
if(DUPVAR==5){
fprintf(FP1,"if(falcvt%d==%s && %s==%s)\n",Temp,arr2,arr1+4,arr2);
}
if(t1->f1!=NULL)t1->f1->codeGen(FP1);
return;
}
 fprintf(FP1,"%scudaMemcpy(&falcvt%d,&(%s),sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s",cpy[0],Temp,arr1,dtypenames[t1->expr1->rhs->dtype],cpy[1],errcnt++,cpy[2]);
//fprintf(FP1,"cudaSetDevice(0);\n");
fprintf(FP1,"\n/*XX*/if(falcvt%d==",Temp);
t1->expr1->rhs->printcode(t1->expr1->rhs);
if(DUPVAR==5){
fprintf(FP1," && %s=",arr1+4);
t1->expr1->rhs->printcode(t1->expr1->rhs);
}
fprintf(FP1,")");
++Temp;
if(t1->f1!=NULL){
t1->f1->codeGen(FP1);
return;
}
}
}
}
}
if(val==2){
printf("value=2");
fprintf(FP1,"//val==2\n");
t1->expr1->lhs->printcode1(t1->expr1->lhs,arr1);
if(t1->expr1->lhs->lhs->libdtype>=0){
t1->expr1->rhs->printcode1(t1->expr1->rhs,arr2);
fprintf(FP1,"//rhsXX");
if((t1->expr1->lhs->lhs->libdtype==0|| t1->expr1->lhs->libdtype==3)&& t1->expr1->rhs->lhs->libdtype==t1->expr1->lhs->lhs->libdtype){
if(t1->expr1->lhs->lhs->clone==0){
t1->expr1->lhs->lhs->clone=1;
fprintf(FP1,"%s.cloneCPU(%s);",arr2,arr1);
}
else {fprintf(FP1,"%s.copytoCPU(%s);",arr2,arr1);}
}
tree_expr *te1=t1->expr1->lhs;
while(te1->lhs!=NULL)te1=te1->lhs;
if(t1->expr1->lhs->lhs->libdtype>0&& t1->expr1->rhs->lhs->libdtype==t1->expr1->lhs->lhs->libdtype){
if(te1->clone==0){
printf("clone==0\n");
}
else {
char temp[25];
//te1=t1->stassign->lhs;
//while(te1->lhs!=NULL)te1=te1->lhs;
printf("HERE");
if(gpcopy_string[0]!='\0'){
//fprintf(FP1,"/*GP*/%s",gpcopy_string);
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
char tarr[200],*p;
for(int i=-0;i<200;i++)tarr[i]='\0';
p=strstr(arr2,"extra))->");
sprintf(tarr,"temp%d.",extemp);
extemp++;
if(extemp<=10)sprintf(tarr+6,"%s",p+(strlen("extra))->")));
if(extemp>10)sprintf(tarr+7,"%s",p+(strlen("extra))->")));

for(int i=-0;i<200;i++)arr2[i]=tarr[i];
for(int i=0;i<25;i++)temp[i]='\0';
if(t1->expr1->lhs->libdtype==7)strcpy(temp,"npoints");
if(t1->expr1->lhs->libdtype==6)strcpy(temp,"nedges");
sprintf(arr3,"%s.%s",te1->name,temp);
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy((%s),(%s),sizeof(int)*%s,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,arr3,cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy((%s),(%s),%s,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
return;
}
for(int i=0;i<25;i++)temp[i]='\0';
if(t1->expr1->lhs->libdtype==7)strcpy(temp,"npoints");
if(t1->expr1->lhs->libdtype==6)strcpy(temp,"nedges");
sprintf(arr3,"%s.%s",te1->name,temp);
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,arr3,cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),%s,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
}
}
}
if(t1->expr1->lhs->lhs->dtype>=0){
t1->expr1->rhs->printcode1(t1->expr1->rhs,arr2);
if(gpcopy_string[0]!='\0'){
//fprintf(FP1,"%s",gpcopy_string);
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
char tarr[200],*p;
printf(" CHECKING\n");
for(int i=-0;i<200;i++)tarr[i]='\0';
if(arr2[strlen(arr2)-1]==']'){
p=strstr(arr2,"extra))->");
sprintf(tarr,"temp%d.",extemp);
extemp++;
sprintf(tarr+6,"%s",p+(strlen("extra))->")));
}
else {
p=strrchr(arr2,'.');
sprintf(tarr,"temp%d.",extemp);
extemp++;
sprintf(tarr+6,p+1);
}
for(int i=-0;i<200;i++)arr2[i]=tarr[i];
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,dtypenames[t1->expr1->lhs->dtype],cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),%s,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
return;
}
if(t1->expr1->lhs->expr_type==VAR && t1->expr1->lhs->lhs->ptrflag==0)fprintf(FP1,"%scudaMemcpyFromSymbol(&(%s),%s,sizeof(%s),0,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,dtypenames[t1->expr1->lhs->dtype],cpy[1],errcnt++,cpy[2]);
else{
 fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,dtypenames[t1->expr1->lhs->dtype],cpy[1],errcnt++,cpy[2]);

}
}
}




}
print_gpu_assign_stmt(statement *t1,int val){
//fprintf(FP1,"/*GPU ASS %d*/",val);
char arr1[200],arr2[200],arr3[200];
for(int i=0;i<200;i++){arr3[i]=arr2[i]=arr1[i]='\0';}
if(t1->stassign && t1->stassign->lhs==NULL && t1->stassign->rhs!=NULL && t1->stassign->rhs->expr_type==GET_TYPE&&t1->stassign->rhs->rhs!=NULL){
int gf,hf;
dir_decl *t2=t1->stassign->rhs->rhs->nextv;
while(t2!=NULL){if (t2->gpu==1)gf=1;if(t2->gpu==0)hf=1; if(hf&&gf) break; t2=t2->nextv;}
if(gf==1){ fprintf(FP1,"GGraph ");
int cnt=0;
t2=t1->stassign->rhs->rhs->nextv;
while(t2!=NULL){if (t2->gpu==1){if(cnt>0)printf(","); printf("%s",t2->name);cnt++; }t2=t2->nextv;
}
fprintf(FP1,";//HERE GETTYPE\n");
}
if(hf==1){ fprintf(FP1,"/*s2*/HGraph ");
int cnt=0;
t2=t1->stassign->rhs->rhs->nextv;
while(t2!=NULL){if (t2->gpu==0){if(cnt>0)fprintf(FP1,","); fprintf(FP1,"%s",t2->name);cnt++; }t2=t2->nextv;}
fprintf(FP1,";");
}
//this->stassign->rhs->printcode(this->stassign->rhs);
return;
}
if(t1->stassign->lhs==NULL|| t1->stassign->rhs==NULL)return -1 ;
//fprintf(FP1,"//val=%d \n",val);
if(val==1){
//fprintf(FP1,"/*");
//recu_expr_type(t1->stassign->rhs);
t1->stassign->lhs->printcode1(t1->stassign->lhs,arr1);
//fprintf(FP1,"*/");
//fprintf(FP1,"/*");
//recu_expr_type(t1->stassign->lhs);
t1->stassign->rhs->printcode1(t1->stassign->rhs,arr2);
//fprintf(FP1,"*/");
//fprintf(FP1, "//val1=1lhs=%s   rhs=%s\n",arr1,arr2);
//fprintf(FP1,"/** lhs is libtype %d**/\n",t1->stassign->lhs->libdtype);
//if(gpcopy_string[0]!='\0')fprintf(FP1,"/*GP COPY NOT NULL */\n");
if(t1->stassign->lhs->lhs->libdtype>=0){
if((t1->stassign->lhs->libdtype==0|| t1->stassign->lhs->libdtype==3)&& t1->stassign->rhs->libdtype==t1->stassign->lhs->libdtype){
if(t1->stassign->lhs->lhs->clone==0){
t1->stassign->lhs->lhs->clone=1;
fprintf(FP1,"%s.cloneGPU(%s,%d );\nint %sflag=0;\n",arr2,arr1,((dir_decl *)(t1->stassign->lhs->lhs))->dev_no,arr1);
dir_decl *d1,*d2;
d1=t1->stassign->lhs->lhs;
d2=t1->stassign->rhs->lhs;
//fprintf(FP1,"/* %s %d */\n",d1->name,d1->dev_no);
//fprintf(FP1,"/* %s %d */\n",d2->name,d2->dev_no);
if(d1 && d2){
struct extra_ppts *ppts=d1->ppts;
int flag=0;
while(ppts!=NULL){
if(ppts->t1->libdatatype==G_P_TYPE){
if(flag==0){
extemp++;
fprintf(FP1,"cudaSetDevice(%d);\ncudaMalloc((void **)(&%s.extra),sizeof(%s));\n",d1->dev_no,d1->name,d1->extra_name);
fprintf(FP1,"%s temp%d;\n",d1->extra_name,extemp);
fprintf(FP1,"%scudaMemcpy(&temp%d,%s.extra,sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s\n",cpy[0],extemp,d1->name,d1->extra_name,cpy[1],errcnt++,cpy[2]);
//fprintf(FP1,"int temp%d;\n",extemp+1);
//fprintf(FP1,"cudaMemcpy(&temp%d,temp%d.n%s,sizeof(int),cudaMemcpyDeviceToHost);\n",extemp+1,extemp,ep->name);
flag=1;

}
fprintf(FP1,"temp%d.n%s=(( %s *)(%s.extra))->n%s;\n",extemp,ppts->name,d2->extra_name,arr2,ppts->name);
fprintf(FP1,"cudaMemcpyToSymbol(falc%s%s,&((( %s *)(%s.extra))->n%s),sizeof(int),0,cudaMemcpyHostToDevice);\n",d1->name,ppts->name,d2->extra_name,d2->name,ppts->name);
}
ppts=ppts->next;
}
if(flag==1) {
fprintf(FP1,"%scudaMemcpy(%s.extra,&temp%d,sizeof(%s),cudaMemcpyHostToDevice)%s%d%s\n%sflag=1;",cpy[0],d1->name,extemp,d1->extra_name,cpy[1],errcnt++,cpy[2],d1->name);

}
extemp++;
}
if(t1->stassign->lhs->lhs->extra_readfun!=NULL && t1->stassign->lhs->lhs->extra_readfun1==NULL) fprintf(FP1,"\n%s\n",t1->stassign->lhs->lhs->extra_readfun);
if(t1->stassign->lhs->lhs->extra_fun!=NULL && t1->stassign->lhs->lhs->extra_readfun1==NULL) fprintf(FP1,"\n%s\n",t1->stassign->lhs->lhs->extra_fun);
if( t1->stassign->lhs->lhs->extra_readfun1!=NULL) fprintf(FP1,"\n%s\n",t1->stassign->lhs->lhs->extra_readfun1);
fprintf(FP1,"\nint TPB%d=findthreadsperblock(&prop%d);\n",d1->dev_no,d1->dev_no);
fprintf(FP1,"\nint %spointkernelblocks=findblocksize(%s,%s.npoints,TPB%d);\n",d1->name,d1->name,d1->name,d1->dev_no);
fprintf(FP1,"\nint %sedgekernelblocks=findblocksize(%s,%s.nedges,TPB%d);\n",d1->name,d1->name,d1->name,d1->dev_no);
if(d1->libdtype==GRAPH_TYPE)fprintf(FP1,"copy%scurrentsize(%s);\n",arr1,arr1);

}
else {fprintf(FP1,"%s.copytoGPU(%s);",arr2,arr1);}
fprintf(FP1,"cudaSetDevice(0);\n");
}
tree_expr *te1=t1->stassign->lhs;
while(te1->lhs!=NULL)te1=te1->lhs;
if(t1->stassign->lhs->libdtype>0 && t1->stassign->lhs->libdtype!=3 && t1->stassign->rhs->libdtype==t1->stassign->lhs->libdtype){
if(te1->clone==0){
printf("clone==0\n");
}
else {
printf(" CHECKING\n");
if(gpcopy_string[0]!='\0'){
fprintf(FP1,"%s\n",gpcopy_string);
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
char tarr[200],*p,temp[200];
printf(" CHECKING\n");
for(int i=-0;i<200;i++)tarr[i]='\0';
p=strstr(arr1,"extra))->");
sprintf(tarr,"temp%d.",extemp);
extemp++;
if(extemp<=10 &&p)sprintf(tarr+6,"%s",p+(strlen("extra))->")));
if(extemp>10 &&p)sprintf(tarr+7,"%s",p+(strlen("extra))->")));
for(int i=0;i<25;i++)temp[i]='\0';
if(t1->stassign->lhs->libdtype==7)strcpy(temp,"npoints");
if(t1->stassign->lhs->libdtype==6)strcpy(temp,"nedges");
sprintf(arr3,"%s.%s",te1->name,temp);

for(int i=-0;i<200;i++)arr1[i]=tarr[i];
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy((%s),(%s),sizeof(int)*%s,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,arr2,arr3,cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy((%s),(%s),%s,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
return;
}
char temp[25];
te1=t1->stassign->rhs;
while(te1->lhs!=NULL)te1=te1->lhs;
for(int i=0;i<25;i++)temp[i]='\0';
if(t1->stassign->lhs->libdtype==7)strcpy(temp,"npoints");
else if(t1->stassign->lhs->libdtype==6)strcpy(temp,"nedges");
else {
if(t1->stassign->rhs&& t1->stassign->rhs->rhs)sprintf(temp,"n%s",t1->stassign->rhs->rhs->name);
}
if(te1!=NULL)sprintf(arr3,"%s.%s",te1->name,temp);
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),sizeof(%s),cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,arr2,arr3,cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),%s,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
}
}
}
if(t1->stassign->lhs->lhs  && t1->stassign->lhs->lhs->libdtype==4){
char arr1[100];
char arr2[100];
for(int ii=0;ii<100;ii++)arr1[ii]=arr2[ii]='\0';
t1->stassign->lhs->printcode1(t1->stassign->lhs,arr1);
t1->stassign->rhs->printcode1(t1->stassign->rhs,arr2);
fprintf(FP1,"cudaMemcpy(%s,&%s,sizeof(int),cudaMemcpyHostToDevice);\n",arr1,arr2);
return;
}
if(t1->stassign->rhs->dtype>=0 && t1->stassign->rhs->dtype==11){
fprintf(FP1,"/********* here something has to be done code has to be written ******************/\n");}
if(t1->stassign->rhs->dtype>=0 && t1->stassign->rhs->dtype!=11){

if(t1->stassign->lhs &&((dir_decl *)(t1->stassign->lhs->lhs))->ctype==CARR_TYPE){

fprintf(FP1,"//array assign\n");
fprintf(FP1,"cudaMemcpy((void *)&");
t1->stassign->lhs->printcode(t1->stassign->lhs);
fprintf(FP1,",&");
t1->stassign->rhs->printcode(t1->stassign->rhs);
fprintf(FP1,",sizeof(%s)*",dtypenames[t1->stassign->rhs->dtype]);
((dir_decl *)(t1->stassign->lhs->lhs))->next->assign->printcode(((dir_decl *)(t1->stassign->lhs->lhs))->next->assign);
fprintf(FP1,",cudaMemcpyHostToDevice);\n");







}
else {
if(((tree_expr *)(t1->stassign->lhs))->expr_type==STRUCTREF){
tree_expr *te1= t1->stassign->lhs;
while(te1->lhs!=NULL)te1=te1->lhs;
dir_decl *dd=te1;
//if(dd!=NULL)fprintf(FP1,"cudaSetDevice(%d);\n",dd->dev_no);
}
fprintf(FP1, "%s  falcvt%d;\n",dtypenames[t1->stassign->rhs->dtype],++Temp);
fprintf(FP1,"falcvt%d=",Temp);
t1->stassign->rhs->printcode(t1->stassign->rhs);
fprintf(FP1,";\n");
if(t1->stassign->lhs->expr_type==VAR && DUPVAR==5)fprintf(FP1,"%scudaMemcpyToSymbol(GPU_%s,&(falcvt%d),sizeof(%s),0,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,Temp,dtypenames[t1->stassign->rhs->dtype],cpy[1],errcnt++,cpy[2]);
else if(t1->stassign->lhs->expr_type==VAR)fprintf(FP1,"%scudaMemcpyToSymbol(%s,&(falcvt%d),sizeof(%s),0,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,Temp,dtypenames[t1->stassign->rhs->dtype],cpy[1],errcnt++,cpy[2]);
else{
if(gpcopy_string[0]!='\0'){
fprintf(FP1,"%s",gpcopy_string);
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
char tarr[200],*p;
printf(" CHECKING\n");
for(int i=-0;i<200;i++)tarr[i]='\0';
if(arr1[strlen(arr1)-1]==']'){
p=strstr(arr1,"extra))->");
sprintf(tarr,"temp%d.",extemp);//this changd made due to strstr in structref tree.c
extemp++;
if(p)sprintf(tarr+6,"%s",p+(strlen("extra))->")));//this changd made due to strstr in structref tree.c
}
else {
p=strrchr(arr1,'.');
sprintf(tarr,"temp%d",extemp);//this changd made due to strstr in structref tree.c
extemp++;
sprintf(tarr+5,"%s",p+1);//this changd made due to strstr in structref tree.c
}
for(int i=-0;i<200;i++)arr1[i]=tarr[i];
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy(&(%s),&(falcvt%d),sizeof(%s),cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,Temp,dtypenames[t1->stassign->rhs->dtype],cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy(&(%s),&(falcvt%d),%s,cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,Temp,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
//fprintf(FP1,"cudaSetDevice(0);\n");
return;
}
 fprintf(FP1,"%scudaMemcpy(&(%s),&(falcvt%d),sizeof(%s),cudaMemcpyHostToDevice)%s%d%s",cpy[0],arr1,Temp,dtypenames[t1->stassign->rhs->dtype],cpy[1],errcnt++,cpy[2]);

}
}
}
}
fprintf(FP1,"//val=%d \n",val);
if(val==2){
printf("value=2");
t1->stassign->lhs->printcode1(t1->stassign->lhs,arr1);
if(t1->stassign->lhs->lhs->libdtype>=0){
//fprintf(FP1,"//RHS PRINT HERE 1\n");
t1->stassign->rhs->printcode1(t1->stassign->rhs,arr2);
//fprintf(FP1, "//val2=1lhs=%s   rhs=%s\n",arr1,arr2);
if((t1->stassign->lhs->libdtype==0|| t1->stassign->lhs->libdtype==3)&& t1->stassign->rhs->libdtype==t1->stassign->lhs->libdtype){
if(t1->stassign->lhs->lhs->clone==0){
t1->stassign->lhs->lhs->clone=1;
fprintf(FP1,"%s.cloneCPU(%s);",arr2,arr1);
}
else {fprintf(FP1,"%s.copytoCPU(%s);",arr2,arr1);}
}
tree_expr *te1=t1->stassign->lhs;
while(te1->lhs!=NULL)te1=te1->lhs;
if(t1->stassign->lhs->libdtype>0&& t1->stassign->rhs->libdtype==t1->stassign->lhs->libdtype){
//if(te1->clone==0){
//fprintf(FP1,"clone==0\n");
//}
//else {
char temp[25];
//te1=t1->stassign->lhs;
//while(te1->lhs!=NULL)te1=te1->lhs;
//fprintf(FP1,"HERE\n");
if(gpcopy_string[0]!='\0'){//arr2 should be gpu part of print
fprintf(FP1,"%s",gpcopy_string);
//fprintf(FP1,"// %s arr1=%s   arr1=%s ",gpcopy_string,arr1,arr2);
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
char tarr[200],*p;
for(int i=-0;i<200;i++)tarr[i]='\0';
p=strstr(arr2,"extra))->");
sprintf(tarr,"temp%d.",extemp);
extemp++;
if(extemp<=10 &&p)sprintf(tarr+6,"%s",p+(strlen("extra))->")));
if(extemp>10 &&p )sprintf(tarr+7,"%s",p+(strlen("extra))->")));

for(int i=-0;i<200;i++)arr2[i]=tarr[i];
for(int i=0;i<25;i++)temp[i]='\0';
if(t1->stassign->lhs->libdtype==7)strcpy(temp,"npoints");
if(t1->stassign->lhs->libdtype==6)strcpy(temp,"nedges");
sprintf(arr3,"%s.%s",te1->name,temp);
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy((%s),(%s),sizeof(int)*%s,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,arr3,cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy((%s),(%s),%s,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
return;
}
for(int i=0;i<25;i++)temp[i]='\0';
if(t1->stassign->lhs->libdtype==7)strcpy(temp,"npoints");
if(t1->stassign->lhs->libdtype==6)strcpy(temp,"nedges");
sprintf(arr3,"%s.%s",te1->name,temp);
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,arr3,cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),%s,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
//}
}
}
if(t1->stassign->rhs->lhs )fprintf(FP1,"//Dtype %d %d=\n",t1->stassign->lhs->libdtype,t1->stassign->rhs->lhs->libdtype);
if(t1->stassign->rhs->lhs  && t1->stassign->rhs->lhs->libdtype==4){
char arr1[100];
char arr2[100];
for(int ii=0;ii<100;ii++)arr1[ii]=arr2[ii]='\0';
t1->stassign->lhs->printcode1(t1->stassign->lhs,arr1);
t1->stassign->rhs->printcode1(t1->stassign->rhs,arr2);
fprintf(FP1,"cudaMemcpy(&%s,%s,sizeof(int),cudaMemcpyDeviceToHost);\n",arr1,arr2,dtypenames[t1->stassign->lhs->dtype]);
return;
}
if(t1->stassign->lhs->dtype>=0){
//fprintf(FP1,"//RHS PRINT HERE\n");
t1->stassign->rhs->printcode1(t1->stassign->rhs,arr2);
if(gpcopy_string[0]!='\0'){
fprintf(FP1,"%s",gpcopy_string);
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
char tarr[200],*p;
printf(" CHECKING\n");
for(int i=-0;i<200;i++)tarr[i]='\0';
if(arr2[strlen(arr2)-1]==']'){
p=strstr(arr2,"extra))->");
sprintf(tarr,"temp%d.",extemp);
extemp++;
sprintf(tarr+6,"%s",p+(strlen("extra))->")));
}
else {
p=strrchr(arr2,'.');
sprintf(tarr,"temp%d.",extemp);
extemp++;
sprintf(tarr+6,p+1);
}
for(int i=-0;i<200;i++)arr2[i]=tarr[i];
if(gp_string[0]=='\0')fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,dtypenames[t1->stassign->lhs->dtype],cpy[1],errcnt++,cpy[2]);
else {
fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),%s,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,gp_string,cpy[1],errcnt++,cpy[2]);
for(int i=0;i<200;i++)gp_string[i]='\0';
}
return;
}
if(t1->stassign->lhs->expr_type==VAR && t1->stassign->lhs->lhs->ptrflag==0)fprintf(FP1,"%scudaMemcpyFromSymbol(&(%s),%s,sizeof(%s),0,cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,dtypenames[t1->stassign->lhs->dtype],cpy[1],errcnt++,cpy[2]);
else{
 fprintf(FP1,"%scudaMemcpy(&(%s),&(%s),sizeof(%s),cudaMemcpyDeviceToHost)%s%d%s",cpy[0],arr1,arr2,dtypenames[t1->stassign->lhs->dtype],cpy[1],errcnt++,cpy[2]);

}
}
}
if(val==3){
if(t1->stassign->lhs->libdtype==COLLECTION_TYPE && t1->stassign->rhs->libdtype==COLLECTION_TYPE){
fprintf(FP1,"%s.ptr=%s.ptr;\n",t1->stassign->lhs->name,t1->stassign->rhs->name);
}
}
if(val==5){
//print_partitioned_assign(t1);
//printexpr_type(t1->stassign->lhs,0);
}
}

