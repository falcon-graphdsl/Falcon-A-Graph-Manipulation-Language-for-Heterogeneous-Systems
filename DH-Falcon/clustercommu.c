#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <symtabold.h>
//#include "falctypes.h"
#include "externs.h"
extern statement *CURR_CLUST_DEF;
extern int CURR_CLUST_CNT;
extern bool findextrpptyexist(dir_decl *,char *);
extern bool checkconstant(tree_expr *);
extern int  checklibfunction(char *);
extern char forcondarr[];
extern void print_assign_type(FILE *FP1,enum ASSIGN_TYPE );
extern char threadsizestring[];
extern intpow(int,int);
extern char peer_access_enable[];
extern char parallel_sections[];
extern char parallel_section[];
extern fpos_t mainfpos;
extern int FUN_EBLOCK_RET,CPUGPU;
extern char *union_string[2];
extern char *pdim_string[4];
extern char *morphalloc_string[3],*cpy[];
extern int utflag,PEER_ENABLE;
extern  char *utflagarr[2][3];
extern char *treenodetypestr;
extern int FOREACH_FLAG,EXTRA_CRBRK,MAINF,foreachhostflag,NESTED_FOREACH;
extern char *updatecopies[];
extern int updatecopiescnt;
//struct globvar_in_fun *globvars_fun;
//extern tree_typedecl *findtypedefsymbol(char *);
//prints expr tree inorder
extern char threadstmt[];
extern int TARGET;
extern globsymtable GLOBAL_TABLE;
void commfindstructref(dir_decl *d1,tree_expr *expr){
if(expr->expr_type==STRUCTREF){
if(expr->lhs && expr->lhs->expr_type==VAR){
if(expr->lhs->lhs==d1){
strcpy(updatecopies[updatecopiescnt++],expr->rhs->name);
//fprintf(FP1,"%s.%s",expr->lhs->name,updatecopies[updatecopiescnt-1]);
}
}
return;
}
if(expr->lhs!=NULL)commfindstructref(d1,expr->lhs);
if(expr->rhs!=NULL)commfindstructref(d1,expr->rhs);
}
//inorder print of expr types in a tree root node.

 commprintexpr_type(tree_expr *expr,int d){
if(expr==NULL){
fprintf(stderr,"null ");
return;
}
printexpr_type(expr->lhs,++d);
d--;
fprintf(stderr," node %d %d\n",expr->expr_type,d);
printexpr_type(expr->rhs,++d);
d--;
}
extern statement *partforeach;
extern int unnicnt,utcnt,singlearr,reduxcnt,tempfun,partcodegen;
extern char curr_fun_name[],curr_fun_name1[];
extern char barr_after[];
extern fpos_t position;
extern struct UNION_KER *head_position;
extern int CPU_COLL_FLAG,PROCESS_CNT,PUSHER_CNT;
//generates openmp paralle section  kernel call for one of the two gpu partitions of graph. 
commcodegenparallelsection(dir_decl *d2, char *fun,int dev,int part){
int flag=0;
char *xx=malloc(sizeof(char)*100);
char *fun1=malloc(sizeof(char)*100);
strcpy(fun1,fun);
 sprintf(xx,"%sgpu",fun);
char *pointsize=malloc(sizeof(char)*100);
for(std::map<char *,statement *>::reverse_iterator it1=fnames.rbegin();it1!=fnames.rend();it1++){//foreach function declaration
fprintf(FP1,"//%s ",it1->first);
if(!strcmp(it1->first,xx)&&(strlen(it1->first)==strlen(xx))){
statement *tt=it1->second;
if(tt->foreachflag==1){flag=1;break;}
}

}

fprintf(FP1,"//FUN %s %d\n",fun,flag);
if(flag==1 && dev==1)strcat(fun1,"gpu");
if(flag==1 && dev==0 && d2->cpugpu==true)strcat(fun1,"gpu");
if(flag==0 && d2->cpugpu==true &&d2->gpugpu>0 && dev==1)strcat(fun1,"gpu");//DEC 26,2015

if(flag==1)sprintf(pointsize,"%s.devparts[%d].npoints/*xx*/",d2->name,part);
else sprintf(pointsize,"%s.npoints",d2->name);
fprintf(FP1,"\n \n/* {\n*/ cudaSetDevice(%d);\nfor(int kk=0;kk<%s.npoints;kk+=%s.devparts[%d].pointkernelblocksize){\n %s<<<%s.devparts[%d].pointkernelblocksize,TPB%d>>>(%s.devparts[%d],kk,%s);\n}/*cudaDeviceSynchronize();\n}*/\n",dev,d2->name,d2->name,part,fun1,d2->name,part,dev,d2->name,part,pointsize);
}
//generated kernel call for two gpu partitions prints parallel sections.
void commprintgpugpuforeach(dir_decl *d2, statement *stmt){

if(stmt->feb==0){
if(stmt->stassign!=NULL && stmt->stassign->rhs->expr_type==FUNCALL&&checklibfunction(stmt->stassign->rhs->name)){
assign_stmt *ta=stmt->stassign->rhs->arglist;
while(ta!=NULL && ta->next!=NULL){
}
}
if(stmt->stassign!=NULL &&stmt->stassign->lhs!=NULL && stmt->stassign->lhs->expr_type==STRUCTREF){
fsetpos(FP1, &position);
fpos_t position1=position;
char tempar[10000];
for(int i=0;i<10000;i++)tempar[i]='\0';
int fpos=0;
char fc;
while(fscanf(FP1,"%c",&fc)!=EOF)tempar[fpos++]=fc;
tempar[fpos]='\0';
fsetpos(FP1, &position1);
char temparr[20000],temparr1[2000],temparr2[2000];
for(int i=0;i<20000;i++)temparr[i]='\0';
for(int i=0;i<2000;i++)temparr1[i]=temparr2[i]='\0';
sprintf(temparr,"__global__ void falctfun%d(GGraph %s,int FALCX,int npoints){\n %s if (id < npoints) {\n ",tempfun,d2->name,threadstmt,d2->name);
dir_decl *dt1;
tree_expr *tex1=stmt->stassign->lhs;
while(tex1->lhs!=NULL)tex1=tex1->lhs;
dt1=(dir_decl *)tex1;
dt1->extra_name1=malloc(sizeof(char)*100);
strcpy(dt1->extra_name1,"id");
stmt->stassign->lhs->printcode1(stmt->stassign->lhs,temparr1);
strcat(temparr,temparr1);
fprintf(FP1,"%s",temparr);
stmt->stassign->rhs->printcode1(stmt->stassign->rhs,temparr2);
fprintf(FP1,"=%s;\n}\n}\n",temparr2);
fgetpos(FP1,&position);
fprintf(FP1,"%s",tempar);
if(d2->gpugpu>0){
fprintf(FP1,"%s",parallel_sections);
//codegenparallelsection(d2,"falcftfun",1,0);
//codegenparallelsection(d2,"falcftfun",2,1);
fprintf(FP1,"\n}\n");
}
}
}
if(stmt->feb!=0){
fprintf(FP1,"%s",parallel_sections);
//fprintf(FP1,"#pragma omp parallel sections \n{\n#pragma omp section \n{\n");
fprintf(FP1,"cudaSetDevice(%d);\nfor(int kk=0;kk<%s.devparts[0].npoints;kk=%s.devparts[0].pointkernelblocks*TPB%d){\n%s<<<%s.devparts[0].pointkernelblocks,TPB%d>>>(",d2->dev_no,d2->name,d2->name,d2->dev_no,stmt->stassign->rhs->name,d2->name,d2->dev_no);
assign_stmt *ta=stmt->stassign->rhs->arglist;
ta=ta->next;
int cnt=0;
while(ta!=NULL && ta->next!=NULL){
fprintf(FP1,"%s,",ta->rhs->name);
ta=ta->next;
}
if(ta!=NULL){
if(ta->rhs->lhs==d2)
fprintf(FP1,"%s.devparts[0],kk);\n",d2->name);
else fprintf(FP1,"%s,kk);\n",ta->rhs->name);
}
fprintf(FP1,"}/*\ncudaDeviceSynchronize();\ncudaSetDevice(0);\n*/");
//codegenparallelsection(d2,stmt->stassign->rhs->name,d2->dev_no,0);
fprintf(FP1,"/*\n}*/cudaSetDevice(%d);\nfor(int kk=0;kk<%s.devparts[1].npoints;kk=%s.devparts[1].pointkernelblocks*TPB%d){\n%s<<<%s.devparts[1].pointkernelblocks,TPB%d>>>(",d2->dev_no,d2->name,d2->name,d2->dev_no,stmt->stassign->rhs->name,d2->name,d2->dev_no);

ta=stmt->stassign->rhs->arglist;
ta=ta->next;
 cnt=0;
while(ta!=NULL && ta->next!=NULL){
fprintf(FP1,"%s,",ta->rhs->name);
ta=ta->next;
}
if(ta!=NULL){
if(ta->rhs->lhs==d2)
fprintf(FP1,"%s.devparts[1],kk);\n",d2->name);
else fprintf(FP1,"%s,kk);\n",ta->rhs->name);
}
fprintf(FP1,"}\ncudaDeviceSynchronize();\ncudaSetDevice(0);\n");

}
}
//not sure with what this and its correctness.
void commprintcpugpuforeach(dir_decl *d2,statement *stmt){
dir_decl *d1=d2;
struct extra_ppts *ep=d1->ppts; 
int tt=++Temp;
statement *tt1=stmt->next;

if(tt1->sttype==ASSIGN_STMT&& tt1->stassign->rhs && tt1->stassign->rhs->expr_type==STRUCTREF&& tt1->stassign->rhs->rhs &&tt1->stassign->rhs->rhs->expr_type==FUNCALL&&!strcmp(tt1->stassign->rhs->rhs->name,"updatePartition")){
fprintf(FP1," %s temp%d;\n",d1->extra_name,tt);
fprintf(FP1,"cudaMemcpy(temp%d,%s.devparts[0].inbuffer,sizeof(%s),cudaMemcpyDeviceToHost);\n",tt,d1->name,d1->extra_name);
while(ep!=NULL){

if(ep->t1->libdatatype!=G_P_TYPE){
fprintf(FP1,"%s *temp%d=((%s )*(%s.hostparts[0].extra))->%s;\n",dtypenames[ep->t1->datatype],++Temp,d1->extra_name,d1->name,ep->name);
fprintf(FP1,"cudaMemcpy(temp%d.%s,temp%d+(%s.hostparts[0].npoints),sizeof(%s)*%s.devparts[0].inbuffsize,cudaMemcpyHostToDevice);\n",tt,ep->name,Temp,d1->name,dtypenames[ep->t1->datatype],d1->name);
fprintf(FP1,"cudamemcpy( ((%s )(%s.hostparts[0].inbuffer))->%s,temp%d.%s+%s.npoints,sizeof(%s)*%s.devparts[0].outbuffsize,cudaMemcpyDeviceToHost);\n",d1->extra_name,d1->name,ep->name,tt,ep->name,d1->name,dtypenames[ep->t1->datatype],d1->name);
}
ep=ep->next;
}
fprintf(FP1,"\nmodi%sgpu<<<%s.devparts[0].inbuffsize/prop%d.maxThreadPerBlock+1,prop%d.maxThreadsPerBlock>>>(%s.devparts[0],0);\n\n",d1->name,d1->name,d1->dev_no,d1->dev_no,d1->name);
fprintf(FP1,"\n#pragma omp parallel for num_threads(12)\n for(int ii=0;ii<%s.hostparts[0].inbuffsize;ii++)modi%s(ii,%s.hostparts[0]);",d1->name,d1->name,d1->name);
}
if(stmt->stassign!=NULL && stmt->stassign->rhs->expr_type==FUNCALL&&d2->gpugpu==0){
int flag=0;
char *xx=malloc(sizeof(char)*100);
 sprintf(xx,"%sgpu",stmt->stassign->rhs->name);
for(std::map<char *,statement *>::reverse_iterator it1=fnames.rbegin();it1!=fnames.rend();it1++){//foreach function declaration
if(!strcmp(it1->first,xx)&&(strlen(it1->first)==strlen(xx))){
statement *tt=it1->second;
if(tt->foreachflag==1){flag=1;break;}
}

}
if(flag==0)fprintf(FP1,"\ncudaSetDevice(%d);\nfor(int kk=0;kk<%s.npoints;kk+=%spointkernelblocks*TPB%d){ \n%sgpu<<<%spointkernelblocks,TPB%d>>>(",d2->dev_no,d2->name,d2->name,d2->dev_no,stmt->stassign->rhs->name,d2->name,d2->dev_no);
if(flag==1)fprintf(FP1,"\ncudaSetDevice(%d);\nfor(int kk=0;kk<%s.devparts[0].npoints;kk+=%spointkernelblocks*TPB%d){ \n%sgpu<<<%spointkernelblocks,TPB%d>>>(",d2->dev_no,d2->name,d2->name,d2->dev_no,stmt->stassign->rhs->name,d2->name,d2->dev_no);

assign_stmt *ta=stmt->stassign->rhs->arglist;
ta=ta->next;
int cnt=0;
while(ta!=NULL && ta->next!=NULL){
fprintf(FP1,"%s,",ta->rhs->name);
ta=ta->next;
}
if(ta!=NULL){
if(ta->rhs->lhs==d2&& d2->cpugpu==true){

if(flag==1)fprintf(FP1,"%s.devparts[0],kk,%s.devparts[0].npoints/*correct here*/);\n",d2->name,d2->name);
if(flag==0)fprintf(FP1,"%s.devparts[0],kk,%s.npoints/*correct here*/);\n",d2->name,d2->name);

}
}
fprintf(FP1,"}\ncudaSetDevice(0);\n");
}else {
if(d2->gpugpu==0)fprintf(FP1,"cudaSetDevice(%d);\nfor(int kk=0;kk<%s.devparts[0].npoints;kk+=%spointkernelblocksize){\n falctfun%d<<<%spointkernelblocksize,TPB%d>>>(%s.devparts[0],kk,%s.devparts[0].npoints);\n}",d2->dev_no,d2->name,d2->name,tempfun++,d2->name,d2->dev_no,d2->name,d2->name);
}
if(stmt->stassign!=NULL && stmt->stassign->rhs->expr_type==FUNCALL){
int flag=0;
char *xx=malloc(sizeof(char)*100);
 sprintf(xx,"%sgpu",stmt->stassign->rhs->name);
for(std::map<char *,statement *>::reverse_iterator it1=fnames.rbegin();it1!=fnames.rend();it1++){//foreach function declaration
if(!strcmp(it1->first,xx)&&(strlen(it1->first)==strlen(xx))){
statement *tt=it1->second;
if(tt->foreachflag==1){flag=1;break;}
}

}
if(flag==0)fprintf(FP1,"#pragma omp parallel for   num_threads(12)\nfor(int i=0;i<%s.npoints;i++)%s(i,",d2->name,stmt->stassign->rhs->name);
if(flag==1)fprintf(FP1,"#pragma omp parallel for   num_threads(12)\nfor(int i=0;i<%s.hostparts[0].npoints;i++)%s(i,",d2->name,stmt->stassign->rhs->name);
assign_stmt *ta=stmt->stassign->rhs->arglist;
ta=ta->next;
int cnt=0;
while(ta!=NULL && ta->next!=NULL){
fprintf(FP1,"%s,",ta->rhs->name);
ta=ta->next;
}
if(ta!=NULL){
if(ta->rhs->lhs==d2&& d2->cpugpu==true)
fprintf(FP1,"%s.hostparts[0]);\n",d2->name);
else fprintf(FP1,"%s,kk);\n",ta->rhs->name);
}
fprintf(FP1,"cudaDeviceSyncrhonize();\n");
}
else {
fprintf(FP1,"#pragma omp parallel for   num_threads(12)\nfor(int id=0;id<%s.hostparts[0].npoints;id++)falctfun%d( id,%s.hostparts[0]);",d2->name,tempfun++,d2->name);
}

}
void commfindextrapropretry(statement *t1){
statement *temp=t1;
if(temp->sttype==ASSIGN_STMT){
if(temp->stassign->rhs!=NULL)temp->stassign->rhs->findextraproperties(temp->stassign->rhs);

}
}


void statement::commcodeGen1(){
for(int i=0;i<300;i++)gpcopy_string[i]='\0';
this->commcodeGen(FP1);

}
//prints code for foreach statement and its rhs if it is function call
void commprint_foreach(statement *t){
tree_expr *e2= t->expr2;
tree_expr *e1= t->expr1;
dir_decl *d2=e2->lhs;
dir_decl *d1=e1->lhs;
char arr[200];
for(int i=0;i<200;i++)arr[i]='\0';
tree_expr *e5= t->expr5;
if(e5!=NULL) e5->rhs->printcode1(e5->rhs,arr);
if(d2!=NULL && d2->forit==1){
int itr=t->itr;
dir_decl *d2p=d2->parent;
//union_string[union_type]
if(d2->libdtype==POINT_TYPE){
d2->extra_name1=malloc(sizeof(char)*100);
if(GLOBAL_ID_FLAG==0&&Gkernel>0)strcpy(d2->extra_name1,"id");
if(GLOBAL_ID_FLAG==0&&Gkernel==0)strcpy(d2->extra_name1,d2->name);
printf("HERE\n");
if(itr==4|| itr==2){
fprintf(FP1," %d",Gkernel);
fprintf(FP1,"/*XXX*/int falcft%d=%s.index[%s+1]-%s.index[%s];\nint falcft%d=%s.index[%s];\n",unnicnt,d2p->name,d2->extra_name1,d2p->name,d2->extra_name1,unnicnt+1,d2p->name,d2->extra_name1);
}
if(itr==3){
fprintf(FP1,"int falcft%d=%s.index[id+1]-%s.index[id];\nint falcft%d=%s.index[id];\n",unnicnt,d2p->name,d2p->name,unnicnt+1,d2p->name);
}
if(itr==2||itr==3|| itr==4){
if(itr==4|| itr==2&&(d2->cpugpu!=true)){
fprintf(FP1,"/***/for(int falcft%d=0;falcft%d<falcft%d;falcft%d++){\n",unnicnt+2,unnicnt+2,unnicnt,unnicnt+2);
fprintf(FP1,"int ut%d=%s*(falcft%d+falcft%d);\n int ut%d=%s.edges[ut%d%s].%s;\nint ut%d=%s.edges[ut%d%s].%s;\n",utcnt,utflagarr[utflag][0],unnicnt+1,unnicnt+2,utcnt+1,d2p->name,utcnt,utflagarr[utflag][1],union_string[union_type],utcnt+2,d2p->name,utcnt,utflagarr[utflag][2],union_string[union_type]);
}
if(itr==3&&(d2->cpugpu!=true)){
fprintf(FP1,"/**/for(int falcft%d=0;falcft%d<falcft%d;falcft%d++){\n",unnicnt+2,unnicnt+2,unnicnt,unnicnt+2);
fprintf(FP1,"int ut%d=3*(%s.inedges[falcft%d+falcft%d]);\n int ut%d=%s.edges[ut%d].%s;\nint ut%d=%s.edges[ut%d+2].%s;\n",utcnt,d2p->name,unnicnt+1,unnicnt+2,utcnt+1,d2p->name,utcnt,union_string[union_type],utcnt+2,d2p->name,utcnt,union_string[union_type]);
}
d2->extra_name=malloc(sizeof(char)*100);
strcpy(d2->extra_name,d2p->extra_name);
if(GLOBAL_ID_FLAG==1){strcpy(d2->extra_name1,"id+graph.start_point");
GLOBAL_ID_FLAG=0;
}
d1->extra_name1=malloc(sizeof(char)*100);
d1->extra_name=malloc(sizeof(char)*100);
sprintf(d1->extra_name1,"ut%d",utcnt+1);
if(d2p!=NULL && d2p->extra_name!=NULL)strcpy(d1->extra_name,d2p->extra_name);
d1->parent=d2p;
FOREACH_FLAG++;
unnicnt=unnicnt+3;
utcnt=utcnt+3;
}
if(itr==2){
char temparr[400];
if(t->stassign!=NULL) {
for(int i=0;i<400;i++)temparr[i]='\0';
t->stassign->rhs->printcode1(t->stassign->rhs,temparr);fprintf(FP1,"%s;",temparr);
}
char arr[200];
for(int i=0;i<200;i++)arr[i]='\0';
tree_expr *e5= t->expr5;
if(e5!=NULL){ 
//e5->rhs->printcode1(e5->rhs,arr);
fprintf(FP1,"int falcftadv%d=%s;\n",unnicnt,e5->rhs->name);
}
d2->extra_name1=malloc(sizeof(char)*100);
if(Gkernel>0)strcpy(d2->extra_name1,"id");
if(Gkernel==0)strcpy(d2->extra_name1,d2->name);
fprintf(FP1,"int falcft%d=%s.index[id+1]-%s.index[id];\nint falcft%d=%s.index[id];\n",unnicnt,d2p->name,d2p->name,unnicnt+1,d2p->name);
if(e5!=NULL)fprintf(FP1,"for(int falcft%d=falcftadv%d;falcft%d<falcft%d;falcft%d++){\n",unnicnt+2,unnicnt,unnicnt+2,unnicnt,unnicnt+2);
else fprintf(FP1,"for(int falcft%d=0;falcft%d<falcft%d;falcft%d++){\n",unnicnt+2,unnicnt+2,unnicnt,unnicnt+2);
fprintf(FP1,"int ut%d=3*(%s.inedges[falcft%d+falcft%d]);\n int ut%d=%s.edges[ut%d].%s;\nint ut%d=%s.edges[ut%d+2].%s;\n",utcnt,d2p->name,unnicnt+1,unnicnt+2,utcnt+1,d2p->name,utcnt,union_string[union_type],utcnt+2,d2p->name,utcnt,union_string[union_type]);
d2->extra_name=malloc(sizeof(char)*100);
strcpy(d2->extra_name,d2p->extra_name);
d1->extra_name1=malloc(sizeof(char)*100);
d1->extra_name=malloc(sizeof(char)*100);
sprintf(d1->extra_name1,"ut%d",utcnt+1);
if(d2p!=NULL && d2p->extra_name!=NULL)strcpy(d1->extra_name,d2p->extra_name);
d1->parent=d2p;
unnicnt=unnicnt+3;
utcnt=utcnt+3;

}
}
}
printf("%s ", d2->name);
if(d2->libdtype==GRAPH_TYPE){
char *name=t->expr3->name;
if(t->itr>=6 && name!=NULL){
for (std::map<char *,int>::iterator it1=newitr.begin(); it1!=newitr.end(); ++it1){
if(!strcmp(it1->first,t->expr3->name)){

fprintf(FP1,"for(int falcft%d=0;falcft%d<(( %s *)(%s.extra))->n%s;falcft%d++){\n",unnicnt,unnicnt,d2->extra_name,d2->name,it1->first,unnicnt);
d1->extra_name1=malloc(sizeof(char)*100);
d1->extra_name=malloc(sizeof(char)*100);
d1->extra_name2=malloc(sizeof(char)*100);
d1->iflag=1;
strcpy(d1->extra_name2,it1->first);
fprintf(stderr,"\n\nEEEEEEE %s EEEEEEEEEEEE\n\n",it1->first);
sprintf(d1->extra_name1,"falcft%d",unnicnt);
sprintf(d1->name,"falcft%d",unnicnt);//foreach changing name of variable itself
if(d2!=NULL && d2->extra_name!=NULL)strcpy(d1->extra_name,d2->extra_name);
if(d1->extra_name==NULL)fprintf(FP1,"parent null");
if(d2->parent!=NULL)d1->parent=d2->parent;
else d1->parent=d2;
unnicnt++;
break;
}
}
}
}
if(d2->libdtype==EDGE_TYPE){






}
//printf(" DONE\n");
}
//prints assignment type
void commprint_assign_type(FILE *FP1,enum ASSIGN_TYPE tp){
if(tp==AASSIGN)fprintf(FP1,"=");
if(tp==AMUL_ASSIGN)fprintf(FP1,"*=");
if(tp==ADIV_ASSIGN)fprintf(FP1,"/=");
if(tp==AMOD_ASSIGN)fprintf(FP1,"%=");
if(tp==AADD_ASSIGN)fprintf(FP1,"+=");
if(tp==ASUB_ASSIGN)fprintf(FP1,"-=");
if(tp==ALEFT_ASSIGN)fprintf(FP1,"<=");
if(tp==ARIGHT_ASSIGN)fprintf(FP1,">=");
if(tp==AAND_ASSIGN)fprintf(FP1,"&=");
if(tp==AXOR_ASSIGN)fprintf(FP1,"^=");
if(tp==AOR_ASSIGN)fprintf(FP1,"|=");
if(tp==EMPTY_ASSIGN)fprintf(FP1,"=");
if(tp==RRADD_ASSIGN)fprintf(FP1,"+R=");
}
//reduction code generation.REDUXMUL and REDUXSUM
void statement::commcodeGen(FILE *FP1){

printf(" \nstmt line=%d  ",this->lineno);
static int n;
int xu,XX;
if(this->sttype==FUNCTION_EBLOCK_STMT ){
return;
}
if(this->barrier==1)fprintf(FP1,"//BARRIER %d %d\n",this->sttype,this->lineno);
if(this->sttype==SBLOCK_STMT){//"{"
}
if(this->sttype==EBLOCK_STMT ){ //"}"
}
if(this->sttype==FOR_EBLOCK_STMT ){//"}"
}
if(this->sttype==FOREACH_EBLOCK_STMT){ //"}"
}
if(this->sttype==RETURN_STMT){
}
if(this->sttype==GOTO_STMT){
}
if(this->sttype==CONTINUE_STMT){
}
if(this->sttype==EMPTY_STMT){
}
if(this->sttype==SWITCH_STMT){
this->expr1->printcode(this->expr1);
if(this->f1!=NULL)this->f1->commcodeGen(FP1);
}
if(this->sttype==CASE_STMT){
this->expr1->printcode(this->expr1);
}
if(this->sttype==DEFAULT_STMT){
this->expr1->printcode(this->expr1);
}
if(this->sttype==BREAK_STMT){
}
if(this->sttype==MACRO_STMT){
this->expr1->printcode(this->expr1);

if(this->f1!=NULL)this->f1->commcodeGen(FP1);
}
if(this->sttype==SECTIONS_STMT){
if(this->f1!=NULL){
this->f1->print();
}
}
if(this->sttype==SECTION_STMT){
if(this->f1!=NULL)this->f1->print();
}
if(this->sttype==SINGLE_STMT){
}
if(this->sttype==IF_STMT){
int ff;
char arr[100];
this->expr1->printcode1(this->expr1,arr);


if(this->f1!=NULL)this->f1->commcodeGen(FP1);
if(this->f2!=NULL){
if(this->f2->nodetype==TREE_IF){
}
this->f2->commcodeGen(FP1);
printf("END\n");
}
}
if(this->sttype==ASSIGN_STMT){//ASSIGNMENT STATEMENT
fprintf(stderr," line=%d \n",this->lineno);
if( this->stassign->lhs!=NULL){
int type1=this->stassign->lhs->expr_type;//LHS TYPE type1
int type=this->stassign->rhs->expr_type;//RHS TYPE type
 fprintf(stderr," type rhs=%d\n",type); 
	if( 	type1==STRUCTREF ){//LHS STRUCTRED

char arr1[200],arr2[200];
for(int i=0;i<200;i++)arr1[i]=arr2[i]='\0';
fprintf(stderr,"comm %s \n", this->stassign->lhs->name);
fprintf(stderr,"comm %s \n", this->stassign->lhs->lhs->name);
//fprintf(stderr,"comm %s \n", this->stassign->lhs->name);
if(this->stassign->lhs!=NULL &&this->stassign->lhs->lhs &&this->stassign->lhs->lhs->expr_type==VAR){

dir_decl *d1=this->stassign->lhs->lhs;
if(d1)fprintf(stderr,"%s \n",d1->name);
if(d1 && d1->parent!=NULL)d1=d1->parent;
if(d1)fprintf(stderr,"%s \n",d1->name);
char *name=this->stassign->lhs->rhs->name;
bool res=false;
if(d1 &&name)res=findextrpptyexist(d1,name); // IF STURCTREF and EXTRAPROPERTY
if(res){
res=checkconstant(this->stassign->rhs);//CHECK RHS CONSTANT CRCW
if(res){//IF NOT CONSTANT
fprintf(FP1," %s needs to be communicated\n",name);
CURR_CLUST_DEF->updatecopies[CURR_CLUST_CNT]=(char *)malloc(sizeof(char)*100);//COPY TO UPDATECOPIES
strcpy(CURR_CLUST_DEF->updatecopies[CURR_CLUST_CNT],name);
	if( 	type==STRUCTREF ){//RHS STRUCTREF

if(this->stassign->rhs!=NULL &&this->stassign->rhs->lhs &&this->stassign->rhs->lhs->expr_type==VAR){

dir_decl *d1=this->stassign->rhs->lhs->lhs;
if(d1)fprintf(stderr,"%s \n",d1->name);
if(d1 && d1->parent!=NULL)d1=d1->parent;
if(d1)fprintf(stderr,"%s \n",d1->name);
char *name=this->stassign->rhs->rhs->name;
bool res=false;
if(d1 &&name)res=findextrpptyexist(d1,name);
if(res){ //IF RHS EXTRA PROPERTY ADDTO UPDATERHS

CURR_CLUST_DEF->updaterhs[CURR_CLUST_CNT]=(char *)malloc(sizeof(char)*100);
strcpy(CURR_CLUST_DEF->updaterhs[CURR_CLUST_CNT],name);
}
}
}
  CURR_CLUST_CNT++;
}
}
}
//if(this->stassign->rhs!=NULL)this->stassign->rhs->printcode1(this->stassign->rhs,arr2);
}
if(type1==VAR){
dir_decl *d1=this->stassign->lhs->lhs;
fprintf(FP1,"// cluster comm %s \n",d1->name);
if(d1->stable!=NULL && (d1->stable)==(GLOBAL_TABLE.sym) && (TARGET==1|| TARGET==3 || TARGET==4 ))d1->gpu=true;
if(d1->stable!=NULL && (d1->stable)==(GLOBAL_TABLE.sym)) {
CURR_CLUST_DEF->updatecopies[CURR_CLUST_CNT]=(char *)malloc(sizeof(char)*100); //ADD THIRD ARGUMENT TO UPDATECOPIES
strcpy(CURR_CLUST_DEF->updatecopies[CURR_CLUST_CNT],d1->name);
  CURR_CLUST_CNT++;
}
}
if(type1==ARRREF && this->stassign->lhs->lhs->libdtype==COLLECTION_TYPE){
char arr1[200],arr2[200];
for(int i=0;i<200;i++)arr1[i]=arr2[i]='\0';
if(this->stassign->lhs!=NULL)this->stassign->lhs->printcode1(this->stassign->lhs,arr1);
print_assign_type(FP1,this->stassign->asstype);
if(this->stassign->rhs!=NULL)this->stassign->rhs->printcode1(this->stassign->rhs,arr2);

}


else{ 
//print_gpu_assign_stmt(this,1);
}
}  
else {

char arr1[200],arr2[200];
for(int i=0;i<200;i++)arr1[i]=arr2[i]='\0';
if(this->stassign->rhs==NULL&&this->sttype==ASSIGN_STMT){if(this->stassign->lhs!=NULL && this->stassign->lhs->name!=NULL){ this->stassign->lhs->printcode1(this->stassign->lhs,arr1);
}}
if(this->stassign->asstype==DOWHILE_EXPR){
this->stassign->rhs->printcode1(this->stassign->rhs,arr2);
}
if(this->stassign->asstype!=WHILE_EXPR && this->stassign->asstype!=-1){
}
if(this->stassign->rhs!=NULL && this->stassign->rhs->expr_type!=FUNCALL){
int repli=0;
tree_expr *Tu1;
if(this->stassign->lhs!=NULL && this->stassign->lhs->name!=NULL){ 
this->stassign->lhs->printcode1(this->stassign->lhs,arr1);
Tu1=this->stassign->lhs;
}
}
//print_assign_type(FP1,this->stassign->asstype);

}
if(this->stassign->rhs->expr_type==FUNCALL&&checklibfunction(this->stassign->rhs->name)>=0){//LIBRARY FUNCTION CALL
fprintf(FP1,"//libfunction call %s\n",this->stassign->rhs->name);
tree_expr *expr=this->stassign->rhs->arglist->rhs;
if(expr->expr_type==STRUCTREF){
fprintf(stderr,"structref\n");
dir_decl *d1=expr->lhs->lhs;

fprintf(stderr,"%s\n",d1->name);
if(d1&&d1->parent)d1=d1->parent;
fprintf(stderr,"%s\n",d1->name);
if(d1&&d1->parent)d1=d1->parent;
fprintf(stderr,"%s\n",d1->name);
char *name=expr->rhs->name;
bool res=false;
if(d1 &&name)res=findextrpptyexist(d1,name);//IF EXTRA PROPERTY
if(res){
res=checkconstant(this->stassign->rhs);//checks whether rhs is const, it const return false
if(res){
fprintf(stderr," %s needs to be communicated\n",name);
CURR_CLUST_DEF->updatecopies[CURR_CLUST_CNT]=(char *)malloc(sizeof(char)*100);//ADD EXTRA PROPERTY TO UPDATECOPIES
strcpy(CURR_CLUST_DEF->updatecopies[CURR_CLUST_CNT],name);
  CURR_CLUST_CNT++;
if(!strcmp(this->stassign->rhs->name,"MIN")){
assign_stmt *ass=this->stassign->rhs->arglist;
while(ass->next)ass=ass->next;
if(ass && ass->rhs && ass->rhs->name)fprintf(FP1,"// clustercomm.c LIB FUNCTION MIN 3rd ARG %s to be communicated\n",ass->rhs->name);
CURR_CLUST_DEF->updatecopies[CURR_CLUST_CNT]=(char *)malloc(sizeof(char)*100); //ADD THIRD ARGUMENT TO UPDATECOPIES
strcpy(CURR_CLUST_DEF->updatecopies[CURR_CLUST_CNT],ass->rhs->name);
  CURR_CLUST_CNT++;
dir_decl *d1=GLOBAL_TABLE.findsymbol(ass->rhs->name);
if(d1 &&(TARGET==3|| TARGET==1 || TARGET==4))d1->gpu=1;



}
}

}
}
}
}
fprintf(stderr,"ass over%d \n",this->lineno);
if(this->sttype==WHILE_STMT){
char temp[100];
for(int i=0;i<100;i++)temp[i]='\0';
this->expr1->printcode1(this->expr1,temp);
if(this->stassign!=NULL){this->stassign->rhs->printcode(this->stassign->rhs);}
}
if(this->sttype==DOWHILE_STMT){
}
if(this->sttype==DOWHILEEXPR_STMT){
this->expr1->printcode(this->expr1);
}
if(this->sttype==FOREACH_STMT ){
int it=this->itr;
dir_decl *d2=this->expr2->lhs;
if(1 ){
if(this->stassign!=NULL) {char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
this->stassign->rhs->printcode1(this->stassign->rhs,temp);
fprintf(stderr,"foreach rhs\n");
if(this->stassign->rhs->expr_type==FUNCALL)fprintf(stderr,"FUNCALL\n");
}
}
else
 {
if(d2!=NULL &&d2->libdtype==GRAPH_TYPE){
}

}
}
if(this->sttype==FOR_STMT){
}



if(this->next!=NULL)this->next->commcodeGen(FP1);
}



