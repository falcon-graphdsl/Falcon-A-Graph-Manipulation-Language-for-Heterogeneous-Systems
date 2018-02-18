extern char *libpartitionfunctions[],*libfunctions[];
extern bool findextrpptyexist(dir_decl *,char *name);
int checklibfunction(char *name){
fprintf(FP1,"// checklibfub %s \n",name);
for(int i=0;i<LIBFUNCSIZE;i++){
if(!strcmp(libfunctions[i],name)&&(strlen(name)==strlen(libfunctions[i]))){
fprintf(FP1,"// LIBFUN %s \n",libfunctions[i]);
return i;
break;
}
}
return -1;

}
int findextrpptydatatype(dir_decl *d1, char *name){
struct extra_ppts *ep=d1->ppts; 
while(ep!=NULL){
if(!strcmp(ep->name,name)&&(strlen(ep->name)==strlen(name)))
{
fprintf(FP1,"//EXTRA DATATYPE %s\n",name);
if(ep->t1){ fprintf(FP1, "//%d %d \n", ep->t1->datatype,ep->t1->libdatatype);
return ep->t1->datatype;
}
}
ep=ep->next;
}
return -1;

}
bool checkconstant(tree_expr *expr){
int type=expr->nodetype;
if(type==TREE_INT )return false;
if(type==TREE_FLOAT)return false;
if(type==TREE_BOOL)return false;
if(expr->expr_type==STRUCTREF){
fprintf(stderr,"structref access\n");
dir_decl *d1=expr->lhs->lhs;
if(d1)fprintf(stderr,"%s \n",d1->name);
if(d1 && d1->parent!=NULL)d1=d1->parent;
if(d1)fprintf(stderr,"%s \n",d1->name);
char *name=expr->rhs->name;
bool res=false;
if(d1 &&name)res=findextrpptyexist(d1,name);
fprintf(stderr,"rhs extra property access\n");
return true;
}
fprintf(stderr,"not extrappty or constant %d\n",type);
return true;
}
int findextrpptylibdatatype(dir_decl *d1, char *name){
struct extra_ppts *ep=d1->ppts; 
while(ep!=NULL){
if(!strcmp(ep->name,name)&&(strlen(ep->name)==strlen(name)))return ep->t1->libdatatype;
ep=ep->next;
}
return -1;

}
bool findextrpptyexist(dir_decl *d1, char *name){
struct extra_ppts *ep=d1->ppts; 
while(ep!=NULL){
if(!strcmp(ep->name,name)&&(strlen(ep->name)==strlen(name)))return true;
ep=ep->next;
}
return false;
}

int LIBFUNCSIZE=10;
extern fun(FILE *FP1,char *, int ,char *,char *[],int ,char *,int ,char *);
printpartitioncodegpugpu(statement *t1){
statement *backup=t1;
t1->ker=1;
Gkernel=1;
tree_decl_stmt *df1=t1->stdecl->dirrhs->params;
while(df1!=NULL){
df1->dirrhs->gpu=1;
df1->dirrhs->gpugpu=t1->gpugpu;
df1=df1->next;
}
CPUGPU=1;
dir_decl *dd=t1->stdecl->dirrhs;
int flag=0;
char *xx=malloc(sizeof(char)*100);
 sprintf(xx,"%s",t1->stdecl->dirrhs->name);
for(std::map<char *,statement *>::reverse_iterator it1=fnames.rbegin();it1!=fnames.rend();it1++){//foreach function declaration
if(!strcmp(it1->first,xx)&&(strlen(it1->first)==strlen(xx))){
statement *tt=it1->second;
fprintf(FP1,"// %s ", it1->first);
if(tt->foreachflag==1){flag=1;break;}
}
}
EXTRA_CRBRK=1;
if(flag==1){
fprintf(FP1,"//inside gpugpu codegen\n");
EXTRA_CRBRK=1;
FUN_EBLOCK_RET=1;
fprintf(FP1,"__global__ void ");
if(dd->params!=NULL){
tree_decl_stmt *df1=dd->params;
while(df1!=NULL){
df1->dirrhs->gpu=1;
df1->dirrhs->cpugpu=2;
if(df1->dirrhs->libdtype==POINT_TYPE)df1->dirrhs->forit=1;
df1=df1->next;
}
t1->stdecl->dirrhs->printcode(0);
fprintf(FP1,"{\n%s\n ",threadstmt);
fprintf(FP1,"if(id <falcnpoints){\n");
}
tree_decl_stmt *df1=t1->stdecl->dirrhs->params;
while(df1!=NULL){
if(df1->dirrhs->libdtype==POINT_TYPE){
strcpy(df1->dirrhs->name,"id");
df1->dirrhs->extra_name1=malloc(sizeof(char)*100);
strcpy(df1->dirrhs->extra_name1,"id");
if(df1->dirrhs->parent!=NULL){
fprintf(FP1,"//%s \n",df1->dirrhs->parent->name);
}else fprintf(FP1,"//PARENT NULL\n");
break;
}
df1=df1->next;
}
t1->next->next->codeGen(FP1);
EXTRA_CRBRK=1;
GLOBAL_ID_FLAG=1;
t1=backup;
strcat(t1->stdecl->dirrhs->name,"gpu");
df1=t1->stdecl->dirrhs->params;
while(df1!=NULL){
//fprintf(FP1,"//%d ",df1->dirrhs->libdtype);
if(df1->dirrhs->libdtype==GRAPH_TYPE || df1->dirrhs->libdtype==POINT_TYPE){
df1->dirrhs->gpu=1;
if(df1->dirrhs->libdtype==POINT_TYPE)df1->dirrhs->forit=1;
}
df1=df1->next;
}
FUN_EBLOCK_RET=0;
}
fprintf(FP1,"//inside gpugpu codegen\n");
if(flag!=1)EXTRA_CRBRK=1;
FUN_EBLOCK_RET=0;
fprintf(FP1,"__global__ void ");
df1=t1->stdecl->dirrhs->params;
while(df1!=NULL){
df1->dirrhs->gpu=1;
if(df1->dirrhs->libdtype==POINT_TYPE){
if(df1->dirrhs->libdtype==POINT_TYPE)df1->dirrhs->forit=1;
strcpy(df1->dirrhs->name,"id");
df1->dirrhs->extra_name1=malloc(sizeof(char)*100);
df1->dirrhs->extra_name=malloc(sizeof(char)*100);
if(flag==1)strcpy(df1->dirrhs->extra_name1,"id+graph.start_point");
if(flag==0)strcpy(df1->dirrhs->extra_name1,"id");
break;
}
//}
df1=df1->next;
}
t1->stdecl->dirrhs->printcode(0);
fprintf(FP1,"{\n%s\n ",threadstmt);
fprintf(FP1,"if(id <falcnpoints){\n");
t1->next->next->codeGen(FP1);
}


printpartitioncodecpugpu(statement **tt1){
statement *t1=*tt1;
statement *backup=t1;
t1->ker=0;
Gkernel=0;
tree_decl_stmt *df1=t1->stdecl->dirrhs->params;
while(df1!=NULL){
//if(df1->dirrhs->libdtype==GRAPH_TYPE || df1->dirrhs->libdtype==POINT_TYPE){
df1->dirrhs->gpu=0;
df1->dirrhs->gpugpu=0;
fprintf(FP1,"//xxSET GPU=1 %s %d %d\n",df1->dirrhs->name,Gkernel,df1->dirrhs->gpugpu);
//}
df1=df1->next;
}
CPUGPU=1;
dir_decl *dd=t1->stdecl->dirrhs;
int flag=0;
char *xx=malloc(sizeof(char)*100);
 sprintf(xx,"%s",t1->stdecl->dirrhs->name);
for(std::map<char *,statement *>::reverse_iterator it1=fnames.rbegin();it1!=fnames.rend();it1++){//foreach function declaration
if(!strcmp(it1->first,xx)&&(strlen(it1->first)==strlen(xx))){
statement *tt=it1->second;
if(tt->foreachflag==1){flag=1;break;}
}
}
t1->ker=0;
FUN_EBLOCK_RET=1;
if(dd->params!=NULL){
tree_decl_stmt *df1=dd->params;
while(df1!=NULL){
df1->dirrhs->gpu=0;
//df1->dirrhs->gpugpu=0;
//df1->dirrhs->cpugpu=0;
if(df1->dirrhs->libdtype==POINT_TYPE){
strcpy(df1->dirrhs->name,"id");
df1->dirrhs->extra_name1=malloc(sizeof(char)*100);
strcpy(df1->dirrhs->extra_name1,"id");
}
df1=df1->next;
}
fprintf(FP1,"void ");
t1->stdecl->dirrhs->printcode(0);
}
EXTRA_CRBRK=0;
t1->next->codeGen(FP1);
t1->ker=1;
Gkernel=1;
EXTRA_CRBRK=1;
if(flag==1)GLOBAL_ID_FLAG=1;
df1=t1->stdecl->dirrhs->params;
strcat(t1->stdecl->dirrhs->name,"gpu");
fprintf(FP1,"__global__ void ");
while(df1!=NULL){
//if(df1->dirrhs->libdtype==GRAPH_TYPE || df1->dirrhs->libdtype==POINT_TYPE){
df1->dirrhs->gpu=1;
if(df1->dirrhs->libdtype==POINT_TYPE){
strcpy(df1->dirrhs->name,"id");
df1->dirrhs->extra_name1=malloc(sizeof(char)*100);
if(df1->dirrhs->libdtype==POINT_TYPE)df1->dirrhs->forit=1;
//strcpy(df1->dirrhs->extra_name1,"id+graph.start_point");
if(flag==1)strcpy(df1->dirrhs->extra_name1,"id+graph.start_point");
if(flag==0)strcpy(df1->dirrhs->extra_name1,"id");
strcpy(df1->dirrhs->name,"id+graph.start_point");
}
//}
df1=df1->next;
}
t1->stdecl->dirrhs->printcode(0);
FUN_EBLOCK_RET=0;
fprintf(FP1,"\n{\n%s\n if(id <falcnpoints){\n",threadstmt);
t1->next->next->codeGen(FP1);
}
print_partitioned_condition(tree_expr *expr){
char arr[100];
char arr1[100],arrcpu[100];
for(int i=0;i<100;i++)arr[i]='\0';
for(int i=0;i<100;i++)arr1[i]='\0';
for(int i=0;i<100;i++)arrcpu[i]='\0';
tree_expr *expr1=expr->lhs;
if(expr1->expr_type==STRUCTREF && expr1->lhs && expr1->lhs->expr_type==VAR){

expr1->rhs->printcode1(expr1->rhs,arr);
expr->rhs->printcode1(expr->rhs,arr1);
dir_decl *d1= expr1->lhs->lhs;
if(d1->cpugpu==true){
sprintf(arrcpu,"&&(((struct struct_%s *)(%s.hostparts[0].extra))->%s==%s)",d1->name,d1->name,arr,arr1);
}
fprintf(FP1,"struct struct_%s tmpif;\n",d1->name);
fprintf(FP1,"cudaMemcpy(&tmpif,((struct struct_%s *)(%s.devparts[0].extra)),sizeof(struct struct_%s),cudaMemcpyDeviceToHost);\n",d1->name,d1->name,d1->name);
fprintf(FP1,"int iftmp1=0,iftmp2=0;\n");
fprintf(FP1,"cudaMemcpy(&iftmp1,&(tmpif.%s),sizeof(int),cudaMemcpyDeviceToHost);\n",arr);
if(d1->gpugpu>1){fprintf(FP1,"cudaMemcpy(&tmpif,((struct struct_%s *)(%s.devparts[1].extra)),sizeof(struct struct_%s),cudaMemcpyDeviceToHost);\n",d1->name,d1->name,d1->name);
fprintf(FP1,"cudaMemcpy(&iftmp2,&(tmpif.%s),sizeof(int),cudaMemcpyDeviceToHost);\n",arr);
//fprintf(FP1,"%s \n",arr);
}
expr->lhs->expr_type=VAR;
expr->lhs->name=malloc(sizeof(char)*100);
//expr->lhss=NULL;
sprintf(expr->lhs->name,"(iftmp1==%s)",arr1);
expr->rhs->expr_type=VAR;
expr->expr_type=AND;
expr->rhs->name=malloc(sizeof(char)*100);
sprintf(expr->rhs->name,"(iftmp2==%s)",arr1);
fprintf(FP1,"if(%s&& %s %s )",expr->lhs->name,expr->rhs->name,arrcpu);

}

}
extern int ftempcnt;
print_partitioned_assign(statement *t1){
dir_decl *dd1=((dir_decl *)(t1->stassign->lhs->lhs->lhs));
if(t1->stassign->lhs->lhs->lhs->lhs){
dd1=t1->stassign->lhs->lhs->lhs->lhs;
//fprintf(FP1,"//  var is here %s\n",t1->stassign->lhs->lhs->lhs->lhs->name);
}
if(dd1->extra_name==NULL){
dd1->extra_name=malloc(sizeof(char)*100 );
sprintf(dd1->extra_name,"struct struct_%s",dd1->name);
}
fprintf(FP1,"/*XX*/%s ftemp%d;\n",dd1->extra_name,++ftempcnt);
int kk=dd1->gpugpu;
if(kk ==0 && dd1->cpugpu)kk=1;
for(int i=0;i<kk;i++){
fprintf(FP1,"cudaMemcpy(&ftemp%d,%s.devparts[%d].extra,sizeof(%s),cudaMemcpyDeviceToHost);\n",ftempcnt,dd1->name,i,dd1->extra_name);
char arr[100];
for(int ii=0;ii<100;ii++)arr[ii]='\0';
t1->stassign->rhs->printcode1(t1->stassign->rhs,arr);
printexpr_type(t1->stassign->lhs,0);
int epdtype=findextrpptydatatype(dd1, t1->stassign->lhs->rhs->name);
int eplibdtype=findextrpptylibdatatype(dd1, t1->stassign->lhs->rhs->name);
fprintf(FP1,"%s asstmp%d=%s;\n",dtypenames[epdtype],ftempcnt+i,arr);
for(int ii=0;ii<100;ii++)arr[ii]='\0';
t1->stassign->lhs->rhs->printcode1(t1->stassign->lhs->rhs,arr);
tree_expr *tt=t1->stassign->lhs->rhs;
if(tt->earr_list==NULL)
fprintf(FP1,"cudaMemcpy(&(ftemp%d.%s[0]),&asstmp%d,sizeof(%s),cudaMemcpyHostToDevice);\n",ftempcnt,arr,ftempcnt+i,dtypenames[epdtype]);
else {
//fprintf(FP1,"//arr not null\n");//for G_P_TYPE [0] is extra,why?
fprintf(FP1,"cudaMemcpy(&(ftemp%d.%s),&asstmp%d,sizeof(%s),cudaMemcpyHostToDevice);\n",ftempcnt,arr,ftempcnt+i,dtypenames[epdtype]);
}
//if( eplibdtype!=G_P_TYPE)break;
}
ftempcnt++;

if(dd1->cpugpu==true){
char arr[100];
for(int ii=0;ii<100;ii++)arr[ii]='\0';
t1->stassign->lhs->rhs->printcode1(t1->stassign->lhs->rhs,arr);
fprintf(FP1,"((struct struct_%s *)(%s.hostparts[0].extra))->%s[0]=",dd1->name,dd1->name,arr);
for(int ii=0;ii<100;ii++)arr[ii]='\0';
t1->stassign->lhs->rhs->printcode1(t1->stassign->rhs,arr);
fprintf(FP1,"%s;\n",arr);
}
}
codegenupdatefunction(dir_decl *d1,tree_expr *expr){
tree_expr *t1=expr->rhs;
//fprintf(FP1,"//update  %d \n",t1->expr_type);
tree_expr *funname= t1->arglist->rhs;
statement *update_decl;

d1->update_fun=malloc(sizeof(char)*100);
strcpy(d1->update_fun,funname->name);
fprintf(FP1,"//funname %s\n",funname->name);
fprintf(FP1,"//funname %s %s\n",funname->name,d1->update_fun);
for (std::map<char *,statement *>::iterator it1=fnames.begin(); it1!=fnames.end(); ++it1){
if (!strcmp(funname->name, it1->first)&&(strlen(funname->name)==strlen(it1->first))){

fprintf(FP1,"//found funname %s\n",funname->name);
update_decl= it1->second;
update_decl->noprint=false;
break;
}
}

fprintf(FP,"__global__ void %s(GGraph %s0 ,GGraph %s1,int npoints,int FALCX){\n",funname->name, d1->name,d1->name);
fprintf(FP,"%s \n",threadstmt);
fprintf(FP,"if (id < npoints)\n");
tree_decl_stmt *df1=update_decl->stdecl->dirrhs->params;
dir_decl *orig=df1->dirrhs;
df1=df1->next;
dir_decl *incom=df1->dirrhs;
dir_decl *par1,*par2;
par1=new dir_decl();
par2=new dir_decl();
par1->name=(char *)malloc(sizeof(char)*100);
par2->name=(char *)malloc(sizeof(char)*100);
par1->extra_name=(char *)malloc(sizeof(char)*100);
par2->extra_name=(char *)malloc(sizeof(char)*100);
incom->extra_name1=(char *)malloc(sizeof(char)*100);
orig->extra_name1=(char *)malloc(sizeof(char)*100);
strcpy(par1->extra_name,d1->extra_name);
strcpy(par2->extra_name,d1->extra_name);
strcpy(incom->extra_name1,"id");
strcpy(orig->extra_name1,"id");
par1->libdtype=GRAPH_TYPE;
par2->libdtype=GRAPH_TYPE;
sprintf(par1->name,"%s0",d1->name);
sprintf(par2->name,"%s1",d1->name);
par1->ppts=par2->ppts=d1->ppts;
incom->parent=par2;
orig->parent=par1;
FUN_EBLOCK_RET=1;
//fprintf(FP," %s %s \n",orig->name,incom->name);
update_decl->next->codeGen(FP);
fprintf(FP,"//RETURN \n");
FUN_EBLOCK_RET=1;
fprintf(FP,"\n}\n");
fprintf(FP,"__global__ void %stemp(GGraph %s0 ,GGraph %s1,int npoints,int FALCX){\n",funname->name, d1->name,d1->name);
fprintf(FP,"%s \n",threadstmt);
fprintf(FP,"if (id < npoints)\n");
df1=update_decl->stdecl->dirrhs->params;
orig=df1->dirrhs;
df1=df1->next;
incom=df1->dirrhs;
par1=new dir_decl();
par2=new dir_decl();
par1->name=(char *)malloc(sizeof(char)*100);
par2->name=(char *)malloc(sizeof(char)*100);
par1->extra_name=(char *)malloc(sizeof(char)*100);
par2->extra_name=(char *)malloc(sizeof(char)*100);
incom->extra_name1=(char *)malloc(sizeof(char)*100);
orig->extra_name1=(char *)malloc(sizeof(char)*100);
sprintf(par1->extra_name,"%s",d1->extra_name);
sprintf(par2->extra_name,"%s",d1->extra_name);
strcpy(incom->extra_name1,"id");
strcpy(orig->extra_name1,"id");
par1->libdtype=GRAPH_TYPE;
par2->libdtype=GRAPH_TYPE;
sprintf(par1->name,"%s0",d1->name);
sprintf(par2->name,"%s1",d1->name);
par1->ppts=par2->ppts=d1->ppts;
incom->parent=par2;
orig->parent=par1;
FUN_EBLOCK_RET=1;
//fprintf(FP1," %s %s \n",orig->name,incom->name);
update_decl->next->codeGen(FP);
FUN_EBLOCK_RET=0;
fprintf(FP,"\n}\n");
return;
}
void genpartitioncopycodecpugpu(tree_expr *expr, dir_decl *d1){
fprintf(FP,"GGraph %sgpucopy;\nHGraph %scopy;\n",d1->name,d1->name);
fprintf(FP,"void Copyproperties(GGraph &gpuorig,GGraph &gpucopy,HGraph &cpuorig,HGraph &cpucopy){\n");
struct extra_ppts *ppts=d1->ppts;

fprintf(FP,"cudaMalloc((void **)(&(gpucopy.extra)),sizeof( %s ));\n",d1->extra_name);
fprintf(FP," %s tmp;\ncudaMemcpy(&tmp,gpucopy.extra,sizeof( %s),cudaMemcpyDeviceToHost);\n",d1->extra_name,d1->extra_name);
int flag=0;
while(ppts!=NULL){
flag=0;
for(int i=0;i<updatecopiescnt;i++){
if(!strcmp(updatecopies[i],ppts->name)){
flag=1;
break;
}
}
if(flag==1){
if(ppts->t1->libdatatype==P_P_TYPE){
fprintf(FP,"cudaMalloc(&(tmp.%s),sizeof(%s)*cpuorig.npoints);\n",ppts->name,ppts->t1->name);
fprintf(FP,"cudaMemcpy(tmp.%s,((  %s *)(cpuorig.extra))->%s,sizeof(%s)*cpuorig.npoints,cudaMemcpyHostToDevice);\n",ppts->name,d1->extra_name,ppts->name,ppts->t1->name);


}
}
ppts=ppts->next;
}
ppts=d1->ppts;
fprintf(FP,"cpucopy.extra=malloc(sizeof( %s ));\n",d1->extra_name);
fprintf(FP,"\ncudaMemcpy(&tmp,gpuorig.extra,sizeof( %s),cudaMemcpyDeviceToHost);\n",d1->extra_name,d1->extra_name);
while(ppts!=NULL){
flag=0;
for(int i=0;i<updatecopiescnt;i++){
if(!strcmp(updatecopies[i],ppts->name)){
flag=1;
break;
}
}
if(flag==1){
if(ppts->t1->libdatatype==P_P_TYPE){
fprintf(FP,"( ( %s *)(cpucopy.extra))->%s=(%s *)malloc(sizeof(%s)*gpuorig.npoints);\n",d1->extra_name,ppts->name,ppts->t1->name,ppts->t1->name);
fprintf(FP,"cudaMemcpy(((  %s *)(cpucopy.extra))->%s,tmp.%s,sizeof(%s)*cpuorig.npoints,cudaMemcpyDeviceToHost);\n",d1->extra_name,ppts->name,ppts->name,ppts->t1->name);


}
}
ppts=ppts->next;
}
fprintf(FP,"}\n");
}
genpartitioncodegpu(tree_expr *expr, dir_decl *d1){
if(!strcmp(expr->rhs->name,"makePartition")&&strlen(expr->rhs->name)==strlen("makePartition")){
if(d1->cpugpu==true)fprintf(FP1,"alloc_extra_%s(%s,%s.npoints);\n",d1->name,d1->name,d1->name);
if(d1->extra_readfun!=NULL){
fprintf(FP1,"%s\n",d1->extra_readfun);
}
fprintf(FP1,"%s.cloneGPUpeer(%s,0);\n",expr->lhs->name,expr->lhs->name);
int pp=d1->gpugpu;
if(pp==0 &&d1->cpugpu)pp=1;
for(int kk=0;kk<pp;kk++){
struct extra_ppts *ppts=d1->ppts;
int flag=0;
while(ppts!=NULL){
if(ppts->t1->libdatatype==G_P_TYPE){
if(flag==0){
extemp++;
char *namevar=malloc(sizeof(char)*100);
sprintf(namevar,"%s.devparts[%d]",d1->name,kk);
fun(FP1,namevar,kk+1,d1->extra_name,cpy,extemp,ppts->name,0,d1->name);
flag=1;

}
}
ppts=ppts->next;
}
if(flag==1) {
fprintf(FP1,"%scudaMemcpy(%s.devparts[%d].extra,&temp%d,sizeof(%s),cudaMemcpyHostToDevice)%s%d%s\n%sflag=1;",cpy[0],d1->name,kk,extemp,d1->extra_name,cpy[1],errcnt++,cpy[2],d1->name);

}
}
extemp++;
for(int kk=0;kk<pp;kk++){
fprintf(FP1,"cudaSetDevice(%d);\nalloc_extra_%s(%s.devparts[%d],%d,%s.npoints);\n",kk+1,expr->lhs->name,expr->lhs->name,kk,kk+1,expr->lhs->name);
}
//fprintf(FP1,"cudaSetDevice(2);\nalloc_extra%s(%s.devparts[1],1,%s.npoints);\n",expr->lhs->name,expr->lhs->name,expr->lhs->name);
return 1;
}
if(!strcmp(expr->rhs->name,"pptyUpdateFn")&&strlen(expr->rhs->name)==strlen("pptyUpdateFn")){
struct extra_ppts *ep=d1->ppts;
assign_stmt *ass=expr->rhs->arglist;
while(ep!=NULL){
if(!strcmp(ass->rhs->name,ep->name)&&strlen(ass->rhs->name)==strlen(ep->name)){
fprintf(FP1,"//mathced %s ",ass->rhs->name);
ass=ass->next;
for(int i=0;i<LIBFUNCSIZE;i++){
if(!strcmp(libfunctions[i],ass->rhs->name)&&(strlen(ass->rhs->name)==strlen(libfunctions[i]))){
fprintf(FP1,"%s \n",libfunctions[i]);
ep->partfun=i;
break;
}
}
break;

}
ep=ep->next;
}
return 1;
}
if(!strcmp(expr->rhs->name,"updatePartition")&&strlen(expr->rhs->name)==strlen("updatePartition")){
fprintf(FP,"__global__ void modi%sgpu(GGraph graph1,GGraph graph2,int FALCX,int npoints ){\n",d1->name);
fprintf(FP,"%s \n",threadstmt);
fprintf(FP,"if (id< npoints){\n");
struct extra_ppts *ep=d1->ppts; 
while(ep!=NULL){
if(ep->t1->libdatatype!=G_P_TYPE){
if(ep->partfun>=0&&ep->partfun<=1){
fprintf(FP,"if((( %s *)(graph1.extra))->%s[id] %s ((%s *)(graph2.extra))->%s[id]){\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun],d1->extra_name,ep->name);
fprintf(FP,"(( %s *)(graph1.extra))->%s.[id]= ((%s *)(graph.extra))->%s[id];\n}\n",d1->extra_name,ep->name,d1->extra_name,ep->name);
}
}
if(ep->partfun>=2 && ep->partfun<=4){

fprintf(FP,"(( %s *)(graph1.extra))->%s[id] %s;\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun]);
}
if(ep->partfun>=4&&ep->partfun<=9){
fprintf(FP,"(( %s *)(graph1.extra))->%s[id] %s ((%s *)(graph2.extra))->%s[id];\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun],d1->extra_name,ep->name);
}
ep=ep->next;
}

fprintf(FP,"\n}\n}\n");
 ep=d1->ppts; 
int tt=++Temp;
fprintf(FP1,"#pragma omp parallel sections \n{\n #pragma omp section \n{\n \nmodi%sgpu<<<%s.npoints/prop%d.maxThreadPerBlock+1,prop%d.maxThreadsPerBlock>>>(%s.devparts[0],%s.devparts[1],0,%s.npoints);\n\n",d1->name,d1->name,d1->dev_no,d1->dev_no,d1->name,d1->name,d1->name);
fprintf(FP1,"\n}\n#pragma omp section \n{\nmodi%sgpu<<<%s.npoints/prop%d.maxThreadPerBlock+1,prop%d.maxThreadsPerBlock>>>(%s.devparts[1],%s.devparts[0],0,%s.npoints);\n}\n}\n",d1->name,d1->name,d1->dev_no,d1->dev_no,d1->name,d1->name,d1->name);

fprintf(FP1,"\n ");
return 1;
}
return 0;
}
genpartitioncodecpu(tree_expr *expr, dir_decl *d1){
if(!strcmp(expr->rhs->name,"makePartition")&&strlen(expr->rhs->name)==strlen("makePartition")){
fprintf(FP1,"%s.hostparts[0].extra=(struct struct_%s *)malloc(sizeof(struct struct_%s));\n",d1->name,d1->name,d1->name);
fprintf(FP1,"alloc_extra_%s(%s.hostparts[0],0);\n",expr->lhs->name,expr->lhs->name);
int pp=1;
for(int kk=0;kk<pp;kk++){
struct extra_ppts *ppts=d1->ppts;
int flag=0;
while(ppts!=NULL){
if(ppts->t1->libdatatype==G_P_TYPE){
if(flag==0){
extemp++;
char *namevar=malloc(sizeof(char)*100);
fprintf(FP1,"((struct struct_%s *)(%s.hostparts[0].extra))->nchanged=((struct struct_%s *)(%s.extra))->nchanged;\n",d1->name,d1->name,d1->name,d1->name);
flag=1;

}
}
ppts=ppts->next;
}
}
return 1;
}
if(!strcmp(expr->rhs->name,"pptyUpdateFn")&&strlen(expr->rhs->name)==strlen("pptyUpdateFn")){
struct extra_ppts *ep=d1->ppts;
assign_stmt *ass=expr->rhs->arglist;
while(ep!=NULL){
if(!strcmp(ass->rhs->name,ep->name)&&strlen(ass->rhs->name)==strlen(ep->name)){
fprintf(FP1,"//mathced %s ",ass->rhs->name);
ass=ass->next;
for(int i=0;i<LIBFUNCSIZE;i++){
if(!strcmp(libfunctions[i],ass->rhs->name)&&(strlen(ass->rhs->name)==strlen(libfunctions[i]))){
fprintf(FP1,"%s \n",libfunctions[i]);
ep->partfun=i;
break;
}
}
break;

}
ep=ep->next;
}
return 1;
}
if(!strcmp(expr->rhs->name,"updatePartition")&&strlen(expr->rhs->name)==strlen("updatePartition")){
//fprintf(FP,"/* \n");
fprintf(FP1,"/*HERE*/ \n");
fprintf(FP,"__global__ void modi%sgpu(GGraph graph,GGraph graph1,int FALCX ){\n",d1->name);
fprintf(FP,"%s \n",threadstmt);
fprintf(FP,"if (id< grap.inbuffsize){\n");
struct extra_ppts *ep=d1->ppts; 
while(ep!=NULL){
if(ep->t1->libdatatype!=G_P_TYPE){
if(ep->partfun>=0&&ep->partfun<=1){
fprintf(FP,"if((( %s *)(graph.extra))->%s[id] %s ((%s *)(graph1.extra))->%s[id]){\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun],d1->extra_name,ep->name);
fprintf(FP,"(( %s *)(graph.extra))->%s[id]= ((%s *)(graph1.extra))->%s[id];\n}\n",d1->extra_name,ep->name,d1->extra_name,ep->name);
}
}
if(ep->partfun>=2 && ep->partfun<=4){

fprintf(FP,"(( %s *)(graph.extra))->%s[id] %s;\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun]);
}
if(ep->partfun>=4&&ep->partfun<=9){
fprintf(FP,"(( %s *)(graph.extra))->%s[id] %s ((%s *)(graph1.extra))->%s[id];\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun],d1->extra_name,ep->name);
}
ep=ep->next;
}

fprintf(FP,"\n}\n}\n");
//fprintf(FP,"/* \n");
fprintf(FP," void modi%s(int id,HGraph &graph,HGraph graph1){\n",d1->name);
 ep=d1->ppts; 
while(ep!=NULL){
if(ep->t1->libdatatype!=G_P_TYPE){
if(ep->partfun>=0 &&ep->partfun<=1){

fprintf(FP,"if((( %s *)(graph.extra))->%s[id] %s ((%s *)(graph1.extra))->%s[id]){\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun],d1->extra_name,ep->name);
fprintf(FP,"(( %s *)(graph.extra))->%s[id]= ((%s *)(graph1.extra))->%s[id];\n}\n",d1->extra_name,ep->name,d1->extra_name,ep->name);
}
if(ep->partfun>=2 && ep->partfun<=4){

fprintf(FP,"(( %s *)(graph.extra))->%s[id] %s;\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun]);
}
if(ep->partfun>=2 && ep->partfun<=9){
fprintf(FP,"(( %s *)(graph.extra))->%s[id] %s ((%s *)(graph1.extra))->%s[id];\n",d1->extra_name,ep->name,libpartitionfunctions[ep->partfun],d1->extra_name,ep->name);
}
}
ep=ep->next;
}

fprintf(FP,"\n}\n");

fprintf(FP1,"\n// ADD CODE HERE\n ");
fprintf(FP1,"CopyProperties(%s.devparts[0],%sgpucopy,%s.hostparts[0],%scopy);\n",d1->name,d1->name,d1->name,d1->name);
fprintf(FP1,"cudaSetDevice(1);\nmodi%sgpu<<< %s.npoints/1024+1,1024>>>(%s.devparts[0],%sgpucopy);\n",d1->name,d1->name,d1->name,d1->name);
fprintf(FP1,"#pragma omp parallel for num_threads(12)\n for(int i=0;i<%s.npoints;i++)modi%s(i,%s.hostparts[0],%scopy);\ncudaDeviceSynchronize();\n",d1->name,d1->name,d1->name,d1->name);
return 1;
}
return 0;
}
