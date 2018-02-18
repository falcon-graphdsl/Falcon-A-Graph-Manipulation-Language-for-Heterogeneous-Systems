#include <symtabold.h>
#include<stdlib.h>
#include "externs.h"
#include "treeprintcode1type.c"
char  *treenodetypestr[]={"tree_node ","tree_id ","tree_field ","tree_mapaccess ","tree_idlist","tree_typedecl","tree_argdecl","tree_procdef","tree_expr","tree_expr_rdc","tree_expr_builtin","tree_expr_foreign","tree_expr_mapaccess","tree_sent","tree_sentblock", "tree_assign", "tree_vardecl","tree_foreach","tree_for","tree_arrref","tree_dowhile","tree_switch","tree_if","tree_while","tree_return","tree_call","tree_foreign","tree_nop","tree_int","tree_float","tree_enumconst","tree_string","tree_bool"};
char *libdatatype_string[]={//stores library datatype of variable(dir_decl)
"graph_type"/*0*/,"edge_type"/*1*/,"point_type"/*2*/,"set_type"/*3*/,"collection_type"/*4*/,"g_p_type"/*5*/,"e_p_type"/*6*/,"p_p_type"/*7*/,"iterator_type","lint_type"};

char *exprtype_string[]={"+","-","*","%","&","|","^","&&","||","~",
"+","*","-","&","|","&","*","!","++","--",
"++","--","sizeof","","[","<<",">>","<",">","<=",
">=","==","!=","var"," "," "," ","->"," ", " ",
" "," ", " ","while ", " ", " ", " ", " "," "," ",
" "," "," "};

char *assigntype_string[]={"=","*=","%=","+=","-=","<<=",">>=","&&=","^=","||=",";"," "," "};
int GPFLG;
int GPFLG1;
extern int DUPVAR;
int isassigntype(enum ASSIGN_TYPE type){//checks for statement assignment type(asstype)
if(type==AASSIGN|| type==AMUL_ASSIGN|| type==AMOD_ASSIGN||type==AADD_ASSIGN|| type==ASUB_ASSIGN|| type==ALEFT_ASSIGN || type==ARIGHT_ASSIGN || type==AAND_ASSIGN || type==AXOR_ASSIGN|| type==AOR_ASSIGN|| type==EMPTY_ASSIGN)return 1;
if(type==RRADD_ASSIGN || type==RRMUL_ASSIGN)return 2;
return 0;
}

isbinary1to10(enum EXPR_TYPE type){//checks whether expr_type is any one from 1 to 10 in the enum
if(type==AND||type==ADD || type==MUL || type==SUB || type==DIV || type==MOD ||  type==OR || type==XOR|| type==BAND || type==BOR)return 1;
return 0;
}
void assign_stmt::printcode1(char *print_string, bool semi){
int flag=0;
int xu;
printf("assign printcode \n");
if(this->asstype==EMPTY_ASSIGN ){
if(semi)sprintf(print_string,";");
return;
}
int type=-1;
if(this->rhs!=NULL)type=this->rhs->expr_type;
//ariflag=1;modif7
ariflag=0;//modif7
if(isbinary1to10(type))ariflag=1;
if(this->lhs!=NULL){
char temp[100];
for(int i=0;i<100;i++)temp[i]='\0';
this->lhs->printcode1(this->lhs,temp);
strcat(print_string,temp);
}
else flag=1;
ariflag=0;
if(isbinary1to10(type))ariflag=1;//modified on feb12
if(this->rhs->expr_type==WHILE_EXPR)return;
if(isassigntype(this->asstype)==1&&flag==0)strcat(print_string,assigntype_string[this->asstype]);
if(this->rhs->pflag!=0)return;
if(this->rhs->pflag==-100){
this->rhs->pflag=100;return;
}
if(this->rhs!=NULL){
char temp[100];
for(int i=0;i<100;i++)temp[i]='\0';
this->rhs->printcode1(this->rhs,temp);
strcat(print_string,temp);
}
if(ariflag==0){
if(this->lhs && this->lhs->libdtype>=0 ){
dir_decl *d1,*d2;
if(this->lhs){
d1=this->lhs->parent; while(d1&&d1->parent!=NULL) d1=d1->parent;
}
if(this->rhs){d2=this->rhs->lhs; while(d2 &&d2->parent!=NULL) d2=d2->parent;}
}
}

if(this->rhs->pflag==-100)return;
if(semi && !(this->rhs&& this->rhs->rhs && this->rhs->rhs->expr_type==GET_TYPE))strcat(print_string,";");
}
void assign_stmt::printcode(bool semi){
int flag=0;
int xu;
char a[100],b[100];
if(this->asstype==EMPTY_ASSIGN ){
if(semi)printf(";");
return;
}
int type=-1;
if(this->rhs!=NULL)type=this->rhs->expr_type;
ariflag=0;//modif7
if(isbinary1to10(type))ariflag=1;
if(this->lhs!=NULL){
for(int i=0;i<100;i++)a[i]='\0';
this->lhs->printcode1(this->lhs,a);
fprintf(FP1,"%s",a);
}
else flag=1;
ariflag=0;
if(isbinary1to10(type))ariflag=1;//modified on feb12
if(this->rhs->expr_type==WHILE_EXPR)return;
if(isassigntype(this->asstype)==1&&flag==0)fprintf(FP1,"%s",assigntype_string[this->asstype]);
if(this->rhs->pflag==100){
this->rhs->pflag=-100;return;
}
if(this->rhs!=NULL){
for(int i=0;i<100;i++)b[i]='\0';
this->rhs->printcode1(this->rhs,b);
fprintf(FP1,"%s",b);
}
if(ariflag==0){
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
//if(this->semi)fprintf(FP1,";");
if(this->semi&& !(this->rhs&& this->rhs>rhs && this->rhs->rhs->expr_type==GET_TYPE))fprintf(FP1,"; ");
}
void tree_expr::findgpuvars(tree_expr *expr){
int type=expr->expr_type;
	if(  type==LT || type==GT|| type==LE || type==GE|| type==LE || type==EQ || type==NEQ|| type==ADD || type==STRUCTREF){
if(expr->lhs)expr->lhs->findgpuvars(expr->lhs);
if(expr->rhs)expr->rhs->findgpuvars(expr->rhs);
}
dir_decl *d1=expr->lhs;
//fprintf(FP1,"// unni %s %d unni  \n",expr->name,expr->expr_type);
if( expr && expr->expr_type==VAR ){ 

if(d1 && d1->gpu==1 && d1->libdtype!=GRAPH_TYPE && Gkernel==0){
//fprintf(FP1,"%s  temp%d;\n cudaMemcpy((void *)&temp%d,%s,sizeof( %s),cudaMemcpyDeviceToHost);\n",dtypenames[d1->dtype],extemp,extemp,expr->name,dtypenames[d1->dtype]);
//expr->cpu_name=malloc(sizeof(char)*100);//this is new name of variable. this will be seen in .cu file generated
//sprintf(expr->cpu_name,"temp%d",extemp);
//extemp++;
}
}
return;
}
void tree_expr::FOR_findgpuvars(tree_expr *expr){
int type=expr->expr_type;
	if(  type==LT || type==GT|| type==LE || type==GE|| type==LE || type==EQ || type==NEQ|| type==ADD){
expr->lhs->FOR_findgpuvars(expr->lhs);
expr->rhs->FOR_findgpuvars(expr->rhs);
}
dir_decl *d1=expr->lhs;
if(expr->expr_type==VAR ){ 

if(d1 && d1->gpu==1 && d1->libdtype!=GRAPH_TYPE){
fprintf(FP1,"%s  temp%d;\n cudaMemcpy((void *)&temp%d,%s,sizeof( %s),cudaMemcpyDeviceToHost);\n",dtypenames[d1->dtype],extemp,extemp,expr->name,dtypenames[d1->dtype]);
expr->cpu_name=malloc(sizeof(char)*100);
sprintf(expr->cpu_name,"temp%d",extemp);
extemp++;
//fprintf(FP1,"//this variable %s is GPU variable\n",expr->name);
}
}
if(d1!=NULL && d1->gpu==1 && d1->libdtype==GRAPH_TYPE){
fprintf(FP1,"%s  temp%d;\n cudaMemcpy((void *)&temp%d,%s.extra,sizeof( %s),cudaMemcpyDeviceToHost);\n",d1->extra_name,extemp,extemp,d1->name,d1->extra_name);
expr->cpu_name=malloc(sizeof(char)*100);
sprintf(expr->cpu_name,"temp%d",extemp);
extemp++;
}
if(type==STRUCTREF && Gkernel==0){
expr->lhs->FOR_findgpuvars(expr->lhs);
expr->rhs->FOR_findgpuvars(expr->rhs);
}
return;
}

int tree_expr::printcode1(tree_expr *expr,char *print_string){
int n;
if(expr==NULL)return;
if(expr->pflag==100){expr->pflag=-100;return;}
int type=expr->expr_type;
if(type==CAST_EXPR){
if(expr->tpname!=NULL){
strcat(print_string,"(");
expr->tpname->printcode(print_string);
strcat(print_string,")");
//expr->rhs->print(expr->rhs);
}
}
if(type==TYPE_SIZEOF){
strcat(print_string,"sizeof(");
if(expr->tpname!=NULL){
char a[100];
expr->tpname->printcode(a);
strcat(print_string,a);
strcat(print_string,")");
}
}

if(type==TYPE_INIT){
if(expr->tpname!=NULL){
strcat(print_string,"(");
expr->tpname->print();
strcat(print_string,")");
}
}
if(type==COND_OP){
strcat(print_string,"(");
expr->lhs->printcode1(expr->lhs,print_string);
strcat(print_string,") ?");
expr->rhs->lhs->printcode1(expr->rhs->lhs,print_string);
strcat(print_string," : ");
expr->rhs->rhs->printcode1(expr->rhs->rhs,print_string);

}
if(type==STRUCTREF){
int xu;
if(expr->lhs!=NULL&& expr->lhs->expr_type==STRUCTREF){
if(expr->cpu_name!=NULL){strcat(print_string,expr->cpu_name);return;}
expr->lhs->printcode1(expr->lhs,print_string);
expr->tp1=expr->lhs->tp1;
if(expr->lhs->libdtype>=0){
if(expr->lhs->libdtype==SET_TYPE){//prints code for SET DATA TYPE
printcode1settype(expr,print_string);
return;
}
if(expr->lhs->libdtype==COLLECTION_TYPE){ //PRINTS CODE FOR COLLECTION DATA TYPE
printcode1collectiontype(expr,print_string);
return;
}
}
if(expr->lhs->libdtype!=4 ){
if(strstr(print_string,"extra")!=NULL){
if(expr->lhs->lhs!=NULL && expr->lhs->lhs->expr_type!=STRUCTREF && print_string[strlen(print_string)-1]!='>')strcat(print_string,".");
}
if(expr->lhs->libdtype==5)strcat(print_string,".");//added M26. 5==G_P_TYPE
tree_expr *ext1=expr;
int dotcnt=0;
while(ext1->expr_type==STRUCTREF){ext1=ext1->lhs;dotcnt++;}
if(dotcnt>=3 && ext1->lhs->extra_name1==NULL){
strcat(print_string,".");
struct extra_ppts *eppt= ((dir_decl *)(ext1->lhs))->ppts;
while(eppt!=NULL){
fprintf(stderr,"pppt%s  %s",eppt->name,expr->lhs->rhs->name);
if(!strcmp(eppt->name,expr->lhs->rhs->name)){
char tempar[100];
for(int i=0;i<100;i++)tempar[i]='\0';
sprintf(tempar,"%s %s temp%d;\n cudaMemcpy((void *)&temp%d,temp%d.%s,sizeof(%s %s),cudaMemcpyDeviceToHost);\n",dtypenames[eppt->t1->datatype],eppt->t1->vname,extemp+1,extemp+1,extemp,expr->lhs->rhs->name,dtypenames[eppt->t1->datatype],eppt->t1->vname);
extemp++;
strcat(gpcopy_string,tempar);
}
eppt=eppt->next;
}
}
if( !(strcmp(expr->rhs->name,"reset"))&&(strlen(expr->rhs->name)==strlen("reset"))){
strcat(print_string,"=99999999");
return;
}
expr->rhs->printcode1(expr->rhs,print_string);
if(expr->tp1!=NULL && expr->tp1->datatype==11){ //11==STRUCTREF
tree_decl_stmt *head= expr->tp1->list;//each declaration in struct definition
while(head!=NULL){
if(head->dirrhs!=NULL){
dir_decl *d1=head->dirrhs;
while(d1!=NULL){
if(!strcmp(d1->name,expr->rhs->name)&&(strlen(d1->name)==strlen(expr->rhs->name))){
expr->tp1=head->lhs;//assing expr type to that of struct declaration type
break;
}
d1=d1->nextv;
}
}
head=head->next;
}
}else {
expr->libdtype=expr->rhs->libdtype;
expr->dtype=expr->rhs->dtype;
}
}
if(expr->lhs && expr->lhs->rhs && expr->lhs->rhs->expr_type==ARRREF&& expr->lhs->libdtype>0){
strcat(print_string,"[");
assign_stmt *y=expr->lhs->rhs->earr_list;
if(y->lhs!=NULL)y->lhs->printcode1(y->lhs,print_string);
y->rhs->printcode1(y->rhs,print_string);
strcat(print_string,"]");
//fprintf(FP1,"/*here*/");
}
if(expr->lhs->libdtype==4 || (expr->lhs->dtype==11)){
tree_expr *ext1=expr;
if(expr->tp1!=NULL && expr->tp1->datatype==11){
}
int dotcnt=0;
while(ext1->expr_type==STRUCTREF){ext1=ext1->lhs;dotcnt++;}
if(dotcnt>=3 && ext1->lhs->extra_name1==NULL){
strcat(print_string,".");
struct extra_ppts *eppt= ((dir_decl *)(ext1->lhs))->ppts;
while(eppt!=NULL){
if(!strcmp(eppt->name,expr->lhs->rhs->name)){
char tempar[100];
for(int i=0;i<100;i++)tempar[i]='\0';
sprintf(tempar,"%s %s temp%d;\n cudaMemcpy((void *)&temp%d,temp%d.%s,sizeof(%s %s),cudaMemcpyDeviceToHost);\n",dtypenames[eppt->t1->datatype],eppt->t1->vname,extemp+1,extemp+1,extemp,expr->lhs->rhs->name,dtypenames[eppt->t1->datatype],eppt->t1->vname);
extemp++;
strcat(gpcopy_string,tempar);
}
eppt=eppt->next;
}
}
expr->rhs->printcode1(expr->rhs,print_string);
expr->libdtype=-1;
expr->dtype=expr->rhs->dtype;
}
return;
}
else {
if(extra_array[0]!='\0'){
for(int i=0;i<200;i++)extra_array[i]='\0';
}
if(((dir_decl *)expr->lhs)->libdtype>=0){
if(((dir_decl *)expr->lhs)->libdtype==8)((dir_decl *)expr->lhs)->libdtype==2;
if(expr->lhs->libdtype==COLLECTION_TYPE && expr->lhs->lhs!=NULL && expr->lhs->lhs->gpu==true && (Gkernel==0 || Gkernel>3)){
if((!strcmp(expr->rhs->name,"size"))&&(strlen(expr->rhs->name)==strlen("size"))){
strcat(print_string,expr->lhs->name);
strcat(print_string,".size/**/");
return;
}
}//SEP13
//for x and y different code needs to be generated for two dimensional coordinate point
//x is first from index and y is second from index. also mulltiplication by point dimension is needed.
if(expr->lhs->libdtype==EDGE_TYPE &&expr->lhs->lhs!=NULL && expr->lhs->lhs->parent!=NULL){
printcode1edgetype(expr,print_string);
return;
}
if(expr->lhs->libdtype==POINT_TYPE &&expr->lhs->lhs!=NULL && expr->lhs->lhs->parent!=NULL){
  if (!strcmp(expr->rhs->name,"getOutNeighbours")){
char temp[100];
for(int i=0;i<100;i++)temp[i]='\0';
sprintf(temp,"%s.index[%s+1].ipe - %s.index[%s].ipe",expr->lhs->lhs->parent->name,expr->lhs->lhs->name,expr->lhs->lhs->parent->name,expr->lhs->lhs->name);
strcat(print_string,temp);
}else 
printcode1pointtype(expr,print_string);
return;
}
if(expr->lhs->libdtype==SET_TYPE){
if(expr->rhs->expr_type==FUNCALL){
if(!strcmp(expr->rhs->name,"find")&& (strlen(expr->rhs->name)==strlen("find"))){
char temp[25];
strcat(print_string,expr->lhs->name);
strcat(print_string,"./**/");
strcat(print_string,"parent");
strcat(print_string,"[");
if(expr->rhs->arglist->rhs->lhs->extra_name1==NULL)strcat(print_string,expr->rhs->arglist->rhs->lhs->name);
else strcat(print_string,expr->rhs->arglist->rhs->lhs->extra_name1);
strcat(print_string,"]");
//fprintf(FP1,"/*here %d*/",341);
return;
}
if(!strcmp(expr->rhs->name,"Union")&& (strlen(expr->rhs->name)==strlen("Union"))){ 
printf(" MAKING HEAD POSITION TO HAVE THIS VALUE %s\n",curr_fun_name);
struct UNION_KER *tempu;
tempu=malloc(sizeof(struct UNION_KER));
tempu->name=malloc(sizeof(char)*100);
tempu->next=NULL;
if(strlen(curr_fun_name)!=0)sprintf(tempu->name,"%s",curr_fun_name);
if(strlen(curr_fun_name1)!=0)sprintf(tempu->name,"%s",curr_fun_name1);
if(head_position==NULL)head_position=tempu;
else{tempu->next=head_position;head_position=tempu;}

}
}
}
if(expr->lhs->libdtype==COLLECTION_TYPE){
if(expr->rhs->expr_type==FUNCALL){

printf("HERE FUNCALL \n");
if(!strcmp(expr->rhs->name,"add")&& (strlen(expr->rhs->name)==strlen("add"))){
if(expr->lhs->lhs && expr->lhs->lhs->gpu==1 &&Gkernel==0){
printf("HERE FUNCALL \n");
char varname[25];
sprintf(varname,"falcvt%d",++Temp);
fprintf(FP1,"int %s;\n",varname);
fprintf(FP1,"cudaMemcpy(&falcvt%d,&(%s.size[0]),sizeof(int),cudaMemcpyDeviceToHost);\n",Temp,expr->lhs->name);
strcat(print_string,expr->lhs->name);
strcat(print_string,"temp[ ");
strcat(print_string,varname);
strcat(print_string,"++]=");
printcode1(expr->rhs->arglist->rhs,print_string);

strcat(print_string,";\ncudaMemcpy(&(");
strcat(print_string,expr->lhs->name);
strcat(print_string,".size[0]),&");
strcat(print_string,varname);
strcat(print_string,",sizeof(int),cudaMemcpyHostToDevice)");
return;

}
if(expr->lhs->lhs && expr->lhs->lhs->gpu==1 &&Gkernel>0){
char varname[25];
sprintf(varname,"falcvt%d",++Temp);
fprintf(FP1,"int %s;\n",varname);

fprintf(FP1,"\n%s=GADD(&(%s.size[0]),1);\n",varname,expr->lhs->name);

strcat(print_string,expr->lhs->name);
strcat(print_string,".ptr[");
strcat(print_string,varname);
strcat(print_string,"]=");
printcode1(expr->rhs->arglist->rhs,print_string);
return;
}
if(expr->lhs->gpu==0 && Gkernel>0){
fprintf(FP1,"\nint falctt=GADD(&(%s.size[0]),1);\n",expr->lhs->name);
strcat(print_string,expr->lhs->name);
strcat(print_string,".ptr[falctt]=");
printcode1(expr->rhs->arglist->rhs,print_string);
return;

}
}
if(!strcmp(expr->rhs->name,"del")&& (strlen(expr->rhs->name)==strlen("del"))){
strcat(print_string,"get_elem_coll_device(");
strcat(print_string,expr->lhs->name);
strcat(print_string,",");
printf("HERE FUNCALL DEL\n");
if(expr->rhs->arglist!=NULL && expr->rhs->arglist->rhs!=NULL && expr->rhs->arglist->rhs->lhs!=NULL && expr->rhs->arglist->rhs->lhs->extra_name1==NULL)strcat(print_string,expr->rhs->arglist->rhs->lhs->name);
else{
printf("HERE FUNCALL DEL\n");
if(expr->rhs->arglist!=NULL && expr->rhs->arglist->lhs!=NULL && expr->rhs->arglist->lhs->extra_name1!=NULL) strcat(print_string,expr->rhs->arglist->rhs->lhs->extra_name1);
else printcode1(expr->rhs->arglist->rhs,print_string);
}
strcat(print_string,")");
return;
}
if(!strcmp(expr->rhs->name,"getSize")&& (strlen(expr->rhs->name)==strlen("getSize"))){
strcat(print_string,expr->lhs->name);
strcat(print_string,"size");//SEP13
return;
}
}
//has to add check for rhs is not funcall then generate code with parent name and extra field based on collection type.
}
if(expr->lhs->libdtype==ITERATOR_TYPE && expr->lhs->lhs->it==1){
//if(expr->lhs->lhs->extra_name!=NULL)
if(!strcmp(expr->rhs->name,"weight")){
char temp[100];
for(int i=0;i<100;i++)temp[i]='\0';
sprintf(temp,"%s.edges[%s+1].ipe",expr->lhs->lhs->parent->name,expr->lhs->lhs->extra_name);
strcat(print_string,temp);
}

else {
char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
if(expr->lhs->lhs->it<6)sprintf(temp,"((%s *)(%s.extra))->%s[%s/2]",expr->lhs->lhs->parent->extra_name,expr->lhs->lhs->parent->name,expr->rhs->name,expr->lhs->lhs->extra_name);

//check needs to be done what is rhs filed type and set extra property accordingly
	if(expr->lhs->lhs->it>=6)sprintf(temp,"((%s *)(%s.extra))->%s[%s/3].%s",expr->lhs->lhs->parent->extra_name,expr->lhs->lhs->parent->name,expr->lhs->name,expr->lhs->lhs->extra_name,expr->rhs->name);
	strcat(print_string,temp);


	}
	return;
	}
if(expr->lhs->libdtype==GRAPH_TYPE ){
if(printcode1addtograph(expr,print_string))return;
if((!strcmp(expr->rhs->name,"points")|| !strcmp(expr->rhs->name,"edges"))&& expr->nodetype!=-10) {
char arr[100];
for(int i=0;i<100;i++)arr[i]='\0';
assign_stmt *ass=expr->rhs->earr_list;
if(ass!=NULL)ass->rhs->printcode1(ass->rhs,arr);
strcat(print_string,arr);
return;
}
	struct extra_ppts *et1;
if(expr->lhs->lhs)	et1=expr->lhs->lhs->ppts;
else	et1=expr->lhs->ppts;
if(et1!=NULL){
	int mf=0,mdtype=0;
	while(et1!=NULL){
	if(!strcmp(et1->name,expr->rhs->name)){mf=1;mdtype=et1->t1->datatype;

break;}
if(strstr(expr->rhs->name,et1->name)!=NULL/* && (strlen(expr->rhs->name)==(strlen(et1->name)+1))*/){
mf=1;mdtype=et1->t1->datatype; break;}
	et1=et1->next;
	}
if(mf==1){ 
char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
sprintf(temp,"/***/((%s *)(%s.extra))->",expr->lhs->lhs->extra_name,expr->lhs->name);
char temp1[100];
for(int i=0;i<100;i++)temp1[i]='\0';
if(expr->rhs->expr_type==ARRREF){
fprintf(FP1,"//ARRREF\n");
if(et1!=NULL && Gkernel==0){
if((( dir_decl *)(expr->lhs->lhs))->gpu==1 && (( dir_decl *)(expr->lhs->lhs))->libdtype!=(COLLECTION_TYPE||SET_TYPE)){fprintf(FP1,"%s temp%d;/*xx*/\n",expr->lhs->lhs->extra_name,extemp);
fprintf(FP1,"cudaSetDevice(%d);\n",expr->lhs->lhs->dev_no);
sprintf(gpcopy_string,"cudaMemcpy(&temp%d,((%s *)(%s.extra)),sizeof(%s),cudaMemcpyDeviceToHost);\n",extemp,expr->lhs->lhs->extra_name,expr->lhs->name,expr->lhs->lhs->extra_name);
}
sprintf(print_string,"((%s *)(%s.extra))->",expr->lhs->lhs->extra_name,expr->lhs->lhs->name);
expr->rhs->printcode1(expr->rhs,temp1);
strcat(print_string,temp1);
}
else {
strcat(print_string,"((");strcat(print_string,expr->lhs->lhs->extra_name);strcat(print_string," *)(");strcat(print_string,expr->lhs->lhs->name);strcat(print_string,".extra))->");
//sprintf(print_string,"((%s *)(%s.extra))->",expr->lhs->lhs->extra_name,expr->lhs->lhs->name);
expr->rhs->printcode1(expr->rhs,temp1);
strcat(print_string,temp1);

}
}
else {

if(et1!=NULL){
if((( dir_decl *)(expr->lhs->lhs))->gpu==1&& (( dir_decl *)(expr->lhs->lhs))->libdtype!=(COLLECTION_TYPE||SET_TYPE)){
fprintf(FP1,"%s temp%d;/*xx*/\n",expr->lhs->lhs->extra_name,extemp);
fprintf(FP1,"cudaSetDevice(%d);\n",expr->lhs->lhs->dev_no);
sprintf(gpcopy_string,"cudaMemcpy(&temp%d,((%s *)(%s.extra)),sizeof(%s),cudaMemcpyDeviceToHost);\n",extemp,expr->lhs->lhs->extra_name,expr->lhs->name,expr->lhs->lhs->extra_name);
}
sprintf(print_string,"((%s *)(%s.extra))->",expr->lhs->lhs->extra_name,expr->lhs->lhs->name);
expr->rhs->printcode1(expr->rhs,temp1);
strcat(print_string,temp1);
//strcat(gpcopy_string,temp1);
}
}
return;
}
}else{
if(expr->lhs && expr->lhs->lhs && expr->lhs->lhs->ppts!=NULL &&(expr->kernel!=5/* ||expr->lhs->lhs->it!=-1*/))et1=expr->lhs->lhs->ppts;
if(et1!=NULL){
	int mf=0,mdtype=0;
	while(et1!=NULL){
if((strstr(expr->rhs->name,et1->name)!=NULL)&&(strstr(expr->rhs->name,"add")!=NULL)){
char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
sprintf(temp,"falcadd%s%s();\n",expr->lhs->name,et1->name);
strcat(print_string,temp);
return;
}
if(!strcmp(et1->name,expr->rhs->name)){mf=1;mdtype=et1->t1->datatype;break;}
if(!strcmp(expr->rhs->name,et1->name)!=NULL && et1->t1->libdatatype!=G_P_TYPE){
mf=1;mdtype=et1->t1->datatype; break;}
if(strstr(expr->rhs->name,et1->name)!=NULL && et1->t1->libdatatype==G_P_TYPE){
mf=1;mdtype=et1->t1->datatype; break;}
	et1=et1->next;
	}
if(mf==1){ 
char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
//Temp++;
if((Gkernel==0 )/*&& CPUGPU==0*/){ 
fprintf(FP1,"%s temp%d;/*xx*/\n",expr->lhs->lhs->extra_name,extemp);
fprintf(FP1,"cudaSetDevice(%d);\n",expr->lhs->lhs->dev_no);
fprintf(FP1,"cudaMemcpy(&temp%d,((%s *)(%s.extra)),sizeof(%s),cudaMemcpyDeviceToHost);\n",extemp,expr->lhs->lhs->extra_name,expr->lhs->name,expr->lhs->lhs->extra_name);
sprintf(temp,"temp%d.",extemp);
extemp++;
}
else{
char temp1[100];
for(int i=0;i<100;i++)temp1[i]='\0';
//if(expr->lhs->lhs->cpugpu==false)
sprintf(temp1,"((%s  *)(/**/graph.extra))->",expr->lhs->lhs->extra_name);
strcat(print_string,temp1);
}
char temp1[100];
for(int i=0;i<100;i++)temp1[i]='\0';
if(expr->rhs->expr_type==ARRREF){
expr->rhs->printcode1(expr->rhs,temp1);
}
strcat(print_string,temp);
if(expr->rhs->expr_type==ARRREF)
strcat(print_string,temp1);
else strcat(print_string,expr->rhs->name);

return;
}
}



}
}
	}
	dir_decl *d1= (dir_decl *)(expr->lhs->lhs);
	if(d1->parent!=NULL){expr->parent=d1->parent;}//FEB14
	sref=sref+1;
	if(d1->extra_name1!=NULL ){
	if(d1->extra_name==NULL){
	d1->extra_name=malloc(sizeof(char)*100);
	if(d1->parent!=NULL){
	strcpy(d1->extra_name,d1->parent->extra_name);
	}
	}
	char temp[100],temp1[100];
	for(int i=0;i<100;i++)temp[i]=temp1[i]='\0';
	if(d1->it<6  && d1->it>=0){sprintf(temp,"((%s *)(%s.extra))->",d1->extra_name,d1->parent->name); 
	strcat(print_string,temp);
	}
	if(d1->it>=6){
//check needs to be done what is type of rhs field type and set expr->libdtype accordingly
	for (std::map<char *,int>::iterator it1=newitr.begin(); it1!=newitr.end(); ++it1){
	if(it1->second==d1->it){
	sprintf(temp,"((%s *)(%s.extra))->%s",d1->extra_name,d1->parent->name,it1->first); 
	break;
	}
	}
	strcat(print_string,temp);
	}
	int flagit=0;

	if(d1->it==-1){
	fprintf(stderr,"HERE %s",d1->name);
	dir_decl *dp;
	if(d1->parent!=NULL)dp=d1->parent;
	else if(d1->libdtype==GRAPH_TYPE)dp=d1;
	if(dp!=NULL){
	struct extra_ppts *ep=dp->ppts;
	while(ep!=NULL){
	tree_typedecl  *dec=ep->t1;
	if(dec && d1->tp1!=NULL){
	fprintf(stderr,"%s %s\n",dec->name, d1->tp1->name);
//here check needs to added to what extra field access type is and then set expr->libdtype;
	if(dec->name && d1->tp1->name && !strcmp(dec->name, d1->tp1->name)){
	flagit=1;
//	if(d1->parent->cpugpu==false)sprintf(temp,"/**YY**/((%s *)(%s.extra))->%s",d1->extra_name,d1->parent->name,ep->var3->name); 
//	if(d1->parent->cpugpu==true&&Gkernel==0)sprintf(temp,"/*XX2*/((%s *)(%s.hostparts[0].extra))->%s",d1->extra_name,d1->parent->name,ep->var3->name); 
//else
	sprintf(temp,"/***/((%s *)(%s.extra))->",d1->extra_name,d1->name);
	//sprintf(temp,"[%s].",d1->extra_name1); 
	strcat(temp,"[");
//fprintf(FP1,"GK=%d",Gkernel);
	strcat(temp,d1->extra_name1);
	strcat(temp,"].");
	break;
	}

	}
	ep=ep->next;
	}
	if(flagit==0){ sprintf(temp,"((%s *)(%s.extra))->",d1->extra_name,d1->parent->name);
	}
	strcat(print_string, temp);
	}
	}
	if(d1->it>=6){
	sprintf(temp1,"[%s].",d1->extra_name1); 
	strcat(print_string,temp1);
	}
	expr->rhs->printcode1(expr->rhs,print_string);
	if(flagit==0 && d1->it==-1){
	sprintf(temp1,"[%s]",d1->extra_name1); 
	strcat(print_string,temp1);
//fprintf(FP1,"/*here %d*/",624);
	}
	if(d1->it<6 && d1->it>=0){
	sprintf(temp1,"/*xx*/[%s]",d1->extra_name1); //krish03
	strcat(print_string,temp1);
	}
	fprintf(stderr,"HERE LAST");
	struct extra_ppts *et1;
	if(d1->libdtype==GRAPH_TYPE)et1=d1->ppts;
	else et1=d1->parent->ppts;
	int mf=0,mdtype=0;
	while(et1!=NULL){
	if(!strcmp(et1->name,expr->rhs->name)){mf=1;mdtype=et1->t1->datatype;break;}
	et1=et1->next;
	} 
	if(mdtype==11){
	strcat(print_string,".");
	expr->tp1=et1->t1;
	}
	return;
	}
	char *name=expr->rhs->name;
	int flag=0,i=0;
	enum SIZES size;

	if(d1->libdtype==GRAPH_TYPE)size=fungraph;
	if(d1->libdtype==EDGE_TYPE)size=funedge;
	if(d1->libdtype==POINT_TYPE)size=funpoint;
	if(d1->libdtype==SET_TYPE)size=funset;
	if(d1->libdtype==COLLECTION_TYPE ){
	if(expr->rhs && expr->rhs->expr_type==FUNCALL){
	for(int i=0;i<size;i++){
	if(!strcmp(expr->rhs->name,libdtypefunctions[4][i])&&(strlen(expr->rhs->name)==strlen(libdtypefunctions[4][i]))){
	expr->libdtype=-1; expr->rhs->printcode1(expr->rhs,print_string);return;}
	}
	}
	}
	if(expr->rhs && d1->libdtype==SET_TYPE ){
	if(expr->rhs->expr_type==FUNCALL){
	for(int i=0;i<size;i++){
	if(!strcmp(expr->rhs->name,libdtypefunctions[4][i])&&(strlen(expr->rhs->name)==strlen(libdtypefunctions[4][i]))){
	expr->libdtype=-1; expr->rhs->printcode1(expr->rhs,print_string);return;}
	}
	}
	}
	if(expr->rhs && d1->libdtype==GRAPH_TYPE ){
	if(expr->rhs->expr_type==FUNCALL){
if(!strcmp(expr->rhs->name,"read")&&strlen(expr->rhs->name)==strlen(name)){
strcpy(expr->rhs->name,"read2");
if(INNBRFLAG==1 &&MORPH_FLAG==0)strcpy(expr->rhs->name,"read1");
if(MORPH_FLAG==1)strcpy(expr->rhs->name,"readmorph");
if(MPIFLAG==1)strcpy(expr->rhs->name,"makeNPartitionsMPI");
char arr[100];
for(int i=0;i<100;i++)arr[i]='\0';
expr->rhs->arglist->rhs->printcode1(expr->rhs->arglist->rhs,arr);
if(MPIFLAG==1){
//fprintf(FP1,"sprintf(partitonfile,\"%%s\",argv[2]);\n");
if(TARGET==3)fprintf(FP1,"//better to read graph in a temporary HGraph object and the clone to GGraph.\n");
if(TARGET==3)fprintf(FP1,"//Temporary HGraph object can be inside the GGraph class itself.\n");
if(TARGET==3)fprintf(FP1,"//For GGraph do not allocate offset in GPU. It is not needed in any kernel.\n");
if(TARGET>1)fprintf(FP1,"%s.readPointsN(partitionfile,FALCsize);\n",expr->lhs->name,partitionfile);
if(TARGET<=0)fprintf(FP1,"%s.read(%s);\n",expr->lhs->name,arr);
if(TARGET!=5)fprintf(FP1,"%s.%s(%s,FALCrank,FALCsize);\n",expr->lhs->name,expr->rhs->name,arr);
if(TARGET==5)fprintf(FP1,"%s.%s(%s,2*FALCrank,FALCsize);\n",expr->lhs->name,expr->rhs->name,arr);
}
else fprintf(FP1,"%s.%s(%s);\n",expr->lhs->name,expr->rhs->name,arr);

//if(d1->cpugpu==false)fprintf(FP1,"alloc_extra_%s(%s,0);\n",expr->lhs->name,expr->lhs->name);
if(TARGET==2)fprintf(FP1,"int host%s=0;\n%s\n",d1->name,d1->extra_fun);
if(TARGET==3)fprintf(FP1,"int %sflag=0;\n%s\n",d1->name,d1->extra_fun);
if(TARGET==2|| TARGET==3){
fprintf(FP1," FALCallocbuff(FALCsendbuff,FALCsize,%s.remotepoints);\n",d1->name);
fprintf(FP1," FALCallocbuff(FALCrecvbuff,FALCsize,%s.npoints);\n",d1->name);
if(TARGET==3)fprintf(FP1,"int TPB0=1024;\n");

}
if(TARGET==5){
fprintf(FP1,"%s.gpupart.readPointsN(partitionfile,FALCsize);\n",expr->lhs->name,partitionfile);
fprintf(FP1,"%s.gpupart.%s(%s,2*FALCrank+1,FALCsize);\n",expr->lhs->name,expr->rhs->name,arr);
fprintf(FP1," FALCallocbuff(FALCsendbuff,FALCsize,%s.remotepoints);\n",d1->name);
fprintf(FP1," FALCallocbuff(FALCrecvbuff,FALCsize,%s.npoints);\n",d1->name);
fprintf(FP1," FALCgpuallocbuff(FALCgpusendbuff,FALCsize,%s.gpupart.remotepoints);\n",d1->name);
fprintf(FP1," FALCgpuallocbuff(FALCgpurecvbuff,FALCsize,%s.gpupart.npoints);\n",d1->name);
fprintf(FP1,"int TPB0=1024;\n");
fprintf(FP1,"alloc_extra_%s(%s.gpupart,0);\n",expr->lhs->name,expr->lhs->name);
fprintf(FP1,"int host%s=0;\n%s\n",d1->name,d1->extra_fun);


}
return;
}
}
}
	if(d1->libdtype==G_P_TYPE){expr->lhs->printcode1(expr->lhs,print_string); expr->libdtype=-1; expr->rhs->printcode1(expr->rhs,print_string);
return;}
	if(d1->libdtype==GRAPH_TYPE)size=fieldgraph;
	if(d1->libdtype==EDGE_TYPE)size=fieldedge;
	if(d1->libdtype==POINT_TYPE)size=fieldpoint;
	for(i=0;i<size;i++){

	if(!strcmp(name,libdtypefields[expr->lhs->lhs->libdtype][i])&&strlen(name)==strlen(libdtypefields[expr->lhs->lhs->libdtype][i])){
	flag=1;
	break;
	}
	}
	if(flag!=1){
	if(d1->libdtype==GRAPH_TYPE)size=fungraph;
	if(d1->libdtype==EDGE_TYPE)size=funedge;
	if(d1->libdtype==POINT_TYPE)size=funpoint;
	if(d1->libdtype==SET_TYPE)size=funset;
	for(i=0;i<size;i++){
	if(!strcmp(name,libdtypefunctions[expr->lhs->lhs->libdtype][i])&&strlen(name)==strlen(libdtypefunctions[expr->lhs->lhs->libdtype][i])){//to do make modular
	flag=2;
	if(d1->libdtype==GRAPH_TYPE){
printcode1graphfunction(d1,i,expr,print_string);
return;
}
	if(d1->libdtype==POINT_TYPE){
printcode1pointfunction(d1,i,expr,print_string);
return;
}
	if(d1->libdtype==EDGE_TYPE)printcode1edgefunction(d1,i,expr,print_string);//if this is called return should be added
	if(d1->libdtype==POINT_TYPE)printcode1pointfunction(d1,i,expr,print_string);//if this is called return should be added
	if(d1->libdtype==SET_TYPE)printcode1setfunction(d1,i,expr,print_string);//if this is called return should be added
	break;
	}
	}
	}
	if(flag==1 /*&& ariflag==1*/){//now make function for field acess 
	if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
	if(i==1||i==3){
EXTRAFLAG=1;
//fprintf(FP1," EF==1 %d ",725);
}
	if(((i==1)||(i==3)) && expr->rhs->expr_type!=ARRREF &&ariflag==1){//points or edges
	fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
	else if(expr->rhs->expr_type==ARRREF&&((i==0)||(i==2)|| (i==4)|| (i==5))&&ariflag==1){//npoints,nedges,directed or twod
	fprintf(stderr,"ivalid use of field %s\n ",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
	else if((i==1) /*&& expr->rhs->expr_type==ARRREF*/){//points
expr->libdtype=POINT_TYPE; expr->parent=d1;}
	else if((i==3) /*&& expr->rhs->expr_type==ARRREF*/){//edges
	expr->libdtype=EDGE_TYPE;
	if(FOREACH_FLAG>0){strcat(print_string,"edges[unni1]");return;}
	expr->parent=d1;
	}
   if(i==0 /*&& expr->lhs->lhs->cpu_name!=NULL*/){
strcat(print_string,expr->lhs->lhs->name);
strcat(print_string,".npoints");
return;
	}
	if(expr->lhs->lhs->libdtype==POINT_TYPE){
	if(((i==4)||(i==5)||(i==6)) && expr->rhs->expr_type!=ARRREF){//nbrs,innbrs,outnbrs
	fprintf(stderr,"ivalid use of field %s\n ",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
	else if(expr->rhs->expr_type==ARRREF&&((i==0)|| (i==1)||(i==2)||(i==3) )){//minEdge,maxEdge,x,y
	fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
	if((i==4) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
	if((i==5) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
	if((i==6) && expr->rhs->expr_type==ARRREF){expr->libdtype=POINT_TYPE; }
	}
	if(expr->lhs->lhs->libdtype==EDGE_TYPE){
	if(((i==0)||(i==1)) && expr->rhs->expr_type!=VAR){
	fprintf(stderr,"ixxvalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
	if(expr->rhs->expr_type==ARRREF&& i==2){
	fprintf(stderr,"iyyvalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
	if((i==0) && expr->rhs->expr_type==VAR){ /*printf("edGE TYPE %d\n",i);*/expr->libdtype=POINT_TYPE;  }
	if((i==1) && expr->rhs->expr_type==VAR){ /*printf("edGE TYPE %d\n",i); */ expr->libdtype=POINT_TYPE; }
	}
	}
	if(flag==2 ){

	if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
	//if(i==5) {expr->libdtype=EDGE_TYPE;expr->parent=expr->lhs->lhs;}
	//else expr->libdtype=-2;

	}
	}
	char tempgp[100];
	for(int i=0;i<100;i++)tempgp[i]=0;
	if(flag==0){
if(d1)fprintf(FP1,"/* J7 %d*/",d1->it);
	extra_ppts *ex=NULL;
	if(d1->libdtype==GRAPH_TYPE)ex=d1->ppts;
	else if(d1->parent!=NULL){ex=d1->parent->ppts;/*fprintf(stderr,"parent=%d %s %s ",d1->parent->arg,d1->name,d1->parent->name);*/}
	else if(d1->arg==1)ex=d1->ppts;
	//if(ex==NULL)fprintf(stderr,"NO EXTRA\n");
	while(ex!=NULL&& flag==0){
	if(!strcmp(ex->name,expr->rhs->name)&& strlen(ex->name)==strlen(expr->rhs->name)){
	if(ex->libdtype!=d1->libdtype){
	//********************************NEED TO ADD CHECK FOR ACCESS OF POINT/EDGE AND EXTRA PROPERTY********************************
	if(d1->libdtype==GRAPH_TYPE)expr->parent=d1;
	else expr->parent=d1->parent;
	if(d1->libdtype==GRAPH_TYPE && ex->libdtype==G_P_TYPE ){
	if(ex->name!=NULL){

	if(expr->rhs && expr->rhs->expr_type==ARRREF)strcpy(tempgp,ex->name);
	if(expr->rhs && expr->rhs->expr_type!=ARRREF){
	if(ex->var2!=NULL&& ex->var1!=NULL&&d1->gpu==0)
	sprintf(gp_string,"sizeof(%s)*((%s *)(%s.extra))->n%s",ex->var1->name,d1->extra_name,d1->name,ex->name);
	else if(ex->var1!=NULL) sprintf(gp_string,"sizeof(%s)*((%s *)(%s.extra))->n%s",ex->var1->name,d1->extra_name,d1->name,ex->name);
	}

	}

	}
//fprintf(FP1,"%d %d\n",d1->libdtype,ariflag);
	if(d1->libdtype==GRAPH_TYPE && ex->libdtype!=G_P_TYPE &&ariflag==1){
	fprintf(stderr,"cannot directly access %s property %s from graph  %s\n",libdtypenames[ex->libdtype] , ex->name,d1->name);
	errflag=1;
//fprintf(FP1,"/*ARIFLAG==1*/\n");
	}else {expr->libdtype=ex->t1->libdatatype;flag=1;
	if(d1->libdtype==GRAPH_TYPE){
//	fprintf(FP1," EF==1 %d ",797);
	EXTRAFLAG=1;
	}
	break;}//mdofied on fe14AN MARCH24 
	}
	if(ex->t1==NULL)printf("T! NULL\n");
	//printf("here %d %d",expr->dtype,ex->t1->datatype);
	if(ex->t1->datatype!=-1&& expr->rhs->expr_type==VAR){expr->libdtype=-1;
	expr->dtype=ex->t1->datatype;
	//printf("here %d",expr->dtype);
	}
	flag=1;
	break;
	}
	//printf("%s",expr->rhs->name);
	ex=ex->next;
	}
	}
	//fprintf(stderr,"\n***%d\n",flag);
	if(flag==2){printf("flga==2"); expr->lhs->print(expr->lhs);expr->rhs->print(expr->rhs);}
	if(flag==0 && d1->arg==0 && !(d1->parent!=NULL && d1->parent->arg==1)&&d1->libdtype>=0){fprintf(stderr,"ivalid field access %s for %s datatype\n",name,libdtypenames[expr->lhs->lhs->libdtype]);errflag=1;}
	//fprintf(stderr,"\n***\n");
	if(flag==0 &&d1->libdtype!=-1){
	extra_ppts *ex=NULL;
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
//	fprintf(FP1,"EXTRAFLAG=%d",EXTRAFLAG);
	if(EXTRAFLAG==1){
	char temp[50];
	if(d1->gpu==1){
	for(int i=0;i<300;i++)gpcopy_string[i]='\0';
	sprintf(gpcopy_string,"%s temp%d;\n cudaMemcpy(&temp%d,((%s *)(%s.extra)),sizeof(%s),cudaMemcpyDeviceToHost);\n",d1->extra_name,extemp,extemp,d1->extra_name,d1->name,d1->extra_name);
	printf("%s ",gpcopy_string);
	}
	for(int i=0;i<50;i++)temp[i]='\0';
if(d1->cpugpu==false)	sprintf(temp,"/***/((%s *)(%s.extra))->",d1->extra_name,d1->name);
if(d1->cpugpu==true && Gkernel==0)	sprintf(temp,"/*XX3*/((%s *)(%s.hostparts[0].extra))->",d1->extra_name,d1->name);
else	sprintf(temp,"/***/((%s *)(%s.extra))->",d1->extra_name,d1->name);
	//fprintf(FP1,"/***/((%s *)(%s.extra))->",d1->extra_name,d1->name);
	strcat(print_string,temp);
	if(tempgp[0]!='\0'&& expr->rhs && expr->rhs->expr_type==ARRREF){
	expr->rhs->printcode1(expr->rhs,print_string);
	}else if(tempgp[0]!='\0'&&expr->rhs){

	strcat(print_string,tempgp);
	}
	}
	//if(d1->gpu==1)printf("OOPS IT A GPU VARIABLE\n");
	}
	else {
	expr->lhs->printcode1(expr->lhs,print_string);
	 expr->libdtype=expr->lhs->libdtype;
	//fprintf(FP1,"printing lhs first %d \n",expr->lhs->libdtype);
	if(expr->lhs->libdtype==-2){fprintf(stderr,"invalid structure access\n");errflag=1;}}
	if(expr->libdtype!=-1 &&expr->libdtype!=-2  && d1->libstable==NULL){

	char *name=expr->rhs->name;
	//dir_decl *d2=expr->rhs->lhs;
	//fprintf(stderr,"here %s\n",name);
	int size,flag=0,i=0;
	if(expr->libdtype==GRAPH_TYPE)size=7;
	if(expr->libdtype==EDGE_TYPE)size=4;
	if(expr->libdtype==POINT_TYPE)size=8;
	for(i=0;i<size;i++){
	if((!strcmp(name,libdtypefields[expr->libdtype][i]))&&(strlen(name)==strlen(libdtypefields[expr->libdtype][i]))){
	flag=1;
	break;
	}
	}


	if(flag!=1 ){
	if(expr->libdtype==GRAPH_TYPE)size=6;
	if(expr->libdtype==EDGE_TYPE)size=4;
	if(expr->libdtype==POINT_TYPE)size=3;
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
	if(i==1||i==3){
EXTRAFLAG=1;
//fprintf(FP1," EF==1 %d ",896);
}
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
	if(((i==0)||(i==1)) && expr->rhs->expr_type!=ARRREF&&ariflag==1){
	fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);errflag=1;}
	else if(expr->rhs->expr_type==ARRREF&& i==2&&ariflag==1){
	fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->libdtype][i]);errflag=1;}
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
	if(flag==0 && d1 && expr->libdtype<=2&& expr->libdtype>=0&&d1->arg==0 &&!(expr->lhs->parent!=NULL&& expr->lhs->parent->arg==1)){fprintf(stderr,"invalid field access %s for %s datatype\n",name,libdtypenames[expr->libdtype]);errflag=1;}
	}
	if(d1->libstable!=NULL&&EXTRAFLAG==0){
//	expr->lhs->printcode1(expr->lhs,print_string);//COMMENTED ON MARCH24
	}
	if(expr->lhs->dtype!=-1 && expr->lhs->dtype<=9){fprintf(stderr,"struct ref on noncompound datatype\n");errflag=1;}

	if(expr->rhs->expr_type==ARRREF && expr->libdtype==-1){
	char temp[100];
	for(int i=0;i<100;i++)temp[i]='\0';
	expr->rhs->printcode1(expr->rhs,temp);
	strcat(print_string,".");
	strcat(print_string,temp);
	}

	if(expr->rhs->expr_type!=ARRREF){
	//if(expr->lhs->rhs&& expr->lhs->rhs->expr_type!=ARRREF)printf(".");
	if(expr->lhs->rhs==NULL){
	char tempgp[10];
	for(int i=0;i<10;i++)tempgp[i]='\0';
	if(strstr(print_string,"extra))->")==NULL)printf(tempgp,".");
	if(EXTRAFLAG==0)strcat(print_string,tempgp);
	}
	expr->rhs->printcode1(expr->rhs,print_string);
	}
	EXTRAFLAG=0;
	if(expr->lhs && expr->lhs->rhs && expr->lhs->rhs->expr_type==ARRREF&& expr->lhs->libdtype>0){
	strcat(print_string,"[");
	assign_stmt *y=expr->lhs->rhs->earr_list;
	if(y->lhs!=NULL)y->lhs->printcode1(y->lhs,print_string);
	y->rhs->printcode1(y->rhs,print_string);
	//if(expr->lhs->rhs->rhs)expr->lhs->rhs->rhs->print(expr->lhs->rhs->rhs);
	strcat(print_string,"]");
//fprintf(FP1,"/*here %d*/",989);
	}
	sref=sref-1;
	return;
	}
	if(strstr(print_string,"extra")!=NULL && expr->lhs->expr_type!=STRUCTREF){
	if(expr->lhs->lhs->extra_name1!=NULL){
	strcat(print_string,"[");
	strcat(print_string,expr->lhs->lhs->extra_name1);
	strcat(print_string,"]");
//fprintf(FP1,"/*here %d*/",999);
	}

	}
	fprintf(stderr,"STRUCTREF");
	}
	if(type==PTRREF){
	expr->lhs->printcode1(expr->lhs,print_string);
	strcat(print_string,"->");
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",expr->rhs->name);
	strcat(print_string,temp);
	if(expr->lhs->parent!=NULL)expr->parent=expr->lhs->parent;//FEB14
	return;
	}
	if(type==UNARYAND||  type==UNARYMUL || type==UNARYADD || type==UNARYOR || type==UNARYSUB || type==NOT||type==PREINC || type==PREDEC ){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",exptypename[type]);
	strcat(print_string,temp);
	}
	if(expr->expr_type==EXPR){
	strcat(print_string,"(");

	if(expr->exprlist!=NULL){
	statement *t1=expr->exprlist;
	//t1->print();xx
	t1->stassign->rhs->printcode1(t1->stassign->rhs,print_string);
	}
	strcat(print_string,")");
	}
	if(expr->expr_type==VAR ){
	//if(expr->lhs==NULL&&expr->name!=NULL&& sref==0&& aref==0){fprintf(stderr,"undeclared variable %s\n",expr->name);errflag=1;}
	if(expr->name!=NULL){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
        if(expr->argflag==1){
if(expr->lhs && expr->lhs->extra_name1!=NULL) strcat(print_string,expr->lhs->extra_name1);
else { 
tree_expr *ttnov=expr;
while(ttnov->lhs && ttnov->lhs->lhs)ttnov=ttnov->lhs;
if(ttnov->lhs && ((dir_decl *)ttnov->lhs)->dup_name && DUPVAR==1)sprintf(temp,"%s", ((dir_decl *)ttnov->lhs)->dup_name); 
	//if( ((dir_decl *)(expr->lhs))->dup_name==NULL)sprintf(temp,"%s",expr->lhs->name);
	else sprintf(temp,"%s",expr->name);
	strcat(print_string,temp);
//strcat(print_string,expr->name);

}
return;}
	fprintf(stderr,"\n");
	fprintf(stderr,"DONE\n");
	if(expr->lhs!=NULL && expr->lhs->iflag && expr->argflag==0){
tree_expr *ttnov=expr;
while(ttnov->lhs && ttnov->lhs->lhs)ttnov=ttnov->lhs;
//if(ttnov->lhs && ((dir_decl *)ttnov->lhs)->dup_name)fprintf(FP1,"%s 1097\n", ((dir_decl *)ttnov->lhs)->dup_name); 
	if( DUPVAR==0)sprintf(temp,"%s",expr->lhs->name);
//	if( ((dir_decl *)(expr->lhs))->dup_name!=NULL)sprintf(temp,"%s",expr->lhs->dup_name);
	strcat(print_string,temp);
	return;
	}
	if(expr->lhs!=NULL && expr->lhs->parent!=NULL && expr->lhs->extra_name2!=NULL && expr->lhs->extra_name1!=NULL && expr->lhs->extra_name!=NULL)sprintf(temp,"((%s *)%s.extra)->%s[%s]",expr->lhs->extra_name,expr->lhs->parent->name,expr->lhs->extra_name2,expr->lhs->extra_name1); 
	else {
	 if(expr->lhs!=NULL && expr->lhs->extra_name1!=NULL)sprintf(temp,"%s",expr->lhs->extra_name1);
	else if(expr->lhs!=NULL && expr->lhs->parent!=NULL && expr->lhs->parent->extra_name!=NULL){
sprintf(temp,"((%s *)(%s.extra))->",expr->lhs->parent->extra_name,expr->lhs->parent->name); 
//fprintf(FP1,"//%s \n",print_string);
strcat(print_string,temp);
if(expr->rhs)expr->rhs->printcode1(expr->rhs,print_string);
//fprintf(FP1,"//%s \n",print_string);
if(expr->rhs==NULL && expr->lhs==NULL)strcat(print_string,"/*NULL*/");
//fprintf(FP1,"//return %d\n",print_string,expr->lhs->expr_type);
return;
}
tree_expr *ttnov=expr;
while(ttnov->lhs && ttnov->lhs->lhs)ttnov=ttnov->lhs;
//if( ttnov->lhs &&((dir_decl *)ttnov->lhs)->dup_name)fprintf(FP1,"%s 1119\n", ((dir_decl *)ttnov->lhs)->dup_name); 
	if( expr->lhs!=NULL && ((dir_decl *)(expr->lhs))->dup_name!=NULL && DUPVAR==1){
sprintf(temp,"%s",((dir_decl *)(expr->lhs))->dup_name);
}
	else if(expr->cpu_name!=NULL)sprintf(temp,"%s /*here*/",expr->cpu_name);
//if DUPVAR==5, then two versions of code needed. one more CPU var and another for GPU var
	else sprintf(temp,"%s",expr->name);
//fprintf(FP1,"/*here %d*/",1054);
	}
	strcat(print_string,temp);

	fprintf(stderr,"DONE\n");
	}
	if(expr->lhs!=NULL && expr->lhs->libdtype>=0&&expr->libdtype<0&&sref==0&&/*ariflag==0&&*/expr->rhs==NULL){expr->libdtype=expr->lhs->libdtype;
	expr->parent=expr->lhs->parent;//FEb14
	}

	if(expr->expr_type==VAR)return;}
	 if (expr->nodetype==TREE_INT){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%d",expr->ival);
	strcat( print_string,temp);
	return;
	}
 if (expr->nodetype==TREE_BOOL){
if(expr->ival)strcat(print_string, "true");
else strcat(print_string,"false");
return;
}
	 if (expr->nodetype==TREE_ENUMCONST){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",expr->name);
	strcat(print_string,temp);
	return;
	}
	 if (expr->nodetype==TREE_FLOAT){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%f",expr->fval);
	strcat( print_string,temp);
	return;
	}
	if(expr->expr_type==NULL_ARR)return;
	 if (expr->nodetype==TREE_STRING){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",expr->name);
	strcat(print_string,temp);
	return;
	}
	if(expr->expr_type==FUNCALL){
	int unni;
	int libf=0,ind=-1;
	if(expr->name!=NULL){
	char temp[20];
	for(int i=0;i<11;i++){if(!strcmp(libfunctions[i],expr->name) && strlen(libfunctions[i])==strlen(expr->name)){ind=i;libf=1;break;}
	}
if(((foreachhostflag>=1/*||kernel==0*/)  && libf==1 && ind<=9 && ind>=0) ){
strcat(print_string,"H");
if(ind==1 || ind==0){
assign_stmt *tass0= expr->arglist;
assign_stmt *tass1=tass0->next;
assign_stmt *tnew=new assign_stmt();
tnew->lhs=NULL;
tree_expr *texpr=(tree_expr *)tass0->rhs;
while(texpr!=NULL && texpr->lhs!=NULL){
texpr=texpr->lhs;
}
tnew->rhs=new tree_expr();
tnew->rhs->name=malloc(sizeof(char)*100);
strcpy(tnew->rhs->name,texpr->extra_name1);
tnew->rhs->expr_type=VAR;
tnew->next=tass1->next;
tass1->next=tnew;
}
}	for(int i=0;i<20;i++)temp[i]='\0';
	if(libf==1&&ind<=9 && ind>=0 ){
if(Gkernel>0 && Gkernel!=5)sprintf(temp,"G%s(&(",expr->name);
         else sprintf(temp,"%s(&(",expr->name);
	}
	else if(libf==1&& ind==10){
	assign_stmt *t1=expr->arglist;
         int barflag=0;
	while(t1!=NULL&&t1->next){
if(t1 && t1->rhs)if(!strcmp(t1->rhs->name,"gpu")&&strlen(t1->rhs->name)==strlen("gpu"))barflag=1;
t1=t1->next;
}

if(barflag==1&&t1 && t1->rhs)if(!strcmp(t1->rhs->name,"block")&&strlen(t1->rhs->name)==strlen("block"))barflag=2;
if(barflag==2)sprintf(temp,"__syncthread();\n");
      

           }

	else {

      

	sprintf(temp,"%s",expr->name);
	}
	strcat(print_string,temp);
if(ind==10)return;
	}
	if(expr->name!=NULL){
	for(std::map<char *,statement *>::iterator it=fnames.begin();it!=fnames.end();it++){
	if(!strcmp(it->first,expr->name)){
	statement *t1=it->second;
	dir_decl *fd1=t1->stdecl->dirrhs;
	}
	}
	}
	if(libf==0)strcat(print_string,"(");
	assign_stmt *t1=expr->arglist;
	while(t1!=NULL&&t1->next){
	t1->rhs->argflag=1;
	t1->rhs->printcode1(t1->rhs,print_string);
	if(libf!=1)strcat(print_string,",/*xx*/");
	if(libf==1){strcat(print_string,"),");libf++;}
	t1=t1->next;
	}
	if(t1!=NULL){
	t1->rhs->argflag=1;
	t1->rhs->printcode1(t1->rhs,print_string);
	}
	if(expr->params!=NULL){
	strcat(print_string,",");
	tree_decl_stmt *td=expr->params;
	while(td->next!=NULL){if(td->lhs){

	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",td->lhs->name);
	strcat(print_string,temp);
	}
	if(td->dirrhs)td->dirrhs->printcode(1);strcat(print_string,",");td=td->next;}
	if(td&& td->lhs){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",td->lhs->name);
	strcat(print_string,temp);
	}
	if(td&& td->dirrhs)td->dirrhs->printcode(1,print_string);
	}
	strcat(print_string,")");
	}
	//this prints lhs first before binary operators
	if (expr->lhs!=NULL &&expr->expr_type!=ARRREF&&expr->expr_type!=COND_OP&&expr->expr_type!=GET_TYPE){if(expr->expr_type!=FUNCALL){
	if(expr->lhs->expr_type==VAR && expr->lhs->lhs && expr->lhs->lhs->libstable!=NULL &&ariflag==1){fprintf(stderr,"\n***binary operation not permitted directly on libdatatype***\n");errflag=1;}
	if(expr->rhs && expr->rhs->expr_type==VAR && expr->rhs->lhs && expr->rhs->lhs->libstable!=NULL&& ariflag==1){fprintf(stderr,"\n***binary operation not permitted directly on libdatatype***\n");errflag=1;}
       if(expr->expr_type==VAR && expr->cpu_name!=NULL){strcat(print_string,expr->cpu_name);fprintf(FP1,"/*CPU_NAME*/");}
else	this->lhs->printcode1(expr->lhs,print_string);
	}
	}
	if(isbinary1to10(type))ariflag=1;
	if(type==AND||type==ADD || type==MUL || type==SUB || type==DIV || type==MOD || type==AND || type==OR || type==XOR|| type==BAND || type==BOR|| type==PTRREF){

if(type==AND&& !(Gkernel))expr->rhs->findgpuvars(expr->rhs);
if(type==AND && !(Gkernel))expr->lhs->findgpuvars(expr->lhs);
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",exptypename[type]);
	strcat(print_string,temp);

	}
	if(type==POSTINC|| type==POSTDEC || type==STRUCTREF || type==LT || type==GT|| type==LE || type==GE|| type==LE || type==EQ || type==NEQ){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",exptypename[type]);
	strcat(print_string,temp);
	}
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
	if(expr->lhs->libdtype==ITERATOR_TYPE&& d1->it==2){
	expr->libdtype=POINT_TYPE;
	expr->parent=d1->parent;

	}
	if(expr->lhs->libdtype==EDGE_TYPE)size=4;
	if(expr->lhs->libdtype==POINT_TYPE)size=8;
	}
	}else if(expr->libdtype!=-1&&expr->libdtype!=-2){
	}
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",expr->name);
	strcat(print_string,temp);
	if(expr->lhs && expr->lhs->libdtype==COLLECTION_TYPE &&Gkernel>0 && Gkernel!=5)strcat(print_string,".D_Vec");
	assign_stmt *y=expr->earr_list;
	while(y!=NULL){
	strcat(print_string,"[");
	if(y->lhs)y->lhs->printcode1(y->lhs,print_string);
	y->rhs->printcode1(y->rhs,print_string);
	strcat(print_string,"]");
//fprintf(FP1,"/*here%d",1249);
	y=y->next;
	}
	aref=0;
	}
	if(type==INITBLOCK){
	strcat(print_string,"{");
	tree_expr *t1=expr->rhs;
	while(t1->prev!=NULL)t1=t1->prev;
	if(t1!=NULL) {
	class designator *t2= t1->desi;
	if(t2!=NULL){

	while(t2!=NULL){
	if(t2->expr!=NULL){
	strcat(print_string,"[");
	t2->expr->printcode1(t2->expr,print_string);
	strcat(print_string,"]");
//fprintf(FP1,"/*here%d",1267);
	}
	if(t2->name!=NULL){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",t2->name);
	strcat(print_string,temp);
	}
	t2=t2->next;
	}
	strcat(print_string,"=");
	}
	if(t1->expr_type==VAR){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	if( ((dir_decl *)(t1->lhs))->dup_name!=NULL && DUPVAR==1)sprintf(temp,"%s",((dir_decl *)(t1->lhs))->dup_name);
	else sprintf(temp,"%s/*xx*/",t1->name);
	strcat(print_string,temp);
	}
	if(t1->nodetype==TREE_INT){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%d",t1->ival);
	strcat(print_string,temp);
	}
 if (expr->nodetype==TREE_BOOL){
if(expr->ival)strcat(print_string, "true");
else strcat(print_string,"false");
return;
}
	t1=t1->next;
	while(t1!=NULL){
	strcat(print_string,",");
	class designator *t2= t1->desi;
	if(t2!=NULL){

	while(t2!=NULL){
	if(t2->expr!=NULL){
	strcat(print_string,"[");
	t2->expr->printcode1(t2->expr,print_string);
	strcat(print_string,"]");
//fprintf(FP1,"/*here %d*/",1304);
	}
	if(t2->name!=NULL){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",t2->name);
	strcat(print_string,temp);


	}
	t2=t2->next;
	}
	strcat(print_string,"=");
	}
	if(t1->expr_type==INITBLOCK)t1->printcode1(t1,print_string);
	if(t1->expr_type==VAR){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%s",t1->name);
	strcat(print_string,temp);
	}
	if(t1->nodetype==TREE_INT){
	char temp[20];
	for(int i=0;i<20;i++)temp[i]='\0';
	sprintf(temp,"%d",t1->ival);
	strcat(print_string,temp);

	}
 if (expr->nodetype==TREE_BOOL){
if(expr->ival)strcat(print_string, "true");
else strcat(print_string,"false");
return;
}
	t1=t1->next;
	}
	strcat(print_string,"}");
	}
	}
	if(expr->rhs!=NULL&& type!=INITBLOCK && type!=COND_OP && type!=GET_TYPE){
	expr->rhs->printcode1(expr->rhs,print_string);
	}
	}

	void tree_expr::printcode(tree_expr *expr){
	int n;
	if(expr==NULL)return;
	if(expr->pflag==100){expr->pflag=-100;return;}
	int type=expr->expr_type;
	if(type==GET_TYPE){
	printf("%s.getType() ", expr->lhs->name);
	dir_decl *d1= expr->rhs->nextv;
	while(d1->nextv!=NULL){if(d1->gpu)printf("<GPU> ");printf("%s,",d1->name);d1=d1->nextv;}
	if(d1!=NULL && d1->gpu)printf("<GPU> ");
	if(d1!=NULL)printf("%s",d1->name);
	d1=expr->rhs->nextv;
	while(d1!=NULL){
	//if(d1->ppts!=NULL)printf("%s has extra properties",d1->name);
	if(d1->ppts!=NULL&& d1->ppts->parent!=NULL){

	fprintf(FP," same as that of %s \n",d1->ppts->parent->name);

	d1->extra_name=(char *)malloc(sizeof(char)*100);
	 sprintf(d1->extra_name,"struct struct_%s ",d1->ppts->parent->name);

	}
	else {




	printf("decicded to give structure property name struct_%s\n ",d1->name);
	d1->extra_name=(char *)malloc(sizeof(char)*100);
	 sprintf(d1->extra_name,"struct struct_%s ",d1->name);
	printf("struct struct_%s { \n",d1->name);
	struct extra_ppts *ep=d1->ppts;


	while(ep!=NULL){
	if(ep->t1->libdatatype!=-1)
	{printf("%s  *%s ;//has to given size of property type\n",ep->t1->name,ep->name);}
	else printf("%s  %s ;\n",ep->t1->name,ep->name);
	ep=ep->next;
	}
	printf("};\n");
	}

	d1=d1->nextv;}

	dir_decl *t1=expr->rhs->nextv;

	while(t1!=NULL){if (t1->ppts!=NULL){
	struct extra_ppts *ep=t1->ppts;
	while(ep!=NULL){
	if(t1->gpu==0){
	if(ep->t1->libdatatype==E_P_TYPE ){
	//printf("((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * %s.nedges) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,t1->name);
	}
	if(ep->t1->libdatatype==P_P_TYPE){
	//printf("((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * %s.npoints) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,t1->name);
	}
	if(ep->t1->libdatatype==G_P_TYPE){
	if(ep->val2!=-1){
	//printf("((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * %d) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,ep->val2);
	}
	else {

	//printf("((%s *)%s.extra)->%s=(%s *)malloc(sizeof(%s) * %s ) ;\n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->t1->name,ep->var2->name);
	}

	}
	}
	else {

	if(ep->t1->libdatatype==E_P_TYPE ){
	//printf("cudaMalloc( (void **)&(((%s *)%s.extra)->%s),sizeof(%s)* %s.nedges); \n",t1->extra_name,t1->name,ep->name,ep->t1->name,t1->name);
	}
	if(ep->t1->libdatatype==P_P_TYPE){
	//printf("cudaMalloc( (void **)&(((%s *)%s.extra)->%s),sizeof(%s)* %s.npoints) ; \n",t1->extra_name,t1->name,ep->name,ep->t1->name,t1->name);
	}
	if(ep->t1->libdatatype==G_P_TYPE){
	if(ep->val2!=-1){
	//printf("cudaMalloc( (void **)&(((%s *)%s.extra)->%s),sizeof(%s)* %d); \n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->val2);
	}
	else {

	//printf("cudaMalloc( (void **)&(((%s *)%s.extra)->%s),sizeof(%s)* %s); \n",t1->extra_name,t1->name,ep->name,ep->t1->name,ep->var2->name);
	}

	}

	}
	ep=ep->next;}
	//printf("}\n");
	}
	 t1=t1->nextv;}
	}
	if(type==CAST_EXPR){
	if(expr->tpname!=NULL){
	fprintf(FP1,"(");
        char a[100];   
	expr->tpname->printcode(a);
	fprintf(FP1,"%s)",a);
	//expr->rhs->print(expr->rhs);
	}
	}
	if(type==TYPE_SIZEOF){
	fprintf(FP1,"sizeof(");
	if(expr->tpname!=NULL){
        char a[100];   
	expr->tpname->printcode(a);
	fprintf(FP1,"%s)",a);
	}
	}

	if(type==TYPE_INIT){
	if(expr->tpname!=NULL){
	fprintf(FP1,"(");
        char a[100];   
	expr->tpname->printcode(a);
	fprintf(FP1,"%s)",a);
	}
	}
	if(type==COND_OP){
	fprintf(FP1,"(");
	expr->lhs->printcode(expr->lhs);
	fprintf(FP1,") ?");
	expr->rhs->lhs->printcode(expr->rhs->lhs);
	fprintf(FP1," : ");
	expr->rhs->rhs->printcode(expr->rhs->rhs);

	}
	if(type==STRUCTREF){
	int xu;
	//if(expr->lhs==NULL)printf("lhs NULL");
	dir_decl *d1= (dir_decl *)(expr->lhs->lhs);
	if(d1->parent!=NULL){expr->parent=d1->parent;}//FEB14
	sref=sref+1;
	//if(d1&& d1->name)printf("XXXX %s   %s\n",d1->name,d1->extra_name);
	//if(d1==NULL)printf("D1 NULL");
	if(d1->libstable!=NULL){
	//printf("LIBDTYPE NOT NULL\n");
	char *name=expr->rhs->name;
	int size=0,flag=0,i=0;
	if(d1->libdtype==COLLECTION_TYPE ){
	if(expr->rhs && expr->rhs->expr_type==FUNCALL){
	for(int i=0;i<6;i++){
	if(!strcmp(expr->rhs->name,libdtypefunctions[4][i])&&(strlen(expr->rhs->name)==strlen(libdtypefunctions[4][i]))){
	printf("%s.",d1->name);
	expr->libdtype=-1; expr->rhs->printcode(expr->rhs);return;}
	}
	}
	}
	if(expr->rhs && d1->libdtype==SET_TYPE ){
	if(expr->rhs->expr_type==FUNCALL){
	for(int i=0;i<4;i++){
	if(!strcmp(expr->rhs->name,libdtypefunctions[3][i])&&(strlen(expr->rhs->name)==strlen(libdtypefunctions[4][i]))){
	//printf("%s.",d1->name);
	expr->libdtype=-1; expr->rhs->printcode(expr->rhs);return;}
	}
	}
	}
	if(d1->libdtype==G_P_TYPE){expr->lhs->printcode(expr->lhs); expr->libdtype=-1; expr->rhs->printcode(expr->rhs);return;}
	if(d1->libdtype==GRAPH_TYPE)size=7;
	if(d1->libdtype==EDGE_TYPE)size=4;
	if(d1->libdtype==POINT_TYPE)size=8;
	for(i=0;i<size;i++){
	if(!strcmp(name,libdtypefields[expr->lhs->lhs->libdtype][i])&&strlen(name)==strlen(libdtypefields[expr->lhs->lhs->libdtype][i])){
	flag=1;
	break;
	}
	}
	if(flag!=1){
	if(expr->lhs->lhs->libdtype==GRAPH_TYPE)size=9;
	if(expr->lhs->lhs->libdtype==EDGE_TYPE)size=9;
	if(expr->lhs->lhs->libdtype==POINT_TYPE)size=9;
	for(i=0;i<size;i++){
	if(!strcmp(name,libdtypefunctions[expr->lhs->lhs->libdtype][i])&&strlen(name)==strlen(libdtypefunctions[expr->lhs->lhs->libdtype][i])){
	flag=2;
	if(expr->lhs->lhs->libdtype==GRAPH_TYPE&&i==1){
	d1->clone=1;//added april 10
	}
	break;
	}
	}
	}
	if(flag==1 /*&& ariflag==1*/){
	//if(i==1||i==3)EXTRAFLAG=1;statement moved inside graph
	if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
	if(i==1||i==3){
EXTRAFLAG=1;

//fprintf(FP1," EF==1 %d ",1530);
}
	if(((i==1)||(i==3)) && expr->rhs->expr_type!=ARRREF &&ariflag==1){
	fprintf(stderr,"ivalid use of field %s \n",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
	else if(expr->rhs->expr_type==ARRREF&&((i==0)||(i==2)|| (i==4)|| (i==5))&&ariflag==1){
	fprintf(stderr,"ivalid use of field %s\n ",libdtypefields[expr->lhs->lhs->libdtype][i]);errflag=1;}
	else if((i==1) /*&& expr->rhs->expr_type==ARRREF*/){expr->libdtype=POINT_TYPE; /*printf("point type\n");*/expr->parent=d1;/*if(expr->rhs->earr_list )printf("xx%s\n",expr->rhs->earr_list->name);if(expr->rhs->earr_list  && expr->rhs->earr_list->lhs->libdtype!=-1)printf("it=%d",expr->earr_list->lhs->it);*/}
	else if((i==3) /*&& expr->rhs->expr_type==ARRREF*/){
	expr->libdtype=EDGE_TYPE;
	expr->parent=d1;
	}
	}
	if(expr->lhs->lhs->libdtype==POINT_TYPE){
	//printf("MATCHED\n");
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
	}
	if(flag==2 ){

	if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
	//printf("here\n");
	if(i==5) {expr->libdtype=EDGE_TYPE;expr->parent=expr->lhs->lhs;}
	else expr->libdtype=-2;

	}
	}
	if(flag==0){
	extra_ppts *ex=NULL;
	if(d1->libdtype==GRAPH_TYPE)ex=d1->ppts;
	else if(d1->parent!=NULL){ex=d1->parent->ppts;/*fprintf(stderr,"parent=%d %s %s ",d1->parent->arg,d1->name,d1->parent->name);*/}
	else if(d1->arg==1)ex=d1->ppts;
	//if(ex==NULL)fprintf(stderr,"NO EXTRA\n");
	while(ex!=NULL&& flag==0){
	//printf("%s %s",expr->rhs->name,ex->name);
	if(!strcmp(ex->name,expr->rhs->name)&& strlen(ex->name)==strlen(expr->rhs->name)){
	if(ex->libdtype!=d1->libdtype){
	//********************************NEED TO ADD CHECK FOR ACCESS OF POINT/EDGE AND EXTRA PROPERTY********************************
	if(d1->libdtype==GRAPH_TYPE)expr->parent=d1;
	else expr->parent=d1->parent;
	if(d1->libdtype==GRAPH_TYPE && ex->libdtype!=G_P_TYPE &&ariflag==1){
	fprintf(stderr,"cannot directly access %s property %s from graph  %s\n",libdtypenames[ex->libdtype] , ex->name,d1->name);
	errflag=1;
	}else {expr->libdtype=ex->t1->libdatatype;flag=1;break;}//mdofied on fe14AN MARCH24 
	}
	if(ex->t1==NULL)printf("T! NULL\n");
	//printf("here %d %d",expr->dtype,ex->t1->datatype);
	if(ex->t1->datatype!=-1&& expr->rhs->expr_type==VAR){expr->libdtype=-1;
	expr->dtype=ex->t1->datatype;
	//printf("here %d",expr->dtype);
	}
	flag=1;
	break;
	}
	//printf("%s",expr->rhs->name);
	ex=ex->next;
	}
	}
	//fprintf(stderr,"\n***%d\n",flag);
	if(flag==0 && d1->arg==0 && !(d1->parent!=NULL && d1->parent->arg==1)&&d1->libdtype>=0){fprintf(stderr,"ivalid field access %s for %s datatype\n",name,libdtypenames[expr->lhs->lhs->libdtype]);errflag=1;}
	//fprintf(stderr,"\n***\n");
	if(flag==0 &&d1->libdtype!=-1){
	//if(flag==0 &&d1->libdtype!=-1){modifeb7
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
	if(EXTRAFLAG==1 && d1->extra_name!=NULL)fprintf(FP1,"((%s *)(%s.extra))->",d1->extra_name,d1->name);
	else if(EXTRAFLAG==1) fprintf(FP1,"%s.",d1->name);
	else if(d1->forit==1)fprintf(FP1,"((%s *)(%s.extra))->",d1->extra_name,d1->name);
	if(d1->gpu==1)printf("OOPS IT A GPU VARIABLE\n");
	}
	else {expr->lhs->printcode(expr->lhs); expr->libdtype=expr->lhs->libdtype;
	//printf("printing lhs first %d \n",expr->lhs->libdtype);
	if(expr->lhs->libdtype==-2){fprintf(stderr,"invalid structure access\n");errflag=1;}}
	if(expr->libdtype!=-1 &&expr->libdtype!=-2  && d1->libstable==NULL){

	char *name=expr->rhs->name;
	//dir_decl *d2=expr->rhs->lhs;
	//fprintf(stderr,"here %s\n",name);
	int size,flag=0,i=0;
	if(expr->libdtype==GRAPH_TYPE)size=7;
	if(expr->libdtype==EDGE_TYPE)size=4;
	if(expr->libdtype==POINT_TYPE)size=8;
	for(i=0;i<size;i++){
	if((!strcmp(name,libdtypefields[expr->libdtype][i]))&&(strlen(name)==strlen(libdtypefields[expr->libdtype][i]))){
	flag=1;
	break;
	}
	}


	if(flag!=1){
	if(expr->libdtype==GRAPH_TYPE)size=6;
	if(expr->libdtype==EDGE_TYPE)size=4;
	if(expr->libdtype==POINT_TYPE)size=3;
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
	if(i==1||i==3){
EXTRAFLAG=1;
//fprintf(FP1," EF==1 %d ",1666);
}
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
	if(d1->libstable!=NULL&&EXTRAFLAG==0){
	expr->lhs->printcode(expr->lhs);//COMMENTED ON MARCH24
	}
	if(expr->lhs->dtype!=-1 && expr->lhs->dtype<=9){fprintf(stderr,"struct ref on noncompound datatype\n");errflag=1;}
	//printf("RHS");

	if(expr->rhs->expr_type!=ARRREF){
	if(expr->lhs->rhs&& expr->lhs->rhs->expr_type!=ARRREF&&EXTRAFLAG==0)fprintf(FP1,".");
	if(expr->lhs->rhs==NULL&&EXTRAFLAG==0)fprintf(FP1,".");
	expr->rhs->printcode(expr->rhs);
	}
	EXTRAFLAG=0;
	if(expr->lhs && expr->lhs->rhs && expr->lhs->rhs->expr_type==ARRREF&& expr->lhs->libdtype>0){
	fprintf(FP1,"[");
	assign_stmt *y=expr->lhs->rhs->earr_list;
	if(y->lhs!=NULL)y->lhs->printcode(y->lhs);
	y->rhs->printcode(y->rhs);
	//if(expr->lhs->rhs->rhs)expr->lhs->rhs->rhs->print(expr->lhs->rhs->rhs);
	fprintf(FP1,"]");
	}
	//printf("RHS");
	sref=sref-1;
	return;
	}
	if(type==PTRREF){
	expr->lhs->printcode(expr->lhs);
	fprintf(FP1,"->");
	fprintf(FP1,"%s ",expr->rhs->name);
	if(expr->lhs->parent!=NULL)expr->parent=expr->lhs->parent;//FEB14
	return;
	}
	if(type==UNARYAND||  type==UNARYMUL || type==UNARYADD || type==UNARYOR || type==UNARYSUB || type==NOT||type==PREINC || type==PREDEC )fprintf(FP1,"%s",exptypename[type]);
	if(expr->expr_type==EXPR){
	fprintf(FP1,"(");

	if(expr->exprlist!=NULL){
	statement *t1=expr->exprlist;
	//t1->print();
	t1->codeGen(FP1);
	}
	fprintf(FP1,")");
	}
	if(expr->expr_type==VAR ){
tree_expr *ttnov=expr;
while(ttnov->lhs && ttnov->lhs->lhs)ttnov=ttnov->lhs;
if(ttnov->lhs && ((dir_decl *)ttnov->lhs)->dup_name)fprintf(FP1,"%s 1850\n", ((dir_decl *)ttnov->lhs)->name); 
	//if(expr->lhs==NULL&&expr->name!=NULL&& sref==0&& aref==0){fprintf(stderr,"undeclared variable %s\n",expr->name);errflag=1;}
	if( ((dir_decl *)(expr->lhs))->dup_name!=NULL)fprintf(FP1,"%sx",((dir_decl *)(expr->lhs))->dup_name);
	if(expr->name!=NULL) fprintf(FP1,"%s",expr->name);
	if(expr->lhs!=NULL && expr->lhs->libdtype>=0&&expr->libdtype<0&&sref==0&&/*ariflag==0&&*/expr->rhs==NULL){expr->libdtype=expr->lhs->libdtype;
	expr->parent=expr->lhs->parent;//FEb14
	}

	if(expr->expr_type==VAR)return;}
	 if (expr->nodetype==TREE_INT){
	fprintf( FP1,"%d", expr->ival);
	return;
	}
 if (expr->nodetype==TREE_BOOL){
if(expr->ival)fprintf(FP1, "true");
else fprintf(FP1,"false");
return;
}
	 if (expr->nodetype==TREE_ENUMCONST){
	fprintf( FP1,"%s", expr->name);
	return;
	}
	 if (expr->nodetype==TREE_FLOAT){
	fprintf(FP1, "%f", expr->fval);
	return;
	}
	if(expr->expr_type==NULL_ARR)return;
	 if (expr->nodetype==TREE_STRING){
	fprintf(FP1, "%s", expr->name);
	return;
	}
	if(expr->expr_type==FUNCALL){
	int unni;
	if(expr->name!=NULL)fprintf(FP1,"%s",expr->name);
	if(expr->name!=NULL){
	//scanf("%d",&unni);
	for(std::map<char *,statement *>::iterator it=fnames.begin();it!=fnames.end();it++){
	//printf("%s %s",it->first,expr->name);
	if(!strcmp(it->first,expr->name)){//printf("\n*******a call to function defined in this file*****\n");
	statement *t1=it->second;
	dir_decl *fd1=t1->stdecl->dirrhs;
	if(fd1->params!=NULL){ 
	tree_decl_stmt *tt=fd1->params;
	assign_stmt *ta=expr->arglist;
	int cnt=0;
	}
	}
	}
	}
	//else printf("NULL\n");
	 //printf("NULL\n");
	fprintf(FP1,"(");
	assign_stmt *t1=expr->arglist;
	while(t1!=NULL&&t1->next){
	t1->printcode(0);
	fprintf(FP1,",");
	t1=t1->next;
	}
	if(t1!=NULL)t1->printcode(0);
	if(expr->params!=NULL){
	fprintf(FP1,",");
	tree_decl_stmt *td=expr->params;
	while(td->next!=NULL){if(td->lhs)fprintf(FP1,"%s ",td->lhs->name);if(td->dirrhs)td->dirrhs->printcode(1);printf(",");td=td->next;}
	if(td&& td->lhs)fprintf(FP1,"%s ",td->lhs->name);
	if(td&& td->dirrhs)td->dirrhs->printcode(1);
	}
	fprintf(FP1,")");
	}
	if (expr->lhs!=NULL &&expr->expr_type!=ARRREF&&expr->expr_type!=COND_OP&&expr->expr_type!=GET_TYPE){if(expr->expr_type!=FUNCALL){
	if(expr->lhs->expr_type==VAR && expr->lhs->lhs && expr->lhs->lhs->libstable!=NULL &&ariflag==1){fprintf(stderr,"\n***binary operation not permitted directly on libdatatype***\n");errflag=1;}
	if(expr->rhs && expr->rhs->expr_type==VAR && expr->rhs->lhs && expr->rhs->lhs->libstable!=NULL&& ariflag==1){fprintf(stderr,"\n***binary operation not permitted directly on libdatatype***\n");errflag=1;}
	this->lhs->printcode(expr->lhs);
	}
	}
	//printf("OK\n");
	if(isbinary1to10(type))ariflag=1;
	if(type==AND||type==ADD || type==MUL || type==SUB || type==DIV || type==MOD || type==AND || type==OR || type==XOR|| type==BAND || type==BOR|| type==PTRREF)fprintf(FP1,"%s",exptypename[type]);
	if(type==POSTINC|| type==POSTDEC || type==STRUCTREF || type==LT || type==GT|| type==LE || type==GE|| type==LE || type==EQ || type==NEQ)fprintf(FP1,"%s",exptypename[type]);

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
	fprintf(FP1,"%s",expr->name);
	assign_stmt *y=expr->lhs->rhs->earr_list;
	while(y!=NULL){
	fprintf(FP1,"[");
	if(y->lhs!=NULL)y->lhs->printcode(y->lhs);
	y->rhs->printcode(y->rhs);
	fprintf(FP1,"]");
//fprintf(FP1,"/*here %d*/",1879);
	y=y->next;
	}
	aref=0;
	}
	if(type==INITBLOCK){
	fprintf(FP1,"{");
	tree_expr *t1=expr->rhs;
	while(t1->prev!=NULL)t1=t1->prev;
	if(t1!=NULL) {
	class designator *t2= t1->desi;
	if(t2!=NULL){

	while(t2!=NULL){
	if(t2->expr!=NULL){
	fprintf(FP1,"[");
	t2->expr->printcode(t2->expr);
	fprintf(FP1,"]");
//fprintf(FP1,"/*here%d",1903);
	}
	if(t2->name!=NULL)fprintf(FP1,".%s",t2->name);
	t2=t2->next;
	}
	fprintf(FP1,"=");
	}
	if(t1->expr_type==VAR)fprintf(FP1,"%s",t1->name);
	if(t1->nodetype==TREE_INT)fprintf(FP1,"%d",t1->ival);
 if (expr->nodetype==TREE_BOOL){
if(expr->ival)fprintf(FP1, "true");
else fprintf(FP1,"false");
return;
}
	t1=t1->next;
	while(t1!=NULL){
	fprintf(FP1,",");
	class designator *t2= t1->desi;
	if(t2!=NULL){

	while(t2!=NULL){
	if(t2->expr!=NULL){
	fprintf(FP1,"[");
	t2->expr->printcode(t2->expr);
	fprintf(FP1,"]");
//fprintf(FP1,"/*here %d*/",1928);
	}
	if(t2->name!=NULL)fprintf(FP1,".%s",t2->name);
	t2=t2->next;
	}
	fprintf(FP1,"=");
	}
	if(t1->expr_type==INITBLOCK)t1->printcode(t1);
	if(t1->expr_type==VAR)fprintf(FP1,"%s",t1->name);
	if(t1->nodetype==TREE_INT)fprintf(FP1,"%d",t1->ival);
 if (expr->nodetype==TREE_BOOL){
if(expr->ival)fprintf(FP1, "true");
else fprintf(FP1,"false");
return;
}
	t1=t1->next;
	}
	fprintf(FP1,"}");
	}
	}
	if(expr->rhs!=NULL&& type!=INITBLOCK && type!=COND_OP && type!=GET_TYPE){
	expr->rhs->printcode(expr->rhs);
	}

	}

	tree_node::tree_node(){
	  nodetype=0; parent=NULL;fval=0.0;ival=0;ptrflag=0;ptrstring=NULL;
	}
	int tree_node::getline() {
		return line;
	    }

	TREE_NODE_TYPE tree_node::getnodetype() {
		return nodetype;
	    }

	tree_node* tree_node::getparent() {
		return parent;
	    
	}
void tree_node::setnodetype(TREE_NODE_TYPE nt) {
		nodetype = nt;
	    }

	  void tree_node::setparent(tree_node* p) {
		parent = p;
	    }

	tree_id::tree_id(){
	name=NULL;next=NULL;stable=NULL;decltype=NULL;dtype=-1;stype=-1;ctype=-1;idrhs=NULL;arr_list=NULL;rhs=NULL;procdef=NULL;ptrfun=0;dirdecl=NULL;}
	char *tree_id::getname(){
	return name;
	}
	void tree_id::setname(char *vname){
	name=vname;
	}
	int  tree_id::find(tree_id *x1,tree_id *x2){
	if(x1==NULL)return 1;
	if(!strcmp(x1->name,x2->name)){
	{fprintf (stderr,"variable with name %s already there\n",x2->name);errflag=1;}
	return -1;
	}
	else return find(x1->next,x2);
	}
	dir_decl::dir_decl(){
        singleflag=0;
	next=NULL;prevv=NULL;brack_flag=0;arr_flag=0;type_qual=NULL;assign=NULL;procd=0;ptrflag=0;sbrack_flag=0;typelist=NULL;params=NULL;open_brack=0;close_brack=0;ptrcnt=0;dup_name=NULL;name=NULL;extra_name=NULL;extra_name1=NULL;extra_fun=NULL;stat=0;rhs=NULL;nextv=NULL;idrhs=NULL;structexpr=NULL;stable=NULL;libstable=NULL;libdtype=-1;gpu=0;parnames=NULL;it=-1;arg=0;parent=NULL;dtype=-1;clone=0;forit=0;extra_name2=NULL;iflag=0;read=0;extra_readfun1=NULL;ordered=0;isparam=false;cpugpu=false;gpugpu=0;update_fun=NULL;}
	//void print();
	int  dir_decl::find(dir_decl *x1,dir_decl *x2){
	if(x1==NULL)return 1;
	if(!strcmp(x1->name,x2->name)){
	printf ("variable with name %s already there\n",x2->name);
	return -1;
	}
	else return find(x1->next,x2);
	}
	designator::designator(){
	expr=NULL;name=NULL;next=NULL;
	}
	tree_expr::tree_expr(){
	lhs=NULL;rhs=NULL;earr_list=NULL;expr_type=-1;prev=NULL;next=NULL;argflag=0;tpname=NULL;exprlibtype=-1;exprlist=NULL;kernel=0;pflag=0;
	ptrflag=0;}
	tree_expr::tree_expr(tree_expr *x){
	this->exprlist=NULL;
	this->rhs=NULL;
	this->lhs=x;
	this->kernel=0;
	}
	tree_expr::tree_expr(dir_decl *id){
	//printf("expected const\n");
	this->exprlist=NULL;
	this->rhs=NULL;
	this->lhs=id;
	this->expr_type=VAR;
	if(id!=NULL)this->dtype=id->dtype;
	if(id!=NULL)this->libdtype=id->libdtype;
	if(id!=NULL){this->lhs->ival=id->ival;
	this->name=id->name;
	this->lhs->nodetype=id->nodetype;
	this->kernel=0;
	}
	}
	tree_typedecl::tree_typedecl(){
	libdatatype=-1;datatype=-1;storagetype=-1;accesstype=-1;name=NULL;list=NULL;next=NULL;vname=NULL;tpdef=0;
	libsize=libelement=-1;t1=NULL;
	d1=d2=d3=NULL;
        exgpu=0;
	}
	statement::statement(){
	sttype=-1;stdecl=NULL;stassign=NULL;next=NULL;prev=NULL;f1=NULL;f2=NULL;f3=NULL;name=NULL;comma=0;stmtno=0;lineno=0;ker=0;feb=0;flist=NULL;barrier=0;expr1=expr2=expr3=expr4=expr5=NULL;cpugpu=false;gpugpu=0;noprint=false;
	}
	iter_stmt::iter_stmt(){
	expr1=NULL;expr2=NULL;expr3=NULL;body=NULL;
	}
