#include "externs.h"
int parse_command_line(int argc,char *args[]){
bool input_file=false;
for(int i=0;i<argc;i++){
char *str=args[i];
if(!strcmp(args[i],"-ptype"))union_type=atoi(args[i+1]);
if(!strcmp(args[i],"-mtype"))morph_size=atoi(args[i+1]);
if(!strcmp(args[i],"-target")){
if(!strcmp(args[i+1],"GPU"))TARGET=0;
if(!strcmp(args[i+1],"CPU"))TARGET=1;
if(!strcmp(args[i+1],"GPUCPU"))TARGET=2;
}
if(!strcmp(args[i],"-numgpus"))NUM_GPUS=atoi(args[i+1]);
if(strstr(args[i],".txt")){
char *xx=strstr(args[i],".txt");
fprintf(stderr,"%s\n",xx);
fprintf(stderr,"%c %c %c \n",xx[0],xx[1],xx[2]);
if((xx[0]=='.')&&(xx[1]=='t')&&(xx[2]=='x')&&(xx[3]=='t')&&xx[4]=='\0')
input_file=true;
}
}
if(!input_file){
fprintf(stderr,"input file missing %d\n",input_file);
return -1;
}
fprintf(stderr,"TARGET=%d, NUM_GPUS=%d\n", TARGET,NUM_GPUS);
return 1;
}

addto_globvars_in_fun(dir_decl *x){
if(x!=NULL && x->stable!=NULL && x->stable==GLOBAL_TABLE.sym){
struct globvar_in_fun *globvars_temp=NULL;
if(globvars_fun==NULL){
//fprintf(FP1,"//GLOBAL VARIABLE %s \n",x->name);
globvars_fun=(struct globvar_in_fun *)malloc(sizeof(struct globvar_in_fun));
globvars_temp=globvars_fun;
}
else {
globvars_temp=globvars_fun;
while(globvars_temp->next!=NULL)globvars_temp=globvars_temp->next;
globvars_temp->next=(struct globvar_in_fun *)malloc(sizeof(struct globvar_in_fun));
globvars_temp=globvars_temp->next;
}
globvars_temp->d1=x;
globvars_temp->next=NULL;
}
}
