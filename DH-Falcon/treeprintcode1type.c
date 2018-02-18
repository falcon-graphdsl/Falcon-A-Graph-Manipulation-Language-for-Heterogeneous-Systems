extern int INNBRFLAG,utflag;

extern char extra_array[200],*utflagarr[2][3];
extern codegenupdatefunction(dir_decl *d1, tree_expr *expr);
void printfalcvt(enum DATATYPE dtype){
fprintf(FP1,"%s falcvt%d;",dtypenames[dtype],++Temp);
}
printcode1settype(tree_expr * expr,char *print_string){

if(expr->rhs->libdtype==FUNCALL){
if(!strcmp(expr->rhs->name,"find")&& (strlen(expr->rhs->name)==strlen("find"))){
char temp[25];
strcat(print_string,expr->lhs->name);
strcat(print_string,".");
strcat(print_string,"parent");
strcat(print_string,"[");
strcat(print_string,expr->rhs->arglist->rhs->lhs->name);
strcat(print_string,"]");
return;
}
}
}
printcode1collectiontype(tree_expr *expr,char *print_string){

if(expr->rhs->libdtype==FUNCALL){

if(!strcmp(expr->rhs->name,"add")&& (strlen(expr->rhs->name)==strlen("add"))){
if(expr->lhs->gpu==1 ){strcat(print_string,"add_coll_device(");
strcat(print_string,expr->lhs->name);
strcat(print_string,",");
assign_stmt *t1=expr->arglist;
while(t1!=NULL&&t1->next){
t1->rhs->printcode1(t1->rhs,print_string);
strcat(print_string,",");
t1=t1->next;
}
if(t1!=NULL)t1->rhs->printcode1(t1->rhs,print_string);
strcat(print_string,");\n");
return;
}
if(expr->lhs->gpu==0 &&Gkernel==0){

strcat(print_string,expr->lhs->name);
strcat(print_string,".push(/*2*/");
assign_stmt *t1=expr->arglist;
while(t1!=NULL&&t1->next){
t1->rhs->printcode1(t1->rhs,print_string);
strcat(print_string,",");
t1=t1->next;
}
fprintf(FP1,"%s %s ",t1->rhs->name,t1->rhs->extra_name);
if(t1!=NULL)t1->rhs->printcode1(t1->rhs,print_string);
strcat(print_string,");\n");
return;
}
if(expr->lhs->gpu==0 && Gkernel>0){

strcat(print_string,expr->lhs->name);
strcat(print_string,".pushgpu(/*2*/");
assign_stmt *t1=expr->arglist;
while(t1!=NULL&&t1->next){
t1->rhs->printcode1(t1->rhs,print_string);
strcat(print_string,",");
t1=t1->next;
}
fprintf(FP1,"%s %s ",t1->rhs->name,t1->rhs->extra_name);
if(t1!=NULL)t1->rhs->printcode1(t1->rhs,print_string);
strcat(print_string,");\n");
return;
}
}
}
}
printcode1edgetype(tree_expr *expr,char *print_string){
char arr[100];
for(int i=0;i<100;i++)arr[i]='\0';
if(!strcmp(expr->rhs->name,"isdel")){
strcat(print_string,expr->lhs->lhs->parent->name);
strcat(print_string,".edel[");
strcat(print_string, expr->lhs->lhs->extra_name1);
strcat(print_string,"]");
return;
}
if(!strcmp(expr->rhs->name,"weight")){
strcat(print_string,expr->lhs->lhs->parent->name);
sprintf(arr,"edges[%s*%s%s].ipe",utflagarr[utflag][0],expr->lhs->lhs->extra_name1,utflagarr[utflag][2]);
strcat(print_string,arr);
return;
}
if(!strcmp(expr->rhs->name,"src")){
INNBRFLAG=1;
sprintf(arr,"edges[%s*%s].ipe",utflagarr[utflag][0],expr->lhs->lhs->extra_name1);
strcat(print_string,arr);
return;
}
if(!strcmp(expr->rhs->name,"dst")){
sprintf(arr,"edges[%s*%s%s].ipe",utflagarr[utflag][0],expr->lhs->lhs->extra_name1,utflagarr[utflag][1]);
strcat(print_string,arr);
return;
}
else {
char temp[100],temp1[100];
for(int i=0;i<100;i++)temp[i]=temp1[i]='\0';
sprintf(temp,"((%s *)(%s.extra))->%s[%s]",expr->lhs->lhs->parent->extra_name,expr->lhs->lhs->parent->name,expr->rhs->name,expr->lhs->lhs->extra_name1); 
strcat(print_string,temp);

}
}
printcode1pointtype(tree_expr *expr,char *print_string){

if(!strcmp(expr->rhs->name,"isdel")){
strcat(print_string,expr->lhs->lhs->parent->name);
strcat(print_string,".pdel[");
strcat(print_string, expr->lhs->lhs->extra_name1);
strcat(print_string,"]");
}
else if((!strcmp(expr->rhs->name,"x")|| !strcmp(expr->rhs->name,"y"))&&(strlen(expr->rhs->name)==1)){

strcat(print_string,expr->lhs->lhs->parent->name);
strcat(print_string,".points[");
strcat(print_string, expr->lhs->lhs->extra_name1);
strcat(print_string,"].");
expr->rhs->printcode1(expr->rhs,print_string);
}
else {
char temp[100],temp1[100];
for(int i=0;i<100;i++)temp[i]=temp1[i]='\0';
sprintf(temp,"((%s *)(%s.extra))->%s[%s]",expr->lhs->lhs->parent->extra_name,expr->lhs->lhs->parent->name,expr->rhs->name,expr->lhs->lhs->extra_name1); 
strcat(print_string,temp);

}
return;
}
int printcode1addtograph(tree_expr *expr ,char *print_string){
if(!strcmp(expr->rhs->name,"addPoint")) {
char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
sprintf(temp,"falc%saddpoint(%s)",expr->lhs->name,expr->lhs->name);
strcat(print_string,temp);
return 1;
}
if(!strcmp(expr->rhs->name,"addEdge")) {
char temp[200];
for(int i=0;i<200;i++)temp[i]='\0';
sprintf(temp,"falc%saddEdge(%s)",expr->lhs->name,expr->lhs->name);
strcat(print_string,temp);
return 1;
}
if(!strcmp(expr->rhs->name,"delEdge")) {
tree_expr *texp=expr->rhs;
char arr[100];
for(int i=0;i<100;i++)arr[i]='\0';
assign_stmt *ass=texp->arglist;
ass->rhs->printcode1(ass->rhs,arr);
fprintf(FP1,"%s.edel[%s]=true",expr->lhs->name,arr);
return 1;
}
if(!strcmp(expr->rhs->name,"delPoint")) {
tree_expr *texp=expr->rhs;
char arr[100];
for(int i=0;i<100;i++)arr[i]='\0';
assign_stmt *ass=texp->arglist;
ass->rhs->printcode1(ass->rhs,arr);
fprintf(FP1,"%s.pdel[%s]=true",expr->lhs->name,arr);
return 1;
}
return 0;
}
printcode1graphfunction(dir_decl *d1, int off, tree_expr *expr,char *print_string){
fprintf(stderr,"off=%d",off);
switch(off) {
case 1:{//read

	if(expr->lhs->lhs->libdtype==GRAPH_TYPE){
	for(int i=0;i<200;i++)extra_array[i]='\0';
	if(expr->lhs->lhs->extra_fun!=NULL)
        sprintf(extra_array,"\nint host%s=1;\n%s.extra=(%s *)malloc(sizeof(%s));\n ",expr->lhs->lhs->name,expr->lhs->lhs->name,expr->lhs->lhs->extra_name,expr->lhs->lhs->extra_name);
         struct extra_ppts *epp=expr->lhs->lhs->ppts;
         if(epp!=NULL){
           char epparr[100];
          while(epp!=NULL){
           if(epp->t1->libdatatype==G_P_TYPE)
{            for(int i=0;i<100;i++)epparr[i]='\0';
             sprintf(epparr,"((%s *)(%s.extra))->n%s=22;\n",expr->lhs->lhs->extra_name,expr->lhs->lhs->name,epp->name);    
if(expr->lhs->lhs->extra_readfun!=NULL)sprintf(epparr,"%s\n",expr->lhs->lhs->extra_readfun);
         strcat(extra_array,epparr);
}epp=epp->next;
            }
           }
strcat(extra_array,expr->lhs->lhs->extra_fun);

	d1->clone=1;//added april 10
	}
return;
}
case 5: {//getweight


	if(expr->lhs->lhs->libdtype==GRAPH_TYPE){//&& FOREACH_FLAG>0)
        tree_expr *ut2=NULL,*ut3=NULL;
       char *utemp=NULL;
       if(expr->rhs && expr->rhs->arglist && expr->rhs->arglist->rhs)ut2=expr->rhs->arglist->rhs; 
       if(expr->rhs && expr->rhs->arglist && expr->rhs->arglist->next && expr->rhs->arglist->next->rhs)ut3=expr->rhs->arglist->next->rhs; 
        if(ut2!=NULL&&(utemp=strstr(ut2->lhs->extra_name1,"ut"))!=NULL)	strcat(print_string,"ut");
        if(utemp!=NULL){
          int tempval=atoi(utemp+2);
              tempval++;
          char itemp[20];
            sprintf(itemp,"%d",tempval);
          strcat(print_string,itemp);
}
      if(ut3->lhs->extra_name==NULL)fprintf(stderr,"EXTRA_NAME NULL\n");
        if(ut3!=NULL&&(utemp=strstr(ut3->lhs->extra_name1,"ut"))!=NULL)	strcat(print_string,"ut");
        if(utemp!=NULL){
          int tempval=atoi(utemp+2);
              tempval++;
          char itemp[20];
            sprintf(itemp,"%d",tempval);
          strcat(print_string,itemp);
}
	return;
	}
}


case 11: {//getedge


	if(expr->lhs->lhs->libdtype==GRAPH_TYPE/*&& FOREACH_FLAG>0*/){
        tree_expr *ut2=NULL,*ut3=NULL;
       char *utemp=NULL;
       if(expr->rhs && expr->rhs->arglist && expr->rhs->arglist->rhs)ut2=expr->rhs->arglist->rhs; 
       if(expr->rhs && expr->rhs->arglist && expr->rhs->arglist->next && expr->rhs->arglist->next->rhs)ut3=expr->rhs->arglist->next->rhs; 
        if(ut2!=NULL&&(utemp=strstr(ut2->lhs->extra_name1,"ut"))!=NULL)	strcat(print_string,"ut ");
        if(utemp!=NULL){
          int tempval=atoi(utemp+2);
              tempval--;
          char itemp[20];
            sprintf(itemp,"%d/%s",tempval,utflagarr[utflag][0]);
          strcat(print_string,itemp);
}

        if(ut3!=NULL&&(utemp=strstr(ut3->lhs->extra_name1,"ut"))!=NULL)	strcat(print_string,"ut");
        if(utemp!=NULL){
          int tempval=atoi(utemp+2);
              tempval--;
          char itemp[20];
            sprintf(itemp,"%d/%s",tempval,utflagarr[utflag][0]);
          strcat(print_string,itemp);
}
	return;
	}
return;

}
case 17:{

           //codegenupdatefunction(d1,expr);
return;


}
default: 
{
}

}

}
printcode1pointfunction(dir_decl *d1, int off, tree_expr *expr,char *print_string){




}
printcode1edgefunction(dir_decl *d1, int off, tree_expr *expr,char *print_string){
}
printcode1setfunction(dir_decl *d1, int off, tree_expr *expr,char *print_string){
}
printcode1collectionfunction(dir_decl *d1, int off, tree_expr *expr,char *print_string){
}
